/*
 * sht31.h
 *
 *  Created on: Mar 13, 2018
 *      Author: felix
 */
#include <stdbool.h>
#include <stdio.h>
#include "main.h"

#ifndef SHT31_H_
#define SHT31_H_

//#define SHT31_ADDR				0x45
#define SHT31_ADDR				0x44
#define SHT31_MEAS_HIGHREP 		0x2400
#define SHT31_MEAS_MEDREP 		0x240B
#define SHT31_MEAS_LOWREP 		0x2416
#define SHT31_SOFTRESET			0x30A2
#define SHT31_READ_STATUSREG	0xF32D
#define SHT31_CLEAR_STATUSREG	0x3041
#define SHT31_HEATER_ENABLE 	0x306D
#define SHT31_HEATER_DISABLE 	0x3066

#define SHT31_WAKEUP_TIME	20
#define SHT31_INITWAIT_TIME	10

typedef struct SHT31_data
{
  volatile float temperature;
  volatile float humidity;
  uint8_t bits[6];
  uint16_t statusReg;
} SHT31_data;


class SHT31
{
public:
	SHT31(I2C_HandleTypeDef* hi2c);

	void StartMeasurement();
	bool ProcessMeasurement();

	float GetTemperature();
	float GetHumidity();
private:
	void TurnON();
	void TurnOFF();
	
	I2C_HandleTypeDef* hi2c;
	void Init();
	void ChangeMSB(uint16_t* cmd);
	void ReadStatusReg(SHT31_data *sht31);
	uint8_t CheckCRC(uint8_t *data, int len);
	void ClearStatusReg(uint16_t clearStatusRegCmd);
	void Reset(uint16_t softResetCmd);
	void ChangeHeaterStatus(uint16_t heaterCmd);
	uint8_t ReadTempHum(SHT31_data *out);

	static const uint16_t Timeout;
	volatile uint32_t startTime;
	SHT31_data data;
	bool initialized;
};

#endif /* SHT31_H_ */
