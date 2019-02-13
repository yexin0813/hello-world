/**
 ****************************************************************************************
 *
 * @file ble_apollo_00_simple_s.c
 *
 * @brief BLE Peripheral Server.
 *
 * Copyright (C) Apollo 2018-2023
 *
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @addtogroup BLE_APOLLO_00_SIMPLE_SERVICE  BLE Peripheral Simple Server
 * @ingroup BLE_APOLLO_00_SIMPLE
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "ble_peripheral_config.h"

#ifdef BLE_APOLLO_00_SIMPLE_SERVER

#include "ble_apollo_00_simple_s.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * MACROS
 ****************************************************************************************
 */

/*
 * LOCAL FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */
static void ble_bx_simple_handler_init(uint16_t start_hdl);
static void ble_bx_simple_write_callback(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static void ble_bx_simple_read_callback(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static void ble_bx_simple_connect_callback(uint8_t is_connect);
/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
static ble_service_env_t  bx_simple_env={0};

static const struct gattm_svc_desc bx_simple_svc_desc = {
        .start_hdl = 0,
        .task_id = TASK_ID_AHI,
        .perm = PERM(SVC_MI,DISABLE)|PERM(SVC_EKS,DISABLE)|\
            PERM(SVC_AUTH,NO_AUTH)|PERM(SVC_UUID_LEN,UUID_128),PERM_VAL(SVC_SECONDARY,0),
        .nb_att = BX_SIMPLES_ATT_NUM,
        .uuid = BX_SIMPLE_SVC_UUID_128,
};
/// define of all characteristics of this demo, permission and data length can be change here.
static const struct gattm_att_desc bx_simple_svc_att_db[] =
{
    // Characteristic1 Declaration
    [BX_SIMPLES_IDX_CHAR1_CHAR]        =   {
            .uuid = ATT_UUID16_TO_ARRAY(ATT_DECL_CHARACTERISTIC),
            .perm = PERM(RD, ENABLE),
            .max_len =0,
            .ext_perm = PERM(UUID_LEN,UUID_16),
    },
    // Characteristic1 Value
    [BX_SIMPLES_IDX_CHAR1_VAL]         =   {
            .uuid = BX_SIMPLE_CHAR1_UUID_128,
            .perm = PERM(WRITE_REQ,ENABLE)|PERM(WRITE_COMMAND,ENABLE)|PERM(WP,NO_AUTH)|PERM(RD, ENABLE)|PERM(RP,NO_AUTH),//write read
            .max_len = BX_SIMPLE_CHAR1_MAX_LEN,
            .ext_perm = PERM(RI,ENABLE)|PERM(UUID_LEN, UUID_128),// if the char's perm has 'read',this must be set.  uuid 128
    },
    // Characteristic2 Declaration
    [BX_SIMPLES_IDX_CHAR2_CHAR]        =   {
            .uuid = ATT_UUID16_TO_ARRAY(ATT_DECL_CHARACTERISTIC),
            .perm = PERM(RD, ENABLE),
            .max_len =0,
            .ext_perm = PERM(UUID_LEN,UUID_16),
    },
    // Characteristic2 Value
    [BX_SIMPLES_IDX_CHAR2_VAL]         =   {
            .uuid = BX_SIMPLE_CHAR2_UUID_128,
            .perm = PERM(RD, ENABLE)|PERM(RP,NO_AUTH),//read
            .max_len = BX_SIMPLE_CHAR2_MAX_LEN,
            .ext_perm = PERM(RI,ENABLE)|PERM(UUID_LEN, UUID_128),// if the char's perm has 'read',this must be set.  uuid 128
    },
    // Characteristic3 Declaration
    [BX_SIMPLES_IDX_CHAR3_CHAR]        =   {
            .uuid = ATT_UUID16_TO_ARRAY(ATT_DECL_CHARACTERISTIC),
            .perm = PERM(RD, ENABLE),
            .max_len =0,
            .ext_perm = PERM(UUID_LEN,UUID_16),
    },
    // Characteristic3 Value
    [BX_SIMPLES_IDX_CHAR3_VAL]         =   {
            .uuid = BX_SIMPLE_CHAR3_UUID_128,
            .perm = PERM(NTF, ENABLE)|PERM(NP,NO_AUTH),//notify
            .max_len = BX_SIMPLE_CHAR3_MAX_LEN,
            .ext_perm = PERM(UUID_LEN, UUID_128),//uuid 128
    },
    // Client Characteristic Configuration Descriptor
    [BX_SIMPLES_IDX_CHAR3_CFG]      =   {
            .uuid = ATT_UUID16_TO_ARRAY(ATT_DESC_CLIENT_CHAR_CFG),
            .perm = PERM(RD, ENABLE) |PERM(WRITE_REQ, ENABLE),
            .max_len =0,
            .ext_perm = PERM(UUID_LEN,UUID_16),
    },
};

static const gattServiceCBs_t bx_simple_callbacks = {
        .pfnReadAttrCB = ble_bx_simple_read_callback,
        .pfnWriteAttrCB = ble_bx_simple_write_callback,
        .pfnConnectCB = ble_bx_simple_connect_callback,
        .pfnHandlerInitCB = ble_bx_simple_handler_init,
};

//
static uint16_t start_handler = 0xffff;

static uint8_t char1_value[BX_SIMPLE_CHAR1_MAX_LEN] = {0x31,0x32,0x33,0x34,0x35};
static uint8_t char2_value[BX_SIMPLE_CHAR2_MAX_LEN] = {'B','X'};
static uint8_t char3_value[BX_SIMPLE_CHAR3_MAX_LEN] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

static uint16_t char3_notify_cfg = 0x0000;
/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief add service and characteristics in service_table
 *
 * @param[in] cb  gatt structure for service callback functions
 *
 * @return message send to ble_task success or not. 0 for failed and 1 for success.
 *
 ****************************************************************************************
 */
int32_t ble_bx_simple_add_svc(gattServiceCBs_t *cb)
{
    struct gattm_add_svc_req *req = AHI_MSG_ALLOC_DYN(GATTM_ADD_SVC_REQ,TASK_ID_GATTM,\
        gattm_add_svc_req,sizeof(bx_simple_svc_att_db));
    struct gattm_svc_desc *svc = &req->svc_desc;
    memcpy(svc,&bx_simple_svc_desc,sizeof(bx_simple_svc_desc));
    memcpy(svc->atts,bx_simple_svc_att_db,sizeof(bx_simple_svc_att_db));

    memcpy(cb,&bx_simple_callbacks,sizeof(gattServiceCBs_t));

    LOG(LOG_LVL_INFO," svc ble_bx_simple_add_svc \n");
    return osapp_msg_build_send(req, sizeof(struct gattm_svc_desc)+sizeof(bx_simple_svc_att_db));
}

/**
 ****************************************************************************************
 * @brief init set service and characteristics handler in service_table
 *
 * @param[in] start_hdl Service Start Handle
 ****************************************************************************************
 */
static void ble_bx_simple_handler_init(uint16_t start_hdl)
{
    LOG(LOG_LVL_INFO,"ble_bx_simple_handler_init\n");
    start_handler = start_hdl;
}
/**
 ****************************************************************************************
 * @brief conect / disconnect call back. Do different things according to the handle.
 *
 * @param[in] is_connect  0 is disconnect. 1 is connect.
 *
 ****************************************************************************************
 */
static void ble_bx_simple_connect_callback(uint8_t is_connect)
{
   LOG(LOG_LVL_INFO,"ble_bx_simple_connect_callback\n");

   LOG(LOG_LVL_INFO,"is_connect 0x%X \n",is_connect);

   bx_simple_env.is_connect = is_connect;
}

/**
 ****************************************************************************************
 * @brief master write to the device. Do different things according to the handle.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAPM).
 * @param[in] src_id    ID of the sending task instance.
 *
 ****************************************************************************************
 */
static void ble_bx_simple_write_callback(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"src_id 0x%X \n",src_id);

    uint16_t handle_idx = start_handler+1;
    struct gattc_write_req_ind *ind = (struct gattc_write_req_ind *)param;

    if((ind->handle >= start_handler) && (ind->handle <= start_handler+BX_SIMPLES_ATT_NUM ))
    {
        struct gattc_write_cfm *cfm = AHI_MSG_ALLOC(GATTC_WRITE_CFM, TASK_ID_GATTC, gattc_write_cfm);

        LOG(LOG_LVL_INFO,"ble_bx_simple_write_callback \n");
        LOG(LOG_LVL_INFO,"gattc write req_ %d \n",ind->handle);
        LOG(LOG_LVL_INFO,"handle_idx %d \n",handle_idx);
        LOG(LOG_LVL_INFO,"ind->length %d \n",ind->length);

        LOG(LOG_LVL_INFO,"ind->value :");
        for(uint8_t i=0;i<ind->length;i++)
            LOG(LOG_LVL_INFO," %X :",ind->value[i]);
        LOG(LOG_LVL_INFO,"\n");

        cfm->handle = ind->handle;
        cfm->status = ATT_ERR_NO_ERROR;

        switch((ind->handle - handle_idx))
        {
        case BX_SIMPLES_IDX_CHAR1_VAL :
            if(ind->length <= BX_SIMPLE_CHAR1_MAX_LEN)
            {
                if(ind->offset ==0)
                {
                    memcpy(char1_value,ind->value,ind->length);
                    memcpy(char3_value,ind->value,ind->length);
                    ble_bx_simple_char3_send_notification(char3_value,BX_SIMPLE_CHAR3_MAX_LEN);
                }
            }
            break;
        case BX_SIMPLES_IDX_CHAR2_VAL :
            cfm->status = ATT_ERR_WRITE_NOT_PERMITTED;
            break;
        case BX_SIMPLES_IDX_CHAR3_VAL :
            cfm->status = ATT_ERR_WRITE_NOT_PERMITTED;
            break;
        case BX_SIMPLES_IDX_CHAR3_CFG :
            if(ind->length == 2)
            {
                if(ind->offset ==0)
                {
                    memcpy((uint8_t *)&char3_notify_cfg,ind->value,ind->length);
                    LOG(LOG_LVL_INFO,"char3_notify_cfg %X\n",char3_notify_cfg);
                }
            }
            break;
        default:
            cfm->status = ATT_ERR_INVALID_HANDLE;
            break;
        }

        LOG(LOG_LVL_INFO,"gattc write cfm\n");
        osapp_msg_build_send(cfm,sizeof(struct gattc_write_cfm));
    }
}
/**
 ****************************************************************************************
 * @brief master read from the device. Do different things according to the handle.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAPM).
 * @param[in] src_id    ID of the sending task instance.
 *
 ****************************************************************************************
 */
static void ble_bx_simple_read_callback(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"src_id 0x%X \n",src_id);
    uint16_t handle_idx = start_handler+1;

    struct gattc_read_req_ind *ind = (struct gattc_read_req_ind *)param;

    if((ind->handle >= start_handler) && (ind->handle <= start_handler+BX_SIMPLES_ATT_NUM ))
    {
        LOG(LOG_LVL_INFO,"ble_bx_simple_read_callback \n");
        LOG(LOG_LVL_INFO,"gattc read req_ %d \n",ind->handle);
        LOG(LOG_LVL_INFO,"handle_idx  %d \n",handle_idx);


        switch((ind->handle - handle_idx))
        {
        case BX_SIMPLES_IDX_CHAR1_VAL :
            {
                struct gattc_read_cfm *cfm = AHI_MSG_ALLOC_DYN(GATTC_READ_CFM,src_id, gattc_read_cfm,BX_SIMPLE_CHAR1_MAX_LEN);

                cfm->handle = ind->handle;
                cfm->status = ATT_ERR_NO_ERROR;
                cfm->length = BX_SIMPLE_CHAR1_MAX_LEN;
                memcpy(cfm->value,char1_value,cfm->length);

                osapp_msg_build_send(cfm,(sizeof(struct gattc_read_cfm)+BX_SIMPLE_CHAR1_MAX_LEN));
            }
            break;
        case BX_SIMPLES_IDX_CHAR2_VAL :
            {
                struct gattc_read_cfm *cfm = AHI_MSG_ALLOC_DYN(GATTC_READ_CFM,src_id, gattc_read_cfm,BX_SIMPLE_CHAR2_MAX_LEN);

                cfm->handle = ind->handle;
                cfm->status = ATT_ERR_NO_ERROR;
                cfm->length = BX_SIMPLE_CHAR2_MAX_LEN;
                memcpy(cfm->value,char2_value,cfm->length);

                osapp_msg_build_send(cfm,(sizeof(struct gattc_read_cfm)+BX_SIMPLE_CHAR2_MAX_LEN));
            }
            break;
        case BX_SIMPLES_IDX_CHAR3_CFG :
            {
                struct gattc_read_cfm *cfm = AHI_MSG_ALLOC(GATTC_READ_CFM,src_id, gattc_read_cfm);

                cfm->length = 0;
                cfm->handle = ind->handle;
                cfm->status = ATT_ERR_NO_ERROR;

                osapp_msg_build_send(cfm,sizeof(struct gattc_read_cfm));
            }
            break;
        case BX_SIMPLES_IDX_CHAR3_VAL :
            {
                struct gattc_read_cfm *cfm = AHI_MSG_ALLOC(GATTC_READ_CFM,src_id, gattc_read_cfm);

                cfm->handle = ind->handle;
                cfm->length = 0;
                cfm->status = ATT_ERR_READ_NOT_PERMITTED;

                osapp_msg_build_send(cfm,sizeof(struct gattc_read_cfm));
            }
            break;
        default:
            {
                struct gattc_read_cfm *cfm = AHI_MSG_ALLOC(GATTC_READ_CFM,src_id, gattc_read_cfm);

                cfm->handle = ind->handle;
                cfm->length = 0;
                cfm->status = ATT_ERR_INVALID_HANDLE;

                osapp_msg_build_send(cfm,sizeof(struct gattc_read_cfm));
            }
            break;
        }
    }
}
/**
 ****************************************************************************************
 * @brief char3 notification data to air.
 *
 * @param[in] data      Pointer to the notification data.
 * @param[in] length    the length of notification data.
 *
 ****************************************************************************************
 */
void ble_bx_simple_char3_send_notification(uint8_t const *data,uint8_t length)
{
    static uint16_t notify_seq_num = 0;
    struct gattc_send_evt_cmd *cmd= AHI_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,TASK_ID_GATTC, gattc_send_evt_cmd, length);

    if(bx_simple_env.is_connect == IS_CONNECTED && char3_notify_cfg)
    {
        LOG(LOG_LVL_INFO,"ble_bx_simple_char3_send_notification\n");

        cmd->operation = GATTC_NOTIFY;
        cmd->seq_num = notify_seq_num++;
        cmd->handle = start_handler + BX_SIMPLES_IDX_CHAR3_CFG;
        cmd->length = length;
        memcpy(cmd->value,data,length);

        osapp_ahi_msg_send(cmd, (sizeof(struct gattc_send_evt_cmd) + length), portMAX_DELAY);
    }
}
/*
 * MESSAGES HANDLERS DEFINITIONS
 ****************************************************************************************
 */

/// @} BLE_APOLLO_00_SIMPLE_SERVICE

#endif/* BLE_APOLLO_00_SIMPLE_SERVER */
