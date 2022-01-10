#ifndef __SENSORDATASET_H_
#define __SENSORDATASET_H_

#include <ctime>
#include <stdint.h>
#include <list>

/**
 * @brief This class handels a set of sensor data. 
 * 
 * This class can create a serialized version of it self.
 * From the serialized version a normal version can be retrieved.
 */
class SensorDataSet {
public:
    SensorDataSet();
    SensorDataSet(uint8_t* data, uint16_t length);
    virtual ~SensorDataSet();

    struct tm GetTimestamp() const;
    
    const std::list<uint16_t>* GetAirTemperatures() const;
    const std::list<uint16_t>* GetSoilTemperatures() const;
    const std::list<uint16_t>* GetSoilMoistures() const;
    const std::list<uint16_t>* GetAirHumidities() const;
    const std::list<uint16_t>* GetIlluminances() const;
    const std::list<uint16_t>* GetpHValues() const;
    const std::list<uint16_t>* GetBatteryVoltages() const;

    std::list<float>* GetAirTemperaturesAsFloat() const;
    std::list<float>* GetAirHumiditiesAsFloat() const;
    std::list<float>* GetSoilTemperaturesAsFloat() const;
    std::list<float>* GetSoilMoisturesAsFloat() const;
    std::list<float>* GetIlluminancesAsFloat() const;
    std::list<float>* GetpHValuesAsFloat() const;
    std::list<float>* GetBatteryVoltagesAsFloat() const;

    void SetTimestamp(time_t time);
    void SetTimestamp(const struct tm* time);
    void AddAirTemperature(float value);
    void AddAirHumidity(float value);
    void AddSoilTemperature(float value);
    void AddSoilMoisture(float value);
    void AddIlluminance(float value);
    void AddpHValue(float value);
    void AddBatteryVoltage(float voltage);

    uint8_t* GetMQTTSensorDataContent(uint16_t* size) const;

    static float AirTemperatureMEMToFloat(uint16_t value);
    static float AirHumidityMEMToFloat(uint16_t value);
    static float SoilTemperatureMEMToFloat(uint16_t value);
    static float SoilMoistureMEMToFloat(uint16_t value);
    static float IlluminanceMEMToFloat(uint16_t value);
    static float pHValueMEMToFloat(uint16_t value);
    static float BatteryVoltageMEMToFloat(uint16_t value);

    static uint16_t AirTemperatureFloatToMEM(float value);
	static uint16_t AirHumidityFloatToMEM(float value);
	static uint16_t SoilTemperatureFloatToMEM(float value);
	static uint16_t SoilMoistureFloatToMEM(float value);
	static uint16_t IlluminanceFloatToMEM(float value);
    static uint16_t pHValueFloatToMEM(float value);
	static uint16_t BatteryVoltageFloatToMEM(float value);

	bool operator==(const SensorDataSet &b) const;
private:
    void InterpretMessage(uint8_t* data, uint16_t length);

    struct tm timeStamp;
    std::list<uint16_t>* airTemperatures;
    std::list<uint16_t>* airHumidities;
    std::list<uint16_t>* soilTemperatures;
    std::list<uint16_t>* soilMoistures;
    std::list<uint16_t>* illuminances;
    std::list<uint16_t>* pHValues;
    std::list<uint16_t>* batteryVoltages;
};

#endif
