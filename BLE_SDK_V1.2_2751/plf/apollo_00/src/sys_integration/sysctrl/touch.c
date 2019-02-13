/*
 * touch.c
 *
 *  Created on: 2018Äê9ÔÂ26ÈÕ
 *      Author: mingzhou
 */
#include "bx_config.h"
#include "rf_reg_settings.h"
#include "io_ctrl.h"
#include "touch.h"
#include "apollo_00.h"
#include "reg_sysc_awo.h"

#include "log.h"

#define TOUCH_INTERVAL  0xff*25 // number of 32K clock cycle
#define TOUCH_NUM       8
#define TOUCH_DELAY     3
#define TOUCH_THRETHOLD(x) ((TOUCH_NUM+1)*(x)) // configurable?

#define TOUCH_EXT_INT 5

void touch_init(void)
{
    reg_config_touch();
}

void touch_start(Touch_param_t* param)
{
    BX_ASSERT(param->threthold < 64);
    sysc_awo_touch_itv_setf(TOUCH_INTERVAL); // 5Hz
    sysc_awo_touch_scan_num_setf(TOUCH_NUM);
    sysc_awo_touch_scan_dly_setf(TOUCH_DELAY);
    sysc_awo_touch_thr_setf(TOUCH_THRETHOLD(param->threthold));
    if(param->int_en)
    {
        uint32_t ext_int_cfg = sysc_awo_ext_inrp_config_get();
        ext_set_touch_cb(param->cb);
        ext_int_cfg &= ~(0x3<<(2*TOUCH_EXT_INT + 8));
        ext_int_cfg |=  EXT_INT_TRIGGER_POS_EDGE<<(2*TOUCH_EXT_INT + 8);
        ext_int_cfg |= 1<<TOUCH_EXT_INT;

        sysc_awo_ext_inrp_config_set(ext_int_cfg);
        NVIC_EnableIRQ(EXT_INTR_IRQn);
    }
    sysc_awo_touch_en_setf(1);                  //enable touch
}

void touch_stop(uint8_t disable_ext_int)
{
    sysc_awo_touch_en_setf(0);
    if(!disable_ext_int)
    {
        NVIC_DisableIRQ(EXT_INTR_IRQn);
    }
}

static void touch_test_cb(void)
{
    LOG(LOG_LVL_INFO, "Touching...\n");
}
void touch_test(void)
{
    Touch_param_t param;
    param.int_en = 1;
    param.threthold = 30;
    param.cb = touch_test_cb;

    touch_init();
    touch_start(&param);
    while(*(volatile uint32_t*)0);
}
