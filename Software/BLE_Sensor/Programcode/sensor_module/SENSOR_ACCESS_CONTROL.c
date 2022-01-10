/*********************************************************************
* SENSOR_ACCESS_CONTROL.c includes all functions for the Sensor access control
*
* Technische Hochschule Koeln | RhizoTech 07.01.2020
*
* @ALPO
/*********************************************************************/
#include "SENSOR_ACCESS_CONTROL.h"


// Global variables
control_flags_struct ctr_flag_data;
ptr_control_flags_struct  ctr_flags = &ctr_flag_data;
uint8_t fsm_state = 0;


 /* @author	ALPO
 *
 * @laste dit   16.01.2020
 *              
 * @brief	sac_init()
 */
void  sac_init(){
  ctr_flags->get_new_sleep_time = 0;
  ctr_flags->new_sensor_sleep_time_received = 0;
  ctr_flags->get_new_sensor_name = 0;
  ctr_flags->new_sensor_name_received = 0;
  ctr_flags->get_new_data = 0;
  ctr_flags->get_new_command = 0;
  ctr_flags->adc_reading_finish = 0;
  ctr_flags->spi_reading_finish = 0;
  ctr_flags->gap_timeout = 0;
  ctr_flags->gatt_client_timeout = 0;
  ctr_flags->gatt_server_timeout = 0;
  ctr_flags->device_connected = 0;
  ctr_flags->device_disconnected = 0;
  ctr_flags->disconnect_by_user = 0;

  fsm_state = DEFAULT;
}



 /* @author	ALPO
 *
 * @laste dit   15.01.2020
 *              
 * @brief	combine_sensor_name()
 * 		The sensor name consists out of
 *              the default name part "RHIZO_SENSOR_"
 *              defined in the MAIN.h header, the system
 *              number X and the sensor number YZ.
 *              "RHIZO_SENSOR_X_YZ"
 *              The advertising module needs the sensor name
 *              as an character array (string), so the numbers
 *              are converted with the ascii code to a character.
 */
 void combine_sensor_name(){
  
    strcpy(global_data_vector->device_name, SENSOR_SHORT_NAME);


    // Save system number in sensor name
    global_data_vector->device_name[13] = SYSTEM_NUMBER + 0x30;
    global_data_vector->device_name[14] = '_';


    // Save sensor naumber in sensor name
    if(SENSOR_NUMBER < 9){
      global_data_vector->device_name[15] = 0 + 0x30;
      global_data_vector->device_name[16] = SENSOR_NUMBER + 0x30;
    } else {
 
      // Convert uint8_t value > 9 to string
      uint8_t temp_system_index = SENSOR_NUMBER;
      uint8_t ten_count = 0;
      while(temp_system_index > 9){
        temp_system_index -= 10;
        ten_count++;
      }
      global_data_vector->device_name[15] = ten_count + 0x30;
      global_data_vector->device_name[16] = temp_system_index + 0x30;
    }

    // Nullterminierung
    global_data_vector->device_name[17] = '\0';
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Sensor Name lautet: %s",global_data_vector->device_name); 
      NRF_LOG_FLUSH();
    #endif


 }  







/* @author	ALPO
 *
 * @laste dit   24.01.2020
 *              
 * @brief	calc_rssi_value
 *              Calculating the moving average
 *              4th order of the rssi values
 *
 * @param calc_rssi_value
 * 		
 *              -
 */
int16_t calc_rssi_value(){

  // Buffer full flag
  static bool buffer_is_full = 0;
  int16_t ma_rssi_value = 0;

  // Wait until buffer is full
    buffer_is_full = 1;
    for(uint8_t i = 0; i<4; i++){
      if(global_rssi_value_buffer[i] == 0){
        buffer_is_full = 0;
      }
    }

    // Wait for more rssi events
    if(buffer_is_full == 0){
      return 0;
    }
  

  // Calculate 4. order moving average 
  for(uint8_t ctr = 0; ctr<4; ctr++){
    ma_rssi_value += global_rssi_value_buffer[ctr];
  }
  ma_rssi_value /= 4;

    // Print RSSI
  #if (NRF_RSSI_LOG_ON)
    NRF_LOG_INFO("RSSI Value: %d", ma_rssi_value);
    NRF_LOG_FLUSH();
  #endif    

  return ma_rssi_value;
}




/* @author	ALPO
 *
 * @laste dit   21.01.2020
 *              
 * @brief	Sensor Finite State Machine
 *
 */
