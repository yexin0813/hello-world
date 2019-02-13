/**
 ****************************************************************************************
 *
 * @file sys_sleep.c
 *
 * @brief Implementation of sys_sleep.c. Sleep and wakeup process of apollo_00
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

#include "bx_config.h"
#include "apollo_00.h"
#include "plf.h"
//#include "flash_cache.h"
//#include "flash.h"
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
//#include "reg_rtc.h"
//#include "rtc.h"
#include "modem.h"
//#include "app_driver_manage.h"
#include "reg_gpio.h"
#include "patch.h"
#include "sysctrl.h"
#include "rf_reg_settings.h"
#include "swint.h"
//#include "adc_bat_monitor.h"
//#include "adc_tem_sensor.h"
#include "rf.h"
#include "periph_mngt.h"
#include "ke_event.h"
#include "ke.h"
blemac_stat_t mac_status = active_clk;

#if (defined(CFG_DYNAMIC_UPDATE)&&(CFG_DYNAMIC_UPDATE==1))
#define SECONDS_TO_RTC(a) (30*1000*(a))
/*
static struct
{
    uint32_t battery_vol;
    uint32_t temperature;
}prev_update_time;
*/
struct dynamic_update_handler
{
    uint8_t index;
    dynamic_update_cb cb;
};
/*
static void sys_low_battery_config(uint16_t bat_volt);
static struct dynamic_update_handler dynamic_handler_array[temp_update_max] =
{
    {bat_update_io_level, NULL},
    {bat_update_low_volt, sys_low_battery_config},
    {temp_update_deri_calib, NULL},
};
const static uint16_t temp_adc_val[temp_highest] = {422, // -40
                                                    429, // -30
                                                    436, // -20
                                                    442, // -10
                                                    449, // 0
                                                    456, // 10
                                                    463, // 20
                                                    470, // 30
                                                    478, // 40
                                                    485, // 50
                                                    492, // 60
                                                    499, // 70
                                                    507};// 80
static inline uint32_t delta_val(uint32_t cur, uint32_t prev)
{
    uint32_t delta;
    if(cur < prev)
    {
        delta = (~0) - prev + cur + 1;
    }
    else
    {
        delta = cur - prev;
    }
    return delta;
}
void sys_param_periodic_update(void)
{
    uint32_t current_time = rtc_currentcountervalue_getf();
    uint16_t bat_volt_cur = 0;
    uint16_t temp_cur = 0;
    uint32_t delta_bat_time = 0;
    uint32_t delta_temp_time = 0;

    delta_bat_time = delta_val(current_time, prev_update_time.battery_vol);
    delta_temp_time = delta_val(current_time, prev_update_time.temperature);

    if(delta_bat_time > SECONDS_TO_RTC(BATTERY_VOLTAGE_UPDATE_SECONDS))
    {
        prev_update_time.battery_vol = current_time;
        bat_volt_cur = adc_battery_level();
    }
    if(delta_temp_time > SECONDS_TO_RTC(TEMPERATURE_UPDATE_SECONDS))
    {
        prev_update_time.temperature = current_time;
        temp_cur = adc_temperature_level();
    }

    for(uint8_t i = 0; i < temp_update_max; i++)
    {
        dynamic_update_cb cb = dynamic_handler_array[i].cb;
        if(cb != NULL)
        {
            if(i < bat_update_max)
            {
                cb(bat_volt_cur);
            }
            else
            {
                cb(temp_cur);
            }
        }
    }

    uint32_t finish_time = rtc_currentcountervalue_getf();
    BX_ASSERT(delta_val(finish_time, current_time) <= XTAL_STARTUP_TIME);
}

static void sys_low_battery_config(uint16_t bat_volt)
{
    if(bat_volt >= LOW_VOLT_ADC_VAL)
    {
        rf_mac_low_battery_config();
        rf_low_battery_config();
    }
}
*/

#endif


