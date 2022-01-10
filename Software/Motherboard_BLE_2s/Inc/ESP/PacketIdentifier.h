/*
 * PacketIdentifier.h
 *
 *  Created on: Okt 1, 2018
 *      Author: Daniel Lohmann
 */
#ifndef ESP_PACKETIDENTIFER_H
#define ESP_PACKETIDENTIFER_H

enum class PacketIdentifier_t {
	MQTTServerURL = 1,
	MQTTTopic = 2,
	MQTTUsername = 3,
	MQTTPassword = 4,
	MQTTIssuerCertificate = 5,
	NTPServerURL = 6,
	WLANSSID = 7,
	WLANPassword = 8,
	SensorData = 9,
	MQTTClientId = 10,
	WLAN_DHCP_Ready = 100,
	TurnOff = 101,
	Timestamp = 102,
	Configuration = 103,
	SensorDataACK = 104,
	Unknown = 0xFFFF
};

#endif // ESP_PACKETIDENTIFIER_H
