#ifndef __REG_PWM_H__
#define __REG_PWM_H__
#include <stdint.h>

typedef struct
{
    struct
    {
        volatile uint32_t PWM_EN;
        volatile uint32_t PWM_SETTING;
        volatile uint32_t reserved[2];
    }ch[5];
}reg_pwm_t;

enum PWM_PWM_EN_FIELD
{
    PWM_PWM_EN_R_MASK = 0x1,
    PWM_PWM_EN_R_POS = 0,
};

enum PWM_PWM_SETTING_FIELD
{
    PWM_PWM_HIGH_MASK = (int)0xffff0000L,
    PWM_PWM_HIGH_POS = 16,
    PWM_PWM_LOW_MASK = 0xffff,
    PWM_PWM_LOW_POS = 0,
};


#endif
