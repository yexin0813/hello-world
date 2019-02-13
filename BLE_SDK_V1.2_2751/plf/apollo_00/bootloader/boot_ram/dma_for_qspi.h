#ifndef DMA_FOR_QSPI_H_
#define DMA_FOR_QSPI_H_
#include <stdint.h>
#include "dmac_typedef.h"
#define MAX_DMAC_TRANSFER_SIZE 4095

enum dw_dmac_device_chn {
    Dmac_Qspi_Tx    = 0,    //!< Dmac_Qspi_Tx
    Dmac_Qspi_Rx, // 0x1
    Dmac_SpiM0_Tx, // 0x2
    Dmac_SpiM0_Rx, // 0x3
    Dmac_SpiM1_Tx, // 0x4
    Dmac_SpiM1_Rx, // 0x5
    Dmac_SpiS_Tx, // 0x6
    Dmac_SpiS_Rx, // 0x7
    Dmac_Uart0_Tx, // 0x8
    Dmac_Uart0_Rx, // 0x9
    Dmac_Uart1_Tx, // 0xa
    Dmac_Uart1_Rx, // 0xb
    Dmac_Iic0_Tx, // 0xc
    Dmac_Iic0_Rx, // 0xd
    Dmac_Iic1_Tx, // 0xe
    Dmac_Iic1_Rx, // 0xf
};
typedef struct
{
    uint8_t *src;
    uint8_t *dst;
    uint32_t length;
    uint32_t src_tr_width:3,
            dst_tr_width:3,
            src_msize:3,
            dst_msize:3,
            tt_fc:3,
            src_per:4,
            dst_per:4;
}app_dmac_transfer_param_t;

void dmac_init(void);

uint32_t dmac_start(app_dmac_transfer_param_t *param,uint8_t ch_idx);

void dmac_transfer_wait(uint32_t remaining_length,uint8_t ch_idx);


#endif

