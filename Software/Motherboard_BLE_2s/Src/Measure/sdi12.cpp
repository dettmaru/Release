/*
 * sdi12.cpp
 *
 *  Created on: Jan 7, 2019
 *      Author: Daniel Lohmann
 */
#include "sdi12.h"
#include "Energy.h"
#include "HardwareSemaphores.h"
#include <algorithm>

/*
 * @brief Shared SDI12 instance
 */
SDI12* sdi12;

void SDI12::Init()
{
	sdi12 = this;
	TurnON();

	for (auto sensor : sensors)
	{
		 delete sensor;
	}
	sensors.clear();
}

void SDI12::DeInit()
{
	sdi12 = nullptr;
	TurnOFF();
	for (auto sensor : sensors)
	{
		delete sensor;
	}
	sensors.clear();
}

/**
 * @brief Turns on the power for the SDI12 sensors and the level shifting circuit
 */
void SDI12::TurnON()
{
	sdi12hardware.Request();
}

/**
 * @brief Turns off the power for the SDI12 sensors and the level shifting circuit
 */
void SDI12::TurnOFF()
{
	// TODO maybe standby pullup's and pulldown's are needed
	sdi12hardware.Free();
}

void SDI12::StartMeasurement()
{
	Init();
	StartReceiving();
	sensors.push_back(new SDI12_Sensor('0', SDI12_SENSOR_TYPE_ECOTECH_HYDRAPROBE));
}

std::vector<float> SDI12::SimpleMeasurement()
{
	HAL_Delay(1000);
	//char* command = SDI12_CMD_ADDITIONAL_MEASURE9;
	char* command = SDI12_CMD_START_MEASURE;
	char commandBuffer[100];
	char buffer[100];

	//sprintf(commandBuffer, SDI12_CMD_ID_SERN, "0");
	//Command('0', (uint8_t*)commandBuffer, (uint8_t*)buffer, 100);

	//HAL_Delay(2000);

	sprintf(commandBuffer, command, "0");
	Command('0', (uint8_t*)commandBuffer, (uint8_t*)buffer, 100);

	uint8_t amount = 0;
	uint16_t waittime = 0;
	ExtractWaitTimeAndAmountOfValues((uint8_t*)buffer, &waittime, &amount);

	HAL_Delay(waittime * 1000);

	sprintf(commandBuffer, SDI12_CMD_GET_DATA0, "0");
	Command('0', (uint8_t*)commandBuffer, (uint8_t*)buffer, 100);

	auto values = ExtractValues((uint8_t*)buffer);

	return values;
}

bool SDI12::ProcessMeasurement()
{
	char commandBuffer[100];
	char buffer[100];
	bool running = false;
	for (auto sensor : sensors)
	{
		if (sensor->state != SDI12_SensorState::Finished)
		{
			running = true;
		}
		switch (sensor->state)
		{
		case SDI12_SensorState::OFF:
			sensor->waitBegin = HAL_GetTick();
			sensor->waitTime = 1000; // Sensors have 100ms to wake up
			sensor->state = SDI12_SensorState::Init;
			// TODO Pin state changes after StopMode??????
			// Some other peripheral doesn this!!!!
			break;
		case SDI12_SensorState::Init:
			if (HAL_GetTick() > sensor->waitBegin + sensor->waitTime)
			{
				sprintf(commandBuffer, SDI12_CMD_ID_SERN, "0");
				Command('0', (uint8_t*)commandBuffer, (uint8_t*)buffer, 100);


				sensor->state = SDI12_SensorState::Measure;

				// Send measurement command and receive wait time
				sprintf(commandBuffer, SDI12_CMD_START_MEASURE, "0");
				Command('0', (uint8_t*)commandBuffer, (uint8_t*)buffer, 100);

				uint8_t amount = 0;
				uint16_t waittime = 0;
				ExtractWaitTimeAndAmountOfValues((uint8_t*)buffer, &waittime, &amount);

				sensor->waitBegin = HAL_GetTick();
				sensor->waitTime = waittime * 1000;
				sensor->expectedAmount = amount;
			}
			break;
		case SDI12_SensorState::Measure:
			if (HAL_GetTick() > sensor->waitBegin + sensor->waitTime)
			{
				char* getDataCommands[10] = {
					SDI12_CMD_GET_DATA0,
					SDI12_CMD_GET_DATA1,
					SDI12_CMD_GET_DATA2,
					SDI12_CMD_GET_DATA3,
					SDI12_CMD_GET_DATA4,
					SDI12_CMD_GET_DATA5,
					SDI12_CMD_GET_DATA6,
					SDI12_CMD_GET_DATA7,
					SDI12_CMD_GET_DATA8,
					SDI12_CMD_GET_DATA9
				};

				int dataRequestIndex = 0;
				std::vector<float> values;
				std::vector<float> receivedValues;
				do
				{
					// Send data retrieve command and receive values
					// Do this a long as the expected amount of values have been received
					// or the data message returns no values
					sprintf(commandBuffer, getDataCommands[dataRequestIndex], "0");
					Command('0', (uint8_t*)commandBuffer, (uint8_t*)buffer, 100);
					receivedValues = ExtractValues((uint8_t*)buffer);
					values.insert(values.end(), receivedValues.begin(), receivedValues.end());
					dataRequestIndex++;
				} while ((dataRequestIndex <= 9) &&
						 (values.size() < sensor->expectedAmount) &&
						 (receivedValues.size() > 0));

				// save values for later use
				sensor->values = values;
				sensor->state = SDI12_SensorState::Finished;
			}
			break;
		case SDI12_SensorState::Finished:
			// Do nothing
			break;
		}
	}
	if (!running)
	{
		StopReceiving();
		TurnOFF();
	}
	return running;
}

