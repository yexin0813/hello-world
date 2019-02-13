#include <stdint.h>        
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bx_dbg.h"
#include "spi_flash_cmd.h"
#include "log.h"
#include "flash.h"
#include "app_qspi_wrapper.h"
#include "arch_init.h"
#if (defined(CFG_FREERTOS_SUPPORT)&&(CFG_FREERTOS_SUPPORT==1))
#include "FreeRTOS.h"
#include "task.h"
#endif

static void os_scheduler_disable()
{
    #if (defined(CFG_FREERTOS_SUPPORT)&&(CFG_FREERTOS_SUPPORT==1))
    vTaskSuspendAll();
    #endif
}

static void os_scheduler_restore()
{
    #if (defined(CFG_FREERTOS_SUPPORT)&&(CFG_FREERTOS_SUPPORT==1))
    xTaskResumeAll();
    #endif
}

static uint8_t flash_operation_wait()
{
    uint8_t flash_status,read_stat_cmd = FLASH_READ_STATUS_REGISTER_1;
    do
    {
        app_qspi_std_read_wrapper(&read_stat_cmd,sizeof(uint8_t),&flash_status,sizeof(uint8_t));
    }while(flash_status&0x1);                                               // wait until erase done
    return flash_status;
}

static void flash_write_enable()
{
    uint8_t write_en_cmd = FLASH_WRITE_ENABLE;
    app_qspi_std_write_wrapper(&write_en_cmd,sizeof(write_en_cmd));
}

void flash_read_sfdp(uint32_t addr,uint8_t *buf,uint8_t buf_len)
{
    uint8_t flash_write_cmd[5] = {FLASH_READ_SFDP,(addr>>16)&0xff,(addr>>8)&0xff,addr&0xff,0};
    app_qspi_std_read_wrapper(flash_write_cmd,sizeof(flash_write_cmd), buf, buf_len);
}

uint32_t flash_read_jedec_ptp()
{
    uint32_t jedec_ptp;
    flash_read_sfdp(0x0c,(uint8_t *)&jedec_ptp,sizeof(jedec_ptp));
    return jedec_ptp;
}

uint32_t flash_read_ID(void)
{
    uint8_t read_cmd[4] = {FLASH_READ_ID, 0, 0, 0};
    uint32_t data;
    app_qspi_std_read_wrapper((uint8_t*)&read_cmd,sizeof(read_cmd), (uint8_t*)&data, 2);
    return data;
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
    app_qspi_std_write_wrapper(&enable_reset,sizeof(uint8_t));
    uint8_t reset_device = FLASH_RESET_DEVICE;
    app_qspi_std_write_wrapper(&reset_device,sizeof(uint8_t));
}

void flash_wakeup()
{
    uint8_t flash_res_cmd = FLASH_RELEASE_POWER_DOWN;
    app_qspi_std_write_wrapper(&flash_res_cmd,sizeof(flash_res_cmd));
}

void flash_deep_power_down()
{
    uint8_t flash_dp_cmd = FLASH_DEEP_POWER_DOWN;
    app_qspi_std_write_wrapper(&flash_dp_cmd, sizeof(flash_dp_cmd));
}

uint8_t flash_read(uint32_t offset, uint32_t length, uint8_t *buffer)
{
    uint8_t read_cmd[4] = {FLASH_READ_DATA_BYTES,offset>>16&0xff,offset>>8&0xff,offset&0xff};
    app_qspi_std_read_wrapper(read_cmd,sizeof(read_cmd), buffer, length);
    return 0;
}


uint8_t flash_erase(uint32_t offset, erase_t type)
{
    BX_ASSERT(type==Sector_Erase || type==Block_32KB_Erase ||  type==Block_64KB_Erase || type==Chip_Erase);
    os_scheduler_disable();
    flash_write_enable();
    uint8_t erase_cmd_addr[4],cmd_length;
    erase_cmd_addr[1] = offset>>16 & 0xff;
    erase_cmd_addr[2] = offset>>8 & 0xff;
    erase_cmd_addr[3] = offset & 0xff;
    switch(type)
    {
    case Sector_Erase:
        erase_cmd_addr[0] = FLASH_SECTOR_ERASE;
        cmd_length = sizeof(erase_cmd_addr);
        break;
    case Block_32KB_Erase:
        erase_cmd_addr[0] = FLASH_32KB_BLOCK_ERASE;
        cmd_length = sizeof(erase_cmd_addr);
        break;
    case Block_64KB_Erase:
        erase_cmd_addr[0] = FLASH_64KB_BLOCK_ERASE;
        cmd_length = sizeof(erase_cmd_addr);
        break;
    case Chip_Erase:
        erase_cmd_addr[0] = FLASH_CHIP_ERASE;
        cmd_length = 1;
        break;
    }
    app_qspi_std_write_wrapper(erase_cmd_addr,cmd_length);
    flash_operation_wait();
    os_scheduler_restore();
    return 0;
}

