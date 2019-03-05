#ifndef FRONT_H
#define FRONT_H
#include "MaTypes.h"
#include "User.h"
#include "Hardware.h"

void FrontInit(void);                            //前板状态机初始化
void FrontTask(void);                            //前板状态机循环
void ChangeNumtoString(void);
uint8_t CheckKey(u8 * checkbuffer,u8 buflen,u8 * password,u8 passwordlen);


 
//===========================前板状态机状态参数结构================================//
typedef struct{
    KeyLed_t key_last;                           //最后一次按键状态
	  u8  password_buff[256];
	  u32 password_lo;                      //开锁密码低8位
    u32 password_hi;                      //开锁密码高4位
	  u32  password_len;
}ParamAwake_t; 
extern ParamAwake_t g_front_param;//唤醒状态的参数结构

#endif // FRONT_H
