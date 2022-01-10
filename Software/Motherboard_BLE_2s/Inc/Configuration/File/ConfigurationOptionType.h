/**
 * @brief List of configuration option index types
 * 
 * @file ConfigurationOptionType.h
 * @author Daniel Lohmann
 * @date 2019-03-13
 */
#ifndef __CONFIGURATION_OPTION_TYPE_H_
#define __CONFIGURATION_OPTION_TYPE_H_

enum class ConfigurationOptionType
{
    String,
    Bool,
    Numeric,
    Interval,
};

#endif /* __CONFIGURATION_OPTION_TYPE_H_ */