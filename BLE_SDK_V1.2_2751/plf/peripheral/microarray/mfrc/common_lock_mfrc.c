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


#include "common_lock_mfrc.h"
#include "m_uart.h"
//#include "common_lock_uart.h"



/*********************************************************************/
//    fuction name:
//        IIC_Start
//    description:
//        IIC_Start IIC bus
/*********************************************************************/
static void Com_Lock_IIC_Start(void)
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
static void Com_Lock_IIC_Stop(void)
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
static char Com_Lock_IIC_Write_Data(uint8_t dat)
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
		io_cfg_output(SDA_PIN);
    return retVal;

}



static char Com_Lock_IIC_Write_AddrData(uint8_t dat)
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
    return retVal;

}

/*********************************************************************/
//    function name:
//        Com_LockIIC_Write_Data(void)
//    description
//        send one byte to IIC device.
/*********************************************************************/
static char Com_Lock_IIC_Read_Data(uint8_t ack)
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
    return retVal;
    
}


uint8_t testcnt = 0;
volatile unsigned char w_ack;
static void Com_Lock_RC522_WR_Reg(uint8_t RCsla,uint8_t addr,uint8_t val) 
{

		Com_Lock_IIC_Start();  	
    w_ack = Com_Lock_IIC_Write_Data(RCsla);
	  if(!w_ack)
		{
      testcnt +=1;
		}
    w_ack = Com_Lock_IIC_Write_Data(addr);
		
    w_ack = Com_Lock_IIC_Write_Data(val);
      
		if(!w_ack)
		{
      testcnt +=1;
		}
    Com_Lock_IIC_Stop();						//stop iic 	   
}
//读寄存器
//addr:寄存器地址
//返回值:读到的值
static uint8_t Com_Lock_RC522_RD_Reg(uint8_t RCsla,uint8_t addr) 		
{
	uint8_t temp=0;		 
	Com_Lock_IIC_Start();  	
  temp = Com_Lock_IIC_Write_Data(RCsla);
    
	   
  temp = Com_Lock_IIC_Write_Data(addr);   		//发送寄存器地址	 										  		   
	Com_Lock_IIC_Start();  	 	   		          //重新启动
	temp = Com_Lock_IIC_Write_AddrData(RCsla+1);	//发送读器件指令	    
  temp=Com_Lock_IIC_Read_Data(0);		//读取一个字节,不继续再读,发送NAK 	    	   
  Com_Lock_IIC_Stop();					//产生一个停止条件 	    
	return temp;				//返回读到的值
}  

uint16_t OK=0;//检测函数状态 位1：读、2：写。
uint8_t DAA;
//读RC522寄存器
//DZ:寄存器地址
//返回：读出的值

uint8_t R_RC522(uint8_t DZ)
{						   
	uint8_t DAT=0;	
  DAT = Com_Lock_RC522_RD_Reg(SLA_ADDR,  DZ);
	return DAT;          		//返回收到的数据
}

//功    能：写RC522寄存器
//参数说明：DZ:寄存器地址
//          DATA:写入的值

void W_RC522(uint8_t DZ,uint8_t DATA)
{
    Com_Lock_RC522_WR_Reg(SLA_ADDR,  DZ, DATA);
}

//功    能：置RC522寄存器位
//参数说明：DZ:寄存器地址
//          mask:置位值
void SetBitMask(uint8_t DZ,uint8_t mask)
{
    uint8_t tmp = 0x0;
    tmp = R_RC522(DZ);
	//CyDelay(2);
    W_RC522(DZ,tmp | mask);	
}

//功    能：清RC522寄存器位
//参数说明：DZ:寄存器地址
//          mask:清位值
void ClearBitMask(uint8_t DZ,uint8_t mask)  
{
    uint8_t tmp = 0x0;
    tmp = R_RC522(DZ);
	//CyDelay(2);
	  tmp = tmp&(~mask);
    W_RC522(DZ, tmp);  // clear bit mask
} 


