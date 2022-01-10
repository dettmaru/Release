/**
 * @brief Configuration handler
 * 
 * @file ConfigurationFiles.cpp
 * @author Daniel Lohmann
 * @date 2019-03-13
 */
#include "ConfigurationFiles.h"
#include "Motherboard.h"
#include "SDCard.h"
#include <string.h>
#include "main.h"
#include "StringHelper.h"
#include "SerialNumberToString.h"
#include "Logging.h"
#include "VersionInfo.h"
#include "DataProcessor.h"

ConfigurationFileOption* LineInterpretationStatecurrentMultilineOption = nullptr;


ConfigurationFiles::ConfigurationFiles()
{

}

ConfigurationFiles::~ConfigurationFiles()
{

}

/**
 * @brief Interpete a line of a configuration/settings file. If an option is 
 *        found in the given line the options is set in the flash memory. 
 * 
 * @param line                   A line of the configuration/settings file
 * @param configurationOptions   Configuration options which could exist in a 
 *                               configuration file, specifying how to parse the 
 *                               options may given in the input line.
 * @param state                  State for multiline processing. Initial value 
 *                               can be an empty @ref{LineInterpretationState}
 */
void ConfigurationFiles::InterpretLine(char* line, std::list<ConfigurationFileOption>& configurationOptions, LineInterpretationState* state)
{
	if (state->currentMultilineOption != nullptr)
	{
		// Currently processing a multiple line option
		char* trimmedInput = strtrim(line);
		int length = strlen(trimmedInput);
		if (length == 0)
		{
			// Process multiple line option value
			ConfigurationOption configOption = state->currentMultilineOption->GetConfigurationOption()[0];
            char* trimmed = strtrim(state->GetStringBuffer());
			configOption.SetValue(trimmed);

			//TODO saving should be done later, but then also the free would be needed to be done later!!!
			Configuration::SaveConfiguration();

			// Finish multiple line processing buffer
            free(trimmed);
            delete state->currentMultilineOption;
			state->currentMultilineOption = nullptr;
			state->ClearStringBuffer();
		}
		else
		{
			// More date to add to the multiple line option
			state->AddStringToBuffer(line);
		}
		free(trimmedInput);
	}

	int length = strlen(line);
    int doubleDot = 0;
    for (int i = 0; i < length; i++)
    {
        if (line[i] == ':')
        {
            doubleDot = i;
            break;
        }
    }

    line[doubleDot] = '\0';
    char* optionName = line;
    char* parameter = line + doubleDot + 1;

    for (auto option : configurationOptions)
    {
        const char* currentOptionName = option.GetFileString();
        if (strcmp(currentOptionName, optionName) == 0)
        {
            if (!option.GetMultiline())
            {
                // Single line option multiple boolean options
                if (option.NamedOptions())
                {
                    int count = 0;
                    char** parts = strsplit(parameter, &count);
                
                    for (auto paramOption : option.GetConfigurationOption())
                    {
                        bool found = false;
                        for (int i = 0; i < count; i++)
                        {
                            if (strcmp(paramOption.first, parts[i]) == 0)
                            {
                                found = true;
                                break;
                            }   
                        }
                        paramOption.second.SetValue(found);
                    }

                    for (int i = 0; i < count; i++)
                    {
                        free(parts[i]);
                    }
                    free(parts);
                }
                // Single line option only one value
                else if (option.GetConfigurationOption().size() == 1)
                {
                    char* valueString = strtrim(parameter);
                    #if DEBUG_CONFIGFILES
                    LOGI("Config", "ParmaterValue:%s", valueString);
                    #endif

                    ConfigurationOption configOption = option.GetConfigurationOption()[0];
                    configOption.SetValue(valueString);

                    //TODO saving should be done later, but then also the free would be needed to be done later!!!
                    Configuration::SaveConfiguration();
                    //free(valueString); 
                }
                else
                {
                    _Error_Handler(__FILE__, __LINE__);
                }   
            }
            else
            {
                // Multiple line option
            	state->currentMultilineOption = new ConfigurationFileOption(option);
            	state->AddStringToBuffer(parameter);
            }
            
            #if DEBUG_CONFIGFILES
            LOGI("Config", "Option found %s", optionName);
            #endif

            break;
        }
    }
}

