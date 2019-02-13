/**
 *
 * @file osapp_gble.c
 *
 * @brief sample interface for Gizwits gble. Gizwits write this file, customer of Gizwits write osapp_gble_itf.c
 *
 * Copyright (C) Apollo 2015-2016
 *
 */

#include "osapp_config.h"

/**
 ****************************************************************************************
 * @addtogroup App Demo
 * @ingroup RTOS App
 * @brief sample interface for Gizwits gble
 * @{
 ****************************************************************************************
 */
#ifdef OSAPP_GBLE
#include "gattm_task.h"
#include "att.h"
#include "motor.h"
#include "dht11.h"
#include "rgb_led.h"
#include "adxl362.h"
#include "osapp_gble_itf.h"

/**
 * @brief struct to store information related to link key
 */
struct store_gapc_ltk
{
    /// Long Term Key
    struct gap_sec_key ltk;
    /// Encryption Diversifier
    uint16_t ediv;
    /// Random Number
    rand_nb_t randnb;
    /// Encryption key size (7 to 16)
    uint8_t key_size;
};

/**
 * @brief struct to store bond information
 */
struct bond_info_t
{
	/// valid or not
    uint8_t valid;
    /// store information related to link key
    struct store_gapc_ltk ltk_store;
    /// store csrk, which will use in encryption later
    struct gap_sec_key csrk;
    /// store auth, which will use in encryption later
    struct gapc_bond_auth auth;
    /// store the master address, should compare with conn_info_t.device_addr when bonding
    bd_addr_t device_addr;
};

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

///store bond information
static struct bond_info_t bond_info;
///store connnect information
static struct conn_info_t conn_info;

/// A key to type in the master when pairing, can be change by user. This mainly make a bonding processes without mitm.
#define BOND_PASSKEY        654321
/// channel map of advertise, 0x7 means we use three channel for advertise.
#define APP_ADV_CHMAP 0x7
/// set the minimum interval of advertise, normally same with APP_ADV_INT_MAX
#define APP_ADV_INT_MIN 40
/// set the maximum interval of advertise, normally same with APP_ADV_INT_MIN
#define APP_ADV_INT_MAX 40

/// shorttened length of the device name, can be set to 248 at most. See nvds_in_ram.h for more information.
#define OSAPP_DEVICE_NAME_SHT_LEN      (18)

/// get device name stored in nvds, the length should be 248.
static uint8_t device_name[NVDS_LEN_DEVICE_NAME];
/// the length should be NVDS_LEN_DEVICE_NAME(248).
static uint8_t device_name_len = NVDS_LEN_DEVICE_NAME;


/// Enter Gizwits company Id here.
uint8_t GIZWITS_COMPANY_ID[2] = {0xff,0xff};
/// Enter product key here.
uint8_t PRODUCT_KEY[16] = {0x12,0x34,0x56,0x78,0x90,0xab,0xcd,0xef,0x12,0x34,0x56,0x78,0x90,0xab,0xcd,0xef};

/// gble service uuid
uint8_t SERVICE_UUID[2] = {0xb0,0xff};
uint8_t CHARACTERISTIC_UUID[2] = {0x03,0x28};
/// gble write uuid - master write data to ble device
uint8_t WRITE_UUID[2] = {0xb1,0xff};
/// gble notify uuid - ble device send data to master
uint8_t NOTIFY_UUID[2] = {0xb2,0xff};
/// notify config - to enable or disable notify
uint8_t CHARCFG_UUID[2] = {0x02,0x29};

/**
 * @brief struct to store handle of all the characteristic handler.
 */
struct DEMO_HANDLE_T
{
    /// valid or not
    uint8_t valid;
    uint16_t service_handle;
    uint16_t write_handle;
    uint16_t notify_handle;
    uint16_t charcfg_handle;

};
/// all characteristics should be stored for future use, handles are use to distinguish which charateristic to read or write.
struct DEMO_HANDLE_T demo_char_handles;

/// array for all characteristics of this demo, and permission are defined in this array
struct att_service_tab
{
    uint8_t *uuid;
    uint16_t perm;
    uint16_t ext_perm;
    uint16_t max_len;
};

/// define of all characteristics of this demo, permission and data length can be change here.
struct att_service_tab service_table[] = 
{
    {CHARACTERISTIC_UUID,   PERM(RD, ENABLE),                           0,                          0},
    {WRITE_UUID,            PERM(WRITE_COMMAND, ENABLE),                0,                          20},
    {CHARACTERISTIC_UUID,   PERM(RD, ENABLE),                           0,                          0},
    {NOTIFY_UUID,           PERM(NTF, ENABLE) | PERM(RD, ENABLE),       0,                          0},
    {CHARCFG_UUID,          PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE), 0,                          0},
};


