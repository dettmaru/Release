/*********************************************************************
* PERIPHERY.c includes all initial functions for periphery elements
*
* Technische Hochschule Koeln | RhizoTech 25.12.2019
*
* @ALPO
*
/*********************************************************************/
#include "PERIPHERY.h"
#include "MAIN.h"
#include "nrf_drv_twis.h"


void stopwatch_timer_start(void);
void app_timer_err_check(ret_code_t err_code);
nrf_drv_spis_t spis = NRF_DRV_SPIS_INSTANCE(SPIS_INSTANCE);   // SPI Slave (SPIS) instance. 
nrf_drv_twis_t twis_instance = NRF_DRV_TWIS_INSTANCE(0);
uint8_t       m_rx_buf[40];                                   // RX buffer. 
uint8_t       m_tx_buf[40];
bool spi_transfer_finish;
uint8_t spi_buffer[16][40]; 




/* @author	ALPO
 *
 * @laste dit   24.01.2020
 *              
 * @brief	all_leds_off
 *              Turn off all LEDs
 */
void all_leds_off(void)
{
    nrf_gpio_pin_set(PWR_LED);
    nrf_gpio_pin_set(BLE_LED);
    nrf_gpio_pin_set(SPI_LED);
    nrf_gpio_pin_set(ERR_LED);
}




/* @author	ALPO
 *
 * @laste dit   24.01.2020
 *              
 * @brief	Interrupt callback, triggert by mainboard
 *              Transmit sensor values via SPI 
 */
static void gpio_event_callback(uint8_t pin_no, uint8_t button_action)
{
      uint32_t err_code;
      
      if ((pin_no == SPI_READY) && (button_action == APP_BUTTON_PUSH))
      {
        // LED Indicator on
        nrf_gpio_pin_clear(ERR_LED);
    
        #if(NRF_LOG_ON)
          NRF_LOG_INFO("Interrupt triggered by Mainboard");
          NRF_LOG_FLUSH();
        #endif

        // Send/Receive SPI Data
        spis_exchange_config_data();
        spis_transmit_sensor_data();

      }

      // LED off
      nrf_delay_ms(100);
      nrf_gpio_pin_set(ERR_LED);
}






/* @author	ALPO
 *
 * @laste dit   24.05.2019
 *              
 * @brief	Event initialization 
 *
 *              
 * @param       Init GPIO Pin Interrupt "gpio_event_callback"
 * 		
 *              -
 */
void gpio_event_init()
{
      uint32_t err_code;
      
      static app_button_cfg_t  button_cfg;

      button_cfg.pin_no         = SPI_READY;
      button_cfg.button_handler = gpio_event_callback;
      button_cfg.pull_cfg       = NRF_GPIO_PIN_PULLDOWN;
      button_cfg.active_state   = APP_BUTTON_ACTIVE_HIGH;
      
      err_code = app_button_init(&button_cfg, 1, 100);
      APP_ERROR_CHECK(err_code);
      
      err_code = app_button_enable();
      APP_ERROR_CHECK(err_code);

      /*Enable an app timer instance to detect long button press*/
      //err_code = app_timer_create(&m_button_action, APP_TIMER_MODE_SINGLE_SHOT, button_timeout_handler);
      //APP_ERROR_CHECK(err_code);
}




/* @author	ALPO
 *
 * @laste dit   24.05.2019
 *              
 * @brief	Function for initialising GPIOs.
 *
 *              
 * @param       gpio_init
 * 		
 *              -
 */
void gpio_init(void)
{
    // LED
    #if(NRF_LED_ON)
      nrf_gpio_cfg_output(PWR_LED);
      nrf_gpio_cfg_output(BLE_LED);
      nrf_gpio_cfg_output(SPI_LED);
      nrf_gpio_cfg_output(ERR_LED);

      nrf_gpio_pin_clear(PWR_LED);
      nrf_gpio_pin_set(BLE_LED);
      nrf_gpio_pin_set(SPI_LED);
      nrf_gpio_pin_set(ERR_LED);
    #endif

    nrf_gpio_cfg_output(DAT_READY);
    nrf_gpio_pin_clear(DAT_READY);

    nrf_gpio_cfg_input(SW0, GPIO_PIN_CNF_PULL_Disabled);
    nrf_gpio_cfg_input(SW1, GPIO_PIN_CNF_PULL_Disabled);
    nrf_gpio_cfg_input(SW2, GPIO_PIN_CNF_PULL_Disabled);
    nrf_gpio_cfg_input(SW3, GPIO_PIN_CNF_PULL_Disabled);
    nrf_gpio_cfg_input(SW4, GPIO_PIN_CNF_PULL_Disabled);
}



