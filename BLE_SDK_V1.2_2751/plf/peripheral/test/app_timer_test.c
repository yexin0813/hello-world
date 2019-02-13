/*
 * app_timer_test.c
 *
 *  Created on: 2018Äê6ÔÂ28ÈÕ
 *      Author: jiachuang
 */

#include "app_timer.h"
#include "string.h"
#include "plf.h"
#include "log.h"
#include "clk_gate.h"


//instance
app_timer_inst_t timer0 = TIMER_INSTANCE(0);
app_timer_inst_t timer1 = TIMER_INSTANCE(1);


//gpio last status
uint8_t test_timer_led0=0;
uint8_t test_timer_led1=0;


///GPIO
void set_gpio_dir_out(uint32_t gpio_index)
{
    volatile uint32_t d_dir = *(volatile uint32_t *)0x20148004;
    d_dir |= (1<<(gpio_index));
    *(volatile uint32_t *)0x20148004 = d_dir;
}


void set_gpio_out_val(uint32_t port , uint32_t val)
{
    volatile uint32_t d_gpio = *(volatile uint32_t *)0x20148000;
    d_gpio &= ~(1<<(port));
    if(val)
    {
        d_gpio |= 1 << port;
    }
    *(volatile uint32_t *)0x20148000 = d_gpio;
}


//end of gpio

void app_timer_interrupt_callback0(void* param)
{
    test_timer_led0=~test_timer_led0;
    set_gpio_out_val(8,test_timer_led0);
}

void app_timer_interrupt_callback1(void* param)
{
    test_timer_led1=~test_timer_led1;
    set_gpio_out_val(9,test_timer_led1);
}

void init_timer_test_gpios(void)
{
    clk_gate_per_g1(PER_CLKG_SET_GPIO);
    set_gpio_dir_out(8);
    set_gpio_dir_out(9);
}

void app_timer_test(void)
{
    //toogle gpio init
    init_timer_test_gpios();
    //timer0
    timer0.param.auto_reload_value = (2000);
    timer0.param.callback = app_timer_interrupt_callback0;
    timer0.param.callback_param = 0;
    app_timer_init (&timer0.inst);
    app_timer_start(&timer0.inst);

    //timer1
    timer1.param.auto_reload_value = (20000);
    timer1.param.callback = app_timer_interrupt_callback1;
    timer1.param.callback_param = 0;
    app_timer_init (&timer1.inst);
    app_timer_start(&timer1.inst);

    while(*(volatile uint32_t *)0 != 0) ;
}