/**
 * @brief change connection parameter including connection interval, latency and timeout
 * @param intval: connection interval, intval * 1.25ms = time of the interval
 * @param latency: connection latency(number of events)
 * @param timeout: time * 10ms = time of timeout
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 */
static int32_t osapp_connparam_update(uint16_t intval, uint16_t latency, uint16_t timeout)
{
    struct gapc_param_update_cmd *cmd = AHI_MSG_ALLOC(GAPC_PARAM_UPDATE_CMD,TASK_ID_GAPC,gapc_param_update_cmd);

    cmd->operation = GAPC_UPDATE_PARAMS;
    cmd->intv_max = intval;
    cmd->intv_min = intval;
    cmd->time_out = timeout;
    cmd->ce_len_max = 0xFFFF;
    cmd->ce_len_min = 0xFFFF;
    cmd->latency = latency;

    return osapp_msg_build_send(cmd, sizeof(struct gapc_param_update_cmd));
}

/**
 * @brief break off a connection.
 * @param dest_id connection id, different connection has different connection id.
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 */
static int32_t osapp_disconnect(ke_task_id_t dest_id)
{
    struct gapc_disconnect_cmd *cmd = AHI_MSG_ALLOC(GAPC_DISCONNECT_CMD,dest_id,gapc_disconnect_cmd);
    
    cmd->operation = GAPC_DISCONNECT;
    cmd->reason    = CO_ERROR_REMOTE_USER_TERM_CON;
    return osapp_msg_build_send(cmd,sizeof(struct gapc_disconnect_cmd));

}

/**
 * @brief configure and start advertising
 * @param interval: advertising interval. interval * 0.625ms = time of the interval
 * @param adv_type: according to gble spec. 0x0 for normal adv, 0x1 for data report adv
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 */
static int32_t osapp_start_advertising(uint16_t interval, uint16_t adv_type)  //    according to spec V4.2 32<=interval<=16384 
{
    BX_ASSERT((interval>=32) && (interval<=16384));
    uint8_t rsp_data[28];
    uint8_t bd_address_len = NVDS_LEN_BD_ADDRESS;
    // Prepare the GAPM_START_ADVERTISE_CMD message
    struct gapm_start_advertise_cmd *cmd = AHI_MSG_ALLOC(GAPM_START_ADVERTISE_CMD,TASK_ID_GAPM, gapm_start_advertise_cmd);

    cmd->op.addr_src    = GAPM_STATIC_ADDR;
    cmd->channel_map    = APP_ADV_CHMAP;
    cmd->intv_min = interval;
    cmd->intv_max = interval;
    cmd->op.code        = GAPM_ADV_UNDIRECT;
    cmd->info.host.mode = GAP_GEN_DISCOVERABLE;

    cmd->info.host.adv_data_len = 4;
    adv_data_pack(cmd->info.host.adv_data,1,GAP_AD_TYPE_SERVICE_16_BIT_DATA,SERVICE_UUID,2);

    memcpy(&rsp_data[0],GIZWITS_COMPANY_ID,2);
    memcpy(&rsp_data[2],PRODUCT_KEY,16);

    nvds_get(NVDS_TAG_BD_ADDRESS,&bd_address_len,&rsp_data[18]);
    rsp_data[24] = adv_type >> 8;
    rsp_data[25] = adv_type & 0xFF;

    adv_data_pack(cmd->info.host.scan_rsp_data,1,GAP_AD_TYPE_MANU_SPECIFIC_DATA,rsp_data,26);
    cmd->info.host.scan_rsp_data_len  = 28;

    return osapp_msg_build_send(cmd,sizeof(struct gapm_start_advertise_cmd));
        
}

/**
 * @brief cancel a gapm operation. For example cancel an advertising operation.
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 */
static int32_t osapp_gapm_cancel()
{
    struct gapm_cancel_cmd *cmd = AHI_MSG_ALLOC(GAPM_CANCEL_CMD,TASK_ID_GAPM, gapm_cancel_cmd);
    cmd->operation = GAPM_CANCEL;
    return osapp_msg_build_send(cmd,sizeof(struct gapm_cancel_cmd));
}

/**
 * @brief update advertise data.
 * @param adv_type: according to gble spec. 0x0 for normal adv, 0x1 for data report adv
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 */
static int32_t osapp_update_advertise_data(uint16_t adv_type)
{
    struct gapm_update_advertise_data_cmd *cmd = AHI_MSG_ALLOC(GAPM_UPDATE_ADVERTISE_DATA_CMD,TASK_ID_GAPM, gapm_update_advertise_data_cmd);
    uint8_t rsp_data[28];
    uint8_t bd_address_len = NVDS_LEN_BD_ADDRESS;
    cmd->operation = GAPM_UPDATE_ADVERTISE_DATA;
    
    cmd->adv_data_len= 4;
    adv_data_pack(cmd->adv_data,1,GAP_AD_TYPE_SERVICE_16_BIT_DATA,SERVICE_UUID,2);

    memcpy(&rsp_data[0],GIZWITS_COMPANY_ID,2);
    memcpy(&rsp_data[2],PRODUCT_KEY,16);

    nvds_get(NVDS_TAG_BD_ADDRESS,&bd_address_len,&rsp_data[18]);
    rsp_data[24] = adv_type >> 8;
    rsp_data[25] = adv_type & 0xFF;

    cmd->scan_rsp_data_len  = 28;
    adv_data_pack(cmd->scan_rsp_data,1,GAP_AD_TYPE_MANU_SPECIFIC_DATA,rsp_data,26);
    

    return osapp_msg_build_send(cmd,sizeof(struct gapm_update_advertise_data_cmd));

}


