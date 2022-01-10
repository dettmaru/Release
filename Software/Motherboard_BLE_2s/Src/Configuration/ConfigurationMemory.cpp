/*
 * ConfigurationMemory.cpp
 *
 *  Created on: Jul 30, 2018
 *      Author: Daniel Lohmann
 */

#include <Configuration.h>
#include "Logging.h"
#include <main.h>
#include <MemoryAddress.h>
#include <stdint.h>
#include <memory.h>
#include <map>

#include <iostream> //TODO remove

#define TRANSACTION_LENGTH_MAX	4096
#define TAG_CONFIG "ConfigMem"
#define DEBUG_CONFIG_MEMORY true

std::map<uint32_t, uint64_t> Configuration::pendingSmallTransactions;
std::list<BlockTransaction> Configuration::pendingBlockTransactions;
uint32_t Configuration::pendingBlockTransactionLengthSum = 0;



/**
  * @brief  Gets the page of a given address.
  * @param[in]  Addr: Address of the FLASH Memory. The address is in the micro 
  * 			controller addressing space
  * @retval The page of a given address
  */
uint32_t Configuration::GetPage(uint32_t Addr)
{
  uint32_t page = 0;

  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }

  return page;
}

/**
  * @brief  Gets the bank of a given address
  * @param[in]  Addr: Address of the FLASH Memory. The address is in the micro 
  * 			controller addressing space
  * @retval The bank of a given address
  */
uint32_t Configuration::GetBank(uint32_t Addr)
{
	uint32_t bank = 0;
	/* More on memory remapping cound be found in the reference manual of the µC */
  	if (READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE) == 0)
  	{
  		/* No Bank swap */
    	if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    	{
      		bank = FLASH_BANK_1;
    	}
    	else
    	{
      		bank = FLASH_BANK_2;
    	}
  	}
  	else
  	{
  		/* Bank swap */
	    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    	{
      		bank = FLASH_BANK_2;
    	}
    	else
    	{
    	  	bank = FLASH_BANK_1;
    	}
  	}

  	return bank;
}


/**
 * @brief Reads a block of data from the configuration memory. 
 * 
 * @param address 	Address on which the block address is located. This address 
 * 					is in virtual configuration address space.
 * @param length 	Pointer to a variable which will get the length of the data in bytes
 * @return uint8_t*	Pointer to the data. The data is one element longer 
 * 				 	than in length specified, because the last element is 
 * 				 	a zero, which could be helpful, when the output 
 * 					should be used as string.
 * 					When the data is no longer needed delete[] must be called!
 * 					When the value could not be read the nullptr is returned
 */
uint8_t* Configuration::ReadDataBlock(uint32_t address, uint16_t* length)
{
	*length = 0;

	// Read the location of the data block
	uint64_t ptrBlock = ReadMemory(address);  
	if (ptrBlock >= CONF_MEM_DYNAMIC / 8 && ptrBlock < CONF_MEM_END / 8)
	{
		// Read the block length
		uint64_t size = ReadMemory(ptrBlock);
		if (size >= CONF_MEM_END)
		{
			//_Error_Handler(__FILE__, __LINE__);
			return nullptr;
		}
		*length = size;
		uint8_t* data = new uint8_t[size+1];

		uint16_t current = 0;
		uint16_t line = 1;
		while (current < size)
		{
			uint64_t lineValue = ReadMemory(ptrBlock + line);

			for (uint8_t byte = 0; byte < 8; byte++)
			{
				data[current] = (lineValue >> (8*byte)) & 0xFF;
				current++;
				if (!(current < size))
				{
					break;
				}
			}
			line++;
		}
		data[size] = '\0';

		return data;
	}
	else
	{
		return nullptr;
	}
}

/**
 * @brief Reads a boolean value from the memory
 * 
 * @param address 	Memory address on which the value was saved. The address is
 * 					in the virtual configuration address space.
 * @param bit 		Bit in the value which should be read
 * @return 			Readed value of the memory bit
 */
bool Configuration::ReadBoolean(uint32_t address, uint8_t bit)
{
	return (ReadMemory(address) & (1<<bit)) != 0;
}

/**
 * @brief Reads a memory value with 64bits
 * 
 * @param address 	Address from which the value should be read. The address is 
 * 					in the virtual configuration address space
 * @return uint64_t Readed memory value	
 */
uint64_t Configuration::ReadMemory(uint32_t address)
{
	return ::ReadMemory(BASEADDRESS + (address << 3));
}

/**
 * @brief 	Unlocks the writing to the configuration
 */
