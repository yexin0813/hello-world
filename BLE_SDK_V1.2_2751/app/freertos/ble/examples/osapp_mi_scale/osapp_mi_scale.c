/**
 *
 * @file osapp_template.c
 *
 * @brief A template for application.
 *
 * Copyright (C) Apollo 2015-2016
 *
 */

#include "osapp_config.h"
#ifdef OSAPP_MI_SCALE
#include "uart.h"
#include "reg_sysc_awo.h"

#define APP_ADV_CHMAP 0x7
#define APP_ADV_INT_MIN 64
#define APP_ADV_INT_MAX 64

#define CMD_START_NUM 0x30
enum rx_cmd_handler_t{
    
    HANDLER_MAX = 31,
};


uint8_t cmd_rx_buf[15];
uint8_t rsp_tx_buf[12];

uint8_t xor_checksum(uint8_t *start,uint8_t length)
{
    uint8_t *ptr = start;
    uint8_t chksum =0 ;
    uint8_t i;
    for(i=0;i<length;++i)
    {
        chksum^= *ptr++;
    }
    return chksum;
}

static void start_weighing_cb(void *param,uint8_t status)
{
    LOG(LOG_LVL_INFO,"start weighing cb\n");
}

static void start_weighing(void *param,uint8_t status)
{
     rsp_tx_buf[0] = 0xbb;
    rsp_tx_buf[1] = 0x20;
    rsp_tx_buf[2] = 0x34;
    rsp_tx_buf[3] = 0x00;
    rsp_tx_buf[4] = xor_checksum(rsp_tx_buf, 4);
    uart_write(rsp_tx_buf, 5, start_weighing_cb, NULL);
}

static void dev_power_up(uint8_t *payload,uint8_t length)
{
    rsp_tx_buf[0] = 0xbb;
    rsp_tx_buf[1] = 0x20;
    rsp_tx_buf[2] = 0x30;
    rsp_tx_buf[3] = 0x00;
    rsp_tx_buf[4] = xor_checksum(rsp_tx_buf, 4);
    uart_write(rsp_tx_buf, 5, start_weighing, NULL);
}
static void osapp_update_adv_data(uint8_t * data, uint8_t length);
uint16_t weight_counts = 0;
static void dynamic_weight(uint8_t *payload,uint8_t length)
{
    
    if(weight_counts == 10)
    {
        osapp_update_adv_data(payload,length);
        for(uint8_t j=0;j<length;++j)
        {
            LOG(LOG_LVL_INFO,"%d ",payload[j]);
        }
        LOG(LOG_LVL_INFO,"\n");
        weight_counts = 0;
    }
    weight_counts += 1;

}

void (*cmd_rx_handler[HANDLER_MAX])(uint8_t *,uint8_t)=
{
    [0] = dev_power_up,
    [4] = dynamic_weight,
    
};


static void cmd_read_start(void);

static void cmd_read_complete(void *param,uint8_t status)
{
    uint8_t length_field = cmd_rx_buf[3];
    uint8_t packet_length = length_field + 4;
    uint8_t chksum_rx = cmd_rx_buf[packet_length];
    uint8_t chksum_calc =  xor_checksum(cmd_rx_buf, packet_length);
    if(chksum_calc == chksum_rx)
    {
        cmd_rx_handler[cmd_rx_buf[2] - CMD_START_NUM](cmd_rx_buf + 4, length_field);
    }else{
        LOG(LOG_LVL_WARN,"error chksum\n");
    }
    
    cmd_read_start();
}

static void cmd_read_body(void *param,uint8_t status)
{
    uint8_t data_length = cmd_rx_buf[3];
    if(cmd_rx_buf[0] == 0xAA)
    {
        uart_read(cmd_rx_buf + 4,data_length + 1,cmd_read_complete,NULL);
    }else{
        cmd_read_start(); 
    }
}

static void cmd_read_start()
{
    uart_read(cmd_rx_buf,4,cmd_read_body,NULL);
}

static void osapp_update_adv_data(uint8_t *data,uint8_t length)
{
    struct gapm_update_advertise_data_cmd *cmd = AHI_MSG_ALLOC(GAPM_UPDATE_ADVERTISE_DATA_CMD,TASK_ID_GAPM, gapm_update_advertise_data_cmd);
    cmd->operation = GAPM_UPDATE_ADVERTISE_DATA;
    cmd->adv_data_len = length;
    memcpy(cmd->adv_data,data,length);
    osapp_ahi_msg_send_isr(cmd,sizeof(struct gapm_update_advertise_data_cmd));
}

