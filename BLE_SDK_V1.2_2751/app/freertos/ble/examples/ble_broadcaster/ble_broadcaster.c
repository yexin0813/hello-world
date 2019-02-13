/**
 ****************************************************************************************
 *
 * @file ble_broadcaster.c
 *
 * @brief BLE broadcaster Demo Config Internal.
 *
 * Copyright (C) Apollo 2018-2023
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup BLE_BROADCASTER  BLE Broadcaster Demo
 * @ingroup BLE_DEMO
 * @brief defines for BLE broadcaster demo
 *
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "ble_broadcaster.h"

#ifdef OSAPP_BLE_BROADCASTER

#include "ble_user_main.h"


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
#define APP_DEVICE_NAME "Apollo ADV"


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
static ble_broadcaster_env_t broadcaster_env;

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
static void osapp_advdata_change_handler(TimerHandle_t xTimer);
static int32_t osapp_update_advertise_data(uint8_t index);
static int32_t osapp_start_advertising(void);

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
    broadcaster_env.Timer =  xTimerCreate("ADVTimer",APP_ADV_UPDATE_PERIOD,pdTRUE,(void *) 0,osapp_advdata_change_handler);
    BX_ASSERT(broadcaster_env.Timer!=NULL);

    broadcaster_env.test_num = 0;
}
/**
 * @brief ble osapp advdata change timer handler
 *
 * @param[in] xTimer  reference freertos
 */
static void osapp_advdata_change_handler(TimerHandle_t xTimer)
{
    osapp_update_advertise_data(++broadcaster_env.test_num);
}
/**
 ****************************************************************************************
 * @brief update adv data.
 *
 * @param[in] index     by index to select which adv data pdu should send to air.
 *
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 *
 ****************************************************************************************
 */
static int32_t osapp_update_advertise_data(uint8_t index)
{
    struct gapm_update_advertise_data_cmd *cmd = AHI_MSG_ALLOC(GAPM_UPDATE_ADVERTISE_DATA_CMD,TASK_ID_GAPM, gapm_update_advertise_data_cmd);

    cmd->operation = GAPM_UPDATE_ADVERTISE_DATA;
    cmd->adv_data_len = 0;

    memcpy(&cmd->adv_data[cmd->adv_data_len],
           OSAPP_BX_ADV_DATA_UUID, OSAPP_BX_ADV_DATA_UUID_LEN);
    cmd->adv_data_len += OSAPP_BX_ADV_DATA_UUID_LEN;

    cmd->adv_data[cmd->adv_data_len] = OSAPP_DATA_TOTAL_LEN-2-OSAPP_BX_ADV_DATA_UUID_LEN+1;
    cmd->adv_data[cmd->adv_data_len+1] = '\x08';

    index %= 4;//(0~3)
    index +=1;//(1~4)

    switch (index)
    {
        case 1:
            LOG(LOG_LVL_INFO,"change1\n");
            memcpy(&cmd->adv_data[6],OSAPP_DEVICE_NAME_CHG1,OSAPP_DEVICE_NAME_CHG1_LEN);
            cmd->adv_data_len = OSAPP_DATA_TOTAL_LEN;
            break;
        case 2:
            LOG(LOG_LVL_INFO,"change2\n");
            memcpy(&cmd->adv_data[6],OSAPP_DEVICE_NAME_CHG2,OSAPP_DEVICE_NAME_CHG2_LEN);
            cmd->adv_data_len = OSAPP_DATA_TOTAL_LEN;
            break;
        case 3:
            LOG(LOG_LVL_INFO,"change3\n");
            memcpy(&cmd->adv_data[6],OSAPP_DEVICE_NAME_CHG3,OSAPP_DEVICE_NAME_CHG3_LEN);
            cmd->adv_data_len = OSAPP_DATA_TOTAL_LEN;
            break;
        case 4:
            LOG(LOG_LVL_INFO,"change4\n");
            memcpy(&cmd->adv_data[6],OSAPP_DEVICE_NAME_CHG4,OSAPP_DEVICE_NAME_CHG4_LEN);
            cmd->adv_data_len = OSAPP_DATA_TOTAL_LEN;
            break;
        default:
            break;
    }


    cmd->scan_rsp_data_len  = OSAPP_SCNRSP_DATA_CHG_LEN;
    memcpy(&cmd->scan_rsp_data[0],OSAPP_SCNRSP_DATA_CHG,cmd->scan_rsp_data_len);

    return osapp_msg_build_send(cmd,sizeof(struct gapm_update_advertise_data_cmd));

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

//    memset(cmd, 0 , sizeof(struct gapm_start_advertise_cmd));

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

    xTimerStart(broadcaster_env.Timer,0);

    return osapp_msg_build_send(cmd,sizeof(struct gapm_start_advertise_cmd));
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
            osapp_start_advertising();
        }
        else
        {
            LOG(LOG_LVL_INFO,"Set Role Failed\n");
        }
        break;
    case GAPM_ADV_NON_CONN:
        if(cmp_evt->status == GAP_ERR_NO_ERROR)
        {
            LOG(LOG_LVL_INFO,"ADV non connect End\n");
        }
        else
        {
            LOG(LOG_LVL_INFO,"ADV non connect End Failed\n");
        }
        break;
    case GAPM_UPDATE_ADVERTISE_DATA:
        if(cmp_evt->status == GAP_ERR_NO_ERROR)
        {
            LOG(LOG_LVL_INFO,"ADV Update End\n");
        }
        else
        {
            LOG(LOG_LVL_INFO,"ADV Update End Failed\n");
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
       ///GAPM event complete
       {GAPM_CMP_EVT,(osapp_msg_handler_t)osapp_gapm_cmp_evt_handler},
       ///ble power on ready and should do a reset
       {GAPM_DEVICE_READY_IND,(osapp_msg_handler_t)osapp_device_ready_ind_handler},
       ///trigger when master need to read device information uuid 0x1800
       {GAPC_GET_DEV_INFO_REQ_IND,(osapp_msg_handler_t)osapp_gapc_get_dev_info_req_ind_handler},
};

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
const osapp_msg_handler_info_t handler_info = ARRAY_INFO(handler_table);

/// @} BLE_BROADCASTER

#endif/* OSAPP_BLE_BROADCASTER */