void testaddress(void)
{
	unsigned char cnt,add = 0;
	for(cnt = 0; cnt <127 ; cnt ++)
	{
		 Com_Lock_RC522_WR_Reg((add<<1),  CommandReg, PCD_RESETPHASE);
		 add++;
	}

	return;

}


void PcdReset(void)
{
    //reset pin = 0
    //CyDelay(10);
    //reset pin = 1
    //CyDelay(10);  //0x01     / 0x0F //复位

		//testaddress();
    W_RC522(CommandReg,PCD_RESETPHASE);//
    //CyDelay(200);
                //0x11
    W_RC522(ModeReg,0x3D); //和Mifare卡通讯，CRC初始值0x6363
	//CyDelay(5);
					//0x2D
    W_RC522(TReloadRegL,30);
	//CyDelay(5);           
					//0x2C
    W_RC522(TReloadRegH,0);
	//CyDelay(5);
					// 0x2A
    W_RC522(TModeReg,0x8D);
	//CyDelay(5);
					// 0x2B
    W_RC522(TPrescalerReg,0x3E);
	//CyDelay(5);
					//0x15
    W_RC522(TxAutoReg,0x40);     
    
}


//开启天线 
//off:1、开启。0、关闭 
//每次启动或关闭天险发射之间应至少有1ms的间隔
/////////////////////////////////////////////////////////////////////
void PcdAntennaOn(uint8_t off)
{
    uint8_t i;
		if(off&1)
		{
	    i = R_RC522(TxControlReg);
	    if (!(i & 0x03))
	    {
	        SetBitMask(TxControlReg, 0x03);
	    }
		}
		else {ClearBitMask(TxControlReg, 0x03);}
}


//////////////////////////////////////////////////////////////////////
//设置RC632的工作方式 
//////////////////////////////////////////////////////////////////////
void M500PcdConfigISOType()
{
   ClearBitMask(Status2Reg,0x08);
   W_RC522(ModeReg,0x3D);//3F
   W_RC522(RxSelReg,0x86);//84
   W_RC522(RFCfgReg,0x7F);   //4F
   W_RC522(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
   W_RC522(TReloadRegH,0);
   W_RC522(TModeReg,0x8D);
   W_RC522(TPrescalerReg,0x3E);
   //CyDelay(10);
//   PcdAntennaOn(1);
}



//功    能：通过RC522和ISO14443卡通讯
//参数说明：Command[IN]:RC522命令字
//          pInData[IN]:通过RC522发送到卡片的数据
//          InLenByte[IN]:发送数据的字节长度
//          pOutData[OUT]:接收到的卡片返回数据
//          *pOutLenBit[OUT]:返回数据的位长度
//成功置OK位1为1，否则为0
/////////////////////////////////////////////////////////////////////
void PcdComMF522(uint8_t Command,uint8_t *pInData,uint8_t InLenByte,uint8_t *pOutData,uint16_t  *pOutLenBit)
{
    uint8_t irqEn   = 0x00;
    uint8_t waitFor = 0x00;
    uint8_t lastBits;
    uint8_t n;
    uint16_t i;
		OK&=~(1<<0);
    switch (Command)
    {
       case PCD_AUTHENT://0x0E//验证密钥
          irqEn   = 0x12;
          waitFor = 0x10;
          break;
       case PCD_TRANSCEIVE://0x0C//发送并接收数据
          irqEn   = 0x77;
          waitFor = 0x30;
          break;
       default:
         break;
    }
    //			0x02/中断请求使、禁能
    W_RC522(ComIEnReg,irqEn|0x80);//写寄存器
    //				0x04/中断请求标志
    ClearBitMask(ComIrqReg,0x80);
    //				0x01				0x00//取消当前命令
    W_RC522(CommandReg,PCD_IDLE);
    //				0x0A指示存储在FIFO中的字节数
    SetBitMask(FIFOLevelReg,0x80);
    
    for (i=0; i<InLenByte; i++)
    {   
		W_RC522(FIFODataReg, pInData[i]);    }
        //					0x01			//
		W_RC522(CommandReg, Command);
   
    //				0x0C//发送并接收数据
    if (Command == PCD_TRANSCEIVE)
    {    //			 0x0D/面向帧调节
			SetBitMask(BitFramingReg,0x80);//作用：开启发送  
	}
    
     //    i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
     i = 600;
    do 
    {
		 
	    //				0x04/中断标志
         n = R_RC522(ComIrqReg);
         i--;
    }//								0x30
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    ClearBitMask(BitFramingReg,0x80);//清除寄存器位：关闭发送
	      
    if (i!=0)
    {    		//	0x06/错误标志
         if(!(R_RC522(ErrorReg)&0x1B))//读寄存器：检测错误标志
         {
             OK|=1<<0;
             if (n & irqEn & 0x01)
				//		(-1)	
             {   OK&=~(1<<0);   }

             if (Command == PCD_TRANSCEIVE)//判断输入命令是否发送接收
             {//					0x0A/指示存储在FIFO中的字节数
               	n = R_RC522(FIFOLevelReg);//读取FIFO缓存区保存的字节数
					//							0x0C/可能是定时器控制
              	lastBits = R_RC522(ControlReg) & 0x07;//显示最后接收到
																		//的字节有效位个数。
                if (lastBits)
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > 18)
                {   n = 18;   }
                for (i=0; i<n; i++)
                {   //					0X09/FIFO缓存区
						pOutData[i] = R_RC522(FIFODataReg);//读取缓存区数据
				}
            }
         }
         else
         {   OK&=~(1<<0);   }
        
   }
   
//				0x0C
   SetBitMask(ControlReg,0x80);           // stop timer now
//					0x01/启动、停止。0x00//取消当前命令
   W_RC522(CommandReg,PCD_IDLE); 
}

 //功    能：寻卡
