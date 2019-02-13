#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "reg_iic.h"
#include "field_manipulate.h"
#include "app_iic.h"
#include "bx_dbg.h"
#include "ll.h"
#include "rwip.h"
#include "log.h"
#include "co_utils.h"
#include "app_dmac_wrapper.h"
#include "string.h"


extern periph_universal_func_set_t iic_universal_func;



//set
#define app_iic_set_enable(reg,enable)              FIELD_WR(reg , IC_ENABLE , IIC_IC_EN               , enable)
#define app_iic_set_restart_en(reg,enable)          FIELD_WR(reg , IC_CON    , IIC_IC_RESTART_EN       , enable)
#define app_iic_set_addr_bit_master(reg,val)        FIELD_WR(reg , IC_CON    , IIC_IC_10BITADDR_MASTER , val)
#define app_iic_set_addr_bit_slave(reg,val)         FIELD_WR(reg , IC_CON    , IIC_IC_10BITADDR_SLAVE  , val)
#define app_iic_set_slave_addr(reg,val)             FIELD_WR(reg , IC_SAR    , IIC_SAR                 , val)
#define app_iic_set_rxtl(reg,val)                   FIELD_WR(reg , IC_RX_TL  , IIC_RX_TL               , val)
#define app_iic_set_txtl(reg,val)                   FIELD_WR(reg , IC_TX_TL  , IIC_TX_TL               , val)
#define app_iic_set_tar(reg,val)                    FIELD_WR(reg , IC_TAR    , IIC_TAR                 , val)
#define app_iic_set_int_unmask_tx_empty(reg)        FIELD_WR(reg , IC_INTR_MASK , IIC_M_TX_EMPTY       , 1  )
#define app_iic_set_int_mask_tx_empty(reg)          FIELD_WR(reg , IC_INTR_MASK , IIC_M_TX_EMPTY       , 0  )
//get
#define app_iic_get_txflr(reg)                      FIELD_RD(reg , IC_TXFLR    , IIC_TXFLR)
#define app_iic_get_rxflr(reg)                      FIELD_RD(reg , IC_RXFLR    , IIC_RXFLR)
#define app_iic_get_tfnf(reg)                       FIELD_RD(reg , IC_STATUS   , IIC_TFNF )
#define app_iic_get_dat(reg)                        FIELD_RD(reg , IC_DATA_CMD , IIC_DAT  )

//regisiter set function
static __inline void app_iic_set_data_cmd_pack(reg_iic_t *reg,uint8_t restart, uint8_t stop, uint8_t cmd, uint8_t dat)
{
    reg->IC_DATA_CMD = (((uint32_t)restart << 10) | ((uint32_t)stop << 9) | ((uint32_t)cmd << 8) | ((uint32_t)dat << 0));
}
void app_iic_send_addr_cmd(reg_iic_t *reg , uint8_t reg_addr, uint8_t rw)
{
    while(app_iic_get_txflr(reg) >= APP_IIC_RX_TX_FIFO_DEPTH); // tx fifo not full
    app_iic_set_data_cmd_pack(reg,0,0,rw,reg_addr);
}
static void iic_set_target_address(reg_iic_t *reg , uint32_t addr)
{
    app_iic_set_enable(reg,0);
    app_iic_set_tar(reg,addr);
    app_iic_set_enable(reg,1);
}


static void iic_rx_complete(app_iic_inst_t *inst)
{
    void (*callback) (void*,uint8_t) = NULL;
    void * dummy = NULL;
    reg_iic_t *reg = inst->reg;
    // Disable TX interrupt
    app_iic_set_int_mask_tx_empty(reg);
    // Retrieve callback pointer
    callback = inst->env.rx.callback;
    dummy = inst->env.rx.dummy;
    BX_ASSERT(callback);
    // Clear callback pointer
    inst->env.rx.callback = NULL;
    inst->env.rx.dummy = NULL;
    //set iic idle status
    iic_universal_func.sys_stat_func(inst,IIC_READ_DONE);
    // Call handler
    callback(dummy, RWIP_EIF_STATUS_OK);
}
static void iic_tx_complete(app_iic_inst_t *inst)
{
    void (*callback) (void*,uint8_t) = NULL;
    void * dummy = NULL;
    reg_iic_t *reg = inst->reg;
    // Disable TX interrupt
    app_iic_set_int_mask_tx_empty(reg);
    // Retrieve callback pointer
    callback = inst->env.tx.callback;
    dummy = inst->env.tx.dummy;
    BX_ASSERT(callback);
    // Clear callback pointer
    inst->env.tx.callback = NULL;
    inst->env.tx.dummy = NULL;
    //set iic idle status
    iic_universal_func.sys_stat_func(inst,IIC_WRITE_DONE);
    // Call handler
    callback(dummy, RWIP_EIF_STATUS_OK);
}




