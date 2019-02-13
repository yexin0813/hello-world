#ifndef PERIPH_RECOVERY_H_
#define PERIPH_RECOVERY_H_
#include "periph_common.h"

typedef struct
{
    periph_inst_handle_t *hdl;
    uint32_t mask;
    uint8_t buf_size;
}periph_recovery_mngt_t;

void recovery_list_add(periph_recovery_mngt_t *mngt,periph_inst_handle_t hdl);

void recovery_list_remove(periph_recovery_mngt_t *mngt,periph_inst_handle_t hdl);

void periph_recovery(periph_recovery_mngt_t *mngt);


#endif
