/**
 * @brief Energy management of the circuits
 * 
 * @file energy.h
 * @author Daniel Lohmann
 * @date 2018-04-25
 */
#ifndef __ENERGY_H_
#define __ENERGY_H_

#include "main.h"
#include <stdbool.h>

/**
 * @brief Controls settings for the energy management.
 * 
 * This class can for example turn on / turn of the high power mode of the LTC3440
 */
class Energy {
public:
	static void EnablePullDown();
	static bool IsUSBLoading();
	static void EnableWakeUpSources();

	static void RequestHighPowerMode();
	static void FreeRequestHighPowerMode();
	static uint32_t GetHighPowerTime();

	static void DetermineWakeUpSources();
	static bool GetWakeUpFromRESET();
	static bool GetWakeUpFromButton1();
	static bool GetWakeUpFromButton2();
	static bool GetWakeUpFromButton3();
	static bool GetWakeUpFromUSBPower();
	static bool GetWakeUpFromRTC();
	static bool GetWakeUpFromIWDG();
	static bool GetWakeUpFromWWDG();
	static bool GetWakeUpFromLPRST();
	static bool GetWakeUpFromSoftwareRST();
private:
	static bool wakeUpRESET;
	static bool wakeUpButton1;
	static bool wakeUpButton2;
	static bool wakeUpButton3;
	static bool wakeUpUSBPower;
	static bool wakeUpRTC;
	static bool wakeUpIWDG;
	static bool wakeUpWWDG;
	static bool wakeUpLPRST;
	static bool wakeUpSoftwareRST;

	static uint8_t openHighPowerModeRequests;
	static uint32_t highPowerModeStart;
};

#endif /* __ENERGY_H_ */
