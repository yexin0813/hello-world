/**
 ****************************************************************************************
 *
 * @file ble_apollo_00_simple_s.h
 *
 * @brief BLE Peripheral Server.
 *
 * Copyright (C) Apollo 2018-2023
 *
 ****************************************************************************************
 */
#ifndef FREERTOS_APP_BLE_PROFILES_BX_SIMPLE_BLE_APOLLO_00_SIMPLE_S_H_
#define FREERTOS_APP_BLE_PROFILES_BX_SIMPLE_BLE_APOLLO_00_SIMPLE_S_H_

/**
 ****************************************************************************************
 * @addtogroup BLE_APOLLO_00_SIMPLE_SERVICE BLE Peripheral Simple Server
 * @ingroup BLE_APOLLO_00_SIMPLE
 * @brief defines for BLE peripheral simple server
 *
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "ble_peripheral_simple.h"

#ifdef BLE_APOLLO_00_SIMPLE_SERVER
/*
 * DEFINES
 ****************************************************************************************
 */
/// Common 128-bit Universal Unique Identifier
 /*----------------- SERVICES ---------------------*/
#define BX_SIMPLE_SVC_UUID_128       {0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x00,0x24,0x40,0x6e}
/*--------------- CHARACTERISTICS ---------------*/
#define BX_SIMPLE_CHAR1_UUID_128    {0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x01,0x24,0x40,0x6e}
#define BX_SIMPLE_CHAR2_UUID_128    {0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x02,0x24,0x40,0x6e}
#define BX_SIMPLE_CHAR3_UUID_128    {0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x03,0x24,0x40,0x6e}

/*--------------- CHARACTERISTICS MAX LEN ---------------*/
#define BX_SIMPLE_CHAR1_MAX_LEN     5
#define BX_SIMPLE_CHAR2_MAX_LEN     2
#define BX_SIMPLE_CHAR3_MAX_LEN     20

/*
 * ENUMERATIONS
 ****************************************************************************************
 */
/// BX Simple Service Attributes Indexes
enum
{
    BX_SIMPLES_IDX_CHAR1_CHAR,
    BX_SIMPLES_IDX_CHAR1_VAL,

    BX_SIMPLES_IDX_CHAR2_CHAR,
    BX_SIMPLES_IDX_CHAR2_VAL,

    BX_SIMPLES_IDX_CHAR3_CHAR,
    BX_SIMPLES_IDX_CHAR3_VAL,
    BX_SIMPLES_IDX_CHAR3_CFG,

    BX_SIMPLES_ATT_NUM,
};

/*
 * INTERNAL API TYPES
 ****************************************************************************************
 */
/// Information about ble service environmental variable
typedef struct
{
    /// Connection state
    uint8_t is_connect;
}ble_service_env_t;



/*
 * MACROS
 ****************************************************************************************
 */
/// Macro used to retrieve field

/// Macro used to convert CPU integer define to LSB first 16-bits UUID
#define ATT_UUID16_TO_ARRAY(uuid)  (uuid)&0x00ff,(uuid)>>8&0x00ff

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */
/// GAP Manager environment variable.
//extern struct gapm_env_tag gapm_env;


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief add service and characteristics in service_table
 *
 * @param[in] cb  gatt structure for service callback functions
 *
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 *
 ****************************************************************************************
 */
extern int32_t ble_bx_simple_add_svc(gattServiceCBs_t *cb);

/**
 ****************************************************************************************
 * @brief char3 notification data to air.
 *
 * @param[in] data      Pointer to the notification data.
 * @param[in] length    the length of notification data.
 *
 ****************************************************************************************
 */
extern void ble_bx_simple_char3_send_notification(uint8_t const *data,uint8_t length);

/// @} BLE_APOLLO_00_SIMPLE_SERVICE

#endif /* BLE_APOLLO_00_SIMPLE_SERVER */
#endif /* FREERTOS_APP_BLE_PROFILES_BX_SIMPLE_BLE_APOLLO_00_SIMPLE_S_H_ */
