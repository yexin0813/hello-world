#include "reg_dmac.h"
#include "_reg_base_addr.h"
#include "dmac_typedef.h"
#include "field_manipulate.h"
#include "dma_for_qspi.h"
#define DMAC_REG ((reg_dmac_t *)REG_DMAC_BASE)

void dmac_init()
{
	DMAC_REG->dma_cfg_reg_l = FIELD_BUILD(DMAC_DMA_EN,DMAC_Enabled);
	DMAC_REG->clear_tfr_l = 0xff;
}

static uint32_t dmac_block_ts_config(uint32_t size,uint8_t ch_idx)
{

    if(size > 4095)
    {
        FIELD_WR(&DMAC_REG->ch[ch_idx],ctl_h,DMAC_BLOCK_TS,4095);
        size -= 4095;
    }else
    {
        FIELD_WR(&DMAC_REG->ch[ch_idx],ctl_h,DMAC_BLOCK_TS,size);
        size = 0;
    }
    return size;
}

uint32_t dmac_start(app_dmac_transfer_param_t *param,uint8_t ch_idx)
{

    uint32_t remaining_length = dmac_block_ts_config(param->length,ch_idx); 
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
        //BX_ASSERT(0);
    break;
    }
    DMAC_REG->ch[ch_idx].ctl_l = FIELD_BUILD(DMAC_TT_FC,param->tt_fc) | 
        FIELD_BUILD(DMAC_SRC_MSIZE, param->src_msize) | FIELD_BUILD(DMAC_DEST_MSIZE, param->dst_msize) |
        FIELD_BUILD(DMAC_SINC, sinc) | FIELD_BUILD(DMAC_DINC, dinc) |
        FIELD_BUILD(DMAC_SRC_TR_WIDTH, param->src_tr_width) | FIELD_BUILD(DMAC_DST_TR_WIDTH, param->dst_tr_width) |
        FIELD_BUILD(DMAC_INT_EN, 0);
    DMAC_REG->ch[ch_idx].cfg_l = FIELD_BUILD(DMAC_SRC_HS_POL, HS_Polarity_Active_High) | FIELD_BUILD(DMAC_DST_HS_POL, HS_Polarity_Active_High) |
        FIELD_BUILD(DMAC_HS_SEL_SRC, Hardware_Handshaking) | FIELD_BUILD(DMAC_HS_SEL_DST, Hardware_Handshaking);
    DMAC_REG->ch[ch_idx].cfg_h = FIELD_BUILD(DMAC_DEST_PER,param->dst_per) | FIELD_BUILD(DMAC_SRC_PER, param->src_per) |
            FIELD_BUILD(DMAC_FIFO_MODE,1);
    DMAC_REG->ch[ch_idx].sar_l = (uint32_t)param->src;
    DMAC_REG->ch[ch_idx].dar_l = (uint32_t)param->dst;
    DMAC_REG->ch_en_reg_l = FIELD_BUILD(DMAC_CH_EN,1<<ch_idx) | FIELD_BUILD(DMAC_CH_EN_WE, 1<<ch_idx);
    return remaining_length;
}

void dmac_transfer_wait(uint32_t remaining_length,uint8_t ch_idx)
{
    while((DMAC_REG->raw_tfr_l&1<<ch_idx)==0);
    DMAC_REG->clear_tfr_l = 1<<ch_idx;
    while(remaining_length)
    {
        remaining_length = dmac_block_ts_config(remaining_length,ch_idx);
        DMAC_REG->ch_en_reg_l = FIELD_BUILD(DMAC_CH_EN,1<<ch_idx) | FIELD_BUILD(DMAC_CH_EN_WE, 1<<ch_idx);
        while((DMAC_REG->raw_tfr_l&1<<ch_idx)==0);
        DMAC_REG->clear_tfr_l = 1<<ch_idx;
    }
}
