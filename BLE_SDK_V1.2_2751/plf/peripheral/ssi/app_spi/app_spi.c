#include "app_spi.h"
#include "periph_common.h"
#include "log.h"
#include "co_utils.h"
#include "field_manipulate.h"
#include "app_dmac_wrapper.h"
extern periph_universal_func_set_t spi_universal_func;

uint8_t spi_get_fifo_depth(app_spi_inst_t *inst);

void app_spi_init(periph_inst_handle_t hdl)
{
    app_spi_inst_t *inst = CONTAINER_OF(hdl, app_spi_inst_t, inst);
    app_spi_init_param_t *param = &inst->param;
    spi_universal_func.sw_rst_func(inst);
    spi_universal_func.clk_gate_func(inst,SET_CLK);
    spi_universal_func.pin_cfg_func(inst,param->mosi_pin_no,MOSI_PIN);
    spi_universal_func.pin_cfg_func(inst,param->miso_pin_no,MISO_PIN);
    if(inst->slave)
    {
        spi_universal_func.pin_cfg_func(inst,param->clk_pin_no,SPIS_CLK_PIN);
        spi_universal_func.pin_cfg_func(inst,param->u.slave.cs_pin_no,SPIS_CS_PIN);
    }else
    {
        spi_universal_func.pin_cfg_func(inst,param->clk_pin_no,SPIM_CLK_PIN);
        uint8_t i;
        for(i=0;i<SPIM_CS_NUM;++i)
        {
            if(1<<i & param->u.master.hw_cs_mask)
            {
                spi_universal_func.pin_cfg_func(inst,param->u.master.cs_pin_no[i],SPIM_CS0_PIN + i);
            }
        }
        inst->reg->BAUDR = param->u.master.clk_div; 
        inst->reg->RXSAMPLE_DLY = param->u.master.rxsample_dly;
    }
    spi_universal_func.intr_op_func(inst,INTR_CLR);
    spi_universal_func.intr_op_func(inst,INTR_ENABLE);
    spi_universal_func.clk_gate_func(inst,CLR_CLK);
    spi_universal_func.sys_stat_func(inst,SPI_INIT);
}

void app_spi_uninit(periph_inst_handle_t hdl)
{
    app_spi_inst_t *inst = CONTAINER_OF(hdl, app_spi_inst_t, inst);
    spi_universal_func.intr_op_func(inst,INTR_DISABLE);
    spi_universal_func.sys_stat_func(inst,SPI_UNINIT);
    spi_universal_func.clk_gate_func(inst,CLR_CLK);

}

static void spi_dma_tx_done_callback(app_spi_inst_t *inst)
{
    inst->env.tx_size = 0;
    inst->reg->TXFTLR = 0;
    REG_FIELD_WR(inst->reg->IMR, SSI_TXEIM,1);
}

static uint8_t spi_get_dma_transfer_width(app_spi_inst_t *inst)
{
    uint8_t tr_width;
    if(inst->param.dfs_32 <= DFS_32_8_bits)
    {
        tr_width = Transfer_Width_8_bits;
    }else if(inst->param.dfs_32 <= DFS_32_16_bits)
    {
        tr_width = Transfer_Width_16_bits;
    }else
    {
        tr_width = Transfer_Width_32_bits;
    }
    return tr_width;
}

static void spi_tx_threshold_config(app_spi_inst_t *inst)
{
    uint8_t fifo_depth = spi_get_fifo_depth(inst);
    if(inst->param.tx_dma)
    {
        inst->reg->DMATDLR = fifo_depth - 8;
        REG_FIELD_WR(inst->reg->DMACR,SSI_TDMAE,1);
        REG_FIELD_WR(inst->reg->IMR, SSI_TXEIM,0);
    }else
    {
        inst->reg->TXFTLR =  fifo_depth / 2;
        REG_FIELD_WR(inst->reg->IMR, SSI_TXEIM,1);
    }
}

