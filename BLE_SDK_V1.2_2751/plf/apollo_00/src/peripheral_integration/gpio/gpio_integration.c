/*
 * gpio_integration.c
 *
 *  Created on: 2018-7-13
 *      Author: mingzhou
 */
#include "gpio_integration.h"
#include "periph_common.h"
#include "app_gpio.h"
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

static app_gpio_inst_t *gpio_inst;
static void gpio_intr_op(app_gpio_inst_t *inst, intr_operation_t op)
{
    if(op == INTR_ENABLE)
    {
        gpio_inst = inst;
        NVIC_EnableIRQ(GPIO_IRQn);
    }else if(op == INTR_DISABLE)
    {
        NVIC_DisableIRQ(GPIO_IRQn);
    }else if(op == INTR_CLR)
    {
        NVIC_ClearPendingIRQ(GPIO_IRQn);
    }else
    {
        BX_ASSERT(0);
    }
}

static void gpio_sw_rst(app_gpio_inst_t *inst)
{
    srst_per(GPIO_SRST_PER);
}

static void gpio_clk_gate(app_gpio_inst_t*inst, clk_gate_operation_t op)
{
    #if (defined(CFG_ON_CHIP)&&(CFG_ON_CHIP==1))
    if(op == SET_CLK )
    {
        clk_gate_per_g1(PER_CLKG_SET_GPIO);
    }else if(op == CLR_CLK)
    {
        clk_gate_per_g1(PER_CLKG_CLR_GPIO);
    }
    #endif
}

static void gpio_sys_stat(app_gpio_inst_t *inst, uint32_t sys_stat)
{
    switch(sys_stat)
    {
    case GPIO_INIT:
        recovery_list_add(&periph_domain_recovery_mngt, &inst->inst);
        break;
    case GPIO_UNINIT:
        recovery_list_remove(&periph_domain_recovery_mngt, &inst->inst);
        break;
    default:
        LOG(LOG_LVL_WARN,"unexpected sys stat: %d\n",sys_stat);
        break;
    }
}

void GPIO_IRQHandler(void)
{
    app_gpio_isr(gpio_inst);
}

const periph_universal_func_set_t gpio_universal_func = {
    .intr_op_func = (intr_op_func_t)gpio_intr_op,
    .sw_rst_func = (sw_rst_func_t)gpio_sw_rst,
    .clk_src_cfg_func = (clk_src_cfg_func_t)NULL,
    .clk_gate_func = (clk_gate_func_t)gpio_clk_gate,
    .pin_cfg_func = (pin_cfg_func_t)NULL,
    .sys_stat_func = (sys_stat_func_t)gpio_sys_stat,
};
