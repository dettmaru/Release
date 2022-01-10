/*********************************************************************
* PERIPHERY.c includes all initial functions for periphery elements
*
* Technische Hochschule Koeln | RhizoTech 10.12.2019
*
* @ALPO
*
/*********************************************************************/
#include "PERIPHERY.h"


const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);  // Create two wire interface instance
APP_PWM_INSTANCE(PWM1,4);                                           // Create the instance "PWM1" using TIMER4.
static volatile bool ready_flag;                                    // A flag indicating PWM status.
volatile uint8_t I2C_FastMode;                                      
uint8_t Slave_Adress;

void error_function(ret_code_t err_code);
void timer_err_check(ret_code_t err_code);




/* @author	ALPO
 *
 * @laste dit   02.01.2020
 *              
 * @brief	sleep_timer_handler
 *              
 *              
 *
 * @param 
 * 		
 *              -
 */ 
static void sleep_timer_handler(void * p_context)
{
  NRF_LOG_INFO("Good morning! %d", global_data_vector->numSleep);
  NRF_LOG_FLUSH();
  //nrf_delay_ms(100);
  // Restart System to be sure that
  // system waiks up
  sleep_timer_stop();
  global_data_vector->numSleep = global_data_vector->numSleep + 1;
  if(global_data_vector->numSleep == 3){
    NVIC_SystemReset();
  }
  sleep_timer_start();
}



/* @author	ALPO
 *
 * @laste dit   27.01.2020
 *              
 * @brief	create_timer
 *              Create all timer modules
 *              
 *
 * @param 
 * 		
 *              -
 */ 
void create_timer()
{
    ret_code_t err_code;

    // Create sleep timer
    err_code = app_timer_create(&sleep_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                sleep_timer_handler);
    timer_err_check(err_code);
}



/* @author	ALPO
 *
 * @laste dit   27.01.2020
 *              
 * @brief	timer_err_check()
 *              Printing error log if an error happend
 *              
 *
 * @param 
 * 		
 *              -
 */ 
void timer_err_check(ret_code_t err_code){

    #if(NRF_LOG_ON)
      if(err_code == NRF_SUCCESS){
          NRF_LOG_INFO("sleep_timer: successful\n");
          NRF_LOG_FLUSH();
      } else if (err_code == NRF_ERROR_INVALID_PARAM){
          NRF_LOG_INFO("sleep_timer: parameter was invalid\n");
          NRF_LOG_FLUSH();
      } else if (err_code == NRF_ERROR_INVALID_STATE){
          NRF_LOG_INFO("sleep_timer: module has not been initialized or is running\n");
          NRF_LOG_FLUSH();
      }
    #endif
    //APP_ERROR_CHECK(err_code);


}





/* @author	ALPO
 *
 * @laste dit   13.04.2020
 *              
 * @brief	sleep_timer_start();
 *              
 * @param 
 */ 
void sleep_timer_start()
{

    ret_code_t err_code;

    app_timer_stop (sleep_timer_id);

    app_timer_start(sleep_timer_id, APP_TIMER_TICKS(global_data_vector->sleep_time*60*1000), NULL);

    // Check for errors
    error_function(err_code);
}



/* @author	ALPO
 *
 * @laste dit   02.01.2020
 *              
 * @brief	error_function()
 *              Comparing error code and printing
 *              log if a known bug happend
 *
 * @param       ret_code_t err_code
 */ 
void error_function(ret_code_t err_code){

    if(err_code == NRF_SUCCESS){
        NRF_LOG_INFO("sleep_timer: successful\n");
        NRF_LOG_FLUSH();
    } else if (err_code == NRF_ERROR_INVALID_PARAM){
        NRF_LOG_INFO("sleep_timer: parameter was invalid\n");
        NRF_LOG_FLUSH();
    } else if (err_code == NRF_ERROR_INVALID_STATE){
        NRF_LOG_INFO("sleep_timer: module has not been initialized or is running\n");
        NRF_LOG_FLUSH();
    }else if (err_code == NRF_ERROR_NO_MEM){
        NRF_LOG_INFO(" timer operations queue was full\n");
        NRF_LOG_FLUSH();
    }
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
    // Disable Timer
    err_code = app_timer_stop(sleep_timer_id);

}