void Configuration::Unlock()
{
	HAL_FLASH_Unlock();
}

/**
 * @brief 	Locks the writing to the configuration
 */
void Configuration::Lock()
{
	HAL_FLASH_Lock();
}

/**
 * @brief Finds a free block of memory beginning with the given startAddress.
 * 
 * @param startAddress 	Address where to start the search.
 * @param address 		Address where a free block was found.
 * @param length 		Length of the found memory block
 * @return true 		A memory block could be found
 * @return false 		No free memory block could be found
 */
bool Configuration::FindFreeBlock(uint32_t startAddress, uint32_t* address, uint16_t* length)
{
	uint32_t currentAddress = startAddress;
	uint32_t currentLength = CONF_MEM_END - startAddress;
	uint32_t lastRoundAddress;

	// Create a list of all dynamic sized blocks
	std::map<uint32_t, uint32_t> blocks;
	for (uint32_t ptrI = CONF_MEM_PTRDYNAMIC; ptrI < CONF_MEM_PTREND; ptrI++)
	{
		// Read the pointer
		uint32_t ptrBlock = 0;
		std::map<uint32_t, uint64_t>::iterator smallTransaction = pendingSmallTransactions.find(ptrI);
		if (smallTransaction != pendingSmallTransactions.end())
		{
			ptrBlock = smallTransaction->second;
		}
		else
		{
			ptrBlock = ReadMemory(ptrI);
		}
		if (ptrBlock < CONF_MEM_END / 8 && ptrBlock >= CONF_MEM_DYNAMIC / 8)
		{
			uint32_t blockSize = ReadMemory(ptrBlock);
			blocks.insert({ ptrBlock, blockSize });
		}
	}

	// Find the next free start address
	do
	{
		lastRoundAddress = currentAddress;

		for (auto block : blocks)
		{
			auto ptrBlock = block.first*8;
			auto blockSize = block.second;
			if (ptrBlock <= currentAddress && ptrBlock + blockSize + 8 >= currentAddress)
			{
				// On line Size + go to the next line
				currentAddress = ptrBlock + blockSize + 8 + 8;
			}
		}
	} while (lastRoundAddress != currentAddress);

	// A free start position was found. Now find the length of the free space
	for (auto block : blocks)
	{
		auto ptrBlock = block.first*8;

		if (ptrBlock > currentAddress && ptrBlock <= currentAddress + currentLength)
		{
			currentLength = ptrBlock - currentAddress;
		}
	}

	*address = currentAddress;
	*length = currentLength;

	return true;
}

/**
 * @brief Checks whether the range is blocked by an other block.
 *
 * @param startAddress Address of the option to start the search from
 * @param length 	   Length of the block which should be found
 * @param next 		   Pointer to variable which will get the address after the block,
 * 					   that was found to be blocking, but this address may also be blocked
 * 					   by another block, because only the first block in this range will be used
 * @return true 	   When an other block is in this range
 * @return false 	   When no other block is in this range, then next will be not set
 */
bool Configuration::OtherBlockInRange(uint32_t startAddress, uint32_t length, uint32_t* next)
{
	uint32_t begin = startAddress;
	uint32_t end = startAddress + length;
	bool blocked = false;
	for (uint32_t ptrI = CONF_MEM_PTRDYNAMIC; ptrI < CONF_MEM_PTREND; ptrI++)
	{
		uint32_t ptrBlock = ReadMemory(ptrI);
		if (ptrBlock < CONF_MEM_END && ptrBlock >= CONF_MEM_DYNAMIC)
		{
			uint64_t size = ReadMemory(ptrBlock);
			if (size < CONF_MEM_END)
			{
				// Start of block is in the range
				if (ptrBlock >= begin && ptrBlock < end)
				{
					blocked = true;
				}

				// End of block is in the range
				if (ptrBlock+size >= begin && ptrBlock+size < end)
				{
					blocked = true;
				}

				// Block starts before begin and end behind end
				if (ptrBlock < begin && ptrBlock+size >= end)
				{
					blocked = true;
				}
			}
			if (blocked)
			{
				*next = ptrBlock + 1 + size;
				break;
			}
		}
	}
	return blocked;
}

/**
 * @brief Writes a block of data in the configuration memory
 *
 * @param[in] address 	Address on which the block address will be written
 * @param[in] ptrBlock	Pointer to the start of the block where it really is written.
 * 						The address is in virtual configuration memory address spacing
 * @param[in] data 		Pointer to the data, giving a nullptr deletes the memory block (destroys reference)
 * @param[in] length 	Length of the data in bytes
 * @return true 	Write was successful
 * @return false 	Write has failed
 */
