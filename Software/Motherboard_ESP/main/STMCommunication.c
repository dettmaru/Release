#include "STMCommunication.h" 
#include "mqtt_publish.h" // Function to publish a message to MQTT broker
#include "wifi.h" // Functions to connect to a WiFi/WLAN network
#include "sntp_time.h" // Functions for retrieving the current time
#include "driver/spi_slave.h" // Driver for the SPI in slave mode
#include "esp_log.h" // ESP-IDF logging meachnism
#include "soc/spi_reg.h" // Direct access to SPI peripheral registers

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_heap_caps.h"
#include "esp_wifi.h"
#include "esp_sleep.h"
#include "crc32.h"

#define SPI_HOST VSPI_HOST

const char *SPI_TAG = "SPI_Receive";

char* CONF_NTPServer = NULL;

int n;
esp_err_t ret;
spi_slave_transaction_t t;
char sendbuf[129] = "";
char recvbuf[129] = "";

volatile bool postSetup = false;
volatile bool postTransmit = false;

volatile ESP_SPI_ReceiveState_t receiveState = INITIAL;
volatile uint8_t countBlocks = 0;
volatile uint16_t countBytes = 0;
volatile uint8_t remainingBlocks = 0;
volatile uint16_t remainingBytes = 0;
volatile uint8_t currentBlockSize = 0;
uint8_t* currentDataSet = NULL;
DataLen_t* blocks = NULL;
volatile bool received = false;



bool End = false;
DataLen_t* receiveBlocks = NULL;
uint8_t receiveBlocksCount = 0;
DataLen_t* sendBlocks = NULL;
uint8_t sendBlockCount = 0;

/**
 * @brief Clear the list of sending packets
 */
void ClearSendingPackets()
{
	for (uint8_t i = 0; i < sendBlockCount; i++)
	{
		free(sendBlocks[i].data);
	}
	free(sendBlocks);
	sendBlockCount = 0;
	sendBlocks = NULL;
}

/**
 * @brief Adds a packet to the list of packets which should be send to the STM
 * 
 * @remark The pointer is copied into the list of packets, which should be send,
 * 		   after the packets are send the pointer and the data in it which be 
 *         freed.
 */
void AddSendingPacket(DataLen_t* packet)
{
	DataLen_t* tmp = sendBlocks;
	sendBlocks = (DataLen_t*)calloc(sendBlockCount + 1, sizeof(DataLen_t));
	if (tmp != NULL)
	{
		memcpy(sendBlocks, tmp, sendBlockCount * sizeof(DataLen_t)); // Copy all old packets
	}
	sendBlocks[sendBlockCount] = *packet; // Copy new packet into the list
	if (tmp != NULL)
	{
		free(tmp); 
	}

	sendBlockCount++;
}

void ReceivePacketCount(uint8_t* ptrrpc)
{
	if (receiveBlocks != NULL)
	{
		// Free the allocated memory
		for (int i = 0; i < receiveBlocksCount; i++)
		{
			free(receiveBlocks[i].data);
		}
		free(receiveBlocks);
		receiveBlocks = NULL;
		receiveBlocksCount = 0;
	}
	Receive(1, (char*)ptrrpc);
}

void ReceiveByteCount(uint8_t crpc, uint16_t* ptrrbc, uint8_t rpc)
{
	if (crpc == 0)
	{
		receiveBlocks = (DataLen_t*)calloc(rpc, sizeof(DataLen_t));
		receiveBlocksCount = rpc;
	}
	Receive(2, (char*)ptrrbc);
}

uint8_t ReceiveBytes(uint8_t crpc, uint16_t crbc, uint16_t rbc)
{
	if (crbc == 0)
	{
		receiveBlocks[crpc - 1].data = (uint8_t*)calloc(rbc, sizeof(DataLen_t));
		receiveBlocks[crpc - 1].len = rbc;
	}
	uint8_t lrbc = rbc - crbc > 128 ? 128 :  rbc -crbc;
	Receive(lrbc, (char*)(receiveBlocks[crpc - 1].data + crbc));
	return lrbc;
}

uint8_t SendPacketCount()
{
	Transmission(1, recvbuf, (char*)&sendBlockCount);
	ESP_LOGI("State", "Transmitted Packet Count: %d", sendBlockCount);
	return sendBlockCount;
}