/* @author	ALPO
 *
 * @laste dit   07.01.2020
 *              
 * @brief	read_switch_state()
 *              Reading DIP Switch SW0-SW4
 *              and copy this bits to the
 *              switch state byte           
 */
uint8_t  read_switch_state(){

    // Create Byte for switch state
    uint8_t switch_state = 0b00000000;
  
    if(nrf_gpio_pin_read(SW0)){
      BIT_SET(switch_state,0);
      //BIT_CLEAR(switch_state,0);
    } else {
      BIT_CLEAR(switch_state,0);
    }

    if(nrf_gpio_pin_read(SW1)){
      BIT_SET(switch_state,1);
    } else {
      BIT_CLEAR(switch_state,1);
    }

    if(nrf_gpio_pin_read(SW2)){
      BIT_SET(switch_state,2);
    } else {
      BIT_CLEAR(switch_state,2);
    }

    if(nrf_gpio_pin_read(SW3)){
      BIT_SET(switch_state,3);
    } else {
      BIT_CLEAR(switch_state,3);
    }

    if(nrf_gpio_pin_read(SW4)){
      BIT_SET(switch_state,4);
    } else {
      BIT_CLEAR(switch_state,4);
    }

    return switch_state;
}



/* @author	ALPO
 *
 * @laste dit   20.01.2020
 *              
 * @brief	stopwatch_start
 *              Function for starting the stopwatch time
 *              
 * @param       timer_init
 * 		
 *              -
 */
void stopwatch_start(ptr_stopwatch_struct stopwatch_t){
  
  // Timer Start
  stopwatch_timer_start();

  if(stopwatch_t->tmr_started == 0){
    stopwatch_t->start_time = app_timer_cnt_get();
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Start Stopwatch!");
      NRF_LOG_FLUSH();
    #endif
    stopwatch_t->tmr_started = 1;
  } else {
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Stopwatch is still running!");
      NRF_LOG_FLUSH();
    #endif
  }
}


/* @author	ALPO
 *
 * @laste dit   20.01.2020
 *              
 * @brief	stopwatch_reset
 *              Clear stopwatch time to zero
 *              
 * @param       timer_init
 * 		
 *              -
 */
void stopwatch_reset(ptr_stopwatch_struct stopwatch_t){
  if(stopwatch_t->tmr_started == 1){
    stopwatch_t->start_time = app_timer_cnt_get();
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Reset Stopwatch!");
      NRF_LOG_FLUSH();
    #endif
    stopwatch_t->tmr_started = 1;
  } else {
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Reset not possible, stopwatch isn't running!");
      NRF_LOG_FLUSH();
    #endif
  }
}



/* @author	ALPO
 *
 * @laste dit   20.01.2020
 *              
 * @brief	stopwatch_get_time
 *              Returns the stopwatch time after starting
 *              in ms
 *              
 * @param       timer_init
 * 		
 *              -
 */
void stopwatch_get_time(ptr_stopwatch_struct stopwatch_t){

  if(stopwatch_t->tmr_started == 1){
    stopwatch_t->stop_time = app_timer_cnt_get();
    stopwatch_t->time_ms = ((stopwatch_t->stop_time - stopwatch_t->start_time)/32.768);

    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Calc Stopwatch Time: %d ms", stopwatch_t->time_ms);
      NRF_LOG_FLUSH();
    #endif
    stopwatch_t->tmr_started = 0;
  } else {
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Calculation of stopwatch time not possible, stopwatch isn't running!");
      NRF_LOG_FLUSH();
    #endif
  }

  stopwatch_timer_stop();

}



/* @author	Nordic Semiconductor
 *
 * @laste dit   
 *              
 * @brief	timer_init
 *              
 */