static void iic_rd_req_isr(app_iic_inst_t *inst)
{
    app_iic_set_int_unmask_tx_empty(inst->reg);
}
static void iic_rx_done_isr(app_iic_inst_t *inst)
{
    //no need
    //iic_tx_complete(inst);
    //LOG(LOG_LVL_WARN , "iic_rx_done_isr\n");
}
static void iic_tx_abort_isr(app_iic_inst_t *inst)
{
    //iic_tx_complete(inst);
    //LOG(LOG_LVL_WARN , "iic_tx_abort_isr\n");
}

static void iic_rx_full_isr(app_iic_inst_t *inst)
{
    reg_iic_t *reg = inst->reg;
    //read received data
    while(app_iic_get_rxflr(reg) > 0)
    {
        //read current
        *inst->env.rx.bufptr = app_iic_get_dat(reg);
        inst->env.rx.bufptr ++;
        if(inst->env.rx.remain > 0) inst->env.rx.remain --;
        //callback
        if(inst->env.rx.remain == 0)
        {
            iic_rx_complete(inst);
            return;
        }
    }
    //tramsmit remain data ,generate clocks
    uint8_t rx_fifo_depth = (inst->env.rx.remain > APP_IIC_RX_TX_FIFO_DEPTH) ? APP_IIC_RX_TX_FIFO_DEPTH / 2 : inst->env.rx.remain-1;
    app_iic_set_rxtl(reg,rx_fifo_depth);

}
static void iic_tx_empty_isr(app_iic_inst_t *inst)
{
    reg_iic_t *reg = inst->reg;
    if(inst->env.tx.remain == 0)
    {
        app_iic_set_int_mask_tx_empty(reg);
        //judge send or read
        if(inst->env.rw_mode == IIC_MODE_WRITE)
        {
            iic_tx_complete(inst);
        }
        return;
    }
    while(app_iic_get_tfnf(reg) == 1)// tx fifo not full
    {
        uint8_t rw,stop_bit;
        rw = (inst->env.rw_mode == IIC_MODE_READ) ? 1 : 0 ;
        stop_bit = (inst->env.tx.remain == 1) ? 1 : 0;
        //write one byte
        app_iic_set_data_cmd_pack(reg,0,stop_bit,rw,*inst->env.tx.bufptr);
        //increase buffer ptr
        inst->env.tx.bufptr ++;
        inst->env.tx.remain --;
        if(inst->env.tx.remain == 0) return;
    }
}

static void iic_clear_all_irq(reg_iic_t *reg , uint32_t irq_stat)
{
    volatile uint32_t temp_clear_irq_val;
    if(irq_stat & (1 << IIC_INTR_RX_UNDER   ))   temp_clear_irq_val = reg->IC_CLR_RX_UNDER ;
    if(irq_stat & (1 << IIC_INTR_RX_OVER    ))   temp_clear_irq_val = reg->IC_CLR_RX_OVER  ;
    if(irq_stat & (1 << IIC_INTR_TX_OVER    ))   temp_clear_irq_val = reg->IC_CLR_TX_OVER  ;
    if(irq_stat & (1 << IIC_INTR_RD_REQ     ))   temp_clear_irq_val = reg->IC_CLR_RD_REQ   ;
    if(irq_stat & (1 << IIC_INTR_TX_ABORT   ))   temp_clear_irq_val = reg->IC_CLR_TX_ABRT  ;
    if(irq_stat & (1 << IIC_INTR_RX_DONE    ))   temp_clear_irq_val = reg->IC_CLR_RX_DONE  ;
    if(irq_stat & (1 << IIC_INTR_ACTIVITY   ))   temp_clear_irq_val = reg->IC_CLR_ACTIVITY ;
    if(irq_stat & (1 << IIC_INTR_STOP_DET   ))   temp_clear_irq_val = reg->IC_CLR_STOP_DET ;
    if(irq_stat & (1 << IIC_INTR_START_DET  ))   temp_clear_irq_val = reg->IC_CLR_START_DET;
    if(irq_stat & (1 << IIC_INTR_GEN_CALL   ))   temp_clear_irq_val = reg->IC_CLR_GEN_CALL ;
}

