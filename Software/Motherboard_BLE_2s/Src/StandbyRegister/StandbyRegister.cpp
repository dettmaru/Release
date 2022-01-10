/*
 * StandbyRegister.cpp
 *
 *  Created on: Sep 13, 2018
 *      Author: Daniel Lohmann
 */
#include "StandbyRegister.h"

void StandbyRegister::SetLEDStatus(bool status)
{
	HAL_RTCEx_BKUPWrite(&hrtc, STBYREG_LEDSTATUS, status == true ? 0xFFFF :  0);
	return;
}

bool StandbyRegister::GetLEDStatus()
{
	return HAL_RTCEx_BKUPRead(&hrtc, STBYREG_LEDSTATUS) == 0xFFFF;
}

/**
 * @brief Returns the current value of the given interval counter
 */
Interval_t StandbyRegister::GetIntervalCounter(Intervals interval)
{
    uint32_t reg = GetIntervalRegister(interval);
    uint32_t value = HAL_RTCEx_BKUPRead(&hrtc, reg);
    return (Interval_t)value;
}

/**
 * @brief Increments the current value of the given interval counter
 */
void StandbyRegister::IncrementIntervalCounter(Intervals interval)
{
    uint32_t reg = GetIntervalRegister(interval);
    uint32_t value = HAL_RTCEx_BKUPRead(&hrtc, reg);
    value += Configuration::GetRTCPeriod();
    HAL_RTCEx_BKUPWrite(&hrtc, reg, value);
    return;
}

/**
 * @brief Returns true if the interval counter is active and resets the counter
 */
bool StandbyRegister::IsIntervalActiveAndReset(Intervals interval)
{
    uint32_t reg = GetIntervalRegister(interval);
    volatile uint32_t value = HAL_RTCEx_BKUPRead(&hrtc, reg);
    bool ret = false;
    if (Configuration::GetInterval(interval) <= (Interval_t)value)
    {
        value = 0;
        ret = true;
        HAL_RTCEx_BKUPWrite(&hrtc, reg, value);
    }
    return ret;
}

/**
 * @brief Returns true if the interval counter indicate that the interval limit has been reached
 */
bool StandbyRegister::IsIntervalActive(Intervals interval)
{
    uint32_t reg = GetIntervalRegister(interval);
    uint32_t value = HAL_RTCEx_BKUPRead(&hrtc, reg);
    return Configuration::GetInterval(interval) <= (Interval_t)value;
}

/**
 * @brief Resets all active intervals. This function is like a default handler for not resetted intervals
 */
void StandbyRegister::ResetActiveIntervals()
{
    for (uint32_t i = 0;
         i < (uint32_t)Intervals::END;
         i++)
    {
        uint32_t reg = GetIntervalRegister((Intervals)i);
        uint32_t value = HAL_RTCEx_BKUPRead(&hrtc, reg);
        if (Configuration::GetInterval((Intervals)i) <= (Interval_t)value)
        {
            HAL_RTCEx_BKUPWrite(&hrtc, reg, 0);
        }
    }
}

/**
 * @brief Increments all interval counters
 */
void StandbyRegister::IncrementIntervalCounters()
{
    for (uint32_t i = 0; i < (uint32_t)Intervals::END; i++)
    {
        IncrementIntervalCounter((Intervals)i);
    }
    return;
}

/**
 * @brief Resets all interval counters
 */
void StandbyRegister::ResetIntervalCounters()
{
	for (uint32_t i = GetIntervalRegister((Intervals)0); i < GetIntervalRegister(Intervals::END); i++)
	{
		HAL_RTCEx_BKUPWrite(&hrtc, i, 0);
	}
}

/**
 * @brief Returns the register address of the given interval
 */
uint32_t StandbyRegister::GetIntervalRegister(Intervals interval)
{
    return STBYREG_INTERVALS_START + (uint32_t)interval;
}

/**
 * @brief Sets a flag which indicates whether the time has been initialized
 * @param init True tells that the time has been initialized and
 * 			   false sets, that the time hasn't been initialized
 */
void StandbyRegister::SetTimeInitialized(bool init)
{
	HAL_RTCEx_BKUPWrite(&hrtc, STBYREG_TIMEINITIAL, init == true ? 0xFFFF : 0);
}

/**
 * @brief Gets whether the RTC has been initialized or not
 * @return True if the time has been initialized and false if the current
 * 		   time in the RTC is not valid/initialized
 */
bool StandbyRegister::GetTimeInitialized()
{
	// As the initial value of the standby register is 0. This function
	// will return false in the uninitialized state!
	return HAL_RTCEx_BKUPRead(&hrtc, STBYREG_TIMEINITIAL) == 0xFFFF;
}
