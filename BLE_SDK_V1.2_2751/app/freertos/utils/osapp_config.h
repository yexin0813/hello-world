
/**
 *
 * @file osapp_config.h
 *
 * @brief Define some general structure and macro for os application
 *
 * Copyright (C) Apollo 2015-2016
 *
 */

#ifndef OSAPP_CONFIG_H_
#define OSAPP_CONFIG_H_
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#define __RAM_CODE__

#include "Freertos.h"
#include "task.h"
#include "event_groups.h"
#include "gapm_task.h"
#include "gapc_task.h"
#include "gattc_task.h"
#include "gattm_task.h"
#include "attm.h"
#include "bx_dbg.h"
#include "log.h"
#include "nvds_in_ram.h"
#include "osapp_task.h"
#include "co_bt.h"
#include "co_math.h"
#include "prf.h"
#include "osapp_utils.h"
#include "bx_app_config.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef void (*osapp_msg_handler_t)(ke_msg_id_t const msgid,
                            void const *param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id);
/**
 * @brief structure to define message and it's callback
 */
typedef struct
{
    ke_msg_id_t         id;
    osapp_msg_handler_t func;
}osapp_msg_handler_table_t;

/**
 * @brief osapp_task use this structure to find callback for a message.
 */
typedef struct
{
    const osapp_msg_handler_table_t *  handler_table;
    uint8_t                            table_size;
}osapp_msg_handler_info_t;
/**
 * @brief structure to define a freertos task.
 */
typedef struct
{
    bool                is_used;        /* if this task is in use */
    const char *const   task_name;      /* Task name,max length is 16 */
    uint8_t             priority;       /* Task priority */
    uint16_t            stack_size;     /* size of stack for this task */
    void *              param;          /* task argument */
    TaskFunction_t      entry_proc;     /* Task entry function */
    TaskHandle_t *      task_handle;
} task_table_t;
/**
 * @brief This structure help to initialize a freertos task.
 */
typedef struct
{
    const task_table_t *task_table;
    const uint8_t       table_size;
}task_table_info_t;

/*
 * MACROS
 ****************************************************************************************
 */

#define ARRAY_INFO(hdl)                         {hdl,sizeof(hdl)/sizeof(hdl[0])}

/*
 * DEFINES
 ****************************************************************************************
 */
///define stack size
#define BLE_TASK_STACK_SIZE                     128  // actual size = (XXX_TASK_STACK_SIZE * 4) Byte
#define APP_TASK_STACK_SIZE                     128

///define task priority
#define OS_PRIORITY_BLE_TASK                    (configMAX_PRIORITIES  - 1)        //5
#define OS_PRIORITY_APP_TASK                    (OS_PRIORITY_BLE_TASK - 1)          // 4

#endif/* OSAPP_CONFIG_H_ */

