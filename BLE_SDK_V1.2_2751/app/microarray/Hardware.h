#ifndef HARDWARE_H
#define HARDWARE_H
#include "MaTypes.h"
#include "common_lock_mfrc.h"  
#include "m_uart.h"


typedef enum{
    KEY_LED_0, KEY_LED_1, KEY_LED_2, KEY_LED_3,  //数字按键
    KEY_LED_4, KEY_LED_5, KEY_LED_6, KEY_LED_7,
    KEY_LED_8, KEY_LED_9,
    KEY_LED_STAR,                                //*按键
    KEY_LED_HASH,                                //#按键
	  KEY_LED_TT,
	  KEY_LED_NONE = 0xff,                         //无按键
}KeyLed_t;                                       //按键/LED类型

#define SCREEN_BYTES         (128 * 64 / 8)      //屏占用的显示缓冲大小
typedef enum{
    VOICE_MUTE,                                  //无语音
    VOICE_BEEP,                                  //按键声
    VOICE_WELCOME,                               //"欢迎使用金点原子高安全自动智能锁"
}Voice_t;

//===========================硬件输入接口=======================================//
u32 SliderGetTouch(void);                        //获取滑盖Touch状态
u32 SliderGetHallOpened(void);                   //获取滑盖开霍尔状态
u32 SliderGetHallClosed(void);                   //获取滑盖关霍尔状态
KeyLed_t FrontGetKey(void);                      //获取前面板当前按键
u32 FrontGetConfigKey(void);                     //获取设置键状态
u32 FrontGetKeyTouch(void);                      //获取key触控状态
u32 FrontGetFingerTouch(void);                   //获取指纹触摸状态
s32 FrontGetMatchFinger(void);                   //获取匹配的指纹
s32 FrontGetRfid(void);                          //获取刷卡卡号

//===========================硬件输入仿真接口====================================//
typedef struct{
    //滑盖部分的输入
    //u32 slider_touch;                            //滑盖Touch:0=未触摸,1=触摸
    //u32 slider_hall_opened;                      //滑盖开到位霍尔:0=未到位,1=到位
    //u32 slider_hall_closed;                      //滑盖关到位霍尔:0=未到位,1=到位
    //前面板部分的输入
    KeyLed_t  key;                                			//当前按下的键
    u32 config_key;                              //设置键:0=未按下,1=按下
    u32 finger_touch;                            //指纹触摸:0=未触摸,1=触摸
	  u32 key_touch;                                 //按键触摸:0=未触摸1=触摸
    s32 finger;                                  //按下的指纹号:-1=无指纹
    s32 rfid;                                    //刷卡的卡号:-1=无刷卡
}HardwareInput_t;
extern volatile HardwareInput_t g_hardware_input;         //全局硬件输入仿真结构

//===========================硬件输出接口=======================================//
void SliderSetMoto(u32 moto);                    //设置滑盖电机:0=不动,1=正转,2=反转
void VoicePlay(Voice_t voice);                   //播放语音
void FrontSetLedOn(u32 leds);                    //点亮LED
void FrontSetLedOff(u32 leds);                   //熄灭LED
void FingerSetLed(u32 on_off);                   //设置指纹LED亮灭
void ScreenClear(void);                          //清屏
void ScreenShowLogo(void);                       //屏显示Logo
void ScreenPrintLine(u8 line, u8 *str);          //打印屏上0~3行之一,每次打印整行

//===========================硬件输出仿真接口====================================//
typedef struct{
    //滑盖部分的输出
    //u32 slider_moto;                             //滑盖Moto:0=不动,1=正转,2=反转
    //前面板部分的输出
    u8  screen[SCREEN_BYTES];                    //屏显示
    Voice_t voice;                               //播放语音
    u32 led;                                     //键盘上的LED：D0~D9=数字键0~9位置的LED,D10=*键位置的LED,D11=#键位置的LED
    u32 finger_led;                              //指纹LED:0=灭,1=绿色,2-红色
}HardwareOutput_t;
extern HardwareOutput_t g_hardware_output;       //全局硬件输出仿真结构


typedef struct 
{
	uint32_t start_time;
  uint32_t end_time;
	uint32_t time_over;
	uint8_t  time_over_flag;
}Timeout;

extern Timeout gtTimeout_M;




void HardwareInputInit(void);
void GetStartTime_M(uint32_t timeover);
uint8_t GetCurrentTimeCnt_M(void);

#if 0
//===========================硬件内部接口=======================================//
u32 FrontGetStamp(void);                         //获取时间戳
u32 FrontCalcElapsed(u32 stamp);                 //获取当前与时间戳的间隔时间
u32 FrontGetDatetime(void);                      //获取日期时间
void FrontSetDatetime(u32 dt);                   //设置日期时间
void FrontDatetimeToLine(u32 dt, u8 *str);       //将日期时间转化为字符串

//===========================硬件内部仿真接口====================================//
typedef struct{
    u32 tick;                                    //定时器计数(单位ms)
    QDateTime dt_start;                          //程序开始日期时间
    QDateTime dt_base;                           //仿真RTC启动时间
}HardwareInner_t;
extern HardwareInner_t g_hardware_inner;         //全局硬件内部仿真结构
#endif
#endif // HARDWARE_H
