/*
 * osapp_ams_demo.c
 *
 *  Created on: 2018��10��23��
 *      Author: mingzhou
 */

#include "osapp_config.h"
#include <stdlib.h>
#include <stdbool.h>
#include "prf_types.h"
#include "gattc_task.h"
#include "co_utils.h"
#include "osapp_ams_demo.h"
#include "log.h"
#include "prf_utils.h"
#include "osapp_utils.h"
#include "ancc_task.h"
#define APP_ADV_CHMAP 0x7
#define APP_ADV_INT 32
#define ADV_NAME "Apollo Apple Media Client"

//slave preferred parameters
#define CONN_INTERVAL_MIN   8
#define CONN_INTERVAL_MAX   10
#define SLAVE_LATENCY       0
#define CONN_TIMEOUT        200

#define BOND_PASSKEY        654321

#define ANCS_ENABLE
const struct prf_char_uuid128_def amsc_char[AMSC_CHAR_MAX] =
{
                                            [AMSC_CHAR_REMOTE_CMD] = { ATT_UUID_128_LEN,    // Remote Command
                                                                       AMSC_REMOTE_COMMAND_UUID_128,
                                                                       ATT_MANDATORY,
                                                                       ATT_CHAR_PROP_WR_NO_RESP|ATT_CHAR_PROP_NTF
                                                                       },

                                            [AMSC_CHAR_ENTITY_UPDATE] = { ATT_UUID_128_LEN, // Entity Update
                                                                          AMSC_ENTITY_UPDATE_UUID_128,
                                                                          ATT_MANDATORY,
                                                                          ATT_CHAR_PROP_WR|ATT_CHAR_PROP_NTF
                                                                          },

                                            [AMSC_CHAR_ENTITY_ATT] = { ATT_UUID_128_LEN, // Entity Attribute
                                                                       AMSC_ENTITY_ATTRIBUTE_UUID_128,
                                                                       ATT_OPTIONAL,
                                                                       ATT_CHAR_PROP_WR_NO_RESP|ATT_CHAR_PROP_RD
                                                                       },
};

const struct prf_char_desc_def ams_char_desc[AMSC_DESC_MAX] =
{
                                            [AMSC_DESC_REMOTE_CMD_CFG] = { ATT_DESC_CLIENT_CHAR_CFG,
                                                                           ATT_MANDATORY,
                                                                           AMSC_CHAR_REMOTE_CMD
                                                                           },
                                            [AMSC_DESC_ENTITY_UPDATE_CFG] = { ATT_DESC_CLIENT_CHAR_CFG,
                                                                           ATT_MANDATORY,
                                                                           AMSC_CHAR_ENTITY_UPDATE
                                                                           },
};

static struct bond_info_t bond_info;
static struct amsc_content amsc_content_env;
static uint8_t amsc_next_notf_entity;
static uint8_t conn_hdl;

#ifdef ANCS_ENABLE
static void osapp_ancc_enable_req(uint8_t conidx)
{
    struct ancc_enable_req *req = AHI_MSG_ALLOC(ANCC_ENABLE_REQ,KE_BUILD_ID(TASK_ID_ANCC,conidx),ancc_enable_req);
    req->con_type = PRF_CON_DISCOVERY;
    osapp_ahi_msg_send(req,sizeof(struct ancc_enable_req),portMAX_DELAY);
}

static void osapp_add_ancc_task()
{
    struct gapm_profile_task_add_cmd *req=AHI_MSG_ALLOC(GAPM_PROFILE_TASK_ADD_CMD, TASK_ID_GAPM, gapm_profile_task_add_cmd);
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl = PERM(SVC_AUTH,NO_AUTH);
    req->prf_task_id = TASK_ID_ANCC;
    req->app_task = TASK_AHI;
    req->start_hdl = 0;
    osapp_ahi_msg_send(req,sizeof(struct gapm_profile_task_add_cmd),portMAX_DELAY);
}

static void osapp_ancc_ntf_src_enable()
{
    struct ancc_cl_cfg_ntf_en_cmd *cmd = AHI_MSG_ALLOC(ANCC_CL_CFG_NTF_EN_CMD,TASK_ID_ANCC,ancc_cl_cfg_ntf_en_cmd);
    cmd->cfg = ANC_DESC_NTF_SRC_CL_CFG;
    osapp_ahi_msg_send(cmd,sizeof(struct ancc_cl_cfg_ntf_en_cmd),portMAX_DELAY);
}

static void osapp_ancc_data_src_enable()
{
    struct ancc_cl_cfg_ntf_en_cmd *cmd = AHI_MSG_ALLOC(ANCC_CL_CFG_NTF_EN_CMD,TASK_ID_ANCC,ancc_cl_cfg_ntf_en_cmd);
    cmd->cfg = ANC_DESC_DATA_SRC_CL_CFG;
    osapp_ahi_msg_send(cmd,sizeof(struct ancc_cl_cfg_ntf_en_cmd),portMAX_DELAY);
}

