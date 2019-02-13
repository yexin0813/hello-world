/*
 * osapp_throughput_server.c
 *
 *  Created on: 2018爛1堎6
 *      Author: mingzhou
 */

#include "osapp_config.h"

#ifdef OSAPP_THROUGHPUT_SERVER

#include "diss_task.h"
#include "bxotas_task.h"

#define APP_ADV_CHMAP 0x7
#define APP_ADV_INT_MIN 32 //20 ms
#define APP_ADV_INT_MAX 32 //20 ms

/// Manufacturer Name Value
#define APP_DIS_MANUFACTURER_NAME       ("APOLLO_00 Throughput Test Server")
#define APP_DIS_MANUFACTURER_NAME_LEN   (sizeof(APP_DIS_MANUFACTURER_NAME))

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
 *      Vendor ID Source : 0x02 (USB Implementer� Forum assigned Vendor ID value)
 *      Vendor ID : 0x045E      (Microsoft Corp)
 *      Product ID : 0x0040
 *      Product Version : 0x0300
 */
#define APP_DIS_PNP_ID               ("\x02\x5E\x04\x40\x00\x00\x03")
#define APP_DIS_PNP_ID_LEN           (7)

#define APP_NOTIFICATION_LENGTH 2000
/// gble service uuid
uint8_t SERVICE_UUID[2] = {0xb0,0xff};
uint8_t CHARACTERISTIC_UUID[2] = {0x03,0x28};
uint8_t LARGE_DATA1_UUID[2] = {0xc0,0xff};
uint8_t LARGE_DATA2_UUID[2] = {0xc1,0xff};
uint8_t LARGE_NOTIF_UUID[2] = {0xc2,0xff};

struct connection_env_t
{
    uint8_t conidx;
    uint16_t conhdl;
    uint16_t con_interval;
}connection_env;

struct DEMO_HANDLE_T
{
    /// valid or not
    uint8_t valid;
    uint16_t service_handle;
    uint16_t data1_handle;
    uint16_t data2_handle;
};
/// all characteristics should be stored for future use, handles are use to distinguish which charateristic to read or write.

struct DEMO_HANDLE_T data_char_handles;

struct gattm_att_desc const service_att_table[] =
{
        {{0xb0,0xff},   PERM(RD, ENABLE),        0,                          0},
        {{0xc0,0xff},   PERM(RD, ENABLE),        2000,                       PERM(RI, ENABLE)},
        {{0xb0,0xff},   PERM(RD, ENABLE),        0,                          0},
        {{0xc1,0xff},   PERM(RD, ENABLE),        2000,                       PERM(RI, ENABLE)},
        {{0xb0,0xff},   PERM(RD, ENABLE),        0,                          0},
        {{0xc2,0xff},   PERM(NTF,ENABLE),        APP_NOTIFICATION_LENGTH,    PERM(RI, ENABLE)},
};


static int32_t osapp_reset()
{
    struct gapm_reset_cmd *cmd = AHI_MSG_ALLOC(GAPM_RESET_CMD,TASK_ID_GAPM,gapm_reset_cmd);
    cmd->operation = GAPM_RESET;
    return osapp_msg_build_send(cmd, sizeof(struct gapm_reset_cmd));
}


static int32_t osapp_add_svc()
{
    struct gattm_add_svc_req *req = AHI_MSG_ALLOC_DYN(GATTM_ADD_SVC_REQ,TASK_ID_GATTM,gattm_add_svc_req,sizeof(service_att_table));
    req->svc_desc.start_hdl = 0;
    req->svc_desc.task_id = TASK_ID_AHI;
    req->svc_desc.perm = 0;
    req->svc_desc.nb_att = (sizeof(service_att_table)/sizeof(struct gattm_att_desc));
    data_char_handles.valid = 0;

    memcpy(req->svc_desc.uuid,SERVICE_UUID,2);

    for(uint32_t i=0;i<(sizeof(service_att_table)/sizeof(struct gattm_att_desc));i++)
    {
        memcpy(&req->svc_desc.atts[i], &service_att_table[i], sizeof(struct gattm_att_desc));
    }

    return osapp_msg_build_send(req, sizeof(struct gattm_add_svc_req) + sizeof(service_att_table));
}


