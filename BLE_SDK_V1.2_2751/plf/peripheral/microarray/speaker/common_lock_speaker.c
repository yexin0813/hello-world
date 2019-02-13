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
  

#include "common_lock_speaker.h"

#if(COMMON_LOCK_USE_SPEAKER)


/*********************************************************************/
//    function name:
//        Com_Lock_Init_Speaker
//    description:
//        init speaker pin.
//    others:
//         none.
/*********************************************************************/
void Com_Lock_Init_Speaker(void)
{
    Com_Lock_Speaker_Send_Byte(0xE8);   // max voice
    Com_Lock_Speaker_Send_Byte(0xFE);   // stop play

    Com_Lock_Play_Voice(11);
}


/*********************************************************************/
//    function name:
//        Com_Lock_Speaker_Send_Byte
//    description:
//        send byte via one wire bus.
//    others:
//         none.
/*********************************************************************/
void Com_Lock_Speaker_Send_Byte(const uint8_t data)
{
    int     pos = 0;
    
	
	  io_cfg_output(SPEAKER_PIN);
	  io_pin_clear(SPEAKER_PIN);
    //timerDelayMs(5);
    for(pos = 0; pos < 8; pos++)
    {
        if((1 << pos) & data)
        {
					  io_pin_set(SPEAKER_PIN);
            BX_DELAY_US(600);
            io_pin_clear(SPEAKER_PIN);
            BX_DELAY_US(200);
        }
        else
        {
					  io_pin_set(SPEAKER_PIN);
            BX_DELAY_US(200);
            io_pin_clear(SPEAKER_PIN);
            BX_DELAY_US(600);
        }
    }

		io_pin_set(SPEAKER_PIN);
}


/*********************************************************************/
//    function name:
//        Com_Lock_Play_Voice
//    description:
//        play voice
//    others:
//         none.
/*********************************************************************/
void Com_Lock_Play_Voice(const uint8_t addr)
{
    //UART_STEP_DEBUG;
    //Com_Lock_Speaker_Send_Byte(0xF3);
    //timerDelayMs(3);                        // 2ms delay.
    Com_Lock_Speaker_Send_Byte(addr);
}


/*********************************************************************/
//    function name:
//        Com_Lock_Play_Voice_Test()
//    description:
//        test voice
//    others:
//         none.
/*********************************************************************/
void Com_Lock_Play_Voice_Test(uint8_t indx)
{
   
    Com_Lock_Play_Voice(indx);
}


/*********************************************************************/
//    function name:
//        Com_Lock_Voice_Entry_Power_Saving()
//    description:
//        voice pin entry power saving.
//    others:
//         none.
/*********************************************************************/
void Com_Lock_Voice_Entry_Power_Saving(void)
{    
   // Cy_GPIO_Write(VOICE_PORT, VOICE_NUM, 0);
   // Cy_GPIO_SetDrivemode(VOICE_PORT, VOICE_NUM,CY_GPIO_DM_OD_DRIVESLOW_IN_OFF);
}


/*********************************************************************/
//    function name:
//        Com_Lock_Voice_Entry_Power_Saving()
//    description:
//        voice pin entry power saving.
//    others:
//         none.
/*********************************************************************/
void Com_Lock_Voice_Exit_Power_Saving(void)
{    
    //Cy_GPIO_SetDrivemode(VOICE_PORT, VOICE_NUM,CY_GPIO_DM_STRONG_IN_OFF);
}


#endif


//=====================================================================
//    no more.
//=====================================================================

