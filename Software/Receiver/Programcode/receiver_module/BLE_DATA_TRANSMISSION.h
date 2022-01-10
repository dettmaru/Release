/*********************************************************************
* BLE_DATA_TRANSMISSION.h includes all functions for transmission of analog
* sensor datas
*
* Technische Hochschule Koeln | RhizoTech 25.12.2019
* @ALPO
*
/*********************************************************************/


#ifndef BLE_DATA_TRANSMISSION_H
#define BLE_DATA_TRANSMISSION_H

// Includes
//#include "POWER_MANAGEMENT.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> // Calloc
#include <string.h>
#include "nrf_drv_saadc.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "nrf_delay.h"
#include "boards.h"
#include "ble_lbs.h"
#include "MAIN.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "SPI_DATA_TRANSMISSION.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

// Macros
#define   SIZE_OF_BLE_FRAME               10
#define   ANALOG_SENSOR_NUMBER            2 
#define   MAX_N_CHECK                     4

#define   READ_SENSOR_VALUES              0x10
#define   GET_SENSOR_VALUES               0x20
#define   SLEEP_MODE_ENTER                0x30
#define   RESET_DEVICE                    0x40
#define   TRANS_PWR_UP                    0x50
#define   TRANS_PWR_DOWN                  0x51
#define   GET_RSSI                        0x52
#define   CHANGE_NAME                     0x60
#define   FLASH_CLEAR                     0x70
#define   READ_READY                      0xF0

/* Data Vector */
typedef struct {

float     temperature[7];
float     humidity[7];
uint8_t   battery_voltage;
uint8_t   current_device_name[17];
uint8_t   state;
bool      new_data;
uint8_t   ble_state;
uint8_t   frame_element;
uint8_t   received_byte;
int8_t    tx_power;
uint8_t   numSleep;
int16_t   sleep_time;   // Minutes
} data_vector;

typedef data_vector *ptr_data_vector; 


typedef struct{
  bool rssi;
  bool fresh_data;
  bool read_ready;
  bool wait_for_sensor;

} receiver_flag_struct;

typedef receiver_flag_struct * p_receiver_flag_t;



// Variable
extern ptr_data_vector global_data_vector;
extern data_vector tvector;
extern p_receiver_flag_t receiver_flag;
extern receiver_flag_struct rec_flag;
extern uint32_t ble_transmission_frame[SIZE_OF_BLE_FRAME];




// Functions
void init_data_vector(void);
void transmit_frame(void);
bool sensor_init_data_transmission(void);
bool sensor_start_data_reading(void);
void error_led(bool ERROR);
void data_to_spi_buffer(uint8_t *sensor_number);
void print_received_data(void);
void sensor_reset(void);
void sensor_sleep_mode_enter(void);
void sensor_tx_power_down(void);
void sensor_tx_power_up(void);
void sensor_get_new_data(void);
void sensor_get_rssi(void);
void sensor_clear_flash(void);
void sensor_change_name(void);
void sensor_read_values(void);
void transmission_error(bool trans_error);
void sensor_sleep_time(void);



#endif /* BLE_DATA_TRANSMISSION_H */
