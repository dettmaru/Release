/*
 * StandbyCache.cpp
 *
 *  Created on: Sep 13, 2018
 *      Author: Daniel Lohmann
 */
#include "StandbyCache.h"
#include "main.h"
#include <algorithm>

#ifndef TESTING
uint8_t* ptrCacheMemory = (uint8_t*)SRAM2_BASE;
#else
uint8_t ptrCacheMemory[SRAM2_SIZE];
#endif

/**
 * @brief Enables the memory retention while being in standby mode
 */
void StandbyCache::EnableCache()
{
	if (!READ_BIT(PWR->CR3, PWR_CR3_RRS))
	{
		HAL_PWREx_EnableSRAM2ContentRetention();
		ClearCache();
	}
}

/**
 * @brief Disables the memory retention while being in standby mode
 */
void StandbyCache::DisableCache()
{
    HAL_PWREx_DisableSRAM2ContentRetention();
}

/**
 * @brief Clears the cache
 */
void StandbyCache::ClearCache()
{
	for (uint32_t i = 0; i < SRAM2_SIZE; i++)
	{
		ptrCacheMemory[i] = CACHE_INITALVALUE;
	}
}

/**
 * @brief Remove packets by ids
 */
void StandbyCache::RemoveIDs(std::vector<int> ids)
{
	std::sort(ids.begin(), ids.end());
	std::vector<int>::iterator it = ids.begin();
	uint8_t* src = ptrCacheMemory;
	uint8_t* dest = ptrCacheMemory;

	int count = GetSensorDataSetsCount();
	for (int i = 0; i < count; i++)
	{
		uint16_t size = (src[0] << 8) | (src[1]);
		if (*it == i)
		{
			// Do not move the parts which should be deleted
			// => skip then, because they will be overwritten
			it++; 
		}
		else
		{
			// Change location of data in the cache
			for (int j = 0; j < size + 2; j++)
			{
				dest[j] = src[j];
			}
			dest += size + 2;
		}

		src += size + 2;
	}

	// Fill rest of the SRAM with the initial value
	while (dest < ptrCacheMemory + SRAM2_SIZE)
	{
		*dest = CACHE_INITALVALUE;
		dest++;
	}

	return;
}

/**
 * @brief Gets the amount of sensor data sets stored in the cache
 * 
 * @return int, Count of sensor data sets in cache
 */
int StandbyCache::GetSensorDataSetsCount()
{
    int count = 0;
	Itterate(&count);
    return count;
}

/**
 * @brief Add a sensor data set to the cache
 * 
 * @param dataSet Data set which will be added to the cache
 * @return true, when the data set could be saved
 * @return false, when the data set couldn't be saved
 */
bool StandbyCache::PushSensorDataSet(const SensorDataSet* dataSet)
{
    int count = 0;
	uint8_t* begin = Itterate(&count);
	if (begin == nullptr)
	{
		// Not enough memory available
		return false; 
	}

	uint16_t size = 0;
    uint8_t* data = dataSet->GetMQTTSensorDataContent(&size);
	if (begin + 2 + size >= ptrCacheMemory + SRAM2_SIZE)
	{
		delete[] data;
		// Not enough memory available
		return false;
	}

    begin[0] = size>>8;
    begin[1] = size;
    begin++; begin++;

    uint8_t* dataDestPtr = (uint8_t*)begin;
    uint8_t* dataSrcPtr = data;
    while (size > 0)
    {
    	*dataDestPtr = *dataSrcPtr;
    	dataDestPtr++;
    	dataSrcPtr++;
    	size--;
    }

    delete[] data;
    return true;
}

/**
 * @brief Itterates over the cache memory 
 * @param[out] Count of blocks
 * @return uint8_t* pointer to a next memory location, or nullptr if it would be 
 *                  behind the available memory
 */
uint8_t* StandbyCache::Itterate(int* count)
{
	uint8_t* ptrCurrent = ptrCacheMemory;
	uint16_t length = 0;
     *count = 0;

	while (ptrCurrent[0] != CACHE_INITALVALUE ||
		   ptrCurrent[1] != CACHE_INITALVALUE)
	{
		length = (ptrCurrent[0]<<8) | ptrCurrent[1];
		ptrCurrent += 2 + length;
        (*count)++;

		if (ptrCurrent >= SRAM2_SIZE + ptrCacheMemory)
		{
			return nullptr;
		}
	}

	return ptrCurrent;
}

/**
 * @brief Gets the data of an indexed sensor data set
 * 
 * @param index, Index of the sensor data set which should be retrived
 * @return SensorDataSet*, reference to a SensorDataSet in RAM memory which is a
 *                         new allocated memory space (needs to be deleted after
 *                         wards)
 */
SensorDataSet* StandbyCache::GetSensorDataSet(int index)
{
    uint8_t* ptrCurrent = ptrCacheMemory;
	uint16_t length = 0;
    int count = 0;

	while ((ptrCurrent[0] != CACHE_INITALVALUE ||
		    ptrCurrent[1] != CACHE_INITALVALUE) && count < index)
	{
		length = (ptrCurrent[0]<<8) | ptrCurrent[1];
		ptrCurrent += 2 + length;
        count++;
	}

    SensorDataSet* dataSet = new SensorDataSet(ptrCurrent + 2, length);

    return dataSet;
}