/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	twi_init
 *              
 *              400kHz Clock Frequency
 *
 * @param 
 * 		
 *              -
 */ 
void twi_init (void)
{
    temp_sensor_active();
    ret_code_t err_code;

    const nrf_drv_twi_config_t twi_config = {
       .scl                = SM_CLK,
       .sda                = SM_DATA,
       .frequency          = NRF_DRV_TWI_FREQ_400K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    err_code = nrf_drv_twi_init(&m_twi, &twi_config, NULL, NULL);

    if(err_code == NRF_SUCCESS){
        NRF_LOG_INFO("i2c_init: successful\n");
        NRF_LOG_FLUSH();

    } else if(err_code == NRF_ERROR_INVALID_STATE){
        NRF_LOG_INFO("i2c_init: driver not ready for new transfer\n");
        NRF_LOG_FLUSH();


    /* If some other peripheral with the same instance ID 
    is already in use. This is possible only if 
    PERIPHERAL_RESOURCE_SHARING_ENABLED 
    is set to a value other than zero. */
    } else if(err_code == NRF_ERROR_BUSY){
        NRF_LOG_INFO("i2c_init: instance id already used\n");
        NRF_LOG_FLUSH();
    }

    APP_ERROR_CHECK(err_code);
    
    // Enable TWI Module
    //nrf_drv_twi_enable(&m_twi);

}





/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	twi_enable()
 *
 */ 
void twi_enable(){
  nrf_drv_twi_enable(&m_twi);
}




/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	twi_disable
 *
 */ 
void twi_disable(){
  nrf_drv_twi_disable(&m_twi);
}



/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	prepare_twi_to_sleep()
 *
 *              Uninit twi Module before sensor
 *              go to sleep mode
 * @param     
 *              -
 */ 
void prepare_twi_to_sleep(){

  // Disable TWI Module
  twi_disable();

  // Uninit Module
  nrf_drv_twi_uninit(&m_twi);
}



/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	temp_sensor_standby
 *
 *              Set Standby Pin to
 *              low level
 */ 
void temp_sensor_standby(){

  // Active Low
  nrf_gpio_pin_clear(STBY);

}



/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	temp_sensor_active
 *
 *              Set Standby Pin to
 *              high level in active
 *              mode
 */ 
void temp_sensor_active(){

  // Active Low
  nrf_gpio_pin_set(STBY);

}




/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	temp_sensor_read
 *
 *              Reading Temperature with sm bus compatible
 *              i2c bus
 *
 * @param 
 * 		
 *              -
 */ 
void temp_sensor_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    ret_code_t err_code0;
    ret_code_t err_code1;

    // Transmit register adress of sensor
    err_code0 = nrf_drv_twi_tx(&m_twi,dev_id,&reg_addr,1,false);

    if(err_code0 == NRF_SUCCESS){
        NRF_LOG_INFO("i2c_read: successful\n");
        NRF_LOG_FLUSH();

    } else if(err_code0 == NRF_ERROR_BUSY){
        NRF_LOG_INFO("i2c_read: driver not ready for new transfer\n");
        NRF_LOG_FLUSH();


    } else if(err_code0 == NRF_ERROR_INTERNAL){
        NRF_LOG_INFO("i2c_read: hardware error\n");
        NRF_LOG_FLUSH();
    }
    
    //APP_ERROR_CHECK(err_code0);
    
    // Receive Sensor temperature values
    err_code1 = nrf_drv_twi_rx(&m_twi,dev_id,reg_data,len);

    if(err_code1 == NRF_SUCCESS){
      NRF_LOG_INFO("i2c_read: successful\n");
      NRF_LOG_FLUSH();

    } else if(err_code1 == NRF_ERROR_BUSY){
        NRF_LOG_INFO("i2c_read: driver not ready for new transfer\n");
        NRF_LOG_FLUSH();


    } else if(err_code1 == NRF_ERROR_INTERNAL){
        NRF_LOG_INFO("i2c_read: hardware error\n");
        NRF_LOG_FLUSH();
    }
    
    //APP_ERROR_CHECK(err_code1);
}



