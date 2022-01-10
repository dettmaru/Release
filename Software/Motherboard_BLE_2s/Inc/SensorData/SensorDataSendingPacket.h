/*
 * SensorDataSendingPacket.h
 *
 *  Created on: April 15, 2019
 *      Author: Daniel Lohmann
 */
#ifndef __SENSORDATASENDINGPACKET_H_
#define __SENSORDATASENDINGPACKET_H_

#include "stdint.h"

class SensorDataSendingPacket
{
public:
	SensorDataSendingPacket(uint16_t CacheIndex);
	uint16_t CacheIndex;
	bool NB_IoT;
	bool LoRa;
	bool WLAN;
	bool SDCard;
};

#endif /* __SENSORDATASENDINGPACKET_H_ */
