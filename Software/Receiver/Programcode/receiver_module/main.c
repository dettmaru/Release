/**
 * Copyright (c) 2014 - 2019, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompi
 , modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/**
 * @brief BLE LED Button Service central and client application main file.
 *
 * This file contains the source code for a sample client application using the LED Button service.
 */

#include "MAIN.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"
#include "nrf_pwr_mgmt.h"
#include "app_timer.h"
#include "boards.h"
#include "bsp.h"
#include "bsp_btn_ble.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "ble_db_discovery.h"
#include "ble_lbs_c.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_scan.h"
#include "nrf_delay.h"
#include "nrf_drv_clock.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"



NRF_BLE_SCAN_DEF(m_scan);                                       /**< Scanning module instance. */
BLE_LBS_C_DEF(m_ble_lbs_c);                                     /**< Main structure used by the LBS client module. */
NRF_BLE_GATT_DEF(m_gatt);                                       /**< GATT module instance. */
BLE_DB_DISCOVERY_DEF(m_db_disc);                                /**< DB discovery module instance. */



// Timeout for Singleshot Timer
static uint32_t timeout = 0;
static bool device_connected = 0;

/**@brief Function to handle asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num     Line number of the failing ASSERT call.
 * @param[in] p_file_name  File name of the failing ASSERT call.
 */
/*void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(0xDEADBEEF, line_num, p_file_name);
}
*/


/**@brief Function to start scanning.
 */
void scan_start(void)
{
    ret_code_t err_code;
     
    err_code = nrf_ble_scan_start(&m_scan);
    if(err_code != NRF_SUCCESS){
      NRF_LOG_INFO("scan_start: ERROR");
      NRF_LOG_FLUSH();
      APP_ERROR_CHECK(err_code);
    }
}


/**@brief Function to stop scanning.
 */
void scan_stop(void)
{
    nrf_ble_scan_stop();
}



/**@brief Handles events coming from the LED Button central module.
 */
