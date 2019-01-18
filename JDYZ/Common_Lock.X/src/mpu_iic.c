//=====================================================================
//    file mane:
//        mpu_iic.c
//    description:
//        MPU function c file.
//    date:
//        2018-12-21
//    author:
//      
//    version:
//        v0.1        init version.
//    others:
//
//=====================================================================


#include <xc.h>
#include <math.h>
#include <stdlib.h>
#include "../inc/mpu_iic.h"
#include "../inc/mcc.h"
#include "../inc/eusart.h"




static int Old_gyro,New_gyro,Frist_gyro;
static uint8_t Sam_cnt = 0;
int16_t G_Offset_X = 0;




/*********************************************************************/
//    fuction name:
//        IIC_Start
//    description:
//        IIC_Start IIC bus
/*********************************************************************/
static void Com_Lock_IIC_Start(void)
{
    MA_KIT_SDA_H;
    //PORTCbits.RC3 = 1;
    __delay_us(2);
    MA_KIT_SCL_H;
    //PORTCbits.RC4 = 1;
    __delay_us(3);
    MA_KIT_SDA_L;
    __delay_us(3);
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
    __delay_us(3);
    MA_KIT_SCL_H;
    __delay_us(3);
    MA_KIT_SDA_H;
    __delay_us(3);
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
    for(i = 0; i < 8; )
    {
        MA_KIT_SCL_L;
        if(dat & 0x80)
        {
            MA_KIT_SDA_H;
        }
        else
        {
            MA_KIT_SDA_L;
        }
        dat <<= 1;
        i++;
       // __delay_us(2);
        MA_KIT_SCL_H;
        //__delay_us(2);    // delay2.0us
        
    }
    MA_KIT_SCL_L;
    // Get ack from devise
    TRISC |= 0x10;//RC4 input
    __delay_us(4);
   // MA_KIT_SDA_H;//release SDA
   //  __delay_us(2);
    MA_KIT_SCL_H;
    __delay_us(2);
    retVal = PORTCbits.RC4;//Read_SDA();
    MA_KIT_SCL_L;
    TRISC &= 0xEF;//RC4 output
    return retVal;

}


/*********************************************************************/
//    function name:
//        Com_Lock_Write_Command(void)
//    description
//        send one command to IIC device.
/*********************************************************************/
static void Com_Lock_MPU_Write_Command(uint8_t reg,uint8_t data)
{
   Com_Lock_IIC_Start();
   Com_Lock_IIC_Write_Data(MPU6500_Slave_Address);                //Salve Adress
   Com_Lock_IIC_Write_Data(reg);                                     
   Com_Lock_IIC_Write_Data(data); 
   Com_Lock_IIC_Stop();
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
    TRISC |= 0x10;//RC4 input;//release SDA
    for(i = 0; i < 8; i++)
    {
        //__delay_us(2);
        MA_KIT_SCL_H;
        //__delay_us(1);
        retVal <<= 1;
        retVal |= PORTCbits.RC4;//Read_SDA();
        MA_KIT_SCL_L;
    }
    TRISC &= 0xEF;//RC4 output
    //ack
    if(ack)
    {
        MA_KIT_SDA_L;
    }
    //noack
    else
    {
        MA_KIT_SDA_H;
    }
    __delay_us(2);
    MA_KIT_SCL_H;
    __delay_us(2);
    MA_KIT_SCL_L;
    return retVal;
    
}

//????
//addr:?????
//???:????
static uint8_t Com_Lock_MPU_RD_Reg(uint8_t addr) 		
{
	uint8_t temp=0;		 
	Com_Lock_IIC_Start();  	
    temp = Com_Lock_IIC_Write_Data(MPU6500_Slave_Address);
    temp = Com_Lock_IIC_Write_Data(addr);   		//???????	 										  		   
	Com_Lock_IIC_Start();                           //????
	temp = Com_Lock_IIC_Write_Data(MPU6500_Slave_Address+1);        //???????	    
    temp=Com_Lock_IIC_Read_Data(0);                 //??????,?????,??NAK 	    	   
    Com_Lock_IIC_Stop();                            //???????? 	    
	return temp;                                    //??????
}





int Get6500Data(uint8_t REG_Address)
{
	char H,L;
	H=Com_Lock_MPU_RD_Reg(REG_Address);
	L=Com_Lock_MPU_RD_Reg(REG_Address+1);
	return (H<<8)+L;   //合成数据
}


void Read_Offset(void)
{
    int32_t G_Temp_X;
    uint8_t cnt;
    G_Temp_X = 0;
    for(cnt = 0; cnt < 50;cnt++)
    {
        G_Temp_X += (int32_t)Get6500Data(GYRO_YOUT_H);
    }
    G_Offset_X = G_Temp_X/cnt;
    Old_gyro = (int)G_Offset_X;
    New_gyro = Old_gyro;
    Frist_gyro = Old_gyro;
    EUSART_Write(0x55);
    EUSART_Write(0xAA);
    return;
}


