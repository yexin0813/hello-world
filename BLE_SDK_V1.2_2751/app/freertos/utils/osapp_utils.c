#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include "osapp_utils.h"
#include "co_utils.h"
#include "gattm_task.h"
#include "osapp_task.h"
static struct
{
    struct co_list svc_helper_list;
    osapp_svc_helper_t *current;
    void (*add_svc_cb)(uint8_t,osapp_svc_helper_t *);
}svc_helper_env;

uint8_t * adv_data_pack(uint8_t *buf,uint8_t field_nums,...)
{
    va_list list;
    uint8_t i;
    va_start(list,field_nums);
    for(i=0;i<field_nums;++i)
    {
        uint8_t ad_type = va_arg(list,uint32_t);
        uint8_t *ad_data = va_arg(list,uint8_t *);
        uint8_t ad_data_length = va_arg(list,uint32_t);
        buf[0] = ad_data_length + 1;
        buf[1] = ad_type;
        memcpy(&buf[2],ad_data,ad_data_length);
        buf += buf[0] + 1;
    }
    va_end(list);
    return buf;
}

static void add_svc_req()
{
    osapp_svc_helper_t *helper = svc_helper_env.current;
    struct gattm_add_svc_req *req = AHI_MSG_ALLOC_DYN(GATTM_ADD_SVC_REQ,TASK_ID_GATTM,gattm_add_svc_req,sizeof(struct gattm_att_desc)*helper->att_num);
    struct gattm_svc_desc *svc = &req->svc_desc;
    memcpy(svc,helper->svc_desc,sizeof(struct gattm_svc_desc));
    memcpy(svc->atts,helper->att_desc,sizeof(struct gattm_att_desc)*helper->att_num);
    osapp_ahi_msg_send(req, sizeof(struct gattm_svc_desc)+sizeof(struct gattm_att_desc)*helper->att_num,portMAX_DELAY);
}

void osapp_add_svc_req_helper(osapp_svc_helper_t *svc_helper,uint8_t num,void (*callback)(uint8_t,osapp_svc_helper_t *))
{
    co_list_init(&svc_helper_env.svc_helper_list);
    svc_helper_env.add_svc_cb = callback;
    uint8_t i;
    for(i=0;i<num;++i)
    {
        co_list_push_back(&svc_helper_env.svc_helper_list,&svc_helper[i].hdr);
    }
    struct co_list_hdr *hdr = co_list_pick(&svc_helper_env.svc_helper_list);
    svc_helper_env.current = CONTAINER_OF(hdr, osapp_svc_helper_t, hdr);
    add_svc_req();
}

void osapp_add_svc_rsp_helper(struct gattm_add_svc_rsp const * param)
{
    if(param->status == ATT_ERR_NO_ERROR)
    {
        svc_helper_env.current->start_hdl = param->start_hdl;
    }else
    {
        svc_helper_env.add_svc_cb(param->status,svc_helper_env.current);
        return;
    }
    struct co_list_hdr *hdr = co_list_next(&svc_helper_env.current->hdr);
    if(hdr)
    {
        svc_helper_env.current = CONTAINER_OF(hdr, osapp_svc_helper_t, hdr);
        add_svc_req();
    }else
    {
        svc_helper_env.add_svc_cb(param->status,svc_helper_env.current);
    }
}

uint16_t osapp_get_att_handle_helper(osapp_svc_helper_t const*svc_helper,uint8_t att_idx)
{
    return svc_helper->start_hdl + att_idx + 1;
}

static bool handle_in_svc(osapp_svc_helper_t *helper,uint16_t handle)
{
    return handle > helper->start_hdl && handle <= helper->start_hdl + helper->att_num;
}

osapp_svc_helper_t *osapp_get_svc_helper_ptr(uint16_t handle)
{
    if(handle_in_svc(svc_helper_env.current, handle))
    {
        return svc_helper_env.current;
    }
    struct co_list_hdr *hdr = co_list_pick(&svc_helper_env.svc_helper_list);
    while(hdr)
    {
        osapp_svc_helper_t *helper = CONTAINER_OF(hdr, osapp_svc_helper_t, hdr);
        if(handle_in_svc(helper,handle))
        {
            svc_helper_env.current = helper;
            return helper;
        }
        hdr = co_list_next(hdr);
    }
    return NULL;
}

void osapp_read_write_req_ind_helper(void const* param,ke_task_id_t const src_id,bool read)
{
    uint16_t handle = *(uint16_t *)param;
    osapp_svc_helper_t *helper = osapp_get_svc_helper_ptr(handle);
    if(helper)
    {
        uint16_t att_idx = handle-helper->start_hdl -1 ;
        if(read)
        {
            if(helper->read)
            {
                helper->read(helper,src_id,att_idx);
            }
        }else
        {
            struct gattc_write_req_ind const* wr_param = param;
            if(helper->write)
            {
                helper->write(helper,src_id,att_idx,wr_param->offset,wr_param->length,wr_param->value);
            }
        }
    }
}

void osapp_write_req_ind_helper_handler(ke_msg_id_t const msgid, struct gattc_write_req_ind const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    osapp_read_write_req_ind_helper(param,src_id,false);
}

void osapp_read_req_ind_helper_handler(ke_msg_id_t const msgid, struct gattc_read_req_ind const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    osapp_read_write_req_ind_helper(param,src_id,true);
}

void osapp_add_svc_rsp_helper_handler(ke_msg_id_t const msgid, struct gattm_add_svc_rsp const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    osapp_add_svc_rsp_helper(param);
}

