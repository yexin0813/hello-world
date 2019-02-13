/**
 ****************************************************************************************
 *
 * @file uart.c
 *
 * @brief UART driver
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup UART
 * @{
 ****************************************************************************************
 */
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stddef.h>     // standard definition
#include "uart.h"       // uart definition
#include "reg_uart.h"   // uart register
#include "bx2400.h"
#include "arch.h"
//#include "rwip.h"       // SW interface
//#if (PLF_NVDS)
//#include "nvds.h"       // NVDS
//#endif // (PLF_NVDS)
#include "dw_apb_uart_typedef.h"
#include "bx_dbg.h"
#include "log.h"
#include "reg_sysc_per.h"
#include "bx_config.h"

/*
 * DEFINES
 *****************************************************************************************
 */

enum rwip_eif_status
{
    /// EIF status OK
    RWIP_EIF_STATUS_OK,
    /// EIF status KO
    RWIP_EIF_STATUS_ERROR,

#if (BLE_EMB_PRESENT == 0)
    /// External interface detached
    RWIP_EIF_STATUS_DETACHED,
    /// External interface attached
    RWIP_EIF_STATUS_ATTACHED,
#endif // (BLE_EMB_PRESENT == 0)
};
#define BIT_FETCH(val,index)	(((val)>>(index))&0x1)
/*
 * STRUCT DEFINITIONS
 *****************************************************************************************
 */
/* TX and RX channel class holding data used for asynchronous read and write data
 * transactions
 */
/// UART TX RX Channel
struct uart_txrxchannel
{
    /// size
    uint32_t  size;
    /// buffer pointer
    uint8_t  *bufptr;
    /// call back function pointer
    void (*callback) (void*,uint8_t);
    //call back function parameter
    void *dummy;
};

/// UART environment structure
struct uart_env_tag
{
    /// tx channel
    struct uart_txrxchannel tx;
    /// rx channel
    struct uart_txrxchannel rx;
    /// error detect
    bool errordetect;
};


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
/// uart environment structure
static struct uart_env_tag uart_env;

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Serves the receive data available interrupt requests. It clears the requests and
 *        executes the callback function.
 *
 ****************************************************************************************
 */

static void uart_rec_data_avail_isr(void)
{
    void (*callback) (void*,uint8_t) = NULL;
    void *data = NULL;

    while (uart_dr_getf()==Data_Ready)
    {
        // Read the received in the FIFO
        *uart_env.rx.bufptr = uart_rbr_get();
//        LOG(LOG_LVL_INFO,"%d ",*uart_env.rx.bufptr);
        // Update RX parameters
        uart_env.rx.size--;
        uart_env.rx.bufptr++;

        // Check if all expected data have been received
        if (uart_env.rx.size == 0)
        {
            // Reset RX parameters
            uart_env.rx.bufptr = NULL;

            // Disable Rx interrupt
			uart_erbfi_setf(Received_Data_Available_Interrupt_Disabled);
			
            // Retrieve callback pointer
            callback = uart_env.rx.callback;
            data = uart_env.rx.dummy;

            if(callback != NULL)
            {
                // Clear callback pointer
                uart_env.rx.callback = NULL;
                uart_env.rx.dummy = NULL;
                if(uart_env.errordetect==false)
                {// Call handler
                	callback(data, RWIP_EIF_STATUS_OK);
                }else
                {
                	uart_env.errordetect = false;
                	callback(data,RWIP_EIF_STATUS_ERROR);
                }
            }
            else
            {
                BX_ASSERT(0);
            }

            // Exit loop
            break;
        }
    }
}

/**
 ****************************************************************************************
 * @brief Serves the receive data error interrupt requests. It clears the requests and
 *        executes the callback function.
 *
 ****************************************************************************************
 */

static void uart_rec_error_isr(void)
{
    void (*callback) (void*,uint8_t) = NULL;
    void *data = NULL;
    uint8_t LSR = uart_lsr_get();
	if(BIT_FETCH(LSR,1)==Overrun_Error)
	{
		LOG(LOG_LVL_ERROR,"uart overrun\n");
		while(uart_dr_getf()==Data_Ready)
		{
			uart_rbr_get();
		}
        if(callback != NULL)
        {
            // Clear callback pointer
            uart_env.rx.callback = NULL;
            uart_env.rx.dummy = NULL;

            // Call handler
            callback(data, RWIP_EIF_STATUS_ERROR);
        }
	}
	if(BIT_FETCH(LSR,3)==Framing_Error)
	{
		LOG(LOG_LVL_ERROR,"uart framing error\n");
		uart_env.errordetect = true;
	}
}

/**
 ****************************************************************************************
 * @brief Serves the transmit data fill interrupt requests. It clears the requests and
 *        executes the callback function.
 *
 * The callback function is called as soon as the last byte of the provided data is
 * put into the FIFO. The interrupt is disabled at the same time.
 ****************************************************************************************
 */
