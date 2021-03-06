//=====================================================================
//    file mane:
//        common_lock_led.c
//    description:
//        led function c file.
//    date:
//        2018-11-15
//    author:
//        ye.xin
//    version:
//        v0.1        init version.
//    others:
//
//=====================================================================


#include "common_lock_oled.h"

void Com_Lock_Init_IC(void);
void Com_Lock_Program_Edit(void);
void Com_Lock_Clear_Screen(void);
void Com_Lock_Power_Edit(const uint8_t *pTab);

////////////////////////////////oled显示///////////////////////////////////////////////

//电量显示4节
uint8_t const power_tab_four[18] = 
{
    0x18,0xFF,
    0x81,0xBD,0xBD,0x81,0xBD,0xBD,0x81,0xBD,0xBD,0x81,0xBD,0xBD,0x81,0xFF,0x00,0x00,
};
//电量显示3节
uint8_t const power_tab_three[18] = 
{
    0x18,0xFF,
    0x81,0x81,0x81,0x81,0xBD,0xBD,0x81,0xBD,0xBD,0x81,0xBD,0xBD,0x81,0xFF,0x00,0x00, 
};
//电量显示2节
uint8_t const power_tab_two[18] = 
{
    0x18,0xFF,
    0x81,0x81,0x81,0x81,0x81,0x81,0x81,0xBD,0xBD,0x81,0xBD,0xBD,0x81,0xFF,0x00,0x00,
};

//电量显示1节
uint8_t const power_tab_one[18] = 
{
    0x18,0xFF,
    0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0xBD,0xBD,0x81,0xFF,0x00,0x00,
};





//1.增加指纹
//2.修改密码
//3.删除普通指纹
//4.返回
uint8_t const tab[1024]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x10,0xF0,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0xF8,0x40,0x00,0xF0,0x9C,0xF0,0xF0,0x9C,
0xF0,0x00,0x00,0x20,0x20,0xF8,0x20,0xE0,0x00,0xF0,0x10,0x10,0xF0,0x00,0x00,0x20,
0xFC,0x20,0x20,0x78,0x50,0x90,0x90,0x88,0x60,0x00,0x00,0x60,0xDC,0x60,0x20,0x30,
0xD0,0x1C,0x90,0x70,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,
0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x04,0x07,0x04,0x04,0x1E,0x16,0x16,0x16,0x16,
0x1E,0x00,0x00,0x18,0x07,0x00,0x10,0x0F,0x00,0x1F,0x08,0x08,0x1F,0x00,0x00,0x11,
0x1F,0x01,0x00,0x1F,0x15,0x15,0x15,0x15,0x1F,0x00,0x00,0x0B,0x0A,0x09,0x15,0x18,
0x0B,0x06,0x0B,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x10,0x10,0x10,0xF0,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x60,0xF8,0xE4,0x80,0xA0,0x58,0x6C,0xE8,0x58,
0x80,0x00,0x00,0x90,0x90,0x90,0xF0,0x40,0xB0,0x28,0x20,0xE0,0x20,0x00,0x00,0x98,
0x48,0x08,0xC8,0xBC,0xA8,0x48,0xA8,0x48,0xD8,0x00,0x00,0x88,0xF8,0x48,0xC8,0x00,
0xF8,0x88,0x88,0xF8,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x0A,0x09,0x08,0x00,
0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x07,0x00,0x12,0x15,0x15,0x0B,0x0A,
0x04,0x00,0x00,0x1F,0x10,0x08,0x14,0x10,0x09,0x06,0x07,0x08,0x10,0x00,0x00,0x02,
0x1D,0x11,0x11,0x11,0x1F,0x11,0x11,0x1D,0x00,0x00,0x00,0x01,0x0F,0x04,0x0F,0x02,
0x04,0x04,0x14,0x12,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x90,0x90,0x70,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xF8,0x88,0xF8,0x80,0xF8,0x88,0xF8,0xF8,0x00,
0xFC,0x00,0x00,0xF8,0x48,0xB8,0x40,0x20,0x50,0xCC,0x50,0x60,0x60,0x00,0x00,0x90,
0xB0,0xD4,0xF8,0x90,0x90,0xF8,0xD4,0xB0,0x90,0x00,0x00,0x48,0xD8,0x00,0xE8,0xA8,
0xB8,0xF8,0xB8,0xA8,0xE0,0x00,0x00,0x20,0xFC,0x20,0x20,0x78,0x50,0x90,0x90,0x88,
0x60,0x00,0x00,0x60,0xDC,0x60,0x20,0x30,0xD0,0x1C,0x90,0x70,0x10,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x08,0x08,0x07,0x00,
0x00,0x08,0x00,0x00,0x00,0x00,0x10,0x0F,0x10,0x1F,0x08,0x07,0x10,0x0F,0x03,0x10,
0x1F,0x00,0x00,0x1F,0x04,0x03,0x08,0x0D,0x15,0x1F,0x01,0x05,0x09,0x00,0x00,0x00,
0x1F,0x15,0x15,0x15,0x15,0x15,0x15,0x1F,0x00,0x00,0x10,0x18,0x0F,0x10,0x17,0x12,
0x12,0x17,0x1A,0x16,0x17,0x00,0x00,0x11,0x1F,0x01,0x00,0x1F,0x15,0x15,0x15,0x15,
0x1F,0x00,0x00,0x0B,0x0A,0x09,0x15,0x18,0x0B,0x06,0x0B,0x08,0x10,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x30,0xF0,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0xD8,0x00,0x00,0xF8,0xA8,0xA8,0x28,0xE8,
0x08,0x00,0x00,0xF8,0x08,0x08,0xE8,0x28,0x28,0xE8,0x08,0x08,0xF8,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x03,0x02,0x02,0x0F,0x02,
0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x10,0x0F,0x08,0x16,0x19,0x14,0x15,0x13,0x12,
0x14,0x00,0x00,0x1F,0x08,0x08,0x09,0x0A,0x0A,0x09,0x08,0x08,0x1F,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};



