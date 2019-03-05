//=====================================================================
//    file mane:
//        common_lock_led.h
//    description:
//        led function head file.
//    date:
//        2018-5-9
//    author:
//        ian.lee
//    version:
//        v0.1        init version.
//    others:
//
//=====================================================================  


#include "common_lock_led.h"


static iicData          ledStatus;


static int keyTable[] = {   COMMON_LOCK_LED_0_INDEX,
                            COMMON_LOCK_LED_1_INDEX,
                            COMMON_LOCK_LED_2_INDEX,
                            COMMON_LOCK_LED_3_INDEX,
                            COMMON_LOCK_LED_4_INDEX,
                            COMMON_LOCK_LED_5_INDEX,
                            COMMON_LOCK_LED_6_INDEX,
                            COMMON_LOCK_LED_7_INDEX,
                            COMMON_LOCK_LED_8_INDEX,
                            COMMON_LOCK_LED_9_INDEX,
                            COMMON_LOCK_LED_STAR_INDEX,
                            COMMON_LOCK_LED_HASH_INDEX,
														COMMON_LOCK_LED_LOCK_INDEX};
/*********************************************************************/
//    function name:
//        Com_Lock_Init_LED
//    description:
//        init LED port.
/*********************************************************************/
void Com_Lock_Init_LED(void)
{
    //Cy_GPIO_Write(LED_SCL_0_PORT, LED_SCL_0_NUM, 1);
    //Cy_GPIO_Write(LED_SDA_0_PORT, LED_SDA_0_NUM, 1);

    ledStatus.busCmd = 0x40;
    ledStatus.devCmd = 0x06;  // configure
    ledStatus.dat0   = 0x00;  // output.
    ledStatus.dat1   = 0x00;
    Com_Lock_Update_LED();
    timerDelayMs(1);
    ledStatus.busCmd = 0x40;
    ledStatus.devCmd = 0x02;  // configure
    ledStatus.dat0   = 0x00;  // output.
    ledStatus.dat1   = 0x00;
    Com_Lock_Update_LED();
}


/*********************************************************************/
//    function name:
//        Com_Lock_Switch_On_LED
//    description:
//        switch on LED.
/*********************************************************************/
void Com_Lock_Switch_On_LED(uint32_t indx)
{
#if 1
    if(indx == COMMON_LOCK_LED_ALL_KEY_ON_INDEX)
    {
        ledStatus.dat0 = 0x00;
        ledStatus.dat1 = 0x00;
    }
    else
    {
        if(indx >= 16)
        {
            return;
        }
    
        if(indx <= 7)
        {
            ledStatus.dat0 &= ~(1ul << indx);
        }
        else
        {
            ledStatus.dat1 &= ~(1ul << (indx - 8));
        }
    }
#else
    if(indx == COMMON_LOCK_LED_ALL_KEY_ON_INDEX)
    {
        ledStatus.dat0 = 0xFF;
        ledStatus.dat1 = 0xFF;
    }
    else
    {
        if(indx >= 16)
        {
            return;
        }
    
        if(indx <= 7)
        {
            ledStatus.dat0 |= 1ul << indx;
        }
        else
        {
            ledStatus.dat1 |= 1ul << (indx - 8);
        }
    }
#endif

    Com_Lock_Update_LED();
}


/*********************************************************************/
//    function name:
//        Com_Lock_Light_LED
//    description:
//        switch off LED.
/*********************************************************************/
void Com_Lock_Switch_Off_LED(uint32_t indx)
{
#if 1
    if(indx == COMMON_LOCK_LED_ALL_KEY_OFF_INDEX)
    {
        ledStatus.dat0 = 0xFF;
        ledStatus.dat1 = 0xFF;
    }
    else
    {
        if(indx <= 7)
        {
            ledStatus.dat0 |= 1 << indx;
        }
        else
        {
            ledStatus.dat1 |= 1 << (indx - 8);
        }
    }
#else
    if(indx == COMMON_LOCK_LED_ALL_KEY_OFF_INDEX)
    {
        ledStatus.dat0 = 0x00;
        ledStatus.dat1 = 0x00;
    }
    else
    {
        if(indx <= 7)
        {
            ledStatus.dat0 &= ~(1ul << indx);
        }
        else
        {
            ledStatus.dat1 &= ~(1ul << (indx - 8));
        }
    } 
    
#endif
    Com_Lock_Update_LED();
}


/*********************************************************************/
//    function name:
//        Com_Lock_Switch_On_LED
//    description:
//        switch on LED.
/*********************************************************************/
void Com_Lock_Switch_On_LED_Table(uint32_t indx)
{
    if(indx == COMMON_LOCK_LED_ALL_KEY_ON_INDEX)
    {
        ledStatus.dat0 = 0xFF;
        ledStatus.dat1 = 0xFF;
    }
    else
    {
        if( keyTable[indx] >= 16)
        {
            return;
        }
        
        
        if( keyTable[indx] <= 7)
        {
            ledStatus.dat0 &= ~(1 << keyTable[indx]);
        }
        else
        {
            ledStatus.dat1 &= ~(1 << (keyTable[indx] - 8));
        }
    
        
    }
   
    Com_Lock_Update_LED();
}


