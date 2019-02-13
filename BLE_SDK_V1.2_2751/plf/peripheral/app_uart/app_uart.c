#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "reg_uart.h"
#include "dw_apb_uart_typedef.h"
#include "field_manipulate.h"
#include "app_uart.h"
#include "bx_dbg.h"
#include "ll.h"
#include "rwip.h"
#include "log.h"
#include "co_utils.h"
#include "app_dmac_wrapper.h"
#define BIT_FETCH(val,index)	(((val)>>(index))&0x1)

extern periph_universal_func_set_t uart_universal_func;

uint16_t uart_get_fifo_depth(app_uart_inst_t *inst);

static void uart_rx_env_clr(app_uart_inst_t *inst)
{
    inst->env.rx.callback = NULL;
    inst->env.rx.dummy = NULL;
    inst->env.rx.bufptr = NULL;
}

static void uart_rx_complete(app_uart_inst_t *inst)
{
    reg_uart_t *reg = inst->reg;
    // Disable Rx interrupt
    FIELD_WR(reg,DLH_IER,UART_ERBFI,Received_Data_Available_Interrupt_Disabled);
    // Retrieve callback pointer
    void (*callback) (void*,uint8_t);
    void *data;
    callback = inst->env.rx.callback;
    data = inst->env.rx.dummy;
    BX_ASSERT(callback);
    uart_universal_func.sys_stat_func(inst,UART_READ_DONE);
    uart_rx_env_clr(inst);
    if(inst->env.errordetect==false)
    {
         // Call handler
        callback(data, RWIP_EIF_STATUS_OK);
    }else
    {
        inst->env.errordetect = false;
        callback(data,RWIP_EIF_STATUS_ERROR);
    }
}

static void uart_rec_data_avail_isr(app_uart_inst_t *inst)
{
    reg_uart_t *reg = inst->reg;
    while (FIELD_RD(reg, LSR, UART_DR)==Data_Ready)
    {
        // Read the received in the FIFO
        *inst->env.rx.bufptr = reg->RBR_THR_DLL;
//        LOG(LOG_LVL_INFO,"%02X ",*inst->env.rx.bufptr);
        // Update RX parameters
        inst->env.rx.size--;
        inst->env.rx.bufptr++;

        // Check if all expected data have been received
        if (inst->env.rx.size == 0)
        {
            uart_rx_complete(inst);
            // Exit loop
            break;
        }
    }
}

static void uart_tx_env_clr(app_uart_inst_t *inst)
{
    inst->env.tx.callback = NULL;
    inst->env.tx.dummy = NULL;
    inst->env.tx.bufptr = NULL;
}

static void uart_tx_complete(app_uart_inst_t *inst)
{
    reg_uart_t *reg = inst->reg;
    while(FIELD_RD(reg,LSR,UART_TEMT) == Transmitter_Not_Empty);
    // Disable TX interrupt
    FIELD_WR(reg,DLH_IER,UART_ETBEI,Transmit_Holding_Register_Empty_Interrupt_Disabled);
    // Retrieve callback pointer
    void (*callback) (void*,uint8_t);
    void * dummy;
    callback = inst->env.tx.callback;
    dummy = inst->env.tx.dummy;
    BX_ASSERT(callback);
    //set uart idle status
    uart_universal_func.sys_stat_func(inst,UART_WRITE_DONE);
    uart_tx_env_clr(inst);
    // Call handler
    callback(dummy, RWIP_EIF_STATUS_OK);
}

static void uart_thr_empty_isr(app_uart_inst_t *inst)
{
    if(inst->env.tx.size)
    {
        reg_uart_t *reg = inst->reg;
        // Fill TX FIFO until there is no more room inside it
        while (FIELD_RD(reg,USR,UART_TFNF)==Transmit_FIFO_Not_Full)
        {
            // Put a byte in the FIFO
            reg->RBR_THR_DLL = *inst->env.tx.bufptr;
            // Update TX parameters
            inst->env.tx.bufptr++;
            inst->env.tx.size--;
            if (inst->env.tx.size == 0)
            {
                // Exit loop
                break;
            }
        }
    }else
    {
        uart_tx_complete(inst);
    }
}

