/**
 * @brief Energy management of the circuits
 * 
 * @file energy.c
 * @author Daniel Lohmann
 * @date 2018-04-25
 */
#include "Energy.h"
#include "rtc.h"
#include "Logging.h"

bool Energy::wakeUpRESET = false;
bool Energy::wakeUpButton1 = false;
bool Energy::wakeUpButton2 = false;
bool Energy::wakeUpButton3 = false;
bool Energy::wakeUpUSBPower = false;
bool Energy::wakeUpRTC = false;
bool Energy::wakeUpIWDG = false;
bool Energy::wakeUpWWDG = false;
bool Energy::wakeUpLPRST = false;
bool Energy::wakeUpSoftwareRST = false;

/**
 * @brief Stores the count of open requests for high power mode of the DC-DC converter
 */
uint8_t Energy::openHighPowerModeRequests = 0;

/**
 * @brief Stores the time at which the current/last high power state was started
 * 		  This value is needed because, it tames some milliseconds, before the state
 * 		  is stationary
 */
uint32_t Energy::highPowerModeStart = 0;

/**
 * @brief Setup pullups and pulldowns in standby mode
 * 
 */
void Energy::EnablePullDown()
{
	//Enable PullDown in StandbyMode
	HAL_PWREx_EnablePullUpPullDownConfig();
	HAL_PWREx_EnableGPIOPullDown(PWR_GPIO_E, PWR_GPIO_BIT_6); 	//BT1
	HAL_PWREx_EnableGPIOPullDown(PWR_GPIO_C, PWR_GPIO_BIT_13); 	//BT2
	HAL_PWREx_EnableGPIOPullDown(PWR_GPIO_A, PWR_GPIO_BIT_0); 	//BT3
	HAL_PWREx_EnableGPIOPullDown(PWR_GPIO_C, PWR_GPIO_BIT_5); 	//USB_PWR

	//Turn on NB-IoT module in standby mode
	//HAL_PWREx_EnableGPIOPullDown(PWR_GPIO_G, PWR_GPIO_BIT_0); This is currently disabled because PSM will not be used!

    //Pull MODE/SYNC-Pin of LTC3440 to low, so that 
    //the DC-DC-Converter can go into the power down mode
	HAL_PWREx_EnableGPIOPullUp(PWR_GPIO_E, PWR_GPIO_BIT_5); 
}

/**
 * @brief Checks whether the circuit is loaded over the USB port
 * 
 * @return true, when the circuit gets external power through the USB port
 * @return false, when the circuit is not powered by USB
 */
bool Energy::IsUSBLoading()
{
	GPIO_PinState USB_PWR = HAL_GPIO_ReadPin(USB_PWR_GPIO_Port, USB_PWR_Pin);
    //Read the status of the three state-signals of the battery charger controller
	//GPIO_PinState Load1 = HAL_GPIO_ReadPin(LOAD_STAT1_GPIO_Port, LOAD_STAT1_Pin);
	//GPIO_PinState Load2 = HAL_GPIO_ReadPin(LOAD_STAT2_GPIO_Port, LOAD_STAT2_Pin);
	//GPIO_PinState PG = 	  HAL_GPIO_ReadPin(LOAD_PG_GPIO_Port, LOAD_PG_Pin);

	return USB_PWR == GPIO_PIN_SET;
}

/**
 * @brief Enables different Wake up sources
 * 
 *  - Button 1 
 *
 *  - Button 2
 * 
 *  - Button 3
 */
void Energy::EnableWakeUpSources()
{
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN5_HIGH);  //USB_PWR
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN3_HIGH);	//Button 1
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2_HIGH);	//Button 2
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1_HIGH);  //Button 3
	return;
}

/**
 * @brief Sets the DC-DC converter into high power mode, and registers
 * 	 	  that one process has asked for high power mode. To get back to
 * 	 	  the energy saving mode FreeRequestHighPowerMode must be called
 * 	 	  for every request done to this method.
 */
void Energy::RequestHighPowerMode()
{
	if (openHighPowerModeRequests == 255)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	openHighPowerModeRequests++;

	if (openHighPowerModeRequests == 1)
	{
		//Turn on high power mode
		HAL_GPIO_WritePin(MODE_SYNC_GPIO_Port, MODE_SYNC_Pin, GPIO_PIN_RESET);
		highPowerModeStart = HAL_GetTick();
	}
	return;
}

/**
 * @brief Turns the DC-DC-Converter back to energy saving mode, when all
 * 		  request have been freed, which must be done with this method.
 */
void Energy::FreeRequestHighPowerMode()
{
	if (openHighPowerModeRequests == 0)
	{
		_Error_Handler(__FILE__, __LINE__);
		return;
	}
	openHighPowerModeRequests--;
	if (openHighPowerModeRequests == 0)
	{
		//All request have been removed, therefore change back to energy saving mode
		HAL_GPIO_WritePin(MODE_SYNC_GPIO_Port, MODE_SYNC_Pin, GPIO_PIN_SET);
	}
	return;
}

