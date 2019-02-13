#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include "mafp_defs.h"
#include "m_flash.h"

#define ROUND_UP(x, aligned)                    (((x) + (aligned) - 1) / (aligned) * (aligned))

#define SENSOR_DATA_OFFSET                      (512)		// code & data in flash = 64K 

#define SENSOR_PROPERTY_ADDR                    (EFM0_MAIN_BASEADDR + 1024 * (SENSOR_DATA_OFFSET + 4))   // 2K for system used

#define VALID_EFLASH_SPACE(addr, size)          (((addr) >= EFM0_MAIN_BASEADDR) && ((addr) + (size) <= (EFM0_MAIN_BASEADDR + 1* 1024 * 1024)))


/////////////////////////// Templates Flash related configuration start ///////////////////////////////

// 4K for SYSTEM, 4K prop, 16 for bkg, 4K for match record, 8K for finger template related
#define TPLS_EFLASH1_START                      (EFM0_MAIN_BASEADDR + 1024 * (SENSOR_DATA_OFFSET + 36))
#define TPLS_EFLASH1_END                        (EFM0_MAIN_BASEADDR + 1024 * (512+256))

#define TPLS_EFLASH2_START                      (TPLS_EFLASH1_END)
#define TPLS_EFLASH2_END                        (TPLS_EFLASH1_END+ 1024 * 240)








                
/////////////////////////// Templates Flash related configuration end  ///////////////////////////////

extern int32_t platform_create(void);
extern int32_t platform_destroy(void);
extern int32_t platform_fp_rw_reg(uint8_t reg, uint8_t data, uint8_t *old_val, uint8_t *new_val);
extern int32_t platform_fp_set_mode(uint8_t mode);
extern int32_t platform_fp_read_line(uint8_t *buff, uint32_t len);
extern int32_t platform_fp_read_frame(uint8_t *buff, uint32_t len);
extern int32_t platform_rand(void);
extern int32_t platform_rand_with_seed(int32_t seed);
extern int32_t platform_msleep(uint32_t ms);
extern int32_t platform_fs_read(uint32_t addr, void *buf, uint32_t size);
extern int32_t platform_fs_write(uint32_t addr, void *buf, uint32_t size, uint32_t key);
extern int32_t platform_fs_page_size(void);
extern int32_t platform_fs_erase_page(uint32_t addr, uint32_t key);
extern int32_t platform_fs_write_dword(uint32_t addr, uint32_t val, uint32_t key);
extern int32_t platform_get_jiffies(uint32_t *jiffies);

#endif

