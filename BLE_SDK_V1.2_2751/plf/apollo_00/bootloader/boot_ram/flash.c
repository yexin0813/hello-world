#include <stdint.h>        
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "spi_flash_cmd.h"
#include "flash.h"
#include "qspi.h"
#include "boot_ram.h"
/*
 * DEFINES
 ****************************************************************************************
 */

#define FLASH_PAGE_SIZE 256
#define QUAD_EN_BIT_OFFSET 1


/*
 * STRUCT DEFINITIONS
 ****************************************************************************************
 */



/*
 * GLOBAL VARIABLE DECLARATION
 ****************************************************************************************
 */

/*
 * LOCAL FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */

static uint8_t flash_operation_wait()
{
    uint8_t flash_status;
    do
    {
        flash_status = flash_read_status_reg_1();
    }while(flash_status&0x1);                                               // wait until erase done
    return flash_status;
}

static void flash_write_enable()
{
    uint8_t write_en_cmd = FLASH_WRITE_ENABLE;
    qspi_std_write(&write_en_cmd,sizeof(write_en_cmd));
}

/*
 * EXPORTED FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */

uint8_t flash_read_status_reg_2()
{
    uint8_t flash_status,read_stat_cmd = FLASH_READ_STATUS_REGISTER_2;
    qspi_std_read(&read_stat_cmd,sizeof(uint8_t),&flash_status,sizeof(uint8_t));
    return flash_status;
}

uint8_t flash_read_status_reg_1()
{
    uint8_t flash_status,read_stat_cmd = FLASH_READ_STATUS_REGISTER_1;
    qspi_std_read(&read_stat_cmd,sizeof(uint8_t),&flash_status,sizeof(uint8_t));
    return flash_status;
}

bool flash_quad_enable_check()
{
    if(flash_read_status_reg_2()&1<<QUAD_EN_BIT_OFFSET)
    {
        return true;
    }else
    {
        return false;
    }
}

void flash_write_status_reg_2bytes(uint8_t reg1,uint8_t reg2)
{
    uint8_t data[3] = {FLASH_WRITE_STATUS_REGISTER_1,reg1&0xff,reg2&0xff};
    flash_write_enable();
    qspi_std_write(data,sizeof(data));
    flash_operation_wait();
}

void flash_quad_enable_01()
{
    flash_write_status_reg_2bytes(0,1<<QUAD_EN_BIT_OFFSET);
}

void flash_read_sfdp(uint32_t addr,uint8_t *buf,uint8_t buf_len)
{
    uint8_t flash_write_cmd[5] = {FLASH_READ_SFDP,(addr>>16)&0xff,(addr>>8)&0xff,addr&0xff,0};
    qspi_std_read(flash_write_cmd,sizeof(flash_write_cmd), buf, buf_len);
}

uint32_t flash_read_jedec_ptp()
{
    uint32_t jedec_ptp;
    flash_read_sfdp(0x0c,(uint8_t *)&jedec_ptp,sizeof(jedec_ptp));
    return jedec_ptp;
}

uint32_t flash_read_memory_density()
{
    uint32_t jedec_ptp = flash_read_jedec_ptp();
    uint32_t mem_density;
    flash_read_sfdp(jedec_ptp + 4,(uint8_t *)&mem_density,sizeof(mem_density));
    return mem_density;
}

void flash_reset()
{
    uint8_t enable_reset = FLASH_ENABLE_RESET;
    qspi_std_write(&enable_reset,sizeof(uint8_t));
    uint8_t reset_device = FLASH_RESET_DEVICE;
    qspi_std_write(&reset_device,sizeof(uint8_t));
}

void flash_wakeup()
{
    uint8_t flash_res_cmd = FLASH_RELEASE_POWER_DOWN;
    qspi_std_write(&flash_res_cmd,sizeof(flash_res_cmd));
}

void flash_deep_power_down()
{
    uint8_t flash_dp_cmd = FLASH_DEEP_POWER_DOWN;
    qspi_std_write(&flash_dp_cmd, sizeof(flash_dp_cmd));
}

uint8_t flash_read(uint32_t offset, uint32_t length, uint8_t *buffer)
{
    uint16_t quotient = length >> 16;
    uint16_t remainder = length & 0xffff;
    uint16_t i;
    for(i=0;i<quotient;++i)
    {
        uint8_t read_cmd[4] = {FLASH_READ_DATA_BYTES,offset>>16&0xff,offset>>8&0xff,offset&0xff};
        qspi_std_read(read_cmd,sizeof(read_cmd), buffer, 0x10000);
        offset += 0x10000;
        buffer += 0x10000;
    }
    if(remainder)
    {
        uint8_t read_cmd[4] = {FLASH_READ_DATA_BYTES,offset>>16&0xff,offset>>8&0xff,offset&0xff};
        qspi_std_read(read_cmd,sizeof(read_cmd), buffer, remainder);

    }
    return 0;
}



