//=====================================================================
//    file mane:
//        common_lock_oled.h
//    description:
//        0led function head file.
//    date:
//        2018-11-15
//    author:
//        ye.xin
//    version:
//        v0.1        init version.
//    others:
//
//=====================================================================


#ifndef __COMMON_LOCK_OLED_H__
#define __COMMON_LOCK_OLED_H__

#include "common_lock_mfrc.h"

    
#define	Slave_Address 0x78		// 器件地址
#define	OP_Command 0x00
#define	OP_Data 0x40

#define PAGE_TOTAL 8
#define PAGE_ONE 1
#define START_HIGH_BIT 0x10
#define START_LOW_BIT 0x02
#define MIN 0
#define STATE_MAX 0xFF
#define STATE_MIN 0x00
#define START_PAGE 0xB0
#define COLUMN_MAX 128

    
void Com_Lock_Init_OLED(void);
void Com_Lock_Oled_Task(void);


#define COMMON_LOCK_RESB_ON        Cy_GPIO_Write(RESB_PORT, RESB_NUM, 1)
#define COMMON_LOCK_RESB_OFF       Cy_GPIO_Write(RESB_PORT, RESB_NUM, 0)



#endif


//=====================================================================
//    no more.
//=====================================================================

