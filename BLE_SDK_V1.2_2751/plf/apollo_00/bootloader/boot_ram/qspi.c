#include <stdbool.h>
#include <stddef.h>
#include "dma_for_qspi.h"
#include "reg_ssi.h"
#include "_reg_base_addr.h"
#include "dw_apb_ssi_typedef.h"
#include "field_manipulate.h"
#include "spi_flash_cmd.h"
#include "qspi.h"
#define ADDR_EXTEND(addr,lsb) ((addr)<<8|(lsb))

#define QSPI_REG ((reg_ssi_t *)REG_QSPI_BASE)
#define QSPI_FIFO_DEPTH         64
#define QSPI_SLAVE_INDEX    1
#define QSPI_MULTI_READ_WAIT_CYCLES 8  

void qspi_init(uint16_t clk_div,uint8_t rxsample_dly)
{
    QSPI_REG->SSIENR = SSI_Disabled;
    QSPI_REG->BAUDR = clk_div;
    QSPI_REG->RXSAMPLE_DLY = rxsample_dly;
    QSPI_REG->SER = 0;
    
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
            .src_per = Dmac_Qspi_Rx,
            .dst_per = Dmac_Qspi_Tx,
    };
    uint32_t remaining_length = dmac_start(&dma_param,0);
    reg->SER = cs_sel_mask;
    dmac_transfer_wait(remaining_length,0);
    while(FIELD_RD(reg,SR,SSI_TFE) == Transmit_FIFO_Not_Empty);
    while(FIELD_RD(reg,SR,SSI_BUSY) == SSI_Busy);
    reg->SSIENR = SSI_Disabled;
    reg->SER = 0;
}

void qspi_std_write(uint8_t *data,uint32_t length)
{
    qspi_std_byte_write_dma_config(QSPI_REG,QSPI_FIFO_DEPTH);
    qspi_std_byte_write_dma_start(QSPI_REG,QSPI_SLAVE_INDEX,data,length);
}

void qspi_flash_program(uint8_t cmd,uint32_t addr,uint8_t *data,uint32_t length)
{
    qspi_std_byte_write_dma_config(QSPI_REG,QSPI_FIFO_DEPTH);
    QSPI_REG->DR = cmd;
    QSPI_REG->DR = addr>>16 & 0xff;
    QSPI_REG->DR = addr>>8 & 0xff;
    QSPI_REG->DR = addr & 0xff;
    qspi_std_byte_write_dma_start(QSPI_REG,QSPI_SLAVE_INDEX,data,length);
}


void qspi_std_read(uint8_t *cmd_buf,uint8_t cmd_len,uint8_t *data_buf,uint32_t data_len)
{
    QSPI_REG->CTRLR0 = FIELD_BUILD(SSI_SPI_FRF,Standard_SPI_Format) | FIELD_BUILD(SSI_DFS,DFS_32_8_bits) | FIELD_BUILD(SSI_TMOD, EEPROM_Read) |
        FIELD_BUILD(SSI_SCPOL,Inactive_Low)|FIELD_BUILD(SSI_SCPH, SCLK_Toggle_In_Middle) | FIELD_BUILD(SSI_FRF, Motorola_SPI);
    QSPI_REG->CTRLR1 = data_len - 1;
    QSPI_REG->DMARDLR = 7;
    QSPI_REG->DMACR = 0;
    QSPI_REG->DMACR= FIELD_BUILD(SSI_RDMAE , Receive_DMA_Enabled);
    QSPI_REG->SSIENR = SSI_Enabled;
    uint8_t i;
    for(i=0;i<cmd_len;++i)
    {
        QSPI_REG->DR = cmd_buf[i];
    }
    app_dmac_transfer_param_t dma_param = {
            .src = (uint8_t *)&QSPI_REG->DR,
            .dst = data_buf,
            .length = data_len,
            .src_tr_width = Transfer_Width_8_bits,
            .dst_tr_width = Transfer_Width_8_bits,
            .src_msize = Burst_Transaction_Length_8,
            .dst_msize = Burst_Transaction_Length_8,
            .tt_fc = Peripheral_to_Memory_DMAC_Flow_Controller,
            .src_per = Dmac_Qspi_Rx,
            .dst_per = Dmac_Qspi_Tx,
    };
    uint32_t remaining_length = dmac_start(&dma_param,0);
    QSPI_REG->SER = QSPI_SLAVE_INDEX;
    dmac_transfer_wait(remaining_length,0);
    QSPI_REG->SSIENR = SSI_Disabled;
    QSPI_REG->SER = 0;
}

