/**
 ****************************************************************************************
 *
 * @file ble_central.c
 *
 * @brief BLE ble_central Demo Config Internal.
 *
 * Copyright (C) Apollo 2018-2023
 *
 ****************************************************************************************
 */

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
#include "ble_central.h"

#ifdef BLE_CENTRAL_SIMPLE

#include "ble_user_main.h"
#include "ble_apollo_00_simple_c.h"

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
/// set response data when active scan, maximum 29bytes long
#define OSAPP_SCNRSP_DATA         ("APOLLO_00-ADVRSP")
/// set response data length, maximum 29bytes long
#define OSAPP_SCNRSP_DATA_LEN     (sizeof(OSAPP_SCNRSP_DATA))

#define OSAPP_SCNRSP_DATA_CHG         ("APOLLO_00-ADVCHG")
#define OSAPP_SCNRSP_DATA_CHG_LEN     (sizeof(OSAPP_SCNRSP_DATA_CHG))


#define OSAPP_DFLT_DEVICE_NAME            ("APOLLO_00-CHG0")
#define OSAPP_DFLT_DEVICE_NAME_LEN        (sizeof(OSAPP_DFLT_DEVICE_NAME))

#define OSAPP_DEVICE_NAME_CHG1            ("APOLLO_00-CHG1")
#define OSAPP_DEVICE_NAME_CHG1_LEN        (sizeof(OSAPP_DEVICE_NAME_CHG1))

#define OSAPP_DEVICE_NAME_CHG2            ("APOLLO_00-CHG2")
#define OSAPP_DEVICE_NAME_CHG2_LEN        (sizeof(OSAPP_DEVICE_NAME_CHG2))

#define OSAPP_DEVICE_NAME_CHG3            ("APOLLO_00-CHG3")
#define OSAPP_DEVICE_NAME_CHG3_LEN        (sizeof(OSAPP_DEVICE_NAME_CHG3))

#define OSAPP_DEVICE_NAME_CHG4            ("APOLLO_00-CHG4")
#define OSAPP_DEVICE_NAME_CHG4_LEN        (sizeof(OSAPP_DEVICE_NAME_CHG4))

#define OSAPP_BX_ADV_DATA_UUID        "\x03\x03\x00\x24"
#define OSAPP_BX_ADV_DATA_UUID_LEN    (4)
#define OSAPP_DATA_TOTAL_LEN        (ADV_DATA_LEN-3)    // 3 bytes for ad type flags
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
/// Update adv data period    1000ms
#define APP_ADV_UPDATE_PERIOD    pdMS_TO_TICKS(1000)

///App device name
#define APP_DEVICE_NAME "Apollo Observer"

/// Maximal MTU acceptable for device
#define BLE_L2CAP_MAX_MTU  23

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




#define CENTRAL_FILTER_BDADDR  0x99,0x22,0x33,0x44,0x55,0x66
/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
static ble_central_env_t central_env;

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
static void osapp_gapc_conn_req_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static void osapp_gapc_disconnect_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static int32_t osapp_gapc_conn_confirm(ke_task_id_t dest_id);
static void osapp_gattc_cmp_evt_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static void osapp_gapc_cmp_evt_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static int32_t osapp_add_user_profiles(uint8_t index);
static void osapp_gapm_profile_added_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static int32_t osapp_start_scan(void);
static void osapp_gapm_adv_report_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static void osapp_find_device(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static int32_t osapp_stop_scan(void);
static void osapp_start_data_communication(void);
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
     osapp_set_dev_config(GAP_ROLE_CENTRAL,GAPM_CFG_ADDR_PUBLIC,GAPM_PAIRING_LEGACY,BLE_L2CAP_MAX_MTU);
     LOG(LOG_LVL_INFO,"osapp_ble_stack_run\n");
}
/**
 * @brief ble stack data init.
 *
 */
static void osapp_ble_stack_data_init(void)
{
    memset(&central_env, 0 , sizeof(ble_central_env_t));
    central_env.svc.max_num = BLE_CENTRAL_SERVICES_NUM;
    central_env.svc.index = 0;
    central_env.svc.handles[PRF_BX_SIMPLES_SERVICE_ID] = ble_bx_simple_prf_add_svc;
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
 * @brief gapc event complete.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAPM).
 * @param[in] src_id    ID of the sending task instance.
 *
 ****************************************************************************************
 */
static void osapp_gapc_cmp_evt_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"GAPC_CMP_EVT\n");
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
    struct gattc_cmp_evt *evt = (struct gattc_cmp_evt *)param;
    switch(evt->operation)
    {
        case GATTC_READ:
            if(evt->status == GAP_ERR_NO_ERROR)
                LOG(LOG_LVL_INFO, "GATTC_READ complete.\n");
            break;
        default:
            break;
    }
}
/**
 ****************************************************************************************
 * @brief callback when receive GAPM_PROFILE_ADDED_IND, indicate that profile has been added.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAPM).
 * @param[in] src_id    ID of the sending task instance.
 *
 ****************************************************************************************
 */
