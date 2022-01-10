/*
 * TimeHelper.h
 *
 *  Created on: Jun 21, 2018
 *      Author: Daniel Lohmann
 */

#ifndef CONFIGURATION_TIME_H_
#define CONFIGURATION_TIME_H_

#include <ctime>

#define TIME_RELATIVE_YEAR 2000
#define TIME_STRUCT_TM_RELATIVE_YEAR 1900

/**
 * @brief Helper class for time and date operations
 */
class Time
{
public:
	static void Get(struct tm* outTime);
	static time_t Get();

	static void Set(time_t inTime);
	static void Set(struct tm* inTime);

	static bool IsInitialized();

	static time_t timegm_uc(const struct tm *tm);
};


#endif /* CONFIGURATION_TIME_H_ */
