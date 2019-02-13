/**
 ****************************************************************************************
 *
 * @file ble_peripheral_simple.c
 *
 * @brief BLE Peripheral Simple Demo.
 *
 * Copyright (C) Apollo 2018-2023
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup BLE_Peripheral Manager HCI Hander
 * @ingroup BLE_PERIPHERAL
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "ble_peripheral_config.h"

#ifdef BLE_PERIPHERAL_SIMPLE

#include "ble_peripheral_simple.h"
#include "ble_user_main.h"

#include "ble_apollo_00_simple_s.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */



/*
 * DEFINES
 ****************************************************************************************
 */
#define BLE_PERIPHERAL_MAX_MTU  23  //max pdu mtu size (23~2048)
/**
 * Default Scan response data
 * --------------------------------------------------------------------------------------
 * x09                             - Length
 * xFF                             - Vendor specific advertising type
 * x00\x60\x52\x57\x2D\x42\x4C\x45 - "RW-BLE"
 * --------------------------------------------------------------------------------------
 */
#define APP_SCNRSP_DATA         "\x09\xFF\x00\x60\x52\x57\x2D\x42\x4C\x45"
#define APP_SCNRSP_DATA_LEN     (10)

/**
 * Advertising Parameters
 */
/// Advertising channel map - 37, 38, 39
#define APP_ADV_CHMAP           (0x07)
/// Advertising minimum interval - 40ms (64*0.625ms)
#define APP_ADV_INT_MIN         (64)
/// Advertising maximum interval - 40ms (64*0.625ms)
#define APP_ADV_INT_MAX         (64)
/// Fast advertising interval
#define APP_ADV_FAST_INT        (32)

///App device name
#define APP_DEVICE_NAME "Apollo BLE Simple Demo"
/// set response data when active scan, maximum 29bytes long
#define OSAPP_SCNRSP_DATA         ("APOLLO_00-ADVRSP")
/// set response data length, maximum 29bytes long
#define OSAPP_SCNRSP_DATA_LEN     (sizeof(OSAPP_SCNRSP_DATA))

/**
 * Slave Preferred Parameters
 */
/// Slave preferred Connection interval Min
#define CONN_INTERVAL_MIN   20
/// Slave preferred Connection interval Max
#define CONN_INTERVAL_MAX   40
/// Slave preferred Connection latency
#define SLAVE_LATENCY       0
/// Slave preferred Link supervision timeout
#define CONN_TIMEOUT        400

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
static ble_peripheral_simple_env_t peripheral_env;
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
static void osapp_ble_stack_data_init(void);
static void osapp_device_ready_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static int32_t osapp_reset(void);
static void osapp_default_msg_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static int32_t osapp_set_dev_config(uint8_t role,uint8_t addr_type,uint8_t pairing_mode,uint16_t max_mtu);
static int32_t osapp_add_user_svc(uint8_t idex);
static void osapp_gattm_add_svc_rsp_handler(ke_msg_id_t const msgid, struct gattm_add_svc_rsp const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int32_t osapp_start_advertising(void);
static void osapp_gapc_conn_req_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static void osapp_gapc_disconnect_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static int32_t osapp_gapc_conn_confirm(ke_task_id_t dest_id);
static void osapp_gattc_write_req_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static void osapp_gattc_read_req_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static void osapp_gattc_cmp_evt_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static void osapp_connect_states_updata(uint8_t is_connect);
/*
 * MESSAGES HANDLERS DEFINITIONS
 ****************************************************************************************
 */
/**
 * @brief user data init set.
 *
 */
__attribute__ ((weak)) void osapp_user_main_init(void)
{
     osapp_ble_stack_run();
     LOG(LOG_LVL_INFO,"osapp_user_main_init\n");
}
/**
 * @brief ble stack start run.
 *
 */
void osapp_ble_stack_run(void)
{
     osapp_ble_stack_data_init();
     osapp_set_dev_config(GAP_ROLE_PERIPHERAL,GAPM_CFG_ADDR_PUBLIC,GAPM_PAIRING_LEGACY,BLE_PERIPHERAL_MAX_MTU);
     LOG(LOG_LVL_INFO,"osapp_ble_stack_run\n");
}
/**
 * @brief ble stack data init.
 *
 */
static void osapp_ble_stack_data_init(void)
{
    memset(&peripheral_env, 0 , sizeof(ble_peripheral_simple_env_t));
    peripheral_env.svc.max_num = BLE_PERIPHERAL_SERVICES_NUM;
    peripheral_env.svc.index = 0;
    peripheral_env.svc.handles[BX_SIMPLES_SERVICE_ID] = ble_bx_simple_add_svc;
}
/**
 ****************************************************************************************
 * @brief print log when there are unhandled message.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAPM).
 * @param[in] src_id    ID of the sending task instance.
 *
 ****************************************************************************************
 */
static void osapp_default_msg_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"default handler,msgid: 0x%x, dest_id: 0x%x, src_id: 0x%x\n",msgid,dest_id,src_id);
}
/**
 ****************************************************************************************
 * @brief a reset should be down when receive a GAPM_DEVICE_READY_IND
 *
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 *
 ****************************************************************************************
 */
