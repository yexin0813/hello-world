/**
 *
 * @file osapp_template.c
 *
 * @brief A template for application.
 *
 * Copyright (C) Apollo 2015-2016
 *
 */

#include "osapp_config.h"
#ifdef OSAPP_TEMPLATE

/**
 * @brief callback function when app_task receive a message which can not found in handler_table. Normally, we should handle all the message and
 * take seriously when we found a default message.
 */
static void osapp_default_msg_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"default handler,msgid: 0x%x, dest_id: 0x%x, src_id: 0x%x",msgid,dest_id,src_id);

}



/**
 * @brief handler table to define the relations between message and it's callback.
 */
static const osapp_msg_handler_table_t handler_table[]=
{
        [0] =   {KE_MSG_DEFAULT_HANDLER,osapp_default_msg_handler},
//           {GAPC_CONNECTION_REQ_IND,osapp_gapc_conn_req_ind_handler},
//           {GAPC_DISCONNECT_IND,       osapp_gapc_disconnect_ind_handler},
//           {GAPM_PROFILE_ADDED_IND,osapp_gapm_profile_added_ind_handler},
//           {GAPM_CMP_EVT,osapp_gapm_cmp_evt_handler},
//           {GAPM_DEVICE_READY_IND,osapp_device_ready_ind_handler},
            
};
const osapp_msg_handler_info_t handler_info = ARRAY_INFO(handler_table);

/**
 * @brief add task needed to run in application exclude ble_task and app_task which have define in task_init.c
 */
static const task_table_t task_table[] ={
        {true,"Template",5,20,NULL,NULL,0},
};
const task_table_info_t extra_task_table_info = ARRAY_INFO(task_table);


#endif


