/*
 * app_timer.c
 *
 *  Created on: 2018Äê6ÔÂ26ÈÕ
 *      Author: jiachuang
 */



#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "reg_timer.h"
#include "field_manipulate.h"
#include "app_timer.h"
#include "bx_dbg.h"
#include "ll.h"
#include "rwip.h"
#include "log.h"
#include "co_utils.h"
#include "string.h"
#include "plf.h"
#include "rf_reg_typedef.h"
#include "reg_sysc_cpu.h"

extern periph_universal_func_set_t timer_universal_func;



void app_timer_isr(app_timer_inst_t *inst)
{
    reg_timer_t *reg = inst->reg;
//
//    uint32_t timer_isr_status;
//    timer_isr_status = FIELD_RD((&reg->ch[inst->idx]), TIMERINTSTATUS, TIMER_TIMERINTERRUPTSTATUSREGISTER);
//    //isr comes
//    if(timer_isr_status)
//    {
        //read EOI to clear interrupt
        FIELD_RD((&reg->ch[inst->idx]), TIMEREOI, TIMER_TIMERENDOFINTERRUPTREGISTER);
        inst->param.callback(inst->param.callback_param);    
//    }
}



void app_timer_init(periph_inst_handle_t hdl)
{
    app_timer_inst_t *inst = CONTAINER_OF(hdl, app_timer_inst_t, inst);
    reg_timer_t *reg = inst->reg;

    //CANNOT SOFT RESET
    timer_universal_func.pin_cfg_func(inst,0,0);
    timer_universal_func.intr_op_func(inst,INTR_CLR);
    timer_universal_func.intr_op_func(inst,INTR_ENABLE);
    timer_universal_func.clk_src_cfg_func(inst,ALL_CHANNEL_TIMER_CLK_DIV);
    timer_universal_func.clk_gate_func(inst,SET_CLK);

    //set parameter
    FIELD_WR((&reg->ch[inst->idx]), TIMERLOADCOUNT , TIMER_TIMERLOADCOUNTREGISTER, inst->param.auto_reload_value);
    FIELD_WR((&reg->ch[inst->idx]), TIMERCONTROLREG, TIMER_TIMERMODE             , 1);
    FIELD_WR((&reg->ch[inst->idx]), TIMERCONTROLREG, TIMER_TIMERINTERRUPTMASK    , 0);
    
    //STAT
    timer_universal_func.sys_stat_func(inst,TIMER_INIT);
    //CANNOT CLEAR CLOCK GATE
}

void app_timer_uninit(periph_inst_handle_t hdl)
{
    app_timer_inst_t *inst = CONTAINER_OF(hdl, app_timer_inst_t, inst);

    timer_universal_func.clk_gate_func(inst,CLR_CLK);
    timer_universal_func.intr_op_func(inst,INTR_DISABLE);
    //STAT
    timer_universal_func.sys_stat_func(inst,TIMER_UNINIT);
}



void app_timer_start(periph_inst_handle_t hdl)
{
    app_timer_inst_t *inst = CONTAINER_OF(hdl, app_timer_inst_t, inst);
    reg_timer_t *reg = inst->reg;

    FIELD_WR((&reg->ch[inst->idx]), TIMERCONTROLREG, TIMER_TIMERENABLE    , 1);
    //STAT
    timer_universal_func.sys_stat_func(inst,TIMER_START);   
}


void app_timer_stop(periph_inst_handle_t hdl)
{
    app_timer_inst_t *inst = CONTAINER_OF(hdl, app_timer_inst_t, inst);
    reg_timer_t *reg = inst->reg;

    FIELD_WR((&reg->ch[inst->idx]), TIMERCONTROLREG, TIMER_TIMERENABLE    , 0);
    //STAT
    timer_universal_func.sys_stat_func(inst,TIMER_STOP);   
}

uint32_t app_timer_getValue(periph_inst_handle_t hdl)
{
    app_timer_inst_t *inst = CONTAINER_OF(hdl, app_timer_inst_t, inst);
    reg_timer_t *reg = inst->reg;

    return FIELD_RD((&reg->ch[inst->idx]), TIMERCURRENTVALUE, TIMER_TIMERCURRENTVALUEREGISTER);
}



