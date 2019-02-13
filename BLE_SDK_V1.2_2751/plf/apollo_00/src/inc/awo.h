#ifndef AWO_MAIN_H_
#define AWO_MAIN_H_
#include "bx_config.h"
#if HW_BX_VERSION == 00
	#include "awo_apollo_00.h"
#elif HW_BX_VERSION == 01
	#include "awo_apollo_01.h"
#else
	#error ERROR: ERROR to define HW_BX_VERSION
#endif

#endif
