#include "platform.h"
#include "sensor_operation_afs121n.h"
#include "m_timer.h"
#include "m_uart.h"
#include "m_flash.h"
#include <stdlib.h>
#include <string.h>
#include "sensor_sal.h"



int32_t platform_create(void)
{
	return 0;
}


int32_t platform_destroy(void)
{
	return 0;
}


int32_t platform_fp_rw_reg(uint8_t reg, uint8_t data, uint8_t *old_val, uint8_t *new_val)
{
	sensorRWReg(reg, data, old_val, new_val);

	return 0;
}


int32_t platform_fp_set_mode(uint8_t mode)
{
	sensorSetMode(mode);

	return 0;       
}


int32_t platform_fp_read_line(uint8_t *buff, uint32_t len)
{

	sensorReadLine(buff, len);

	return 0;
}


int32_t platform_fp_read_frame(uint8_t *buff, uint32_t len)
{
	sensorReadFrame(buff, len);
        
	return 0;
}


int32_t platform_msleep(uint32_t ms)
{
	timerDelayMs((uint16_t)ms);
        
	return 0;
}


int32_t platform_get_jiffies(uint32_t *jiffies)
{
	*jiffies = systemTick();

	return 0;
}


int32_t platform_rand(void)
{
	return rand();
}


int32_t platform_rand_with_seed(int32_t seed)
{
	srand(seed);
	
	return rand();
}


int32_t platform_fs_read(uint32_t addr, void *buf, uint32_t size)
{
	if (size >= 20 * 1024) {
		LOGE("size %d too long.", size);
		return -1;
	}

	if (addr < EFM0_MAIN_BASEADDR) {
		LOGE("out of range %x %d", addr, size);
		return -2;
	} 

	if (!VALID_EFLASH_SPACE(addr, size)) {
		LOGE("out of range %x %d", addr, size);
		return -3;
	}

	//LOGE("Reading from Flash, addr = %#x, size = %d bytes.", addr, size);
	//timerRestart();

	memcpy(buf, (void *)(addr), size);
	//LOGE("read eflash , size = %d bytes ,time cost = %d us. \r\n", size,timerElapsedUs());
	
	return 0;
}


int32_t platform_fs_write(uint32_t addr, void *buf, uint32_t size, uint32_t key)
{
	uint32_t page_size = 0;
	uint32_t addr_erase = 0;
	int32_t ret = 0;
	
	if (addr % 0x800) {
		LOGE("addr is not page aligned.");
		return -1;
	}

	if (size >= 20 * 1024) {
		LOGE("size %d too long.", size);
		return -2;
	}

	if (!VALID_EFLASH_SPACE(addr, size)) {
		LOGE("out of range %x %d", addr, size);
		return -3;
	}

	//LOGE("Writing to Flash, addr = %#x, size = %d bytes.", addr, size);
	//timerRestart();
	//debugPrint("Writing to Flash, addr = %#x, size = %d bytes.", addr, size);

    

	addr_erase = addr;
	while(addr_erase < addr + size) {
		ret = eflash_page_erase(addr_erase);
		if (ret != EFLASH_PROG_ERASE_PASS){
			LOGE("eflash page erase %x failed @ %#x.", addr_erase, ret);
			return -4;
		}

		//if(addr_erase < EFM1_MAIN_BASEADDR)
			page_size = EFLASH_PAGESIZE;
		//else
			//page_size = EFLASH1_PAGESIZE;

		LOGD("Erase Flash, addr = %#x, size = %d bytes.", addr_erase, page_size);
		
		addr_erase += page_size;
	}
	
	ret = eflash_bulk_program(addr, ROUND_UP(size, 4) / 4, (uint32_t *)buf);
	//LOGE("Write eflash , size = %d bytes ,time cost = %d us. \r\n",  ROUND_UP(size, 4) ,timerElapsedUs());	

	return ret;
}


int32_t platform_fs_write_dword(uint32_t addr, uint32_t val, uint32_t key)
{
	eflash_program(addr, val);
    //LOGE("flash address:%x,%x\n",addr,val);
	
	return 0;
}


int32_t platform_fs_erase_page(uint32_t addr, uint32_t key)
{
	int32_t ret = 0;
	
	ret = eflash_page_erase(addr);
	if (ret != EFLASH_PROG_ERASE_PASS){
		LOGE("eflash page erase failed.");
		return -1;
	}

	return 0;
}


int32_t platform_fs_page_size(void)
{
	return EFLASH_PAGESIZE;	
}


int32_t platform_printf(const char *fmt, ...)
{

	return 0;
}