uint16_t SendByteCount(uint8_t cspc)
{
	Transmission(2, recvbuf, (char*)(&sendBlocks[cspc].len));
	ESP_LOGI("State", "Transmitted ByteCout: %d", sendBlocks[cspc].len);
	return sendBlocks[cspc].len;
}

uint8_t SendBytes(uint8_t cspc, uint16_t csbc)
{
	uint8_t lsbc = sendBlocks[cspc - 1].len - csbc > 128 ? 128 : sendBlocks[cspc - 1].len - csbc;
	Transmission(lsbc, recvbuf, (char*)(sendBlocks[cspc - 1].data + csbc));
	return lsbc;
}


PacketIdentifier_t GetIdentifier(uint8_t* dataPtr)
{
	return (PacketIdentifier_t)(dataPtr[0]<<8|dataPtr[1]);
}

void SaveConfiguration(char** configurationDest, DataLen_t* packet)
{
	if (*configurationDest != NULL)
	{
		free(*configurationDest);
	}
	*configurationDest = calloc(packet->len - 2 + 1, sizeof(uint8_t));
	for (int i = 0; i < packet->len - 2; i++)
	{
		(*configurationDest)[i] = packet->data[2 + i];
	}
	return;
}

void Processing()
{
	uint8_t ACK_packets[receiveBlocksCount];
	uint16_t ACK_counter = 0;
	for (uint16_t i = 0; i < receiveBlocksCount; i++)
	{
		if (receiveBlocks[i].len < 2)
		{
			// Following code can not be executed!!!
			// => go to next block, this one is invalid!
			continue; 
		}
		PacketIdentifier_t identifier = GetIdentifier(receiveBlocks[i].data);
		ESP_LOGI("State", "PacketIdentifier: %d", identifier);
		switch (identifier)
		{
			case MQTTServerURL:
				SaveConfiguration(&CONF_MQTTServerUrl, receiveBlocks + i);
				break;
			case MQTTTopic:
				SaveConfiguration(&CONF_MQTTTopic, receiveBlocks + i);
				break;
			case MQTTUsername:
				SaveConfiguration(&CONF_MQTTUsername, receiveBlocks + i);
				break;
			case MQTTPassword:
				SaveConfiguration(&CONF_MQTTPassword, receiveBlocks + i);
				break;
			case MQTTIssuerCertificate:
				SaveConfiguration(&CONF_MQTTIssuerCertificate, receiveBlocks + i);
				break;
			case MQTTClientID:
				SaveConfiguration(&CONF_MQTTClientID, receiveBlocks + i);
				break;
			case WLANSSID:
				SaveConfiguration(&CONF_WLANSSID, receiveBlocks + i);
				break;
			case WLANPassword:
				SaveConfiguration(&CONF_WLANPassword, receiveBlocks + i);
				break;
			case SensorData:
				MQTT_Connect();
				int ret = MQTT_Publish(
					receiveBlocks[i].data + 2,
					receiveBlocks[i].len - 2);
				ACK_packets[ACK_counter] = ret >= 0 ? 0 : 255;
				ACK_counter++;
				break;
			case NTPServerURL:
				MQTT_Disconnect(); // TODO this shouldn't be here, because the connection will then not be closed every time.
				SaveConfiguration(&CONF_NTPServer, receiveBlocks + i);
				bool synced = false;
				time_t now = GetTime(CONF_NTPServer, &synced);
				DataLen_t packet;
				packet.len = 2 + (1 + 8 + 4);
				packet.data = (uint8_t*)calloc(2 + (1 + 8 + 4), sizeof(uint8_t));
				packet.data[0] = ((uint16_t)Timestamp) >> 8;
				packet.data[1] = ((uint16_t)Timestamp) & 0xFF;
				packet.data[2] = synced ? 0x3B : 0x85;
				for (int i = 7; i >= 0; i--)
				{
					packet.data[3+7-i] = (now >> i*8) & 0xFF;
					ESP_LOGI("NTP", "%d : %d", 7-i, packet.data[2+7-i]);
				}
				CCRC32_Initialize();
				uint32_t crc = CCRC32_FullCRC(packet.data + 2, 8 + (1+3));
				crc ^= 0xFFFFFFFF;
				packet.data[11] = (crc >> 24) & 0xFF;
				packet.data[12] = (crc >> 16) & 0xFF;
				packet.data[13] = (crc >>  8) & 0xFF;
				packet.data[14] = (crc >>  0) & 0xFF;

				AddSendingPacket(&packet);
				break;
			case TurnOff:
				ESP_LOGI("State", "Received TurnOff");
				esp_wifi_stop();
				esp_sleep_enable_gpio_wakeup();
				esp_light_sleep_start();
				End = true;
				break;
			default:
				break;
		}
	}
	initialise_wifi();

	if (ACK_counter > 0)
	{
		// An acknowledgement of packets is needed, because some packets have 
		// been processed, and the transmission might have failed, 
		// give this information to the STM therefore the following 
		// message contains a byte for every packet which should have been send!
		DataLen_t ACKpacket;
		ACKpacket.len = 2 + ACK_counter;
		ACKpacket.data = (uint8_t*)calloc(ACKpacket.len, sizeof(uint8_t));
		ACKpacket.data[0] = ((uint16_t)SensorDataACK) >> 8;
		ACKpacket.data[1] = ((uint16_t)SensorDataACK) & 0xFF;
		for (uint16_t i = 0; i < ACK_counter; i++)
		{
			ACKpacket.data[2+i] = ACK_packets[i];
		}
		AddSendingPacket(&ACKpacket);
	}
}

