/**
 ****************************************************************************************
 *
 * @file ble_observer.c
 *
 * @brief BLE broadcaster Demo Config Internal.
 *
 * Copyright (C) Apollo 2018-2023
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup BLE_OBSERVER  BLE Observer Demo
 * @ingroup BLE_DEMO
 * @brief defines for BLE observer demo
 *
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "ble_observer.h"

#ifdef OSAPP_BLE_OBSERVER

#include "ble_user_main.h"


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */



/*
 * DEFINES
 ****************************************************************************************
 */
///App device name
#define APP_DEVICE_NAME "Apollo Observer"

/// Maximal MTU acceptable for device
#define BLE_L2CAP_MAX_MTU  23
/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */


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
static void osapp_gapm_adv_report_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static int32_t osapp_start_scan(void);
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
     osapp_set_dev_config(GAP_ROLE_OBSERVER,GAPM_CFG_ADDR_PUBLIC,GAPM_PAIRING_LEGACY,BLE_L2CAP_MAX_MTU);
     LOG(LOG_LVL_INFO,"osapp_ble_stack_run\n");
}
/**
 * @brief ble stack data init.
 *
 */
static void osapp_ble_stack_data_init(void)
{

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

    cmd->mode = GAP_OBSERVER_MODE;
    cmd->interval = 0x20;
    cmd->window = 0x20;

    return osapp_msg_build_send(cmd, sizeof(struct gapm_start_scan_cmd));
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
        default: /* Do Nothing */
            break;
    }
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
            osapp_start_scan();
        }
        else
        {
            LOG(LOG_LVL_INFO,"Set Role Failed\n");
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
       ///triggered when scanning operation of selective connection establishment procedure receive advertising report information.
       {GAPM_ADV_REPORT_IND,osapp_gapm_adv_report_ind_handler},
};

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
const osapp_msg_handler_info_t handler_info = ARRAY_INFO(handler_table);

/// @} BLE_OBSERVER

#endif/* OSAPP_BLE_OBSERVER */