void timer_init(void)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}




/* @author	ALPO
 *
 * @laste dit   07.01.2020
 *              
 * @brief	sleep_timer_handler
 *              
 */
void sleep_timer_handler(void * p_context)
{
  /*#if(NRF_LOG_ON)
    NRF_LOG_INFO("Sleep Timer: WAKE UP!");
    NRF_LOG_FLUSH();
  #endif

  // System reset
  NVIC_SystemReset();*/

  #if(NRF_LOG_ON)
    NRF_LOG_INFO("Sleep Timer: WAKE UP FOR THE %d Time!", global_data_vector->numSleep + 1);
    NRF_LOG_FLUSH();
  #endif
  global_data_vector->numSleep = global_data_vector->numSleep + 1;
  if(global_data_vector->numSleep == 3){
    NVIC_SystemReset();
  }
  else{
    sleep_timer_start_ms(SENSOR_SLEEPTIME_MIN * 60 * 1000 - 7*1000);
  }

  // System reset
}



/* @author	ALPO
 *
 * @laste dit   07.01.2020
 *              
 * @brief	stopwatch_timer_handler
 *              
 */
void stopwatch_timer_handler(void * p_context)
{
  #if(NRF_LOG_ON)
    NRF_LOG_INFO("STOPWATCH_TIMER");
    NRF_LOG_FLUSH();
  #endif
}



/* @author	ALPO
 *
 * @laste dit   08.01.2020
 *              
 * @brief	conf_mode_timer_handler
 *              Time Handler for indicating the
 *              configuration mode with led toggle
 */
void conf_mode_timer_handler(void * p_context)
{
  #if(NRF_LOG_ON)
    //NRF_LOG_INFO("CONFMODE_TIMER");
    //NRF_LOG_FLUSH();
  #endif


  if(led->ERR == 1){
    nrf_gpio_pin_toggle(ERR_LED);
  } else {
    nrf_gpio_pin_set(ERR_LED);
  }

  if(led->BLE == 1){
    nrf_gpio_pin_toggle(BLE_LED);
  } else {
    nrf_gpio_pin_set(BLE_LED);
  }

  if(led->SPI == 1){
    nrf_gpio_pin_toggle(SPI_LED);
  } else {
    nrf_gpio_pin_set(SPI_LED);
  }

  if(led->PWR == 1){
    nrf_gpio_pin_toggle(PWR_LED);
  } else {
    nrf_gpio_pin_set(PWR_LED);
  }
}


/* @author	ALPO
 *
 * @laste dit   08.01.2020
 *              
 * @brief	error_timer_handler
 */
void error_timer_handler(void * p_context)
{
  nrf_gpio_pin_toggle(ERR_LED);
}



/* @author	Nordic Semiconductor
 *
 * @edit by     ALPO
 *
 * @laste dit   24.02.2020
 *              
 * @brief	Create timers.
 */
void create_timers()
{
    ret_code_t err_code;

    // Create timers
    err_code = app_timer_create(&m_repeated_sleep_timer,
                                APP_TIMER_MODE_REPEATED,
                                sleep_timer_handler);

    app_timer_err_check(err_code);


    err_code = app_timer_create(&m_repeated_stopwatch_timer,
                                APP_TIMER_MODE_REPEATED,
                                stopwatch_timer_handler);

    app_timer_err_check(err_code);


    err_code = app_timer_create(&m_repeated_conf_mode_timer,
                                APP_TIMER_MODE_REPEATED,
                                conf_mode_timer_handler);

    app_timer_err_check(err_code);


    err_code = app_timer_create(&m_repeated_error_timer,
                                APP_TIMER_MODE_REPEATED,
                                error_timer_handler);

    app_timer_err_check(err_code);


}




/* @author	ALPO
 *
 * @laste dit   07.01.2020
 *              
 * @brief	app_timer_err_check
 *              Log Printing if an error happened
 */
