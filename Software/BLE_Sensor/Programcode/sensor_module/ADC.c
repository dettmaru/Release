/*********************************************************************
* ADC.c includes all analog digital converter functions
*
* Technische Hochschule Koeln | RhizoTech 10.12.2019
*
* @ALPO
*
/*********************************************************************/
#include "ADC.h"




// Defines
uint8_t state = 1;
int32_t ADC_VAL0 = 0;
// One Channel Buffer for ADC Values
nrf_saadc_value_t           m_buffer_pool[SAMPLES_IN_BUFFER];
nrf_ppi_channel_t           m_ppi_channel;
nrf_drv_timer_t m_timer =   NRF_DRV_TIMER_INSTANCE(2);
int16_t                     saadc_offset = 8;
uint8_t                     moisture_sensor_number = 0;




/* @author	ALPO
 *
 * @laste dit   09.01.2019
 *              
 * @brief	saadc_read_values
 *              Start ADC reading by enabling sampling
 *              event. After all sensors have been read,
 *              the sampling event the event is deactivated.
 * @param       
 *              
 */
void saadc_read_values(){

    ctr_flags->adc_reading_finish = 0;

    // Reset Moisture Sensor
    moisture_sensor_number = 0;
    moisture_sensor_select(&moisture_sensor_number);

    // Enable ADC event
    saadc_sampling_event_enable();

    
    while(moisture_sensor_number < 7);

    // Disable ADC Event
    saadc_sampling_event_disable();

    // Downgrade (from 7 down to 2 sensors)
    // New Moisture Sensors are connected to pH1 and pH2 Analog Input Pins
    // 
    global_data_vector->moisture[0] = global_data_vector->ph2;
    global_data_vector->moisture[1] = global_data_vector->ph1;


    // Printing Values
    NRF_LOG_INFO("ADC UBAT Value is: 0x%x", global_data_vector->battery_voltage);
    NRF_LOG_INFO("Moisture 1 Value is: 0x%x", global_data_vector->moisture[0]);
    NRF_LOG_INFO("Moisture 2 Value is: 0x%x", global_data_vector->moisture[1]);
    NRF_LOG_FLUSH();

    ctr_flags->adc_reading_finish = 1;

}








/* @author	ALPO
 *
 * @laste dit   26.12.2019
 *              
 * @brief	moisture_sensor_select
 *
 *              The single ones of moisture sensors are selected
 *              by a multiplexer, that is controlled by
 *              MPXA, B and C.
 *
 *              Sensor 1 is the highest, sensor 6 the deepest sensor
 *              Sensor 7 does not exist but this pin is connected to
 *              ground and is used for the saadc zero calibration.
 *
 * @param       uint8_t *Sensor_Number - Number of selected moisture
 * 		sensor
 *              
 */
void moisture_sensor_select(uint8_t *sensor_number){

    // Control Multiplexer by setting gpio pins
    switch(*sensor_number){

        case 0:
        nrf_gpio_pin_clear(MPX_A);
        nrf_gpio_pin_set(MPX_B);
        nrf_gpio_pin_clear(MPX_C);
        break;
    
        case 1:
        nrf_gpio_pin_set(MPX_A);
        nrf_gpio_pin_clear(MPX_B);
        nrf_gpio_pin_clear(MPX_C);
        break;

        case 2:
        nrf_gpio_pin_clear(MPX_A);
        nrf_gpio_pin_clear(MPX_B);
        nrf_gpio_pin_clear(MPX_C);
        break;

        case 3:
        nrf_gpio_pin_set(MPX_A);
        nrf_gpio_pin_set(MPX_B);
        nrf_gpio_pin_clear(MPX_C);
        break;

        case 4:
        nrf_gpio_pin_set(MPX_A);
        nrf_gpio_pin_clear(MPX_B);
        nrf_gpio_pin_set(MPX_C);
        break;

        case 5:
        nrf_gpio_pin_clear(MPX_A);
        nrf_gpio_pin_set(MPX_B);
        nrf_gpio_pin_set(MPX_C);
        break;

        case 6:
        nrf_gpio_pin_clear(MPX_A);
        nrf_gpio_pin_clear(MPX_B);
        nrf_gpio_pin_set(MPX_C);
        break;

        // Default Pin is 7, that pin is connected
        // to Ground
        default:
        nrf_gpio_pin_set(MPX_A);
        nrf_gpio_pin_set(MPX_B);
        nrf_gpio_pin_set(MPX_C);
        break;
    }
}





