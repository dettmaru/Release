#include "SensorDataSet.h"
#include "main.h"
#include "Motherboard.h"
#include "crc.h"
#include "TimeHelper.h"

SensorDataSet::SensorDataSet() :
	airTemperatures(new std::list<uint16_t>()),
	airHumidities(new std::list<uint16_t>()),
	soilTemperatures(new std::list<uint16_t>()),
	soilMoistures(new std::list<uint16_t>()),
	illuminances(new std::list<uint16_t>()),
	pHValues(new std::list<uint16_t>()),
	batteryVoltages(new std::list<uint16_t>())
{
	timeStamp.tm_hour = 0;
	timeStamp.tm_min = 0;
	timeStamp.tm_sec = 0;
	timeStamp.tm_year = 30;
	timeStamp.tm_mon = 0;
	timeStamp.tm_mday = 0;
	timeStamp.tm_isdst = 0;
}

SensorDataSet::SensorDataSet(uint8_t* data, uint16_t length) : SensorDataSet()
{
	InterpretMessage(data, length);
}

SensorDataSet::~SensorDataSet()
{
	delete airTemperatures;
	delete airHumidities;
	delete soilTemperatures;
	delete soilMoistures;
	delete illuminances;
	delete batteryVoltages;
	delete pHValues;
}

/**
 * @brief Returns the timestamp of the sensor data set
 */
struct tm SensorDataSet::GetTimestamp() const
{
    return timeStamp;
}

/**
 * @brief Returns the list of saved air temperatures in the sensor data set
 */
const std::list<uint16_t>* SensorDataSet::GetAirTemperatures() const
{
    return airTemperatures;
}

/**
 * @brief Returns the list of saved air humidities in the sensor data set
 */
const std::list<uint16_t>* SensorDataSet::GetAirHumidities() const
{
    return airHumidities;
}

/**
 * @brief Returns the list of soil temperatures in the sensor data set
 */
const std::list<uint16_t>* SensorDataSet::GetSoilTemperatures() const
{
    return soilTemperatures;
}

/**
 * @brief Returns the soil moistures in the sensor data set
 */
const std::list<uint16_t>* SensorDataSet::GetSoilMoistures() const
{
    return soilMoistures;
}

/**
 * @brief Returns the illuminances in the sensor data set
 */
const std::list<uint16_t>* SensorDataSet::GetIlluminances() const
{
    return illuminances;
}

/**
 * @brief Returns the pH values in the sensor data set
 */
const std::list<uint16_t>* SensorDataSet::GetpHValues() const
{
	return pHValues;
}

/**
 * @brief Returns the battery voltages in the sensor data set
 */
const std::list<uint16_t>* SensorDataSet::GetBatteryVoltages() const
{
	return batteryVoltages;
}


/**
 * In the following all getters as float value:
 */

/**
 * @brief Returns all air temperature values as float values in °C
 * 
 * @return std::list<float>* Pointer to a new list of float values. The class behind this pointer must be deleted, if not needed any more!
 */
std::list<float>* SensorDataSet::GetAirTemperaturesAsFloat() const
{
	std::list<float>* values = new std::list<float>();

	for (auto temperature : *airTemperatures)
	{
		values->push_back(AirTemperatureMEMToFloat(temperature));
	}

	return values;
}

/**
 * @brief Returns all air humidity values as float values in percent
 * 
 * @return std::list<float>* Pointer to a new list of float values. The class behind this pointer must be deleted, if not needed any more!
 */
std::list<float>* SensorDataSet::GetAirHumiditiesAsFloat() const
{
	std::list<float>* values = new std::list<float>();

	for (auto humidity : *airHumidities)
	{
		values->push_back(AirHumidityMEMToFloat(humidity));
	}

	return values;
}

/**
 * @brief Returns all soil temperature values as float values in °C
 * 
 * @return std::list<float>* Pointer to a new list of float values. The class behind this pointer must be deleted, if not needed any more!
 */
