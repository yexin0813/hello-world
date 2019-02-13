#include "osapp_config.h"
#ifdef OSAPP_ADV


#define APP_ADV_CHMAP 0x7
#define APP_ADV_INT_MIN 64
#define APP_ADV_INT_MAX 64

#define OSAPP_DEVICE_NAME_MAX_LEN      (18)
#define OSAPP_SCNRSP_DATA         ("APOLLO_00-ADVRSP")
#define OSAPP_SCNRSP_DATA_LEN     (sizeof(OSAPP_SCNRSP_DATA))

#define OSAPP_DFLT_DEVICE_NAME            ("APOLLO_00-DEMO")
#define OSAPP_DFLT_DEVICE_NAME_LEN        (sizeof(OSAPP_DFLT_DEVICE_NAME))

#define OSAPP_BX_ADV_DATA_UUID        "\x03\x03\x00\x24"
#define OSAPP_BX_ADV_DATA_UUID_LEN    (4)
#define OSAPP_DATA_TOTAL_LEN        (ADV_DATA_LEN-3)    // 3 bytes for ad type flags     


struct connect_env_t
{
    uint16_t connect_idx;
};

static struct connect_env_t conn_env;

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

    memcpy(&cmd->info.host.adv_data[cmd->info.host.adv_data_len],
           OSAPP_BX_ADV_DATA_UUID, OSAPP_BX_ADV_DATA_UUID_LEN);
    cmd->info.host.adv_data_len += OSAPP_BX_ADV_DATA_UUID_LEN;

    cmd->info.host.adv_data[cmd->info.host.adv_data_len] = OSAPP_DATA_TOTAL_LEN-2-OSAPP_BX_ADV_DATA_UUID_LEN+1;
    cmd->info.host.adv_data[cmd->info.host.adv_data_len+1] = '\x08';
    memcpy(&cmd->info.host.adv_data[6],OSAPP_DFLT_DEVICE_NAME,OSAPP_DFLT_DEVICE_NAME_LEN);
    cmd->info.host.adv_data_len = OSAPP_DATA_TOTAL_LEN;

    memcpy(&cmd->info.host.scan_rsp_data[0],OSAPP_DFLT_DEVICE_NAME,cmd->info.host.scan_rsp_data_len);
    cmd->info.host.scan_rsp_data_len  = OSAPP_SCNRSP_DATA_LEN;

    return osapp_msg_build_send(cmd,sizeof(struct gapm_start_advertise_cmd));
        
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
    cmd->addr_type = addr_type;
    cmd->pairing_mode = pairing_mode;
   
    return osapp_msg_build_send(cmd, sizeof(struct gapm_set_dev_config_cmd) );
}


static int32_t osapp_gapc_conn_confirm(ke_task_id_t dest_id)
{
    struct gapc_connection_cfm *cfm = AHI_MSG_ALLOC(GAPC_CONNECTION_CFM, dest_id, gapc_connection_cfm);
    cfm->auth = GAP_AUTH_REQ_NO_MITM_NO_BOND;
    return osapp_msg_build_send(cfm,sizeof(struct gapc_connection_cfm));
}


static int32_t osapp_disconnect(ke_task_id_t dest_id)
{
    struct gapc_disconnect_cmd *cmd = AHI_MSG_ALLOC(GAPC_DISCONNECT_CMD,dest_id,gapc_disconnect_cmd);
    
    cmd->operation = GAPC_DISCONNECT;
    cmd->reason    = CO_ERROR_REMOTE_USER_TERM_CON;
    return osapp_msg_build_send(cmd,sizeof(struct gapc_disconnect_cmd));

}

static int32_t osapp_reset()
{
    struct gapm_reset_cmd *cmd = AHI_MSG_ALLOC(GAPM_RESET_CMD,TASK_ID_GAPM,gapm_reset_cmd);
    cmd->operation = GAPM_RESET;
    return osapp_msg_build_send(cmd, sizeof(struct gapm_reset_cmd));
}



static void osapp_defualt_msg_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"defualt handler,msgid: 0x%x, dest_id: 0x%x, src_id: 0x%x",msgid,dest_id,src_id);

}

static void osapp_gapc_conn_req_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"Connect req indicator\n");
    conn_env.connect_idx = src_id;
    osapp_gapc_conn_confirm(src_id);
    
}

static void osapp_gapc_disconnect_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"Disconnect\n");
    conn_env.connect_idx = 0;
    LOG(LOG_LVL_INFO,"Restart Advertising...\n");
    osapp_start_advertising();
}

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
                                                    gapc_get_dev_info_cfm, OSAPP_DEVICE_NAME_MAX_LEN);
            cfm->req = dev_info->req;
            memcpy(cfm->info.name.value,"APOLLO_00_App_Demo",18);
            cfm->info.name.length = 18;

            osapp_msg_build_send(cfm,sizeof(struct gapc_get_dev_info_cfm) + OSAPP_DEVICE_NAME_MAX_LEN);

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

static void osapp_gapm_cmp_evt_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapm_cmp_evt const *cmp_evt = param;
    switch(cmp_evt->operation)
    {
    case GAPM_RESET:
        if(cmp_evt->status == GAP_ERR_NO_ERROR)
        {
            LOG(LOG_LVL_INFO,"Reset OK\n");
            osapp_set_dev_config(GAP_ROLE_PERIPHERAL,GAPM_CFG_ADDR_PUBLIC,GAPM_PAIRING_LEGACY);
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
            LOG(LOG_LVL_INFO,"Start Advertising...\n");
        }
        else
        {
            LOG(LOG_LVL_INFO,"Set Role Failed\n");
        }
        break;
    case GAPM_ADV_UNDIRECT:
        if(cmp_evt->status == GAP_ERR_CANCELED)
        {
            LOG(LOG_LVL_WARN,"adv cancelled\n");
        }
        else if(cmp_evt->status == GAP_ERR_NO_ERROR)
        {
            LOG(LOG_LVL_INFO,"ADV Complete with No ERR\n");
        }
        else
        {
            LOG(LOG_LVL_INFO,"Stop Advertising Failed\n");
        }
        break;
    default:
        LOG(LOG_LVL_WARN,"gapm_cmp_evt operation:0x%x\n",cmp_evt->operation);
        break;
            
    }

}

static void osapp_device_ready_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    //reset cmd
    LOG(LOG_LVL_INFO,"Device Ready\n");
    osapp_reset();
}


static const osapp_msg_handler_table_t handler_table[]=
{
    [0] =   {KE_MSG_DEFAULT_HANDLER,osapp_defualt_msg_handler},
       {GAPC_CONNECTION_REQ_IND,osapp_gapc_conn_req_ind_handler},
       {GAPC_DISCONNECT_IND,       osapp_gapc_disconnect_ind_handler},
       {GAPC_GET_DEV_INFO_REQ_IND,osapp_gapc_get_dev_info_req_ind_handler},
       {GAPM_CMP_EVT,osapp_gapm_cmp_evt_handler},
       {GAPM_DEVICE_READY_IND,osapp_device_ready_ind_handler},
};
const osapp_msg_handler_info_t handler_info = ARRAY_INFO(handler_table);
#endif