void ble_lp_intr_clr()
{
    #if HW_BX_VERSION == 00
    sysc_awo_blelp_inrp_clr_set(0x3);
    #elif HW_BX_VERSION == 01
    sysc_awo_ble_lp_intr_clr_setf(0x3);
    #else
    #error ERROR: ERROR to define HW_BX_VERSION
    #endif
}

void ble_lp_irq_enable()
{
    #if HW_BX_VERSION == 00
    sysc_awo_blelp_inrp_en_setf(0x3);
    #elif HW_BX_VERSION == 01
    sysc_awo_ble_lp_intr_msk_setf(0x3);
    #else
    #error ERROR: ERROR to define HW_BX_VERSION
    #endif
}

void ble_lp_irq_disable()
{
    #if HW_BX_VERSION == 00
    sysc_awo_blelp_inrp_en_setf(0);
    #elif HW_BX_VERSION == 01
    sysc_awo_ble_lp_intr_msk_setf(0);
    #else
    #error ERROR: ERROR to define HW_BX_VERSION
    #endif
}


/**
 * @brief do ble task.
 */
void ble_schedule()
{
    if(mac_status == sleep_low_power_clk && ke_event_get_all())
    {
        ble_soft_wakeup();
    }
    if(mac_status==active_clk)
    {
        // schedule all pending events
        rwip_schedule();
    }
}

/**
 * @brief get sleep type. Sometimes we should not power off(just WFI), sometimes we can go deep sleep. It depends, so we should get the sleep type and do different process
 * @return return a struct of sleep type.
 */
uint8_t sleep_prepare_and_check()
{
    uint8_t sleep_type = 0;
    if(mac_status==active_clk)
    {
        sleep_type = rwip_sleep();
        if(sleep_type & BLE_DEEP_SLEEP)
        {
            mac_status = sleep_low_power_clk;
        }
    }else if(mac_status == sleep_low_power_clk)
    {
        sleep_type = ke_sleep_check() ? PROCESSOR_SLEEP | BLE_DEEP_SLEEP : 0 ;
    }else
    {
        BX_ASSERT(0);
    }
    bool busy = periph_domain_busy() || cpu_domain_busy();
    if(busy)
    {
        sleep_type &= ~SYS_DEEP_SLEEP;
    }else
    {
        sleep_type |= SYS_DEEP_SLEEP;
    }
    return sleep_type;
}

void ble_soft_wakeup()
{
    LOG(LOG_LVL_INFO,"soft wakeup\n");
    clk_gate_awo(AWO_CLKG_SET_BLE);
    ble_soft_wakeup_req_setf(1);
    while(mac_status!=active_clk)
    {
        __WFI();
    }
}

void ble_lp_isr_mp()
{
    clk_gate_awo(AWO_CLKG_SET_BLE);
    modem_init();
    ble_lp_irq_disable();
    mac_status = wakeup_requested; 
    NVIC_EnableIRQ(BLE_MAC_IRQn);
    uint32_t redundant_time = 0;
    while(NVIC_GetPendingIRQ(BLE_MAC_IRQn)==0)           //wait for BLE_MAC_IRQ
    {
        ++redundant_time;
    }
    BX_ASSERT(redundant_time);
}

void BLE_LP_IRQHandler(void)
{
    ble_lp_isr_mp();
}

uint32_t enable_32m_rc()
{
    sysc_awo_dr_16m_rcosc_en_setf(1);
    sysc_awo_reg_16m_rcosc_en_setf(1);
    return get_real_rtc();
}

void system_recovery()
{
    SWINT_SYS_INT_EN();
    patch_init();
    periph_recovery(&periph_domain_recovery_mngt);
    periph_recovery(&cpu_domain_recovery_mngt);
    #if (defined(CFG_DYNAMIC_UPDATE)&&(CFG_DYNAMIC_UPDATE==1))
//    sys_param_periodic_update();
    #endif
}

void switch_to_32m_xtal(uint32_t current_time)
{
    wait_rtc_until(current_time+ XTAL_STARTUP_TIME);
    reg_set_xtal_current_normal();
    wait_rtc_until(current_time+ XTAL_STARTUP_TIME + 1);
    sysc_awo_o_ana_16m_selrc_setf(0);
}

