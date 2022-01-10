/*********************************************************************
* SENSOR_ACCESS_CONTROL.c includes all functions for the Sensor access control
*
* Technische Hochschule Koeln | RhizoTech 07.01.2020
*
* @ALPO
/*********************************************************************/
#include "SENSOR_ACCESS_CONTROL.h"


// Global variables
led_struct led_v;
ptr_led_struct led = &led_v;
int8_t global_rssi_value_buffer[4];
control_flags ctr_flag;
ptr_control_flags ctr_flg = &ctr_flag;
sensor_name_struct sns;
ptr_sensor_name_struct sens_name_t = &sns;



 /* @author	ALPO
 *
 * @laste dit   12.01.2020
 *              
 * @brief	sac_init()
 *
 */
 void sac_init(){

    // Init global_data_vector
    init_data_vector();  

    for(int i = 0; i<4; i++){
      global_rssi_value_buffer[i] = 0;
    }

    // Set flags to zero
    ctr_flg->config_mode = 0;
    ctr_flg->all_sensors_found = 0;
    ctr_flg->no_sensors_found = 0;
    ctr_flg->sensor_found = 0;

    // Read data from flash
    flash_read();
    if(flash_data->all_sensors_init == 1){
      sens_name_t->sensor_number = NUMBER_OF_SENSORS;
      sens_name_t->system_number = SYSTEM_NUMBER;
    } else {
      sens_name_t->sensor_number = 0;
      sens_name_t->system_number = 0;
    }

    sens_name_t->sensor_index = 0;

}


 /* @author	ALPO
 *
 * @laste dit   08.01.2020
 *              
 * @brief	sensor_access_loop()
 *
 */
void sensor_access_loop(){

  uint16_t counter                        = 0;
  sens_name_t->sensor_index               = 0;
  ctr_flg->sensor_found                   = 0;
  ctr_flg->config_mode                    = 0;
  ctr_flg->sensor_access_loop             = 1;
  bool  sensor_found                      = 0;
  bool  sensor_not_found                  = 0;
  bool  first_sensor_connected            = 1;
  int8_t first_sensor_number              =-1;
  static bool first_sensor_is_saved       = 0;
  uint8_t count_sensor_connection_attempt = 0;
  uint8_t time_count                      = 0;

  // Prevent data transmission to Mainboard
  nrf_gpio_pin_clear(DAT_READY);
  nrf_delay_ms(1000);


  while(sens_name_t->sensor_number >= sens_name_t->sensor_index){

  NRF_LOG_INFO("Sensor number %d" ,sens_name_t->sensor_number);
  NRF_LOG_INFO("Sensor index %d" ,sens_name_t->sensor_index);
  NRF_LOG_FLUSH();
  nrf_delay_ms(100);

    // Start full name scan
    time_count = 0;
    if(get_connection_state() == 1){
      close_connection();
    }
    while((get_connection_state() == 1) && (time_count < 30)){
      time_count++;
      nrf_delay_ms(100);
    }
    combine_sensor_name();
    full_name_scan_init();
    if(ctr_flg->auto_scan_on_diconnect == 1){
      scan_stop();
    }
    scan_start(); 
    ctr_flg->auto_scan_on_diconnect = 1;
    sensor_not_found = 0;
    sensor_found = 0;
    counter = 0;

    while((sensor_found == 0) && (sensor_not_found == 0)){
      // sensor found and connected
      if(ctr_flg->sensor_found == 1){
        ctr_flg->sensor_found = 0;
        counter = 0;

        // Save first sensor number
        if(first_sensor_is_saved == 0){
          first_sensor_is_saved = 1;
          first_sensor_number = sens_name_t->sensor_index;
        }

        // Control transmission power from sensor and receiver
        control_transmission_power();
        nrf_delay_ms(200);


        // Send next sleep time
        sensor_sleep_time();
        nrf_delay_ms(200);

        // Send command for sensor data reading
        // and request the data
        global_data_vector->new_data = 0;
        sensor_read_values();

        time_count = 0;
        while((global_data_vector->new_data == 0) && (get_connection_state() == 1) && (time_count < 20)){
          
          #if(NRF_LOG_ON)
            NRF_LOG_INFO("Connection Wait State");
            NRF_LOG_FLUSH();
          #endif
          
          time_count++;
          nrf_delay_ms(200);
        }
            
        if(time_count == 20){
          close_connection();
        }

        // First sensor? Start Stopwatch after data transmission!
        if((sens_name_t->sensor_index == first_sensor_number)){
          sleep_2nd_call();
        }

        if(global_data_vector->new_data == 1){

        // copy new data to spi buffer
        data_to_spi_buffer(&sens_name_t->sensor_index);
        
        }

        // Reset frame element
        global_data_vector->frame_element = 0;
        // Increase index number
        sens_name_t->sensor_index++;
        sensor_found = 1;
        scan_stop();


      } else {

        nrf_delay_ms(10);
        counter++;
      }

      
      // Timeout!
      if(counter == 300){
        sensor_not_found = 1;
        sens_name_t->sensor_index++;
      }
    }

    // No sensors found?
    // Reset index number, start again
    if((first_sensor_number == -1) && (sens_name_t->sensor_index > sens_name_t->sensor_number)){
      sens_name_t->sensor_index = 0;
      count_sensor_connection_attempt++;
      // After the fifth attempt, go sleep for 1m and try again
      if(count_sensor_connection_attempt == 5){
        count_sensor_connection_attempt = 0;
        sleep_timer_start_1m();
        system_on_sleep_mode_enter();
      }
    }
  }


    // Ready for Data Transmission to Mainboard
    nrf_gpio_pin_set(DAT_READY);

    // Go to Sleepmode
    ctr_flg->auto_scan_on_diconnect = 0;
    scan_stop();
    sleep_2nd_call();
}


 /* @author	ALPO
 *
 * @laste dit   08.01.2020
 *              
 * @brief	sensor_configuration()
 *
 */
