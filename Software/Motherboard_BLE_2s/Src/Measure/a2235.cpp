/**
 * @file a2235.c
 *
 *  Created on: Apr 27, 2018
 *      Author: Felix Schuette, Joschka Wirges, Daniel Lohmann
 */
#include "a2235.h"
#include "stm32l4xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stdint.h"
#include "stdbool.h"
#include "main.h"
#include "usart.h"
#include "Logging.h"
#include "time.h"
#include "stdlib.h"
#include "TimeHelper.h"
#include "Energy.h"
#include "HardwareSemaphores.h"

A2235 hGPS;
const char* GPSTag = "GPS";
uint8_t GPS_RxBuffer = 0;

/**
 * Takes the GPSRMC string and extracts the location and time out of it.
 * Parameters
 * -------
 * @param buf (chararray) stores the GPSRMC string
 *
 * @retVal 0 if the RMC string was empty
 * @retVal 1 if the RMC string contained only time and date information
 * @retVal 2 if the RMC string contained time, date and time information
 */
uint8_t A2235::ProcessNMEA(char* buf)
{
	uint8_t getRMC = 0;
	getRMC = DecodeNMEA(buf);
	if (getRMC == 1) {
		/* when the GPS module only offers time and date, the time might still be unsynchronized and wrong
			* this can be detected by the date which is always of the year 2011, the timestamp is then wrong
			* should be discarded which is done here
			*/
		if (hGPS.date[4] == '1' && hGPS.date[5] == '1') { // this will be a problem in year 2111
			hGPS.time_available = false;
		} else {
			hGPS.time_available = true;
			struct tm tm;
			if (hGPS.GetTime(&tm)) {
				Time::Set(&tm);
			}
		}
		hGPS.location_available = false;
	} else if (getRMC == 2) {
		hGPS.time_available = true;
		hGPS.location_available = true;
		struct tm tm;
		if (hGPS.GetTime(&tm)) {
			Time::Set(&tm);
		}
	} else if (getRMC == 0 || getRMC == 255) {
		hGPS.time_available = false;
		hGPS.location_available = false;
	}
	return getRMC;
}

/**
 * Takes the GPSRMC string and extracts the location and time out of it.
 * Parameters
 * -------
 * @param buf (chararray) stores the GPSRMC string
 *
 * @retVal 0 if the RMC string was empty
 * @retVal 1 if the RMC string contained only time and date information
 * @retVal 2 if the RMC string contained time, date and time information
 * @retVal 255 if the RMC string had bad checksum
 */
uint8_t A2235::DecodeNMEA(char *buf) {
	uint8_t ret = 0;
	on = true;
	uint8_t numToken = 0;
	if (!CheckChecksum(buf))
		return 255;

	// Saving buffer in a temporary buf
	char *temp = strdup(buf);

	//Splitting the RMC string into pieces
	char *token = strtok(temp, ",");
	while (token != NULL) {
		//printf("%s \r\n", token);
		token = strtok(NULL, ",");
		numToken++;
	}

	// No signal, no information
	if (numToken <= 4) {
		ret = 0;

		// Just timestamp and date
	} else if (numToken == 5) {
		token = strtok(buf, ",");
		numToken = 1;
		while (token != NULL) {
			if (numToken == 2) {
				// Extract time from token
				//hGPS->time = (uint32_t) strtol(token, NULL, 10);
				strcpy(time, token);
			} else if (numToken == 4) {
				// Extract date from token
				//hGPS->date = (uint32_t) strtol(token, NULL, 10);
				strcpy(date, token);
			}
			token = strtok(NULL, ",");
			numToken++;

		}
		ret = 1;

		// Received time, date and location
	} else if (numToken > 5) {
		token = strtok(buf, ",");
		numToken = 1;
		while (token != NULL) {
			if (numToken == 2) {
				// Extract time from token
				//hGPS->time = (uint32_t) strtol(token, NULL, 10);
				strcpy(time, token);
			} else if (numToken == 4) {
				// Extract Latitude
				//hGPS->latitude = (float) strtod(token, NULL);
				strcpy(latitude, token);
			} else if (numToken == 5) {
				// Extract Latitude Direction
				//hGPS->latitudeDir = *token;
				strcpy(latitudeDir, token);
			} else if (numToken == 6) {
				//Extract Longitude
				//hGPS->longitude = (float) strtod(token, NULL);
				strcpy(longitude, token);
			} else if (numToken == 7) {
				//Extract Longitude Direction
				//hGPS->longitudeDir = *token;
				strcpy(longitudeDir, token);
			} else if (numToken == 10) {
				//Extract Longitude Direction
				//hGPS->longitudeDir = *token;
				strcpy(date, token);
			}
			token = strtok(NULL, ",");
			numToken++;
		}
		ret = 2;
	}
	free(temp);
	return ret;
}

/**
 * @brief Initializes the GPS module and waits for the first RMC NMEA sentence
 * 
 * @param huart Handle to the UART which is connected to the GPS module
 */
