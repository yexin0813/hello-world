#include "field_manipulate.h"
#include "app_qspi.h"
#include "dw_apb_ssi_typedef.h"
#include "app_dmac_wrapper.h"
#include "log.h"
#define ADDR_EXTEND(addr,lsb) ((addr)<<8|(lsb))
extern periph_universal_func_set_t qspi_universal_func;

uint16_t qspi_get_fifo_depth(app_qspi_inst_t *inst);

void app_qspi_init(periph_inst_handle_t hdl)
{
    app_qspi_inst_t *inst = CONTAINER_OF(hdl, app_qspi_inst_t, inst);
    qspi_universal_func.sw_rst_func(inst);
    qspi_universal_func.clk_gate_func(inst,SET_CLK);
    qspi_universal_func.pin_cfg_func(inst,inst->param.clk_pin_no,QSPI_CLK_PIN);
    if(inst->param.mode == Quad_SPI_Format)
    {
        qspi_universal_func.pin_cfg_func(inst,inst->param.data_pin_no[3],QSPI_D3_PIN);
        qspi_universal_func.pin_cfg_func(inst,inst->param.data_pin_no[2],QSPI_D2_PIN);
    }
    qspi_universal_func.pin_cfg_func(inst,inst->param.data_pin_no[0],QSPI_D0_PIN);
    qspi_universal_func.pin_cfg_func(inst,inst->param.data_pin_no[1],QSPI_D1_PIN);
    uint8_t i;
    uint8_t pin_role;
    for(i=0,pin_role = QSPI_CS0_PIN;i<QSPI_CS_NUM;++i,++pin_role)
    {
        qspi_universal_func.pin_cfg_func(inst,inst->param.cs_pin_no[i],pin_role);     
    }
    inst->reg->SSIENR = SSI_Disabled;
    inst->reg->BAUDR = inst->param.clk_div;
    inst->reg->RXSAMPLE_DLY = inst->param.rxsample_dly;
    qspi_universal_func.sys_stat_func(inst,QSPI_INIT);
    qspi_universal_func.clk_gate_func(inst,CLR_CLK);
}

void app_qspi_uninit(periph_inst_handle_t hdl)
{
    app_qspi_inst_t *inst = CONTAINER_OF(hdl, app_qspi_inst_t, inst);
    qspi_universal_func.clk_gate_func(inst,CLR_CLK);
    qspi_universal_func.sys_stat_func(inst,QSPI_UNINIT);
}

static void qspi_std_byte_write_dma_config(reg_ssi_t *reg,uint16_t fifo_depth)
{
    reg->CTRLR0 = FIELD_BUILD(SSI_SPI_FRF,Standard_SPI_Format) | FIELD_BUILD(SSI_DFS,DFS_32_8_bits) | FIELD_BUILD(SSI_TMOD, Transmit_Only) |
        FIELD_BUILD(SSI_SCPOL,Inactive_Low)|FIELD_BUILD(SSI_SCPH, SCLK_Toggle_In_Middle) | FIELD_BUILD(SSI_FRF, Motorola_SPI);
    reg->DMATDLR = fifo_depth - 4;
    reg->DMACR = 0;
    reg->DMACR = FIELD_BUILD(SSI_TDMAE,Transmit_DMA_Enabled);
    reg->SSIENR = SSI_Enabled;
}
/*
static void qspi_flush_fifo_data(reg_ssi_t *reg,uint8_t depth)
{
    reg->SSIENR = SSI_Enabled;
    uint8_t i;
    for(i=0;i<depth;++i)
    {
        reg->DR = 0;
    }
    reg->SSIENR = SSI_Disabled;

}
*/
static void qspi_std_byte_write_dma_start(reg_ssi_t *reg,uint8_t cs_sel_mask,uint8_t *data,uint32_t length)
{
    app_dmac_transfer_param_t dma_param = {
            .src = data,
            .dst = (uint8_t *)&reg->DR,
            .length = length,
            .src_tr_width = Transfer_Width_8_bits,
            .dst_tr_width = Transfer_Width_8_bits,
            .src_msize = Burst_Transaction_Length_4,
            .dst_msize = Burst_Transaction_Length_4,
            .tt_fc = Memory_to_Peripheral_DMAC_Flow_Controller,
            .src_per = dmac_qspi_rx_handshake_enum(0),
            .dst_per = dmac_qspi_tx_handshake_enum(0),
            .int_en = Interrupt_Disabled,
    };
    uint8_t ch_idx = app_dmac_start_wrapper(&dma_param);
    reg->SER = cs_sel_mask;
    app_dmac_transfer_wait_wrapper(ch_idx);
    while(FIELD_RD(reg,SR,SSI_TFE) == Transmit_FIFO_Not_Empty);
    while(FIELD_RD(reg,SR,SSI_BUSY) == SSI_Busy);
    reg->SSIENR = SSI_Disabled;
    reg->SER = 0;
//    qspi_flush_fifo_data(reg,3);
}

