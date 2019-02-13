#ifndef ARCH_INIT_H_
#define ARCH_INIT_H_
#include "rwip.h"

typedef enum{
    Std_Read_Mode,
    Dual_Output_Read_Mode,
    Quad_Output_Read_Mode,
    Dual_IO_Read_Mode,
    Quad_IO_Read_Mode,
    Read_Mode_Max,
}flash_op_mode_t;

void cache_config(void);

void cache_enable(void);

void cache_disable(void);

void soc_initialize(void);

void baremetal_blestack_init(void);

void soc_baremetal_loop(void);

const struct rwip_eif_api *serial_eif_get(void);
#endif
