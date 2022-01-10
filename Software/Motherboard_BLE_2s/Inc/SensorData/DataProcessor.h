#ifndef __DATAPROCESSOR_H_
#define __DATAPROCESSOR_H_

#include "SensorDataSet.h"
#include "SensorDataSendingPacket.h"
#include "ConfiguredSensorCount.h"
#include <vector>
#include "lptim.h"

/**
 * @brief The @ref DataProcessor class handles the general measurement and data
 *        handling of measurements. It controls the measurements at a high level.
 */
class DataProcessor{
public:
    DataProcessor();
    virtual ~DataProcessor();

    void SaveAndSendSensorData(std::vector<SensorDataSendingPacket>* packets);

    bool StartMeasurement();
    bool FinishedMeasurement();
    void Process();
    const SensorDataSet* GetMeasuredData() const;

    static int GetConfiguredBatteryVoltageCount();
	static int GetConfiguredAirTemperatureCount();
	static int GetConfiguredAirHumidityCount();
	static int GetConfiguredIlluminanceCount();
	static int GetConfiguredSoilTemperatureCount();
	static int GetConfiguredSoilMoistureCount();
	static int GetConfiguredSoilpHCount();
    static ConfiguredSensorCount GetConfiguredSensorCount();
private:
    bool SaveSensorData(std::vector<SensorDataSendingPacket>* packets);

    SensorDataSet* sensorData;
    // Flags for determining the state of acquisition (finished or not)
    volatile bool runningAirHumidity;
    volatile bool runningAirTemperature;
    volatile bool runningBatteryVoltage;
    volatile bool runningIlluminance;
    volatile bool runningSoilMoisture;
    volatile bool runningSoilTemperature;
    volatile bool runningSDI12;

    // Flags for determining the state of request (was the measurement requested?)
    volatile bool requestedAirHumidity;
    volatile bool requestedAirTemperature;
    volatile bool requestedBatteryVoltage;
    volatile bool requestedIlluminance;
    volatile bool requestedSoilMoisture;
    volatile bool requestedSoilTemperature;
    volatile bool requestedSDI12;

    static DataProcessor* processor;

    friend void HAL_LPTIM_CompareMatchCallback(LPTIM_HandleTypeDef *hlptim);
};

#endif /* __DATAPROCESSOR_H_ */
