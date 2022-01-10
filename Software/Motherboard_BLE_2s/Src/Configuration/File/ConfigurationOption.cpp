/**
 * @brief Configuration option connection between index value and the type of the index
 * 
 * @file ConfigurationOption.h
 * @author Daniel Lohmann
 * @date 2019-03-13
 */
#include "ConfigurationOption.h"
#include "Configuration.h"
#include "main.h"
#include "string.h"
#include "stdlib.h"
#include "errno.h"
#include "limits.h"

ConfigurationOption::ConfigurationOption()
{
	index.boolOption = (BoolOption)0;
	type = (ConfigurationOptionType)INT_MAX;
}

ConfigurationOption::ConfigurationOption(StringOption option) :
    index({ .stringOption = option }),
    type(ConfigurationOptionType::String)
{

}

ConfigurationOption::ConfigurationOption(BoolOption option)
{
	index.boolOption = option;
	type = ConfigurationOptionType::Bool;
}

ConfigurationOption::ConfigurationOption(NumericOption option)
{
	index.numericOption = option;
	type = ConfigurationOptionType::Numeric;
}

ConfigurationOption::ConfigurationOption(Intervals option)
{
	index.intervalOption = option;
	type = ConfigurationOptionType::Interval;
}

void ConfigurationOption::SetValue(const char* string)
{
	if (string == nullptr || string == NULL)
	{
		return;
	}

	char* end = nullptr;
	long value = 0;
	switch(type)
	{
		case ConfigurationOptionType::Bool:
			if (strcmp(string, "true") == 0)
			{
				Configuration::SetBool(index.boolOption, true);
			}
			else if(strcmp(string, "false") == 0)
			{
				Configuration::SetBool(index.boolOption, false);
			}
			else
			{
				// TODO default customizable ???
				Configuration::SetBool(index.boolOption, false);
			}
			break;
		case ConfigurationOptionType::Interval:
			value = strtol(string, &end, 10);
			if (errno == ERANGE)
			{
				// TODO set default value
				errno = 0;
			}
			else
			{
				// TODO limit value range
				Configuration::SetInterval(index.intervalOption, (Interval_t)value);
			}
			break;
		case ConfigurationOptionType::Numeric:
			value = strtol(string, &end, 10);
			if (errno == ERANGE)
			{
				// TODO set default value
				errno = 0;
			}
			else
			{
				// TODO limit value range
				Configuration::SetNumber(index.numericOption, (Interval_t)value);
			}
			break;
		case ConfigurationOptionType::String:
			Configuration::SetString(index.stringOption, (uint8_t*)string, strlen(string));
			break;
		default:
			_Error_Handler(__FILE__, __LINE__);
			break;
	}
}

void ConfigurationOption::SetValue(bool value)
{
	switch (type)
	{
		case ConfigurationOptionType::Bool:
			Configuration::SetBool(index.boolOption, value);
			break;
		default:
			_Error_Handler(__FILE__, __LINE__);
			break;
	}
}

/** 
 * @brief Converts the current configuration option value into a string
 * @remark The returned array needs to be deleted after use
 */
char* ConfigurationOption::GetValueAsString()
{
	char buffer[20];
	char* retString = nullptr;
	switch (type)
	{
		case ConfigurationOptionType::Bool:
			if (Configuration::GetBool(index.boolOption))
			{
				retString = strdup("true");
			}
			else
			{
				retString = strdup("false");
			}
			break;
		case ConfigurationOptionType::Interval:
			snprintf(buffer, sizeof(buffer), "%d", 
				(int)Configuration::GetInterval(index.intervalOption));
			retString = strdup(buffer);
			break;
		case ConfigurationOptionType::Numeric:
			snprintf(buffer, sizeof(buffer), "%d",
				(int)Configuration::GetNumber(index.numericOption));
			retString = strdup(buffer);
			break;
		case ConfigurationOptionType::String:
			retString = (char*)Configuration::GetString(index.stringOption, nullptr);
			break;
		default:
			_Error_Handler(__FILE__, __LINE__);
	}
	if (retString == nullptr)
	{
		retString = strdup("");
	}
	return retString;
}
