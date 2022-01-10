/*
 * Cellular.h
 *
 *  Created on: May 22, 2019
 *      Author: Joschka Wirges, Daniel Lohmann
 */

#ifndef CELLULAR_CELLULAR_H_
#define CELLULAR_CELLULAR_H_

#include "SensorDataSendingPacket.h"
#include <vector>


class Cellular {
public:
	static void InitNBIoT();
	static void SendSensorDataNBIoT(std::vector<SensorDataSendingPacket>* packets);
	static void SendSensorDataGSM(std::vector<SensorDataSendingPacket>* packets);
};


#endif /* CELLULAR_CELLULAR_H_ */
