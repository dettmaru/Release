/*
 * sdi12.h
 *
 *  Created on: Jan 7, 2019
 *      Author: Daniel Lohmann
 */

#ifndef MEASURE_SDI12_H_
#define MEASURE_SDI12_H_

#include "main.h"
#include "stdint.h"
#include "stdlib.h"
#include "gpio.h"
#include "usart.h"
#include "string.h"
#include "tim.h"
#include "sdi12_Sensor.h"
#include <vector>

#define SDI12UART hlpuart1
#define SDI12TIMER htim17
#define SDI12TIMERINIT() {MX_TIM17_Init();}

#define SDI12_CMD_ACK_ACTIVE "%s!" // Sensor active?
#define SDI12_CMD_ID_SERN "%sI!" // Identification and serial number
#define SDI12_CMD_CHANGE_ADDR "%sA%s!" // Change address
#define SDI12_CMD_ADDRESS_QUERY "?!" // Read the address of only one connected sensor
#define SDI12_CMD_START_MEASURE "%sM!" // Start measurement
#define SDI12_CMD_ADDITIONAL_MEASURE1 "%sM1!" // Start additional measurement 1
#define SDI12_CMD_ADDITIONAL_MEASURE2 "%sM2!" // Start additional measurement 2
#define SDI12_CMD_ADDITIONAL_MEASURE3 "%sM3!" // Start additional measurement 3
#define SDI12_CMD_ADDITIONAL_MEASURE4 "%sM4!" // Start additional measurement 4
#define SDI12_CMD_ADDITIONAL_MEASURE5 "%sM5!" // Start additional measurement 5
#define SDI12_CMD_ADDITIONAL_MEASURE6 "%sM6!" // Start additional measurement 6
#define SDI12_CMD_ADDITIONAL_MEASURE7 "%sM7!" // Start additional measurement 7
#define SDI12_CMD_ADDITIONAL_MEASURE8 "%sM8!" // Start additional measurement 8
#define SDI12_CMD_ADDITIONAL_MEASURE9 "%sM9!" // Start additional measurement 9
#define SDI12_CMD_START_MEASURE_CRC "%sMC!" // Start measurement with CRC
#define SDI12_CMD_ADDITIONAL_MEASURE1_CRC "%sMC1!" // Start additional measurement 1 with CRC
#define SDI12_CMD_ADDITIONAL_MEASURE2_CRC "%sMC2!" // Start additional measurement 2 with CRC
#define SDI12_CMD_ADDITIONAL_MEASURE3_CRC "%sMC3!" // Start additional measurement 3 with CRC
#define SDI12_CMD_ADDITIONAL_MEASURE4_CRC "%sMC4!" // Start additional measurement 4 with CRC
#define SDI12_CMD_ADDITIONAL_MEASURE5_CRC "%sMC5!" // Start additional measurement 5 with CRC
#define SDI12_CMD_ADDITIONAL_MEASURE6_CRC "%sMC6!" // Start additional measurement 6 with CRC
#define SDI12_CMD_ADDITIONAL_MEASURE7_CRC "%sMC7!" // Start additional measurement 7 with CRC
#define SDI12_CMD_ADDITIONAL_MEASURE8_CRC "%sMC8!" // Start additional measurement 8 with CRC
#define SDI12_CMD_ADDITIONAL_MEASURE9_CRC "%sMC9!" // Start additional measurement 9 with CRC
#define SDI12_CMD_GET_DATA0 "%sD0!" // Get data slot 0
#define SDI12_CMD_GET_DATA1 "%sD1!" // Get data slot 1
#define SDI12_CMD_GET_DATA2 "%sD2!" // Get data slot 2
#define SDI12_CMD_GET_DATA3 "%sD3!" // Get data slot 3
#define SDI12_CMD_GET_DATA4 "%sD4!" // Get data slot 4
#define SDI12_CMD_GET_DATA5 "%sD5!" // Get data slot 5
#define SDI12_CMD_GET_DATA6 "%sD6!" // Get data slot 6
#define SDI12_CMD_GET_DATA7 "%sD7!" // Get data slot 7
#define SDI12_CMD_GET_DATA8 "%sD8!" // Get data slot 8
#define SDI12_CMD_GET_DATA9 "%sD9!" // Get data slot 9
#define SDI12_CMD_START_VERIFY "%sV1" // Start verification
#define SDI12_CMD_START_CONCURRENT_MEASURE "%sC!" // Start concurrent measurement
#define SDI12_CMD_ADDITIONAL_CONCURRENT_MEASURE1 "%sC1!" // Start additional concurrent measurement 1
#define SDI12_CMD_ADDITIONAL_CONCURRENT_MEASURE2 "%sC2!" // Start additional concurrent measurement 2
#define SDI12_CMD_ADDITIONAL_CONCURRENT_MEASURE3 "%sC3!" // Start additional concurrent measurement 3
#define SDI12_CMD_ADDITIONAL_CONCURRENT_MEASURE4 "%sC4!" // Start additional concurrent measurement 4
#define SDI12_CMD_ADDITIONAL_CONCURRENT_MEASURE5 "%sC5!" // Start additional concurrent measurement 5
#define SDI12_CMD_ADDITIONAL_CONCURRENT_MEASURE6 "%sC6!" // Start additional concurrent measurement 6
#define SDI12_CMD_ADDITIONAL_CONCURRENT_MEASURE7 "%sC7!" // Start additional concurrent measurement 7
#define SDI12_CMD_ADDITIONAL_CONCURRENT_MEASURE8 "%sC8!" // Start additional concurrent measurement 8
#define SDI12_CMD_ADDITIONAL_CONCURRENT_MEASURE9 "%sC9!" // Start additional concurrent measurement 9
#define SDI12_CMD_START_CONCURRENT_MEASURE_CRC "%sCC!" // Start concurrent measurement with CRC
#define SDI12_CMD_ADDITIONAL_CONCURRENT_MEASURE1_CRC "%sCC1!" // Start additional concurrent measurement 1 with CRC
#define SDI12_CMD_ADDITIONAL_CONCURRENT_MEASURE2_CRC "%sCC2!" // Start additional concurrent measurement 2 with CRC
#define SDI12_CMD_ADDITIONAL_CONCURRENT_MEASURE3_CRC "%sCC3!" // Start additional concurrent measurement 3 with CRC
#define SDI12_CMD_ADDITIONAL_CONCURRENT_MEASURE4_CRC "%sCC4!" // Start additional concurrent measurement 4 with CRC
#define SDI12_CMD_ADDITIONAL_CONCURRENT_MEASURE5_CRC "%sCC5!" // Start additional concurrent measurement 5 with CRC
#define SDI12_CMD_ADDITIONAL_CONCURRENT_MEASURE6_CRC "%sCC6!" // Start additional concurrent measurement 6 with CRC
#define SDI12_CMD_ADDITIONAL_CONCURRENT_MEASURE7_CRC "%sCC7!" // Start additional concurrent measurement 7 with CRC
#define SDI12_CMD_ADDITIONAL_CONCURRENT_MEASURE8_CRC "%sCC8!" // Start additional concurrent measurement 8 with CRC
#define SDI12_CMD_ADDITIONAL_CONCURRENT_MEASURE9_CRC "%sCC9!" // Start additional concurrent measurement 9 with CRC
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_0 "%sR0!" // Get data continuos slot 0
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_1 "%sR1!" // Get data continuos slot 1
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_2 "%sR2!" // Get data continuos slot 2
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_3 "%sR3!" // Get data continuos slot 3
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_4 "%sR4!" // Get data continuos slot 4
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_5 "%sR5!" // Get data continuos slot 5
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_6 "%sR6!" // Get data continuos slot 6
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_7 "%sR7!" // Get data continuos slot 7
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_8 "%sR8!" // Get data continuos slot 8
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_9 "%sR9!" // Get data continuos slot 9
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_0_CRC "%sRC0!" // Get data continuos slot 0 CRC
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_1_CRC "%sRC1!" // Get data continuos slot 1 CRC
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_2_CRC "%sRC2!" // Get data continuos slot 2 CRC
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_3_CRC "%sRC3!" // Get data continuos slot 3 CRC
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_4_CRC "%sRC4!" // Get data continuos slot 4 CRC
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_5_CRC "%sRC5!" // Get data continuos slot 5 CRC
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_6_CRC "%sRC6!" // Get data continuos slot 6 CRC
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_7_CRC "%sRC7!" // Get data continuos slot 7 CRC
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_8_CRC "%sRC8!" // Get data continuos slot 8 CRC
#define SDI12_CMD_CONTINUOS_MEASUREMENTS_9_CRC "%sRC9!" // Get data continuos slot 9 CRC


