/**
 * @brief Measurement of values connected to the motherboard
 * 
 * @file Extern.cpp
 * @author Daniel Lohmann
 * @date 2018-04-25
 */

#include "main.h"
#include "Measure/Extern.h"
#include "i2c.h"
#include "Energy.h"
#include "HardwareSemaphores.h"
#include <string.h>
#include "math.h"
#include "spi.h"



ADC_HandleTypeDef* Extern::adc = nullptr;
const uint16_t Extern::SoilComTimeout = 500;
SensorDataSet* Extern::sensorData = nullptr;
Extern::SoilMeasurementState_t Extern::SoilMeasurementState;

#define RXBUFFERSIZE 				12

#define SOIL_WAKEUP_TIME			100
#define SOIL_MEASUREMENT_PERIOD		200
#define SOIL_I2CRESET_TIME			10

/**
 * @brief Gets the TempArray of an external TC1047AVN sensor, which is connected via an ADC
 * 
 * @return Returns a float value of the TempArray in °C
 */
float Extern::GetTemperature()
{
	HAL_GPIO_WritePin(PWR_SW1_GPIO_Port, PWR_SW1_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);

	ADC_ChannelConfTypeDef sConfig;
	sConfig.Channel = ADC_CHANNEL_7; //Needs to be done
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(adc, &sConfig) != HAL_OK)
		return -1;

	HAL_ADC_Start(adc);
	HAL_ADC_PollForConversion(adc, 1000);
	uint32_t adcvalue = HAL_ADC_GetValue(adc);
	HAL_ADC_PollForConversion(adc, 1000);
	adcvalue = HAL_ADC_GetValue(adc);
	HAL_ADC_Stop(adc);
	HAL_GPIO_WritePin(PWR_SW1_GPIO_Port, PWR_SW1_Pin, GPIO_PIN_SET);
	float temp = (((3.3f / 4096) * adcvalue) - 0.5) / 0.01;
	return temp;
}

uint32_t lastTime = 0;

/**
 * @brief Startes the soil measurement process
 * 
 * @param sensorData, SensorDataSet in which the data should be saved
 */
void Extern::StartSoilMeasurement(SensorDataSet* sensorData)
{
	MX_I2C2_Init();
	
	Extern::sensorData = sensorData;
	SoilMeasurementState.id = 1;
	SoilMeasurementState.state = SoilSensorState::OFF;

	lastTime = HAL_GetTick();

	ProcessSoilMeasurement();
	return;
}

/**
 * @brief Process a soil measurement step
 *
 * @return True when measurement is in progress
 * 	       False when measurement is finished
 */
