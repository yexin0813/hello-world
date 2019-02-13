/*
 * bx_dbg.h
 *
 *  Created on: 2016-3-22
 *      Author: Administrator
 */

#ifndef BX_DBG_H_
#define BX_DBG_H_
#include "bx_config.h"
//#define NDEBUG

#define __ASSERT_MSG
#include <assert.h>
#include <stdint.h>
#define BX_ASSERT(e) assert(e)
 #ifdef __RAM_CODE__
void rwip_assert_asm(uint32_t,uint32_t,uint32_t);
 #else
typedef void (*rwip_assert_fn)(uint32_t,uint32_t,uint32_t);
 #define rwip_assert_asm(lvl,param0,param1) ((rwip_assert_fn)jump_table[RWIP_ASSERT_DBG])((lvl),(param0),(param1))
 #endif

#define LVL_ERROR 1
#define LVL_WARN 2
#define LVL_INFO 3
#define RWIP_ASSERT(lvl,e,param0,param1) if(!(e)) rwip_assert_asm((lvl),(uint32_t)(param0),(uint32_t)(param1))   


//for print and debug
#define SIM_REPORT_BASE                 0x2010f004
#define SIM_PRINT_BASE                  (SIM_REPORT_BASE+4)
#define SIM_PRINT_DATA_BASE             (SIM_REPORT_BASE+8)
#if (defined(BX_VERF)&&(BX_VERF==1))
#define SIMU_PASS() bx_simu_pass()
#define SIMU_FAIL() bx_simu_fail()
#define SIMU_FINISH() bx_simu_finish()
#else
#define SIMU_PASS()
#define SIMU_FAIL()
#define SIMU_FINISH()
#endif
void bx_simu_pass(void);
void bx_simu_fail(void);
void bx_simu_finish(void);


#endif /* BX_DBG_H_ */
