/*********************************************************************
* BLE_DATA_TRANSMISSION.c includes all functions for transmission of analog
* sensor datas
*
* Technische Hochschule Koeln | RhizoTech 25.12.2019
*
* @ALPO
*
/*********************************************************************/
#include "BLE_DATA_TRANSMISSION.h"

data_vector tvector;                                // Data_Vector
ptr_data_vector global_data_vector = &tvector;
receiver_flag_struct rec_flag;                      // Receiver Flag
p_receiver_flag_t receiver_flag = &rec_flag;
uint32_t ble_transmission_frame[SIZE_OF_BLE_FRAME]; // Transmission Frame

// Functions
void transmission_error(bool trans_error);



/* @author	ALPO
 *
 * @laste dit   24.05.2019
 *              
 * @brief	Initialising Data Vector
 *
 *              The Transmission Vector includes all
 *              necessary datas (sensor values, battery capacity etc.) and
 *              device names of the ble tranmission.
 *
 * @param       init_Data_Vector
 * 		
 *              -
 */
void init_data_vector(){

  // Allokieren von Speicher 
  //global_data_vector = (Data_Vector*)calloc(1,sizeof(Data_Vector));

  for(uint8_t i = 0; i<17; i++){
    global_data_vector->current_device_name[i] = 0;
  }
  global_data_vector->battery_voltage = 0;
  global_data_vector->state = 0;
  global_data_vector->new_data = 0;
  global_data_vector->ble_state = 0;

  global_data_vector->frame_element = 0;
  global_data_vector->tx_power = 8;

  for(uint8_t i = 0; i<7; i++){
    global_data_vector->temperature[i] = 0;
    global_data_vector->humidity[i] = 0;
  }
  global_data_vector->tx_power = 8;
  global_data_vector->sleep_time = SENSOR_SLEEPTIME_MIN;
  global_data_vector->numSleep = 0;
}




/* @author	ALPO
 *
 * @laste dit   20.06.2019
 *              
 * @brief	error_led
 *              LED0 (yellow LED) for indication
 *              of an transmission error
 *
 * @param       bool ERROR 
 * 		
 *              
 */
void error_led(bool ERROR){

  if(ERROR == true){
    bsp_board_led_on(0);
    while(true);
  } else {
    bsp_board_led_off(0);
  }
}


/* @author	ALPO
 *
 * @laste dit   28.01.2020
 *              
 * @brief       print_temperature();
 *              Printing received temperature
 *              values
 * @param 
 * 		
 *              -
 */
void print_temperature(){


    float temp_array[ANALOG_SENSOR_NUMBER];

    for(uint8_t n = 0; n<ANALOG_SENSOR_NUMBER; n++){
      temp_array[n] = ((float)(((spi_buffer[sens_name_t->sensor_index][n*2+2]-64) << 3) + ((spi_buffer[sens_name_t->sensor_index][(n*2)+3] >> 5)))/8);
    }

    // Print Sensor Value
    NRF_LOG_INFO("The Temperature Sensor 1 value is: " NRF_LOG_FLOAT_MARKER "", NRF_LOG_FLOAT(temp_array[0]));
    NRF_LOG_FLUSH();
    NRF_LOG_INFO("The Temperature Sensor 2 value is: " NRF_LOG_FLOAT_MARKER "", NRF_LOG_FLOAT(temp_array[1]));
    NRF_LOG_FLUSH();
}




/* @author	ALPO
 *
 * @laste dit   28.01.2020
 *              
 * @brief       print_humidity()
 *              Printing received humidity values
 */
void print_humidity(){


  float hum_array[ANALOG_SENSOR_NUMBER];

  for(uint8_t k = 0; k<ANALOG_SENSOR_NUMBER; k++){
    //hum_array[k] = -0.1964*((spi_buffer[sens_name_t->sensor_index][(k*2)+16] << 8) + spi_buffer[sens_name_t->sensor_index][(k*2)+17])+700;
    hum_array[k] =((spi_buffer[sens_name_t->sensor_index][(k*2)+18] << 8) + spi_buffer[sens_name_t->sensor_index][(k*2)+19]);    
    //NRF_LOG_INFO("Hum Sensor in %: " NRF_LOG_FLOAT_MARKER "", NRF_LOG_FLOAT(hum_array[k]));
    NRF_LOG_INFO("Hum Sensor in 0x%x:", hum_array[k]);
    NRF_LOG_FLUSH();
  }
}



