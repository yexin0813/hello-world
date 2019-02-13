#include "osapp_config.h"
#ifdef OSAPP_SELFIE
#include "hogpd_task.h"
#define ADV_NAME "APOLLO_00_SELFIE"
#define APP_ADV_CHMAP 0x7
#define APP_ADV_INT_MIN 64
#define APP_ADV_INT_MAX 64
#define HID_REPORT_NUMS 10
/// States of the Application HID Module
enum hid_states
{
    /// Module is disabled (Service not added in DB)
    APP_HID_DISABLED,
    /// Module is idle (Service added but profile not enabled)
    APP_HID_IDLE,
    /// Module is enabled (Device is connected and the profile is enabled)
    APP_HID_ENABLED,
    /// The application can send reports
    APP_HID_READY,
    /// Waiting for a report
    APP_HID_WAIT_REP,

    APP_HID_STATE_MAX,
};
typedef struct {
    bool bonded;
    uint8_t loc_irk[KEY_LEN];
}sec_env_t;
typedef struct {
	uint8_t conidx;
	uint8_t state;
	uint8_t nb_report;
}hid_env_t;
static sec_env_t osapp_sec_env;
static hid_env_t osapp_hid_env;
static const uint8_t keyboard_report_map[]=
{
    0x05,0x01,  /// Usage Page (Generic Desktop)
    0x09,0x06,  /// Usage (Keyboard)
    0xa1,0x01,  /// Collection (Application)
    0x05,0x07,  /// Usage Page (Key Codes)
    0x19,0xe0,  /// Usage Minimum (224) 
    0x29,0xe7,  /// Usage Maximum (231)
    0x15,0x00,  /// Logical Minimum (0)
    0x25,0x01,  /// Logical Maximum (1)
    0x75,0x01,  /// Report Size (1)
    0x95,0x08,  /// Report Count (8)
    0x81,0x02,  /// Input (Data, Variable, Absolute) ;Modifier byte
    0x95,0x01,  /// Report Count (1)
    0x75,0x08,  /// Report Size (8)
    0x81,0x01,  /// Input (Constant) ;Reserved byte
    0x95,0x06,  /// Report Count (6)
    0x75,0x08,  /// Report Size (8)
    0x15,0x00,  /// Logical Minimum (0)
    0x25,0x84,  /// Logical Maximum (132)
    0x05,0x07,  /// Usage Page (Key Codes)
    0x19,0x00,  /// Usage Minimum (0)
    0x29,0x84,  /// Usage Maximum (132)
    0x81,0x00,  /// Input (Data, Array)
    0xc0,            /// End Collection    
};
#define KEYBOARD_REPORT_LENGTH 8
static void osapp_sec_init()
{
    osapp_sec_env.bonded = false;
}
static void osapp_hid_init()
{
	osapp_hid_env.nb_report = HID_REPORT_NUMS;
}
static int32_t osapp_gapm_reset()
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
    cmd->addr_type = addr_type;
    cmd->pairing_mode = pairing_mode;
   
    return osapp_msg_build_send(cmd, sizeof(struct gapm_set_dev_config_cmd) );
}
static int32_t osapp_add_hogpd_task()
{
	struct hogpd_db_cfg *db_cfg;
    // Prepare the HOGPD_CREATE_DB_REQ message
    struct gapm_profile_task_add_cmd *req = AHI_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,TASK_ID_GAPM,gapm_profile_task_add_cmd, sizeof(struct hogpd_db_cfg));

    // Fill message
    req->operation   = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl     = PERM(SVC_AUTH, ENABLE);
    req->prf_task_id = TASK_ID_HOGPD;
    req->app_task    = TASK_AHI;
    req->start_hdl   = 0;

    // Set parameters
    db_cfg = (struct hogpd_db_cfg* ) req->param;

    // Only one HIDS instance is useful
    db_cfg->hids_nb = 1;

    // The device is a mouse
    db_cfg->cfg[0].svc_features = HOGPD_CFG_KEYBOARD;

    // Only one Report Characteristic is requested
    db_cfg->cfg[0].report_nb    = 1;

    db_cfg->cfg[0].report_id[0] = 1;

    // The report is an input report
    db_cfg->cfg[0].report_char_cfg[0] = HOGPD_CFG_REPORT_IN;

    // HID Information
    db_cfg->cfg[0].hid_info.bcdHID       = 0x0111;         // HID Version 1.11
    db_cfg->cfg[0].hid_info.bCountryCode = 0x00;
    db_cfg->cfg[0].hid_info.flags        = HIDS_REMOTE_WAKE_CAPABLE | HIDS_NORM_CONNECTABLE;
	return osapp_msg_build_send(req,sizeof(struct gapm_profile_task_add_cmd)+sizeof(struct hogpd_db_cfg));
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

    adv_data_pack(cmd->info.host.adv_data,1,GAP_AD_TYPE_COMPLETE_NAME,ADV_NAME,sizeof(ADV_NAME));
    return osapp_msg_build_send(cmd,sizeof(struct gapm_start_advertise_cmd));
        
}
static int32_t osapp_sec_send_security_req(uint16_t dest_id)
{
	struct gapc_security_cmd *cmd = AHI_MSG_ALLOC(GAPC_SECURITY_CMD,dest_id,gapc_security_cmd);
	cmd->operation = GAPC_SECURITY_REQ;
	cmd->auth = GAP_AUTH_REQ_MITM_BOND;
	return osapp_msg_build_send(cmd,sizeof(struct gapc_security_cmd));
}
static int32_t osapp_gapc_conn_confirm(ke_task_id_t dest_id,enum gap_auth auth)
{
    struct gapc_connection_cfm *cfm = AHI_MSG_ALLOC(GAPC_CONNECTION_CFM, dest_id, gapc_connection_cfm);
    cfm->auth = auth;
    return osapp_msg_build_send(cfm,sizeof(struct gapc_connection_cfm));
}
static int32_t osapp_connection_update_param(uint16_t intv_min,uint16_t intv_max,uint16_t latency,uint16_t timeout)
{
	struct gapc_param_update_cmd *cmd = AHI_MSG_ALLOC(GAPC_PARAM_UPDATE_CMD,KE_BUILD_ID(TASK_ID_GAPC, osapp_hid_env.conidx),gapc_param_update_cmd);
	cmd->operation = GAPC_UPDATE_PARAMS;
	cmd->intv_min = intv_min;
    cmd->intv_max = intv_max;
    cmd->latency  = latency;
    cmd->time_out = timeout;
    // not used by a slave device
    cmd->ce_len_min = 0xFFFF;
    cmd->ce_len_max = 0xFFFF;
	return osapp_msg_build_send(cmd,sizeof(struct gapc_param_update_cmd));
}
static int32_t osapp_hid_enable_prf(uint8_t conidx)
{
	    // Store the connection handle
    osapp_hid_env.conidx = conidx;
	uint16_t ntf_cfg;
    // Allocate the message
    struct hogpd_enable_req * req = AHI_MSG_ALLOC(HOGPD_ENABLE_REQ,TASK_ID_HOGPD,hogpd_enable_req);
	// Fill in the parameter structure
    req->conidx     = conidx;
    // Notifications are disabled
    ntf_cfg         = 0;
    // Go to Enabled state
    osapp_hid_env.state = APP_HID_ENABLED;
	
	if(osapp_sec_env.bonded)
	{
		//TODO
	}
	req->ntf_cfg[conidx] = ntf_cfg;
	// Requested connection parameters
//	osapp_connection_update_param(8,8,25,100);
	
	return osapp_msg_build_send(req,sizeof(struct hogpd_enable_req));
	
}
static void osapp_defualt_msg_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"defualt handler,msgid: 0x%x, dest_id: 0x%x, src_id: 0x%x\n",msgid,dest_id,src_id);

}
static void osapp_gapm_profile_added_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"GAPM profile added indication\n");
}
static void osapp_device_ready_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
	osapp_sec_init();
	osapp_hid_init();
	osapp_gapm_reset();
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
        osapp_add_hogpd_task();
        break;
    case GAPM_PROFILE_TASK_ADD:
        BX_ASSERT(cmp_evt->status==GAP_ERR_NO_ERROR);
        osapp_start_advertising();
        break;
    default:
        LOG(LOG_LVL_WARN,"gapm_cmp_evt operation:0x%x,status:0x%x\n",cmp_evt->operation,cmp_evt->status);
        break;
            
    }
}
static void osapp_gapc_disconnect_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    osapp_start_advertising();
}
static void osapp_gapc_conn_req_ind_handler(ke_msg_id_t const msgid,struct gapc_connection_req_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"conn_req_ind:src_id:%d,conhdl:%d\n",src_id,param->conhdl);
    osapp_gapc_conn_confirm(src_id,osapp_sec_env.bonded?GAP_AUTH_REQ_NO_MITM_BOND:GAP_AUTH_REQ_NO_MITM_NO_BOND);
	osapp_hid_enable_prf(param->conhdl);
	if(osapp_sec_env.bonded)
	{
		osapp_sec_send_security_req(src_id);
	}
	
}
static void osapp_hogpd_enable_rsp_handler(ke_msg_id_t const msgid,struct hogpd_enable_rsp const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
	LOG(LOG_LVL_INFO,"osapp_hogpd_enable_rsp\n");
}
static void osapp_gapc_bond_req_ind_handler(ke_msg_id_t const msgid,struct gapc_bond_req_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
        LOG(LOG_LVL_INFO,"bond_req,request:%d\n",param->request);
	struct gapc_bond_cfm *cfm = AHI_MSG_ALLOC(GAPC_BOND_CFM,src_id,gapc_bond_cfm);
	switch(param->request)
	{
	    case GAPC_PAIRING_REQ:
	    {
	        cfm->request = GAPC_PAIRING_RSP;
	        cfm->accept = true;
        	        cfm->data.pairing_feat.auth      = GAP_AUTH_REQ_MITM_BOND;
	        cfm->data.pairing_feat.iocap     = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
	        cfm->data.pairing_feat.key_size  = 16;
                cfm->data.pairing_feat.oob       = GAP_OOB_AUTH_DATA_NOT_PRESENT;
                cfm->data.pairing_feat.sec_req   = GAP_NO_SEC;
                cfm->data.pairing_feat.ikey_dist = GAP_KDIST_ENCKEY |GAP_KDIST_SIGNKEY;
                cfm->data.pairing_feat.rkey_dist = GAP_KDIST_ENCKEY |GAP_KDIST_SIGNKEY;
	    }
	    break;
            case GAPC_CSRK_EXCH:
            {
                cfm->accept  = true;
                cfm->request = GAPC_CSRK_EXCH;
                uint8_t i;
                for(i=0;i<KEY_LEN;i+=sizeof(uint32_t))
                {
                    uint32_t rand_num = co_rand_word();
                    memcpy(&cfm->data.csrk.key[i],&rand_num,sizeof(uint32_t));
                }
            }
            break;
	    case GAPC_LTK_EXCH:
	    {
			// Counter
                uint8_t counter;
                cfm->accept  = true;
                cfm->request = GAPC_LTK_EXCH;

                // Generate all the values
                cfm->data.ltk.ediv = (uint16_t)co_rand_word();

                for (counter = 0; counter < RAND_NB_LEN; counter++)
                {
                    cfm->data.ltk.ltk.key[counter]    = (uint8_t)co_rand_word();
                    cfm->data.ltk.randnb.nb[counter] = (uint8_t)co_rand_word();
                }

                for (counter = RAND_NB_LEN; counter < KEY_LEN; counter++)
                {
                    cfm->data.ltk.ltk.key[counter]    = (uint8_t)co_rand_word();
                }

	    }
	    break;
	    default:
	    {
		   LOG(LOG_LVL_INFO,"bond_req->request:0x%x",param->request);
	            BX_ASSERT(0);
	    }
	    break;
	}
	osapp_msg_build_send(cfm,sizeof(struct gapc_bond_cfm));
}
static int32_t osapp_hid_keyboard_report()
{
	                // Allocate the HOGPD_REPORT_UPD_REQ message
    struct hogpd_report_upd_req * req = AHI_MSG_ALLOC_DYN(HOGPD_REPORT_UPD_REQ,prf_get_task_from_id(TASK_ID_HOGPD),hogpd_report_upd_req,KEYBOARD_REPORT_LENGTH);
    req->conidx = osapp_hid_env.conidx;
    req->report.hid_idx = osapp_hid_env.conidx;
    req->report.idx = 0;
    req->report.length = KEYBOARD_REPORT_LENGTH;
    memset(req->report.value,0,KEYBOARD_REPORT_LENGTH);
    req->report.value[2] = 0x81;        // Keyboard Volume Down
    
    return osapp_msg_build_send(req,sizeof(struct hogpd_report_upd_req)+KEYBOARD_REPORT_LENGTH);
}
static void osapp_gapc_bond_ind_handler(ke_msg_id_t const msgid,struct gapc_bond_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
         LOG(LOG_LVL_INFO,"gapc_bond_ind\n");
	switch(param->info)
	{
		case GAPC_PAIRING_SUCCEED:
		{
			osapp_sec_env.bonded = true;		
		}
		break;
		case GAPC_REPEATED_ATTEMPT:
		{
			LOG(LOG_LVL_INFO,"GAPC_REPEATED_ATTEMPT\n");
		}
		break;
                  case GAPC_CSRK_EXCH:
                  {
                            LOG(LOG_LVL_INFO,"bond_ind:csrk\n");
                  }
		case GAPC_LTK_EXCH:
		{
			LOG(LOG_LVL_INFO,"bond_ind:ltk\n");
		}
		break;
		case GAPC_PAIRING_FAILED:
		{
			LOG(LOG_LVL_INFO,"GAPC_PAIRING_FAILED,reason:0x%x\n",param->data.reason);
			osapp_sec_send_security_req(src_id);
		}
		break;
		default:
		{
			LOG(LOG_LVL_INFO,"bond_ind->info:0x%x",param->info);
			BX_ASSERT(0);
		}
		break;
	}
}
static void osapp_hogpd_report_req_ind_handler(ke_msg_id_t const msgid,
                                    struct hogpd_report_req_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"report_req,op:%d,type:%d\n",param->operation,param->report.type);
    if((param->operation==HOGPD_OP_REPORT_READ)&&(param->report.type == HOGPD_REPORT_MAP))
    {
        uint16_t report_length = sizeof(keyboard_report_map);
        struct hogpd_report_cfm *req = AHI_MSG_ALLOC_DYN(HOGPD_REPORT_CFM, src_id, hogpd_report_cfm, report_length);
        req->conidx = osapp_hid_env.conidx;
        req->operation = HOGPD_OP_REPORT_READ;
        req->status = GAP_ERR_NO_ERROR;
        req->report.hid_idx = param->report.hid_idx;
        req->report.type = HOGPD_REPORT_MAP;
        req->report.length = report_length;
        req->report.idx = 0; // for report map
        memcpy(req->report.value,keyboard_report_map,report_length);
        osapp_msg_build_send(req, sizeof(struct hogpd_report_cfm)+report_length);
    }else
    {
        struct hogpd_report_cfm *req = AHI_MSG_ALLOC_DYN(HOGPD_REPORT_CFM, src_id, hogpd_report_cfm, param->report.length);
        req->conidx = param->conidx;
        req->operation = HOGPD_OP_REPORT_READ;
        req->status = GAP_ERR_NO_ERROR;
        req->report.type = param->report.type;
        req->report.length = param->report.length;
        req->report.idx = param->report.idx;
        memset(req->report.value,0,param->report.length);
        osapp_msg_build_send(req, sizeof(struct hogpd_report_cfm)+param->report.length);
    }
}
static void osapp_hogpd_proto_mode_req_ind_handler(ke_msg_id_t const msgid,
                                        struct hogpd_proto_mode_req_ind const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"proto_mode_req:conidx:%d,op:%d,mode:%d,hid_idx:%d\n",param->conidx,param->operation,param->proto_mode,param->hid_idx);
    struct hogpd_proto_mode_cfm *cfm = AHI_MSG_ALLOC(HOGPD_PROTO_MODE_CFM,src_id,hogpd_proto_mode_cfm);
    cfm->conidx = param->conidx;
    cfm->status = GAP_ERR_NO_ERROR;
    cfm->hid_idx = param->hid_idx;
    cfm->proto_mode = HOGP_REPORT_PROTOCOL_MODE;
    osapp_msg_build_send(cfm,sizeof(struct hogpd_proto_mode_cfm));
}
static void osapp_hogpd_report_upd_handler(ke_msg_id_t const msgid,
                                   struct hogpd_report_upd_rsp const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"report_upd\n");
}
static void osapp_hogpd_ctnl_pt_ind_handler(ke_msg_id_t const msgid,
                                     struct hogpd_ctnl_pt_ind const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"cntl_pt_ind:conidx:%d,hid_idx:%d,cntl_pt:%d\n",param->conidx,param->hid_idx,param->hid_ctnl_pt);
}
static void osapp_hogpd_ntf_cfg_ind_handler(ke_msg_id_t const msgid,
                                     struct hogpd_ntf_cfg_ind const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"ntf_cfg_ind\n");
    if(osapp_hid_env.conidx == param->conidx)
    {
        if ((param->ntf_cfg[param->conidx] & HOGPD_CFG_REPORT_NTF_EN ) != 0)
        {
            // The device is ready to send reports to the peer device
            osapp_hid_env.state = APP_HID_READY;
        }
        else
        {
            // Come back to the Enabled state
            if (osapp_hid_env.state == APP_HID_READY)
            {
                osapp_hid_env.state = APP_HID_ENABLED;
            }
        }
    }
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
static void osapp_gapc_cmp_evt_handler(ke_msg_id_t const msgid, struct gapc_cmp_evt const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"gapc_cmp_evt:op:%d,status:%d\n",param->operation,param->status);
}
static void osapp_gattc_mtu_changed_ind_handler(ke_msg_id_t const msgid,struct gattc_mtu_changed_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"Exchanged MTU value:%d, seq_num:%d\n",param->mtu,param->seq_num);
}
static const osapp_msg_handler_table_t handler_table[]=
{
        [0] =   {KE_MSG_DEFAULT_HANDLER,(osapp_msg_handler_t)osapp_defualt_msg_handler},
            {HOGPD_REPORT_REQ_IND,(osapp_msg_handler_t)osapp_hogpd_report_req_ind_handler},
            {HOGPD_PROTO_MODE_REQ_IND, (osapp_msg_handler_t) osapp_hogpd_proto_mode_req_ind_handler},
            {HOGPD_REPORT_UPD_RSP,(osapp_msg_handler_t)osapp_hogpd_report_upd_handler},
            {HOGPD_CTNL_PT_IND, (osapp_msg_handler_t)osapp_hogpd_ctnl_pt_ind_handler},
            {HOGPD_NTF_CFG_IND,(osapp_msg_handler_t) osapp_hogpd_ntf_cfg_ind_handler},
            {HOGPD_ENABLE_RSP,      (osapp_msg_handler_t)osapp_hogpd_enable_rsp_handler},
            {GAPC_BOND_REQ_IND,  (osapp_msg_handler_t)osapp_gapc_bond_req_ind_handler},
            {GAPC_BOND_IND,      (osapp_msg_handler_t)osapp_gapc_bond_ind_handler},
            {GAPC_CONNECTION_REQ_IND,(osapp_msg_handler_t)osapp_gapc_conn_req_ind_handler},
            {GAPC_DISCONNECT_IND,       (osapp_msg_handler_t)osapp_gapc_disconnect_ind_handler},
            {GAPC_GET_DEV_INFO_REQ_IND,(osapp_msg_handler_t)osapp_gapc_get_dev_info_req_ind_handler},
            {GAPC_CMP_EVT,(osapp_msg_handler_t)osapp_gapc_cmp_evt_handler},
            {GAPM_PROFILE_ADDED_IND,(osapp_msg_handler_t)osapp_gapm_profile_added_ind_handler},
            {GAPM_CMP_EVT,(osapp_msg_handler_t)osapp_gapm_cmp_evt_handler},
            {GATTC_MTU_CHANGED_IND,(osapp_msg_handler_t)osapp_gattc_mtu_changed_ind_handler},
            {GAPM_DEVICE_READY_IND,(osapp_msg_handler_t)osapp_device_ready_ind_handler},
            
};
const osapp_msg_handler_info_t handler_info = ARRAY_INFO(handler_table);
#endif
