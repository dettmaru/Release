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
#include "SENSOR_ACCESS_CONTROL.h"


// Defines
#define FILE_ID_SENSOR_NAME         0x1111  /* The ID of the file to write the records into. */
#define RECORD_KEY_SENSOR_NAME      0x2222  /* A key for the first record. */


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
  bool      all_sensors_init;
  uint8_t   sensor_number;
  uint8_t   system_number;
  uint8_t   sleep_time;
  int8_t    tx_power[18];
} flash_data_struct;

typedef flash_data_struct *ptr_flash_data_struct;
extern ptr_flash_data_struct  flash_data;




// Deklaration of Functions
void flash_read(void);
void flash_write(void);
void flash_init(void);
void flash_update(void);
void flash_clear(void);
void flash_clear_all(void);
#endif /* FLASH */