/**
 * @brief set device role and global settings when start. Should be down before advertising, just after the reset (GAPM_RESET).
 * @param role :
 *
			No role set yet
			GAP_ROLE_NONE        = 0x00,

			Observer role
			GAP_ROLE_OBSERVER    = 0x01,

			Broadcaster role
			GAP_ROLE_BROADCASTER = 0x02,

			Master/Central role
			GAP_ROLE_CENTRAL     = (0x04 | GAP_ROLE_OBSERVER),

			Peripheral/Slave role
			GAP_ROLE_PERIPHERAL  = (0x08 | GAP_ROLE_BROADCASTER),

			Device has all role, both peripheral and central
			GAP_ROLE_ALL         = (GAP_ROLE_CENTRAL | GAP_ROLE_PERIPHERAL),

			Debug mode used to force LL configuration on BLE 4.0
			GAP_ROLE_DBG_LE_4_0      = 0x80

 * @param addr_type:
 *
			Device Address is a Public Static address
			GAPM_CFG_ADDR_PUBLIC        = 0,

			Device Address is a Private Static address
			GAPM_CFG_ADDR_PRIVATE       = 1,

			Device Address generated using host-based Privacy feature
			GAPM_CFG_ADDR_HOST_PRIVACY  = 2,

			Device Address generated using controller-based Privacy feature
			GAPM_CFG_ADDR_CTNL_PRIVACY  = 4,

 * @param pairing_mode:
 *
			No pairing authorized
			GAPM_PAIRING_DISABLE  = 0,

			Legacy pairing Authorized
			GAPM_PAIRING_LEGACY   = (1 << 0),

			Secure Connection pairing Authorized
			GAPM_PAIRING_SEC_CON  = (1 << 1),

			Force re-generation of P256 private and public keys
			GAPM_PAIRING_FORCE_P256_KEY_GEN = (1<<7),

 * @param max_mtu: set the max byte send in a connection interval, when max_mtu>23 more than on event will happen in a connection interval.
 *        Should be no bigger than 2048
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 */
static int32_t osapp_set_dev_config(uint8_t role,uint8_t addr_type,uint8_t pairing_mode,uint16_t max_mtu)
{
    // Set Device configuration
    struct gapm_set_dev_config_cmd* cmd = AHI_MSG_ALLOC(GAPM_SET_DEV_CONFIG_CMD,TASK_ID_GAPM,gapm_set_dev_config_cmd);
    cmd->operation = GAPM_SET_DEV_CONFIG;
    cmd->role      = role;
   
    // Set Data length parameters
    cmd->sugg_max_tx_octets = BLE_MIN_OCTETS;
    cmd->sugg_max_tx_time   = BLE_MIN_TIME;
    cmd->max_mtu = max_mtu;
    cmd->addr_type = addr_type;
    cmd->pairing_mode = pairing_mode;
   
    return osapp_msg_build_send(cmd, sizeof(struct gapm_set_dev_config_cmd));
}

/**
 * @brief add service and characteristics in service_table
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 */
static int32_t osapp_add_svc()
{
    uint8_t i = 0;  
    struct gattm_add_svc_req *req = AHI_MSG_ALLOC_DYN(GATTM_ADD_SVC_REQ,TASK_ID_GATTM,gattm_add_svc_req,sizeof(struct gattm_att_desc)*(sizeof(service_table)/sizeof(struct att_service_tab)));
    req->svc_desc.start_hdl = 0;
    req->svc_desc.task_id = TASK_ID_AHI;
    req->svc_desc.perm = 0;
    req->svc_desc.nb_att = (sizeof(service_table)/sizeof(struct att_service_tab));
    demo_char_handles.valid = 0;

    memcpy(req->svc_desc.uuid,SERVICE_UUID,2);

    struct gattm_att_desc desc;

    
    for(i=0;i<(sizeof(service_table)/sizeof(struct att_service_tab));i++)
    {
        memcpy(desc.uuid,service_table[i].uuid,2);
        desc.perm = service_table[i].perm;
        desc.ext_perm = service_table[i].ext_perm;
        desc.max_len = service_table[i].max_len;
        memcpy(&req->svc_desc.atts[i], &desc, sizeof(struct gattm_att_desc));
    }

    return osapp_msg_build_send(req, sizeof(struct gattm_add_svc_req) + sizeof(struct gattm_att_desc)*(sizeof(service_table)/sizeof(struct att_service_tab)));
}