static void osapp_gapm_profile_added_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapm_profile_added_ind *ind = (struct gapm_profile_added_ind *)param;

    LOG(LOG_LVL_INFO,"osapp_gapm_profile_added_ind_handler\n");

    central_env.svc.start_hdl[central_env.svc.index]=ind->start_hdl;
    /// update start handler
    if((&central_env.svc.gattcbs[central_env.svc.index] != NULL) && (central_env.svc.gattcbs[central_env.svc.index].pfnHandlerInitCB != NULL))
    {
        central_env.svc.gattcbs[central_env.svc.index].pfnHandlerInitCB(ind->start_hdl,ind->prf_task_id,ind->prf_task_nb);
    }

    central_env.svc.index++;//updata svc index
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
//    memcpy(&peripheral_env.conn_info.device_addr,&ind->peer_addr,sizeof(bd_addr_t));
//    peripheral_env.conn_info.conn_idx = src_id;
//    peripheral_env.conn_info.valid = 1;

    osapp_gapc_conn_confirm(src_id);
    LOG(LOG_LVL_INFO,"Connect comfirm\n");
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
}
/**
 ****************************************************************************************
 * @brief add service and characteristics in client database
 *
 * @param[in] index  now which svc should add
 *
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 *
 ****************************************************************************************
 */
static int32_t osapp_add_user_profiles(uint8_t index)
{
    if(central_env.svc.index < central_env.svc.max_num)
    {
        prf_add_svc_handler_t p_add_svc_func = central_env.svc.handles[central_env.svc.index];
        return p_add_svc_func((central_env.svc.gattcbs+central_env.svc.index));
    }
    else
    {
        LOG(LOG_LVL_INFO,"prf client add done\n");
        osapp_start_scan();
        return 0;
    }
}
/**
 ****************************************************************************************
 * @brief configure and start scan
 *
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 *
 ****************************************************************************************
 */
static int32_t osapp_start_scan(void)
{
    struct gapm_start_scan_cmd *cmd = AHI_MSG_ALLOC(GAPM_START_SCAN_CMD,TASK_ID_GAPM,gapm_start_scan_cmd);
    cmd->op.code = GAPM_SCAN_PASSIVE;

    cmd->mode = GAP_GEN_DISCOVERY;
    cmd->interval = 0x20;
    cmd->window = 0x20;

    return osapp_msg_build_send(cmd, sizeof(struct gapm_start_scan_cmd));
}
/**
 ****************************************************************************************
 * @brief configure and stop scan
 *
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 *
 ****************************************************************************************
 */
static int32_t osapp_stop_scan(void)
{
    struct gapm_cancel_cmd *cmd = AHI_MSG_ALLOC(GAPM_CANCEL_CMD,TASK_ID_GAPM,gapm_cancel_cmd);
    cmd->operation = GAPM_CANCEL;
    return osapp_msg_build_send(cmd, sizeof(struct gapm_cancel_cmd));
}
/**
 ****************************************************************************************
 * @brief find devices match service uuid filter
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAPM).
 * @param[in] src_id    ID of the sending task instance.
 *
 ****************************************************************************************
 */
static void osapp_find_device(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct adv_report const *report = param;

    bd_addr_t filter_addr={
            .addr = {CENTRAL_FILTER_BDADDR}
    };

    if(memcmp(filter_addr.addr ,report->adv_addr.addr , GAP_BD_ADDR_LEN) == 0)//match
    {
        LOG(LOG_LVL_INFO,"find the device :\n");
        osapp_stop_scan();
    }
}
/**
 ****************************************************************************************
 * @brief connect to peer device
 *
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 *
 ****************************************************************************************
 */
static int32_t  osapp_connect(void)
{
    struct gap_bdaddr addr;
    struct gapm_start_connection_cmd conn_cmd;
    uint8_t  connect_addr[GAP_BD_ADDR_LEN] = {CENTRAL_FILTER_BDADDR};

    memset(&addr,0,sizeof(addr));
    memset(&conn_cmd,0,sizeof(conn_cmd));

    conn_cmd.op.code = GAPM_CONNECTION_DIRECT;
    conn_cmd.op.addr_src = GAPM_STATIC_ADDR;
    conn_cmd.scan_interval = 0x20;
    conn_cmd.scan_window = 0x20;

    conn_cmd.con_intv_max = 0x6;
    conn_cmd.con_intv_min = 0x6;
    conn_cmd.con_latency = 0;
    conn_cmd.superv_to = 0x2a;

    conn_cmd.ce_len_max = 0x0;
    conn_cmd.ce_len_min = 0x0;

    conn_cmd.nb_peers = 1;

    addr.addr_type = ADDR_PUBLIC;
    memcpy(addr.addr.addr,connect_addr,GAP_BD_ADDR_LEN);

    struct gapm_start_connection_cmd* new_cmd = AHI_MSG_ALLOC_DYN(GAPM_START_CONNECTION_CMD,
                                                                     TASK_ID_GAPM,
                                                                     gapm_start_connection_cmd, sizeof(struct gap_bdaddr));

    memcpy(new_cmd,&conn_cmd,sizeof(struct gapm_start_connection_cmd));

    memcpy(new_cmd->peers,&addr,sizeof(struct gap_bdaddr));

    return osapp_msg_build_send(new_cmd, sizeof(struct gapm_start_connection_cmd) + sizeof(struct gap_bdaddr) );
}
/**
 ****************************************************************************************
 * @brief start data communication over air
 *
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 *
 ****************************************************************************************
 */