/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	temp_sensor_read
 *
 *              Reading Temperature with sm bus compatible
 *              i2c bus
 *
 * @param 
 * 		
 *              -
 */ 
void temp_sensor_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    ret_code_t err_code;

    uint8_t write_data[MAX_WRITE_LENGTH];
    
    write_data[0] = reg_addr;
    
    memcpy(&write_data[1],reg_data,len);
    
    err_code = nrf_drv_twi_tx(&m_twi,dev_id,write_data,len+1,false);

    // Print Log
    if(err_code == NRF_SUCCESS){
        NRF_LOG_INFO("i2c_write: successful\n");
        NRF_LOG_FLUSH();

    } else if(err_code == NRF_ERROR_BUSY){
        NRF_LOG_INFO("i2c_write: driver not ready for new transfer\n");
        NRF_LOG_FLUSH();


    } else if(err_code == NRF_ERROR_INTERNAL){
        NRF_LOG_INFO("i2c_write: hardware error\n");
        NRF_LOG_FLUSH();
    } 
}




/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	tem_sensor_device_test
 *
 *              The used multichannel temperature sensor has 4 different
 *              slaves adresses. The first step is to find out the right
 *              adress. If the adress is founded, the secound step is to
 *              test the SM Bus (I2C) connection with reading and comparing 
 *              the manufacturer id
 * @param 
 * 		
 *              -
 */ 
bool temp_sensor_device_test()
{

    // Go from Standby Mode on Active Mode
    // and enable TWI Module
    twi_enable();
    temp_sensor_active();
    nrf_delay_ms(1000);

    uint8_t MAX6581_id_register = 0x0a;
    uint8_t MAX6581_manufacturer_id = 0x4d;
    uint8_t read_data;
    uint8_t Slave_Adresses[] = {Slv_Adr_0, Slv_Adr_1, Slv_Adr_2, Slv_Adr_3};
    Slave_Adress = 0;

    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Start I2C Device Test");
      NRF_LOG_FLUSH();
    #endif

    // Call all slave Adresses and check for feedback
    for(uint8_t i = 0; i<4; i++){

      temp_sensor_read(Slave_Adresses[i], MAX6581_id_register, &read_data, 1);
      
      // If there is an feedback, compare manufacurer id from maxim integrated and save adress
      // for this session
      if(read_data==MAX6581_manufacturer_id){
      #if(NRF_LOG_ON)
          NRF_LOG_INFO("Slave Adress found, the Adress is:0x%x", Slave_Adresses[i]);
          NRF_LOG_FLUSH();
      #endif
        Slave_Adress = Slave_Adresses[i];
        break;
      }
    }

    // Print log Feedback and set Sensor to standby mode
    if(Slave_Adress > 0)
    {
        #if(NRF_LOG_ON)
          NRF_LOG_INFO("I2C communicate test pass!\n");
          NRF_LOG_FLUSH();
        #endif
        // Set Sensor to Standby Mode
        // and disable TWI Module
        //temp_sensor_standby();
        twi_disable();
        return true; 
    }
    else
    {
        #if(NRF_LOG_ON)
          NRF_LOG_INFO("I2C communicate test faiure!\n");
          NRF_LOG_INFO("Slave Adress not found\n");
          NRF_LOG_FLUSH();
        #endif
    }

    // Set Sensor to Standby Mode
    // and disable TWI Module
    //temp_sensor_standby();
    twi_disable();
    return false;
}


/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	temp_sensor_write_settings()
 *
 *              From MAX6581 Datasheet S.16 Table 4
 *              
 *              Bit 0
 *              Reserved - Must be set to 0
 *              
 *              Bit 1 
 *              Extended-Range Enable on. 
 *              Set bit 1 to logic 1 to set the temperature 
 *              and limit data range to -64°C to +191°C. 
 *              Set bit 1 to logic 0 to set the range to 0°C to +255°C.
 *
 *              Bit 2
 *              Reserved - Must be set to 0
 *
 *              Bit 3
 *              Reserved - Must be set to 0
 *
 *              Bit 4
 *              Reserved - Must be set to 0
 *
 *              Bit 5
 *              SMBus Timeout Bit (0 - Enable)
 *              To use i2c Bus, this but must be disable
 *              -> put to 1 !!
 *
 *              Bit 6
 *              Reset Bit
 *
 *              Bit 7
 *              Standby Mode Control Bit
 *
 * @param 
 * 		
 *              -
 */ 