static void osapp_ancc_cmp_evt_handler(ke_msg_id_t const msgid, struct ancc_cmp_evt const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    BX_ASSERT(param->status == 0);
    switch(param->operation)
    {
    case ANCC_NTF_SRC_CL_CFG_NTF_ENABLING:
        osapp_ancc_data_src_enable();
    break;
    case ANCC_DATA_SRC_CL_CFG_NTF_ENABLING:

    break;
    }

}

static void osapp_ancc_enable_rsp_handler(ke_msg_id_t const msgid, struct ancc_enable_rsp const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"ancc enable rsp:%d\n",param->status);
    osapp_ancc_ntf_src_enable();
}

static void osapp_ancc_ntf_att_ind_handler(ke_msg_id_t const msgid, struct ancc_att_ind const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"ntf_att_ind:%d,length:%d\n",param->att_id,param->length);

}

static void osapp_ancc_app_att_ind_handler(ke_msg_id_t const msgid, struct ancc_att_ind const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"app_att_ind:%d,length:%d\n",param->att_id,param->length);
}

static void osapp_ancc_ntf_src_ind_handler(ke_msg_id_t const msgid, struct anc_ntf_src const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{

    LOG(LOG_LVL_INFO,"ntf_src:%d,%d,%d,%d,0x%x",param->event_id,param->event_flags,param->cat_id,param->cat_cnt,param->ntf_uid);
    if((param->cat_cnt != 0) && (param->event_id == EVT_ID_NTF_ADDED) && (param->cat_id == CAT_ID_CALL))
    {
        struct ancc_perform_ntf_action_cmd *cmd = AHI_MSG_ALLOC(ANCC_PERFORM_NTF_ACTION_CMD,src_id,ancc_perform_ntf_action_cmd);
        cmd->ntf_uid = param->ntf_uid;
        cmd->action_id = ACT_ID_NEGATIVE;
        osapp_ahi_msg_send(cmd,sizeof(struct ancc_perform_ntf_action_cmd),portMAX_DELAY);
    }
}

#endif

/*********************************               AMS                ******************************************/
static void set_amsc_state_op(uint8_t state, uint8_t op)
{
    amsc_content_env.amsc_state = state;
    amsc_content_env.operation = op;
}

static void osapp_amsc_init(void)
{
    memset((void*)&amsc_content_env, 0, sizeof(amsc_content_env));
    amsc_next_notf_entity = AMS_ENTITY_ID_PLAYER;
    set_amsc_state_op(AMSC_STATE_FREE, AMSC_OP_NULL);
}

static void osapp_amsc_prf_disc_svc_128_send(uint8_t conidx, uint8_t *uuid)
{
    //send GATT discover primary services by UUID request
    struct gattc_sdp_svc_disc_cmd * svc_req = AHI_MSG_ALLOC_DYN(GATTC_SDP_SVC_DISC_CMD, KE_BUILD_ID(TASK_ID_GATTC, conidx), \
                                                            gattc_sdp_svc_disc_cmd, ATT_UUID_128_LEN);
    //gatt request type: by UUID
    svc_req->operation         = GATTC_SDP_DISC_SVC;
    svc_req->start_hdl        = ATT_1ST_REQ_START_HDL;
    svc_req->end_hdl          = ATT_1ST_REQ_END_HDL;
    svc_req->uuid_len = ATT_UUID_128_LEN;
    memcpy((void*)&(svc_req->uuid[0]), (void*)uuid, ATT_UUID_128_LEN);
    osapp_ahi_msg_send(svc_req,sizeof(struct gattc_sdp_svc_disc_cmd)+ATT_UUID_128_LEN,portMAX_DELAY);
}

static void osapp_amsc_svc_disc_start(void)
{
    uint8_t service_uuid[ATT_UUID_128_LEN] = AMSC_SERVICE_UUID_128;
    osapp_amsc_prf_disc_svc_128_send(conn_hdl, &service_uuid[0]); // AMS service discovery start
    set_amsc_state_op(AMSC_STATE_DISCOVERING, AMSC_OP_SRV_DISC);
}

static void osapp_start_advertising(void)
{
    static uint8_t service_uuid_16[] = {0x12, 0x18};
    static uint8_t manu_specific_data[] = {0x01, 0x03, 0x01, 0x03, 0x00, 0xff, 0xcc};
    struct gapm_start_advertise_cmd *cmd = AHI_MSG_ALLOC(GAPM_START_ADVERTISE_CMD,TASK_ID_GAPM, gapm_start_advertise_cmd);
    cmd->op.addr_src    = GAPM_STATIC_ADDR;
    cmd->channel_map    = APP_ADV_CHMAP;
    cmd->intv_min = APP_ADV_INT;
    cmd->intv_max = APP_ADV_INT;
    cmd->op.code        = GAPM_ADV_UNDIRECT;
    cmd->info.host.mode = GAP_GEN_DISCOVERABLE;
    cmd->info.host.adv_data_len = ADV_DATA_PACK(cmd->info.host.adv_data,2,\
            GAP_AD_TYPE_MORE_16_BIT_UUID, service_uuid_16, sizeof(service_uuid_16),\
        GAP_AD_TYPE_MANU_SPECIFIC_DATA, manu_specific_data, sizeof(manu_specific_data));
    cmd->info.host.scan_rsp_data_len  = ADV_DATA_PACK(cmd->info.host.scan_rsp_data,1,\
            GAP_AD_TYPE_COMPLETE_NAME, ADV_NAME, sizeof(ADV_NAME));
    osapp_ahi_msg_send(cmd,sizeof(struct gapm_start_advertise_cmd),portMAX_DELAY);

    osapp_amsc_init();
}

void osapp_amsc_sec_send_security_req(uint16_t dest_id)
{
    struct gapc_security_cmd *cmd = AHI_MSG_ALLOC(GAPC_SECURITY_CMD,dest_id,gapc_security_cmd);
    cmd->operation = GAPC_SECURITY_REQ;
    cmd->auth = GAP_AUTH_REQ_NO_MITM_BOND;
    set_amsc_state_op(AMSC_STATE_ENCRYPT, AMSC_OP_ENCRYPT);
    osapp_msg_build_send(cmd,sizeof(struct gapc_security_cmd));
}

static void osapp_amsc_register_atthdl2gatt(uint8_t conidx)
{
    //register profile task in gatt for indication/notifications
    struct gattc_reg_to_peer_evt_cmd *reg = AHI_MSG_ALLOC(GATTC_REG_TO_PEER_EVT_CMD, KE_BUILD_ID(TASK_ID_GATTC, conidx), gattc_reg_to_peer_evt_cmd);

    reg->operation =  GATTC_REGISTER;
    reg->start_hdl = amsc_content_env.svc.shdl;
    reg->end_hdl =   amsc_content_env.svc.ehdl;

    LOG(LOG_LVL_INFO,"register shdl=%d, ehdl=%d\n", reg->start_hdl, reg->end_hdl);

    osapp_ahi_msg_send(reg,sizeof(struct gattc_reg_to_peer_evt_cmd),portMAX_DELAY);
}

static void osapp_amsc_write_remote_command(uint8_t conidx, uint8_t* rmtCmds, uint16_t cmdNum)
{
    struct gattc_write_cmd * wr_char = AHI_MSG_ALLOC_DYN(GATTC_WRITE_CMD, KE_BUILD_ID(TASK_ID_GATTC, conidx), \
                gattc_write_cmd, cmdNum);

    wr_char->offset = 0;
    wr_char->cursor = 0;
    wr_char->operation = GATTC_WRITE;//GATTC_WRITE_NO_RESPONSE;
    wr_char->handle = amsc_content_env.chars[AMSC_CHAR_REMOTE_CMD].val_hdl;
    wr_char->length = cmdNum;
    wr_char->auto_execute = true;
    memcpy(&wr_char->value[0], rmtCmds, cmdNum);

    osapp_ahi_msg_send(wr_char,sizeof(struct gattc_write_cmd)+cmdNum,portMAX_DELAY);

    set_amsc_state_op(AMSC_STATE_BUSY, AMSC_OP_WR_RMT_CMD);
}

static void osapp_amsc_write_entity_update(uint8_t conidx, uint8_t entityID, uint8_t* attIDs, uint16_t attNum)
{
    struct gattc_write_cmd * wr_char = AHI_MSG_ALLOC_DYN(GATTC_WRITE_CMD, KE_BUILD_ID(TASK_ID_GATTC, conidx), \
                gattc_write_cmd, attNum+1);

    wr_char->offset = 0;
    wr_char->cursor = 0;
    wr_char->operation = GATTC_WRITE;
    wr_char->handle = amsc_content_env.chars[AMSC_CHAR_ENTITY_UPDATE].val_hdl;
    wr_char->length = attNum+1;
    wr_char->auto_execute = true;
    wr_char->value[0] = entityID;
    memcpy(&wr_char->value[1], attIDs, attNum);

    osapp_ahi_msg_send(wr_char,sizeof(struct gattc_write_cmd)+attNum+1,portMAX_DELAY);
}

static void osapp_amsc_enable_all_notification(void)
{
    uint8_t attID_array[4];
    switch(amsc_next_notf_entity)
    {
        case AMS_ENTITY_ID_PLAYER:
            for(uint8_t i = AMS_PLAYER_ATTRIBUTE_ID_NAME; i < AMS_PLAYER_ATTRIBUTE_ID_MAX; i++)
            {
                attID_array[i] = i;
            }
            osapp_amsc_write_entity_update(conn_hdl, AMS_ENTITY_ID_PLAYER, &attID_array[0], AMS_PLAYER_ATTRIBUTE_ID_MAX);
            break;
        case AMS_ENTITY_ID_QUEUE:
            for(uint8_t i = AMS_QUEUE_ATTRIBUTE_ID_INDEX; i < AMS_QUEUE_ATTRIBUTE_ID_MAX; i++)
            {
                attID_array[i] = i;
            }
            osapp_amsc_write_entity_update(conn_hdl, AMS_ENTITY_ID_QUEUE, &attID_array[0], AMS_QUEUE_ATTRIBUTE_ID_MAX);
            break;
        case AMS_ENTITY_ID_TRACK:
            for(uint8_t i = AMS_TRACK_ATTRIBUTE_ID_ARTIST; i < AMS_TRACK_ATTRIBUTE_ID_MAX; i++)
            {
                attID_array[i] = i;
            }
            osapp_amsc_write_entity_update(conn_hdl, AMS_ENTITY_ID_TRACK, &attID_array[0], AMS_TRACK_ATTRIBUTE_ID_MAX);
            break;
        case AMS_ENTITY_ID_MAX:
            LOG(LOG_LVL_INFO,"asmc enable all notificatoin finished\n");
            set_amsc_state_op(AMSC_STATE_IDLE, AMSC_OP_NULL);
            break;
        default:
            BX_ASSERT(0);
            break;
    }
}

static void osapp_amsc_write_ntf_ind(uint16_t handle, uint8_t conidx, uint16_t ntf_ind_cfg)
{
    uint8_t value[2];
    // put value in air format
    co_write16p((&value[0]), ntf_ind_cfg);

    struct gattc_write_cmd * wr_char = AHI_MSG_ALLOC_DYN(GATTC_WRITE_CMD, KE_BUILD_ID(TASK_ID_GATTC, conidx), \
            gattc_write_cmd, 2);

    wr_char->offset = 0;
    wr_char->cursor = 0;
    wr_char->operation = GATTC_WRITE;
    wr_char->handle = handle;
    wr_char->length = 2;
    wr_char->auto_execute = true;
    memcpy(&wr_char->value[0], &value[0], 2);

    osapp_ahi_msg_send(wr_char,sizeof(struct gattc_write_cmd)+2,portMAX_DELAY);
}

static void osapp_amsc_read_truncated_value_enable(uint8_t conidx, uint16_t handle, uint8_t entityID, uint8_t attID)
{
    LOG(LOG_LVL_INFO,"read truncated value enable\n");
    struct gattc_write_cmd * wr_char = AHI_MSG_ALLOC_DYN(GATTC_WRITE_CMD, KE_BUILD_ID(TASK_ID_GATTC, conidx), \
            gattc_write_cmd, 2);

    wr_char->offset = 0;
    wr_char->cursor = 0;
    wr_char->operation = GATTC_WRITE;
    wr_char->handle = handle;
    wr_char->length = 2;
    wr_char->auto_execute = true;
    wr_char->value[0] = entityID;
    wr_char->value[1] = attID;

    osapp_ahi_msg_send(wr_char,sizeof(struct gattc_write_cmd)+2,portMAX_DELAY);
    set_amsc_state_op(AMSC_STATE_BUSY, AMSC_OP_WR_ET_ATT);
}

static void osapp_amsc_read_truncated_value(uint16_t handle)
{
    LOG(LOG_LVL_INFO,"read truncated value, value=%d\n", handle);
    struct gattc_read_cmd *req = AHI_MSG_ALLOC(GATTC_READ_CMD, KE_BUILD_ID(TASK_ID_GATTC, conn_hdl), gattc_read_cmd);
    req->operation                       = GATTC_READ;
    req->nb                             = 1;
    req->req.simple.offset              = 0;
    req->req.simple.length              = 0;
    req->req.simple.handle              = handle;
    osapp_msg_build_send(req, sizeof(struct gattc_read_cmd));
    set_amsc_state_op(AMSC_STATE_BUSY, AMSC_OP_RD_ET_ATT);
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
    cmd->max_mtu = 27;//GAP_MAX_LE_MTU;
    osapp_ahi_msg_send(cmd, sizeof(struct gapm_set_dev_config_cmd),portMAX_DELAY);
}

static void osapp_gapc_conn_confirm(ke_task_id_t dest_id)
{
    struct gapc_connection_cfm *cfm = AHI_MSG_ALLOC(GAPC_CONNECTION_CFM, dest_id, gapc_connection_cfm);
    memset(cfm,0,sizeof(struct gapc_connection_cfm));
    osapp_ahi_msg_send(cfm,sizeof(struct gapc_connection_cfm),portMAX_DELAY);

    osapp_amsc_sec_send_security_req(dest_id);
}

static void osapp_amsc_en_ntf(uint8_t step)
{
    BX_ASSERT(step == AMSC_OP_EN_RMT_CMD_NTF || step == AMSC_OP_EN_ET_UDT_NTF);
    if(AMSC_OP_EN_RMT_CMD_NTF == step)
    {
        osapp_amsc_write_ntf_ind(amsc_content_env.descs[AMSC_DESC_REMOTE_CMD_CFG].desc_hdl, conn_hdl, PRF_CLI_START_NTF);
    }
    else
    {
        osapp_amsc_write_ntf_ind(amsc_content_env.descs[AMSC_DESC_ENTITY_UPDATE_CFG].desc_hdl, conn_hdl, PRF_CLI_START_NTF);
    }
}

static void osapp_asmc_notf_parse(struct gattc_event_ind const * param)
{
    uint16_t length = param->length;
    uint16_t handle = param->handle;
    const uint8_t *value = param->value;

    LOG(LOG_LVL_INFO,"before notf parse, hdl=%d, length=%d\n", handle, length);

    if(handle == amsc_content_env.chars[AMSC_CHAR_REMOTE_CMD].val_hdl)
    {
        LOG(LOG_LVL_INFO,"remote command notification\n");
        for(uint32_t i = 0; i < length; i++)
        {
            LOG(LOG_LVL_INFO,"cmd[%d]%d", i, value[i]);
        }
        uint8_t rmt_cmd = AMS_REMOTE_CMD_ID_NEXT_TRACK;
        if(amsc_content_env.amsc_state != AMSC_STATE_NOTIF_INIT)
        {
            osapp_amsc_write_remote_command(conn_hdl, &rmt_cmd, 1);
        }
    }
    else if(handle == amsc_content_env.chars[AMSC_CHAR_ENTITY_UPDATE].val_hdl)
    {
        LOG(LOG_LVL_INFO,"entity update notification\n");
        struct amsc_entity_notf const *entity_notf = (struct amsc_entity_notf *)value;
        LOG(LOG_LVL_INFO,"entityID=%d, attID=%d, flag=%d\n", entity_notf->entityID, entity_notf->attID, entity_notf->entityUpdateFlags);
        if(entity_notf->entityUpdateFlags & 0x1) // truncated value
        {
            osapp_amsc_read_truncated_value_enable(conn_hdl, amsc_content_env.chars[AMSC_CHAR_ENTITY_ATT].val_hdl,
                                                    entity_notf->entityID, entity_notf->attID);
        }
    }
    else
    {
        BX_ASSERT(0);
    }
}

static int32_t osapp_disconnect(uint8_t conn_id)
{
    struct gapc_disconnect_cmd *cmd = AHI_MSG_ALLOC(GAPC_DISCONNECT_CMD, KE_BUILD_ID(TASK_ID_GAPC, conn_id),gapc_disconnect_cmd);
    cmd->operation = GAPC_DISCONNECT;
    cmd->reason    = CO_ERROR_REMOTE_USER_TERM_CON;
    return osapp_msg_build_send(cmd,sizeof(struct gapc_disconnect_cmd));
}

static void osapp_reset()
{
    struct gapm_reset_cmd *cmd = AHI_MSG_ALLOC(GAPM_RESET_CMD,TASK_ID_GAPM,gapm_reset_cmd);
    cmd->operation = GAPM_RESET;
    osapp_ahi_msg_send(cmd, sizeof(struct gapm_reset_cmd),portMAX_DELAY);
}

static void osapp_gapc_param_update_req_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"param update request\n");
    osapp_gapc_param_update_cfm(src_id);
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
#ifdef ANCS_ENABLE
        osapp_add_ancc_task();
