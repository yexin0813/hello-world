#include "reg_dmac.h"
#include "dmac_typedef.h"
#include "reg_uart1.h"
#include "dw_apb_uart_typedef.h"
#include "FreeRTOS.h"
#include "task.h"
#include "log.h"
#include "hw_ecc.h"
#include "flash.h"
#include "reg_iic1.h"
#include "iic.h"
#include "bx_dbg.h"
#include <string.h>
TaskHandle_t handler_dma_m2m_task;
TaskHandle_t handler_division_task;
TaskHandle_t handler_uart_tx_task;
TaskHandle_t handler_flash_write_task;
TaskHandle_t handler_iic_task;
void osperi_iic0_pinShare_init(void)
{
    // set func_io[7..8] as iic0 scl/sda
    *((uint32_t*)(0x20132000+0x24)) |= 0x180;
    // func_io[7] -> iic0_scl
    // func_io[8] -> iic0_sda
    *((uint32_t*)(0x20132000+0x30)) |= (0x6<<28);
    *((uint32_t*)(0x20132000+0x34)) |= (0x7<<0);
}
void osperi_iic0_Master()
{

    //iic0_pinShare_init();
    osperi_iic0_pinShare_init();

    iic0_Disable();
    iic0_RestartModeEnable();

    iic0_Set7_10Addressing_Master(iic_7_Addressing);
    iic0_Set7_10Addressing_Slave(iic_7_Addressing);

    iic0_SetSpeed(iic_HighSpeedMode);

    iic0_SetMode(iic_Master);

    iic0_SetTarAddr(0xA0>>1);
    iic0_Hs_SetMaddr(0);
    iic0_UnmaskAllIrq();
    iic0_SetRxTl(0);
    iic0_SetTxTl(0);

    iic0_Enable();
    iic0_WriteData(0, 256);
}
void osperi_iic1_pinShare_init(void)
{
    // set func_io[5..6] as iic0 scl/sda
    *((uint32_t*)(0x20132000+0x24)) |= 0x60;
    // func_io[5] -> iic1_scl
    // func_io[6] -> iic1_sda
    *((uint32_t*)(0x20132000+0x30)) |= ((0x8<<20)|(0x9<<24));
    //*((uint32_t*)(0x20132000+0x34)) |= (0x9<<24);
}
void osperi_iic1_Slave()
{
    //iic1_pinShare_init();
    osperi_iic1_pinShare_init();

    iic1_Disable();

    iic1_tx_empty_ctrl_setf(1); // Added by ZM. For infinite TX_EMPTY irq
    iic1_SetSlvAddr(0xA0>>1);// Set slave addr

    iic1_RestartModeEnable();

    iic1_Set7_10Addressing_Master(iic_7_Addressing);
    iic1_Set7_10Addressing_Slave(iic_7_Addressing);

    iic1_SetSpeed(iic_HighSpeedMode);

    iic1_SetMode(iic_Slave);


    //iic1_Hs_SetMaddr(0);
    iic1_UnmaskAllIrq();
    iic1_SetRxTl(0);
    iic1_SetTxTl(0);

    iic1_Enable();
    //iic1_Write(data, sizeof(data));
}
void iic_task(void * params)
{
    
    while(1)
    {
        osperi_iic1_Slave();
        osperi_iic0_Master();
    }
}
static uint8_t flash_data[256];

void flash_write_task(void *params)
{
//	flash_init();

    memset(flash_data,0x55,256);

	while(1)
	{
		LOG(LOG_LVL_INFO,"Flash Write\n");
		flash_program(0,256,flash_data);
	}
}
void dma_m2m_task(void *params)
{
	dmac_dmacfgreg_l_set(1);
	dmac_ch1_ctrl_l_pack(Memory_to_Memory_DMAC_Flow_Controller,Burst_Transaction_Length_32,Burst_Transaction_Length_32,Address_Increment,Address_Increment,Transfer_Width_8_bits,Transfer_Width_8_bits,Interrupt_Enabled);
	dmac_ch1_ctrl_h_pack(0,4095);
	dmac_masktfr_l_pack(2,2);
	while(1)
	{		
	    LOG(LOG_LVL_INFO,"m2m\n");
		dmac_ch1_sar_l_set(0);
		dmac_ch1_dar_l_set(0x12a1d8);
		dmac_chenreg_l_pack(2,2);
		ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
	}
}
void uart1_set_baudrate(uint16_t divisor)
{
	uart1_dlab_setf(Divisor_Latch_Register_Enabled);	
	uart1_dll_set((uint8_t)divisor);
	uart1_dlh_set((uint8_t)(divisor>>8));
	uart1_dlab_setf(Divisor_Latch_Register_Disabled);
}
void uart_tx_task(void *params)
{
    dmac_dmacfgreg_l_set(1);
	uart1_set_baudrate(1);
	uart1_pen_setf(Parity_Disabled);
	uart1_stop_setf(One_Stop_bit);
	uart1_dls_setf(Data_Length_8_bits);
	uart1_ier_pack(0,Modem_Status_Interrupt_Disabled,Receiver_Line_Status_Interrupt_Enabled,
		Transmit_Holding_Register_Empty_Interrupt_Disabled,Received_Data_Available_Interrupt_Disabled);
	uart1_fcr_pack(RCVR_Trigger_One_Character,TX_Empty_Trigger_FIFO_Empty,0,0,0,FIFO_Enabled);
	
	
	dmac_ch2_ctrl_l_pack(Memory_to_Peripheral_DMAC_Flow_Controller,Burst_Transaction_Length_32,Burst_Transaction_Length_32,Address_Increment,Address_No_Change,Transfer_Width_8_bits,Transfer_Width_8_bits,Interrupt_Enabled);
	dmac_ch2_cfg_l_pack(0, HS_Polarity_Active_High, HS_Polarity_Active_High, Hardware_Handshaking, Hardware_Handshaking, 0, 2);
	dmac_ch2_cfg_h_pack(UART1_TX_Handshaking,UART1_RX_Handshaking, 1, 0, 0);
	dmac_ch2_ctrl_h_pack(0,4095);
	dmac_masktfr_l_pack(4,4);
    while(1)
    {
		LOG(LOG_LVL_INFO,"uarttx\n");
		dmac_ch2_sar_l_set(0);
		dmac_ch2_dar_l_set((uint32_t)&hwp_uart1->rbr_thr_dll.rbr.val);
		dmac_chenreg_l_pack(4,4);
		ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
    }

}
void dma_isr()
{
    LOG(LOG_LVL_INFO,"dmaisr\n");
    BaseType_t xHigherPriorityTaskWoken;
	if(dmac_statustfr_l_get()&0x2){
		vTaskNotifyGiveFromISR(handler_dma_m2m_task,&xHigherPriorityTaskWoken);
	}
	if(dmac_statustfr_l_get()&0x4){
		vTaskNotifyGiveFromISR(handler_uart_tx_task,&xHigherPriorityTaskWoken);
	}
	
	dmac_cleartfr_l_set(0xffff);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
ecc_in_t ecc_in = {
            .secret_key = (uint8_t *)0,
            .public_key = {
                (uint8_t *)0,
                (uint8_t *)0
            }
    };
ecc_out_t ecc_out = {
            .key = {
                (uint8_t *)0x12e000,
                (uint8_t *)0x12e200
            }
    };
void ecc_cb(void *params)
{
    SIMU_FINISH();
}
void division_task(void * params)
{
    volatile double x,y,z;
	full_ecc_calc_start(&ecc_in,&ecc_out,ecc_cb,NULL);
	y = 16.35;
	z = 7.76;
    while(1)
    {
        x = y / z;
    }
}