/* @author	ALPO
 *
 * @laste dit   28.01.2020
 *              
 * @brie        print_ph()
 *              Printing received ph values
 *              At the moment the ph sensor is in progress, 
 *              there is no sensor available for testing
 */
void print_ph(){

  uint16_t ph1 = 0, ph2 = 0;

  ph1 =((spi_buffer[sens_name_t->sensor_index][36] << 8) + spi_buffer[sens_name_t->sensor_index][37]);
  ph2 =((spi_buffer[sens_name_t->sensor_index][38] << 8) + spi_buffer[sens_name_t->sensor_index][39]);

  NRF_LOG_INFO("PH1: 0x%x", ph1);
  NRF_LOG_INFO("PH2: 0x%x", ph2);

}

/* @author	ALPO
 *
 * @laste dit   28.12.2019
 *              
 * @brief       print_battery_state()
 *              Calculating batterie capacity in percent
 *              out of the adc raw values
 */
void print_battery_state(){

  uint32_t  bat = 0;
  float     ubat = 0;
  float     bat_cap = 0;

  bat =((spi_buffer[sens_name_t->sensor_index][34] << 8) + spi_buffer[sens_name_t->sensor_index][35]);



  // Convert ADC value in battery voltage (mV)
  ubat = (float)((3.3/4096)*bat)/2;

  // Determine the battery capacity
  // Capacity from 0% to 20%
  if((ubat >= 0.820) && (ubat < 1.1)){
    bat_cap = (64.6*ubat) - 54.2;

    // Capacity from 20% to 100%
  } else if ((ubat >= 0.820) && (ubat <= 1.500)){
    float temp = (10*ubat) - 12.7;
    bat_cap = (111)/(1+exp(-temp));
  } else {
    bat_cap = 0;
  }

    NRF_LOG_INFO("Battery Voltage in V : " NRF_LOG_FLOAT_MARKER "", NRF_LOG_FLOAT(ubat));
    NRF_LOG_INFO("Battery Capacity in percent:" NRF_LOG_FLOAT_MARKER "", NRF_LOG_FLOAT(bat_cap));

}



/* @author	ALPO
 *
 * @laste dit   20.12.2019
 *              
 * @brief	print_received_data            
 *
 */
void print_received_data(void){


  // Calculate and print Temperature 
  print_temperature();             
                      
  // Humidity calculation out of raw values
  print_humidity();

  // Battery State calculation
  print_battery_state();

  // PH1 and PH2 values
  print_ph();

}





/* @author	ALPO
 *
 * @laste dit   23.01.2020
 *              
 * @brief	data_to_spi_buffer            
 *              All ble data are received in a 4 byte buffer
 *              This data must be distribute in a one byte buffer
 *              to transmit this data over spi to the mainboard.
 */
void data_to_spi_buffer(uint8_t *sensor_number){

    spi_buffer[*sensor_number][0] = sens_name_t->sensor_index;
    spi_buffer[*sensor_number][1] = sens_name_t->sensor_number;

  // Temperature and Moisture sensor values
  for(uint8_t k = 0; k<8; k++){
    uint8_t b = (k*4)+2;
    spi_buffer[*sensor_number][b]   = ((ble_transmission_frame[k] >> 24) & 0xFF);
    spi_buffer[*sensor_number][b+1] = ((ble_transmission_frame[k] >> 16) & 0xFF);
    spi_buffer[*sensor_number][b+2] = ((ble_transmission_frame[k] >> 8)  & 0xFF);
    spi_buffer[*sensor_number][b+3] = ((ble_transmission_frame[k])       & 0xFF);
  }


  // Battery values
    spi_buffer[*sensor_number][34] = ((ble_transmission_frame[8] >> 8)  & 0xFF);
    spi_buffer[*sensor_number][35] = ((ble_transmission_frame[8])       & 0xFF);


  // PH 1 values
    spi_buffer[*sensor_number][36] = ((ble_transmission_frame[9] >> 24)  & 0xFF);
    spi_buffer[*sensor_number][37] = ((ble_transmission_frame[9] >> 16)  & 0xFF);

  // PH 2 values
    spi_buffer[*sensor_number][38] = ((ble_transmission_frame[9] >> 8)  & 0xFF);
    spi_buffer[*sensor_number][39] = ((ble_transmission_frame[9])       & 0xFF);

  // Print received data
  #if(NRF_LOG_ON)
    print_received_data();
  #endif

}


