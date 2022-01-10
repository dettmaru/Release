/*********************************************************************
* DATA_TRANSMISSION.c includes all functions for transmission of analog
* sensor datas
*
* Technische Hochschule Koeln | RhizoTech 10.12.2019
*
* @ALPO
*
/*********************************************************************/
#include "DATA_TRANSMISSION.h"


data_vector tvector;                            // Data_Vector
ptr_data_vector global_data_vector = &tvector;  // 
int8_t global_rssi_value_buffer[4];             // Buffer for averaging
uint32_t transmission_frame[SIZE_OF_FRAME];     // Transmission Frame




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

  for(int i = 0; i<7; i++){
    global_data_vector->moisture[i] = 0;
    global_data_vector->temperature[i] = 0;
  }
  global_data_vector->ph1 = 0;
  global_data_vector->ph2 = 0;
  global_data_vector->battery_voltage = 0;
  global_data_vector->current_frame_element = 0;
  global_data_vector->adc_new_data = 0;
  global_data_vector->tx_power = flash_data->tx_power;
  global_data_vector->tx_power = 8;
  global_data_vector->sleep_time = SLEEP_TIME;
  global_data_vector->numSleep = 0;

  for(int i = 0; i<4; i++){
    global_rssi_value_buffer[i] = 0;
  }

  for(int i = 0; i<17; i++){
    global_data_vector->device_name[i] = 0;

  }

}






/* @author	ALPO
 *
 * @laste dit   29.12.2019
 *              
 * @brief	create_transmission_frame
 *              Maximum size of transmission frame is 8bit,
 *              12bit adc values are distributed to two frames
 *
 * @param create_transmission_frame
 * 		
 *              -
 */
void create_transmission_frame(){
  
  // Temperature
  for(uint8_t k = 0; k < 4; k++){
    transmission_frame[k] = (global_data_vector->temperature[k*4]     << 24)
                           +(global_data_vector->temperature[(k*4)+1] << 16)
                           +(global_data_vector->temperature[(k*4)+2] << 8)
                           +(global_data_vector->temperature[(k*4)+3]);
  }

  // Moisture
  for(uint8_t k = 0; k < 4; k++){
    transmission_frame[k+4] = (global_data_vector->moisture[k*2] << 16)
                            + global_data_vector->moisture[(k*2)+1];
  }

  transmission_frame[8] = global_data_vector->battery_voltage;

  transmission_frame[9] = (global_data_vector->ph1 << 16) + global_data_vector->ph2;

}





/* @author	ALPO
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
void transmit_frame(){
  
  uint8_t i = 0;
  bool send_okay = 0;
  uint8_t count = 0;

  for(i = 0; i<SIZE_OF_FRAME;i++){

    // Sending new frame Element
    while(send_data_frame(i) != send_okay){
      count++;
      if(count  == 10){
        NRF_LOG_INFO("Transmission Error!");
        NRF_LOG_INFO("Program Stopped");
        NRF_LOG_FLUSH();
        while(true);
      }
    }
    //Wait 10ms
    nrf_delay_ms(10);
  }

}



/* @author	ALPO
 *
 * @laste dit   24.01.2020
 *              
 * @brief	send_rssi_value
 *
 *
 * @param send_rssi_value
 * 		
 *              -
 */
void send_rssi_value(){

  // Send average
  send_data_frame(calc_rssi_value());
}






        