/*
 * SDCard.cpp
 *
 *  Created on: May 23, 2018
 *      Author: Daniel Lohmann
 */
#include "TimeHelper.h"
#include "SDCard.h"
#include "Energy.h"
#include "stdio.h"
#include "Logging.h"
#include "stdint.h"
#include "DataProcessor.h"

/**
 * @brief Save time between turnoff and turnon of the SD Card
 */
#define SDCARD_SAVE_OFF_TIME 10

/**
 * @brief Name of the sensor file on the SD Card
 */
#define SDCARD_FILENAME_SENSOR "Sensor.csv"

/**
 * @brief Filesystem object. This object needs to be persistent 
 * 		  as long as the file system is mounted
 */
FATFS SDCard::diskFatFS;

/**
 * @brief Stores the time of the last turnoff of the SD Card
 */
volatile uint32_t lastDeinit = UINT32_MAX;

/**
 * @brief Indicates whether the SD card is initialized or not
 */
volatile bool initialized = false; 

/**
 * Returns the initialization status of the SD card
 * @return
 */
bool SDCard::Initialized()
{
	return initialized;
}

/**
 * @brief Initialises the SD card. This includes the mounting process.
 * 
 * @return HAL_StatusTypeDef HAL_OK, when operation was successfull, 
 * 							 otherwise HAL_ERROR
 */
HAL_StatusTypeDef SDCard::Init()
{
	if (initialized == false)
	{
		// Save time before re-enabling SD Card, because fast power cycling can 
		// create issues
		while (HAL_GetTick() - lastDeinit < SDCARD_SAVE_OFF_TIME) ;

		FRESULT res = FR_OK;
		uint8_t init_res = HAL_OK;

		Energy::RequestHighPowerMode();

		MX_SDMMC1_SD_Init();
		MX_FATFS_Init();

		//Give the SD-Card power
		HAL_GPIO_WritePin(SD_PWR_GPIO_Port, SD_PWR_Pin, GPIO_PIN_RESET);

		init_res = BSP_SD_Init();
		if (init_res == MSD_ERROR_SD_NOT_PRESENT)
		{
			LOGW("SDCard", "SD Card not present!");
		}
		else if (init_res != FR_OK)
		{
			LOGE("SDCard", "SD Card could not init! %d", init_res);
			return HAL_ERROR;
		}

		// Mount the file system. This is necessary to read and write files
		res = f_mount(&diskFatFS, (TCHAR const*)SDPath, 1);
		if (res != FR_OK)
		{
			LOGE("SDCard", "SD Card could not init, mounting failed res=%d!", res);
			return HAL_ERROR;
		}
		initialized = true;
	}
	return HAL_OK;
}

/**
 * @brief Writes one sensor data set onto the SD card.
 * 
 * @param dataSet Pointer to the sensor data set which should be written onto 
 * 				  the SD card
 * @return HAL_StatusTypeDef Returns HAL_OK, when the data was written 
 * 							 successfully, otherwise HAL_ERROR
 */
HAL_StatusTypeDef SDCard::WriteSensorData(const SensorDataSet* dataSet)
{
	// If no file exists a new file header should be put into the file which 
	// labels for the data columns
	FIL fileHandlerTest;
	if (f_open(&fileHandlerTest, (const TCHAR*)SDCARD_FILENAME_SENSOR, FA_OPEN_EXISTING) == FR_NO_FILE)
	{
		WriteSensorDataTableHeader();	
	}
	else
	{
		if (f_close(&fileHandlerTest) != FR_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
			return HAL_ERROR;
		}
	}
	
	// Write sensor data into file
	FIL fileHandler;
	if (f_open(&fileHandler, (const TCHAR*)SDCARD_FILENAME_SENSOR, FA_OPEN_APPEND | FA_WRITE) == FR_OK)
	{
		/* Current columns */
		/*    - TimeStamp
		 * 0  - BatteryVoltage
		 * 1  - AirTemperature (STM)
		 * 2  - AirTemperature (Extern - SHT31)
		 * 3  - Humidity
		 * 4  - Illuminance
		 * 5  - Moisture XX top
		 * 6  - Moisture XX bottom
		 * ..
		 * XX - SoilTemperature XX top
		 * XX - SoilTemperature XX bottom
		 * ---
		 */

		std::list<float>* valueList;
		const int CountOfValues =  
			DataProcessor::GetConfiguredBatteryVoltageCount() + 
			DataProcessor::GetConfiguredAirTemperatureCount() + 
			DataProcessor::GetConfiguredAirHumidityCount() + 
			DataProcessor::GetConfiguredIlluminanceCount() + 
			DataProcessor::GetConfiguredSoilTemperatureCount() + 
			DataProcessor::GetConfiguredSoilMoistureCount() +
			DataProcessor::GetConfiguredSoilpHCount();

		float values[CountOfValues] = {0};
		int index = 0;

		valueList = dataSet->GetBatteryVoltagesAsFloat();
		index += AppendValuesOrNull(values + index, valueList, DataProcessor::GetConfiguredBatteryVoltageCount());
		delete valueList;

		valueList = dataSet->GetAirTemperaturesAsFloat();
		index += AppendValuesOrNull(values + index, valueList, DataProcessor::GetConfiguredAirTemperatureCount());
		delete valueList;

		valueList = dataSet->GetAirHumiditiesAsFloat();
		index += AppendValuesOrNull(values + index, valueList, DataProcessor::GetConfiguredAirHumidityCount());
		delete valueList;

		valueList = dataSet->GetIlluminancesAsFloat();
		index += AppendValuesOrNull(values + index, valueList, DataProcessor::GetConfiguredIlluminanceCount());
		delete valueList;

		valueList = dataSet->GetSoilMoisturesAsFloat();
		index += AppendValuesOrNull(values + index, valueList, DataProcessor::GetConfiguredSoilMoistureCount());
		delete valueList;

		valueList = dataSet->GetSoilTemperaturesAsFloat();
		index += AppendValuesOrNull(values + index, valueList, DataProcessor::GetConfiguredSoilTemperatureCount());
		delete valueList;

		valueList = dataSet->GetpHValuesAsFloat();
		index += AppendValuesOrNull(values + index, valueList, DataProcessor::GetConfiguredSoilpHCount());
		delete valueList;

		struct tm time = dataSet->GetTimestamp();
		f_printf(&fileHandler, "%02d.%02d.%04d %02d:%02d:%02d",
				time.tm_mday,
				time.tm_mon + 1,
				time.tm_year + TIME_STRUCT_TM_RELATIVE_YEAR,
				time.tm_hour,
				time.tm_min,
				time.tm_sec);

		char buffer[30];
		for (uint16_t i = 0; i < CountOfValues; i++)
		{
			sprintf(buffer, ",%f", values[i]);
			f_printf(&fileHandler, "%s", buffer);
		}
		f_printf(&fileHandler, "\n");

		if (f_close(&fileHandler) != FR_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
			return HAL_ERROR;
		}
	}
	else
	{
		_Error_Handler(__FILE__, __LINE__);
		return HAL_ERROR;
	}
	return HAL_OK;
}

