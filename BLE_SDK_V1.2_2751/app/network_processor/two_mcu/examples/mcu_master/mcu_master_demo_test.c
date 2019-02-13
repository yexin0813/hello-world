/**
 ****************************************************************************************
 *
 * @file   mcu_demo_test.c
 *
 * @brief  .
 *
 * @author  jiachuang
 * @date    2018-10-24 09:44
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
#include "mcu_master_ahi.h"
#include "mcu_master_demo_test.h"
#include <stddef.h>
#include "log.h"
#include "app_uart.h"
#include "flash.h"
#include "plf.h"
#include "mcu_master_ota.h"
#include "mcu_master_nvds_sleep.h"
#include "io_ctrl.h"
/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
//system const
static volatile char tmp = 0;   //to fix warnings.
const static crc_func_t crc_func = (crc_func_t)0x18d;
//random buffer
uint8_t uart_rx_buff[UART_TX_BUFFER_SIZE];
uint8_t uart_tx_buff[BOOTRAM_LENGTH];

/*local volatile parameter*/
//boot ram
static boot_header_t       *boot_header;
static Uart_Ramrun_Header_t bootram_cmd;
static uint16_t             header_crc;
//tx user app
static volatile uint32_t tx_ram_offset = 0;
static volatile uint32_t tx_ram_remain = 0;
static volatile uint32_t tx_ram_current_block = 0;
static volatile uint32_t tx_ram_total = 0;
//system stage
static volatile mcu_stage_t mcu_stage = MCU_BOOTROM_INIT;
static volatile uint8_t  need_perform = 0;
//OTA
static volatile uint8_t  mcu_ota_done = 0;
/*
 * CONST VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*
 * FUNCTIONS DECLARE
 ****************************************************************************************
 */
extern app_uart_inst_t uart0;
extern uint32_t ota_image_size;
extern void uart0_read(uint8_t * bufptr, uint32_t size, void(* callback)(void *, uint8_t), void * dummy);
extern void uart0_write(uint8_t * bufptr, uint32_t size, void(* callback)(void *, uint8_t), void * dummy);
static void mcu_uart_init(app_uart_baudrate_t baud_rate);
static void mcu_uart_uninit(void);
static void mcu_tx_bootram_cmd_callback(void *param, uint8_t stat);
static void mcu_tx_bootram_cmd_callback(void *param, uint8_t stat);
static void mcu_tx_callback(void *param, uint8_t stat);
static void mcu_rx_callback(void *param, uint8_t stat);
static void mcu_tx_write_to_ram(void);
static void mcu_reset(void);
/*
 * FUNCTIONS TOOLBOX
 ****************************************************************************************
 */
static void mcu_uart_init(app_uart_baudrate_t baud_rate)
{
    uart0.param.baud_rate = baud_rate;
    uart0.param.rx_pin_no = 13;
    uart0.param.tx_pin_no = 12;
    uart0.param.tx_dma = 1;
    uart0.param.rx_dma = 1;
    app_uart_init(&uart0.inst);
}
static void mcu_uart_init_flow_ctrl(app_uart_baudrate_t baud_rate)
{
    uart0.param.baud_rate = baud_rate;
    uart0.param.rx_pin_no = 13;
    uart0.param.tx_pin_no = 12;
    uart0.param.tx_dma = 1;
    uart0.param.rx_dma = 1;
    uart0.param.cts_pin_no = USER_GPIO_UART_CTS;
    uart0.param.rts_pin_no = USER_GPIO_UART_RTS;
    uart0.param.auto_flow_control = 1;
    uart0.param.flow_control_en = 1;
    app_uart_init(&uart0.inst);
    app_uart_flow_on(&uart0.inst);
}

static void mcu_uart_uninit(void)
{
    uint32_t remain_length=0;
    app_uart_read_cancel(&uart0.inst,&remain_length);
    app_uart_uninit(&uart0.inst);
}


/*
 * MCU PROJECT TOOLBOX
 ****************************************************************************************
 */
static void mcu_reset(void)
{
    //init variable
    memset(&bootram_cmd , 0 , sizeof(bootram_cmd));
    header_crc = 0;
    tx_ram_offset = tx_ram_remain = tx_ram_current_block = 0;
    mcu_state_change(MCU_BOOTROM_INIT);
    //read bootram from flash to RAM
    flash_read(BOOTRAM_IN_FLASH_OFFSET,BOOTRAM_LENGTH,uart_tx_buff);
    //make cmd pack
    boot_header = (boot_header_t*) uart_tx_buff;
    boot_header->length = BOOTRAM_LENGTH;
    //calc CRC
    header_crc = crc_func(header_crc,uart_tx_buff,BOOTRAM_LENGTH);
}
static void fill_bootram_cmd(uint8_t section , uint8_t r_w , uint8_t cmd ,uint16_t length , uint32_t addr)
{
    bootram_cmd.header_flag = 0xad;
    bootram_cmd.cmd.section = section;
    bootram_cmd.cmd.r_w     = r_w;
    bootram_cmd.cmd.op      = cmd;
    bootram_cmd.length      = length;
    bootram_cmd.addr        = addr;
}

