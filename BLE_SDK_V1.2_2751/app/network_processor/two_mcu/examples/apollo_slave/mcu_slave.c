/**
 ****************************************************************************************
 *
 * @file   mcu_slave.c
 *
 * @brief  .
 *
 * @author  jiachuang
 * @date    2018-10-25 16:30
 * @version <0.0.0.1>
 *
 * @license 
 *              Copyright (C) APOLLO Microelectronics 2018
 *                         ALL Right Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "mcu_slave.h"
#include "io_ctrl.h"
#include "app_uart.h"
#include "plf.h"

/*
 * DEFINES
 ****************************************************************************************
 */
#define USER_GPIO_EXTI_TX               15       //wake up MCU before tx
#define USER_GPIO_EXTI_RX               17      //wake up myself before receive rx
#define USER_GPIO_UART_CTS              20      //flow control
#define USER_GPIO_UART_RTS              21      //flow control
//LED
#define LED1_PIN_NUM        8   //GREEN
#define LED2_PIN_NUM        9   //RED
#define MCU_DEMO_LED_PIN    LED1_PIN_NUM

/*
 * LOCAL VARIABLE DEFINITIONS
 *****************************************************************************************/


/*
 * FUNCTIONS DECLARE
 ****************************************************************************************
 */
extern void h4tl_read_start_2();
extern app_uart_inst_t uart0;

/*
 *  INTERNAL FUNCTIONS
 ****************************************************************************************
 */

//SLAVE EXIT wake up myself
static void two_mcu_slave_exit_isr(void)
{
    io_ext_int_mask(USER_GPIO_EXTI_RX,false);
    h4tl_read_start_2();
}

//uninit and reinit uart,because it have inited in debug_init function.
static void two_mcu_slave_init_uart(void)
{
    app_uart_uninit(&uart0.inst);
    uart0.param.baud_rate = UART_BAUDRATE_2000000;
    uart0.param.rx_pin_no = 13;
    uart0.param.tx_pin_no = 12;
    uart0.param.tx_dma = 0;
    uart0.param.rx_dma = 0;
    uart0.param.cts_pin_no = USER_GPIO_UART_CTS;
    uart0.param.rts_pin_no = USER_GPIO_UART_RTS;
    uart0.param.auto_flow_control = 1;
    uart0.param.flow_control_en = 1;
    app_uart_init(&uart0.inst);
}


/*
 *  EXPORT FUNCTIONS
 ****************************************************************************************
 */
//call after debug_init
void two_mcu_slave_init(void)
{
    //RX to wake up me
    io_cfg_input(USER_GPIO_EXTI_RX);
    io_ext_int_cfg(USER_GPIO_EXTI_RX,EXT_INT_TRIGGER_POS_EDGE,two_mcu_slave_exit_isr);
    io_ext_int_en(USER_GPIO_EXTI_RX,true);
    //wake up BlueX before TX
    io_cfg_output(USER_GPIO_EXTI_TX);
    io_pin_clear (USER_GPIO_EXTI_TX);
    //LED
    io_cfg_output(MCU_DEMO_LED_PIN);
    io_pin_clear (MCU_DEMO_LED_PIN);
    //uart init to new condition
    two_mcu_slave_init_uart();
}

//call before uart0_read.
void two_mcu_slave_wakeup_master(void)
{
    io_pin_set(USER_GPIO_EXTI_TX);//reset
    BX_DELAY_US(2*1000);
    io_pin_clear(USER_GPIO_EXTI_TX);//work
}

//call after h4tl_read_start_patch.
void two_mcu_slave_unmask_exti(void)
{
    //unmask EXIT
    io_ext_int_mask(USER_GPIO_EXTI_RX,true);
}



