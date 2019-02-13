#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "app_gpio.h"
#include "co_utils.h"
#include "field_manipulate.h"
extern periph_universal_func_set_t gpio_universal_func;

static void gpio_int_cfg(app_gpio_inst_t *inst,app_gpio_int_cfg_t *int_cfg)
{
    REG_BIT_WR(inst->reg->GPIO_INT_BOTHEDGE,int_cfg->idx,int_cfg->both_edge);
    REG_BIT_WR(inst->reg->GPIO_INTTYPE_LEVEL,int_cfg->idx,int_cfg->edge);
    REG_BIT_WR(inst->reg->GPIO_INT_POLARITY,int_cfg->idx,int_cfg->polarity);
    REG_BIT_WR(inst->reg->GPIO_INTEN,int_cfg->idx,1);
    REG_BIT_WR(inst->reg->GPIO_INTMASK,int_cfg->idx,0);
}

void app_gpio_init(periph_inst_handle_t hdl)
{
    app_gpio_inst_t *inst = CONTAINER_OF(hdl, app_gpio_inst_t, inst);
    gpio_universal_func.clk_gate_func(inst,SET_CLK);
    gpio_universal_func.intr_op_func(inst,INTR_CLR);
    gpio_universal_func.intr_op_func(inst,INTR_ENABLE);
    struct co_list_hdr *hdr = co_list_pick(&inst->env.int_list);
    while(hdr)
    {
        app_gpio_int_cfg_t * int_cfg = CONTAINER_OF(hdr, app_gpio_int_cfg_t, hdr);
        gpio_int_cfg(inst,int_cfg);
        hdr = co_list_next(hdr);
    }
    gpio_universal_func.sys_stat_func(inst,GPIO_INIT);
}

void app_gpio_uninit(periph_inst_handle_t hdl)
{

    app_gpio_inst_t *inst = CONTAINER_OF(hdl, app_gpio_inst_t, inst);
    gpio_universal_func.clk_gate_func(inst,CLR_CLK);
    gpio_universal_func.intr_op_func(inst,INTR_DISABLE);
    gpio_universal_func.sys_stat_func(inst,GPIO_UNINIT);
}

void app_gpio_port_dir_output_set(periph_inst_handle_t hdl,uint32_t out_mask)
{
    app_gpio_inst_t *inst = CONTAINER_OF(hdl, app_gpio_inst_t, inst);
    inst->reg->GPIO_SWPORTA_DDR |= out_mask;
}

void app_gpio_port_dir_input_set(periph_inst_handle_t hdl,uint32_t in_mask)
{
    app_gpio_inst_t *inst = CONTAINER_OF(hdl, app_gpio_inst_t, inst);
    inst->reg->GPIO_SWPORTA_DDR &= ~in_mask;
}

void app_gpio_port_dir_write(periph_inst_handle_t hdl,uint32_t dir_mask)
{
    app_gpio_inst_t *inst = CONTAINER_OF(hdl, app_gpio_inst_t, inst);
    inst->reg->GPIO_SWPORTA_DDR = dir_mask;
}

uint32_t app_gpio_port_dir_read(periph_inst_handle_t hdl)
{
    app_gpio_inst_t *inst = CONTAINER_OF(hdl, app_gpio_inst_t, inst);
    return inst->reg->GPIO_SWPORTA_DDR;
}

uint32_t app_gpio_port_in_read(periph_inst_handle_t hdl)
{
    app_gpio_inst_t *inst = CONTAINER_OF(hdl, app_gpio_inst_t, inst);
    return inst->reg->GPIO_EXT_PORTA;
}

uint32_t app_gpio_port_out_read(periph_inst_handle_t hdl)
{
    app_gpio_inst_t *inst = CONTAINER_OF(hdl, app_gpio_inst_t, inst);
    return inst->reg->GPIO_SWPORTA_DR;
}

void app_gpio_port_out_write(periph_inst_handle_t hdl,uint32_t val)
{
    app_gpio_inst_t *inst = CONTAINER_OF(hdl, app_gpio_inst_t, inst);
    inst->reg->GPIO_SWPORTA_DR = val;
}

void app_gpio_port_out_set(periph_inst_handle_t hdl,uint32_t set_mask)
{
    app_gpio_inst_t *inst = CONTAINER_OF(hdl, app_gpio_inst_t, inst);
    inst->reg->GPIO_SWPORTA_DR |= set_mask;
}

void app_gpio_port_out_clr(periph_inst_handle_t hdl,uint32_t clr_mask)
{
    app_gpio_inst_t *inst = CONTAINER_OF(hdl, app_gpio_inst_t, inst);
    inst->reg->GPIO_SWPORTA_DR &= ~clr_mask;
}

void app_gpio_port_out_toggle(periph_inst_handle_t hdl,uint32_t toggle_mask)
{
    app_gpio_inst_t *inst = CONTAINER_OF(hdl, app_gpio_inst_t, inst);
    inst->reg->GPIO_SWPORTA_DR ^= toggle_mask;
}

void app_gpio_int_enable(periph_inst_handle_t hdl,app_gpio_int_cfg_t *int_cfg)
{
    app_gpio_inst_t *inst = CONTAINER_OF(hdl, app_gpio_inst_t, inst);
    if(co_list_find(&inst->env.int_list, &int_cfg->hdr) == false)
    {
        co_list_push_back(&inst->env.int_list, &int_cfg->hdr);
    }
    gpio_int_cfg(inst,int_cfg);
}

void app_gpio_int_disable(periph_inst_handle_t hdl,uint8_t idx)
{
    app_gpio_inst_t *inst = CONTAINER_OF(hdl, app_gpio_inst_t, inst);
    struct co_list_hdr *hdr = co_list_pick(&inst->env.int_list);
    while(hdr)
    {
        app_gpio_int_cfg_t * int_cfg = CONTAINER_OF(hdr, app_gpio_int_cfg_t, hdr);
        if(int_cfg->idx == idx)
        {
            REG_BIT_WR(inst->reg->GPIO_INTEN,idx,0);
            co_list_extract(&inst->env.int_list,hdr, 0);
            return;
        }
        hdr = co_list_next(hdr);
    }

}

void app_gpio_isr(app_gpio_inst_t *inst)
{
    struct co_list_hdr *hdr = co_list_pick(&inst->env.int_list);
    while(hdr)
    {
        app_gpio_int_cfg_t * int_cfg = CONTAINER_OF(hdr, app_gpio_int_cfg_t, hdr);
        if(REG_BIT_RD(inst->reg->GPIO_INTSTATUS, int_cfg->idx))
        {
            REG_BIT_WR(inst->reg->GPIO_PORTA_EOI,int_cfg->idx,1);
            int_cfg->callback();
        }
        hdr = co_list_next(hdr);
    }
}


