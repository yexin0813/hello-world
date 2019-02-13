//#include "osapp_config.h"

//#ifdef OSAPP_DIS_SERVER

#include "mcu_master_ahi.h"
#include "diss_task.h"
#include "bxotas_task.h"
#include "app_uart.h"
#include "mcu_master_nvds_sleep.h"
#include "plf.h"
//#define ENABLE_UART

#define APP_ADV_CHMAP 0x7
//#define APP_ADV_INT_MIN 32 //20 ms
//#define APP_ADV_INT_MAX 32 //20 ms
#define APP_ADV_INT_MIN 160 //100 ms
#define APP_ADV_INT_MAX 160 //100 ms
//#define APP_ADV_INT_MIN 2048 //1.28 s
//#define APP_ADV_INT_MAX 2048 //1.28 s
#define CONN_INTV 40 //50ms

/// Manufacturer Name Value
#define APP_DIS_MANUFACTURER_NAME       ("APOLLO Microelectronics")
#define APP_DIS_MANUFACTURER_NAME_LEN   (sizeof(APP_DIS_MANUFACTURER_NAME))

/// ADV Data
#define APP_DIS_ADV_DATA                ("APOLLO Dis Server")

/// Model Number String Value
#define APP_DIS_MODEL_NB_STR            ("RW-BLE-1.0")
#define APP_DIS_MODEL_NB_STR_LEN        (10)

/// Serial Number
#define APP_DIS_SERIAL_NB_STR           ("1.0.0.0-LE")
#define APP_DIS_SERIAL_NB_STR_LEN       (10)

/// Firmware Revision
#define APP_DIS_FIRM_REV_STR            ("6.1.2")
#define APP_DIS_FIRM_REV_STR_LEN        (5)

/// System ID Value - LSB -> MSB
#define APP_DIS_SYSTEM_ID               ("\x12\x34\x56\xFF\xFE\x9A\xBC\xDE")
#define APP_DIS_SYSTEM_ID_LEN           (8)

/// Hardware Revision String
#define APP_DIS_HARD_REV_STR           ("1.0.0")
#define APP_DIS_HARD_REV_STR_LEN       (5)

/// Software Revision String
#define APP_DIS_SW_REV_STR              ("6.3.0")
#define APP_DIS_SW_REV_STR_LEN          (5)

/// IEEE
#define APP_DIS_IEEE                    ("\xFF\xEE\xDD\xCC\xBB\xAA")
#define APP_DIS_IEEE_LEN                (6)

/**
 * PNP ID Value - LSB -> MSB
 *      Vendor ID Source : 0x02 (USB Implementer’s Forum assigned Vendor ID value)
 *      Vendor ID : 0x045E      (Microsoft Corp)
 *      Product ID : 0x0040
 *      Product Version : 0x0300
 */
