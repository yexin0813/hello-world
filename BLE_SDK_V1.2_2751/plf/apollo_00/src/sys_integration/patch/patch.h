
/**
 ****************************************************************************************
 *
 * @file patch.h
 *
 * @brief bus patch driver interface
 *
 * Copyright (C) Apollo
 *
 *
 ****************************************************************************************
 */

#ifndef PATCH_H_
#define PATCH_H_


/**
 ****************************************************************************************
 * @addtogroup bus patch
 * @ingroup DRIVERS
 *
 * @brief bus patch driver
 *
 * @{
 ****************************************************************************************
 */



/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h>
#include <stdbool.h>
#include "reg_bus_ptch.h"

/*
 * DEFINES
 ****************************************************************************************
 */
//#define V4_ROM
#define MP_ROM
#define PATCH_ENABLE(no)     hwp_bus_ptch->trap_en.val |= 1<<(no)

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
bool patch_alloc(uint8_t *no);
/**
 * @brief set the trap's entrance and exit address.
 * @param patch_no: patch index range from 0 to 15.
 * @param entrance_addr: address where we need to jump from.
 * @param run_code: replace the code to run_code. Normally we first jump to an assembly file, and than jump to the patch function.
 */
void patch_entrance_exit_addr(uint8_t patch_no, uint32_t entrance_addr, uint32_t run_code);


/**
 * @brief enable or disable a bus patch.
 * @param trap_en_map: map to enable a patch. bit 0 = 1 means enable patch_no 0.
 */
void patch_enable_set(uint16_t trap_en_map);

/**
 * @brief get patch status.
 */
uint16_t patch_status_get(void);

/**
 * @brief help to calculate the jump code, generate a 32bits code to jump from srcAddr to dstAddr.
 * @param srcAddr: jump from srcAddr.
 * @param dstAddr: jump to dstAddr.
 * @return return code to jump from srcAddr to dstAddr.
 */
uint32_t cal_patch_bl(uint32_t srcAddr,uint32_t dstAddr);

uint32_t cal_patch_bl_pre(uint32_t srcAddr,uint32_t dstAddr);

uint32_t cal_patch_pushlr(uint32_t patched_func);


void patch_init(void);


#endif // PATCH_H_

