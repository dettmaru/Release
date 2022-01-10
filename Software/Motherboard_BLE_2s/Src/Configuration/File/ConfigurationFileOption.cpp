/**
 * @brief Configuration option connection between FLASH memory and SD card, etc.
 * 
 * @file ConfigurationFileOption.h
 * @author Daniel Lohmann
 * @date 2019-03-13
 */
#include "ConfigurationFileOption.h"
#include "string.h"

ConfigurationFileOption::ConfigurationFileOption(
    const char* fileString,
    ConfigurationOption option) :
    fileString(fileString),
    configurationOptions({{ nullptr, option }}),
    multiline(false)
{
    
}

ConfigurationFileOption::ConfigurationFileOption(
    const char* fileString,
    ConfigurationOption option,
    bool multiline) :
    fileString(fileString),
    configurationOptions({{ nullptr, option }}),
    multiline(multiline)
{
    
}

ConfigurationFileOption::ConfigurationFileOption(
    const char* fileString, 
    std::map<const char*, ConfigurationOption> options) :
    fileString(fileString),
    configurationOptions(options),
    multiline(false)
{
    
}

ConfigurationFileOption::ConfigurationFileOption(const ConfigurationFileOption& option)
{
    this->fileString = option.fileString;
    this->configurationOptions = option.configurationOptions;
    this->multiline = option.multiline;
}

ConfigurationFileOption::~ConfigurationFileOption()
{
    
}


/**
 * @brief Name of option in file
 */
const char* ConfigurationFileOption::GetFileString()
{
    return this->fileString;
}

/**
 * @brief List of configuration options set by the value in the file, multiple e.g. by option based configuration settings 
 * The string is empty if the value should be set by the value of the configuration setting
 */
std::map<const char*, ConfigurationOption> ConfigurationFileOption::GetConfigurationOption()
{
    return this->configurationOptions;
}

/**
 * @brief Parameter value maybe multiline, would be needed for certificate
 */
bool ConfigurationFileOption::GetMultiline()
{
    return this->multiline;
}

/**
 * @brief Returns true if this option contains named parameters
 */
bool ConfigurationFileOption::NamedOptions()
{
    bool hasNamedOptions = false;
    for (auto option : configurationOptions)
    {
        if (option.first != nullptr && strcmp(option.first, "") != 0)
        {
            hasNamedOptions = true;
        }
    }
    return hasNamedOptions;
}
