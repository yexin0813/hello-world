#ifndef OSMSG_QUEUE_H_
#define OSMSG_QUEUE_H_
#include "FreeRTOS.h"
#include "queue.h"
#include "ke_msg.h"
typedef struct
{
    ///Message id
    ke_msg_id_t  id;
    ///Destination task identifier for KE
    ke_task_id_t dest_id;
    ///Source task identifier for KE
    ke_task_id_t src_id;
    ///Message parameter length
    uint16_t     param_len;
    
    uint32_t     param[1];
}AHI_MSG_t;
typedef struct{
    uint16_t opcode;
    uint8_t param_len;
    uint8_t param[];
}hci_cmd_t;
typedef struct{
    uint16_t handle_flag;
    uint16_t data_len;
    uint8_t param[];
}hci_data_t;
typedef struct{
    uint8_t evt_code;
    uint8_t param_len;
    uint8_t param[];
}hci_event_t;

typedef struct
{
    void *pmsg;
    uint16_t len;
    uint8_t indicator;
}AHI_MSGBOX_t;
typedef struct{
    QueueHandle_t cmd_q;    /**< BLE Manager command queue */
    QueueHandle_t rsp_q;    /**< BLE Manager response queue */
}msg_queue_t;

#endif

