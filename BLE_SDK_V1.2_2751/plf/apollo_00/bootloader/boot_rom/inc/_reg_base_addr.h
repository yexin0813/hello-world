//////////////////////////////////////////////////////////////////////////////
//  Copyright (C) by BLUEX.
//  This module is a confidential and proprietary property of BLUEX
//  and a possession or use of this module requires written permission 
//  from BLUEX.
//----------------------------------------------------------------------------
// Author           : None
// Company          : BLUEX
// ---------------------------------------------------------------------------
// Description      :C:\doc\svn\design\registers\fr.exe -f wdt_regmap.xlsx uart1_regmap.xlsx uart0_regmap.xlsx timer_regmap.xlsx sysc_per_regmap.xlsx sysc_cpu_regmap.xlsx sysc_awo_regmap.xlsx spis_regmap.xlsx spim_regmap.xlsx rtc_regmap.xlsx qspi_regmap.xlsx pwm_regmap.xlsx modem_regmap.xlsx iic0_regmap.xlsx iic1_regmap.xlsx gpio_regmap.xlsx ecc_regmap.xlsx dmac_regmap.xlsx cache_regmap.xlsx bx2400_bus_arch_regmap.xlsx bus_ptch_regmap.xlsx adc_ctrl_regmap.xlsx -cheader
// Simulation Notes :
// Synthesis Notes  :
// Application Note :
// Simulator        :
// Parameters       :
// Terms & concepts :
// Bugs             :
// Open issues and future enhancements :
// References       :
// Revision History :
// ---------------------------------------------------------------------------
//
//////////////////////////////////////////////////////////////////////////////
#ifndef ___REG_BASE_ADDR_H__
#define ___REG_BASE_ADDR_H__
#include "bx2400.h"
//Auto-gen by fr
#define    REG_SRAM_BASE                  0x00100000          
#define    REG_DMAC_BASE                  0x20120000          
#define    REG_SYSC_AWO_BASE              0x20201000          
#define    REG_BROM_BASE                  0x00000000          
#define    REG_PWM_BASE                   0x20147000          
#define    REG_TIMER_BASE                 0x20130000          
#define    REG_QSPI_BASE                  0x20300000          
#define    REG_RTC_BASE                   0x20200000          
#define    REG_SYSC_PER_BASE              0x20149000          
#define    REG_IIC1_BASE                  0x20146000          
#define    REG_SPIS_BASE                  0x20142000          
#define    REG_WDT_BASE                   0x20131000          
#define    REG_BLE_MDM_BASE               0x20110000
#define    REG_SYSC_BLE_BASE			  0x20111000
#define    REG_ADC_CTRL_BASE              0x20136000          
#define    REG_SPIM_1_BASE                0x20141000          
#define    REG_UART1_BASE                 0x20144000          
#define    REG_ECC_BASE                   0x20134000          
#define    REG_SPIM_0_BASE                0x20140000          
#define    REG_SYSC_CPU_BASE              0x20132000          
#define    REG_IIC0_BASE                  0x20145000          
#define    REG_GPIO_BASE                  0x20148000          
#define    REG_BLE_MAC_BASE               0x20100000          
#define    REG_CACHE_MEM_BASE             0x00130000          
#define    REG_BUS_PTCH_BASE              0x20133000          
#define    REG_CACHE_BASE                 0x20121000          
#define    REG_UART0_BASE                 0x20143000          

#endif


