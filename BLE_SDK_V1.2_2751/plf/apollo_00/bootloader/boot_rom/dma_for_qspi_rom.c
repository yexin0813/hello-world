#include "reg_dmac.h"
#include "dmac_typedef.h"
void dmac_transfer_qspi_init()
{
	dmac_dmacfgreg_l_set(1);
	dmac_ch0_cfg_l_pack(0,HS_Polarity_Active_High,HS_Polarity_Active_High, Hardware_Handshaking,Hardware_Handshaking,0,DMAC_CHANNEL_NUM-1);
	dmac_ch0_cfg_h_pack(QSPI_TX_Handshaking, QSPI_RX_Handshaking, 1 , 0, 0);
}

void dmac_transfer_QSPI_RX(void *src, void *dst, uint32_t length)
{
	dmac_ch0_sar_l_set((uint32_t)src);
	dmac_ch0_dar_l_set((uint32_t)dst);
	dmac_ch0_ctrl_l_pack(Peripheral_to_Memory_DMAC_Flow_Controller,Burst_Transaction_Length_32,Burst_Transaction_Length_32,Address_No_Change,Address_Increment,Transfer_Width_8_bits,Transfer_Width_8_bits,Interrupt_Disabled);
	dmac_ch0_ctrl_h_pack(0,length);
	dmac_chenreg_l_pack(1,1);
	while((dmac_rawblock_l_get()&0x1)==0);
	dmac_clearblock_l_set(1);
}
void dmac_transfer_QSPI_RX_32Bit(void *src, void *dst, uint32_t length)
{
	dmac_ch0_sar_l_set((uint32_t)src);
	dmac_ch0_dar_l_set((uint32_t)dst);
	dmac_ch0_ctrl_l_pack(Peripheral_to_Memory_DMAC_Flow_Controller,Burst_Transaction_Length_32,Burst_Transaction_Length_32,Address_No_Change,Address_Increment,Transfer_Width_32_bits,Transfer_Width_32_bits,Interrupt_Disabled);
	dmac_ch0_ctrl_h_pack(0,length);
	dmac_chenreg_l_pack(1,1);
	while((dmac_rawblock_l_get()&0x1)==0);
	dmac_clearblock_l_set(1);
}

void dmac_transfer_QSPI_TX(void *src, void *dst, uint32_t length)
{
	dmac_ch0_sar_l_set((uint32_t)src);
	dmac_ch0_dar_l_set((uint32_t)dst);
	dmac_ch0_ctrl_l_pack(Memory_to_Peripheral_DMAC_Flow_Controller,Burst_Transaction_Length_32,Burst_Transaction_Length_32,Address_Increment,Address_No_Change,Transfer_Width_8_bits,Transfer_Width_8_bits,Interrupt_Disabled);
	dmac_ch0_ctrl_h_pack(0,length);
	dmac_chenreg_l_pack(1,1);
	while((dmac_rawblock_l_get()&0x1)==0);
	dmac_clearblock_l_set(1);
}
/// @}
