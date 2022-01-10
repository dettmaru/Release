/*
 * StandbyRegister.h
 *
 *  Created on: Aug 15, 2018
 *      Author: Daniel Lohmann
 */
#ifndef STANDBYREGISTER_H
#define STANDBYREGISTER_H

#include "Configuration.h"
#include "StandbyRegisterMemoryAddress.h"

/**
 * @brief Registers which will retain their content in the standby mode, can be 
 *        accessed through this interface.
 */
class StandbyRegister
{
public:
    static Interval_t GetIntervalCounter(Intervals interval);
    static void IncrementIntervalCounters();
    static void ResetIntervalCounters();
    static void ResetActiveIntervals();

    static bool IsIntervalActiveAndReset(Intervals interval);
    static bool IsIntervalActive(Intervals interval);

    static void SetTimeInitialized(bool init = true);
    static bool GetTimeInitialized();

    static void SetLEDStatus(bool status);
    static bool GetLEDStatus();
private:
    static void IncrementIntervalCounter(Intervals interval);
    static uint32_t GetIntervalRegister(Intervals interval);
};


#endif /* STANDBYREGISTER_H */