void app_timer_err_check(ret_code_t err_code){
  #if(NRF_LOG_ON)
    if(err_code == NRF_SUCCESS){
        NRF_LOG_INFO("timer: successful\n");
        NRF_LOG_FLUSH();
    } else if (err_code == NRF_ERROR_INVALID_PARAM){
        NRF_LOG_INFO("timer: parameter was invalid\n");
        NRF_LOG_FLUSH();
    } else if (err_code == NRF_ERROR_INVALID_STATE){
        NRF_LOG_INFO("timer: module has not been initialized or is running\n");
        NRF_LOG_FLUSH();
    }else if (err_code == NRF_ERROR_NO_MEM){
        NRF_LOG_INFO(" timer operations queue was full\n");
        NRF_LOG_FLUSH();
    }
  #endif
}





/* @author	ALPO
 *
 * @laste dit   02.01.2020
 *              
 * @brief	conf_mode_timer_start_500ms()
 *              
 */ 
void conf_mode_timer_start_500ms()
{

    ret_code_t err_code;

    // Timer erzeugt jede minute ein Interrupt
    err_code = app_timer_start(m_repeated_conf_mode_timer, APP_TIMER_TICKS(500), NULL);


    // Check for errors
    app_timer_err_check(err_code);
}




/* @author	ALPO
 *
 * @laste dit   02.01.2020
 *              
 * @brief	error_timer_start
 *              
 */ 
void error_timer_start()
{

    ret_code_t err_code;

    // Timer erzeugt jede minute ein Interrupt
    err_code = app_timer_start(m_repeated_error_timer, APP_TIMER_TICKS(50), NULL);


    // Check for errors
    app_timer_err_check(err_code);
}



/* @author	ALPO
 *
 * @laste dit   20.01.2020
 *              
 * @brief	stopwatch_timer_start()
 *              
 */ 
void stopwatch_timer_start()
{

    ret_code_t err_code;

    // Interrupt every 500ms
    err_code = app_timer_start(m_repeated_stopwatch_timer, APP_TIMER_TICKS(500), NULL);


    // Check for errors
    app_timer_err_check(err_code);
}



/* @author	ALPO
 *
 * @laste dit   02.01.2020
 *              
 * @brief	sleep_timer_start_xms()
 *              
 */ 
void sleep_timer_start_xms()
{

    ret_code_t err_code;

    if((global_data_vector->sleep_time >= 1) && (global_data_vector->sleep_time <= 240)){
      
      // Timer generate an interrupt every time that is set in "sleep_time"
      err_code = app_timer_start(m_repeated_stopwatch_timer, APP_TIMER_TICKS(global_data_vector->sleep_time*60*1000), NULL);
    } else {
      err_code = app_timer_start(m_repeated_stopwatch_timer, APP_TIMER_TICKS(60000), NULL);
    }


    // Check for errors
    app_timer_err_check(err_code);
}




/* @author	ALPO
 *
 * @laste dit   02.01.2020
 *              
 * @brief	sleep_timer_start_1m()
 *              
 */ 
void sleep_timer_start_1m()
{

    ret_code_t err_code;

    // Timer erzeugt jede minute ein Interrupt
    err_code = app_timer_start(m_repeated_sleep_timer, APP_TIMER_TICKS(60000), NULL);


    // Check for errors
    app_timer_err_check(err_code);
}




/* @author	ALPO
 *
 * @laste dit   02.01.2020
 *              
 * @brief	sleep_timer_start_ms()
 *              Timer with variable time setting
 *              
 */ 
void sleep_timer_start_ms(uint32_t *milliseconds)
{

    ret_code_t err_code;

    // Timer erzeugt alle 5min ein Interrupt
    err_code = app_timer_start(m_repeated_sleep_timer, APP_TIMER_TICKS(*milliseconds), NULL);

    // Check for errors
    app_timer_err_check(err_code);
}






/* @author	ALPO
 *
 * @laste dit   02.01.2020
 *              
 * @brief	sleep_timer_stop()
 *              
 */ 
void sleep_timer_stop()
{
    ret_code_t err_code;
    // Deaktiviere Timer
    err_code = app_timer_stop(m_repeated_sleep_timer);
    // Check for errors
    app_timer_err_check(err_code);
}



/* @author	ALPO
 *
 * @laste dit   02.01.2020
 *              
 * @brief	conf_mode_timer_stop()
 *              
 */ 