bool Configuration::WriteTransactionDataBlock(uint32_t address, uint32_t ptrBlock, uint8_t* data, uint16_t length)
{
	if (data != nullptr)
	{
		if (WriteMemory(address, ptrBlock) == false)
		{
			return false;
		}
		if (WriteMemory(ptrBlock, length) == false)
		{
			return false;
		}
		ptrBlock++;

		for (uint16_t i = 0; i < length; i+=8)
		{
			uint64_t line = 0;
			for (uint8_t j = 0; j < 8; j++)
			{
				line |= (uint64_t)data[i+j] << j*8;
			}

			if (WriteMemory(ptrBlock, line) == false)
			{
				return false;
			}

			ptrBlock++;
		}
		return true;
	}
	else
	{
		// Write default value into the memory address
		if (WriteMemory(address, 0xFFFFFFFFFFFFFFFF) != HAL_OK)
		{
			return false;
		}
		return true;
	}
}



/**
 * @brief Writes a boolean value to the memory
 * 
 * @param address 	Memory address on which the value should be saved
 * @param bit 		Bit in the value which should be as storage
 * @param value 	Value to write
 * @return true 	Write was successful
 * @return false 	Write has failed
 */
bool Configuration::WriteBoolean(uint32_t address, uint8_t bit, bool value)
{
	uint64_t oldvalue = ReadMemory(address);
	if (value)
	{
		oldvalue |= (1<<bit);
	}
	else
	{
		oldvalue &= ~(1<<bit);
	}
	
	auto element = pendingSmallTransactions.insert({address, oldvalue});
	if (element.second == false) 
	{
		// Alreay an element for this address is in the list
		// So change the correstponding bit there
		uint32_t currentValue = element.first->second;
		if (value)
		{
			currentValue |= (1<<bit);
		}
		else 
		{
			currentValue &= ~(1<<bit);
		}
		element.first->second = currentValue;
	}

	return true;
}

/**
 * @brief Writes a 64bit value to the configuration memory. This operation is 
 * 		  buffered, a call to SaveConfiguration is required to flush the 
 * 		  configuration to the memory
 * 
 * @param[in] address 	Address on which the address should be written. This is 
 * 						a virtual configuration memory address.
 * @param[in] value 	Value which should be written
 * @return true 		Transaction was successfully buffered
 * @return false 		Transaction could not be buffered/an error occured
 */
bool Configuration::WriteMemory(uint32_t address, uint64_t value)
{
	uint64_t oldValue = ReadMemory(address);
	// Save memory write cycles
	if (oldValue != value) 
	{
		auto element = pendingSmallTransactions.insert({address, value});
		if (element.second == false) 
		{
			// Already a transaction is in the list for this address
			// => Update it
			element.first->second = value;
		}
	}
	return true;
}

/**
 * @brief Writes a block of data in the configuration memory
 *
 * @param[in] address 	Address on which the block address will be written. The 
 * 						address is a virtual configration address. At the 
 * 						address a pointer to the written datablock is saved. An 
 * 						appropriate memory location for the data block will be 
 * 						found and used
 * @param[in] data 		Pointer to the data, giving a nullptr deletes the memory 
 * 						block (destroys reference/pointer)
 * @param[in] length 	Length of the data in bytes. For C-style strings the 
 * 						length without the string end character could be used.
 * @return true 		Write was successful
 * @return false 		Write has failed
 */
bool Configuration::WriteDataBlock(uint32_t address, uint8_t* data, uint16_t length)
{
	uint16_t readLength = 0;
	uint8_t* readData = ReadDataBlock(address, &readLength);
	bool changed = readLength != length || // Change in length is a change of data
		(memcmp(data, readData, length) != 0 && length > 0) ||  // Change of the data is a change
		(data != readData && length == 0); // Change from or to an nullpointer is a change
	delete[] readData;
	
	if (changed)
	{
		BlockTransaction transaction;
		transaction.Address = address;
		transaction.Data = data;
		transaction.Length = length;
		pendingBlockTransactions.push_back(transaction);
		pendingBlockTransactionLengthSum += length;

		if (pendingBlockTransactionLengthSum > TRANSACTION_LENGTH_MAX) 
		{
			return WriteTransactions();
		}
	}
	return true;
}

