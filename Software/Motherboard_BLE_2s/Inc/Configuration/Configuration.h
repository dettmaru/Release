/*
 * Configuration.h
 *
 *  Created on: Apr 30, 2018
 *      Author: Daniel Lohmann
 */

#ifndef CONFIGURATION_CONFIGURATION_H_
#define CONFIGURATION_CONFIGURATION_H_

#include "main.h"
#include <stdint.h>
#include <list>
#include <map>
#include "MemoryAddress.h"

/**
 * @brief An interval is a number which is a multiple of one second. 
 */
typedef uint32_t Interval_t;

typedef enum  {
	ComStd_None = 0x0000,
	ComStd_WLAN = 0x0001,
	ComStd_LoRa = 0x0002,
	ComStd_NBIoT = 0x0004
} CommunicationStandard_t;

typedef enum {
	SDCardOp_None = 0,
	SDCardOp_DirectSave = 1,
	SDCardOp_ShortCaching = 2
} SDCardOperation_T;

uint64_t ReadMemory(uint32_t address);

/**
 * @brief Describes a transaction into the Flask memory. 
 * 
 * It holds the data for a transaction of long length. 
 */
class BlockTransaction
{
public:
	/** 
	 * @brief Destination address in Flask memory 
	 */
	uint32_t Address;
	/**
	 * @brief Pointer to data which is associated with this transaction
	 */
	uint8_t* Data;
	/**
	 * @brief Length of the data (in bytes) which is associated with this transaction.
	 */
	uint32_t Length;
};

class Packet;

/**
 * @brief Controls the saving and reading of configuration settings.
 */
class Configuration {
public:
	static bool Init();

	static bool GetBool(BoolOption option);
	static uint32_t GetNumber(NumericOption option);
	static uint8_t* GetString(StringOption option, uint16_t* length);
	static Interval_t GetInterval(Intervals interval);

	static bool SetBool(BoolOption option, bool value);
	static bool SetNumber(NumericOption option, uint32_t value);
	static bool SetString(StringOption option, uint8_t* value, uint16_t length);
	static bool SetInterval(Intervals interval, Interval_t value);

	static void Unlock();
	static void Lock();

	/* Methods that return the values for the execution*/
	static uint32_t GetRTCPeriod();

	static bool SaveConfiguration();
	static HAL_StatusTypeDef WriteMemoryPage(uint32_t address, uint64_t* data);
	static volatile uint32_t toggle_ESP_LED;
#ifdef TESTING
public:
#else
private:
#endif
	static bool needToWrite;

	static std::map<uint32_t, uint64_t> pendingSmallTransactions;
	static std::list<BlockTransaction> pendingBlockTransactions;
	static uint32_t pendingBlockTransactionLengthSum;

	static void ReadMemoryPage(uint32_t address, uint64_t* data);

	static uint32_t GetPage(uint32_t addr);
	static uint32_t GetBank(uint32_t addr);
	static uint64_t ReadMemory(uint32_t address);
	static void CheckLastFLASHErrorCode();
	static bool WriteMemory(uint32_t address, uint64_t data);
	static bool WriteTransactions(bool onlyBlock = false);
	static bool WriteTransactionDataBlock(uint32_t address, uint32_t ptrBlock, uint8_t* data, uint16_t length);

	static bool FindFreeBlock(uint32_t startAddress, uint32_t* address, uint16_t* length);
	static bool OtherBlockInRange(uint32_t addressStart, uint32_t length, uint32_t* next);

	static uint8_t* ReadDataBlock(uint32_t address, uint16_t* length);
	static bool WriteDataBlock(uint32_t address, uint8_t* data, uint16_t length);

	static bool ReadBoolean(uint32_t address, uint8_t bit);
	static bool WriteBoolean(uint32_t address, uint8_t bit, bool value);

friend class Packet;
};

/**
 * @brief Constant which identifies the initial configuration variable state
 *        when the initial configuration changes during development this variable
 *        will be changed to run an initial configuration at startup
 */
#define INITIAL_CONFIG_DONE	3897530

#define RTC_TICKS	32768

#endif /* CONFIGURATION_CONFIGURATION_H_ */
