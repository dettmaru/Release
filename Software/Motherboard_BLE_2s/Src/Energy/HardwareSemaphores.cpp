/**
 * @brief Hardware energy management (turn on and off)
 *        Contais a list of HardwareSemaphores
 * 
 * @file energy.c
 * @author Daniel Lohmann
 * @date 2019-05-08
 */
#include "HardwareSemaphores.h"
#include "Energy.h"
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "i2c.h"

void turnONsoilSensorLine()
{
    Energy::RequestHighPowerMode();
    HAL_GPIO_WritePin(PWR_SW6_GPIO_Port, PWR_SW6_Pin, GPIO_PIN_RESET);
}

void turnOFFsoilSensorLine()
{
    HAL_GPIO_WritePin(PWR_SW6_GPIO_Port, PWR_SW6_Pin, GPIO_PIN_SET);
    Energy::FreeRequestHighPowerMode();
}

void turnONairSensorLine()
{
    MX_I2C1_Init();
    HAL_GPIO_WritePin(PWR_SW5_GPIO_Port, PWR_SW5_Pin, GPIO_PIN_RESET);
}
 
void turnOFFairSensorLine()
{
    HAL_I2C_DeInit(&hi2c1);
    HAL_GPIO_WritePin(PWR_SW5_GPIO_Port, PWR_SW5_Pin, GPIO_PIN_SET);
}

void turnONsdi12hardware()
{
    Energy::RequestHighPowerMode();
	HAL_GPIO_WritePin(PWR_SW1_GPIO_Port, PWR_SW1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PWR_SW3_GPIO_Port, PWR_SW3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SDI12_PWR_GPIO_Port, SDI12_PWR_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SDI12_PWR_5V_GPIO_Port, SDI12_PWR_5V_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SDI12_PWR_12V_GPIO_Port, SDI12_PWR_12V_Pin, GPIO_PIN_SET);
}

void turnOFFsdi12hardware()
{
    HAL_GPIO_WritePin(PWR_SW1_GPIO_Port, PWR_SW1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PWR_SW3_GPIO_Port, PWR_SW3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SDI12_PWR_GPIO_Port, SDI12_PWR_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SDI12_PWR_5V_GPIO_Port, SDI12_PWR_5V_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SDI12_PWR_12V_GPIO_Port, SDI12_PWR_12V_Pin, GPIO_PIN_RESET);

	Energy::FreeRequestHighPowerMode();
}

void turnONUART1()
{
    MX_USART1_UART_Init();
}

void turnOFFUART1()
{
    HAL_UART_DeInit(&huart1);
}

HardwareSemaphore soilSensors(
    &turnONsoilSensorLine,
    &turnOFFsoilSensorLine);
HardwareSemaphore airSensors(
    &turnONairSensorLine,
    &turnOFFairSensorLine);
HardwareSemaphore sdi12hardware(
    &turnONsdi12hardware,
    &turnOFFsdi12hardware);
HardwareSemaphore uart1(
    &turnONUART1,
    &turnOFFUART1);
    
