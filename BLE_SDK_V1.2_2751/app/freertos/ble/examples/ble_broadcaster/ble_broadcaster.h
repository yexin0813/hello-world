/**
 ****************************************************************************************
 *
 * @file ble_broadcaster.h
 *
 * @brief BLE broadcaster Demo Config Internal.
 *
 * Copyright (C) Apollo 2018-2023
 *
 ****************************************************************************************
 */
#ifndef FREERTOS_APP_BLE_BROADCASTER_BLE_BROADCASTER_H_
#define FREERTOS_APP_BLE_BROADCASTER_BLE_BROADCASTER_H_

/**
 ****************************************************************************************
 * @addtogroup BLE_BROADCASTER  BLE Broadcaster Demo
 * @ingroup BLE_DEMO
 * @brief defines for BLE broadcaster demo
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
#ifdef OSAPP_BLE_BROADCASTER
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
/// Information about ble broadcaster environmental variable
typedef struct
{
    ///update adv data information
    TimerHandle_t Timer;
    ///update num in adv data
    uint8_t test_num;
}ble_broadcaster_env_t;

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

/// @} BLE_BROADCASTER

#endif /* OSAPP_BLE_BROADCASTER */
#endif /* FREERTOS_APP_BLE_BROADCASTER_BLE_BROADCASTER_H_ */
