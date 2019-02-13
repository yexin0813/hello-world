/**
 ****************************************************************************************
 *
 * @file   mcu_ahi.c
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

#include "mcu_master_ahi.h"
#include "mcu_master_nvds_sleep.h"
#include "plf.h"


/*
 * DEFINES
 ****************************************************************************************
 */
/// Kernel message type
#define AHI_KE_MSG_TYPE         0x05

#define AHI_MSG_DUMMY_LEN       3   //3 bytes for reverse

#define AHI_TX_QUEUE_LEN        10



/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 *****************************************************************************************/
volatile ahi_msg_t msg_rx;//message rx tmp buffer
DEF_RING_QUEUE(mcu_ahi_tx_q , AHI_TX_QUEUE_LEN , mcu_ahi_queue_t);  //Add queue in uart tx.
mcu_ahi_tx_stat_t mcu_ahi_tx_stat = MCU_AHI_TX_STAT_IDLE;       //AHI tx queue status


/*
 * FUNCTIONS DECLARE
 ****************************************************************************************
 */
extern void uart0_read(uint8_t * bufptr, uint32_t size, void(* callback)(void *, uint8_t), void * dummy);
extern void uart0_write(uint8_t * bufptr, uint32_t size, void(* callback)(void *, uint8_t), void * dummy);
extern uint8_t uart_rx_buff[];
extern const osapp_msg_handler_info_t handler_info;
extern const osapp_msg_handler_info_t handler_info_ota;
extern const osapp_msg_handler_info_t handler_info_nvds;
static void mcu_ahi_msg_para_rx(void);
static void mcu_ahi_tx_send(mcu_ahi_queue_t* ptr);

/*
 * TOOLBOX FUNCTIONS
 ****************************************************************************************
 */
uint8_t * adv_data_pack(uint8_t *buf,uint8_t field_nums,...)
{
    va_list list;
    uint8_t i;
    va_start(list,field_nums);
    for(i=0;i<field_nums;++i)
    {
        uint8_t ad_type = va_arg(list,uint32_t);
        uint8_t *ad_data = va_arg(list,uint8_t *);
        uint8_t ad_data_length = va_arg(list,uint32_t);
        buf[0] = ad_data_length + 1;
        buf[1] = ad_type;
        memcpy(&buf[2],ad_data,ad_data_length);
        buf += buf[0] + 1;
    }
    va_end(list);
    return buf;
}

/*
 * AHI RX MESSAGE
 ****************************************************************************************
 */
static void mcu_ahi_msg_para_rx_callback(void *param, uint8_t stat)
{
    ahi_msg_t *msg = (ahi_msg_t *)&msg_rx;
    uint16_t i;
    bool specific_handler = false;
    //dis server
    for(i=1;i<handler_info.table_size;++i)
    {
        if(msg->id==handler_info.handler_table[i].id)
        {
            specific_handler = true;
            handler_info.handler_table[i].func(msg->id,uart_rx_buff,msg->dest_id,msg->src_id);
            break;
        }
    }
    //ota server
    for(i=0;i<handler_info_ota.table_size;++i)
    {
        if(msg->id==handler_info_ota.handler_table[i].id)
        {
            specific_handler = true;
            handler_info_ota.handler_table[i].func(msg->id,uart_rx_buff,msg->dest_id,msg->src_id);
            break;
        }
    }
    //nvds  task
    for(i=0;i<handler_info_nvds.table_size;++i)
    {
        if(msg->id==handler_info_nvds.handler_table[i].id)
        {
            specific_handler = true;
            handler_info_nvds.handler_table[i].func(msg->id,uart_rx_buff,msg->dest_id,msg->src_id);
            break;
        }
    }
    //no matched
    if(specific_handler==false)
    {
        handler_info.handler_table[0].func(msg->id,uart_rx_buff,msg->dest_id,msg->src_id);
    }
    //receive next
    mcu_ahi_msg_head_rx();
}

static void mcu_ahi_msg_para_rx(void)
{
    if(msg_rx.param_len > 0)
    {
        uart0_read(uart_rx_buff , msg_rx.param_len , mcu_ahi_msg_para_rx_callback , uart_rx_buff);
    }
    else
    {
        mcu_ahi_msg_para_rx_callback(0,0);
    }
}
static void mcu_ahi_msg_head_rx_callback(void *param, uint8_t stat)
{

    memcpy((void*)&msg_rx.flag , uart_rx_buff , sizeof(ahi_msg_t) - AHI_MSG_DUMMY_LEN);
    //LOG(LOG_LVL_INFO , "mcu_ahi_msg_head_rx_callback,flag=0x%x,id=0x%x\n",uart_rx_buff[0] , msg_rx.id);
    if(msg_rx.flag == AHI_KE_MSG_TYPE)
    {
        mcu_ahi_msg_para_rx();
    }
}

