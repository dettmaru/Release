/*********************************************************************
* PERIPHERY.h includes all functions for periphery elements
*
* Technische Hochschule Koeln | RhizoTech 25.12.2019
*
* @ALPO
*
/*********************************************************************/

#ifndef PHERIPHERY_H
#define PHERIPHERY_H


// Includes 

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "boards.h"
#include "nrf_delay.h"
#include "bsp.h"
#include "app_timer.h"
#include "ble_lbs_c.h"
#include <string.h>
#include "app_error.h"
#include "nrf_gpio.h"
#include "sdk_config.h"
#include "nrf_drv_spis.h"
#include "nrf_temp.h"
#include "nrf_gpiote.h"
#include "nrf_drv_gpiote.h"
#include "nrf_drv_twis.h"

#include "SENSOR_ACCESS_CONTROL.h"

// Macros
#define BIT_SET(x,y) x |= (1 << y)
#define BIT_CLEAR(x,y) x &= ~(1<< y)


// Defines for SPI Slave
#define SPIS_INSTANCE 1 
#define SIZE_OF_SPI_FRAME 40



/*--------------------------------------------------------------------------------------*/
/*!<   RhizoTech HAL - Receiver Module GPIO defines                                     */                                     
/*--------------------------------------------------------------------------------------*/
/*!<          Define    Pinnumber                      Describtion                      */
/*--------------------------------------------------------------------------------------*/
#define       SW0       NRF_GPIO_PIN_MAP(1,9)     /*!< High Active Switches             */
#define       SW1       NRF_GPIO_PIN_MAP(0,12)    /*!<                                  */
#define       SW2       NRF_GPIO_PIN_MAP(0,23)    /*!<                                  */
#define       SW3       NRF_GPIO_PIN_MAP(0,21)    /*!<                                  */
#define       SW4       NRF_GPIO_PIN_MAP(0,19)    /*!<                                  */
                                                  /*!<                                  */
#define       BLE_LED   NRF_GPIO_PIN_MAP(1,4)     /*!< Shows active BLE connection      */
#define       SPI_LED   NRF_GPIO_PIN_MAP(1,2)     /*!< Shows active data transmission   */
#define       ERR_LED   NRF_GPIO_PIN_MAP(0,9)     /*!< Shows if an error happend        */
#define       PWR_LED   NRF_GPIO_PIN_MAP(1,6)     /*!< Power LED                        */
                                                  /*!<                                  */
#define       SPI_SS    NRF_GPIO_PIN_MAP(0,22)    /*!< Slave Select                     */
#define       SPI_MOSI  NRF_GPIO_PIN_MAP(1,0)     /*!< Master Out, Slave In             */
#define       SPI_MISO  NRF_GPIO_PIN_MAP(1,3)     /*!< Master In, Slave Out             */
#define       SPI_SCK   NRF_GPIO_PIN_MAP(1,1)     /*!< Clock                            */
#define       SPI_READY NRF_GPIO_PIN_MAP(0,5)     /*!< GPIO Interrupt from Mainboard    */
#define       DAT_READY NRF_GPIO_PIN_MAP(0,4)     /*!< GPIO Data Ready for Mainboard    */
/*--------------------------------------------------------------------------------------*/


// Variable Declaration

typedef struct{

  uint32_t  time_ms;
  uint32_t  start_time;
  uint32_t  stop_time;
  bool      tmr_started;

} stopwatch_struct;

typedef stopwatch_struct *ptr_stopwatch_struct;


/**
 * @brief Structure for TWIS configuration.
 */
typedef struct
{
    uint8_t slave_addr;                //!< Slave address that this simulator will use to respond to master.
    uint8_t scl_s;                     //!< Slave SCL.
    uint8_t sda_s;                     //!< Slave SCL.
    uint32_t flash_start_addr;         //!< Flash start address for the EEPROM simulation.
} eeprom_sim_config_t;


APP_TIMER_DEF(m_repeated_sleep_timer);  
APP_TIMER_DEF(m_repeated_stopwatch_timer);  
APP_TIMER_DEF(m_repeated_conf_mode_timer);  
APP_TIMER_DEF(m_repeated_error_timer);  

extern nrf_drv_spis_t spis;                      
static volatile bool spis_xfer_done;        
extern uint8_t       m_rx_buf[40];    
extern uint8_t       m_tx_buf[40];

// Variable
extern uint8_t spi_buffer[16][40]; 
extern bool spi_transfer_finish;

// Function Declaration

void gpio_init(void);
void spi_buffer_init(void);
void gpio_event_init(void);
void all_leds_off(void);
void leds_init(void);
void timer_init(void);
void create_timers(void);
void repeated_timer_handler(void * p_context);
void lfclk_request(void);
void spis_event_handler(nrf_drv_spis_event_t event);
void spis_init(void);
void spis_get_sleep_time();
void read_chip_temperature(void);
void read_chip_temperature_init(void);
void timer_stop_all(void);
void stopwatch_timer_stop(void);
void sleep_timer_stop(void);
void stopwatch_timer_start_ms(uint32_t *milliseconds);
void sleep_timer_start_ms(uint32_t *milliseconds);
void sleep_timer_start_5m(void);
void sleep_timer_start_3m(void);
void sleep_timer_start_1m(void);
void conf_mode_timer_start_500ms(void);
void conf_mode_timer_stop(void);
void error_timer_stop(void);
void error_timer_start(void);
void stopwatch_get_time(ptr_stopwatch_struct stopwatch_t);
void stopwatch_reset(ptr_stopwatch_struct stopwatch_t);
void stopwatch_start(ptr_stopwatch_struct stopwatch_t);
bool spi_err_check(ret_code_t err_check);
uint8_t read_switch_state(void);
void spis_exchange_config_data(void);
void spis_transmit_sensor_data(void);
void twis_init(void);
void twis_send_sensor_nbr(void);
#endif /* PERIPHERY_H */
