/*
 * ESP.cpp
 *
 *  Created on: May 23, 2018
 *      Author: Daniel Lohmann
 */
#include "main.h"
#include "spi.h"
#include "crc.h"
#include "ESP.h"
#include "Packet.h"
#include "Energy.h"
#include "StandbyCache.h"
#include "StandbyRegister.h"
#include "SensorDataSendingPacket.h"
#include "TimeHelper.h"
#include "Motherboard.h"
#include "SerialNumberToString.h"
#include <stdio.h>
#include <vector>
#include <string.h>

#define ESP_HSPI hspi2

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{

}

void HAL_SPI_TxHalfCpltCallback(SPI_HandleTypeDef* hspi)
{

}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi)
{

}

void HAL_SPI_RxHalfCpltCallback(SPI_HandleTypeDef* hspi)
{

}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef* hspi)
{

}

void HAL_SPI_TxRxHalfCpltCallback(SPI_HandleTypeDef* hspi)
{

}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef* hspi)
{
	// HAL_SPI_GetState(hspi);
	// HAL_SPI_GetError(hspi);
}

/**
 * @brief Triggered ready signal of the ESP
 */
volatile bool ESP::Ready = false;

/**
 * @brief Turns the ESP on 
 */
void ESP::TurnON()
{
	Energy::RequestHighPowerMode();

	HAL_Delay(100); //TODO This could maybe deleted

	HAL_GPIO_WritePin(ESP_ON_GPIO_Port, ESP_ON_Pin, GPIO_PIN_RESET);
}

/**
 * @brief Turns the ESP off 
 */
void ESP::TurnOFF()
{
	//HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ESP_ON_GPIO_Port, ESP_ON_Pin, GPIO_PIN_SET);

	Energy::FreeRequestHighPowerMode();
}

bool ESPReady()
{
	return HAL_GPIO_ReadPin(SE_REA_GPIO_Port, SE_REA_Pin) == GPIO_PIN_SET;
}

enum class STM_ESP_SendingState_t {
	CONFIGURATION,
	DATA,
	TIME,
	SHUTDOWN,
	WAITSHUTDOWN,
	FINISHED,
};

enum class STM_ESP_CommunicationState_t{
	INITIAL = 0,
	WAITREADY0,
	WAITREADY1,
	WAITREADY2,
	WAITREADY3,
	WAITREADY4,
	WAITREADY5,
	WAITREADY6,
	FINISHED = 0xFFFF
};

/**
 * @brief Timeout for ESP processing, if the communicationState doesn't change,
 * 		  so no communication between STM and ESP is happening this timeout,
 * 		  will be used!
 */
const uint32_t ESP::RESPONSE_TIMEOUT = 20000;

/**
 * @brief Last time at which the
 */
volatile uint32_t communicationStateChangeTime = 0;

/**
 * @brief Currently loaded sensor data sets
 */
std::list<const SensorDataSet*>* sensorData;


/**
 * @brief Sending state of the STM<->ESP communication. This could also be
 *        described as the phase of communication
 */
volatile STM_ESP_SendingState_t sendingState = STM_ESP_SendingState_t::CONFIGURATION;

/**
 * @brief Communication state of the STM<->ESP communication. This is the
 *        state of an inner communication in one communication phase
 */
volatile STM_ESP_CommunicationState_t communicationState = STM_ESP_CommunicationState_t::INITIAL;


/**
 * @brief SensorDataSets which should be send with status information
 */
std::vector<SensorDataSendingPacket>* packets;
/**
 * @brief Iterator for packets, which handles the transmission to the ESP
 */
std::vector<SensorDataSendingPacket>::iterator packetsIter;
/**
 * @brief Iterator for packets, which handles the acknowledgment of package sending from the ESP
 */
std::vector<SensorDataSendingPacket>::iterator packetsACKIter;

/**
 * @brief Current list of packets which should be send to the ESP
 */
std::vector<Packet*> sendingPackets{};
/**
 * @brief Current list of packets which have been received from the ESP
 */
std::vector<Packet*> receivePackets{};


/* The following variables are used for the STM<->ESP communication.
 * Look at the state diagram of this communication for a more detailed
 * description of these variables
 */
bool End = false;
bool More = true;
uint8_t  spc = 0;
uint8_t  cspc = 0;
uint16_t sbc = 0;
uint16_t csbc = 0;
uint8_t  rpc = 0;
uint8_t  crpc = 0;
uint16_t rbc = 0;
uint16_t crbc = 0;
/**
 * @brief Buffer for DMA-SPI communications
 */
uint8_t buffer[128];

/**
 * @brief Sends the count of packets
 *
 * @return uint8_t Returns the count of packets (sbc)
 */