void conf_mode_timer_stop()
{
    ret_code_t err_code;
    // Deaktiviere Timer
    err_code = app_timer_stop(m_repeated_conf_mode_timer);
    // Check for errors
    app_timer_err_check(err_code);
}




/* @author	ALPO
 *
 * @laste dit   02.01.2020
 *              
 * @brief	stopwatch_timer_stop()
 *              
 */ 
void stopwatch_timer_stop()
{
    ret_code_t err_code;
    // Deaktiviere Timer
    err_code = app_timer_stop(m_repeated_stopwatch_timer);
    // Check for errors
    app_timer_err_check(err_code);
}



/* @author	ALPO
 *
 * @laste dit   02.01.2020
 *              
 * @brief	timer_stop_all()
 *              
 */ 
void timer_stop_all()
{
    ret_code_t err_code;
    // Deaktiviere Timer
    err_code = app_timer_stop_all();
    // Check for errors
    app_timer_err_check(err_code);
}




/* @author	ALPO
 *
 * @laste dit   12.12.2019
 *              
 * @brief	spis_event_handler
 *              SPIS user event handler.
 *
 *              
*/
void spis_event_handler(nrf_drv_spis_event_t event)
{
    if (event.evt_type == NRF_DRV_SPIS_XFER_DONE)
    {
        spis_xfer_done = true;
        #if(NRF_LOG_ON)
          NRF_LOG_INFO("SPIS: Transfer completed.");
          NRF_LOG_FLUSH();
        #endif

    } else if(event.evt_type == NRF_DRV_SPIS_BUFFERS_SET_DONE){
      #if(NRF_LOG_ON)
        NRF_LOG_INFO("SPIS: Memory buffer set event.");
        NRF_LOG_FLUSH();
      #endif
      
      spi_transfer_finish = 1;
    } else if(event.evt_type == NRF_DRV_SPIS_EVT_TYPE_MAX ){
      #if(NRF_LOG_ON)
        NRF_LOG_INFO("SPIS: Aufzaehlungs-Obergrenze.");
        NRF_LOG_FLUSH();
      #endif

    }



}





/* @author	ALPO
 *
 * @laste dit   07.01.2020
 *              
 * @brief	spis_init()
 *              SPI Slave initializing
 *
 */
 void spis_init(){

    // Enable the constant latency sub power mode to minimize the time it takes
    // for the SPIS peripheral to become active after the Slave Select line is asserted
    // (when the CPU is in sleep mode).
    NRF_POWER->TASKS_CONSTLAT = 1;

    // Log
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("SPIS Init");
      NRF_LOG_FLUSH();
    #endif

    // Pin Configuration
    nrf_drv_spis_config_t spis_config = NRF_DRV_SPIS_DEFAULT_CONFIG;
    spis_config.csn_pin               = SPI_SS;
    spis_config.miso_pin              = SPI_MISO;
    spis_config.mosi_pin              = SPI_MOSI;
    spis_config.sck_pin               = SPI_SCK;

    spis_config.mode                  = NRF_SPIS_MODE_0;                       
    spis_config.bit_order             = NRF_SPIS_BIT_ORDER_MSB_FIRST;         
    spis_config.csn_pullup            = NRFX_SPIS_DEFAULT_CSN_PULLUP;          
    spis_config.miso_drive            = NRFX_SPIS_DEFAULT_MISO_DRIVE;          
    spis_config.def                   = NRFX_SPIS_DEFAULT_DEF;                 
    spis_config.orc                   = NRFX_SPIS_DEFAULT_ORC;                
    spis_config.irq_priority          = NRFX_SPIS_DEFAULT_CONFIG_IRQ_PRIORITY;




    // Initialisation SPI Slave
    APP_ERROR_CHECK(nrf_drv_spis_init(&spis, &spis_config, spis_event_handler));

 }






/* @author	ALPO
 *
 * @laste dit   24.12.2019
 *              
 * @brief	spis_transmit_sensor_data()
 *
 */
