/**
 *
 * @file ble_task.h
 *
 * @brief The header file of ble_task.c
 *
 * Copyright (C) Apollo 2015-2016
 *
 */

#ifndef BLE_TASK_H_
#define BLE_TASK_H_
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "osmsg_queue.h"

/// the handler of ble task which can be use when create ble task.
extern TaskHandle_t handler_ble_stack_task;

/**
 * @brief send message to app_task.
 * @param msgBox: a struct including pointer to the data and data length.
 */
void ble_msg_2_app(AHI_MSGBOX_t *rsp);
/**
 * @brief Ble task. Must exist in Apollo bluetooth system.
 * @param params: pointer to the input parameter for ble task.
 */
void ble_stack_task(void *params);
/**
 * @brief when ble exit isr, ble_schedule() must run again because something may be down when interrupt happened.
 * We Send a message from isr to ble_task in order to run ble_schedule.
 */
void msg2blestack_from_ISR(void);
#endif
