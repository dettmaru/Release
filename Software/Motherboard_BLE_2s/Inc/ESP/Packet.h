/*
 * Packet.h
 *
 *  Created on: Okt 1, 2018
 *      Author: Daniel Lohmann
 */
#ifndef ESP_PACKET_H
#define ESP_PACKET_H

#include <stdint.h>
#include "PacketIdentifier.h"
#include "SensorDataSet.h"
#include "Configuration.h"

const uint16_t ConfigurationNone = 0xFFFF;

/**
 * @brief Describes a Packet which is exchanged between the STM and the ESP
 */
class Packet {
public:
	Packet(uint16_t size);
	Packet(PacketIdentifier_t identifier);
	Packet(PacketIdentifier_t identifier, uint8_t* data, uint16_t size);
	Packet(const SensorDataSet* dataSet);
	Packet(PacketIdentifier_t identifier, BoolOption configurationOption);
	Packet(PacketIdentifier_t identifier, NumericOption configurationOption);
	Packet(PacketIdentifier_t identifier, StringOption configurationOption);
    ~Packet();
	uint16_t GetByteCount(); 
    uint8_t* GetPointer(uint16_t csbc, uint8_t* ptrlsbc);
	PacketIdentifier_t GetIdentifier();
	uint8_t* GetDataPointer();
	uint16_t GetDataLength();
	void Load();
	void Free();
private:
	uint16_t size;
	uint8_t* data;
	const SensorDataSet* dataSet;
	uint16_t configurationOption;
};

#endif // ESP_PACKET_H