void temp_sensor_write_settings()
{

    // Go from Standby Mode on Active Mode
    // and enable TWI Module
    twi_enable();
    temp_sensor_active();

    uint8_t config_register = 0x41;
    uint8_t config = 0b00100010;

    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Write Settings to Sensor");
      NRF_LOG_FLUSH();
    #endif

    // Enable offset on Sensor (1-7)
    temp_sensor_write(Slave_Adress, config_register, &config, 1);


    // Set Sensor to Standby Mode
    // and disable TWI Module
    twi_disable();
}




void temp_sensor_calibration()
{

    // Go from Standby Mode on Active Mode
    // and enable TWI Module
    twi_enable();
    temp_sensor_active();

    uint8_t offset_select = 0x4e;
    uint8_t offset_register = 0x4d;
    uint8_t claibration = 0b00000111; // +1 degree
    uint8_t sensor_select = 0b01111111;

    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Write calibration settings to Sensor");
      NRF_LOG_FLUSH();
    #endif



      // Select Sensor Channel
      temp_sensor_write(Slave_Adress, sensor_select, &sensor_select, 1);

      temp_sensor_write(Slave_Adress, offset_register, &claibration, 1);   
    

    // Set Sensor to Standby Mode
    // and disable TWI Module
    twi_disable();
}




/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	temp_sensor_offset
 *
 *              From MAX6581 Datasheet S.22 Table 13
 *              
 *              To compensate for remote temperature reporting errors
 *              due to issues with the board layout, the offset register
 *              allow for a two's complement value to be add to the
 *              final adc converter.
 *
 *              Bit 0       + 0.25C
 *              Bit 1       + 0.5C
 *              Bit 2       + 1C
 *              Bit 3       + 2C
 *              Bit 4       + 4C
 *              Bit 5       + 8C
 *              Bit 6       + 16sC
 *              Bit 7 (MSB) xxxx
 * @param 
 * 		
 *              -
 */ 
void temp_sensor_offset()
{

    // Go from Standby Mode on Active Mode
    // and enable TWI Module
    twi_enable();
    temp_sensor_active();
    nrf_delay_ms(1000);
    
    uint8_t offset_register = 0x4d;
    uint8_t offset_select_register = 0x4e;
    uint8_t digital_offset_polarity = 0b00000000;
    uint8_t remote_sensor = 0b01111111;
    
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Write Settings to Sensor");
      NRF_LOG_FLUSH();
    #endif


    // Enable offset on Sensor (1-7)
    temp_sensor_write(Slave_Adress, offset_select_register, &remote_sensor, 1);
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Write Sensor: 0x%x", remote_sensor);
      NRF_LOG_FLUSH();
    #endif


    // Write Digital Offset Polarity on the Remote Sensor Register
    temp_sensor_write(Slave_Adress, offset_register, &digital_offset_polarity, 1);


    // Set Sensor to Standby Mode
    // and disable TWI Module
    //temp_sensor_standby();
    twi_disable();
}





/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	temp_sensor_read_all
 *
 *              - Reading all sensor datas with high and low byte
 *              - Calculating the sum of all sensor high bytes. At 0 degree
 *                the sensor high byte has a value of 64. If the values
 *                are lower then 64 degree, the temperature is negative
 *              - Calculating temperature values in degree
 *              - Printing temperature values on log
 * @param 
 *              -
 */ 
