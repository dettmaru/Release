/**
 * @mainpage Motherboard STM Software
 * 
 * @section Documentation
 * 
 * General software documentation could be found in the document Software.pdf
 * which is the entry point into the software documentation.
 * In this documentation the source code is documented and only the functions,
 * variables, class, etc. are documented.
 * 
 * @section MainFunc Main function
 * 
 * The main() function is defined in main.c a change from c-source code to c++
 * source code is done by calling CallMain() which calls then maincpp() in mainCpp.cpp
 * 
 * @section CubeMxCompatibility CubeMx compatibility
 * 
 * This project is compatible with CubeMx. It is possible to regenerate code via
 * CubeMx. This allows a graphical configuration of the hardware peripherals. 
 */


/* Include framework headers */
#include "main.h"
#include "spi.h"
#include "rtc.h"
#include "usb_device.h"
#include "usbd_storage_if.h"
#include "stdbool.h"

/* Include "own"-code headers */
#include "Energy.h"
#include "DataProcessor.h"
#include "Extern.h"
#include "Configuration.h"
#include "ConfigurationFiles.h"
#include "StandbyRegister.h"
#include "StandbyCache.h"
#include "ESP.h"
#include "Motherboard.h"
#include "sdi12.h"
#include "string.h"
#include "a2235.h"
#include "m95.h"
#include "bc95g.h"
#include "linkedlist.h"
#include "mqttsn.h"
#include "Cellular.h"
#include "SensorDataSet.h"
#include "Logging.h"
#include "TimeHelper.h"

/* Callback functions */

extern volatile uint32_t FlashWriteOnGoing;

extern "C" void SystemClock_Config();

/**
 * @brief RTC periodic event handler
 *
 * In case of a wake up by the RTC this function is not called
 */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	// Increase the interval counter
	StandbyRegister::IncrementIntervalCounters();
}

/**
 * @brief Callback for EXTI line. For example a Button press while 
 * 		  the controller is running is handled here
 * 
 * @param GPIO_Pin EXTI line which has triggered this event
 */
