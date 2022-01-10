/*********************************************************************
* DATA_TRANSMISSION.c includes all functions for transmission of analog
* sensor datas
*
* Technische Hochschule Koeln | RhizoTech 25.12.2019
*
* @Alexander Porta
*
/*********************************************************************/
#include "DATA_TRANSMISSION.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

// Data_Vector
data_vector tvector;
ptr_data_vector global_data_vector = &tvector;

// Receiver Flag
receiver_flag_struct rec_flag;
p_receiver_flag_t receiver_flag = &rec_flag;

// Transmission Frame
uint8_t transmission_frame[SIZE_OF_FRAME];

// Functions
void transmission_error(bool trans_error);



/* @author	Alexander Porta
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

  global_data_vector->frame_element = 1;
  global_data_vector->tx_power = 8;

  for(uint8_t i = 0; i<7; i++){
    global_data_vector->temperature[i] = 0;
    global_data_vector->humidity[i] = 0;
  }
  global_data_vector->tx_power = 8;
}




/* @author	Alexander Porta
 *
 * @laste dit   20.06.2019
 *              
 * @brief	
 *              error_LED trun on 
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










/* @author	Alexander Porta
 *
 * @laste dit   24.05.2019
 *              
 * @brief	transmit_frame
 *
 *              Transmission of Frame element by element
 *              Max time of transmission: 16*10ms = 160ms
 *
 * @param transmit_frame
 * 		
 *              -
 */
void get_frame(){
  
  int i = 0; // Counter

  for(i = 0; i<SIZE_OF_FRAME;i++){

    // Sending new frame Element
    //send_data_frame(i);

    //Wait 10ms
    nrf_delay_ms(10);
  }

}




/* @author	Alexander Porta
 *
 * @laste dit   28.12.2019
 *              
 * @brie
 *
 * @param calc_temperature
 * 		
 *              -
 */
static void calc_temperature(){


    uint16_t sum = 0;

    for(int i = 0; i<7; i++){
      global_data_vector->temperature[i] = ((float)(((transmission_frame[i*2+2]-64) << 3) + ((transmission_frame[(i*2)+3] >> 5)))/8);
    }

    // Print Sensor Value
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("The Temperature Sensor 1 value is: " NRF_LOG_FLOAT_MARKER "", NRF_LOG_FLOAT(global_data_vector->temperature[0]));
      NRF_LOG_FLUSH();
      NRF_LOG_INFO("The Temperature Sensor 2 value is: " NRF_LOG_FLOAT_MARKER "", NRF_LOG_FLOAT(global_data_vector->temperature[1]));
      NRF_LOG_FLUSH();
      NRF_LOG_INFO("The Temperature Sensor 3 value is: " NRF_LOG_FLOAT_MARKER "", NRF_LOG_FLOAT(global_data_vector->temperature[2]));
      NRF_LOG_FLUSH();
      NRF_LOG_INFO("The Temperature Sensor 4 value is: " NRF_LOG_FLOAT_MARKER "", NRF_LOG_FLOAT(global_data_vector->temperature[3]));
      NRF_LOG_FLUSH();
      NRF_LOG_INFO("The Temperature Sensor 5 value is: " NRF_LOG_FLOAT_MARKER "", NRF_LOG_FLOAT(global_data_vector->temperature[4]));
      NRF_LOG_FLUSH();
      NRF_LOG_INFO("The Temperature Sensor 6 value is: " NRF_LOG_FLOAT_MARKER "", NRF_LOG_FLOAT(global_data_vector->temperature[5]));
      NRF_LOG_FLUSH();
      NRF_LOG_INFO("The Temperature Sensor 7 value is: " NRF_LOG_FLOAT_MARKER "", NRF_LOG_FLOAT(global_data_vector->temperature[6]));
      NRF_LOG_FLUSH();
    #endif
}





/* @author	Alexander Porta
 *
 * @laste dit   20.06.2019
 *              
 * @brief	Data_to_Vector copy the two uint8_t 
 *              frames into a uin16_t variable.               
 *
 * @param Data_to_Vector
 * 		
 *              -
 */
