#include <ctime>
#include "main.h"
#include "DataProcessor.h"
#include "Motherboard.h"
#include "StandbyRegister.h"
#include "StandbyCache.h"
#include "SDCard.h"
#include "Measure.h"
#include "Energy.h"
#include "TimeHelper.h"
#include "ESP.h"
#include "sdi12.h"
#include "spi.h"
#include "adc.h"
#include "i2c.h"
#include "lptim.h"
#include "Cellular.h"
#include "Extern.h"

DataProcessor* DataProcessor::processor = nullptr;

TSL2591* illuminanceSensor = nullptr;
SHT31* airSensor = nullptr;
SDI12* sdi12Sensors = nullptr;

DataProcessor::DataProcessor() :
	sensorData(nullptr),
	runningAirHumidity(false),
	runningAirTemperature(false),
	runningBatteryVoltage(false),
	runningIlluminance(false),
	runningSoilMoisture(false),
	runningSoilTemperature(false),
	runningSDI12(false),
	requestedAirHumidity(false),
	requestedAirTemperature(false),
	requestedBatteryVoltage(false),
	requestedIlluminance(false),
	requestedSoilMoisture(false),
	requestedSoilTemperature(false),
	requestedSDI12(false)
{
	processor = this;
}

DataProcessor::~DataProcessor()
{
	if (processor == this)
	{
		processor = nullptr;
	}
}

void DataProcessor::SaveAndSendSensorData(std::vector<SensorDataSendingPacket>* packets)
{
	// This is the place where further transmission systems should be implemented!!!
	if (Configuration::GetBool(BoolOption::SendLoRa))
	{

	}
	if (Configuration::GetBool(BoolOption::SendNB_IoT))
	{
		Cellular::SendSensorDataNBIoT(packets);
	}
	if (Configuration::GetBool(BoolOption::SendWLAN))
	{
		ESP::SendSensorData(packets);
	}
	if (Configuration::GetBool(BoolOption::SaveToSDCard))
	{
		SaveSensorData(packets);
	}

	std::vector<int> ids;
	for (auto packet : *packets)
	{
		if (packet.NB_IoT || packet.WLAN || packet.LoRa)
		{
			ids.push_back(packet.CacheIndex);
		}
	}
	StandbyCache::RemoveIDs(ids);
}

/**
 * @brief Saves the sensor data onto the SD-card
 * 
 * @param sensorData, reference to the sensor data set
 * @return true, Sensor data was successfully written onto the sd card
 * @return false, Sensor data failed to write on sd card
 */
bool DataProcessor::SaveSensorData(std::vector<SensorDataSendingPacket>* packets)
{
	HAL_StatusTypeDef ret = HAL_ERROR;
	if (SDCard::Init() != HAL_OK)
	{
		return false;
	}
	__disable_irq(); // TODO: this is a work around for the, problem that currently interupts can damage the sd card while writing to it
	for (auto packet : *packets)
	{
		// Attention: Independent watch dog (IWDG) Reset!
		HAL_IWDG_Refresh(&hiwdg);

		SensorDataSet* dataSetPtr = StandbyCache::GetSensorDataSet(packet.CacheIndex);
		ret = SDCard::WriteSensorData(dataSetPtr);
		free(dataSetPtr);

		if (ret != HAL_OK)
		{
			break;
		}
		packet.SDCard = true;
	}
	__enable_irq();
	if (SDCard::DeInit() != HAL_OK)
	{
		return false;
	}
    return ret == HAL_OK;
}

/**
 * @brief Initializes the measurement process. It doesn't handle the real 
 *        measurement with on exception. Measurements which could be executed in
 *        a short time will be executed directly.
 */
