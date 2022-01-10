/*
 * Configuration.cpp
 *
 *  Created on: Apr 30, 2018
 *      Author: Daniel Lohmann
 */

#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "Configuration.h"

/*
 * @brief Operation on the flash is running, while this variable is
 * 		  unequal to zero
 */
volatile uint32_t FlashWriteOnGoing = 0;
volatile uint32_t Configuration::toggle_ESP_LED = false;

/**
  * @brief  FLASH end of operation interrupt callback.
  * @param  ReturnValue: The value saved in this parameter depends on the ongoing procedure
  *                  Mass Erase: Bank number which has been requested to erase
  *                  Page Erase: Page which has been erased
  *                    (if 0xFFFFFFFF, it means that all the selected pages have been erased)
  *                  Program: Address which was selected for data program
  * @retval None
  */
void HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue)
{
	/* Call CleanUp callback when all requested pages have been erased */
	if (ReturnValue == 0xFFFFFFFF)
	{
		FlashWriteOnGoing = 0;
	}
}

/**
 * @brief Gets called when an error occurs
 */
void HAL_FLASH_OperationErrorCallback()
{
	//TODO what do with these errors
	//HAL_FLASH_GetError();

	_Error_Handler(__FILE__, __LINE__);
}

/**
 * @brief Calculates the greatest common divisor of two numbers
 * 
 * @return int Returns the greatest common divisor of two numbers
 */
int gcd(int a, int b)
{
    while (1)
    {
        if (a == 0)
		{ 
			return b;
		}
        b %= a;
        if (b == 0) 
		{
			return a;
		}
        a %= b;
    }
}

/**
 * @brief Calculates the least multiple of two numbers
 * 
 * @return int least multiple of the two numbers or zero
 */
int lcm(int a, int b)
{
    int temp = gcd(a, b);
    return temp ? (a / temp * b) : 0;
}

/**
 * @brief Initializing the configuration reading. On initial start some default 
 * 		  configuration are done.
 */
