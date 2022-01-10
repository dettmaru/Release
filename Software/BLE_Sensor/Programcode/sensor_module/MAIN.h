#ifndef MAIN_H
#define MAIN_H

 
// Defines

//-----------------------------------------------------------------------------
// Please set !!
//-----------------------------------------------------------------------------
#define SENSOR_SHORT_NAME               "RHIZO_SENSOR_"    // Name of device. Will be included in the advertising data
#define SENSOR_NUMBER                   4                 // Number of current sensor
#define SYSTEM_NUMBER                   1                   // Definiton of System Number, important to set!
#define SLEEP_TIME                      5                 // Sleep time in 1m, 3m, 5m, 10m, 15m
//-----------------------------------------------------------------------------


#ifndef NRF_LOG_ON                                         // Log printing On
#define NRF_LOG_ON 1
#endif

#ifndef FSM_TIMEOUT                                        // Timeout timer of the fsm
#define FSM_TIMEOUT 0
#endif

#ifndef NRF_RSSI_LOG_ON                                    // Printing of RSSI Values during a connection
#define NRF_RSSI_LOG_ON 1
#endif

#ifndef NRF_ADC_LOG_ON                                      // Printing ADC Log
#define NRF_ADC_LOG_ON 1
#endif

#ifndef PWR_LED_ON                                          // Turn On/Off the Power Led to save energy
#define PWR_LED_ON 1
#endif

//-----------------------------------------------------------------------------
#define APP_BLE_OBSERVER_PRIO           3                                       /**< Application's BLE observer priority. You shouldn't need to modify this value. */
#define APP_BLE_CONN_CFG_TAG            1                                       /**< A tag identifying the SoftDevice BLE configuration. */

#define APP_ADV_INTERVAL                200                                     /**< The advertising interval (in units of 0.625 ms; this value corresponds to 40 ms). */
#define APP_ADV_DURATION                BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED   /**< The advertising time-out (in units of seconds). When set to 0, we will never time out. */


#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(8, UNIT_1_25_MS)        /**< Minimum acceptable connection interval (0.5 seconds). */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(80, UNIT_1_25_MS)        /**< Maximum acceptable connection interval (1 second). */
#define SLAVE_LATENCY                   0                                       /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)         /**< Connection supervisory time-out (4 seconds). */

#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(20000)                  /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (15 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(5000)                   /**< Time between each call to sd_ble_gap_conn_param_update after the first call (5 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                       /**< Number of attempts before giving up the connection parameter negotiation. */

#define BUTTON_DETECTION_DELAY          APP_TIMER_TICKS(50)                     /**< Delay from a GPIOTE event until a button is reported as pushed (in number of timer ticks). */

#define DEAD_BEEF                       0xDEADBEEF                              /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */
#define UART_PRINTING_ENABLED
#define SAADC_BURST_MODE 1                                                      //Set to 1 to enable BURST mode, otherwise set to 0.

#define TRY_AGAIN                       1
#define NO_ERROR                        0
#define TRANSMISSION_ERROR              2

#define SECURITY_MODE                   1
#define SECUIRTY_LEVEL                  1


#define PASSKEY_TXT                     "Passkey:"                                  /**< Message to be displayed together with the pass-key. */
#define PASSKEY_TXT_LENGTH              8                                           /**< Length of message to be displayed together with the pass-key. */
#define PASSKEY_LENGTH                  6           

#define DONE                            1


// Deklaration of variables
bool send_data_frame(uint32_t i);
void advertising_stop();
static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID;                        /**< Handle of the current connection. */
static uint8_t m_adv_handle = BLE_GAP_ADV_SET_HANDLE_NOT_SET;                   /**< Advertising handle used to identify an advertising set. */
static uint8_t m_enc_advdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];                    /**< Buffer for storing an encoded advertising set. */
static uint8_t m_enc_scan_response_data[BLE_GAP_ADV_SET_DATA_SIZE_MAX];         /**< Buffer for storing an encoded scan data. */
static uint16_t global_event_id = 0;                                            /**< Current Event ID*/
static uint16_t global_transmission_count = 0;   



// Deklaration of Functions
void change_tx_power();
void close_connection();
bool get_connection_state(void);
 
#endif