/**
 ****************************************************************************************
 *
 * @file ble_apollo_00_simple_c.c
 *
 * @brief BLE apollo_00 simple profile Client.
 *
 * Copyright (C) Apollo 2018-2023
 *
 ****************************************************************************************
 */

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
#include "ble_apollo_00_simple_c.h"

#ifdef BLE_APOLLO_00_SIMPLE_CLIENT


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
 * LOCAL FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

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
int32_t ble_bx_simple_prf_add_svc(gattServicePrfCBs_t *cb)
{
//    struct gattm_add_svc_req *req = AHI_MSG_ALLOC_DYN(GATTM_ADD_SVC_REQ,TASK_ID_GATTM,\
//        gattm_add_svc_req,sizeof(bx_simple_svc_att_db));
//    struct gattm_svc_desc *svc = &req->svc_desc;
//
//    memcpy(svc,&bx_simple_svc_desc,sizeof(bx_simple_svc_desc));
//    memcpy(svc->atts,bx_simple_svc_att_db,sizeof(bx_simple_svc_att_db));
//
//    memcpy(cb,&bx_simple_callbacks,sizeof(gattServiceCBs_t));
//
//    LOG(LOG_LVL_INFO," svc ble_bx_simple_add_svc \n");
//    return osapp_msg_build_send(req, sizeof(struct gattm_svc_desc)+sizeof(bx_simple_svc_att_db));
    return 1;
}
/*
 * MESSAGES HANDLERS DEFINITIONS
 ****************************************************************************************
 */

/// @} BLE_APOLLO_00_SIMPLE_CLIENT

#endif/* BLE_APOLLO_00_SIMPLE_CLIENT */