/**
 * @brief Gets the time since the high power mode start
 * @return Returns the time in milliseconds since the start of the high power mode
 */
uint32_t Energy::GetHighPowerTime()
{
	if (openHighPowerModeRequests > 0)
	{
		return HAL_GetTick() - highPowerModeStart;
	}
	else
	{
		return 0;
	}
}


void Energy::DetermineWakeUpSources()
{
	if (__HAL_PWR_GET_FLAG(PWR_FLAG_WUF1)) //BT3
	{
		wakeUpButton3 = true;
		LOGV("main", "Wakeup from BT3");
	}
	if (__HAL_PWR_GET_FLAG(PWR_FLAG_WUF2)) //BT2
	{
		wakeUpButton2 = true;
		LOGV("main", "Wakeup from BT2");
	}
	if (__HAL_PWR_GET_FLAG(PWR_FLAG_WUF3)) //BT1
	{
		wakeUpButton1 = true;
		LOGV("main", "Wakeup from BT1");
	}
	if (__HAL_PWR_GET_FLAG(PWR_FLAG_WUF5)) //USB Power
	{
		wakeUpUSBPower = true;
		LOGV("main", "Wakeup from USB cable connected");
	}
	if (__HAL_RTC_WAKEUPTIMER_GET_FLAG(&hrtc, RTC_FLAG_WUTF))
	{
		// Will be reseted by new wake up period
		wakeUpRTC = true;
		LOGV("main", "Wakeup from RTC");
	}

	if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST)) // Independent Watchdog reset
	{
		wakeUpIWDG = true;
		LOGE("main", "Wakeup from Independent Watchdog reset!!!");
	}
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST)) // Window Watchdog reset
	{
		wakeUpWWDG = true;
		LOGE("main", "Wakeup from Window Watchdog reset!!!");
	}
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST)) // Low Power reset
	{
		wakeUpLPRST = true;
		LOGE("main", "Wakeup from Low Power reset!!!");
	}
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST)) // Software reset
	{
		wakeUpSoftwareRST = true;
		LOGE("main", "Wakeup from Software reset!!!");
	}

	__HAL_RCC_CLEAR_RESET_FLAGS();

	wakeUpRESET = !(
			wakeUpButton1 |
			wakeUpButton2 |
			wakeUpButton3 |
			wakeUpUSBPower |
			wakeUpRTC |
			wakeUpIWDG |
			wakeUpWWDG |
			wakeUpLPRST |
			wakeUpSoftwareRST);

	if (wakeUpRESET)
	{
		LOGV("main", "Wakeup from RESET");
	}
}

/**
 * @brief Returns whether the RESET button was pressed or a normal
 * 		  power-on-reset have been executed.
 * @return True, when wake up reason is RESET button press or a power-on-reset
 */
bool Energy::GetWakeUpFromRESET()
{
	return wakeUpRESET;
}

/**
 * @brief Returns whether the current wake up was caused by Button1
 * @return True, when wake up reason is Button1
 */
bool Energy::GetWakeUpFromButton1()
{
	return wakeUpButton1;
}

/**
 * @brief Returns whether the current wake up was caused by Button2
 * @return True, when wake up reason is Button2
 */
bool Energy::GetWakeUpFromButton2()
{
	return wakeUpButton2;
}

/**
 * @brief Returns whether the current wake up was caused by Button3
 * @return True, when wake up reason is Button3
 */
bool Energy::GetWakeUpFromButton3()
{
	return wakeUpButton3;
}

/**
 * @brief Returns, whether the current wake up was caused by the USBPower connection
 * @return True, when wake up reason is the connection of a USB cable
 */
bool Energy::GetWakeUpFromUSBPower()
{
	return wakeUpUSBPower;
}

/**
 * @brief Returns, whether the current wake up was caused by the RTC (periodic wake up)
 * @return True, when wake up reason is the RTC
 */
bool Energy::GetWakeUpFromRTC()
{
	return wakeUpRTC;
}

/**
 * @brief Returns, whether the current wake up was caused by the IWDG (Independent Watchdog)
 * @return True, when wake up reason is the IWDG
 */
bool Energy::GetWakeUpFromIWDG()
{
	return wakeUpIWDG;
}

/**
 * @brief Returns, whether the current wake up was caused by the WWDG (Window Watchdog)
 * @return True, when wake up reason is the WWDG
 */
bool Energy::GetWakeUpFromWWDG()
{
	return wakeUpWWDG;
}

/**
 * @brief Returns, whether the current wake up was caused by a Low Power Reset
 * @return True, when wake up reason is Low Power Reset
 */
bool Energy::GetWakeUpFromLPRST()
{
	return wakeUpLPRST;
}

/**
 * @brief Returns, whether the current wake up was caused by a software reset
 * @return True, when wake up reason is a software reset
 */
bool Energy::GetWakeUpFromSoftwareRST()
{
	return wakeUpSoftwareRST;
}
