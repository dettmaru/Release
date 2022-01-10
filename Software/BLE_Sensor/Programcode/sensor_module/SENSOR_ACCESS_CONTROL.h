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
#include "DATA_TRANSMISSION.h"
#include "POWER_MANAGEMENT.h"
#include "MAIN.h"
#include "nrf_delay.h"
#include "ble.h"
#include "ble_err.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

// Macros
#define   DEFAULT                         0x00
#define   READ_SENSOR_VALUES              0x10
#define   SEND_SENSOR_VALUES              0x20
#define   SLEEP_MODE_ENTER                0x30
#define   RESET_DEVICE                    0x40
#define   TRANS_PWR_UP                    0x50
#define   TRANS_PWR_DOWN                  0x51
#define   SEND_TX_PWR                     0x52
#define   CHANGE_NAME                     0x60
#define   FLASH_CLEAR                     0x70
#define   CHANGE_SLEEP_TIME               0x80
#define   TIMEOUT                         0xFF
#define   WAIT                            0xFE


// Variables                                     
typedef struct{

  bool  get_new_sleep_time;
  bool  new_sensor_sleep_time_received;
  bool  get_new_sensor_name;
  bool  new_sensor_name_received;
  bool  get_new_data;
  bool  get_new_command;
  bool  spi_reading_finish;
  bool  adc_reading_finish;
  bool  gap_timeout;
  bool  gatt_server_timeout;
  bool  gatt_client_timeout;
  bool  device_connected;
  bool  device_disconnected;
  bool  disconnect_by_user;

} control_flags_struct;

typedef control_flags_struct *ptr_control_flags_struct;
extern ptr_control_flags_struct  ctr_flags;



// Functions
int16_t calc_rssi_value(void);
void combine_sensor_name(void);
void sensor_fsm(void);
void sensor_fsm_state_transition(void);
void sac_init(void);
bool device_still_connected(void);
#endif