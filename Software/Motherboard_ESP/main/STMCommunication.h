#ifndef __STMCommunication_H_
#define __STMCommunication_H_

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#define GPIO_MOSI   18
#define GPIO_MISO   5
#define GPIO_SCLK   19
#define GPIO_REA    17

#define READY_ON()  {gpio_set_level(GPIO_REA, 1);}
#define READY_OFF() {gpio_set_level(GPIO_REA, 0);}

void ConfigureSPISlave();
void Communicate();

void ProcessStateMachine();
void ProcessReceiveStateMachine();
void SendNTP();
void Receive(uint8_t count, char* currentPtr);
void ReceiveBlock();
void Transmission(uint8_t len, char* recv, char* send);

typedef enum {
	INITIAL = 0,
	WAITCOUNTOFSETS,
	WAITBYTECOUNT,
	WAITBLOCKRECEIVE,
	WAITNTPREQUEST,
	WAITALLSENT,
	FINISHED = 0xFFFF
} ESP_SPI_ReceiveState_t;

typedef struct {
	uint8_t* data;
	uint16_t len;
} DataLen_t;

extern volatile ESP_SPI_ReceiveState_t receiveState;
extern volatile bool postTransmit;

extern volatile uint8_t countBlocks;
extern volatile uint16_t countBytes;
extern volatile uint8_t remainingBlocks;
extern volatile uint16_t remainingBytes;
extern volatile uint8_t currentBlockSize;
extern uint8_t* currentDataSet;
extern DataLen_t* blocks;
extern volatile bool received;

typedef enum {
	MQTTServerURL = 1,
	MQTTTopic = 2,
	MQTTUsername = 3,
	MQTTPassword = 4,
	MQTTIssuerCertificate = 5,
	NTPServerURL = 6,
	WLANSSID = 7,
	WLANPassword = 8,
	SensorData = 9,
	MQTTClientID = 10,
	WLAN_DHCP_Ready = 100,
	TurnOff = 101,
	Timestamp = 102,
	Configuration = 103,
	SensorDataACK = 104,
	Unknown = 0xFFFF
} PacketIdentifier_t;


#endif