typedef enum CommunicationState {
	INIT,
	WAITREADY0,
	WAITREADY1,
	WAITREADY2,
	WAITREADY3,
	WAITREADY4,
	WAITREADY5,
	WAITREADY6,
	FIN
} CommunicationState_t;
volatile CommunicationState_t communicationState = INIT;

uint8_t rpc = 0;
uint8_t crpc = 0;
uint16_t rbc = 0;
uint16_t crbc = 0;
uint8_t spc = 0;
uint8_t cspc = 0;
uint16_t sbc = 0;
uint16_t csbc = 0;
bool Received = false;

void ProcessStateMachine()
{
	Received = received;

	CommunicationState_t newState = INIT;
	switch (communicationState)
	{
		case INIT:
			newState = WAITREADY0;
			break;
		case WAITREADY0:
			newState = WAITREADY1;
			break;
		case WAITREADY1:
			if (Received)
			{
				if (rpc == crpc)
				{
					newState = WAITREADY4;
				}
				else // rpc<>crpc
				{
					newState = WAITREADY2;
				}
			}
			break;
		case WAITREADY2:
			if (Received)
			{
				if (rbc == crbc)
				{
					if (rpc == crpc)
					{
						newState = WAITREADY4;
					}
					else // rpc<>crpc
					{
						newState = WAITREADY2;
					}
				}
				else // rbc<>crbc
				{
					newState = WAITREADY3;
				}
			}
			break;
		case WAITREADY3:
			if (Received)
			{
				if (rbc == crbc)
				{
					if (rpc == crpc)
					{
						newState = WAITREADY4;
					}
					else // rpc<>crpc
					{
						newState = WAITREADY2;
					}
				}
				else // rbc<>crbc
				{
					newState = WAITREADY3;
				}
			}
			break;
		case WAITREADY4:
			if (Received)
			{
				if (spc == cspc)
				{
					newState = WAITREADY1;
				}
				else // spc<>cspc 
				{
					if (End)
					{
						newState = FIN;
					}
					else // !End
					{
						newState = WAITREADY5;
					}
				}
			}
			break;
		case WAITREADY5:
			if (Received)
			{
				if (sbc == csbc)
				{
					if (spc == cspc)
					{
						if (End)
						{
							newState = FIN;
						}
						else // !End
						{
							newState = WAITREADY1;
						}
					}
					else // spc<>cspc
					{
						newState = WAITREADY5;
					}
				}
				else // sbc<>cspc
				{
					newState = WAITREADY6;
				}
			}
			break;
		case WAITREADY6:
			if (Received)
			{
				if (sbc == csbc)
				{
					if (spc == cspc)
					{
						if (End)
						{
							newState = FIN;
						}
						else // !End
						{
							newState = WAITREADY1;
						}
					}
					else // spc<>cspc
					{
						newState = WAITREADY5;
					}
				}
				else // sbc<>cspc
				{
					newState = WAITREADY6;
				}
			}
			break;
		case FIN:
			break;
	}

	switch (newState)
	{
		case WAITREADY0:
			break;
		case WAITREADY1:
			ESP_LOGD("State", "WaitReady1 - Receive Packet Count");
			ClearSendingPackets();
			ReceivePacketCount(&rpc);
			ESP_LOGD("State", "Byte Count: %d", rpc);
			crpc = 0;
			break;
		case WAITREADY2:
			ESP_LOGD("State", "WaitReady2 - Receive Byte Count");
			ReceiveByteCount(crpc, &rbc, rpc);
			ESP_LOGD("State", "Byte Count: %d", rbc);
			crbc = 0;
			crpc += 1;
			break;
		case WAITREADY3:
			ESP_LOGD("State", "Packets to go %d", rpc - crpc);
			ESP_LOGD("State", "WaitReady3 - Receive Bytes");
			crbc += ReceiveBytes(crpc, crbc, rbc); // += lrbc
			break;
		case WAITREADY4:
			ESP_LOGI("State", "Begin received data processing!");
			Processing();
			ESP_LOGI("State", "Finished received data processing!");

			ESP_LOGD("State", "WaitReady4 - Send Packet Count");
			spc = SendPacketCount();
			cspc = 0;
			break;
		case WAITREADY5:
			ESP_LOGD("State", "WaitReady5 - Send Byte Count");
			sbc = SendByteCount(cspc);
			csbc = 0;
			cspc += 1;
			break;	
		case WAITREADY6:
			ESP_LOGD("State", "WaitReady6 - Send Bytes");
			csbc += SendBytes(cspc, csbc); // += lsbc
			break;
		case FIN:
			ESP_LOGD("State", "FIN");
			// TODO turn off
			break;
		case INIT:
			// No new state -> nothing to do
			break;
	}
	if (Received && newState != INIT)
	{
		Received = false;
	}

	communicationState = newState;
}