/*********************************************************************/
//    function name:
//        Com_Lock_Init_LED
//    description:
//        init LED port.
/*********************************************************************/
void Com_Lock_Init_OLED(void)
{
    Com_Lock_Init_IC();					 //初始化IC
    Com_Lock_Clear_Screen();
		Com_Lock_Program_Edit();
    Com_Lock_Power_Edit(power_tab_four);
}




/*********************************************************************/
//    fuction name:
//        IIC_Start
//    description:
//        IIC_Start IIC bus
/*********************************************************************/
static void Com_Lock_IIC_Start(void)
{
    MA_KIT_SCL_H;
    MA_KIT_SDA_H;
    BX_DELAY_US(3);
    MA_KIT_SDA_L;
    BX_DELAY_US(3);
    MA_KIT_SCL_L;
}


/*********************************************************************/
//    fuction name:
//        IIC_Stop
//    description:
//        IIC_Stop IIC bus
/*********************************************************************/
static void Com_Lock_IIC_Stop(void)
{
    MA_KIT_SDA_L;
    BX_DELAY_US(3);
    MA_KIT_SCL_H;
    BX_DELAY_US(3);
    MA_KIT_SDA_H;
    BX_DELAY_US(3);
}


/*********************************************************************/
//    function name:
//        Com_LockIIC_Write_Data(void)
//    description
//        send one byte to IIC device.
/*********************************************************************/
static char Com_Lock_IIC_Write_Data(uint8_t dat)
{
    int          i = 0;
    char        retVal;

    for(i = 0; i < 8; i++)
    {
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
        BX_DELAY_US(2);    // delay2.0us
        MA_KIT_SCL_L;
    }

    // Get ack from devise
    io_cfg_input(SDA_PIN);
    BX_DELAY_US(2);
    MA_KIT_SCL_H;
    BX_DELAY_US(2);
    retVal = (char)MA_KIT_SDA_DATA;
    MA_KIT_SCL_L;
		io_cfg_output(SDA_PIN);
    return retVal;

}