void app_iic_isr(app_iic_inst_t *inst)
{
    reg_iic_t *reg = inst->reg;
    uint32_t irq_stat = reg->IC_INTR_STAT;
    iic_clear_all_irq(reg , irq_stat);
    //IRQ handler
    if(irq_stat & (1<<IIC_INTR_RX_FULL )) {iic_rx_full_isr (inst); irq_stat &= ~(1<<IIC_INTR_RX_FULL );}
    if(irq_stat & (1<<IIC_INTR_TX_EMPTY)) {iic_tx_empty_isr(inst); irq_stat &= ~(1<<IIC_INTR_TX_EMPTY);}
    if(irq_stat & (1<<IIC_INTR_RD_REQ  )) {iic_rd_req_isr  (inst); irq_stat &= ~(1<<IIC_INTR_RD_REQ  );}
    if(irq_stat & (1<<IIC_INTR_TX_ABORT)) {iic_tx_abort_isr(inst); irq_stat &= ~(1<<IIC_INTR_TX_ABORT);}
    if(irq_stat & (1<<IIC_INTR_RX_DONE )) {iic_rx_done_isr (inst); irq_stat &= ~(1<<IIC_INTR_RX_DONE );}
    //unexpected irq
    if(irq_stat)
    {
        LOG(LOG_LVL_WARN , "iic%d:unexpected irq,stat=0x%x\n" , inst->idx , irq_stat);
    }
}




static void app_iic_set_work_mode(reg_iic_t *reg , uint8_t work_mode)
{
    if(work_mode == IIC_SLAVE) //slave
    {
        GLOBAL_INT_DISABLE();
        FIELD_WR(reg , IC_CON    , IIC_IC_SLAVE_DISABLE , 0);
        FIELD_WR(reg , IC_CON    , IIC_MASTER_MODE      , 0);
        GLOBAL_INT_RESTORE(); 
    }
    else // master
    {
        GLOBAL_INT_DISABLE();
        FIELD_WR(reg , IC_CON    , IIC_IC_SLAVE_DISABLE , 1);
        FIELD_WR(reg , IC_CON    , IIC_MASTER_MODE      , 1);
        GLOBAL_INT_RESTORE(); 
    }
}

void app_iic_init(periph_inst_handle_t hdl)
{
    //get parameter
    app_iic_inst_t *inst = CONTAINER_OF(hdl, app_iic_inst_t, inst);
    app_iic_comm_params_t *param = &inst->param;
    reg_iic_t *reg = inst->reg;
    
    //set system config
    iic_universal_func.sw_rst_func(inst);                           //software reset
    iic_universal_func.pin_cfg_func(inst,param->scl_pin,IIC_SCL_PIN); 
    iic_universal_func.pin_cfg_func(inst,param->sda_pin,IIC_SDA_PIN);
    
    //set parameter
    app_iic_set_enable(reg , 0);                                    //disable
    iic_universal_func.clk_src_cfg_func(inst , param->speed_mode);  //set speed 
    iic_universal_func.clk_gate_func(inst,SET_CLK);                 //set clock gate
    app_iic_set_restart_en(reg , 1);                                //set restart en
    app_iic_set_addr_bit_master(reg,param->dev_addr_bit_num);       //set master address bit number
    app_iic_set_addr_bit_slave(reg,param->dev_addr_bit_num);        //set slave address bit number
    app_iic_set_work_mode(reg , param->work_mode);                  //set work mode
    if(param->work_mode == IIC_SLAVE)                               //set slave address
    {
        app_iic_set_slave_addr(reg,param->slave_address >> 1);
    }
    
    //set enable
    iic_universal_func.intr_op_func(inst,INTR_CLR);                            //clear  interrupt
    iic_universal_func.intr_op_func(inst,INTR_ENABLE);                         //enable interrupt
    reg->IC_INTR_MASK = 0xef;                                                  //unmask all IRQ
    if(param->use_dma)
    {
        reg->IC_DMA_CR    =  3;     //set dma parameter
        reg->IC_DMA_TDLR  = 15;     //
        reg->IC_DMA_RDLR  =  0;     //
        reg->IC_INTR_MASK =  0;     //mask all iic IRQ
    }
    app_iic_set_enable(reg , 1);                            //enable
    iic_universal_func.sys_stat_func(inst,IIC_INIT);        //set state
    iic_universal_func.clk_gate_func(inst,CLR_CLK);         //clear clock gate
}

