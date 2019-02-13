/**
 ****************************************************************************************
 *
 * @file ble_central_config.h
 *
 * @brief BLE Central Demo Config Internal.
 *
 * Copyright (C) Apollo 2018-2023
 *
 ****************************************************************************************
 */
#ifndef FREERTOS_APP_BLE_CENTRAL_BLE_CENTRAL_CONFIG_H_
#define FREERTOS_APP_BLE_CENTRAL_BLE_CENTRAL_CONFIG_H_

/**
 ****************************************************************************************
 * @addtogroup BLE_CENTRAL_CONFIG  BLE Peripheral Demo Config Internal
 * @ingroup BLE_CENTRAL
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
#ifdef OSAPP_BLE_CENTRAL



#define BLE_CENTRAL_SIMPLE    /**<An example for APOLLO_00 using as a ble peripheral. Only one BLE_CENTRAL_xx macro can be defined. */
#define BLE_APOLLO_00_SIMPLE_CLIENT /**<An example for APOLLO_00 using as a ble apollo_00 simple client.*/




#endif /* OSAPP_BLE_CENTRAL */
/// @} BLE_CENTRAL_CONFIG
#endif /* FREERTOS_APP_BLE_CENTRAL_BLE_CENTRAL_CONFIG_H_ */
