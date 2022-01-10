/*********************************************************************
* POWER_MANAGEMENT.c includes all power management functions
*
* Technische Hochschule Koeln | RhizoTech 25.12.2019
*
* @ALPO
*
/*********************************************************************/
#include "POWER_MANAGEMENT.h"
#include "bsp_btn_ble.h"




/* @author	Nordic Semiconductor
 *
 *              
 * @brief	Function for initializing the Power manager.
 *              
 */
void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}

 
 


/* @author	ALPO
 *
 * @laste dit   21.12.2019
 *              
 * @brief	idle_state_handle
 *
 */
void idle_state_handle(void)
{
    nrf_pwr_mgmt_run();
}





/* @author	ALPO
 *
 * @laste dit   21.12.2019
 *              
 * @brief	system_on_sleep_mode_enter
 *
 */
void system_on_sleep_mode_enter(void)
{
    // Close connection
    all_leds_off();

    if(get_connection_state() == 1){
      close_connection();
    }

    nrf_delay_ms(1000);

    // Stop Scan
    scan_stop();

    // Go into system on sleep mode
    while(true){
      sd_app_evt_wait();
      idle_state_handle();
    }
}





/* @author	ALPO
 *
 * @laste dit   22.01.2020
 *              
 * @brief	sleep_2nd_call()
 *              First function call:
 *              Start stopwatch timer and save starttime
 *              Second function call:
 *              calculate the stopwatchtime and 
 *              subtract this from sleep time
 */
void  sleep_2nd_call(){


  static stopwatch_struct    stopwatch_t;
  static bool first_call = 1;
  uint32_t  sleep_time_ms;



  // Default sleep time every minute
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Sleeptime: %d!", SENSOR_SLEEPTIME_MIN);
    #endif

    // Sleeptime Calculation in ms + 5s safety time
    //sleep_time_ms = (SENSOR_SLEEPTIME_MIN * 60 * 1000) + (5 * 1000);
    sleep_time_ms = (SENSOR_SLEEPTIME_MIN * 60 * 1000 - 7*1000);
  

  if(first_call == 1){
    first_call = 0;
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("first call!");
      NRF_LOG_FLUSH();
    #endif
    stopwatch_start(&stopwatch_t);
  } else {
    // Start sleep timer
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("2nd call!");
      NRF_LOG_INFO("Go to sleep!");
    #endif
    stopwatch_get_time(&stopwatch_t);
    //sleep_time_ms -= stopwatch_t.time_ms;
    NRF_LOG_INFO("Stoptime: %d!", stopwatch_t.time_ms);
    NRF_LOG_INFO("Sleeptime: %d!", sleep_time_ms);
    NRF_LOG_FLUSH();
    sleep_timer_start_ms(&sleep_time_ms);
    system_on_sleep_mode_enter();
  }
}
      

/* @author	ALPO
*
* @laste dit   23.12.2019
*              
* @brief       tx_power_up()
*              Increase transmission power
*              The transmission power can onle assume
*              the values specified below
*/
bool tx_power_up(){
  
  bool limit = 0;

  switch (global_data_vector->tx_power){
    
    case -40:
    {
      global_data_vector->tx_power = -20;
      limit = 0;
    }
    break;

    case -20:
    {
      global_data_vector->tx_power = -16;
      limit = 0;
    }
    break;

    case -16:
    {
      global_data_vector->tx_power = -12;
      limit = 0;
    }
    break;

    case -12:
    {
      global_data_vector->tx_power = -8;
      limit = 0;
    }
    break;

    case -8:
    {
      global_data_vector->tx_power = -4;
      limit = 0;
    }
    break;

    case -4:
    {
      global_data_vector->tx_power = 0;
      limit = 0;
    }
    break;

    case 0:
    {
      global_data_vector->tx_power = 2;
      limit = 0;
    }
    break;

    case 2:
    {
      global_data_vector->tx_power = 3;
      limit = 0;
    }
    break;

    case 3:
    {
      global_data_vector->tx_power = 4;
      limit = 0;
    }
    break;

    case 4:
    {
      global_data_vector->tx_power = 5;
      limit = 0;
    }
    break;

    case 5:
    {
      global_data_vector->tx_power = 6;
      limit = 0;
    }
    break;

    case 6:
    {
      global_data_vector->tx_power = 7;
      limit = 0;
    }
    break;

    case 7:
    {
      global_data_vector->tx_power = 8;
      limit = 0;
    }
    break;

    case 8:   
    {
      global_data_vector->tx_power = 8;
      limit = 1;      
    }        
    break;

    default:
    {
    }      
    break;
  }

  // Change transmission power
  if(limit == 0){
    //Change Transmission Power in active connection
    change_tx_power();
  }
  return limit;
}

 
 
/* @author	ALPO
*
* @laste dit    23.12.2019
*              
* @brief        Decrease the transmission power
*               The transmission power can onle assume
*               the values specified below
*
* @param 
* 		
*              -
*/
bool tx_power_down(){
  
  bool limit = 0;

  switch (global_data_vector->tx_power){
    
    case -40:
    {
      global_data_vector->tx_power = -40;
      limit = 1;
    }
    break;

    case -20:
    {
      global_data_vector->tx_power = -40;
      limit = 0;
    }
    break;

    case -16:
    {
      global_data_vector->tx_power = -20;
      limit = 0;
    }
    break;

    case -12:
    {
      global_data_vector->tx_power = -16;
      limit = 0;
    }
    break;

    case -8:
    {
      global_data_vector->tx_power = -12;
      limit = 0;
    }
    break;

    case -4:
    {
      global_data_vector->tx_power = -8;
      limit = 0;
    }
    break;

    case 0:
    {
      global_data_vector->tx_power = -4;
      limit = 0;
    }
    break;

    case 2:
    {
      global_data_vector->tx_power = 0;
      limit = 0;
    }
    break;

    case 3:
    {
      global_data_vector->tx_power = 2;
      limit = 0;
    }
    break;

    case 4:
    {
      global_data_vector->tx_power = 3;
      limit = 0;
    }
    break;

    case 5:
    {
      global_data_vector->tx_power = 4;
      limit = 0;
    }
    break;

    case 6:
    {
      global_data_vector->tx_power = 5;
      limit = 0;
    }
    break;

    case 7:
    {
      global_data_vector->tx_power = 6;
      limit = 0;
    }
    break;

    case 8:
    {           
      global_data_vector->tx_power = 7;
      limit = 0;
    }           
    break;

    default:
    {
    }        
    break;
  }

  if(limit == 0){
    change_tx_power();
  }
  return limit;

}