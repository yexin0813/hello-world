/*
 * UART0 Operations
 *
 * Copyright (C) 2017 Microarray Electronic Co., LTD.
 *
 * Written by:
 *     Hans.Huang  hux@microarray.com.cn
 *     
 * Hans.Huang    09/05/17     Create this file for UART0 Function Defines
 */


#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include "app_uart.h"
#include "m_uart.h"
#include "field_manipulate.h"
#include "dw_apb_uart_typedef.h" 
#include "m_timer.h"


/* Define a uart instance for uart0, uart0 for debug print,
 * uart1 use to communicate with PC test tool
 */
app_uart_inst_t debugPrint_uart0    = UART_INSTANCE(0);
app_uart_inst_t communication_uart1 = UART_INSTANCE(1);


/*
 * Define a data structure for Tx and Rx data of uart1
 */
volatile USARTypDef	uart_tag ;
/*
 * Define a data structure for Tx and Rx data of uart0
 */
volatile USARTypDef	uart_tag0 ;

/*
 * For uart1 rx isr store the rx character
 */
static uint8_t uart1_rx_data = 0x00;
/*
 * For uart0 rx isr store the rx character
 */
static uint8_t uart0_rx_data = 0x00;

/*
 * Define a string buffer use to debugPrint function,
 * move out from debugPrint function, in and out stack waste time.
 */
static uint8_t debugPrint_buf[256] = {0x00};



static void dummy_func0(void *dummy,uint8_t status);
static void dummy_func1(void *dummy,uint8_t status);



/*
 * Callback function of uart1 receive interrupt
 *
 *
 *
 */
 
static void uart00_rx_callback(void *dummy,uint8_t status)
{
    if(!(uart_tag0.rx_stat&RX_BUF_F))
    {
        uart_tag0.rx_buf[uart_tag0.rx_size] = (uart0_rx_data&0xFF);
        uart_tag0.rx_size++;
        uart_tag0.rx_size = uart_tag0.rx_size&RX_MASK;
    
        if(uart_tag0.rx_size == uart_tag0.rx_seek)
        {
            uart_tag0.rx_stat |= RX_BUF_F;
        }
        uart_tag0.rx_stat &= ~RX_BUF_E;
    }
    app_uart_read(&debugPrint_uart0.inst,&uart0_rx_data,1,uart00_rx_callback,NULL);
}

/*
 * Init the uart0 for debug print message
 *
 *
 */
void init_uart0(const uint32_t baudrate)
{
    debugPrint_uart0.param.baud_rate = (app_uart_baudrate_t)baudrate;
    debugPrint_uart0.param.rx_pin_no = 13;
    debugPrint_uart0.param.tx_pin_no = 12;
    debugPrint_uart0.param.tx_dma = 1;
    debugPrint_uart0.param.rx_dma = 1;
    app_uart_init(&debugPrint_uart0.inst);
	
	  memset((uint8_t *)&uart_tag0,0,sizeof(USARTypDef));
    uart_tag0.rx_stat = RX_BUF_E;
    uart_tag0.tx_done = 1;
    

    app_uart_read(&debugPrint_uart0.inst,&uart0_rx_data,1,uart00_rx_callback,NULL);
}


/*
 * Callback function of uart1 receive interrupt
 *
 *
 *
 */
 
static void uart1_rx_callback(void *dummy,uint8_t status)
{
    
    if(!(uart_tag.rx_stat&RX_BUF_F))
    {
        uart_tag.rx_buf[uart_tag.rx_size] = (uart1_rx_data&0xFF);
        uart_tag.rx_size++;
        uart_tag.rx_size = uart_tag.rx_size&RX_MASK;
    
        if(uart_tag.rx_size == uart_tag.rx_seek)
        {
            uart_tag.rx_stat |= RX_BUF_F;
        }
        uart_tag.rx_stat &= ~RX_BUF_E;
    }

   
    app_uart_read(&communication_uart1.inst,&uart1_rx_data,1,uart1_rx_callback,NULL);
}



/*
 * Init the uart1
 * For communicate with PC test tool
 * @baudrate  baudrate of uart1
 * 
 */
void uartInit(uint32_t baudrate)
{
    communication_uart1.param.baud_rate = (app_uart_baudrate_t)baudrate;
    communication_uart1.param.rx_pin_no = 21;
    communication_uart1.param.tx_pin_no = 22;
    communication_uart1.param.tx_dma = 1;
    communication_uart1.param.rx_dma = 1;
    app_uart_init(&communication_uart1.inst);
	


    memset((uint8_t *)&uart_tag,0,sizeof(USARTypDef));
    uart_tag.rx_stat = RX_BUF_E;
	  uart_tag.tx_done = 1;

    

    app_uart_read(&communication_uart1.inst,&uart1_rx_data,1,uart1_rx_callback,NULL);
}


