#include <stddef.h>
#include "app_qspi_wrapper.h"
#include "spi_flash_cmd.h"
#include "reg_cache.h"
#include "_reg_base_addr.h"
#include "periph_common.h"
#include "field_manipulate.h"
#include "flash_cache.h"
#include "arch_init.h"
#define CACHE_REG ((reg_cache_t *)REG_CACHE_BASE)
#define CACHE_LINE_SIZE_IN_BYTES 32

typedef enum
{
	Little_Endian =0,//!< Little_Endian
	Big_Endian       //!< Big_Endian
}FLASH_OUTPUT_DATA_FORMAT;
typedef enum
{
	Right_Justified = 0,
	Left_Justified
}CMD_ADDR_ALIGNMENT;

extern clk_gate_func_t cache_clk_gate_func;
extern sys_stat_func_t cache_sys_stat_func;

void flash_cache_flush(uint32_t low_addr,uint32_t high_addr)
{
    CACHE_REG->REG_FLUSH_ALL = 0;
    CACHE_REG->REG_FLUSH_ADDR_L = low_addr;
    CACHE_REG->REG_FLUSH_ADDR_H = high_addr;
    CACHE_REG->REG_FLUSH_EN = 1;
    while(CACHE_REG->REG_FLUSH_EN);    
}

void flash_cache_flush_all()
{
    CACHE_REG->REG_FLUSH_ALL = 1;
    CACHE_REG->REG_FLUSH_EN = 1;
    while(CACHE_REG->REG_FLUSH_EN);
}

void flash_cache_config(uint32_t flash_base_addr)
{
    cache_sys_stat_func(NULL,1);
    cache_clk_gate_func(NULL,SET_CLK);
    flash_cache_flush_all();
    CACHE_REG->REG_FLASH_BASE_ADDR = flash_base_addr;
    CACHE_REG->REG_QSPI_DR_ADDR = app_qspi_dr_reg_addr_for_cache_wrapper();
    CACHE_REG->REG_FLASH_CMD = FIELD_BUILD(CACHE_TRAM_PARA,0x1380)|
        FIELD_BUILD(CACHE_FLASH_RD_CMD_FORMAT,Right_Justified)|
        FIELD_BUILD(CACHE_FLASH_RD_DATA_FORMAT,Little_Endian)|
        FIELD_BUILD(CACHE_FLASH_RD_ADDR_FORMAT,Right_Justified);
}

periph_err_t flash_cache_enable(uint8_t type)
{
    cache_clk_gate_func(NULL,SET_CLK);
    if(type == Std_Read_Mode || type >= Read_Mode_Max)
    {
        return PERIPH_INVALID_PARAM;
    }
    uint8_t read_cmd;
    qspi_multi_read_param_t param=
     {
        .length = CACHE_LINE_SIZE_IN_BYTES,
        .inst_l = Instruction_Length_8_bits,
     };
    switch(type)
    {
    case Dual_Output_Read_Mode:
        param.trans_type = Both_Standard_SPI_Mode;
        param.dual_quad = Dual_SPI_Format;
        param.wait_cycles = 8;
        read_cmd = FLASH_DUAL_OUTPUT_FAST_READ;
    break;
    case Quad_Output_Read_Mode:
        param.trans_type = Both_Standard_SPI_Mode;
        param.dual_quad = Quad_SPI_Format;
        param.wait_cycles = 8;
        read_cmd = FLASH_QUAD_OUTPUT_FAST_READ;
    break;
    case Dual_IO_Read_Mode:
        param.trans_type = Instruction_Standard_Address_Specific;
        param.dual_quad = Dual_SPI_Format;
        param.wait_cycles = 4;
        read_cmd = FLASH_DUAL_IO_FAST_READ;
    break;
    case Quad_IO_Read_Mode:
        param.trans_type = Instruction_Standard_Address_Specific;
        param.dual_quad = Quad_SPI_Format;
        param.wait_cycles = 6;
        read_cmd = FLASH_QUAD_IO_FAST_READ;
    break;
    }
    REG_FIELD_WR(CACHE_REG->REG_FLASH_CMD,CACHE_FLASH_RD_CMD,read_cmd);
    app_qspi_config_for_cache_wrapper(&param);
    CACHE_REG->REG_CACHE_EN = 1;
    return PERIPH_NO_ERROR;
}

void flash_cache_disable()
{
    if(CACHE_REG->REG_CACHE_EN)
    {
        app_qspi_disable_for_cache_wrapper();
    }
    cache_clk_gate_func(NULL,CLR_CLK);
}