/**
 * Sends a SDI12 command
 * @param dataIn String which contains the SDI!2 command
 * @return Returns HAL_OK is the function was successful and HAL_ERROR in
 * case of an EROR with the peripherals or other errors
 */
uint32_t SDI12::Command(uint8_t address, uint8_t* dataIn)
{
	//int len = strlen((char*)dataIn);

/*

	// A command message contains the following bit flow
	//

	SDI12UART.Init.BaudRate = 1200*8.3/12.5*0.85;
	HAL_UART_Init(&SDI12UART);

	HAL_Delay(20);

	HAL_GPIO_WritePin(SDI12DIR_GPIO_Port, SDI12DIR_Pin, GPIO_PIN_RESET); // set outgoing

	// send break
	if (HAL_LIN_SendBreak(&SDI12UART) != HAL_OK)
	{
		return HAL_ERROR;
	}

	// set marking
	HAL_Delay(15); //9ms have been discovered by measurement
	HAL_Delay(20);

	SDI12UART.Init.BaudRate = 1200;
	HAL_UART_Init(&SDI12UART);

	// send message
	// Testweise abgeschaltet
	if (HAL_UART_Transmit(&SDI12UART, dataIn, len, 1000))
	{
		return HAL_ERROR;
	}
*/
	// Testweise abgeschaltet
	//HAL_GPIO_TogglePin(SDI12DIR_GPIO_Port, SDI12DIR_Pin); // set incoming

	return HAL_OK;
}

/**
 * @brief Sends a SDI12 command and receives the answer
 */
uint32_t SDI12::Command(uint8_t address, uint8_t* dataIn, uint8_t* dataOut, uint8_t dataOutLength)
{
	SDI12_Sensor* sensor = nullptr;
	for (auto s : sensors)
	{
		if (s->address == address)
		{
			sensor = s;
			break;
		}
	}
	if (sensor == nullptr)
	{
		return HAL_ERROR;
	}

	sensor->lastResponses.clear();
	sensor->waitingForResponse = true;

	Command(address, dataIn);

	StopReceiving();
	StartReceiving();

	memset(dataOut, '\0', dataOutLength);

	// TODO timeout ???
	while (sensor->waitingForResponse == true) ;
	if (sensor->lastResponses.begin() != sensor->lastResponses.end())
	{
		strcpy((char*)dataOut, (char*)*sensor->lastResponses.begin());
	}
	else
	{
		return HAL_ERROR;
	}

	return HAL_OK;
}

void SDI12::StartReceiving()
{
	SDI12TIMERINIT();
	// The SDI12Timer is used a clock source for the SDI12 communication 
	// time measurement. The timer runs with an interval 4*1200
	HAL_TIM_Base_Start_IT(&SDI12TIMER);
	lastChange = CURRENT_SDI12_TIME;
	lastState = CURRENT_SDI12_RX_STATE();
	currentBit = -1;
	end = false;
	error = false;

	__HAL_TIM_SET_AUTORELOAD(&SDI12TIMER, 600);
}

void SDI12::StopReceiving()
{
	HAL_TIM_Base_Stop_IT(&SDI12TIMER);
}

