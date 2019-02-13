#ifndef _SYSC_AWO_MAIN_H__
#define _SYSC_AWO_MAIN_H__
#include "bx_config.h"

#if HW_BX_VERSION == 00
	#include "reg_sysc_awo_apollo_00.h"
#elif HW_BX_VERSION == 01
	#include "reg_sysc_awo_apollo_01.h"
	#include "reg_sysc_ble.h"
#else
	#error ERROR: ERROR to define HW_BX_VERSION
#endif

#endif
