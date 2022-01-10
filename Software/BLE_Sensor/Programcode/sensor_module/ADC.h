/*********************************************************************
* ADC.c includes all analog digital converter functions
*
* Technische Hochschule Koeln | RhizoTech 10.12.2019
*
* @ALPO
*
/*********************************************************************/
#ifndef ADC_H
#define ADC_H


// Includes
//#include "POWER_MANAGEMENT.h"
#include "nrf_drv_saadc.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "nrfx_timer.h"
#include "nrf_delay.h"
#include "boards.h"
#include "DATA_TRANSMISSION.h"


// Defines
#define SAMPLES_IN_BUFFER 32


// Deklaration of variables
extern uint8_t state;
extern int32_t ADC_VAL0;

extern nrf_saadc_value_t            m_buffer_pool[SAMPLES_IN_BUFFER];
extern nrf_ppi_channel_t            m_ppi_channel;
extern uint32_t                     m_adc_evt_counter;





// Deklaration of Functions
void saadc_timer_callback(nrf_drv_saadc_evt_t const * p_event);
void saadc_init(void);
void saadc_sampling_trigger(void);
void saadc_stop_ll(bool reset);
void saadc_sampling_event_enable(void);
void saadc_sampling_event_init(void);
void saadc_init(void);
void averaging_adc_values();
void saadc_calibration(void);
void moisture_sensor_select(uint8_t *sensor_number);
void saadc_read_values(void);
void saadc_sampling_event_disable(void);
void saadc_stop(void);
#endif /* ADC_H */