void spiPostSetupCb(spi_slave_transaction_t* t)
{
    postSetup = true;
}

void spiPostTransmit(spi_slave_transaction_t* t)
{
    postTransmit = true;
}

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    READY_OFF();
    for (int i = 0; i< 10000; i++)
    {
    	asm volatile("nop");
    }
}

extern time_t now;

void SendNTP()
{
	memset(recvbuf, '\0', 8);
	for (int i = 7; i >= 0; i--)
	{
		sendbuf[7-i] = (now >> i*8) & 0xFF;
		ESP_LOGI("NTP", "%d : %d", 7-i, sendbuf[7-i]);
	}
	Transmission(8, recvbuf, sendbuf);
}

void Receive(uint8_t count, char* currentPtr)
{
	uint8_t* sendbuf = heap_caps_malloc(count, MALLOC_CAP_DMA);
	for (int i = 0; i < count; i++)
	{
		sendbuf[i] = 0;
	}

	/* Creating here a temporary buffer only for the transmission solves 
	 * sporadic bugs in the data transmission 
	 */
	char* buffer2 = heap_caps_malloc(count, MALLOC_CAP_DMA);
	//ESP_LOGI("State", "Count before: %d", count);
	Transmission(count, buffer2, (char*)sendbuf);
	//ESP_LOGI("State", "Count after: %d", count);
	//ESP_LOGI("State", "Count currentPtr: %p", currentPtr);
	//ESP_LOGI("State", "Count buffer2: %p", buffer2);
	memcpy(currentPtr, buffer2, count);
	free(buffer2);
	free(sendbuf);

	/* For debug purposes output the received values */
	ESP_LOGD("Data", "Bytes: ");
	if (LOG_LOCAL_LEVEL >= ESP_LOG_DEBUG)
	{
		for (int i = 0; i < count; i++)
		{
			printf("%.2x ", currentPtr[i]);
		}
		printf("\n");
	}
}

