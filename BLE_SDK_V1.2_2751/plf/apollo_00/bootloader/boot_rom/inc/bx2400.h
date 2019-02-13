/**************************************************************************//**
 * @file     bx2400.h
 * @brief    CMSIS Cortex-M0 Core Peripheral Access Layer Header File for
 *           Device BX2400
 * @version  V3.10
 * @date     23. November 2012
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2012 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


#ifndef BX2400_H     
#define BX2400_H

#ifdef __cplusplus
 extern "C" {
#endif


/** @addtogroup BX2400_Definitions BX2400 Definitions
  This file defines all structures and symbols for BX2400:
    - registers and bitfields
    - peripheral base address
    - peripheral ID
    - Peripheral definitions
  @{
*/


/******************************************************************************/
/*                Processor and Core Peripherals                              */
/******************************************************************************/
/** @addtogroup <Device>_CMSIS Device CMSIS Definitions
  Configuration of the Cortex-M# Processor and Core Peripherals
  @{
*/

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

typedef enum IRQn
{
/******  Cortex-M# Processor Exceptions Numbers ***************************************************/

/* ToDo: use this Cortex interrupt numbers if your device is a CORTEX-M0 device                   */
  NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt                        */
  HardFault_IRQn                = -13,      /*!<  3 Hard Fault Interrupt                          */
  SVCall_IRQn                   = -5,       /*!< 11 SV Call Interrupt                             */
  PendSV_IRQn                   = -2,       /*!< 14 Pend SV Interrupt                             */
  SysTick_IRQn                  = -1,       /*!< 15 System Tick Interrupt                         */

/******  Device Specific Interrupt Numbers ********************************************************/
  WDT_IRQn               		=   0,      
  BLE_LP_IRQn               	=   1,     
  BLE_MAC_IRQn             	  	=   2,      
  RTC_IRQn                 		=   3,      
  EXT_INTR_IRQn             	=   4,     
  ECC_IRQn               	    =   5,
  DMAC_IRQn            	    	=   6,     
  QSPI_IRQn                     =   7,      
  SPIM0_IRQn                    =   8,     
  SPIM1_IRQn                    =   9,     
  SPIS_IRQn                     =  10,    
  UART0_IRQn                    =  11,     
  UART1_IRQn                    =  12,     
  IIC0_IRQn                     =  13,      
  IIC1_IRQn                     =  14,    
  GPIO_IRQn                     =  15,      
  TIMER_IRQn                    =  16,     
  SFT_IRQn                      =  17,
  PGSPY_IRQn                    =  18,

} IRQn_Type;


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M0 Processor and Core Peripherals */

#define __CM0PLUS_REV             0x0000    /*!< Core Revision r0p0                               */
#define __VTOR_PRESENT            1         /*!< VTOR present or not 															*/
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */
#define __MPU_PRESENT             0         /*!< MPU present or not                               */
#define __FPU_PRESENT             0         /*!< FPU present or not                               */

/*@}*/ /* end of group BX2400_CMSIS */

#include <core_cm0plus.h>                 /* Cortex-M0+ processor and core peripherals        */
#include "system_bx2400.h"                /* BX2400 System  include file                      */


#ifdef __cplusplus
}
#endif

#endif  /* BX2400_H */