static void osapp_start_data_communication(void)
{

}
/**
 ****************************************************************************************
 * @brief handler of advertising report information, callback when receive GAPM_ADV_REPORT_IND
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAPM).
 * @param[in] src_id    ID of the sending task instance.
 *
 ****************************************************************************************
 */
static void osapp_gapm_adv_report_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct adv_report const *report = param;
    uint8_t i=0;

    switch(report->evt_type)
    {
    case ADV_CONN_UNDIR:
        LOG(LOG_LVL_INFO,"ADV_CONN_UNDIR\n");
        break;
    case ADV_CONN_DIR:
        LOG(LOG_LVL_INFO,"ADV_CONN_DIR\n");
        break;
    case ADV_DISC_UNDIR:
        LOG(LOG_LVL_INFO,"ADV_DISC_UNDIR\n");
        break;
    case ADV_NONCONN_UNDIR:
        LOG(LOG_LVL_INFO,"ADV_NONCONN_UNDIR\n");
        break;
    case ADV_CONN_DIR_LDC:
        LOG(LOG_LVL_INFO,"ADV_CONN_DIR_LDC\n");
        break;

    default:
        LOG(LOG_LVL_WARN,"NO THIS TYPE ADV");
        break;
    }

    LOG(LOG_LVL_INFO,"adv_addr_type : %d \n",report->adv_addr_type);//GAPM_CFG_ADDR_PUBLIC
    LOG(LOG_LVL_INFO,"adv_addr: ");
    for(i=0;i<GAP_BD_ADDR_LEN;i++)
        LOG(LOG_LVL_INFO,":0x%02X",report->adv_addr.addr[i]);//GAPM_CFG_ADDR_PUBLIC

    LOG(LOG_LVL_INFO,"\n ADV len : %d \n  ADV data ",report->data_len);
    for(uint8_t i=0;i<report->data_len;i++)
        LOG(LOG_LVL_INFO,":0x%02X",report->data[i]);

    LOG(LOG_LVL_INFO,"\n ADV RSSI : %d \n",report->rssi);

    osapp_find_device(msgid,param,dest_id,src_id);
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

    LOG(LOG_LVL_INFO,"gapm_cmp_evt status: 0x%x \n",cmp_evt->status);
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
            osapp_add_user_profiles(central_env.svc.index);
            LOG(LOG_LVL_INFO,"Add Profile\n");
        }
        else
        {
            LOG(LOG_LVL_INFO,"Set Role Failed\n");
        }
        break;
    case GAPM_SCAN_PASSIVE:
    case GAPM_SCAN_ACTIVE:
        {
            LOG(LOG_LVL_INFO,"Scan Done.\n");
            osapp_connect();
        }
        break;
    case GAPM_CONNECTION_DIRECT:
        if(cmp_evt->status == GAP_ERR_NO_ERROR)
        {
            LOG(LOG_LVL_INFO,"Connect Complete\n");
            osapp_start_data_communication();
        }
        else
        {
            LOG(LOG_LVL_INFO,"Connect Failed\n");
        }
        break;
    case GAPM_PROFILE_TASK_ADD:
        if(cmp_evt->status == GAP_ERR_NO_ERROR)
        {
            LOG(LOG_LVL_INFO,"GAPM PROFILE TASK ADD\n");
            osapp_add_user_profiles(central_env.svc.index);//add next svc
        }
        else
        {
            LOG(LOG_LVL_INFO,"GAPM PROFILE TASK ADD Failed\n");
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
        ///GAPC event complete
        {GAPC_CMP_EVT,(osapp_msg_handler_t)osapp_gapc_cmp_evt_handler},
        ///This command is used to allocate a task for specific profile (service or client).
        {GAPM_PROFILE_ADDED_IND,osapp_gapm_profile_added_ind_handler},
        ///triggered when scanning operation of selective connection establishment procedure receive advertising report information.
        {GAPM_ADV_REPORT_IND,osapp_gapm_adv_report_ind_handler},
        ///gattc event has completed
        {GATTC_CMP_EVT,(osapp_msg_handler_t)osapp_gattc_cmp_evt_handler},

};

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
const osapp_msg_handler_info_t handler_info = ARRAY_INFO(handler_table);

/// @} BLE_CENTRAL

#endif/* BLE_CENTRAL_SIMPLE */