/**
 * @brief Writes a header line for the sensor data CSV file
 * 
 * @return HAL_StatusTypeDef Returns HAL_OK, when the data was written 
 * 							 successfully, otherwise HAL_ERROR
 */
HAL_StatusTypeDef SDCard::WriteSensorDataTableHeader()
{
	FIL fileHandler;
	if (f_open(&fileHandler, (const TCHAR*)SDCARD_FILENAME_SENSOR, FA_OPEN_APPEND | FA_WRITE) == FR_OK)
	{
		f_printf(&fileHandler, "Date");
		for (int i = 0; i < DataProcessor::GetConfiguredBatteryVoltageCount(); i++) 
		{
			f_printf(&fileHandler, ",Battery Voltage %d", i + 1);
		}
		for (int i = 0; i < DataProcessor::GetConfiguredAirTemperatureCount(); i++) 
		{
			f_printf(&fileHandler, ",Air Temperature %d", i + 1);
		}
		for (int i = 0; i < DataProcessor::GetConfiguredAirHumidityCount(); i++) 
		{
			f_printf(&fileHandler, ",Air Humidity %d", i + 1);
		}
		for (int i = 0; i < DataProcessor::GetConfiguredIlluminanceCount(); i++) 
		{
			f_printf(&fileHandler, ",Illuminance %d", i + 1);
		}
		for (int i = 0; i < DataProcessor::GetConfiguredSoilMoistureCount(); i++) 
		{
			if (i % 2 == 0)
			{
				f_printf(&fileHandler, ",Soil Moisture %d Top", i/2 + 1);
			}
			else
			{
				f_printf(&fileHandler, ",Soil Moisture %d Bottom", i/2 + 1);
			}
		}
		for (int i = 0; i < DataProcessor::GetConfiguredSoilTemperatureCount(); i++) 
		{
			if (i % 2 == 0)
			{
				f_printf(&fileHandler, ",Soil Temperature %d Top", i/2 + 1);
			}
			else
			{
				f_printf(&fileHandler, ",Soil Temperature %d Bottom", i/2 + 1);
			}
		}
		for (int i = 0; i < DataProcessor::GetConfiguredSoilpHCount(); i++) 
		{
			f_printf(&fileHandler, ",pH Value %d", i + 1);
		}
		f_printf(&fileHandler, "\n");

		if (f_close(&fileHandler) != FR_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
			return HAL_ERROR;
		}
	}
	else
	{
		_Error_Handler(__FILE__, __LINE__);
		return HAL_ERROR;
	}
	return HAL_OK;
}

/**
 * @brief Turns the SD card off
 */
HAL_StatusTypeDef SDCard::DeInit()
{
	// SD-Card is now not initialized anymore
	initialized = false;

	// Deinit FatFS
	f_mount(NULL, (TCHAR const*)SDPath, 1);

	// Deinit SD Card peripheral
	HAL_SD_DeInit(&hsd1);

	// Turn off the SD-Card
	HAL_GPIO_WritePin(SD_PWR_GPIO_Port, SD_PWR_Pin, GPIO_PIN_SET);

	Energy::FreeRequestHighPowerMode();

	// Remember last turn off, because fast cycling is problematic
	lastDeinit = HAL_GetTick();

	return HAL_OK;
}

/**
 * @brief Append defined amount of values into array or null, if the source list has less values!
 * 
 * @return Amount of written values into destination, which is equal to count
 */
int SDCard::AppendValuesOrNull(float * destination, std::list<float>* src, int count)
{
	auto itr = src->begin();
	for (int i = 0; i < count; i++)
	{
		if (itr != src->end())
		{
			destination[i] = *itr;
			itr++;
		}
		else
		{
			destination[i] = 0;			
		}
	}
	return count;
}

