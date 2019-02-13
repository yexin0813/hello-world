#ifndef _SYSC_PER_MAIN_H__
#define _SYSC_PER_MAIN_H__
#include "bx_config.h"

#if HW_BX_VERSION == 2400
	#include "reg_sysc_per_2400.h"
#elif HW_BX_VERSION == 2401
	#include "reg_sysc_per_2401.h"
#else
	#error ERROR: ERROR to define HW_BX_VERSION
#endif

#endif