void temp_sensor_read_all()
{

    // Go from Standby Mode on Active Mode
    // and enable TWI Module
    twi_enable();
    temp_sensor_active();

    uint8_t MAX6581_id_register = 0x0a;
    uint8_t MAX6581_manufacturer_id = 0x4d;
    uint8_t read_data[2][7];
    float sensor_val[7];
    uint16_t sum = 0;
    
    #if(NRF_LOG_ON)
      NRF_LOG_INFO("Reading Temperature Sensors");
      NRF_LOG_FLUSH();
    #endif

    // Reading High Byte
    temp_sensor_read(Slave_Adress, Remote_1_HB, &read_data[0][0], 1);
    temp_sensor_read(Slave_Adress, Remote_2_HB, &read_data[0][1], 1);


    // Reading Low Byte
    temp_sensor_read(Slave_Adress, Remote_1_LB, &read_data[1][0], 1);
    temp_sensor_read(Slave_Adress, Remote_2_LB, &read_data[1][1], 1);


    // Copy data to global vector
    for(int i = 0; i<7; i++){
      global_data_vector->temperature[i*2] = read_data[0][i];
      global_data_vector->temperature[(i*2)+1] = read_data[1][i];
    }


    // Calculate sum from all received high byte vlaues
    // to check the temperature
    for(int i = 0; i<7; i++){ 
      sum = sum + read_data[0][1];
    }


    // If Value of high byte register is larger than 64, the temperature value is positive
    if(sum >= 64*7){

        for(int i = 0; i<7; i++){
          sensor_val[i] = (((read_data[0][i]-64) << 3) + ((read_data[1][i] >> 5)));
        }

        // Print Sensor Value
        #if(NRF_LOG_ON)
          NRF_LOG_INFO("Temp Value 1 is: " NRF_LOG_FLOAT_MARKER "", NRF_LOG_FLOAT(sensor_val[0]/8));
          NRF_LOG_FLUSH();
          NRF_LOG_INFO("Temp Value 2 is: " NRF_LOG_FLOAT_MARKER "", NRF_LOG_FLOAT(sensor_val[1]/8));
          NRF_LOG_FLUSH();
        #endif
    } else {

        for(int i = 0; i<7; i++){
          sensor_val[i] = ((read_data[0][i] << 3) + ((read_data[1][i] >> 5)));
        }

        // Print Sensor Value
        #if(NRF_LOG_ON)
          NRF_LOG_INFO("The Sensor 1 value is: -" NRF_LOG_FLOAT_MARKER "", NRF_LOG_FLOAT(sensor_val[0]/8));
          NRF_LOG_FLUSH();
          NRF_LOG_INFO("The Sensor 2 value is: -" NRF_LOG_FLOAT_MARKER "", NRF_LOG_FLOAT(sensor_val[1]/8));
          NRF_LOG_FLUSH();
        #endif
    }

    // Set Sensor to Standby Mode
    // and disable TWI Module
    temp_sensor_standby();
    twi_disable();
    ctr_flags->spi_reading_finish = 1;
}



/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	pwm_callback
 *
 *              PWM Ready Callback Funktion
 *              PWM callback that is executed when a PWM 
 *              duty change has been completed.
 * @param 
 * 		
 *              -
 */ 
void pwm_callback(uint32_t pwm_id)   
{
    ready_flag = true;
}




/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	init_pwm
 *
 *              Function for init pwm
 *              1 channel 
 *              50% duty cycle
 *              1 MHz Frequency
 * @param 
 * 		
 *              -
 */
void moisture_clock_init(){
    ret_code_t err_code;
    // 1-channel PWM, 1MHz, output on GPIO_PWM pins
    app_pwm_config_t pwm1_cfg = APP_PWM_DEFAULT_CONFIG_1CH(1, GPIO_PWM);

    // Initialize and enable PWM
    err_code = app_pwm_init(&PWM1,&pwm1_cfg,pwm_callback);

    // Disable PWM to save energy
    stop_moisture_clock();

    // Log Output for RTT Viewer
    #if(NRF_LOG_ON)
      if(err_code == NRF_SUCCESS){
          NRF_LOG_INFO("pwm_init: init successful");
          NRF_LOG_FLUSH();

      } else if(err_code == NRF_ERROR_NO_MEM){
          NRF_LOG_INFO("pwm_init: not enough free resources");
          NRF_LOG_FLUSH();

      } else if(err_code == NRF_ERROR_INVALID_PARAM){
          NRF_LOG_INFO("pwm_init: invaild configuration structure");
          NRF_LOG_FLUSH();

      } else if(err_code == NRF_ERROR_INVALID_STATE){
          NRF_LOG_INFO("pwm_init: tmr or pwm instance is already in use or init failed");
          NRF_LOG_FLUSH();
      }
    #endif
    APP_ERROR_CHECK(err_code);
}




