#include "osapp_config.h"
#include "m_uart.h"
#include <stdlib.h>
#include "log.h"
#include "osapp_utils.h"
#include "app_uart.h"
#define APP_ADV_CHMAP 0x7
#define APP_ADV_INT 32
#define UART_SVC_ADV_NAME "Apollo BLE UART Server"
#define UART_SVC_UUID_128 {0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,\
    0xe0,0x93,0xf3,0xa3,0xb5,0x01,0x00,0x40,0x6e}
#define UART_SVC_RX_CHAR_UUID_128 {0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,\
    0xe0,0x93,0xf3,0xa3,0xb5,0x02,0x00,0x40,0x6e}
#define UART_SVC_TX_CHAR_UUID_128 {0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,\
    0xe0,0x93,0xf3,0xa3,0xb5,0x03,0x00,0x40,0x6e}
#define ATT_DECL_CHAR_ARRAY {0x03,0x28}
#define ATT_DESC_CLIENT_CHAR_CFG_ARRAY {0x02,0x29}
#define UART_SVC_RX_BUF_SIZE 64
#define UART_SVC_TX_BUF_SIZE 64
//slave preferred parameters
#define CONN_INTERVAL_MIN   8
#define CONN_INTERVAL_MAX   10
#define SLAVE_LATENCY       0
#define CONN_TIMEOUT        200


enum uart_svc_att_db_handles
{
    UART_SVC_IDX_RX_CHAR,
    UART_SVC_IDX_RX_VAL,
    UART_SVC_IDX_TX_CHAR,
    UART_SVC_IDX_TX_VAL,
    UART_SVC_IDX_TX_NTF_CFG,
    UART_SVC_ATT_NUM
};
struct gattm_svc_desc const uart_svc_desc ={
        .start_hdl = 0,
        .task_id = TASK_ID_AHI,
        .perm = PERM(SVC_MI,DISABLE)|PERM(SVC_EKS,DISABLE)|\
            PERM(SVC_AUTH,NO_AUTH)|PERM(SVC_UUID_LEN,UUID_128),PERM_VAL(SVC_SECONDARY,0),
        .nb_att = UART_SVC_ATT_NUM,
        .uuid = UART_SVC_UUID_128,
};
struct gattm_att_desc const uart_svc_att_db[UART_SVC_ATT_NUM] = {
            [UART_SVC_IDX_RX_CHAR] = {
                .uuid = ATT_DECL_CHAR_ARRAY,
                .perm = PERM(RD,ENABLE),
                .max_len = 0,
                .ext_perm= PERM(UUID_LEN,UUID_16),
            },
            [UART_SVC_IDX_RX_VAL] = {
                .uuid = UART_SVC_RX_CHAR_UUID_128,
                .perm = PERM(WRITE_REQ,ENABLE)|PERM(WRITE_COMMAND,ENABLE)|PERM(WP,NO_AUTH),
                .max_len = UART_SVC_RX_BUF_SIZE,
                .ext_perm = PERM(UUID_LEN,UUID_128)|PERM(RI,ENABLE),
            },
            [UART_SVC_IDX_TX_CHAR] = {
                .uuid = ATT_DECL_CHAR_ARRAY,
                .perm = PERM(RD,ENABLE),
                .max_len = 0,
                .ext_perm = PERM(UUID_LEN,UUID_16),
            },
            [UART_SVC_IDX_TX_VAL] = {

                .uuid = UART_SVC_TX_CHAR_UUID_128,
                .perm = PERM(NTF,ENABLE),
                .max_len = UART_SVC_TX_BUF_SIZE,
                .ext_perm = PERM(UUID_LEN,UUID_128)|PERM(RI,ENABLE),
            },
            [UART_SVC_IDX_TX_NTF_CFG] = {
              .uuid = ATT_DESC_CLIENT_CHAR_CFG_ARRAY,
              .perm = PERM(RD,ENABLE)|PERM(WRITE_REQ,ENABLE),
               .max_len = 0,
               .ext_perm = PERM(UUID_LEN,UUID_16),
            },
};
app_uart_inst_t uart0_inst = UART_INSTANCE(0);
uint8_t uart0_buf;
static uint8_t conn_hdl;
bool uart0_tx_busy;
static void uart_server_read_req_ind(osapp_svc_helper_t const *,ke_task_id_t const,uint16_t);
static void uart_server_write_req_ind(osapp_svc_helper_t const *,ke_task_id_t const,uint16_t,uint16_t,uint16_t,uint8_t const*);
osapp_svc_helper_t uart_server_svc_helper = 
{
    .svc_desc = &uart_svc_desc,
    .att_desc = uart_svc_att_db,
    .att_num = UART_SVC_ATT_NUM,
    .read = uart_server_read_req_ind,
    .write = uart_server_write_req_ind,
};