void app_iic_uninit(periph_inst_handle_t hdl)
{
    app_iic_inst_t *inst = CONTAINER_OF(hdl, app_iic_inst_t, inst);
    reg_iic_t *reg = inst->reg;
    
    app_iic_set_enable(reg , 0);                                   //disable
    iic_universal_func.clk_gate_func(inst,CLR_CLK);                //
    iic_universal_func.intr_op_func (inst,INTR_DISABLE);           //
    iic_universal_func.sys_stat_func(inst,IIC_UNINIT);             //
}



void iic_transfer_sanity_check(uint8_t *bufptr,uint32_t size,void (*callback)(void *,uint8_t))
{
    // Sanity check
    BX_ASSERT(bufptr != NULL);
    BX_ASSERT(size != 0);
    BX_ASSERT(callback != NULL);
}


static void iic_read_dma_send_clock_callback(app_iic_inst_t *inst)
{
    //NOTHING TO DO
}

static void iic_read_dma_callback(app_iic_inst_t *inst)
{
    iic_rx_complete(inst);
}


static void app_iic_read_dma(app_iic_inst_t *inst,uint8_t *bufptr, uint32_t size , uint16_t device_address, uint16_t mem_address)
{
    reg_iic_t *reg = inst->reg;
    uint32_t *p_rx_pointer = inst->env.rx.dma_buffer;
    memset((void*)inst->env.rx.dma_buffer,0,sizeof(inst->env.rx.dma_buffer));
    
    //--------------------------Read data DMA--------------------------------//
    app_dmac_transfer_param_t param = 
    {
        .callback = (void (*)(void*))iic_read_dma_callback,
        .callback_param = inst,
        .src = (uint8_t *)&reg->IC_DATA_CMD,
        .dst = (uint8_t *)bufptr,
        .length = size,
        .src_tr_width = Transfer_Width_8_bits,
        .dst_tr_width = Transfer_Width_8_bits,
        .src_msize = Burst_Transaction_Length_1,
        .dst_msize = Burst_Transaction_Length_1,
        .tt_fc = Peripheral_to_Memory_DMAC_Flow_Controller,
        .src_per = dmac_iic_rx_handshake_enum(inst->idx),
        .dst_per = 0,
        .int_en = 1,
    };
    app_dmac_start_wrapper(&param);
    
    //--------------------------send clock DMA----------------------------------//
    param.src_tr_width = Transfer_Width_32_bits;
    param.dst_tr_width = Transfer_Width_32_bits;
    param.src = (uint8_t *)inst->env.rx.dma_buffer;//anything
    param.dst = (uint8_t *)&reg->IC_DATA_CMD;
    param.callback = (void (*)(void*))iic_read_dma_send_clock_callback;
    param.tt_fc = Memory_to_Peripheral_DMAC_Flow_Controller;
    param.dst_per = 0;
    param.dst_per = dmac_iic_tx_handshake_enum(inst->idx);
    //memory address generation
    if(inst->param.mem_addr_bit_num == IIC_16BIT_MEMORY_ADDRESS)
    {
    	param.length = (size+2);// 16*4 bytes data written to EEPROM, and 1 write cmd. Write reg for 17 times.
    	*p_rx_pointer = (mem_address >> 8  );  p_rx_pointer ++;// restart = 0, stop = 0, cmd = write, data(addr) = 0
    	*p_rx_pointer = (mem_address & 0xFF);  p_rx_pointer ++;
        //Restart command
        *p_rx_pointer = (1 << 10)|(0 << 9)|(1 << 8)|device_address;// restart = 1, stop = 0, cmd = read, data(addr) = 0xA0
        p_rx_pointer++;
    }
    if(inst->param.mem_addr_bit_num == IIC_8BIT_MEMORY_ADDRESS)
	{
		param.length = (size+1);//new dma driver is no need *4
		*p_rx_pointer = (mem_address & 0xFF); p_rx_pointer ++;
	    //Restart command
	    *p_rx_pointer = (1 << 10)|(0 << 9)|(1 << 8)|device_address;// restart = 1, stop = 0, cmd = read, data(addr) = 0xA0
	    p_rx_pointer++;
	}
    if(inst->param.mem_addr_bit_num == IIC_NO_MEMORY_ADDRESS)
	{
		param.length = (size);
	}

    //data receive
    for(uint8_t cnt = 0 ; cnt < (size-1) ; cnt ++) //need to reduce one byte (send data will auto send device address)
    {
        uint8_t stop_bits = (cnt == (size - 2)) ? 1 : 0;
    	*p_rx_pointer = (stop_bits<<9) | (1 << 8) | *bufptr;// restart = 0, stop = ?, cmd = write, 
    	p_rx_pointer++;
    	bufptr++;
    }
    //output pulse
    iic_set_target_address(reg , device_address >> 1);
    app_dmac_start_wrapper(&param);
}

