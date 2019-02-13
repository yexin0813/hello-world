#include "osapp_config.h"
#include "osapp_task.h"
#include "bx_dbg.h"
#include "log.h"
 
extern const osapp_msg_handler_info_t handler_info;
TaskHandle_t handler_osapp_task;
static struct
{
    msg_queue_t *queue_ptr;
}osapp_task_env;
void *ahi_msg_alloc(ke_msg_id_t const id, ke_task_id_t const dest_id, uint16_t const param_len)
{
    AHI_MSG_t *msg = (AHI_MSG_t*) pvPortMalloc(sizeof(AHI_MSG_t) +
                                                    param_len - sizeof (uint32_t));
    void *param_ptr = NULL;

    BX_ASSERT(msg != NULL);

    msg->id        = id;
    msg->dest_id   = dest_id;
    msg->src_id    = TASK_ID_AHI;                   //ahi task
    msg->param_len = param_len;

    param_ptr = ahi_msg2param(msg);

    memset(param_ptr, 0, param_len);

    return param_ptr;
}

void ahi_msg_free(void *ptr)
{
    vPortFree(ahi_param2msg(ptr));
}

void *usr_msg_alloc(uint16_t length)
{
    return pvPortMalloc(length);
}

void *hci_cmd_alloc(uint16_t ocf,uint8_t ogf,uint16_t param_len)
{
    hci_cmd_t *cmd = pvPortMalloc(param_len+offsetof(hci_cmd_t,param));
    if(NULL == cmd)
    {
        return NULL;
    }
    cmd->opcode = HCI_OPCODE(ocf,ogf);
    cmd->param_len = param_len;
    return cmd->param;
}

void *hci_data_alloc(uint16_t handle,uint8_t pb_flag,uint8_t bc_flag,uint16_t data_len)
{
    hci_data_t *data = pvPortMalloc(data_len + offsetof(hci_data_t,param));
    if(NULL == data)
    {
        return NULL;
    }
    data->handle_flag = bc_flag<<14 | pb_flag<<12 | handle;
    data->data_len = data_len;
    return data->param;
}

static void msg_send(QueueHandle_t q,AHI_MSGBOX_t *msg_box,uint32_t xTicksToWait)
{
    if(xQueueSend(q,msg_box,xTicksToWait)!=pdPASS)
    {
        BX_ASSERT(0);
    }
}

static void msg_send_isr(QueueHandle_t q,AHI_MSGBOX_t *msg_box)
{
    BaseType_t ret = xQueueSendFromISR(q,msg_box,NULL);
    if(ret!=pdPASS)
    {
        BX_ASSERT(0);
    }
    portYIELD_FROM_ISR(pdTRUE);
}

void msg_send_to_ble_task(AHI_MSGBOX_t *msg_box,uint32_t xTicksToWait)
{
    msg_send(osapp_task_env.queue_ptr->cmd_q,msg_box,xTicksToWait);
}

void msg_send_to_ble_task_isr(AHI_MSGBOX_t *msg_box)
{
    msg_send_isr(osapp_task_env.queue_ptr->cmd_q,msg_box);

}

void osapp_ahi_msg_send(void *param_ptr,uint16_t param_length,uint32_t xTicksToWait)
{
    AHI_MSGBOX_t msg_box ={
        .pmsg = ahi_param2msg(param_ptr),
        .len = param_length + sizeof(AHI_MSG_t) - sizeof(uint32_t),
        .indicator = AHI_KE_MSG_TYPE,
    };
    msg_send_to_ble_task(&msg_box,xTicksToWait);
}

void osapp_ahi_msg_send_isr(void * param_ptr, uint16_t param_length)
{
    AHI_MSGBOX_t msg_box ={
        .pmsg = ahi_param2msg(param_ptr),
        .len = param_length + sizeof(AHI_MSG_t) - sizeof(uint32_t),
        .indicator = AHI_KE_MSG_TYPE,
    };
    msg_send_to_ble_task_isr(&msg_box);
}

int32_t osapp_msg_build_send(void *param_ptr,uint16_t param_length)
{
    osapp_ahi_msg_send(param_ptr,param_length,portMAX_DELAY);
    return pdTRUE;
}

void osapp_hci_cmd_send(void *param_ptr,uint16_t param_length,uint32_t xTicksToWait)
{
    AHI_MSGBOX_t msg_box ={
        .pmsg = hci_param2cmd(param_ptr),
        .len = param_length + offsetof(hci_cmd_t,param),
        .indicator = HCI_CMD_MSG_TYPE,
    };
    msg_send_to_ble_task(&msg_box,xTicksToWait);
}