static void flash_program_base(uint32_t offset, uint32_t length, uint8_t *buffer)
{
    BX_ASSERT(length<=256);
    os_scheduler_disable();
    flash_write_enable();
    app_qspi_flash_program_wrapper(FLASH_PAGE_PROGRAM,offset,buffer,length);
    flash_operation_wait();
    os_scheduler_restore();
}

uint8_t flash_program(uint32_t offset, uint32_t length, uint8_t *buffer)
{
    BX_ASSERT((offset&0xff000000)==0);
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

void flash_multi_read(uint32_t offset, uint32_t length, uint8_t * buffer,uint8_t type)
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
    app_qspi_multi_read_wrapper(&param);
}

uint8_t flash_dual_output_read(uint32_t offset, uint32_t length, uint8_t *buffer)
{
    flash_multi_read(offset,length,buffer,Dual_Output_Read_Mode);
    return 0;
}

uint8_t flash_quad_output_read(uint32_t offset, uint32_t length, uint8_t *buffer)
{
    flash_multi_read(offset,length,buffer,Quad_Output_Read_Mode);
    return 0;
}

uint8_t flash_dual_io_read(uint32_t offset, uint32_t length, uint8_t *buffer)
{
    flash_multi_read(offset,length,buffer,Dual_IO_Read_Mode);
    return 0;
}

uint8_t flash_quad_io_read(uint32_t offset, uint32_t length, uint8_t *buffer)
{
    flash_multi_read(offset,length,buffer,Quad_IO_Read_Mode);
    return 0;
}

uint8_t (*flash_read_func_ptr_get(void))(uint32_t,uint32_t,uint8_t*)
{
    extern uint32_t flash_op_mode;
    uint8_t (*fn)(uint32_t,uint32_t,uint8_t*)= NULL;
    switch(flash_op_mode)
    {
    case Std_Read_Mode:
        fn= flash_read;
    break;
    case Dual_Output_Read_Mode:
        fn= flash_dual_output_read;
    break;
    case Quad_Output_Read_Mode:
        fn= flash_quad_output_read;
    break;
    case Dual_IO_Read_Mode:
        fn= flash_dual_io_read;
    break;
    case Quad_IO_Read_Mode:
        fn= flash_quad_io_read;
    break;
    }
    return fn;
}


//#define FLASH_TEST
#ifdef FLASH_TEST
#include "io_ctrl.h"
#define SECTOR_SIZE 0x1000
#define FLASH_SIZE (1<<20)
#define SECTOR_SIZE_IN_WORD (SECTOR_SIZE/sizeof(uint32_t))
uint32_t fbuf[SECTOR_SIZE_IN_WORD];
uint32_t fsrc[SECTOR_SIZE_IN_WORD];
void flash_test()
{
//    sysc_cpu_qspi_en_setf(0xf);
//    *(uint32_t*)0x20201074 = 0x0010000;// 0x8010000 on V2/0x0010000 on V3 - set VDD_PAD3 to 3.3V for qspi
    uint32_t offset = 0;
    io_cfg_output(23);
    io_pin_clear(23);
    while(offset<FLASH_SIZE)
    {
        /* erase test */
        io_pin_set(23);
        flash_erase(offset,Sector_Erase);
        io_pin_clear(23);
//        flash_read(offset,SECTOR_SIZE,(uint8_t *)fbuf);
        flash_quad_read(offset,SECTOR_SIZE,(uint8_t *)fbuf);
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
        io_pin_set(23);
        flash_program(offset,SECTOR_SIZE,(uint8_t *)fsrc);
        io_pin_clear(23);
//        flash_read(offset,SECTOR_SIZE,(uint8_t *)fbuf);
        flash_quad_read(offset,SECTOR_SIZE,(uint8_t *)fbuf);
        for(i=0;i<SECTOR_SIZE_IN_WORD;++i)
        {
            if(fsrc[i]!=fbuf[i])
            {
            	LOG(LOG_LVL_ERROR,"%d\n",i);
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

