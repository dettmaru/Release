/**
 * @brief Configuration option index combination type
 * 
 * @file ConfigurationOptionIndex.h
 * @author Daniel Lohmann
 * @date 2019-03-13
 */
#ifndef __CONFIGURATION_OPTION_INDEX_H_
#define __CONFIGURATION_OPTION_INDEX_H_

#include "BooleanConfigurationOptions.h"
#include "StringDataConfigurationOptions.h"
#include "NumericConfigurationOptions.h"
#include "Intervals.h"

union ConfigurationOptionIndex
{
    StringOption stringOption;
    BoolOption boolOption;
    NumericOption numericOption;
    Intervals intervalOption;
};

#endif /* __CONFIGURATION_OPTION_INDEX_H_ */