#include "patch.h"
#include "bx_dbg.h"
#if defined(V4_ROM)
#define ADV_PROG_LATENCY_ADDR 0x15460
#define CONN_PROG_LATENCY_ADDR 0x14ca0
#elif defined(MP_ROM)
#define ADV_PROG_LATENCY_ADDR 0x15e60
#define CONN_PROG_LATENCY_ADDR 0x156a0
#else
#error patch version not defined
#endif

void set_adv_prog_latency_patch()
{
    uint8_t patch_no;
    if(patch_alloc(&patch_no)==false)
    {
        BX_ASSERT(0);
    }
    patch_entrance_exit_addr(patch_no,ADV_PROG_LATENCY_ADDR,0x2001f9f9);
    PATCH_ENABLE(patch_no);
}

void set_conn_prog_latency_patch()
{
    uint8_t patch_no;
    if(patch_alloc(&patch_no)==false)
    {
        BX_ASSERT(0);
    }
    patch_entrance_exit_addr(patch_no,CONN_PROG_LATENCY_ADDR,0x2101d07d);
    PATCH_ENABLE(patch_no);
}

void patch_prog_latency()
{
    set_adv_prog_latency_patch();
    set_conn_prog_latency_patch();
}
