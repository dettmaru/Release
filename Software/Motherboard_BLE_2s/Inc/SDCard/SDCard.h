/*
 * SDCard.h
 *
 *  Created on: May 23, 2018
 *      Author: Daniel Lohmann
 */

#ifndef SDCARD_SDCARD_H_
#define SDCARD_SDCARD_H_

#include "fatfs.h"
#include "sdmmc.h"
#include "SensorDataSet.h"
#include "main.h"
#include "stm32l4xx_hal.h"

/**
 * @brief High level control of the SD Card
 */
class SDCard
{
public:
	static bool Initialized();
	static HAL_StatusTypeDef Init();
	static HAL_StatusTypeDef DeInit();
	static HAL_StatusTypeDef WriteSensorData(const SensorDataSet* dataSet);
	static HAL_StatusTypeDef WriteSensorDataTableHeader();
private:
	static FATFS diskFatFS;
	static int AppendValuesOrNull(float * destination, std::list<float>* src, int count);
};

#endif /* SDCARD_SDCARD_H_ */