static int32_t osapp_reset(void)
{
    struct gapm_reset_cmd *cmd = AHI_MSG_ALLOC(GAPM_RESET_CMD,TASK_ID_GAPM,gapm_reset_cmd);
    cmd->operation = GAPM_RESET;
    return osapp_msg_build_send(cmd, sizeof(struct gapm_reset_cmd));
}

/**
 ****************************************************************************************
 * @brief Callback when receive GAPM_DEVICE_READY_IND.
 * Every time after power on receive a GAPM_DEVICE_READY_IND and osapp_reset must be executed.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAPM).
 * @param[in] src_id    ID of the sending task instance.
 *
 ****************************************************************************************
 */
static void osapp_device_ready_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    int32_t result = 0;
    //reset cmd
    result = osapp_reset();
    BX_ASSERT(result!=0);
}
/**
 ****************************************************************************************
 * @brief set device role and global settings when start. Should be down before advertising, just after the reset (GAPM_RESET).
 * @param[in] role :
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

 * @param[in] addr_type :
 *
            Device Address is a Public Static address
            GAPM_CFG_ADDR_PUBLIC        = 0,

            Device Address is a Private Static address
            GAPM_CFG_ADDR_PRIVATE       = 1,

            Device Address generated using host-based Privacy feature
            GAPM_CFG_ADDR_HOST_PRIVACY  = 2,

            Device Address generated using controller-based Privacy feature
            GAPM_CFG_ADDR_CTNL_PRIVACY  = 4,

 * @param[in] pairing_mode :
 *
            No pairing authorized
            GAPM_PAIRING_DISABLE  = 0,

            Legacy pairing Authorized
            GAPM_PAIRING_LEGACY   = (1 << 0),

            Secure Connection pairing Authorized
            GAPM_PAIRING_SEC_CON  = (1 << 1),

            Force re-generation of P256 private and public keys
            GAPM_PAIRING_FORCE_P256_KEY_GEN = (1<<7),

 * @param[in] max_mtu : set the max byte send in a connection interval, when max_mtu>23 more than on event will happen in a connection interval.
 *        Should be no bigger than 2048
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 *
 ****************************************************************************************
 */
