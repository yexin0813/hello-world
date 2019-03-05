 
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "bx_config.h"
#include "osapp_config.h"
#include "rwip_config.h"
#include <stdbool.h>
#include <stdint.h>
#include "task_init.h"
#include "rwip.h"
#include "bx_dbg.h"
#include "log.h"
#include "arch.h"
#include "ble_task.h"
#include "osapp_task.h"
#include "gap.h"
#include "gapm_task.h"
#include "apollo_00.h"
#include "awo.h"
#include "rtc.h"
#include "log.h"
#include "rst_gen.h"
#include "plf.h"
#include "reg_sysc_awo.h"
#include "sys_sleep.h"


#include "app_uart.h"
#include "m_uart.h"
#include "clk_gate.h"
#include "m_flash.h"
#include "m_timer.h"
#include "logic/fsm.h"
#include "system.h"
#include "m_uart.h"
#include "m_timer.h"
#include "sensor_operation_afs121n.h"
#include "io_ctrl.h"
#include "app_pwm.h"
#include "string.h"
#include "plf.h"


#include "Hardware.h"
#include "common_lock_speaker.h"
#include "common_lock_mfrc.h"
#include "common_lock_oled.h"
#include "common_lock_led.h"
#include "common_lock_bs81x.h"
#include "common_lock_sensor3403.h"





#if defined( INCLUDE_uxTaskGetStackHighWaterMark )
extern void stk_chk_dbg( void );
#endif

/*
 * DEFINES
 ****************************************************************************************
 */
#define APP_MSG_TX_QUEUE_SIZE     3    
#define APP_MSG_RX_QUEUE_SIZE     8    

#if (RC32K_USED)
extern uint32_t rc_freq;
#define portRTC_FREQUENCY rc_freq
#else
#if (HZ32000)
#define portRTC_FREQUENCY 32000
#else
#define portRTC_FREQUENCY 32768
#endif
#endif

void test_task(void *params);
void transmit_to_ble_client_task(void *parameter);
void app_task(void *parameter);


static msg_queue_t msg_queue;

extern const task_table_info_t extra_task_table_info;
static const task_table_t default_task_table[] =
{
/*	  IsUse  TaskName	            Prio	         Stksize			Arg    Entry	  handler */
/* ----------------------------------------------------------------- */
    { true, "BLE_TASK",  OS_PRIORITY_BLE_TASK,	 BLE_TASK_STACK_SIZE,  &msg_queue,  ble_stack_task,  &handler_ble_stack_task },  
    { true, "APP_TASK",  OS_PRIORITY_APP_TASK,   APP_TASK_STACK_SIZE,  &msg_queue,  osapp_task,  &handler_osapp_task },
};
const task_table_info_t core_task_table_info = ARRAY_INFO(default_task_table);
__attribute__((weak)) const task_table_info_t extra_task_table_info = {NULL,0};

static uint32_t ulTimerCountsForOneTick;
static uint32_t RTCWakeupDelay;
static uint32_t rtcMatchCount;
#define RTC_INITIAL_VAL 0xfff00000


/*
 * EXPORTED FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */
#if defined( INCLUDE_uxTaskGetStackHighWaterMark )
void stk_chk_dbg( void )
{
	unsigned portBASE_TYPE uxHighWaterMark;
	uint8_t task_num,i;

	task_num = core_task_table_info.table_size;
    const task_table_t *task_table = core_task_table_info.task_table;
	for(i=0; i < task_num; i++)
	{
		if(task_table[i].is_used == true)
		{
			BX_ASSERT(task_table[i].task_handle != NULL);									// task should already been initialized

			uxHighWaterMark = uxTaskGetStackHighWaterMark(*task_table[i].task_handle);		// null means check current task stack
			LOG(LOG_LVL_INFO,"%s stk remain 0x%x*4Byte\n",task_table[i].task_name,uxHighWaterMark);
		}
	}

    
    task_num = extra_task_table_info.table_size;
    task_table = extra_task_table_info.task_table;
    for(i=0; i < task_num; i++)
    {
        if(task_table[i].is_used == true)
        {
            BX_ASSERT(task_table[i].task_handle != NULL);                                   // task should already been initialized

            uxHighWaterMark = uxTaskGetStackHighWaterMark(*task_table[i].task_handle);      // null means check current task stack
            LOG(LOG_LVL_INFO,"%s stk remain 0x%x*4Byte\n",task_table[i].task_name,uxHighWaterMark);
        }
    }

}
#endif