#define APP_DIS_PNP_ID               ("\x02\x5E\x04\x40\x00\x00\x03")
#define APP_DIS_PNP_ID_LEN           (7)
#ifdef ENABLE_UART
static app_uart_inst_t uart_inst = UART_INSTANCE(0);  /**< UART0 instance. */
#endif
static int32_t osapp_reset()
{
    struct gapm_reset_cmd *cmd = AHI_MSG_ALLOC(GAPM_RESET_CMD,TASK_ID_GAPM,gapm_reset_cmd);
    cmd->operation = GAPM_RESET;
    return osapp_msg_build_send(cmd, sizeof(struct gapm_reset_cmd));
}
static int32_t osapp_set_dev_config(uint8_t role,uint8_t addr_type,uint8_t pairing_mode)
{
    // Set Device configuration
    struct gapm_set_dev_config_cmd* cmd = AHI_MSG_ALLOC(GAPM_SET_DEV_CONFIG_CMD,TASK_ID_GAPM,gapm_set_dev_config_cmd);
    cmd->operation = GAPM_SET_DEV_CONFIG;
    cmd->role      = role;
   
    // Set Data length parameters
    cmd->sugg_max_tx_octets = BLE_MIN_OCTETS;
    cmd->sugg_max_tx_time   = BLE_MIN_TIME;
    cmd->att_cfg = GAPM_MASK_ATT_SVC_CHG_EN | GAPM_MASK_ATT_SLV_PREF_CON_PAR_EN;
    cmd->addr_type = addr_type;
    cmd->pairing_mode = pairing_mode;
    cmd->max_mtu = GAP_MAX_LE_MTU;
    return osapp_msg_build_send(cmd, sizeof(struct gapm_set_dev_config_cmd) );
}
static int32_t osapp_add_dis_server_task()
{
    struct diss_db_cfg* db_cfg;
    // Allocate the DISS_CREATE_DB_REQ
    struct gapm_profile_task_add_cmd *req = AHI_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,TASK_ID_GAPM, gapm_profile_task_add_cmd, sizeof(struct diss_db_cfg));
    // Fill message
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl = PERM(SVC_AUTH, NO_AUTH);
    req->prf_task_id = TASK_ID_DISS;
    req->app_task = TASK_AHI;
    req->start_hdl = 0;

    // Set parameters
    db_cfg = (struct diss_db_cfg* ) req->param;
    db_cfg->features = DIS_ALL_FEAT_SUP;
    return osapp_msg_build_send(req,sizeof(struct gapm_profile_task_add_cmd)+sizeof(struct diss_db_cfg));

}
static int32_t osapp_add_bxotas_task()
{
    struct gapm_profile_task_add_cmd *req=AHI_MSG_ALLOC(GAPM_PROFILE_TASK_ADD_CMD, TASK_ID_GAPM, gapm_profile_task_add_cmd);
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl = PERM(SVC_AUTH,NO_AUTH);
    req->prf_task_id = TASK_ID_BXOTAS;
    req->app_task = TASK_AHI;
    req->start_hdl = 0;
    return osapp_msg_build_send(req,sizeof(struct gapm_profile_task_add_cmd));
}
static int32_t osapp_start_advertising()
{
        // Prepare the GAPM_START_ADVERTISE_CMD message
    struct gapm_start_advertise_cmd *cmd = AHI_MSG_ALLOC(GAPM_START_ADVERTISE_CMD,TASK_ID_GAPM, gapm_start_advertise_cmd);

    cmd->op.addr_src    = GAPM_STATIC_ADDR;
    cmd->channel_map    = APP_ADV_CHMAP;
    cmd->intv_min = APP_ADV_INT_MIN;
    cmd->intv_max = APP_ADV_INT_MAX;
    cmd->op.code        = GAPM_ADV_UNDIRECT;
    cmd->info.host.mode = GAP_GEN_DISCOVERABLE;
    cmd->info.host.adv_data_len = ADV_DATA_PACK(cmd->info.host.adv_data,1,GAP_AD_TYPE_COMPLETE_NAME,\
            APP_DIS_ADV_DATA,sizeof(APP_DIS_ADV_DATA));
            // Flag value is set by the GAP
    cmd->info.host.adv_data_len       = ADV_DATA_LEN - 3;
    cmd->info.host.scan_rsp_data_len  = SCAN_RSP_DATA_LEN;
//    nvds_get(NVDS_TAG_APP_BLE_ADV_DATA, &cmd->info.host.adv_data_len,                    &cmd->info.host.adv_data[0]) ;
//    nvds_get(NVDS_TAG_APP_BLE_SCAN_RESP_DATA, &cmd->info.host.scan_rsp_data_len,                      &cmd->info.host.scan_rsp_data[0]);
    return osapp_msg_build_send(cmd,sizeof(struct gapm_start_advertise_cmd));
        
}

