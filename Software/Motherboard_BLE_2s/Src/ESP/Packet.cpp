/*
 * Packet.cpp
 * 
 *  Created on: Okt 1, 2018
 *      Author: Daniel Lohmann
 */
#include "Packet.h"
#include <cstring>
#include "Configuration.h"

const int CONF_STRING = 0x8000;
const int CONF_NUMERIC = 0x4000;
const int CONF_BOOL = 0xC000;
const int CONF_VALUE = 0x3FFF;

/**
 * @brief Construct a new Packet object, which has no inner data. These 
*         packets are used for simple yes/no information
 * 
 * @param identifier Identifer for this Packet
 */
Packet::Packet(PacketIdentifier_t identifier)
{
    this->configurationOption = ConfigurationNone;
    this->dataSet = nullptr;
    this->size = 2;
    this->data = new uint8_t[2];
    this->data[0] = ((uint16_t)identifier) >> 8;
    this->data[1] = ((uint16_t)identifier) & 0xFF;
    return;
}

/**
 * @brief Construct a new Packet object, which has a defined size. This 
 *        is used for packet receiving!
 * 
 * @param size Amount of bytes in this packet. These amount of bytes could be 
 *             written through the content pointer
 */
Packet::Packet(uint16_t size)
{
	this->configurationOption = ConfigurationNone;
	this->dataSet = nullptr;

    this->size = size;
    this->data = new uint8_t[this->size];
    if (size >= 2)
    {
        this->data[0] = 0xFF;
        this->data[1] = 0xFF;
    }
}

/**
 * @brief Construct a new Packet object, with a given data content
 * 
 * @param identifier Identifier for the packet
 * @param data[in] Pointer to the data  
 * @param size Size of data in bytes
 */
Packet::Packet(
    PacketIdentifier_t identifier, 
    uint8_t* data, 
    uint16_t size)
{
    this->configurationOption = ConfigurationNone;
    this->dataSet = nullptr;

    this->size = size + 2;
    this->data = new uint8_t[this->size];
    this->data[0] = ((uint16_t)identifier) >> 8;
    this->data[1] = ((uint16_t)identifier) & 0xFF;
    if (size > 0)
    {
        memcpy(this->data + 2, data, size);
    }
    return;
}

/**
 * @brief Construct a new Packet object from a SensorDataSet. 
 *        The memory for the content will be later allocated by Load()
 * 
 * @param dataSet[in] Pointer to the SensorDataSet
 */
Packet::Packet(const SensorDataSet* dataSet)
{   
    this->configurationOption = ConfigurationNone;
    this->dataSet = dataSet;
    this->size = 0;
    this->data = nullptr;
}

/**
 * @brief Construct a new Packet object
 * 
 * @param identifier 
 * @param configurationOption 
 */
Packet::Packet(PacketIdentifier_t identifier, BoolOption configurationOption)
{
    this->configurationOption = (int)configurationOption | CONF_BOOL;
    this->dataSet = nullptr;

	this->size = 2;
	this->data = new uint8_t[2]{
		(uint8_t)(((uint16_t)identifier) >> 8),
		(uint8_t)(((uint16_t)identifier) & 0xFF)
	};
}

/**
 * @brief Construct a new Packet object
 * 
 * @param identifier 
 * @param configurationOption 
 */
Packet::Packet(PacketIdentifier_t identifier, NumericOption configurationOption)
{
    this->configurationOption = (int)configurationOption | CONF_NUMERIC;
    this->dataSet = nullptr;

	this->size = 2;
	this->data = new uint8_t[2]{
		(uint8_t)(((uint16_t)identifier) >> 8),
		(uint8_t)(((uint16_t)identifier) & 0xFF)
	};
}

/**
 * @brief Construct a new Packet object
 * 
 * @param identifier 
 * @param configurationOption 
 */
Packet::Packet(PacketIdentifier_t identifier, StringOption configurationOption)
{
    this->configurationOption = (int)configurationOption | CONF_STRING;
    this->dataSet = nullptr;

	this->size = 2;
	this->data = new uint8_t[2]{
		(uint8_t)(((uint16_t)identifier) >> 8),
		(uint8_t)(((uint16_t)identifier) & 0xFF)
	};
}

/**
 * @brief Destroy the Packet object
 */
Packet::~Packet()
{
    if (data != nullptr)
    {
        delete[] data;
    }
}

