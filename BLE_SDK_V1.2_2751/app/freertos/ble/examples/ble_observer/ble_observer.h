/**
 ****************************************************************************************
 *
 * @file ble_observer.h
 *
 * @brief BLE observer Demo Config Internal.
 *
 * Copyright (C) Apollo 2018-2023
 *
 ****************************************************************************************
 */
#ifndef FREERTOS_APP_BLE_OBSERVER_BLE_OBSERVER_H_
#define FREERTOS_APP_BLE_OBSERVER_BLE_OBSERVER_H_

/**
 ****************************************************************************************
 * @addtogroup BLE_OBSERVER  BLE Observer Demo
 * @ingroup BLE_DEMO
 * @brief defines for BLE observer demo
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
#ifdef OSAPP_BLE_OBSERVER
/*
 * INTERNAL API TYPES
 ****************************************************************************************
 */

/*
 * MACROS
 ****************************************************************************************
 */
/// Macro used to retrieve field

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/**
 * @brief ble stack start run.
 *
 */
extern void osapp_ble_stack_run(void);

/// @} BLE_OBSERVER

#endif /* OSAPP_BLE_OBSERVER */
#endif /* FREERTOS_APP_BLE_OBSERVER_BLE_OBSERVER_H_ */
