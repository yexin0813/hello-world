
#include "clk_gate.h"
#include "apollo_00.h"
#include "bx_dbg.h"
#define CLK_MASK_CLR    0xaaaaaaaa
#define CLK_MASK_SET    (~CLK_MASK_CLR)

void clk_gate_clr_all_clk()
{
    #if HW_BX_VERSION == 00
    sysc_cpu_clkg0_set(CLK_MASK_CLR);
    #endif
    sysc_cpu_clkg1_set(CLK_MASK_CLR);
    sysc_per_clkg0_set(CLK_MASK_CLR);
    sysc_per_clkg1_set(CLK_MASK_CLR);
}

void clk_gate_awo(uint32_t map)
{
    #if HW_BX_VERSION == 00
    sysc_awo_reg_awo_clkg_en_set(map);
    if(map&CLK_MASK_CLR)
    {
        while(sysc_awo_reg_awo_clkg_en_get()&map);
    }else if(map&CLK_MASK_SET)
    {
        while((~ sysc_awo_reg_awo_clkg_en_get())&map);
    }else
    {
        BX_ASSERT(0);
    }
    #endif
}

void clk_gate_per_g0(uint32_t map)
{
    sysc_per_clkg0_set(map);
    if(map & CLK_MASK_CLR)            //clr
    {
        while((sysc_per_clkg0_get() & map));
    }
    else if(map & CLK_MASK_SET)       //set
    {
        while((~ sysc_per_clkg0_get()) & map);
    }
    else
    {
        BX_ASSERT(0);
    }
        
}

void clk_gate_per_g1(uint32_t map)
{
    sysc_per_clkg1_set(map);
    if(map & CLK_MASK_CLR)            //clr
    {
        while((sysc_per_clkg1_get() & map));
    }
    else if(map & CLK_MASK_SET)       //set
    {
        while((~ sysc_per_clkg1_get()) & map);
    }
    else
    {
        BX_ASSERT(0);
    }
}

#if HW_BX_VERSION == 00
void clk_gate_cpu_g0(uint32_t map)
{
    sysc_cpu_clkg0_set(map);
    if(map & CLK_MASK_CLR)            //clr
    {
        while((sysc_cpu_clkg0_get() & map));
    }
    else if(map & CLK_MASK_SET)       //set
    {
        while((~ sysc_cpu_clkg0_get()) & map);
    }
    else
    {
        BX_ASSERT(0);
    }

}
#endif

void clk_gate_cpu_g1(uint32_t map)
{
    sysc_cpu_clkg1_set(map);
    if(map & CLK_MASK_CLR)            //clr
    {
        while((sysc_cpu_clkg1_get() & map));
    }
    else if(map & CLK_MASK_SET)       //set
    {
        while((~ sysc_cpu_clkg1_get()) & map);
    }
    else
    {
        BX_ASSERT(0);
    }

}