/*
 * For external call
 *
 */
void UART_Configuration(void)
{
    uartInit(UART_BAUDRATE_115200);
}






/*
 * Check the available data
 * @1,has available data,0 data unavailable
 */
int32_t  uartCheckData(void)
{
	int32_t ret = 0x00;

	ret = ( uart_tag.rx_seek!= uart_tag.rx_size);

	return ret;
}

int32_t  uart0CheckData(void)
{
	int32_t ret = 0x00;

	ret = ( uart_tag0.rx_seek!= uart_tag0.rx_size);

	return ret;
}





/*
 * Read a byte from the uart buffer
 * @ch  read a byte to store into it
 * @read a byte and return 1, otherwise return 0
 */

uint8_t   uartReadByte(int32_t *ch)
{

	uint8_t	res = 0;
    
	
	if(!(uart_tag.rx_stat&RX_BUF_E))
	{
		*ch = uart_tag.rx_buf[uart_tag.rx_seek];
		uart_tag.rx_seek++;
		uart_tag.rx_seek = uart_tag.rx_seek&RX_MASK;
		if(uart_tag.rx_size == uart_tag.rx_seek)
		{
			uart_tag.rx_stat |= RX_BUF_E;
		}
		uart_tag.rx_stat &= ~RX_BUF_F;
		res=1;
	}
	else
	{
		*ch=-1;
	}
	return res;
}

uint8_t   uart0ReadByte(int32_t *ch)
{

	uint8_t	res = 0;
    
	
	if(!(uart_tag0.rx_stat&RX_BUF_E))
	{
		*ch = uart_tag0.rx_buf[uart_tag0.rx_seek];
		uart_tag0.rx_seek++;
		uart_tag0.rx_seek = uart_tag0.rx_seek&RX_MASK;
		if(uart_tag0.rx_size == uart_tag0.rx_seek)
		{
			uart_tag0.rx_stat |= RX_BUF_E;
		}
		uart_tag0.rx_stat &= ~RX_BUF_F;
		res=1;
	}
	else
	{
		*ch=-1;
	}
	return res;
}


/*
 * Transmit the data
 * @dat, a pointer for external buffer,ready to send 
 * @len,  the length for send from dat
 */
void uartSendBuff(uint8_t *dat, uint32_t len)
{
	 while(uart_tag.tx_done == 0)
	 {
	 }
    app_uart_write(&communication_uart1.inst,dat,len,dummy_func1,NULL);
	  uart_tag.tx_done = 0;
}



void uart0SendBuff(uint8_t *dat, uint32_t len)
{
		while(uart_tag0.tx_done == 0)
		{
			
		}
    app_uart_write(&debugPrint_uart0.inst,dat,len,dummy_func0,NULL);
	  uart_tag0.tx_done = 0;
}


/*
 * See if the UART available to transmit
 * @1,available to transmit.0,unavailable to transmit
 */
int32_t  uartSendBuffEmpty(void)
{
    reg_uart_t *reg = communication_uart1.reg;
    
    if (FIELD_RD(reg,USR,UART_TFNF)==Transmit_FIFO_Not_Full)
        return 1;
    else
        return 0;

    


}

int32_t  uart0SendBuffEmpty(void)
{
    reg_uart_t *reg = debugPrint_uart0.reg;
    
    if (FIELD_RD(reg,USR,UART_TFNF)==Transmit_FIFO_Not_Full)
        return 1;
    else
        return 0;
}


/*
 * Do Nothing.
 * For app_uart_write function obey the BlueX soft architecture.
 *
 */
static void dummy_func0(void *dummy,uint8_t status)
{
    volatile uint32_t dummy_value = 0x00;
    uart_tag0.tx_done = 1;
}


/*
 * Do Nothing.
 * For app_uart_write function obey the BlueX soft architecture.
 *
 */
static void dummy_func1(void *dummy,uint8_t status)
{
    volatile uint32_t dummy_value = 0x00;
    uart_tag.tx_done = 1;
}



/*
 * Print the log
 * @fmt,the variable parameter
 * @None return value
 */
void debugPrint(const char *fmt, ...)
{
	int32_t ret;
	while(uart_tag0.tx_done == 0)
	{
	}
	va_list args;
	va_start(args, fmt);
	ret = vsprintf((char *)debugPrint_buf, fmt, args);
	va_end(args);


    app_uart_write(&debugPrint_uart0.inst,debugPrint_buf,ret,dummy_func0,NULL);
		uart_tag0.tx_done = 0;
   // timerDelayMs(5);
}



void uartDisableReceive(void)
{
    ;
}


void uartEnableReceive(void)
{
    ;
}