void sensor_fsm_state_transition(){

  switch(fsm_state){

    // Get sleep time
    case CHANGE_SLEEP_TIME:
    {
      if ((global_data_vector->new_received_data == READ_SENSOR_VALUES) && (device_still_connected() == 1)){
         fsm_state = READ_SENSOR_VALUES;
      } else if (device_still_connected() == 1){
         fsm_state = DEFAULT;
      } else if ((ctr_flags->device_connected == 1) && (ctr_flags->device_disconnected == 1)){
         fsm_state = RESET_DEVICE;
      }
    }

    // Read all sensor values
    case READ_SENSOR_VALUES:
    {
       if((fsm_state == READ_SENSOR_VALUES) && (device_still_connected() == 1)){
          fsm_state = SEND_SENSOR_VALUES;
           NRF_LOG_INFO("State: %x", fsm_state);
           NRF_LOG_FLUSH();
       } else if ((fsm_state == READ_SENSOR_VALUES) && (device_still_connected() == 0)){
          fsm_state = RESET_DEVICE;
       }
    }
    break;
    
     // Send measured values to receiver
    case SEND_SENSOR_VALUES:
    {
       if((fsm_state == SEND_SENSOR_VALUES) && (device_still_connected() == 1)){
          fsm_state = SLEEP_MODE_ENTER;
       }else if ((fsm_state == SEND_SENSOR_VALUES) && (device_still_connected() == 0)){
          fsm_state = RESET_DEVICE;
       }
    }
    break;

    
    // System On Sleep Mode
    case SLEEP_MODE_ENTER:
    {
      if(fsm_state == SLEEP_MODE_ENTER){
        fsm_state = DEFAULT;
      }
    }
    break;


    case RESET_DEVICE:
    {
       if(fsm_state == RESET_DEVICE){
          fsm_state = DEFAULT;
       }
    }
    break;


    case TRANS_PWR_UP:
    {
       if((fsm_state == TRANS_PWR_UP) && (device_still_connected() == 1)){
          fsm_state = DEFAULT;
       } else if ((fsm_state == TRANS_PWR_UP) && (device_still_connected() == 0)){
          fsm_state = RESET_DEVICE;
       }
    }
    break;


    case TRANS_PWR_DOWN:
    {
       if((fsm_state == TRANS_PWR_DOWN) && (device_still_connected() == 1)){
          fsm_state = DEFAULT;
       }  else if ((fsm_state == TRANS_PWR_DOWN) && (device_still_connected() == 0)){
          fsm_state = RESET_DEVICE;
       }
    }
    break;


    case CHANGE_NAME:
    {
       if((fsm_state == CHANGE_NAME) && (device_still_connected() == 1)){
          fsm_state = SLEEP_MODE_ENTER;
       }else if ((fsm_state == CHANGE_NAME) && (device_still_connected() == 0) && (ctr_flags->disconnect_by_user == 0)){
          fsm_state = RESET_DEVICE;
       }
    }
    break;


    case FLASH_CLEAR:
    {
       if((fsm_state == FLASH_CLEAR) && (device_still_connected() == 1)){
          fsm_state = WAIT;
       }else if ((fsm_state == FLASH_CLEAR) && (device_still_connected() == 0)){
          fsm_state = RESET_DEVICE;
       }
    }
    break;



    case WAIT:
    {
       if((fsm_state == WAIT) && (global_data_vector->new_received_data == CHANGE_NAME) && (device_still_connected() == 1)){
          fsm_state = CHANGE_NAME;
       }else if ((fsm_state == WAIT) && (device_still_connected() == 0)){
          fsm_state = RESET_DEVICE;
       }
    }
    break;



    case TIMEOUT:
    {
       if((fsm_state == TIMEOUT)){
          fsm_state = DEFAULT;
       }
    }
    break;

    default:
    {
       if((fsm_state == DEFAULT) && (global_data_vector->new_received_data == FLASH_CLEAR) && (device_still_connected() == 1)){
          fsm_state = FLASH_CLEAR;
       } else if ((fsm_state == DEFAULT) && (global_data_vector->new_received_data == TRANS_PWR_DOWN) && (device_still_connected() == 1)){
          fsm_state = TRANS_PWR_DOWN;
       } else if ((fsm_state == DEFAULT) && (global_data_vector->new_received_data == TRANS_PWR_UP) && (device_still_connected() == 1)){
          fsm_state = TRANS_PWR_UP;
       } else if ((fsm_state == DEFAULT) && (global_data_vector->new_received_data == CHANGE_SLEEP_TIME) && (device_still_connected() == 1)){
          fsm_state = CHANGE_SLEEP_TIME;
       } else if ((ctr_flags->device_connected == 1) && (ctr_flags->device_disconnected == 1)){
          fsm_state = RESET_DEVICE;
       } else if ((global_data_vector->new_received_data == READ_SENSOR_VALUES) && (device_still_connected() == 1)){
         fsm_state = READ_SENSOR_VALUES;
       }
    }
    break;

  }
}