void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if malloc failed */
        LOG(LOG_LVL_INFO,"Malloc Failed\n");
        __BKPT(2);
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* vApplicationStackOverflowHook will only be called if stackoverflow */
    LOG(LOG_LVL_INFO,"StackOverFlow\n");
    __BKPT(2);
}

/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}


static void rtos_tasks_create(const task_table_info_t * task_table_info)
{
    uint8_t task_num = task_table_info->table_size;
    const task_table_t *task_table = task_table_info->task_table;
    uint8_t i;
    for( i = 0; i < task_num; i++ )
    {
        if( (task_table[i].is_used) && (task_table[i].entry_proc != NULL) )
        {
            xTaskCreate(task_table[i].entry_proc,task_table[i].task_name,task_table[i].stack_size,task_table[i].param,task_table[i].priority,task_table[i].task_handle);
            BX_ASSERT(task_table[i].task_handle != NULL);
        }
    }
}
void rtos_task_init()
{
	
    msg_queue.cmd_q = xQueueCreate(APP_MSG_TX_QUEUE_SIZE,sizeof(AHI_MSGBOX_t));
    msg_queue.rsp_q = xQueueCreate(APP_MSG_RX_QUEUE_SIZE,sizeof(AHI_MSGBOX_t));
	
	// Here : Init variables and state before all the freertos tasks run

	////////////////////////////////////////////////////////////////////
	
    rtos_tasks_create(&core_task_table_info);
    rtos_tasks_create(&extra_task_table_info);
	
	
	    /* For test */
    xTaskCreate(test_task,"Test Task",1024,NULL,(configMAX_PRIORITIES  - 2),NULL);
	
    
    vTaskStartScheduler();

}

void xPortSysTickHandler( void );
void vPortOSTick_IRQ()
{
    RTC_INT_CLR();
    uint32_t prev_rtc_matchcnt = rtcMatchCount;
    rtcMatchCount += ulTimerCountsForOneTick;
    /* --start-- for breakpoint debug use */
    bool wrap = prev_rtc_matchcnt > rtcMatchCount ? true : false;
    if(wrap == false)
    {
        uint32_t current_rtc_val = RTC_CURRENTCNT_GET();
        if(current_rtc_val >= rtcMatchCount)
        {
            LOG(LOG_LVL_WARN,"missing ticks\n");
            uint32_t step_ticks = (current_rtc_val - rtcMatchCount) / ulTimerCountsForOneTick + 1;
            rtcMatchCount += step_ticks * ulTimerCountsForOneTick;
        }
    }
    
    /* --end-- */
    RTC_MATCHCNT_SET(rtcMatchCount);
    xPortSysTickHandler();
}

void RTC_IRQHandler(void)
{
    vPortOSTick_IRQ();
}

void system_tick_init()
{
    RTCWakeupDelay = rwip_us_2_lpcycles(FREERTOS_WAKEUP_DELAY);
    ulTimerCountsForOneTick = portRTC_FREQUENCY / configTICK_RATE_HZ;
    rtcMatchCount = RTC_INITIAL_VAL + ulTimerCountsForOneTick;
    RTC_MATCHCNT_SET(rtcMatchCount);
    RTC_LOADCNT_SET(RTC_INITIAL_VAL);
    RTC_EN_WITH_INT();

}

static void clr_rtc_intr()
{   
    RTC_INT_CLR();
    NVIC_ClearPendingIRQ(RTC_IRQn);
}