std::list<float>* SensorDataSet::GetSoilTemperaturesAsFloat() const
{
	std::list<float>* values = new std::list<float>();

	for (auto temperature : *soilTemperatures)
	{
		values->push_back(SoilTemperatureMEMToFloat(temperature));
	}

	return values;
}

/**
 * @brief Returns all soil moisture values as float values in percent
 * 
 * @return std::list<float>* Pointer to a new list of float values. The class behind this pointer must be deleted, if not needed any more!
 */
std::list<float>* SensorDataSet::GetSoilMoisturesAsFloat() const
{
	std::list<float>* values = new std::list<float>();

	for (auto moisture : *soilMoistures)
	{
		values->push_back(SoilMoistureMEMToFloat(moisture));
	}

	return values;
}

/**
 * @brief Returns all illuminances values as float values in ??? (this needs definition)
 * 
 * @return std::list<float>* Pointer to a new list of float values. The class behind this pointer must be deleted, if not needed any more!
 */
std::list<float>* SensorDataSet::GetIlluminancesAsFloat() const
{
	std::list<float>* values = new std::list<float>();

	for (auto illuminance : *illuminances)
	{
		values->push_back(IlluminanceMEMToFloat(illuminance));
	}

	return values;
}

/**
 * @brief Returns all pH values as float values 
 * 
 * @return std::list<float>* Pointer to a new list of float values. 
 * The class behind this pointer must be deleted, if not needed anymore!
 */
std::list<float>* SensorDataSet::GetpHValuesAsFloat() const
{
	std::list<float>* values = new std::list<float>();

	for (auto pH : *pHValues)
	{
		values->push_back(pHValueMEMToFloat(pH));
	}

	return values;
}

/**
 * @brief Returns the battery voltages in this SensorDataSet. The values are provided in volt.
 * 
 * @return Battery voltages in volt.
 */
std::list<float>* SensorDataSet::GetBatteryVoltagesAsFloat() const
{
	std::list<float>* values = new std::list<float>();

	for (auto batteryVoltage : *batteryVoltages)
	{
		values->push_back(BatteryVoltageMEMToFloat(batteryVoltage));
	}

	return values;
}

/**
 * In the following all setters for values are placed:
 */

/**
 * @brief Sets the time stamp with an time_t value, that 
 * 		  counts the seconds since 1970.
 */
void SensorDataSet::SetTimestamp(time_t time)
{
	struct tm* t = gmtime(&time);
	timeStamp = *t;
}

/**
 * @brief Sets the time stamp with an struct tm value, 
 * 		  that has an "human" readable time since 1970
 */
void SensorDataSet::SetTimestamp(const struct tm* time)
{
	timeStamp = *time;
}

/**
 * @brief Adds an air temperature value into the sensor data set. 
 * 
 * The value will be added at the end of the list. 
 */
void SensorDataSet::AddAirTemperature(float value)
{
	airTemperatures->push_back(AirTemperatureFloatToMEM(value));
}

/**
 * @brief Adds an air humidity value into the sensor data set.
 * 
 * The value will be added at the end of the list.
 */
void SensorDataSet::AddAirHumidity(float value)
{
	airHumidities->push_back(AirHumidityFloatToMEM(value));
}

/**
 * @brief Adds a soil temperature value into the sensor data set
 * 
 * The value will be added at the end of the list.
 */
void SensorDataSet::AddSoilTemperature(float value)
{
	soilTemperatures->push_back(SoilTemperatureFloatToMEM(value));
}

/**
 * @brief Add a soil moisture value into the sensor data set
 * 
 * The value will be added at the end of the list.
 */
void SensorDataSet::AddSoilMoisture(float value)
{
	soilMoistures->push_back(SoilMoistureFloatToMEM(value));
}

/**
 * @brief Add an illuminance value into the sensor data set
 * 
 * The value will be added at the end of the list.
 */
void SensorDataSet::AddIlluminance(float value)
{
	illuminances->push_back(IlluminanceFloatToMEM(value));
}

/**
 * @brief Add an pH value into the sensor data set
 * 
 * The value will be added at the end of the list.
 */
