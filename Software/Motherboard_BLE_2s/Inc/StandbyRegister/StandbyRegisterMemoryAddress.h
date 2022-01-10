/*
 * StandbyRegiterMemoryAddress.h
 *
 *  Created on: Aug 15, 2018
 *      Author: Daniel Lohmann
 */
#ifndef STANDBYREGISTERMEMORYADDRESS_H
#define STANDBYREGISTERMEMORYADDRESS_H

#include "Intervals.h"
#include "rtc.h"

#define STBYREG_INTERVALS_START (RTC_BKP_DR1)
#define STBYREG_INTERVALS_END   (STBYREG_INTERVALS_START + (uint32_t)Intervals::END)
#define STBYREG_TIMEINITIAL  	(STBYREG_INTERVALS_END)
#define STBYREG_LEDSTATUS 		(STBYREG_TIMEINITIAL + 1)


#endif /* STANDBYREGISTERMEMORYADDRESS_H */
