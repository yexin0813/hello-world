/**
 ****************************************************************************************
 *
 * @file ble_central.h
 *
 * @brief BLE central Demo Config Internal.
 *
 * Copyright (C) Apollo 2018-2023
 *
 ****************************************************************************************
 */
#ifndef FREERTOS_APP_BLE_CENTRAL_BLE_CENTRAL_H_
#define FREERTOS_APP_BLE_CENTRAL_BLE_CENTRAL_H_

/**
 ****************************************************************************************
 * @addtogroup BLE_CENTRAL  BLE Central Demo
 * @ingroup BLE_DEMO
 * @brief defines for BLE central demo
 *
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "ble_central_config.h"

/*
 * DEFINES
 ****************************************************************************************
 */
#ifdef BLE_CENTRAL_SIMPLE
/*
 * INTERNAL API TYPES
 ****************************************************************************************
 */

/**
 * @brief struct to connect information
 */
struct conn_info_t
{
    /// valid or not
    uint8_t valid;
    /// store connect index
    uint16_t conn_idx;
    /// store the master address
    bd_addr_t device_addr;
};

/*
 * MACROS
 ****************************************************************************************
 */
/// Macro used to retrieve field
/*
 * ENUMERATIONS
 ****************************************************************************************
 */
/// BX Central Service Table
enum
{
    PRF_BX_SIMPLES_SERVICE_ID,


    BLE_CENTRAL_SERVICES_NUM,
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/// BX Central is Connect
enum
{
    IS_DISCONNECTED = 0,
    IS_CONNECTED,
};
/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/// GATT Structure for profile callback functions
typedef void (*pfnGATTConnect_t)(uint8_t is_connect);
typedef void (*pfnGATTHandlerInit_t)(uint16_t handler,uint16_t task_id,uint16_t prf_nb);

typedef struct
{
  pfnGATTConnect_t pfnConnectCB;    //!< connect callback function pointer
  pfnGATTHandlerInit_t pfnHandlerInitCB;    //!< init set start handler
} gattServicePrfCBs_t;

/// Information about ble Central add service func handler
typedef int32_t (*prf_add_svc_handler_t)(gattServicePrfCBs_t *cb);

/// Information about ble Central service environmental variable
typedef struct
{
    /// max svc num
    uint8_t  max_num;
    /// now svc index
    uint8_t  index;
    ///service start att handler
    uint16_t start_hdl[BLE_CENTRAL_SERVICES_NUM];
    ///service callback functions
    gattServicePrfCBs_t gattcbs[BLE_CENTRAL_SERVICES_NUM];
    ///add svc func handler
    prf_add_svc_handler_t handles[BLE_CENTRAL_SERVICES_NUM];

}ble_profiles_client_env_t;
 /// Information about ble peripheral simple environmental variable
 typedef struct
 {
     /// profiles client environmental variable
     ble_profiles_client_env_t  svc;
     ///store connnect information
     struct conn_info_t conn_info;
 }ble_central_env_t;
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

/// @} BLE_CENTRAL

#endif /* BLE_CENTRAL_SIMPLE */
#endif /* FREERTOS_APP_BLE_CENTRAL_BLE_CENTRAL_H_ */
