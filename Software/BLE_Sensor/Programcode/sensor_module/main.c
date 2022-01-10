#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "ADC.h"
#include "POWER_MANAGEMENT.h"
#include "PERIPHERY.h"
#include "DATA_TRANSMISSION.h"
#include "SENSOR_ACCESS_CONTROL.h"
#include "MAIN.h"
#include "FLASH.h"
#include "CRYPTO.h"

#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "ble.h"
#include "ble_err.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_conn_params.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "boards.h"
#include "app_timer.h"
#include "app_button.h"
#include "ble_lbs.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_power.h"
#include "nrf_delay.h"
#include "nrf_drv_clock.h"
#include "math.h"
#include "bsp.h"


// GPIO Pins
#include "nrf_drv_gpiote.h"
#include "nrf_gpiote.h"

// Logging
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


BLE_LBS_DEF(m_lbs);            /**< LED Button Service instance. */
NRF_BLE_GATT_DEF(m_gatt);      /**< GATT module instance. */
NRF_BLE_QWR_DEF(m_qwr);        /**< Context for the Queued Write module.*/






/**@brief Struct that contains pointers to the encoded advertising data. */
static ble_gap_adv_data_t m_adv_data =
{
    .adv_data =
    {
        .p_data = m_enc_advdata,
        .len    = BLE_GAP_ADV_SET_DATA_SIZE_MAX
    },
    .scan_rsp_data =
    {
        .p_data = m_enc_scan_response_data,
        .len    = BLE_GAP_ADV_SET_DATA_SIZE_MAX

    }
};




/**@brief Function for assert macro callback.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num    Line number of the failing ASSERT call.
 * @param[in] p_file_name File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}







/**@brief Function for the GAP initialization.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name, appearance, and the preferred connection parameters.
 */
static void gap_params_init(void)
{
    ret_code_t              err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;


    // Security Modes MAIN.h

    if((SECURITY_MODE == 1) && (SECUIRTY_LEVEL == 1)){
      // LE Security Mode 1 - Level 1
      BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    } else if ((SECURITY_MODE == 1) && (SECUIRTY_LEVEL == 2)){
      // LE Security Mode 1 - Level 2
      BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&sec_mode);

    } else if ((SECURITY_MODE == 1) && (SECUIRTY_LEVEL == 3)){
      // LE Security Mode 1 - Level 3
      BLE_GAP_CONN_SEC_MODE_SET_ENC_WITH_MITM(&sec_mode);

    } else if ((SECURITY_MODE == 1) && (SECUIRTY_LEVEL == 4)){
      // LE Security Mode 1 - Level 4
      BLE_GAP_CONN_SEC_MODE_SET_LESC_ENC_WITH_MITM(&sec_mode);

    } else if ((SECURITY_MODE == 2) && (SECUIRTY_LEVEL == 1)){
      // LE Security Mode 2 - Level 1
      BLE_GAP_CONN_SEC_MODE_SET_SIGNED_NO_MITM(&sec_mode);

    } else if ((SECURITY_MODE == 2) && (SECUIRTY_LEVEL == 2)){
      // LE Security Mode 2 - Level 2
      BLE_GAP_CONN_SEC_MODE_SET_SIGNED_WITH_MITM(&sec_mode);
    }

    // Set Device Name
    combine_sensor_name();
    err_code = sd_ble_gap_device_name_set(&sec_mode,(const uint8_t *)global_data_vector->device_name,strlen(global_data_vector->device_name));
    APP_ERROR_CHECK(err_code);


    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the GATT module.
 */
static void gatt_init(void)
{
    ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, NULL);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the Advertising functionality.
 *
 * @details Encodes the required advertising data and passes it to the stack.
 *          Also builds a structure to be passed to the stack when starting advertising.
 */
