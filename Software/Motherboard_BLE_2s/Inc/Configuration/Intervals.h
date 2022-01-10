/*
 * Intervals.h
 *
 *  Created on: Aug 6, 2018
 *      Author: Daniel Lohmann
 *
 * This header contains an enum of intervals
 */

#ifndef CONFIGURATION_INTERVALS_H_
#define CONFIGURATION_INTERVALS_H_

enum class Intervals {
    Sending,
    BatteryVoltage,
    SoilMoisture,
    SoilTemperature,
    AirTemperature,
    AirHumidity,
    Illuminance,
    pHValue,
    NTPTimeSync,
    END
};

#endif