/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	moisture_clock_ready_check
 *
 *              Check for duty cycle changes
 *              and pwm ready
 *
 * @param 
 * 		
 *              -
 */ 
bool moisture_clock_ready_check()   
{
    return app_pwm_busy_check(&PWM1);
}




/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	prepare_moisture_clock_to_sleep
 *
 *              Disabling PWM and uninit pwm module
 * @param 
 * 		
 *              -
 */ 
bool prepare_moisture_clock_to_sleep()   
{
    // Stop PWM Clock
    stop_moisture_clock();
    // Uninut PWM Module
    if(app_pwm_uninit(&PWM1) == NRF_SUCCESS){
        NRF_LOG_INFO("pwm_uninit: succesful");
        NRF_LOG_FLUSH();
        return NO_ERROR;

    } else if(app_pwm_uninit(&PWM1) == NRF_ERROR_BUSY){
        NRF_LOG_INFO("pwm_uninit: pwm not ready yet");
        NRF_LOG_FLUSH();
        return NO_ERROR;

    } else if(app_pwm_uninit(&PWM1) == NRF_ERROR_INVALID_STATE){
        NRF_LOG_INFO("pwm_uninit: instance was not init");
        NRF_LOG_FLUSH();
        return ERROR;
    }

}









/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	stop_moisture_clock
 *
 *              PWM Clock should stopped
 *              after moisture sensor reading
 *              to save energy
 * @param 
 * 		
 *              -
 */ 
void stop_moisture_clock(){
    app_pwm_disable(&PWM1);
}



/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	uninit_moisture_clock
 *
 *              PWM Clock should stopped
 *              after moisture sensor reading
 *              to save energy
 * @param 
 * 		
 *              -
 */ 
void uninit_moisture_clock(){
    app_pwm_uninit(&PWM1);
}




/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	start_moisture_clock
 *
 *              Start PWM clock with 50% duty cycle
 *              on channel 0 for moisture sensor reading
 * @param 
 * 		
 *              -
 */ 
void start_moisture_clock(){

    app_pwm_enable(&PWM1);
    // Set duty cycle 50%
    app_pwm_channel_duty_set(&PWM1, 0, 50); 

    NRF_LOG_INFO("pwm: enabled");
    NRF_LOG_FLUSH();

}





/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module.
 */
void timers_init(void)
{
    // Initialize timer module, making it use the scheduler
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

}





/**@brief Function for the LEDs initialization.
 *
 * @details Initializes all LEDs used by the application.
 */
void leds_init(void)
{
    bsp_board_init(BSP_INIT_LEDS);
}







/* @author	ALPO
 *
 * @laste dit   19.05.2019
 *              
 * @brief	gpio_init
 *
 * @param gpio_init
 * 		
 *              -
 * Set GPIO Pin 1.13 to Output 
 */
void gpio_init(void)
{
    // Ausgang
    nrf_gpio_cfg_output(STBY);
    nrf_gpio_cfg_output(MPX_A);
    nrf_gpio_cfg_output(MPX_B);
    nrf_gpio_cfg_output(MPX_C);

    #if(PWR_LED_ON)
      nrf_gpio_cfg_output(PWR_LED);
      nrf_gpio_pin_set(PWR_LED);		
    #endif

    // Power On Blinky
    for(int i = 0; i<10; i++){
      nrf_gpio_pin_clear(PWR_LED);
      nrf_delay_ms(50);
      nrf_gpio_pin_set(PWR_LED);
      nrf_delay_ms(50);
    }
}



/* @author	ALPO
 *
 * @laste dit   19.12.2019
 *              
 * @brief	gpio_init
 *
 * @param gpio_init
 * 		
 *              -
 * Set GPIO Pin 1.13 to Output 
 */
void gpio_uninit(void)
{
    // Ausgang
    nrf_gpio_cfg_default(STBY);
    nrf_gpio_cfg_default(MPX_A);
    nrf_gpio_cfg_default(MPX_B);
    nrf_gpio_cfg_default(MPX_C);

    #if(PWR_LED_ON)
      nrf_gpio_cfg_default(PWR_LED);
      nrf_gpio_pin_clear(PWR_LED);		
    #endif
}