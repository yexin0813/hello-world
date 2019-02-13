/*
 * app_pwm_test.c
 *
 *  Created on: 2018Äê6ÔÂ28ÈÕ
 *      Author: jiachuang
 */

#include "app_pwm.h"
#include "string.h"
#include "plf.h"



//instance
app_pwm_inst_t pwm0 = PWM_INSTANCE(0);
app_pwm_inst_t pwm1 = PWM_INSTANCE(1);
app_pwm_inst_t pwm2 = PWM_INSTANCE(2);
app_pwm_inst_t pwm3 = PWM_INSTANCE(3);
app_pwm_inst_t pwm4 = PWM_INSTANCE(4);




static void app_pwm_delay_ms(uint32_t ms)
{
    while(ms --)
    {
        BX_DELAY_US(1000);
    }
}



void app_pwm_test_init(void)
{
    //PWM0
    pwm0.param.pin_num   = 8;
    pwm0.param.high_time = 100;
    pwm0.param.low_time  = 100;
    app_pwm_init(&pwm0.inst);
    //PWM1
    pwm1.param.pin_num   = 9;
    pwm1.param.high_time = 100;
    pwm1.param.low_time  = 200;
    app_pwm_init(&pwm1.inst);
    //PWM2
    pwm2.param.pin_num   = 10;
    pwm2.param.high_time = 200;
    pwm2.param.low_time  = 100;
    app_pwm_init(&pwm2.inst);
    //PWM3
    pwm3.param.pin_num   = 11;
    pwm3.param.high_time = 100;
    pwm3.param.low_time  = 400;
    app_pwm_init(&pwm3.inst);
    //PWM4
    pwm4.param.pin_num   = 12;
    pwm4.param.high_time = 1;
    pwm4.param.low_time  = 1;
    app_pwm_init(&pwm4.inst);
}


void app_pwm_output_test(void)
{
    app_pwm_test_init();
    while(*(__IO uint32_t *)0 != 0)
    {
        //change
        pwm0.param.high_time = 100;
        pwm0.param.low_time  = 100;
        app_pwm_start(&pwm0.inst);app_pwm_delay_ms(100);
        //start
        app_pwm_start(&pwm0.inst);app_pwm_delay_ms(100);
        app_pwm_start(&pwm1.inst);app_pwm_delay_ms(100);
        app_pwm_start(&pwm2.inst);app_pwm_delay_ms(100);
        app_pwm_start(&pwm3.inst);app_pwm_delay_ms(100);
        app_pwm_start(&pwm4.inst);app_pwm_delay_ms(100);
        //change
        pwm0.param.high_time = 400;
        pwm0.param.low_time  = 400;
        app_pwm_start(&pwm0.inst);app_pwm_delay_ms(100);
        //stop
        app_pwm_stop (&pwm0.inst);app_pwm_delay_ms(100);
        app_pwm_stop (&pwm1.inst);app_pwm_delay_ms(100);
        app_pwm_stop (&pwm2.inst);app_pwm_delay_ms(100);
        app_pwm_stop (&pwm3.inst);app_pwm_delay_ms(100);
        app_pwm_stop (&pwm4.inst);app_pwm_delay_ms(100);
    }

}