void mcu_ahi_msg_head_rx(void)
{
    uart0_read(uart_rx_buff , sizeof(ahi_msg_t)- AHI_MSG_DUMMY_LEN , mcu_ahi_msg_head_rx_callback , NULL);
}

/*
 * AHI TX MESSAGE
 ****************************************************************************************
 */
void *ahi_msg_alloc(ke_msg_id_t const id, ke_task_id_t const dest_id, uint16_t const param_len)
{
    ahi_msg_t *msg = (ahi_msg_t*) malloc(sizeof(ahi_msg_t) + param_len);
    uint8_t * param_pointer = (uint8_t *)msg + sizeof(ahi_msg_t);
    BX_ASSERT(msg != NULL);
    memset(msg, 0, sizeof(ahi_msg_t) + param_len);
    //fill
    msg->flag      = AHI_KE_MSG_TYPE;
    msg->id        = id;
    msg->dest_id   = dest_id;
    msg->src_id    = TASK_ID_AHI;
    msg->param_len = param_len;
    //return
    return param_pointer;
}

static void mcu_ahi_msg_reply_callback(void *param, uint8_t stat)
{
    free(param);
    //next
    mcu_ahi_queue_t *ptr0 = bx_dequeue(&mcu_ahi_tx_q);
    if(bx_ring_queue_empty(&mcu_ahi_tx_q)==false)
    {
        mcu_ahi_queue_t *ptr1 = bx_ring_queue_glance(&mcu_ahi_tx_q,0);
        mcu_ahi_tx_send(ptr1);
    }
    else
    {
        mcu_ahi_tx_stat = MCU_AHI_TX_STAT_IDLE;
    }
}


static void mcu_ahi_tx_send(mcu_ahi_queue_t* ptr)
{
    void *param_ptr = ptr->param_ptr;
    uint16_t param_length = ptr->param_length;
    
    ahi_msg_t *msg = (ahi_msg_t*)(((uint8_t*)param_ptr) - sizeof(ahi_msg_t));
    uint16_t total_length = msg->param_len + sizeof(ahi_msg_t) - AHI_MSG_DUMMY_LEN;
    uint8_t* uart_tx_start = &msg->flag;

    //send
    mcu_io_wakeup_slave();
    uart0_write(uart_tx_start , total_length , mcu_ahi_msg_reply_callback , msg);
}

int32_t osapp_msg_build_send(void *param_ptr,uint16_t param_length)
{
    mcu_ahi_queue_t *ptr = bx_enqueue_position(&mcu_ahi_tx_q);
    ptr->param_ptr = param_ptr;
    ptr->param_length = param_length;
    bx_enqueue_nocopy(&mcu_ahi_tx_q);
    if(mcu_ahi_tx_stat == MCU_AHI_TX_STAT_IDLE)
    {
        mcu_ahi_tx_stat = MCU_AHI_TX_STAT_BUSY;
        mcu_ahi_queue_t *ptr1 = bx_ring_queue_glance(&mcu_ahi_tx_q,0);
        mcu_ahi_tx_send(ptr);
    }
    return 0;
}



void osapp_ahi_msg_send(void *param_ptr,uint16_t param_length,uint32_t xTicksToWait)
{
    osapp_msg_build_send(param_ptr,param_length);
}



void mcu_log_hex_data(const uint8_t * data , uint8_t length)
{
    const uint8_t mesh_log_hex_tab[] = "0123456789ABCDEF";
    uint8_t tmp_h,tmp_l;
    uint8_t *mesh_log_format_buff=0;
    uint8_t *bufptr=0;
    uint32_t total_length;
    //init
    total_length = length * 2 + 1;
    mesh_log_format_buff = malloc(total_length);
    bufptr = mesh_log_format_buff;
    //content
    for(uint8_t i=0;i<length;i++)
    {
        tmp_h = data[i] >> 4;
        tmp_l = data[i] & 0x0F;
        *bufptr = mesh_log_hex_tab[tmp_h];  bufptr++;
        *bufptr = mesh_log_hex_tab[tmp_l];  bufptr++;
    }
    //end
    *bufptr = '\n'; bufptr ++;
    //print
    SEGGER_RTT_Write(0,mesh_log_format_buff,total_length);
    #if (USE_INTERNAL_LOG == 1)
    internal_log_write(mesh_log_format_buff,total_length);
    #endif
    //free
    free(mesh_log_format_buff);
}