uint8_t SendPacketCount()
{
	for (auto receivedPacket : receivePackets)
	{
		if (receivedPacket->GetIdentifier() == PacketIdentifier_t::Timestamp)
		{
			if (receivedPacket->GetDataLength() == 1 + 8 + 4)
			{
				uint8_t* data = receivedPacket->GetDataPointer();
				uint32_t receivedCRC = (data[9] << 24) | (data[10] << 16) | (data[11] << 8) | (data[12] << 0);
				data[9] = data[10] = data[11] = data[12] = 0;
				MX_CRC_Init();
				uint32_t crcCalc[3] = {0};
				crcCalc[0] = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | (data[3] << 0);
				crcCalc[1] = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | (data[7] << 0);
				crcCalc[2] = (data[8] << 24) | (data[9] << 16) | (data[10] << 8) | (data[11] << 0);
				uint32_t calculatedCRC = HAL_CRC_Calculate(&hcrc, crcCalc, 3);

				// Check that the time is available and that the message
				// has not changed in the transmission
				if (data[0] == 0x3B && receivedCRC == calculatedCRC)
				{
					uint8_t* NTPdata = data + 1;
					volatile time_t now = 0;
					for (int i = 7; i >= 0; i--)
					{
						now |= NTPdata[7-i] << i*8;
					}
					Time::Set(now & 0xFFFFFFFF);
				}
			}
		}
		else if (receivedPacket->GetIdentifier() == PacketIdentifier_t::SensorDataACK)
		{
			uint16_t length = receivedPacket->GetDataLength();
			uint8_t* data = receivedPacket->GetDataPointer();
			for (int i = 0; i < length; i++)
			{
				packetsACKIter->WLAN = data[i] == 0;
				packetsACKIter++;
			}
		}
	}

	for (auto packet : sendingPackets)
	{
		delete packet;
	}
	sendingPackets.clear();

	int spc = 0;
	switch (sendingState)
	{
		case STM_ESP_SendingState_t::CONFIGURATION:
			char clientID[100];
			SerialNumber_t number;
			char* serialNumberString;
			number = Motherboard::GetSerialNumber();
			serialNumberString = GetSerialNumberString(number);
			sprintf(clientID, "SensorBox-ESP-V2-%s", serialNumberString);
			free(serialNumberString);

			sendingPackets.push_back(new Packet(PacketIdentifier_t::WLANSSID, StringOption::WLAN_SSID));
			sendingPackets.push_back(new Packet(PacketIdentifier_t::WLANPassword, StringOption::WLAN_Password));
			sendingPackets.push_back(new Packet(PacketIdentifier_t::MQTTServerURL, StringOption::MQTT_Broker));
			sendingPackets.push_back(new Packet(PacketIdentifier_t::MQTTTopic, StringOption::MQTT_Topic));
			sendingPackets.push_back(new Packet(PacketIdentifier_t::MQTTUsername, StringOption::MQTT_Username));
			sendingPackets.push_back(new Packet(PacketIdentifier_t::MQTTPassword, StringOption::MQTT_Password));
			sendingPackets.push_back(new Packet(PacketIdentifier_t::MQTTIssuerCertificate, StringOption::CA_Certificate));
			sendingPackets.push_back(new Packet(PacketIdentifier_t::MQTTClientId, (uint8_t*)clientID, (uint16_t)strlen(clientID)));
			
			sendingState = STM_ESP_SendingState_t::DATA;
			break;
		case STM_ESP_SendingState_t::DATA:
			for (auto dataSet : *::sensorData)
			{
				delete dataSet;
			}

			::sensorData->clear();
			for (int i = 0; i < 20; i++)
			{
				if (packetsIter == packets->end())
				{
					break;
				}

				SensorDataSet* dataSet = StandbyCache::GetSensorDataSet(packetsIter->CacheIndex);
				::sensorData->push_back(dataSet);
				packetsIter++;
			}

			for (auto dataSet : *::sensorData)
			{
				sendingPackets.push_back(new Packet(dataSet));
			}
			if (packets->end() == packetsIter)
			{
				if (Configuration::GetBool(BoolOption::SyncTimeNTP) && 
					(StandbyRegister::IsIntervalActiveAndReset(Intervals::NTPTimeSync) ||
					 !Time::IsInitialized()))
				{
					sendingState = STM_ESP_SendingState_t::TIME;
				}
				else
				{
					sendingState = STM_ESP_SendingState_t::SHUTDOWN;
				}
			}
			break;
		case STM_ESP_SendingState_t::TIME:
			sendingPackets.push_back(new Packet(PacketIdentifier_t::NTPServerURL, StringOption::NTPTimeServer));

			sendingState = STM_ESP_SendingState_t::SHUTDOWN;
			break;
		case STM_ESP_SendingState_t::SHUTDOWN:
			sendingPackets.push_back(new Packet(PacketIdentifier_t::TurnOff));

			sendingState = STM_ESP_SendingState_t::WAITSHUTDOWN;
			break;
		case STM_ESP_SendingState_t::WAITSHUTDOWN:
			
			break;
		case STM_ESP_SendingState_t::FINISHED:

			break;
	}
	spc = sendingPackets.size();
	buffer[0] = spc;
	HAL_SPI_Transmit_DMA(&ESP_HSPI, buffer, 1);
	return spc;
}

