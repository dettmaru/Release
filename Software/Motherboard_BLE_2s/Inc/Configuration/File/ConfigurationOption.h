/**
 * @brief Configuration option connection between index value and the type of the index
 * 
 * @file ConfigurationOption.h
 * @author Daniel Lohmann
 * @date 2019-03-13
 */
#ifndef __CONFIGURATION_OPTION_H_
#define __CONFIGURATION_OPTION_H_

#include "ConfigurationOptionIndex.h"
#include "ConfigurationOptionType.h"

class ConfigurationOption
{
public:
    ConfigurationOption();
    ConfigurationOption(StringOption option);
    ConfigurationOption(BoolOption option);
    ConfigurationOption(NumericOption option);
    ConfigurationOption(Intervals option);

    void SetValue(const char* string);
    void SetValue(bool value);
    char* GetValueAsString();
private:    
    ConfigurationOptionIndex index;
    ConfigurationOptionType type;
};

#endif /* __CONFIGURATION_OPTION_H_ */