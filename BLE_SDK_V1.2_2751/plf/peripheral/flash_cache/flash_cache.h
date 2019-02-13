#ifndef FLASH_CACHE_H_
#define FLASH_CACHE_H_
#include <stdint.h>
#include "periph_error.h"
void flash_cache_config(uint32_t);
periph_err_t flash_cache_enable(uint8_t);
void flash_cache_disable(void);
void flash_cache_flush(uint32_t,uint32_t);
void flash_cache_flush_all(void);

#endif