/* @author	ALPO
*
* @laste dit    21.12.2019
*              
* @brief	Sensor Finite State Machine
*
*
* @param       
* 		
*              -
*/
void sensor_fsm(){

  switch(fsm_state){


    // Read all sensor values
    case CHANGE_SLEEP_TIME:
    {

    // Implementation of Sleep Time change by Mainboard and Webservice.
    // The implementation is deactivated and the sleep time is set to a fixed value of 5 minutes
        global_data_vector->sleep_time = 1;


        // Set flag to receive sleep_time
        ctr_flags->get_new_sleep_time = 1;

        // Wait for new time
        while(ctr_flags->new_sensor_sleep_time_received == 0);


    }
    break;

    // Read all sensor values
    case READ_SENSOR_VALUES:
    {

        ctr_flags->adc_reading_finish = 0;
        ctr_flags->spi_reading_finish = 0;

        #if (NRF_LOG_ON)
          NRF_LOG_INFO("Read Sensor Values");
          NRF_LOG_FLUSH();
        #endif

        // TWI I2C 
        temp_sensor_device_test();
        temp_sensor_calibration();
        temp_sensor_write_settings();
        temp_sensor_read_all();

        // ADC
        saadc_sampling_event_init();
        saadc_init();

        // Init moisture clock (PWM)
        moisture_clock_init();
        start_moisture_clock();
        nrf_delay_ms(100);


        // Read analog Values
        saadc_read_values();

        // Wait for ADC and SPI
        while( (ctr_flags->spi_reading_finish == 0));
        #if (NRF_LOG_ON)
          NRF_LOG_INFO("Read sensor values finish");
          NRF_LOG_FLUSH();
        #endif


        // Disable Modules
        stop_moisture_clock();
        saadc_stop();
        uninit_moisture_clock();
        prepare_twi_to_sleep();
    }
    break;
    

     // Send measured values to receiver
    case SEND_SENSOR_VALUES:
    {


        #if (NRF_LOG_ON)
          NRF_LOG_INFO("Send Sensor Values");
          NRF_LOG_FLUSH();
        #endif

        create_transmission_frame();
        transmit_frame();
        nrf_gpio_pin_clear(PWR_LED);
    }
    break;

    
    // System On Sleep Mode
    case SLEEP_MODE_ENTER:
    {

        #if (NRF_LOG_ON)
          NRF_LOG_INFO("System On Sleep Mode");
          NRF_LOG_FLUSH();
        #endif

        system_on_sleep_mode_enter();
    }
    break;

    // System Reset
    case RESET_DEVICE:
    {

        #if (NRF_LOG_ON)
          NRF_LOG_INFO("System Reset");
          NRF_LOG_FLUSH();
        #endif
        NVIC_SystemReset();
    }
    break;

    // Increase transmission power
    case TRANS_PWR_UP:
    {
        
        tx_power_up();
        #if (NRF_LOG_ON)
          NRF_LOG_INFO("Transmission Power up %d db", global_data_vector->tx_power);
          NRF_LOG_FLUSH();
        #endif
    }
    break;

    // Decrease transmission power
    case TRANS_PWR_DOWN:
    {

        tx_power_down();
        #if (NRF_LOG_ON)
          NRF_LOG_INFO("Transmission Power down %d db", global_data_vector->tx_power);
          NRF_LOG_FLUSH();
        #endif

    }
    break;


    // Send transmission power
    case SEND_TX_PWR:
    {

        send_rssi_value();
        #if (NRF_LOG_ON)
          NRF_LOG_INFO("Send TX Power to Central");
          NRF_LOG_FLUSH();
        #endif

    }
    break;

    // Change Sensor Name
    case CHANGE_NAME:
    {

        #if (NRF_LOG_ON)
          NRF_LOG_INFO("Change Sensor Name");
          NRF_LOG_FLUSH();
        #endif

        // Set flag to receive sensor number
        ctr_flags->get_new_sensor_name = 1;

        // Wait for new name
        while(ctr_flags->new_sensor_name_received == 0);
        flash_data->sensor_name_number = global_data_vector->new_received_data;
        ctr_flags->new_sensor_name_received = 0;

        // Save sensor number in flash
        // Set init flag
        flash_data->sensor_init = true;
        flash_write();

        // Combine sensor name
        combine_sensor_name();

    }
    break;

    case FLASH_CLEAR:
    {

        #if (NRF_LOG_ON)
          NRF_LOG_INFO("Clear Flash");
          NRF_LOG_FLUSH();
        #endif
        flash_delete_file();
    }
    break;


    case WAIT:
    {
        #if (NRF_LOG_ON)
          NRF_LOG_INFO("Wait for new instructions");
          NRF_LOG_FLUSH();
        #endif
    }
    break;


    case TIMEOUT:
    {
      #if(NRF_LOG_ON)
        NRF_LOG_INFO("State Timeout!");
        NRF_LOG_FLUSH();
      #endif

      // Restart System to be sure that
      // system waiks up
      NVIC_SystemReset();
    }
    break;

    default:




    break;

  }
}






/* @author	ALPO
*
* @laste dit    29.01.2020
*              
* @brief	device_still_connected()
*               Function to verify the connection
*               during the data transmission and 
*               the state transistion of the fsm
*
*/
bool  device_still_connected(){

  // Timeout by GAP -> disconnect
  if(ctr_flags->gap_timeout == 1){
    return 0;
  // Timeout by GATT Client -> disconnect
  } else if(ctr_flags->gatt_client_timeout == 1){
    return 0;
  // Timeout by GATT Server -> disconnect
  } else if(ctr_flags->gatt_server_timeout == 1){
    return 0;
  // Premature disconnect
  } else if(ctr_flags->device_disconnected == 1) {
    return 0;
  // Device is still connected
  } else{
    return 1;
  }
}