/**
 * @brief confirm a connection, use this function when receive GAPC_CONNECTION_REQ_IND
 * @param dest_id connection id, different connection has different connection id.
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 */
static int32_t osapp_gapc_conn_confirm(ke_task_id_t dest_id)
{
    struct gapc_connection_cfm *cfm = AHI_MSG_ALLOC(GAPC_CONNECTION_CFM, dest_id, gapc_connection_cfm);

    if(bond_info.valid == 0)
        cfm->auth = GAP_AUTH_REQ_NO_MITM_NO_BOND;
    else
    {
        if(!memcmp(&conn_info.device_addr,&bond_info.device_addr,sizeof(bd_addr_t)))
        {
            LOG(LOG_LVL_INFO,"bond connect\n");
            memcpy(&cfm->rcsrk,&bond_info.csrk,sizeof(struct gap_sec_key));
            cfm->auth = bond_info.auth.info;
            cfm->ltk_present = bond_info.auth.ltk_present;            
        }

    }
    return osapp_msg_build_send(cfm,sizeof(struct gapc_connection_cfm));
}

/**
 * @brief a reset should be down when receive a GAPM_DEVICE_READY_IND
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 */
static int32_t osapp_reset()
{
    struct gapm_reset_cmd *cmd = AHI_MSG_ALLOC(GAPM_RESET_CMD,TASK_ID_GAPM,gapm_reset_cmd);
    cmd->operation = GAPM_RESET;
    return osapp_msg_build_send(cmd, sizeof(struct gapm_reset_cmd));
}


/**
 * @brief print log when there are unhandled message.
 */
static void osapp_default_msg_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"default handler,msgid: 0x%x, dest_id: 0x%x, src_id: 0x%x\n",msgid,dest_id,src_id);

}

/**
 * @brief handler of get device information, callback when receive GAPC_GET_DEV_INFO_REQ_IND
 */
static void osapp_gapc_get_dev_info_req_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapc_get_dev_info_req_ind const *dev_info = param;
    LOG(LOG_LVL_INFO,"Get device information\n");
    switch(dev_info->req)
    {
        case GAPC_DEV_NAME:
        {
            struct gapc_get_dev_info_cfm * cfm = AHI_MSG_ALLOC_DYN(GAPC_GET_DEV_INFO_CFM,
                                                    src_id,
                                                    gapc_get_dev_info_cfm, OSAPP_DEVICE_NAME_SHT_LEN);
            cfm->req = dev_info->req;

            nvds_get(NVDS_TAG_DEVICE_NAME,&device_name_len,device_name);

            memcpy(cfm->info.name.value,device_name,OSAPP_DEVICE_NAME_SHT_LEN);
            cfm->info.name.length = OSAPP_DEVICE_NAME_SHT_LEN;

            osapp_msg_build_send(cfm,sizeof(struct gapc_get_dev_info_cfm) + OSAPP_DEVICE_NAME_SHT_LEN);
        } break;
        
        case GAPC_DEV_APPEARANCE:
        {
            // Allocate message
            struct gapc_get_dev_info_cfm *cfm = AHI_MSG_ALLOC(GAPC_GET_DEV_INFO_CFM,
                    src_id, gapc_get_dev_info_cfm);
            cfm->req = dev_info->req;
            // No appearance
            cfm->info.appearance = 0;

            osapp_msg_build_send(cfm,sizeof(struct gapc_get_dev_info_cfm));


        } break;

        case GAPC_DEV_SLV_PREF_PARAMS:
        {
            // Allocate message
            struct gapc_get_dev_info_cfm *cfm = AHI_MSG_ALLOC(GAPC_GET_DEV_INFO_CFM,
                    src_id, gapc_get_dev_info_cfm);
            cfm->req = dev_info->req;
            // Slave preferred Connection interval Min
            cfm->info.slv_params.con_intv_min = 8;
            // Slave preferred Connection interval Max
            cfm->info.slv_params.con_intv_max = 10;
            // Slave preferred Connection latency
            cfm->info.slv_params.slave_latency  = 0;
            // Slave preferred Link supervision timeout
            cfm->info.slv_params.conn_timeout    = 200;  // 2s (500*10ms)

            osapp_msg_build_send(cfm,sizeof(struct gapc_get_dev_info_cfm));


        } break;

        default: /* Do Nothing */ 
            break;
    }

}

/**
 * @brief gapc event complete.
 */
static void osapp_gapc_cmp_evt_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapc_cmp_evt const *cmp_evt = param; 
    switch(cmp_evt->operation)
    {
        case GAPC_UPDATE_PARAMS:
            if(cmp_evt->status == GAP_ERR_NO_ERROR)
            {
                LOG(LOG_LVL_INFO,"Connect param update success.\n");
            }
            else
            {
                LOG(LOG_LVL_INFO,"Connect param update error.\n");
            }
            break;
    }

}

/**
 * @brief slave comfirm connection parameter change.
 * @param src_id: connection id, different connection has different connection id.
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 */
static int32_t osapp_gapc_param_update_cfm(ke_task_id_t const src_id)
{
    struct gapc_param_update_cfm *cfm = AHI_MSG_ALLOC(GAPC_PARAM_UPDATE_CFM, src_id, gapc_param_update_cfm);
    cfm->accept = 0x01;
    cfm->ce_len_max = 0xffff;
    cfm->ce_len_min = 0xffff;
    return osapp_msg_build_send(cfm, sizeof(struct gapc_param_update_cfm));

}

/**
 * @brief indicate master need to change connection parameter.
 */
