
#ifndef LED_H_
#define LED_H_

#include <stdint.h>          // standard integer definitions


#define led_set(index)
#define led_reset(index)
#define led_toggle(index)

void led_init(void);


void led_set_all(uint32_t value);



#endif // LED_H_