static int32_t osapp_set_dev_config(uint8_t role,uint8_t addr_type,uint8_t pairing_mode)
{
    // Set Device configuration
    struct gapm_set_dev_config_cmd* cmd = AHI_MSG_ALLOC(GAPM_SET_DEV_CONFIG_CMD,TASK_ID_GAPM,gapm_set_dev_config_cmd);
    cmd->operation = GAPM_SET_DEV_CONFIG;
    cmd->role      = role;

    // Set Data length parameters
    cmd->sugg_max_tx_octets = BLE_MAX_OCTETS;
    cmd->sugg_max_tx_time   = BLE_MAX_TIME;
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
            // Flag value is set by the GAP
    cmd->info.host.adv_data_len       = ADV_DATA_LEN - 3;
    cmd->info.host.scan_rsp_data_len  = SCAN_RSP_DATA_LEN;
    return osapp_msg_build_send(cmd,sizeof(struct gapm_start_advertise_cmd));

}

static void osapp_send_notification(void)
{
    static uint16_t notify_seq_num = 0;
    struct gattc_send_evt_cmd *cmd= AHI_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,TASK_ID_GATTC, gattc_send_evt_cmd, APP_NOTIFICATION_LENGTH);
    cmd->operation = GATTC_NOTIFY;
    cmd->seq_num = notify_seq_num++;
    BX_ASSERT(data_char_handles.data2_handle != 0);
    cmd->handle = data_char_handles.data2_handle + 2;
    cmd->length = APP_NOTIFICATION_LENGTH;
    memcpy(cmd->value,(void*)0,APP_NOTIFICATION_LENGTH);
    osapp_ahi_msg_send(cmd, sizeof(struct gattc_send_evt_cmd) + APP_NOTIFICATION_LENGTH, portMAX_DELAY);
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
    nvds_get(NVDS_TAG_DEVICE_NAME, &device_name_length, cfm->info.name.value);
    cfm->info.name.length = device_name_length;
    LOG(LOG_LVL_INFO,"get device name\n");
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

static void gattm_add_svc_rsp_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gattm_add_svc_rsp *rsp = (struct gattm_add_svc_rsp *)param;
    LOG(LOG_LVL_INFO,"database_add -> %d,%d\n",rsp->start_hdl,rsp->status);

    data_char_handles.valid = 1;
    data_char_handles.service_handle = rsp->start_hdl;
    data_char_handles.data1_handle= rsp->start_hdl + 2;
    data_char_handles.data2_handle= rsp->start_hdl + 4;
    osapp_start_advertising();
}

static void gapc_le_phy_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapc_le_phy_ind *ind = (struct gapc_le_phy_ind *)param;
    LOG(LOG_LVL_INFO,"phy rate = %d,%d\n",ind->tx_rate, ind->rx_rate);
//    osapp_send_notification();
}

static void osapp_device_ready_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    osapp_reset();
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
//        osapp_add_bxotas_task();
        break;
    case GAPM_PROFILE_TASK_ADD:
        BX_ASSERT(cmp_evt->status==GAP_ERR_NO_ERROR);
//        osapp_start_advertising();
        osapp_add_svc();
        break;
    default:
        LOG(LOG_LVL_WARN,"gapm_cmp_evt operation:0x%x\n",cmp_evt->operation);
        break;

    }
}

static void osapp_gattc_cmp_evt_handler(ke_msg_id_t const msgid,struct gattc_cmp_evt const *param,ke_task_id_t const dest_id,ke_task_id_t src_id)
{
    switch(param->operation)
    {
        case GATTC_NOTIFY:
            if(param->status == ATT_ERR_NO_ERROR)
            {
                LOG(LOG_LVL_INFO,"notification done\n");
                osapp_send_notification();
            }else
            {
                LOG(LOG_LVL_INFO,"noti status:%d\n",param->status);
            }
            break;
        default:
            LOG(LOG_LVL_INFO,"op:%d,seq:%d,status:%d\n",param->operation,param->seq_num,param->status);
            break;
    }
}

static void osapp_gapm_profile_added_ind_handler(ke_msg_id_t const msgid, struct gapm_profile_added_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"GAPM profile added indication,id:%d,nb:%d,hdl:%d\n",param->prf_task_id,param->prf_task_nb,param->start_hdl);
    if(param->prf_task_id==TASK_ID_BXOTAS)
    {
        struct bxotas_enable_req *req =(struct bxotas_enable_req *)AHI_MSG_ALLOC(BXOTAS_ENABLE_REQ,TASK_ID_BXOTAS,bxotas_enable_req);
        req->enable = true;
        osapp_msg_build_send(req,sizeof(struct bxotas_enable_req));
    }
}

