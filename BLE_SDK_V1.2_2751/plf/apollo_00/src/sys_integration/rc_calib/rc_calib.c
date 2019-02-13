/*
 * rc_calib.c
 *
 *  Created on: 2016Äê6ÔÂ21ÈÕ
 *      Author: mingzhou
 */

//#include "rc_calib.h"
#include <stdint.h>
#include <stdbool.h>
#include "reg_sysc_cpu.h"
#include "log.h"

uint32_t calib32k;
uint32_t rc_freq;
void rc_calib_start(bool en)
{
    sysc_cpu_calb32k_start_setf((uint8_t)en);
}

void rc_calib_end(void)
{
    uint32_t i = 0;
    while(!(sysc_cpu_calb32k_start_getf() == 0))
    {
        i++;
    }
    LOG(LOG_LVL_INFO, "rc_calib_getVal, i=%d.\r\n", i);
    calib32k = sysc_cpu_calb32k_rslt_getf();
    rc_freq = (1000000*64*32)/calib32k;
}