/**
 * @brief Writes pending transactions into configuration memory
 * 
 * @param[in] onlyBlock 	Writes only block transaction in the configuration 
 * 							memory, this option allows to free the RAM.
 * @return true 		
 * @return false 		
 */
bool Configuration::WriteTransactions(bool onlyBlock)
{
	HAL_StatusTypeDef status = HAL_OK;
	std::list<BlockTransaction>::iterator itr = pendingBlockTransactions.begin();

	while (itr != pendingBlockTransactions.end())
	{
#if DEBUG_CONFIG_MEMORY
	LOGI(TAG_CONFIG, "BlockTransaction: %x write %s with length %x", itr->Address, itr->Data, itr->Length);
#endif

		uint32_t currentAddress = CONF_MEM_DYNAMIC;

		uint32_t nextAddress = 0;
		uint16_t nextLength = 0;

		do {
			currentAddress += nextLength;
			if (FindFreeBlock(currentAddress, &nextAddress, &nextLength) == 0)
			{
				status = HAL_ERROR;
				LOGE(TAG_CONFIG, "No free block found!");
				break;
			}
#if DEBUG_CONFIG_MEMORY
			LOGI(TAG_CONFIG, "Found free block %x with length %x", nextAddress, nextLength);
#endif
			currentAddress = nextAddress;
		} while (nextLength < itr->Length + 8);

#if DEBUG_CONFIG_MEMORY
			LOGI(TAG_CONFIG, "Selected free block %x with length %x", nextAddress, nextLength);
#endif

		if (status != HAL_ERROR)
		{
			uint32_t page = GetPage(BASEADDRESS + currentAddress);
			uint32_t lastPage = GetPage(BASEADDRESS + currentAddress + nextLength);

			uint16_t i = 0;
			for (uint32_t pageI = page; pageI <= lastPage; pageI++)
			{
				uint64_t pageData[FLASH_PAGE_SIZE / sizeof(uint64_t)];
				ReadMemoryPage(pageI * FLASH_PAGE_SIZE, pageData);

				uint32_t startOnPage = 0;
				uint32_t endOnPage = (FLASH_PAGE_SIZE - 1) / 8;
				
				if (pageI == page)
				{
					startOnPage = (currentAddress % FLASH_PAGE_SIZE) / 8;
				}
				if (pageI == lastPage)
				{
					endOnPage = (currentAddress + nextLength % FLASH_PAGE_SIZE) / 8;
				}

				uint32_t ptrBlock = startOnPage; 
				
				while (itr != pendingBlockTransactions.end())
				{
					BlockTransaction& transaction = *itr;
					if (transaction.Data != nullptr)
					{
						if (i == 0)
						{
							if (pendingSmallTransactions.find(transaction.Address) != pendingSmallTransactions.end())
							{
								pendingSmallTransactions.erase(transaction.Address);
							}
							// Enqueue the pointer into small transactions
							pendingSmallTransactions.insert({ transaction.Address, 
								ptrBlock + pageI * FLASH_PAGE_SIZE / 8}); 

							// Write size
							pageData[ptrBlock++] = transaction.Length;
						}
						if (ptrBlock > endOnPage)
						{
							break;
						}

						// Write content of the data block
						for (; i < transaction.Length; i+=8)
						{
							uint64_t line = 0;
							for (uint8_t j = 0; j < 8; j++)
							{
								if (i+j >= transaction.Length)
								{
									// No more to write, stop from accessing
									// invalid memory address
									break; 
								}
								line |= (uint64_t)transaction.Data[i+j] << j*8;
							}
							pageData[ptrBlock++] = line;
							
							if (ptrBlock > endOnPage)
							{
								i+= 8;
								break;
							}
						}

						if (i < transaction.Length)
						{
							ptrBlock = 0;
						}
						else
						{
							startOnPage = ptrBlock;
							i = 0;
						}
					}
					else
					{
						if (pendingSmallTransactions.find(transaction.Address) != pendingSmallTransactions.end())
						{
							pendingSmallTransactions.erase(transaction.Address);
						}
						// Enqueue the pointer into small transactions							
						// The pointer is set to the default FLASH value
						pendingSmallTransactions.insert({ transaction.Address, 
							0xFFFFFFFFFFFFFFFF});
					}

					if (i == 0)
					{
						// Transaction is finished => remove it from the list
						pendingBlockTransactionLengthSum -= itr->Length;
						itr = pendingBlockTransactions.erase(itr);
					}
					else
					{
						break;
					}
				}

				if (WriteMemoryPage(pageI * FLASH_PAGE_SIZE, pageData))
				{
					status = HAL_ERROR;
					LOGE(TAG_CONFIG, "Writing memory page failed");
					break;
				}

				if (pendingBlockTransactions.empty())
				{
					break;
				}
			}
		}
		else
		{
			status = HAL_ERROR;
			LOGE(TAG_CONFIG, "An error has occurred during processing => break execution");
			break;
		}
	}

	for (auto block : pendingBlockTransactions)
	{
		pendingBlockTransactionLengthSum -= block.Length;
	}
	pendingBlockTransactions.clear();

	if (!onlyBlock) // Save also the small transactions?
	{
		uint64_t datablock[FLASH_PAGE_SIZE / sizeof(uint64_t)];

		ReadMemoryPage(0, datablock);

		for (auto const &t : pendingSmallTransactions)
		{
			auto const &address = t.first;
			auto const &value = t.second;

			datablock[address] = value;
		}
		// TODO maybe multiple pages with small transaction should be possible?
		if (WriteMemoryPage(0, datablock) != HAL_OK)
		{
			status = HAL_ERROR;
		}

		pendingSmallTransactions.clear();
	}
	return status == HAL_OK;
}

