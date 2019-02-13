#include "bx_config.h"
#include "apollo_00.h"
#include "app_qspi_wrapper.h"
#include "app_qspi.h"
#include "arch_init.h"
#if (defined(CFG_FREERTOS_SUPPORT)&&(CFG_FREERTOS_SUPPORT==1))
#include "FreeRTOS.h"
#include "task.h"
#endif

#define QSPI_CS_SEL_MASK 0x1
app_qspi_inst_t qspi_inst = QSPI_INSTANCE();

static uint32_t flash_qspi_operation_enter()
{
    uint32_t __irq_mask = 0;
    #if (defined(FLASH_XIP)&&(FLASH_XIP==1))
    #if (defined(CFG_FREERTOS_SUPPORT)&&(CFG_FREERTOS_SUPPORT==1))
    taskENTER_CRITICAL();
    #else
    __irq_mask = __get_PRIMASK();
    __disable_irq();
    #endif
    cache_disable();
    #endif
    return __irq_mask;
}

static void flash_qspi_operation_exit(uint32_t __irq_mask)
{
    #if (defined(FLASH_XIP)&&(FLASH_XIP==1))
    cache_enable();
    #if (defined(CFG_FREERTOS_SUPPORT)&&(CFG_FREERTOS_SUPPORT==1))
    taskEXIT_CRITICAL();
    #else
    if(__irq_mask == 0)
    {
        __enable_irq();
    }
    #endif
    #endif
}


void app_qspi_init_wrapper()
{
    extern uint32_t flash_op_mode;
    qspi_inst.param.clk_div = 2;
    qspi_inst.param.rxsample_dly = 1;
    switch(flash_op_mode)
    {
    case Std_Read_Mode:
        qspi_inst.param.mode = Standard_SPI_Format;
    break;
    case Dual_Output_Read_Mode:
    case Dual_IO_Read_Mode:
        qspi_inst.param.mode = Dual_SPI_Format;
    break;
    case Quad_Output_Read_Mode:
    case Quad_IO_Read_Mode:
        qspi_inst.param.mode = Quad_SPI_Format;
    break;
    }
    qspi_inst.param.cs_pin_no[0] = QSPI_CS0_PIN_NO;
    qspi_inst.param.clk_pin_no = QSPI_CLK_PIN_NO;
    qspi_inst.param.data_pin_no[0] = QSPI_D0_PIN_NO;
    qspi_inst.param.data_pin_no[1] = QSPI_D1_PIN_NO;
    qspi_inst.param.data_pin_no[2] = QSPI_D2_PIN_NO;
    qspi_inst.param.data_pin_no[3] = QSPI_D3_PIN_NO;
    app_qspi_init(&qspi_inst.inst); 
}

void app_qspi_uninit_wrapper()
{
    app_qspi_uninit(&qspi_inst.inst);
}

void app_qspi_std_write_wrapper(uint8_t *data,uint32_t length)
{
    uint32_t irq_mask = flash_qspi_operation_enter();
    app_qspi_std_write(&qspi_inst.inst,QSPI_CS_SEL_MASK,data,length);
    flash_qspi_operation_exit(irq_mask);
}

void app_qspi_flash_program_wrapper(uint8_t cmd,uint32_t addr,uint8_t *data,uint32_t length)
{
    uint32_t irq_mask = flash_qspi_operation_enter();
    app_qspi_flash_program(&qspi_inst.inst,QSPI_CS_SEL_MASK,cmd,addr,data,length);
    flash_qspi_operation_exit(irq_mask);
}

void app_qspi_std_read_wrapper(uint8_t *cmd_buf,uint8_t cmd_len,uint8_t *data_buf,uint16_t data_len)
{
    uint32_t irq_mask = flash_qspi_operation_enter();
    app_qspi_std_read(&qspi_inst.inst,QSPI_CS_SEL_MASK,cmd_buf,cmd_len,data_buf,data_len);
    flash_qspi_operation_exit(irq_mask);
}

void app_qspi_multi_read_wrapper(qspi_multi_read_param_t *param)
{
    uint32_t irq_mask = flash_qspi_operation_enter();
    app_qspi_multi_read(&qspi_inst.inst,QSPI_CS_SEL_MASK,param);
    flash_qspi_operation_exit(irq_mask);
}

void app_qspi_config_for_cache_wrapper(qspi_multi_read_param_t *param)
{
    app_qspi_config_for_cache(&qspi_inst.inst,QSPI_CS_SEL_MASK,param);
}

uint32_t app_qspi_dr_reg_addr_for_cache_wrapper()
{
    return app_qspi_dr_reg_addr_for_cache(&qspi_inst.inst);
}

void app_qspi_disable_for_cache_wrapper()
{
    app_qspi_disable_for_cache(&qspi_inst.inst);
}

