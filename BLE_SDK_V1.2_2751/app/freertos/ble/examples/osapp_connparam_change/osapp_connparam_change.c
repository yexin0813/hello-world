#include "osapp_config.h"
#ifdef OSAPP_CONNPARAM_CHG
#include "gapc_task.h"
#include "timers.h"

struct connect_env_t
{
    uint16_t connect_idx;
    uint16_t gapc_instance;
};

static struct connect_env_t conn_env;
static uint8_t time = 0;
static uint8_t success = 0;
static TimerHandle_t Timer;


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
    
    conn_cmd.con_intv_max = 0x50;
    conn_cmd.con_intv_min = 0x50;
    conn_cmd.con_latency = 0;
    conn_cmd.superv_to = 0x2a;

    conn_cmd.ce_len_max = 0x0;
    conn_cmd.ce_len_min = 0x0;

    conn_cmd.nb_peers = 1;

    addr.addr.addr[0] = 0x11;
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

static int32_t osapp_connparam_update(uint16_t intval, uint16_t latency)
{
    struct gapc_param_update_cmd *cmd = AHI_MSG_ALLOC(GAPC_PARAM_UPDATE_CMD,TASK_ID_GAPC,gapc_param_update_cmd);

    cmd->operation = GAPC_UPDATE_PARAMS;
    cmd->intv_max = intval;
    cmd->intv_min = intval;
    cmd->time_out = 0x80;
    cmd->ce_len_max = 0xFFFF;
    cmd->ce_len_min = 0xFFFF;
    cmd->latency = latency;

    return osapp_msg_build_send(cmd, sizeof(struct gapc_param_update_cmd));
}

void osapp_connparam_update_handler()
{
	time ++;
	if(time == 1)
	{
		osapp_connparam_update(0x10,0);
    	LOG(LOG_LVL_INFO,"Connect param update_0x10\n");
	}
	else if(time == 2)
	{
		osapp_connparam_update(0x8,0);
    	LOG(LOG_LVL_INFO,"Connect param update_0x8\n");
	}

	if(success == 2)
	{
		LOG(LOG_LVL_INFO,"Finish!!!\n");
        xTimerDelete(Timer,0);
		SIMU_FINISH();
	}
}

static void osapp_gapc_cmp_evt_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapc_cmp_evt const *cmp_evt = param; 
    LOG(LOG_LVL_INFO,"GAPC Event Complete!\n");
    switch(cmp_evt->operation)
    {
        case GAPC_UPDATE_PARAMS:
            if(cmp_evt->status == GAP_ERR_NO_ERROR)
            {
            	success++;
                LOG(LOG_LVL_INFO,"Connect param update success.\n");
            }
            else
            {
                LOG(LOG_LVL_INFO,"Connect param update error.\n");
            }
            break;
    }



}


static void osapp_defualt_msg_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"defualt handler,msgid: 0x%x, dest_id: 0x%x, src_id: 0x%x",msgid,dest_id,src_id);

}


static void osapp_gapc_conn_req_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    conn_env.connect_idx = src_id;
    osapp_gapc_conn_confirm(src_id);
    LOG(LOG_LVL_INFO,"Connect comfirm\n");
    
}

static void osapp_gapc_param_updated_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"GAPC param update ind\n");
    


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
            osapp_connect();
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

            Timer = xTimerCreate("Timer",4,pdTRUE,(void *) 0,osapp_connparam_update_handler);        //50 ticks
            if(Timer != NULL)
                xTimerStart(Timer,0);
            
        }
        else
        {
            LOG(LOG_LVL_INFO,"Connect Failed\n");
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


static const osapp_msg_handler_table_t handler_table[]=
{
    [0] =   {KE_MSG_DEFAULT_HANDLER,osapp_defualt_msg_handler},
       {GAPC_CONNECTION_REQ_IND,osapp_gapc_conn_req_ind_handler},
       {GAPM_CMP_EVT,osapp_gapm_cmp_evt_handler},
       {GAPM_DEVICE_READY_IND,osapp_device_ready_ind_handler},
       {GAPC_PARAM_UPDATED_IND,osapp_gapc_param_updated_ind_handler},
       {GAPC_CMP_EVT,osapp_gapc_cmp_evt_handler},

};
const osapp_msg_handler_info_t handler_info = ARRAY_INFO(handler_table);
#endif

