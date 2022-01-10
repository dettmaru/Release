/*********************************************************************
* POWER_MANAGEMENT.h includes all power management functions
*
* Technische Hochschule Koeln | RhizoTech 20.12.2019
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
#include "DATA_TRANSMISSION.h"
#include "math.h"
#include "ADC.h"
#include "bsp.h"
#include "MAIN.h"

// Defines
#define AVG_LENGTH 16


// Deklaration of variables



// Deklaration of Functions
void power_management_init(void);
void idle_state_handle(void);
void check_battery_state(void);
void deep_sleep_mode_enter(void);
void idle_state_handle(void);
void sleep_timer_start(void);
void system_on_sleep_mode_enter(void);
bool tx_power_up(void);
bool tx_power_down(void);
#endif /* POWER_MANAGEMENT */
