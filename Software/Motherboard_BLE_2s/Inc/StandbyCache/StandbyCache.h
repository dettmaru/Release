/*
 * StandbyCache.h
 *
 *  Created on: Aug 15, 2018
 *      Author: Daniel Lohmann
 */
#ifndef STANDBYCACHE_H
#define STANDBYCACHE_H

#include "SensorDataSet.h"
#include "StandbyCacheMemoryAddress.h"
#include <vector>

#define CACHE_INITALVALUE 0

/**
 * @brief Cache for sensor data while in standby mode
 */
class StandbyCache 
{
public: 
    static void EnableCache();
    static void DisableCache();
    static void ClearCache();
    static void RemoveIDs(std::vector<int> ids);
    static int GetSensorDataSetsCount();
    static bool PushSensorDataSet(const SensorDataSet* dataSet);
    static SensorDataSet* GetSensorDataSet(int index);
private:
    static uint8_t* Itterate(int* count);
};

#endif /* STANDBYCACHE_H */