static int32_t osapp_start_advertising()
{
        // Prepare the GAPM_START_ADVERTISE_CMD message
    struct gapm_start_advertise_cmd *cmd = AHI_MSG_ALLOC(GAPM_START_ADVERTISE_CMD,TASK_ID_GAPM, gapm_start_advertise_cmd);

    cmd->op.addr_src    = GAPM_STATIC_ADDR;
    cmd->channel_map    = APP_ADV_CHMAP;
    cmd->intv_min = APP_ADV_INT_MIN;
    cmd->intv_max = APP_ADV_INT_MAX;
    cmd->op.code        = GAPM_ADV_UNDIRECT;
    cmd->info.host.mode = GAP_GEN_DISCOVERABLE;
            // Flag value is set by the GAP
    cmd->info.host.adv_data_len       = ADV_DATA_LEN - 3;
    cmd->info.host.scan_rsp_data_len  = SCAN_RSP_DATA_LEN;
//    nvds_get(NVDS_TAG_APP_BLE_ADV_DATA, &cmd->info.host.adv_data_len,                    &cmd->info.host.adv_data[0]) ;
//    nvds_get(NVDS_TAG_APP_BLE_SCAN_RESP_DATA, &cmd->info.host.scan_rsp_data_len,                      &cmd->info.host.scan_rsp_data[0]);
    return osapp_msg_build_send(cmd,sizeof(struct gapm_start_advertise_cmd));

}
static int32_t osapp_set_dev_config(uint8_t role,uint8_t addr_type,uint8_t pairing_mode)
{
    // Set Device configuration
    struct gapm_set_dev_config_cmd* cmd = AHI_MSG_ALLOC(GAPM_SET_DEV_CONFIG_CMD,TASK_ID_GAPM,gapm_set_dev_config_cmd);
    cmd->operation = GAPM_SET_DEV_CONFIG;
    cmd->role      = role;

    // Set Data length parameters
    cmd->sugg_max_tx_octets = BLE_MIN_OCTETS;
    cmd->sugg_max_tx_time   = BLE_MIN_TIME;
    cmd->addr_type = addr_type;
    cmd->pairing_mode = pairing_mode;
    cmd->max_mtu = GAP_MAX_LE_MTU;
    return osapp_msg_build_send(cmd, sizeof(struct gapm_set_dev_config_cmd) );
}

static int32_t osapp_reset()
{
    struct gapm_reset_cmd *cmd = AHI_MSG_ALLOC(GAPM_RESET_CMD,TASK_ID_GAPM,gapm_reset_cmd);
    cmd->operation = GAPM_RESET;
    return osapp_msg_build_send(cmd, sizeof(struct gapm_reset_cmd));
}

/**
 * @brief callback function when app_task receive a message which can not found in handler_table. Normally, we should handle all the message and
 * take seriously when we found a default message.
 */
static void osapp_default_msg_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"default handler,msgid: 0x%x, dest_id: 0x%x, src_id: 0x%x",msgid,dest_id,src_id);

}
void led_test();
static void osapp_device_ready_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    //reset cmd
    LOG(LOG_LVL_INFO,"Device Ready\n");
    if(osapp_reset()!=pdTRUE)
    {
        BX_ASSERT(0);

    }
    led_test();
}


static void osapp_gapm_cmp_evt_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapm_cmp_evt const *cmp_evt = param;
    switch(cmp_evt->operation)
    {
    case GAPM_RESET:
        if(cmp_evt->status == GAP_ERR_NO_ERROR)
        {
            LOG(LOG_LVL_INFO,"Reset OK\n");
            osapp_set_dev_config(GAP_ROLE_PERIPHERAL,GAPM_CFG_ADDR_PUBLIC,GAPM_PAIRING_LEGACY);
        }
        else
        {
            LOG(LOG_LVL_INFO,"Reset Failed\n");
        }
        break;
    case GAPM_SET_DEV_CONFIG:
        osapp_start_advertising();
        cmd_read_start();
        break;
    case GAPM_UPDATE_ADVERTISE_DATA:
        LOG(LOG_LVL_WARN,"adv data update complete,status:%d\n",cmp_evt->status);
        break;
    default:
        LOG(LOG_LVL_WARN,"gapm_cmp_evt operation:0x%x\n",cmp_evt->operation);
        break;
            
    }

}

