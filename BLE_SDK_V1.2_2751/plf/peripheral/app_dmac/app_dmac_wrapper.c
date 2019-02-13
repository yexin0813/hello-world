#include "app_dmac.h"

app_dmac_inst_t dmac_inst = DMAC_INSTANCE();

void app_dmac_init_wrapper(void)
{
    app_dmac_init(&dmac_inst.inst);
}

void app_dmac_uninit_wrapper(void)
{
    app_dmac_uninit(&dmac_inst.inst);
}

uint8_t app_dmac_start_wrapper(app_dmac_transfer_param_t * param)
{
    return app_dmac_start(&dmac_inst.inst,param);
}

uint32_t app_dmac_transfer_cancel_wrapper(uint8_t ch_idx)
{
    return app_dmac_transfer_cancel(&dmac_inst.inst,ch_idx);
}

void app_dmac_transfer_wait_wrapper(uint8_t ch_idx)
{
    app_dmac_transfer_wait(&dmac_inst.inst,ch_idx);
}


