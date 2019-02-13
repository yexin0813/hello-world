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

#ifndef FREERTOS_APP_BLE_PERIPHERAL_BLE_USER_MAIN_H_
#define FREERTOS_APP_BLE_PERIPHERAL_BLE_USER_MAIN_H_

/**
 ****************************************************************************************
 * @addtogroup BLE_DEMO_MAIN  BLE Demo Simple Data init
 * @ingroup BLE_DEMO
 * @brief defines for BLE Demo Simple Data init
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
#ifdef OSAPP_BLE_DEMO
/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * INTERNAL API TYPES
 ****************************************************************************************
 */


/// Retrieve information about memory usage
//struct gapm_dbg_get_mem_info_cmd
//{
//    /// GAPM requested operation:
//    ///  - GAPM_DBG_GET_MEM_INFO: Get memory usage
//    uint8_t operation;
//};




/*
 * MACROS
 ****************************************************************************************
 */
/// Macro used to retrieve field


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// Information about registered LE_PSM
//struct gapm_le_psm_info
//{
//    /// List header structure to put information within a list
//    struct co_list_hdr hdr;
//
//    /// LE Protocol/Service Multiplexer
//    uint16_t le_psm;
//
//    /// Registered task identifier
//    ke_task_id_t task_id;
//
//    /// Security level
//    ///   7   6   5   4   3   2   1   0
//    /// +---+---+---+---+---+---+---+---+
//    /// |MI |      RFU      |EKS|SEC_LVL|
//    /// +---+---+---+---+---+---+---+---+
//    /// bit[0-1]: Security level requirement (0=NO_AUTH, 1=UNAUTH, 2=AUTH, 3=SEC_CON)
//    /// bit[2]  : Encryption Key Size length must have 16 bytes
//    /// bit[7]  : Multi-instantiated task
//    uint8_t sec_lvl;
//
//    /// Number of established link
//    uint8_t nb_est_lk;
//};

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
 * @brief user data init set.
 *
 */
extern void osapp_user_main_init(void);
/**
 ****************************************************************************************
 * @brief Send operation completed message according to operation type.
 * Perform also am operation clean-up
 *
 * @param[in] op_type Operation type
 * @param[in] status  Command status
 *****************************************************************************************
 */
//void gapm_send_complete_evt(uint8_t op_type, uint8_t status);



/// @} BLE_DEMO_MAIN

#endif/* OSAPP_BLE_DEMO */
#endif /* FREERTOS_APP_BLE_PERIPHERAL_BLE_USER_MAIN_H_ */
