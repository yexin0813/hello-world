#include "rst_gen.h"
#include "clk_gate.h"
#include "apollo_00.h"
void rtc_init()
{
    srst_awo(RTC_SRST_AWO);
    clk_gate_awo(AWO_CLKG_SET_RTC);
    NVIC_ClearPendingIRQ(RTC_IRQn);
    NVIC_EnableIRQ(RTC_IRQn);
}