/**
 * @brief Loads the SensorDataSet or Configuration option into the memory, so 
 *        that a ready to send memory block is created.
 * 
 */
void Packet::Load()
{
    //if (this->data == nullptr)
    {
        if (configurationOption != ConfigurationNone)
        {
        	PacketIdentifier_t identifier = GetIdentifier();
			if (this->data != nullptr)
			{
				delete this->data;
			}

        	uint16_t size = 0;
        	uint8_t* data = nullptr;
            if ((configurationOption & CONF_BOOL) == CONF_BOOL)
            {
                bool data = Configuration::GetBool((BoolOption)(configurationOption & CONF_VALUE));
                this->size = 3;
                this->data = new uint8_t[this->size];
                this->data[0] = ((uint16_t)identifier) >> 8;
                this->data[1] = ((uint16_t)identifier) & 0xFF;
                this->data[2] = data ? 0xFFFFFF : 0;
            }
            else if ((configurationOption & CONF_STRING) == CONF_STRING)
            {
                data = Configuration::GetString((StringOption)(configurationOption & CONF_VALUE), &size);
                this->size = size + 3;
                this->data = new uint8_t[this->size];
                this->data[0] = ((uint16_t)identifier) >> 8;
                this->data[1] = ((uint16_t)identifier) & 0xFF;
                memcpy(this->data + 2, data, size + 1);
            }
            else if ((configurationOption & CONF_NUMERIC) == CONF_NUMERIC)
            {
                uint32_t data = Configuration::GetNumber((NumericOption)(configurationOption & CONF_VALUE));
                this->size = 3;
                this->data = new uint8_t[this->size];
                this->data[0] = ((uint16_t)identifier) >> 8;
                this->data[1] = ((uint16_t)identifier) & 0xFF;
                this->data[2] = data;
            }
            else
            {
                this->data = new uint8_t[2];
                this->data[0] = ((uint16_t)identifier) >> 8;
                this->data[1] = ((uint16_t)identifier) & 0xFF;
                this->size = 2;
            }

            if (data != nullptr)
            {
            	delete data;
            }
        }
        else if (dataSet != nullptr)
        {
            uint16_t size = 0;
            uint8_t* data = dataSet->GetMQTTSensorDataContent(&size);

            this->size = size + 2;
            this->data = new uint8_t[this->size];
            this->data[0] = ((uint16_t)PacketIdentifier_t::SensorData) >> 8;
            this->data[1] = ((uint16_t)PacketIdentifier_t::SensorData) & 0xFF;
            memcpy(this->data + 2, data, size);
            delete data;
        }
    }
}

/**
 * @brief Frees allocated memory in case of SensorDataSet packet or 
 *        Configuration packet
 */
void Packet::Free()
{
    if (this->data != nullptr)
    {
        if (configurationOption != ConfigurationNone ||
            dataSet != nullptr)
        {
            delete this->data;
            this->data = nullptr;
        }
    }
}

/**
 * @brief Returns the count of content bytes
 */
uint16_t Packet::GetByteCount()
{
    return this->size;
}

/**
 * @brief Returns a pointer to the content that has the offset csbc. 
 *        The length of the remaining content, limited to 128, is also returned.
 * 
 * @param csbc Count of already send bytes
 * @param ptrlsbc[out] Count of remaining bytes, limited to 128
 * @return uint8_t* Pointer to the content with the given offset
 */
uint8_t* Packet::GetPointer(uint16_t csbc, uint8_t* ptrlsbc)
{
    uint16_t lsbc = (GetByteCount() - csbc > 128) ? 128 : GetByteCount() - csbc;
    *ptrlsbc = lsbc;
    return this->data + csbc;
}

/**
 * @brief Gets the identifier of the packet
 */
PacketIdentifier_t Packet::GetIdentifier()
{
    if (this->data != nullptr)
    {
        return (PacketIdentifier_t)(this->data[0] << 8 | this->data[1]);
    }
    else 
    {
        return PacketIdentifier_t::Unknown;
    }
}

/**
 * @brief Returns the length of the data in the packet
 */
uint16_t Packet::GetDataLength()
{
    return this->size - 2;
}

/**
 * @brief Returns a pointer to the data of the packet
 */
uint8_t* Packet::GetDataPointer()
{
    return this->data + 2;
}
