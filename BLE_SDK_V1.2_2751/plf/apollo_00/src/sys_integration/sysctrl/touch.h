/*
 * touch.h
 *
 *  Created on: 2018Äê9ÔÂ26ÈÕ
 *      Author: mingzhou
 */

#ifndef _TOUCH_H_
#define _TOUCH_H_
#include <stdint.h>

typedef struct
{
    uint8_t threthold;
    uint8_t int_en;
    void (*cb)(void);
}Touch_param_t;

void touch_init(void);

void touch_start(Touch_param_t* param);

void touch_stop(uint8_t disable_ext_int);

#endif /* _TOUCH_H_ */