/*
**********************************************
**函数名  ：float Mpu6500GyroAngle(int8 dir)
**函数功能：输出陀螺仪测量的倾角加速度
**            范围为2000deg/s时，换算关系：16.4 LSB/(deg/s)
**返回参数：测量的倾角加速度值
**传入参数：dir - 需要测量的方向
**           GYRO_XOUT - X轴方向
**           GYRO_YOUT - Y轴方向
**           GYRO_ZOUT - Z轴方向
**********************************************
*/


float Mpu6500GyroAngle(int8_t dir)
{
    float gyro_angle;
    uint8_t gyro_config;
     
    New_gyro = Get6500Data(dir);   // 
    //gyro_config =  Com_Lock_MPU_RD_Reg(GYRO_CONFIG);
    EUSART_Write(0x55);
    EUSART_Write(New_gyro >>8);
    EUSART_Write(New_gyro);
    EUSART_Write(0xAA);
    gyro_angle = (float)New_gyro;
    
    if(abs(New_gyro - Old_gyro) <= 10)
    {
        if(Sam_cnt == 0)
        {
            Frist_gyro = New_gyro;
        }
        Sam_cnt ++;
        if(abs(New_gyro - Frist_gyro) > 10)
        {
            Sam_cnt = 0;
        }
    }
    else 
    {
        Sam_cnt = 0;
    }
    Old_gyro = New_gyro;
    if(Sam_cnt >= 100)
    {
        EUSART_Write(0x5A);
        EUSART_Write(New_gyro >>8);
        EUSART_Write(New_gyro);
        EUSART_Write(0xA5);
        G_Offset_X = New_gyro;
        Sam_cnt = 0;
        
    }
    
    gyro_angle = (gyro_angle - G_Offset_X)/131;    //去除零点偏移，计算角速度值,负号为方向处理 
    
    return gyro_angle; // 返回测量值
}



void Init_MPU6500(void)
{
    __delay_ms(200);

    if(Com_Lock_MPU_RD_Reg(WHO_AM_I) == 0x70)			//?????6500???
	{		
		Com_Lock_MPU_Write_Command(PWR_MGMT_1,0x00);   		//????,??MPU6500
        Com_Lock_MPU_Write_Command(SMPLRT_DIV, 0x07);
        Com_Lock_MPU_Write_Command(CONFIG, 0x06);
        Com_Lock_MPU_Write_Command(GYRO_CONFIG, 0x00);  //250度/秒
        Com_Lock_MPU_Write_Command(PWR_MGMT_2,0x3d);   		//????,??MPU6500
        //Com_Lock_MPU_Write_Command(ACCEL_CONFIG, 0x01);
        __delay_ms(200);
        Read_Offset();
	}
}


int8_t Test_Sleep_Mpu6500(void)
{
    int test;
    Com_Lock_MPU_Write_Command(PWR_MGMT_1,0x40);   		//????,??MPU6500
    __delay_ms(100);
    Com_Lock_MPU_Write_Command(PWR_MGMT_1,0x00);   		//????,??MPU6500
    __delay_ms(200);
    test = Get6500Data(GYRO_YOUT_H);
    EUSART_Write(0x55);//G_Offset_X
    EUSART_Write(G_Offset_X>>8);
    EUSART_Write(G_Offset_X);
    EUSART_Write(test>>8);
    EUSART_Write(test);
    EUSART_Write(0xAA);
    //if(Com_Lock_MPU_RD_Reg(SMPLRT_DIV) != 0x07)
    //{
    //    return 1;
    //}
   // if(Com_Lock_MPU_RD_Reg(ACCEL_CONFIG) != 0x01)
    //{
    //    return 1;
    //}
    //return 0;
}

#ifdef DEBUD
void Test_MPU_6500(void)
{
    float result_accel,result_gyro; // 转换结果缓存
    float filter_result[2]; // 缓存滤波后的倾角值和倾角加速度值
    int32_t sum1,sum2;
    result_gyro = Mpu6500GyroAngle(GYRO_YOUT_H); // 测量陀螺仪的X轴值
    AngleCalcu(result_accel,result_gyro,filter_result);// 卡尔曼滤波及融合
    

    sum1 =  filter_result[0];
    sum2 =  filter_result[1];
    EUSART_Write(0x55);
    EUSART_Write(sum1>>8);
    EUSART_Write(sum1);
    EUSART_Write(sum2>>8);
    EUSART_Write(sum2);
    EUSART_Write(0xAA);

}
#endif
//=====================================================================
//    no more.
//=====================================================================

