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


#include "common_lock_bs81x.h"
#include "m_uart.h"
#include "common_lock_led.h"
#include "Hardware.h"

#define   KEY_TOUCH  17
//#include "common_lock_uart.h"


void Init_KeyTouch(void);


static uint8_t TkeyTable[] = { COMMON_LOCK_KEY_1_INDEX,
                            COMMON_LOCK_KEY_4_INDEX,
                            COMMON_LOCK_KEY_3_INDEX,
                            COMMON_LOCK_KEY_6_INDEX,
                            COMMON_LOCK_KEY_9_INDEX,
                            COMMON_LOCK_KEY_HASH_INDEX,
                            COMMON_LOCK_KEY_0_INDEX,
                            COMMON_LOCK_KEY_8_INDEX,
                            COMMON_LOCK_KEY_5_INDEX,
                            COMMON_LOCK_KEY_2_INDEX,
                            COMMON_LOCK_KEY_7_INDEX,
                            COMMON_LOCK_KEY_STAR_INDEX,
														COMMON_LOCK_KEY_LOCK_INDEX};
/*********************************************************************/
//    fuction name:
//        IIC_Start
//    description:
//        IIC_Start IIC bus
/*********************************************************************/
static void Com_Lock_IIC_Start_BS81X(void)
{
    MA_KIT_SDA_H;
    BX_DELAY_US(1);
    MA_KIT_SCL_H;
    
    BX_DELAY_US(2);
    MA_KIT_SDA_L;
    BX_DELAY_US(2);
    MA_KIT_SCL_L;
}


/*********************************************************************/
//    fuction name:
//        IIC_Stop
//    description:
//        IIC_Stop IIC bus
/*********************************************************************/
static void Com_Lock_IIC_Stop_BS81X(void)
{
    MA_KIT_SCL_L;
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
static char Com_Lock_IIC_Write_Data_BS81X(uint8_t dat)
{
    int          i = 0;
    char        retVal;
    MA_KIT_SCL_L;
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
        BX_DELAY_US(2);    
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
		BX_DELAY_US(20);
		/*
		io_cfg_input(SCL_PIN);
		while(MA_KIT_SCL_DATA == 0)
		{
			
		}
		io_cfg_output(SCL_PIN);
		MA_KIT_SCL_L;*/
    return retVal;

}


/*********************************************************************/
//    function name:
//        Com_LockIIC_Write_Data(void)
//    description
//        send one byte to IIC device.
/*********************************************************************/
static char Com_Lock_IIC_Write_Data_BS81X_AddrRead(uint8_t dat)
{
    int          i = 0;
    char        retVal;
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
        BX_DELAY_US(2);    
        
    }

    // Get ack from devise
    io_cfg_input(SDA_PIN);
		MA_KIT_SCL_L;
    BX_DELAY_US(2);
    MA_KIT_SCL_H;
    BX_DELAY_US(2);
    retVal = (char)MA_KIT_SDA_DATA;
    MA_KIT_SCL_L;
		//io_cfg_output(SDA_PIN);
		BX_DELAY_US(20);
		/*
		io_cfg_input(SCL_PIN);
		while(MA_KIT_SCL_DATA == 0)
		{
			
		}
		io_cfg_output(SCL_PIN);
		MA_KIT_SCL_L;*/
    return retVal;

}
/*********************************************************************/
//    function name:
//        Com_LockIIC_Write_Data(void)
//    description
//        send one byte to IIC device.
/*********************************************************************/
static char Com_Lock_IIC_Read_Data_BS81X(uint8_t ack)
{
    int          i = 0;
    char        retVal = 0;
    MA_KIT_SCL_L;
		io_cfg_input(SDA_PIN);
    for(i = 0; i < 8; i++)
    {
        BX_DELAY_US(2);
        MA_KIT_SCL_H;
        BX_DELAY_US(1);
        retVal <<= 1;
        retVal |= (char)MA_KIT_SDA_DATA;
        MA_KIT_SCL_L;
    }
		io_cfg_output(SDA_PIN);
    //ack
    if(ack)
    {
        MA_KIT_SDA_L;
        BX_DELAY_US(2);
        MA_KIT_SCL_H;
        BX_DELAY_US(1);
        MA_KIT_SCL_L;
    }
    //noack
    else
    {
        MA_KIT_SDA_H;
        BX_DELAY_US(2);
        MA_KIT_SCL_H;
        BX_DELAY_US(1);
        MA_KIT_SCL_L;
    }
		BX_DELAY_US(20);
		/*
		io_cfg_input(SCL_PIN);
		while(MA_KIT_SCL_DATA == 0)
		{
			
		}
		io_cfg_output(SCL_PIN);
		MA_KIT_SCL_L;*/
    return retVal;
    
}





