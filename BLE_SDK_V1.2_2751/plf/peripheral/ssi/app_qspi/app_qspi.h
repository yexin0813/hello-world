#ifndef APP_QSPI_H_
#define APP_QSPI_H_

#include <stdint.h>
#include "periph_common.h"
#include "reg_ssi.h"
#include "_reg_base_addr.h"
#include "qspi_integration.h"

enum qspi_sys_stat_config
{
    QSPI_INIT,
    QSPI_UNINIT,
    QSPI_OP_START,
    QSPI_OP_DONE,
};

enum qspi_pin_role_config
{
    QSPI_CLK_PIN,
    QSPI_D0_PIN,
    QSPI_D1_PIN,
    QSPI_D2_PIN,
    QSPI_D3_PIN,
    QSPI_CS0_PIN,
    QSPI_CS1_PIN,
    QSPI_CS2_PIN,
    QSPI_CS3_PIN,
};



typedef struct
{
    uint32_t extend_byte:8,
                    addr:24;
}qspi_addr_data_t;

typedef struct
{
    uint8_t *data;
    uint32_t length;
    qspi_addr_data_t addr_field;
    uint16_t cmd;
    uint16_t inst_l:2,
                    trans_type:2,
                    wait_cycles:4,
                    dual_quad:2;
}qspi_multi_read_param_t;

typedef struct
{
    uint16_t clk_div;
    uint8_t rxsample_dly;
    uint8_t clk_pin_no;
    uint8_t data_pin_no[4];
    uint8_t cs_pin_no[QSPI_CS_NUM];
    uint8_t mode;
}app_qspi_init_param_t;

typedef struct
{
    periph_inst_t inst;
    app_qspi_init_param_t param;
    reg_ssi_t *reg;
    uint8_t idx;
}app_qspi_inst_t;

#define QSPI_INSTANCE(id) \
{\
    .inst = {.init_func = app_qspi_init,},\
    .reg = (reg_ssi_t *)REG_QSPI##id##_BASE,\
}

void app_qspi_init(periph_inst_handle_t hdl);

void app_qspi_uninit(periph_inst_handle_t hdl);

void app_qspi_std_write(periph_inst_handle_t hdl,uint8_t cs_sel_mask,uint8_t *data,uint32_t length);

void app_qspi_flash_program(periph_inst_handle_t hdl,uint8_t cs_sel_mask,uint8_t cmd,uint32_t addr,uint8_t *data,uint32_t length);

void app_qspi_std_read(periph_inst_handle_t hdl,uint8_t cs_sel_mask,uint8_t *cmd_buf,uint8_t cmd_len,uint8_t *data_buf,uint16_t data_len);

void app_qspi_multi_read(periph_inst_handle_t hdl,uint8_t cs_sel_mask,qspi_multi_read_param_t *param);

void app_qspi_config_for_cache(periph_inst_handle_t hdl,uint8_t cs_sel_mask,qspi_multi_read_param_t *param);

uint32_t app_qspi_dr_reg_addr_for_cache(periph_inst_handle_t hdl);

void app_qspi_disable_for_cache(periph_inst_handle_t hdl);

#endif