void A2235::Init(UART_HandleTypeDef *huart) {
	LOGV(GPSTag, "Booting GPS module");

	// Initialize the UART only when needed
	uart1.Request();

	Energy::RequestHighPowerMode();

	HAL_UART_Receive_DMA(huart, &GPS_RxBuffer, 1);

	/* turn on powerswitch */
	HAL_GPIO_WritePin(GPS_ON_GPIO_Port, GPS_ON_Pin, GPIO_PIN_RESET);
	HAL_Delay(500);
	date[6] = '\0';
	latitude[9] = '\0';
	latitudeDir[1] = '\0';
	longitude[9] = '\0';
	longitudeDir[1] = '\0';
	time[19] = '\0';
	this->huart = huart;
	uint8_t attempts=0;

	while(!on && attempts < 60){// 600attempts*100ms delay=60s timeout
		HAL_GPIO_WritePin(GPS_ON_SW_GPIO_Port, GPS_ON_SW_Pin, GPIO_PIN_SET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPS_ON_SW_GPIO_Port, GPS_ON_SW_Pin, GPIO_PIN_RESET);
		HAL_Delay(1500);
		attempts++;
	}
	time_available = false;
	location_available = false;
	LOGIS(GPSTag, "GPS module boot complete");
}

/**
 * @brief Deinitializes the GPS modules, and waits for its shutdown
 */
void A2235::DeInit() {
	LOGV(GPSTag, "Powering off the GPS module");
	uint8_t attempts = 0;
	while(on && attempts < 30) // 30attempts*2000ms delay=60s timeout
	{
		HAL_GPIO_WritePin(GPS_ON_SW_GPIO_Port, GPS_ON_SW_Pin, GPIO_PIN_SET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPS_ON_SW_GPIO_Port, GPS_ON_SW_Pin, GPIO_PIN_RESET);
		on = false;//TODO check if module is really off with hGPS->On
		HAL_Delay(2000);
		attempts++;
	}

	if(on)
	{
		LOGE(GPSTag, "Couldn't power off GPS module safely");
	}

	HAL_UART_DMAStop(huart);

	/* turn off powerswitch */
	HAL_GPIO_WritePin(GPS_ON_GPIO_Port, GPS_ON_Pin, GPIO_PIN_SET);
	Energy::FreeRequestHighPowerMode();

	/* Free the request from GPS module for the UART */
	uart1.Free();

	on = false;
	time_available = false;
	location_available = false;
	LOGV(GPSTag, "Powered off the GPS module");
}

/**
 * @brief Waits for a RMC location information
 * 
 * @param timeout Timeout in milliseconds after which the pause will stop.
 * @return true, when the location could be successfull retrieved from a RMC sentence.
 * @return false, when the location was not retrieved and the operation timed out.
 */
bool A2235::GetRMC(uint32_t timeout) {
	uint32_t tickstart = HAL_GetTick(); //current timer as a reference for the timeout
	bool timed_out = false; //return value
	while (!location_available && (!timed_out)) 
	{
		/* Check for the timeout */
		if (timeout != HAL_MAX_DELAY) 
		{
			if ((timeout == 0U) || ((HAL_GetTick() - tickstart) > timeout)) 
			{
				timed_out = true;
				LOGW(GPSTag, "GPS_GetLocation timed out");
			}
		}
	}

	return timed_out;
}

/**
 * @brief Parses the stored time which has been received from the GPS module
 * @param t[out] Pointer to a c time structure
 * @returns true: The time was available and has been written into the provided 
 * 		          time structure.
 * 			false: No time has been received, therefore no time has been written 
 * 				  into the provided time structure.
 */
bool A2235::GetTime(struct tm* t)
{
	if (time_available)
	{
		char buffer[3] = {0};

		buffer[0] = date[0]; buffer[1] = date[1]; buffer[2] = '\0';
		t->tm_mday = strtol(buffer, NULL, 10);
		buffer[0] = date[2]; buffer[1] = date[3]; buffer[2] = '\0';
		t->tm_mon = strtol(buffer, NULL, 10) - 1;
		buffer[0] = date[4]; buffer[1] = date[5]; buffer[2] = '\0';
		t->tm_year = strtol(buffer, NULL, 10) + 100;

		buffer[0] = time[0]; buffer[1] = time[1]; buffer[2] = '\0';
		t->tm_hour = strtol(buffer, NULL, 10);
		buffer[0] = time[2]; buffer[1] = time[3]; buffer[2] = '\0';
		t->tm_min = strtol(buffer, NULL, 10);
		buffer[0] = time[4]; buffer[1] = time[5]; buffer[2] = '\0';
		t->tm_sec = strtol(buffer, NULL, 10);
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * @brief Checks the NEMA message checksum
 * @returns true: The checksum is correct
 *          false: The checksum is wrong
 */
bool A2235::CheckChecksum(char* buf)
{
	char* end = strchr(buf, '*');
	if (end == NULL) // The begining of the checksum was not found!
	{
		return false;
	}
	uint8_t calculatedChecksum = 0;
	for (int ctr = 0; buf + ctr < end; ctr++) {
		calculatedChecksum ^= buf[ctr];
	}

	uint8_t receivedChecksum = 0;
	char buffer[3] = {0};
	buffer[0] = end[1];
	buffer[1] = end[2];
	receivedChecksum = strtol(buffer, NULL, 16);

	return receivedChecksum == calculatedChecksum;
}