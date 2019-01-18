//=====================================================================
//    file mane:
//        common_lock_oled.h
//    description:
//        6500 function head file.
//    date:
//        2018-12-21
//    author:
//      
//    version:
//        v0.1        init version.
//    others:
//
//=====================================================================


#ifndef _IIC_H
#define _IIC_H

    
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "../inc/filter.h"


#define MA_KIT_SCL_H                        PORTCbits.RC3 = 1//PORTC |= 0x08//Cy_GPIO_Write(LED_SCL_0_PORT, LED_SCL_0_NUM, 1)    
#define MA_KIT_SCL_L                        PORTCbits.RC3 = 0//PORTC &= 0xF7//Cy_GPIO_Write(LED_SCL_0_PORT, LED_SCL_0_NUM, 0)    
#define MA_KIT_SDA_H                        PORTCbits.RC4 = 1// PORTC |= 0x10//Cy_GPIO_Write(LED_SDA_0_PORT, LED_SDA_0_NUM, 1)    
#define MA_KIT_SDA_L                        PORTCbits.RC4 = 0//PORTC &= 0xEF//Cy_GPIO_Write(LED_SDA_0_PORT, LED_SDA_0_NUM, 0)    
//#define MA_KIT_SDA_DATA                     Cy_GPIO_Read(LED_SDA_0_PORT, LED_SDA_0_NUM)
    
#define	MPU6500_Slave_Address 0xD0	


// ??MPU9250????
//****************************************
#define	SMPLRT_DIV		0x19	//???????????0x07(125Hz)
#define	CONFIG			0x1A	//???????????0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//???????????????0x18(????2000deg/s)
#define	ACCEL_CONFIG	0x1C	//??????????????????????0x01(????2G?5Hz)

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

		
#define MAG_XOUT_L		0x03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08


#define	PWR_MGMT_1		0x6B	//?????????0x00(????)
#define PWR_MGMT_2      0x6C
#define	WHO_AM_I		0x75	//IIC?????(????0x70???)


void Init_MPU6500(void);
void Test_MPU_6500(void);
float Mpu6500GyroAngle(int8_t dir);
int8_t Test_Sleep_Mpu6500(void);

#endif


//=====================================================================
//    no more.
//=====================================================================