static void lbs_c_evt_handler(ble_lbs_c_t * p_lbs_c, ble_lbs_c_evt_t * p_lbs_c_evt)
{
    switch (p_lbs_c_evt->evt_type)
    {
        case BLE_LBS_C_EVT_DISCOVERY_COMPLETE:
        {
            ret_code_t err_code;

            err_code = ble_lbs_c_handles_assign(&m_ble_lbs_c,
                                                p_lbs_c_evt->conn_handle,
                                                &p_lbs_c_evt->params.peer_db);
            NRF_LOG_INFO("LED Button service discovered on conn_handle 0x%x.", p_lbs_c_evt->conn_handle);

            err_code = app_button_enable();
            APP_ERROR_CHECK(err_code);

            // LED Button service discovered. Enable notification of Button.
            err_code = ble_lbs_c_button_notif_enable(p_lbs_c);
            APP_ERROR_CHECK(err_code);
        } break; // BLE_LBS_C_EVT_DISCOVERY_COMPLETE




        // New data received
        case BLE_LBS_C_EVT_BUTTON_NOTIFICATION:
        {   

            // Toggle BLE LED
            nrf_gpio_pin_toggle(BLE_LED);
            // Receive new sensor data

              receiver_flag->fresh_data = 0;
              // Save new data to transmission vector
              NRF_LOG_INFO("Data received!: %x", p_lbs_c_evt->params.button.button_state);
              NRF_LOG_FLUSH();
              ble_transmission_frame[global_data_vector->frame_element] = p_lbs_c_evt->params.button.button_state;
              global_data_vector->frame_element++;

              if(global_data_vector->frame_element > 9){
                global_data_vector->frame_element = 0;
                global_data_vector->new_data = true;
              }
            
           

        } break; // BLE_LBS_C_EVT_BUTTON_NOTIFICATION

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
    ret_code_t err_code;

    // RSSI Reading
    int16_t rssi_value = 0;

    // For readability.
    ble_gap_evt_t const * p_gap_evt = &p_ble_evt->evt.gap_evt;

    // save event state
    global_data_vector->ble_state = p_ble_evt->header.evt_id;

    switch (p_ble_evt->header.evt_id)
    {
        // Upon connection, check which peripheral has connected (HR or RSC), initiate DB
        // discovery, update LEDs status and resume scanning if necessary. */
        case BLE_GAP_EVT_CONNECTED:
        {
            NRF_LOG_INFO("Connected.");
            err_code = ble_lbs_c_handles_assign(&m_ble_lbs_c, p_gap_evt->conn_handle, NULL);
            //APP_ERROR_CHECK(err_code);
            
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;

            err_code = ble_db_discovery_start(&m_db_disc, p_gap_evt->conn_handle);
            //APP_ERROR_CHECK(err_code);

            // Start RSSI reading
            sd_ble_gap_rssi_start(p_ble_evt->evt.gap_evt.conn_handle, 0 , 0);

            // Update LEDs status, and check if we should be looking for more
            // peripherals to connect to.
            nrf_gpio_pin_clear(BLE_LED);

            //Change Transmission Power in active connection
            change_tx_power();

            // Check for configuration mode
            if(ctr_flg->config_mode == 1){
              ctr_flg->sensor_found = 1;
            }

            // Check for sensor_access_loop
            if(ctr_flg->sensor_access_loop == 1){
              ctr_flg->sensor_found = 1;
            }

            // Set connected flag
            device_connected = 1;
  

        } break;

        // Upon disconnection, reset the connection handle of the peer which disconnected, update
        // the LEDs status and start scanning again.
        case BLE_GAP_EVT_DISCONNECTED:
        {
            NRF_LOG_INFO("Disconnected.");
            nrf_gpio_pin_set(BLE_LED);

            // Start Scan only if it needed
            if(ctr_flg->auto_scan_on_diconnect){
              scan_start();
            }
            device_connected = 0;
        } break;

        case BLE_GAP_EVT_TIMEOUT:
        {
            // We have not specified a timeout for scanning, so only connection attemps can timeout.
            if (p_gap_evt->params.timeout.src == BLE_GAP_TIMEOUT_SRC_CONN)
            {
                NRF_LOG_DEBUG("Connection request timed out.");
                // Lost connection
                // Transmission power up to 8dbm
                global_data_vector->tx_power = 8;
                tx_power_up();
            }
        } break;

        case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:
        {
            // Accept parameters requested by peer.
            err_code = sd_ble_gap_conn_param_update(p_gap_evt->conn_handle,
                                        &p_gap_evt->params.conn_param_update_request.conn_params);
            APP_ERROR_CHECK(err_code);
        } break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            NRF_LOG_DEBUG("PHY update request.");
            ble_gap_phys_t const phys =
            {
                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,
            };
            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
            APP_ERROR_CHECK(err_code);
        } break;

        case BLE_GATTC_EVT_TIMEOUT:
        {
            // Disconnect on GATT Client timeout event.
            NRF_LOG_DEBUG("GATT Client Timeout.");
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);

            // Lost connection
            // Transmission power up to 8dbm
            global_data_vector->tx_power = 8;
            tx_power_up();
        } break;

        case BLE_GATTS_EVT_TIMEOUT:
        {
            // Disconnect on GATT Server timeout event.
            NRF_LOG_DEBUG("GATT Server Timeout.");
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);

            // Lost connection
            // Transmission power up to 8dbm
            global_data_vector->tx_power = 8;
            tx_power_up();

        } break;



        case BLE_GAP_EVT_RSSI_CHANGED:
        {
            static uint8_t buffer_counter = 0;

            global_rssi_value_buffer[buffer_counter] = p_ble_evt->evt.gap_evt.params.rssi_changed.rssi;

            if(buffer_counter == 3){
              buffer_counter = 0;
            } else {
              buffer_counter++;
            }
            
            // Calculate moving average of rssi
            // Print rssi value if defined in MAIN.h
            calc_rssi_value();
        }

        default:
            // No implementation needed.
            break;
    }
}




/* @author	ALPO
 *
 * @laste dit   20.01.2020
 *              
 * @brief	check_connection_state()
 *              Returns the state of an
 *              connection with:
 *              connected = true
 *              disconnect = false
 * @param 
 * 		
 *              -
 */
bool get_connection_state(){

  if (device_connected == 1){
    return true;
  } else{
    return false;
  }
}




/**@brief LED Button client initialization.
 */
static void lbs_c_init(void)
{
    ret_code_t       err_code;
    ble_lbs_c_init_t lbs_c_init_obj;

    lbs_c_init_obj.evt_handler = lbs_c_evt_handler;

    err_code = ble_lbs_c_init(&m_ble_lbs_c, &lbs_c_init_obj);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupts.
 */
static void ble_stack_init(void)
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}


/**@brief Function for handling events from the button handler module.
 *
 * @param[in] pin_no        The pin that the event applies to.
 * @param[in] button_action The button action (press/release).
 */
static void button_event_handler(uint8_t pin_no, uint8_t button_action)
{
    ret_code_t err_code;

    switch (pin_no)
    {
        case LEDBUTTON_BUTTON_PIN:
            err_code = ble_lbs_get_data_send(&m_ble_lbs_c, button_action);
            if (err_code != NRF_SUCCESS &&
                err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
                err_code != NRF_ERROR_INVALID_STATE)
            {
                APP_ERROR_CHECK(err_code);
            }
            if (err_code == NRF_SUCCESS)
            {
                NRF_LOG_INFO("LBS write LED state %d", button_action);
            }
            break;

        default:
            APP_ERROR_HANDLER(pin_no);
            break;
    }
}



