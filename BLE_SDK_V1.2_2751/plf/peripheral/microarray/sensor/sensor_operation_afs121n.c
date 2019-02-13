/*
 * AFS121N Sensor Interface Operation
 *
 * Copyright (C) 2017 Microarray Electronic Co., LTD.
 *
 * Written by:
 *     Hans.Huang  hux@microarray.com.cn
 *     
 * Hans.Huang    09/04/17     Create this file for AFS121N operation
 *                             
 */

#include <stdint.h>
#include "sensor_operation_afs121n.h"
#include <string.h>
#include "log.h"
#include "app_spi.h"
#include "co_math.h"
#include "m_timer.h"
#include "m_uart.h"


app_spi_inst_t spim0_inst = SPIM_INSTANCE(0);


volatile uint8_t spi_dma_finish = 1;






#define SPI0_CS_PIN (8)

/* Set the CS0 of SPI0 to low level */
#define SPI0_CS_LOW_LEVEL()     do {    \
    uint16_t i = 0;    \
	set_gpio_out_val(SPI0_CS_PIN,0);    \
	for(i = 0; i < 0x1000;i++)   \
	{   \
		__asm("nop");   \
	}   \
}while (0)
        
/* Set the CS0 of SPI0 to high level */
#define SPI0_CS_HIGH_LEVEL()     do {    \
    uint16_t i = 0;    \
	for(i = 0; i < 0x1000;i++)   \
	{   \
		__asm("nop");   \
	}   \
	set_gpio_out_val(SPI0_CS_PIN,1);    \
}while (0)





static void set_gpio_dir_out(uint32_t gpio_index)
{
    volatile uint32_t d_dir = *(volatile uint32_t *)0x20148004;
    d_dir |= (1<<(gpio_index));
    *(volatile uint32_t *)0x20148004 = d_dir;
}


static void set_gpio_out_val(uint32_t port , uint32_t val)
{
    volatile uint32_t d_gpio = *(volatile uint32_t *)0x20148000;
    d_gpio &= ~(1<<(port));
    if(val)
    {
        d_gpio |= 1 << port;
    }
    *(volatile uint32_t *)0x20148000 = d_gpio;
}


static void spim_dummy_cb(void *cb_param)
{
    spi_dma_finish = 0;
}



/*
 * Init the sensor interface
 */
void sensorInit(void)
{
    set_gpio_out_val(SPI0_CS_PIN,1);
    set_gpio_dir_out(SPI0_CS_PIN);
    timerDelayMs(2);


    spim0_inst.param.u.master.clk_div = 8;
    spim0_inst.param.dfs_32 = DFS_32_8_bits;
    spim0_inst.param.cpol = Inactive_Low;
    spim0_inst.param.cph = SCLK_Toggle_In_Middle;
    spim0_inst.param.clk_pin_no = 4;
    spim0_inst.param.mosi_pin_no = 6;
    spim0_inst.param.miso_pin_no = 5;
    spim0_inst.param.u.master.cs_pin_no[1] = 3;
    spim0_inst.param.u.master.hw_cs_mask = 0x0;
    spim0_inst.param.tx_dma = 1;
    spim0_inst.param.rx_dma = 1;
    app_spi_init(&spim0_inst.inst);
    
    timerDelayMs(2);
}











/*
 * Set the work mode of AFS121N
 * @mode, work mode 0x80 Power Down,0x84 Detect,0x88 Capture,0x8C Reset
 * @None Return Value
 */
void sensorSetMode(const uint8_t mode)
{
    uint8_t mode_buffer[1] = {0x00};
    uint8_t rx_buffer[3]   = {0x00};
    
    /* Set the work mode */
    mode_buffer[0]   = mode;



    SPI0_CS_LOW_LEVEL();
    spi_dma_finish = 1;
    app_spi_transmit_receive(&spim0_inst.inst,0x1,mode_buffer,rx_buffer,1,spim_dummy_cb,&spim0_inst);
    while(spi_dma_finish);
    SPI0_CS_HIGH_LEVEL();

    
    //timerDelayMs(2);
}


/*
 * Register read and write
 * @addr     Register address
 * @w_data   The data need to write
 * @old_data The first byte read from SPI slave
 * @new_data The second byte read from SPI slave
 * None return value
 */