void data_to_vector(void){

  uint16_t temperature_raw_values = 0;


  //global_data_vector->device_name = transmission_frame[0]; 
  //global_data_vector->device_name = transmission_frame[1]; 

  // Calculate Temperature from adc values
  // Element [2] high byte, Element [1] low byte
  calc_temperature();             
                      
  // Humidity calculation out of raw values happens in
  // the humidity sensor
  for(uint8_t k = 0; k<7; k++){
    global_data_vector->humidity[k] = -0.1964*((transmission_frame[(k*2)+17] << 8) + transmission_frame[(k*2)+16])+700;
    #if(NRF_LOG_ON) 
      NRF_LOG_INFO("Hum Sensor in %: " NRF_LOG_FLOAT_MARKER "", NRF_LOG_FLOAT(global_data_vector->humidity[k]));
      NRF_LOG_FLUSH();
    #endif
  }
}


/* @author	Alexander Porta
 *
 * @laste dit   24.01.2020
 *              
 * @brief	
 *
 *
 * @param 
 * 		
 *              -
 */
void sensor_get_new_data(){

    bool trans_error = 0;
  
    // Send command to sensor for data receiving
    send_data_frame_element(GET_SENSOR_VALUES);
    transmission_error(trans_error);
    // Set flag for rssi receive
    receiver_flag->fresh_data = 1;

    // Wait for new data
    while(receiver_flag->fresh_data == 1);
}








/* @author	Alexander Porta
 *
 * @laste dit   24.01.2020
 *              
 * @brief	
 *
 *
 * @param 
 * 		
 *              -
 */
void sensor_tx_power_up(){

    bool trans_error = 0;
  
    // Send command to increase the transmission power
    send_data_frame_element(TRANS_PWR_UP);
    transmission_error(trans_error);
}


/* @author	Alexander Porta
 *
 * @laste dit   24.01.2020
 *              
 * @brief	
 *
 *
 * @param 
 * 		
 *              -
 */
void sensor_tx_power_down(){

    bool trans_error = 0;
  
    // Send command to decrease the transmission power
    send_data_frame_element(TRANS_PWR_DOWN);
    transmission_error(trans_error);
}



/* @author	Alexander Porta
 *
 * @laste dit   24.01.2020
 *              
 * @brief	
 *
 *
 * @param 
 * 		
 *              -
 */
void sensor_sleep_mode_enter(){
  
    bool trans_error = 0;

    // Send command to sensor for sleep mode enter
    send_data_frame_element(SLEEP_MODE_ENTER);
    transmission_error(trans_error);
}






/* @author	Alexander Porta
 *
 * @laste dit   24.01.2020
 *              
 * @brief	
 *
 *
 * @param 
 * 		
 *              -
 */
void sensor_reset(){

    bool trans_error = 0;
  
    // Send command to sensor for system reset
    send_data_frame_element(RESET_DEVICE);
    transmission_error(trans_error);
}


/* @author	Alexander Porta
 *
 * @laste dit   24.01.2020
 *              
 * @brief	
 *
 *
 * @param 
 * 		
 *              -
 */
void sensor_change_name(){

    bool trans_error = 0;
  
    // Send command to sensor for system reset
    trans_error = send_data_frame_element(CHANGE_NAME);
    transmission_error(trans_error);
    nrf_delay_ms(100);

    // Send sensor name number
    trans_error = send_data_frame_element(sens_name_t->sensor_number);
    transmission_error(trans_error);
}



/* @author	Alexander Porta
 *
 * @laste dit   22.01.2020
 *              
 * @brief	
 *
 *
 * @param 
 * 		
 *              -
 */
void sensor_clear_flash(){

    bool trans_error = 0;
  
    // Send command to sensor for system reset
    trans_error = send_data_frame_element(FLASH_CLEAR);
    transmission_error(trans_error);
    nrf_delay_ms(100);

    // Send sensor name number
    trans_error = send_data_frame_element(sens_name_t->sensor_number);
    transmission_error(trans_error);
}



/* @author	Alexander Porta
 *
 * @laste dit   25.01.2020
 *              
 * @brief	
 *
 *
 * @param 
 * 		
 *              -
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
  }
}








/* @author	Alexander Porta
 *
 * @laste dit   20.01.2020
 *              
 * @brief	check_connection_state()
 *              Returns the state of an
 *              connection with:
 *              connecte = true
 *              disconnect = false
 * @param 
 * 		
 *              -
 */
bool get_connection_state(){

  if (m_conn_handle != BLE_CONN_HANDLE_INVALID){
    return true;
  } else{
    return false;
  }
}