static void osapp_gapc_param_update_req_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"param update request\n");
    osapp_gapc_param_update_cfm(src_id);
}

/**
 * @ brief indicate that the connection parameter has already changed.
 */
static void osapp_gapc_param_update_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapc_param_updated_ind *ind = (struct gapc_param_updated_ind *)param;
    LOG(LOG_LVL_INFO,"param update ind_%x,%x,%x\n",ind->con_interval,ind->con_latency,ind->sup_to);
}

/**
 * @brief callback when service and characteristics added complete.
 */
static void gattm_add_svc_rsp_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gattm_add_svc_rsp *rsp = (struct gattm_add_svc_rsp *)param;
    LOG(LOG_LVL_INFO,"database_add -> %d,%d\n",rsp->start_hdl,rsp->status);

    demo_char_handles.valid = 1;
    demo_char_handles.service_handle = rsp->start_hdl;
    demo_char_handles.write_handle= rsp->start_hdl + 2;
    demo_char_handles.notify_handle= rsp->start_hdl + 4;
    demo_char_handles.charcfg_handle= rsp->start_hdl + 5;

    osapp_start_advertising(32,1);
}

/**
 * @brief callback when receive GAPC_CONNECTION_REQ_IND, indicate the connect has established.
 */
static void osapp_gapc_conn_req_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapc_connection_req_ind *ind = (struct gapc_connection_req_ind *)param;
    memcpy(&conn_info.device_addr,&ind->peer_addr,sizeof(bd_addr_t));
    conn_info.conn_idx = src_id;
    conn_info.valid = 1;

    osapp_gapc_conn_confirm(src_id);
    LOG(LOG_LVL_INFO,"Connect comfirm\n");
    
}

/**
 * @brief callback when receive GAPC_DISCONNECT_IND, indicate the connect has broken off. And normally, we should restart advertising when connection break off.
 */
static void osapp_gapc_disconnect_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"Connect lost\n");
    osapp_start_advertising(32,1);
}

/**
 * @brief handler of gapm complete message. Callback when receive GAPM_CMP_EVT
 */
static void osapp_gapm_cmp_evt_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapm_cmp_evt const *cmp_evt = param;
    switch(cmp_evt->operation)
    {
    case GAPM_RESET:
        if(cmp_evt->status == GAP_ERR_NO_ERROR)
        {
            LOG(LOG_LVL_INFO,"Reset OK\n");
            osapp_set_dev_config(GAP_ROLE_ALL,GAPM_CFG_ADDR_PUBLIC,GAPM_PAIRING_LEGACY,200);
        }
        else
        {
            LOG(LOG_LVL_INFO,"Reset Failed\n");
        }
        break;
    case GAPM_SET_DEV_CONFIG:
        if(cmp_evt->status == GAP_ERR_NO_ERROR)
        {
            LOG(LOG_LVL_INFO,"Set Role OK\n");
            osapp_add_svc();
            LOG(LOG_LVL_INFO,"Add Profile\n");
        }
        else
        {
            LOG(LOG_LVL_INFO,"Set Role Failed\n");
        }
        break;
    case GAPM_CONNECTION_DIRECT:
        if(cmp_evt->status == GAP_ERR_NO_ERROR)
        {
            LOG(LOG_LVL_INFO,"Connect Complete\n");
//            SCB->AIRCR = ((0x5FA<<16) | (4));         //for over time case
        }
        else
        {
            LOG(LOG_LVL_INFO,"Connect Failed\n");
        }
        break;
    case GAPM_ADV_UNDIRECT:
        if(cmp_evt->status == GAP_ERR_NO_ERROR)
        {
            LOG(LOG_LVL_INFO,"ADV Undirect End\n");
        }
        else
        {
            LOG(LOG_LVL_INFO,"ADV Undirect End Failed\n");
        }
    default:
        LOG(LOG_LVL_WARN,"gapm_cmp_evt operation:0x%x\n",cmp_evt->operation);
        break;
            
    }

}

/**
 * @brief Callback when receive GAPM_DEVICE_READY_IND. Every time after power on receive a GAPM_DEVICE_READY_IND and osapp_reset must be executed.
 */
static void osapp_device_ready_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    //reset cmd
    osapp_reset();
}

/**
 * @brief indicate the mtu has changed.
 */
static void osapp_gattc_mtu_changed_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gattc_mtu_changed_ind *ind = (struct gattc_mtu_changed_ind *)param;
    LOG(LOG_LVL_INFO,"mtu = %d,%d\n",ind->mtu, ind->seq_num);
}

/**
 * @brief master write to the device. Do different things according to the handle.
 */
