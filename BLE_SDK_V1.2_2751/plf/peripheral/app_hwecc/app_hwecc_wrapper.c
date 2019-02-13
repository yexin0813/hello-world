#include "app_hwecc.h"

app_hwecc_inst_t hwecc_inst = HWECC_INSTANCE();

void app_hwecc_init_wrapper()
{
    app_hwecc_init(&hwecc_inst.inst);
}

void app_hwecc_uninit_wrapper()
{
    app_hwecc_uninit(&hwecc_inst.inst);   
}

void app_hwecc_calculate_wrapper(ecc_queue_t *param)
{
    app_hwecc_calculate(&hwecc_inst.inst,param);   
}
