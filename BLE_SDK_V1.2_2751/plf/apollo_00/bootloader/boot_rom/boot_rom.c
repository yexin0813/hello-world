#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "bx_config.h"
#include "boot_typedef.h"
#include "reg_sysc_awo.h"
#include "crc16ccitt.h"
#include "pshare_rom.h"
#include "clk_gen_rom.h"
#include "uart.h"
#include "flash.h"
#define VDD_PAD3_MASK 0x10000
#define VDD_PAD2_MASK 0x08000
#define VDD_PAD1_MASK 0x04000
#define VDD_PAD0_MASK 0x02000
#define QSPI_PAD_MASK VDD_PAD3_MASK
#define UART_PAD_MASK VDD_PAD2_MASK
#define SWD_PAD_MASK VDD_PAD0_MASK
#define PAYLOAD_WITH_CRC_LENGTH(len) ((len) - sizeof(boot_header_t) + CRC_FIELD_LENGTH)
#define NOP_COUNTS 1024
typedef void (*read_fn_t)(uint8_t *buf,uint32_t length,void (*callback)(void));
typedef union{
    struct
    {
        uint32_t  boot_source :1;
        uint32_t pad_voltage :1;
    }s;
    uint32_t n;
}boot_mode_t;

uint8_t const bx_flag[FLAG_CONST_SIZE] = BX_FLAG_CONST;
boot_mode_t boot_mode;
struct {
    boot_header_t header;
    uint8_t *rx_ptr;
    bool done;
}uart_boot_env;
struct {
    boot_header_t header;
}flash_boot_env;

static void uart_listen(void);

void pad_voltage_set(uint8_t pad_voltage,uint32_t pad_mask)
{
    uint32_t rf_reg_1 = sysc_awo_rf_reg_1_get();
    if(pad_voltage == VOLTAGE_1V8)
    {    
        rf_reg_1 &= ~pad_mask;
    }else
    {
        rf_reg_1 |= pad_mask;
    }
    sysc_awo_rf_reg_1_set(rf_reg_1);
}

static bool crc_chk(boot_header_t *header,uint16_t header_crc)
{
    uint16_t rx_crc;
    uint8_t *ptr = (uint8_t *)&rx_crc;
    uint32_t payload_size = header->length - sizeof(boot_header_t);
    ptr[0] = header->base_addr[payload_size];
    ptr[1] = header->base_addr[payload_size + 1];
    return rx_crc == crc16ccitt(header_crc, header->base_addr, payload_size);
}

static void uart_payload_rx_done(void *param,uint8_t status)
{
    uint16_t header_crc = (uint32_t)param;
    if(crc_chk(&uart_boot_env.header,header_crc))
    {
        uart_boot_env.done = true;
    }else
    {
        uart_listen();
    }
}

static void uart_payload_rx(uint16_t header_crc)
{
    uart_read(uart_boot_env.header.base_addr,PAYLOAD_WITH_CRC_LENGTH(uart_boot_env.header.length), uart_payload_rx_done, (void *)header_crc);
}

static void uart_header_ack_tx_done(void *param,uint8_t status)
{
    uint16_t header_crc = crc16ccitt(0,&uart_boot_env.header,sizeof(uart_boot_env.header));
    uart_payload_rx(header_crc);
}

static void uart_header_ack()
{
    uart_write(BX_UART_BOOT_ACK,sizeof(BX_UART_BOOT_ACK),uart_header_ack_tx_done,NULL);
}

static void uart_header_rx_done(void *param,uint8_t status)
{
    pshare_rom_uart0_tx_init();
    uart_header_ack();    
}

static void uart_header_rx_start()
{
    uart_read(uart_boot_env.rx_ptr, sizeof(boot_header_t) - sizeof(bx_flag), uart_header_rx_done, NULL);
}

static void uart_bx_flag_check(void *param,uint8_t status)
{
    uint8_t idx = uart_boot_env.rx_ptr - uart_boot_env.header.bx_flag;
    if(*uart_boot_env.rx_ptr == bx_flag[idx])
    {
        ++uart_boot_env.rx_ptr;
        if(idx == FLAG_CONST_SIZE -1)
        {
            uart_header_rx_start();
        }else{
            uart_read(uart_boot_env.rx_ptr, 1, uart_bx_flag_check, NULL);
        }    
    }else
    {
        uart_listen();
    }
}

static void uart_listen()
{
    uart_boot_env.rx_ptr = uart_boot_env.header.bx_flag;
    uart_read(uart_boot_env.rx_ptr,1, uart_bx_flag_check, NULL);
}

static void uart_boot_init()
{
    clk_uart0_115200();
    pshare_rom_uart0_rx_init();
    uart_init(1);
    NVIC_EnableIRQ(UART0_IRQn);
}

static void flash_boot_init()
{
    pshare_rom_spi_flash_init();
    flash_init();
}

static void nop_delay(uint32_t counts)
{
    uint32_t i;
    for(i=0;i<counts;++i)
    {
        __nop();
    }
}

static bool flash_boot_try()
{
    flash_reset();
    nop_delay(NOP_COUNTS);
    flash_wakeup();
    nop_delay(NOP_COUNTS);
    flash_read(0,sizeof(boot_header_t),(uint8_t *)&flash_boot_env.header);
    if(memcmp(flash_boot_env.header.bx_flag,bx_flag,FLAG_CONST_SIZE))
    {
        return false;
    }else
    {
        uint16_t header_crc = crc16ccitt(0,&flash_boot_env.header,sizeof(flash_boot_env.header));
        flash_read(sizeof(boot_header_t),PAYLOAD_WITH_CRC_LENGTH(flash_boot_env.header.length) ,flash_boot_env.header.base_addr);
        if(crc_chk(&flash_boot_env.header,header_crc))
        {
            return true;
        }else
        {
            return false;
        }
    }
}

int main()
{
    pad_voltage_set(VOLTAGE_3V,SWD_PAD_MASK);
    boot_mode.n = sysc_awo_boot_mode_get();
    uart_boot_init();
    uart_listen();
    if(boot_mode.s.boot_source == BOOT_FROM_SPI_FLASH)
    {
        pad_voltage_set(VOLTAGE_3V,UART_PAD_MASK);
        pad_voltage_set(boot_mode.s.pad_voltage,QSPI_PAD_MASK);
        flash_boot_init();
        while(flash_boot_try()==false)
        {
            nop_delay(NOP_COUNTS);
        }
        flash_boot_env.header.entry_point();
    }else
    {
        pad_voltage_set(boot_mode.s.pad_voltage,UART_PAD_MASK);
        while(1)
        {
            if(uart_boot_env.done)
            {
                uart_boot_env.header.entry_point();
            }
        }
    }
    return 0;
}


void awo_reg_reset()
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
    uint8_t i;
    for(i=0;i<16;++i)
    {
       rf_reg[i]=0x0;
    }
        
    hwp_sysc_awo->pwr_pwm0.val = 0x0;
    hwp_sysc_awo->pwr_pwm1.val = 0x0;
    hwp_sysc_awo->pwr_pwm2.val = 0x0;
    hwp_sysc_awo->pwr_pwm_ctrl.val = 0x111;
    
}

void SystemInit(uint32_t param0,uint32_t param1,uint32_t param2,uint32_t param3)
{
//    awo_reg_reset();
    hwp_sysc_awo->pwr_pwm_ctrl.val = 0x111;
    hwp_sysc_awo->gpio_is.val = 0x3fffffff;
}

void UART0_IRQHandler(void)
{
    uart_isr();
}