static void gattc_write_req_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gattc_write_req_ind *ind = (struct gattc_write_req_ind *)param;

    LOG(LOG_LVL_INFO,"gattc write req_%d\n",ind->handle);
    
    if(demo_char_handles.valid == 0)
    {
        LOG(LOG_LVL_ERROR,"ERROR: write char handle null\n");
        return;
    }
    
    if(ind->handle == demo_char_handles.write_handle)
    {
        gble_receive_data_ind(ind->value,ind->length);
        
        LOG(LOG_LVL_INFO,"len = %d",ind->length);
    }
    else
    {
        LOG(LOG_LVL_ERROR,"ERROR: handle error_%d\n",ind->handle);
    }

    struct gattc_write_cfm *cfm = AHI_MSG_ALLOC(GATTC_WRITE_CFM, TASK_ID_GATTC, gattc_write_cfm);
    LOG(LOG_LVL_INFO,"gattc write cfm\n");

    cfm->handle = ind->handle;
    cfm->status = GAP_ERR_NO_ERROR;
    
    osapp_msg_build_send(cfm,sizeof(struct gattc_write_cfm));
}

/**
 * @brief Notify and indicate message send by this function.
 * @param type: to identify the message type
 * @param value: pointer of the input data
 */
void osapp_gattc_send_evt_cmd(uint8_t *value, uint8_t len)
{
    BX_ASSERT(len <= 20);
    
    struct gattc_send_evt_cmd *cmd = AHI_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD, TASK_ID_GATTC, gattc_send_evt_cmd, len);
    cmd->operation = GATTC_NOTIFY;
    cmd->seq_num = 0;
    cmd->handle = demo_char_handles.notify_handle;
    cmd->length = len;

    memcpy(cmd->value,value,len);
    osapp_msg_build_send(cmd,sizeof(struct gattc_send_evt_cmd) + len);
}

/**
 * @brief callback when gattc event has completed.
 */
static void gattc_cmp_evt_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gattc_cmp_evt *evt = (struct gattc_cmp_evt *)param;
    switch(evt->operation)
    {
        case GATTC_NOTIFY:
            if(evt->status == GAP_ERR_NO_ERROR)
                LOG(LOG_LVL_INFO, "Notify complete.\n");
            break;
        default:
            break;
    }
    
}


/**
 * @brief receive a bond request from the master, device should response to the master and some data need restore for future use.
 */
static void osapp_gapc_bond_req_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapc_bond_req_ind const *ind = param;
    LOG(LOG_LVL_INFO,"gapc_bond_req_ind_%d\n",ind->request);
    switch(ind->request)
    {
        case GAPC_PAIRING_REQ:
        {
                LOG(LOG_LVL_INFO,"GAPC_PAIRING_REQ\n");
                struct gapc_bond_cfm *cfm = AHI_MSG_ALLOC(GAPC_BOND_CFM, conn_info.conn_idx, gapc_bond_cfm);
                cfm->request = GAPC_PAIRING_RSP;
                cfm->accept = 0x01;
                cfm->data.pairing_feat.iocap = GAP_IO_CAP_DISPLAY_ONLY;
                cfm->data.pairing_feat.oob = GAP_OOB_AUTH_DATA_NOT_PRESENT;
                cfm->data.pairing_feat.auth = GAP_AUTH_REQ_MITM_BOND;
                cfm->data.pairing_feat.key_size = 0x10;
                cfm->data.pairing_feat.ikey_dist = GAP_KDIST_SIGNKEY;
                cfm->data.pairing_feat.rkey_dist = GAP_KDIST_ENCKEY | GAP_KDIST_IDKEY;
                cfm->data.pairing_feat.sec_req = GAP_SEC1_SEC_CON_PAIR_ENC;         
                osapp_msg_build_send(cfm, sizeof(struct gapc_bond_cfm));
            break;
        }
        case GAPC_LTK_EXCH:
        {
                uint8_t i;
                LOG(LOG_LVL_INFO,"GAPC_LTK_EXCH\n");
                struct gapc_bond_cfm *cfm = AHI_MSG_ALLOC(GAPC_BOND_CFM, conn_info.conn_idx, gapc_bond_cfm);
                cfm->accept = 0x1;
                cfm->request = GAPC_LTK_EXCH;
                cfm->data.ltk.ediv = (uint16_t)co_rand_word();
                for(i=0;i<KEY_LEN;i++)
                {
                    cfm->data.ltk.ltk.key[i] = (uint8_t)co_rand_word();
                }

                LOG(LOG_LVL_INFO,"Gen ltk = %x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x\n",
                    cfm->data.ltk.ltk.key[15],cfm->data.ltk.ltk.key[14],cfm->data.ltk.ltk.key[13],cfm->data.ltk.ltk.key[12],
                    cfm->data.ltk.ltk.key[11],cfm->data.ltk.ltk.key[10],cfm->data.ltk.ltk.key[9],cfm->data.ltk.ltk.key[8],
                    cfm->data.ltk.ltk.key[7],cfm->data.ltk.ltk.key[6],cfm->data.ltk.ltk.key[5],cfm->data.ltk.ltk.key[4],
                    cfm->data.ltk.ltk.key[3],cfm->data.ltk.ltk.key[2],cfm->data.ltk.ltk.key[1],cfm->data.ltk.ltk.key[0]);
//                memcpy(cfm->data.ltk.ltk.key, ltk_key, sizeof(ltk_key));
                for(i=0;i<RAND_NB_LEN;i++)
                {
                    cfm->data.ltk.randnb.nb[i] = (uint8_t)co_rand_word();
                }

                LOG(LOG_LVL_INFO,"gen rnb = %x:%x:%x:%x:%x:%x:%x:%x\n",
                    cfm->data.ltk.randnb.nb[7],cfm->data.ltk.randnb.nb[6],cfm->data.ltk.randnb.nb[5],cfm->data.ltk.randnb.nb[4],
                    cfm->data.ltk.randnb.nb[3],cfm->data.ltk.randnb.nb[2],cfm->data.ltk.randnb.nb[1],cfm->data.ltk.randnb.nb[0]);
//                memcpy(cfm->data.ltk.randnb.nb, ltk_randnb, sizeof(ltk_randnb));
                cfm->data.ltk.key_size = KEY_LEN;

                memcpy(&bond_info.ltk_store,&cfm->data.ltk,sizeof(struct store_gapc_ltk));
                osapp_msg_build_send(cfm, sizeof(struct gapc_bond_cfm));
            break;
        }
        case GAPC_TK_EXCH:
        {
            struct gapc_bond_cfm *cfm = AHI_MSG_ALLOC(GAPC_BOND_CFM, conn_info.conn_idx, gapc_bond_cfm);
            cfm->request = GAPC_TK_EXCH;
            cfm->accept = 0x1;
            cfm->data.tk.key[0]=BOND_PASSKEY & 0xFF;
            cfm->data.tk.key[1]=(BOND_PASSKEY>>8) & 0xFF;
            cfm->data.tk.key[2]=(BOND_PASSKEY>>16) & 0xFF;
            osapp_msg_build_send(cfm, sizeof(struct gapc_bond_cfm));
            break;
            
        }
        default:
            break;
    }


}


