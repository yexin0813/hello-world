#ifndef DMAC_INTEGRATION_H_
#define DMAC_INTEGRATION_H_
#include <stdint.h>
#include "apollo_00.h"
#define DMAC_CHANNEL_NUM    6
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
    uint32_t size;
    void (*callback)(void *);
    void *callback_param;
}dmac_ch_env_t;

__STATIC_INLINE uint8_t dmac_qspi_rx_handshake_enum(uint8_t idx)
{
    return Dmac_Qspi_Rx;
}

__STATIC_INLINE uint8_t dmac_qspi_tx_handshake_enum(uint8_t idx)
{
    return Dmac_Qspi_Tx;
}


__STATIC_INLINE uint8_t dmac_uart_rx_handshake_enum(uint8_t idx)
{
    return idx ? Dmac_Uart1_Rx : Dmac_Uart0_Rx;
}

__STATIC_INLINE uint8_t dmac_uart_tx_handshake_enum(uint8_t idx)
{
    return idx ? Dmac_Uart1_Tx : Dmac_Uart0_Tx;
}

__STATIC_INLINE uint8_t dmac_spi_master_rx_handshake_enum(uint8_t idx)
{
    return idx ? Dmac_SpiM1_Rx : Dmac_SpiM0_Rx;
}

__STATIC_INLINE uint8_t dmac_spi_master_tx_handshake_enum(uint8_t idx)
{
    return idx ? Dmac_SpiM1_Tx : Dmac_SpiM0_Tx;
}

__STATIC_INLINE uint8_t dmac_spi_slave_rx_handshake_enum(uint8_t idx)
{
    return Dmac_SpiS_Rx;
}

__STATIC_INLINE uint8_t dmac_spi_slave_tx_handshake_enum(uint8_t idx)
{
    return Dmac_SpiS_Tx;
}

__STATIC_INLINE uint8_t dmac_iic_rx_handshake_enum(uint8_t idx)
{
    return idx ? Dmac_Iic1_Rx : Dmac_Iic0_Rx;
}

__STATIC_INLINE uint8_t dmac_iic_tx_handshake_enum(uint8_t idx)
{
    return idx ? Dmac_Iic1_Tx :Dmac_Iic0_Tx;
}

extern dmac_ch_env_t dmac_ch_env[DMAC_CHANNEL_NUM];

#endif