void qspi_multi_read(qspi_multi_read_param_t *param)
{
    uint32_t length_in_word = param->length / sizeof(uint32_t);
    uint8_t remainder = param->length % sizeof(uint32_t);
    QSPI_REG->SPI_CTRLR0 = FIELD_BUILD(SSI_WAIT_CYCLES,param->wait_cycles) | FIELD_BUILD(SSI_INST_L, param->inst_l)|
                                FIELD_BUILD(SSI_ADDR_L,Addr_Width_32_bits)|FIELD_BUILD(SSI_TRANS_TYPE, param->trans_type);
    if(length_in_word)
    {
        QSPI_REG->CTRLR0 = FIELD_BUILD(SSI_SPI_FRF,param->dual_quad) | FIELD_BUILD(SSI_DFS,DFS_32_32_bits) | FIELD_BUILD(SSI_TMOD, Receive_Only) |
            FIELD_BUILD(SSI_SCPOL,Inactive_Low)|FIELD_BUILD(SSI_SCPH, SCLK_Toggle_In_Middle) | FIELD_BUILD(SSI_FRF, Motorola_SPI);
        QSPI_REG->CTRLR1 = length_in_word - 1;
        QSPI_REG->DMARDLR = 7;
        QSPI_REG->DMACR = 0;
        QSPI_REG->DMACR = FIELD_BUILD(SSI_RDMAE , Receive_DMA_Enabled);
        QSPI_REG->SSIENR = SSI_Enabled;
        QSPI_REG->DR = param->cmd;
        QSPI_REG->DR = ADDR_EXTEND(param->addr_field.addr,param->addr_field.extend_byte);
        app_dmac_transfer_param_t dma_param = {
            .src = (uint8_t *)&QSPI_REG->DR_REVERSED,
            .dst = param->data,
            .length = length_in_word,
            .src_tr_width = Transfer_Width_32_bits,
            .dst_tr_width = Transfer_Width_8_bits,
            .src_msize = Burst_Transaction_Length_8,
            .dst_msize = Burst_Transaction_Length_8,
            .tt_fc = Peripheral_to_Memory_DMAC_Flow_Controller,
            .src_per = Dmac_Qspi_Rx,
            .dst_per = Dmac_Qspi_Tx,
        };
        uint32_t remaining_length = dmac_start(&dma_param,0);
        QSPI_REG->SER = QSPI_SLAVE_INDEX;
        dmac_transfer_wait(remaining_length,0);
        QSPI_REG->SSIENR = SSI_Disabled;
        QSPI_REG->SER = 0;
    }
    if(remainder)
    {
        QSPI_REG->CTRLR0 = FIELD_BUILD(SSI_SPI_FRF,param->dual_quad) | FIELD_BUILD(SSI_DFS,DFS_32_8_bits) | FIELD_BUILD(SSI_TMOD, Receive_Only) |
            FIELD_BUILD(SSI_SCPOL,Inactive_Low)|FIELD_BUILD(SSI_SCPH, SCLK_Toggle_In_Middle) | FIELD_BUILD(SSI_FRF, Motorola_SPI);
        QSPI_REG->CTRLR1 = remainder - 1;
        QSPI_REG->DMACR = 0;
        QSPI_REG->SSIENR = SSI_Enabled;
        QSPI_REG->DR = param->cmd;
        QSPI_REG->DR = ADDR_EXTEND(param->addr_field.addr + length_in_word*sizeof(uint32_t),param->addr_field.extend_byte);
        QSPI_REG->SER = QSPI_SLAVE_INDEX;
        while(FIELD_RD(QSPI_REG,SR, SSI_TFE)!=Transmit_FIFO_Empty);
        while(FIELD_RD(QSPI_REG,SR, SSI_BUSY)==SSI_Busy);
        uint8_t i;
        uint8_t *remainder_ptr = &param->data[length_in_word*sizeof(uint32_t)];
        for(i=0;i<remainder;i++)
        {
            remainder_ptr[i] = QSPI_REG->DR;
        }
        QSPI_REG->SSIENR = SSI_Disabled;
        QSPI_REG->SER = 0;
    }
}