static int32_t osapp_gapc_conn_confirm(ke_task_id_t dest_id)
{
    struct gapc_connection_cfm *cfm = AHI_MSG_ALLOC(GAPC_CONNECTION_CFM, dest_id, gapc_connection_cfm);
    cfm->auth = GAP_AUTH_REQ_NO_MITM_NO_BOND;
    return osapp_msg_build_send(cfm,sizeof(struct gapc_connection_cfm));
}
static int32_t osapp_get_dev_name(ke_task_id_t const dest_id)
{
    nvds_tag_len_t device_name_length = NVDS_LEN_DEVICE_NAME;
    struct gapc_get_dev_info_cfm *cfm=AHI_MSG_ALLOC_DYN(GAPC_GET_DEV_INFO_CFM, dest_id,gapc_get_dev_info_cfm, device_name_length);
    cfm->req = GAPC_DEV_NAME;
//    nvds_get(NVDS_TAG_DEVICE_NAME, &device_name_length, cfm->info.name.value);
    memcpy(cfm->info.name.value , "6666666牛掰6666666" , 20);
    cfm->info.name.length = device_name_length;
    return osapp_msg_build_send(cfm, sizeof(struct gapc_get_dev_info_cfm)+device_name_length);
}
static int32_t osapp_get_dev_appearance(ke_task_id_t const dest_id)
{
    struct gapc_get_dev_info_cfm *cfm=AHI_MSG_ALLOC(GAPC_GET_DEV_INFO_CFM, dest_id,gapc_get_dev_info_cfm);
    cfm->req = GAPC_DEV_APPEARANCE;
    cfm->info.appearance = 0;
    return osapp_msg_build_send(cfm, sizeof(struct gapc_get_dev_info_cfm));
}
static int32_t osapp_get_dev_slv_pref_params(ke_task_id_t const dest_id)
{
    struct gapc_get_dev_info_cfm *cfm=AHI_MSG_ALLOC(GAPC_GET_DEV_INFO_CFM, dest_id,gapc_get_dev_info_cfm);
    cfm->req = GAPC_DEV_SLV_PREF_PARAMS;
    cfm->info.slv_params.con_intv_min = 8;
    cfm->info.slv_params.con_intv_max = 10;
    cfm->info.slv_params.slave_latency = 0;
    cfm->info.slv_params.conn_timeout = 200;
    return osapp_msg_build_send(cfm, sizeof(struct gapc_get_dev_info_cfm));

}

static int32_t osapp_gapc_param_update_cfm(ke_task_id_t const src_id)
{
    struct gapc_param_update_cfm *cfm = AHI_MSG_ALLOC(GAPC_PARAM_UPDATE_CFM, src_id, gapc_param_update_cfm);
    cfm->accept = 0x01;
    cfm->ce_len_max = 0xffff;
    cfm->ce_len_min = 0xffff;
    return osapp_msg_build_send(cfm, sizeof(struct gapc_param_update_cfm));

}


static void osapp_device_ready_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    mcu_nvds_set_data();
    osapp_reset();

    #ifdef ENABLE_UART
    uart_inst.param.baud_rate = UART_BAUDRATE_9600;
    uart_inst.param.rx_pin_no = 13;
    uart_inst.param.tx_pin_no = 12;
    uart_inst.param.tx_dma = 1;
    uart_inst.param.rx_dma = 1;
    app_uart_init(&uart_inst.inst);
    #endif
}


static void osapp_gapc_param_update_req_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"param update request\n");
    osapp_gapc_param_update_cfm(src_id);
}


static void osapp_gapm_cmp_evt_handler(ke_msg_id_t const msgid, struct gapm_cmp_evt const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{    
    struct gapm_cmp_evt const *cmp_evt = param;
    switch(cmp_evt->operation)
    {
    case GAPM_RESET:
        BX_ASSERT(cmp_evt->status==GAP_ERR_NO_ERROR);
        osapp_set_dev_config(GAP_ROLE_PERIPHERAL,GAPM_CFG_ADDR_PUBLIC,GAPM_PAIRING_LEGACY);
        break;
    case GAPM_SET_DEV_CONFIG:
        BX_ASSERT(cmp_evt->status==GAP_ERR_NO_ERROR);
        osapp_add_dis_server_task();
        break;
    case GAPM_PROFILE_TASK_ADD:
        BX_ASSERT(cmp_evt->status==GAP_ERR_NO_ERROR);
        LOG(LOG_LVL_INFO,"gapm cmp evt profile task add,status:0x%x\n",cmp_evt->status);
        break;
    case GAPM_ADV_UNDIRECT:
        LOG(LOG_LVL_INFO,"gapm_cmp_evt adv_ind,status:0x%x\n",cmp_evt->status);
        break;
    default:
        LOG(LOG_LVL_WARN,"gapm_cmp_evt operation:0x%x\n",cmp_evt->operation);
        break;
            
    }
}

static void osapp_bxotas_config()
{
    struct bxotas_firmware_dest_cmd *cmd =AHI_MSG_ALLOC(BXOTAS_FIRMWARE_DEST_CMD,TASK_ID_BXOTAS,bxotas_firmware_dest_cmd);
    cmd->firmware_dest = SERIAL_UART;
    osapp_ahi_msg_send(cmd,sizeof(struct bxotas_firmware_dest_cmd),portMAX_DELAY);

}

static void osapp_gapm_profile_added_ind_handler(ke_msg_id_t const msgid, struct gapm_profile_added_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"GAPM profile added indication,id:%d,nb:%d,hdl:%d\n",param->prf_task_id,param->prf_task_nb,param->start_hdl);
    if(param->prf_task_id == TASK_ID_DISS)
    {
        osapp_add_bxotas_task();
    }
    if(param->prf_task_id==TASK_ID_BXOTAS)
    {
        osapp_bxotas_config();
        osapp_start_advertising();
    }
}

