#include "bx_config.h"

#include "io_ctrl.h"
#include "app_gpio.h"
#include "reg_sysc_awo.h"
#include "log.h"
#include "sysctrl.h"

static void (*ext_int_cb[5])();
static void (*touch_cb)();
app_gpio_inst_t app_gpio_inst = GPIO_INSTANCE();

void io_init(void)
{
    app_gpio_init(&app_gpio_inst.inst);
    // enable ext interrupt
    NVIC_ClearPendingIRQ(EXT_INTR_IRQn);
    NVIC_EnableIRQ(EXT_INTR_IRQn);
}

void io_write_all(uint32_t val)
{
    app_gpio_port_out_write(&app_gpio_inst.inst,val);
}

uint32_t io_read_all(void)
{
    return app_gpio_port_in_read(&app_gpio_inst.inst);
}

uint32_t io_out_read_all(void)
{
    return app_gpio_port_out_read(&app_gpio_inst.inst);
}

uint32_t io_dir_read_all(void)
{
    return app_gpio_port_dir_read(&app_gpio_inst.inst);
}

void io_dir_write_all_noie(uint32_t dir)
{
    app_gpio_port_dir_write(&app_gpio_inst.inst,dir);
}

void io_dir_write_all(uint32_t dir)
{
    sysctrl_io_config_mask(0xffffffff,GENERAL_PURPOSE_IO);
    app_gpio_port_dir_write(&app_gpio_inst.inst,dir);
}

void io_dir_output_set(uint32_t output_mask)
{
    sysctrl_io_config_mask(output_mask,GENERAL_PURPOSE_IO);
    app_gpio_port_dir_output_set(&app_gpio_inst.inst, output_mask);
}

void io_dir_input_set(uint32_t input_mask)
{
    sysctrl_io_config_mask(input_mask,GENERAL_PURPOSE_IO);
    app_gpio_port_dir_input_set(&app_gpio_inst.inst, input_mask);    
}

void io_cfg_output(uint8_t pin_num)
{
    sysctrl_io_config_mask(1<<pin_num,GENERAL_PURPOSE_IO);
    app_gpio_port_dir_output_set(&app_gpio_inst.inst, 1<<pin_num);
}

void io_cfg_input(uint8_t pin_num)
{
    sysctrl_io_config_mask(1<<pin_num,GENERAL_PURPOSE_IO);
    app_gpio_port_dir_input_set(&app_gpio_inst.inst, 1<<pin_num);
}

void io_pin_dir_set(uint8_t pin_num,uint8_t dir)
{
    if(dir)
    {
        io_cfg_output(pin_num); 
    }else
    {
        io_cfg_input(pin_num);
    }
}

uint8_t io_pin_read(uint8_t pin_num)
{
    uint32_t input = app_gpio_port_in_read(&app_gpio_inst.inst);
    return input >> pin_num & 0x1;
}

uint8_t io_pin_out_read(uint8_t pin_num)
{
    uint32_t output = app_gpio_port_out_read(&app_gpio_inst.inst);
    return output >> pin_num & 0x1;    
}

void io_pin_write(uint8_t pin_num,uint8_t val)
{
    uint32_t output = app_gpio_port_out_read(&app_gpio_inst.inst);
    if(val)
    {
        output |= 1<<pin_num;
    }else
    {
        output &= ~(1<<pin_num);
    }
    app_gpio_port_out_write(&app_gpio_inst.inst,output);
}

void io_pin_set(uint8_t pin_num)
{
    app_gpio_port_out_set(&app_gpio_inst.inst,1<<pin_num);    
}

void io_pin_clear(uint8_t pin_num)
{
    app_gpio_port_out_clr(&app_gpio_inst.inst,1<<pin_num); 
}

void io_pin_toggle(uint8_t pin_num)
{
    app_gpio_port_out_toggle(&app_gpio_inst.inst,1<<pin_num); 
}