static void uart_server_read_req_ind(osapp_svc_helper_t const *svc_helper,ke_task_id_t const src_id,uint16_t att_idx)
{

    LOG(LOG_LVL_INFO,"read att_idx:%d\n",att_idx);
    if(att_idx == UART_SVC_IDX_TX_NTF_CFG)
    {
        struct gattc_read_cfm *cfm = AHI_MSG_ALLOC(GATTC_READ_CFM,src_id, gattc_read_cfm);
        cfm->handle = osapp_get_att_handle_helper(svc_helper,att_idx);
        cfm->length=0;
        cfm->status = 0;
        osapp_ahi_msg_send(cfm, sizeof(struct gattc_read_cfm),portMAX_DELAY);
    }

}

void uart_write_callback(void *dummy,uint8_t status)
{
	uart0_tx_busy = false;
    LOG(LOG_LVL_WARN,"uart tx done\n");
}

extern uint8_t rx_ble_cmd;

static void uart_server_write_req_ind(osapp_svc_helper_t const *svc_helper,ke_task_id_t const src_id,uint16_t att_idx,uint16_t offset,uint16_t length,uint8_t const*value)
{

    LOG(LOG_LVL_INFO,"write att_idx:%d\n",att_idx);
    struct gattc_write_cfm *cfm = AHI_MSG_ALLOC(GATTC_WRITE_CFM,src_id,gattc_write_cfm);
    cfm->status = ATT_ERR_NO_ERROR;
    cfm->handle = osapp_get_att_handle_helper(svc_helper,att_idx);
    osapp_ahi_msg_send(cfm, sizeof(struct gattc_write_cfm),portMAX_DELAY);  
    if(att_idx != UART_SVC_IDX_RX_VAL)
    {
        debugPrint("Rx data error%x\n" );
    
    	return;
    }

    /*
    if(uart0_tx_busy)
    {
        LOG(LOG_LVL_WARN,"tx busy,data discard\n");
    }else
    {
    	uart0_tx_busy = true;
        app_uart_write(&uart0_inst.inst, value, length, uart_write_callback,NULL);
    }*/
    //debugPrint the data



    rx_ble_cmd = value[0];



}

static void osapp_start_advertising()
{
        // Prepare the GAPM_START_ADVERTISE_CMD message
    struct gapm_start_advertise_cmd *cmd = AHI_MSG_ALLOC(GAPM_START_ADVERTISE_CMD,TASK_ID_GAPM, gapm_start_advertise_cmd);
    cmd->op.addr_src    = GAPM_STATIC_ADDR;
    cmd->channel_map    = APP_ADV_CHMAP;
    cmd->intv_min = APP_ADV_INT;
    cmd->intv_max = APP_ADV_INT;
    cmd->op.code        = GAPM_ADV_UNDIRECT;
    cmd->info.host.mode = GAP_GEN_DISCOVERABLE;
            // Flag value is set by the GAP
    cmd->info.host.adv_data_len = ADV_DATA_PACK(cmd->info.host.adv_data,1,GAP_AD_TYPE_COMPLETE_NAME,\
        UART_SVC_ADV_NAME,sizeof(UART_SVC_ADV_NAME));
    cmd->info.host.scan_rsp_data_len  = 0;
    osapp_ahi_msg_send(cmd,sizeof(struct gapm_start_advertise_cmd),portMAX_DELAY);
        
}

static int32_t osapp_gapc_param_update_cfm(ke_task_id_t const src_id)
{
    struct gapc_param_update_cfm *cfm = AHI_MSG_ALLOC(GAPC_PARAM_UPDATE_CFM, src_id, gapc_param_update_cfm);
    cfm->accept = 0x01;
    cfm->ce_len_max = 0xffff;
    cfm->ce_len_min = 0xffff;
    return osapp_msg_build_send(cfm, sizeof(struct gapc_param_update_cfm));

}

static void osapp_set_dev_config(uint8_t role,uint8_t addr_type,uint8_t pairing_mode)
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
    cmd->max_mtu = GAP_MAX_LE_MTU;
    osapp_ahi_msg_send(cmd, sizeof(struct gapm_set_dev_config_cmd),portMAX_DELAY);
}

static void osapp_gapc_param_update_req_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"param update request\n");
    osapp_gapc_param_update_cfm(src_id);
}

static void add_svc_callback(uint8_t status,osapp_svc_helper_t *svc_helper)
{
    osapp_start_advertising();
}

