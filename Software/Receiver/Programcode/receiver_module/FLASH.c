/*********************************************************************
* FLASH.c includes all read/write flash functions
*
* Technische Hochschule Koeln | RhizoTech 04.01.2020
*
* @ALPO
*
/*********************************************************************/
#include "FLASH.h"
#include "fds.h"

// Declaration of private functions
void fds_err_check(ret_code_t ret);
void flash_clear();

int8_t flash_space[21];
int8_t flash_space_temp[21];
flash_flags ff;
flash_data_struct flash_data_space;
ptr_flash_data_struct flash_data = &flash_data_space;






/* @author	ALPO
 *
 * @laste dit   04.01.2020
 *              
 * @brief	cpy_arr_to_struct()
 *
 * @param 
 */
void cpy_arr_to_struct(){

  flash_data->sensor_number     =  (int8_t)flash_space[0];
  flash_data->system_number     =  (int8_t)flash_space[1];
  flash_data->all_sensors_init  =  (int8_t)flash_space[2];
  flash_data->sleep_time        =  (int8_t)flash_space[3];

  #if(NRF_FLASH_LOG_ON)
    NRF_LOG_INFO("flash_data->sensor_number %d", flash_data->sensor_number);
    NRF_LOG_INFO("flash_data->system_number %d", flash_data->system_number);
    NRF_LOG_INFO("flash_data->all_sensors_init %d", flash_data->all_sensors_init);
    NRF_LOG_INFO("flash_data->sleep_time %d", flash_data->sleep_time);
    NRF_LOG_FLUSH();
  #endif

}


/* @author	ALPO
 *
 * @laste dit   16.01.2020
 *              
 * @brief	cpy_struct_to_arr()
 *
 * @param 
 */
void cpy_struct_to_arr(){
  flash_space[0] = (uint8_t)flash_data->sensor_number;
  flash_space[1] = (uint8_t)flash_data->system_number;
  flash_space[2] = (uint8_t)flash_data->all_sensors_init;
  flash_space[3] = (uint8_t)flash_data->sleep_time;
}





/* @author	ALPO
 *
 * @laste dit   17.01.2020
 *              
 * @brief	fds_evt_handler
 *              Flash event handler for checking
 *              all possible error states
 * @param 
 */
void fds_evt_handler(fds_evt_t const * p_fds_evt)
{
    switch (p_fds_evt->id)
    {
        case FDS_EVT_INIT:
            if (p_fds_evt->result != FDS_SUCCESS)
            {
                #if(NRF_FLASH_LOG_ON)
                NRF_LOG_INFO("FDS_Init: Failed.");
                NRF_LOG_FLUSH();
                #endif
            }
            ff.fds_init_finish = 1;
            break;

        case FDS_EVT_WRITE:
            #if(NRF_FLASH_LOG_ON)
              NRF_LOG_INFO("Event for fds_record_write and fds_record_write_reserved.");
              NRF_LOG_FLUSH();
            #endif
            ff.flash_write_finish = 1;
            break;

        case FDS_EVT_UPDATE:
            #if(NRF_FLASH_LOG_ON)
              NRF_LOG_INFO("Event for fds_record_update.");
              NRF_LOG_FLUSH();
            #endif
            ff.flash_update_finish = 1;
            break;

        case FDS_EVT_DEL_RECORD:
            #if(NRF_FLASH_LOG_ON)
              NRF_LOG_INFO("Event for fds_record_delete.");
              NRF_LOG_FLUSH();
            #endif
            ff.flash_rec_del_finish = 1;
            break;

        case FDS_EVT_DEL_FILE:
            #if(NRF_FLASH_LOG_ON)
              NRF_LOG_INFO("Event for fds_file_delete.");
              NRF_LOG_FLUSH();
            #endif
            ff.flash_file_del_finish = 1;
            break;

        case FDS_EVT_GC:
            #if(NRF_FLASH_LOG_ON)
              NRF_LOG_INFO("Event for fds_gc.");
              NRF_LOG_FLUSH();
            #endif
            ff.flash_gc_finish = 1;
            break;


        default:
            break;
    }
}




/* @author	ALPO
 *
 * @laste dit   04.01.2020
 *              
 * @brief	flash_vector_init()
 *
 * @param 
 */
void flash_vector_init(){

    for(uint8_t i = 0; i<3; i++){
      flash_space[i] = 0x5;
      flash_space_temp[i] = 0;
    }
}



/* @author	ALPO
 *
 * @laste dit   04.01.2020
 *              
 * @brief	flash_init()
 *
 * @param 
 */
void flash_init(){

  flash_vector_init();

  ret_code_t ret = fds_register(fds_evt_handler);
  if (ret != FDS_SUCCESS){
      #if(NRF_FLASH_LOG_ON)
        NRF_LOG_INFO("FDS_Register: Registering of the FDS event handler has failed.");
        NRF_LOG_FLUSH();
      #endif

  } else {
      #if(NRF_FLASH_LOG_ON)
        NRF_LOG_INFO("FDS_Register: Success.");
        NRF_LOG_FLUSH();
      #endif
  }
  
  ret = fds_init();
  if (ret != FDS_SUCCESS)
  {
      #if(NRF_FLASH_LOG_ON)
        NRF_LOG_INFO("FDS_Init: Handle Error.");
        NRF_LOG_FLUSH();
      #endif
  }

}





