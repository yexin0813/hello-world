#include "osapp_config.h"

#ifdef OSAPP_BOND_SLAVE

#include "co_bt.h"
#include "co_math.h"

#define APP_ADV_CHMAP 0x7
#define APP_ADV_INT_MIN 32 //20 ms
#define APP_ADV_INT_MAX 32 //20 ms

uint16_t conn_idx;					//store connect index

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
    cmd->max_mtu = 200;
    cmd->addr_type = addr_type;
    cmd->pairing_mode = pairing_mode;
   
    return osapp_msg_build_send(cmd, sizeof(struct gapm_set_dev_config_cmd) );
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
    nvds_get(NVDS_TAG_DEVICE_NAME, &device_name_length, cfm->info.name.value);
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


static void osapp_device_ready_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    osapp_reset();
}


static void osapp_gapc_bond_req_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapc_bond_req_ind const *ind = param;
    LOG(LOG_LVL_INFO,"gapc_bond_req_ind_%d\n",ind->request);
    switch(ind->request)
    {
        case GAPC_PAIRING_REQ:
        {
                LOG(LOG_LVL_INFO,"GAPC_PAIRING_REQ\n");
                struct gapc_bond_cfm *cfm = AHI_MSG_ALLOC(GAPC_BOND_CFM, conn_idx, gapc_bond_cfm);
                cfm->request = GAPC_PAIRING_RSP;
                cfm->accept = 0x01;
                cfm->data.pairing_feat.iocap = 0x03;
                cfm->data.pairing_feat.oob = 0x00;
                cfm->data.pairing_feat.auth = 0x09;
                cfm->data.pairing_feat.key_size = 0x10;
                cfm->data.pairing_feat.ikey_dist = 0x04;
                cfm->data.pairing_feat.rkey_dist = 0x03;
                cfm->data.pairing_feat.sec_req = 0x00;         
                osapp_msg_build_send(cfm, sizeof(struct gapc_bond_cfm));
            break;
        }
        case GAPC_LTK_EXCH:
        {
                uint8_t i;
                LOG(LOG_LVL_INFO,"GAPC_LTK_EXCH\n");
                struct gapc_bond_cfm *cfm = AHI_MSG_ALLOC(GAPC_BOND_CFM, conn_idx, gapc_bond_cfm);
                cfm->accept = 0x1;
                cfm->request = GAPC_LTK_EXCH;
                cfm->data.ltk.ediv = 0xA6A4;
                for(i=0;i<KEY_LEN;i++)
                {
                    cfm->data.ltk.ltk.key[i] = (uint8_t)co_rand_word();
                }
//                memcpy(cfm->data.ltk.ltk.key, ltk_key, sizeof(ltk_key));
                for(i=0;i<RAND_NB_LEN;i++)
                {
                    cfm->data.ltk.randnb.nb[i] = (uint8_t)co_rand_word();
                }
//                memcpy(cfm->data.ltk.randnb.nb, ltk_randnb, sizeof(ltk_randnb));
                cfm->data.ltk.key_size = KEY_LEN;
                osapp_msg_build_send(cfm, sizeof(struct gapc_bond_cfm));
            break;
        }
        default:
            break;
    }


}


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
            break;
        case GAPC_PAIRING_SUCCEED:
            LOG(LOG_LVL_INFO,"Auth_%d,Ltk Present_%d\n",ind->data.auth.info,ind->data.auth.ltk_present);
            break;
    }
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
        osapp_start_advertising();
        break;
    case GAPM_ADV_UNDIRECT:
        LOG(LOG_LVL_INFO,"Adv Complete\n");
        break;
    default:
        LOG(LOG_LVL_WARN,"gapm_cmp_evt operation:0x%x\n",cmp_evt->operation);
        break;
            
    }
}



static void osapp_gapc_conn_req_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
	conn_idx = src_id;
    osapp_gapc_conn_confirm(src_id);
}

static void osapp_defualt_msg_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"defualt handler,msgid: 0x%x, dest_id: 0x%x, src_id: 0x%x",msgid,dest_id,src_id);

}
static void osapp_gapc_disconnect_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
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

static const osapp_msg_handler_table_t handler_table[]=
{
        [0] =   {KE_MSG_DEFAULT_HANDLER,(osapp_msg_handler_t)osapp_defualt_msg_handler},
                {GAPC_CONNECTION_REQ_IND,(osapp_msg_handler_t)osapp_gapc_conn_req_ind_handler},
                {GAPC_DISCONNECT_IND,       (osapp_msg_handler_t)osapp_gapc_disconnect_ind_handler},
                {GAPC_GET_DEV_INFO_REQ_IND,(osapp_msg_handler_t)osapp_gapc_get_dev_info_req_ind_handler},
                {GAPM_CMP_EVT,(osapp_msg_handler_t)osapp_gapm_cmp_evt_handler},
                {GAPM_DEVICE_READY_IND,(osapp_msg_handler_t)osapp_device_ready_ind_handler},
                {GAPC_BOND_REQ_IND,(osapp_msg_handler_t)osapp_gapc_bond_req_ind_handler},
                {GAPC_BOND_IND,(osapp_msg_handler_t)osapp_gapc_bond_ind_handler},                
};
const osapp_msg_handler_info_t handler_info = ARRAY_INFO(handler_table);


#endif