/*********************************************************************/
//    function name:
//        Com_Lock_Light_LED
//    description:
//        switch off LED.
/*********************************************************************/
void Com_Lock_Switch_Off_LED_Table(uint32_t indx)
{
    if(indx == COMMON_LOCK_LED_ALL_KEY_OFF_INDEX)
    {
        ledStatus.dat0 = 0x00;
        ledStatus.dat1 = 0x00;
    }
    else
    {
        if( keyTable[indx] <= 7)
        {
            ledStatus.dat0 |= 1 << keyTable[indx];
        }
        else
        {
            ledStatus.dat1 |= 1 << (keyTable[indx] - 8);
        }
    }

    Com_Lock_Update_LED();
}


/*********************************************************************/
//    function name:
//        Com_Lock_Update_LED
//    description:
//        update led status.
/*********************************************************************/
void Com_Lock_Update_LED(void)
{
    BX_DELAY_US(5);
    Com_Lock_IIC_Start_Condition();
    Com_Lock_IIC_Write_Byte(ledStatus.busCmd);
    Com_Lock_IIC_Write_Byte(ledStatus.devCmd);
    Com_Lock_IIC_Write_Byte(ledStatus.dat0);
    Com_Lock_IIC_Write_Byte(ledStatus.dat1);
    Com_Lock_IIC_Stop_Condition();
    BX_DELAY_US(5);
}

/*********************************************************************/
//    fuction name:
//        IIC_Start_Condition
//    description:
//        IIC_Start_Condition IIC bus
/*********************************************************************/
void Com_Lock_IIC_Start_Condition(void)
{
    MA_KIT_SCL_H;
    MA_KIT_SDA_H;
    BX_DELAY_US(5);
    MA_KIT_SDA_L;
    BX_DELAY_US(5);
}


/*********************************************************************/
//    fuction name:
//        IIC_Stop_Condition
//    description:
//        IIC_Stop_Condition IIC bus
/*********************************************************************/
void Com_Lock_IIC_Stop_Condition(void)
{
    MA_KIT_SDA_L;
    BX_DELAY_US(5);
    MA_KIT_SCL_H;
    BX_DELAY_US(5);
    MA_KIT_SDA_H;
    BX_DELAY_US(5);
}


/*********************************************************************/
//    function name:
//        Com_LockIIC_Write_Byte(void)
//    description
//        send one byte to IIC device.
/*********************************************************************/
void Com_Lock_IIC_Write_Byte(uint8_t dat)
{
    int          i = 0;

    // Send char to devise
    MA_KIT_SCL_L;
    for(i = 0; i < 8; i++)
    {
        MA_KIT_SCL_L;
        BX_DELAY_US(2);
        if(dat & (1 << (7 - i)))
        {
            MA_KIT_SDA_H;
        }
        else
        {
            MA_KIT_SDA_L;
        }

        BX_DELAY_US(2);
        MA_KIT_SCL_H;
        BX_DELAY_US(4);    // delay4.0us
    }

    // Get ack from devise
    MA_KIT_SCL_L;
		io_cfg_input(SDA_PIN);
    BX_DELAY_US(5);
		
    MA_KIT_SCL_H;

    // retVal = (char)MA_KIT_SDA_DATA;
    BX_DELAY_US(5);
    MA_KIT_SCL_L;
		io_cfg_output(SDA_PIN);
    BX_DELAY_US(5);
    // MA_KIT_SDA_OUT;
}


/*********************************************************************/
//    fuction name:
//        Com_Lock_LED_Entry_PowerSaving
//    description:
//        IIC pin entry powersaving.
/*********************************************************************/
void Com_Lock_LED_Entry_PowerSaving(void)
{
   // Com_Lock_Switch_Off_LED(COMMON_LOCK_LED_ALL_KEY_OFF_INDEX);
   // MA_KIT_SDA_L;
   // MA_KIT_SDA_L;
   // CyDelayUs(5);
    
   // Cy_GPIO_SetDrivemode(LED_SCL_PORT,LED_SCL_NUM,CY_GPIO_DM_OD_DRIVESLOW_IN_OFF);
   // Cy_GPIO_SetDrivemode(LED_SDA_PORT,LED_SDA_NUM,CY_GPIO_DM_OD_DRIVESLOW_IN_OFF);
}


/*********************************************************************/
//    fuction name:
//        Com_Lock_LED_Exit_PowerSaving
//    description:
//        IIC pin exit powersaving.
/*********************************************************************/
void Com_Lock_LED_Exit_PowerSaving(void)
{    
   // Cy_GPIO_SetDrivemode(LED_SCL_PORT, LED_SCL_NUM, CY_GPIO_DM_STRONG_IN_OFF);
   // Cy_GPIO_SetDrivemode(LED_SDA_PORT, LED_SDA_NUM, CY_GPIO_DM_STRONG_IN_OFF);
}


//=====================================================================
//    no more.
//=====================================================================

