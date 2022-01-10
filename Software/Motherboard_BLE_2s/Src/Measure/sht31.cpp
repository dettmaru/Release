/*
 * sht31.c
 *
 *  Created on: Mar 14, 2018
 *      Author: Felix Schütte
 */
#include "sht31.h"
#include "main.h"
#include "i2c.h"
#include "HardwareSemaphores.h"

const uint16_t SHT31::Timeout = 1000;

SHT31::SHT31(I2C_HandleTypeDef* hi2c)
{
	this->initialized = false;
	this->hi2c = hi2c;
}

void SHT31::StartMeasurement()
{
    startTime = HAL_GetTick();
    TurnON();
}

bool SHT31::ProcessMeasurement()
{
	// Wait for ready for the measurement
    if (HAL_GetTick() - startTime > SHT31_INITWAIT_TIME + SHT31_WAKEUP_TIME)
	{
    	int ret = 0;
        for (int i = 0; i < 10; i++)
        {
        	ret = ReadTempHum(&data);
        	if (ret == 1)
        		break;
        }

		TurnOFF();
		return false;
	}
    else if (HAL_GetTick() - startTime > SHT31_WAKEUP_TIME)
    {
        Init();
        return true;
    }
    else // Not ready for measurement
    {
        return true;
    }
}

float SHT31::GetTemperature()
{
	return data.temperature;
}

float SHT31::GetHumidity()
{
	return data.humidity;
}

void SHT31::TurnON()
{
	// Request the required hardware resources (power)
	airSensors.Request();
}

void SHT31::TurnOFF()
{
	// Free the request for power on the air Sensors cable
	airSensors.Free();
	initialized = false;
}

void SHT31::Init(){
	if (!initialized)
	{
		Reset(SHT31_SOFTRESET);
		HAL_Delay(10);
		initialized = true;
	}
}

uint8_t SHT31::ReadTempHum(SHT31_data *out)
{
	initialized = false;
	Init();
	HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(hi2c, (SHT31_ADDR<<1), SHT31_MEAS_HIGHREP, I2C_MEMADD_SIZE_16BIT, out->bits, 6, Timeout);
	if (ret != HAL_OK) return 0;

	uint8_t crc = CheckCRC(&out->bits[0], 2);
	// CRC is incorrect, Transmission has to be restarted
	if (crc != out->bits[2]) return 0;

	crc = CheckCRC(&out->bits[3], 2);
	// CRC is incorrect, Transmission has to be restarted
	if (crc != out->bits[5]) return 0;

	//Calculate temperature
 	uint16_t temp = (out->bits[0] <<8 ) | (out->bits[1]);
	out->temperature = -45 + (temp *175.0)/(65534);
	uint16_t hum = (out->bits[3] <<8 ) | (out->bits[4]);
	out->humidity = (hum *100.0)/(65534);

	//printf("Temperature: %.2f \r\n", out->temperature);
	//printf("Humidity: %.2f \r\n", out->humidity);

	return 1;
}

void SHT31::Reset(uint16_t softResetCmd)
{
	ChangeMSB(&softResetCmd);
	HAL_I2C_Master_Transmit(hi2c, (SHT31_ADDR<<1), (uint8_t*) &softResetCmd, 2, Timeout);
	HAL_Delay(10);
}

void SHT31::ReadStatusReg(SHT31_data *sht31)
{
	HAL_I2C_Mem_Read(hi2c, (SHT31_ADDR<<1), SHT31_READ_STATUSREG, I2C_MEMADD_SIZE_16BIT, sht31->bits, 6, Timeout);
	sht31->statusReg = (sht31->bits[0] << 8 | sht31->bits[1]);
	printf("statusReg: %d \r\n", sht31->statusReg);
}

void SHT31::ClearStatusReg(uint16_t clearStatusRegCmd)
{
	ChangeMSB(&clearStatusRegCmd);
	HAL_I2C_Master_Transmit(hi2c, (SHT31_ADDR<<1), (uint8_t*)&clearStatusRegCmd, 2, Timeout);
	HAL_Delay(10);
}

void SHT31::ChangeHeaterStatus(uint16_t heaterCmd)
{
	ChangeMSB(&heaterCmd);
	HAL_I2C_Master_Transmit(hi2c, (SHT31_ADDR<<1), (uint8_t*)&heaterCmd, 2, Timeout);
	HAL_Delay(10);
}

void SHT31::ChangeMSB(uint16_t* cmd)
{
	*cmd = (((*cmd) & 0x00FF) << 8) | ((*cmd) >> 8);
}

uint8_t SHT31::CheckCRC(uint8_t *data, int len)
{
	const uint8_t POLYNOMIAL = 0x31;
	uint8_t crc =0xFF;

	for ( int j = len; j; --j ) {
		crc ^= *data++;

		for ( int i = 8; i; --i ) {
			crc = ( crc & 0x80 )
			  ? (crc << 1) ^ POLYNOMIAL
		  	: (crc << 1);
		}
	}
	return crc;
}
