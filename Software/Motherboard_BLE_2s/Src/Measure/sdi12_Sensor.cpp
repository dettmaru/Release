/*
 * sdi12_Sensor.cpp
 *
 *  Created on: Apr 23, 2019
 *      Author: Daniel Lohmann
 */
#include "sdi12_Sensor.h"
#include "stdlib.h"

SDI12_Sensor::SDI12_Sensor(uint8_t address, uint32_t sensorType)
{
	this->address = address;
	this->lastCommand = nullptr;
	this->readingInProgress = false;
	this->waitBegin = 0;
	this->waitTime = 0;
	this->waitingForResponse = false;
	this->state = SDI12_SensorState::OFF;
	this->sensorType = sensorType;
}

SDI12_Sensor::SDI12_Sensor() :
	SDI12_Sensor('0', 0)
{

}

SDI12_Sensor::~SDI12_Sensor()
{
	for (auto string : lastResponses)
	{
		free(string);
	}
	lastResponses.clear();
}