static void spi_dma_tx_start_if_enable(app_spi_inst_t *inst,uint8_t *data,uint16_t size)
{
    if(inst->param.tx_dma)
    {
        uint8_t tr_width =spi_get_dma_transfer_width(inst);
        app_dmac_transfer_param_t param = 
        {
            .callback = (void (*)(void*))spi_dma_tx_done_callback,
            .callback_param = inst,
            .src = data,
            .dst = (uint8_t *)&inst->reg->DR,
            .length = size,
            .src_tr_width = tr_width,
            .dst_tr_width = tr_width,
            .src_msize = Burst_Transaction_Length_8,
            .dst_msize = Burst_Transaction_Length_8,
            .tt_fc = Memory_to_Peripheral_DMAC_Flow_Controller,
            .src_per = inst->slave ? dmac_spi_slave_rx_handshake_enum(inst->idx) : dmac_spi_master_rx_handshake_enum(inst->idx),
            .dst_per = inst->slave ? dmac_spi_slave_tx_handshake_enum(inst->idx) : dmac_spi_master_tx_handshake_enum(inst->idx),
            .int_en = Interrupt_Enabled,
        };
        app_dmac_start_wrapper(&param);
        if(inst->slave == false)
        {
            inst->reg->SER = inst->env.cs_sel_mask;
        }
    }
}

void app_spi_transmit(periph_inst_handle_t hdl,uint8_t cs_sel_mask,void *data,uint16_t size,void (*cb)(void *),void *cb_param)
{
    app_spi_inst_t *inst = CONTAINER_OF(hdl, app_spi_inst_t, inst);
    inst->env.tx_data = data;
    inst->env.tx_size = size;
    inst->env.rx_size = 0;
    inst->env.callback = cb;
    inst->env.callback_param = cb_param;
    inst->env.cs_sel_mask = cs_sel_mask;
    spi_universal_func.clk_gate_func(inst,SET_CLK);
    spi_universal_func.sys_stat_func(inst,SPI_TX_START);
    app_spi_init_param_t *param = &inst->param;
    inst->reg->CTRLR0 = FIELD_BUILD(SSI_DFS,param->dfs_32) | FIELD_BUILD(SSI_TMOD,Transmit_Only) |FIELD_BUILD(SSI_SCPOL,param->cpol) 
        | FIELD_BUILD(SSI_SCPH,param->cph) | FIELD_BUILD(SSI_FRF,Motorola_SPI);
    inst->reg->DMACR = 0;
    spi_tx_threshold_config(inst);
    inst->reg->SSIENR = SSI_Enabled;
    spi_dma_tx_start_if_enable(inst,data,size);
}

static void spi_rx_done(app_spi_inst_t *inst)
{
    if(REG_FIELD_RD(inst->reg->CTRLR0,SSI_TMOD) == Receive_Only)
    {
        spi_universal_func.sys_stat_func(inst,SPI_RX_DONE);
    }else if(REG_FIELD_RD(inst->reg->CTRLR0,SSI_TMOD) == Transmit_and_Receive)
    {
        spi_universal_func.sys_stat_func(inst,SPI_TXRX_DONE);
    }
    inst->reg->SSIENR = SSI_Disabled;
    inst->reg->SER = 0;
    inst->env.callback(inst->env.callback_param);
}

static void spi_dma_rx_done_callback(app_spi_inst_t *inst)
{
    inst->env.rx_size = 0;
    spi_rx_done(inst);
    if(inst->reg->ISR == 0 && inst->reg->SSIENR == SSI_Disabled)
    {
        spi_universal_func.clk_gate_func(inst,CLR_CLK);
    }
}

static void spi_dma_rx_start_if_enable(app_spi_inst_t *inst,uint8_t *data,uint16_t size)
{
    if(inst->param.rx_dma == 1)
    {
        uint8_t tr_width =spi_get_dma_transfer_width(inst);
        app_dmac_transfer_param_t param = 
        {
            .callback = (void (*)(void*))spi_dma_rx_done_callback,
            .callback_param = inst,
            .src = (uint8_t *)&inst->reg->DR,
            .dst = data,
            .length = size,
            .src_tr_width = tr_width,
            .dst_tr_width = tr_width,
            .src_msize = Burst_Transaction_Length_8,
            .dst_msize = Burst_Transaction_Length_8,
            .tt_fc = Peripheral_to_Memory_DMAC_Flow_Controller,
            .src_per = inst->slave ? dmac_spi_slave_rx_handshake_enum(inst->idx) : dmac_spi_master_rx_handshake_enum(inst->idx),
            .dst_per = inst->slave ? dmac_spi_slave_tx_handshake_enum(inst->idx) : dmac_spi_master_tx_handshake_enum(inst->idx),
            .int_en = Interrupt_Enabled,
        };
        app_dmac_start_wrapper(&param);
    }
}