void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin)
{
	if (GPIO_Pin == BT1_Pin)
	{

	}
	if (GPIO_Pin == BT2_Pin)
	{
		ToogleLEDControl();
	}
	if (GPIO_Pin == BT3_Pin)
	{

	}
	if (GPIO_Pin == SE_REA_Pin)
	{
		//for (int i = 0; i < 1000000; i++)
		{
			asm("nop");
		}
		ESP::Ready = true;
	}
	if (GPIO_Pin == SDI12_RX_Pin)
	{
		SDI12_LineProcessing();
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if (htim == &SDI12TIMER)
	{
		SDI12_LineProcessing();
	}
}

/**
 * @brief Main function in C++ code
 */
void maincpp()
{
	InitLEDs();

	// Determine wakeup sources
	Energy::DetermineWakeUpSources();

	// Set next wake up time
	if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, Configuration::GetRTCPeriod(), RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	// Configuration initialization
	Configuration::Init();
	// Enable the chip internal pull ups
	Energy::EnablePullDown();
	// Enable the wake up sources like the buttons
	Energy::EnableWakeUpSources();
	// Enable and maybe initialize the sensor data cache
	StandbyCache::EnableCache();

	#if DEBUG_CONFIGFILES
	// Dump configuration options, when configured in debugging 
	ConfigurationFiles::DumpOptions();
	#endif

	//if (Energy::GetWakeUpFromRTC())
	{
		// Increase the interval counters
		StandbyRegister::IncrementIntervalCounters();
	}

	if (Energy::GetWakeUpFromRESET())
	{
		if (SDCard::Init() == HAL_OK)
		{
			__disable_irq(); // TODO: this is a work around for the, problem that currently interrupts can damage the SD card while writing to it
			ConfigurationFiles* configFiles = new ConfigurationFiles();
			// Write basic information onto the SD card e.g. the serial number
			configFiles->WriteInfoFile();
			__enable_irq();

			Configuration::Unlock();
			// Read settings files from the SD card
			configFiles->ReadBasicSetupFile();
			configFiles->ReadSettingsFile();
			Configuration::Lock();

			delete configFiles;
		}
		if (SDCard::DeInit() == HAL_OK)
		{

		}

		/*
		 * The following code block has been disabled, because the current
		 * strategy for NB-IoT is to register to the network and sent a bunch of
		 * cache messages, so that only a few times a day the registration process
		 * needs to be executed
		 */
//		if (Configuration::GetBool(BoolOption::SendNB_IoT))
//		{
//			/* After a reset of the Sensorbox a restart of the NB_IoT module
//			   will be executed */
//			Cellular::InitNBIoT();
//		}
	}

	if ((StandbyRegister::IsIntervalActive(Intervals::NTPTimeSync) || !Time::IsInitialized())
		&& Configuration::GetBool(BoolOption::SyncTImeGPS))
	{
		uint32_t timeout = 1000*60;
		uint32_t startTime = HAL_GetTick();
		hGPS.Init(&huart1);
		while (!hGPS.time_available && (HAL_GetTick() - startTime < timeout))
		{
            // Attention: Independent watch dog (IWDG) Reset!
            HAL_IWDG_Refresh(&hiwdg);
		}
		if (!hGPS.time_available)
		{
			LOGW("GPS", "Time synchronization unsuccessful!");
		}
		else
		{
			LOGI("GPS", "Time synchronization successful");
		}
		hGPS.DeInit();
		StandbyRegister::IsIntervalActiveAndReset(Intervals::NTPTimeSync);
	}

	if(Energy::GetWakeUpFromButton2())
	{
		ToogleLEDControl();

		// Show the new state for one second
		HAL_Delay(1000);
		
		// Clear Flags and begin Standby Mode, nothing more should be done!
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);
		HAL_PWR_EnterSTANDBYMode(); // END
	}

	if(Energy::GetWakeUpFromButton3())
	{
		//Toggle All LED's Off
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);

		//Create new data set
		uint32_t countTime = 0;
		SensorDataSet* sensorData = new SensorDataSet();
		DataProcessor process;
		struct tm timeStamp;
		Time::Get(&timeStamp);
		sensorData->SetTimestamp(&timeStamp);
		float BatteryVoltage = Motherboard::ReadVBATT(false);
		sensorData->AddBatteryVoltage(BatteryVoltage);
		StandbyCache::PushSensorDataSet(sensorData);
		free(sensorData);
		std::vector<SensorDataSendingPacket> packets{};
		uint16_t rangeEnd = StandbyCache::GetSensorDataSetsCount();
		for (uint16_t i = 0; i < rangeEnd; i++)
		{
			packets.push_back(SensorDataSendingPacket(i));
		}


		Configuration::toggle_ESP_LED = true;
		//Testing Wlan-Connection if Wlan is the set Sending Mode
		if(Configuration::GetBool(BoolOption::SendWLAN))
		{
			//Hold specific LED's three Seconds
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
			HAL_Delay(3000);
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);

			//Sending Packets with Wlan
			ESP::SendSensorData(&packets);

			//Show Results
			if(packets[0].WLAN)
			{
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
			}
		}

		//Testing NB-Iot-Connection if NB-Iot is the set Sending Mode
		else if(Configuration::GetBool(BoolOption::SendNB_IoT))
		{
			//Hold specific LED's three Seconds
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
			HAL_Delay(3000);
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);

			//Sending Packets with NB-IoT
			//ESP::SendSensorData(&packets);

			//Show Results
			if(packets[0].NB_IoT)
			{
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
			}

		}

		//Testing GSM-Connection (--> SendLoRa) if GSM is the set Sending Mode
		else if(Configuration::GetBool(BoolOption::SendLoRa))
		{
			//Hold specific LED's three Seconds
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
			HAL_Delay(3000);
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);

			//Sending Packets with GSM (--> LoRa)
			//ESP::SendSensorData(&packets);

			//Show Results
			if(packets[0].LoRa)
			{
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
			}
		}
		Configuration::toggle_ESP_LED = false;

		//Show Results until Button 3 is pressed
		if(StandbyRegister::GetLEDStatus())
		{
			countTime = HAL_GetTick();
			uint16_t  btState = false;
			btState = HAL_GPIO_ReadPin(BT3_GPIO_Port, BT3_Pin);
			while( ((HAL_GetTick() - countTime) < (60*1000)) && !btState)
			{
				btState = HAL_GPIO_ReadPin(BT3_GPIO_Port, BT3_Pin);
			}
		}
		//Toggle All LED's Off
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
		//Clear Flags and begin Standby Mode
		HAL_Delay(1000);
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF1);
		HAL_PWR_EnterSTANDBYMode(); // ENDE
	}


	DataProcessor processor;
	bool measure = processor.StartMeasurement();
	if (measure)
	{
		while (!processor.FinishedMeasurement())
		{
			SetLed(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
			__HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_HSI);
			HAL_SuspendTick();
			HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
			for (int i = 0; i < 10; i++)
			{
				HAL_IncTick();
			}
			HAL_ResumeTick();
			SystemClock_Config();
			SetLed(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);

			processor.Process();
			//Wait for finishing of the measurements

            // Attention: Independent watch dog (IWDG) Reset!
            HAL_IWDG_Refresh(&hiwdg);
		}
		const SensorDataSet* sensorData = processor.GetMeasuredData();

		volatile bool saved = StandbyCache::PushSensorDataSet(sensorData);
		//saved = false; // DEBUG Implementation!!!!!!!!!!!!!!!!!!!!!!!!!
		if (!saved || StandbyRegister::IsIntervalActiveAndReset(Intervals::Sending) ||
			(!Time::IsInitialized() && Configuration::GetBool(BoolOption::SyncTimeNTP)))
		{
			uint16_t rangeBegin = 0;
			uint16_t rangeEnd = StandbyCache::GetSensorDataSetsCount();

			std::vector<SensorDataSendingPacket> packets{};
			for (uint16_t i = rangeBegin; i < rangeEnd; i++)
			{
				packets.push_back(SensorDataSendingPacket(i));
			}
			processor.SaveAndSendSensorData(&packets);
		}
	}

	HAL_PWR_EnterSTANDBYMode(); // ENDE


	while(1) {}

	HAL_PWREx_DisableGPIOPullDown(PWR_GPIO_F, PWR_GPIO_BIT_11);

	uint32_t wakeupCounter = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1);
	wakeupCounter++;
	if (wakeupCounter > 60*4)
	{
		wakeupCounter = 0;
	}
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, wakeupCounter);

	if (__HAL_PWR_GET_FLAG(PWR_FLAG_WUF3) || wakeupCounter == 0)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF3);
	}

	//HAL_Delay(100);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU); // Clear any existing wakeup-flags (Buttons)
	//__HAL_RTC_WAKEUPTIMER_EXTI_CLEAR_FLAG();
	//HAL_PWR_EnterSTANDBYMode();

	if (Energy::IsUSBLoading())
	{
		MX_USB_DEVICE_Init();
	}

	/* Infinite loop */
	while (1)
	{
		ToggleLed(LED2_GPIO_Port, LED2_Pin);
		if (!Energy::IsUSBLoading())
		{
			//The change into standby mode, will "restart" the program at
			//the main function, after wake up

			/* Wait for any write to complete before entering standby/shutdown mode */
    		while (FlashWriteOnGoing == 1) { }

    		/* Enter low-power mode */
    		/* NOTE: Care must be taken when using shutdown mode.
             	The SBF bit from the PWR_SR1 register used in this example can not
             	be used anymore. The system status has to be stored in the
             	RTC backup registers. */
			HAL_PWR_EnterSTANDBYMode();
		}
	}
}

/**
 * @brief Helper function to do the transistion from c to c++ source code
 */
extern "C" void CallMain() { maincpp(); }