/*
 * TOOLS FUNCTIONS
 ****************************************************************************************
 */
static void mcu_tx_write_to_ram(void)
{
    uint32_t flash_offset = 0;
    if(mcu_ota_done == 0)
    {
        flash_offset = USERAPP_IN_FLASH_OFFSET;
        tx_ram_total = USERAPP_LENGTH;
    }
    else
    {
        flash_offset = USEROTA_IN_FLASH_OFFSET + OTA_IMAGE_HEADER_SIZE;
        tx_ram_total = ota_image_size - OTA_IMAGE_HEADER_SIZE;
    }

    //transmit
    tx_ram_remain = tx_ram_total - tx_ram_offset;
    tx_ram_current_block = (tx_ram_remain >= TX_USER_APP_BLOCK_MAX) ? TX_USER_APP_BLOCK_MAX : tx_ram_remain;
    //make cmd
    fill_bootram_cmd(1,1,0,tx_ram_current_block,tx_ram_offset);
    //read from flash
    flash_read(flash_offset + tx_ram_offset,tx_ram_current_block,uart_tx_buff);
    //send to uart cmd
    tx_ram_offset += tx_ram_current_block;
    uart0_write((uint8_t*)&bootram_cmd , sizeof(bootram_cmd) , mcu_tx_bootram_cmd_callback , NULL);
}


/*
 * BOOTRAM CMD CALLBACK FUNCTIONS
 ****************************************************************************************
 */
static void mcu_tx_bootram_data_callback(void *param, uint8_t stat)
{
    LOG(LOG_LVL_INFO , "mcu_tx_bootram_cmd_callback,mcu_stage=%d\n",mcu_stage);
    switch(mcu_stage)
    {
        case MCU_BOOTRAM_TX_PC_READY:
            uart0_read (uart_rx_buff , 5 , mcu_rx_callback , NULL);
            break;
        case MCU_BOOTRAM_TX_CMD_TO_RAM://NO RESPONCE
            mcu_state_change(MCU_BOOTRAM_TX_FLASH_PROG);
            break;
        case MCU_BOOTRAM_TX_RAM_COMPLETE://NO RESPONCE
            mcu_state_change(MCU_CHANGE_TO_AHI);
            break;
        case MCU_BOOTRAM_TX_FLASH_PROG:
            uart0_read (uart_rx_buff , 5 , mcu_rx_callback , NULL);
            break;
    }
    BX_DELAY_US(1*1000);
}
static void mcu_tx_bootram_cmd_callback(void *param, uint8_t stat)
{
    LOG(LOG_LVL_INFO , "mcu_tx_bootram_cmd_callback,mcu_stage=%d\n",mcu_stage);
    switch(mcu_stage)
    {
        case MCU_BOOTRAM_TX_PC_READY:
        case MCU_BOOTRAM_TX_CMD_TO_RAM:
        case MCU_BOOTRAM_TX_RAM_COMPLETE:
            uart_tx_buff[0] = 'P';
            uart_tx_buff[1] = 'R';
            uart0_write(uart_tx_buff , 2 , mcu_tx_bootram_data_callback , NULL);
            break;
        case MCU_BOOTRAM_TX_FLASH_PROG:
            uart0_write(uart_tx_buff , tx_ram_current_block , mcu_tx_bootram_data_callback , NULL);
            break;
    }
    BX_DELAY_US(1*1000);
}


/*
 * NORMAL CALLBACK FUNCTIONS
 ****************************************************************************************
 */
static void mcu_tx_callback(void *param, uint8_t stat)
{
    LOG(LOG_LVL_INFO , "mcu_tx_callback,mcu_stage=%d\n",mcu_stage);
    switch(mcu_stage)
    {
        case MCU_BOOTROM_TX_BOOTRAM:
            mcu_state_change(MCU_BOOTROM_TX_CRC);
            break;
        case MCU_BOOTROM_TX_CRC:
            mcu_state_change(MCU_CHANGE_TO_BOOTRAM);
            break;
    }
}

