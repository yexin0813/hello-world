/**
 ****************************************************************************************
 *
 * @file rst_gen.h
 *
 * @brief Declaration of functions used for rst_gen
 *
 * Copyright (C) Apollo 2015-2016
 *
 *
 ****************************************************************************************
 */
#ifndef RST_GEN_H
#define RST_GEN_H
#include <stdint.h>
#include "co_math.h"
/**
 ****************************************************************************************
 * @defgroup RST_GEN
 * @ingroup DRIVERS
 * @brief
 *
 * @{
 ****************************************************************************************
 */

/**********************AWO**************************/
#define CHIP_SRST_AWO         CO_BIT(1)
#define BLE_LP_SRST_AWO     CO_BIT(3)
#define RTC_SRST_AWO           CO_BIT(5)
#define BLE_MAC_MDM_SRST_AWO CO_BIT(7)
#define TOUCH_SRST_AWO      CO_BIT(9)

void srst_awo(uint32_t mask);

/**********************CPU**************************/

#define DMAC_SRST_CPU       CO_BIT(5)
#define CACHE_SRST_CPU      CO_BIT(7)
#define WDT_SRST_CPU          CO_BIT(9)
#define TIMER_SRST_CPU      CO_BIT(11)
#define QSPI_SRST_CPU           CO_BIT(13)
#define ECC_SRST_CPU            CO_BIT(23)

void srst_cpu(uint32_t mask);

/**********************PER**************************/

#define IIC0_SRST_PER           CO_BIT(1)
#define IIC1_SRST_PER           CO_BIT(3)
#define UART0_SRST_PER      CO_BIT(5)
#define UART1_SRST_PER      CO_BIT(7)
#define SPIM0_SRST_PER      CO_BIT(9)
#define SPIM1_SRST_PER      CO_BIT(11)
#define SPIS_SRST_PER           CO_BIT(13)
#define PWM_SRST_PER        CO_BIT(15)
#define GPIO_SRST_PER        CO_BIT(17)

void srst_per(uint32_t mask);

/// @}
#endif
