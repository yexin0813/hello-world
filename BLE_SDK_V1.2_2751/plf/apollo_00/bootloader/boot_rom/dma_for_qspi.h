/**
 ****************************************************************************************
 *
 * @file dma_for_qspi.h
 *
 * @brief Declaration of functions used for dmac
 *
 * Copyright (C) Bluexmicro electronics 2015-2016
 *
 *
 ****************************************************************************************
 */
#ifndef DMA_FOR_QSPI_H_
#define DMA_FOR_QSPI_H_
#include <stdint.h>
/**
 ****************************************************************************************
 * @defgroup DMA_FOR_QSPI
 * @ingroup DRIVERS
 * @brief Dmac functions specified for QSPI.
 *
 * @{
 ****************************************************************************************
 */
#define MAX_DMAC_TRANSFER_SIZE 4095

/**
 * @brief Initialization of qspi dma transfer.
 */
void dmac_transfer_qspi_init(void);

/**
 * @brief TX transfer of qspi.
 * @param src Source address of transfer.
 * @param dst Destination address of transfer.
 * @param length Length to transfer.
 */
void dmac_transfer_QSPI_TX(void *src, void *dst, uint32_t length);

/**
 * @brief RX transfer of qspi, with 8-bits transfer width.
 * @param src Source address of transfer.
 * @param dst Destination address of transfer.
 * @param length Length to transfer.
 */
void dmac_transfer_QSPI_RX(void *src, void *dst, uint32_t length);

/**
 * @brief RX transfer of qspi, with 32-bits transfer width.
 * @param src Source address of transfer.
 * @param dst Destination address of transfer.
 * @param length Length to transfer.
 */
void dmac_transfer_QSPI_RX_32Bit(void *src, void *dst, uint32_t length);

/// @}
#endif

