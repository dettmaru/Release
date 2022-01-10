/*
 * sdi12_Sensor.cpp
 *
 *  Created on: Apr 23, 2019
 *      Author: Daniel Lohmann
 */

#ifndef MEASURE_SDI12_SENSOR_CPP_
#define MEASURE_SDI12_SENSOR_CPP_

#include <vector>
#include "stdint.h"
#include "sdi12_SensorState.h"

#define SDI12_SENSOR_TYPE_ECOTECH_TENSIOMARK 0x1
#define SDI12_SENSOR_TYPE_ECOTECH_HYDRAPROBE 0x2

class SDI12_Sensor
{
public:
	SDI12_Sensor();
	SDI12_Sensor(uint8_t address, uint32_t sensorType);
	~SDI12_Sensor();

	bool readingInProgress;
	bool waitingForResponse;
	uint8_t address;
	uint8_t* lastCommand;
	std::vector<uint8_t*> lastResponses;
	/**
	 * @brief Millisecond time on which the last waiting has begun
	 */
	uint32_t waitBegin;
	/**
	 * @brief Millisecond wait time for next planned step
	 */
	uint16_t waitTime;
	/**
	 * @brief Current state of transmission (start measurement or finished measurement?)
	 */
	SDI12_SensorState state;
	/**
	 * @brief Expected amount of sensor values, retrieved with the
	 */
	uint8_t expectedAmount;
	/**
	 * @brief Type of sensor to identify which values should be saved in which data set
	 */
	uint32_t sensorType;
	/**
	 * @brief retrieved values
	 */
	std::vector<float> values;
};


#endif /* MEASURE_SDI12_SENSOR_CPP_ */
