/*********************************************************************
* POWER_MANAGEMENT.h includes all power management functions
*
* Technische Hochschule Koeln | RhizoTech 25.12.2019
*
* @ALPO
*
/*********************************************************************/
#ifndef POWER_MANAGEMENT_H
#define POWER_MANAGEMENT_H
 
// Includes
#include "nrf_pwr_mgmt.h"
#include "app_error.h"
#include "ble.h"
#include "ble_err.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "bsp.h"
#include "math.h"
#include "ble.h"
#include "ble_err.h"

#include "MAIN.h"
#include "BLE_DATA_TRANSMISSION.h"

// Defines
 
 
 
// Deklaration of variables
 
 
 
// Deklaration of Functions
void power_management_init(void);
void idle_state_handle(void);
void system_on_sleep_mode_enter(void);
bool tx_power_down(void);
bool tx_power_up(void);
void sleep_2nd_call(void);
#endif /* POWER_MANAGEMENT */