/*********************************************************************/
//    function name:
//        Com_Lock_Init_Mfrc
//    description:
//        init Mfrc522.
/*********************************************************************/
uint8_t read[25];
void Com_Lock_Init_bs81x(void)
{
	 uint8_t cnt;
	 Com_Lock_IIC_Start_BS81X();
	 Com_Lock_IIC_Write_Data_BS81X(BS81X_SLA_ADDR);                //Salve Adress
   Com_Lock_IIC_Write_Data_BS81X(0xB0);                      //写数据
	
	 Com_Lock_IIC_Start_BS81X();  	 	   		          //重新启动
	 Com_Lock_IIC_Write_Data_BS81X_AddrRead(BS81X_SLA_ADDR+1);	//发送读器件指令	   
   for(cnt = 0;cnt <21;cnt++)
   {
			read[cnt] = Com_Lock_IIC_Read_Data_BS81X(1);		//读取一个字节,不继续再读,发送NAK 	

	 }		 
	 
   read[cnt] = Com_Lock_IIC_Read_Data_BS81X(0);		//读取一个字节,不继续再读,发送NAK 

   Com_Lock_IIC_Stop_BS81X();					//产生一个停止条件 	
	 
	 for(cnt = 5;cnt <20;cnt++)
	 {
		  read[cnt] = 0x30;
	 }
	 read[21] = 0;
	 for(cnt = 0;cnt <21;cnt++)
	 {
		  read[21] += read[cnt];
	 }
	 
	 
	 Com_Lock_IIC_Start_BS81X();
	 Com_Lock_IIC_Write_Data_BS81X(BS81X_SLA_ADDR);                //Salve Adress
   Com_Lock_IIC_Write_Data_BS81X(0xB0);   
	 for(cnt = 0;cnt <22;cnt++)
   {
		 Com_Lock_IIC_Write_Data_BS81X(read[cnt]);	
	 }		 
   Com_Lock_IIC_Stop_BS81X();					//产生一个停止条件 	
	 
	 
	 memset(read,0,22);
	 Com_Lock_IIC_Start_BS81X();
	 Com_Lock_IIC_Write_Data_BS81X(BS81X_SLA_ADDR);                //Salve Adress
   Com_Lock_IIC_Write_Data_BS81X(0xB0);                      //写数据
	
	 Com_Lock_IIC_Start_BS81X();  	 	   		          //重新启动
	 Com_Lock_IIC_Write_Data_BS81X_AddrRead(BS81X_SLA_ADDR+1);	//发送读器件指令	   
   for(cnt = 0;cnt <21;cnt++)
   {
			read[cnt] = Com_Lock_IIC_Read_Data_BS81X(1);		//读取一个字节,不继续再读,发送NAK 	

	 }		 
	 
   read[cnt] = Com_Lock_IIC_Read_Data_BS81X(0);		//读取一个字节,不继续再读,发送NAK 

   Com_Lock_IIC_Stop_BS81X();					//产生一个停止条件 
	 
	 Init_KeyTouch();

}

uint8_t ChanneltoData(uint16_t Chan)
{
	 uint8_t cnt,onecnt,data;
	 uint16_t tempdata = Chan;
	 onecnt = 0;
	 data = 0xff;
	 for(cnt = 0;cnt <15;cnt++)
	 {
		  if((cnt == 12)||(cnt == 13))
			{
				tempdata>>= 1;
				continue;
			}
			if(tempdata&0x01)
			{
				  onecnt ++;
				  data = TkeyTable[cnt];
				  if(cnt == 14)
					{
						 data = TkeyTable[12];
					}
		//			Com_Lock_Switch_Off_LED(COMMON_LOCK_LED_ALL_KEY_OFF_INDEX);
		//			Com_Lock_Switch_On_LED_Table(TkeyTable[cnt]);
			}
			tempdata>>= 1;
	 }	
	 if(onecnt > 1)
	 {
		  data = 0xff;
	 }
	 return data;
}


