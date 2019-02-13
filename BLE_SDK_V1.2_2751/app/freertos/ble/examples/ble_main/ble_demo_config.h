/**
 ****************************************************************************************
 *
 * @file ble_demo_config.h
 *
 * @brief BLE Demo Config Internal.
 *
 * Copyright (C) Apollo 2018-2023
 *
 ****************************************************************************************
 */
#ifndef FREERTOS_APP_BLE_MAIN_BLE_DEMO_CONFIG_H_
#define FREERTOS_APP_BLE_MAIN_BLE_DEMO_CONFIG_H_
/**
 ****************************************************************************************
 * @addtogroup BLE_DEMO_CONFIG  BLE Demo Config Internal
 * @ingroup BLE_DEMO
 * @brief defines for BLE demo config
 *
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "osapp_config.h"
#ifdef OSAPP_BLE_DEMO

/*
 * DEFINES
 ****************************************************************************************
 */
#define OSAPP_BLE_PERIPNERAL    /**<An example for APOLLO_00 using as a ble peripheral. Only one OSAPP_XXX macro can be defined.  */
//#define OSAPP_BLE_CENTRAL        /**<An example for APOLLO_00 using as a ble central. Only one OSAPP_XXX macro can be defined.  */
//#define OSAPP_BLE_BROADCASTER    /**<An example for APOLLO_00 using as a ble broadcater. Only one OSAPP_XXX macro can be defined.  */
//#define OSAPP_BLE_OBSERVER        /**<An example for APOLLO_00 using as a ble observer. Only one OSAPP_XXX macro can be defined.  */


/// @} BLE_DEMO_CONFIG
#endif /* OSAPP_BLE_DEMO */
#endif /* FREERTOS_APP_BLE_MAIN_BLE_DEMO_CONFIG_H_ */
