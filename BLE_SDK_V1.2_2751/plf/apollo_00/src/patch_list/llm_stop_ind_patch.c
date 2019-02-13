#include "bx_config.h"
#include "patch.h"
#include "bx_dbg.h"


#define LLD_STOP_IND_PATCH_ADDR 0x18ba4

void mesh_sched_stop_ind_send(uint16_t opcode);

#if (MESH_SCHED_PATCH == 1)
void set_llm_lld_stop_ind_patch()
{
    uint32_t code = cal_patch_bl(LLD_STOP_IND_PATCH_ADDR,(uint32_t)mesh_sched_stop_ind_send - 1);    
    uint8_t patch_no;
    if(patch_alloc(&patch_no)==false)
    {
        BX_ASSERT(0);
    }
    patch_entrance_exit_addr(patch_no,LLD_STOP_IND_PATCH_ADDR,code);
    PATCH_ENABLE(patch_no);    
    
}
#endif