static void osapp_gapm_cmp_evt_handler(ke_msg_id_t const msgid, struct gapm_cmp_evt const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
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
        osapp_add_svc_req_helper(&uart_server_svc_helper,1,add_svc_callback);
        break;
    case GAPM_ADV_UNDIRECT:
        LOG(LOG_LVL_WARN,"adv status:%d\n",cmp_evt->status);
        break;
    default:
        LOG(LOG_LVL_WARN,"gapm_cmp_evt operation:0x%x\n",cmp_evt->operation);
        break;
            
    }
}

static void osapp_gapc_disconnect_ind_handler(ke_msg_id_t const msgid, struct gapc_disconnect_ind const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"disconn:%d\n",param->reason);
    osapp_start_advertising();
}

static void osapp_gapc_conn_confirm(ke_task_id_t dest_id)
{
    struct gapc_connection_cfm *cfm = AHI_MSG_ALLOC(GAPC_CONNECTION_CFM, dest_id, gapc_connection_cfm);
    memset(cfm,0,sizeof(struct gapc_connection_cfm));
    osapp_ahi_msg_send(cfm,sizeof(struct gapc_connection_cfm),portMAX_DELAY);
}

void osapp_send_notification_isr(ke_task_id_t const dest_id,uint8_t const *data,uint16_t length)
{
    static uint16_t notify_seq_num = 0;
    struct gattc_send_evt_cmd *cmd= AHI_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,dest_id, gattc_send_evt_cmd, length);
    cmd->operation = GATTC_NOTIFY;
    cmd->seq_num = notify_seq_num++;
    cmd->handle = osapp_get_att_handle_helper(&uart_server_svc_helper,UART_SVC_IDX_TX_VAL);
    cmd->length = length;
    memcpy(cmd->value,data,length);
    osapp_ahi_msg_send_isr(cmd, sizeof(struct gattc_send_evt_cmd) + length);
}

static void osapp_send_notification(ke_task_id_t const dest_id,uint8_t const *data,uint16_t length)
{
    static uint16_t notify_seq_num = 0;
    struct gattc_send_evt_cmd *cmd= AHI_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,dest_id, gattc_send_evt_cmd, length);
    cmd->operation = GATTC_NOTIFY;
    cmd->seq_num = notify_seq_num++;
    cmd->handle = osapp_get_att_handle_helper(&uart_server_svc_helper,UART_SVC_IDX_TX_VAL);
    cmd->length = length;
    memcpy(cmd->value,data,length);
    osapp_ahi_msg_send(cmd, sizeof(struct gattc_send_evt_cmd) + length,portMAX_DELAY);
}

void uart0_rx_callback(void *dummy,uint8_t status)
{
    osapp_send_notification(KE_BUILD_ID(TASK_ID_GATTC, conn_hdl),&uart0_buf,sizeof(uart0_buf));
}

static void osapp_gapc_conn_req_ind_handler(ke_msg_id_t const msgid, struct gapc_connection_req_ind const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    osapp_gapc_conn_confirm(src_id);
    conn_hdl = KE_IDX_GET(src_id);

    /*
    uart0_inst.param.baud_rate = UART_BAUDRATE_115200;
    uart0_inst.param.tx_pin_no = 12;
    uart0_inst.param.rx_pin_no = 13;
    app_uart_init(&uart0_inst.inst);
    app_uart_read(&uart0_inst.inst,&uart0_buf,sizeof(uart0_buf), uart0_rx_callback,NULL);
    */
}

extern int32_t whole_bmpfile_size;
extern uint8_t *bmp_src_ptr ;
extern uint32_t ble_transmit_counter ;

static void osapp_gattc_cmp_evt_handler(ke_msg_id_t const msgid,struct gattc_cmp_evt const *param,ke_task_id_t const dest_id,ke_task_id_t src_id)
{
    switch(param->operation)
    {
        case GATTC_NOTIFY:
            
            //debugPrint("Done\n\r" );
            
            //Fill data into uart0_buf and call rx call back in another task,until total size
            //app_uart_read(&uart0_inst.inst,&uart0_buf,sizeof(uart0_buf), uart0_rx_callback,NULL);
            if(param->status == ATT_ERR_NO_ERROR)
            {
                if (ble_transmit_counter < whole_bmpfile_size){
                
                    uart0_buf = bmp_src_ptr[ble_transmit_counter];
                    //debugPrint("Tx %d:%x\n\r",ble_transmit_counter, uart0_buf );
                    ble_transmit_counter++;
                
                    uart0_rx_callback(NULL,0);
                }


            
                //LOG(LOG_LVL_INFO,"notification done\n\r");
            }else
            {
                LOG(LOG_LVL_INFO,"noti status:%d\n\r",param->status);
            }
            break;
        default:
            LOG(LOG_LVL_INFO,"op:%d,seq:%d,status:%d\n",param->operation,param->seq_num,param->status);
            break;
    }
}


