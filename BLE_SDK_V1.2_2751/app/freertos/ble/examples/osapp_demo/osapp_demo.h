/**
 *
 * @file osapp_demo.h
 *
 * @brief header file of osapp_demo.c
 *
 * Copyright (C) Apollo 2015-2016
 *
 */

/**
 ****************************************************************************************
 * @addtogroup App Demo
 * @ingroup RTOS App
 * @brief Demo to show the capability of APOLLO_00.
 * @{
 ****************************************************************************************
 */
#ifndef OSAPP_DEMO_H

#define OSAPP_DEMO_H

/**
 * @brief This function normally call in another freertos task. For Example, dht_task need to send a temperature, this function will be called.
 * @param msg_id: defined by user to identify the message type.
 * @param param: use a pointer to input message data.
 */
void osapp_taskmsg_handler(uint16_t msg_id, void const *param);

/// stack size of mems task
#define MEMS_TASK_STACK_SIZE			256	
/// stack size of oled task
#define OLED_TASK_STACK_SIZE            256
/// stack size of dht task
#define DHT_TASK_STACK_SIZE             256

/// define task priority of DHT_TASK. Should be careful some priority are defined in task_init.h
#define OS_PRIORITY_DHT_TASK			(tskIDLE_PRIORITY + configMAX_PRIORITIES  - 3)      // 3
/// define task priority of MEMS_TASK. Should be careful some priority are defined in task_init.h
#define OS_PRIORITY_MEMS_TASK           (tskIDLE_PRIORITY + configMAX_PRIORITIES  - 4)		// 2
/// define task priority of OLED_TASK. Should be careful some priority are defined in task_init.h
#define OS_PRIORITY_OLED_TASK           (tskIDLE_PRIORITY + configMAX_PRIORITIES  - 5)		// 1

#endif

///@}