static int32_t osapp_set_dev_config(uint8_t role,uint8_t addr_type,uint8_t pairing_mode,uint16_t max_mtu)
{
    // Set Device configuration
    struct gapm_set_dev_config_cmd* cmd = AHI_MSG_ALLOC(GAPM_SET_DEV_CONFIG_CMD,TASK_ID_GAPM,gapm_set_dev_config_cmd);

    memset(cmd, 0 , sizeof(struct gapm_set_dev_config_cmd));

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
 ****************************************************************************************
 * @brief add service and characteristics in service_table
 *
 * @param[in] index  now which svc should add
 *
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 *
 ****************************************************************************************
 */

static int32_t osapp_add_user_svc(uint8_t index)
{
    if(peripheral_env.svc.index < peripheral_env.svc.max_num)
    {
        add_svc_handler_t p_add_svc_func = peripheral_env.svc.handles[peripheral_env.svc.index];
        return p_add_svc_func((peripheral_env.svc.gattcbs+peripheral_env.svc.index));
    }
    else
    {
        LOG(LOG_LVL_INFO,"svc add done\n");
        osapp_start_advertising();
        return 0;
    }
}
/**
 ****************************************************************************************
 * @brief callback when service and characteristics added complete.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAPM).
 * @param[in] src_id    ID of the sending task instance.
 *
 ****************************************************************************************
 */
static void osapp_gattm_add_svc_rsp_handler(ke_msg_id_t const msgid, struct gattm_add_svc_rsp const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"osapp_gattm_add_svc_rsp_handler\n");
    if(param->status == ATT_ERR_NO_ERROR)
    {
        peripheral_env.svc.start_hdl[peripheral_env.svc.index]=param->start_hdl;
        /// update start handler
        if((&peripheral_env.svc.gattcbs[peripheral_env.svc.index] != NULL) && (peripheral_env.svc.gattcbs[peripheral_env.svc.index].pfnHandlerInitCB != NULL))
        {
            peripheral_env.svc.gattcbs[peripheral_env.svc.index].pfnHandlerInitCB(param->start_hdl);
        }

        peripheral_env.svc.index++;//updata svc index
        osapp_add_user_svc(peripheral_env.svc.index);//add next svc
    }else
    {
        LOG(LOG_LVL_ERROR,"add svc fail\n");
        LOG(LOG_LVL_INFO,"SVC fail num = 0x%x\n",param->status);
    }
}


/**
 ****************************************************************************************
 * @brief configure and start advertising
 *
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 *
 ****************************************************************************************
 */
static int32_t osapp_start_advertising(void)
{
    // Prepare the GAPM_START_ADVERTISE_CMD message
    struct gapm_start_advertise_cmd *cmd = AHI_MSG_ALLOC(GAPM_START_ADVERTISE_CMD,TASK_ID_GAPM, gapm_start_advertise_cmd);

    memset(cmd, 0 , sizeof(struct gapm_start_advertise_cmd));

    cmd->op.addr_src    = GAPM_STATIC_ADDR;
    cmd->channel_map    = APP_ADV_CHMAP;
    cmd->intv_min = APP_ADV_INT_MIN;
    cmd->intv_max = APP_ADV_INT_MAX;
    cmd->op.code        = GAPM_ADV_UNDIRECT;
    cmd->info.host.mode = GAP_GEN_DISCOVERABLE;

    // Flag value is set by the GAP
    cmd->info.host.adv_data_len = ADV_DATA_PACK(cmd->info.host.adv_data,1,GAP_AD_TYPE_COMPLETE_NAME,APP_DEVICE_NAME,sizeof(APP_DEVICE_NAME));
    cmd->info.host.scan_rsp_data_len = ADV_DATA_PACK(cmd->info.host.scan_rsp_data,1,GAP_AD_TYPE_MANU_SPECIFIC_DATA,OSAPP_SCNRSP_DATA,OSAPP_SCNRSP_DATA_LEN);

    LOG(LOG_LVL_INFO,"osapp_start_advertising \n");
    return osapp_msg_build_send(cmd,sizeof(struct gapm_start_advertise_cmd));
}
/**
 ****************************************************************************************
 * @brief callback when receive GAPC_CONNECTION_REQ_IND, indicate the connect has established.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAPM).
 * @param[in] src_id    ID of the sending task instance.
 *
 ****************************************************************************************
 */