bool Configuration::Init()
{
	bool ret = true;
	/* First start of the program after a complete rewrite of the program FLASH 
	   memory, needs to have some sort of configuration which will be written
	   into the FLASH at this program stage.
	   For Development usage: If a configuration value is added or removed, the 
	   #define INITIAL_CONFIG_DONE value needs to be changed to trigger the 
	   initial configuration step */
	if (ReadMemory(CHECK_INITIAL_MEMADDR) != INITIAL_CONFIG_DONE)
	{
		/* Unlock the Flash Program Erase controller
		   following code execution is not as secure as before!!! */
		Unlock();

		ret &= SetInterval(Intervals::Sending, 30);
		ret &= SetInterval(Intervals::BatteryVoltage, 5);
		ret &= SetInterval(Intervals::SoilMoisture, 30);
		ret &= SetInterval(Intervals::SoilTemperature, 30);
		ret &= SetInterval(Intervals::AirTemperature, 5);
		ret &= SetInterval(Intervals::AirHumidity, 5);
		ret &= SetInterval(Intervals::pHValue, 60*5*1000);
		ret &= SetInterval(Intervals::Illuminance, 5);
		ret &= SetInterval(Intervals::NTPTimeSync, 60);
		ret &= SetBool(BoolOption::SendNB_IoT, true);
		ret &= SetBool(BoolOption::SendWLAN, true);
		ret &= SetBool(BoolOption::GotoStandby, true);
		ret &= SetBool(BoolOption::SendLoRa, false);
		ret &= SetBool(BoolOption::SyncTImeGPS, false);
		ret &= SetBool(BoolOption::SyncTimeNTP, true);

		ret &= SetNumber(NumericOption::NBIoT_DestinationPort, 10000);

		char* mqttBroker = "iot.nt.th-koeln.de";
		char* mqttTopic = "rhizotech/sensorbox1/values";
		char* mqttUsername = "Test";
		char* mqttPassword = "Test";
		char* wlanSSID = "TKWLAN";
		char* wlanPassword = "hottentottenstottermutter";
		char* nbiotAPN = "iot.1nce.net";
		char* nbiotDestIP = "10.64.72.81";
		char* nbiotNetwork = "26201"; // Default use telekom (DE)
		char* ntpserver = "time.th-koeln.de";
		char* caCertificate = ""
			"-----BEGIN CERTIFICATE-----\n"
			"MIIDwzCCAqugAwIBAgIBATANBgkqhkiG9w0BAQsFADCBgjELMAkGA1UEBhMCREUx\n"
			"KzApBgNVBAoMIlQtU3lzdGVtcyBFbnRlcnByaXNlIFNlcnZpY2VzIEdtYkgxHzAd\n"
			"BgNVBAsMFlQtU3lzdGVtcyBUcnVzdCBDZW50ZXIxJTAjBgNVBAMMHFQtVGVsZVNl\n"
			"YyBHbG9iYWxSb290IENsYXNzIDIwHhcNMDgxMDAxMTA0MDE0WhcNMzMxMDAxMjM1\n"
			"OTU5WjCBgjELMAkGA1UEBhMCREUxKzApBgNVBAoMIlQtU3lzdGVtcyBFbnRlcnBy\n"
			"aXNlIFNlcnZpY2VzIEdtYkgxHzAdBgNVBAsMFlQtU3lzdGVtcyBUcnVzdCBDZW50\n"
			"ZXIxJTAjBgNVBAMMHFQtVGVsZVNlYyBHbG9iYWxSb290IENsYXNzIDIwggEiMA0G\n"
			"CSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCqX9obX+hzkeXaXPSi5kfl82hVYAUd\n"
			"AqSzm1nzHoqvNK38DcLZSBnuaY/JIPwhqgcZ7bBcrGXHX+0CfHt8LRvWurmAwhiC\n"
			"FoT6ZrAIxlQjgeTNuUk/9k9uN0goOA/FvudocP05l03Sx5iRUKrERLMjfTlH6VJi\n"
			"1hKTXrcxlkIF+3anHqP1wvzpesVsqXFP6st4vGCvx9702cu+fjOlbpSD8DT6Iavq\n"
			"jnKgP6TeMFvvhk1qlVtDRKgQFRzlAVfFmPHmBiiRqiDFt1MmUUOyCxGVWOHAD3bZ\n"
			"wI18gfNycJ5v/hqO2V81xrJvNHy+SE/iWjnX2J14np+GPgNeGYtEotXHAgMBAAGj\n"
			"QjBAMA8GA1UdEwEB/wQFMAMBAf8wDgYDVR0PAQH/BAQDAgEGMB0GA1UdDgQWBBS/\n"
			"WSA2AHmgoCJrjNXyYdK4LMuCSjANBgkqhkiG9w0BAQsFAAOCAQEAMQOiYQsfdOhy\n"
			"NsZt+U2e+iKo4YFWz827n+qrkRk4r6p8FU3ztqONpfSO9kSpp+ghla0+AGIWiPAC\n"
			"uvxhI+YzmzB6azZie60EI4RYZeLbK4rnJVM3YlNfvNoBYimipidx5joifsFvHZVw\n"
			"IEoHNN/q/xWA5brXethbdXwFeilHfkCoMRN3zUA7tFFHei4R40cR3p1m0IvVVGb6\n"
			"g1XqfMIpiRvpb7PO4gWEyS8+eIVibslfwXhjdFjASBgMmTnrpMwatXlajRWc2BQN\n"
			"9noHV8cigwUtPJslJj0Ys6lDfMjIq2SPDqO/nBudMNva0Bkuqjzx+zOAduTNrRlP\n"
			"BSeOE6Fuwg==\n"
			"-----END CERTIFICATE-----";
		ret &= SetString(StringOption::CA_Certificate, (uint8_t*)caCertificate, strlen(caCertificate));
		ret &= SetString(StringOption::MQTT_Broker, (uint8_t*)mqttBroker, strlen(mqttBroker));
		ret &= SetString(StringOption::MQTT_Topic, (uint8_t*)mqttTopic, strlen(mqttTopic));
		ret &= SetString(StringOption::MQTT_Username, (uint8_t*)mqttUsername, strlen(mqttUsername));
		ret &= SetString(StringOption::MQTT_Password, (uint8_t*)mqttPassword, strlen(mqttPassword));
		ret &= SetString(StringOption::WLAN_SSID, (uint8_t*)wlanSSID, strlen(wlanSSID));
		ret &= SetString(StringOption::WLAN_Password, (uint8_t*)wlanPassword, strlen(wlanPassword));
		ret &= SetString(StringOption::NBIoT_APN, (uint8_t*)nbiotAPN, strlen(nbiotAPN));
		ret &= SetString(StringOption::NBIoT_DestinationIP, (uint8_t*)nbiotDestIP, strlen(nbiotDestIP));
		ret &= SetString(StringOption::NBIoT_Network, (uint8_t*)nbiotNetwork, strlen(nbiotNetwork)); 
		ret &= SetString(StringOption::NTPTimeServer, (uint8_t*)ntpserver, strlen(ntpserver));

		/* Indicate, that the initial memory configuration is done */
		ret &= WriteMemory(CHECK_INITIAL_MEMADDR, INITIAL_CONFIG_DONE);

		/* Save the configuration to memory (maybe some is also written but finish the write process*/
		SaveConfiguration();

		/* Lock the Flash Program Erase controller */
		Lock();
	}
	return ret;
}

