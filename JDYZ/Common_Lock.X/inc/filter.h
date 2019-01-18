
#ifndef _Filter
#define _Filter


#include <stdint.h>

#define dt 0.01     // dt的取值为kalman滤波器采样时间,此处为10ms;

//***********变量的宏定义**************
#ifdef DEBUD

#define Q_angle 0.001
#define Q_gyro 0.003
#define R_angle 0.5
#define C_0 1
#endif

//***********输出接口******************
//extern float Angle, Angle_dot; // 滤波后的倾角和倾角加速度
extern void AngleCalcu(float angle_m,float gyro_m,float *p); // 
void ClearAngle(void);
uint8_t CheckDoorOpened(void);
uint8_t CheckDoorClosed(void);

#endif