static void uart_rec_error_isr(app_uart_inst_t *inst)
{
    reg_uart_t *reg = inst->reg;
    uint8_t LSR = (uint8_t)reg->LSR;
    if(BIT_FETCH(LSR,1)==Overrun_Error)
    {
        LOG(LOG_LVL_ERROR,"uart overrun\n");
        while(FIELD_RD(reg,LSR,UART_DR)==Data_Ready)
        {
            reg->RBR_THR_DLL;
        }
    }
    if(BIT_FETCH(LSR,3)==Framing_Error)
    {
        LOG(LOG_LVL_ERROR,"uart framing error\n");
        inst->env.errordetect = true;
    }
}

void app_uart_isr(app_uart_inst_t *inst)
{
    reg_uart_t *reg = inst->reg;
    while(1)
    {
        IIR_INTERRUPT_ID_FIELD irq_stat = (IIR_INTERRUPT_ID_FIELD)FIELD_RD(reg,IIR_FCR,UART_IID);
        if(irq_stat==No_Interrupt_Pending)  break;
        switch(irq_stat)
        {
        case Receiver_Line_Status_Interrupt:
            uart_rec_error_isr(inst);
        break;
        case Received_Data_Available_Interrupt:
        case Character_Timeout_Interrupt:
            uart_rec_data_avail_isr(inst);
        break;
        case THR_Empty_Interrupt:
            uart_thr_empty_isr(inst);
        break;
        default:
            LOG(LOG_LVL_WARN,"uart:%d unexpected iid\n",inst->idx);
        break;
        }
    }
}


void uart_set_divisor(reg_uart_t *reg,uint16_t divisor)
{
    FIELD_WR(reg,LCR,UART_DLAB,Divisor_Latch_Register_Enabled);	
    reg->RBR_THR_DLL = (uint8_t)divisor;
    reg->DLH_IER = (uint8_t)(divisor>>8);
    FIELD_WR(reg,LCR,UART_DLAB,Divisor_Latch_Register_Disabled);	
}

void app_uart_init(periph_inst_handle_t hdl)
{
    app_uart_inst_t *inst = CONTAINER_OF(hdl, app_uart_inst_t, inst);
    app_uart_comm_params_t *param = &inst->param;
    uint32_t clk_src = (*(uint32_t *)&param->baud_rate>>10);
    uint16_t divisor = (uint16_t)(*(uint32_t *)&param->baud_rate)&0x3ff;
    uart_universal_func.sw_rst_func(inst);
    uart_universal_func.clk_src_cfg_func(inst,clk_src);
    uart_universal_func.clk_gate_func(inst,SET_CLK);
    uart_universal_func.pin_cfg_func(inst,param->rx_pin_no,UART_RXD_PIN);
    uart_universal_func.pin_cfg_func(inst,param->tx_pin_no,UART_TXD_PIN);
    reg_uart_t *reg = inst->reg;
    if(param->flow_control_en)
    {
        uart_universal_func.pin_cfg_func(inst,param->rts_pin_no,UART_RTS_PIN);
        uart_universal_func.pin_cfg_func(inst,param->cts_pin_no,UART_CTS_PIN);
    }
    if(param->auto_flow_control)
    {
        reg->IIR_FCR = FIELD_BUILD(UART_RCVR,RCVR_Trigger_FIFO_Half_Full)
            |FIELD_BUILD(UART_FIFOE,FIFO_Enabled);
        FIELD_WR(reg, MCR, UART_AFCE,1);
    }else
    {
        reg->IIR_FCR = FIELD_BUILD(UART_RCVR,RCVR_Trigger_One_Character)
            |FIELD_BUILD(UART_FIFOE,FIFO_Enabled);
        FIELD_WR(reg, MCR, UART_AFCE,0);
    }
    if(param->rts_stat)
    {
        app_uart_flow_on(hdl);
    }else
    {
        app_uart_flow_off(hdl);
    }
    reg->LCR = FIELD_BUILD(UART_STOP, One_Stop_bit) | FIELD_BUILD(UART_DLS, Data_Length_8_bits) 
        | FIELD_BUILD(UART_PEN, param->parity_en) | FIELD_BUILD(UART_EPS, param->even_parity);
    FIELD_WR(reg,DLH_IER,UART_ELSI,Receiver_Line_Status_Interrupt_Enabled);
    uart_universal_func.intr_op_func(inst,INTR_CLR);
    uart_universal_func.intr_op_func(inst,INTR_ENABLE);
    uart_set_divisor(reg,divisor);
    uart_universal_func.sys_stat_func(inst,UART_INIT);
}

