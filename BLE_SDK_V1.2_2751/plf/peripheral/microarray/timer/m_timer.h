#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

extern volatile unsigned int tick_count;

void timerInit(void);
void timerRestart(void);
uint32_t  timerElapsedUs(void);
void timerDelayUs(uint32_t us);
void timerDelayMs(uint32_t ms);
uint32_t systemTick(void);

#endif /* TIMEOUT_H_ */