/*********************************************************************/
//    function name:
//        Com_Lock_Write_Command(void)
//    description
//        send one command to IIC device.
/*********************************************************************/
static void Com_Lock_Write_Command(uint8_t command)
{
   Com_Lock_IIC_Start();
   Com_Lock_IIC_Write_Data(Slave_Address);                //Salve Adress
   Com_Lock_IIC_Write_Data(OP_Command);                   //写命令
   Com_Lock_IIC_Write_Data(command); 
   Com_Lock_IIC_Stop();
}


/*********************************************************************/
//    function name:
//        Com_Lock_Write_Data(void)
//    description
//        send data to IIC device.
/*********************************************************************/
static void Com_Lock_Write_Data(uint8_t data)
{
   Com_Lock_IIC_Start();
   Com_Lock_IIC_Write_Data(Slave_Address);                //Salve Adress
   Com_Lock_IIC_Write_Data(OP_Data);                      //写数据
   Com_Lock_IIC_Write_Data(data);
   Com_Lock_IIC_Stop();
}

/*********************************************************************/
//    function name:
//        Com_Lock_Reset_IC(void)
//    description
//        init reset pin of oled
/*********************************************************************/
void Com_Lock_Reset_IC(void)
{
   timerDelayMs(10);
   //reset = 0
   timerDelayMs(50);
   //reset = 1
   timerDelayMs(100);
}


/*********************************************************************/
//    function name:
//        Com_Lock_Init_IC(void)
//    description
//        init ic registers
/*********************************************************************/
void Com_Lock_Init_IC(void)
{
    io_pin_set(RESET_PIN);
    //Cy_GPIO_SetDrivemode(RESB_PORT, RESB_NUM, CY_GPIO_DM_STRONG_IN_OFF);
    timerDelayMs(10);
    io_pin_clear(RESET_PIN);
    timerDelayMs(50);
    io_pin_set(RESET_PIN);
    timerDelayMs(100);
    
    
    
   Com_Lock_Write_Command(0xAE);     //Set Display Off 
   Com_Lock_Write_Command(0xd5);     //display divide ratio/osc. freq. mode	
   Com_Lock_Write_Command(0x80);     //

   Com_Lock_Write_Command(0xA8);     //multiplex ration mode:63 
   Com_Lock_Write_Command(0x3F);

   Com_Lock_Write_Command(0xD3);     //Set Display Offset   
   Com_Lock_Write_Command(0x00);
   
   Com_Lock_Write_Command(0x40);     //Set Display Start Line 
   
   Com_Lock_Write_Command(0xAD);     //DC-DC Control Mode Set 
   Com_Lock_Write_Command(0x8B);     //DC-DC ON/OFF Mode Set 
   //Write_Command(0x8A);     //DC-DC ON/OFF Mode Set 

   Com_Lock_Write_Command(0x32);     //Set Pump voltage value 

   Com_Lock_Write_Command(0xA1);     //Segment Remap	 

   Com_Lock_Write_Command(0xC8);     //Sst COM Output Scan Direction	

   Com_Lock_Write_Command(0xDA);     //common pads hardware: alternative	
   Com_Lock_Write_Command(0x12);

   Com_Lock_Write_Command(0x81);     //contrast control 
   Com_Lock_Write_Command(0xFF);

   Com_Lock_Write_Command(0xD9);	    //set pre-charge period	  
   Com_Lock_Write_Command(0x22);

   Com_Lock_Write_Command(0xDB);     //VCOM deselect level mode 
   Com_Lock_Write_Command(0x18);	    //

   Com_Lock_Write_Command(0xA4);     //Set Entire Display On/Off	

   Com_Lock_Write_Command(0xA6);     //Set Normal Display 

   Com_Lock_Write_Command(0xAF);     //Set Display On  
}


/*********************************************************************/
//    function name:
//        Com_Lock_Clear_Screen(void)
//    description
//        clear the oled
/*********************************************************************/

void Com_Lock_Clear_Screen(void)
{
   uint8_t page_number,column_number;
   for(page_number=MIN;page_number<PAGE_TOTAL;page_number++)
   {
     Com_Lock_Write_Command(START_PAGE+page_number);
     Com_Lock_Write_Command(START_HIGH_BIT);
     Com_Lock_Write_Command(START_LOW_BIT);
     for(column_number=MIN;column_number<COLUMN_MAX;column_number++)
     {
        Com_Lock_Write_Data(STATE_MIN);
     }
   }
}