void bxSuppressTicksAndSleep(uint32_t xExpectedIdleTime )
{
    taskENTER_CRITICAL();
    uint8_t sleep_type = sleep_prepare_and_check();
    eSleepModeStatus eSleepStatus = eTaskConfirmSleepModeStatus();
    if(eSleepStatus != eAbortSleep && sleep_type == (PROCESSOR_SLEEP|BLE_DEEP_SLEEP|SYS_DEEP_SLEEP))
    {
        uint32_t wakeupMatchValue; 
        uint32_t prevRTCMatchCount = rtcMatchCount - ulTimerCountsForOneTick;
        if(eSleepStatus == eStandardSleep)
        {
            wakeupMatchValue = prevRTCMatchCount + ulTimerCountsForOneTick*(xExpectedIdleTime+1) - RTCWakeupDelay;
        }else
        {
            wakeupMatchValue = prevRTCMatchCount - 1; // long enough before RTC IRQ raise
        }
        RTC_MATCHCNT_SET(wakeupMatchValue);
        clr_rtc_intr();
        pre_deepsleep_processing_mp();
        __WFI();
        post_deepsleep_processing_mp();
        bool rtc_int = RTC_INT_STAT_GET()? true : false;
        uint32_t RTCValueAfterSleep = RTC_CURRENTCNT_GET();
        if(eSleepStatus == eStandardSleep &&  rtc_int) 
        {
            BX_ASSERT((uint64_t)wakeupMatchValue + RTCWakeupDelay >RTCValueAfterSleep );
        }
        uint32_t RTCIncrement;
        if(RTCValueAfterSleep < prevRTCMatchCount)
        {
            RTCIncrement = (~0)-prevRTCMatchCount  + RTCValueAfterSleep + 1;
        }else
        {
            RTCIncrement = RTCValueAfterSleep - prevRTCMatchCount;
        }
        uint32_t OSTickIncrement = RTCIncrement /ulTimerCountsForOneTick;
        vTaskStepTick(OSTickIncrement);
        bool wrapping;
        if(rtcMatchCount<prevRTCMatchCount || rtcMatchCount > (~0) - ulTimerCountsForOneTick * OSTickIncrement)
        {
            wrapping = true;
        }else
        {
            wrapping = false;
        }
        rtcMatchCount += ulTimerCountsForOneTick * OSTickIncrement;
        RTC_MATCHCNT_SET(rtcMatchCount);
        clr_rtc_intr();
        if(wrapping == false)
        {
            BX_ASSERT(RTC_CURRENTCNT_GET()<=rtcMatchCount);
        }
    }else if(sleep_type & PROCESSOR_SLEEP)
    {
        __WFI();
    }
    taskEXIT_CRITICAL();
}




/*
 *Demo task test
 *By HansHuang on 09/10/18
 */
 
#define  GREEN_LED  (17)
#define  RED_LED    (16)
#define  BLUE_LED   (23)
extern int8_t adc_cp_RO ;
void test_task(void *params)
{
    S32 ch = 0x00;
    uint32_t old_adc_value = 0;
    int32_t newest_adc_value = 0;
    int32_t diff_times = 0;
    int32_t i = 0;
	  uint32_t cnt;
    
    //io_cfg_output(23);
   // io_pin_clear(23);
   
    timerInit();

	  hardwareInit();
	  HardwareInputInit();
    
	  //initDeviceInfo();
    
    uartInit(UART_BAUDRATE_57600);
	  Com_Lock_Init_Speaker();
	  Com_Lock_Init_Mfrc();
	  //Com_Lock_Init_OLED();
	  Com_Lock_Init_LED();
		Com_Lock_Init_Sensor();
		Com_Lock_Init_bs81x();


    //fsmInit();

#if DEBUG_PRINT
        debugPrint("start\r\n");
        debugPrint("sizeof(_SysParaTag) == %d\r\n",sizeof(_SysParaTag));
#endif
        //deviceReady();


        /* Time cost test */
        //timerRestart();
        
        //io_pin_set(23);
        //timerDelayMs(16);
        //io_pin_clear(23);
        //cost_us = timerElapsedUs();
        //debugPrint("~~~Delay time cost(us). = %u\n",cost_us);

        //xTaskCreate(transmit_to_ble_client_task,"BLE Transmit Task",128,NULL,(configMAX_PRIORITIES  - 2),NULL);

        xTaskCreate(app_task,"APP Task",128,NULL,(configMAX_PRIORITIES  - 2),NULL);

         /*
         io_cfg_output(22);
         io_pin_clear(22);

         
        io_cfg_output(GREEN_LED);
        io_pin_set(GREEN_LED);
        io_cfg_output(RED_LED);
        io_pin_set(RED_LED);
        io_cfg_output(BLUE_LED);
        io_pin_clear(BLUE_LED);
        */
#if 0
        BX_DELAY_US(1000*1000);

        app_adc_cp_sim();

        BX_DELAY_US(1000*1000);
        
        debugPrint("adc_cp_RO:%d\n\r",adc_cp_RO);

        BX_DELAY_US(1000*1000);

        
        old_adc_value = newest_adc_value = app_adc_gpadc_single_end(0)&0x01;

        while(1)
        {

          for (i = 0; i < 1000; i++){

            newest_adc_value = app_adc_gpadc_single_end(0)&0x01;
            if (old_adc_value != newest_adc_value){
                diff_times++;
                
                old_adc_value = newest_adc_value;
            }
            
            //debugPrint("ADC 0:%d\n\r",app_adc_gpadc_single_end(0));
            vTaskDelay(1);
        } 

          
        debugPrint("diff_times:%d\n\r",diff_times);
          
        vTaskDelay(1);
        }
        #endif

        while(1)
        {
           //if(uartReadByte(&ch))
					 //{
						//		uartSendBuff((uint8_t *)&ch,4);
					 //}
           //
             //  debugPrint("Rx:%x\n\r",ch);
           
               // vTaskDelay(1);
            
            //fsmTask(ch);

           //debugPrint("system tick:%d\n\r",systemTick());
          //vTaskDelay(10);
					//taskENTER_CRITICAL();
					//Com_Lock_Play_Voice(11);
					//for(cnt = 0;cnt <13;cnt++)
					//{
					//	Com_Lock_Switch_Off_LED(COMMON_LOCK_LED_ALL_KEY_OFF_INDEX);
					//	Com_Lock_Switch_On_LED(cnt);
					//}
					//Com_Lock_bs81x_Task();
					//taskEXIT_CRITICAL();
            
        }
    
     
    
}


