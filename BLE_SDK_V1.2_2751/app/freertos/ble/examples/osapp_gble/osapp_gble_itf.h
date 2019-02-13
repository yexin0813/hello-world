/**
 *
 * @file osapp_gble_itf.h
 *
 * @brief header file of osapp_gble_itf.c
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

#ifndef GBLE_ITF_H
#define GBLE_ITF_H
#include "stdint.h"

/**
 * @brief This function indicate gble has received a command.User should decode the data according to the protocol.
 * @param data: pointer to the received data.
 * @param length: length is the length of data.
 * @return return 1 when decode complete and everything is expected.
 */
int32_t gble_receive_data_ind(uint8_t *data, uint8_t length);
/**
 * @brief encode and send data to master, long data should be cut into pieces no longer than 20 here
 * @param data: pointer to the received data.
 * @param length: length is the length of data.
 * @return return 1 when decode complete and everything is expected.
 */
int32_t gble_send_data_cmd(uint8_t *data, uint8_t length);
/**
 * @brief callback for dht_task. When temperature and humid data available, this function will be called and send data to master.
 * @param type: message type.
 * @param value: pointer to the received data.
 * @return return 1 when decode complete and everything is expected.
 */
int32_t gble_dht_data_avaliable(uint8_t type, uint8_t *value);
#endif

///@}

