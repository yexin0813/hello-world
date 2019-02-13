/**
 ****************************************************************************************
 *
 * @file   mcu_ahi.h
 *
 * @brief  .
 *
 * @author  jiachuang
 * @date    2018-10-25 16:31
 * @version <0.0.0.1>
 *
 * @license 
 *              Copyright (C) APOLLO Microelectronics 2018
 *                         ALL Right Reserved.
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup MESH_mcu_ahi_API Mesh mcu_ahi API
 * @ingroup MESH_API
 * @brief Mesh mcu_ahi  API
 *
 * @{
 ****************************************************************************************
 */

#ifndef PLF_SRC_ARCH_MAIN_MCU_DEMO_MCU_AHI_H_
#define PLF_SRC_ARCH_MAIN_MCU_DEMO_MCU_AHI_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "stdint.h"
#include "gapm_task.h"
#include "gapc_task.h"
#include "gattc_task.h"
#include "ke_msg.h"
#include "string.h"
#include "log.h"
#include <stdlib.h>
#include "rwble_config.h"
#include "co_bt_defines.h"
#include "nvds_typedef.h"
#include "bx_ring_queue.h"

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
//void send_ahi_message(uint16_t msg , uint8_t operation);
void mcu_ahi_msg_head_rx(void);
uint8_t *adv_data_pack(uint8_t *buf,uint8_t field_nums,...);
void *ahi_msg_alloc(ke_msg_id_t const id, ke_task_id_t const dest_id, uint16_t const param_len);
int32_t osapp_msg_build_send(void *param_ptr,uint16_t param_length);
void osapp_ahi_msg_send(void *param_ptr,uint16_t param_length,uint32_t xTicksToWait);
void mcu_log_hex_data(const uint8_t * data , uint8_t length);
/*
 * MACROS
 ****************************************************************************************
 */
/* Default Message handler code to handle several message type in same handler. */
#define KE_MSG_DEFAULT_HANDLER  (0xFFFF)
#define portMAX_DELAY 0xffff
#define ADV_DATA_PACK(buf,field_nums,...) (adv_data_pack((buf),(field_nums),__VA_ARGS__) - (buf))
#define AHI_MSG_ALLOC(id, dest,param_str)              (struct param_str*) ahi_msg_alloc(id, dest, sizeof(struct param_str))
#define AHI_MSG_ALLOC_DYN(id, dest, param_str,length)  (struct param_str*)ahi_msg_alloc(id, dest,  (sizeof(struct param_str) + length));
#define ARRAY_INFO(hdl) {hdl,sizeof(hdl)/sizeof(hdl[0])}

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct
{
    //reversed
    uint16_t    reverse1;//alien to 4 byte offset.
    uint8_t     reverse2;
    //Message Flag
    uint8_t     flag;
    ///Message id
    uint16_t    id;
    ///Destination task identifier for KE
    uint16_t    dest_id;
    ///Source task identifier for KE
    uint16_t    src_id;
    ///Message parameter length
    uint16_t     param_len;
}ahi_msg_t;

//tx queue
typedef struct
{
    void *param_ptr;
    uint16_t param_length;
}mcu_ahi_queue_t;

typedef enum
{
    MCU_AHI_TX_STAT_IDLE,
    MCU_AHI_TX_STAT_BUSY,
}mcu_ahi_tx_stat_t;




typedef void (*osapp_msg_handler_t)(ke_msg_id_t const msgid,
                            void const *param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id);
/**
 * @brief structure to define message and it's callback
 */
typedef struct
{
    ke_msg_id_t id;
    osapp_msg_handler_t func;
}osapp_msg_handler_table_t;

/**
 * @brief osapp_task use this structure to find callback for a message.
 */
typedef struct
{
    const osapp_msg_handler_table_t *  handler_table;
    uint8_t table_size;
}osapp_msg_handler_info_t;

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */




#endif /*  */
/// @} MESH_mcu_ahi_API