bool DataProcessor::StartMeasurement()
{
	// Create new data set
	if (sensorData != nullptr)
	{
		delete sensorData;
		sensorData = nullptr;
	}
	sensorData = new SensorDataSet();

	// Set time
	struct tm timeStamp;
	Time::Get(&timeStamp);
	sensorData->SetTimestamp(&timeStamp);

	requestedBatteryVoltage = runningBatteryVoltage = 
		StandbyRegister::IsIntervalActiveAndReset(Intervals::BatteryVoltage);
	if (runningBatteryVoltage)
	{
		Motherboard::StartVBATT();
	}

	requestedSoilMoisture = runningSoilMoisture = 
		StandbyRegister::IsIntervalActiveAndReset(Intervals::SoilMoisture);
	requestedSoilTemperature = runningSoilTemperature = 
		StandbyRegister::IsIntervalActiveAndReset(Intervals::SoilTemperature);
	if (runningSoilMoisture || runningSoilTemperature)
	{
		#if(MEASURE_SENSOR_DATA_I2C)
			Extern::StartSoilMeasurement(sensorData);
		#endif

		#if(MEASURE_SENSOR_DATA_BLE)
			runningSoilMoisture =
				runningSoilTemperature = Extern::BLEGetDataFromReceiver(sensorData);
		#endif
	}

	requestedAirTemperature = runningAirTemperature = 
		StandbyRegister::IsIntervalActiveAndReset(Intervals::AirTemperature);
	requestedAirHumidity = runningAirHumidity = 
		StandbyRegister::IsIntervalActiveAndReset(Intervals::AirHumidity);
	if (runningAirTemperature || runningAirHumidity)
	{
		airSensor = new SHT31(&hi2c1);
		airSensor->StartMeasurement();
	}
	if (runningAirTemperature)
	{
		Motherboard::StartSTMTemperature();
	}

	requestedIlluminance = runningIlluminance = 
		StandbyRegister::IsIntervalActiveAndReset(Intervals::Illuminance);
	if (runningIlluminance)
	{
		illuminanceSensor = new TSL2591(&hi2c1);
		illuminanceSensor->StartMeasurement();
	}

	//runningSDI12 = runningSoilMoisture || runningSoilTemperature;
	requestedSDI12 = runningSDI12 = false;
	if (runningSDI12)
	{
		sdi12Sensors = new SDI12();
		sdi12Sensors->StartMeasurement();
	}

	if (runningSoilMoisture || runningSoilTemperature ||
		runningAirTemperature || runningAirHumidity ||
		runningIlluminance || runningBatteryVoltage ||
		runningSDI12)
	{
		if (HAL_LPTIM_TimeOut_Start_IT(&hlptim1, 20, 0) != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}
	}

	return runningSoilMoisture || runningSoilMoisture ||
		   runningAirTemperature || runningAirHumidity ||
		   runningIlluminance || runningBatteryVoltage ||
		   runningSDI12;
}

/**
 * @brief Checks if all measurements of the current recording process have 
 * 		  finished. This includes the shutdown procedures of the sensors.
 * 
 * @return true, all measurements are finished
 * @return false, some measurements are running
 */
bool DataProcessor::FinishedMeasurement()
{
	return !runningSoilTemperature &&
		   !runningSoilMoisture &&
		   !runningAirTemperature &&
		   !runningAirHumidity &&
		   !runningIlluminance &&
		   !runningBatteryVoltage &&
		   !runningSDI12;
}

/**
 * @brief Interrupt which periodically is running while measurements are 
 * 		  running, to control the measurement process
 * 
 * @param hlptim, reference to the Timer instance
 */
void HAL_LPTIM_CompareMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
	// Wake up from STOP mode
	return;
}

/**
 * @brief Process function to run an iteration of the data acquiring process
 */