void sensor_configuration(){

  uint8_t wait_for_sensor_count = 0;
  uint8_t wait_count = 0;

  NRF_LOG_INFO("Read Switch State %x", read_switch_state());
  NRF_LOG_FLUSH();

  // Check switch 5 state for configuration mode
  if(read_switch_state() == 0x10){
    while(read_switch_state() != 0);


  // Start LED blinky for mode indication
  led->ERR = 1;
  led->BLE = 1;
  led->SPI = 1;
  led->PWR = 1;
  all_leds_off();
  conf_mode_timer_start_500ms();

  

  // Read switch input for sensor system number
  while(read_switch_state() < 0x10){
    sens_name_t->system_number = read_switch_state();
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Switchvalue: %d", sens_name_t->system_number);
      NRF_LOG_FLUSH();
    #endif
    nrf_delay_ms(400);
  }
  while(read_switch_state() >= 0x10);
  led->ERR = 0;
  led->BLE = 1;
  led->SPI = 1;
  led->PWR = 1;



    // Clear flash space
    flash_clear_all();


    // First configuration Step
    // Enabling scan for new sensors with default name
    // "RHIZO_SENSOR_x_"
    ctr_flg->auto_scan_on_diconnect = 1;
    short_name_scan_init();
    scan_start();

    // Set configuration flag
    ctr_flg->config_mode = 1;
    sens_name_t->sensor_number  = 0;
    sens_name_t->sensor_index   = 0;
    ctr_flg->all_sensors_found  = 0;
    ctr_flg->no_sensors_found   = 0;
    ctr_flg->sensor_found       = 0;

    // Scan for all sensors and change name
    while((ctr_flg->all_sensors_found == 0) && (ctr_flg->no_sensors_found == 0)){
      nrf_delay_ms(500);

      // sensor found and connected
      if(ctr_flg->sensor_found == 1){
        ctr_flg->sensor_found = 0;
      
      // Change sensor name
      sensor_clear_flash();
      combine_sensor_name();
      nrf_delay_ms(100);
      sensor_change_name();

      // Count sensor number
      sens_name_t->sensor_number++;
      NRF_LOG_INFO("Sensor Number: %d", sens_name_t->sensor_number);
      NRF_LOG_FLUSH();
      sens_name_t->sensor_index++;

      // Wait for disconnect by sensor
      // Close connection after 0,5s
      wait_count = 0;
      while((get_connection_state() == 0) && (wait_count < 5)){
        nrf_delay_ms(200);
        wait_count++;
        NRF_LOG_INFO("Wait Count: %d", wait_count);
        NRF_LOG_FLUSH();
        if(wait_count == 5){
          close_connection();
          NRF_LOG_INFO("Close Connection by Receiver");
          NRF_LOG_FLUSH();
        }
      }

        nrf_delay_ms(100);
        all_leds_off();

      } else {
        nrf_delay_ms(500);
        wait_for_sensor_count++;
        #if(NRF_LOG_ON)
          NRF_LOG_INFO("Zaehler: %d", wait_for_sensor_count);
          NRF_LOG_FLUSH();
        #endif
      }

      // abort condition
      if(sens_name_t->sensor_number == 31){
        ctr_flg->all_sensors_found = 1;
      } else if ((wait_for_sensor_count == 10) && (sens_name_t->sensor_number == 0)){
        ctr_flg->no_sensors_found = 1;
      } else if (wait_for_sensor_count == 10){
        ctr_flg->all_sensors_found = 1;
      }
    }


    // Count from zero
    sens_name_t->sensor_number--;
    sens_name_t->sensor_index--;

    // Save new sensors in flash space
    flash_data->sensor_number = sens_name_t->sensor_number;
    flash_data->system_number = sens_name_t->system_number;
    flash_data->all_sensors_init = 1;
    flash_write();

    // Stop LED Blinking
    led->ERR = 0;
    led->BLE = 0;
    led->SPI = 0;
    led->PWR = 0;
    conf_mode_timer_stop();
    nrf_delay_ms(600);
    nrf_gpio_pin_clear(PWR_LED);
    ctr_flg->auto_scan_on_diconnect = 0;
    scan_stop();

  }
}        


 /* @author	ALPO
 *
 * @laste dit   10.01.2020
 *              
 * @brief	Automatic transmission power change
 *              The function is called every new sensor connection
 *              and change the transmission power depending the
 *              received signal strength indication value
 * @param       
 * 		
 *              -
 */
 void control_transmission_power(){

    int8_t rssi_value = calc_rssi_value();

    // Change TX Power from receiver module
    if (rssi_value < -80){
      // TX Power up to improve connection quality
      //tx_power_up();
      sensor_tx_power_up();
    } else if (rssi_value > -60){
      // TX Power down to save energy 
      //tx_power_down();
      sensor_tx_power_down();
    }
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
  
    strcpy(global_data_vector->current_device_name, SENSOR_SHORT_NAME);

    // System number can only be 0 to 9
    if(sens_name_t->system_number > 9){
      sens_name_t->system_number = 9;
    }

    // Save system number in sensor name
    global_data_vector->current_device_name[13] = sens_name_t->system_number + 0x30;
    global_data_vector->current_device_name[14] = '_';

    // Save sensor naumber in sensor name
    if(sens_name_t->sensor_index < 9){
      global_data_vector->current_device_name[15] = 0 + 0x30;
      global_data_vector->current_device_name[16] = sens_name_t->sensor_index + 0x30;
    } else {
     
      // Convert uint8_t value > 9 to string
      uint8_t temp_system_index = sens_name_t->sensor_index;
      uint8_t ten_count = 0;
      while(temp_system_index > 9){
        temp_system_index -= 10;
        ten_count++;
      }
      global_data_vector->current_device_name[15] = ten_count + 0x30;
      global_data_vector->current_device_name[16] = temp_system_index + 0x30;
    }
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Sensor Name lautet: %s",global_data_vector->current_device_name); 
      NRF_LOG_FLUSH();
    #endif
 }  







/* @author	ALPO
 *
 * @laste dit   24.01.2020
 *              
 * @brief	calc_rssi_value
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


