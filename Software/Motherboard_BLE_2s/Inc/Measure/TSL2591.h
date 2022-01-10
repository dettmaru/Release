/**
 * @brief Measurement of illuminance values
 * 
 * @file TSL2591.h
 * @author Daniel Lohmann & Felix Schütte
 * @date 2019-03-13
 */
#ifndef __TSL2591_H_
#define __TSL2591_H_

#include <stdbool.h>
#include <stdint.h>
#include "i2c.h"

#define TSL2591_REGISTER_CHAN0_LOW  	0x14
#define TSL2591_REGISTER_CHAN0_HIGH 	0x15
#define TSL2591_REGISTER_CHAN1_LOW  	0x16
#define	TSL2591_REGISTER_CHAN1_HIGH 	0x17

#define TSL2591_INTEGRATIONTIME_100MS	0x00
#define TSL2591_INTEGRATIONTIME_200MS   0x01
#define TSL2591_INTEGRATIONTIME_300MS   0x02
#define TSL2591_INTEGRATIONTIME_400MS   0x03
#define TSL2591_INTEGRATIONTIME_500MS   0x04
#define TSL2591_INTEGRATIONTIME_600MS 	0x05

#define TSL2591_GAIN_LOW				0x00
#define TSL2591_GAIN_MED				0x10
#define TSL2591_GAIN_HIGH				0x20
#define TSL2591_GAIN_MAX				0x30

#define TSL2591_ADDR					0x29
#define TSL2591_ID						0x12

#define TSL2591_COMMAND_BIT				0xA0

#define TSL2591_REGISTER_ENABLE			0X00
#define TSL2591_REGISTER_CONTROL 		0x01
#define TSL2591_ENABLE_POWEROFF			0x00
#define TSL2591_ENABLE_POWERON			0X01
#define TSL2591_ENABLE_AEN				0x02
#define TSL2591_ENABLE_AIEN				0x10

#define TSL2591_LUX_DF            (408.0F)
#define TSL2591_LUX_COEFB         (1.64F)
#define TSL2591_LUX_COEFC         (0.59F)
#define TSL2591_LUX_COEFD 		  (0.86F)

#define TSL2591_WAKEUP_TIME     10
#define TSL2591_INITWAIT_TIME   100

class TSL2591
{
public:
    TSL2591(I2C_HandleTypeDef* hi2c);
    ~TSL2591();

    void StartMeasurement();
    bool ProcessMeasurement();

    uint16_t GetLux();
private:
    void TurnON();
    void TurnOFF();

    uint8_t Init();
    void Enable();
    void Disable();
    void CheckID();
    void SetGain(uint8_t gain);
    void SetIntegrationTime(uint8_t time);
    void ReadChannels();
    uint16_t CalculateLux();

    volatile bool initialized;
	volatile bool is_enabled;
	volatile uint16_t ch0;
	volatile uint16_t ch1;
	volatile float lux;
	volatile uint16_t gain;
	volatile uint16_t integrationTime;
	I2C_HandleTypeDef* hi2c;

    volatile uint32_t startTime;
};

#endif /* __TSL2591_H_ */
