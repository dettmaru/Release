/**
 * @brief Measurement of values on the motherboard
 * 
 * @file Motherboard.h
 * @author Daniel Lohmann
 * @date 2018-04-25
 */
#ifndef __MOTHERBOARD_MEASURE_H_
#define __MOTHERBOARD_MEASURE_H_

#include "main.h"
#include <stdbool.h>

/**
 * @brief Represents the serial number of the STM32 which will also be used as 
 *        sensor box serial number
 */
typedef struct {
	uint32_t low;
	uint32_t mid;
	uint32_t high;
} SerialNumber_t;

enum LoadingState
{
	Standby,
	Charging,
	Charged,
	SystemTest,
	NotLoading
};

/**
 * @brief Handles measurements and other stuff related to the Motherboard
 */
class Motherboard {
public:
	Motherboard();
	virtual ~Motherboard();

	static bool GetVBATTOK();
	static enum LoadingState GetLoadingState();

	static void StartSTMTemperature();
	static bool ProcessSTMTemperature();
	static void StartVBATT();
	static bool ProcessVBATT();
	static float GetSTMTemperature(bool cached);
	static float ReadVBATT(bool cached);

	static const SerialNumber_t GetSerialNumber();
private:
	static float stmTemperature;
	static float vbattVoltage;
};

#endif /*__MOTHERBOARD_MEASURE_H_*/