#endif
        osapp_start_advertising();
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

static void osapp_gapm_profile_added_ind_handler(ke_msg_id_t const msgid, struct gapm_profile_added_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"GAPM profile added indication,id:%d,nb:%d,hdl:%d\n",param->prf_task_id,param->prf_task_nb,param->start_hdl);
}

static void osapp_gapc_conn_req_ind_handler(ke_msg_id_t const msgid, struct gapc_connection_req_ind const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    osapp_gapc_conn_confirm(src_id);
    conn_hdl = KE_IDX_GET(src_id);
}

static void osapp_gattc_sdp_svc_ind_handler(ke_msg_id_t const msgid,
                                            struct gattc_sdp_svc_ind const *ind,
                                            ke_task_id_t const dest_id,
                                            ke_task_id_t const src_id)
{
    prf_extract_svc_uuid128_desc_uuid16_info(ind, AMSC_CHAR_MAX, &amsc_char[0], &amsc_content_env.chars[0],
                                                  AMSC_DESC_MAX, &ams_char_desc[0], &amsc_content_env.descs[0]);
    amsc_content_env.svc.shdl = ind->start_hdl;
    amsc_content_env.svc.ehdl = ind->end_hdl;

    osapp_amsc_register_atthdl2gatt(conn_hdl);
}