/*********************************************************************/
//    function name:
//        Com_Lock_All_Screen(void)
//    description
//        All Screen light
/*********************************************************************/

void Com_Lock_All_Screen()
{
   uint8_t page_number,column_number;
   for(page_number=MIN;page_number<PAGE_TOTAL;page_number++)
   {
     Com_Lock_Write_Command(START_PAGE+page_number);
     Com_Lock_Write_Command(START_HIGH_BIT);
     Com_Lock_Write_Command(START_LOW_BIT);
     for(column_number=MIN;column_number<COLUMN_MAX;column_number++)
     {
        Com_Lock_Write_Data(STATE_MAX);
     }
   }
}

/*********************************************************************/
//    function name:
//        Com_Lock_Program_Edit(void)
//    description
//        show a full image
/*********************************************************************/
void Com_Lock_Program_Edit(void)
{
   uint8_t page_number,column_number;
   uint16_t piexl=MIN;
   for(page_number=MIN;page_number<PAGE_TOTAL;page_number++)
   {
      Com_Lock_Write_Command(START_PAGE+page_number);
      Com_Lock_Write_Command(START_HIGH_BIT);
      Com_Lock_Write_Command(START_LOW_BIT);
      for(column_number=MIN;column_number<COLUMN_MAX;column_number++)
      {
		 Com_Lock_Write_Data(tab[piexl]);
		 piexl++;
	  }
  }
}

/*********************************************************************/
//    function name:
//        Com_Lock_Power_Edit(void)
//    description
//        show power information
/*********************************************************************/
void Com_Lock_Power_Edit(const uint8_t *pTab)
{
   uint8_t page_number,column_number;
   uint16_t piexl=MIN;
   for(page_number=MIN;page_number<PAGE_ONE;page_number++)
   {
      Com_Lock_Write_Command(START_PAGE+page_number);
      Com_Lock_Write_Command(0x16);
      Com_Lock_Write_Command(0x0E);
      for(column_number=110;column_number<COLUMN_MAX;column_number++)
      {
				Com_Lock_Write_Data(pTab[piexl]);
				piexl++;
	  }
  }
}



/*********************************************************************/
//    function name:
//        Com_Lock_Oled_Task(void)
//    description
//        show oled task
/*********************************************************************/
void Com_Lock_Oled_Task(void)
{
#if 1
    timerDelayMs(500);
    Com_Lock_Power_Edit(power_tab_one);
    timerDelayMs(500);
    Com_Lock_Power_Edit(power_tab_two);
    timerDelayMs(500);
    Com_Lock_Power_Edit(power_tab_three);
    timerDelayMs(500);
    Com_Lock_Power_Edit(power_tab_four);
#endif

}


/*********************************************************************/
//    fuction name:
//        Com_Lock_OLED_Entry_PowerSaving
//    description:
//        IIC pin entry powersaving.
/*********************************************************************/
void Com_Lock_Oled_Entry_PowerSaving(void)
{
    //MA_KIT_SDA_L;
    //MA_KIT_SDA_L;
    //CyDelayUs(5);
    
    //Cy_GPIO_SetDrivemode(LED_SCL_PORT,LED_SCL_NUM,CY_GPIO_DM_OD_DRIVESLOW_IN_OFF);
    //Cy_GPIO_SetDrivemode(LED_SDA_PORT,LED_SDA_NUM,CY_GPIO_DM_OD_DRIVESLOW_IN_OFF);
}


/*********************************************************************/
//    fuction name:
//        Com_Lock_OLED_Exit_PowerSaving
//    description:
//        IIC pin exit powersaving.
/*********************************************************************/
void Com_Lock_Oled_Exit_PowerSaving(void)
{    
    //Cy_GPIO_SetDrivemode(LED_SCL_PORT, LED_SCL_NUM, CY_GPIO_DM_STRONG_IN_OFF);
    //Cy_GPIO_SetDrivemode(LED_SDA_PORT, LED_SDA_NUM, CY_GPIO_DM_STRONG_IN_OFF);
}


//=====================================================================
//    no more.
//=====================================================================

