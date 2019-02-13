
//#include "pshare.h"
#include "reg_sysc_cpu.h"
#include "bx_config.h"

void pshare_rom_uart_init()
{
    sysc_cpu_func_io10_sel_setf(0);         //IO_UART0_TXD  funcio 10 = gpio_mux[12]
    sysc_cpu_func_io11_sel_setf(1);         //IO_UART0_RXD  funcio 11 = gpio_mux[13]
    #if HW_BX_VERSION == 00
    sysc_cpu_func_io_en_r_setf((1<<10) | (1<<11));
    #elif HW_BX_VERSION == 01
	sysc_cpu_func_io_en_setf((1<<10) | (1<<11));
    #else
    #error ERROR: ERROR to define HW_BX_VERSION
    #endif

}

void pshare_rom_spi_init()
{
    sysc_cpu_qspi_en_setf(3);               //spi clk cs data0 data1
}


/// @} pin share in rom
