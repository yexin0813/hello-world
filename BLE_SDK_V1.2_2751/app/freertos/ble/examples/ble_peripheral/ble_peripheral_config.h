/**
 ****************************************************************************************
 *
 * @file ble_peripheral_config.h
 *
 * @brief BLE Peripheral Demo Config Internal.
 *
 * Copyright (C) Apollo 2018-2023
 *
 ****************************************************************************************
 */

#ifndef FREERTOS_APP_BLE_PERIPHERAL_BLE_PERIPHERAL_CONFIG_H_
#define FREERTOS_APP_BLE_PERIPHERAL_BLE_PERIPHERAL_CONFIG_H_

/**
 ****************************************************************************************
 * @addtogroup BLE_PERIPHERAL_CONFIG  BLE Peripheral Demo Config Internal
 * @ingroup BLE_PERIPHERAL
 * @brief defines for BLE peripheral demo config
 *
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "ble_demo_config.h"


/*
 * DEFINES
 ****************************************************************************************
 */
#ifdef OSAPP_BLE_PERIPNERAL



#define BLE_PERIPHERAL_SIMPLE    /**<An example for APOLLO_00 using as a ble peripheral. Only one BLE_PERIPHERAL_xx macro can be defined. */
#define BLE_APOLLO_00_SIMPLE_SERVER /**<An example for APOLLO_00 using as a ble apollo_00 simple server.*/




#endif /* OSAPP_BLE_PERIPNERAL */
/// @} BLE_PERIPHERAL_CONFIG
#endif /* FREERTOS_APP_BLE_PERIPHERAL_BLE_PERIPHERAL_CONFIG_H_ */
