//=====================================================================
//    file mane:
//        common_lock_speaker.c
//    description:
//        speaker  function head file.
//    date:
//        2018-5-9
//    author:
//        ian.lee
//    version:
//        v0.1        init version.
//    others:
//
//=====================================================================


#ifndef __COMMON_LOCK_SPEAKER_H__
#define __COMMON_LOCK_SPEAKER_H__


#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include "io_ctrl.h"
#include "m_timer.h"
#include "plf.h"

#define COMMON_LOCK_USE_SPEAKER 1
#define SPEAKER_PIN  3

#if(COMMON_LOCK_USE_SPEAKER)


#define COMMON_LOCK_VOICE_0_ADDRESS        (0x00)
#define COMMON_LOCK_VOICE_1_ADDRESS        (0x01)
#define COMMON_LOCK_VOICE_2_ADDRESS        (0x02)
#define COMMON_LOCK_VOICE_3_ADDRESS        (0x03)

    
void Com_Lock_Init_Speaker(void);
void Com_Lock_Speaker_Send_Byte(const uint8_t data);
void Com_Lock_Play_Voice(const uint8_t addr);
void Com_Lock_Play_Voice_Test(uint8_t indx);
void Com_Lock_Voice_Entry_Power_Saving(void);
void Com_Lock_Voice_Exit_Power_Saving(void);

#endif


#endif


//=====================================================================
//    no more.
//=====================================================================