/**@brief Function for handling Scaning events.
 *
 * @param[in]   p_scan_evt   Scanning event.
 */
static void scan_evt_handler(scan_evt_t const * p_scan_evt)
{
    ret_code_t err_code;

    switch(p_scan_evt->scan_evt_id)
    {
        case NRF_BLE_SCAN_EVT_CONNECTING_ERROR:
            err_code = p_scan_evt->params.connecting_err.err_code;
            APP_ERROR_CHECK(err_code);
            break;
        default:
          break;
    }
}





/**@brief Function for handling database discovery events.
 *
 * @details This function is callback function to handle events from the database discovery module.
 *          Depending on the UUIDs that are discovered, this function should forward the events
 *          to their respective services.
 *
 * @param[in] p_event  Pointer to the database discovery event.
 */
static void db_disc_handler(ble_db_discovery_evt_t * p_evt)
{
    ble_lbs_on_db_disc_evt(&m_ble_lbs_c, p_evt);
}


/**@brief Database discovery initialization.
 */
static void db_discovery_init(void)
{
    ret_code_t err_code = ble_db_discovery_init(db_disc_handler);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the log.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}






/* @author	ALPO
 *
 * @laste dit   07.01.2020
 *              
 * @brief	
 *
 * @param 
 * 		
 *              -
 */
static void scan_init_err_check(ret_code_t  err_code){

  switch (err_code){

    case NRF_SUCCESS:
      NRF_LOG_INFO("scan_init: the filter is added successfully");
      NRF_LOG_FLUSH();
    break;


    case NRF_ERROR_NULL:
      NRF_LOG_INFO("scan_init: a NULL pointer is passed as input");
      NRF_LOG_FLUSH();
    break;


    case NRF_ERROR_DATA_SIZE:
      NRF_LOG_INFO("scan_init: the name filter length is too long");
      NRF_LOG_FLUSH();
    break;


    case NRF_ERROR_NO_MEM:
      NRF_LOG_INFO("scan_init: the number of available filters is exceeded");
      NRF_LOG_FLUSH();
    break;

    case NRF_ERROR_INVALID_PARAM:
      NRF_LOG_INFO("scan_init: the filter type is incorrect");
      NRF_LOG_FLUSH();
    break;


    case BLE_ERROR_GAP_INVALID_BLE_ADDR:
      NRF_LOG_INFO("scan_init:  the BLE address type is invalid");
      NRF_LOG_FLUSH();
    break;

    default:
    break;

  }
}




/* @author	Nordic Semiconductor
 *
 * @edit by     ALPO
 *
 * @laste dit   09.01.2020
 *              
 * @brief	
 *
 * @param 
 * 		
 *              -
 */
void short_name_scan_init(void)
{
    ret_code_t          err_code;
    nrf_ble_scan_init_t init_scan;
    nrf_ble_scan_short_name_t short_name_struct;
    uint8_t short_name[14];

    strcpy(short_name, SENSOR_SHORT_NAME);
    short_name[13] = sens_name_t->system_number + 0x30;
    short_name[14] = '\0'; // End of string !

    short_name_struct.p_short_name = short_name;
    short_name_struct.short_name_min_len = SENSOR_SHORT_NAME_LENGTH;

    memset(&init_scan, 0, sizeof(init_scan));

    init_scan.connect_if_match = true;
    init_scan.conn_cfg_tag     = APP_BLE_CONN_CFG_TAG;

    err_code = nrf_ble_scan_init(&m_scan, &init_scan, scan_evt_handler);
    //APP_ERROR_CHECK(err_code);

    // Setting filters for scanning.
    err_code = nrf_ble_scan_filters_enable(&m_scan, NRF_BLE_SCAN_SHORT_NAME_FILTER, false);
    scan_init_err_check(err_code);

    //err_code = nrf_ble_scan_filter_set(&m_scan, SCAN_NAME_FILTER, default_sensor_name);
    //scan_init_err_check(err_code);

    err_code = nrf_ble_scan_filter_set(&m_scan, SCAN_SHORT_NAME_FILTER, &short_name_struct);
    scan_init_err_check(err_code);

    //Change Transmission Power in active connection
    err_code = sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_SCAN_INIT, NULL, global_data_vector->tx_power); 
    scan_init_err_check(err_code);   
}



/* @author	Nordic Semiconductor
 *
 * @edit by     ALPO
 *
 * @laste dit   09.01.2020
 *              
 * @brief	
 *
 * @param 
 * 		
 *              -
 */