//参数说明: req_code[IN]:寻卡方式
//                0x52 = 寻感应区内所有符合14443A标准的卡
//                0x26 = 寻未进入休眠状态的卡
//          pTagType[OUT]：卡片类型代码
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
//成功置OK位2为1，否则为0
/////////////////////////////////////////////////////////////////////
void PcdRequest(uint8_t req_code,uint8_t *pTagType)
{
//   u8 KO;
   uint16_t unLen;
   uint8_t ucComMF522Buf[18];
   OK&=~(1<<1); 
//  unsigned char xTest ;
//              0x08//接收发送状态寄存器
   ClearBitMask(Status2Reg,0x08);//清寄存器位 
//					0x0D/调整为面向比特帧
   W_RC522(BitFramingReg,0x07);//写寄存器

//  xTest = ReadRawRC(BitFramingReg);
//  if(xTest == 0x07 )
 //   { LED_GREEN  =0 ;}
 // else {LED_GREEN =1 ;while(1){}}
//				 0x14/控制天线驱动管脚TX1和TX2的逻辑行为
   SetBitMask(TxControlReg,0x03);//置寄存器位 
 
   ucComMF522Buf[0] = req_code;

   PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);//通过RC522和ISO14443卡通讯
//     if(status  == MI_OK )
//   { LED_GREEN  =0 ;}
//   else {LED_GREEN =1 ;}
//   KO=OK&1;
   if ((OK&1) && (unLen == 0x10))
   {    
       *pTagType     = ucComMF522Buf[0];
       *(pTagType+1) = ucComMF522Buf[1];
	   OK|=1<<1;
   }
//   else
//   {   OK&=~(1<<4);   }
   
}