/**
 * @brief Generic handler for configuration/settings files
 * 
 * @params[in] filename             Name/Path of file to be read
 * @params[in] configurationOptions Configuration options which could be present 
 *                                  in the configuration file. The list of 
 *                                  ConfigurationFileOption's contains the 
 *                                  information on how to parse and save the 
 *                                  values in Flash memory.
 */
void ConfigurationFiles::ReadConfigFile(char* filename, std::list<ConfigurationFileOption>& configurationOptions)
{
    FIL fileHandler;
	if (f_open(&fileHandler, (const TCHAR*)filename, FA_OPEN_EXISTING | FA_READ) == FR_OK)
    {
        char line[1000];
        LineInterpretationState state;
        while (f_gets(line, 1000, &fileHandler))
        {
            InterpretLine(line, configurationOptions, &state);

            // Attention: Independent watch dog (IWDG) Reset!
            HAL_IWDG_Refresh(&hiwdg);
            /* The Independent watch do Reset is necessary at this point, 
               because the saving operations to Flash memory are a slow 
               operation and the Watch Dog might stop the program to early
             */
        }

        f_close(&fileHandler);

        // Force that a file without new line at the end is interpreted like it had one
        memset(line, '\0', 1000);
        InterpretLine(line, configurationOptions, &state);
    }
}

void ConfigurationFiles::ReadBasicSetupFile()
{
    auto configurationOptions = GetOptionsListBasicSetup();
    ReadConfigFile(FILENAME_BASICSETUP_CONF, configurationOptions);
}

/**
 * @brief Read in the settings file from the SD card and save the settings in
 *        Flash memory to reduce access count to SD card.
 * 
 *        In this function also the table header for the sensor data file will 
 *        be written, if the amount of columns changes.
 */
void ConfigurationFiles::ReadSettingsFile()
{
    ConfiguredSensorCount oldCounts = DataProcessor::GetConfiguredSensorCount();

    auto configurationOptions = GetOptionsListSettings();
    ReadConfigFile(FILENAME_SETTINGS_CONF, configurationOptions);

    // Compare the old with the new count of sensors
    // if the amount has changed, write new SD card header for the next data
    ConfiguredSensorCount newCounts = DataProcessor::GetConfiguredSensorCount();
    if (memcmp(&oldCounts,&newCounts, sizeof(newCounts)) != 0)
    {
        SDCard::WriteSensorDataTableHeader();    
    }
}

