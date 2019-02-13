/**
 ****************************************************************************************
 *
 * @file rst_gen.c
 *
 * @brief Implementation of functions used for rst_gen
 *
 * Copyright (C) Apollo 2015-2016
 *
 *
 ****************************************************************************************
 */
#include <stdint.h>
#include "bx_config.h"
#include "reg_sysc_cpu.h"
#include "reg_sysc_per.h"
#include "reg_sysc_awo.h"

/**
 ****************************************************************************************
 * @addtogroup RST_GEN
 * @ingroup DRIVERS
 * @brief
 *
 * @{
 ****************************************************************************************
 */

/**********************AWO*************************/
void srst_awo(uint32_t mask)
{
    #if HW_BX_VERSION == 00
    sysc_awo_awo_reset_signal_set(mask);
    sysc_awo_awo_reset_signal_set(mask>>1);
    #endif
}

/**********************CPU**************************/
void srst_cpu(uint32_t mask)
{
    sysc_cpu_srst_set(mask);
    sysc_cpu_srst_set(mask>>1);
}

/**********************PER**************************/
void srst_per(uint32_t mask)
{
    sysc_per_srst_set(mask);
    sysc_per_srst_set(mask>>1);
}

/// @}