static void osapp_param_update_req(ke_task_id_t dest_id)
{
    struct gapc_param_update_cmd *cmd = AHI_MSG_ALLOC(GAPC_PARAM_UPDATE_CMD,dest_id,gapc_param_update_cmd);
    cmd->operation = GAPC_UPDATE_PARAMS;
    cmd->intv_max = CONN_INTV;
    cmd->intv_min = CONN_INTV;
    cmd->latency = 0;
    cmd->time_out = 1000;
    osapp_ahi_msg_send(cmd,sizeof(struct gapc_param_update_cmd), portMAX_DELAY);
}

static void osapp_gapc_conn_req_ind_handler(ke_msg_id_t const msgid, struct gapc_connection_req_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    osapp_gapc_conn_confirm(src_id);
    LOG(LOG_LVL_INFO,"conidx:%d\n",param->conhdl);
    //osapp_param_update_req(src_id);
    //osapp_start_advertising();
}

static void osapp_defualt_msg_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"default handler,msgid: 0x%x, dest_id: 0x%x, src_id: 0x%x\n",msgid,dest_id,src_id);

}
static void osapp_gapc_disconnect_ind_handler(ke_msg_id_t const msgid, struct gapc_disconnect_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"disconn:%d\n",param->reason);
    osapp_start_advertising();
}
static void osapp_gapc_get_dev_info_req_ind_handler(ke_msg_id_t const msgid, struct gapc_get_dev_info_req_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapc_get_dev_info_req_ind const *req_ind =param;
    switch(req_ind->req)
    {
    case GAPC_DEV_NAME:
        osapp_get_dev_name(src_id);
        break;
    case GAPC_DEV_APPEARANCE:
        osapp_get_dev_appearance(src_id);
        break;
    case GAPC_DEV_SLV_PREF_PARAMS:
        osapp_get_dev_slv_pref_params(src_id);
        break;
    default :
        BX_ASSERT(0);
        break;
    }
}
static void osapp_diss_value_req_ind_handler(ke_msg_id_t const msgid,struct diss_value_req_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    // Initialize length
    uint8_t len = 0;
    // Pointer to the data
    uint8_t *data = NULL;

    // Check requested value
    switch (param->value)
    {
        case DIS_MANUFACTURER_NAME_CHAR:
        {
            // Set information
            len = APP_DIS_MANUFACTURER_NAME_LEN;
            data = (uint8_t *)APP_DIS_MANUFACTURER_NAME;
        } break;

        case DIS_MODEL_NB_STR_CHAR:
        {
            // Set information
            len = APP_DIS_MODEL_NB_STR_LEN;
            data = (uint8_t *)APP_DIS_MODEL_NB_STR;
        } break;

        case DIS_SYSTEM_ID_CHAR:
        {
            // Set information
            len = APP_DIS_SYSTEM_ID_LEN;
            data = (uint8_t *)APP_DIS_SYSTEM_ID;
        } break;

        case DIS_PNP_ID_CHAR:
        {
            // Set information
            len = APP_DIS_PNP_ID_LEN;
            data = (uint8_t *)APP_DIS_PNP_ID;
        } break;

        case DIS_SERIAL_NB_STR_CHAR:
        {
            // Set information
            len = APP_DIS_SERIAL_NB_STR_LEN;
            data = (uint8_t *)APP_DIS_SERIAL_NB_STR;
        } break;

        case DIS_HARD_REV_STR_CHAR:
        {
            // Set information
            len = APP_DIS_HARD_REV_STR_LEN;
            data = (uint8_t *)APP_DIS_HARD_REV_STR;
        } break;

        case DIS_FIRM_REV_STR_CHAR:
        {
            // Set information
            len = APP_DIS_FIRM_REV_STR_LEN;
            data = (uint8_t *)APP_DIS_FIRM_REV_STR;
        } break;

        case DIS_SW_REV_STR_CHAR:
        {
            // Set information
            len = APP_DIS_SW_REV_STR_LEN;
            data = (uint8_t *)APP_DIS_SW_REV_STR;
        } break;

        case DIS_IEEE_CHAR:
        {
            // Set information
            len = APP_DIS_IEEE_LEN;
            data = (uint8_t *)APP_DIS_IEEE;
        } break;

        default:
            BX_ASSERT(0);
            break;
    }

    // Allocate confirmation to send the value
    struct diss_value_cfm *cfm_value = AHI_MSG_ALLOC_DYN(DISS_VALUE_CFM,
            src_id,
            diss_value_cfm,
            len);

    // Set parameters
    cfm_value->value = param->value;
    cfm_value->length = len;
    if (len)
    {
        // Copy data
        memcpy(&cfm_value->data[0], data, len);
    }
    // Send message
    osapp_msg_build_send(cfm_value,sizeof(struct diss_value_cfm)+len);

}
static void osapp_gattc_mtu_changed_ind_handler(ke_msg_id_t const msgid,struct gattc_mtu_changed_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"Exchanged MTU value:%d, seq_num:%d\n",param->mtu,param->seq_num);
}