void DataProcessor::Process()
{
	ToggleLed(LED4_GPIO_Port, LED4_Pin);

	if (runningSoilMoisture || runningSoilTemperature)
	{
		#if(MEASURE_SENSOR_DATA_I2C)
				runningSoilMoisture =
					runningSoilTemperature = Extern::ProcessSoilMeasurement();
		#endif

		#if(MEASURE_SENSOR_DATA_BLE)
			runningSoilMoisture =
				runningSoilTemperature = 0;
		#endif

	}

	if (runningAirHumidity || runningAirTemperature)
	{
		runningAirHumidity = airSensor->ProcessMeasurement();
	}

	if (runningIlluminance)
	{
		runningIlluminance = illuminanceSensor->ProcessMeasurement();
	}

	if (runningAirTemperature)
	{
		runningAirTemperature = Motherboard::ProcessSTMTemperature();
	}

	if (runningBatteryVoltage)
	{
		runningBatteryVoltage = Motherboard::ProcessVBATT();
	}

	if (runningSDI12)
	{
		runningSDI12 = sdi12Sensors->ProcessMeasurement();
	}

	// After every measurement is done, stop the timer
	if (DataProcessor::processor->FinishedMeasurement())
	{
		if (requestedBatteryVoltage)
		{
			sensorData->AddBatteryVoltage(Motherboard::ReadVBATT(true));
		}
		if (requestedAirTemperature)
		{
			sensorData->AddAirTemperature(Motherboard::GetSTMTemperature(true));
			sensorData->AddAirTemperature(airSensor->GetTemperature());
		}
		if (requestedAirHumidity)
		{
			sensorData->AddAirHumidity(airSensor->GetHumidity());
		}
		if (requestedIlluminance)
		{
			sensorData->AddIlluminance(illuminanceSensor->GetLux());
		}
		
		if (sdi12Sensors != nullptr && requestedSDI12)
		{
			for (auto sensor : sdi12Sensors->sensors)
			{
				if (sensor->sensorType == SDI12_SENSOR_TYPE_ECOTECH_HYDRAPROBE)
				{
					// Parameters of the HydraProbe:
					// 1 - Soil Moisture - Water fraction by volume
					// 2 - Bulk Electrical Conductivity with Temperature Correction - S/m
					// 3 - Temperature - °C
					// 4 - Temperature - °F
					// 5 - Bulk Electrical Conductivity - S/m
					// 6 - Real Dielectric Permittivity - Unitless
					// 7 - Imaginary Dielectric Permittivity - Unitless
					// 8 - Real Dielectric Permittivity (temp. corrected) - Unitless
					// 9 - Imaginary Dielectric Permittivity (temp. corrected) - Unitless
					sensorData->AddSoilTemperature(sensor->values[2]);
					float tmp = sensor->values[0]*500;
					tmp = tmp < 0 ? 0 : tmp;
					tmp = tmp > 0xFFFF ? 0xFFFF : tmp;
					sensorData->AddSoilMoisture(tmp);

					tmp = sensor->values[4]*500;
					tmp = tmp < 0 ? 0 : tmp;
					tmp = tmp > 0xFFFF ? 0xFFFF : tmp;
					sensorData->AddSoilMoisture(tmp);

					tmp = sensor->values[5]*100;
					tmp = tmp < 0 ? 0 : tmp;
					tmp = tmp > 0xFFFF ? 0xFFFF : tmp;
					sensorData->AddSoilMoisture(tmp);

					tmp = sensor->values[6]*100;
					tmp = tmp < 0 ? 0 : tmp;
					tmp = tmp > 0xFFFF ? 0xFFFF : tmp;
					sensorData->AddSoilMoisture(tmp);

					tmp = sensor->values[7]*100;
					tmp = tmp < 0 ? 0 : tmp;
					tmp = tmp > 0xFFFF ? 0xFFFF : tmp;
					sensorData->AddSoilMoisture(tmp);

					tmp = sensor->values[8]*100;
					tmp = tmp < 0 ? 0 : tmp;
					tmp = tmp > 0xFFFF ? 0xFFFF : tmp;
					sensorData->AddSoilMoisture(tmp);
				}
				else if (sensor->sensorType == SDI12_SENSOR_TYPE_ECOTECH_TENSIOMARK)
				{
					sensorData->AddSoilMoisture(sensor->values[1]);
					sensorData->AddSoilTemperature(sensor->values[0]);
				}
			}
		}

		if (HAL_LPTIM_TimeOut_Stop_IT(&hlptim1) != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}

		// Clean up the sensor instances
		if (airSensor != nullptr)
		{
			delete airSensor;
		}
		if (illuminanceSensor != nullptr)
		{
			delete illuminanceSensor;
		}
		if (sdi12Sensors != nullptr)
		{
			delete sdi12Sensors;
		}
	}
}

/**
 * @brief Returns a pointer to the dataset which was used in the last measurement
 */
const SensorDataSet* DataProcessor::GetMeasuredData() const
{
	return sensorData;
}

/**
 * @brief Gets the amount of configured battery voltage sensors
 */
int DataProcessor::GetConfiguredBatteryVoltageCount()
{
	return 1;
}

/**
 * @brief Gets the amount of configured air temperature sensors
 */
int DataProcessor::GetConfiguredAirTemperatureCount()
{
	return 2;
}

/**
 * @brief Gets the amount of configured air humidity sensors
 */
int DataProcessor::GetConfiguredAirHumidityCount()
{
	return 1;
}

/**
 * @brief Gets the amount of configured illuminance sensors
 */
int DataProcessor::GetConfiguredIlluminanceCount()
{
	return 1;
}

/**
 * @brief Gets the amount of configured soil temperature sensors
 */
int DataProcessor::GetConfiguredSoilTemperatureCount()
{
	return COUNT_SOILSENSOR * 2;
}

/**
 * @brief Gets the amount of configured soil moisture sensors
 */
int DataProcessor::GetConfiguredSoilMoistureCount()
{
	return COUNT_SOILSENSOR * 2;
}

/**
 * @brief Gets the amount of configured soil pH sensors
 */
int DataProcessor::GetConfiguredSoilpHCount()
{
	return 0;
}

/**
 * @brief Get all sensor counts in one struct
 */
ConfiguredSensorCount DataProcessor::GetConfiguredSensorCount()
{
	ConfiguredSensorCount counts = 
	{
		.BatteryVoltageSensorCount = GetConfiguredBatteryVoltageCount(),
		.AirTemperatureSensorCount = GetConfiguredAirTemperatureCount(),
		.AirHumiditySensorCount = GetConfiguredAirHumidityCount(),
		.IlluminanceSensorCount = GetConfiguredIlluminanceCount(),
		.SoilTemperatureSensorCount = GetConfiguredSoilTemperatureCount(),
		.SoilMoistureSensorCount = GetConfiguredSoilMoistureCount(),
		.SoilpHSensorCount = GetConfiguredSoilpHCount()
	};
	return counts;
}



