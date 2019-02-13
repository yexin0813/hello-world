/*
 *-----------------------------------------------------------------------------
 * The confidential and proprietary information contained in this file may
 * only be used by a person authorised under and to the extent permitted
 * by a subsisting licensing agreement from ARM Limited.
 *
 *            (C) COPYRIGHT 2009-2010 ARM Limited.
 *                ALL RIGHTS RESERVED
 *
 * This entire notice must be reproduced on all copies of this file
 * and copies of this file may only be made by a person if such person is
 * permitted to do so under the terms of a subsisting license agreement
 * from ARM Limited.
 *
 *      SVN Information
 *
 *      Checked In          : $Date: 2008-12-31 10:59:44 +0000 (Wed, 31 Dec 2008) $
 *
 *      Revision            : $Revision: 97564 $
 *
 *      Release Information : Cortex-M0-AT510-r0p0-03rel0
 *-----------------------------------------------------------------------------
 */

//
// printf retargetting functions
//
#include "bx_config.h"
#include <stdio.h>
#include <stdint.h>
#include "apollo_00.h"
#include "bx_dbg.h"

#if (defined(BX_VERF)&&(BX_VERF==1))

#if defined ( __CC_ARM   )
#if (__ARMCC_VERSION < 400000)
#else
// Insist on keeping widthprec, to avoid X propagation by benign code in C-lib
#pragma import _printf_widthprec
#endif
#endif



//
// C library retargetting
//

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;


#if defined ( __CC_ARM   )
    
int fputc(int ch, FILE *f)
{
  *(int32_t *)SIM_PRINT_BASE = ch;
  return ch;
}
int ferror(FILE *f) {
  return 0;
}
    
#else
    int _write_r(void *reent, int fd, char *ptr, size_t len) {
        *(uint32_t *)SIM_PRINT_BASE = *ptr;
        return 1;
    }
#endif
#endif

