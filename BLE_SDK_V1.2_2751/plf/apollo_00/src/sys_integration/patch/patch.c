
/**
 ****************************************************************************************
 *
 * @file patch.c
 *
 * @brief bus patch driver interface
 *
 * Copyright (C) Apollo
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup bus patch
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "bx_dbg.h"
#include "reg_bus_ptch.h"
#include <stdbool.h>

//#pragma anon_unions
union bl_instruction_t
{
    uint32_t label;
    struct
    {
        uint32_t imm10:      10,
                 s:          1,
                 reserved_5: 5,
                 imm11:      11,
                 j2:         1,
                 reserved_1: 1,
                 j1:         1,
                 reserved_2: 2;
    }ins;
};

union bl_imm32_t
{
    uint32_t imm32;
    struct
    {
        uint32_t reserved_1: 1,
                 imm11:      11,
                 imm10:      10,
                 i2:         1,
                 i1:         1,
                 s:          1,
                 reserved_7: 7;
    }imm;
};

/*
 * DEFINES
 ****************************************************************************************
 */
#define MAX_PATCH_NUM 16

/*
 * STRUCT DEFINITIONS
 ****************************************************************************************
 */


/*
 * GLOBAL VARIABLE DECLARATION
 ****************************************************************************************
 */
/// push lr & nop. b500 = push lr , bf00 = nop
static uint32_t patch_pushlr = 0xbf00b500; 

/*
 * LOCAL FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */



/*
 * EXPORTED FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */
bool patch_alloc(uint8_t *no)
{
    uint8_t i;
    for(i=0;i<MAX_PATCH_NUM;++i)
    {
        if((bus_ptch_trap_en_get() & 1<<i)==0)
        {
            *no = i;
            return true;
        }
    }
    return false;
}
 
/**
 * @brief set the trap's entrance and exit address.
 * @param patch_no: patch index range from 0 to 15.
 * @param entrance_addr: address where we need to jump from, it is 4 bytes aligned. If not aligned, this function will make it align.
 * @param run_code: replace the code to run_code. Normally we first jump to an assembly file, and than jump to the patch function.
 */
void patch_entrance_exit_addr(uint8_t patch_no, uint32_t entrance_addr, uint32_t run_code)
{

	uint32_t ent;

	BX_ASSERT(patch_no <= 0xf);								// max: 16 patch

	ent = (entrance_addr & 0x3ffff) >> 2;					// keep 16bits (2~17)
    T_BUS_PTCH_TRAP0_ADDR *trap = (T_BUS_PTCH_TRAP0_ADDR *)&hwp_bus_ptch->trap0_addr;
    T_BUS_PTCH_OUT0_ADDR *out = (T_BUS_PTCH_OUT0_ADDR *)&hwp_bus_ptch->out0_addr;
    trap[patch_no].bit_field.trap0 = (uint16_t)ent;
    out[patch_no].bit_field.out0 = run_code;

}

/**
 * @brief enable or disable a bus patch.
 * @param trap_en_map: map to enable a patch. bit 0 = 1 means enable patch_no 0.
 */
void patch_enable_set(uint16_t trap_en_map)
{
	bus_ptch_trap_en_r_setf(trap_en_map);
}

/**
 * @brief get patch status.
 */
uint16_t patch_status_get()
{
    return bus_ptch_trap_en_r_getf();
}

/**
 * @brief help to calculate the jump code, generate a 32bits code to jump from srcAddr to dstAddr.
 * @param srcAddr: jump from srcAddr.
 * @param dstAddr: jump to dstAddr.
 * @return return code to jump from srcAddr to dstAddr.
 */
uint32_t cal_patch_bl(uint32_t srcAddr,uint32_t dstAddr)
{
    union bl_instruction_t bl_ins;
    union bl_imm32_t bl_imm32;

    bl_imm32.imm32 = dstAddr - srcAddr - 4;

    bl_ins.ins.imm11 = bl_imm32.imm.imm11;
    bl_ins.ins.j2 = ~(bl_imm32.imm.s ^ bl_imm32.imm.i2);
    bl_ins.ins.reserved_1 = 0x1;
    bl_ins.ins.j1 = ~(bl_imm32.imm.s ^ bl_imm32.imm.i1);
    bl_ins.ins.reserved_2 = 0x3;
    bl_ins.ins.imm10 = bl_imm32.imm.imm10;
    bl_ins.ins.s = bl_imm32.imm.s;
    bl_ins.ins.reserved_5 = 0x1e;

    return bl_ins.label;
}

uint32_t cal_patch_bl_pre(uint32_t srcAddr,uint32_t dstAddr)
{
	uint32_t offset = 0;
	uint32_t high;
	uint32_t low;

	offset = dstAddr - srcAddr;

	offset = (offset -4) & 0x007fffff;

	high = offset >> 12;
	low  = (offset & 0x00000fff) >> 1;

	return ((0xF800 | low) << 16) | (0xF000 | high);
}

/**
 * @brief help to calculate the push lr code, and this function make sure no side effect will happen if the address of a function patched can not be divided by 4.
 * @param patched_func: Address of a patched function.
 * @return return the code of push lr.
 */
uint32_t cal_patch_pushlr(uint32_t patched_func)
{
    uint32_t *data;
    patched_func -= 1;		//thumb2 code
    if((patched_func % 4) == 0)
    {
        return patch_pushlr;
    }
    else if((patched_func % 4) == 2)
    {
        data = (uint32_t *)(patched_func-2);
        return (uint32_t)(((*data) & (uint32_t)0xFFFF) | ((uint32_t)0xb500 << 16));
    }
    else
    {
        BX_ASSERT(0);
        return 0;
    }

}

void set_h4tl_patch(void);
void patch_prog_latency(void);
void set_adv_payload_31Byte_patch(void);
void patch_adv_int(void);
void set_h4tl_read_start_skip_patch(void);
void set_task_id_patch(void);
void set_llm_end_evt_defer_patch(void);

void patch_init()
{
    bus_ptch_trap_en_set(0);
    #if (CFG_FREERTOS_SUPPORT == 0 && !defined(CFG_APP))
    set_h4tl_patch();
    #endif
//    patch_prog_latency();
    #if (defined(ENABLE_ADV_PAYLOD_31BYTE_PATCH)&&(ENABLE_ADV_PAYLOD_31BYTE_PATCH==1))
    set_adv_payload_31Byte_patch();
    #endif
    #if (defined(PATCH_SKIP_H4TL_READ_START)&&(PATCH_SKIP_H4TL_READ_START==1))
    set_h4tl_read_start_skip_patch();
    #endif
    #if (LOCAL_NVDS == 0 || MESH_SCHED_PATCH == 1)
    set_task_id_patch();
    #endif
    #if(TX_TEST_PACKET_NUM_PATCH == 1 || MESH_SCHED_PATCH == 1)
    set_llm_end_evt_defer_patch();
    #endif
    patch_adv_int();

}



/// @} bus patch