void app_qspi_std_write(periph_inst_handle_t hdl,uint8_t cs_sel_mask,uint8_t *data,uint32_t length)
{
    app_qspi_inst_t *inst = CONTAINER_OF(hdl, app_qspi_inst_t, inst);
    uint16_t fifo_depth = qspi_get_fifo_depth(inst);
    reg_ssi_t *reg = inst->reg;
    qspi_universal_func.clk_gate_func(inst,SET_CLK);
    qspi_universal_func.sys_stat_func(inst,QSPI_OP_START);
    qspi_std_byte_write_dma_config(reg,fifo_depth);
    qspi_std_byte_write_dma_start(reg,cs_sel_mask,data,length);
    qspi_universal_func.sys_stat_func(inst,QSPI_OP_DONE);
    qspi_universal_func.clk_gate_func(inst,CLR_CLK);
}

void app_qspi_flash_program(periph_inst_handle_t hdl,uint8_t cs_sel_mask,uint8_t cmd,uint32_t addr,uint8_t *data,uint32_t length)
{
    app_qspi_inst_t *inst = CONTAINER_OF(hdl, app_qspi_inst_t, inst);
    uint16_t fifo_depth = qspi_get_fifo_depth(inst);
    reg_ssi_t *reg = inst->reg;
    qspi_universal_func.clk_gate_func(inst,SET_CLK);
    qspi_universal_func.sys_stat_func(inst,QSPI_OP_START);
    qspi_std_byte_write_dma_config(reg,fifo_depth);
    reg->DR = cmd;
    reg->DR = addr>>16 & 0xff;
    reg->DR = addr>>8 & 0xff;
    reg->DR = addr & 0xff;
    qspi_std_byte_write_dma_start(reg,cs_sel_mask,data,length);
    qspi_universal_func.sys_stat_func(inst,QSPI_OP_DONE);
    qspi_universal_func.clk_gate_func(inst,CLR_CLK);
}