void spis_transmit_sensor_data(){

  uint8_t m_length = sizeof(m_rx_buf);
  ret_code_t err_code;
  uint8_t spi_timeout = 0; // Counts transmission time in 100ms steps

    

  // Set Buffer
  NRF_LOG_INFO("NUMBER_OF_SENSORS %x", NUMBER_OF_SENSORS);
  NRF_LOG_FLUSH();

  
  for(int k = 0; k <= NUMBER_OF_SENSORS; k++){
    spis_xfer_done = false;
    memset(m_rx_buf, 0, m_length);
    NRF_LOG_INFO("k = %x", k);
    NRF_LOG_FLUSH();

    err_code = nrf_drv_spis_buffers_set(&spis, spi_buffer[k], 40, m_rx_buf, 40);
    spi_timeout = 0;
    while((!spis_xfer_done) && (spi_timeout < 10)){
      nrf_delay_ms(300);
      spi_timeout++;
      if(spi_timeout == 10){
        NRF_LOG_INFO("SPIS: Transmission abort, timeout!");
        NRF_LOG_FLUSH();
      }
    }
  }
}






/* @author	ALPO
 *
 * @laste dit   16.06.2020
 *              
 * @brief	spis_exchange_config_data
 *
 */
void spis_exchange_config_data(){

    uint8_t m_length = sizeof(global_data_vector->sleep_time);
    ret_code_t err_code;
    uint8_t spi_timeout = 0;
    nrf_gpio_pin_toggle(SPI_LED);


    // Clear Buffer and flag
    memset(&global_data_vector->sleep_time, 0, m_length);
    spis_xfer_done = false;
    uint8_t sleep_time = global_data_vector->sleep_time;


    // Set Buffer
    flash_data->sensor_number = NUMBER_OF_SENSORS-1;
    err_code = nrf_drv_spis_buffers_set(&spis, &flash_data->sensor_number, 1, &sleep_time, 1);
    spi_err_check(err_code);
    global_data_vector->sleep_time = sleep_time;
  
    while ((!spis_xfer_done) && (spi_timeout < 5))
    {
      spi_timeout++;
      nrf_delay_ms(300);
      NRF_LOG_INFO("SPIS Config: WAIT");
      NRF_LOG_FLUSH();
    }

    if(spi_timeout == 5){
      NRF_LOG_INFO("SPIS Config: TIMEOUT!!");
      NRF_LOG_FLUSH();
    } else{
      NRF_LOG_INFO("SPIS Config:: Sleep Time from MB received!: %x", sleep_time);
      NRF_LOG_INFO("SPIS Config:: Sensor Number to MB transmitted!: %x", flash_data->sensor_number);
      NRF_LOG_FLUSH();
    }


    nrf_gpio_pin_toggle(SPI_LED);

 }



/* @author	ALPO
 *
 * @laste dit   24.12.2019
 *              
 * @brief	spi_err_check
 *
 */
bool spi_err_check(ret_code_t err_check){

  #if(NRF_LOG_ON)
    switch(err_check){
      case NRF_SUCCESS:
        NRF_LOG_INFO("SPIS: The operation was successful.");
        NRF_LOG_FLUSH();
        return 1;
      break;

      case NRF_ERROR_NULL:
        NRF_LOG_INFO("SPIS: The operation failed because a NULL pointer was supplied.");
        NRF_LOG_FLUSH();
        return 0;
      break;

      case NRF_ERROR_INVALID_STATE:
        NRF_LOG_INFO("SPIS: The operation failed because the SPI slave device is in an incorrect state.");
        NRF_LOG_FLUSH();
        return 0;
      break;

      case NRF_ERROR_INVALID_ADDR:
        NRF_LOG_INFO("SPIS: The provided buffers are not placed in the Data RAM region.");
        NRF_LOG_FLUSH();
        return 0;
      break;

      case NRF_ERROR_INTERNAL:
        NRF_LOG_INFO("SPIS: The operation failed because of an internal error.");
        NRF_LOG_FLUSH();
        return 0;
      break;
    }
  #endif
}


/* @author	ALPO
 *
 * @laste dit   23.01.2020
 *              
 * @brief	spi_buffer_init            
 *
 */
void spi_buffer_init(void){

  for(int i = 0; i < (sens_name_t->sensor_number + 1); i++){
    for(int ii = 0; ii < (SIZE_OF_SPI_FRAME); ii++){  
        spi_buffer[i][ii] = ii;
    }
  }
}







