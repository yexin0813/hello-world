#include "osapp_config.h"
#ifdef OSAPP_DIS_CLIENT
#include "disc_task.h"

struct connect_env_t
{
    uint16_t connect_idx;
    uint16_t gapc_instance;
};

static struct connect_env_t conn_env;
static uint8_t time = 0;            //connect twice and read two different data.

static int32_t  osapp_connect()   //struct gap_bdaddr *addr, struct gapm_start_connection_cmd *conn_cmd
{
    struct gap_bdaddr addr;
    struct gapm_start_connection_cmd conn_cmd;
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

    addr.addr.addr[0] = 0x99;
    addr.addr.addr[1] = 0x22;
    addr.addr.addr[2] = 0x33;
    addr.addr.addr[3] = 0x44;
    addr.addr.addr[4] = 0x55;
    addr.addr.addr[5] = 0x66;


    addr.addr_type = ADDR_PUBLIC;


    struct gapm_start_connection_cmd* new_cmd = AHI_MSG_ALLOC_DYN(GAPM_START_CONNECTION_CMD,
                                                                     TASK_ID_GAPM,
                                                                     gapm_start_connection_cmd, sizeof(struct gap_bdaddr));

    memcpy(new_cmd,&conn_cmd,sizeof(struct gapm_start_connection_cmd));
    
    memcpy(new_cmd->peers,&addr,sizeof(struct gap_bdaddr));

    return osapp_msg_build_send(new_cmd, sizeof(struct gapm_start_connection_cmd) + sizeof(struct gap_bdaddr) );

}


static int32_t  osapp_disc_add_profile()
{
    // Allocate the DISS_CREATE_DB_REQ
    struct gapm_profile_task_add_cmd *req = AHI_MSG_ALLOC(GAPM_PROFILE_TASK_ADD_CMD,
                                                  TASK_ID_GAPM,
                                                  gapm_profile_task_add_cmd);
    // Fill message
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl = PERM(SVC_AUTH, ENABLE);
    req->prf_task_id = TASK_ID_DISC;
    req->app_task = TASK_ID_AHI;
    req->start_hdl = 0;

    return osapp_msg_build_send(req, sizeof(struct gapm_profile_task_add_cmd));
}

static int32_t osapp_disc_enable_req()
{
    struct disc_enable_req *param = AHI_MSG_ALLOC(DISC_ENABLE_REQ,
                                    TASK_ID_DISC,
                                    disc_enable_req);
    param->con_type = 0;

    return osapp_msg_build_send(param, sizeof(struct disc_enable_req));
}

static int32_t app_dis_rd_char_req(uint8_t code)
{
    struct disc_rd_char_req *req = AHI_MSG_ALLOC(DISC_RD_CHAR_REQ,
                                    TASK_ID_DISC,
                                    disc_rd_char_req);
    req->char_code = code;

    return osapp_msg_build_send(req, sizeof(struct disc_rd_char_req));

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
   
    return osapp_msg_build_send(cmd, sizeof(struct gapm_set_dev_config_cmd));
}


static int32_t osapp_gapc_conn_confirm(ke_task_id_t dest_id)
{
    struct gapc_connection_cfm *cfm = AHI_MSG_ALLOC(GAPC_CONNECTION_CFM, dest_id, gapc_connection_cfm);
    cfm->auth = GAP_AUTH_REQ_NO_MITM_NO_BOND;
    return osapp_msg_build_send(cfm,sizeof(struct gapc_connection_cfm));
}

static int32_t osapp_exc_mtu()
{
    struct gattc_exc_mtu_cmd *cmd =  AHI_MSG_ALLOC(GATTC_EXC_MTU_CMD, TASK_ID_GATTC, gattc_exc_mtu_cmd);
    cmd->operation = GATTC_MTU_EXCH;
    cmd->seq_num = 0;
    return osapp_msg_build_send(cmd, sizeof(struct gattc_exc_mtu_cmd));
}