static void osapp_gapc_conn_req_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapc_connection_req_ind *ind = (struct gapc_connection_req_ind *)param;
    memcpy(&peripheral_env.conn_info.device_addr,&ind->peer_addr,sizeof(bd_addr_t));
    peripheral_env.conn_info.conn_idx = src_id;
    peripheral_env.conn_info.valid = 1;

    osapp_gapc_conn_confirm(src_id);
    LOG(LOG_LVL_INFO,"Connect comfirm\n");
    osapp_connect_states_updata(IS_CONNECTED);
}
/**
 ****************************************************************************************
 * @brief confirm a connection, use this function when receive GAPC_CONNECTION_REQ_IND
 *
 * @param[in] dest_id connection id, different connection has different connection id.
 *
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 *
 ****************************************************************************************
 */
static int32_t osapp_gapc_conn_confirm(ke_task_id_t dest_id)
{
    struct gapc_connection_cfm *cfm = AHI_MSG_ALLOC(GAPC_CONNECTION_CFM, dest_id, gapc_connection_cfm);

    cfm->auth = GAP_AUTH_REQ_NO_MITM_NO_BOND;

    return osapp_msg_build_send(cfm,sizeof(struct gapc_connection_cfm));
}
/**
 ****************************************************************************************
 * @brief handler of get device information, callback when receive GAPC_GET_DEV_INFO_REQ_IND
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAPM).
 * @param[in] src_id    ID of the sending task instance.
 *
 ****************************************************************************************
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
                                                    gapc_get_dev_info_cfm, sizeof(APP_DEVICE_NAME));
            cfm->req = dev_info->req;
            cfm->info.name.length = sizeof(APP_DEVICE_NAME);
            memcpy(cfm->info.name.value,APP_DEVICE_NAME,sizeof(APP_DEVICE_NAME));
            osapp_msg_build_send(cfm,sizeof(struct gapc_get_dev_info_cfm) + sizeof(APP_DEVICE_NAME));
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
            cfm->info.slv_params.con_intv_min = CONN_INTERVAL_MIN;
            // Slave preferred Connection interval Max
            cfm->info.slv_params.con_intv_max = CONN_INTERVAL_MAX;
            // Slave preferred Connection latency
            cfm->info.slv_params.slave_latency  = SLAVE_LATENCY;
            // Slave preferred Link supervision timeout
            cfm->info.slv_params.conn_timeout    = CONN_TIMEOUT;

            osapp_msg_build_send(cfm,sizeof(struct gapc_get_dev_info_cfm));
        } break;

        default: /* Do Nothing */
            break;
    }
}
/**
 ****************************************************************************************
 * @brief master write to the device. Do different things according to the handle.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAPM).
 * @param[in] src_id    ID of the sending task instance.
 *
 ****************************************************************************************
 */
static void osapp_gattc_write_req_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    uint8_t i=0;
    for(i=0;i<peripheral_env.svc.max_num;i++)
    {
        if((&peripheral_env.svc.gattcbs[i] != NULL) && (peripheral_env.svc.gattcbs[i].pfnWriteAttrCB != NULL))
        {
            peripheral_env.svc.gattcbs[i].pfnWriteAttrCB(msgid,param,dest_id,src_id);
        }
    }
}
/**
 ****************************************************************************************
 * @brief master read from the device. Do different things according to the handle.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAPM).
 * @param[in] src_id    ID of the sending task instance.
 *
 ****************************************************************************************
 */
static void osapp_gattc_read_req_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    uint8_t i=0;
    for(i=0;i<peripheral_env.svc.max_num;i++)
    {
        if((&peripheral_env.svc.gattcbs[i] != NULL) && (peripheral_env.svc.gattcbs[i].pfnReadAttrCB != NULL))
        {
            peripheral_env.svc.gattcbs[i].pfnReadAttrCB(msgid,param,dest_id,src_id);
        }
    }
}

/**
 ****************************************************************************************
 * @brief callback when receive GAPC_DISCONNECT_IND, indicate the connect has broken off. And normally, we should restart advertising when connection break off.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAPM).
 * @param[in] src_id    ID of the sending task instance.
 *
 ****************************************************************************************
 */