/* @author	ALPO
 *
 * @laste dit   20.12.2019
 *              
 * @brief	averaging_adc_values
 *
 *              Averaging ADC values
 *              The analog digital converter is reading on
 *              4 channel and store this data on every 4th place
 *              in the buffer.
 *
 * @param averaging_adc_values	
 * 		
 *              
 */
void averaging_adc_values(){

  int32_t ph1 = 0;
  int32_t ph2 = 0;
  int32_t ubat = 0;
  int32_t moist = 0;


  // Calculate the sum of all smaples
  for(int i = 0; i<(SAMPLES_IN_BUFFER/4); i++){
        ph1 += m_buffer_pool[i*4];
        ph2 += m_buffer_pool[(i*4)+1];
        ubat += m_buffer_pool[(i*4)+2];
        moist += m_buffer_pool[(i*4)+3];
  }
  
  global_data_vector->ph1 = (ph1/(SAMPLES_IN_BUFFER/4))+saadc_offset;
  global_data_vector->ph2 = (ph2/(SAMPLES_IN_BUFFER/4))+saadc_offset;
  global_data_vector->battery_voltage = 2*(ubat/(SAMPLES_IN_BUFFER/4))+saadc_offset;
  global_data_vector->moisture[moisture_sensor_number] = (moist/(SAMPLES_IN_BUFFER/4))+saadc_offset;
}







/* @author	Nordic Semiconductor
 *
 * @laste dit   01.05.2019
 *              
 * @brief	timer_handler
 *
 * @param timer_handler
 * 		
 *              * p_event -> 
 *
 *
 */
void timer_handler(nrf_timer_event_t event_type, void * p_context)
{

}








/* @author	ALPO
 *
 * @laste dit   24.12.2019
 *              
 * @brief	saadc_sampling_event_enable
 *              Enables the adc sampling event
 *
 * @param 
 * 		
 *              -
 */
void saadc_sampling_event_enable(void)
{
    ret_code_t err_code;

    err_code = nrf_drv_ppi_channel_enable(m_ppi_channel);

    APP_ERROR_CHECK(err_code);

}


/* @author	ALPO
 *
 * @laste dit   24.12.2019
 *              
 * @brief	saadc_sampling_event_disable
 *              Disables the adc sampling event
 *
 * @param 
 * 		
 *              -
 */
void saadc_sampling_event_disable(void)
{
    ret_code_t err_code;

    err_code = nrf_drv_ppi_channel_disable(m_ppi_channel);

    APP_ERROR_CHECK(err_code);

}





/* @author	Nordic Semiconductor
 *
 * @edit by     ALPO
 *
 * @laste dit   20.12.2019
 *              
 * @brief	saadc_sampling_event_init 
 *
 *              Generate a sampling event with timer instance 4
 *              After every 1ms, a timer interrupt starts a
 *              ADC Sampling event on all channels.
 */
