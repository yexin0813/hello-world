/*
 * pwm_integration.c
 *
 *  Created on: 2018Äê6ÔÂ26ÈÕ
 *      Author: jiachuang
 */



#include "periph_common.h"
#include "app_pwm.h"
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


static void pwm_intr_op(app_pwm_inst_t * inst,intr_operation_t op)
{
    //NOTHING TO DO
    return;
}
static void pwm_sw_rst(app_pwm_inst_t * inst)
{
    srst_per(PWM_SRST_PER);
    return;
}
static void pwm_clk_src_cfg(app_pwm_inst_t * inst,uint32_t clk_cfg)
{
    GLOBAL_INT_DISABLE();
    sysc_per_clkg1_set(1<<1);
    sysc_per_clk_div_pwm_para_m1_setf(clk_cfg);
    sysc_per_clkg1_set(1<<0);
    GLOBAL_INT_RESTORE();
    return;
}
static void pwm_clk_gate(app_pwm_inst_t * inst,clk_gate_operation_t op)
{
    if(op == SET_CLK)
    {
        switch(inst->idx)
        {
            case 0:     sysc_per_clkg1_set(1<<2);    break;
            case 1:     sysc_per_clkg1_set(1<<4);    break;
            case 2:     sysc_per_clkg1_set(1<<6);    break;
            case 3:     sysc_per_clkg1_set(1<<8);    break;
            case 4:     sysc_per_clkg1_set(1<<10);    break;
        }
    }
    if(op == CLR_CLK)
    {
        switch(inst->idx)
        {
            case 0:     sysc_per_clkg1_set(1<<3);    break;
            case 1:     sysc_per_clkg1_set(1<<5);    break;
            case 2:     sysc_per_clkg1_set(1<<7);    break;
            case 3:     sysc_per_clkg1_set(1<<9);    break;
            case 4:     sysc_per_clkg1_set(1<<11);   break;
        }
    }
    return;
}
static void pwm_pin_cfg(app_pwm_inst_t * inst,uint8_t pin_num,uint32_t pin_role)
{
    sysctrl_io_config(pin_num, GPIO_OUTPUT);
    if((pin_num < 2) || (pin_num > 23))
    {
        return ;
    }
    else
    {
        if(pin_role == PWM_ENABLE)
        {
            pshare_funcio_set(pin_num - 2,IO_PWM_0 + inst->idx,ENABLE);
        }
        else
        {
            pshare_funcio_set(pin_num - 2,IO_PWM_0 + inst->idx,DISABLE);
        }
    }
    return;
}

static void pwm_sys_stat(app_pwm_inst_t *inst,uint32_t sys_stat)
{
    switch(sys_stat)
    {
    case PWM_INIT:
        recovery_list_add(&periph_domain_recovery_mngt, &inst->inst);
        break;
    case PWM_UNINIT:
        recovery_list_remove(&periph_domain_recovery_mngt,&inst->inst);
        break;
    case PWM_OUTPUT_START:
        periph_domain_stat.pwm = 1;
        break;
    case PWM_OUTPUT_STOP:
        periph_domain_stat.pwm = 0;
        break;
    default:
        LOG(LOG_LVL_WARN,"unexpected sys stat: %d\n",sys_stat);
        break;
    }
}

const periph_universal_func_set_t pwm_universal_func =
{
    .intr_op_func     = (intr_op_func_t    )pwm_intr_op,
    .sw_rst_func      = (sw_rst_func_t     )pwm_sw_rst,
    .clk_src_cfg_func = (clk_src_cfg_func_t)pwm_clk_src_cfg,
    .clk_gate_func    = (clk_gate_func_t   )pwm_clk_gate,
    .pin_cfg_func     = (pin_cfg_func_t    )pwm_pin_cfg,
    .sys_stat_func    = (sys_stat_func_t   )pwm_sys_stat,
};