/* @author	ALPO
 *
 * @laste dit   24.01.2020
 *              
 * @brief	sensor_get_new_data()
 *              Request new data from sensor
 *
 */
void sensor_get_new_data(){

    bool trans_error = 0;


    // Send command to sensor for system reset
    if(get_connection_state() != true){
      #if(NRF_LOG_ON)
        NRF_LOG_INFO("Not connected!!");
        NRF_LOG_FLUSH();
        // System reset
        NVIC_SystemReset();
      #endif
    } else {

    // Send command to sensor for data receiving
    trans_error = send_data_frame_element(GET_SENSOR_VALUES);
    transmission_error(trans_error);
    // Set flag for rssi receive
    receiver_flag->fresh_data = 1;

    // Wait for new data
    while((receiver_flag->fresh_data == 1) && (trans_error == 0));
    }
}



/* @author	ALPO
 *
 * @laste dit   22.01.2020
 *              
 * @brief	sensor_read_values()
 *              Request sensor to read new data 
 *
 */
void sensor_read_values(){

    bool trans_error = 0;
    receiver_flag->read_ready = 0;
  
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Send: READ_SENSOR_VALUES");
      NRF_LOG_FLUSH();
    #endif

    // Send command to sensor for data receiving
    // Send command to sensor for system reset
    if(get_connection_state() != true){
      #if(NRF_LOG_ON)
        NRF_LOG_INFO("Not connected!!");
        NRF_LOG_FLUSH();
      #endif
    } else {

      trans_error = send_data_frame_element(READ_SENSOR_VALUES);
      transmission_error(trans_error);
      nrf_delay_ms(100);

      // Send sensor name number
      trans_error = send_data_frame_element(sens_name_t->sensor_number);
      transmission_error(trans_error);
    }

    nrf_delay_ms(1000);
}




/* @author	ALPO
 *
 * @laste dit   17.04.2020
 *              
 * @brief	sensor_sleep_time();
 *              Send sleep time to sensor 
 *
 */
void sensor_sleep_time(){

    bool trans_error = 0;
    receiver_flag->read_ready = 0;
  
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Send: CHANGE_SLEEP_TIME");
      NRF_LOG_FLUSH();
    #endif

    // Send command to sensor for data receiving
    // Send command to sensor for system reset
    if(get_connection_state() != true){
      #if(NRF_LOG_ON)
        NRF_LOG_INFO("Not connected!!");
        NRF_LOG_FLUSH();
      #endif
    } else {

      trans_error = send_data_frame_element(CHANGE_SLEEP_TIME);
      transmission_error(trans_error);
      nrf_delay_ms(100);

      // Send sleep time
      trans_error = send_data_frame_element(global_data_vector->sleep_time);
      #if(NRF_LOG_ON)
        NRF_LOG_INFO("Sleeptime: %x", global_data_vector->sleep_time);
        NRF_LOG_FLUSH();
      #endif
      transmission_error(trans_error);
      nrf_delay_ms(100);
    }
}



/* @author	ALPO
 *
 * @laste dit   24.01.2020
 *              
 * @brief	sensor_tx_power_up()
 *              Request sensor to increase the
 *              transmission power
 */
void sensor_tx_power_up(){

    bool trans_error = 0;
  
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Send: TRANS_PWR_UP");
      NRF_LOG_FLUSH();
    #endif

    // Send command to increase the transmission power
    // Send command to sensor for system reset
    if(get_connection_state() != true){
      #if(NRF_LOG_ON)
        NRF_LOG_INFO("Not connected!!");
        NRF_LOG_FLUSH();
      #endif
    } else {

      trans_error = send_data_frame_element(TRANS_PWR_UP);
      transmission_error(trans_error);
      nrf_delay_ms(100);

      // Send sensor name number
      trans_error = send_data_frame_element(sens_name_t->sensor_number);
      transmission_error(trans_error);
    }
}


/* @author	ALPO
 *
 * @laste dit   24.01.2020
 *              
 * @brief	sensor_tx_power_down()
 *              Request sensor to decrease the
 *              transmission power
 */