void Transmission(uint8_t len, char* recv, char* send)
{
	t.length = len * 8;
	t.tx_buffer = send;
	t.rx_buffer = recv;

	/* For debug purposes output the received values */
	ESP_LOGD("Data", "Bytes to Transmitt: ");
	if (LOG_LOCAL_LEVEL >= ESP_LOG_DEBUG)
	{
		for (int i = 0; i < len; i++)
		{
			printf("%.2x ", send[i]);
		}
		printf("\n");
	}

	gpio_set_level(12, 1);
	ets_delay_us(10);
	gpio_set_level(12, 0);

	//This call enables the SPI slave interface to send/receive to the sendbuf and recvbuf. The transaction is
	//initialized by the SPI master, however, so it will not actually happen until the master starts a hardware transaction
	//by pulling CS low and pulsing the clock etc. In this specific example, we use the handshake line, pulled up by the
	//.post_setup_cb callback that is called as soon as a transaction is ready, to let the master know it is free to transfer
	//data.
	
	//ESP_LOGD("SPI", "STATUS: %8x", (REG_READ(SPI_DMA_STATUS_REG(3))));

	spi_slave_queue_trans(SPI_HOST, &t, portMAX_DELAY);
	
	while (REG_READ(SPI_DMA_STATUS_REG(3)) != 3)  // Wait for queued transation
	{
		//ESP_LOGI("SPI", "STATUS: %8x", (REG_READ(SPI_DMA_STATUS_REG(3))));
		//vTaskDelay(1000 / portTICK_RATE_MS);
	}
	READY_ON();

	spi_slave_transaction_t *ret_trans;
	spi_slave_get_trans_result(SPI_HOST, &ret_trans, portMAX_DELAY);
	//ret=spi_slave_transmit(HSPI_HOST, &t, portMAX_DELAY);

	//spi_slave_transmit does not return until the master has done a transmission, so by here we have sent our data and
	//received data from the master. Print it.
	//printf("Received: %s\n", recvbuf);
	//n++;

	//Every second received data is wrong, why???
	//spi_slave_transmit(SPI_HOST, &t, portMAX_DELAY);

    received = true;
	ESP_LOGD("Data", "Bytes Transmitted");
}

void ConfigureSPISlave()
{
    //Configuration for the SPI Bus
    spi_bus_config_t busConfig = {
        .mosi_io_num = GPIO_MOSI,
        .miso_io_num = GPIO_MISO,
        .sclk_io_num = GPIO_SCLK,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = 0,
		.flags = 0
    };

    //Configuration for the SPI slave interface
    spi_slave_interface_config_t slaveConfig = {
        .mode=0,
        .spics_io_num=12,
        .queue_size=3,
        .flags=0,
        .post_setup_cb=spiPostSetupCb,
        .post_trans_cb=spiPostTransmit
    };

    //Configuration for the handshake line
    gpio_config_t ioConfig={
        .intr_type=GPIO_INTR_DISABLE,
        .mode=GPIO_MODE_OUTPUT,
        .pin_bit_mask=(1<<GPIO_REA)
    };

    //Configure REA line as output
    //this line is also used as CS of the internal
    //SPI peripheral component
    gpio_config(&ioConfig);


    ioConfig.intr_type=GPIO_INTR_DISABLE;
    ioConfig.mode=GPIO_MODE_INPUT;
    ioConfig.pull_down_en=1;
    ioConfig.pin_bit_mask=(1<<12);

    gpio_config(&ioConfig);

    gpio_set_level(GPIO_REA, 0);

    //Enable pull-ups on SPI lines so we don't detect rogue
    //pulses when the STM-Controller is in Standby/Off
    gpio_set_pull_mode(GPIO_MOSI, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_MISO, GPIO_PULLUP_ONLY);


	// Enable wakeup for SCK line 
	gpio_wakeup_enable(GPIO_SCLK, GPIO_INTR_HIGH_LEVEL);


    /*gpio_config_t io_conf;
    //interrupt of rising edge
    io_conf.intr_type = GPIO_PIN_INTR_ANYEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = GPIO_SCLK;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);*/

    gpio_set_intr_type(GPIO_SCLK, GPIO_INTR_POSEDGE);
    //install gpio isr service
    gpio_install_isr_service(0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_SCLK, gpio_isr_handler, (void*) GPIO_SCLK);


    //Initialize SPI slave interface
    ret = spi_slave_initialize(SPI_HOST, &busConfig, &slaveConfig, 1);
    assert (ret == ESP_OK);

    memset(recvbuf, 0, 33);
    memset(&t, 0, sizeof(t));
}