void SensorDataSet::AddpHValue(float value)
{
	pHValues->push_back(pHValueFloatToMEM(value));
}

/**
 * @brief Add a battery voltage in the sensor data set
 * 
 * The value will be added at the end of the list.
 */
void SensorDataSet::AddBatteryVoltage(float voltage)
{
	batteryVoltages->push_back(BatteryVoltageFloatToMEM(voltage));
}

/**
 * In the following all static conversions from Memory(MEM) representation to float is done
 */

/**
 * @brief Converts the memory representation of the air temperature into a float value in °C
 * 
 * @param value in memory representation 
 * @return value in float representation
 */
float SensorDataSet::AirTemperatureMEMToFloat(uint16_t value)
{
	return (((int32_t)value) - 15000) / 500.0;
}

/**
 * @brief Converts the memory representation of the air humidity into a float value in percent
 * 
 * @param value in memory representation 
 * @return value in float representation
 */
float SensorDataSet::AirHumidityMEMToFloat(uint16_t value)
{
	return value / 655.34;
}

/**
 * @brief Converts the memory representation of the soil temperature into a float value in °C
 * 
 * @param value in memory representation 
 * @return value in float representation
 */
float SensorDataSet::SoilTemperatureMEMToFloat(uint16_t value)
{
	return (((int32_t)value) - 15000) / 500.0;
}

/**
 * @brief Converts the memory representation of the soil moisture into a float value in percent
 * 
 * @param value in memory representation 
 * @return value in float representation
 */
float SensorDataSet::SoilMoistureMEMToFloat(uint16_t value)
{
	//return value / 655.34;
	return value;
}

/**
 * @brief Converts the memory representation of the illuminance into a float value in ???
 * 
 * @param value in memory representation 
 * @return value in float representation
 */
float SensorDataSet::IlluminanceMEMToFloat(uint16_t value)
{
	return value;
}

/**
 * @brief Converts the memory representation of the pH value into a float value
 * 
 * @param value in memory representation
 * @return float in float representation
 */
float SensorDataSet::pHValueMEMToFloat(uint16_t value)
{
	return (uint32_t)value / 20000.0f;
}

/**
 * @brief Converts the memory representation of the battery voltage into a float value in volt
 * 
 * @param value in memory representation 
 * @return value in float representation
 */
float SensorDataSet::BatteryVoltageMEMToFloat(uint16_t value)
{
	return value / 1000.0;
}

/**
 * In the following all static conversions from float to Memory(MEM) representation is done
 */

/**
 * @brief Converts the float representation of the air temperature in °C into the memory representation
 * 
 * @param value in float representation
 * @return uint16_t in memory representation
 */
uint16_t SensorDataSet::AirTemperatureFloatToMEM(float value)
{
	return (uint16_t)((int16_t)(value * 500) + 15000);
}

/**
 * @brief Converts the float representation of the air humidity in percent into the memory representation
 * 
 * @param value in float representation
 * @return uint16_t in memory representation
 */
uint16_t SensorDataSet::AirHumidityFloatToMEM(float value)
{
	return (uint16_t)(value * 655.34);
}

/**
 * @brief Converts the float representation of the soil temperature in °C into the memory representation
 * 
 * @param value in float representation
 * @return uint16_t in memory representation
 */
uint16_t SensorDataSet::SoilTemperatureFloatToMEM(float value)
{
	return (uint16_t)((int16_t)(value * 500) + 15000);
}

/**
 * @brief Converts the float representation of the soil moisture in percent into the memory representation
 * 
 * @param value in float representation
 * @return uint16_t in memory representation
 */
uint16_t SensorDataSet::SoilMoistureFloatToMEM(float value)
{
	//return (uint16_t)(value * 655.34);
	return (uint16_t)value;
}

/**
 * @brief Converts the float representation of the illuminance in ??? into the memory representation
 * 
 * @param value in float representation
 * @return uint16_t in memory representation
 */
uint16_t SensorDataSet::IlluminanceFloatToMEM(float value)
{
	return (uint16_t)value;
}

