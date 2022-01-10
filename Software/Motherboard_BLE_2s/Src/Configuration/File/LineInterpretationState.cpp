/**
 * @brief Line interpretation state storage class
 *
 * @file LineInterpretationState.cpp
 * @author Daniel Lohmann
 * @date 2020-03-21
 */
#include "LineInterpretationState.h"
#include "string.h"

LineInterpretationState::LineInterpretationState() :
	currentMultilineOption(nullptr),
	multilineStringBuffer(nullptr)
{

}

void LineInterpretationState::AddStringToBuffer(char* string)
{
	int currentLength = 0;
	if (multilineStringBuffer != nullptr)
	{
		currentLength = strlen(this->multilineStringBuffer);
	}
	int additionalLength = strlen(string);
	int newBufferLength = currentLength + additionalLength + 1;

	char* newBuffer = new char[newBufferLength];
	if (multilineStringBuffer != nullptr)
	{
		memcpy(newBuffer, this->multilineStringBuffer, currentLength);
	}
	memcpy(newBuffer + currentLength, string, additionalLength);
	newBuffer[newBufferLength - 1] = 0;

	delete[] this->multilineStringBuffer;
	this->multilineStringBuffer = newBuffer;
}

void LineInterpretationState::ClearStringBuffer()
{
	if (multilineStringBuffer != nullptr)
	{
		delete[] multilineStringBuffer;
		multilineStringBuffer = nullptr;
	}
}

char* LineInterpretationState::GetStringBuffer()
{
	return multilineStringBuffer;
}
