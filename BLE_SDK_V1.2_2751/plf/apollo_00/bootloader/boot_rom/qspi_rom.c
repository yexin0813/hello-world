#include <stdbool.h>
#include <stddef.h>

#include "dw_apb_ssi_typedef.h"
#include "reg_qspi.h"
#include "dma_for_qspi.h"
#include "bx_dbg.h"

#include "qspi.h"

void qspi_init()
{
	qspi_ssi_en_setf(SSI_Disabled);
	qspi_sckdv_setf(4);
	dmac_transfer_qspi_init();
}

void qspi_read(uint8_t *rd_ptr,uint32_t rd_len,uint8_t *wr_ptr,uint8_t wr_len)
{
	BX_ASSERT(wr_len<=QSPI_TX_FIFO_DEPTH);
	BX_ASSERT(rd_len<=MAX_DMAC_TRANSFER_SIZE);
    qspi_ssi_en_setf(SSI_Disabled);
	qspi_ser_set(0);
	qspi_ctrlr0_pack(Standard_SPI_Format,DFS_32_8_bits,0,0,0,EEPROM_Read,Inactive_Low,SCLK_Toggle_In_Middle,Motorola_SPI);
	qspi_ctrlr1_pack(rd_len-1);
	qspi_ssienr_set(SSI_Enabled);
	uint16_t i;
	for(i=0;i<wr_len;i++)
	{
		qspi_dr_set(*wr_ptr++);
	}
	qspi_ser_set(QSPI_SLAVE_INDEX);
	if(rd_len<=QSPI_RX_FIFO_DEPTH)
	{
		while(qspi_tfe_getf()!=Transmit_FIFO_Empty);
		while(qspi_busy_getf()==SSI_Busy);
		for(i=0;i<rd_len;i++)
		{
			*rd_ptr++=qspi_dr_get();
		}
	}else
	{
		qspi_dmardlr_set(QSPI_DMA_Receive_Data_Level-1);
        qspi_dmacr_pack(Transmit_DMA_Disabled,Receive_DMA_Disabled);
		qspi_dmacr_pack(Transmit_DMA_Disabled,Receive_DMA_Enabled);
		dmac_transfer_QSPI_RX((void*)&(hwp_qspi->dr),rd_ptr,rd_len);
	}
	qspi_ssienr_set(SSI_Disabled);
}

void qspi_flash_read_data(uint32_t *rd_ptr,uint32_t rd_len_in_word,uint8_t instruction,uint32_t addr)
{
	uint16_t cycles = rd_len_in_word / MAX_DMAC_TRANSFER_SIZE;
	uint16_t tailing_length = rd_len_in_word % MAX_DMAC_TRANSFER_SIZE;
    qspi_ssi_en_setf(SSI_Disabled);
	qspi_ser_set(0);
	qspi_ctrlr0_pack(Standard_SPI_Format,DFS_32_32_bits,0,0,0,EEPROM_Read,Inactive_Low,SCLK_Toggle_In_Middle,Motorola_SPI);
	qspi_ctrlr1_pack(rd_len_in_word-1);
	qspi_dmardlr_set(QSPI_DMA_Receive_Data_Level-1);
    qspi_dmacr_pack(Transmit_DMA_Disabled,Receive_DMA_Disabled);
	qspi_dmacr_pack(Transmit_DMA_Disabled,Receive_DMA_Enabled);
	qspi_ssienr_set(SSI_Enabled);
	qspi_dr_set((addr&0x00ffffff)|(instruction<<24));
	qspi_ser_set(QSPI_SLAVE_INDEX);
	uint16_t i;
	for(i=0;i<cycles;i++)
	{
		dmac_transfer_QSPI_RX_32Bit((void*)&(hwp_qspi->dr_reversed),rd_ptr,MAX_DMAC_TRANSFER_SIZE);
		rd_ptr += MAX_DMAC_TRANSFER_SIZE;
	}
	dmac_transfer_QSPI_RX_32Bit((void*)&(hwp_qspi->dr_reversed),rd_ptr,tailing_length);
	qspi_ssienr_set(SSI_Disabled);
	
}

void qspi_write(uint8_t *bufptr,uint32_t length)
{
    BX_ASSERT(length<=MAX_DMAC_TRANSFER_SIZE);
    qspi_ssi_en_setf(SSI_Disabled);
    qspi_ser_set(0);
    qspi_ctrlr0_pack(Standard_SPI_Format,DFS_32_8_bits,0,0,0,Transmit_Only,Inactive_Low,SCLK_Toggle_In_Middle,Motorola_SPI);
    qspi_ssienr_set(SSI_Enabled);
    if(length<=QSPI_TX_FIFO_DEPTH)
    {
        while(length-->0)
        {
            qspi_dr_set(*bufptr++);
        }
        qspi_ser_set(QSPI_SLAVE_INDEX);
    }else
    {
        qspi_dmatdlr_set(QSPI_DMA_Transmit_Data_Level);
        qspi_dmacr_pack(Transmit_DMA_Disabled,Receive_DMA_Disabled);
        qspi_dmacr_pack(Transmit_DMA_Enabled,Receive_DMA_Disabled);
        qspi_ser_set(QSPI_SLAVE_INDEX);
        dmac_transfer_QSPI_TX(bufptr,(void*)&(hwp_qspi->dr),length);
    }
    while(qspi_tfe_getf()!=Transmit_FIFO_Empty);
    while(qspi_busy_getf()==SSI_Busy);
    qspi_ssienr_set(SSI_Disabled);
}
/// @}