void app_qspi_std_read(periph_inst_handle_t hdl,uint8_t cs_sel_mask,uint8_t *cmd_buf,uint8_t cmd_len,uint8_t *data_buf,uint16_t data_len)
{
    app_qspi_inst_t *inst = CONTAINER_OF(hdl, app_qspi_inst_t, inst);
    reg_ssi_t *reg = inst->reg;
    qspi_universal_func.clk_gate_func(inst,SET_CLK);
    qspi_universal_func.sys_stat_func(inst,QSPI_OP_START);
    reg->CTRLR0 = FIELD_BUILD(SSI_SPI_FRF,Standard_SPI_Format) | FIELD_BUILD(SSI_DFS,DFS_32_8_bits) | FIELD_BUILD(SSI_TMOD, EEPROM_Read) |
        FIELD_BUILD(SSI_SCPOL,Inactive_Low)|FIELD_BUILD(SSI_SCPH, SCLK_Toggle_In_Middle) | FIELD_BUILD(SSI_FRF, Motorola_SPI);
    reg->CTRLR1 = data_len - 1;
    reg->DMARDLR = 7;
    reg->DMACR = 0;
    reg->DMACR= FIELD_BUILD(SSI_RDMAE , Receive_DMA_Enabled);
    reg->SSIENR = SSI_Enabled;
    uint8_t i;
    for(i=0;i<cmd_len;++i)
    {
        reg->DR = cmd_buf[i];
    }
    app_dmac_transfer_param_t dma_param = {
            .src = (uint8_t *)&reg->DR,
            .dst = data_buf,
            .length = data_len,
            .src_tr_width = Transfer_Width_8_bits,
            .dst_tr_width = Transfer_Width_8_bits,
            .src_msize = Burst_Transaction_Length_8,
            .dst_msize = Burst_Transaction_Length_8,
            .tt_fc = Peripheral_to_Memory_DMAC_Flow_Controller,
            .src_per = dmac_qspi_rx_handshake_enum(0),
            .dst_per = dmac_qspi_tx_handshake_enum(0),
            .int_en = Interrupt_Disabled,
    };
    uint8_t ch_idx = app_dmac_start_wrapper(&dma_param);
    reg->SER = cs_sel_mask;
    app_dmac_transfer_wait_wrapper(ch_idx);
    reg->SSIENR = SSI_Disabled;
    reg->SER = 0;
    qspi_universal_func.sys_stat_func(inst,QSPI_OP_DONE);
    qspi_universal_func.clk_gate_func(inst,CLR_CLK);
}


void app_qspi_multi_read(periph_inst_handle_t hdl,uint8_t cs_sel_mask,qspi_multi_read_param_t *param)
{
    app_qspi_inst_t *inst = CONTAINER_OF(hdl, app_qspi_inst_t, inst);
    reg_ssi_t *reg = inst->reg;
    BX_ASSERT(param->length < 0x40000);
    uint32_t length_in_word = param->length / sizeof(uint32_t);
    uint8_t remainder = param->length % sizeof(uint32_t);
    qspi_universal_func.clk_gate_func(inst,SET_CLK);
    qspi_universal_func.sys_stat_func(inst,QSPI_OP_START);
    inst->reg->SPI_CTRLR0 = FIELD_BUILD(SSI_WAIT_CYCLES,param->wait_cycles) | FIELD_BUILD(SSI_INST_L, param->inst_l)|
                                FIELD_BUILD(SSI_ADDR_L,Addr_Width_32_bits)|FIELD_BUILD(SSI_TRANS_TYPE, param->trans_type);
    if(length_in_word)
    {
        reg->CTRLR0 = FIELD_BUILD(SSI_SPI_FRF,param->dual_quad) | FIELD_BUILD(SSI_DFS,DFS_32_32_bits) | FIELD_BUILD(SSI_TMOD, Receive_Only) |
            FIELD_BUILD(SSI_SCPOL,Inactive_Low)|FIELD_BUILD(SSI_SCPH, SCLK_Toggle_In_Middle) | FIELD_BUILD(SSI_FRF, Motorola_SPI);
        reg->CTRLR1 = length_in_word - 1;
        reg->DMARDLR = 7;
        reg->DMACR = 0;
        reg->DMACR = FIELD_BUILD(SSI_RDMAE , Receive_DMA_Enabled);
        reg->SSIENR = SSI_Enabled;
        reg->DR = param->cmd;
        reg->DR = ADDR_EXTEND(param->addr_field.addr,param->addr_field.extend_byte);
        app_dmac_transfer_param_t dma_param = {
            .src = (uint8_t *)&reg->DR_REVERSED,
            .dst = param->data,
            .length = length_in_word,
            .src_tr_width = Transfer_Width_32_bits,
            .dst_tr_width = Transfer_Width_8_bits,
            .src_msize = Burst_Transaction_Length_8,
            .dst_msize = Burst_Transaction_Length_8,
            .tt_fc = Peripheral_to_Memory_DMAC_Flow_Controller,
            .src_per = dmac_qspi_rx_handshake_enum(0),
            .dst_per = dmac_qspi_tx_handshake_enum(0),
            .int_en = Interrupt_Disabled,
        };
        uint8_t ch_idx = app_dmac_start_wrapper(&dma_param);
        reg->SER = cs_sel_mask;
        app_dmac_transfer_wait_wrapper(ch_idx);
        reg->SSIENR = SSI_Disabled;
        reg->SER = 0;
    }
    if(remainder)
    {
        reg->CTRLR0 = FIELD_BUILD(SSI_SPI_FRF,param->dual_quad) | FIELD_BUILD(SSI_DFS,DFS_32_8_bits) | FIELD_BUILD(SSI_TMOD, Receive_Only) |
            FIELD_BUILD(SSI_SCPOL,Inactive_Low)|FIELD_BUILD(SSI_SCPH, SCLK_Toggle_In_Middle) | FIELD_BUILD(SSI_FRF, Motorola_SPI);
        reg->CTRLR1 = remainder - 1;
        reg->DMACR = 0;
        reg->SSIENR = SSI_Enabled;
        reg->DR = param->cmd;
        reg->DR = ADDR_EXTEND(param->addr_field.addr + length_in_word*sizeof(uint32_t),param->addr_field.extend_byte);
        reg->SER = cs_sel_mask;
        while(FIELD_RD(reg,SR, SSI_TFE)!=Transmit_FIFO_Empty);
        while(FIELD_RD(reg,SR, SSI_BUSY)==SSI_Busy);
        uint8_t i;
        uint8_t *remainder_ptr = &param->data[length_in_word*sizeof(uint32_t)];
        for(i=0;i<remainder;i++)
        {
            remainder_ptr[i] = reg->DR;
        }
        reg->SSIENR = SSI_Disabled;
        reg->SER = 0;
    }
    qspi_universal_func.sys_stat_func(inst,QSPI_OP_DONE);
    qspi_universal_func.clk_gate_func(inst,CLR_CLK);

}

