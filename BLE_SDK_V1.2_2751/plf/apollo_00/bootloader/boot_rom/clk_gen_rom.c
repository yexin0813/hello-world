#include "reg_sysc_per.h"

void clk_uart0_115200(void)
{
    sysc_per_clkg0_set(1<<9);
    sysc_per_clk_div_param1_pack(9, 5, 17, 16);
    sysc_per_clk_sel_uart0_setf(1);
    sysc_per_clkg0_set(1<<8);
}

