/*
 * TimeHelper.cpp
 *
 *  Created on: Jun 21, 2018
 *      Author: Daniel Lohmann
 */
#include "TimeHelper.h"
#include "rtc.h"
#include "StandbyRegister.h"

/**
 * @brief Returns the current time of the RTC
 * 
 * @param[out] outTime current time of the RTC
 */
void Time::Get(struct tm* outTime)
{
	RTC_DateTypeDef date;
	RTC_TimeTypeDef time;
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	outTime->tm_hour = time.Hours;
	outTime->tm_min = time.Minutes;
	outTime->tm_sec = time.Seconds;

	outTime->tm_mday = date.Date;
	outTime->tm_mon = date.Month - 1;
	outTime->tm_year = date.Year + (TIME_RELATIVE_YEAR - TIME_STRUCT_TM_RELATIVE_YEAR);
}

/**
 * @brief Returns the current time of the RTC
 * 
 * @returns Current time of the RTC
 */
time_t Time::Get()
{
	struct tm time;
	Get(&time);
	time_t seconds = timegm_uc(&time);
	return seconds;
}

/**
 * @brief Sets the current time of the RTC
 */
void Time::Set(struct tm* inTime)
{
	time_t t = timegm_uc(inTime);
	Set(t);
}

/**
 * @brief Sets the current time of the RTC
 */
void Time::Set(time_t inTime)
{
	struct tm* inTimes = gmtime(&inTime);
	RTC_DateTypeDef date;
	RTC_TimeTypeDef time;

	date.Date = inTimes->tm_mday;
	date.Month = inTimes->tm_mon + 1;
	date.Year = inTimes->tm_year + (TIME_STRUCT_TM_RELATIVE_YEAR - TIME_RELATIVE_YEAR);
	date.WeekDay = inTimes->tm_wday + 1;

	time.Hours = inTimes->tm_hour;
	time.Minutes = inTimes->tm_min;
	time.Seconds = inTimes->tm_sec;

	time.TimeFormat = RTC_HOURFORMAT_24;
	time.SubSeconds = 0;
	time.SecondFraction = 0;
	time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	time.StoreOperation = RTC_STOREOPERATION_SET;

	HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);

	StandbyRegister::SetTimeInitialized();
}

/**
 * @brief Returns whether the time has been initialized or not
 * @return If time has been initialized true otherwise false
 */
bool Time::IsInitialized()
{
	volatile bool r = StandbyRegister::GetTimeInitialized();
	return r;
}

/* This is a fix for time problem, with mktime function, because mktime depends on the local time */
time_t Time::timegm_uc(const struct tm *tm)
{
    // Month-to-day offset for non-leap-years.
    static const int month_day[12] =
    {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    // Most of the calculation is easy; leap years are the main difficulty.
    volatile int month = tm->tm_mon % 12;
    volatile int year = tm->tm_year + tm->tm_mon / 12;
    if (month < 0) {   // Negative values % 12 are still negative.
        month += 12;
        --year;
    }

    // This is the number of Februaries since 1900.
    volatile const int year_for_leap = (month > 1) ? year + 1 : year;

    volatile time_t rt = tm->tm_sec 			// Seconds
        + 60 * (tm->tm_min                          // Minute = 60 seconds
        + 60 * (tm->tm_hour                         // Hour = 60 minutes
        + 24 * (month_day[month] + tm->tm_mday - 1  // Day = 24 hours
        + 365 * (year - 70)                         // Year = 365 days
        + (year_for_leap - 69) / 4                  // Every 4 years is     leap...
        - (year_for_leap - 1) / 100                 // Except centuries...
        + (year_for_leap + 299) / 400)));           // Except 400s.
	
    return rt < 0 ? -1 : rt;
}
