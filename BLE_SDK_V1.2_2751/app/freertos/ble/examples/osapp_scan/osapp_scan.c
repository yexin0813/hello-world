#include "osapp_config.h"
#ifdef OSAPP_SCAN

struct connect_env_t
{
    uint16_t connect_idx;
    uint16_t gapc_instance;
};

static struct connect_env_t conn_env;
static uint8_t time = 4;            //connect twice and read two different data.


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

static int32_t osapp_start_scan()
{
    struct gapm_start_scan_cmd *cmd = AHI_MSG_ALLOC(GAPM_START_SCAN_CMD,TASK_ID_GAPM,gapm_start_scan_cmd);
    cmd->op.code = GAPM_SCAN_PASSIVE;

    cmd->mode = GAP_OBSERVER_MODE;
    cmd->interval = 0x20;
    cmd->window = 0x20;
    
    return osapp_msg_build_send(cmd, sizeof(struct gapm_start_scan_cmd));
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

static void osapp_gapm_adv_report_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct adv_report const *report = param;
    char adv_data[32];
    LOG(LOG_LVL_INFO,"adv_ind\n");
    memset(adv_data,0,32);

    memcpy(adv_data,&report->data[9],report->data_len-9);

    LOG(LOG_LVL_INFO,adv_data);
    switch(time)
    {
    case 0:
    	if(adv_data[10]=='0')
    		time = 1;
    	break;
    case 1:
    	if(adv_data[10]=='1')
    		time = 2;
    	break;
    case 2:
    	if(adv_data[10]=='2')
    		time = 3;
    	break;
    case 3:
    	if(adv_data[10]=='3')
    		time = 4;
    	break;
    case 4:
    	if(adv_data[10]=='4')
    		time = 5;
    	break;
    }
    if(time == 5)
    {
    	LOG(LOG_LVL_INFO,"Finish\n");
    	SIMU_FINISH();
    }
    LOG(LOG_LVL_INFO,"\n");
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
            osapp_start_scan();
            LOG(LOG_LVL_INFO,"Add Profile\n");
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

static void osapp_device_ready_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    //reset cmd
    osapp_reset();
}


static const osapp_msg_handler_table_t handler_table[]=
{
    [0] =   {KE_MSG_DEFAULT_HANDLER,osapp_defualt_msg_handler},
       {GAPM_CMP_EVT,osapp_gapm_cmp_evt_handler},
       {GAPM_DEVICE_READY_IND,osapp_device_ready_ind_handler},
       {GAPM_ADV_REPORT_IND,osapp_gapm_adv_report_ind_handler},
    
};
const osapp_msg_handler_info_t handler_info = ARRAY_INFO(handler_table);
#endif