/**
 * @brief Sends the count of bytes in the current packet
 *
 * @param cspc Count of already send packets
 * @return uint16_t Number of bytes in the current packet (sbc)
 */
uint16_t SendByteCount(uint8_t cspc)
{
	if (cspc > 0)
	{
		sendingPackets[cspc - 1]->Free();
	}
	Packet* packet = sendingPackets[cspc];
	packet->Load();

	int sbc = packet->GetByteCount();
	buffer[0] = sbc & 0xFF;
	buffer[1] = sbc >> 8;
	HAL_SPI_Transmit_DMA(&ESP_HSPI, buffer, 2);
	return sbc;
}

/**
 * @brief Sends a bunch of data bytes
 *
 * @param cspc Count of already send packtes
 * @param csbc Count of already send bytes
 * @return uint8_t Amount of bytes in the curren transaction (lsbc)
 */
uint8_t SendBytes(uint8_t cspc, uint16_t csbc)
{
	uint8_t lsbc = 0;
	uint8_t* data = sendingPackets[cspc - 1]->GetPointer(csbc, &lsbc);
	HAL_SPI_Transmit_DMA(&ESP_HSPI, data, lsbc);
	return lsbc;
}

/**
 * @brief Receives the count of packets which will be received
 *
 * @param ptrrpc Pointer to the destination of the packet count
 */
void ReceivePacketCount(uint8_t* ptrrpc)
{
	for (auto packet : receivePackets)
	{
		delete packet;
	}
	receivePackets.clear();

	HAL_SPI_Receive_DMA(&ESP_HSPI, ptrrpc, 1);
	return;
}

/**
 * @brief Receives the count of bytes in the current packet
 *
 * @param crpc Count of received packets
 * @param ptrrbc Pointer to the destination of the byte count
 */
void ReceiveByteCount(uint8_t crpc, uint16_t* ptrrbc)
{
	HAL_SPI_Receive_DMA(&ESP_HSPI, (uint8_t*)ptrrbc, 2);
}

/**
 * @brief Receives a bunch of data bytes for a packet
 *
 * @param crpc Count of received/receiving packets
 * @param crbc Count of received bytes
 * @return uint8_t Count of bytes in the transaction executed by this method
 */
uint8_t ReceiveBytes(uint8_t crpc, uint16_t crbc)
{
	Packet* packet = nullptr;
	if (crbc == 0)
	{
		packet = new Packet(rbc);
		receivePackets.push_back(packet);
	}
	else
	{
		packet = receivePackets[crpc - 1];
	}

	uint8_t lrbc = 0;
	uint8_t* dataDest = packet->GetPointer(crbc, &lrbc);
	HAL_SPI_Receive_DMA(&ESP_HSPI, dataDest, lrbc);
	return lrbc;
}

