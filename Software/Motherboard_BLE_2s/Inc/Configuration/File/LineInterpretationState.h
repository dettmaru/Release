/**
 * @brief Line interpretation state storage class
 *
 * @file LineInterpretationState.h
 * @author Daniel Lohmann
 * @date 2020-03-21
 */
#ifndef __CONFIGURATION_LINE_INTERPRETATION_STATE_H_
#define __CONFIGURATION_LINE_INTERPRETATION_STATE_H_

#include "ConfigurationFileOption.h"

class LineInterpretationState
{
public:
	LineInterpretationState();
	ConfigurationFileOption* currentMultilineOption;
	void AddStringToBuffer(char* string);
	void ClearStringBuffer();
	char* GetStringBuffer();
private:
	char* multilineStringBuffer;
};


#endif /* __CONFIGURATION_LINE_INTERPRETATION_STATE_H_ */
