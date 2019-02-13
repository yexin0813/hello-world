#ifndef SWINT_H_
#define SWINT_H_
#include "reg_sysc_cpu.h"
#include "apollo_00.h"
#define SWINT_REQ() sysc_cpu_sft_intr_set(1)
#define SWINT_CLR() sysc_cpu_sft_intr_set(0)
#define SWINT_SYS_INT_CLR() NVIC_ClearPendingIRQ(SFT_IRQn)
#define SWINT_SYS_INT_EN() NVIC_EnableIRQ(SFT_IRQn)
#endif