/**
 * @brief receive a encrypt request from the master, device should response to the master and some data need restore for future use.
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 */
static int gapc_encrypt_req_ind_handler(ke_msg_id_t const msgid,
                                        struct gapc_encrypt_req_ind const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{

    // Prepare the GAPC_ENCRYPT_CFM message
    LOG(LOG_LVL_INFO,"encrypt request\n");
    struct gapc_encrypt_cfm *cfm = AHI_MSG_ALLOC(GAPC_ENCRYPT_CFM, src_id, gapc_encrypt_cfm);

    if ((bond_info.valid == true) && (param->ediv == bond_info.ltk_store.ediv) &&
        !memcmp(&param->rand_nb.nb[0], &bond_info.ltk_store.randnb.nb[0], sizeof(bond_info.ltk_store.randnb)))
    {
        LOG(LOG_LVL_INFO,"encrypt found\n");
        cfm->found    = true;
        cfm->key_size = 16;

        memcpy(&cfm->ltk, &bond_info.ltk_store.ltk, sizeof(struct gap_sec_key));

        LOG(LOG_LVL_INFO,"Gen ltk = %x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x\n",
                    cfm->ltk.key[15],cfm->ltk.key[14],cfm->ltk.key[13],cfm->ltk.key[12],
                    cfm->ltk.key[11],cfm->ltk.key[10],cfm->ltk.key[9],cfm->ltk.key[8],
                    cfm->ltk.key[7],cfm->ltk.key[6],cfm->ltk.key[5],cfm->ltk.key[4],
                    cfm->ltk.key[3],cfm->ltk.key[2],cfm->ltk.key[1],cfm->ltk.key[0]);
    }

    osapp_msg_build_send(cfm, sizeof(struct gapc_encrypt_cfm));

    return (KE_MSG_CONSUMED);
}

/**
 * @brief receive a encrypt request from the master, device should response to the master and some data need restore for future use.
 */
static void gapc_encrypt_ind_handler(ke_msg_id_t const msgid,
                                        struct gapc_encrypt_ind const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"Auth_level_%d\n",param->auth);

}

/**
 * @brief indicate master the bond information
 */
static void osapp_gapc_bond_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{    
    struct gapc_bond_ind const *ind = param;
    LOG(LOG_LVL_INFO,"bond_ind_%d\n",ind->info);
    switch(ind->info)
    {
        case GAPC_CSRK_EXCH:
            LOG(LOG_LVL_INFO,"Csrk = %x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x\n",
                ind->data.csrk.key[15],ind->data.csrk.key[14],ind->data.csrk.key[13],ind->data.csrk.key[12],
                ind->data.csrk.key[11],ind->data.csrk.key[10],ind->data.csrk.key[9],ind->data.csrk.key[8],
                ind->data.csrk.key[7],ind->data.csrk.key[6],ind->data.csrk.key[5],ind->data.csrk.key[4],
                ind->data.csrk.key[3],ind->data.csrk.key[2],ind->data.csrk.key[1],ind->data.csrk.key[0]);
            memcpy(&bond_info.csrk,&ind->data.csrk,sizeof(bond_info.csrk));
            break;
        case GAPC_PAIRING_SUCCEED:
            LOG(LOG_LVL_INFO,"Auth_%d,Ltk Present_%d\n",ind->data.auth.info,ind->data.auth.ltk_present);
            LOG(LOG_LVL_INFO,"LTK = %x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x\n",
                ind->data.ltk.ltk.key[15],ind->data.ltk.ltk.key[14],ind->data.ltk.ltk.key[13],ind->data.ltk.ltk.key[12],
                ind->data.ltk.ltk.key[11],ind->data.ltk.ltk.key[10],ind->data.ltk.ltk.key[9],ind->data.ltk.ltk.key[8],
                ind->data.ltk.ltk.key[7],ind->data.ltk.ltk.key[6],ind->data.ltk.ltk.key[5],ind->data.ltk.ltk.key[4],
                ind->data.ltk.ltk.key[3],ind->data.ltk.ltk.key[2],ind->data.ltk.ltk.key[1],ind->data.ltk.ltk.key[0]);
            memcpy(&bond_info.auth,&ind->data.auth,sizeof(struct gapc_bond_auth));
            memcpy(&bond_info.device_addr,&conn_info.device_addr,sizeof(bd_addr_t));
            bond_info.valid = 1;
            break;
         case  GAPC_PAIRING_FAILED:
            LOG(LOG_LVL_INFO,"Bond Failed, error code = %d\n",ind->data.reason);
//            if(!memcmp(&conn_info.device_addr, &bond_info.device_addr,sizeof(bd_addr_t)))
            {
                bond_info.valid = 0;
                LOG(LOG_LVL_INFO,"Set Bond data invalid\n");
            }
            osapp_disconnect(conn_info.conn_idx);
            break;
            
    }
}


