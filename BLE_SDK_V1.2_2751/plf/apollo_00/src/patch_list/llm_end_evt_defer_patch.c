#include "bx_config.h"
#include "patch.h"
#include "bx_dbg.h"
#include "co_bt.h"
#include "co_error.h"
#include "co_list.h"
#include "em_buf.h"
#include "co_utils.h"
#include "co_math.h"
#include "co_endian.h"
#include "ke_msg.h"
#include "ke_timer.h"
#include "ke_event.h"
#include "llm.h"
#include "llm_task.h"
#include "llm_util.h"
#include "co_version.h"
#include "llc.h"
#include "llc_task.h"
#include "llc_util.h"
#include "lld.h"
#include "lld_util.h"
#include "lld_pdu.h"
#include "lld_evt.h"
#include "rwip.h"
#include "dbg.h"

void llm_util_chk_tst_mode_patch(void);

void llm_util_chk_tst_mode(void);

void mesh_sched_adv_stop_patch()
{
    ke_state_t current_state = ke_state_get(TASK_LLM);
    if(current_state == LLM_ADVERTISING && (llm_le_env.advertising_params->type == LL_ADV_NONCONN_UNDIR || llm_le_env.advertising_params->type == LL_ADV_CONN_UNDIR))
    {
        ke_state_set(TASK_LLM, LLM_STOPPING);
        lld_evt_delete_elt_push(llm_le_env.elt, true, true);
    }

}

void llm_end_evt_defer_patch()
{
    #if(TX_TEST_PACKET_NUM_PATCH == 1)
    llm_util_chk_tst_mode_patch();
    #else
    llm_util_chk_tst_mode();
    #endif
    #if(MESH_SCHED_PATCH == 1)
    mesh_sched_adv_stop_patch();
    #endif
}

void set_llm_end_evt_defer_patch()
{
    uint32_t code = cal_patch_bl(0x19be6,(uint32_t)llm_end_evt_defer_patch - 1);
    uint32_t *src_low = (uint32_t *)0x19be4;
    uint32_t *src_high = (uint32_t *)0x19be8;
    uint8_t patch_no[2];
    if(patch_alloc(&patch_no[0])==false)
    {
        BX_ASSERT(0);
    }
    patch_entrance_exit_addr(patch_no[0],(uint32_t)src_low,(code&0xffff)<<16|*src_low&0xffff);
    PATCH_ENABLE(patch_no[0]);
    if(patch_alloc(&patch_no[1])==false)
    {
        BX_ASSERT(0);
    }
    patch_entrance_exit_addr(patch_no[1],(uint32_t)src_high,(code&0xffff0000)>>16|*src_high&0xffff0000);
    PATCH_ENABLE(patch_no[1]);
}