void app_uart_uninit(periph_inst_handle_t hdl)
{
    app_uart_inst_t *inst = CONTAINER_OF(hdl, app_uart_inst_t, inst);
    uart_universal_func.clk_gate_func(inst,CLR_CLK);
    uart_universal_func.intr_op_func(inst,INTR_DISABLE);
    uart_universal_func.sys_stat_func(inst,UART_UNINIT);
}

static bool uart_transfer_sanity_check(uint8_t *bufptr,uint32_t size,void (*callback)(void *,uint8_t))
{
    if(bufptr&&size&&callback)
    {
        return true;
    }else
    {
        return false;
    }
}

static void uart_read_dma_callback(app_uart_inst_t *inst)
{
    inst->env.dma_en_rx = 0;
    inst->env.rx.size = 0;
    uart_rx_complete(inst);
}

static void app_uart_read_dma(app_uart_inst_t *inst)
{
    uint16_t fifo_depth = uart_get_fifo_depth(inst);
    app_dmac_transfer_param_t param = 
    {
        .callback = (void (*)(void*))uart_read_dma_callback,
        .callback_param = inst,
        .src = (uint8_t *)&inst->reg->RBR_THR_DLL,
        .dst = (uint8_t *)inst->env.rx.bufptr,
        .length = inst->env.rx.size,
        .src_tr_width = Transfer_Width_8_bits,
        .dst_tr_width = Transfer_Width_8_bits,
        .src_msize = dmac_get_burst_transaction_size_enum(inst->param.auto_flow_control ? fifo_depth / 2 : 1),
        .dst_msize = Burst_Transaction_Length_4,
        .tt_fc = Peripheral_to_Memory_DMAC_Flow_Controller,
        .src_per = dmac_uart_rx_handshake_enum(inst->idx),
        .dst_per = 0,
        .int_en = 1,
    };
    inst->env.dma_ch_rx =  app_dmac_start_wrapper(&param);
    inst->env.dma_en_rx = 1;
}

static void app_uart_read_no_dma(app_uart_inst_t *inst)
{
    reg_uart_t *reg = inst->reg;
    FIELD_WR(reg,DLH_IER,UART_ERBFI,Received_Data_Available_Interrupt_Enabled);
}

static bool uart_read_busy(app_uart_inst_t *inst)
{
    return inst->env.rx.bufptr ? true : false;
}

periph_err_t app_uart_read(periph_inst_handle_t hdl,uint8_t *bufptr, uint32_t size, void (*callback) (void*,uint8_t),void* dummy)
{
    if(uart_transfer_sanity_check(bufptr,size,callback)==false)
    {
        return PERIPH_INVALID_PARAM;
    }
    app_uart_inst_t *inst = CONTAINER_OF(hdl, app_uart_inst_t, inst);
    if(uart_read_busy(inst))
    {
        return PERIPH_BUSY;
    }
//    LOG(LOG_LVL_INFO,"%x,%d\n",bufptr,size);
    // Prepare RX parameters
    inst->env.rx.bufptr = bufptr;
    inst->env.rx.size = size;
    inst->env.rx.callback = callback; 
    inst->env.rx.dummy = dummy;
    uart_universal_func.sys_stat_func(inst,UART_READ_START);
    if(inst->param.rx_dma)
    {
        app_uart_read_dma(inst);
    }else
    {
        app_uart_read_no_dma(inst);
    }
    return PERIPH_NO_ERROR;
}

periph_err_t app_uart_read_cancel(periph_inst_handle_t hdl,uint32_t *remaining_length)
{
    app_uart_inst_t *inst = CONTAINER_OF(hdl, app_uart_inst_t, inst);
    if(inst->env.rx.bufptr == NULL)
    {
        return PERIPH_STATE_ERROR;
    }
    reg_uart_t *reg = inst->reg;
    if(REG_FIELD_RD(reg->DLH_IER, UART_ERBFI)==Received_Data_Available_Interrupt_Enabled)
    {
        REG_FIELD_WR(reg->DLH_IER,UART_ERBFI,Received_Data_Available_Interrupt_Disabled);
    }
    if(inst->env.dma_en_rx)
    {
        inst->env.rx.size = app_dmac_transfer_cancel_wrapper(inst->env.dma_ch_rx);
    }
    uart_universal_func.sys_stat_func(inst,UART_READ_CANCEL);
    *remaining_length = inst->env.rx.size;
    inst->env.rx.size = 0;
    uart_rx_env_clr(inst);
    return PERIPH_NO_ERROR;
}

