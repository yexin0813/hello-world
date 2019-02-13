/**
 ****************************************************************************************
 *
 * @file sys_sleep.h
 *
 * @brief Declaration of sys_sleep.c. Sleep and wakeup process of apollo_00
 *
 * Copyright (C) Apollo
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup SYS_SLEEP
  * @ingroup SLEEP
 * @{
 ****************************************************************************************
 */



#ifndef SYS_SLEEP_H_
#define SYS_SLEEP_H_
#include <stdbool.h>

/**
 * @brief two type of sleep.
 */
typedef enum
{
    sleep_low_power_clk,//!< sleep_low_power_clk
    wakeup_requested, //!< wakeup_requested
    active_clk          //!< active_clk
}blemac_stat_t;
extern blemac_stat_t mac_status;

#if (defined(CFG_DYNAMIC_UPDATE)&&(CFG_DYNAMIC_UPDATE==1))
#define LOW_VOLT_ADC_VAL 0xffff // re-config this value after ADC range moved
enum
{
    bat_update_io_level = 0,
    bat_update_low_volt,
    bat_update_max,
    temp_update_deri_calib = bat_update_max,
    temp_update_max
};

enum
{
    temp_neg_40,
    temp_neg_30,
    temp_neg_20,
    temp_neg_10,
    temp_0,
    temp_pos_10,
    temp_pos_20,
    temp_pos_30,
    temp_pos_40,
    temp_pos_50,
    temp_pos_60,
    temp_pos_70,
    temp_pos_80,
    temp_highest
};

typedef void (*dynamic_update_cb)(uint16_t);

void sys_param_periodic_update_init(void);
void sys_param_periodic_update(void);
#endif
/**
 * @brief handler of ble lp interrupt
 * after wakeup from deep sleep. Firstly, we get ble lp interrupt and than mac interrupt. Actually, we do nothing in ble lp isr. Ble mac handles ble things.
 * we use "deep_sleep_duration = ble_deepsldur_getf()" to store a value of last sleep time. ble_deepsldur_getf() doesn't change when wakeup.
 * This can be use in next sleep, "when deep_sleep_duration != ble_deepsldur_getf()" ble mac is ready to count sleep duration, than we can sleep.
 */
void ble_lp_isr(void);

void ble_lp_isr_mp(void);

void ble_lp_intr_clr(void);

void ble_lp_irq_disable(void);

void ble_lp_irq_enable(void);

void ble_soft_wakeup(void);

/**
 * @brief do ble task.
 */
void ble_schedule(void);
/**
 * @brief get sleep type. Sometimes we should not power off(just WFI), sometimes we can go deep sleep. It depends, so we should get the sleep type and do different process
 * @return  sleep type.
 */
uint8_t sleep_prepare_and_check(void);
/**
 * @brief The outermost function execute just before WFI
 */
void pre_deepsleep_processing_mp(void);
/**
 * @brief The outermost function execute just after WFI
 */
void post_deepsleep_processing_mp(void);

uint32_t enable_32m_rc(void);

void wait_rtc_until(uint32_t target);

uint32_t get_real_rtc(void);

void sysctrl_close_32m_rc(void);


void system_recovery(void);

void flash_recovery(void);

void switch_to_32m_xtal(uint32_t current_time);

#endif

///@}