void sensor_tx_power_down(){

    bool trans_error = 0;

    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Send: TRANS_PWR_DOWN");
      NRF_LOG_FLUSH();
    #endif
  
    // Send command to decrease the transmission power
    // Send command to sensor for system reset
    if(get_connection_state() != true){
      #if(NRF_LOG_ON)
        NRF_LOG_INFO("Not connected!!");
        NRF_LOG_FLUSH();
      #endif
    } else {

      trans_error = send_data_frame_element(TRANS_PWR_DOWN);
      transmission_error(trans_error);
      nrf_delay_ms(100);

      // Send sensor name number
      trans_error = send_data_frame_element(sens_name_t->sensor_number);
      transmission_error(trans_error);
    }
}



/* @author	ALPO
 *
 * @laste dit   24.01.2020
 *              
 * @brief	sensor_sleep_mode_enter()
 *              Request  sensor to go 
 *              into sleep mode
 *
 * @param 
 * 		
 *              -
 */
void sensor_sleep_mode_enter(){
  
    bool trans_error = 0;

    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Send: SLEEP_MODE_ENTER");
      NRF_LOG_FLUSH();
    #endif

    // Send command to sensor for system reset
    if(get_connection_state() != true){
      #if(NRF_LOG_ON)
        NRF_LOG_INFO("Not connected!!");
        NRF_LOG_FLUSH();
      #endif
    } else {

      trans_error = send_data_frame_element(SLEEP_MODE_ENTER);
      transmission_error(trans_error);
      nrf_delay_ms(100);

      // Send sensor name number
      trans_error = send_data_frame_element(sens_name_t->sensor_number);
      transmission_error(trans_error);
    }
}






/* @author	ALPO
 *
 * @laste dit   24.01.2020
 *              
 * @brief	sensor_reset
 *              Request sensor to make a
 *              system reset
 * 
 */
void sensor_reset(){

    bool trans_error = 0;
  

    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Send: RESET_DEVICE");
      NRF_LOG_FLUSH();
    #endif

    // Send command to sensor for system reset
    if(get_connection_state() != true){
      #if(NRF_LOG_ON)
        NRF_LOG_INFO("Not connected!!");
        NRF_LOG_FLUSH();
      #endif
    } else {

      trans_error = send_data_frame_element(RESET_DEVICE);
      transmission_error(trans_error);
      nrf_delay_ms(100);

      // Send sensor name number
      trans_error = send_data_frame_element(sens_name_t->sensor_number);
      transmission_error(trans_error);
    }
}


/* @author	ALPO
 *
 * @laste dit   24.01.2020
 *              
 * @brief	sensor_change_name()
 *              Request sensor to change its name
 */
void sensor_change_name(){

    bool trans_error = 0;

    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Send: CHANGE_NAME");
      NRF_LOG_FLUSH();
    #endif
  
    // Send command to sensor for system reset
    if(get_connection_state() != true){
      #if(NRF_LOG_ON)
        NRF_LOG_INFO("Not connected!!");
        NRF_LOG_FLUSH();
      #endif
    } else {

      trans_error = send_data_frame_element(CHANGE_NAME);
      transmission_error(trans_error);
      nrf_delay_ms(100);

      // Send sensor name number
      trans_error = send_data_frame_element(sens_name_t->sensor_number);
      transmission_error(trans_error);
    }
}






/* @author	ALPO
 *
 * @laste dit   23.01.2020
 *              
 * @brief	sensor_clear_flash()
 *
 *              The sensor should clear 
 *              the flash before changing its name
 */
void sensor_clear_flash(){

    bool trans_error = 0;

    // Mandatory delay
    nrf_delay_ms(1000);

    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Send: FLASH_CLEAR");
      NRF_LOG_FLUSH();
    #endif

  
    // Send command to sensor for system reset
    if(get_connection_state() != true){
      #if(NRF_LOG_ON)
        NRF_LOG_INFO("Not connected!!");
        NRF_LOG_FLUSH();
      #endif
    } else {

      trans_error = send_data_frame_element(FLASH_CLEAR);
      transmission_error(trans_error);
      nrf_delay_ms(100);
    }
}






/* @author	ALPO
 *
 * @laste dit   25.01.2020
 *              
 * @brief	transmission_error()
 *              If a transmission error happened, 
 *              the error led will be glowing
 *
 * @param       bool trans_error
 */
void transmission_error(bool trans_error){
  
  // Error happend and device is connected 
  if((trans_error == 1) && (get_connection_state() == 1)){
    // Print log error
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Transmission error!");
      NRF_LOG_FLUSH();
    #endif
    // Start error timer, error led blinking
    error_timer_start();
    // Clode connection
    close_connection();
    nrf_delay_ms(1000);
  }
}