void sensorRWReg(uint8_t addr, uint8_t w_data, uint8_t *old_data, uint8_t *new_data)
{
    uint8_t cmd_buffer[2] = {0x00};
    uint8_t rx_buffer[3] = {0x00};
    
    /* Write Register and Read Register */
    cmd_buffer[0] = addr;
    cmd_buffer[1] = w_data;

    SPI0_CS_LOW_LEVEL();
    spi_dma_finish = 1;
    app_spi_transmit_receive(&spim0_inst.inst,0x1,cmd_buffer,rx_buffer,2,spim_dummy_cb,&spim0_inst);
    while (spi_dma_finish);
    cmd_buffer[0] = AFS121N_DUMMY_DATA;
    cmd_buffer[1] = AFS121N_DUMMY_DATA;

    spi_dma_finish = 1;
    app_spi_transmit_receive(&spim0_inst.inst,0x1,cmd_buffer,rx_buffer,2,spim_dummy_cb,&spim0_inst);
    while (spi_dma_finish);


    
    SPI0_CS_HIGH_LEVEL();
    *old_data = rx_buffer[0];
    *new_data = rx_buffer[1];

    
    //timerDelayMs(3);
}


/*
 * Read a line data from AFS121N sensor
 * @buff, Store the data
 * @len,  Read the data length
 * None   return value
 */
void sensorReadLine(uint8_t *buff, uint32_t len)
{
    uint8_t cmd_buffer[2] = {0x00};
    uint8_t rx_buffer[3]  = {0x00};
    uint32_t i            = 0;

    

    /* Write Register and Read Register */
    cmd_buffer[0] = AFS121N_LINE_READ_CMD;
    cmd_buffer[1] = AFS121N_DUMMY_DATA;

    
    

    SPI0_CS_LOW_LEVEL();

    spi_dma_finish = 1;

    app_spi_transmit_receive(&spim0_inst.inst,0x1,cmd_buffer,rx_buffer,2,spim_dummy_cb,&spim0_inst);
    while (spi_dma_finish);

    /*
    cmd_buffer[0] = AFS121N_DUMMY_DATA;

    for (i = 0; i < read_length; i++)
    {
        app_spi_transmit_receive(&spim0_inst.inst,0x1,cmd_buffer,buff+i,1,spim_dummy_cb,&spim0_inst);
    }*/

    spi_dma_finish = 1;
    app_spi_transmit_receive(&spim0_inst.inst,0x1,buff,buff,121,spim_dummy_cb,&spim0_inst);
    while (spi_dma_finish);


	for(i = 0; i < 0x11000;i++)   
	{   
		__asm("nop");   
	}   



    SPI0_CS_HIGH_LEVEL();

    
    //timerDelayMs(5);
    
}


/*
 * Read a frame data from ASF121N sensor
 * @buff, Store the data
 * @len,  Read the data length
 * None   return value
 */
void sensorReadFrame(uint8_t *buff, uint32_t len)
{
    uint8_t cmd_buffer[2] = {0x00};
    uint8_t rx_buffer[3]  = {0x00};
    uint32_t i            = 0;

    


    /* Write Register and Read Register */
    cmd_buffer[0] = AFS121N_FRAME_READ_CMD;
    cmd_buffer[1] = AFS121N_DUMMY_DATA;
    
    SPI0_CS_LOW_LEVEL();

    spi_dma_finish = 1;

    app_spi_transmit_receive(&spim0_inst.inst,0x1,cmd_buffer,rx_buffer,2,spim_dummy_cb,&spim0_inst);
    while (spi_dma_finish);
    /*cmd_buffer[0] = AFS121N_DUMMY_DATA;

    for (i = 0; i < read_length; i++)
    {
        app_spi_transmit_receive(&spim0_inst.inst,0x1,cmd_buffer,buff+i,1,spim_dummy_cb,&spim0_inst);
    }*/
    spi_dma_finish = 1;

    app_spi_transmit_receive(&spim0_inst.inst,0x1,buff,buff,121*121,spim_dummy_cb,&spim0_inst);
    while (spi_dma_finish);



	for(i = 0; i < 0x21000;i++)   
	{   
		__asm("nop");   
	}   


    
    SPI0_CS_HIGH_LEVEL();
    
    //timerDelayMs(5);
    
}




/*
 * Read the product code of AFS121N
 * @code Store the product code return to caller,4 bytes
 * @None return values
 */
