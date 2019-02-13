#include <stdbool.h>
#include <stddef.h>
#include "boot_typedef.h"
#include "boot_ram.h"
#include "boot_memory_map.h"
#include "flash.h"
#include "apollo_00.h"
#include "reg_sysc_cpu.h"
#include "reg_sysc_per.h"
#include "reg_sysc_awo.h"
#include "qspi.h"
#include "dma_for_qspi.h"
typedef uint16_t (*crc_func_t)(uint16_t ,const void *,uint32_t);
#define BOOT_HEADER_ATTRIBUTE __attribute__((section("boot_header_area")))
#define TO_BE_FILLED        0xffffffff
#define BOOTINIT_FLAG 0x3399
#define APOLLO_00_A_CRC ((crc_func_t)0x1b1)
#define APOLLO_00_B_CRC ((crc_func_t)0x18d)
void boot_ram_entry(void);
boot_ram_header_t const boot_ram_header BOOT_HEADER_ATTRIBUTE = {
    .header = {
        .bx_flag = BX_FLAG_CONST,
        .base_addr = (uint8_t *)RAM_CODE_EXEC_BASE,
        .length = TO_BE_FILLED,
        .entry_point = boot_ram_entry,
    },
    .flash_total_size = TO_BE_FILLED,
    .nvds_base = TO_BE_FILLED,
    .image_limit = TO_BE_FILLED,
};
image_header_t image_header[2];
#define GET_CODE_POSITION_IN_FLASH(header_ptr) ((header_ptr)==image_header?\
    (IMAGE_OFFSET_BASE + FLASH_IMAGE_HEADER_SIZE) :\
    (IMAGE_OFFSET_BASE + boot_ram_header.image_limit + FLASH_IMAGE_HEADER_SIZE))

void boot_ram_awo_reset()
{

    hwp_sysc_awo->awo_reset_signal.val = 0x2a8;
    hwp_sysc_awo->awo_reset_signal.val = 0x154;

    hwp_sysc_awo->reg_awo_clkg_en.val = 0x5555;
    hwp_sysc_awo->reg_misc_1.val = 0xe00081;
    hwp_sysc_awo->reg_clk_ldo_sel.val = 0x1a1c10;

    hwp_sysc_awo->reg_io_state.val = 0x0;
    hwp_sysc_awo->reg_pd_onoff.val = 0x0;
    hwp_sysc_awo->reg_sram_lowvdd.val = 0x7f07007f;
    hwp_sysc_awo->reg_pmu_timer.val = 0x8a51a10;
    hwp_sysc_awo->reg_wakeup_src_mask.val = 0x680;

    hwp_sysc_awo->reg_pd_state.val = 0xf7f;
    hwp_sysc_awo->ext_inrp_config.val = 0x2aa00;
    
    hwp_sysc_awo->clkgen_awo_signal_2.val = 0x2;
    hwp_sysc_awo->ram_parameter.val = 0x12c012c0;
    hwp_sysc_awo->vdd_value.val = 0x3;
    hwp_sysc_awo->gpio_ds0.val = 0x3fffffff;
    hwp_sysc_awo->gpio_ds1.val = 0x3fffffff;
    hwp_sysc_awo->gpio_ie.val = 0x3fffffff;
    hwp_sysc_awo->gpio_is.val = 0x3fffffff;
    hwp_sysc_awo->gpio_pe.val = 0x0;
    hwp_sysc_awo->gpio_ps.val = 0x0;
    hwp_sysc_awo->touch_cfg_1.val = 0x30000;
    hwp_sysc_awo->touch_cfg_2.val = 0x0;
    hwp_sysc_awo->touch_cfg_2.val = 0x0;

    volatile uint32_t *rf_reg = &hwp_sysc_awo->rf_reg_0.val;
    rf_reg[0] = 0;
    rf_reg[1] &= 0x1e000;
    uint8_t i;
    for(i=2;i<16;++i)
    {
        rf_reg[i]=0x0;
    }
    
    hwp_sysc_awo->pwr_pwm0.val = 0x0;
    hwp_sysc_awo->pwr_pwm1.val = 0x0;
    hwp_sysc_awo->pwr_pwm2.val = 0x0;
    hwp_sysc_awo->pwr_pwm_ctrl.val = 0x111;
}