uint8_t flash_erase(uint32_t offset,erase_t type)
{
    flash_write_enable();
    if(type == Chip_Erase)
    {
        uint8_t erase_cmd_addr[1] = {FLASH_CHIP_ERASE};
        uint8_t cmd_length = sizeof(erase_cmd_addr);
        qspi_std_write(erase_cmd_addr,cmd_length);
    }
        else if(type == Block_64KB_Erase)
        {
        uint8_t erase_cmd_addr[4] = {FLASH_64KB_BLOCK_ERASE,offset>>16&0xff,offset>>8&0xff,offset&0xff};
        uint8_t cmd_length = sizeof(erase_cmd_addr);
        qspi_std_write(erase_cmd_addr,cmd_length);
    }
        else if(type == Block_32KB_Erase)
        {
        uint8_t erase_cmd_addr[4] = {FLASH_32KB_BLOCK_ERASE,offset>>16&0xff,offset>>8&0xff,offset&0xff};
        uint8_t cmd_length = sizeof(erase_cmd_addr);
        qspi_std_write(erase_cmd_addr,cmd_length);
    }        
        else
    {
        uint8_t erase_cmd_addr[4] = {FLASH_SECTOR_ERASE,offset>>16&0xff,offset>>8&0xff,offset&0xff};
        uint8_t cmd_length = sizeof(erase_cmd_addr);
        qspi_std_write(erase_cmd_addr,cmd_length);
    }
    flash_operation_wait();
    return 0;
}

static void flash_program_base(uint32_t offset, uint32_t length, uint8_t *buffer)
{
    flash_write_enable();
    qspi_flash_program(FLASH_PAGE_PROGRAM,offset,buffer,length);
    flash_operation_wait();
}

uint8_t flash_program(uint32_t offset, uint32_t length, uint8_t *buffer)
{
    uint32_t offset_base = offset&(~(FLASH_PAGE_SIZE-1));
    if(offset_base!=offset)
    {
        uint32_t tailing_length = offset_base+FLASH_PAGE_SIZE-offset;
        uint32_t prog_length = length<tailing_length? length : tailing_length;
        flash_program_base(offset,prog_length,buffer);
        buffer += prog_length;
        offset += prog_length;
        length -= prog_length;
    }
    uint16_t cycles = length / FLASH_PAGE_SIZE;
    uint16_t remainder = length % FLASH_PAGE_SIZE;
    uint16_t i;
    for(i=0;i<cycles;++i)
    {
        flash_program_base(offset,FLASH_PAGE_SIZE,buffer);
        offset += FLASH_PAGE_SIZE;
        buffer += FLASH_PAGE_SIZE;
    }
    if(remainder)
    {
        flash_program_base(offset,remainder,buffer);
    }
    return 0;
}

uint8_t flash_multi_read(uint32_t offset, uint32_t length, uint8_t *buffer,uint8_t type)
{
    qspi_multi_read_param_t param=
     {
        .data = buffer,
        .length = length,
        .addr_field = 
        {
            .extend_byte = 0x00,
            .addr = offset,
        },
        .inst_l = Instruction_Length_8_bits,
     };
    switch(type)
    {
    case Dual_Output_Read_Mode:
        param.cmd = FLASH_DUAL_OUTPUT_FAST_READ;
        param.trans_type = Both_Standard_SPI_Mode;
        param.wait_cycles = 0;
        param.dual_quad = Dual_SPI_Format;
    break;
    case Quad_Output_Read_Mode:
        param.cmd = FLASH_QUAD_OUTPUT_FAST_READ;
        param.trans_type = Both_Standard_SPI_Mode;
        param.wait_cycles = 0;
        param.dual_quad = Quad_SPI_Format;
    break;
    case Dual_IO_Read_Mode:
        param.cmd = FLASH_DUAL_IO_FAST_READ;
        param.trans_type = Instruction_Standard_Address_Specific;
        param.wait_cycles = 0;
        param.dual_quad = Dual_SPI_Format;
    break;
    case Quad_IO_Read_Mode:
        param.cmd = FLASH_QUAD_IO_FAST_READ;
        param.trans_type = Instruction_Standard_Address_Specific;
        param.wait_cycles = 4;
        param.dual_quad = Quad_SPI_Format;
    break;
    }
    qspi_multi_read(&param);
    return 0;
}




