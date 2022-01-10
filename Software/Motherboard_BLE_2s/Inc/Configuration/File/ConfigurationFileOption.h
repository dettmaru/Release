/**
 * @brief Configuration option connection between FLASH memory and SD card, etc.
 * 
 * @file ConfigurationFileOption.h
 * @author Daniel Lohmann
 * @date 2019-03-13
 */
#ifndef __CONFIGURATION_FILE_OPTION_H_
#define __CONFIGURATION_FILE_OPTION_H_

#include <vector>
#include <map>
#include <string>
#include "ConfigurationOption.h"

class ConfigurationFileOption
{
public:
    ConfigurationFileOption(const char* fileString, ConfigurationOption option);
    ConfigurationFileOption(const char* fileString, ConfigurationOption option, bool multiline);
    ConfigurationFileOption(const char* fileString, std::map<const char*, ConfigurationOption> options);
    ConfigurationFileOption(const ConfigurationFileOption& option);
    ~ConfigurationFileOption();
    
    const char* GetFileString();
    std::map<const char*, ConfigurationOption> GetConfigurationOption();
    bool GetMultiline();
    bool NamedOptions();
private:
    const char* fileString;
    std::map<const char*, ConfigurationOption> configurationOptions;
    bool multiline;
};

#endif /* __CONFIGURATION_FILE_OPTION_H_ */