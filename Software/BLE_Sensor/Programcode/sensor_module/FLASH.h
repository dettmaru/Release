/*********************************************************************
* FLASH.h includes all read/write flash functions
*
* Technische Hochschule Koeln | RhizoTech 04.01.2020
*
* @ALPO
*
/*********************************************************************/
#ifndef FLASH_H
#define FLASH_H

// Includes
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "fds.h"
#include "nrf_delay.h"
//#include "MAIN.h"


// Defines
#define FILE_ID_SENSOR_NAME         0x3333  // The ID of the file to write the records into
#define RECORD_KEY_SENSOR_NAME      0x4444  // A key for the first record



#ifndef NRF_FLASH_LOG_ON
#define NRF_FLASH_LOG_ON 1
#endif

// Deklaration of variables
typedef struct {
  bool  fds_init_finish;
  bool  flash_write_finish;
  bool  flash_update_finish;
  bool  flash_rec_del_finish;
  bool  flash_file_del_finish;
  bool  flash_gc_finish;
} flash_flags;




typedef struct {
  bool      sensor_init;
  uint8_t   sensor_name_number;
  int8_t    tx_power;
} flash_data_struct;

typedef flash_data_struct *ptr_flash_data_struct;
extern ptr_flash_data_struct  flash_data;


// Deklaration of Functions
void flash_write(void);
void flash_read(void);
void flash_init(void);
void flash_update(void);
void flash_clear(void);
void flash_clear_all(void);
void flash_delete_file(void);
#endif /* FLASH */
