#ifndef RC_CALIB_H_
#define RC_CALIB_H_
#include <stdint.h>
#include <stdbool.h>
#include "apollo_00.h"

extern uint32_t calib32k;
extern uint32_t rc_freq;
void rc_calib_start(bool en);
void rc_calib_end(void);



#endif
