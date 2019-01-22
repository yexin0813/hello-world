/**
  EUSART Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    eusart.c

  @Summary
    This is the generated driver implementation file for the EUSART driver using MPLAB(c) Code Configurator

  @Description
    This header file provides implementations for driver APIs for EUSART.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 3.15.0
        Device            :  PIC16F18855
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.20
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include "../inc/eusart.h"
#include "../inc/adc.h"
#include <stdio.h>
#include <stdarg.h>

/**
  Section: Macro Declarations
*/
#define EUSART_TX_BUFFER_SIZE 8
#define EUSART_RX_BUFFER_SIZE 20
#define EUSART_RX_TEMP_BUFFER_SIZE 20
#define COMMON_LOCK_CMD_FRAME_LENGTH 5


#define UART_FRAME_HEAD   0xF5
#define UART_FRAME_TAIL   0x5F

//uart frame cmd
#define UART_OPENDOOR 0x01
#define UART_GET_V    0x02

/**
  Section: Global Variables
*/

static uint8_t eusartTxHead = 0;
static uint8_t eusartTxTail = 0;
static uint8_t eusartTxBuffer[EUSART_TX_BUFFER_SIZE];
volatile uint8_t eusartTxBufferRemaining;

static uint8_t eusartRxHead = 0;
static uint8_t eusartRxTail = 0;
static uint8_t eusartRxBuffer[EUSART_RX_BUFFER_SIZE];
volatile uint8_t eusartRxCount;

static uint8_t eusartTempRxBuffer[EUSART_RX_TEMP_BUFFER_SIZE];
volatile uint8_t eusartTempRxCount;
static uint8_t eusartTempRxTail = 0;

static uint8_t eusartFrameDone = 0;

/**
  Section: EUSART APIs
*/

void EUSART_Initialize(void)
{
    // disable interrupts before changing states
    PIE3bits.RC1IE = 0;
    PIE3bits.TX1IE = 0;

    // Set the EUSART module to the options selected in the user interface.

    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD1CON = 0x48;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RC1STA = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN Enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TX1STA = 0x24;

    // Baud Rate = 9615; SP1BRGL 833; 32M clock    FOSC/[4 (n+1)]
    SP1BRGL = 0x44;

    // Baud Rate = 9615; SP1BRGH 0; 
    SP1BRGH = 0x00;


    // initializing the driver state
    eusartTxHead = 0;
    eusartTxTail = 0;
    eusartTxBufferRemaining = sizeof(eusartTxBuffer);

    eusartRxHead = 0;
    eusartRxTail = 0;
    eusartRxCount = 0;
    eusartTempRxCount = 0;

    // enable receive interrupt
    PIE3bits.RC1IE = 1;
}


void EUSART_Enable_WakeUp(void)
{
    BAUD1CON |= 0x02;
}

uint8_t EUSART_Read(void)
{
    uint8_t bitChk;
    //RC1STAbits.SREN = 1;
    if(eusartRxCount)
    {
        PIE3bits.RC1IE = 0;
        if((eusartTempRxCount == 0)&&(eusartRxBuffer[eusartRxTail]== UART_FRAME_HEAD))
        {
            eusartTempRxBuffer[eusartTempRxCount++] = eusartRxBuffer[eusartRxTail];
        }
        else if((eusartTempRxCount > 0) && (eusartTempRxCount < (COMMON_LOCK_CMD_FRAME_LENGTH - 1)))        // receive cmd data. 7
        {
             eusartTempRxBuffer[eusartTempRxCount++] = eusartRxBuffer[eusartRxTail];
        }
         else if(eusartTempRxCount == (COMMON_LOCK_CMD_FRAME_LENGTH - 1))   // one frame done.
        {
             if(eusartRxBuffer[eusartRxTail] == UART_FRAME_TAIL)
             {
                    // checke frame.
                    eusartTempRxBuffer[eusartTempRxCount++] = eusartRxBuffer[eusartRxTail];
                    bitChk = eusartTempRxBuffer[0] ^ eusartTempRxBuffer[1] ^ eusartTempRxBuffer[2];

                    if(eusartTempRxBuffer[3] == bitChk)
                    {
                        eusartFrameDone = 1;
                        //timeOut   = 0;
                    }
             }
             eusartTempRxCount   = 0;
        }

        eusartRxTail++;
        if(sizeof(eusartRxBuffer) <= eusartRxTail)
        {
            eusartRxTail = 0;
        }
        eusartRxCount--;
        PIE3bits.RC1IE = 1;
    }
    if(eusartFrameDone == 1)
    {
        return 1;
    }
    return 0;
}