void ESP::ProcessStateMachine()
{
	STM_ESP_CommunicationState_t old_communicationState = communicationState;
	switch (communicationState)
	{
		case STM_ESP_CommunicationState_t::INITIAL:
			buffer[0] = 0;
			HAL_SPI_Transmit_DMA(&ESP_HSPI, buffer, 1);
			ESP::TurnON();
			HAL_Delay(2000); // TODO improve this delay
			Ready = ESPReady();

			communicationState = STM_ESP_CommunicationState_t::WAITREADY0;
			break;
		case STM_ESP_CommunicationState_t::WAITREADY0:
			if (Ready)
			{
				Ready = false;
				spc = SendPacketCount();
				cspc = 0;
				sbc = 0;
				csbc = 0;
				communicationState = STM_ESP_CommunicationState_t::WAITREADY1;
			}
			break;
		case STM_ESP_CommunicationState_t::WAITREADY1:
			if (Ready)
			{
				Ready = false;
				if (cspc == spc)
				{
					communicationState = STM_ESP_CommunicationState_t::WAITREADY3;
				}
				else // cspc<>spc
				{
					sbc = SendByteCount(cspc);
					cspc++;
					communicationState = STM_ESP_CommunicationState_t::WAITREADY2;
				}
			}
			break;
		case STM_ESP_CommunicationState_t::WAITREADY2:
			if (Ready && sbc != csbc)
			{
				Ready = false;
				uint8_t lsbc = SendBytes(cspc, csbc);
				csbc += lsbc;
				communicationState = STM_ESP_CommunicationState_t::WAITREADY3;
			}
			else if (sbc == csbc)
			{
				communicationState = STM_ESP_CommunicationState_t::WAITREADY3;
			}
			break;
		case STM_ESP_CommunicationState_t::WAITREADY3:
			if (Ready)
			{
				Ready = false;
				if (sbc == csbc)
				{
					if (spc == cspc)
					{
						ReceivePacketCount(&rpc);
						crpc = 0;
						rbc = 0;
						crbc = 0;
						communicationState = STM_ESP_CommunicationState_t::WAITREADY4;
					}
					else // spc<>csoc
					{
						sbc = SendByteCount(cspc);
						csbc = 0;
						cspc++;
						communicationState = STM_ESP_CommunicationState_t::WAITREADY2;
					}
				}
				else // sbc<>csbc
				{
					uint8_t lsbc = SendBytes(cspc, csbc);
					csbc += lsbc;
					communicationState = STM_ESP_CommunicationState_t::WAITREADY3;
				}
			}
			break;
		case STM_ESP_CommunicationState_t::WAITREADY4:
			if (Ready && rpc != crpc)
			{
				Ready = false;
				ReceiveByteCount(crpc, &rbc);
				crpc++;
				communicationState = STM_ESP_CommunicationState_t::WAITREADY5;
			}
			else if (rpc == crpc)
			{
				communicationState = STM_ESP_CommunicationState_t::WAITREADY6;
			}
			break;
		case STM_ESP_CommunicationState_t::WAITREADY5:
			if (Ready && rbc != crbc)
			{
				Ready = false;
				uint8_t lrbc = ReceiveBytes(crpc, crbc);
				crbc += lrbc;
				communicationState = STM_ESP_CommunicationState_t::WAITREADY6;
			}
			else if (rbc == crbc)
			{
				communicationState = STM_ESP_CommunicationState_t::WAITREADY6;
			}
			break;
		case STM_ESP_CommunicationState_t::WAITREADY6:
			if (Ready)
			{
				Ready = false;
				if (rbc == crbc)
				{
					if (rpc == crpc)
					{
						// TODO packet processing required
						if (End)
						{
							communicationState = STM_ESP_CommunicationState_t::FINISHED;
						}
						else // !end
						{
							spc = SendPacketCount();
							cspc = 0;
							sbc = 0;
							csbc = 0;
							communicationState = STM_ESP_CommunicationState_t::WAITREADY1;
						}
					}
					else // rpc<>crpc
					{
						ReceiveByteCount(crpc, &rbc);
						crpc++;
						crbc = 0;
						communicationState = STM_ESP_CommunicationState_t::WAITREADY5;
					}
				}
				else // rbc<>crbc
				{
					uint8_t lrbc = ReceiveBytes(crpc, crbc);
					crbc += lrbc;
					communicationState = STM_ESP_CommunicationState_t::WAITREADY6;
				}
			}
			break;
		case STM_ESP_CommunicationState_t::FINISHED:
			break;
	}
	if (old_communicationState != communicationState)
	{
		communicationStateChangeTime = HAL_GetTick();
	}
	return;
}

/**
 * @brief Executes the process of sending sensor data message with the ESP
 */
void ESP::SendSensorData(std::vector<SensorDataSendingPacket>* packets)
{
	MX_SPI2_Init();

	::packets = packets;
	::packetsIter = ::packets->begin();
	::packetsACKIter = ::packets->begin();
	::sensorData = new std::list<const SensorDataSet*>();

	sendingState = STM_ESP_SendingState_t::CONFIGURATION;
	communicationStateChangeTime = HAL_GetTick();
	communicationState = STM_ESP_CommunicationState_t::INITIAL;

	uint32_t countTime = 0;

	while(sendingState != STM_ESP_SendingState_t::FINISHED)
	{
		ProcessStateMachine();

		// Attention: Independent watch dog (IWDG) Reset!
		HAL_IWDG_Refresh(&hiwdg);

		// Toggles LED D7 every 500 sec.
		if(HAL_GetTick() - countTime > 500)
		{
			ToggleLed(LED2_GPIO_Port, LED2_Pin);
			countTime = HAL_GetTick();
		}

		if (sendingState == STM_ESP_SendingState_t::WAITSHUTDOWN &&
			!ESPReady())
		{
			sendingState = STM_ESP_SendingState_t::FINISHED;
			ESP::TurnOFF();
		}

		if (HAL_GetTick() - communicationStateChangeTime > RESPONSE_TIMEOUT)
		{
			// Timeout condition fulfilled, break execution
			break;
		}
	}

	HAL_SPI_DeInit(&ESP_HSPI);

	return;
}



