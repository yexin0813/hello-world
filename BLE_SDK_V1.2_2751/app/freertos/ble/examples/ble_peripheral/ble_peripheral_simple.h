/**
 ****************************************************************************************
 *
 * @file ble_peripheral_simple.h
 *
 * @brief BLE Peripheral Demo Config Internal.
 *
 * Copyright (C) Apollo 2018-2023
 *
 ****************************************************************************************
 */
#ifndef FREERTOS_APP_BLE_PERIPHERAL_BLE_PERIPHERAL_SIMPLE_H_
#define FREERTOS_APP_BLE_PERIPHERAL_BLE_PERIPHERAL_SIMPLE_H_

/**
 ****************************************************************************************
 * @addtogroup BLE_PERIPHERAL_SIMPLE  BLE Peripheral Simple Demo
 * @ingroup BLE_PERIPHERAL
 * @brief defines for BLE peripheral simple demo
 *
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "ble_peripheral_config.h"

#ifdef BLE_PERIPHERAL_SIMPLE
/*
 * DEFINES
 ****************************************************************************************
 */

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
/// BX Peripheral Service Table
enum
{
    BX_SIMPLES_SERVICE_ID,


    BLE_PERIPHERAL_SERVICES_NUM,
};


/// BX Peripheral is Connect
enum
{
    IS_DISCONNECTED = 0,
    IS_CONNECTED,
};
/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/// GATT Structure for service callback functions
typedef void (*pfnGATTReadAttrCB_t)(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
typedef void (*pfnGATTWriteAttrCB_t)(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
typedef void (*pfnGATTConnect_t)(uint8_t is_connect);
typedef void (*pfnGATTHandlerInit_t)(uint16_t handler);

typedef struct
{
  pfnGATTReadAttrCB_t pfnReadAttrCB;           //!< Read callback function pointer
  pfnGATTWriteAttrCB_t pfnWriteAttrCB;         //!< Write callback function pointer
  pfnGATTConnect_t pfnConnectCB;    //!< connect callback function pointer
  pfnGATTHandlerInit_t pfnHandlerInitCB;    //!< init set start handler
} gattServiceCBs_t;

/// Information about ble peripheral add service func handler
typedef int32_t (*add_svc_handler_t)(gattServiceCBs_t *cb);

/// Information about ble peripheral service environmental variable
typedef struct
{
    /// max svc num
    uint8_t  max_num;
    /// now svc index
    uint8_t  index;
    ///service start att handler
    uint16_t start_hdl[BLE_PERIPHERAL_SERVICES_NUM];
    ///service callback functions
    gattServiceCBs_t gattcbs[BLE_PERIPHERAL_SERVICES_NUM];
    ///add svc func handler
    add_svc_handler_t handles[BLE_PERIPHERAL_SERVICES_NUM];

}ble_peripheral_service_env_t;
/// Information about ble peripheral simple environmental variable
typedef struct
{
    /// peripheral service environmental variable
    ble_peripheral_service_env_t  svc;
    ///store connnect information
    struct conn_info_t conn_info;
}ble_peripheral_simple_env_t;


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

/// @} BLE_PERIPHERAL_SIMPLE

#endif /* BLE_PERIPHERAL_SIMPLE */
#endif /* FREERTOS_APP_BLE_PERIPHERAL_BLE_PERIPHERAL_SIMPLE_H_ */
