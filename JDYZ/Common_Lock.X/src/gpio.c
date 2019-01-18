/**
  @Generated PIC10 / PIC12 / PIC16 / PIC18 MCUs Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    gpio.c

  @Summary:
    This is the gpio.c file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65
        Device            :  PIC16F15354
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.45 or later
        MPLAB             :  MPLAB X 4.15
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
#include <xc.h>
#include <stdint.h>
#include "../inc/gpio.h"


void PIN_MANAGER_Initialize(void)
{
    /**
    LATx registers
    */   
    LATA = 0x00;    
    LATB = 0x00;    
    LATC = 0x00;    

    /**
    TRISx registers
    */    
    // RA0 RA1   motor1 output  
    TRISA = 0xFC;
    //RB0 RB1 RB2 RB5 input,RB3 RB4 output
    TRISB = 0xE7;
    //RC0 input,RC3 IIC clk ,RC4 IIC DATA,RC6 Tx output,RC7 Rx input
    TRISC = 0xA7;

    /**
    ANSELx registers
    */   
    //RA0 RA1 digital
    ANSELA = 0xFC;
    //RB1 RB2 RB3 RB4 RB5 digital ,Rb0 Analog
    ANSELB = 0xC1;
    //RC0  RC3 RC4 RC5 RC6 RC7 digital ,
    ANSELC = 0x06;

    /**
    WPUx registers
    */ 
    WPUE = 0x00;
    WPUB = 0x26;
    WPUA = 0x00;
    WPUC = 0x01;

    /**
    ODx registers
    */   
    ODCONA = 0x00;
    ODCONB = 0x00;
    ODCONC = 0x00;
    
    
    //Uart pin 
    //Rx1
    RX1DTPPSbits.RX1DTPPS = 0x17;
    //Tx1
    RC6PPS = 0x0F;
    //RC2 output H
    PORTC |= 0x04;
    
    //Rc3 Rc4 IIC
    ODCONC |= 0x18;  
    
    
    /**
    IOCx registers
    */
    // interrupt on change for group IOCBF - flag
    //IOCBFbits.IOCBF1 = 0;
    //IOCBFbits.IOCBF2 = 0;
    //IOCBFbits.IOCBF3 = 0;  
    //IOCCFbits.IOCCF0 = 0;
    // interrupt on change for group IOCBN - negative
    //IOCBN |= 0x26;
    IOCBNbits.IOCBN1 = 1;
    IOCBNbits.IOCBN2 = 1;
    IOCBNbits.IOCBN5 = 1;
    IOCCNbits.IOCCN0 = 1;
    // interrupt on change for group IOCBP - positive
    //IOCBPbits.IOCBP1 = 0;
   
    // Enable IOCI interrupt 
    PIE0bits.IOCIE = 1; 

    //PORTA |= 0x02;    motor
   // PORTB |= 0x20;    buzzer
    //TX1CKPPSbits.TX1CKPPS = 0x16;
    
    //SSP1CLKPPSbits.SSP1CLKPPS = 0x13;   //RC3->MSSP1:SCK1;
    //SSP1DATPPSbits.SSP1DATPPS = 0x14;   //RC4->MSSP1:SDI1;
    //RC5PPS = 0x16;   //RC5->MSSP1:SDO1;
    //RC3PPS = 0x15;   //RC3->MSSP1:SCK1;

}       


uint8_t ReadRstPIN(void)
{
    if(PORTBbits.RB5)
    {
        return 1;
    }
    return 0;
}

uint8_t ReadTouchKeyPIN(void)
{
    if((PORTBbits.RB1)&&(PORTBbits.RB2))
    {
        return 2;
    }
    else if((PORTBbits.RB1)||(PORTBbits.RB2))
    {
        return 1;
    }
    return 0;
}

/**
 End of File
*/
