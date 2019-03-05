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


#ifndef __COMMON_LOCK_LED_H__
#define __COMMON_LOCK_LED_H__

#include "common_lock_mfrc.h"    


typedef struct{
  uint8_t  busCmd;
  uint8_t  devCmd;
  uint8_t  dat0;
  uint8_t  dat1;
}iicData;

 

// key led
#define COMMON_LOCK_LED_0_INDEX             0x04
#define COMMON_LOCK_LED_1_INDEX             0x0A
#define COMMON_LOCK_LED_2_INDEX             0x00
#define COMMON_LOCK_LED_3_INDEX             0x0B
#define COMMON_LOCK_LED_4_INDEX             0x07
#define COMMON_LOCK_LED_5_INDEX             0x01
#define COMMON_LOCK_LED_6_INDEX             0x09
#define COMMON_LOCK_LED_7_INDEX             0x06
#define COMMON_LOCK_LED_8_INDEX             0x02
#define COMMON_LOCK_LED_9_INDEX             0x08
#define COMMON_LOCK_LED_STAR_INDEX          0x03
#define COMMON_LOCK_LED_HASH_INDEX          0x05
#define COMMON_LOCK_LED_LOCK_INDEX          0x0C

// arm led
#define COMMON_LOCK_LED_PASS_INDEX          0x0E
#define COMMON_LOCK_LED_FAIL_INDEX          0x0D
#define COMMON_LOCK_LED_ACTIVE_INDEX        0x0C

#define COMMON_LOCK_LED_ALL_KEY_ON_INDEX    0x15
#define COMMON_LOCK_LED_ALL_KEY_OFF_INDEX   0x10


void Com_Lock_Init_LED(void);
void Com_Lock_Switch_On_LED(uint32_t indx);
void Com_Lock_Switch_Off_LED(uint32_t indx);
void Com_Lock_Switch_On_LED_Table(uint32_t indx);
void Com_Lock_Switch_Off_LED_Table(uint32_t indx);
void Com_Lock_Update_LED(void);

// IIC port.
void Com_Lock_IIC_Start_Condition(void);
void Com_Lock_IIC_Stop_Condition(void);
void Com_Lock_IIC_Write_Byte(uint8_t);
void Com_Lock_LED_Entry_PowerSaving(void);
void Com_Lock_LED_Exit_PowerSaving(void);




#endif


//=====================================================================
//    no more.
//=====================================================================

