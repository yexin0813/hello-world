/*
 * UART0 Operations
 *
 * Copyright (C) 2017 Microarray Electronic Co., LTD.
 *
 * Written by:
 *     Hans.Huang  hux@microarray.com.cn
 *     
 * Hans.Huang    09/05/17     Create this file for UART0 Function Declars and MACRO Defines
 */


#ifndef _UART_H
#define _UART_H


#include <stdint.h>

#define RX_MASK						0x00FF
#define	RX_BUF_F					0x01   
#define RX_BUF_E					0x02   
#define USART_REC_LEN  		        (RX_MASK+1)






typedef struct
{
	uint8_t	    rx_buf[USART_REC_LEN];        
	uint16_t	rx_seek;                 
	uint16_t	rx_size;                 
	uint8_t	    rx_stat;    
  uint8_t   tx_done;	
}USARTypDef;



void uartInit(uint32_t baudrate);
int32_t  uartCheckData(void);
uint8_t   uartReadByte(int32_t *ch);
void uartSendBuff(uint8_t *dat, uint32_t len);
int32_t  uartSendBuffEmpty(void);


void debugPrint(const char *fmt, ...);
void init_uart0(const uint32_t baudrate);
int32_t  uart0CheckData(void);
uint8_t   uart0ReadByte(int32_t *ch);
void uart0SendBuff(uint8_t *dat, uint32_t len);
int32_t  uart0SendBuffEmpty(void);



 void uartDisableReceive(void);
 void uartEnableReceive(void);



#endif /* End of _UART_H */
