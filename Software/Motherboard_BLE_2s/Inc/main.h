/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_ll_rng.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_dma.h"

#include "stm32l4xx_ll_exti.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "iwdg.h"
#include "gpio.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
void _Error_Handler(char* file, int line);
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LOAD_STAT1_Pin GPIO_PIN_2
#define LOAD_STAT1_GPIO_Port GPIOE
#define LOAD_STAT2_Pin GPIO_PIN_3
#define LOAD_STAT2_GPIO_Port GPIOE
#define LOAD_PG_Pin GPIO_PIN_4
#define LOAD_PG_GPIO_Port GPIOE
#define MODE_SYNC_Pin GPIO_PIN_5
#define MODE_SYNC_GPIO_Port GPIOE
#define BT1_Pin GPIO_PIN_6
#define BT1_GPIO_Port GPIOE
#define BT2_Pin GPIO_PIN_13
#define BT2_GPIO_Port GPIOC
#define GPS_ON_SW_Pin GPIO_PIN_2
#define GPS_ON_SW_GPIO_Port GPIOF
#define VBATT_SENSE_Pin GPIO_PIN_3
#define VBATT_SENSE_GPIO_Port GPIOF
#define SX1272_RESET_Pin GPIO_PIN_5
#define SX1272_RESET_GPIO_Port GPIOF
#define DEBUG_RX_Pin GPIO_PIN_0
#define DEBUG_RX_GPIO_Port GPIOC
#define DEBUG_TX_Pin GPIO_PIN_1
#define DEBUG_TX_GPIO_Port GPIOC
#define BT3_Pin GPIO_PIN_0
#define BT3_GPIO_Port GPIOA
#define M95_TX_Pin GPIO_PIN_2
#define M95_TX_GPIO_Port GPIOA
#define M95_RX_Pin GPIO_PIN_3
#define M95_RX_GPIO_Port GPIOA
#define BC95_TX_Pin GPIO_PIN_4
#define BC95_TX_GPIO_Port GPIOC
#define USB_PWR_Pin GPIO_PIN_5
#define USB_PWR_GPIO_Port GPIOC
#define ESP_ON_Pin GPIO_PIN_11
#define ESP_ON_GPIO_Port GPIOF
#define NRF_ON_Pin GPIO_PIN_12
#define NRF_ON_GPIO_Port GPIOF
#define BC95_ON_Pin GPIO_PIN_0
#define BC95_ON_GPIO_Port GPIOG
#define PWR_SW3_Pin GPIO_PIN_1
#define PWR_SW3_GPIO_Port GPIOG
#define PWR_SW5_Pin GPIO_PIN_7
#define PWR_SW5_GPIO_Port GPIOE
#define PWR_SW6_Pin GPIO_PIN_8
#define PWR_SW6_GPIO_Port GPIOE
#define PWR_SW1_Pin GPIO_PIN_9
#define PWR_SW1_GPIO_Port GPIOE
#define M95_ON_SW_Pin GPIO_PIN_10
#define M95_ON_SW_GPIO_Port GPIOE
#define M95_STATUS_Pin GPIO_PIN_11
#define M95_STATUS_GPIO_Port GPIOE
#define M95_DTR_Pin GPIO_PIN_12
#define M95_DTR_GPIO_Port GPIOE
#define SMSPIN_M95_Pin GPIO_PIN_13
#define SMSPIN_M95_GPIO_Port GPIOE
#define ANT_M95_Pin GPIO_PIN_14
#define ANT_M95_GPIO_Port GPIOE
#define SMSPIN_BC95_Pin GPIO_PIN_15
#define SMSPIN_BC95_GPIO_Port GPIOE
#define BC95_RX_Pin GPIO_PIN_11
#define BC95_RX_GPIO_Port GPIOB
#define SE_REA_Pin GPIO_PIN_12
#define SE_REA_GPIO_Port GPIOB
#define SE_REA_EXTI_IRQn EXTI15_10_IRQn
#define BC95_RST_Pin GPIO_PIN_11
#define BC95_RST_GPIO_Port GPIOD
#define ANT_BC95_Pin GPIO_PIN_12
#define ANT_BC95_GPIO_Port GPIOD
#define SD_PWR_Pin GPIO_PIN_13
#define SD_PWR_GPIO_Port GPIOD
#define LED3_Pin GPIO_PIN_14
#define LED3_GPIO_Port GPIOD
#define LED4_Pin GPIO_PIN_15
#define LED4_GPIO_Port GPIOD
#define NRF_CE_Pin GPIO_PIN_2
#define NRF_CE_GPIO_Port GPIOG
#define NRF_IRQ_Pin GPIO_PIN_3
#define NRF_IRQ_GPIO_Port GPIOG
#define NRF_CSN_Pin GPIO_PIN_4
#define NRF_CSN_GPIO_Port GPIOG
#define SENSE_ON_Pin GPIO_PIN_5
#define SENSE_ON_GPIO_Port GPIOG
#define SDI12_RX_Pin GPIO_PIN_8
#define SDI12_RX_GPIO_Port GPIOG
#define SDI12_RX_EXTI_IRQn EXTI9_5_IRQn
#define VBATT_OK_Pin GPIO_PIN_7
#define VBATT_OK_GPIO_Port GPIOC
#define GPS_TX_Pin GPIO_PIN_9
#define GPS_TX_GPIO_Port GPIOA
#define GPS_RX_Pin GPIO_PIN_10
#define GPS_RX_GPIO_Port GPIOA
#define SX1272_DIO2_Pin GPIO_PIN_15
#define SX1272_DIO2_GPIO_Port GPIOA
#define SX1272_DIO0_Pin GPIO_PIN_0
#define SX1272_DIO0_GPIO_Port GPIOD
#define SX1272_DIO1_Pin GPIO_PIN_1
#define SX1272_DIO1_GPIO_Port GPIOD
#define SX1272_DIO3_Pin GPIO_PIN_3
#define SX1272_DIO3_GPIO_Port GPIOD
#define SDI12_PWR_12V_Pin GPIO_PIN_4
#define SDI12_PWR_12V_GPIO_Port GPIOD
#define SDI12_PWR_5V_Pin GPIO_PIN_5
#define SDI12_PWR_5V_GPIO_Port GPIOD
#define SDI12_PWR_Pin GPIO_PIN_6
#define SDI12_PWR_GPIO_Port GPIOD
#define SDI12DIR_Pin GPIO_PIN_7
#define SDI12DIR_GPIO_Port GPIOD
#define GPS_ON_Pin GPIO_PIN_8
#define GPS_ON_GPIO_Port GPIOB
#define M95_ON_Pin GPIO_PIN_9
#define M95_ON_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOE
#define LED2_Pin GPIO_PIN_1
#define LED2_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */
//#define MOTHERBOARD_V1
#define MOTHERBOARD_V2
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
