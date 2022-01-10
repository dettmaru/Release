/*
 * Cellular.cpp
 *
 *  Created on: May 22, 2019
 *      Author: Joschka Wirges, Daniel Lohmann
 */
#include "Cellular.h"
#include "main.h"
#include "a2235.h"
#include "m95.h"
#include "bc95g.h"
#include "linkedlist.h"
#include "mqttsn.h"
#include "Energy.h"
#include "usart.h"
#include "StandbyCache.h"
#include <string.h>
#include "stm32l4xx_hal.h"
#include "Logging.h"
#include "Configuration.h"

BC95G bc95g;

/**
 * @brief Initialize NBIoT module
 */
void Cellular::InitNBIoT()
{
	uint8_t BC95G_RxBuffer = 0;

	Energy::RequestHighPowerMode();
	/* configure required UART peripheral  */
	MX_USART3_UART_Init();
	HAL_UART_Receive_DMA(&huart3, &BC95G_RxBuffer, 1);

	/* LTC high power mode*/
	Energy::RequestHighPowerMode();
	HAL_Delay(100); // set to 100 instead of 50, may be cause of AT+CFUN=1 problem???

	// bc95g.Reset(); may be cause of AT+CFUN=1 problem???

	/* initialise and configure the module */
	bc95g.Init(&huart3);

	/* connect to operator */
	if (bc95g.Connect())
	{
		bc95g.AT_Command("AT+CPSMS?", 1, 1000);
		display(head);
		// Returns something like this:
		//+CPSMS:1,,,01000001,00000101
		
		/* wait for PSM to become active */
		while (!(bc95g.PSM_active))
		{
			bc95g.ProcessMessageQueue();
		}
		LOGIS("NB-IoT", "NB-IoT started successfully");
	}
	else
	{
		/* Connection attempt failed */
		/* Deinitialise and power off the BC95G */
		bc95g.DeInit();
	}

	/* LTC low power mode */
	Energy::FreeRequestHighPowerMode();

	/* turn of UART, because it is not need */
	HAL_UART_DeInit(&huart3);

	/* free malloc of linked list head and all nodes */
	dispose(head);
}

/**
 * @brief Send sensor data packets via NBIoT
 */
void Cellular::SendSensorDataNBIoT(std::vector<SensorDataSendingPacket>* packets)
{
	uint8_t BC95G_RxBuffer = 0;

	/* configure required UART peripheral  */
	MX_USART3_UART_Init();
	HAL_UART_Receive_DMA(&huart3, &BC95G_RxBuffer, 1);

	/* LTC high power mode*/
	Energy::RequestHighPowerMode();
	HAL_Delay(100);

	/* connect to operator */
	if (!bc95g.Init(&huart3))
	{
		LOGE("NB-IoT", "NB-IoT init failed");
	}
	else
	{
		/* connect to operator */
		if (bc95g.Connect())
		{
			LOGIS("NB-IoT", "NB-IoT started successfully");
			LOGI("NB-IoT", "Start publishing via MQTTSN QoS-1");
			if (bc95g.OpenUDPSocket())
			{
				/* NBIoT connection established, start sending messages */
				for (auto& sendingPacket : *packets)
				{
					SensorDataSet* dataSet = StandbyCache::GetSensorDataSet(sendingPacket.CacheIndex);
					uint16_t length = 0;
					uint8_t* message = dataSet->GetMQTTSensorDataContent(&length);

					/*MQTTSN publish */
					uint8_t* destIP = Configuration::GetString(StringOption::NBIoT_DestinationIP, nullptr);
					char destPort[20];
					snprintf(destPort, sizeof(destPort), "%d", 
						Configuration::GetNumber(NumericOption::NBIoT_DestinationPort));

					int retry = 0;
					while (retry < 3)
					{
						if (bc95g.MQTTSN_PublishQoSMinus1(
								(char*)destIP, destPort,
								0x0001, 0x0000, (char*)message, length)) {
							LOGI("NB-IoT", "Published via MQTTSN QoS-1");

							// Mark that the message was send successfully
							sendingPacket.NB_IoT = true;

							// No further retry is needed
							retry = 10;
							break;
						} else {
							LOGE("NB-IoT", "Error while publishing via MQTTSN Qos-1");
							retry++;
						}
					}

					delete[] destIP;
					delete message;
				}
			}
			bc95g.CloseUDPSocket();
		}
	}
	
	/* LTC low power mode */
	Energy::FreeRequestHighPowerMode();

	/* Deinitialize UART */
	HAL_UART_DeInit(&huart3);

	/* Deinitialize the BC95G to shut it down completely */
	bc95g.DeInit();

	/* free malloc of linked list head and all nodes */
	dispose(head);
}

/**
 * @brief Send sensor data packets via GSM
 */
void Cellular::SendSensorDataGSM(std::vector<SensorDataSendingPacket>* packets)
{

}