void osapp_usr_msg_handler(uint8_t *data,uint16_t length)
{
    LOG(LOG_LVL_INFO,"usr msg handler\n");
}

static void osapp_gapc_param_update_ind_handler(ke_msg_id_t const msgid, struct gapc_param_updated_ind const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
   LOG(LOG_LVL_INFO,"param update ind_%x,%x,%x\n",param->con_interval,param->con_latency,param->sup_to);
}

static const osapp_msg_handler_table_t handler_table[]=
{
        [0] =   {KE_MSG_DEFAULT_HANDLER,(osapp_msg_handler_t)osapp_defualt_msg_handler},
                {DISS_VALUE_REQ_IND,     (osapp_msg_handler_t)osapp_diss_value_req_ind_handler},
                {GAPC_CONNECTION_REQ_IND,(osapp_msg_handler_t)osapp_gapc_conn_req_ind_handler},
                {GAPC_DISCONNECT_IND,       (osapp_msg_handler_t)osapp_gapc_disconnect_ind_handler},
                {GAPM_PROFILE_ADDED_IND,(osapp_msg_handler_t)osapp_gapm_profile_added_ind_handler},
                {GAPC_GET_DEV_INFO_REQ_IND,(osapp_msg_handler_t)osapp_gapc_get_dev_info_req_ind_handler},
                {GAPM_CMP_EVT,(osapp_msg_handler_t)osapp_gapm_cmp_evt_handler},
                {GAPM_DEVICE_READY_IND,(osapp_msg_handler_t)osapp_device_ready_ind_handler},
                {GAPC_PARAM_UPDATE_REQ_IND,(osapp_msg_handler_t)osapp_gapc_param_update_req_handler},        
                {GATTC_MTU_CHANGED_IND,(osapp_msg_handler_t)osapp_gattc_mtu_changed_ind_handler},
                {GAPC_PARAM_UPDATED_IND,(osapp_msg_handler_t)osapp_gapc_param_update_ind_handler}
};
const osapp_msg_handler_info_t handler_info = ARRAY_INFO(handler_table);

//#endif
