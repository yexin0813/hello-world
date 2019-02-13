#include "periph_common.h"
#include "clk_gate.h"
#include "rst_gen.h"
#include "reg_sysc_cpu.h"
static void cache_clk_gate(void *inst,clk_gate_operation_t op)
{
    if(op == SET_CLK)
    {
        clk_gate_cpu_g1(CPU_CLKG_SET_CACHE);
    }else
    {
        clk_gate_cpu_g1(CPU_CLKG_CLR_CACHE);
    }
    
}

static void cache_sys_stat(void *inst,uint32_t sys_stat)
{
    sysc_cpu_cache_has_sram_setf((uint8_t)sys_stat);
}

clk_gate_func_t cache_clk_gate_func = cache_clk_gate;
sys_stat_func_t cache_sys_stat_func = cache_sys_stat;