void read_product_code(uint8_t *code)
{
    uint8_t cmd_buffer[2] = {0x00};
    uint8_t rx_buffer[2] = {0x00};

    /* Read the Product Code 1,0x41 */
    cmd_buffer[0] = AFS121N_PRODUCT_CODE_CMD1;
    cmd_buffer[1] = 0x00;





    SPI0_CS_LOW_LEVEL();
    app_spi_transmit_receive(&spim0_inst.inst,0x1,cmd_buffer,rx_buffer,2,spim_dummy_cb,&spim0_inst);
    cmd_buffer[0] = AFS121N_DUMMY_DATA;
    cmd_buffer[1] = AFS121N_DUMMY_DATA;
    app_spi_transmit_receive(&spim0_inst.inst,0x1,cmd_buffer,code,2,spim_dummy_cb,&spim0_inst);
    SPI0_CS_HIGH_LEVEL();

    /* Read the Product Code 2,0x79 */
    cmd_buffer[0] = AFS121N_PRODUCT_CODE_CMD2;
    cmd_buffer[1] = 0x00;

    SPI0_CS_LOW_LEVEL();
    app_spi_transmit_receive(&spim0_inst.inst,0x1,cmd_buffer,rx_buffer,2,spim_dummy_cb,&spim0_inst);
    cmd_buffer[0] = AFS121N_DUMMY_DATA;
    cmd_buffer[1] = AFS121N_DUMMY_DATA;
    app_spi_transmit_receive(&spim0_inst.inst,0x1,cmd_buffer,&code[1],2,spim_dummy_cb,&spim0_inst);
    SPI0_CS_HIGH_LEVEL();


    /* Read the Company Code 1,0x4D */
    cmd_buffer[0] = AFS121N_COMPANY_CODE_CMD1;
    cmd_buffer[1] = 0x00;

    SPI0_CS_LOW_LEVEL();
    app_spi_transmit_receive(&spim0_inst.inst,0x1,cmd_buffer,rx_buffer,2,spim_dummy_cb,&spim0_inst);
    cmd_buffer[0] = AFS121N_DUMMY_DATA;
    cmd_buffer[1] = AFS121N_DUMMY_DATA;
    app_spi_transmit_receive(&spim0_inst.inst,0x1,cmd_buffer,&code[2],2,spim_dummy_cb,&spim0_inst);
    SPI0_CS_HIGH_LEVEL();




    /* Read the Company Code 2,0x41 */
    cmd_buffer[0] = AFS121N_COMPANY_CODE_CMD2;
    cmd_buffer[1] = 0x00;

    SPI0_CS_LOW_LEVEL();
    app_spi_transmit_receive(&spim0_inst.inst,0x1,cmd_buffer,rx_buffer,2,spim_dummy_cb,&spim0_inst);
    cmd_buffer[0] = AFS121N_DUMMY_DATA;
    cmd_buffer[1] = AFS121N_DUMMY_DATA;
    app_spi_transmit_receive(&spim0_inst.inst,0x1,cmd_buffer,&code[3],2,spim_dummy_cb,&spim0_inst);
    SPI0_CS_HIGH_LEVEL();

    
}


//#define AFS121N_TEST
#ifdef AFS121N_TEST
static uint8_t buff[121*121] = {0xFF};
static uint8_t product_id[5] = {0x00};




void afs121n_test(void)
{
    int32_t i = 0;
    uint32_t need_us = 0;
    
    read_product_code((uint8_t *)(&product_id[0]));

    debugPrint("Product ID 1 of AFS121N:%x\n", product_id[0]);
    
    timerDelayMs(20);
    debugPrint("Product ID 2  of AFS121N:%x\n", product_id[1]);
    
    timerDelayMs(20);
    debugPrint("Company ID 1 of AFS121N:%x\n", product_id[2]);
    
    timerDelayMs(20);
    debugPrint("Company ID 2 of AFS121N:%x\n", product_id[3]);

    timerDelayMs(20);

    

    
    sensorRWReg(0x00,0x00,&product_id[0],&product_id[1]);
    memset(product_id,0,4);
	sensorReadLine(buff,121);
    sensorReadFrame(buff, 121*121);
	sensorRWReg(0x00,0x00,&product_id[0],&product_id[1]);
	
	sensorRWReg(0x30,0x00,&product_id[0],&product_id[1]);
    sensorRWReg(0x34,0x00,&product_id[0],&product_id[1]);
    sensorRWReg(0x38,0x00,&product_id[0],&product_id[1]);
    sensorRWReg(0x3c,0x00,&product_id[0],&product_id[1]);

    timerRestart();
    sensorReadLine(buff,121);
    need_us = timerElapsedUs();
    debugPrint("***Read one line need_us = %u\n",need_us);

    
    timerDelayMs(50);

    
    for (i = 0; i < 121; i++){
        debugPrint(":%d is %x of one line\n",i,buff[i]);
        
        timerDelayMs(30);
    }

    


    timerRestart();
    sensorReadFrame(buff, 121*121);
    need_us = timerElapsedUs();
    debugPrint("***Read one frame need_us = %u\n",need_us);

    
    timerDelayMs(50);

    
    for (i = 0; i < 121*121; i++){
        debugPrint("*%d is %x of one frame\n",i,buff[i]);
        
        timerDelayMs(30);
    }

    
    
}
#endif




/*
 * Get the  level of IRQ pin
 * High level mean's touch, otherwise without touch
 *
 */
uint8_t get_touch_status(void)
{
    
    //return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11);
	  return 0;
}

void hardwareInit(void)
{
    /* Print the debug message through uart0 */
    init_uart0(UART_BAUDRATE_115200);
    
	  //sensorInit();
}



