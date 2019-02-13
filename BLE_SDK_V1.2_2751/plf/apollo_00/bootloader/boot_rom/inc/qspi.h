/**
 ****************************************************************************************
 *
 * @file qspi.h
 *
 * @brief Declaration of functions used for QSPI
 *
 * Copyright (C) Bluexmicro electronics 2015-2016
 *
 *
 ****************************************************************************************
 */
#ifndef QSPI_H_
#define QSPI_H_
#include <stdint.h>
#include "plf.h"
#include "dw_apb_ssi_typedef.h"
/**
 ****************************************************************************************
 * @defgroup QSPI
 * @ingroup SSI
 * @brief Functions specified for QSPI.
 *
 * @{
 ****************************************************************************************
 */
#define QSPI_READ_MAX_LENGTH	0x10000
#define QSPI_CLK_IN		AHB_CLK
#define QSPI_SCLK_OUT 	(QSPI_CLK_IN/4)
#define QSPI_TX_FIFO_DEPTH	64
#define QSPI_RX_FIFO_DEPTH	64
#define QSPI_SLAVE_INDEX	1
#define QSPI_DMA_Receive_Data_Level 32  //match with DMAC.CTL.SRC_MSIZE
#define QSPI_DMA_Transmit_Data_Level 32  //match with DMAC.CTL.DST_MSIZE

#define QSPI_D0_MASK (1<<26)
#define QSPI_D1_MASK (1<<27)
#define QSPI_D2_MASK (1<<28)
#define QSPI_D3_MASK (1<<29)
#define QSPI_STD_DATA_INPUT_MASK (QSPI_D1_MASK)
#define QSPI_DUAL_DATA_INPUT_MASK (QSPI_D0_MASK | QSPI_D1_MASK)
#define QSPI_QUAD_DATA_INPUT_MASK (QSPI_D0_MASK | QSPI_D1_MASK | QSPI_D2_MASK |QSPI_D3_MASK)

void qspi_set_clk_division(uint8_t dv);

void qspi_set_rx_sample_dly(uint8_t dly);

/**
 * @brief Initialization of QSPI.
 */
void qspi_init(void);

/**
 * @brief Write data by QSPI.
 *
 * @param bufptr Data buffer.
 * @param length Data length.
 */
void qspi_write(uint8_t *bufptr,uint32_t length);

/**
 * @brief Read data after writing.
 *
 * @param[in] rd_ptr Data buffer of received data.
 * @param[in] rd_len Received data length.
 * @param[in] wr_ptr Data buffer of sent data.
 * @param[in] wr_len Sent data length.
 */
void qspi_read(uint8_t *rd_ptr,uint32_t rd_len,uint8_t *wr_ptr,uint8_t wr_len);
/* suitable for flash Read Data Bytes CMD 0x03 */
/**
 * @brief Read data from flash in standard SPI format.
 *
 * @param[in] rd_ptr Data buffer of received data.
 * @param[in] rd_len_in_word Received data length in word.
 * @param[in] instruction Instruction sent to flash.
 * @param[in] addr Address/offset of sector in flash to read.
 */
void qspi_flash_read_data(uint32_t *rd_ptr,uint32_t rd_len_in_word,uint8_t instruction,uint32_t addr);
/* suitable for flash Quad Read CMD 0x6B */
/**
 * @brief Read data from flash in quad SPI format.
 *
 * @param[in] rd_ptr Data buffer of received data.
 * @param[in] rd_len_in_word Received data length in word.
 * @param[in] instruction Instruction sent to flash.
 * @param[in] addr Address/offset of sector in flash to read.
 * @param[in] wait_cycles Waiting cycles to configured.
 */
void qspi_flash_multi_read_data(uint32_t *rd_ptr,uint32_t rd_len_in_word,uint8_t instruction,uint32_t addr,uint8_t wait_cycles,uint8_t dual_or_quad);

/// @}
#endif
