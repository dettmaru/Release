/*
 * ESP.h
 *
 *  Created on: May 23, 2018
 *      Author: Daniel Lohmann
 */
#ifndef ESP_ESP_H_
#define ESP_ESP_H_

#include <vector>
#include "SensorDataSendingPacket.h"
#include "SensorDataSet.h"

/**
 * @brief Controls and handles the communication with the ESP32
 */
class ESP
{
public:
	static void TurnON();
	static void TurnOFF();
	static void SendSensorData(std::vector<SensorDataSendingPacket>* packets);

	static volatile bool Ready;
private:
	static void ProcessStateMachine();
	static const uint32_t RESPONSE_TIMEOUT;
};

#endif /* ESP_ESP_H_ */
