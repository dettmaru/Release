/**
 * @brief Measurement of values on the motherboard
 * 
 * @file Motherboard.cpp
 * @author Daniel Lohmann
 * @date 2018-04-25
 */
#include "Measure/Motherboard.h"
#include "Energy.h"
#include "stm32l4xx_hal.h"
#include "adc.h"
#include "Logging.h"
#include <algorithm>

#define STM_TEMPERATURE_HIGH_POWER_TIME	100
#define VBATT_VOLTAG_HIGH_POWER_TIME	100

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

bool floatSort (float i, float j) { return (i<j); }

/**
 * @brief Stores the last temperature of the STM
 */
float Motherboard::stmTemperature = 0;
/**
 * @brief Stores the last VBATT voltage
 */
float Motherboard::vbattVoltage = 0;

Motherboard::Motherboard()
{

}

Motherboard::~Motherboard()
{

}

/**
 * @brief Reads the voltage of the battery
 * 
 * @return Voltage in volt as float value
 */
float Motherboard::ReadVBATT(bool cached = false)
{
	if (!cached)
	{
		StartVBATT();
		while (ProcessVBATT())
		{

		}
	}
	return vbattVoltage;
}

/**
 * Returns whether VBATTOK signal is given or not
 */
bool Motherboard::GetVBATTOK()
{
	return HAL_GPIO_ReadPin(VBATT_OK_GPIO_Port, VBATT_OK_Pin) == GPIO_PIN_SET;
}

/**
 * @brief Gets a unique serial number information for the motherboard
 */
const SerialNumber_t Motherboard::GetSerialNumber()
{
	SerialNumber_t serialNumber;
	uint32_t word0 = HAL_GetUIDw0(); //first word of the unique device identifier (UID based on 96 bits)
	uint32_t word1 = HAL_GetUIDw1(); //second word of the unique device identifier (UID based on 96 bits)
	uint32_t word2 = HAL_GetUIDw2(); //third word of the unique device identifier (UID based on 96 bits)
	//uint32_t REVID = HAL_GetREVID(); //device identifier
	//uint32_t DEVID = HAL_GetDEVID(); //device revision identifier
	serialNumber.low = word0;
	serialNumber.mid = word1;
	serialNumber.high = word2;
	return serialNumber;
}

/**
 * @brief Gets the internal temperature sensor of the STM micro controller
 *        on the motherboard
 * 
 * @return Returns the temperature in °C
 */
float Motherboard::GetSTMTemperature(bool cached = false)
{
	if (!cached)
	{
		StartSTMTemperature();
		while (ProcessSTMTemperature())
		{

		}
	}
	return stmTemperature;
}

/**
 * @brief Starts the reading of the STM temperature
 */
void Motherboard::StartSTMTemperature()
{
	Energy::RequestHighPowerMode();
}

/**
 * @brief Process a reading of the STM temperature
 * @return True when measurement is in progress
 * 	       False when measurement is finished
 */