#ifdef FLASH_TEST
#define SECTOR_SIZE 0x1000
#define FLASH_SIZE (1<<20)
#define SECTOR_SIZE_IN_WORD (SECTOR_SIZE/sizeof(uint32_t))
uint32_t fbuf[SECTOR_SIZE_IN_WORD];
uint32_t fsrc[SECTOR_SIZE_IN_WORD];
void flash_test()
{
    sysc_cpu_qspi_en_setf(0xf);
//    *(uint32_t*)0x20201074 = 0x0010000;// 0x8010000 on V2/0x0010000 on V3 - set VDD_PAD3 to 3.3V for qspi
    uint32_t offset = 0;
    while(offset<FLASH_SIZE)
    {
        /* erase test */
        flash_erase(offset,Sector_Erase);
//        flash_read(offset,SECTOR_SIZE,(uint8_t *)fbuf);
        flash_multi_read(offset,SECTOR_SIZE,(uint8_t *)fbuf, 1);
        uint32_t i;
        for(i=0;i<SECTOR_SIZE_IN_WORD;++i)
        {
            if(fbuf[i] != 0xffffffff)
            {
                BX_ASSERT(0);
                break;
            }
        }
        /* program test */
        for(i=0;i<SECTOR_SIZE_IN_WORD;++i)
        {
            fsrc[i] = rand();
        }
        flash_program(offset,SECTOR_SIZE,(uint8_t *)fsrc);
        flash_multi_read(offset,SECTOR_SIZE,(uint8_t *)fbuf, 1);
        for(i=0;i<SECTOR_SIZE_IN_WORD;++i)
        {
            if(fsrc[i]!=fbuf[i])
            {
                BX_ASSERT(0);
                break;
            }
        }
        offset += SECTOR_SIZE;
    }

//    __asm("bkpt 01");
}
#endif
#if 0
#include "gpio.h"
void flash_readID_winbond(void)
{
    sysc_cpu_qspi_en_setf(0x3);
    struct gpioConfigStruct gpio_28_29;

    gpio_Init();

    memset(&gpio_28_29, 0, sizeof(gpio_28_29));

    gpio_InitN(gpio3_4);
    gpio_InitN(gpio3_5);

    // cfg3_5: output/high
    gpio_28_29.debounce = gpio_Deb;
    gpio_28_29.dir = gpio_Output;
    gpio_28_29.value = gpio_High;

    gpio_SetConfig(gpio3_4, &gpio_28_29);
    gpio_SetConfig(gpio3_5, &gpio_28_29);

    flash_quad_enable();
    sysc_cpu_qspi_en_setf(0xf);

    uint8_t write_en_cmd = FLASH_WRITE_ENABLE;
    qspi_write(&write_en_cmd,sizeof(write_en_cmd));

    uint32_t jedec_ID;
    uint8_t read_stat_cmd = 0x9f; // 0x9f = Read JEDEC ID
    qspi_read((uint8_t*)&jedec_ID,sizeof(jedec_ID)-1,&read_stat_cmd,sizeof(read_stat_cmd));
    flash_operation_wait();

    LOG(LOG_LVL_INFO,"jedec_ID=0x%x\n", jedec_ID);
}
#endif

//read reg
// 0 1 2
uint8_t flash_status_read(uint8_t reg_idx)
{
    uint8_t flash_status,read_stat_cmd = 0;
    
        switch(reg_idx)
        {
            case 0 :
            {
                read_stat_cmd = FLASH_READ_STATUS_REGISTER_1;
                qspi_std_read(&read_stat_cmd,sizeof(read_stat_cmd),&flash_status,sizeof(flash_status));
            }break;
            case 1 :
            {
                read_stat_cmd = FLASH_READ_STATUS_REGISTER_2;
                qspi_std_read(&read_stat_cmd,sizeof(read_stat_cmd),&flash_status,sizeof(flash_status));
            }break;
            case 2 :
            {
                read_stat_cmd = FLASH_READ_STATUS_REGISTER_3;
                qspi_std_read(&read_stat_cmd,sizeof(read_stat_cmd),&flash_status,sizeof(flash_status));
            }break;
            default:break;
        }            
    
    return flash_status;
}
//write single   0/1/2  val
//fw1b  c v
uint8_t flash_status_write(uint8_t reg,uint8_t value)
{
      uint8_t quad_en[2] = {FLASH_WRITE_STATUS_REGISTER_2,value};
        uint8_t flash_status=0;
        
        switch(reg)
        {
            case 0 :
            {
                flash_write_enable();
                quad_en[0] = FLASH_WRITE_STATUS_REGISTER_1;
                qspi_std_write(quad_en,sizeof(quad_en));
                flash_operation_wait();
                flash_status = flash_status_read(reg);
            }break;
            case 1 :
            {
                flash_write_enable();
                quad_en[0] = FLASH_WRITE_STATUS_REGISTER_2;
                qspi_std_write(quad_en,sizeof(quad_en));
                flash_operation_wait();
                flash_status = flash_status_read(reg);
            }break;
            case 2 :
            {
                flash_write_enable();
                quad_en[0] = FLASH_WRITE_STATUS_REGISTER_3;
                qspi_std_write(quad_en,sizeof(quad_en));
                flash_operation_wait();
                flash_status = flash_status_read(reg);
            }break;
            default:break;
        }        
        
    return flash_status;
}
//weiter 2    byte1 byte2
// fw2b
uint16_t flash_status_write_2bytes(uint8_t val_l,uint8_t val_h)
{
      uint8_t quad_en[3] = {FLASH_WRITE_STATUS_REGISTER_1,val_l,val_h};
        uint16_t flash_status=0;
        
        flash_write_enable();
        qspi_std_write(quad_en,sizeof(quad_en));
        flash_operation_wait();
        flash_status = flash_status_read(0);        
        flash_status |= (flash_status_read(1)<<8)&0xff00;
        
    return flash_status; 
}