static void uart_write_dma_callback(app_uart_inst_t *inst)
{
    reg_uart_t *reg = inst->reg;
    inst->env.dma_en_tx = 0;
    inst->env.tx.size = 0;
    FIELD_WR(reg,DLH_IER,UART_ETBEI,Transmit_Holding_Register_Empty_Interrupt_Enabled);
}

static void app_uart_write_dma(app_uart_inst_t *inst)
{
    uint16_t fifo_depth = uart_get_fifo_depth(inst);
    app_dmac_transfer_param_t param = 
    {
        .callback = (void (*)(void*))uart_write_dma_callback,
        .callback_param = inst,
        .src = (uint8_t *)inst->env.tx.bufptr,
        .dst = (uint8_t *)&inst->reg->RBR_THR_DLL,
        .length = inst->env.tx.size,
        .src_tr_width = Transfer_Width_8_bits,
        .dst_tr_width = Transfer_Width_8_bits,
        .src_msize = Burst_Transaction_Length_4,
        .dst_msize = Burst_Transaction_Length_4,
        .tt_fc = Memory_to_Peripheral_DMAC_Flow_Controller,
        .src_per = 0,
        .dst_per = dmac_uart_tx_handshake_enum(inst->idx),
        .int_en = 1,
    };
    inst->env.dma_ch_tx = app_dmac_start_wrapper(&param);
    inst->env.dma_en_tx = 1;
}

static void app_uart_write_no_dma(app_uart_inst_t *inst)
{
    reg_uart_t *reg = inst->reg;
    FIELD_WR(reg,DLH_IER,UART_ETBEI,Transmit_Holding_Register_Empty_Interrupt_Enabled);
}

static bool uart_write_busy(app_uart_inst_t *inst)
{
    return inst->env.tx.bufptr ? true : false;
}

periph_err_t app_uart_write(periph_inst_handle_t hdl,uint8_t *bufptr, uint32_t size, void (*callback) (void*,uint8_t),void* dummy)
{
    if(uart_transfer_sanity_check(bufptr,size,callback)==false)
    {
        return PERIPH_INVALID_PARAM;
    }
    app_uart_inst_t *inst = CONTAINER_OF(hdl, app_uart_inst_t, inst);
    if(uart_write_busy(inst))
    {
        return PERIPH_BUSY;
    }
    // Prepare TX parameters
    inst->env.tx.bufptr = bufptr;
    inst->env.tx.size = size;
    inst->env.tx.callback = callback; 
    inst->env.tx.dummy = dummy;
    uart_universal_func.sys_stat_func(inst,UART_WRITE_START);
    if(inst->param.tx_dma)
    {
        app_uart_write_dma(inst);
    }else
    {
        app_uart_write_no_dma(inst);
    }
    return PERIPH_NO_ERROR;
}

void app_uart_flow_on(periph_inst_handle_t hdl)
{
    app_uart_inst_t *inst = CONTAINER_OF(hdl, app_uart_inst_t, inst);
    reg_uart_t *reg = inst->reg;
    inst->param.rts_stat = 1;
    FIELD_WR(reg,MCR,UART_RTS,RTS_Signal_Low);
}

bool app_uart_flow_off(periph_inst_handle_t hdl)
{
    app_uart_inst_t *inst = CONTAINER_OF(hdl, app_uart_inst_t, inst);
    reg_uart_t *reg = inst->reg;
    bool flow_off;
    GLOBAL_INT_DISABLE();
    if(FIELD_RD(reg,USR,UART_RFNE)==Receive_FIFO_Empty && FIELD_RD(reg,USR,UART_TFE)==Transmit_FIFO_Empty)
    {
        inst->param.rts_stat = 0;
        FIELD_WR(reg,MCR,UART_RTS,RTS_Signal_High);
        flow_off = true;
    }else
    {
        flow_off = false;
    }
    GLOBAL_INT_RESTORE();
    return (flow_off);
}