static image_header_t *image_select()
{
    if(image_header[0].feature.valid== 0 && image_header[0].feature.obsolete == 1)
    {
        return &image_header[0];
    }else if(image_header[1].feature.valid == 0 && image_header[1].feature.obsolete == 1)
    {
        return &image_header[1];
    }else
    {
        return NULL;
    }
}

static void system_reset()
{
    #if HW_BX_VERSION == 00
    sysc_awo_awo_reset_signal_pack(0,0,0,0,0,0,0,0,1,0);
    #elif HW_BX_VERSION == 01
    sysc_awo_sft_rst_set(SRST_ALL_CLR);       
    #else
    #error ERROR: ERROR to define HW_BX_VERSION
    #endif
}

bool ram_code_crc_check(image_header_t *image,crc_func_t crc_func)
{
    uint16_t quotient = image->image_ram_size / CRC_BLOCK_SIZE_MAX;
    uint16_t remainder = image->image_ram_size % CRC_BLOCK_SIZE_MAX;
    uint16_t i;
    uint16_t ram_crc;
    uint8_t *base = (uint8_t *)RAM_BASE;
    for(i=0;i<quotient;++i)
    {
        ram_crc = crc_func(0, base , CRC_BLOCK_SIZE_MAX);
        if(ram_crc != image->crc[i])
        {
            return false;
        }
        base += CRC_BLOCK_SIZE_MAX;
    }
    ram_crc = crc_func(0, base , remainder);
    if(ram_crc != image->crc[i])
    {
        return false;
    }
    return true;
}

void boot_ram_entry()
{
//    boot_ram_awo_reset();
    dmac_init();
    qspi_init(2,1);
    uint32_t *vec_int_base;
    image_header_t *image;
    do
    {
        flash_read(IMAGE_OFFSET_BASE,sizeof(image_header_t), (void *)&image_header[0]);
        flash_read(IMAGE_OFFSET_BASE + boot_ram_header.image_limit,sizeof(image_header_t),(void *)&image_header[1]);
        image = image_select();
    }while (image == NULL);
    
    uint32_t flash_offset = image == image_header? 0: boot_ram_header.image_limit;
    uint32_t flash_xip_size = image->image_total_size - image->image_ram_size;
    uint32_t image_offset = flash_offset + IMAGE_OFFSET_BASE + FLASH_IMAGE_HEADER_SIZE + flash_xip_size;
    if(image->feature.flash_op_mode == Quad_Output_Read_Mode || image->feature.flash_op_mode == Quad_IO_Read_Mode)
    {
        sysc_cpu_qspi_en_setf(0xf);
        if(flash_quad_enable_check()==false)
        {
            flash_quad_enable_01();
        }
    }else
    {
        sysc_cpu_qspi_en_setf(0x3);
    }
    if(image->feature.flash_op_mode == Std_Read_Mode)
    {
        flash_read(image_offset, image->image_ram_size, (uint8_t *)RAM_BASE);
    }else
    {
        flash_multi_read(image_offset, image->image_ram_size, (uint8_t *)RAM_BASE,image->feature.flash_op_mode);
    }
    crc_func_t crc16ccitt = (hwp_sysc_awo->rf_reg_rd.val & 0x1f) >=  0x14 ? APOLLO_00_B_CRC : APOLLO_00_A_CRC;
    if(ram_code_crc_check(image,crc16ccitt)==false)
    {
        system_reset();
    }
    vec_int_base = (uint32_t *)RAM_BASE;
    __set_MSP(*vec_int_base);
    typedef void (*void_fn)(uint32_t,uint32_t,uint32_t,uint32_t);
    void_fn *reset_handler = (void_fn *)(vec_int_base + 1);
    // Jump to user's Reset_Handler
    (*reset_handler)(flash_offset,boot_ram_header.nvds_base,
        image->feature.flash_op_mode,BOOTINIT_FLAG);        //0x3399 indicate bootram has runned.
}


	