TaskHandle_t handle_uart_task;
void uart_task(void *param)
{


    while(1)
    {

    }
}

/**
 * @brief handler table to define the relations between message and it's callback.
 */
static const osapp_msg_handler_table_t handler_table[]=
{
        [0] =   {KE_MSG_DEFAULT_HANDLER,osapp_default_msg_handler},
//           {GAPC_CONNECTION_REQ_IND,osapp_gapc_conn_req_ind_handler},
//           {GAPC_DISCONNECT_IND,       osapp_gapc_disconnect_ind_handler},
//           {GAPM_PROFILE_ADDED_IND,osapp_gapm_profile_added_ind_handler},
           {GAPM_CMP_EVT,osapp_gapm_cmp_evt_handler},
           {GAPM_DEVICE_READY_IND,osapp_device_ready_ind_handler},
            
};
const osapp_msg_handler_info_t handler_info = ARRAY_INFO(handler_table);

/**
 * @brief add task needed to run in application exclude ble_task and app_task which have define in task_init.c
 */
//static const task_table_t task_table[] ={
//        {true,"UART",OS_PRIORITY_APP_TASK - 1,APP_TASK_STACK_SIZE,NULL,uart_task,&handle_uart_task},
//};
//const task_table_info_t extra_task_table_info = ARRAY_INFO(task_table);

#include "gpio.h"
#include "clk_gen.h"



void Delay(__IO uint32_t nCount);
void HT1632C_Writer(unsigned char Data,unsigned char cnt);
void HT1632C_Writer_CMD(unsigned char cmd);
void HT1632C_Writer_DATA(unsigned char Addr,unsigned char Data);
void HT1632C_Writer_AllDATA(unsigned char Addr,unsigned char *p,unsigned char cnt);
void HT1632C_clr(void)  ;
void HT1632C_Init(void)  ;
void display_left_num(uint16_t num);
void display_right_num(uint16_t num, uint8_t point, uint8_t negative);
void display_num(uint16_t left_num, uint16_t right_num);
void clear_tab3(void);
void display_left_char(uint16_t num);


#define HT_DATA_Pin   15
#define HT_WR_Pin           7
#define HT_RD_Pin       3
#define HT_CS_Pin       2
#define HT_Port             GPIOA
#define GPIO_ResetBits(port, pin) gpio_SetOutputValue((pin),0)
#define GPIO_SetBits(port,pin) gpio_SetOutputValue((pin),1)
//指令表
#define SYS_DIS 0x00                //关闭系统震荡
#define SYS_EN  0x01                //打开系统震荡
#define LED_OFF 0x02                //关闭LED显示
#define LED_ON  0x03                //打开LED显示
#define BLINK_OFF   0x08            //关闭闪烁
#define BLINK_ON    0x09            //打开闪烁
#define SLAVE_MODE  0x10            //从模式
#define RC_MASTER_MODE  0x18        //内部RC时钟
#define EXT_CLK_MASTER_MODE 0x1C    //外部时钟
#define COM_OPTION  0x20            //8COM n-mos模式
//#define PWM_DUTY  0XA0            //PWM亮度控制
#define PWM_DUTY    0xAf            //PWM亮度控制

//0--9 - 不带小数点
unsigned char tab1[] = {0xee, 0x28, 0xcd, 0x6d, 0x2b, 0x67, 0xe7, 0x2c, 0xef, 0x6f, 0x01};
//0--9 带小数点
unsigned char tab2[] = {0xfe, 0x38, 0xdd, 0x7d, 0x3b, 0x77, 0xf7, 0x3c, 0xff, 0x7f};

