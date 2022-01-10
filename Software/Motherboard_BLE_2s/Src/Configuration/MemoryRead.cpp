/*
 * MemoryRead.cpp
 *
 *  Created on: Aug 8, 2018
 *      Author: Daniel Lohmann
 */
#include <stdint.h>
#include "Configuration.h"

uint64_t ReadMemory(uint32_t address)
{
	volatile uint64_t value = *((volatile uint64_t*)address);
	return value;
}