bool Motherboard::ProcessSTMTemperature()
{
	if (Energy::GetHighPowerTime() < STM_TEMPERATURE_HIGH_POWER_TIME)
	{
		return true;
	}

	MX_ADC1_Init();
	MX_ADC3_Init();

	ADC_ChannelConfTypeDef sConfig;
	sConfig.Channel = ADC_CHANNEL_VREFINT;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Energy::FreeRequestHighPowerMode();
		return false;
	}

	if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK)
	{
		Energy::FreeRequestHighPowerMode();
		return false;
	}

	sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
	{
		Energy::FreeRequestHighPowerMode();
		return false;
	}

	if (HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED) != HAL_OK)
	{
		Energy::FreeRequestHighPowerMode();
		return false;
	}

	const int averaging = 11;
	float tempValues[averaging] = {0};
	float temp = 0;
	float vref = 0;

	for (int i = 0; i < averaging; i++)
	{
		if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK)
		{
			Energy::FreeRequestHighPowerMode();
			return false;
		}

		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 1000);
		uint32_t vrefValue = HAL_ADC_GetValue(&hadc1);
		HAL_ADC_Stop(&hadc1);
		vref = __HAL_ADC_CALC_VREFANALOG_VOLTAGE(vrefValue, ADC_RESOLUTION_12B);

		if (HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED) != HAL_OK)
		{
			Energy::FreeRequestHighPowerMode();
			return false;
		}

		HAL_ADC_Start(&hadc3);
		HAL_ADC_PollForConversion(&hadc3, 1000);
		register uint32_t value = HAL_ADC_GetValue(&hadc3);
		HAL_ADC_Stop(&hadc3);

		// Following commented line only allows int value which is changed in
		// the used code
		//temp += __HAL_ADC_CALC_TEMPERATURE(vref, value, ADC_RESOLUTION_12B);
		tempValues[i] = ((( (((value * vref) / TEMPSENSOR_CAL_VREFANALOG)                          \
				- (int32_t) * TEMPSENSOR_CAL1_ADDR)                                        \
			 ) * (int32_t)(TEMPSENSOR_CAL2_TEMP - TEMPSENSOR_CAL1_TEMP)                    \
			) / (int32_t)((int32_t)*TEMPSENSOR_CAL2_ADDR - (int32_t)*TEMPSENSOR_CAL1_ADDR) \
		   ) + TEMPSENSOR_CAL1_TEMP;

	}
	Energy::FreeRequestHighPowerMode();

	std::sort(tempValues, tempValues + averaging, floatSort);

	temp = tempValues[averaging / 2];

	Motherboard::stmTemperature = temp;
	return false;
}

/**
 * @brief Starts the reading of the VBATT temperature
 */
void Motherboard::StartVBATT()
{
	HAL_GPIO_WritePin(SENSE_ON_GPIO_Port, SENSE_ON_Pin, GPIO_PIN_RESET);
	Energy::RequestHighPowerMode();
}

/**
 * @brief Process a reading of the VBATT voltage
 * @return True when measurement is in progress
 * 	       False when measurement is finished
 */
bool Motherboard::ProcessVBATT()
{
	if (Energy::GetHighPowerTime() < VBATT_VOLTAG_HIGH_POWER_TIME)
	{
		return true;
	}

	MX_ADC3_Init();

	const int averaging = 25;
	volatile float value =  0;

	ADC_ChannelConfTypeDef sConfig;
	sConfig.Channel = ADC_CHANNEL_6;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
	{
		value = -1;
	}
	else
	{
		if (HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED) != HAL_OK)
		{
			value = -1;
		}
		else
		{
			for (int i = 0; i < averaging; i++)
			{
				HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);
				HAL_ADC_Start(&hadc3);
				HAL_ADC_PollForConversion(&hadc3, 1000);
				value += HAL_ADC_GetValue(&hadc3);
			}
		}
	}

	Energy::FreeRequestHighPowerMode();
	HAL_GPIO_WritePin(SENSE_ON_GPIO_Port, SENSE_ON_Pin, GPIO_PIN_SET);
	HAL_ADC_Stop(&hadc3);

	value /= averaging;

#ifdef MOTHERBOARD_V1
	float voltage = 3.3f / 4096 / 0.7297f * value * 1.029531509f;
#else
	float voltage = 3.3f / 4096 / 0.5f * value;
#endif
	Motherboard::vbattVoltage = voltage;
	return false;
}

/**
 * @brief Gets the current state of the battery charger
 * @return Returns a value of enum LoadingState
 */
enum LoadingState Motherboard::GetLoadingState()
{
	volatile enum LoadingState state;
	volatile bool stat1 = HAL_GPIO_ReadPin(LOAD_STAT1_GPIO_Port, LOAD_STAT1_Pin) == GPIO_PIN_SET;
	volatile bool stat2 = HAL_GPIO_ReadPin(LOAD_STAT2_GPIO_Port, LOAD_STAT2_Pin) == GPIO_PIN_SET;
	volatile bool pg = HAL_GPIO_ReadPin(LOAD_PG_GPIO_Port, LOAD_PG_Pin) == GPIO_PIN_SET;
	if (stat1 == true && stat2 == true && pg == false)
	{
		state = Standby;
	}
	else if (stat1 == false && stat2 == true && pg == false)
	{
		state = Charging;
	}
	else if (stat1 == true && stat2 == false && pg == false)
	{
		state = Charged;
	}
	else if (stat1 == false && stat2 == false && pg == false)
	{
		state = SystemTest;
	}
	return state;
}
