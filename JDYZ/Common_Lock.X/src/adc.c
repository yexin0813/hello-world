/**
  ADC Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    adc.c

  @Summary
    This is the generated driver implementation file for the ADC driver using Foundation Services Library

  @Description
    This source file provides implementations for driver APIs for ADC.
    Generation Information :
        Product Revision  :  Foundation Services Library - 0.1.24
        Device            :  PIC16F15354
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
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

/**
  Section: Included Files
*/

#include <xc.h>
#include "../inc/adc.h"
#include "../inc/mcc.h"

/**
  Section: Macro Declarations
*/

#define ACQ_US_DELAY 5

/**
  Section: ADC Module APIs
*/

void ADC_Initialize(void)
{
    // set the ADC to the options selected in the User Interface
    
    // GO stop; ADON enabled; CHS ANC3; 
    ADCON0 = 0x4D; 
    
    // ADFM right; ADPREF VDD; ADCS FOSC/8; 
    ADCON1 = 0x90;
    
    // ADACT disabled; 
    ADACT = 0x00;
    
    // ADRESL 0; 
    ADRESL = 0x00;
    
    // ADRESH 0; 
    ADRESH = 0x00;
    //FVR 2.048v
    FVRCON |= 0x8A;
    
}
/*
void ADC_SelectChannel(adc_channel_t channel)
{
    // select the A/D channel
    ADCON0bits.CHS = channel;    
    // Turn on the ADC module
    ADCON0bits.ADON = 1;  
}

void ADC_StartConversion()
{
    // Start the conversion
    ADCON0bits.GO = 1;
}


bool ADC_IsConversionDone()
{
    // Start the conversion
    return (!ADCON0bits.GO);
}

adc_result_t ADC_GetConversionResult(void)
{
    // Conversion finished, return the result
    return ((ADRESH << 8) + ADRESL);
}
*/
adc_result_t ADC_GetConversion(adc_channel_t channel)
{
    // select the A/D channel
    ADCON0bits.CHS = channel;    

    // Turn on the ADC module
    ADCON0bits.ADON = 1;
    // Acquisition time delay
    __delay_us(ACQ_US_DELAY);

    // Start the conversion
    ADCON0bits.GO = 1;

    // Wait for the conversion to finish
    while (ADCON0bits.GO)
    {
    }
    // Conversion finished, return the result
    return ((ADRESH << 8) + ADRESL);

}


uint8_t ADC_Get_Vol(void)
{
    adc_result_t adcdata,voldata;
    uint8_t vol;
    //uint32_t vol;
    adcdata = ADC_GetConversion(channel_FVR_BUF2);//2.048V
    //EUSART_Write(adcdata >>8);  
    //EUSART_Write(adcdata);  
    voldata = ADC_GetConversion(0x08);
    vol = voldata*205/adcdata;
    //vol /= adcdata;
    //fvol = (voldata/adcdata)*2.048;
    //EUSART_Write(adcdata >>8);  
    //EUSART_Write(adcdata); 
    return vol;
    
}

uint8_t ADC_Test(void)
{
    adc_result_t adcdata;
    adcdata = ADC_GetConversion(channel_FVR_BUF2);
    //EUSART_Write(adcdata >>8);  
    //EUSART_Write(adcdata);  
    adcdata = ADC_GetConversion(0x08);
    //EUSART_Write(adcdata >>8);  
    //EUSART_Write(adcdata); 
    
}

/**
 End of File
*/