#ifndef OSAPP_UTILS_H_
#define OSAPP_UTILS_H_
#include <stdint.h>
#include <stdarg.h>
#include "compiler.h"
#include "gattm_task.h"
#include "gattc_task.h"
#include "co_list.h"
#include "ke_msg.h"
/* (ad_type1, ad_data1, ad_data_length1), (ad_type2, ad_data2, ad_data_length2) ... */
uint8_t *adv_data_pack(uint8_t *buf,uint8_t field_nums,...);
#define ADV_DATA_PACK(buf,field_nums,...) (adv_data_pack((buf),(field_nums),__VA_ARGS__) - (buf))

#define TO_2BYTES_ARRAY(u16) {(u16)&0xff,(u16)>>8&0xff}
typedef struct osapp_svc_helper_s
{
    struct co_list_hdr hdr;
    struct gattm_svc_desc const *svc_desc;
    struct gattm_att_desc const *att_desc;
    void (*read)(struct osapp_svc_helper_s const *,ke_task_id_t const,uint16_t);
    void (*write)(struct osapp_svc_helper_s const *,ke_task_id_t const,uint16_t,uint16_t,uint16_t,uint8_t const*);
    uint16_t start_hdl;
    uint8_t att_num;
}osapp_svc_helper_t;

void osapp_add_svc_req_helper(osapp_svc_helper_t *svc_helper,uint8_t num,void (*callback)(uint8_t,osapp_svc_helper_t *));

void osapp_add_svc_rsp_helper(struct gattm_add_svc_rsp const * param);

uint16_t osapp_get_att_handle_helper(osapp_svc_helper_t const *svc_helper,uint8_t att_idx);

osapp_svc_helper_t *osapp_get_svc_helper_ptr(uint16_t handle);

void osapp_read_write_req_ind_helper(void const* param,ke_task_id_t const src_id,bool read);

void osapp_write_req_ind_helper_handler(ke_msg_id_t const msgid, struct gattc_write_req_ind const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id);

void osapp_read_req_ind_helper_handler(ke_msg_id_t const msgid, struct gattc_read_req_ind const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id);

void osapp_add_svc_rsp_helper_handler(ke_msg_id_t const msgid, struct gattm_add_svc_rsp const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id);








#endif
