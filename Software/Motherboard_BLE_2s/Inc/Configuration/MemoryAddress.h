/*
 * MemoryAddress.h
 *
 *  Created on: Aug 6, 2018
 *      Author: Daniel Lohmann
 *
 * This header contains a list of memory addresses which are used by the
 * configuration options
 */

#ifndef CONFIGURATION_MEMORYADDRESS_H_
#define CONFIGURATION_MEMORYADDRESS_H_

#include "Intervals.h"
#include "BooleanConfigurationOptions.h"
#include "NumericConfigurationOptions.h"
#include "StringDataConfigurationOptions.h"

#define GOTOSTANDBY_MEMADDR				0
#define SYNCTIMEWITHNTPSERVER_MEMADDR	1
#define	DIRECTSAVETOSDCARD_MEMADDR		2
#define CHECK_INITIAL_MEMADDR           3

#define BOOLOPTION_START                4
#define BOOLOPTION_END                  (BOOLOPTION_START + (int)(BoolOption::END))

#define NUMERICOPTION_START             (BOOLOPTION_END)
#define NUMERICOPTION_END               (NUMERICOPTION_START + (int)(NumericOption::END))

#define STRINGDATAOPTION_START          (NUMERICOPTION_END)
#define STRINGDATAOPTION_END            (STRINGDATAOPTION_START + (int)(StringOption::END))

#define INTERVALS_START                 (STRINGDATAOPTION_END)
#define INTERVALS_END                   (INTERVALS_START + (int)(Intervals::END))

#define CONF_MEM_PTRDYNAMIC (STRINGDATAOPTION_START)	    //Start of the space where pointers to dynamic sized memory are located
#define CONF_MEM_PTREND		(STRINGDATAOPTION_END)	        //End of the space where pointers to dynamic sized memory are located

// Base flash memory address, where the configuration memory address space starts
// Bank 2 of flash memory is here used
#define BASEADDRESS 0x08080000U


#define CONF_MEM_DYNAMIC 	2048 	//Start of the space where dynamic sized configurations are located
#define CONF_MEM_END 		0xFFFF	//End of the space where configurations are located

#endif /* CONFIGURATION_MEMORYADDRESS_H_ */