static void spi_rx_threshold_config(app_spi_inst_t *inst)
{
    uint8_t fifo_depth = spi_get_fifo_depth(inst);
    if(inst->param.rx_dma)
    {
        inst->reg->DMARDLR = 7;
        REG_FIELD_WR(inst->reg->DMACR,SSI_RDMAE,1);
        REG_FIELD_WR(inst->reg->IMR, SSI_RXFIM, 0);
    }else
    {
        inst->reg->RXFTLR = inst->env.rx_size > fifo_depth ? fifo_depth / 2 -1: inst->env.rx_size -1;
        REG_FIELD_WR(inst->reg->IMR, SSI_RXFIM, 1);
    }
}

void app_spi_receive(periph_inst_handle_t hdl,uint8_t cs_sel_mask,void *data,uint16_t size,void (*cb)(void *),void *cb_param)
{
    app_spi_inst_t *inst = CONTAINER_OF(hdl, app_spi_inst_t, inst);
    inst->env.rx_data = data;
    inst->env.rx_size = size;
    inst->env.tx_size = 0;
    inst->env.callback = cb;
    inst->env.callback_param = cb_param;
    inst->env.cs_sel_mask = cs_sel_mask;
    spi_universal_func.clk_gate_func(inst,SET_CLK);
    spi_universal_func.sys_stat_func(inst,SPI_RX_START);
    app_spi_init_param_t *param = &inst->param;
    inst->reg->CTRLR0 = FIELD_BUILD(SSI_DFS,param->dfs_32) | FIELD_BUILD(SSI_TMOD,Receive_Only) |FIELD_BUILD(SSI_SCPOL,param->cpol) 
        | FIELD_BUILD(SSI_SCPH,param->cph) | FIELD_BUILD(SSI_FRF,Motorola_SPI);
    if(inst->slave == false) inst->reg->CTRLR1 = size - 1;
    inst->reg->DMACR = 0;
    spi_rx_threshold_config(inst);
    inst->reg->SSIENR = SSI_Enabled;
    spi_dma_rx_start_if_enable(inst,data,size);
    if(inst->slave == false) 
    {
        inst->reg->SER= inst->param.u.master.hw_cs_mask ? inst->env.cs_sel_mask : 0xff;
        inst->reg->DR = 0;
    }
}

void app_spi_transmit_receive(periph_inst_handle_t hdl,uint8_t cs_sel_mask,void *tx_data,void *rx_data,uint16_t size,void (*cb)(void *),void *cb_param)
{
    app_spi_inst_t *inst = CONTAINER_OF(hdl, app_spi_inst_t, inst);
    inst->env.tx_data = tx_data;
    inst->env.rx_data = rx_data;
    inst->env.tx_size = size;
    inst->env.rx_size = size;
    inst->env.callback = cb;
    inst->env.callback_param = cb_param;
    inst->env.cs_sel_mask = cs_sel_mask;
    spi_universal_func.clk_gate_func(inst,SET_CLK);
    spi_universal_func.sys_stat_func(inst,SPI_TXRX_START);
    app_spi_init_param_t *param = &inst->param;
    inst->reg->CTRLR0 = FIELD_BUILD(SSI_DFS,param->dfs_32) | FIELD_BUILD(SSI_TMOD,Transmit_and_Receive) |FIELD_BUILD(SSI_SCPOL,param->cpol) 
        | FIELD_BUILD(SSI_SCPH,param->cph) | FIELD_BUILD(SSI_FRF,Motorola_SPI);
    inst->reg->DMACR = 0;
    spi_tx_threshold_config(inst);
    spi_rx_threshold_config(inst);
    inst->reg->SSIENR = SSI_Enabled;
    spi_dma_rx_start_if_enable(inst,rx_data,size);
    spi_dma_tx_start_if_enable(inst,tx_data,size);
}