static void osapp_gapc_get_dev_info_req_ind_handler(ke_msg_id_t const msgid, struct gapc_get_dev_info_req_ind const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	struct gapc_get_dev_info_cfm *cfm;
    switch(param->req)
    {
    	case GAPC_DEV_NAME:
    		cfm = AHI_MSG_ALLOC_DYN(GAPC_GET_DEV_INFO_CFM,src_id, gapc_get_dev_info_cfm,sizeof(UART_SVC_ADV_NAME));
    		cfm->req = GAPC_DEV_NAME;
			cfm->info.name.length = sizeof(UART_SVC_ADV_NAME);
			memcpy(cfm->info.name.value,UART_SVC_ADV_NAME,sizeof(UART_SVC_ADV_NAME));
			osapp_ahi_msg_send(cfm, sizeof(struct gapc_get_dev_info_cfm)+sizeof(UART_SVC_ADV_NAME),portMAX_DELAY);
			break;
		case GAPC_DEV_APPEARANCE:
			cfm=AHI_MSG_ALLOC(GAPC_GET_DEV_INFO_CFM, src_id, gapc_get_dev_info_cfm);
			cfm->req = GAPC_DEV_APPEARANCE;
			cfm->info.appearance = 0;
			osapp_ahi_msg_send(cfm, sizeof(struct gapc_get_dev_info_cfm),portMAX_DELAY);
			break;
		case GAPC_DEV_SLV_PREF_PARAMS:
			cfm=AHI_MSG_ALLOC(GAPC_GET_DEV_INFO_CFM, src_id, gapc_get_dev_info_cfm);
			cfm->req = GAPC_DEV_SLV_PREF_PARAMS;
			cfm->info.slv_params.con_intv_min  = CONN_INTERVAL_MIN;
			cfm->info.slv_params.con_intv_max  = CONN_INTERVAL_MAX;
			cfm->info.slv_params.slave_latency = SLAVE_LATENCY;
			cfm->info.slv_params.conn_timeout  = CONN_TIMEOUT;
			osapp_ahi_msg_send(cfm, sizeof(struct gapc_get_dev_info_cfm),portMAX_DELAY);
			break;
    }
}

static void osapp_reset()
{
    struct gapm_reset_cmd *cmd = AHI_MSG_ALLOC(GAPM_RESET_CMD,TASK_ID_GAPM,gapm_reset_cmd);
    cmd->operation = GAPM_RESET;
    osapp_ahi_msg_send(cmd, sizeof(struct gapm_reset_cmd),portMAX_DELAY);
}

static void osapp_device_ready_ind_handler(ke_msg_id_t const msgid, void const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    osapp_reset();
}

static void osapp_default_msg_handler(ke_msg_id_t const msgid, void const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"default handler,msgid: 0x%x, dest_id: 0x%x, src_id: 0x%x\n",msgid,dest_id,src_id);
}

static osapp_msg_handler_table_t const handler_table[]=
{
    [0] =   {KE_MSG_DEFAULT_HANDLER,(osapp_msg_handler_t)osapp_default_msg_handler},
                {GAPM_DEVICE_READY_IND,(osapp_msg_handler_t)osapp_device_ready_ind_handler},
                {GAPM_CMP_EVT,(osapp_msg_handler_t)osapp_gapm_cmp_evt_handler},
                {GAPC_PARAM_UPDATE_REQ_IND,(osapp_msg_handler_t)osapp_gapc_param_update_req_handler},
                {GATTM_ADD_SVC_RSP,(osapp_msg_handler_t)osapp_add_svc_rsp_helper_handler},
                {GAPC_CONNECTION_REQ_IND,(osapp_msg_handler_t)osapp_gapc_conn_req_ind_handler},
                {GAPC_DISCONNECT_IND,(osapp_msg_handler_t)osapp_gapc_disconnect_ind_handler},
                {GATTC_WRITE_REQ_IND,(osapp_msg_handler_t)osapp_write_req_ind_helper_handler},
                {GATTC_CMP_EVT,(osapp_msg_handler_t)osapp_gattc_cmp_evt_handler},
                {GAPC_GET_DEV_INFO_REQ_IND,(osapp_msg_handler_t)osapp_gapc_get_dev_info_req_ind_handler},
                {GATTC_READ_REQ_IND,(osapp_msg_handler_t)osapp_read_req_ind_helper_handler}
};
osapp_msg_handler_info_t const handler_info = ARRAY_INFO(handler_table);