static void osapp_gapc_conn_req_ind_handler(ke_msg_id_t const msgid, struct gapc_connection_req_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    connection_env.conidx = KE_IDX_GET(src_id); // connection index
    connection_env.conhdl = param->conhdl; // connection handle
    connection_env.con_interval = param->con_interval;
    osapp_gapc_conn_confirm(src_id);
//    osapp_send_notification();
//    osapp_start_advertising();
}

static void osapp_defualt_msg_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"default handler,msgid: 0x%x, dest_id: 0x%x, src_id: 0x%x\n",msgid,dest_id,src_id);

}
void delay(uint32_t i)
{
    uint32_t j;
    for(j=0;j<i;j++);

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
            len = 128;
            data = "dsdadasd";
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

static void osapp_gapc_cmp_evt_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapc_cmp_evt const *evt =param;

    LOG(LOG_LVL_INFO,"gapc_evt_%d,%d\n",evt->operation,evt->status);
}

static void osapp_gapc_le_pkt_size_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapc_le_pkt_size_ind *ind = (struct gapc_le_pkt_size_ind *)param;

    LOG(LOG_LVL_INFO,"tx_oct=%d, rx_oct=%d\n",ind->max_tx_octets,ind->max_rx_octets);

    osapp_send_notification();  // !!!
                                // If the default phy is 1Mbps, then there will be no GAPC_LE_PHY_IND sent for app
                                // as long as the post-set phy remains at 1Mbps. So the first sending notification action
                                // is moved here.
                                // !!!
}


static void gattc_read_req_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gattc_read_req_ind *ind = (struct gattc_read_req_ind *)param;
    struct gattc_read_cfm *cfm = AHI_MSG_ALLOC_DYN(GATTC_READ_CFM, TASK_ID_GATTC, gattc_read_cfm, 2000);

    if(ind->handle == data_char_handles.data1_handle)
    {
        cfm->handle = data_char_handles.data1_handle;
        cfm->length = 2000;
        cfm->status = 0;

        memcpy(cfm->value,"This is string1-> num 1, 2000 byte.",36);
        osapp_msg_build_send(cfm,sizeof(struct gattc_read_cfm) + 2000);

    }
    else if(ind->handle == data_char_handles.data2_handle)
    {
        cfm->handle = data_char_handles.data2_handle;
        cfm->length = 2000;
        cfm->status = 0;

        memcpy(cfm->value,"This is string2-> num 2, 2000 byte.",36);
        osapp_msg_build_send(cfm,sizeof(struct gattc_read_cfm) + 2000);

    }


}

static const osapp_msg_handler_table_t handler_table[]=
{
        [0] =   {KE_MSG_DEFAULT_HANDLER,(osapp_msg_handler_t)osapp_defualt_msg_handler},
                {DISS_VALUE_REQ_IND,     (osapp_msg_handler_t)osapp_diss_value_req_ind_handler},
                {GAPC_CONNECTION_REQ_IND,(osapp_msg_handler_t)osapp_gapc_conn_req_ind_handler},
                {GAPC_DISCONNECT_IND,       (osapp_msg_handler_t)osapp_gapc_disconnect_ind_handler},
                {GAPC_LE_PKT_SIZE_IND,    (osapp_msg_handler_t)osapp_gapc_le_pkt_size_ind_handler},
                {GAPM_PROFILE_ADDED_IND,(osapp_msg_handler_t)osapp_gapm_profile_added_ind_handler},
                {GAPC_GET_DEV_INFO_REQ_IND,(osapp_msg_handler_t)osapp_gapc_get_dev_info_req_ind_handler},
                {GAPM_CMP_EVT,(osapp_msg_handler_t)osapp_gapm_cmp_evt_handler},
                {GAPC_LE_PHY_IND,gapc_le_phy_ind_handler},
                {GAPC_CMP_EVT,osapp_gapc_cmp_evt_handler},
                {GAPM_DEVICE_READY_IND,(osapp_msg_handler_t)osapp_device_ready_ind_handler},
                {GAPC_PARAM_UPDATE_REQ_IND,(osapp_msg_handler_t)osapp_gapc_param_update_req_handler},
                {GATTC_CMP_EVT,(osapp_msg_handler_t)osapp_gattc_cmp_evt_handler},
                {GATTC_MTU_CHANGED_IND,(osapp_msg_handler_t)osapp_gattc_mtu_changed_ind_handler},
                {GATTC_READ_REQ_IND,(osapp_msg_handler_t)gattc_read_req_ind_handler},
                {GATTM_ADD_SVC_RSP,(osapp_msg_handler_t)gattm_add_svc_rsp_handler},
};
const osapp_msg_handler_info_t handler_info = ARRAY_INFO(handler_table);

#endif

