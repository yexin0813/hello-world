#define __RAM_CODE__
#include "bx_config.h"
#include "arch.h"      // architectural platform definitions
#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include <string.h>
#include "plf.h"
#include "rwip.h"      // RW SW initialization
#include "modem.h"
#include "boot.h"
#include "nvds.h"
#include "log.h"
#include "bx_dbg.h"
#include "rc_calib.h"
#include "patch.h"
#include "rst_gen.h"
#include "pshare.h"
#include "clk_gate.h"
#include "sysctrl.h"
#include "em_map.h"
#include "rf_reg_settings.h"
#include "app_dmac_wrapper.h"
#include "app_qspi_wrapper.h"
#include "app_hwecc_wrapper.h"
#include "rtc.h"
#include "swint.h"
#include "io_ctrl.h"
#include "sys_sleep.h"
#if (defined(FLASH_XIP)&&(FLASH_XIP==1))
#include "flash_cache.h"
#endif
#if (CFG_FREERTOS_SUPPORT==1)
#include "os_bridge.h" 
#endif
#include "compiler_flag.h"
#include "arch_init.h"

#define RAM_BASE        0x00100000
#define ROM_DATA_BASE   0x0012f5cc
#define RAM_LIMIT       0x00130000
#define BOOT_TUNNEL __attribute__((section("boot_tunnel"), zero_init))
///indicate bootram_init has run in bootram
#define BOOTINIT_FLAG       0x3399

/// Description of unloaded RAM area content
struct unloaded_area_tag
{
    // status error
    uint32_t error;
};

uint32_t error;
/// Pointer to access unloaded RAM area
struct unloaded_area_tag* unloaded_area;

uint32_t current_flash_offset BOOT_TUNNEL;
uint32_t nvds_base BOOT_TUNNEL;
uint32_t flash_op_mode BOOT_TUNNEL;

static void unloaded_area_init(void)
{

    // Map unloaded RAM area pointer onto RAM memory
     unloaded_area = (struct unloaded_area_tag*)RAM_UNLOADED_BASE;


    /*****************************************************************************
     ***********************        RESET REASON        **************************
     *****************************************************************************/

    // Check if reset is due to FW failure
    error = unloaded_area->error;
    
    // Clear value in unloaded area
    unloaded_area->error = RESET_NO_ERROR;

}

uint16_t get_stack_usage(void)
{
    return STACK_TOP - __get_MSP();
}

void platform_reset(uint32_t error)
{

    // Disable interrupts
    GLOBAL_INT_STOP();

    // Store information in unloaded area
    unloaded_area->error = error;

    if(error == RESET_AND_LOAD_FW || error == RESET_TO_ROM)
    {
        // Not yet supported
    	//TODO
    }
    else
    {
        // Restart whole chip
        #if HW_BX_VERSION == 00
        srst_awo(CHIP_SRST_AWO);
        #elif HW_BX_VERSION == 01
        sysc_awo_sft_rst_set(SRST_ALL_CLR);
        #else
        #error ERROR: ERROR to define HW_BX_VERSION
        #endif
        
    }
}

void SystemInit(uint32_t param0,uint32_t param1,uint32_t param2,uint32_t param3) 
{
    if(param3 != BOOTINIT_FLAG)    //indicate bootram has already run.
    {                                        
        current_flash_offset = 0;
        nvds_base = 0;
        flash_op_mode = 2;
    }else
    {
        current_flash_offset = param0;
        nvds_base = param1;
        flash_op_mode = param2;
        memset((uint8_t *)RAM_LIMIT,0,0x4000);
    }
    srst_per(UART0_SRST_PER);
    // Redirect Interrupt Vectors
    __DMB();
    SCB->VTOR = RAM_BASE;
    __DSB();

    memset((void *)EM_BASE_ADDR,0,EM_BLE_END);
    #ifndef ALL_IN_RAM
    // ROM Code RW ZI Section Initialization
    memset((void *)ROM_DATA_BASE,0,RAM_LIMIT-ROM_DATA_BASE);
    #endif
}

void cache_config()
{
    flash_cache_config(current_flash_offset);
}

void cache_enable()
{
    flash_cache_enable(flash_op_mode);
}

void cache_disable()
{
    flash_cache_disable();
}

static void sys_setup()
{
    sysctrl_set_ahb_apb_blemac_clk();
    rf_reg_settings_init_mp();
    rc_calib_start(true);
    rc_calib_end();
    sysctrl_32k_clk_init();
    srst_cpu(CACHE_SRST_CPU);
    clk_gate_clr_all_clk();
    pshare_reset_to_gpio();
    sysctrl_awo_init();
    io_init();
    app_dmac_init_wrapper();
    app_qspi_init_wrapper();
    #if(FLASH_XIP == 1)
    cache_config();
    cache_disable();
    #endif
    NVIC_ClearPendingIRQ(BLE_LP_IRQn);
    NVIC_ClearPendingIRQ(BLE_MAC_IRQn);
    NVIC_EnableIRQ(BLE_LP_IRQn);
    NVIC_EnableIRQ(BLE_MAC_IRQn);
    SWINT_SYS_INT_CLR();
    SWINT_SYS_INT_EN();
    rtc_init();
    app_hwecc_init_wrapper();
}


void soc_initialize()
{
    sys_setup();
    
    // Initialize unloaded RAM area
    #if (!(defined(BX_VERF)&&(BX_VERF==1)))
    unloaded_area_init();
    #endif
    
    LOG_INIT();
    LOG(LOG_LVL_INFO,"main\n");   
    
    patch_init();
    modem_init();    
    
    modem_dev_calib_in_main();
    
    nvds_init((uint8_t*)( nvds_base ),0);

}

void baremetal_blestack_init()
{
    RTC_EN_WITHOUT_INT();
    rwip_init(error);
    GLOBAL_INT_START();    
}

void soc_baremetal_loop()
{
    while(1)
    {
        uint8_t sleep_type;
        ble_schedule();
        GLOBAL_INT_DISABLE();
        sleep_type = sleep_prepare_and_check();
        if(sleep_type == (SYS_DEEP_SLEEP|BLE_DEEP_SLEEP|PROCESSOR_SLEEP))
        {
            pre_deepsleep_processing_mp();
            WFI();
            post_deepsleep_processing_mp();
        }else if(sleep_type & PROCESSOR_SLEEP)
        {
            WFI();
        }
        GLOBAL_INT_RESTORE();
    }    
}
#if defined(CFG_APP)
static void dummy_eif_cb(uint8_t *bufptr, uint32_t size, rwip_eif_callback callback, void* dummy)
{
    LOG(LOG_LVL_ERROR,"in dummy_eif\n");
}

static const struct rwip_eif_api *dummy_eif_get()
{
    static const struct rwip_eif_api dummy_eif =
    {
        dummy_eif_cb,
        dummy_eif_cb,
        (void (*)(void))dummy_eif_cb,
        (bool (*)(void))dummy_eif_cb,
    };
    return &dummy_eif;
}
#endif

const struct rwip_eif_api* rwip_eif_get(uint8_t type)
{
    const struct rwip_eif_api* ret = NULL;
    switch(type)
    {
        case RWIP_EIF_AHI:
        {
        #if (CFG_FREERTOS_SUPPORT==1)
            ret = os_get_eif_api();             //os_msg_api;
        #elif (defined(CFG_APP))
            ret = dummy_eif_get();
        #else
            ret = serial_eif_get();
        #endif
        }
        break;
        default:
        {
            ASSERT_INFO(0, type, 0);
        }
        break;
    }
    return ret;
}
