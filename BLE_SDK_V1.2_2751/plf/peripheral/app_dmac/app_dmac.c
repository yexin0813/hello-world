#include <stddef.h>
#include "periph_common.h"
#include "field_manipulate.h"
#include "dmac_typedef.h"
#include "app_dmac.h"
#include "co_utils.h"
#include "bx_dbg.h"
#include "ll.h"
extern periph_universal_func_set_t dmac_universal_func;
static void dmac_stat_clean_if_all_transfer_done(app_dmac_inst_t *inst);

void app_dmac_init(periph_inst_handle_t hdl)
{
    app_dmac_inst_t *inst = CONTAINER_OF(hdl, app_dmac_inst_t, inst);
    dmac_universal_func.sw_rst_func(inst);
    dmac_universal_func.clk_gate_func(inst,SET_CLK);
    dmac_universal_func.intr_op_func(inst,INTR_CLR);
    dmac_universal_func.intr_op_func(inst,INTR_ENABLE);
    reg_dmac_t *reg = inst->reg;
    reg->dma_cfg_reg_l = FIELD_BUILD(DMAC_DMA_EN,DMAC_Enabled);
    uint8_t ch_mask = (1<<inst->env.ch_num) - 1;
    reg->mask_err_l = FIELD_BUILD(DMAC_INT_MASK, ch_mask) | FIELD_BUILD(DMAC_INT_MASK_WE, ch_mask);
    reg->mask_tfr_l = FIELD_BUILD(DMAC_INT_MASK, ch_mask) | FIELD_BUILD(DMAC_INT_MASK_WE, ch_mask);
    dmac_universal_func.sys_stat_func(inst,DMAC_INIT);
    dmac_universal_func.clk_gate_func(inst,CLR_CLK);
}

void app_dmac_uninit(periph_inst_handle_t hdl)
{
    app_dmac_inst_t *inst = CONTAINER_OF(hdl, app_dmac_inst_t, inst);
    dmac_universal_func.clk_gate_func(inst,CLR_CLK);
    dmac_universal_func.intr_op_func(inst,INTR_DISABLE);
    dmac_universal_func.sys_stat_func(inst,DMAC_UNINIT);
}

static uint8_t dmac_ch_alloc(app_dmac_inst_t *inst)
{
    uint8_t i;
    for(i=0;i<inst->env.ch_num;++i)
    {
        uint8_t ch_mask = 1<<i;
        bool ch_available;
        GLOBAL_INT_DISABLE();
        ch_available = (inst->env.ch_stat & ch_mask) == 0;
        if(ch_available)
        {
            inst->env.ch_stat |= ch_mask;
        }
        GLOBAL_INT_RESTORE();
        if(ch_available)
        {
            return i;
        }
    }
    BX_ASSERT(0);
    return 0;
}

static void dmac_ch_free(app_dmac_inst_t *inst,uint8_t ch_idx)
{
    uint8_t free_ch_mask = ~(1<<ch_idx);
    ATOMIC_OP(inst->env.ch_stat &= free_ch_mask);
}

static void dmac_block_ts_config(app_dmac_inst_t *inst,uint8_t ch_idx)
{
    reg_dmac_t *reg = inst->reg;
    if(inst->env.ch[ch_idx].size > 4095)
    {
        FIELD_WR(&reg->ch[ch_idx],ctl_h,DMAC_BLOCK_TS,4095);
        inst->env.ch[ch_idx].size -= 4095;
    }else
    {
        FIELD_WR(&reg->ch[ch_idx],ctl_h,DMAC_BLOCK_TS,inst->env.ch[ch_idx].size);
        inst->env.ch[ch_idx].size = 0;
    }
}

uint8_t app_dmac_start(periph_inst_handle_t hdl,app_dmac_transfer_param_t *param)
{
    app_dmac_inst_t *inst = CONTAINER_OF(hdl, app_dmac_inst_t, inst);
    uint8_t ch_idx = dmac_ch_alloc(inst);
    inst->env.ch[ch_idx].callback = param->callback;
    inst->env.ch[ch_idx].callback_param = param->callback_param;
    inst->env.ch[ch_idx].size = param->length;
    dmac_universal_func.clk_gate_func(inst,SET_CLK);
    dmac_universal_func.sys_stat_func(inst,DMAC_TRANSFER_START);
    dmac_block_ts_config(inst,ch_idx);
    uint8_t sinc,dinc;
    switch(param->tt_fc)
    {
    case Peripheral_to_Memory_DMAC_Flow_Controller:
        sinc = Address_No_Change;
        dinc = Address_Increment;
    break;
    case Memory_to_Peripheral_DMAC_Flow_Controller:
        sinc = Address_Increment;
        dinc = Address_No_Change;
    break;
    case Memory_to_Memory_DMAC_Flow_Controller:
        sinc = Address_Increment;
        dinc = Address_Increment;
    break;
    default:
        BX_ASSERT(0);
    break;
    }
    reg_dmac_t *reg = inst->reg;
    reg->ch[ch_idx].ctl_l = FIELD_BUILD(DMAC_TT_FC,param->tt_fc) | 
        FIELD_BUILD(DMAC_SRC_MSIZE, param->src_msize) | FIELD_BUILD(DMAC_DEST_MSIZE, param->dst_msize) |
        FIELD_BUILD(DMAC_SINC, sinc) | FIELD_BUILD(DMAC_DINC, dinc) |
        FIELD_BUILD(DMAC_SRC_TR_WIDTH, param->src_tr_width) | FIELD_BUILD(DMAC_DST_TR_WIDTH, param->dst_tr_width) |
        FIELD_BUILD(DMAC_INT_EN, param->int_en);
    reg->ch[ch_idx].cfg_l = FIELD_BUILD(DMAC_SRC_HS_POL, HS_Polarity_Active_High) | FIELD_BUILD(DMAC_DST_HS_POL, HS_Polarity_Active_High) |
        FIELD_BUILD(DMAC_HS_SEL_SRC, Hardware_Handshaking) | FIELD_BUILD(DMAC_HS_SEL_DST, Hardware_Handshaking);
    reg->ch[ch_idx].cfg_h = FIELD_BUILD(DMAC_DEST_PER,param->dst_per) | FIELD_BUILD(DMAC_SRC_PER, param->src_per) |
            FIELD_BUILD(DMAC_FIFO_MODE,1);
    reg->ch[ch_idx].sar_l = (uint32_t)param->src;
    reg->ch[ch_idx].dar_l = (uint32_t)param->dst;
    reg->ch_en_reg_l = FIELD_BUILD(DMAC_CH_EN,1<<ch_idx) | FIELD_BUILD(DMAC_CH_EN_WE, 1<<ch_idx);
    return ch_idx;
}

