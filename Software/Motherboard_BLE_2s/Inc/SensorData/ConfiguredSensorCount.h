/*
 * ConfiguredSensorCount.h
 *
 *  Created on: Oct 31, 2019
 *      Author: Daniel Lohmann
 */

#ifndef SENSORDATA_CONFIGUREDSENSORCOUNT_H_
#define SENSORDATA_CONFIGUREDSENSORCOUNT_H_

typedef struct {
    int BatteryVoltageSensorCount;
    int AirTemperatureSensorCount;
    int AirHumiditySensorCount;
    int IlluminanceSensorCount;
    int SoilTemperatureSensorCount;
    int SoilMoistureSensorCount;
    int SoilpHSensorCount;
} ConfiguredSensorCount;

#endif /* SENSORDATA_CONFIGUREDSENSORCOUNT_H_ */
