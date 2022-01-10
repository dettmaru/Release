/*
 * led.cpp
 *
 *  Created on: Nov 1, 2019
 *      Author: Daniel Lohmann, Kai Lübeck
 */
#include "gpio.h"
#include "Configuration.h"
#include "StandbyRegister.h"

void ToggleLed(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	//when toggleState == true, Led is allowed to Toggle, when toggleState == false, Led is not allowed to Toggle
	if(Configuration::toggle_ESP_LED && StandbyRegister::GetLEDStatus())
	{
		HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
	}
}

void SetLed(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
	if(StandbyRegister::GetLEDStatus())
	{
		HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
	}
}

void ToogleLEDControl()
{
	if(StandbyRegister::GetLEDStatus())
	{
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
		StandbyRegister::SetLEDStatus(false);
	}
	else
	{
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
		StandbyRegister::SetLEDStatus(true);
	}
}

void InitLEDs()
{
	if (!StandbyRegister::GetLEDStatus())
	{
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
	}
	else
	{
		SetLed(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		SetLed(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		SetLed(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
		SetLed(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
	}
}

