/**
 ****************************************************************************************
 *
 * @file   mcu_ota.c
 *
 * @brief  .
 *
 * @author  jiachuang
 * @date    2018-11-01 11:07
 * @version <0.0.0.1>
 *
 * @license 
 *              Copyright (C) APOLLO Microelectronics 2018
 *                         ALL Right Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "mcu_master_ahi.h"
#include "mcu_master_ota.h"
#include "mcu_master_demo_test.h"
#include "diss_task.h"
#include "bxotas_task.h"
#include "app_uart.h"
#include "plf.h"
#include "flash.h"

/*
 * MACROS
 ****************************************************************************************
 */

/*
 * DEFINES
 ****************************************************************************************
 */
#define MCU_OTA_SEGEMENTS_PER_BLOCK     (21*8)


/*
 * ENUMERATIONS
 ****************************************************************************************
 */


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */




/*
 * VARIABLE DEFINITIONS
 ****************************************************************************************
 */
volatile uint32_t ota_image_size = 0;
static volatile uint32_t current_block_id = 0;
static volatile uint32_t segment_data_max_length = 0;
static volatile uint32_t param_length = 0;
static volatile uint32_t program_offset = 0;




/*
 * DECLARE
 ****************************************************************************************
 */
static void osapp_bxotas_start_cfm(uint8_t status);


/*
 * USER FUNCTION
 ****************************************************************************************
 */


/*
 * SEND MESSAGE
 ****************************************************************************************
 */



static void osapp_bxotas_start_cfm(uint8_t status)
{
    struct bxotas_start_cfm *cfm = AHI_MSG_ALLOC(BXOTAS_START_CFM, TASK_ID_BXOTAS, bxotas_start_cfm);
    cfm->status = status;
    osapp_ahi_msg_send(cfm, sizeof(struct bxotas_start_cfm), portMAX_DELAY);
}



/*
 * RECEIVE MESSAGE
 ****************************************************************************************
 */
static void osapp_bxotas_start_req_ind_handler(ke_msg_id_t const msgid,struct bxotas_start_req_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"OTA START:%d,%d\n",param->conidx,param->segment_data_max_length);
    //handle
    segment_data_max_length = param->segment_data_max_length;
    //erase ota flash : erase 256KB
    flash_erase(USEROTA_IN_FLASH_OFFSET + 0x00000, Block_64KB_Erase);
    flash_erase(USEROTA_IN_FLASH_OFFSET + 0x10000, Block_64KB_Erase);
    flash_erase(USEROTA_IN_FLASH_OFFSET + 0x20000, Block_64KB_Erase);
    flash_erase(USEROTA_IN_FLASH_OFFSET + 0x30000, Block_64KB_Erase);
    LOG(LOG_LVL_INFO,"OTA FLASH ERASE DONE!\n");
    //confirm
    osapp_bxotas_start_cfm(OTA_REQ_ACCEPTED);
}

static void osapp_bxotas_new_block_ind_handler(ke_msg_id_t const msgid,struct bxotas_new_block_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"OTA_new_block\n");
    current_block_id = param->block_id;
}

static void osapp_bxotas_data_rx_ind_handler(ke_msg_id_t const msgid,struct bxotas_data_rx_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    param_length = param->data_length;
//    LOG(LOG_LVL_INFO,"OTA DAT,len=%d,segid=%d\n",param_length,param->segment_id);
    program_offset = (current_block_id * MCU_OTA_SEGEMENTS_PER_BLOCK + param->segment_id) * segment_data_max_length;
    //flash program
    flash_program(USEROTA_IN_FLASH_OFFSET + program_offset , param_length , (void*)param->data);
    //total size calc
    ota_image_size = program_offset + param_length;
}

static void osapp_bxotas_finish_ind_handler(ke_msg_id_t const msgid,void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"OTA_DONE\n");
    LOG(LOG_LVL_INFO,"IMAGE SIZE = %d\n",ota_image_size);
    mcu_state_change(MCU_OTA_DONE);
}


static const osapp_msg_handler_table_t handler_table_ota[]=
{
        [0] =   {BXOTAS_START_REQ_IND,  (osapp_msg_handler_t)osapp_bxotas_start_req_ind_handler},
                {BXOTAS_NEW_BLOCK_IND,  (osapp_msg_handler_t)osapp_bxotas_new_block_ind_handler},
                {BXOTAS_DATA_RX_IND,    (osapp_msg_handler_t)osapp_bxotas_data_rx_ind_handler},
                {BXOTAS_FINISH_IND,     (osapp_msg_handler_t)osapp_bxotas_finish_ind_handler},
};

const osapp_msg_handler_info_t handler_info_ota = ARRAY_INFO(handler_table_ota);



