/*********************************************************************
* SENSOR_ACCESS_CONTROL.h includes all functions for the Sensor access control
*
* Technische Hochschule Koeln | RhizoTech 07.01.2020
*
* @ALPO
/*********************************************************************/
#ifndef SENSOR_ACCESS_CONTROL_H
#define SENSOR_ACCESS_CONTROL_H


// Includes
#include "PERIPHERY.h"
#include "FLASH.h"
#include "BLE_DATA_TRANSMISSION.h"
#include "POWER_MANAGEMENT.h"
#include "MAIN.h"
#include "nrf_delay.h"
#include "ble.h"
#include "ble_err.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

// Macros
#define   READ_SENSOR_VALUES              0x10
#define   SEND_SENSOR_VALUES              0x20
#define   SLEEP_MODE_ENTER                0x30
#define   RESET_DEVICE                    0x40
#define   TRANS_PWR_UP                    0x50
#define   TRANS_PWR_DOWN                  0x51
#define   GET_TX_PWR                      0x52
#define   CHANGE_NAME                     0x60
#define   FLASH_CLEAR                     0x70
#define   CHANGE_SLEEP_TIME               0x80

// Variables                                     
typedef struct{

  bool new_init;
  bool change_system_number;
  bool search_sensors;
  bool initialized;

} init_state_vector;


// Struct for LED blinking in config mode
typedef struct{
  bool SPI;
  bool BLE;
  bool ERR;
  bool PWR;

} led_struct;
typedef led_struct *ptr_led_struct;



// Flags for configuration mode
typedef struct{
  bool config_mode;
  bool all_sensors_found;
  bool no_sensors_found;
  bool sensor_found;
  bool sensor_access_loop;
  bool all_data_received;
  bool auto_scan_on_diconnect;
} control_flags;
typedef control_flags *ptr_control_flags;



// Sensor name information
// RHIZO_SENSOR_"system_number"_"sensor_index"
// RHIZO_SENSOR_      0_             00
typedef struct{
  uint8_t sensor_number;
  uint8_t sensor_index;
  uint8_t system_number;

} sensor_name_struct;

typedef sensor_name_struct *ptr_sensor_name_struct;



extern ptr_sensor_name_struct sens_name_t;
extern ptr_control_flags ctr_flg;
extern int8_t global_rssi_value_buffer[4];
extern ptr_led_struct led;


// Functions
void sensor_configuration(void);
void control_transmission_power(void);
void sac_init(void);
void sensor_access_loop(void);
void combine_sensor_name(void);
int16_t calc_rssi_value(void);

#endif