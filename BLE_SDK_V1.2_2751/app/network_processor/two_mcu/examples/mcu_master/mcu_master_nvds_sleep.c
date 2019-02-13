/**
 ****************************************************************************************
 *
 * @file   mcu_nvds_sleep.c
 *
 * @brief  .
 *
 * @author  jiachuang
 * @date    2018-11-05 11:07
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
#include "app_uart.h"
#include "plf.h"
#include "bx_config.h"
#include "nvds_tag.h"
#include "nvds_task.h"
#include "mcu_master_nvds_sleep.h"
#include "string.h"
#include "io_ctrl.h"
/*
 * MACROS
 ****************************************************************************************
 */
#define TAG_INIT()                             \
    .head =                                                     \
        {                                                             \
            .magic_flag = {'N','V','D','S'},     \
            .ver = 1,                                          \
        },                                                          \
    .tail = 0xff
#define DEFINE_TAG(tag,...)           \
    .tag##_env =                              \
    {                                           \
        .label = {                              \
            .valid = 1,                 \
            .reserved = 0x7f,   \
        },                                      \
        .struct_size_r = (uint8_t)(~sizeof(nvds_data.tag##_env)),\
        .name = tag,                            \
        .data = __VA_ARGS__,                        \
    }


/*
 * DEFINES
 ****************************************************************************************
 */
#define MCU_BX_DEV_NAME         {'B','X','N','B','6','6'}
#define MCU_BX_DEV_ADDR         {0xAA,0xFF,0x33,0x44,0x55,0x66}
#define MCU_BLE_WAKEUP_TIME     {1200}
#define MCU_DEEP_SLEEP_ENABLE   {1}
#define MCU_EXT_WAKE_UP_ENABLE  {1}


/*
 * ENUMERATIONS
 ****************************************************************************************
 */


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/*
 * VARIABLE DEFINITIONS
 ****************************************************************************************
 */
nvds_data_t mcu_nvds_data = {
    TAG_INIT(),
    DEFINE_TAG(NVDS_TAG_DEVICE_NAME         ,MCU_BX_DEV_NAME),
    DEFINE_TAG(NVDS_TAG_BD_ADDRESS          ,MCU_BX_DEV_ADDR),
    DEFINE_TAG(NVDS_TAG_EXT_WAKEUP_TIME     ,MCU_BLE_WAKEUP_TIME),
    DEFINE_TAG(NVDS_TAG_OSC_WAKEUP_TIME     ,MCU_BLE_WAKEUP_TIME),
    DEFINE_TAG(NVDS_TAG_RM_WAKEUP_TIME      ,MCU_BLE_WAKEUP_TIME),
    DEFINE_TAG(NVDS_TAG_SLEEP_ENABLE        ,MCU_DEEP_SLEEP_ENABLE),
    DEFINE_TAG(NVDS_TAG_EXT_WAKEUP_ENABLE   ,MCU_EXT_WAKE_UP_ENABLE),
    DEFINE_TAG(NEW_TAG_1,{0}),

};



/*
 * DECLARE
 ****************************************************************************************
 */


/*
 * USER FUNCTION
 ****************************************************************************************
 */


/*
 * SLEEP
 ****************************************************************************************
 */
void mcu_exit_isr(void)
{
#if USER_MCU_ENABLE_SLEEP == 1
        mcu_ahi_msg_head_rx();
#endif
}
void mcu_io_init(void)
{
    //RX to wake up me
    io_cfg_input(USER_GPIO_EXTI_RX);
    io_ext_int_cfg(USER_GPIO_EXTI_RX,EXT_INT_TRIGGER_POS_EDGE,mcu_exit_isr);
    io_ext_int_en(USER_GPIO_EXTI_RX,true);
    //wake up APOLLO before TX
    io_cfg_output(USER_GPIO_EXTI_TX);
    io_pin_clear (USER_GPIO_EXTI_TX);
    //OTA reset pin
    io_cfg_output(USER_GPIO_RST);
    io_pin_clear (USER_GPIO_RST);    //cancel reset , start work
}

void mcu_io_reset_slave(void)
{
    io_pin_set(USER_GPIO_RST);//reset
    BX_DELAY_US(200*1000);
    io_pin_clear(USER_GPIO_RST);//work
}

void mcu_io_wakeup_slave(void)
{
    io_pin_set(USER_GPIO_EXTI_TX);//reset
    BX_DELAY_US(2000);//can't too small
    io_pin_clear(USER_GPIO_EXTI_TX);//work
}


/*
 * SEND MESSAGE
 ****************************************************************************************
 */
void mcu_nvds_set_data(void)
{
    uint16_t param_length = sizeof(mcu_nvds_data);
    struct nvds_set_data_cmd *cmd = AHI_MSG_ALLOC_DYN(NVDS_SET_DATA_CMD, TASK_ID_NVDS, nvds_set_data_cmd , param_length);
    cmd->length = param_length;
    memcpy(cmd->nvds , &mcu_nvds_data , param_length);
    osapp_ahi_msg_send(cmd, sizeof(struct nvds_set_data_cmd) + param_length, portMAX_DELAY);
}




/*
 * RECEIVE MESSAGE
 ****************************************************************************************
 */
static int nvds_data_put_handler(ke_msg_id_t const msgid, struct nvds_data_put_ind const *cmd,
        ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO , "NVDS_PUT_RX:tag_name=0x%x,length=0x%x,data=",cmd->tag_name,cmd->length);
    mcu_log_hex_data(cmd->data , cmd->length);
    return (KE_MSG_CONSUMED);
}


static const osapp_msg_handler_table_t handler_table_nvds[]=
{
        [0] =   {NVDS_DATA_PUT_IND,  (osapp_msg_handler_t)nvds_data_put_handler},
};

const osapp_msg_handler_info_t handler_info_nvds = ARRAY_INFO(handler_table_nvds);



