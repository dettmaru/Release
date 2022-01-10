/*********************************************************************
* POWER_MANAGEMENT.c includes all power management functions
*
* Technische Hochschule Koeln | RhizoTech 10.12.2019
*
* @ALPO
*
/*********************************************************************/
#include "POWER_MANAGEMENT.h"
#include "bsp_btn_ble.h"






/* @author	Nordic Semiconductor
 *
 * @edit by     ALPO
 *
 * @laste dit   01.01.2019
 *              
 * @brief	power_management_init
 * Function for initializing power management.
 *
 * @param power_management_init
 * 		
 *              -
 */
void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
    temp_sensor_active();
}




/* @author	Nordic Semiconductor
 *
 * @brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
void idle_state_handle(void)
{
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }
}




/* @author	ALPO
 *
 * @laste dit   21.12.2019
 *              
 * @brief	power_management_init
 * Function for initializing power management.
 *
 * @param power_management_init
 * 		
 *              -
 */
void system_on_sleep_mode_enter(void)
{
    // Close connection
    global_data_vector->sleep_time = SLEEP_TIME;
    close_connection(); 
    while(ctr_flags->device_disconnected == 0);


    // Stop Advertising
    advertising_stop();

    gpio_uninit();

    sleep_timer_start();

    // Go into system on sleep mode
    while(true){
      __WFE(); // Wait for Event
      __SEV();
      __WFE();

      //sd_app_evt_wait();
      //idle_state_handle();
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
      global_data_vector->tx_power = 0;
      limit = 0;
    }        
    break;
  }

  if(limit == 0){
    change_tx_power();
  }
  return limit;

}