void saadc_sampling_event_init(void)
{
    ret_code_t err_code;

    err_code = nrf_drv_ppi_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32;
    err_code = nrf_drv_timer_init(&m_timer, &timer_cfg, timer_handler);
    APP_ERROR_CHECK(err_code);

    /* setup m_timer for compare event every 1ms */
    uint32_t ticks = nrf_drv_timer_ms_to_ticks(&m_timer, 1);
    nrf_drv_timer_extended_compare(&m_timer,
                                   NRF_TIMER_CC_CHANNEL0,
                                   ticks,
                                   NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK,
                                   false);
    nrf_drv_timer_enable(&m_timer);

    uint32_t timer_compare_event_addr = nrf_drv_timer_compare_event_address_get(&m_timer,
                                                                                NRF_TIMER_CC_CHANNEL0);
    uint32_t saadc_sample_task_addr   = nrf_drv_saadc_sample_task_get();

    /* setup ppi channel so that timer compare event is triggering sample task in SAADC */
    err_code = nrf_drv_ppi_channel_alloc(&m_ppi_channel);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_ppi_channel_assign(m_ppi_channel,
                                          timer_compare_event_addr,
                                          saadc_sample_task_addr);
    APP_ERROR_CHECK(err_code);
}


/* @author	ALPO
 *
 * @laste dit   24.12.2019
 *              
 * @brief	adc_timer_disable
 *              Disable timer instance 2
 *
 * @param
 * 		
 *              -
 */
void adc_timer_disable(){
    nrf_drv_timer_uninit(&m_timer);
}


/* @author	Nordic Semiconductor
 *
 * @edit by     ALPO
 *
 * @laste dit   01.05.2019
 *              
 * @brief	 
 *
 *              Initialisation ADC for reading sensor value
 *              ADC Input is AIN5
 *
 *             // reference voltage configuration for adc
 *             // reference Voltage for Input is VDD / 4
 *             channel_config.reference = NRF_SAADC_REFERENCE_VDD4;

 *             // Input Voltage multiplied with 1/GAIN
 *             // (3.3/4)/(1/4) = 3.3V max
 *             channel_config.gain = NRF_SAADC_GAIN1_4;       
 *             channel_config.acq_time = NRF_SAADC_ACQTIME_10US;
    
 *             //Set SAADC as single ended. 
 *             //This means it will only have the positive pin as input, 
 *             //and the negative pin is shorted to ground (0V) internally.         
 *             channel_config.mode = NRF_SAADC_MODE_SINGLE_ENDED;  
          
 *             //Disable pullup resistor on the input pin
 *             channel_config.resistor_p = NRF_SAADC_RESISTOR_DISABLED;     
 *             channel_config.resistor_n = NRF_SAADC_RESISTOR_DISABLED;
 *
 * @param saadc_init
 * 		
 *              -
 */