static void osapp_gattc_mtu_changed_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gattc_mtu_changed_ind *ind = (struct gattc_mtu_changed_ind *)param;
    LOG(LOG_LVL_INFO,"mtu = %d,%d\n",ind->mtu, ind->seq_num);
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

static void osapp_gapm_profile_added_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"GAPM PROFILE ADDED\n");
}

static void osapp_gapc_cmp_evt_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"GAPC_CMP_EVT\n");
    if(time == 1)
    {
        osapp_connect();
    }
    else if(time == 2)
    {
        LOG(LOG_LVL_INFO,"Finish!!!\n");
        SIMU_FINISH();

    }
}

static void osapp_disc_enable_rsp_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"Receive Enable Rsp\n");
    if(time == 1)
    	app_dis_rd_char_req(0);
    else if(time == 2)
    	app_dis_rd_char_req(1);
}

static void osapp_disc_rd_char_rsp_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
	char buff[30];
	char const *data = param;
	uint8_t len;
	len = *(data+2) | *(data+3);
    LOG(LOG_LVL_INFO,"Get Device information, len=%d\n",len);
    memset(buff,0,30);
    memcpy(buff,data+5,len);
    LOG(LOG_LVL_INFO,buff);
    LOG(LOG_LVL_INFO,"\n");
    if(time == 1)
    	LOG(LOG_LVL_INFO,"\nReconnect and read data again\n");
    
    //osapp_disconnect(conn_env.connect_idx);
    
}


static void osapp_gapc_conn_req_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    conn_env.connect_idx = src_id;
    time++;
    osapp_gapc_conn_confirm(src_id);
    LOG(LOG_LVL_INFO,"Connect comfirm\n");
    
}

static void osapp_gapc_disconnect_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    conn_env.connect_idx = 0;
    LOG(LOG_LVL_INFO,"disconnected\n");
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
            osapp_set_dev_config(GAP_ROLE_ALL,GAPM_CFG_ADDR_PUBLIC,GAPM_PAIRING_LEGACY);
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
            osapp_disc_add_profile();
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
            osapp_disc_enable_req();
            osapp_exc_mtu();
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
            osapp_connect();
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

static void osapp_device_ready_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    //reset cmd
    osapp_reset();
}

static void osapp_gattc_cmp_evt_handler(ke_msg_id_t const msgid,struct gattc_cmp_evt const *param,ke_task_id_t const dest_id,ke_task_id_t src_id)
{
    switch(param->operation)
    {
        default:
            LOG(LOG_LVL_INFO,"op:%d,seq:%d,status:%d\n",param->operation,param->seq_num,param->status);
            break;
    }
}

static const osapp_msg_handler_table_t handler_table[]=
{
    [0] =   {KE_MSG_DEFAULT_HANDLER,osapp_defualt_msg_handler},
       {GAPC_CONNECTION_REQ_IND,osapp_gapc_conn_req_ind_handler},
       {GAPC_DISCONNECT_IND,osapp_gapc_disconnect_ind_handler},
       {GAPM_CMP_EVT,osapp_gapm_cmp_evt_handler},
       {GAPM_DEVICE_READY_IND,osapp_device_ready_ind_handler},
       {GATTC_MTU_CHANGED_IND,osapp_gattc_mtu_changed_ind_handler},
       {GAPC_CMP_EVT,osapp_gapc_cmp_evt_handler},
       {GAPM_PROFILE_ADDED_IND,osapp_gapm_profile_added_ind_handler},
       {DISC_ENABLE_RSP,osapp_disc_enable_rsp_handler},
       {DISC_RD_CHAR_RSP,osapp_disc_rd_char_rsp_handler},
       {GATTC_CMP_EVT,osapp_gattc_cmp_evt_handler}
};
const osapp_msg_handler_info_t handler_info = ARRAY_INFO(handler_table);
#endif