void osapp_hci_cmd_send_isr(void *param_ptr,uint16_t param_length)
{
    AHI_MSGBOX_t msg_box ={
        .pmsg = hci_param2cmd(param_ptr),
        .len = param_length + offsetof(hci_cmd_t,param),
        .indicator = HCI_CMD_MSG_TYPE,
    };
    msg_send_to_ble_task_isr(&msg_box);
}

void osapp_hci_data_send(void *param_ptr,uint16_t param_length,uint32_t xTicksToWait)
{
    AHI_MSGBOX_t msg_box ={
        .pmsg = hci_param2data(param_ptr),
        .len = param_length + offsetof(hci_data_t,param),
        .indicator = HCI_ACL_MSG_TYPE,
    };
    msg_send_to_ble_task(&msg_box,xTicksToWait);
}

void osapp_hci_data_send_isr(void *param_ptr,uint16_t param_length)
{
    AHI_MSGBOX_t msg_box ={
        .pmsg = hci_param2data(param_ptr),
        .len = param_length + offsetof(hci_data_t,param),
        .indicator = HCI_ACL_MSG_TYPE,
    };
    msg_send_to_ble_task_isr(&msg_box);
}

void osapp_usr_msg_send(uint8_t indicator,void *param_ptr,uint16_t param_length,uint32_t xTicksToWait)
{
    AHI_MSGBOX_t msg_box ={
        .pmsg = param_ptr,
        .len = param_length,
        .indicator = indicator,
    };
    msg_send(osapp_task_env.queue_ptr->rsp_q,&msg_box,xTicksToWait);
}

void osapp_usr_msg_send_isr(uint8_t indicator,void *param_ptr,uint16_t param_length)
{
    AHI_MSGBOX_t msg_box ={
        .pmsg = param_ptr,
        .len = param_length,
        .indicator = indicator,
    };
    msg_send_isr(osapp_task_env.queue_ptr->rsp_q,&msg_box);
}

static void osapp_ahi_msg_rx(AHI_MSG_t *msg)
{
    uint16_t i;
    bool specific_handler = false;
    for(i=1;i<handler_info.table_size;++i)
    {
        if(msg->id==handler_info.handler_table[i].id)
        {
            specific_handler = true;
            handler_info.handler_table[i].func(msg->id,msg->param,msg->dest_id,msg->src_id);
            break;
        }
    }
    if(specific_handler==false)
    {
        handler_info.handler_table[0].func(msg->id,msg->param,msg->dest_id,msg->src_id);
    }
}

static void osapp_hci_evt_rx(hci_event_t *evt)
{

}

static void osapp_hci_data_rx(hci_data_t *data)
{

}


__attribute__((weak)) void osapp_usr_msg_handler(uint8_t *data,uint16_t length)
{
    LOG(LOG_LVL_INFO,"weak definition usr_msg_handler\n");
}

__attribute__((weak)) void osapp_mesh_queued_msg_handler(uint8_t *data,uint16_t length)
{
    LOG(LOG_LVL_INFO,"weak definition mesh_queued_msg_handler\n");
}


void osapp_task(void *params)
{
    osapp_task_env.queue_ptr = params;
    while(1)
    {
        AHI_MSGBOX_t msgBox;
        if(xQueueReceive(osapp_task_env.queue_ptr->rsp_q,&msgBox,portMAX_DELAY)!=pdPASS)
        {
            BX_ASSERT(0);
        }
        switch (msgBox.indicator)
        {
        case AHI_KE_MSG_TYPE:
            osapp_ahi_msg_rx(msgBox.pmsg);
            break;
        case HCI_EVT_MSG_TYPE:
            osapp_hci_evt_rx(msgBox.pmsg);
            break;
        case HCI_ACL_MSG_TYPE:
            osapp_hci_data_rx(msgBox.pmsg);
            break;
        case USR_DEF_MSG_TYPE:
            osapp_usr_msg_handler(msgBox.pmsg,msgBox.len);
            break;
        case MESH_QUEUED_MSG_TYPE:
            osapp_mesh_queued_msg_handler(msgBox.pmsg,msgBox.len);
            break;
        default:
            LOG(LOG_LVL_WARN,"osapp packet rx invalid indicator\n");
            break;
        }
//            LOG(LOG_LVL_INFO,"osapp msg received\n");
        vPortFree(msgBox.pmsg);
    }

}
