#include "patch.h"
#include "bx_dbg.h"
#if defined(V4_ROM)
#define ADV_INT_ADDR 0x14a2c
#elif defined(MP_ROM)
#define ADV_INT_ADDR 0x1542c
#else
#error patch version not defined
#endif

#define ADV_INT 1380
void patch_adv_int()
{
    uint8_t patch_no;
    if(patch_alloc(&patch_no)==false)
    {
        BX_ASSERT(0);
    }
    patch_entrance_exit_addr(patch_no,ADV_INT_ADDR,ADV_INT);
    PATCH_ENABLE(patch_no);
} 
