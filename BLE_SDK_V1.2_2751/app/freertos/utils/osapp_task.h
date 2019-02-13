#ifndef OSAPP_TASK_H_
#define OSAPP_TASK_H_
#include "FreeRTOS.h"
#include "task.h"
#include "ke_task.h"
#include "ahi.h"
#include "co_bt.h"
#include "osmsg_queue.h"
#define USR_DEF_MSG_TYPE 0x06
#define MESH_QUEUED_MSG_TYPE 0x07
extern TaskHandle_t handler_osapp_task;
void osapp_task(void *params);
void *ahi_msg_alloc(ke_msg_id_t const id, ke_task_id_t const dest_id, uint16_t const param_len);
/* You should call this ahi_msg_free() API only if you do not send your msg to ble stack and won't send it any more.*/
void ahi_msg_free(void *ptr);
void *usr_msg_alloc(uint16_t length);
void msg_send_to_ble_task(AHI_MSGBOX_t *msg_box,uint32_t xTicksToWait);
void msg_send_to_ble_task_isr(AHI_MSGBOX_t *msg_box);

void osapp_ahi_msg_send(void *param_ptr,uint16_t param_length,uint32_t xTicksToWait);
void osapp_ahi_msg_send_isr(void *param_ptr,uint16_t param_length);
int32_t osapp_msg_build_send(void *param_ptr,uint16_t param_length);

void *hci_cmd_alloc(uint16_t ocf,uint8_t ogf,uint16_t param_len);
void *hci_data_alloc(uint16_t handle,uint8_t pb_flag,uint8_t bc_flag,uint16_t data_len);
void osapp_hci_cmd_send(void * param_ptr, uint16_t param_length, uint32_t xTicksToWait);
void osapp_hci_cmd_send_isr(void * param_ptr, uint16_t param_length);
void osapp_hci_data_send(void * param_ptr, uint16_t param_length, uint32_t xTicksToWait);
void osapp_hci_data_send_isr(void * param_ptr, uint16_t param_length);
void osapp_usr_msg_send(uint8_t indicator,void *param_ptr,uint16_t param_length,uint32_t xTicksToWait);
void osapp_usr_msg_send_isr(uint8_t indicator,void *param_ptr,uint16_t param_length);


__INLINE AHI_MSG_t * ahi_param2msg(void const *param_ptr)
{
    return (AHI_MSG_t*) (((uint8_t*)param_ptr) - offsetof(AHI_MSG_t, param));
}

__INLINE void * ahi_msg2param(AHI_MSG_t const *msg)
{
    return (void*) (((uint8_t*) msg) + offsetof(AHI_MSG_t, param));
}

__INLINE hci_cmd_t *hci_param2cmd(void const *param_ptr)
{
    return (hci_cmd_t *)((uint8_t *)param_ptr - offsetof(hci_cmd_t,param));
}

__INLINE void *hci_cmd2param(hci_cmd_t const *cmd)
{
    return (void *)((uint8_t *)cmd + offsetof(hci_cmd_t,param));
}

__INLINE hci_data_t *hci_param2data(void const *param_ptr)
{
    return (hci_data_t *)((uint8_t *)param_ptr - offsetof(hci_data_t,param));
}

__INLINE void *hci_data2param(hci_data_t const *data)
{
    return (void *)((uint8_t *)data + offsetof(hci_data_t,param));
}






#define AHI_MSG_ALLOC(id, dest,param_str) \
    (struct param_str*) ahi_msg_alloc(id, dest, sizeof(struct param_str))
    
#define AHI_MSG_ALLOC_DYN(id, dest, param_str,length)  (struct param_str*)ahi_msg_alloc(id, dest, \
    (sizeof(struct param_str) + length));



#endif

