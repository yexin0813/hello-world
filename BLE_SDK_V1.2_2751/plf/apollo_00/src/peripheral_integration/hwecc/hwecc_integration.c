#include "app_hwecc.h"
#include "bx_ring_queue.h"
#include "log.h"
#include "apollo_00.h"
#include "rst_gen.h"
#include "clk_gate.h"
#include "periph_recovery.h"
#include "periph_mngt.h"
#define ECC_QUEUE_SIZE 3

DEF_RING_QUEUE(ecc_q,ECC_QUEUE_SIZE,ecc_queue_t);

static app_hwecc_inst_t *hwecc_inst_env;

static void hwecc_intr_op(app_hwecc_inst_t *inst,intr_operation_t op)
{
    if(op == INTR_ENABLE)
    {
        hwecc_inst_env = inst;
        NVIC_EnableIRQ(ECC_IRQn);
    }else if(op == INTR_DISABLE)
    {
        NVIC_DisableIRQ(ECC_IRQn);        
    }else if(op == INTR_CLR)
    {
        NVIC_ClearPendingIRQ(ECC_IRQn);        
    }else
    {
        BX_ASSERT(0);
    }
}

static void hwecc_sw_rst(app_hwecc_inst_t *inst)
{
    srst_cpu(ECC_SRST_CPU);    
}

static void hwecc_clk_gate(app_hwecc_inst_t *inst,clk_gate_operation_t op)
{
    if( op == SET_CLK )
    {
        clk_gate_cpu_g1(CPU_CLKG_SET_ECC);
    }else
    {
        clk_gate_cpu_g1(CPU_CLKG_CLR_ECC);
    }    
}

static void hwecc_sys_stat(app_hwecc_inst_t *inst,uint32_t sys_stat)
{
    switch(sys_stat)
    {
    case HWECC_INIT:
        recovery_list_add(&cpu_domain_recovery_mngt, &inst->inst);
    break;
    case HWECC_UNINIT:
        recovery_list_remove(&cpu_domain_recovery_mngt,&inst->inst);
    break;
    case HWECC_START:
        cpu_domain_stat.hwecc = 1;
    break;
    case HWECC_DONE:
        cpu_domain_stat.hwecc = 0;
    break;
    default:
        LOG(LOG_LVL_WARN,"unexpected sys_stat:%d\n",sys_stat);
    break;
    }
}

periph_universal_func_set_t hwecc_universal_func =
{
    .intr_op_func = (intr_op_func_t)hwecc_intr_op,
    .sw_rst_func = (sw_rst_func_t)hwecc_sw_rst,
    .clk_src_cfg_func = NULL,
    .clk_gate_func = (clk_gate_func_t)hwecc_clk_gate,
    .pin_cfg_func = NULL,
    .sys_stat_func = (sys_stat_func_t)hwecc_sys_stat,
};

void ECC_IRQHandler(void)
{
    app_hwecc_isr(hwecc_inst_env);
}
