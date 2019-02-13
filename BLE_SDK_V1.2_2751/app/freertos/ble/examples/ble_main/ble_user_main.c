/**
 ****************************************************************************************
 *
 * @file ble_user_main.h
 *
 * @brief BLE Demo user init .
 *
 * Copyright (C) Apollo 2018-2023
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup BLE_DEMO_MAIN  BLE Demo Simple Data init
 * @ingroup BLE_DEMO
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "ble_demo_config.h"

#ifdef OSAPP_BLE_DEMO


#ifdef OSAPP_BLE_CENTRAL
#include "ble_central.h"
#endif /* OSAPP_BLE_CENTRAL */

#ifdef OSAPP_BLE_PERIPNERAL
#include "ble_apollo_00_simple_s.h"
#endif /* OSAPP_BLE_PERIPNERAL */

#ifdef OSAPP_BLE_BROADCASTER
#include "ble_broadcaster.h"
#endif /* OSAPP_BLE_BROADCASTER */

#ifdef OSAPP_BLE_OBSERVER
#include "ble_observer.h"
#endif /* OSAPP_BLE_OBSERVER */

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */



/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * MACROS
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*
 * LOCAL FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */

/*
 * MESSAGES HANDLERS DEFINITIONS
 ****************************************************************************************
 */


/**
 * @brief user data init set.
 *
 */
void osapp_user_main_init(void)
{
    ///user data init
    {
        //do ...

    }
    ///ble stack run
    osapp_ble_stack_run();

    LOG(LOG_LVL_INFO,"ble user run\n");
}



/// @} BLE_DEMO_MAIN

#endif/* OSAPP_BLE_DEMO */