bool Extern::ProcessSoilMeasurement()
{
	/*
	 * The received value is 12 bytes long. It will include six 12-bit ADC Values:
	 * {moisture_bottom | moisture_top | temp_top | temp_bottom | ...
	 *  ... supplyvolt_moisturesens | supplyvolt_tempsens }
	 *
	 *   This means 24 bits are unused, CRC/Parity Bits can be implemented later.
	 *
	 */
	uint8_t buffer[RXBUFFERSIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint32_t currentTime = HAL_GetTick();

	if (SoilMeasurementState.id <= COUNT_SOILSENSOR)
	{
		switch (SoilMeasurementState.state)
		{
			case SoilSensorState::OFF:
				Extern::sensorData = sensorData;

				/* Turn the corresponding physical line on */
				soilSensors.Request();
				SoilMeasurementState.state = SoilSensorState::IDLE;
				lastTime = currentTime;
				break;
			case SoilSensorState::IDLE:
				if (currentTime - lastTime > SOIL_WAKEUP_TIME)
				{
					RequestSoilMeasurement(&hi2c2, (uint16_t)(SoilMeasurementState.id<<1));
					SoilMeasurementState.state = SoilSensorState::ACTIVE;
					lastTime = currentTime;
				}
				break;
			case SoilSensorState::ACTIVE:
				if (currentTime - lastTime > SOIL_MEASUREMENT_PERIOD)
				{
					RetriveSoilMeasurementValues(&hi2c2, (uint16_t)(SoilMeasurementState.id<<1), buffer);
					AddSoilMeasurementToDataSet(sensorData, buffer);

					SoilMeasurementState.state = SoilSensorState::TRANSMITTING;
					lastTime = currentTime;
				}
				break;
			case SoilSensorState::TRANSMITTING:
				if (currentTime - lastTime > SOIL_I2CRESET_TIME)
				{
					I2CReInitialize(&hi2c2);

					SoilMeasurementState.state = SoilSensorState::FINISHED;
					lastTime = currentTime;
				}
				break;
			case SoilSensorState::FINISHED:
				FinishSoilMeasurement();
				SoilMeasurementState.state = SoilSensorState::OFF;
				SoilMeasurementState.id++;
				break;
		}
		return true;
	}
	return false;
}

void Extern::AddSoilMeasurementToDataSet(SensorDataSet* sensorData, uint8_t* buffer)
{
	if (buffer[0] > 0) //Measurement done and values received?
	{
		volatile uint16_t moistureBottom 			= (buffer[0]  << 8 | buffer[1])  & 0x0FFF;
		volatile uint16_t moistureTop 				= (buffer[2]  << 8 | buffer[3])  & 0x0FFF;
		volatile uint16_t TempArrayTop			= (buffer[4]  << 8 | buffer[5])  & 0x0FFF;
		volatile uint16_t TempArrayBottom			= (buffer[6]  << 8 | buffer[7])  & 0x0FFF;
		//uint16_t supplyVoltageMoisture			= (buffer[8]  << 8 | buffer[9])  & 0x0FFF;
		//uint16_t supplyVoltageTempArray			= (buffer[10] << 8 | buffer[11]) & 0x0FFF;
		sensorData->AddSoilMoisture(moistureTop);
		sensorData->AddSoilMoisture(moistureBottom);
		sensorData->AddSoilTemperature((TempArrayTop / 4096.0f * 3.3f - 0.5) * 100);
		sensorData->AddSoilTemperature((TempArrayBottom / 4096.0f * 3.3f - 0.5) * 100);
	}
	else //Add zeros so that the positions are correct
	{
		sensorData->AddSoilMoisture(0);
		sensorData->AddSoilMoisture(0);
		sensorData->AddSoilTemperature(0);
		sensorData->AddSoilTemperature(0);
	}
	return;
}

void Extern::FinishSoilMeasurement()
{
	/* Turn the corresponding physical line off */
	soilSensors.Free();
}






bool Extern::BLEGetDataFromReceiver(SensorDataSet* sensorData)
{

	uint8_t 	SPISensorData[16][40];
	uint8_t 	dummy_data[40];
	uint8_t 	spi_sleep_time_data = 2;
	uint8_t 	SensorNumber = 0;



	// Init SPI3
	MX_SPI3_Init();

	Extern::sensorData = sensorData;


	memset(SPISensorData, 0, sizeof(SPISensorData));
	memset(dummy_data, 0, sizeof(dummy_data));

	uint8_t Wait_Counter = 0;

	// Wait for receiver module is ready to exchange data
	while(HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_9) == 0){
		HAL_Delay(1000);
		Wait_Counter++;
		HAL_IWDG_Refresh(&hiwdg);

		if(Wait_Counter >= 180){
			// Reset Receiver Module
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_SET);
			// Clear Wait_Counter
			Wait_Counter = 0;
		}
	}


	// Trigger Interrupt on Receiver-Module
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET);


	// Exchange configuration data
	SensorNumber = 0;
	for(int i = 0; i<2;i++)
	{
	  __HAL_SPI_ENABLE(&hspi3);
	  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_RESET);
	  HAL_SPI_TransmitReceive(&hspi3, &spi_sleep_time_data, &SensorNumber, sizeof(spi_sleep_time_data), HAL_MAX_DELAY);
	  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_SET);
	  HAL_Delay(500);
	  while( !(SPI3->SR & SPI_FLAG_TXE) );
	  __HAL_SPI_DISABLE(&hspi3);
	}


	// Receive all Sensor-Data from Receiverboard	//
	//--------------------------------------------//
	for(uint8_t i = 0; i<=12; i++){
	  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_RESET);
	  HAL_SPI_TransmitReceive(&hspi3, dummy_data, SPISensorData[i], sizeof(dummy_data), HAL_MAX_DELAY);
	  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_SET);
	  HAL_Delay(500);
	  while( !(SPI3->SR & SPI_FLAG_TXE) );
	}


	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_0);


	HAL_Delay(1000);


	BLECalcSensorData(sensorData, &SensorNumber, SPISensorData);


	return false;

}