void EUSART_Write(uint8_t txData)
{
    while(0 == eusartTxBufferRemaining)
    {
    }
    while(!(TX1STA&0x02))
    {
        
    }
    if(0 == PIE3bits.TX1IE)
    {
        TX1REG = txData;
    }
    else
    {
        PIE3bits.TX1IE = 0;
        eusartTxBuffer[eusartTxHead++] = txData;
        if(sizeof(eusartTxBuffer) <= eusartTxHead)
        {
            eusartTxHead = 0;
        }
        eusartTxBufferRemaining--;
        PIE3bits.TX1IE = 1;
    }
    //PIE3bits.TX1IE = 1;
}

void EUSART_Write_Frame(uint8_t cmd)
{
    uint8_t u8cnt;
    eusartTxBuffer[0] = UART_FRAME_HEAD;
    switch(cmd)
    {
        case UART_OPENDOOR:
            eusartTxBuffer[1] = UART_OPENDOOR;
            eusartTxBuffer[2] = 0x00;
            break;
        case UART_GET_V:
            eusartTxBuffer[1] = UART_GET_V;
            eusartTxBuffer[2] = ADC_Get_Vol();
            break;
        default:
            break;
    }
    eusartTxBuffer[3] = eusartTxBuffer[0] ^ eusartTxBuffer[1] ^ eusartTxBuffer[2];
    eusartTxBuffer[4] = UART_FRAME_TAIL;
    for(u8cnt = 0; u8cnt< COMMON_LOCK_CMD_FRAME_LENGTH;u8cnt++)
    {
        EUSART_Write(eusartTxBuffer[u8cnt]);
    }
    return;
}
void UART_Task(void)
{
    switch(eusartTempRxBuffer[1])
    {
        case UART_OPENDOOR:
            //open the dool
            EUSART_Write_Frame(UART_OPENDOOR);
            break;
        case UART_GET_V:
            EUSART_Write_Frame(UART_GET_V);
            break;
        default:
            break;
                    
        
    }
    
    
}

void Clear_FrameDone(void)
{
    eusartFrameDone = 0;
    eusartTempRxCount   = 0;
}



void EUSART_Transmit_ISR(void)
{

    // add your EUSART interrupt custom code
    if(sizeof(eusartTxBuffer) > eusartTxBufferRemaining)
    {
        TX1REG = eusartTxBuffer[eusartTxTail++];
        if(sizeof(eusartTxBuffer) <= eusartTxTail)
        {
            eusartTxTail = 0;
        }
        eusartTxBufferRemaining++;
    }
    else
    {
        PIE3bits.TX1IE = 0;
    }
}

void EUSART_Receive_ISR(void)
{
    uint8_t dumread;
    if(1 == RC1STAbits.OERR)
    {
        // EUSART error - restart

        RC1STAbits.SPEN = 0;
        RC1STAbits.SPEN = 1;
    }

    // buffer overruns are ignored
    if(BAUD1CON&0x02)
    {
        dumread = RC1REG;
    }
    else
    {
        eusartRxBuffer[eusartRxHead++] = RC1REG;
        if(sizeof(eusartRxBuffer) <= eusartRxHead)
        {
            eusartRxHead = 0;
        }
        eusartRxCount++;
    }
}


/**
  End of File
*/
