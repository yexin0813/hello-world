#include <stdint.h>        
#include <string.h>

#include "flash.h"
#include "bx_dbg.h"
#include "gd25q64c_typedef.h"
void flash_reset()
{
    uint8_t enable_reset = FLASH_ENABLE_RESET;
    qspi_write(&enable_reset,sizeof(enable_reset));
    uint8_t reset_device = FLASH_RESET_DEVICE;
    qspi_write(&reset_device,sizeof(reset_device));
}

void flash_wakeup()
{
    uint8_t flash_res_cmd = FLASH_RELEASE_POWER_DOWN;
    qspi_write(&flash_res_cmd,sizeof(flash_res_cmd));
}

void flash_deep_power_down()
{
    uint8_t flash_dp_cmd = FLASH_DEEP_POWER_DOWN;
    qspi_write(&flash_dp_cmd, sizeof(flash_dp_cmd));
}

void flash_init(void)
{
	qspi_init();
}

uint8_t flash_read(uint32_t offset, uint32_t length, uint8_t *buffer)
{
	BX_ASSERT(offset<FLASH_MAX_SIZE);
	uint32_t length_in_words = length / sizeof(uint32_t);
	uint8_t remainder = length % sizeof(uint32_t);
	BX_ASSERT(length_in_words<=QSPI_READ_MAX_LENGTH);
    if(length_in_words)
    {
	qspi_flash_read_data((uint32_t *)buffer,length_in_words,FLASH_READ_DATA_BYTES,offset);
	buffer += sizeof(uint32_t)*length_in_words;
	offset += sizeof(uint32_t)*length_in_words;
    }
	if(remainder)
	{
		uint8_t read_cmd_buf[4];
		read_cmd_buf[0]= FLASH_READ_DATA_BYTES;
		read_cmd_buf[1]= (offset>>16)&0xff;
		read_cmd_buf[2]= (offset>>8)&0xff;
		read_cmd_buf[3]= (offset)&0xff;
		qspi_read(buffer,remainder,read_cmd_buf,sizeof(read_cmd_buf));
	}
	return 0;
}
