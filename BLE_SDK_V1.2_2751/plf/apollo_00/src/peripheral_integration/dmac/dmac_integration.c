#include "app_dmac.h"
#include "periph_common.h"
#include "periph_mngt.h"
#include "log.h"
#include "apollo_00.h"
#include "rst_gen.h"
#include "clk_gate.h"
dmac_ch_env_t dmac_ch_env[DMAC_CHANNEL_NUM];
static app_dmac_inst_t *dmac_inst_env;
static void dmac_intr_op(app_dmac_inst_t *inst,intr_operation_t op)
{
    if(op == INTR_ENABLE)
    {
        dmac_inst_env = inst;
        NVIC_EnableIRQ(DMAC_IRQn);
    }else if(op == INTR_DISABLE)
    {
        NVIC_DisableIRQ(DMAC_IRQn);
    }else if(op == INTR_CLR)
    {
        NVIC_ClearPendingIRQ(DMAC_IRQn);
    }else
    {
        BX_ASSERT(0);
    }
}

static void dmac_sw_rst(app_dmac_inst_t *inst)
{
    srst_cpu(DMAC_SRST_CPU);
}

static void dmac_clk_gate(app_dmac_inst_t *inst,clk_gate_operation_t op)
{
    if( op == SET_CLK )
    {
        clk_gate_cpu_g1(CPU_CLKG_SET_DMAC);
    }else
    {
        clk_gate_cpu_g1(CPU_CLKG_CLR_DMAC);
    }
}

static void dmac_sys_stat(app_dmac_inst_t *inst,uint32_t sys_stat)
{
    switch(sys_stat)
    {
    case DMAC_INIT:
        recovery_list_add(&cpu_domain_recovery_mngt, &inst->inst);
    break;
    case DMAC_UNINIT:
        recovery_list_remove(&cpu_domain_recovery_mngt,&inst->inst);
    break;
    case DMAC_TRANSFER_START:
        cpu_domain_stat.dmac = 1;
    break;
    case DMAC_TRANSFER_DONE:
        cpu_domain_stat.dmac = 0;
    break;
    default:
        LOG(LOG_LVL_WARN,"unexpected sys_stat:%d\n",sys_stat);
    break;
    }
}

periph_universal_func_set_t dmac_universal_func =
{
    .intr_op_func = (intr_op_func_t)dmac_intr_op,
    .sw_rst_func = (sw_rst_func_t)dmac_sw_rst,
    .clk_src_cfg_func = NULL,
    .clk_gate_func = (clk_gate_func_t)dmac_clk_gate,
    .pin_cfg_func = NULL,
    .sys_stat_func = (sys_stat_func_t)dmac_sys_stat,
};

void DMAC_IRQHandler()
{
    app_dmac_isr(dmac_inst_env);
}