/**
 * @brief Writes all configuration options into memory. Some may already be in 
 * 		  memory, but with this option the remaining options are flushed to the 
 * 		  memory
 * 
 * @return true 	Configuration was successfully written to memory
 * @return false 	Saving of configuration had a problem
 */
bool Configuration::SaveConfiguration()
{
	return WriteTransactions();
}

/**
 * @brief Reads a complete page of flash memory. Length of the memory block is 
 * 		  FLASH_PAGE_SIZE. The array is then FLASH_PAGE_SIZE / sizeof(uint64_t)
 * 
 * @param[in] address 	Beginning address of the page. The address is in the 
 * 						virtual configuration address space.
 * @param[out] data 	Pointer to a FLASH_PAGE_SIZE / sizeof(uint64_t) sized 
 * 						memory block in which the contents of the memory page 
 * 						are written
 */
void Configuration::ReadMemoryPage(uint32_t address, uint64_t* data)
{
	int ptr = address;
	for (int i = 0; i < (int)(FLASH_PAGE_SIZE / sizeof(uint64_t)); i++)
	{
		data[i] = ::ReadMemory(BASEADDRESS + ptr);
		ptr += 8;
	}
	return;
}

void Configuration::CheckLastFLASHErrorCode()
{
	uint32_t flashErrorCode = HAL_FLASH_GetError();
	LOGI(TAG_CONFIG, "FLASH Error code = %x", flashErrorCode);
	if (flashErrorCode & HAL_FLASH_ERROR_RD)   { LOGIE(TAG_CONFIG, "HAL_FLASH_ERROR_RD: FLASH Read Protection error flag (PCROP)"); }
	if (flashErrorCode & HAL_FLASH_ERROR_PGS)  { LOGIE(TAG_CONFIG, "HAL_FLASH_ERROR_PGS: FLASH Programming Sequence error flag"); }
	if (flashErrorCode & HAL_FLASH_ERROR_PGA)  { LOGIE(TAG_CONFIG, "HAL_FLASH_ERROR_PGA: FLASH Programming Alignment error flag"); }
	if (flashErrorCode & HAL_FLASH_ERROR_WRP)  { LOGIE(TAG_CONFIG, "HAL_FLASH_ERROR_WRP: FLASH Write protected error flag"); }
	if (flashErrorCode & HAL_FLASH_ERROR_NONE) { LOGI (TAG_CONFIG, "HAL_FLASH_ERROR_NONE: No error set"); }
	if (flashErrorCode & HAL_FLASH_ERROR_PROG) { LOGIE(TAG_CONFIG, "HAL_FLASH_ERROR_PROG: FLASH Programming error"); }
	if (flashErrorCode & HAL_FLASH_ERROR_WRP)  { LOGIE(TAG_CONFIG, "HAL_FLASH_ERROR_WRP: FLASH Write protection error"); }
	if (flashErrorCode & HAL_FLASH_ERROR_SIZ)  { LOGIE(TAG_CONFIG, "HAL_FLASH_ERROR_SIZ: FLASH Size error"); }
	if (flashErrorCode & HAL_FLASH_ERROR_MIS)  { LOGIE(TAG_CONFIG, "HAL_FLASH_ERROR_MIS: FLASH Fast programming data miss error"); }
	if (flashErrorCode & HAL_FLASH_ERROR_FAST) { LOGIE(TAG_CONFIG, "HAL_FLASH_ERROR_FAST: FLASH Fast programming error"); }
	if (flashErrorCode & FLASH_FLAG_PEMPTY)    { LOGIE(TAG_CONFIG, "FLASH_FLAG_PEMPTY: FLASH Boot from not programmed flash"); }
#if DEBUG_CONFIG_MEMORY
	if (flashErrorCode == 0)                   { LOGI (TAG_CONFIG, "No FLASH Error!"); }
#endif
	__HAL_FLASH_CLEAR_FLAG(flashErrorCode);
}