static void osapp_gattc_cmp_evt_handler(ke_msg_id_t const msgid,struct gattc_cmp_evt const *param,ke_task_id_t const dest_id,ke_task_id_t src_id)
{
    LOG(LOG_LVL_WARN,"gattc cmp operation:0x%x, status=%d\n",param->operation, param->status);
    switch(param->operation)
    {
        case GATTC_NOTIFY:
            if(param->status == ATT_ERR_NO_ERROR)
            {
                LOG(LOG_LVL_INFO,"notification done\n");
            }else
            {
                LOG(LOG_LVL_INFO,"noti status:%d\n",param->status);
            }
            break;
        case GATTC_REGISTER:
            if(param->status == ATT_ERR_NO_ERROR)
            {
                LOG(LOG_LVL_INFO,"register att notif done\n");
                if(amsc_content_env.amsc_state == AMSC_STATE_DISCOVERING)
                {
                    osapp_amsc_en_ntf(AMSC_OP_EN_RMT_CMD_NTF);
                    set_amsc_state_op(AMSC_STATE_NOTIF_INIT, AMSC_OP_EN_RMT_CMD_NTF);
                }
            }
            else
            {
                BX_ASSERT(0);
            }
            break;
        case GATTC_WRITE:
            if(param->status == ATT_ERR_NO_ERROR)
            {
                LOG(LOG_LVL_INFO,"write done, op=%d\n", amsc_content_env.operation);
                switch(amsc_content_env.amsc_state)
                {
                    case AMSC_STATE_NOTIF_INIT:
                        if(amsc_content_env.operation == AMSC_OP_EN_RMT_CMD_NTF)
                        {
                            osapp_amsc_en_ntf(AMSC_OP_EN_ET_UDT_NTF);
                            set_amsc_state_op(AMSC_STATE_NOTIF_INIT, AMSC_OP_EN_ET_UDT_NTF);
                        }
                        else if(amsc_content_env.operation == AMSC_OP_EN_ET_UDT_NTF)
                        {
                            osapp_amsc_enable_all_notification();
                            set_amsc_state_op(AMSC_STATE_NOTIF_INIT, AMSC_OP_WR_ET_UDT);
                        }
                        else if(amsc_content_env.operation == AMSC_OP_WR_ET_UDT)
                        {
                            amsc_next_notf_entity++;
                            osapp_amsc_enable_all_notification();
                        }
                        break;
                    default:
                        if(amsc_content_env.operation == AMSC_OP_WR_ET_ATT) // try to read entity attribute
                        {
                            osapp_amsc_read_truncated_value(amsc_content_env.chars[AMSC_CHAR_ENTITY_ATT].val_hdl);
                        }
                        else if(amsc_content_env.operation == AMSC_OP_WR_RMT_CMD) // write remote command done
                        {
                            LOG(LOG_LVL_INFO,"write remote command done\n");
                        }
                        break;
                }
            }
            else
            {
                BX_ASSERT(0);
            }
            break;
        default:
            LOG(LOG_LVL_INFO,"op:%d,seq:%d,status:%d\n",param->operation,param->seq_num,param->status);
            break;
    }
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
                struct gapc_bond_cfm *cfm = AHI_MSG_ALLOC(GAPC_BOND_CFM, KE_BUILD_ID(TASK_ID_GAPC, conn_hdl), gapc_bond_cfm);
                cfm->request = GAPC_PAIRING_RSP;
                cfm->accept = 0x01;
                cfm->data.pairing_feat.iocap = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
                cfm->data.pairing_feat.oob = GAP_OOB_AUTH_DATA_NOT_PRESENT;
                cfm->data.pairing_feat.auth = GAP_AUTH_REQ_SEC_CON_BOND;
                cfm->data.pairing_feat.key_size = 0x10;
                cfm->data.pairing_feat.ikey_dist = GAP_KDIST_ENCKEY | GAP_KDIST_IDKEY;
                cfm->data.pairing_feat.rkey_dist = GAP_KDIST_ENCKEY | GAP_KDIST_IDKEY;
                cfm->data.pairing_feat.sec_req = GAP_SEC1_NOAUTH_PAIR_ENC; // ??
                osapp_msg_build_send(cfm, sizeof(struct gapc_bond_cfm));
            break;
        }
        case GAPC_LTK_EXCH:
        {
                uint8_t i;
                LOG(LOG_LVL_INFO,"GAPC_LTK_EXCH\n");
                struct gapc_bond_cfm *cfm = AHI_MSG_ALLOC(GAPC_BOND_CFM, KE_BUILD_ID(TASK_ID_GAPC, conn_hdl), gapc_bond_cfm);
                cfm->accept = 0x1;
                cfm->request = GAPC_LTK_EXCH;
                cfm->data.ltk.ediv = (uint16_t)co_rand_word();
                for(i=0;i<KEY_LEN;i++)
                {
                    cfm->data.ltk.ltk.key[i] = (uint8_t)co_rand_word();
                }

                for(i=0;i<RAND_NB_LEN;i++)
                {
                    cfm->data.ltk.randnb.nb[i] = (uint8_t)co_rand_word();
                }

                cfm->data.ltk.key_size = KEY_LEN;

                memcpy(&bond_info.ltk_store,&cfm->data.ltk,sizeof(struct store_gapc_ltk));
                osapp_msg_build_send(cfm, sizeof(struct gapc_bond_cfm));
            break;
        }
        case GAPC_TK_EXCH:
        {
            struct gapc_bond_cfm *cfm = AHI_MSG_ALLOC(GAPC_BOND_CFM, KE_BUILD_ID(TASK_ID_GAPC, conn_hdl), gapc_bond_cfm);
            cfm->request = GAPC_TK_EXCH;
            cfm->accept = 0x1;
            cfm->data.tk.key[0]=BOND_PASSKEY & 0xFF;
            cfm->data.tk.key[1]=(BOND_PASSKEY>>8) & 0xFF;
            cfm->data.tk.key[2]=(BOND_PASSKEY>>16) & 0xFF;
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
            memcpy(&bond_info.csrk,&ind->data.csrk,sizeof(bond_info.csrk));
            break;
        case GAPC_PAIRING_SUCCEED:
            memcpy(&bond_info.auth,&ind->data.auth,sizeof(struct gapc_bond_auth));
//            memcpy(&bond_info.device_addr,&conn_info.device_addr,sizeof(bd_addr_t));
            bond_info.valid = 1;
#ifdef ANCS_ENABLE
            osapp_ancc_enable_req(KE_IDX_GET(src_id));
#endif
            osapp_amsc_svc_disc_start();
            break;
         case  GAPC_PAIRING_FAILED:
            LOG(LOG_LVL_INFO,"Bond Failed, error code = %d\n",ind->data.reason);
//            if(!memcmp(&conn_info.device_addr, &bond_info.device_addr,sizeof(bd_addr_t)))
            {
                bond_info.valid = 0;
                LOG(LOG_LVL_INFO,"Set Bond data invalid\n");
            }
            osapp_disconnect(conn_hdl);
            break;

    }
}

