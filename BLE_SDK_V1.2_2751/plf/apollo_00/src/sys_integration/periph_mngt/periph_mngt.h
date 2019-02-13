#ifndef PERIPH_MNGT_H_
#define PERIPH_MNGT_H_
#include <stdint.h>
#include <stdbool.h>
#include "periph_common.h"
#include "periph_recovery.h"
typedef struct
{
    uint32_t uart0_tx:1,
             uart0_rx:1,
             uart1_tx:1,
             uart1_rx:1,
             spim0:1,
             spim1:1,
             spis:1,
             iic0:1,
             iic1:1,
             pwm:1,
             charger:1;
}periph_domain_stat_t;


typedef struct
{
    uint32_t dmac:1,
             qspi:1,
             cache:1,
             timer0:1,
             timer1:1,
             wdt:1,
             hwecc:1,
             adc_ctrl:1,
             pgspy:1,
             rom_patch:1;
}cpu_domain_stat_t;


extern periph_domain_stat_t periph_domain_stat;
extern cpu_domain_stat_t cpu_domain_stat;
extern periph_recovery_mngt_t periph_domain_recovery_mngt;
extern periph_recovery_mngt_t cpu_domain_recovery_mngt;

bool periph_domain_busy(void);
bool cpu_domain_busy(void);

#endif