/**
 * @brief Gets the value of a boolean configuration option
 */
bool Configuration::GetBool(BoolOption option)
{
	uint32_t address = BOOLOPTION_START + (int)option;
	bool ret = ReadMemory(address) ? true : false;
	return ret;
}

/**
 * @brief Gets the value of numeric configuration option
 */
uint32_t Configuration::GetNumber(NumericOption option)
{
	uint32_t address = NUMERICOPTION_START + (int)option;
	uint32_t ret = ReadMemory(address);
	return ret;
}

/**
 * @brief Gets the value of a string or general data configuration option
 * @return uint8_t*	Pointer to the data. The data is one element longer 
 * 				 	than in length specified, because the last element is 
 * 				 	a zero, which could be helpful, when the output 
 * 					should be used as string.
 * 					When the data is no longer needed delete[] musst be called!
 * 					When the value could not be read the nullptr is returned
 */
uint8_t* Configuration::GetString(StringOption option, uint16_t* length)
{
	uint16_t bytes = 0;
	uint32_t address = STRINGDATAOPTION_START + (int)option;
	uint8_t*  ret = ReadDataBlock(address, &bytes);
	if (length != nullptr)
	{
		*length = bytes;
	}
	return ret;
}

/**
 * @brief Reads an interval configuration value
 */
Interval_t Configuration::GetInterval(Intervals interval)
{
	volatile uint64_t value = ReadMemory(INTERVALS_START + (uint32_t)interval);
	return (Interval_t)value;
}

/**
 * @brief Gets the RTC wakeup period value for the corresponding rtc register
 */
uint32_t Configuration::GetRTCPeriod()
{
	volatile uint32_t RTCPeriod = gcd(
		GetInterval(Intervals::BatteryVoltage),
		GetInterval(Intervals::SoilMoisture));
	RTCPeriod = gcd(RTCPeriod, GetInterval(Intervals::SoilTemperature));
	RTCPeriod = gcd(RTCPeriod, GetInterval(Intervals::AirTemperature));
	RTCPeriod = gcd(RTCPeriod, GetInterval(Intervals::AirHumidity));
	RTCPeriod = gcd(RTCPeriod, GetInterval(Intervals::Illuminance));
	RTCPeriod = gcd(RTCPeriod, GetInterval(Intervals::pHValue));
	RTCPeriod = gcd(RTCPeriod, GetInterval(Intervals::Sending));
	RTCPeriod = gcd(RTCPeriod, GetInterval(Intervals::NTPTimeSync));

	return (RTCPeriod == 0 ? 1 : RTCPeriod);
}

/**
 * @brief Sets a boolean configuration option
 */
bool Configuration::SetBool(BoolOption option, bool value)
{
	uint32_t address = BOOLOPTION_START + (int)option;
	return WriteMemory(address, value);
}

/**
 * @brief Sets a numeric configuration option
 */
bool Configuration::SetNumber(NumericOption option, uint32_t value)
{
	uint32_t address = NUMERICOPTION_START + (int)option;
	return WriteMemory(address, value);
}

/**
 * @brief Sets a string or general data configuration option
 */
bool Configuration::SetString(StringOption option, uint8_t* value, uint16_t length)
{
	uint32_t address = STRINGDATAOPTION_START + (int)option;
	return WriteDataBlock(address, value, length);
}

/**
 * @brief Sets the specified interval to the given value
 * 
 * @param interval 	Interval, which should be set
 * @param value		Value, which should be set
 * @return true 	Interval was successfully set
 * @return false 	Interval could not be set
 */
bool Configuration::SetInterval(Intervals interval, Interval_t value)
{
	return WriteMemory(INTERVALS_START + (uint32_t)interval, (uint64_t)value);
}