/* @author	ALPO
 *
 * @laste dit   04.01.2020
 *              
 * @brief	flash_write()
 *
 * @param 
 */
void flash_write(){


    cpy_struct_to_arr();

    // Save the values in Flash_Vector
    memcpy(flash_space_temp, flash_space, sizeof(flash_space));

    fds_record_t        record;
    fds_record_desc_t   record_desc;
    ret_code_t ret;

    // Set up record.
    record.file_id           = FILE_ID_SENSOR_NAME;
    record.key               = RECORD_KEY_SENSOR_NAME;
    record.data.p_data       = flash_space_temp;
    record.data.length_words   = sizeof(flash_space_temp)/4;

    NRF_LOG_INFO("Dateigroesse: %d", sizeof(flash_space_temp)/4);
                    
    ret = fds_record_write(&record_desc, &record);
    if(ret == FDS_ERR_NO_SPACE_IN_QUEUES){
      while(fds_record_write(&record_desc, &record) != FDS_SUCCESS);
    }
    fds_err_check(ret);



    uint8_t flash_brake_up_counter = 0;

    while((ff.flash_write_finish == 0) && (flash_brake_up_counter < 100)){
      flash_brake_up_counter++;
      nrf_delay_ms(10);
    }

    if(flash_brake_up_counter == 100){
          NRF_LOG_INFO("Flash: Data saving error, it takes too much time! Brake up");
    }
    ff.flash_write_finish = 0;

    //for(uint8_t i = 0; i<3; i++){
    //  flash_space[i] = 0xFF;
    //  flash_space_temp[i] = 0xFF;
    //}

}


/* @author	ALPO
 *
 * @laste dit   04.01.2020
 *              
 * @brief	flash_read()
 *
 * @param 
 */
void flash_read(){

    fds_flash_record_t  flash_record;
    fds_record_desc_t   record_desc;
    fds_find_token_t    ftok;
    ret_code_t          ret;
    uint8_t             *data;


    /* It is required to zero the token before first use. */
    memset(&ftok, 0x00, sizeof(fds_find_token_t));
    ftok.page=0;
    ftok.p_addr=NULL;

    /* Loop until all records with the given key and file ID have been found. */
    while (fds_record_find_in_file(FILE_ID_SENSOR_NAME, &record_desc, &ftok) == FDS_SUCCESS)
    {

        /* Open the record */
        
        ret = fds_record_open(&record_desc, &flash_record);
        if(ret == FDS_ERR_NO_SPACE_IN_QUEUES){
          while(fds_record_open(&record_desc, &flash_record) != FDS_SUCCESS);
        }
        fds_err_check(ret);


        /* Access the record through the flash_record structure. */
        //Flash_Vector * Flash_Work_Vector = (Flash_Vector *)(flash_record.p_data);        
        #if(NRF_FLASH_LOG_ON)
          NRF_LOG_INFO("Found Record ID = %d\r\n",record_desc.record_id);
          NRF_LOG_INFO("Data = ");
        #endif
        data = (uint8_t *) flash_record.p_data;
        for (uint8_t i=0;i<(flash_record.p_header->length_words)*4;i++)
        {
        #if(NRF_FLASH_LOG_ON)
            NRF_LOG_INFO("0x%x ",data[i]);
        #endif
        }

        #if(NRF_FLASH_LOG_ON)
          NRF_LOG_INFO("\r\n");
        #endif
        memcpy(flash_space, flash_record.p_data, sizeof(flash_space));


        cpy_arr_to_struct();

        /* Close the record when done. */
        ret = fds_record_close(&record_desc);
        if(ret == FDS_ERR_NO_SPACE_IN_QUEUES){
          while(fds_record_close(&record_desc) != FDS_SUCCESS);
        }
        fds_err_check(ret);
    }
}


/* @author	ALPO
 *
 * @laste dit   04.01.2020
 *              
 * @brief	flash_update()
 *              Combination of flash clear and flash write
 *              in the same function
 * @param 
 */
void flash_update(){


    fds_record_t        record;
    fds_record_desc_t   record_desc;
    ret_code_t ret;
    static uint8_t const m_deadbeef[4] = {0x20,0x21,0x22,0x23};
    fds_find_token_t    ftok;



    /* It is required to zero the token before first use. */
    memset(&ftok, 0x00, sizeof(fds_find_token_t));
    ftok.page=0;
    ftok.p_addr=NULL;

    // Set up record.
    record.file_id           = FILE_ID_SENSOR_NAME;
    record.key               = RECORD_KEY_SENSOR_NAME;
    record.data.p_data       = &m_deadbeef;
    record.data.length_words   = sizeof(m_deadbeef)/sizeof(uint8_t);


    /* Loop until all records with the given key and file ID have been found. */
    while (fds_record_find(FILE_ID_SENSOR_NAME, RECORD_KEY_SENSOR_NAME, &record_desc, &ftok) == FDS_SUCCESS)
    {

        ret = fds_record_update(&record_desc, &record);	
        if(ret == FDS_ERR_NO_SPACE_IN_QUEUES){
          while(fds_record_update(&record_desc, &record) != FDS_SUCCESS);
        }
        fds_err_check(ret);
        #if(NRF_FLASH_LOG_ON)
          NRF_LOG_INFO("Updated record ID: %d \r\n",record_desc.record_id);
        #endif
        
 
    }

}


