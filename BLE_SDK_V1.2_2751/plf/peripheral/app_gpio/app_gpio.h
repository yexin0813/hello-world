#ifndef APP_GPIO_H_
#define APP_GPIO_H_
#include <stdint.h>
#include <stdbool.h>
#include "periph_common.h"
#include "_reg_base_addr.h"
#include "reg_gpio.h"
#include "co_list.h"

enum gpio_sys_stat_config
{
    GPIO_INIT,
    GPIO_UNINIT,
};

typedef struct
{
    struct co_list_hdr hdr;
    void (*callback)();
    uint8_t idx;
    uint8_t edge: 1,
            polarity: 1,
            both_edge: 1;
}app_gpio_int_cfg_t;

typedef struct
{
    struct co_list int_list;
}app_gpio_env_t;

typedef struct
{
    periph_inst_t inst;
    app_gpio_env_t env;
    reg_gpio_t *reg;
    uint8_t idx;
}app_gpio_inst_t;


#define GPIO_INSTANCE(id) \
    {\
        .inst = {.init_func = app_gpio_init,},\
        .reg = (reg_gpio_t *)REG_GPIO##id##_BASE,\
    }

void app_gpio_init(periph_inst_handle_t hdl);

void app_gpio_uninit(periph_inst_handle_t hdl);

void app_gpio_port_dir_output_set(periph_inst_handle_t hdl,uint32_t out_mask);

void app_gpio_port_dir_input_set(periph_inst_handle_t hdl,uint32_t in_mask);

void app_gpio_port_dir_write(periph_inst_handle_t hdl,uint32_t dir_mask);

uint32_t app_gpio_port_dir_read(periph_inst_handle_t hdl);

uint32_t app_gpio_port_in_read(periph_inst_handle_t hdl);

uint32_t app_gpio_port_out_read(periph_inst_handle_t hdl);

void app_gpio_port_out_write(periph_inst_handle_t hdl,uint32_t val);

void app_gpio_port_out_set(periph_inst_handle_t hdl,uint32_t set_mask);

void app_gpio_port_out_clr(periph_inst_handle_t hdl,uint32_t clr_mask);

void app_gpio_port_out_toggle(periph_inst_handle_t hdl,uint32_t toggle_mask);

void app_gpio_int_enable(periph_inst_handle_t hdl,app_gpio_int_cfg_t *int_cfg);

void app_gpio_int_disable(periph_inst_handle_t hdl,uint8_t idx);

void app_gpio_isr(app_gpio_inst_t *inst);


#endif