void SDI12::LineProcessing()
{
	static bool currentlyProcessing = false;
	if (currentlyProcessing)
		return;
	currentlyProcessing = true;
	uint32_t currentTime = CURRENT_SDI12_TIME;
	GPIO_PinState currentState = CURRENT_SDI12_RX_STATE();
	if (lastState != currentState || currentTime - lastChange > 9*4)
	{
		// Check for the start condition!
		if (currentBit == -1 && lastState==GPIO_PIN_SET && currentState==GPIO_PIN_RESET)
		{
			currentBit = 0;
			lastState = currentState;
			lastChange = currentTime;
		}
		else if (currentBit >= 0)
		{
			uint8_t bitCount = (currentTime - lastChange + 1) / 4;
			if (currentTime - lastChange > 14*4)
			{
				// Marking detected?
			}
			if (currentTime - lastChange > 9*4)
			{
				end = true;
			}
			else
			{
				for (uint32_t i = 0; i < bitCount; i++)
				{
					uint16_t byte = currentBit / 10;
					uint8_t bitInByte = currentBit % 10;
					if (bitInByte > 0 && bitInByte < 8) // Handle data bits
					{
						bitInByte -= 1;
						receivedData[byte] |= ((lastState == GPIO_PIN_SET ? 1 : 0) << bitInByte);
					}
					else if (bitInByte == 8) // Check parity
					{
						uint8_t data = receivedData[byte];
						data ^= data >> 4;
						data ^= data >> 2;
						data ^= data >> 1;
						bool partityBitShould = data & 1;

						error |= (lastState == GPIO_PIN_SET) != partityBitShould;
					}
					else if (bitInByte == 0) // Check start bit
					{
						error |= lastState != GPIO_PIN_RESET;
					}
					else if (bitInByte == 9) // Check stop bit
					{
						error |= lastState != GPIO_PIN_SET;
					}
					currentBit++;
				}
			}

			lastState = currentState;
			lastChange = currentTime;
		}
	}
	if (end)
	{
		{
			// TODO process the message
			SaveResponse(receivedData);
			memset(receivedData, '\0', 100);
		}
		// Reset pointers after processing the message
		currentBit = -1;
		error = false;
		end = false;
	}
	__HAL_TIM_SET_AUTORELOAD(&SDI12TIMER, (uint32_t)(currentTime + (10*4)));
	currentlyProcessing = false;
}

void SDI12::SaveResponse(uint8_t* message)
{
	uint8_t* endOfCommand = (uint8_t*)strstr((char*)message, "!"); // Search for a command end
	uint8_t* endOfResponse = (uint8_t*)strstr((char*)message, "\r\n"); // Search for a response end
	if (endOfCommand == NULL)
	{
		// No command was captured, begin of message must be a response
		endOfCommand = message;
	}
	if (endOfResponse == NULL)
	{
		// A non-valid response have been received -> drop it
		return;
	}

	int responseLength = strlen((char*)endOfCommand);
	uint8_t sensorAddress = endOfCommand[0];
	for (auto sensor : sensors)
	{
		if (sensor->address == sensorAddress)
		{
			uint8_t* responseCopy = (uint8_t*)calloc(responseLength + 1, sizeof(uint8_t));
			memcpy(responseCopy, endOfCommand, responseLength);
			sensor->lastResponses.push_back(responseCopy);
			sensor->waitingForResponse = false;
		}
	}
}

bool SDI12::ExtractWaitTimeAndAmountOfValues(uint8_t* message, uint16_t* waitTime, uint8_t* amount)
{
	char buffer[4];
	buffer[0] = (char)message[1];
	buffer[1] = (char)message[2];
	buffer[2] = (char)message[3];
	buffer[3] = '\0';
	*waitTime = (uint16_t)atoi(buffer);
	*amount = message[4] - '0';
	return true;
}

std::vector<float> SDI12::ExtractValues(uint8_t* message)
{
	std::vector<float> values;
	uint8_t buffer[100];
	memset(buffer, '\0', 100);

	////////////////////////////////////////////////////////////////////////
	/// This block is a temporary fix for problem where sometimes
	/// 0o0/000 was received but 0+0.000 was on the data line,
	/// cause is unknown but no other error patterns have been seen!!!
	///
	/// Remove this block is possible!!!
	////////////////////////////////////////////////////////////////////////
	uint8_t* ptr = message;
	while (*ptr != '\r' && *ptr != '\n' && *ptr != '\0')
	{
		if (*ptr == 'o')
		{
			*ptr = '+';
		}
		if (*ptr == '/')
		{
			*ptr = '.';
		}
		ptr++;
	}
	////////////////////////////////////////////////////////////////////////
	/// End of temp fix problem block!!!
	////////////////////////////////////////////////////////////////////////

	uint8_t* currentData = message;
	while (*currentData != '\r' && *currentData != '\n' && *currentData != '\0')
	{
		if (*currentData == '-' || *currentData == '+')
		{
			uint8_t* end = (uint8_t*)strpbrk((char*)(currentData + 1), "+-\r\n");
			if (end == NULL)
			{
				break;
			}
			memcpy(buffer, currentData, end - currentData);
			buffer[end - currentData + 1] = '\0';
			float value = atof((char*)buffer);
			values.push_back(value);
			currentData = end;
		}
		else
		{
			currentData++;
		}
	}
	return values;
}

void SDI12_LineProcessing()
{
	sdi12->LineProcessing();
}
