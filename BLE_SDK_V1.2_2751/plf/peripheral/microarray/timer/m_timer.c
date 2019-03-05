/*
 * timer.c
 *
 *  Created on: 2017/08/15
 *      Author: kevin song
 */
#include "string.h"
#include "clk_gate.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "reg_timer.h"
#include "field_manipulate.h"
#include "app_timer.h"
#include "bx_dbg.h"
#include "ll.h"
#include "rwip.h"
#include "log.h"
#include "co_utils.h"
#include "string.h"
#include "plf.h"
#include "rf_reg_typedef.h"
#include "reg_sysc_cpu.h"
#include "rtc.h"




#define TIMER0_COUNTER_RELOAD_VALUE  (2000UL)
#define TIMER1_COUNTER_RELOAD_VALUE  (2000UL)


app_timer_inst_t timer0 = TIMER_INSTANCE(0);
app_timer_inst_t timer1 = TIMER_INSTANCE(1);



volatile uint32_t timer1_delay_ms = 0;
volatile unsigned int tick_count = 0;


volatile uint32_t pre_tick = 0;
volatile uint32_t post_tick = 0;


void app_timer0_interrupt_callback(void* param)
{
    tick_count ++ ;

}


void app_timer1_interrupt_callback(void* param)
{
    timer1_delay_ms++;
}



///////////////////////////////////////////////////////////////////////////////
//TIMER2初始化函数
///////////////////////////////////////////////////////////////////////////////
void timerInit(void)
{
    timer0.param.auto_reload_value = (TIMER0_COUNTER_RELOAD_VALUE);
    timer0.param.callback = app_timer0_interrupt_callback;
    timer0.param.callback_param = 0;
    app_timer_init (&timer0.inst);
    app_timer_start(&timer0.inst);


}




///////////////////////////////////////////////////////////////////////////////
//开始计算时间间隔
///////////////////////////////////////////////////////////////////////////////
void timerRestart(void)
{
#if 0
    periph_inst_handle_t hdl;

    hdl = &(timer0.inst);
    
    app_timer_inst_t *inst = CONTAINER_OF(hdl, app_timer_inst_t, inst);


    reg_timer_t *reg = timer0.reg;
	start_int  = tick_count;
	start_frac = FIELD_RD((&reg->ch[inst->idx]), TIMERCURRENTVALUE, TIMER_TIMERCURRENTVALUEREGISTER);
#endif
    pre_tick = RTC_CURRENTCNT_GET();
}


///////////////////////////////////////////////////////////////////////////////
//计算从timerRestart执行到现在的时间间隔，单位微秒
///////////////////////////////////////////////////////////////////////////////
uint32_t  timerElapsedUs(void)
{
#if 0
    uint32_t period   = 0x00;
    uint32_t us_value = 0x00;
    uint32_t end_int  = 0;
    uint32_t end_frac = 0;
    int32_t  diff_frac = 0;
    int32_t  tmp = 0;
    
    reg_timer_t *reg = timer0.reg;


    
    periph_inst_handle_t hdl;

    hdl = &(timer0.inst);
    
    app_timer_inst_t *inst = CONTAINER_OF(hdl, app_timer_inst_t, inst);

    
    period = TIMER0_COUNTER_RELOAD_VALUE;


	
    end_frac  = FIELD_RD((&reg->ch[inst->idx]), TIMERCURRENTVALUE, TIMER_TIMERCURRENTVALUEREGISTER);
    diff_frac =  (signed int)start_frac  - (signed int)end_frac;
    tmp       = (diff_frac*(signed int)1000)/(signed int)period;
	end_int   = tick_count;
    us_value  = (end_int - start_int) * 1000 + tmp;   

    
	return us_value;
#endif

    uint32_t round_tick = 0x00;
    uint32_t us_value   = 0;

    post_tick = RTC_CURRENTCNT_GET();

    if (post_tick < pre_tick)
    {
        round_tick = 0xFFFFFFFF - pre_tick;
        round_tick += post_tick;
        us_value = round_tick * 32;
    }else{
        us_value = (post_tick - pre_tick) * 31;
    }


    /* 50 us tolerance */
    //us_value = us_value -((us_value / 1000) *30);


    return us_value;

}



///////////////////////////////////////////////////////////////////////////////
//延时指定毫秒
///////////////////////////////////////////////////////////////////////////////
void timerDelayMs(uint32_t ms)
{
    #if 0
    timer1_delay_ms = 0;
    timer1.param.auto_reload_value = (TIMER1_COUNTER_RELOAD_VALUE);
    timer1.param.callback = app_timer1_interrupt_callback;
    timer1.param.callback_param = 0;
    app_timer_init (&timer1.inst);
    app_timer_start(&timer1.inst);





    while (timer1_delay_ms < ms)
    {
        ;
    }

    app_timer_stop(&timer1.inst);
    #endif

    uint32_t current_tick = 0x00;
    uint32_t delay_tick   = 0;
    uint32_t part2_tick   = 0;


    delay_tick = (ms*1000) / 31;

    current_tick = RTC_CURRENTCNT_GET();


    part2_tick = 0xFFFFFFFF - current_tick;
    part2_tick = delay_tick - part2_tick;
    

    if ((0xFFFFFFFF - current_tick)<delay_tick){
        while(RTC_CURRENTCNT_GET() >= current_tick);
        while(RTC_CURRENTCNT_GET()<=part2_tick);
    }else{
        while (RTC_CURRENTCNT_GET()<= (current_tick+delay_tick));
    }

}

//时钟节拍  1ms
uint32_t systemTick(void)
{
/*
    uint32_t sys_tick = 0;

    sys_tick = (RTC_CURRENTCNT_GET()*31);
    sys_tick /= 1000;
    
	return sys_tick;
	*/

    return tick_count;
}