//功    能：防冲撞
//参数说明: pSnr[OUT]:卡片序列号，4字节
//成功置OK位3为1，否则为0
/////////////////////////////////////////////////////////////////////  
void PcdAnticoll(uint8_t *pSnr)
{
  
    uint8_t i,snr_check=0;
    uint16_t  unLen;
    uint8_t   ucComMF522Buf[18]; 
    OK&=~(1<<2);
//					 0x08//接收发送状态寄存器
    ClearBitMask(Status2Reg,0x08);//清寄存器位
//					0x0D/调整为面向比特帧
    W_RC522(BitFramingReg,0x00);//写寄存器
//				0x0E/检测冲突
    ClearBitMask(CollReg,0x80);//清寄存器位

//							  0x93 //防冲撞
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;
//						 0x0C//发送并接收数据
    PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (OK&1)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
		 OK|=1<<2;
    }
    
    SetBitMask(CollReg,0x80);
    
}




/*********************************************************************/
//    function name:
//        Com_Lock_Reset_IC(void)
//    description
//        init reset pin of oled
/*********************************************************************/
void Com_Lock_Reset_Mfrc(void)
{
   timerDelayMs(10);
   io_pin_clear(RESET_PIN);
   timerDelayMs(50);
   io_pin_set(RESET_PIN);
   timerDelayMs(100);
}


/*********************************************************************/
//    function name:
//        Com_Lock_Init_Mfrc
//    description:
//        init Mfrc522.
/*********************************************************************/
void Com_Lock_Init_Mfrc(void)
{
	  io_pin_set(SCL_PIN);
	  io_pin_set(SDA_PIN);
	  io_cfg_output(SCL_PIN);
	  io_cfg_output(SDA_PIN);
	  io_cfg_output(RESET_PIN);
	
	  io_pin_set(RESET_PIN);
    /*
    Com_Lock_Reset_Mfrc();
    PcdReset();
    M500PcdConfigISOType();
		PcdAntennaOn(0);
		timerDelayMs(2);
		PcdAntennaOn(1);*/
}





/*********************************************************************/
//    function name:
//        Com_Lock_Mfrc_Task(void)
//    description
//        show oled task
/*********************************************************************/
unsigned char cnt,g_ucTempbuf[20];
void Com_Lock_Mfrc_Task(void)
{
	for(cnt = 0;cnt <20;cnt ++)
	{
		g_ucTempbuf[cnt] = 0;
	}


  PcdRequest(PICC_REQALL, g_ucTempbuf);//寻卡
	PcdAnticoll(g_ucTempbuf);//防冲撞
 
	PcdReset();
  M500PcdConfigISOType();
	PcdAntennaOn(0);
	timerDelayMs(2);
	PcdAntennaOn(1);

}

void  printRFID(void)
{
	 debugPrint("\r\n RFID Num");
	for(cnt = 0;cnt <16;cnt ++)
	{
		debugPrint("%d ",g_ucTempbuf[cnt]);
	}
}


/*********************************************************************/
//    fuction name:
//        Com_Lock_Mfrc_Entry_PowerSaving
//    description:
//        IIC pin entry powersaving.
/*********************************************************************/
void Com_Lock_Mfrc_Entry_PowerSaving(void)
{
   // MA_KIT_SDA_L;
   // MA_KIT_SDA_L;
   // CyDelayUs(5);
    
   // Cy_GPIO_SetDrivemode(LED_SCL_PORT,LED_SCL_NUM,CY_GPIO_DM_OD_DRIVESLOW_IN_OFF);
   // Cy_GPIO_SetDrivemode(LED_SDA_PORT,LED_SDA_NUM,CY_GPIO_DM_OD_DRIVESLOW_IN_OFF);
}

//*********************************************************************/
//    fuction name:
//        Com_Lock_Mfrc_Exit_PowerSaving
//    description:
//        IIC pin exit powersaving.
/*********************************************************************/
void Com_Lock_Mfrc_Exit_PowerSaving(void)
{    
   // Cy_GPIO_SetDrivemode(LED_SCL_PORT, LED_SCL_NUM, CY_GPIO_DM_STRONG_IN_OFF);
   // Cy_GPIO_SetDrivemode(LED_SDA_PORT, LED_SDA_NUM, CY_GPIO_DM_STRONG_IN_OFF);
}


//=====================================================================
//    no more.
//=====================================================================