/**
 * @brief Converts the float representation of the pH value into the memory representation
 * 
 * @param value in float representation
 * @return uint16_t in memory representation
 */
uint16_t SensorDataSet::pHValueFloatToMEM(float value)
{
	return (uint16_t)(value * 20000);
}

/**
 * @brief Converts the float representation of the battery voltage in volt into the memory representation
 * 
 * @param value in float representation
 * @return uint16_t in memory representation
 */
uint16_t SensorDataSet::BatteryVoltageFloatToMEM(float value)
{
	return (uint16_t)(value * 1000);
}

extern CRC_HandleTypeDef hcrc;

#define ADD_PRESENCE_BIT(bits, current) {bits |= (1<<current);}

/**
 * @brief Serializes the content of the current message. The result can be used 
 * a content of the transmitted message to the web service.
 */
uint8_t* SensorDataSet::GetMQTTSensorDataContent(uint16_t* size) const
{
	const SensorDataSet* dataSet = this;
	uint8_t dataBits1 = 0;
	uint8_t currentVectorIndex = 0;
	uint8_t countVector[8] = {0};
	uint16_t countOfDataBytes = 0;
	if (!dataSet->GetAirTemperatures()->empty())
	{
		ADD_PRESENCE_BIT(dataBits1, 7)
		countVector[currentVectorIndex++] = dataSet->GetAirTemperatures()->size();
	}
	if (!dataSet->GetAirHumidities()->empty())
	{
		ADD_PRESENCE_BIT(dataBits1, 6)
		countVector[currentVectorIndex++] = dataSet->GetAirHumidities()->size();
	}
	if (!dataSet->GetSoilTemperatures()->empty())
	{
		ADD_PRESENCE_BIT(dataBits1, 5)
		countVector[currentVectorIndex++] = dataSet->GetSoilTemperatures()->size();
	}
	if (!dataSet->GetSoilMoistures()->empty())
	{
		ADD_PRESENCE_BIT(dataBits1, 4)
		countVector[currentVectorIndex++] = dataSet->GetSoilMoistures()->size();
	}
	if (!dataSet->GetIlluminances()->empty())
	{
		ADD_PRESENCE_BIT(dataBits1, 3)
		countVector[currentVectorIndex++] = dataSet->GetIlluminances()->size();
	}
	if (!dataSet->GetBatteryVoltages()->empty())
	{
		ADD_PRESENCE_BIT(dataBits1, 2);
		countVector[currentVectorIndex++] = dataSet->GetBatteryVoltages()->size();
	}
	if (!dataSet->GetpHValues()->empty())
	{
		ADD_PRESENCE_BIT(dataBits1, 1)
		countVector[currentVectorIndex++] = dataSet->GetpHValues()->size();
	}

	uint8_t countOfDiffData = __builtin_popcount(dataBits1);
	for (uint8_t i = 0; i < countOfDiffData; i++)
	{
		countOfDataBytes += 2 * countVector[i];
	}

	uint16_t bufferSize = 1 + 4 + 3*4 + countOfDiffData + countOfDataBytes + 4;
	uint8_t* buffer = new uint8_t[bufferSize]();
	*size = bufferSize;

	buffer[0] = dataBits1;
	for (uint8_t i = 0; i < countOfDiffData; i++)
	{
		buffer[1 + i] = countVector[i];
	}

	uint8_t* bufferCurrent = buffer + 1 + countOfDiffData;

	// Set time stamp bytes in message buffer
	struct tm timestamp = dataSet->GetTimestamp();
	time_t time = Time::timegm_uc(&timestamp); //This will generate a problem on January 19, 2038
	//time_t time = mktime(&timestamp);


	//If processor is LITTLE_ENDIAN this needs to be commented
	time = __builtin_bswap32((uint32_t)(time & 0xFFFFFFFF));

	for (int i = 0; i < 4; i++)
	{
		bufferCurrent[0] = *(((uint8_t*)(&time)) + i);
		bufferCurrent++;
	}

	const SerialNumber_t serialNumber = Motherboard::GetSerialNumber();
	bufferCurrent[0]  = (serialNumber.high >> 24) & 0xFF;
	bufferCurrent[1]  = (serialNumber.high >> 16) & 0xFF;
	bufferCurrent[2]  = (serialNumber.high >>  8) & 0xFF;
	bufferCurrent[3]  = (serialNumber.high >>  0) & 0xFF;
	bufferCurrent[4]  = (serialNumber.mid  >> 24) & 0xFF;
	bufferCurrent[5]  = (serialNumber.mid  >> 16) & 0xFF;
	bufferCurrent[6]  = (serialNumber.mid  >>  8) & 0xFF;
	bufferCurrent[7]  = (serialNumber.mid  >>  0) & 0xFF;
	bufferCurrent[8]  = (serialNumber.low  >> 24) & 0xFF;
	bufferCurrent[9]  = (serialNumber.low  >> 16) & 0xFF;
	bufferCurrent[10] = (serialNumber.low  >>  8) & 0xFF;
	bufferCurrent[11] = (serialNumber.low  >>  0) & 0xFF;
	bufferCurrent += 12;

	// Set sensor data in the message buffer
	if (!dataSet->GetAirTemperatures()->empty())
	{
		for (auto temperature : *(dataSet->GetAirTemperatures()))
		{
			bufferCurrent[0] = (temperature >> 8) & 0xFF;
			bufferCurrent[1] = (temperature >> 0) & 0xFF;
			bufferCurrent += 2;
		}
	}
	if (!dataSet->GetAirHumidities()->empty())
	{
		for (auto humidity : *(dataSet->GetAirHumidities()))
		{
			bufferCurrent[0] = (humidity >> 8) & 0xFF;
			bufferCurrent[1] = (humidity >> 0) & 0xFF;
			bufferCurrent += 2;
		}
	}
	if (!dataSet->GetSoilTemperatures()->empty())
	{
		for (auto temperature : *(dataSet->GetSoilTemperatures()))
		{
			bufferCurrent[0] = (temperature >> 8) & 0xFF;
			bufferCurrent[1] = (temperature >> 0) & 0xFF;
			bufferCurrent += 2;
		}
	}
	if (!dataSet->GetSoilMoistures()->empty())
	{
		for (auto moisture : *(dataSet->GetSoilMoistures()))
		{
			bufferCurrent[0] = (moisture >> 8) & 0xFF;
			bufferCurrent[1] = (moisture >> 0) & 0xFF;
			bufferCurrent += 2;
		}
	}
	if (!dataSet->GetIlluminances()->empty())
	{
		for (auto illuminance : *(dataSet->GetIlluminances()))
		{
			bufferCurrent[0] = (illuminance >> 8) & 0xFF;
			bufferCurrent[1] = (illuminance >> 0) & 0xFF;
			bufferCurrent += 2;
		}
	}
	if (!dataSet->GetBatteryVoltages()->empty())
	{
		for (auto batteryVoltage : *(dataSet->GetBatteryVoltages()))
		{
			bufferCurrent[0] = (batteryVoltage >> 8) & 0xFF;
			bufferCurrent[1] = (batteryVoltage >> 0) & 0xFF;
			bufferCurrent += 2;
		}
	}

	if (!dataSet->GetpHValues()->empty())
	{
		for (auto pH : *(dataSet->GetpHValues()))
		{
			bufferCurrent[0] = (pH >> 8) & 0xFF;
			bufferCurrent[1] = (pH >> 0) & 0xFF;
			bufferCurrent += 2;
		}
	}

	MX_CRC_Init();

	// Calculate the CRC for this message
	int crcLength = (bufferSize - 1) / 4;
	volatile uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)buffer, crcLength);
	crc ^= 0xFFFFFFFF;

	bufferCurrent[0] = (crc >> 24) & 0xFF;
	bufferCurrent[1] = (crc >> 16) & 0xFF;
	bufferCurrent[2] = (crc >>  8) & 0xFF;
	bufferCurrent[3] = (crc >>  0) & 0xFF;

	return buffer;
}


