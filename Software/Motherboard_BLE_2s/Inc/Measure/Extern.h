/**
 * @brief Measurement of values connected to the motherboard
 * 
 * @file Extern.h
 * @author Daniel Lohmann
 * @date 2018-04-25
 */
#ifndef MEASURE_EXTERN_H_
#define MEASURE_EXTERN_H_

#include "main.h"
#include <stdbool.h>
#include <list>
#include "SensorDataSet.h"
#include "sht31.h"
#include "TSL2591.h"


#define COUNT_SOILSENSOR	22

#ifndef MEASURE_SENSOR_DATA_I2C
#define MEASURE_SENSOR_DATA_I2C 0
#endif

#ifndef MEASURE_SENSOR_DATA_BLE
#define MEASURE_SENSOR_DATA_BLE 1
#endif




/**
 * @brief Handles connections to external stuff (external to the STM)
 * 
 * This includes for example the SoilSensors
 */
class Extern {
public:
	static float GetTemperature();
	static ADC_HandleTypeDef* adc;

	static void StartSoilMeasurement(SensorDataSet* sensorData);
	static bool ProcessSoilMeasurement();
	static void FinishSoilMeasurement();
	static bool BLEGetDataFromReceiver(SensorDataSet* sensorData);
private:
	static void BLECalcSensorData(SensorDataSet* sensorData, uint8_t* buffer, uint8_t SPISensorData[][40]);
	static void RequestSoilMeasurement(I2C_HandleTypeDef* hi2c, uint16_t slave_address);
	static void RetriveSoilMeasurementValues(I2C_HandleTypeDef* hi2c, uint16_t slave_address, uint8_t *i2c_receive_buffer);
	static void AddSoilMeasurementToDataSet(SensorDataSet* sensorData, uint8_t* buffer);
	static void I2CReInitialize(I2C_HandleTypeDef* hi2c);
	static const uint16_t SoilComTimeout;
	static SensorDataSet* sensorData;

	enum class SoilSensorState {
		OFF,
		IDLE,
		ACTIVE,
		TRANSMITTING,
		FINISHED,
	};
	typedef struct {
		uint8_t id;
		SoilSensorState state;
	} SoilMeasurementState_t;
	static SoilMeasurementState_t SoilMeasurementState;

};

#endif /* MEASURE_EXTERN_H_ */