static void advertising_init(void)
{
    ret_code_t    err_code;
    ble_advdata_t advdata;
    ble_advdata_t srdata;

    ble_uuid_t adv_uuids[] = {{LBS_UUID_SERVICE, m_lbs.uuid_type}};

    // Build and set advertising data.
    memset(&advdata, 0, sizeof(advdata));

    //advdata.name_type          = BLE_ADVDATA_SHORT_NAME;
    //advdata.short_name_len     = 10;
    advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance = true;
    advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE; 


    memset(&srdata, 0, sizeof(srdata));
    srdata.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    srdata.uuids_complete.p_uuids  = adv_uuids;

    err_code = ble_advdata_encode(&advdata, m_adv_data.adv_data.p_data, &m_adv_data.adv_data.len);
    APP_ERROR_CHECK(err_code);

    err_code = ble_advdata_encode(&srdata, m_adv_data.scan_rsp_data.p_data, &m_adv_data.scan_rsp_data.len);
    APP_ERROR_CHECK(err_code);

    ble_gap_adv_params_t adv_params;

    // Set advertising parameters.
    memset(&adv_params, 0, sizeof(adv_params));

    adv_params.primary_phy     = BLE_GAP_PHY_1MBPS;
    adv_params.duration        = APP_ADV_DURATION;
    adv_params.properties.type = BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED;
    adv_params.p_peer_addr     = NULL;
    adv_params.filter_policy   = BLE_GAP_ADV_FP_ANY;
    adv_params.interval        = APP_ADV_INTERVAL;

    err_code = sd_ble_gap_adv_set_configure(&m_adv_handle, &m_adv_data, &adv_params);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling Queued Write Module errors.
 *
 * @details A pointer to this function will be passed to each service which may need to inform the
 *          application about an error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void nrf_qwr_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for starting advertising.
 */
static void advertising_start(void)
{
    ret_code_t           err_code;

    err_code = sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_ADV, m_adv_handle, global_data_vector->tx_power);
    APP_ERROR_CHECK(err_code);

    err_code = sd_ble_gap_adv_start(m_adv_handle, APP_BLE_CONN_CFG_TAG);
    APP_ERROR_CHECK(err_code);
}



/**@brief Function for starting advertising.
 */
void advertising_stop(void)
{
    ret_code_t           err_code;

    err_code = sd_ble_gap_adv_stop(m_adv_handle);
    //APP_ERROR_CHECK(err_code);

    //bsp_board_led_on(ADVERTISING_LED);
}



/**@brief Function for handling read analog values
 *
 *  Central sends request for Analog reading (sensor)
 *  Calculating average of values in buffer
 *
 *  ADC Values are averaging internal by 32x oversampling (sdk_config)
 *
 * @param[in] p_lbs     Instance of ADC request Service.
 * @param[in] state     Trigger of ADC read.
 */


static void sensor_handler(uint16_t conn_handle, ble_lbs_t * p_lbs, uint16_t state)
{
    NRF_LOG_INFO("Packet received!: 0x%x", state);
    NRF_LOG_FLUSH();
    nrf_gpio_pin_toggle(PWR_LED);
    global_data_vector->new_received_data = state;

    // CHANGE_NAME
    if(ctr_flags->get_new_sensor_name == 1){
      ctr_flags->get_new_sensor_name = 0;
      ctr_flags->new_sensor_name_received = 1;
    
    // CHANGE_SLEEP_TIME
    } else if(ctr_flags->get_new_sleep_time == 1){
      ctr_flags->get_new_sleep_time = 0;
      ctr_flags->new_sensor_sleep_time_received = 1;

    } else if (ctr_flags->get_new_command) {
      // EMPTY
    } 
}






/**@brief Function for initializing services that will be used by the application.
 */
static void services_init(void)
{
    ret_code_t         err_code;
    ble_lbs_init_t     init     = {0};
    nrf_ble_qwr_init_t qwr_init = {0};

    // Initialize Queued Write Module.
    qwr_init.error_handler = nrf_qwr_error_handler;

    // Module for handling Queued Write operations.
    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
    APP_ERROR_CHECK(err_code);

    // Initialize LBS.
    init.sensor_handler = sensor_handler;

    err_code = ble_lbs_init(&m_lbs, &init);
    APP_ERROR_CHECK(err_code);
}



/**@brief Function for handling the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module that
 *          are passed to the application.
 *
 * @note All this function does is to disconnect. This could have been done by simply
 *       setting the disconnect_on_fail config parameter, but instead we use the event
 *       handler mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    ret_code_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}


/**@brief Function for handling a Connection Parameters error.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for initializing the Connection Parameters module.
 */
static void conn_params_init(void)
{
    ret_code_t             err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}