#define ADD_16BIT_VALUE(countVar, vectorVar, ptrCurrent) { \
	for (uint16_t i = 0; i < countVar; i++) \
	{ \
		vectorVar->push_back((((uint8_t)ptrCurrent[0]) << 8) | ptrCurrent[1]); \
		ptrCurrent += 2; \
	}}

/**
 * @brief Interpret serialized message and load the data into the current instance
 */
void SensorDataSet::InterpretMessage(uint8_t* data, uint16_t length)
{
	uint8_t byte1 = data[0];
	uint8_t countAirTemperatures = 0;
	uint8_t countAirHumidities = 0;
	uint8_t countSoilTemperatures = 0;
	uint8_t countSoilMoistures = 0;
	uint8_t countIlluminances = 0;
	uint8_t countBatteryVoltages = 0;
	uint8_t countpHValues = 0;

	uint8_t* ptrCurrent = data + 1;
	if (byte1 & (1<<7))
	{
		countAirTemperatures = *ptrCurrent;
		ptrCurrent++;
	}
	if (byte1 & (1<<6))
	{
		countAirHumidities = *ptrCurrent;
		ptrCurrent++;
	}
	if (byte1 & (1<<5))
	{
		countSoilTemperatures = *ptrCurrent;
		ptrCurrent++;
	}
	if (byte1 & (1<<4))
	{
		countSoilMoistures = *ptrCurrent;
		ptrCurrent++;
	}
	if (byte1 & (1<<3))
	{
		countIlluminances = *ptrCurrent;
		ptrCurrent++;
	}
	if (byte1 & (1<<2))
	{
		countBatteryVoltages = *ptrCurrent;
		ptrCurrent++;
	}
	if (byte1 & (1<<1))
	{
		countpHValues = *ptrCurrent;
		ptrCurrent++;
	}

	time_t time = (((time_t)ptrCurrent[0]) << 24) |
				  (((time_t)ptrCurrent[1]) << 16) |
				  (((time_t)ptrCurrent[2]) << 8) |
				   ((time_t)ptrCurrent[3]);

	SetTimestamp(time);

	ptrCurrent += 4+12; // Time byte count + Serial number byte count

	ADD_16BIT_VALUE(countAirTemperatures, airTemperatures, ptrCurrent);
	ADD_16BIT_VALUE(countAirHumidities, airHumidities, ptrCurrent);
	ADD_16BIT_VALUE(countSoilTemperatures, soilTemperatures, ptrCurrent);
	ADD_16BIT_VALUE(countSoilMoistures, soilMoistures, ptrCurrent);
	ADD_16BIT_VALUE(countIlluminances, illuminances, ptrCurrent);
	ADD_16BIT_VALUE(countBatteryVoltages, batteryVoltages, ptrCurrent);
	ADD_16BIT_VALUE(countpHValues, pHValues, ptrCurrent);
	return;
}

/**
 * @brief Compare operator. The inner data will be compared for equality, not 
 *        the pointers!
 */
bool SensorDataSet::operator==(const SensorDataSet &b) const
{
	if (timeStamp.tm_year == b.timeStamp.tm_year &&
		timeStamp.tm_mon  == b.timeStamp.tm_mon &&
		timeStamp.tm_mday == b.timeStamp.tm_mday &&
		timeStamp.tm_hour == b.timeStamp.tm_hour &&
		timeStamp.tm_min  == b.timeStamp.tm_min &&
		timeStamp.tm_sec  == b.timeStamp.tm_sec &&
	    *airTemperatures  == *b.airTemperatures &&
	    *airHumidities    == *b.airHumidities &&
	    *soilTemperatures == *b.soilTemperatures &&
	    *soilMoistures    == *b.soilMoistures &&
		*illuminances     == *b.illuminances &&
		*pHValues         == *b.pHValues /*&&
		*batteryVoltages  == *b.batteryVoltages*/)
	{
		return true;
	}
	return false;
}
