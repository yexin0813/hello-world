/**
 *
 * @file ble_task.c
 *
 * @brief This file mainly packed ble system into a freertos task.
 *
 * Copyright (C) Apollo 2015-2016
 *
 */

#include "ble_task.h"
#include "sys_sleep.h"
#include "os_bridge.h"
#include "log.h"

/// the handler of ble task which can be use when create ble task.
TaskHandle_t handler_ble_stack_task;
extern uint32_t error;
static struct{
    msg_queue_t *queue_ptr;
}ble_stack_env;

/**
 * @brief send message to app_task.
 * @param msgBox: a struct including pointer to the data and data length.
 */
void ble_msg_2_app(AHI_MSGBOX_t *msgBox)
{
    if(xQueueSend(ble_stack_env.queue_ptr->rsp_q,msgBox, portMAX_DELAY)!=pdPASS)
    {
        BX_ASSERT(0);
    }
}

/**
 * @brief Ble task. Must exist in Apollo bluetooth system.
 * @param params: pointer to the input parameter for ble task.
 */
void ble_stack_task(void *params)
{
    AHI_MSGBOX_t Msg;
    ble_stack_env.queue_ptr = params;
    rwip_init(error);
    while(1)
    {
        ble_schedule();
        if(xQueueReceive(ble_stack_env.queue_ptr->cmd_q, &Msg, portMAX_DELAY)==pdTRUE)
        {
            if(Msg.len!=0)                                    // msg from AHI
            {
                osapp_msg_start_recv(Msg.pmsg, Msg.len,Msg.indicator);
            }
        }    
    }
}

/**
 * @brief when ble exit isr, ble_schedule() must run again because something may be done when interrupt happened.
 * We Send a message from isr to ble_task in order to run ble_schedule.
 */
void msg2blestack_from_ISR()
{
    AHI_MSGBOX_t msgBox= 
    {
        .len = 0,
        .pmsg = NULL
    };
    /// doesn't matter it return false(only when the queue is full). We send queue only to prevent there are no message in the queue.
    /// When there are no message in the queue ble_schedule will not be called.
    if(xQueueIsQueueEmptyFromISR(ble_stack_env.queue_ptr->cmd_q))
    {
        xQueueSendFromISR(ble_stack_env.queue_ptr->cmd_q, &msgBox, NULL);
    }
    portYIELD_FROM_ISR(pdTRUE);
}