static uint32_t gapc_encrypt_req_ind_handler(ke_msg_id_t const msgid,
                                        struct gapc_encrypt_req_ind const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"encrypt request\n");
    struct gapc_encrypt_cfm *cfm = AHI_MSG_ALLOC(GAPC_ENCRYPT_CFM, src_id, gapc_encrypt_cfm);

    if ((bond_info.valid == true) && (param->ediv == bond_info.ltk_store.ediv) &&
        !memcmp(&param->rand_nb.nb[0], &bond_info.ltk_store.randnb.nb[0], sizeof(bond_info.ltk_store.randnb)))
    {
        LOG(LOG_LVL_INFO,"encrypt found\n");
        cfm->found    = true;
        cfm->key_size = 16;

        memcpy(&cfm->ltk, &bond_info.ltk_store.ltk, sizeof(struct gap_sec_key));
    }

    osapp_msg_build_send(cfm, sizeof(struct gapc_encrypt_cfm));

    osapp_amsc_svc_disc_start(); // reconnect

    return (KE_MSG_CONSUMED);
}

static void gapc_encrypt_ind_handler(ke_msg_id_t const msgid,
                                        struct gapc_encrypt_ind const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"Auth_level_%d\n",param->auth);
}

static void osapp_gattc_event_ind_handler(ke_msg_id_t const msgid, struct gattc_event_ind const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"event ind received, hdl=%d\n", param->handle);
    switch(param->type)
    {
        case GATTC_NOTIFY:
            osapp_asmc_notf_parse(param);
            break;

        default:
            BX_ASSERT(0);
            break;
    }
}