/**
 * @brief Writes a memory page. The length of the memory block must be 
 * 		  FLASH_PAGE_SIZE bytes in size. The given array must have  
 * 		  FLASH_PAGE_SIZE / sizeof(uint64_t) elements
 * 
 * @param[in] address 	Beginning address of the page. The address is in the 
 * 					  	virtual configuration address space.
 * @param[in] data 		Pointer to a FLASH_PAGE_SIZE / sizeof(uint64_t) sized 
 * 						memory block in which the contents are, that will be 
 * 						written to the memory
 * @return HAL_StatusTypeDef HAL_OK when write was successful, otherwise HAL_ERROR
 */
HAL_StatusTypeDef Configuration::WriteMemoryPage(uint32_t address, uint64_t* data)
{
	HAL_StatusTypeDef ret = HAL_OK;
	__HAL_FLASH_DATA_CACHE_DISABLE();
	__HAL_FLASH_INSTRUCTION_CACHE_DISABLE();
	__HAL_FLASH_PREFETCH_BUFFER_ENABLE();
	/* Program the user Flash area word by word */

	/* Clear OPTVERR bit set on virgin samples */
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR | FLASH_FLAG_SIZERR);

	// Erase required, before writing to flash memory
	FLASH_EraseInitTypeDef erase;
	erase.Banks = GetBank(BASEADDRESS + address);
	erase.Page = GetPage(BASEADDRESS + address);
	erase.NbPages = 1;
	erase.TypeErase = FLASH_TYPEERASE_PAGES;

	uint32_t pageError = 0;
	CheckLastFLASHErrorCode();
	LOGIW(TAG_CONFIG, "Erase FLASH");
	ret = HAL_FLASHEx_Erase(&erase, &pageError);
	if (ret != HAL_OK)
	{
		if (ret == HAL_ERROR)
		{
			LOGIE(TAG_CONFIG, "Erase FLASH resulted in HAL_ERROR");
		}
		else if (ret == HAL_BUSY)
		{
			LOGIE(TAG_CONFIG, "Erase FLASH resulted in HAL_BUSY");
		}
		else if (ret == HAL_TIMEOUT)
		{
			LOGIE(TAG_CONFIG, "Erase FLASH resulted in HAL_BUSY");
		}
		else
		{
			LOGIE(TAG_CONFIG, "Erase FLASH resulted in error");
		}
		if (pageError == 0)
		{
			LOGIE(TAG_CONFIG, "Erase Failed probably at waiting for waiting for finished operation");
		}
		if (pageError != 0xFFFFFFFF)
		{
			LOGIE(TAG_CONFIG, "Erase Failed at page index: %x", pageError);
		}
		CheckLastFLASHErrorCode();

		// In case of an error pageError could be evaluated
		return HAL_ERROR;
	}

	HAL_StatusTypeDef returnStatus = HAL_OK;

	for (int i = 0; i < (int)(FLASH_PAGE_SIZE / sizeof(uint64_t)); i++)
	{
		// TODO decrease calculation work
		int addr = BASEADDRESS + address + i * 8;
		uint64_t currentData = ReadMemory(address/8 + i);
		//LOGIW(TAG_CONFIG, "Write to FLASH Address: %x CurrentData: %x", addr, currentData);
		ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, data[i]);
		if (ret != HAL_OK)
		{
			if (ret == HAL_ERROR)
			{
				LOGIE(TAG_CONFIG, "Write to FLASH Address resulted in HAL_ERROR: %x", addr);
			}
			else if (ret == HAL_BUSY)
			{
				LOGIE(TAG_CONFIG, "Write to FLASH Address resulted in HAL_BUSY: %x", addr);
			}
			else if (ret == HAL_TIMEOUT)
			{
				LOGIE(TAG_CONFIG, "Write to FLASH Address resulted in HAL_BUSY: %x", addr);
			}
			CheckLastFLASHErrorCode();

			returnStatus = HAL_ERROR;
			break;
		}
	}

	return returnStatus;
}