/**
 * @brief message and handler table. This define the connection of message and it's callback.
 */
static const osapp_msg_handler_table_t handler_table[]=
{
    [0] =   {KE_MSG_DEFAULT_HANDLER,(osapp_msg_handler_t)osapp_default_msg_handler},
        /* connection indicate: receive connect request from master */
       {GAPC_CONNECTION_REQ_IND,(osapp_msg_handler_t)osapp_gapc_conn_req_ind_handler}, 
       /* connection lost indicate handler */
       {GAPC_DISCONNECT_IND,(osapp_msg_handler_t)osapp_gapc_disconnect_ind_handler},
       /* GAPM event complete */
       {GAPM_CMP_EVT,(osapp_msg_handler_t)osapp_gapm_cmp_evt_handler},
       /* ble power on ready and should do a reset */
       {GAPM_DEVICE_READY_IND,(osapp_msg_handler_t)osapp_device_ready_ind_handler},
       /* trigger when master need to read device information uuid 0x1800 */
       {GAPC_GET_DEV_INFO_REQ_IND,(osapp_msg_handler_t)osapp_gapc_get_dev_info_req_ind_handler},
       /* GAPC event complete */
       {GAPC_CMP_EVT,(osapp_msg_handler_t)osapp_gapc_cmp_evt_handler},
       /* master require to change connection parameter */
       {GAPC_PARAM_UPDATE_REQ_IND,(osapp_msg_handler_t)osapp_gapc_param_update_req_handler}, 
       /* connection parameter has changed */
       {GAPC_PARAM_UPDATED_IND,(osapp_msg_handler_t)osapp_gapc_param_update_ind_handler},
       /* trigger when mtu exchange complete */
       {GATTC_MTU_CHANGED_IND,(osapp_msg_handler_t)osapp_gattc_mtu_changed_ind_handler},
       /* add service complete and store service handler */
       {GATTM_ADD_SVC_RSP,(osapp_msg_handler_t)gattm_add_svc_rsp_handler},
       /* master write data to gble */
       {GATTC_WRITE_REQ_IND,(osapp_msg_handler_t)gattc_write_req_ind_handler},
       /* gattc cmd complete */
       {GATTC_CMP_EVT,(osapp_msg_handler_t)gattc_cmp_evt_handler},
       /* device receive a bond request */
       {GAPC_BOND_REQ_IND,(osapp_msg_handler_t)osapp_gapc_bond_req_ind_handler},
       /* upper layer receive some bond information, some data need store by user */
       {GAPC_BOND_IND,(osapp_msg_handler_t)osapp_gapc_bond_ind_handler}, 
       /* device receive a encrypt request */
       {GAPC_ENCRYPT_REQ_IND,(osapp_msg_handler_t)gapc_encrypt_req_ind_handler},
       /* upper layer receive encrypt information */
       {GAPC_ENCRYPT_IND,(osapp_msg_handler_t)gapc_encrypt_ind_handler},
};



const osapp_msg_handler_info_t handler_info = ARRAY_INFO(handler_table);

/// stack size of dht task
#define DHT_TASK_STACK_SIZE             256
/// define task priority of DHT_TASK. Should be careful some priority are defined in task_init.h
#define OS_PRIORITY_DHT_TASK			(tskIDLE_PRIORITY + configMAX_PRIORITIES  - 3)      // 3

/**
 * @brief define freertos tasks here. Exclude the BLE_TASK and APP_TASK
 */
static const task_table_t task_table[] =
{
    { true, "DHT_TASK",  OS_PRIORITY_DHT_TASK,   DHT_TASK_STACK_SIZE, gble_dht_data_avaliable, dht_task, &handler_dht_task },
};

const task_table_info_t extra_task_table_info = ARRAY_INFO(task_table);

#endif

///@}