uint32_t app_dmac_transfer_cancel(periph_inst_handle_t hdl,uint8_t ch_idx)
{
    app_dmac_inst_t *inst = CONTAINER_OF(hdl, app_dmac_inst_t, inst);
    reg_dmac_t *reg = inst->reg;
    REG_FIELD_WR(reg->ch[ch_idx].cfg_l,DMAC_CH_SUSP,1);
    while(REG_FIELD_RD(reg->ch[ch_idx].cfg_l, DMAC_FIFO_EMPTY)==0);
    reg->ch_en_reg_l = FIELD_BUILD(DMAC_CH_EN,0) | FIELD_BUILD(DMAC_CH_EN_WE, 1<<ch_idx);
    inst->env.ch[ch_idx].size -= REG_FIELD_RD(reg->ch[ch_idx].ctl_h, DMAC_BLOCK_TS);
    dmac_ch_free(inst,ch_idx);
    dmac_stat_clean_if_all_transfer_done(inst);
    return inst->env.ch[ch_idx].size;
}

static void dmac_stat_clean_if_all_transfer_done(app_dmac_inst_t *inst)
{
    if(inst->reg->ch_en_reg_l == 0 && inst->reg->status_tfr_l == 0 && inst->reg->status_err_l == 0)
    {
        dmac_universal_func.clk_gate_func(inst,CLR_CLK);
        dmac_universal_func.sys_stat_func(inst,DMAC_TRANSFER_DONE);
    }
}

void app_dmac_transfer_wait(periph_inst_handle_t hdl,uint8_t ch_idx)
{
    app_dmac_inst_t *inst = CONTAINER_OF(hdl, app_dmac_inst_t, inst);
    reg_dmac_t *reg = inst->reg;
    while((reg->raw_tfr_l&1<<ch_idx)==0);
    reg->clear_tfr_l = 1<<ch_idx;
    while(inst->env.ch[ch_idx].size)
    {
        dmac_block_ts_config(inst,ch_idx);
        reg->ch_en_reg_l = FIELD_BUILD(DMAC_CH_EN,1<<ch_idx) | FIELD_BUILD(DMAC_CH_EN_WE, 1<<ch_idx);
        while((reg->raw_tfr_l&1<<ch_idx)==0);
        reg->clear_tfr_l = 1<<ch_idx;
    }
    dmac_ch_free(inst,ch_idx);
    dmac_stat_clean_if_all_transfer_done(inst);
}

uint8_t dmac_get_burst_transaction_size_enum(uint16_t msize)
{
    if(msize ==  1)
    {
        return Burst_Transaction_Length_1;
    }
    uint8_t val = 0;
    switch(msize)
    {
    case 4:
        val = Burst_Transaction_Length_4;
    break;
    case 8:
        val = Burst_Transaction_Length_8;
    break;
    case 16:
        val = Burst_Transaction_Length_16;
    break;
    case 32:
        val = Burst_Transaction_Length_32;
    break;
    case 64:
        val = Burst_Transaction_Length_64;
    break;
    case 128:
        val = Burst_Transaction_Length_128;
    break;
    case 256:
        val = Burst_Transaction_Length_256;
    break;
    default:
        BX_ASSERT(0);
    break;
    }
    return val;
}

void dmac_tfr_isr(app_dmac_inst_t *inst,uint8_t ch_idx)
{
    reg_dmac_t *reg = inst->reg;
    if(inst->env.ch[ch_idx].size)
    {
        dmac_block_ts_config(inst,ch_idx);
        reg->ch_en_reg_l = FIELD_BUILD(DMAC_CH_EN,1<<ch_idx) | FIELD_BUILD(DMAC_CH_EN_WE, 1<<ch_idx);
    }else
    {
        void (*cb)(void *) = inst->env.ch[ch_idx].callback;
        void *param = inst->env.ch[ch_idx].callback_param;
        dmac_ch_free(inst,ch_idx);
        cb(param);
    }
}

void app_dmac_isr(app_dmac_inst_t *inst)
{
    reg_dmac_t *reg = inst->reg;
    BX_ASSERT(reg->status_err_l == 0);
    while(reg->status_tfr_l)
    {
        uint8_t i;
        for(i=0;i<inst->env.ch_num;++i)
        {
            if(reg->status_tfr_l & 1<<i)
            {
                reg->clear_tfr_l = 1<<i;
                dmac_tfr_isr(inst,i);
            }
        }
    }
    dmac_stat_clean_if_all_transfer_done(inst);
}