void full_name_scan_init(void)
{
    ret_code_t          err_code;
    nrf_ble_scan_init_t init_scan;
 
    memset(&init_scan, 0, sizeof(init_scan));

    init_scan.connect_if_match = true;
    init_scan.conn_cfg_tag     = APP_BLE_CONN_CFG_TAG;

    err_code = nrf_ble_scan_init(&m_scan, &init_scan, scan_evt_handler);
    //APP_ERROR_CHECK(err_code);

    // Setting filters for scanning.
    err_code = nrf_ble_scan_filters_enable(&m_scan, NRF_BLE_SCAN_NAME_FILTER, false);
    scan_init_err_check(err_code);


    err_code = nrf_ble_scan_filter_set(&m_scan, SCAN_NAME_FILTER, global_data_vector->current_device_name);
    scan_init_err_check(err_code);

    //Change Transmission Power in active connection
    err_code = sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_SCAN_INIT, NULL, global_data_vector->tx_power); 
    scan_init_err_check(err_code);   
}


/**@brief Function for initializing the GATT module.
 */
static void gatt_init(void)
{
    ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, NULL);
    APP_ERROR_CHECK(err_code);
}



void close_connection(){

    ret_code_t err_code;
    if(device_connected == 1){
      err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
      APP_ERROR_CHECK(err_code);
    }
}


/* @author	ALPO
 *
 * @laste dit   09.01.2020
 *              
 * @brief	Function to send data crames
 *              
 *
 * @param 
 * 		
 *              -
 */
bool send_data_frame_element(uint16_t byte_to_send){

    ret_code_t err_code;
    uint8_t counter = 0;

    // Send first Element
    err_code = ble_lbs_get_data_send(&m_ble_lbs_c, byte_to_send);  

    if(err_code == NRF_SUCCESS){
      #if(NRF_LOG_ON)
        NRF_LOG_INFO("Sending success!");
        NRF_LOG_FLUSH();
      #endif
    }

    if (err_code != NRF_SUCCESS){
      nrf_delay_ms(100);
      while((ble_lbs_get_data_send(&m_ble_lbs_c, byte_to_send != NRF_SUCCESS)) && (counter < 100)){
      nrf_delay_ms(20);
      counter++;
      }
      if(counter == 100){
        #if(NRF_LOG_ON)
          NRF_LOG_INFO("TRANSMISSION_ERROR");
          NRF_LOG_FLUSH();
        #endif
        return TRANSMISSION_ERROR;
      } else {
        #if(NRF_LOG_ON)
          NRF_LOG_INFO("NO_ERROR");
          NRF_LOG_FLUSH();
        #endif
        return NO_ERROR;
      }
    } else {
      #if(NRF_LOG_ON)
        NRF_LOG_INFO("NO_ERROR");
        NRF_LOG_FLUSH();
      #endif
      return NO_ERROR;
    }
}



void change_tx_power(){
    	   
    uint32_t err_code = sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_CONN, m_conn_handle, global_data_vector->tx_power); 

    #if(NRF_LOG_ON)
      switch (err_code){
        case  NRF_SUCCESS:
            NRF_LOG_INFO("Successfully changed the transmit power: %d dB", global_data_vector->tx_power);
            NRF_LOG_FLUSH();
            break;
        case  NRF_ERROR_INVALID_PARAM:
            NRF_LOG_INFO("Invalid parameter(s) supplied.");
            NRF_LOG_FLUSH();
            break;
        case  BLE_ERROR_INVALID_ADV_HANDLE:
            NRF_LOG_INFO("Advertising handle not found.");
            NRF_LOG_FLUSH();
            break;
        case  BLE_ERROR_INVALID_CONN_HANDLE:
            NRF_LOG_INFO("Invalid connection handle supplied.");
            NRF_LOG_FLUSH();
            break;
      }
    #endif
}



void main_init_functions(){


    log_init();
    spis_init(); 
    timer_init();
    gpio_init();
    flash_init();

}




int main(void)
{

    // Initialize.
    //read_chip_temperature_init();
    main_init_functions();
    gpio_event_init();
    power_management_init();
    ble_stack_init();
    create_timers();
    gatt_init();
    db_discovery_init();
    lbs_c_init();
    sac_init();
     

    // check switch state for config mode
    sensor_configuration();


    // Start execution.
    NRF_LOG_INFO("BLE-Part of Sensor Box started!");
    NRF_LOG_FLUSH();

   
   
    sensor_access_loop();

    // Main routine for Data receiving/transmission


    for (;;)
    {
      //nrf_delay_ms(1000);
      idle_state_handle();
    }
}