static void app_iic_read_no_dma(app_iic_inst_t *inst,uint8_t *bufptr, uint32_t size,uint16_t device_address, uint16_t mem_address)
{
    //TODO
    reg_iic_t *reg = inst->reg;
    if(inst->param.work_mode == IIC_MASTER)
    {
        iic_set_target_address(reg,device_address >> 1);//slave don't need set target addr.
        //send head data
        if(inst->param.mem_addr_bit_num == IIC_16BIT_MEMORY_ADDRESS) app_iic_send_addr_cmd(reg , mem_address >> 8  , 0);
        if(inst->param.mem_addr_bit_num != IIC_NO_MEMORY_ADDRESS)    app_iic_send_addr_cmd(reg , mem_address & 0xFF, 0);
        //restart = 1, r/w = 1, dat = 0xA0, stop bit=0
        if(inst->param.mem_addr_bit_num != IIC_NO_MEMORY_ADDRESS)    app_iic_set_data_cmd_pack(reg,1, 0, 1, 0);
    }
    //send main data
    inst->env.tx.remain = inst->env.rx.remain = size;   //tx need send clock.
    inst->env.tx.bufptr = inst->env.rx.bufptr = bufptr;
    inst->env.rw_mode = IIC_MODE_READ;
    //tramsmit remain data ,generate clocks,transmit depth need decrease 1
    uint8_t tl_value = (size > APP_IIC_RX_TX_FIFO_DEPTH) ? APP_IIC_RX_TX_FIFO_DEPTH/2 : size-1;
    app_iic_set_rxtl(reg, tl_value);
    app_iic_set_txtl(reg, tl_value);
    //send clocks
    app_iic_set_int_unmask_tx_empty(reg);
}

void app_iic_read(periph_inst_handle_t hdl,uint8_t *bufptr, uint32_t size , uint16_t device_address, uint16_t mem_address , void (*callback) (void*,uint8_t),void* dummy)
{
    iic_transfer_sanity_check(bufptr,size,callback);
    app_iic_inst_t *inst = CONTAINER_OF(hdl, app_iic_inst_t, inst);
    inst->env.rx.callback = callback; 
    inst->env.rx.dummy = dummy;
    iic_universal_func.sys_stat_func(inst,IIC_READ_START);
    iic_universal_func.clk_gate_func(inst,SET_CLK);
    if(inst->param.use_dma)
    {
        app_iic_read_dma(inst,bufptr,size,device_address,mem_address);
    }else
    {
        app_iic_read_no_dma(inst,bufptr,size,device_address,mem_address);
    }
}

static void iic_write_dma_callback(app_iic_inst_t *inst)
{
    iic_tx_complete(inst);
}