static void gattc_read_ind_handler(ke_msg_id_t const msgid, struct gattc_read_ind const *param,
                                    ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    BX_ASSERT(amsc_content_env.operation == AMSC_OP_RD_ET_ATT); // only after read entity attribute
    LOG(LOG_LVL_INFO,"read ind, hdl=%d, length=%d\n", param->handle, param->length);
}

static void osapp_gapc_get_dev_info_req_ind_handler(ke_msg_id_t const msgid, struct gapc_get_dev_info_req_ind const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gapc_get_dev_info_cfm *cfm;
    switch(param->req)
    {
        case GAPC_DEV_NAME:
            cfm = AHI_MSG_ALLOC_DYN(GAPC_GET_DEV_INFO_CFM,src_id, gapc_get_dev_info_cfm,sizeof(ADV_NAME));
            cfm->req = GAPC_DEV_NAME;
            cfm->info.name.length = sizeof(ADV_NAME);
            memcpy(cfm->info.name.value,ADV_NAME,sizeof(ADV_NAME));
            osapp_ahi_msg_send(cfm, sizeof(struct gapc_get_dev_info_cfm)+sizeof(ADV_NAME),portMAX_DELAY);
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

static void osapp_device_ready_ind_handler(ke_msg_id_t const msgid, void const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    osapp_reset();
}

static void osapp_gapc_cmp_evt_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapc_cmp_evt const *cmp_evt = param;
    LOG(LOG_LVL_INFO,"gapc cmp evt op=%d\n", cmp_evt->operation);
    switch(cmp_evt->operation)
    {
        case GAPC_UPDATE_PARAMS:
            if(cmp_evt->status == GAP_ERR_NO_ERROR)
            {
                LOG(LOG_LVL_INFO,"Connect param update success.\n");
            }
            else
            {
                LOG(LOG_LVL_INFO,"Connect param update error.\n");
            }
            break;
        case GAPC_SECURITY_REQ:
            if(cmp_evt->status == GAP_ERR_NO_ERROR)
            {
                LOG(LOG_LVL_INFO,"GAPC security request completed\n");
            }
            else
            {
                LOG(LOG_LVL_INFO,"GAPC security request error=%d\n", cmp_evt->status);
            }
            break;
    }
}

static void osapp_gattc_mtu_changed_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gattc_mtu_changed_ind *ind = (struct gattc_mtu_changed_ind *)param;
    LOG(LOG_LVL_INFO,"mtu changed= %d,%d\n",ind->mtu, ind->seq_num);
}

