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


#ifndef __COMMON_LOCK_BS81X_H__
#define __COMMON_LOCK_BS81X_H__


#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include "io_ctrl.h"
#include "m_timer.h"
#include "plf.h"
#include "common_lock_mfrc.h"


//touch  key 
#define COMMON_LOCK_KEY_0_INDEX             0x00
#define COMMON_LOCK_KEY_1_INDEX             0x01
#define COMMON_LOCK_KEY_2_INDEX             0x02
#define COMMON_LOCK_KEY_3_INDEX             0x03
#define COMMON_LOCK_KEY_4_INDEX             0x04
#define COMMON_LOCK_KEY_5_INDEX             0x05
#define COMMON_LOCK_KEY_6_INDEX             0x06
#define COMMON_LOCK_KEY_7_INDEX             0x07
#define COMMON_LOCK_KEY_8_INDEX             0x08
#define COMMON_LOCK_KEY_9_INDEX             0x09
#define COMMON_LOCK_KEY_STAR_INDEX          0x0A
#define COMMON_LOCK_KEY_HASH_INDEX          0x0B
#define COMMON_LOCK_KEY_LOCK_INDEX          0x0C
// 器件地址
#define BS81X_SLA_ADDR 0xA0


/////////////////////////////////////////////////////////////////////
//MF522 FIFO长度定义
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
//MF522寄存器定义
/////////////////////////////////////////////////////////////////////
// PAGE 0

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void Com_Lock_Init_bs81x(void);
void Com_Lock_bs81x_Task(void);





#endif


//=====================================================================
//    no more.
//=====================================================================

