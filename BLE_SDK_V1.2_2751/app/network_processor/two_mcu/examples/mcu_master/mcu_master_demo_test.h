/**
 ****************************************************************************************
 *
 * @file   mcu_demo_test.h
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



#ifndef PLF_SRC_ARCH_MAIN_MCU_DEMO_TEST_H_
#define PLF_SRC_ARCH_MAIN_MCU_DEMO_TEST_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "stdint.h"

/*
 * MACROS
 ****************************************************************************************
 */

/*
 * DEFINES
 ****************************************************************************************
 */
//op code cmd
#define OP_PC_CMD                       0x01
#define OP_READ_FLASH_STATUS_REG        0x02
#define OP_WRITE_FLASH_STATUS_REG       0x05
#define OP_WRITE_W25_FLASH_STATUS_REG   0x06
#define OP_WRITE_TO_RAM_AND_RUN         0x07
#define OP_WRITE_TO_RAM_COMPLETE        0x08
// MCU BOUND RATE
#define MCU_BOOT_ROM_BOUND_RATE         UART_BAUDRATE_115200
#define MCU_BOOT_RAM_DOWN_BOUND_RATE    UART_BAUDRATE_2000000
#define MCU_AHI_BOUND_RATE              UART_BAUDRATE_2000000
//LOCAL
#define UART_TX_BUFFER_SIZE             1024
//SYSTEM
#define RAM_BASE                        0x00100000
#define UART_DOWNLOAD_EXEC_BASE         0x0012a000
#define BOOT_RAM_SAVE_ADDR              UART_DOWNLOAD_EXEC_BASE
#define TX_USER_APP_BLOCK_MAX           1024        //1024 Byte per block
#define OTA_IMAGE_HEADER_SIZE           (128+2) //OTA iamge offset
/******user change******/
#define BOOTRAM_IN_FLASH_OFFSET         0x80000     //512K Byte
#define USERAPP_IN_FLASH_OFFSET         0xA0000     //640K Byte
#define USEROTA_IN_FLASH_OFFSET         0xC0000     //768K Byte
#define BOOTRAM_LENGTH                  4696          //Byte
#define USERAPP_LENGTH                  40000         //Byte
#define USER_MCU_ENABLE_SLEEP           0           //enable sleep in this MCU
/******user GPIO******/
#define USER_GPIO_RST                   22      //GPIO reset slave APOLLO
#define USER_GPIO_EXTI_TX               15      //wake up APOLLO before tx
#define USER_GPIO_EXTI_RX               17      //wake up myself before receive rx
#define USER_GPIO_UART_CTS              20      //flow control
#define USER_GPIO_UART_RTS              21      //flow control

/*
 * ENUMERATIONS
 ****************************************************************************************
 */
typedef enum
{
    //boot rom
    MCU_BOOTROM_INIT,           //init:bootrom bound rate
    MCU_BOOTROM_TX_BOOT_HEADER, //tx:boot_header_t,wait ack
    MCU_BOOTROM_TX_BOOTRAM,     //tx:bootram.bin
    MCU_BOOTROM_TX_CRC,         //tx:bootram.bin's CRC16
    MCU_CHANGE_TO_BOOTRAM,      //wait:wait for bootram run (400ms),change to bootram bound rate

    //boot ram
    MCU_BOOTRAM_TX_PC_READY,    //tx:pc is ready,wait ack.
    MCU_BOOTRAM_TX_CMD_TO_RAM,  //tx:write to RAM , wait ack
    MCU_BOOTRAM_TX_FLASH_PROG,  //tx:write flash.bin , wait ack , until complete
    MCU_BOOTRAM_TX_RAM_COMPLETE,//tx:write to RAM complete , wait ack
    //AHI cmd
    MCU_CHANGE_TO_AHI,           //ahi command
    MCU_OTA_DONE,                //OTA done and restart
}mcu_stage_t;

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
typedef void (*entry_point_t)();
typedef struct{
    uint8_t bx_flag[4];
    uint8_t *base_addr;
    uint32_t length;        //flash+header,not include crc
    entry_point_t entry_point;
}boot_header_t;
typedef struct
{
    uint8_t header_flag;
    struct
    {
        uint8_t r_w         :2; // 0:read 1:write 2:erase
        uint8_t section     :1; // 0:other cmd 1:read write flash
        uint8_t op          :5; // operation, only use when section equal 0.
                                // op = 1 finish download
    }cmd;
    uint16_t length;
    uint32_t addr;
}Uart_Ramrun_Header_t;
typedef uint16_t (*crc_func_t)(uint16_t ,const void *,uint32_t);

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
void mcu_master_demo_init(void);
void mcu_state_change(mcu_stage_t new_stage);


#endif /*  */