static void uart_thr_empty_isr(void)
{
    void (*callback) (void*,uint8_t) = NULL;
    void * dummy = NULL;
   	if(uart_env.tx.size == 0)
    {
   		while(uart_thre_getf() == Transmitter_Not_Empty);
    	// Reset TX parameters
      	uart_env.tx.bufptr = NULL;

      	// Disable TX interrupt
		uart_etbei_setf(Transmit_Holding_Register_Empty_Interrupt_Disabled);
			
       	// Retrieve callback pointer
      	callback = uart_env.tx.callback;
      	dummy = uart_env.tx.dummy;

     	if(callback != NULL)
     	{
           	// Clear callback pointer
       		uart_env.tx.callback = NULL;
           	uart_env.tx.dummy = NULL;

         	// Call handler
         	callback(dummy, RWIP_EIF_STATUS_OK);	
     	}
    	else
   		{
          BX_ASSERT(0);
    	}

       	// Exit loop
  		return;
   }
    // Fill TX FIFO until there is no more room inside it
    while (uart_tfnf_getf()==Transmit_FIFO_Not_Full)
    {
        // Put a byte in the FIFO
        uart_thr_set(*uart_env.tx.bufptr);

        // Update TX parameters
        uart_env.tx.size--;
        uart_env.tx.bufptr++;

        if (uart_env.tx.size == 0)
        {
            // Exit loop
            break;
        }
    }
}


void uart_set_baudrate(uint16_t divisor)
{
	uart_dlab_setf(Divisor_Latch_Register_Enabled);	
	uart_dll_set((uint8_t)divisor);
	uart_dlh_set((uint8_t)(divisor>>8));
	uart_dlab_setf(Divisor_Latch_Register_Disabled);
}

void uart_init(uint16_t divisor)
{   
    uart_set_baudrate(divisor);
	uart_pen_setf(Parity_Disabled);
	uart_stop_setf(One_Stop_bit);
	uart_dls_setf(Data_Length_8_bits);
	uart_ier_pack(0,Modem_Status_Interrupt_Disabled,Receiver_Line_Status_Interrupt_Enabled,
		Transmit_Holding_Register_Empty_Interrupt_Disabled,Received_Data_Available_Interrupt_Disabled);
	uart_fcr_pack(RCVR_Trigger_One_Character,TX_Empty_Trigger_FIFO_Empty,0,0,0,FIFO_Enabled);
	uart_env.errordetect = false;
}

void uart_flow_on(void)
{
	uart_rts_setf(RTS_Signal_Low);
}


bool uart_flow_off(void)
{
    bool flow_off = true;
    GLOBAL_INT_DISABLE();
    if(uart_rfne_getf()==Receive_FIFO_Empty && uart_tfe_getf()==Transmit_FIFO_Empty)
    {
    	uart_rts_setf(RTS_Signal_High);
    }else
    {
    	flow_off = false;
    }
    GLOBAL_INT_RESTORE();
    return (flow_off);
}

void uart_finish_transfers(void)
{
	uart_rts_setf(RTS_Signal_High);
	while(uart_tfe_getf()==Transmit_FIFO_Not_Empty);
}

void uart_read(uint8_t *bufptr, uint32_t size, void (*callback) (void*,uint8_t),void* dummy)
{
    // Sanity check
    BX_ASSERT(bufptr != NULL);
    BX_ASSERT(size != 0);
    BX_ASSERT(callback != NULL);

    // Prepare RX parameters
    uart_env.rx.size = size;
    uart_env.rx.bufptr = bufptr;
    uart_env.rx.callback = callback; 
    uart_env.rx.dummy = dummy;

	uart_erbfi_setf(Received_Data_Available_Interrupt_Enabled);
}

void uart_write(uint8_t *bufptr, uint32_t size, void (*callback) (void*,uint8_t), void *dummy)
{
    // Sanity check
    BX_ASSERT(bufptr != NULL);
    BX_ASSERT(size != 0);
    BX_ASSERT(callback != NULL);

    // Prepare TX parameters
    uart_env.tx.size = size;
    uart_env.tx.bufptr = bufptr;
    uart_env.tx.callback = callback; 
    uart_env.tx.dummy = dummy;

 	uart_etbei_setf(Transmit_Holding_Register_Empty_Interrupt_Enabled);
}


void uart_isr(void)
{
	IIR_INTERRUPT_ID_FIELD irq_stat;
   	while(1)
   	{
   		irq_stat = (IIR_INTERRUPT_ID_FIELD)uart_iid_getf();
		if(irq_stat==No_Interrupt_Pending)
			break;
		switch(irq_stat)
		{
		case Receiver_Line_Status_Interrupt:
			uart_rec_error_isr();
			break;
		case Received_Data_Available_Interrupt:
			uart_rec_data_avail_isr();
			break;
		case THR_Empty_Interrupt:
			uart_thr_empty_isr();
			break;
		case Character_Timeout_Interrupt:
			uart_rec_data_avail_isr();
			break;
		default:
			break;
		}
			
   	}
}

/// @} UART