/*********************************************************************/
//    function name:
//        Com_Lock_Mfrc_Task(void)
//    description
//        show oled task
/*********************************************************************/
uint16_t readdata_l,readdata = 0;
uint8_t keyPressedNew,keyPressedOld,sameFlag;
//uint8_t read[25];
void Com_Lock_bs81x_Task(void)
{
	 
	 Com_Lock_IIC_Start_BS81X();
	 Com_Lock_IIC_Write_Data_BS81X(BS81X_SLA_ADDR);                //Salve Adress
   Com_Lock_IIC_Write_Data_BS81X(0x08);                      //写数据
	
	 Com_Lock_IIC_Start_BS81X();  	 	   		          //重新启动
	 Com_Lock_IIC_Write_Data_BS81X_AddrRead(BS81X_SLA_ADDR+1);	//发送读器件指令	    
	 readdata_l = Com_Lock_IIC_Read_Data_BS81X(1);		//读取一个字节,不继续再读,发送NAK 	
   readdata = Com_Lock_IIC_Read_Data_BS81X(0);	
	 readdata <<= 8;
	 readdata |= readdata_l;
   Com_Lock_IIC_Stop_BS81X();					//产生一个停止条件 
	 sameFlag = 1;
	
   keyPressedNew = ChanneltoData(readdata);
	 if ( keyPressedOld != keyPressedNew )
	 {
			sameFlag = 0;
	 }
   keyPressedOld = keyPressedNew;
	 if ( sameFlag )
	{
			if ( keyPressedNew <=12 )  
			{
					//if ( ledStatus[ keyPressedNew ] == 0 )
					//{
							Com_Lock_Switch_On_LED_Table( keyPressedNew );
					//		ledStatus[ keyPressedNew ] = 1;
					//}
					//else
					//{
					//		Com_Lock_Switch_Off_LED_Table( keyPressedNew );
					//		ledStatus[ keyPressedNew ] = 0;
					//}
					//keyStatus = 1;
					//Key_Sensor_Status |= 1 << keyPressedNew;
					//Com_Lock_M0_Printf( "Status = %x \n", Key_Sensor_Status );
					//Com_Lock_M0_Printf( "key %d pressed\n", keyPressedNew );
					//Com_Lock_Play_Voice(keyPressedNew + 1);
			}
			else
			{
				 Com_Lock_Switch_Off_LED(COMMON_LOCK_LED_ALL_KEY_OFF_INDEX);
			}
	}
		
		
	/*
	 Com_Lock_IIC_Start_BS81X();
	 Com_Lock_IIC_Write_Data_BS81X(BS81X_SLA_ADDR);                //Salve Adress
   Com_Lock_IIC_Write_Data_BS81X(0xB0);                      //写数据
	
	 Com_Lock_IIC_Start_BS81X();  	 	   		          //重新启动
	 Com_Lock_IIC_Write_Data_BS81X_AddrRead(BS81X_SLA_ADDR+1);	//发送读器件指令	   
   for(cnt = 0;cnt <21;cnt++)
   {
			read[cnt] = Com_Lock_IIC_Read_Data_BS81X(1);		//读取一个字节,不继续再读,发送NAK 	

	 }		 
	 
   read[cnt] = Com_Lock_IIC_Read_Data_BS81X(0);		//读取一个字节,不继续再读,发送NAK 

   Com_Lock_IIC_Stop_BS81X();					//产生一个停止条件 	
	 */
	 

}


void Key_Int_Callback(void)
{
	 if(g_hardware_input.key_touch)
	 {
		   io_ext_int_en(KEY_TOUCH,0);
		   io_ext_int_cfg(KEY_TOUCH,EXT_INT_TRIGGER_NEG_EDGE,Key_Int_Callback);
	     io_ext_int_en(KEY_TOUCH,1);
	     g_hardware_input.key_touch = 0;
		   //debugPrint(" key touch release\n");
		   g_hardware_input.key = KEY_LED_NONE;
	 }
	 else
	 {
			 Com_Lock_IIC_Start_BS81X();
			 Com_Lock_IIC_Write_Data_BS81X(BS81X_SLA_ADDR);                //Slave Adress
			 Com_Lock_IIC_Write_Data_BS81X(0x08);                      //写数据
			
			 Com_Lock_IIC_Start_BS81X();  	 	   		          //重新启动
			 Com_Lock_IIC_Write_Data_BS81X_AddrRead(BS81X_SLA_ADDR+1);	//发送读器件指令	    
			 readdata_l = Com_Lock_IIC_Read_Data_BS81X(1);		//读取一个字节,不继续再读,发送NAK 	
			 readdata = Com_Lock_IIC_Read_Data_BS81X(0);	
			 readdata <<= 8;
			 readdata |= readdata_l;
			 Com_Lock_IIC_Stop_BS81X();					//产生一个停止条件 
			 g_hardware_input.key = ChanneltoData(readdata);
		 
		   
		 
			 io_ext_int_en(KEY_TOUCH,0);
			 //debugPrint(" key touch press  %d\n",g_hardware_input.key);
			 io_ext_int_cfg(KEY_TOUCH,EXT_INT_TRIGGER_POS_EDGE,Key_Int_Callback);
			 io_ext_int_en(KEY_TOUCH,1);
			 g_hardware_input.key_touch = 1;
	 }
}
void Init_KeyTouch(void)
{
		io_ext_int_cfg(KEY_TOUCH,EXT_INT_TRIGGER_NEG_EDGE,Key_Int_Callback);
	  io_ext_int_en(KEY_TOUCH,1);
	  g_hardware_input.key_touch = 0;
	  
}






//=====================================================================
//    no more.
//=====================================================================

