#ifndef SYSCTRL_H_
#define SYSCTRL_H_
#include <stdint.h>
#include <stdbool.h>
typedef struct
{
    uint32_t active_ie;
    uint32_t deep_sleep_ie;
    uint32_t util_io_mask;
    uint32_t util_io_ret_dir;
    uint32_t util_io_ret_val;
}io_mngt_t;
extern io_mngt_t io_mngt;

void sysctrl_pwr_3v2_drv_capability_maintain(bool enable);
bool sysctrl_pwr_pwm_2_sleep_en_get(void);
void sysctrl_io_config_mask(uint32_t mask,uint32_t config);
void sysctrl_io_config(uint8_t num,uint32_t config);
void sysctrl_awo_init(void);

void sysctrl_32k_clk_init(void);
void sysctrl_set_ahb_apb_blemac_clk(void);

#endif
