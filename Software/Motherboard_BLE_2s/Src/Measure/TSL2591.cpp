/**
 * @brief Measurement of illuminance values
 * 
 * @file TSL2591.cpp
 * @author Daniel Lohmann
 * @date 2019-03-13
 */
#include "TSL2591.h"
#include "main.h"
#include "HardwareSemaphores.h"

#define TSL2591_I2C_TIMEOUT 50

TSL2591::TSL2591(I2C_HandleTypeDef* hi2c)
{
    this->hi2c = hi2c;
    this->initialized = false;
    this->is_enabled = false;
}

TSL2591::~TSL2591()
{

}

uint8_t TSL2591::Init()
{
	if (initialized)
	{
		return 1;
	}
	CheckID();
	if (initialized == 0) {
		return 0;
	}
	Enable();
	SetIntegrationTime(TSL2591_INTEGRATIONTIME_100MS);
	//setGain(luxSensor, TSL2591_GAIN_MED);
	SetGain(TSL2591_GAIN_LOW);
	Disable();
	return 1;
}

void TSL2591::CheckID()
{
    uint8_t _id = 0;
	HAL_I2C_Mem_Read(hi2c, (TSL2591_ADDR << 1),
			(TSL2591_COMMAND_BIT | TSL2591_ID), I2C_MEMADD_SIZE_8BIT, &_id, 1,
			100);
	if (_id == 0x50) {
		initialized = true;
	} else {
		initialized = false;
	}
}

void TSL2591::Enable()
{
	if (initialized)
	{
		uint8_t cmd = (TSL2591_ENABLE_POWERON | TSL2591_ENABLE_AEN
			| TSL2591_ENABLE_AIEN);
		HAL_I2C_Mem_Write(hi2c, (TSL2591_ADDR << 1),
			(TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE),
			I2C_MEMADD_SIZE_8BIT, (uint8_t*) &cmd, 1, TSL2591_I2C_TIMEOUT);
		is_enabled = true;
	}
}

void TSL2591::Disable()
{
    //uint8_t cmd = (TSL2591_ENABLE_POWEROFF);
	//HAL_I2C_Mem_Write(&hi2c1, (TSL2591_ADDR<<1), (TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE), I2C_MEMADD_SIZE_8BIT, (uint8_t*)&cmd, 1, 0xffff);
	is_enabled = false;
}

void TSL2591::SetGain(uint8_t gain)
{
    Enable();
	this->gain = gain;
	uint8_t cmd = (integrationTime | gain);
	HAL_I2C_Mem_Write(hi2c, (TSL2591_ADDR << 1),
			(TSL2591_COMMAND_BIT | TSL2591_REGISTER_CONTROL),
			I2C_MEMADD_SIZE_8BIT, (uint8_t*) &cmd, 1, TSL2591_I2C_TIMEOUT);
	Disable();
}

void TSL2591::SetIntegrationTime(uint8_t time)
{
    Enable();
	this->integrationTime = time;
	uint8_t cmd = (integrationTime | this->gain);
	HAL_I2C_Mem_Write(hi2c, (TSL2591_ADDR << 1),
			(TSL2591_COMMAND_BIT | TSL2591_REGISTER_CONTROL),
			I2C_MEMADD_SIZE_8BIT, (uint8_t*) &cmd, 1, TSL2591_I2C_TIMEOUT);
	Disable();
}

void TSL2591::ReadChannels()
{
    uint8_t ch0[2], ch1[2];
	Enable();
	if (is_enabled & initialized)
	{
		HAL_Delay(100 * integrationTime + 100);

		HAL_I2C_Mem_Read(hi2c, (TSL2591_ADDR << 1),
				(TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN0_LOW),
				I2C_MEMADD_SIZE_8BIT, (uint8_t *) &ch0, 2, TSL2591_I2C_TIMEOUT);
		this->ch0 = ch0[1] << 8 | ch0[0];

		HAL_I2C_Mem_Read(hi2c, (TSL2591_ADDR << 1),
				(TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN1_LOW),
				I2C_MEMADD_SIZE_8BIT, (uint8_t *) &ch1, 2, TSL2591_I2C_TIMEOUT);
		this->ch1 = ch1[1] << 8 | ch1[0];
	}
	Disable();
}

uint16_t TSL2591::CalculateLux()
{
    uint16_t atime, again;
	float cpl, lux1, lux2;
	ReadChannels();
	if (initialized)
	{
		atime = (integrationTime * 100) + 100;
		if (gain == TSL2591_GAIN_LOW) {
			again = 1.0F;
		} else if (gain == TSL2591_GAIN_MED) {
			again = 25.0F;
		} else if (gain == TSL2591_GAIN_HIGH) {
			again = 428.0F;
		} else if (gain == TSL2591_GAIN_MAX) {
			again = 9876.0F;
		} else {
			again = 1.0F;
		}
		cpl = (atime * again) / TSL2591_LUX_DF;

		lux1 = ((float)ch0
				- (TSL2591_LUX_COEFB * (float)ch1)) / cpl;
		lux2 = (( TSL2591_LUX_COEFC * (float)ch0)
				- ( TSL2591_LUX_COEFD * (float)ch1)) / cpl;

		lux = lux1 > lux2 ? lux1 : lux2;
	}
	else
	{
		// If the sensor is not connected we will result here
		// TODO default value for sensor not available???
		lux = 0;
	}
	return lux;
}

uint16_t TSL2591::GetLux()
{
    return lux;
}

void TSL2591::TurnON()
{
    airSensors.Request();

}

void TSL2591::TurnOFF()
{
    airSensors.Free();
    initialized = false;
}

void TSL2591::StartMeasurement()
{
    startTime = HAL_GetTick();
    TurnON();
}


bool TSL2591::ProcessMeasurement()
{
    // Wait for ready for the measurement
	if (HAL_GetTick() - startTime > TSL2591_INITWAIT_TIME + TSL2591_WAKEUP_TIME)
	{
		CalculateLux();

		TurnOFF();
		return false;
	}
    if (HAL_GetTick() - startTime > TSL2591_WAKEUP_TIME) 
    {
        if (Init() == 1)
        {
        	return true;
        }
        else
        {
            lux = 0;
            return false;
        }
    }
    else // Not ready for measurement
    {
        return true;
    }
}