void io_pin_pull_write(uint8_t pin_num,io_pull_type_t pull)
{
    uint32_t pull_en = sysc_awo_gpio_pe_get();
    uint32_t pull_sel = sysc_awo_gpio_ps_get();
    switch(pull)
    {
    case IO_PULL_DISABLE:
        pull_en &= ~(1<<pin_num);
        sysc_awo_gpio_pe_set(pull_en);
    break;
    case IO_PULL_UP:
        pull_en |= 1<<pin_num;
        pull_sel |= 1<<pin_num;
        sysc_awo_gpio_ps_set(pull_sel);
        sysc_awo_gpio_pe_set(pull_en);
    break;
    case IO_PULL_DOWN:
        pull_en |= 1<<pin_num;
        pull_sel &= ~(1<<pin_num);
        sysc_awo_gpio_ps_set(pull_sel);
        sysc_awo_gpio_pe_set(pull_en);
    break;
    default:
        BX_ASSERT(0);
    break;
    }
    
}

io_pull_type_t io_pin_pull_read(uint8_t pin_num)
{
    uint32_t pull_en = sysc_awo_gpio_pe_get();
    uint32_t pull_sel = sysc_awo_gpio_ps_get();
    if(pull_en & 1<<pin_num)
    {
        if(pull_sel & 1<<pin_num)
        {
            return IO_PULL_UP;
        }else
        {
            return IO_PULL_DOWN;
        }
    }else
    {
        return IO_PULL_DISABLE;
    }
}

void ext_set_touch_cb(void (*cb)(void))
{
    touch_cb = cb;
}
static uint8_t ext_int_idx_get(uint8_t pin_num)
{
    uint8_t idx = 0;
    switch(pin_num)
    {
    case 15:
        idx = 0;
    break;
    case 16:
        idx = 1;
    break;
    case 17:
        idx = 2;
    break;
    case 22:
        idx = 3;
    break;
    case 23:
        idx = 4;
    break;
    default:
        LOG(LOG_LVL_ERROR,"%d ext int cfg fail\n",pin_num);
    break;
    }    
    return idx;
}

void io_ext_int_cfg(uint8_t pin_num,ext_int_mode_t mode,void (*callback)())
{
    uint8_t idx = ext_int_idx_get(pin_num);    
    uint32_t ext_int_cfg = sysc_awo_ext_inrp_config_get();
    ext_int_cb[idx] = callback;
    ext_int_cfg &= ~(0x3<<(2*idx + 8));
    ext_int_cfg |=  mode<<(2*idx + 8);
    sysc_awo_ext_inrp_config_set(ext_int_cfg);
}

void io_ext_int_en(uint8_t pin_num,bool enable)
{
    uint8_t idx = ext_int_idx_get(pin_num);  
    uint32_t ext_int_cfg = sysc_awo_ext_inrp_config_get();
    if(enable)
    {
        sysctrl_io_config(pin_num,EXTERNAL_INTR_IO);
        ext_int_cfg |= 1<<idx | 1<<(idx+20);
    }else
    {
        sysctrl_io_config(pin_num,0);
        ext_int_cfg &= ~(1<<idx | 1<<(idx+20));
    }
    sysc_awo_ext_inrp_config_set(ext_int_cfg);
}

void io_ext_int_pin_en(uint8_t pin_num,bool enable)
{
    uint8_t idx = ext_int_idx_get(pin_num);  
    uint32_t ext_int_cfg = sysc_awo_ext_inrp_config_get();
    if(enable)
    {
        sysctrl_io_config(pin_num,EXTERNAL_INTR_IO);
        ext_int_cfg |= 1<<(idx+20);
    }else
    {
        sysctrl_io_config(pin_num,0);
        ext_int_cfg &= ~(1<<(idx+20));
    }
    sysc_awo_ext_inrp_config_set(ext_int_cfg);
}

void io_ext_int_mask(uint8_t pin_num,bool unmask)
{
    uint8_t idx = ext_int_idx_get(pin_num);  
    uint32_t ext_int_cfg = sysc_awo_ext_inrp_config_get();
    if(unmask)
    {
        ext_int_cfg |= 1<<idx;
    }else
    {
        ext_int_cfg &= ~(1<<idx);
    }
    sysc_awo_ext_inrp_config_set(ext_int_cfg);
}

void EXT_INTR_IRQHandler()
{
    uint8_t ext_int_stat = sysc_awo_ext_inrp_value_r_getf();
    uint8_t i;
    for(i=0;i<5;++i)
    {
        if(1<<i & ext_int_stat)
        {
            sysc_awo_ext_inrp_clr_set(1<<i);
            ext_int_cb[i]();
        }
    }
    if(ext_int_stat & 1<<5)
    {
        sysc_awo_ext_inrp_clr_set(1<<5);
        touch_cb();
    }
}
