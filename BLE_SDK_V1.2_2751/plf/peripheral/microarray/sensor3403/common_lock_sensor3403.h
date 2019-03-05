//=====================================================================
//    file mane:
//        common_lock_m4_uart.h
//    description:
//        m4 uart function head file.
//    date:
//        2018-5-9
//    author:
//        ian.lee
//    version:
//        v0.1        init version.
//    others:
//
//=====================================================================


#ifndef __COMMON_LOCK_SENSOR_H__
#define __COMMON_LOCK_SENSOR_H__

#include "common_lock_mfrc.h"   

// frame 
#define COMMON_LOCK_CMD_FRAME_HEAD               0xEF
//#define COMMON_LOCK_CMD_FRAME_HEAD             0xEF
#define COMMON_LOCK_CMD_FRAME_LENGTH             9

#define COMMON_LOCK_CMD_TEST                  0x27
#define COMMON_LOCK_GET_NUM                   0x1D
#define COMMON_LOCK_MATCH                     0x04
#define COMMON_LOCK_INIT                      0x26
#define COMMON_LOCK_CMD_ALL_DELETE            0x0D
#define COMMON_LOCK_CMD_ID_ENROLL             0x01
#define COMMON_LOCK_CMD_READ_ID_LIST          0x1F

#define COMMON_LOCK_CMD_ID_ENROLL_STEP2       0xFF

// command ID.

//#define COMMON_LOCK_CMD_ID_MATCH_S          0x04 
#define COMMON_LOCK_CMD_ID_DELETE             0x0C



#define COMMON_LOCK_CMD_ID_CONFIGENROLL       0x24
#define COMMON_LOCK_CMD_ID_MATCH              0x25
#define COMMON_LOCK_CMD_ID_ISFINGERLEAVE      0x50
    
//frame    
#define FRAME_START     0xEF01
#define FRAME_PID       0x06
#define FRAME_STA_LEN   9


#define FRAME_LEN_HIGH  0x07
#define FRAME_LEN_LOW   0x08
#define FRAME_CRC 1

    
void Com_Lock_Init_Sensor(void);

void Com_Lock_Sensor_Task(void);


#endif


//=====================================================================
//    no more.
//=====================================================================

