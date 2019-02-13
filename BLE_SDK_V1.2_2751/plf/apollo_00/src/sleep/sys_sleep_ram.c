#include "bx_config.h"
#include "apollo_00.h"
#include "plf.h"
#include "flash.h"
#include "sys_sleep.h"
#include "clk_gate.h"
//#include "clk_gen.h"
#include "awo.h"
#include "pshare.h"
//#include "uart.h"
#include "rwip.h"
#include "log.h"
#include "reg_blecore.h"
#include "reg_sysc_awo.h"
#include "rtc.h"
#include "modem.h"
//#include "app_driver_manage.h"
//#include "reg_gpio.h"
#include "app_gpio.h"
#include "patch.h"
#include "sysctrl.h"
#include "rf_reg_settings.h"
#include "io_ctrl.h"
#include "arch_init.h"

static uint32_t io_retention_val;
static uint32_t io_retention_dir;

#ifdef DO_NOT_WAKEUP
static void do_not_wakeup()
{
    RTC_MATCHCNT_SET(0xffffffff);
    NVIC_DisableIRQ(BLE_LP_IRQn);
    uint32_t i;
    for(i = 0 ;i<2000; ++i) __NOP();
}
#endif

uint32_t get_real_rtc()
{
    uint32_t last = RTC_CURRENTCNT_GET();
    uint32_t current;
    do
    {
        current = RTC_CURRENTCNT_GET();
    }
    while(last == current);
    return current;
}

static void sync_with_ble_sleep(uint8_t step)
{
    if(step == 1)
    {
        clk_gate_awo(AWO_CLKG_SET_BLE);
        while(!ble_deep_sleep_stat_getf());
    }else if(step == 2)
    {
        /* wait until mac register latched by 32k clock */
        while(0 == ble_deepsldur_getf());        
        clk_gate_awo(AWO_CLKG_CLR_BLE);
    }
}

void wait_rtc_until(uint32_t target)
{
    while(target > RTC_CURRENTCNT_GET());
}

static void switch_to_32m_rc(uint32_t current_time)
{
    wait_rtc_until(current_time+1);
    sysc_awo_o_ana_16m_selrc_setf(1);
}

static void close_32m_xtal()
{
    sysc_awo_o_16m_xtal_en_setf(0);
}

static void deep_sleep_io_state_set()
{
    io_retention_val = io_out_read_all() &(~io_mngt.util_io_mask) | io_mngt.util_io_ret_val;
    io_retention_dir = io_dir_read_all() &(~io_mngt.util_io_mask) | io_mngt.util_io_ret_dir;
    io_write_all(io_retention_val);
    io_dir_write_all_noie(io_retention_dir);
    pshare_reset_to_gpio();
    sysc_awo_gpio_ie_set(io_mngt.deep_sleep_ie);
}

static void pwr_pwm_setting_for_sleep()
{
    if(sysctrl_pwr_pwm_2_sleep_en_get())
    {
        sysc_awo_pwm2_fc_h_setf(1);
    }else
    {
        sysc_awo_pwm2_fc_h_setf(0);
    }
}

static void pwr_pwm_setting_for_active()
{
    sysc_awo_pwm2_fc_h_setf(1);
}

static void deepsleep_prepare()
{
    uint32_t current_time =enable_32m_rc();
    sysc_awo_dr_16m_rcosc_en_setf(0); // 32m rc en ctrl by pmu
    NVIC->ICER[0] = ~((1<<BLE_LP_IRQn)|(1<<RTC_IRQn)|(1<<EXT_INTR_IRQn)); // disable all ints except WIC ints
    ble_lp_intr_clr();
    ble_lp_irq_enable();
    flash_deep_power_down();
    pwr_pwm_setting_for_sleep();
    sync_with_ble_sleep(1);
    sysc_awo_o_clk_sel_hbus0_setf(1); // set ahb to 32m
    reg_pll_enable(0);
    switch_to_32m_rc(current_time);
    close_32m_xtal();
    #if (defined(DEBUGGER_ATTACHED)&&(DEBUGGER_ATTACHED==1))
    sysc_awo_io_stat_ret_pmu_setf(0);
    #else
    sysc_awo_io_stat_ret_pmu_setf(1);
    SCB->SCR |= (1<<2);
    #endif
    deep_sleep_io_state_set();
    sync_with_ble_sleep(2);
}

void pre_deepsleep_processing_mp()
{
    deepsleep_prepare();
    #ifdef DO_NOT_WAKEUP
    do_not_wakeup();
    #endif
}

static uint32_t config_and_enable_32m_xtal()
{
    reg_set_xtal_current_startup();
    sysc_awo_o_16m_xtal_en_setf(1);
    return get_real_rtc();
}

static void gpio_status_restore()
{
    io_write_all(io_retention_val);
    io_dir_write_all_noie(io_retention_dir);
    sysc_awo_gpio_ie_set(io_mngt.active_ie);
    pshare_reset_to_gpio();
    sysc_awo_io_ctrl_sel_setf(0);//restore from IO retention
}

void flash_recovery()
{
    flash_wakeup();
    #if(FLASH_XIP == 1)
    cache_config();
    cache_disable();
    #endif
}

void post_deepsleep_processing_mp()
{
    uint32_t current_time = config_and_enable_32m_xtal();
    pwr_pwm_setting_for_active();
    gpio_status_restore();
    clk_gate_clr_all_clk();
    system_recovery();
    flash_recovery();
    switch_to_32m_xtal(current_time);
    sysctrl_close_32m_rc();
    sysctrl_set_ahb_apb_blemac_clk();
    SCB->SCR &= ~(1<<2);
}