/**@brief Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
    // RSSI Reading
    int16_t rssi_value = 0;

    ret_code_t err_code;

    sd_ble_gattc_hv_confirm(p_ble_evt->evt.gap_evt.conn_handle, p_ble_evt->evt.gattc_evt.params.hvx.handle);

    // Save event ID for other applications
    global_event_id = p_ble_evt->header.evt_id;


    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            NRF_LOG_INFO("Connected");
            NRF_LOG_INFO("Startzeit: %d", app_timer_cnt_get());
            NRF_LOG_FLUSH();
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, m_conn_handle);
            APP_ERROR_CHECK(err_code);
            err_code = app_button_enable();
            APP_ERROR_CHECK(err_code);
            
            // Start RSSI reading
            sd_ble_gap_rssi_start(p_ble_evt->evt.gap_evt.conn_handle, 0 , 0);
            
            // Turn on LED
            nrf_gpio_pin_clear(PWR_LED);
            
            // Change transmission power
	    change_tx_power();

            // Set connected flag
            ctr_flags->device_connected = 1;

            break;

        case BLE_GAP_EVT_DISCONNECTED:
            NRF_LOG_INFO("Disconnected");
            NRF_LOG_FLUSH();
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            err_code = app_button_disable();
            APP_ERROR_CHECK(err_code);
            nrf_gpio_pin_set(PWR_LED);
            // Reset connected flag
            ctr_flags->device_disconnected = 1;
            sensor_fsm_state_transition();
            sensor_fsm();
            break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:

            err_code = sd_ble_gap_sec_params_reply(m_conn_handle,
                                                   BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP,
                                                   NULL,
                                                   NULL);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            NRF_LOG_INFO("PHY update request.")
            NRF_LOG_FLUSH();
            ble_gap_phys_t const phys =
            {
                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,
            };
            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
            APP_ERROR_CHECK(err_code);
        } break;


        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            // No system attributes have been stored.
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
            break;


        case BLE_GATTC_EVT_TIMEOUT:
            // Disconnect on GATT Client timeout event.
            NRF_LOG_INFO("GATT Client Timeout.");
            NRF_LOG_FLUSH();
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            //APP_ERROR_CHECK(err_code);

            // Lost connection
            // Transmission power up to 8dbm
            global_data_vector->tx_power = 8;
            tx_power_up();

            // Finite State Machine
            ctr_flags->gatt_client_timeout = 1;
            sensor_fsm_state_transition();
            sensor_fsm();
            break;


        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server timeout event.
            NRF_LOG_INFO("GATT Server Timeout.");
            NRF_LOG_FLUSH();
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            //APP_ERROR_CHECK(err_code);

            // Lost connection
            // Transmission power up to 8dbm
            global_data_vector->tx_power = 8;
            tx_power_up();

            // Finite State Machine
            ctr_flags->gatt_server_timeout = 1;
            sensor_fsm_state_transition();
            sensor_fsm();
            break;


        case BLE_GATTS_EVT_HVC:
            // Confirmation received from the peer.
            NRF_LOG_INFO("Confirmation received from the peer");
            NRF_LOG_FLUSH();
            break;


        case BLE_GATTS_EVT_HVN_TX_COMPLETE:
            // Notification transmission complete.
            NRF_LOG_INFO("Transmission complete");
            NRF_LOG_FLUSH();
            // Save transmitted packages
            global_transmission_count = p_ble_evt->evt.gatts_evt.params.hvn_tx_complete.count;
            break;

        case BLE_GAP_TIMEOUT_SRC_CONN:
        {
            NRF_LOG_INFO("Connection request timed out.");

            // Lost connection
            // Transmission power up to 8dbm
            global_data_vector->tx_power = 8;
            tx_power_up();

            // Finite State Machine
            ctr_flags->gap_timeout = 1;
            sensor_fsm_state_transition();
            sensor_fsm();
            
        }break;

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


/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
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

  if (ctr_flags->device_connected == 1){
    return true;
  } else{
    return false;
  }
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
        case LEDBUTTON_BUTTON:
            NRF_LOG_INFO("Send button state change.");
            err_code = ble_lbs_send_frame(m_conn_handle, &m_lbs, button_action);
            if (err_code != NRF_SUCCESS &&
                err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
                err_code != NRF_ERROR_INVALID_STATE &&
                err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING)
            {
                APP_ERROR_CHECK(err_code);
            }
            break;

        default:
            APP_ERROR_HANDLER(pin_no);
            break;
    }
}





/* @author	ALPO
 *
 * @laste dit   12.01.2019
 *              
 * @brief	send_data_frame
 *              
 *              Function to start event and sending
 *              Frame element
 *
 *              // Check before and after Transmission
 *              BLE_GATTS_EVT_HVN_TX_COMPLETE	Notification transmission complete.
 *              BLE_GATTS_EVT_HVC               Confirmation received from the peer.
 *
 *
 * @param       uint16_t current fame element
 *              
 */
