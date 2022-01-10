/*********************************************************************
* PERIPHERY.c includes all initial functions for periphery elements
*
* Technische Hochschule Koeln | RhizoTech 10.12.2019
*
* @ALPO
*
/*********************************************************************/
#ifndef PERIPHERY_H
#define PERIPHERY_H

// Includes
#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "app_error.h"
#include "bsp.h"
#include "app_pwm.h"
#include "boards.h"
#include "app_timer.h"
#include "app_button.h"
#include "sdk_config.h"
#include "nrf_delay.h"
#include "DATA_TRANSMISSION.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_drv_twi.h"
#include "nrf_saadc.h"
#include "SEGGER_RTT.h"


// Macros
#define ADVERTISING_LED                 PWR_LED                   
#define CONNECTED_LED                   PWR_LED              
#define LEDBUTTON_LED                   PWR_LED                    
#define LEDBUTTON_BUTTON                BSP_BUTTON_0                           
#define ERROR                           1
#define NO_ERROR                        0
#define TWI_INSTANCE_ID                 0

/*
TWI_ENABLED - nrf_drv_twi - instance 1
TWIS_ENABLED - nrf_drv_twis- instance 0
*/


/*--------------------------------------------------------------------------------------*/
/*!< RhizoTech HAL - Sensor Module GPIO defines                                         */                                
/*--------------------------------------------------------------------------------------*/
/*!<          Define    Pinnumber                      Describtion                      */
/*--------------------------------------------------------------------------------------*/
#define       AIN_MO    NRF_SAADC_INPUT_AIN5      /*!< Analog Input Moisture Reading    */
#define       MPX_A     NRF_GPIO_PIN_MAP(0,12)    /*!< Control Pin for Sensor selection */              
#define       MPX_B     NRF_GPIO_PIN_MAP(0,23)    /*!<                                  */            
#define       MPX_C     NRF_GPIO_PIN_MAP(0,21)    /*!<                                  */            
#define       GPIO_PWM  NRF_GPIO_PIN_MAP(0,19)    /*!< Clock generation with PWM        */            
                                                  /*!<                                  */            
#define       AIN_U_BAT NRF_SAADC_INPUT_AIN4      /*!< Measure UB                       */      
#define       AIN_PH1   NRF_SAADC_INPUT_AIN1      /*!< External PH Sensor Voltage       */   
#define       AIN_PH2   NRF_SAADC_INPUT_AIN0      /*!<                                  */
#define       PH_ON     NRF_GPIO_PIN_MAP(1,15)    /*!<                                  */
                                                  /*!<                                  */        
#define       PWR_LED   NRF_GPIO_PIN_MAP(1,6)     /*!< Power LED                        */                        
                                                  /*!<                                  */                       
#define       SM_DATA   NRF_GPIO_PIN_MAP(0,29)    /*!< SM Bus Data                      */                     
#define       SM_CLK    NRF_GPIO_PIN_MAP(0,30)    /*!< SM Bus Clock                     */             
#define       STBY      NRF_GPIO_PIN_MAP(0,4)     /*!< Active low standby output        */ 
                                                  /*!<                                  */                          
/*--------------------------------------------------------------------------------------*/


// Two wire interface 
extern const nrf_drv_twi_t m_twi;
extern volatile uint8_t I2C_FastMode;
extern uint8_t Slave_Adress;


/* Definieren der Display Ports mit GPIO Pins
*/
#define VDD_Sensor   NRF_GPIO_PIN_MAP(1,13) // P1.13



/* Multichannel Temperature Sensor Defines */
#define MAX6581TG9A          0X9a    // Slave Adress MAX6581TG9A+
#define MAX6581TG9C          0x9c    // Slave Adress MAX6581TG9C+
#define MAX6581TG9E          0x9e    // Slave Adress MAX6581TG9E+
#define MAX6581TG98          0x98    // Slave Adress MAX6581TG98+
#define Slv_Adr_0    (MAX6581TG9A>>1)// Slave Adress 
#define Slv_Adr_1    (MAX6581TG9C>>1)// Slave Adress 
#define Slv_Adr_2    (MAX6581TG9E>>1)// Slave Adress 
#define Slv_Adr_3    (MAX6581TG98>>1)// Slave Adress 

#define MAX_WRITE_LENGTH    128     // Maximum length for i2c writing
#define Read                1       // Read Mode
#define Write               0       // Write Mode
#define Local_HB            0x07    // Local internal Temperature Sensor
#define Local_LB            0x57    // Local internal Temperature Sensor

#define Remote_2_HB         0x01    // Temperature Sensor 1 (deepest sensor) High Byte
#define Remote_1_HB         0x02    // Temperature Sensor 2 High Byte

#define Remote_2_LB         0x51    // Temperature Sensor 1 Low Byte
#define Remote_1_LB         0x52    // Temperature Sensor 2 Low Byte

    


// Deklaration of variables
APP_TIMER_DEF(sleep_timer_id); 
APP_TIMER_DEF(fsm_timeout_timer_id); 


// Deklaration of Functions
void timers_init(void);
void leds_init(void);
void buttons_init(void);
void sensor_on(void);
void sensor_off(void);
void gpio_init(void);
void gpio_uninit(void);
void moisture_clock_init(void);
bool moisture_clock_ready_check(void);
bool prepare_moisture_clock_to_sleep(void); 
void stop_moisture_clock(void);
void start_moisture_clock(void);
void twi_init (void);
void twi_enable(void);
void twi_disable(void);
void prepare_twi_to_sleep(void);
void temp_sensor_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
void temp_sensor_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
bool temp_sensor_device_test(void);
void temp_sensor_read_all(void);
void temp_sensor_write_settings(void);
void temp_sensor_standby(void);
void temp_sensor_active(void);
void clear_screen_RTT(void);
void sleep_timer_stop(void);
void sleep_timer_start_1m(void);
void sleep_timer_start_3m(void);
void sleep_timer_start_5m(void);
void sleep_timer_start_10m(void);
void sleep_timer_start_15m(void);
void create_timer(void);
void fsm_timeout_timer_stop(void);
void fsm_timeout_timer_start(void);
void fsm_timeout_timer_reset(uint8_t new_fsm_state);
void temp_sensor_calibration(void);
void uninit_moisture_clock(void);
#endif /* PERIPHERY_H */