void Extern::BLECalcSensorData(SensorDataSet* sensorData, uint8_t* SensorNumber, uint8_t SPISensorData[][40]){


	uint16_t 	pH1[16], pH2[16];
	float    	BatCap[16];
	float 		TempArray[16][2];
	uint16_t 	HumArray[16][2];
	float 		oldTempArray[16][2];
	uint16_t 	oldHumArray[16][2];
	uint32_t  	Bat = 0;
	float     	UBat = 0;




	for(uint8_t SensorIndex = 0; SensorIndex <= 12; SensorIndex++){

		//-----------------------------------------------------------------------------------------
		// TempArray
		//-----------------------------------------------------------------------------------------
		for(uint8_t n = 0; n<7; n++){
		  TempArray[SensorIndex][n] = ((float)(((SPISensorData[SensorIndex][n*2+2]-64) << 3) + ((SPISensorData[SensorIndex][(n*2)+3] >> 5)))/8);

		  // When the buffer value is zero, the temperature calculation gives a temperature about 60�C
		  if (TempArray[SensorIndex][n] < -30 || TempArray[SensorIndex][n] > 60){
			  TempArray[SensorIndex][n] = oldTempArray[SensorIndex][n];
		  }
		}


		//-----------------------------------------------------------------------------------------
		// Humidity
		//-----------------------------------------------------------------------------------------
		for(uint8_t k = 0; k<7; k++){
			HumArray[SensorIndex][k] =(uint16_t)((SPISensorData[SensorIndex][(k*2)+18] << 8) + SPISensorData[SensorIndex][(k*2)+19]);
			if (HumArray[SensorIndex][k] < 1000 || HumArray[SensorIndex][k] > 4096){
				HumArray[SensorIndex][k] = oldHumArray[SensorIndex][k];
			}
		}


		//-----------------------------------------------------------------------------------------
		// PH Value
		//-----------------------------------------------------------------------------------------
		pH1[SensorIndex] =((SPISensorData[SensorIndex][36] << 8) + SPISensorData[SensorIndex][37]);
		pH2[SensorIndex] =((SPISensorData[SensorIndex][38] << 8) + SPISensorData[SensorIndex][39]);



		//-----------------------------------------------------------------------------------------
		// Sensor Battery Value in percent
		//-----------------------------------------------------------------------------------------
		Bat =((SPISensorData[SensorIndex][34] << 8) + SPISensorData[SensorIndex][35]);


		// Convert ADC value in Battery voltage (mV)
		UBat = (float)((3.3/4096)*Bat)/2;

		// Determine the Battery capacity
		// Capacity from 0% to 20%
		if((UBat >= 0.820) && (UBat < 1.1)){
		BatCap[SensorIndex] = (64.6*UBat) - 54.2;

		// Capacity from 20% to 100%
		} else if ((UBat >= 0.820) && (UBat <= 1.600)){
		float temp = (10*UBat) - 12.7;
		BatCap[SensorIndex] = (111)/(1+exp(-temp));
		} else {
		BatCap[SensorIndex] = 0;
		}
	}


	// Set Sensor Data
	for(uint8_t n = 0; n <= 12; n++){
		for(uint8_t k = 0; k < 2; k++){

			// Set Data and save to relace zero values one times
			sensorData->AddSoilTemperature(TempArray[n][k]);
			if(TempArray[n][k] >= -30 || TempArray[n][k] < 60){
				oldTempArray[n][k] = TempArray[n][k];
			} else {
				oldTempArray[n][k] = 0;
			}

			sensorData->AddSoilMoisture(HumArray[n][k]);
			if (HumArray[n][k] > 1000 || HumArray[n][k] < 4096){
				oldHumArray[n][k] = HumArray[n][k];
			}

		}
	}
	return;
}




