/*
 * timer_integration.c
 *
 *  Created on: 2018Äê6ÔÂ26ÈÕ
 *      Author: jiachuang
 */


#include "periph_common.h"
#include "app_timer.h"
#include "periph_mngt.h"
#include "bx_config.h"
#include "apollo_00.h"
#include "bx_dbg.h"
#include "rst_gen.h"
#include "clk_gate.h"
#include "reg_sysc_per.h"
#include "pshare.h"
#include "log.h"
#include "sysctrl.h"
#include "ll.h"






static app_timer_inst_t *timer_inst_env[2]={0,};

static void timer_intr_op(app_timer_inst_t * inst,intr_operation_t op)
{
    if(op == INTR_ENABLE)
    {
        timer_inst_env[inst->idx] = inst;
        NVIC_EnableIRQ(TIMER_IRQn);
    }else if(op == INTR_DISABLE)
    {
        NVIC_DisableIRQ(TIMER_IRQn);
    }else if(op == INTR_CLR)
    {
        NVIC_ClearPendingIRQ(TIMER_IRQn);
    }else
    {
        BX_ASSERT(0);
    }
}

static void timer_sw_rst(app_timer_inst_t * inst)
{
    srst_cpu(TIMER_SRST_CPU);
}

//parameter is speed mode
static void timer_clk_src_cfg(app_timer_inst_t * inst,uint32_t clk_cfg)
{
    GLOBAL_INT_DISABLE();
    sysc_cpu_clkg0_set(1<<7); // clk_16m_pd_cpu gate clr
    sysc_cpu_clk_div_timer_para_m1_setf(ALL_CHANNEL_TIMER_CLK_DIV);
    sysc_cpu_clkg0_set(1<<6); // clk_16m_pd_cpu gate set
    GLOBAL_INT_RESTORE();
}

static void timer_clk_gate(app_timer_inst_t * inst,clk_gate_operation_t op)
{
    if(op == SET_CLK)
    {
        switch(inst->idx)
        {
            case 0:     sysc_cpu_clkg1_set(1<<12);    break;
            case 1:     sysc_cpu_clkg1_set(1<<14);    break;
        }
    }
    if(op == CLR_CLK)
    {
        switch(inst->idx)
        {
            case 0:     sysc_cpu_clkg1_set(1<<13);    break;
            case 1:     sysc_cpu_clkg1_set(1<<15);    break;
        }
    }
    return;

}

static void timer_pin_cfg(app_timer_inst_t * inst,uint8_t pin_num,uint32_t pin_role)
{
    //NOTING TO DO
    return;
}

static void timer_sys_stat(app_timer_inst_t *inst,uint32_t sys_stat)
{
    uint8_t idx = inst->idx;
    switch(sys_stat)
    {
    case TIMER_INIT:
        recovery_list_add(&cpu_domain_recovery_mngt, &inst->inst);
    break;
    case TIMER_UNINIT:
        recovery_list_remove(&cpu_domain_recovery_mngt,&inst->inst);
    break;
    case TIMER_START:
        if(idx)
        {
            cpu_domain_stat.timer1 = 1;
        }else
        {
            cpu_domain_stat.timer0 = 1;
        }
    break;
    case TIMER_STOP:
        if(idx)
        {
            cpu_domain_stat.timer1 = 0;
        }else
        {
            cpu_domain_stat.timer0 = 0;
        }
    break;
    default:
        LOG(LOG_LVL_WARN,"unexpected sys stat: %d\n",sys_stat);
    break;
    }
}

const periph_universal_func_set_t timer_universal_func = 
{
    .intr_op_func     = (intr_op_func_t    )timer_intr_op,
    .sw_rst_func      = (sw_rst_func_t     )timer_sw_rst,
    .clk_src_cfg_func = (clk_src_cfg_func_t)timer_clk_src_cfg,
    .clk_gate_func    = (clk_gate_func_t   )timer_clk_gate,
    .pin_cfg_func     = (pin_cfg_func_t    )timer_pin_cfg,
    .sys_stat_func    = (sys_stat_func_t   )timer_sys_stat,
};


volatile uint32_t temp_val0,temp_val1;
void TIMER_IRQHandler(void)
{
    reg_timer_t *reg = (reg_timer_t *)REG_TIMER_BASE;
    uint32_t timer_isr_status0 , timer_isr_status1;
    temp_val0 = reg->ch[0].TIMERCURRENTVALUE;
    temp_val1 = reg->ch[1].TIMERCURRENTVALUE;

    timer_isr_status0 = FIELD_RD((&reg->ch[0]), TIMERINTSTATUS, TIMER_TIMERINTERRUPTSTATUSREGISTER);
    timer_isr_status1 = FIELD_RD((&reg->ch[1]), TIMERINTSTATUS, TIMER_TIMERINTERRUPTSTATUSREGISTER);
    if(timer_isr_status0) app_timer_isr(timer_inst_env[0]);
    if(timer_isr_status1) app_timer_isr(timer_inst_env[1]);
}




