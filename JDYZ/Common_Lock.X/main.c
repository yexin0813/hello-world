/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65
        Device            :  PIC16F15354
        Driver Version    :  2.00
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

#include "inc/mcc.h"
#include "inc/gpio.h"
#include "inc/eusart.h"
#include "inc/adc.h"
#include "inc/mpu_iic.h"
#include "inc/interrupt_manager.h"
#include "inc/tmr0.h"

uint8_t  gu8OldDoorState = DOOR_INIT;
uint8_t  gu8DoorState = DOOR_COLSE;
uint8_t  gu8OperationState = NO_OPERATION;

extern uint16_t gu16RstTimCnt;
extern uint16_t gu16OpenTimCnt;
extern uint16_t gu16OpenTimCnt;



/*
                         Main application   
 */
void main(void) 
{
   // uint8_t ucdata;
    //adc_result_t adcdata;
    SYSTEM_Initialize();
    
    while (1)
    {
        switch(gu8DoorState)
        {
            case DOOR_COLSE:
                 //sleep
                if(gu8OperationState == NO_OPERATION)
                {
                    EUSART_Write(0x22); 
                    EUSART_Write(0x33); 
                    TMR0_StopTimer();
                    EUSART_Enable_WakeUp();
                    SLEEP();
                }
                gu8OldDoorState = DOOR_COLSE;
                gu8DoorState = DOOR_WAIT_OPERATON;
                break;
            case DOOR_OPEN:
                if(CheckDoorOpened())
                {
                    if(CheckDoorClosed())
                    {
                        //close the door
                        EUSART_Write(0x22); 
                        EUSART_Write(0x77); 
                        gu16OpenTimCnt = 0;
                        gu8OldDoorState = DOOR_OPEN;
                        gu8DoorState = DOOR_COLSE;
                        break;
                    }  
                }
                if(gu16OpenTimCnt >= 18000)//18000*10ms = 180s = 3min
                {
                    //报警
                    EUSART_Write(0x22); 
                    EUSART_Write(0x44); 
                    gu16OpenTimCnt = 0;
                }

            case DOOR_WAIT_OPERATON:
                if(gu8OperationState&SYS_REST)//系统恢复出厂状态
                {
                    if(ReadRstPIN())
                    {
                        gu8OperationState &=~SYS_REST;
                        gu16RstTimCnt = 0;
                    }
                    else
                    {
                        if(gu16RstTimCnt >= 300)//300*10ms = 3s
                        {
                            //add reset operation send by uart
                            EUSART_Write(0x22); 
                            EUSART_Write(0x88); 
                            gu8OperationState &=~SYS_REST;
                            gu16RstTimCnt = 0;
                        }
                    }
                }
                if(gu8OperationState&ONE_KEY_CLOSE)//一键关门
                {
                    if(gu8OldDoorState != DOOR_COLSE)
                    {
                        EUSART_Write(0x22); 
                        EUSART_Write(0x55); 
                        //add close door operation
                    }
                    gu8OperationState &= ~ONE_KEY_CLOSE;
                }
                if(gu8OperationState&0x03)//touch or key press
                {
                    if(ReadTouchKeyPIN() == 2)
                    {
                        gu8OperationState &= 0xFC;
                    }
                    else if(ReadTouchKeyPIN() == 0)
                    {
                        if(gu8DoorState == DOOR_WAIT_OPERATON)
                        {
                            gu16OpenTimCnt = 0;
                            ClearAngle();
                            TMR0_StartTimer();  //cal time
                            EUSART_Write(0x22); 
                            EUSART_Write(0x66); 
                            //open the door operation
                        }
                        gu8OldDoorState = DOOR_OPEN;
                        gu8DoorState = DOOR_OPEN;
                        gu8OperationState &= 0xFC;
                    }
                }
                if(gu8OperationState == NO_OPERATION)
                {
                    if(gu8OldDoorState == DOOR_COLSE)
                    {
                        //TMR0_StopTimer();
                        gu8DoorState = DOOR_COLSE;
                    }
                }
                break;
            
            default:
                break;
        }
        /*
        if(gbDoorState)//if door closed
        {
            //sleep
            EUSART_Write(0x22); 
            EUSART_Write(0x33); 
            TMR0_StopTimer();
            EUSART_Enable_WakeUp();
            SLEEP();
        }
        else
        {
            
        }*/
        //EUSART_Write(0xAA);
        
        //Test_Sleep_Mpu6500();
        //Test_MPU_6500();
        //EUSART_Write(0x55); 
        
        //EUSART_Enable_WakeUp();
        //SLEEP();
        //__delay_ms(10);
        //while(1)
       //{
            //ucdata = EUSART_Read();
            //adcdata = ADC_GetConversion(0x3F);
            //EUSART_Write(adcdata >>8);  
            //EUSART_Write(0x55);  
            //adcdata = ADC_GetConversion(0x03);
            //EUSART_Write(adcdata >>8);  
            //EUSART_Write(adcdata); 
       // }
       // __delay_ms(1000);
    }
}

/**
 End of File
*/