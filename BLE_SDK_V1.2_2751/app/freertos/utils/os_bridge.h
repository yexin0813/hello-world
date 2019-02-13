
/**
 *
 * @file os_bridge.h
 *
 * @brief The header file of os_bridge.c. Handle the message between ble_task and app_task.
 *
 * Copyright (C) Apollo 2015-2016
 *
 */

#ifndef OS_BRIDGE_H_
#define OS_BRIDGE_H_

#include <stddef.h>          // standard definition

#include "rwip.h"

#include <stdint.h>
#include <string.h>
#include "ble_task.h"
/**
 * @brief interface for virtual_uart(handle the message between ble_task and app_task).
 */
const struct rwip_eif_api *os_get_eif_api(void);
/**
 * @brief When ble task receive a message, use this function to start read and decode the message.
 * @param pMsg: pointer to the message
 * @param len: length of the message.
 */
void osapp_msg_start_recv(void *pMsg, uint16_t len,uint8_t indicator);



#endif

