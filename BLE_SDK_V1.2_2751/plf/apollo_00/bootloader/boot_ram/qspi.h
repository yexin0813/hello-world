#ifndef QSPI_H_
#define QSPI_H_
#include <stdint.h>
#include "dw_apb_ssi_typedef.h"
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
void qspi_init(uint16_t clk_div,uint8_t rxsample_dly);

void qspi_std_write(uint8_t *data,uint32_t length);

void qspi_std_read(uint8_t *cmd_buf,uint8_t cmd_len,uint8_t *data_buf,uint32_t data_len);

void qspi_multi_read(qspi_multi_read_param_t *param);

void qspi_flash_program(uint8_t cmd,uint32_t addr,uint8_t *data,uint32_t length);

void qspi_config_for_cache(uint16_t cache_line_size,uint8_t spi_mode);

uint32_t qspi_dr_reg_addr_for_cache(void);

void qspi_disable_for_cache(void);

#endif