void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{

}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{

}

/**
 * @brief Requests the RhizoSensor to acquires sensor values
 *
 * @note  The Sensor works as a state machine using I2C RX/TX-Cplt Callbacks
 *
 * The states change in the TxCplt-Callback or by receiving a request via I2C
 *
 * State - Description
 *
 * IDLE 		- Waiting for Measurement Request by Master, LPRUN-Mode
 * ACTIVE 		- Acquiring Measurement Values, RUN (2,097 kHz SYSCLK)
 * TRANSMITTING - Waiting for Transmit Request by Master, LPRUN-Mode
 *
 * @note This function sends a command byte 0x01 which will trigger a
 * measurement of the sensor. After approx. 220 ms (300 to be safe)
 * the data can be retrieved (RhizoSensor is waiting for an interrupt,
 * which is triggered with a transmission of its slave address using HAL_
 * I2C_IsDeviceReady(...)
 */
void Extern::RequestSoilMeasurement(I2C_HandleTypeDef* hi2c, uint16_t slave_address)
{
	uint8_t cmd_byte = 0x01;
	/*Transmit an acquisition request (0x01 is transmitted to the slave */
	HAL_I2C_Master_Transmit(hi2c, slave_address, &cmd_byte, 1, 1000);
	/* Variable to provide a timeout */
	uint32_t tick = HAL_GetTick();
	/* Wait for transmission to finish */
	while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY && HAL_GetTick() - tick < SoilComTimeout)
	{

	}
}

/**
 * @brief Read the sensor values from the RhizoSensor. This requires that prior 
 * 		  to this a request was made and at least 220ms have been waited.
 *
 * @note  The Sensor works as a state machine using I2C RX/TX-Cplt Callbacks
 *
 * The states change in the TxCplt-Callback or by receiving a request via I2C
 *
 * State - Description
 *
 * IDLE 		- Waiting for Measurement Request by Master, LPRUN-Mode
 * ACTIVE 		- Acquiring Measurement Values, RUN (2,097 kHz SYSCLK)
 * TRANSMITTING - Waiting for Transmit Request by Master, LPRUN-Mode
 *
 * @note This function sends a command byte 0x01 which will trigger a
 * measurement of the sensor. After approx. 220 ms (300 to be safe)
 * the data can be retrieved (RhizoSensor is waiting for an interrupt,
 * which is triggered with a transmission of its slave address using HAL_
 * I2C_IsDeviceReady(...)
 */
void Extern::RetriveSoilMeasurementValues(I2C_HandleTypeDef* hi2c, uint16_t slave_address, uint8_t *i2c_receive_buffer)
{
	/* Enable interrupt to receive the data */
	HAL_I2C_Master_Receive_IT(hi2c, slave_address, i2c_receive_buffer, 12);

	HAL_Delay(20);
	/* Send the slave address to trigger an interrupt of the slave */
	HAL_I2C_IsDeviceReady(hi2c, slave_address, 1, 1000);
	/* Wait for transmission to finish */
	uint32_t tick = HAL_GetTick();
	while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY && HAL_GetTick() - tick < SoilComTimeout)
	{
		//Wait for reception to finish
	}
}

/**
 * @brief clears and reinitializes i2c interface i2c1
 */
void Extern::I2CReInitialize(I2C_HandleTypeDef* hi2c)
{
	/* Clear PE bit. */
	CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_PE);

	/* Deinitialize the I2C-Interface */
	HAL_I2C_DeInit(hi2c);

	/* Enable the I2C peripheral by setting the PE bit in I2Cx_CR1 register */
	SET_BIT(hi2c->Instance->CR1, I2C_CR1_PE);
	asm("nop");

	/* Initialize the I2C-Interface */
	HAL_I2C_Init(hi2c);
}