static void osapp_gapc_disconnect_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapc_disconnect_ind const *ind = param;

    LOG(LOG_LVL_INFO,"Connect lost 0x%X \n",ind->conhdl);
    LOG(LOG_LVL_INFO,"Disconnect Reason 0x%X \n",ind->reason);
    osapp_connect_states_updata(IS_DISCONNECTED);
    osapp_start_advertising();
}
/**
 ****************************************************************************************
 * @brief conect / disconnect call back. Do different things according to the handle.
 *
 * @param[in] is_connect  0 is disconnect. 1 is connect.
 *
 ****************************************************************************************
 */
static void osapp_connect_states_updata(uint8_t is_connect)
{
    uint8_t i=0;
    for(i=0;i<peripheral_env.svc.max_num;i++)
    {
        if((&peripheral_env.svc.gattcbs[i] != NULL) && (peripheral_env.svc.gattcbs[i].pfnConnectCB != NULL))
        {
            peripheral_env.svc.gattcbs[i].pfnConnectCB(is_connect);
        }
    }
}
/**
 ****************************************************************************************
 * @brief callback when gattc event has completed.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAPM).
 * @param[in] src_id    ID of the sending task instance.
 *
 ****************************************************************************************
 */
static void osapp_gattc_cmp_evt_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO, "osapp_gattc_cmp_evt_handler.\n");
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
 ****************************************************************************************
 * @brief handler of gapm complete message. Callback when receive GAPM_CMP_EVT
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAPM).
 * @param[in] src_id    ID of the sending task instance.
 *
 ****************************************************************************************
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
            osapp_user_main_init();
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
            osapp_add_user_svc(peripheral_env.svc.index);
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
            //osapp_exc_mtu();
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
        break;
    default:
        LOG(LOG_LVL_WARN,"gapm_cmp_evt operation:0x%x\n",cmp_evt->operation);
        break;

    }

}

/**
 * @brief message and handler table. This define the connection of message and it's callback.
 */
static const osapp_msg_handler_table_t handler_table[]=
{
    [0] =   {KE_MSG_DEFAULT_HANDLER,(osapp_msg_handler_t)osapp_default_msg_handler},
       ///connection indicate: receive connect request from master
       {GAPC_CONNECTION_REQ_IND,(osapp_msg_handler_t)osapp_gapc_conn_req_ind_handler},
       ///connection lost indicate handler
       {GAPC_DISCONNECT_IND,(osapp_msg_handler_t)osapp_gapc_disconnect_ind_handler},
       ///GAPM event complete
       {GAPM_CMP_EVT,(osapp_msg_handler_t)osapp_gapm_cmp_evt_handler},
       ///ble power on ready and should do a reset
       {GAPM_DEVICE_READY_IND,(osapp_msg_handler_t)osapp_device_ready_ind_handler},
       ///trigger when master need to read device information uuid 0x1800
       {GAPC_GET_DEV_INFO_REQ_IND,(osapp_msg_handler_t)osapp_gapc_get_dev_info_req_ind_handler},
       ///add service complete and store service handler
       {GATTM_ADD_SVC_RSP,(osapp_msg_handler_t)osapp_gattm_add_svc_rsp_handler},
       ///master write data to device
       {GATTC_WRITE_REQ_IND,(osapp_msg_handler_t)osapp_gattc_write_req_ind_handler},
       ///master read data from device
       {GATTC_READ_REQ_IND,(osapp_msg_handler_t)osapp_gattc_read_req_ind_handler},
       ///gattc event has completed
       {GATTC_CMP_EVT,(osapp_msg_handler_t)osapp_gattc_cmp_evt_handler},
};

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
const osapp_msg_handler_info_t handler_info = ARRAY_INFO(handler_table);

/// @} BLE_Peripheral

#endif/* BLE_PERIPHERAL_SIMPLE */

