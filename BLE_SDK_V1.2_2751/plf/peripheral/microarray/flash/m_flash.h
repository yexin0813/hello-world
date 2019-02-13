/*
 * Built-in Function Declares and MACRO Defines
 *
 * Copyright (C) 2017 Microarray Electronic Co., LTD.
 *
 * Written by:
 *     Hans.Huang  hux@microarray.com.cn
 *     
 * Hans.Huang    09/04/17     Define MACRO and declare functions for built-in flash operation
 */



#ifndef _FLASH_H
#define _FLASH_H

#include <stdint.h>
#include "flash.h"



/*
 * One template need about 4K bytes to store the data
 * One finger enroll max 12 templates, so one finger need about 50K bytes
 * The size of W25Q80EW is 1M bytes, the first 512K bytes for store firmware,
 * the last 8K bytes for the BLE parameters, so only (0x00080000 + 512K - 8K)
 * for store the fingerprint data.
 */
#define EFM0_MAIN_BASEADDR		(0x00800000UL)   /* 5 Fingers */
//#define EFM1_MAIN_BASEADDR		(0x008C1000UL)   /* 3 Fingers */


/* eflash page size. */
#define EFLASH_PAGESIZE			(0x1000)     /* 4K */
#define EFLASH1_PAGESIZE		(0x1000)	 /* 4K */


#define EFLASH_PROG_ERASE_PASS		(0)
#define EFLASH_PROG_ERASE_FAIL		(1)


#define EFLASH_OPERATION_KEY		(0x9536142e)

#define getKeyLL()	((EFLASH_OPERATION_KEY)&(0xff))
#define getKeyLH()	((EFLASH_OPERATION_KEY>>8)&(0xff))
#define getKeyHL()	((EFLASH_OPERATION_KEY>>16)&(0xff))
#define getKeyHH()	((EFLASH_OPERATION_KEY>>24)&(0xff))




void flashInit(void);

uint8_t eflash_write(uint32_t flashaddr, uint32_t datalen, uint32_t *bufaddr);

uint8_t eflash_page_erase (uint32_t addr) ;
uint8_t eflash_bulk_program(uint32_t addr, uint32_t num_words, uint32_t *data_buf);
uint8_t eflash_program (uint32_t addr, uint32_t data);
void *wb_memcpy(void *dest, const void *src, uint32_t n);




#endif /* End of _FLASH_H */