void ConfigurationFiles::WriteInfoFile()
{
    FIL fileHandler;
	if (f_open(&fileHandler, (const TCHAR*)FILENAME_INFOFILE_CONF, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
    {
        const SerialNumber_t number = Motherboard::GetSerialNumber();
        char* serialNumberString = GetSerialNumberString(number);
        char* timmedSerialNumberString = strusertrim(serialNumberString, " 0", 2);
        f_printf(&fileHandler, "Serialnumber: %s\n", timmedSerialNumberString);
        free(timmedSerialNumberString);
        free(serialNumberString);

        f_close(&fileHandler);
    }
}

const std::list<ConfigurationFileOption> ConfigurationFiles::GetOptionsListBasicSetup() const
{
    std::list<ConfigurationFileOption> list = 
    {
        ConfigurationFileOption(
            "Communication",
            {{ "SD",     ConfigurationOption(BoolOption::SaveToSDCard) },
             { "WLAN",   ConfigurationOption(BoolOption::SendWLAN) },
             { "LoRa",   ConfigurationOption(BoolOption::SendLoRa) },
             { "NB-IoT", ConfigurationOption(BoolOption::SendNB_IoT) }}),
        ConfigurationFileOption(
            "WLAN-SSID",
            ConfigurationOption(StringOption::WLAN_SSID)),
        ConfigurationFileOption(
            "WLAN-Password",
            ConfigurationOption(StringOption::WLAN_Password)),
        ConfigurationFileOption(
            "MQTT-Broker",
            ConfigurationOption(StringOption::MQTT_Broker)),
        ConfigurationFileOption(
            "MQTT-Username",
            ConfigurationOption(StringOption::MQTT_Username)),
        ConfigurationFileOption(
            "MQTT-Password",
            ConfigurationOption(StringOption::MQTT_Password)),
		ConfigurationFileOption(
			"MQTT-Topic",
			ConfigurationOption(StringOption::MQTT_Topic)),
        ConfigurationFileOption(
            "NB-IoT-APN",
            ConfigurationOption(StringOption::NBIoT_APN)),
        ConfigurationFileOption(
            "NB-IoT-Network",
            ConfigurationOption(StringOption::NBIoT_Network)),
        ConfigurationFileOption(
            "NB-IoT-DestIP",
            ConfigurationOption(StringOption::NBIoT_DestinationIP)),
        ConfigurationFileOption(
            "NB-IoT-DestPort",
            ConfigurationOption(NumericOption::NBIoT_DestinationPort)),
        ConfigurationFileOption(
            "Server-CA-Certificate",
            ConfigurationOption(StringOption::CA_Certificate), true),
    }; 

    return list;
}

const std::list<ConfigurationFileOption> ConfigurationFiles::GetOptionsListSettings() const
{
    std::list<ConfigurationFileOption> list =
    {
        ConfigurationFileOption(
            "AirTemperature",
            ConfigurationOption(Intervals::AirTemperature)),
        ConfigurationFileOption(
            "AirHumidity",
            ConfigurationOption(Intervals::AirHumidity)),
        ConfigurationFileOption(
            "SoilTemperature",
            ConfigurationOption(Intervals::SoilTemperature)),
        ConfigurationFileOption(
            "SoilMoisture",
            ConfigurationOption(Intervals::SoilMoisture)),
        ConfigurationFileOption(
            "Illuminance",
            ConfigurationOption(Intervals::Illuminance)),
        ConfigurationFileOption(
            "BatteryVoltage",
            ConfigurationOption(Intervals::BatteryVoltage)),
        ConfigurationFileOption(
            "pHValue",
            ConfigurationOption(Intervals::pHValue)),
        ConfigurationFileOption(
            "Transmission",
            ConfigurationOption(Intervals::Sending)),

        ConfigurationFileOption(
            "TimeSource",
            {{ "NTP", ConfigurationOption(BoolOption::SyncTimeNTP) },
             { "GPS", ConfigurationOption(BoolOption::SyncTImeGPS) }}),
        ConfigurationFileOption(
            "TimeURL",
            ConfigurationOption(StringOption::NTPTimeServer)),
        ConfigurationFileOption(
            "TimeSynchronisationInterval",
            ConfigurationOption(Intervals::NTPTimeSync))
    };

    return list;
}

#if DEBUG_CONFIGFILES
/**
 * @brief Dumps the content of the list of options into the log
 */
void ConfigurationFiles::DumpOptionsList(std::list<ConfigurationFileOption>& configurationOptions)
{
    for (auto fileOption : configurationOptions)
    {
        const char* optionString = fileOption.GetFileString();
        auto options = fileOption.GetConfigurationOption();
        if (options.size() == 1) // Can print value directly
        {
            auto option = options[0];
            char* value = option.GetValueAsString();
            LOGV("Config", "%s: %s", optionString, value);
            delete[] value;
        }
        else
        {
            bool first = false;
            char buffer[1000];
            memset(buffer, '\0', sizeof(buffer));

            for (auto option : fileOption.GetConfigurationOption())
            {
                char* value = option.second.GetValueAsString();
                bool valueTrue = strcmp(value, "true") == 0;
                if (valueTrue)
                {
                	if (!first)
                	{
                		strcat(buffer, " ");
                	}
                    strcat(buffer, option.first);

                    first = false;
                }
                delete[] value;
            }
            LOGV("Config", "%s: %s", optionString, buffer);
        }
    }
}

/**
 * @brief Dumps the content of all options into the log
 */
void ConfigurationFiles::DumpOptions()
{
    ConfigurationFiles* configFiles = new ConfigurationFiles();

    auto configurationOptions = configFiles->GetOptionsListSettings();
    configFiles->DumpOptionsList(configurationOptions);
    
    configurationOptions = configFiles->GetOptionsListBasicSetup();
    configFiles->DumpOptionsList(configurationOptions);

    delete configFiles;
}
#endif
