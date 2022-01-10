/*
 * SensorDataSendingPacket.cpp
 *
 *  Created on: Apr 15, 2019
 *      Author: Daniel Lohmann
 */
#include "SensorDataSendingPacket.h"

SensorDataSendingPacket::SensorDataSendingPacket(uint16_t cacheIndex)
	: CacheIndex(cacheIndex),
	  NB_IoT(false),
	  LoRa(false),
	  WLAN(false),
	  SDCard(false)
{

}