void saadc_init(void)
{

    ret_code_t err_code;

   // Channel default configuration
   nrf_saadc_channel_config_t channel_config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(AIN_PH1);

    channel_config.reference = NRF_SAADC_REFERENCE_VDD4; 
    //channel_config.reference = NRF_SAADC_REFERENCE_INTERNAL;
    channel_config.gain = NRF_SAADC_GAIN1_4;       
    channel_config.acq_time = NRF_SAADC_ACQTIME_10US;        
    channel_config.mode = NRF_SAADC_MODE_SINGLE_ENDED;            
    channel_config.resistor_p = NRF_SAADC_RESISTOR_DISABLED;     
    channel_config.resistor_n = NRF_SAADC_RESISTOR_DISABLED;


   // SAADC default configuration
   nrfx_saadc_config_t saadc_config;

   saadc_config.resolution = NRF_SAADC_RESOLUTION_12BIT; 
   saadc_config.oversample = NRF_SAADC_OVERSAMPLE_16X; 
   saadc_config.interrupt_priority = NRFX_SAADC_CONFIG_IRQ_PRIORITY;
   saadc_config.low_power_mode = true;



    // Init function for SA-ADC
    err_code = nrf_drv_saadc_init(NULL, saadc_timer_callback);

    // Print Log
    #if(NRF_ADC_LOG_ON)
      if(err_code == NRF_SUCCESS){
          NRF_LOG_INFO("adc_init: successful\n");
          NRF_LOG_FLUSH();

      } else if(err_code == NRF_ERROR_INVALID_STATE){
          NRF_LOG_INFO("adc_init: driver is already initialized\n");
          NRF_LOG_FLUSH();

      } else if(err_code == NRF_ERROR_INVALID_PARAM	){
          NRF_LOG_INFO("adc_init: event handler is NULL\n");
          NRF_LOG_FLUSH();
      }
      APP_ERROR_CHECK(err_code);
    #endif


    // Init Channel 0 with Analog-Input on PH2
    err_code = nrf_drv_saadc_channel_init(0, &channel_config);
    
    // Print Log
    #if(NRF_ADC_LOG_ON)
      if(err_code == NRF_SUCCESS){
          NRF_LOG_INFO("adc_channel_init_PH2: successful\n");
          NRF_LOG_FLUSH();

      } else if(err_code == NRF_ERROR_INVALID_STATE){
          NRF_LOG_INFO("adc_channel_init_PH2: adc is not initialized\n");
          NRF_LOG_FLUSH();

      } else if(err_code == NRFX_ERROR_NO_MEM){
          NRF_LOG_INFO("adc_channel_init_PH2: specified channel is already allocated\n");
          NRF_LOG_FLUSH();
      }
      APP_ERROR_CHECK(err_code);
    #endif


    // Init Channel 1 with Analog-Input on PH1
    channel_config.pin_p = AIN_PH2;
    err_code = nrf_drv_saadc_channel_init(1, &channel_config);
    
    // Print Log
    #if(NRF_ADC_LOG_ON)
      if(err_code == NRF_SUCCESS){
          NRF_LOG_INFO("adc_channel_init_PH1: successful\n");
          NRF_LOG_FLUSH();

      } else if(err_code == NRF_ERROR_INVALID_STATE){
          NRF_LOG_INFO("adc_channel_init_PH1: adc is not initialized\n");
          NRF_LOG_FLUSH();

      } else if(err_code == NRFX_ERROR_NO_MEM){
          NRF_LOG_INFO("adc_channel_init_PH1: specified channel is already allocated\n");
          NRF_LOG_FLUSH();
      }
      APP_ERROR_CHECK(err_code);
    #endif


    // Init Channel 2 with Analog-Input on UBAT
    channel_config.pin_p = AIN_U_BAT;
    err_code = nrf_drv_saadc_channel_init(4, &channel_config);
    
    // Print Log
    #if(NRF_ADC_LOG_ON)
      if(err_code == NRF_SUCCESS){
          NRF_LOG_INFO("adc_channel_init_UBAT: successful\n");
          NRF_LOG_FLUSH();

      } else if(err_code == NRF_ERROR_INVALID_STATE){
          NRF_LOG_INFO("adc_channel_init_UBAT: adc is not initialized\n");
          NRF_LOG_FLUSH();

      } else if(err_code == NRFX_ERROR_NO_MEM){
          NRF_LOG_INFO("adc_channel_init_UBAT: specified channel is already allocated\n");
          NRF_LOG_FLUSH();
      }
      APP_ERROR_CHECK(err_code);
    #endif

    // Init Channel 3 with Analog-Input on Moisture Sensor
    channel_config.pin_p = AIN_MO;
    err_code = nrf_drv_saadc_channel_init(5, &channel_config);
    
    // Print Log
    #if(NRF_ADC_LOG_ON)
      if(err_code == NRF_SUCCESS){
          NRF_LOG_INFO("adc_channel_init_MO: successful\n");
          NRF_LOG_FLUSH();

      } else if(err_code == NRF_ERROR_INVALID_STATE){
          NRF_LOG_INFO("adc_channel_init_MO: adc is not initialized\n");
          NRF_LOG_FLUSH();

      } else if(err_code == NRFX_ERROR_NO_MEM){
          NRF_LOG_INFO("adc_channel_init_MO: specified channel is already allocated\n");
          NRF_LOG_FLUSH();
      }
      APP_ERROR_CHECK(err_code);
    #endif


    // Buffer for analog values
    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool, SAMPLES_IN_BUFFER);

    // Print Log
    #if(NRF_ADC_LOG_ON)
      if(err_code == NRF_SUCCESS){
          NRF_LOG_INFO("adc_buf_conv: successful\n");
          NRF_LOG_FLUSH();

      } else if(err_code == NRF_ERROR_BUSY){
          NRF_LOG_INFO("adc_buf_conv: driver has already two buffer set (busy)\n");
          NRF_LOG_FLUSH();

      }
      APP_ERROR_CHECK(err_code);
    #endif

} 


