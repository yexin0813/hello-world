/*
 * app_pwm.c
 *
 *  Created on: 2018Äê6ÔÂ26ÈÕ
 *      Author: jiachuang
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "reg_pwm.h"
#include "field_manipulate.h"
#include "app_pwm.h"
#include "bx_dbg.h"
#include "ll.h"
#include "rwip.h"
#include "log.h"
#include "co_utils.h"
#include "app_dmac_wrapper.h"
#include "string.h"
#include "plf.h"
#include "rf_reg_typedef.h"
#include "reg_sysc_cpu.h"

extern periph_universal_func_set_t pwm_universal_func;






void app_pwm_init(periph_inst_handle_t hdl)
{
    app_pwm_inst_t *inst = CONTAINER_OF(hdl, app_pwm_inst_t, inst);
    reg_pwm_t *reg = inst->reg;

    //pwm_universal_func.sw_rst_func(inst);
    pwm_universal_func.pin_cfg_func(inst,inst->param.pin_num,PWM_ENABLE);
    pwm_universal_func.intr_op_func(inst,INTR_CLR);
    pwm_universal_func.clk_src_cfg_func(inst,ALL_CHANNEL_PWM_CLK_DIV);
    pwm_universal_func.clk_gate_func(inst,SET_CLK);

    //set parameter
    FIELD_WR((&reg->ch[inst->idx]), PWM_SETTING, PWM_PWM_HIGH, inst->param.high_time);
    FIELD_WR((&reg->ch[inst->idx]), PWM_SETTING, PWM_PWM_LOW,  inst->param.low_time );
    
    //STAT
    pwm_universal_func.sys_stat_func(inst,PWM_INIT);
    pwm_universal_func.clk_gate_func(inst,CLR_CLK);
}

void app_pwm_uninit(periph_inst_handle_t hdl)
{
    app_pwm_inst_t *inst = CONTAINER_OF(hdl, app_pwm_inst_t, inst);
    

    pwm_universal_func.clk_gate_func(inst,CLR_CLK);
    pwm_universal_func.intr_op_func(inst,INTR_DISABLE);
    pwm_universal_func.pin_cfg_func(inst,inst->param.pin_num,PWM_DISABLE);
    //STAT
    pwm_universal_func.sys_stat_func(inst,PWM_UNINIT);
}



void app_pwm_start(periph_inst_handle_t hdl)
{
    app_pwm_inst_t *inst = CONTAINER_OF(hdl, app_pwm_inst_t, inst);
    reg_pwm_t *reg = inst->reg;
    //clk,pin,enable
    pwm_universal_func.pin_cfg_func(inst,inst->param.pin_num,PWM_ENABLE);
    pwm_universal_func.clk_gate_func(inst,SET_CLK);
    //set parameter
    FIELD_WR((&reg->ch[inst->idx]), PWM_SETTING, PWM_PWM_HIGH, inst->param.high_time);
    FIELD_WR((&reg->ch[inst->idx]), PWM_SETTING, PWM_PWM_LOW,  inst->param.low_time );
    FIELD_WR((&reg->ch[inst->idx]), PWM_EN, PWM_PWM_EN_R, 1);
     //STAT
    pwm_universal_func.sys_stat_func(inst,PWM_OUTPUT_START);   
}


void app_pwm_stop(periph_inst_handle_t hdl)
{
    app_pwm_inst_t *inst = CONTAINER_OF(hdl, app_pwm_inst_t, inst);
    reg_pwm_t *reg = inst->reg;
    //clk,pin,enable
    pwm_universal_func.pin_cfg_func(inst,inst->param.pin_num,PWM_DISABLE);
    pwm_universal_func.clk_gate_func(inst,CLR_CLK);
    FIELD_WR((&reg->ch[inst->idx]), PWM_EN, PWM_PWM_EN_R, 0);
     //STAT
    pwm_universal_func.sys_stat_func(inst,PWM_OUTPUT_STOP);   
}




