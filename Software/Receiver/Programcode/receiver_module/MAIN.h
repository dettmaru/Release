/*********************************************************************
* MAIN.h includes all power management functions
*
* Technische Hochschule Koeln | RhizoTech 25.12.2019
*
* @ALPO
*
/*********************************************************************/

#ifndef MAIN_H
#define MAIN_H
 

// Includes
#include "POWER_MANAGEMENT.h"
#include "PERIPHERY.h"
#include "BLE_DATA_TRANSMISSION.h"
#include "SENSOR_ACCESS_CONTROL.h"
#include "FLASH.h"


// Defines
          
//______________________________________________________________________
// Please set !!
//______________________________________________________________________                                             
#define NUMBER_OF_SENSORS               4                                   // Number of Sensors                                                   
#define SYSTEM_NUMBER                   1                                    // System Number#
#define SENSOR_SLEEPTIME_MIN            5                                    // Sleeptime in Minutes
//______________________________________________________________________

#define SENSOR_SHORT_NAME               "RHIZO_SENSOR_"                     // Advertising short name "RHIZO_SENSOR_X"
#define SENSOR_SHORT_NAME_LENGTH        13+1                                // RHIZO_SENSOR_ = 13 + System Number X
#define SENSOR_FULL_NAME_LENGTH         17                                  // RHIZO_SENSOR_X_YZ = 17 character


#ifndef NRF_LOG_ON                                                          // Printing log on
#define NRF_LOG_ON 1
#endif

#ifndef NRF_RSSI_LOG_ON                                                     // Printing RSSI Values during the connection
#define NRF_RSSI_LOG_ON 0
#endif

#ifndef NRF_LED_ON                                                          // Turn on/off leds
#define NRF_LED_ON 1
#endif

#ifndef NRF_FLASH_LOG_ON                                                    // Printing Flash Log
#define NRF_FLASH_LOG_ON 0
#endif
//______________________________________________________________________
#define SCAN_INTERVAL                   0x00A0                              /**< Determines scan interval in units of 0.625 millisecond. */
#define SCAN_WINDOW                     0x0050                              /**< Determines scan window in units of 0.625 millisecond. */
#define SCAN_DURATION                   0x0000                              /**< Timout when scanning. 0x0000 disables timeout. */

#define MIN_CONNECTION_INTERVAL         MSEC_TO_UNITS(8, UNIT_1_25_MS)    /**< Determines minimum connection interval in milliseconds. */
#define MAX_CONNECTION_INTERVAL         MSEC_TO_UNITS(80, UNIT_1_25_MS)     /**< Determines maximum connection interval in milliseconds. */
#define SLAVE_LATENCY                   0                                   /**< Determines slave latency in terms of connection events. */
#define SUPERVISION_TIMEOUT             MSEC_TO_UNITS(4000, UNIT_10_MS)     /**< Determines supervision time-out in units of 10 milliseconds. */

#define LEDBUTTON_BUTTON_PIN            BSP_BUTTON_0                        /**< Button that will write to the LED characteristic of the peer */
#define BUTTON_DETECTION_DELAY          APP_TIMER_TICKS(50)                 /**< Delay from a GPIOTE event until a button is reported as pushed (in number of timer ticks). */

#define APP_BLE_CONN_CFG_TAG            1                                   /**< A tag identifying the SoftDevice BLE configuration. */
#define APP_BLE_OBSERVER_PRIO           3                                   /**< Application's BLE observer priority. You shouldn't need to modify this value. */

#define NO_ERROR                        0
#define TRANSMISSION_ERROR              1

#define SECURITY_MODE                   1
#define SECUIRTY_LEVEL                  1




// Deklaration of variables
bool send_data_frame_element(uint16_t byte_to_send);
static bool device_connected;


// Deklaration of functions
static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID;                       
void short_name_scan_init(void);
void scan_start(void);
void scan_stop(void); 
void change_tx_power(void);
void close_connection(void);
void full_name_scan_init(void);
bool get_connection_state(void);
void main_init_functions(void);

#endif