static void mcu_rx_callback(void *param, uint8_t stat)
{
    LOG(LOG_LVL_INFO , "mcu_rx_callback,mcu_stage=%d\n",mcu_stage);
    switch(mcu_stage)
    {
        case MCU_BOOTROM_TX_BOOT_HEADER:
            if(memcmp(uart_rx_buff,"BX_UART_BOOT",13) == 0)
            {
                LOG(LOG_LVL_INFO,"rx:BX_UART_BOOT\n");
                mcu_state_change(MCU_BOOTROM_TX_BOOTRAM);
            }
            break;
        case MCU_BOOTRAM_TX_PC_READY:
            mcu_state_change(MCU_BOOTRAM_TX_CMD_TO_RAM);
            break;
        case MCU_BOOTRAM_TX_FLASH_PROG:
            if(tx_ram_offset == tx_ram_total)
            {
                //complete
                mcu_state_change(MCU_BOOTRAM_TX_RAM_COMPLETE);
            }
            else
            {
                mcu_tx_write_to_ram();
            }
            break;
    }
}


/*
 * STAGE FUNCTIONS
 ****************************************************************************************
 */
void mcu_state_change(mcu_stage_t new_stage)
{
    mcu_stage = new_stage;
    need_perform = 1;
}

static void mcu_perform_action(void)
{
    LOG(LOG_LVL_INFO , "mcu_process_action,mcu_stage=%d\n",mcu_stage);
    switch(mcu_stage)
    {
        case MCU_BOOTROM_INIT:
            mcu_uart_init(MCU_BOOT_ROM_BOUND_RATE);
            mcu_state_change(MCU_BOOTROM_TX_BOOT_HEADER);
            break;
        case MCU_BOOTROM_TX_BOOT_HEADER:
            uart0_write((uint8_t *)boot_header , sizeof(boot_header_t) , mcu_tx_callback , NULL);
            uart0_read (uart_rx_buff , 13                              , mcu_rx_callback , NULL);
            break;
        case MCU_BOOTROM_TX_BOOTRAM:
            uart0_write(uart_tx_buff+sizeof(boot_header_t) , BOOTRAM_LENGTH-sizeof(boot_header_t) , mcu_tx_callback , NULL);
            break;
        case MCU_BOOTROM_TX_CRC:
            memcpy(uart_tx_buff , &header_crc , 2);
            uart0_write(uart_tx_buff , 2 , mcu_tx_callback , NULL);
            break;
        case MCU_CHANGE_TO_BOOTRAM://wait:wait for bootram run (400ms),change to bootram bound rate
            BX_DELAY_US(400*1000);
            mcu_uart_uninit();
            mcu_uart_init(MCU_BOOT_RAM_DOWN_BOUND_RATE);
            mcu_state_change(MCU_BOOTRAM_TX_PC_READY);
            break;
        case MCU_BOOTRAM_TX_PC_READY:
            fill_bootram_cmd(0,0,OP_PC_CMD,2,0);
            uart0_write((uint8_t*)&bootram_cmd , sizeof(bootram_cmd) , mcu_tx_bootram_cmd_callback , NULL);
            break;
        case MCU_BOOTRAM_TX_CMD_TO_RAM:
            fill_bootram_cmd(0,0,OP_WRITE_TO_RAM_AND_RUN,2,0);
            uart0_write((uint8_t*)&bootram_cmd , sizeof(bootram_cmd) , mcu_tx_bootram_cmd_callback , NULL);
            break;
        case MCU_BOOTRAM_TX_FLASH_PROG:
            mcu_tx_write_to_ram();
            break;
        case MCU_BOOTRAM_TX_RAM_COMPLETE:
            fill_bootram_cmd(0,0,OP_WRITE_TO_RAM_COMPLETE,2,0);
            uart0_write((uint8_t*)&bootram_cmd , sizeof(bootram_cmd) , mcu_tx_bootram_cmd_callback , NULL);
            break;
        case MCU_CHANGE_TO_AHI:
            mcu_uart_uninit();
            mcu_uart_init_flow_ctrl(MCU_AHI_BOUND_RATE);
            #if USER_MCU_ENABLE_SLEEP == 0
                mcu_ahi_msg_head_rx();
            #endif
            break;
        case MCU_OTA_DONE:
            mcu_ota_done = 1;
            BX_DELAY_US(200*1000);
            mcu_io_reset_slave();
            BX_DELAY_US(200*1000);
            //toogle reset pin
            mcu_uart_uninit();
            mcu_reset();//change to initial version
            break;
        default:
            break;
    }
}



/*
 * GLOBAL FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */
void mcu_master_demo_init(void)
{
    mcu_io_init();
    mcu_reset();
    //Infinity loop
    while(tmp == 0)
    {
        if(need_perform == 1)
        {
            need_perform = 0;
            mcu_perform_action();
        }
        else
        {
            #if USER_MCU_ENABLE_SLEEP == 1
                WFI();
            #endif
        }
    }
}