extern uint8_t uart0_buf;
extern int32_t whole_bmpfile_size;
extern uint8_t *bmp_src_ptr ;
uint32_t ble_transmit_counter = 0;

void uart0_rx_callback(void *dummy,uint8_t status);
uint8_t rx_ble_cmd = 0x00;

void transmit_to_ble_client_task(void *parameter)
{

    while (1)
    {

        /*
        for (i = 0; i < 0xFFFFFFFF; i++)
        {
            uart0_buf = i%0xFF;
            uart0_rx_callback(NULL, 0);

        
            vTaskDelay(50);

         }*/

       
        //debugPrint("Rx:%x\n\r", rx_ble_cmd );
        
        if (rx_ble_cmd == 0x55)
        {
            
            rx_ble_cmd = 0x00;
            ble_transmit_counter = 0;
            uart0_buf = bmp_src_ptr[ble_transmit_counter++];
            debugPrint("Tx %d:%x\n\r",0, uart0_buf );
            
            uart0_rx_callback(NULL,0);
        }

        


        vTaskDelay(1);
    }

}


app_pwm_inst_t pwm0 = PWM_INSTANCE(0);



void app_task(void *parameter)
{
    S32 ch = 0x00;
    io_pin_set(GREEN_LED);
    io_pin_set(RED_LED);
    io_pin_set(BLUE_LED);
    io_cfg_output(GREEN_LED);
    io_cfg_output(RED_LED);
    io_cfg_output(BLUE_LED);

    //io_pin_clear(22);
    //io_cfg_output(22);



    /*9999,6664*/
    //pwm0.param.pin_num   = 22;
    //pwm0.param.high_time = 9999;
    //pwm0.param.low_time  = 9999;
    //app_pwm_init(&pwm0.inst);
   // app_pwm_start(&pwm0.inst);



    /*
    io_pin_clear(20);
    io_pin_clear(21);
    io_cfg_output(20);
    io_cfg_output(21);

    io_pin_set(21);
    Motor test
    */
    

    while (1)
    {
       /* pwm0.param.high_time = 9999;
        pwm0.param.low_time  = 9999;
        app_pwm_start(&pwm0.inst);*/
        

       // if(uart0ReadByte(&ch))
			 //{
				//		uart0SendBuff((uint8_t *)&ch,4);
			 //}
        //taskENTER_CRITICAL();
        //io_pin_clear(RED_LED);
			  //timerDelayMs(1);
        //vTaskDelay(20);
        //io_pin_set(RED_LED);
        //timerDelayMs(1);
				//Com_Lock_Oled_Task();   
			  //Com_Lock_Mfrc_Task();
			  //Com_Lock_Sensor_Task();
			  //taskEXIT_CRITICAL();
			  //timerDelayMs(1000);
			  //printRFID(); 

        
        //io_pin_clear(RED_LED);
				//timerDelayMs(20);
        //vTaskDelay(20);
        //io_pin_set(RED_LED);

        //io_pin_clear(BLUE_LED);
				//timerDelayMs(20);
        //vTaskDelay(20);
        //io_pin_set(BLUE_LED);
			  //debugPrint("dyiuyicyi\n\r");
        //debugPrint("U");
        /*pwm0.param.high_time = 6664;
        pwm0.param.low_time  = 6664;

        app_pwm_start(&pwm0.inst);
        vTaskDelay(20);*/
				FrontTask();
        
    }
}