unsigned char tab3[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// 0 s e t   l n d
unsigned char tab4[] = {0x00, 0x67, 0xc7, 0xc3, 0xc2, 0xa1, 0xe9};

void io_init()
{
    sysc_awo_rf_reg_1_set(sysc_awo_rf_reg_1_get()|(1<<14)|(1<<15)|(1<<16));
    clkg_gpio_set_PER();
    GPIO_SetBits(HT_Port,HT_CS_Pin);
    GPIO_SetBits(HT_Port,HT_WR_Pin);
    GPIO_SetBits(HT_Port,HT_DATA_Pin);
    *(uint32_t *)0x20148004 = (1<<7)|(1<<15)|(1<<2);
}
void led_test(void)
{
    io_init();
    HT1632C_Init();
    HT1632C_Writer_CMD(LED_ON);

    //  display_num(tab[10]);

//  while(1)
    {
        display_num(1,2);                                                                     //每次显示完清空tab3
        Delay(1000000);
    }

}
/*******************************
*函数名:display_num
*描    述:    左右显示
*参    数:
*返    回:
********************************/
void display_num(uint16_t left_num, uint16_t right_num)
{
        display_left_num(left_num);
        display_right_num(right_num, 1, 0);
        HT1632C_Writer_AllDATA(0x00, tab3, 8);
        clear_tab3();

}
/*******************************
*函数名:display_left_num
*描    述:    左边数字显示
*参    数:
*返    回:
********************************/
void display_left_num(uint16_t num)
{
    uint8_t bits = 0;
    uint8_t decade = 0;
    uint8_t hundreds = 0;
    uint8_t i = 0;

    bits = num % 10;
    decade = num / 10 % 10;
    hundreds = num / 100;

    for(i = 0; i < 8; i++)
    {
        if(hundreds !=  0)
            tab3[i] |= (tab1[hundreds] & (1 << i)) << (7 - i);
        if(i <= 6)
            tab3[i] |= (tab2[decade] & (1 << i)) << (6 - i);
        else
            tab3[i] |= (tab2[decade] & (1 << i)) >> (i - 6);
        if(i <= 5)
            tab3[i] |= (tab1[bits] & (1 << i)) << (5 - i);
        else
            tab3[i] |= (tab1[bits] & (1 << i)) >> (i - 5);
    }
}
/*******************************
*函数名:display_left_char
*描    述:    左边字符显示
*参    数:
*返    回:
********************************/
void display_left_char(uint16_t num)
{
    uint8_t bits = 0;
    uint8_t decade = 0;
    uint8_t hundreds = 0;
    uint8_t i = 0;

    bits = num % 10;
    decade = num / 10 % 10;
    hundreds = num / 100;

    for(i = 0; i < 8; i++)
    {
        if(hundreds !=  0)
            tab3[i] |= (tab4[hundreds] & (1 << i)) << (7 - i);
        if(i <= 6)
            tab3[i] |= (tab4[decade] & (1 << i)) << (6 - i);
        else
            tab3[i] |= (tab4[decade] & (1 << i)) >> (i - 6);
        if(i <= 5)
            tab3[i] |= (tab4[bits] & (1 << i)) << (5 - i);
        else
            tab3[i] |= (tab4[bits] & (1 << i)) >> (i - 5);
    }
}
/*******************************
*函数名:display_right_num
*描    述:    是否有符号 是否有小数点
*参    数:
*返    回:
********************************/
void display_right_num(uint16_t num, uint8_t point, uint8_t negative)
{
    uint8_t bits = 0;
    uint8_t decade = 0;
    uint8_t hundreds = 0;
    uint8_t i = 0;

    bits = num % 10;
    decade = num / 10 % 10;
    hundreds = num / 100;

    for(i = 0; i < 8; i++)
    {
        if(!negative)                               //正数
        {
        if(i <= 4)
            tab3[i] |= (tab1[hundreds] & (1 << i)) << (4 - i);
        else
            tab3[i] |= (tab1[hundreds] & (1 << i)) >> (i - 4);
        }
        else                                                        //负数 第4位为-
        {
            if(i <= 4)
            tab3[i] |= (tab1[10] & (1 << i)) << (4 - i);
        else
            tab3[i] |= (tab1[10] & (1 << i)) >> (i - 4);
        }
        if(point)
        {
        if(i <= 3)
            tab3[i] |= (tab2[decade] & (1 << i)) << (3 - i);
        else
            tab3[i] |= (tab2[decade] & (1 << i)) >> (i - 3);
        }
        else
        {
            if(i <= 3)
            tab3[i] |= (tab1[decade] & (1 << i)) << (3 - i);
        else
            tab3[i] |= (tab1[decade] & (1 << i)) >> (i - 3);
        }

        if(i <= 2)
            tab3[i] |= (tab1[bits] & (1 << i)) << (2 - i);
        else
            tab3[i] |= (tab1[bits] & (1 << i)) >> (i - 2);
    }
}
/*******************************
*函数名: 清空tab3
*描    述:清空tab3
*参    数:
*返    回:
********************************/
void clear_tab3(void)
{
    uint8_t i;
    for(i = 0; i < 8; i++)
        tab3[i] = 0;
}

//-------------------------------------------------------------------------------------------------------------

/*******************************
*函数名:HT1632C_clr
*描    述:清屏函数
*参    数:
*返    回:
********************************/
void HT1632C_clr(void)
{
    unsigned char i;
    GPIO_ResetBits(HT_Port, HT_CS_Pin);             //di..
    HT1632C_Writer(0xa0,3);
    HT1632C_Writer(0x00<<1,7);
    for(i=0;i<36;i++)
    {
        HT1632C_Writer(0,8);
    }
    GPIO_SetBits(HT_Port, HT_CS_Pin);
}
/*******************************
*函数名:HT1632C_Init
*描    述:HT1632C初始化函数
*参    数:
*返    回:
********************************/
void HT1632C_Init(void)
{
    GPIO_SetBits(HT_Port, HT_DATA_Pin);
    GPIO_SetBits(HT_Port, HT_WR_Pin);
    GPIO_SetBits(HT_Port, HT_CS_Pin);
    HT1632C_Writer_CMD(SYS_DIS);
    HT1632C_Writer_CMD(COM_OPTION);                 //--------------
    HT1632C_Writer_CMD(RC_MASTER_MODE);
    HT1632C_Writer_CMD(SYS_EN);
    HT1632C_Writer_CMD(PWM_DUTY);
    HT1632C_Writer_CMD(BLINK_OFF);
    //HT1632C_Writer_CMD(BLINK_ON);
    HT1632C_Writer_CMD(LED_ON);
}

/************************************************************
*函数名:HT1632C_Writer_AllDATA
*描    述:指定地址写入连续数据，cnt为写入数据个数，按8位算
*参    数:Addr,*p,cnt
*返    回:
*************************************************************/
void HT1632C_Writer_AllDATA(unsigned char Addr,unsigned char *p,unsigned char cnt)
{                                                                                               //指定起始地址连续写入数据，cnt为写入数据个数，每个数据按位算
    unsigned char i;
    GPIO_ResetBits(HT_Port, HT_CS_Pin);
    HT1632C_Writer(0xa0,3);
    HT1632C_Writer(Addr<<1,7);
    for(i=0;i<cnt;i++)
    {
        HT1632C_Writer(*p,8);
        p++;
    }
    GPIO_SetBits(HT_Port, HT_CS_Pin);
}
/*******************************
*函数名:HT1632C_Writer_DATA
*描    述:指定地址写入数据
*参    数:Addr,Data
*返    回:
********************************/
void HT1632C_Writer_DATA(unsigned char Addr,unsigned char Data)
{
    GPIO_ResetBits(HT_Port, HT_CS_Pin);
    HT1632C_Writer(0xa0,3);                                    //命令码101
    HT1632C_Writer(Addr<<1,7);                                  //写入7位地址
    HT1632C_Writer(Data,4);                                    //写入4位数据
    GPIO_SetBits(HT_Port, HT_CS_Pin);
}
/*******************************
*函数名:HT1632C_Writer_CMD
*描    述:发送命令函数
*参    数:cmd
*返    回:
********************************/
void HT1632C_Writer_CMD(unsigned char cmd)
{
    GPIO_ResetBits(HT_Port, HT_CS_Pin);
    HT1632C_Writer(0x80,3);                                    //命令码100
    HT1632C_Writer(cmd,9);                                     //命令数据，第9位为0
    GPIO_SetBits(HT_Port, HT_CS_Pin);
}
/****************************************
*函数名:HT1632C_Writer
*描    述:HT1632C发送数据函数，高位在前
*参    数:Data,cnt
*返    回:
****************************************/
void HT1632C_Writer(unsigned char Data,unsigned char cnt)
{
    unsigned char i;
    for(i=0;i<cnt;i++)
    {
        GPIO_ResetBits(HT_Port, HT_WR_Pin);
        if(Data&0x80)
            GPIO_SetBits(HT_Port, HT_DATA_Pin);
        else
            GPIO_ResetBits(HT_Port, HT_DATA_Pin);
        Data<<=1;
        GPIO_SetBits(HT_Port, HT_WR_Pin);
    }
}



void Delay(__IO uint32_t nCount)     //简单的延时函数
{
    for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

#endif
