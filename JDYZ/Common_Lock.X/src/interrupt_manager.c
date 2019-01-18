/**
  Generated Interrupt Manager Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    interrupt_manager.c

  @Summary:
    This is the Interrupt Manager file generated using MPLAB(c) Code Configurator

  @Description:
    This header file provides implementations for global interrupt handling.
    For individual peripheral handlers please see the peripheral driver for
    all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 3.15.0
        Device            :  PIC16F18855
        Driver Version    :  1.02
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

#include "../inc/interrupt_manager.h"
#include "../inc/mcc.h"
#include "../inc/eusart.h"
#include "../inc/tmr0.h"

 extern uint8_t  gu8DoorState;
 extern uint8_t  gu8OperationState;
 extern uint16_t gu16RstTimCnt;

void interrupt INTERRUPT_InterruptManager (void)
{
   // interrupt handler
    if(PIE3bits.TX1IE == 1 && PIR3bits.TX1IF == 1)
    {
        EUSART_Transmit_ISR();
    }
    if(PIE3bits.RC1IE == 1 && PIR3bits.RC1IF == 1)
    {
        EUSART_Receive_ISR();
    }
    if(PIE0bits.TMR0IE == 1 && PIR0bits.TMR0IF == 1)
    {
        TMR0_ISR();
    }
    if(IOCBFbits.IOCBF1)//key open
    {
        gu8OperationState |= KEY_OPEN_PRESSED;
        IOCBFbits.IOCBF1 = 0;
    }
     if(IOCBFbits.IOCBF2)//touch open
    {
        gu8OperationState |= TOUCH_PERSSED;
        IOCBFbits.IOCBF2 = 0;
    }
     if(IOCBFbits.IOCBF5)//system reset
    {
        gu8OperationState  |= SYS_REST;
        gu16RstTimCnt = 0;
        TMR0_StartTimer();  //cal time
        IOCBFbits.IOCBF5 = 0;
    }
     if(IOCCFbits.IOCCF0)//one key close
    {
        gu8OperationState |= ONE_KEY_CLOSE;
        IOCCFbits.IOCCF0 = 0;
    }
}



void Interrupt_Enable(void)
{  
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
}
/**
 End of File
*/