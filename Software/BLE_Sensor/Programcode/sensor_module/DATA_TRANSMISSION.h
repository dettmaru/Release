/*********************************************************************
* DATA_TRANSMISSION.c includes all functions for transmission of analog
* sensor datas
*
* Technische Hochschule Koeln | RhizoTech 10.12.2019
*
* @ALPO
*
/*********************************************************************/


#ifndef DATA_TRANSMISSION_H
#define DATA_TRANSMISSION_H

// Includes
//#include "POWER_MANAGEMENT.h"
#include "nrf_drv_saadc.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "nrf_delay.h"
#include "boards.h"
#include "ble_lbs.h"
#include "MAIN.h"
#include "ADC.h"
#include "POWER_MANAGEMENT.h"
#include "SENSOR_ACCESS_CONTROL.h"

// Macros
#define   SIZE_OF_FRAME 10

#define   READ_SENSOR_VALUES              0x10
#define   SEND_SENSOR_VALUES              0x20
#define   SLEEP_MODE_ENTER                0x30
#define   RESET_DEVICE                    0x40
#define   TRANS_PWR_UP                    0x50
#define   TRANS_PWR_DOWN                  0x51
#define   SEND_RSSI                       0x52
#define   CHANGE_NAME                     0x60

#define   READ_READY                      0xF0

/* Data Vector
*   
* Analog Data Values from Sensors
* Battery state in percent
* Name of Device  
*
*/
typedef struct {

uint16_t  temperature[14];
uint16_t  moisture[7];
uint16_t  ph1;
uint16_t  ph2;
uint16_t   battery_voltage;
uint8_t   current_frame_element;
bool      adc_new_data;
int8_t    tx_power;
uint8_t   device_name[18];
int16_t   sleep_time;
uint8_t   numSleep;   
// RHIZO_SENSOR_      - default name
// RHIZO_SENSOR_0_    - depends on system number
// RHIZO_SENSOR_0_00  - sensor number
// + '\0'
uint16_t  new_received_data;
} data_vector;


typedef data_vector *ptr_data_vector; 







// Variable
extern ptr_data_vector global_data_vector;
extern uint32_t transmission_frame[SIZE_OF_FRAME];
extern int8_t global_rssi_value_buffer[4];



// Functions
void init_data_vector(void);
void send_rssi_value(void);
void create_transmission_frame(void);
void transmit_frame(void);
void send_read_ready(void);


#endif /* DATA_TRANSMISSION_H */