static void osapp_default_msg_handler(ke_msg_id_t const msgid, void const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"default handler,msgid: 0x%x, dest_id: 0x%x, src_id: 0x%x\n",msgid,dest_id,src_id);
}

static osapp_msg_handler_table_t const handler_table[]=
{
    [0] =   {KE_MSG_DEFAULT_HANDLER,(osapp_msg_handler_t)osapp_default_msg_handler},
                {GAPM_DEVICE_READY_IND,(osapp_msg_handler_t)osapp_device_ready_ind_handler},
                {GATTC_SDP_SVC_IND,(osapp_msg_handler_t)osapp_gattc_sdp_svc_ind_handler},
                {GAPM_PROFILE_ADDED_IND,(osapp_msg_handler_t)osapp_gapm_profile_added_ind_handler},
                {GAPM_CMP_EVT,(osapp_msg_handler_t)osapp_gapm_cmp_evt_handler},
                {GAPC_PARAM_UPDATE_REQ_IND,(osapp_msg_handler_t)osapp_gapc_param_update_req_handler},
                {GATTC_MTU_CHANGED_IND,(osapp_msg_handler_t)osapp_gattc_mtu_changed_ind_handler},
                {GAPC_CONNECTION_REQ_IND,(osapp_msg_handler_t)osapp_gapc_conn_req_ind_handler},
                {GAPC_DISCONNECT_IND,(osapp_msg_handler_t)osapp_gapc_disconnect_ind_handler},
                {GATTC_CMP_EVT,(osapp_msg_handler_t)osapp_gattc_cmp_evt_handler},
                {GAPC_CMP_EVT,(osapp_msg_handler_t)osapp_gapc_cmp_evt_handler},
                {GATTC_EVENT_IND, (osapp_msg_handler_t)osapp_gattc_event_ind_handler},
                {GATTC_READ_IND, (osapp_msg_handler_t)gattc_read_ind_handler},
                {GAPC_GET_DEV_INFO_REQ_IND,(osapp_msg_handler_t)osapp_gapc_get_dev_info_req_ind_handler},
                {GAPC_BOND_REQ_IND,(osapp_msg_handler_t)osapp_gapc_bond_req_ind_handler},
                {GAPC_BOND_IND,(osapp_msg_handler_t)osapp_gapc_bond_ind_handler},
                {GAPC_ENCRYPT_REQ_IND,(osapp_msg_handler_t)gapc_encrypt_req_ind_handler},
                {GAPC_ENCRYPT_IND,(osapp_msg_handler_t)gapc_encrypt_ind_handler},

                {ANCC_ENABLE_RSP,(osapp_msg_handler_t)osapp_ancc_enable_rsp_handler},
                {ANCC_CMP_EVT,(osapp_msg_handler_t)osapp_ancc_cmp_evt_handler},
                {ANCC_NTF_ATT_IND,(osapp_msg_handler_t)osapp_ancc_ntf_att_ind_handler},
                {ANCC_APP_ATT_IND,(osapp_msg_handler_t)osapp_ancc_app_att_ind_handler},
                {ANCC_NTF_SRC_IND,(osapp_msg_handler_t)osapp_ancc_ntf_src_ind_handler},
};
osapp_msg_handler_info_t const handler_info = ARRAY_INFO(handler_table);