/* @author	ALPO
 *
 * @laste dit   04.01.2020
 *              
 * @brief	flash_clear()
 *              Deletes only the last record
 * @param 
 * 		
 *              -
 */
void flash_clear(){
    /* Assume a descriptor returned by a call to fds_record_write() or fds_record_find(),
       as shown in the previous example. */

    fds_record_desc_t   record_desc;
    fds_find_token_t    ftok;

    ftok.page=0;
    ftok.p_addr=NULL;
    // Loop and find records with same ID and rec key and mark them as deleted. 
    fds_record_find(FILE_ID_SENSOR_NAME, RECORD_KEY_SENSOR_NAME, &record_desc, &ftok);
    
    while(fds_record_delete(&record_desc) != FDS_SUCCESS);

    #if(NRF_FLASH_LOG_ON)
      NRF_LOG_INFO("Deleted record ID: %d \r\n",record_desc.record_id);
    #endif
    // running garbage collection
    while(fds_gc() != FDS_SUCCESS);
}





/* @author	ALPO
 *
 * @laste dit   04.01.2020
 *              
 * @brief	flash_clear_all()
 *              Deletes all records
 * @param 
 * 		
 *              -
 */
void flash_clear_all(){
    /* Assume a descriptor returned by a call to fds_record_write() or fds_record_find(),
       as shown in the previous example. */

    fds_record_desc_t   record_desc;
    fds_find_token_t    ftok;

    ftok.page=0;
    ftok.p_addr=NULL;
    // Loop and find records with same ID and rec key and mark them as deleted. 
    while (fds_record_find_by_key(RECORD_KEY_SENSOR_NAME, &record_desc, &ftok) == FDS_SUCCESS)
    {
            while(fds_record_delete(&record_desc) != FDS_SUCCESS);
              #if(NRF_FLASH_LOG_ON)
                NRF_LOG_INFO("Deleted record ID: %d \r\n",record_desc.record_id);
                NRF_LOG_FLUSH();
              #endif
            while(ff.flash_rec_del_finish == 0);
            ff.flash_rec_del_finish = 0;
    }



    // Loop and find records with same ID and rec key and mark them as deleted. 
    while (fds_record_find_in_file(FILE_ID_SENSOR_NAME, &record_desc, &ftok) == FDS_SUCCESS)
    {
            while(fds_record_delete(&record_desc) != FDS_SUCCESS);
              #if(NRF_FLASH_LOG_ON)
                NRF_LOG_INFO("Deleted record ID: %d \r\n",record_desc.record_id);
                NRF_LOG_FLUSH();
              #endif
            while(ff.flash_file_del_finish == 0);
            ff.flash_file_del_finish = 0;
    }



    // running garbage collection
    fds_gc();
    while(ff.flash_gc_finish == 0);
    ff.flash_gc_finish = 0;

}


/* @author	ALPO
 *
 * @laste dit   04.01.2020
 *              
 * @brief	fds_err_check
 *              Check and print all possible error states
 * @param 
 * 		
 *              -
 */
void fds_err_check(ret_code_t ret){

  #if(NRF_FLASH_LOG_ON)
    switch(ret){
      case FDS_SUCCESS:{
        NRF_LOG_INFO("FDS: successfully");
        NRF_LOG_FLUSH();
      }break;

      case FDS_ERR_NOT_INITIALIZED:{
        NRF_LOG_INFO("FDS: the module is not initialized");
        NRF_LOG_FLUSH();
      }break;

      case FDS_ERR_NULL_ARG:{
        NRF_LOG_INFO("FDS: p_record is NULL");
        NRF_LOG_FLUSH();
      }break;

      case FDS_ERR_INVALID_ARG:{
        NRF_LOG_INFO("FDS: the file ID or the record key is invalid");
        NRF_LOG_FLUSH();
      }break;

      case FDS_ERR_UNALIGNED_ADDR:{
        NRF_LOG_INFO("FDS: the record data is not aligned to a 4 byte boundary");
        NRF_LOG_FLUSH();
      }break;

      case FDS_ERR_RECORD_TOO_LARGE:{
        NRF_LOG_INFO("FDS: the record data exceeds the maximum length");
        NRF_LOG_FLUSH();
      }break;

      case FDS_ERR_NO_SPACE_IN_QUEUES:{
        NRF_LOG_INFO("FDS: the operation queue is full or there are more record chunks than can be buffered");
        NRF_LOG_FLUSH();
      }break;

      case FDS_ERR_NO_SPACE_IN_FLASH:{
        NRF_LOG_INFO("FDS: there is not enough free space in flash to store the record");
        NRF_LOG_FLUSH();
      }break;

      default:
        break;

    }
  #endif
}