static void spi_tx_empty_isr(app_spi_inst_t *inst)
{
    if(inst->env.tx_size)
    {
        while(REG_FIELD_RD(inst->reg->SR,SSI_TFNF))
        {
            if(inst->param.dfs_32 <= DFS_32_8_bits)
            {
                inst->reg->DR = *(uint8_t *)inst->env.tx_data;
                inst->env.tx_data += 1;
            }else if(inst->param.dfs_32 <= DFS_32_16_bits)
            {
                inst->reg->DR = *(uint16_t *)inst->env.tx_data;
                inst->env.tx_data += 2;
            }else
            {
                inst->reg->DR = *(uint32_t *)inst->env.tx_data;
                inst->env.tx_data += 4;
            }
            --inst->env.tx_size;
            if(inst->env.tx_size == 0)
            {
                break;
            }
        }
        if(REG_FIELD_RD(inst->reg->SR,SSI_BUSY) == SSI_Idle_or_Disabled && inst->slave == false)
        {
            inst->reg->SER= inst->param.u.master.hw_cs_mask ? inst->env.cs_sel_mask : 0xff;
        }
    }else
    {
        if(inst->reg->TXFLR)
        {
            inst->reg->TXFTLR = 0;
        }else
        {
            REG_FIELD_WR(inst->reg->IMR,SSI_TXEIM,0);
            if(REG_FIELD_RD(inst->reg->CTRLR0,SSI_TMOD) == Transmit_Only)
            {
                while(REG_FIELD_RD(inst->reg->SR,SSI_BUSY) == SSI_Busy);
                spi_universal_func.sys_stat_func(inst,SPI_TX_DONE);
                inst->reg->SSIENR = SSI_Disabled;
                inst->reg->SER = 0;
                inst->env.callback(inst->env.callback_param);
            }
        }
    }

}

static void spi_rx_full_isr(app_spi_inst_t *inst)
{
    while(REG_FIELD_RD(inst->reg->SR,SSI_RFNE))
    {
        if(inst->param.dfs_32 <= DFS_32_8_bits)
        {
            *(uint8_t *)inst->env.rx_data = (uint8_t)inst->reg->DR;
            inst->env.rx_data += 1;
        }else if(inst->param.dfs_32 <= DFS_32_16_bits)
        {
            *(uint16_t *)inst->env.rx_data = (uint16_t)inst->reg->DR;
            inst->env.rx_data += 2;
        }else
        {
            *(uint32_t *)inst->env.rx_data = (uint32_t)inst->reg->DR;
            inst->env.rx_data += 4;
        }
        --inst->env.rx_size;
    }
    if(inst->env.rx_size)
    {
        uint8_t fifo_depth = spi_get_fifo_depth(inst);
        inst->reg->RXFTLR = inst->env.rx_size > fifo_depth ? fifo_depth / 2 -1: inst->env.rx_size -1;
    }else
    {
        spi_rx_done(inst);
    }
}

void app_spi_isr(app_spi_inst_t *inst)
{
    while(1)
    {
        uint8_t irq =inst->reg->ISR;
        if(irq == 0) break;
        if(irq & SSI_MSTIS_MASK)
        {
            LOG(LOG_LVL_WARN,"spi%d,multi_master contention\n",inst->idx);
            inst->reg->MSTICR;
            BX_ASSERT(0);
        }
        if(irq & SSI_RXFIS_MASK)
        {
            spi_rx_full_isr(inst);
        }
        if(irq & SSI_RXOIS_MASK)
        {
            LOG(LOG_LVL_WARN,"spi%d,Receive FIFO Overflow\n",inst->idx);
            inst->reg->RXOICR;
            BX_ASSERT(0);
        }
        if(irq & SSI_RXUIS_MASK)
        {
            LOG(LOG_LVL_WARN,"spi%d,Receive FIFO Underflow\n",inst->idx);
            inst->reg->RXUICR;
            BX_ASSERT(0);
        }
        if(irq & SSI_TXOIS_MASK)
        {
            LOG(LOG_LVL_WARN,"spi%d,Transmit FIFO Overflow\n",inst->idx);
            inst->reg->TXOICR;
            BX_ASSERT(0);
        }
        if(irq & SSI_TXEIS_MASK)
        {
            spi_tx_empty_isr(inst);
        }
    }
    if(inst->reg->SSIENR == SSI_Disabled)
    {
        spi_universal_func.clk_gate_func(inst,CLR_CLK);
    }
}

