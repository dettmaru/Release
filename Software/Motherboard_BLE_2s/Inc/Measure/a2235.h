/**
 * @file a2235.h
 *
 *  Created on: Apr 27, 2018
 *      Author: Felix Schuette, Joschka Wirges, Daniel Lohmann
 */
#include <stdbool.h>
#include <stdio.h>
#include "main.h"

#ifndef A2235_H_
#define A2235_H_

/**
 * @brief Stores extracted information of GPS-RMC strings sent by the A2235-H 
 * GPS Receiver
 */
class A2235 
{
public:
    /**
     * @brief Pointer to the UART interface
     */
	UART_HandleTypeDef *huart;
    /**
     * @brief Stores the UTC time in HHMMSS format
     */
    char time[21];
    /**
     * @brief Stores the date in DDMMYY format
     */
    char date[8];
    /** 
     * @brief Stores the latitude 
     * 
     * Example:
     * 
     * The value "4916.45" refers to a latitude 49 deg. 16.45min
     */
    char latitude[11];
    /**
     * @brief stores the longitude
     * 
     * Example:
     * 
     * The value "12311.12" refers to a longitude 123 deg. 11.12min
     */
    char longitude[11];
    /** 
     * @brief Stores the direction of the longitude
     * 
     * - "N" for north
     * - "S" for south
     */
    char latitudeDir[2];
    /**
     * @brief Stores the direction of the latitude
     * 
     * - "W" for west
     * - "E" for east
     */
    char longitudeDir[2];
    /**
     * @brief Signals whether the time and date is available or not
     * 
     * True, when the time and date has been received and is available in 
     * \ref time and \ref date, Otherwise false
     */
    bool time_available;
    /**
     * @brief Signals whether the time
     * 
     * True, when the location has been received and is decoded. The decoded 
     * values are stored in \ref latitude, \ref longitude, \ref latitudeDir and
     * \ref longitudeDir
     */
    bool location_available;
    /**
     * @brief Signals whether the GPS module is on (true) or off (false)
     */
    bool on;

    uint8_t ProcessNMEA(char* buf);
    uint8_t DecodeNMEA(char *buf);
    void Init(UART_HandleTypeDef *huart);
    void DeInit();
    bool GetRMC(uint32_t timeout);
    bool GetTime(struct tm* time);
private:
    bool CheckChecksum(char* buf); 
};

extern A2235 hGPS;

#endif /* A2235_H_ */