/* @author	ALPO
 *
 * @laste dit   24.12.2019
 *              
 * @brief	saadc_start
 *              Initialisation of the adc instance
 *
 * @param 
 * 		
 *              -
 */
void saadc_start()
{
    saadc_sampling_event_init();
    saadc_init();
    saadc_sampling_event_enable();
}


/* @author	ALPO
 *
 * @laste dit   24.12.2019
 *              
 * @brief	saadc_stop
 *  
 *
 * @param saadc_stop
 * 		
 *              -
 */
void saadc_stop()
{
    nrf_drv_timer_disable(&m_timer);
    nrf_drv_timer_uninit(&m_timer);
    nrf_drv_ppi_channel_disable(m_ppi_channel);
    nrf_drv_ppi_uninit();
    nrf_drv_saadc_abort();
    nrf_drv_saadc_uninit();
    while(nrf_drv_saadc_is_busy());
}




/* @author	ALPO
 *
 * @laste dit   24.12.2019
 *              
 * @brief	saadc_calibration
 *
 * @param 		             
 */
void saadc_calibration()
{
    // Stop ADC to get idle state for calibration
    //saadc_stop();
    nrf_drv_saadc_abort();

    //Trigger calibration task
    while(nrf_drv_saadc_calibrate_offset() != NRF_SUCCESS); 

    while(!nrf_saadc_event_check(NRF_SAADC_EVENT_CALIBRATEDONE ))
    {
      #if(NRF_ADC_LOG_ON)
        nrf_delay_ms(1000);
        NRF_LOG_INFO("adc_calibrate: Waiting for event done!\n");
        NRF_LOG_FLUSH();
      #endif
    }

    // Start ADC again
    saadc_start();

}





/* @author	Nordic Semiconductor
 *
 * @edit by     ALPO
 *
 * @laste dit   01.05.2019
 *              
 * @brief	saadc_sampling_trigger
 *  
 *              Trigger for manuel adc conversion start.
 *              After the function call, the adc will start
 *              of reading analoge values without a timer event.
 *              No DMA support
 *
 * @param saadc_sampling_trigger
 * 		
 *              -
 */
void saadc_sampling_trigger(void)
{
    ret_code_t err_code;
	//Event handler is called immediately after conversion is finished.
	err_code = nrf_drv_saadc_sample(); // Check error
	APP_ERROR_CHECK(err_code);
}










/* @author	Nordic Semiconductor
 *
 * @edit by     ALPO
 *
 * @laste dit   08.06.2019
 *              
 * @brief	saadc_timer_callback
 *
 *              Callback function for analog digital converter
 *              this function is called when the timer reached the
 *              compare time of x ms. 
 *              We use the two channel ADC, in this case the callback
 *              function is called two times!
 *
 *              After the second call, the new data flag will set to true 
 *           -
 */
void saadc_timer_callback(nrf_drv_saadc_evt_t const * p_event){




    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
        ret_code_t err_code;
        err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, SAMPLES_IN_BUFFER);
        APP_ERROR_CHECK(err_code);

        // Generate mean value
        averaging_adc_values();

        // Select new moisture Sensor
        moisture_sensor_number++;
        moisture_sensor_select(&moisture_sensor_number);
    }
}