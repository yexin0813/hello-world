/*
 * charger.c
 *
 *  Created on: 2018Äê8ÔÂ6ÈÕ
 *      Author: mingzhou
 */
#include <stddef.h>
#include "charger.h"
#include "bx_config.h"
#include "rf_reg_typedef.h"
#include "io_ctrl.h"
#include "periph_mngt.h"

static void (*charger_user_callback)(void) = NULL;

static void charger_sys_stat(void *inst,uint32_t sys_stat)
{
    switch(sys_stat)
    {
        case Charger_Init:
            periph_domain_stat.charger = 1;
        break;
        case Charger_Uinit:
            periph_domain_stat.charger = 0;
        break;
        default:
        break;
    }
}

void charger_init(void)
{
    io_ext_int_en(CHARGER_PIN, 1);
}

void charger_uninit(void)
{
    io_ext_int_en(CHARGER_PIN, 0);
}

void charger_setCur(uint8_t cur)
{
    hwp_rf_reg->rf_reg_1.Charger_Current = cur&0xf;
}

void charger_isr(void)
{
    if(!periph_domain_stat.charger)
    {
        charger_sys_stat(NULL, Charger_Init);
        hwp_rf_reg->rf_reg_1.Charger_EN = 0;
        io_ext_int_cfg(CHARGER_PIN, EXT_INT_TRIGGER_NEG_EDGE, charger_isr);
    }
    else
    {
        charger_sys_stat(NULL, Charger_Uinit);
        hwp_rf_reg->rf_reg_1.Charger_EN = 1;
        io_ext_int_cfg(CHARGER_PIN, EXT_INT_TRIGGER_POS_EDGE, charger_isr);
    }

    if(charger_user_callback)
    {
        charger_user_callback();
    }
}

void charger_config(uint8_t cur, void (*callback)())
{
    io_ext_int_cfg(CHARGER_PIN, EXT_INT_TRIGGER_POS_EDGE, charger_isr);
    charger_user_callback = callback;
    charger_setCur(cur);
}

