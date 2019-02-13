/**
 ****************************************************************************************
 *
 * @file flash.h
 *
 * @brief Types definition and declaration of functions used for flash
 *
 * Copyright (C) Bluexmicro electronics 2015-2016
 *
 *
 ****************************************************************************************
 */
#ifndef FLASH_H_
#define FLASH_H_

#include <stdint.h>               // standard integer functions
#include <stdbool.h>
#include "qspi.h"  
/**
 ****************************************************************************************
 * @addtogroup FLASH
 * @ingroup DRIVERS
 *
 * @brief Flash memory driver
 *
 * @{
 ****************************************************************************************
 */



/*
 * DEFINES
 ****************************************************************************************
 */
#define FLASH_TYPE_UNKNOWN             0

typedef enum{
    Sector_Erase,
    Block_32KB_Erase,
    Block_64KB_Erase,
    Chip_Erase,
}erase_t;
#define FLASH_READ_MAX_BYTES (QSPI_READ_MAX_LENGTH*sizeof(uint32_t))
#define FLASH_MAX_SIZE			0x100000
#define BLOCK_64K               65536
#define FLASH_PAGE_SIZE               256

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize flash driver.
 ****************************************************************************************
 */
void flash_init(void);
void flash_qspi_io_init(uint32_t mode);
void flash_init_mp(uint32_t);
void flash_reset(void);
void flash_wakeup(void);
uint32_t flash_read_memory_density(void);
void flash_read_sfdp(uint32_t addr,uint8_t *buf,uint8_t buf_len);

/**
 ****************************************************************************************
 * @brief   Erase a flash section.
 *
 * This function is used to erase a part of the flash memory.
 * 
 * Note: callback parameter is not used
 *
 * @param[in]    offset      Starting offset from the beginning of the flash device
 * @param[in]    type        Erase command
 * @return       status      0 if operation can start successfully
 ****************************************************************************************
 */
uint8_t flash_erase(uint32_t offset, erase_t type);

/**
 ****************************************************************************************
 * @brief   Write a flash section.
 *
 * This function is used to write a part of the flash memory.
 * 
 * Note: callback parameter is not used
 *
 * @param[in]    offset      Starting offset from the beginning of the flash device
 * @param[in]    length      Size of the portion of flash to write
 * @param[in]    buffer      Pointer on data to write
 * @return       status      0 if operation can start successfully
 ****************************************************************************************
 */
uint8_t flash_program(uint32_t offset, uint32_t length, uint8_t *buffer);

uint8_t flash_program_jflash(uint32_t offset, uint32_t length, uint8_t *buffer);

/**
 ****************************************************************************************
 * @brief   Read a flash section.
 *
 * This function is used to read a part of the flash memory.
 * 
 * Note: callback parameter is not used
 *
 * @param[in]    offset      Starting offset from the beginning of the flash device
 * @param[in]    length      Size of the portion of flash to read
 * @param[out]   buffer      Pointer on data to read
 * @return       status      0 if operation can start successfully
 ****************************************************************************************
 */
uint8_t flash_read(uint32_t offset, uint32_t length, uint8_t *buffer);

uint8_t flash_multi_read(uint32_t offset, uint32_t length, uint8_t *buffer,bool quad);

uint8_t flash_quad_read(uint32_t offset, uint32_t length, uint8_t *buffer);

uint8_t flash_dual_read(uint32_t offset, uint32_t length, uint8_t *buffer);

void flash_deep_power_down(void);

void flash_test(void);

/// @} FLASH

#endif // FLASH_H_