void app_qspi_config_for_cache(periph_inst_handle_t hdl,uint8_t cs_sel_mask,qspi_multi_read_param_t *param)
{
    app_qspi_inst_t *inst = CONTAINER_OF(hdl, app_qspi_inst_t, inst);
    reg_ssi_t *reg = inst->reg;
    qspi_universal_func.clk_gate_func(inst,SET_CLK);
    reg->SPI_CTRLR0 = FIELD_BUILD(SSI_WAIT_CYCLES,param->wait_cycles) | FIELD_BUILD(SSI_INST_L, param->inst_l)|
                                FIELD_BUILD(SSI_ADDR_L,Addr_Width_24_bits)|FIELD_BUILD(SSI_TRANS_TYPE, param->trans_type);
    reg->CTRLR0 = FIELD_BUILD(SSI_SPI_FRF,param->dual_quad) | FIELD_BUILD(SSI_DFS,DFS_32_32_bits) | FIELD_BUILD(SSI_TMOD, Receive_Only) |
        FIELD_BUILD(SSI_SCPOL,Inactive_Low)|FIELD_BUILD(SSI_SCPH, SCLK_Toggle_In_Middle) | FIELD_BUILD(SSI_FRF, Motorola_SPI);
    reg->CTRLR1 = param->length/sizeof(uint32_t) - 1;
    reg->SER = cs_sel_mask;
    reg->SSIENR = SSI_Enabled;
}

uint32_t app_qspi_dr_reg_addr_for_cache(periph_inst_handle_t hdl)
{
    app_qspi_inst_t *inst = CONTAINER_OF(hdl, app_qspi_inst_t, inst);
    return (uint32_t)&inst->reg->DR;
}

void app_qspi_disable_for_cache(periph_inst_handle_t hdl)
{
    app_qspi_inst_t *inst = CONTAINER_OF(hdl, app_qspi_inst_t, inst);
    reg_ssi_t *reg = inst->reg;
    while(FIELD_RD(reg, SR,SSI_BUSY));
    reg->SSIENR = SSI_Disabled;
    reg->SER = 0;
    qspi_universal_func.clk_gate_func(inst,CLR_CLK);
}
