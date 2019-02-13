/**
 ****************************************************************************************
 *
 * @file ble_apollo_00_simple_c.h
 *
 * @brief BLE apollo_00 simple profile Client.
 *
 * Copyright (C) Apollo 2018-2023
 *
 ****************************************************************************************
 */
#ifndef FREERTOS_APP_BLE_PROFILES_BX_SIMPLE_BLE_APOLLO_00_SIMPLE_C_H_
#define FREERTOS_APP_BLE_PROFILES_BX_SIMPLE_BLE_APOLLO_00_SIMPLE_C_H_
/**
 ****************************************************************************************
 * @addtogroup BLE_APOLLO_00_SIMPLE_CLIENT  apollo_00 simple profile Client
 * @ingroup BLE_APOLLO_00_SIMPLE
 * @brief defines for apollo_00 simple profile Client
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
#include "ble_central.h"

#ifdef BLE_APOLLO_00_SIMPLE_CLIENT
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
extern int32_t ble_bx_simple_prf_add_svc(gattServicePrfCBs_t *cb);

/// @} BLE_APOLLO_00_SIMPLE_CLIENT
#endif /* BLE_APOLLO_00_SIMPLE_CLIENT */

#endif /* FREERTOS_APP_BLE_PROFILES_BX_SIMPLE_BLE_APOLLO_00_SIMPLE_C_H_ */
