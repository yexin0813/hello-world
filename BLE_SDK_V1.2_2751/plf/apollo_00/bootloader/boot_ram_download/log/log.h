#ifndef LOG_H_
#define LOG_H_
#include "bx_config.h"
#include <stdio.h>
#include <stdarg.h>
#if (defined(CFG_SYS_LOG)&&(CFG_SYS_LOG==1))
#include "SEGGER_RTT.h"
#endif
#include "bx_dbg.h"


#define LOG_LVL_INFO LVL_INFO
#define LOG_LVL_WARN LVL_WARN
#define LOG_LVL_ERROR LVL_ERROR
void rtt_log(int8_t level, const char * format, ...);
#define LOG_ON ((defined(CFG_SYS_LOG)&&(CFG_SYS_LOG==1))&&!defined(__BOOT_ROM__)&&!defined(__BOOT_RAM__))
#if LOG_ON	
    #if (defined(BX_VERF)&&(BX_VERF==1))
        #define LOG(level,...) 				\
				do{							\
					printf(__VA_ARGS__);	\
					*(uint32_t *)SIM_REPORT_BASE = 0x7;\
				}while(0)					
        #define LOG_INIT()
    #else
        #define LOG(level,...)	 \
        do{\
            rtt_log((level),__VA_ARGS__);\
        }while(0)
        #define LOG_INIT()	SEGGER_RTT_Init()
    #endif
#else
    #define LOG(level,...)
    #define LOG_INIT()
#endif	

#endif