static void app_iic_write_dma(app_iic_inst_t *inst,uint8_t *bufptr, uint32_t size , uint16_t device_address, uint16_t mem_address)
{
    reg_iic_t *reg = inst->reg;
    uint32_t *p_tx_pointer = inst->env.tx.dma_buffer;
    
    app_dmac_transfer_param_t param = 
    {
        .callback = (void (*)(void*))iic_write_dma_callback,
        .callback_param = inst,
        .src = (uint8_t *)p_tx_pointer,
        .dst = (uint8_t *)&reg->IC_DATA_CMD,
        .length = 0,//set value later
        .src_tr_width = Transfer_Width_32_bits,
        .dst_tr_width = Transfer_Width_32_bits,
        .src_msize = Burst_Transaction_Length_1,
        .dst_msize = Burst_Transaction_Length_1,
        .tt_fc = Memory_to_Peripheral_DMAC_Flow_Controller,
        .src_per = 0,
        .dst_per = dmac_iic_tx_handshake_enum(inst->idx),
        .int_en = 1,
    };

    //memory address generation
    if(inst->param.mem_addr_bit_num == IIC_16BIT_MEMORY_ADDRESS)
    {
        param.length = (size+2);// 16*4 bytes data written to EEPROM, and 1 write cmd. Write reg for 17 times.
        *p_tx_pointer = mem_address >> 8;    p_tx_pointer ++;// restart = 0, stop = 0, cmd = write, data(addr) = 0
        *p_tx_pointer = mem_address & 0xFF;  p_tx_pointer ++;
    }
    if(inst->param.mem_addr_bit_num == IIC_8BIT_MEMORY_ADDRESS)
    {
		param.length = (size+1);//new dma driver is no need *4
		*p_tx_pointer  = mem_address & 0xFF; p_tx_pointer ++;
    }
    if(inst->param.mem_addr_bit_num == IIC_NO_MEMORY_ADDRESS)
	{
		param.length = (size);
	}
	//data make
    for(uint8_t cnt = 0 ; cnt < size ; cnt ++)
    {
        uint8_t stop_bit = (cnt == (size - 1)) ? 1 : 0;
        *p_tx_pointer = (stop_bit << 9) | *bufptr;// restart = 0, stop = ?, cmd = write
    	p_tx_pointer++;
    	bufptr++;
    }
    //output pulse
    iic_set_target_address(reg , device_address >> 1);
    app_dmac_start_wrapper(&param);
}

static void app_iic_write_no_dma(app_iic_inst_t *inst,uint8_t *bufptr, uint32_t size , uint16_t device_address, uint16_t mem_address)
{
    reg_iic_t *reg = inst->reg;
    inst->env.tx.size = size;
    inst->env.tx.remain = size;
    inst->env.tx.bufptr = bufptr;
    inst->env.rw_mode = IIC_MODE_WRITE;
    app_iic_set_txtl(reg,APP_IIC_RX_TX_FIFO_DEPTH/2);
    if(inst->param.work_mode == IIC_MASTER)
    {
        iic_set_target_address(reg, device_address >> 1);
        //send head data
        if(inst->param.mem_addr_bit_num == IIC_16BIT_MEMORY_ADDRESS) app_iic_send_addr_cmd(reg , mem_address >> 8  , 0);
        if(inst->param.mem_addr_bit_num != IIC_NO_MEMORY_ADDRESS)    app_iic_send_addr_cmd(reg , mem_address & 0xFF, 0);
        //start transfer
        app_iic_set_int_unmask_tx_empty(reg);
    }
}

void app_iic_write(periph_inst_handle_t hdl,uint8_t *bufptr, uint32_t size , uint16_t device_address, uint16_t mem_address , void (*callback) (void*,uint8_t),void* dummy)
{
    iic_transfer_sanity_check(bufptr,size,callback);
    app_iic_inst_t *inst = CONTAINER_OF(hdl, app_iic_inst_t, inst);
    inst->env.tx.callback = callback; 
    inst->env.tx.dummy = dummy;
    iic_universal_func.sys_stat_func(inst,IIC_WRITE_START);
    iic_universal_func.clk_gate_func(inst,SET_CLK);
    if(inst->param.use_dma)
    {
        app_iic_write_dma(inst,bufptr,size,device_address,mem_address);
    }else
    {
        app_iic_write_no_dma(inst,bufptr,size,device_address,mem_address);
    }
}