#ifdef CFG_LOWEST_CURRENT
static void first_deep_sleep(void)
{
    SCB->SCR |= (1<<2);
	awo_pd_onoff_sw(PD_PER|PD_BLE_MAC, PD_POWER_DOWN);
	awo_pd_onoff_sw(PD_PER|PD_BLE_RF|PD_BLE_MAC|PD_SRAM0|PD_SRAM1|PD_SRAM2|PD_SRAM3|PD_SRAM4|PD_SRAM5|PD_SRAM6, PD_POWER_ON);

	rtc_Init();
	rtc_SetConfig(320, 0); // sleep 10ms
	rtc_Run(NULL);

    uint32_t i;

	for(i=0;i<1000;i++)                              //
			__NOP();

    __asm("cpsid i");

	__asm("wfi");

	SCB->SCR &= ~(1<<2);
	rtc_DisableIrq();
	rtc_ClearIrq();
	awo_reset_signal(SRST_RTC_CLR);
    awo_reset_signal(SRST_RTC_SET); // reset RTC
	rtc_rtc_cmr_set(1);
    WAIT_RTC_RST(1); // wait RTC reset completed
    rtc_rtc_cmr_set(0); // restore matchcounter with 0
}

void set_vdd_in_systeminit(void)
{
    extern void intc_init(void);
	intc_init();
	GLOBAL_INT_START();
    awo_vdd_value_set(0);
	first_deep_sleep();
	GLOBAL_INT_STOP();
}
#endif

//#define TEST_SLEEP

#ifdef TEST_SLEEP

void sleep_test()
{
    uint32_t i;
    *( ( volatile uint32_t * )  0x130000 ) = 0x1234;
    __ASM volatile ("wfi");
    for(i=0;i<1500;i++)                              //
        __NOP();
    *( ( volatile uint32_t * )  0x130000 ) = 0x8888;
    while(1);
}

void rtc_interrpt()
{
	*( ( volatile uint32_t * )  0x130108 ) = *( ( volatile uint32_t * )  0x130108 ) +1;

}

void test_wakeup_by_rtc()
{
	uint16_t i,j;
	*( ( volatile uint32_t * )  0x130100 ) = 0x0;
	*( ( volatile uint32_t * )  0x130108 ) = 0;

    rtc_Init();
	rtc_SetConfig(32000*5, 0);
	rtc_Run(rtc_interrpt);


    while(1)
    {
    	GLOBAL_INT_DISABLE();
    	*( ( volatile uint32_t * )  0x130104 ) = 0x55;
    	slp_sleep_process();
        *( ( volatile uint32_t * )  0x130104 ) = 0x66;
        __asm("wfi");
        *( ( volatile uint32_t * )  0x130100 ) = *( ( volatile uint32_t * )  0x130100 ) + 1;
        slp_wakeup_process();
        *( ( volatile uint32_t * )  0x130104 ) = 0x33;
//        rtc_rtc_eoi_getf();
        *( ( volatile uint32_t * )  0x130104 ) = 0x44;
        GLOBAL_INT_RESTORE();
//        while(1);
        for(j=0;j<30000;j++)
        	for(i=0;i<16*120;i++)
                __NOP();
    }


}

#include "gpio.h"
#include "clk_gate.h"
void toggle_gpio(void)
{

	struct gpioConfigStruct gpio04;
	clk_gate_per_g1(PER_CLKG_SET_GPIO);
	gpio_InitN(gpio0_4);

	memset(&gpio04, 0, sizeof(gpio04));
	// cfg3_5: output/high
	gpio04.debounce = gpio_Deb;
	gpio04.dir = gpio_Output;
	gpio04.value = gpio_Low;

	gpio_SetConfig(gpio0_4, &gpio04);
    while(1)
	{
//    	gpio_SetOutputValue(gpio0_4,gpio_High);
//    	gpio_SetOutputValue(gpio0_4,gpio_Low);
    }

}
#endif
///@}