bool send_data_frame(uint32_t i){

    ret_code_t err_code;

    err_code = ble_lbs_send_frame(m_conn_handle, &m_lbs, transmission_frame[i]);  


    // Handling return values
    switch(err_code){

        case NRF_SUCCESS:
            NRF_LOG_INFO("BLE_Trans: Transmission success");
            NRF_LOG_FLUSH();

            // Wait for feedback from GATT Server
            uint8_t count = 0;
            while(global_event_id != BLE_GATTS_EVT_HVN_TX_COMPLETE){
              nrf_delay_ms(10);
              count++;
              if(count == 100){
                count = 0;
                return TRY_AGAIN;
              }
            }
            global_event_id = 0;
            count = 0;
            return 0;

        break;

        case BLE_ERROR_INVALID_CONN_HANDLE:
            NRF_LOG_INFO("BLE_Trans: Invalid Connection Handle");
            NRF_LOG_FLUSH();
        break;

        case NRF_ERROR_INVALID_STATE:
            NRF_LOG_INFO("BLE_Trans: Invalid conn State or notifications not enabled in the cccd");
            NRF_LOG_INFO("BLE_Trans: notifications not enabled in the cccd");
            NRF_LOG_INFO("BLE_Trans: , an ATT_MTU exchange is ongoing");
            NRF_LOG_FLUSH();
        break;

        case NRF_ERROR_INVALID_ADDR:
            NRF_LOG_INFO("BLE_Trans: Invalid pointer supplied.");
            NRF_LOG_FLUSH();
        break;

        case NRF_ERROR_INVALID_PARAM:
            NRF_LOG_INFO("BLE_Trans: Invalid parameter(s) supplied.");
            NRF_LOG_FLUSH();
        break;

        case BLE_ERROR_INVALID_ATTR_HANDLE:
            NRF_LOG_INFO("BLE_Trans: nvalid attribute handle(s) supplied.");
            NRF_LOG_FLUSH();
        break;

        case BLE_ERROR_GATTS_INVALID_ATTR_TYPE:
            NRF_LOG_INFO("BLE_Trans: Invalid attribute type(s) supplied.");
            NRF_LOG_FLUSH();
        break;

        case NRF_ERROR_NOT_FOUND:
            NRF_LOG_INFO("BLE_Trans: Attribute not found.");
            NRF_LOG_FLUSH();
        break;

        case NRF_ERROR_FORBIDDEN:
            NRF_LOG_INFO("BLE_Trans: connection's current security level is lower than the one required");
            NRF_LOG_FLUSH();
        break;

        case NRF_ERROR_DATA_SIZE:
            NRF_LOG_INFO("BLE_Trans: Invalid data size(s) supplied.");
            NRF_LOG_FLUSH();
        break;

        case NRF_ERROR_BUSY:
            NRF_LOG_INFO("BLE_Trans: Procedure already in progress, retry.");
            NRF_LOG_FLUSH();
            return TRY_AGAIN;
        break;


        case BLE_ERROR_GATTS_SYS_ATTR_MISSING:
            NRF_LOG_INFO("BLE_Trans: System attributes missing, ");
            NRF_LOG_INFO("BLE_Trans: use sd_ble_gatts_sys_attr_set to set them to a known value.");
            NRF_LOG_FLUSH();
        break;


        case NRF_ERROR_RESOURCES:
            NRF_LOG_INFO("BLE_Trans: Too many notifications queued! ");
            NRF_LOG_FLUSH();
            return TRY_AGAIN;
        break;

        case NRF_ERROR_TIMEOUT:
            NRF_LOG_INFO("BLE_Trans: GATT procedure timeout, no new GATT");
            NRF_LOG_INFO("BLE_Trans: procedure can be performed without reestablishing the connection.");
            NRF_LOG_FLUSH();
        break;

        default:
            // No implementation needed
        break;
    }
}




static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}


/**@brief Function for initializing the button handler module.
 */
void buttons_init(void)
{
    ret_code_t err_code;

    //The array must be static because a pointer to it will be saved in the button handler module.
    static app_button_cfg_t buttons[] =
    {
        {LEDBUTTON_BUTTON, false, BUTTON_PULL, button_event_handler}
    };

    err_code = app_button_init(buttons, ARRAY_SIZE(buttons),
                               BUTTON_DETECTION_DELAY);
    APP_ERROR_CHECK(err_code);
}



void close_connection(){

  ret_code_t err_code;
  if(get_connection_state() == 1){
    ctr_flags->disconnect_by_user = 1;
    err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
    APP_ERROR_CHECK(err_code);
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

/**@brief Function for application main entry.
 */
int main(void)
{

    // Initialisation
    init_data_vector();
    log_init();
    leds_init();
    gpio_init();
    timers_init();
    buttons_init();
    power_management_init();
    ble_stack_init();
    flash_init();
    gap_params_init();
    gatt_init();
    services_init();
    conn_params_init();
    sac_init();
    create_timer();
    twi_init();

    // Start Advertising
    advertising_init();
    advertising_start();
    NRF_LOG_INFO("Advertising Started!");
    NRF_LOG_FLUSH();


    // Enter main loop.
    for (;;)

    {
       sensor_fsm_state_transition();
       sensor_fsm();
       idle_state_handle();
    }
   
}