#define CURRENT_SDI12_TIME (SDI12TIMER.Instance->CNT)
#define CURRENT_SDI12_RX_STATE() HAL_GPIO_ReadPin(SDI12_RX_GPIO_Port, SDI12_RX_Pin)


void SDI12_LineProcessing();

class SDI12
{
public:
	void StartMeasurement();
	bool ProcessMeasurement();
	std::vector<float> SimpleMeasurement();
	void Init();
	void DeInit();
	uint32_t Command(uint8_t address, uint8_t* dataIn);
	uint32_t Command(uint8_t address, uint8_t* dataIn, uint8_t* dataOut, uint8_t dataOutLength);
	std::vector<SDI12_Sensor*> sensors;
private:
	void TurnON();
	void TurnOFF();
	void StartReceiving();
	void StopReceiving();
	void LineProcessing();
	void SaveResponse(uint8_t* message);

	bool ExtractWaitTimeAndAmountOfValues(uint8_t* message, uint16_t* waitTime, uint8_t* amount);
	std::vector<float> ExtractValues(uint8_t* message);


	volatile uint32_t lastChange;
	volatile GPIO_PinState lastState;
	int32_t currentBit;
	bool error;
	bool end;
	uint8_t receivedData[100];

// Allow the interrupt to call into the class
friend void SDI12_LineProcessing();
};



#endif /* MEASURE_SDI12_H_ */
