#include "bx_config.h"
#include "reg_sysc_cpu.h"


void pshare_rom_uart0_rx_init()
{
    sysc_cpu_func_io11_sel_setf(1);
    uint32_t func_io_en = sysc_cpu_func_io_en_get();
    func_io_en |= 1<<11;
    sysc_cpu_func_io_en_set(func_io_en);

}

void pshare_rom_uart0_tx_init()
{
    sysc_cpu_func_io10_sel_setf(0);
    uint32_t func_io_en = sysc_cpu_func_io_en_get();
    func_io_en |= 1<<10;
    sysc_cpu_func_io_en_set(func_io_en);
}

void pshare_rom_spi_flash_init()
{
    sysc_cpu_qspi_en_setf(3);               //spi clk cs data0 data1
}


/// @} pin share in rom
