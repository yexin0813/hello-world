#include "app_spi.h"
#include "apollo_00.h"
#include "log.h"
#include "reg_sysc_cpu.h"
#include "periph_mngt.h"
#include "clk_gate.h"
#include "rst_gen.h"
#include "sysctrl.h"
#define SPIM0_FIFO_DEPTH 32
#define SPIM1_FIFO_DEPTH 32
#define SPIS_FIFO_DEPTH 32
static app_spi_inst_t *spim_inst_env[2];
static app_spi_inst_t *spis_inst_env;
static void spi_intr_op(app_spi_inst_t *inst,intr_operation_t op)
{
    IRQn_Type irq;
    if(inst->slave)
    {
        irq = SPIS_IRQn;
        spis_inst_env = inst;
    }else
    {
        irq = inst->idx ? SPIM1_IRQn : SPIM0_IRQn;
        spim_inst_env[inst->idx] = inst;
    }
    if(op == INTR_ENABLE)
    {
        NVIC_EnableIRQ(irq);
    }else if(op == INTR_DISABLE)
    {
        NVIC_DisableIRQ(irq);
    }else if(op == INTR_CLR)
    {
        NVIC_ClearPendingIRQ(irq);
    }else
    {
        BX_ASSERT(0);
    }
}

static void spi_sw_rst(app_spi_inst_t * inst)
{
    uint32_t rst_mask;
    if(inst->slave)
    {
       rst_mask = SPIS_SRST_PER;
    }else
    {
       rst_mask = inst->idx ? SPIM1_SRST_PER : SPIM0_SRST_PER;
    }
    srst_per(rst_mask);
}

static void spi_clk_gate(app_spi_inst_t * inst,clk_gate_operation_t op)
{
    uint8_t idx = inst->idx;
    if(op == SET_CLK )
    {
        if(inst->slave)
        {
            clk_gate_per_g0(PER_CLKG0_SET_SPIS|PER_CLKG1_SET_SPIS);
        }else
        {
            if(idx)
            {
                clk_gate_per_g0(PER_CLKG0_SET_SPIM1|PER_CLKG1_SET_SPIM1);
            }else
            {
                clk_gate_per_g0(PER_CLKG0_SET_SPIM0|PER_CLKG1_SET_SPIM0);
            }
        }
    }else
    {
        if(inst->slave)
        {
            clk_gate_per_g0(PER_CLKG0_CLR_SPIS|PER_CLKG1_CLR_SPIS);
        }else
        {
            if(idx)
            {
                clk_gate_per_g0(PER_CLKG0_CLR_SPIM1|PER_CLKG1_CLR_SPIM1);
            }else
            {
                clk_gate_per_g0(PER_CLKG0_CLR_SPIM0|PER_CLKG1_CLR_SPIM0);
            }
        }
    }   
}

static void spi_pin_cfg(app_spi_inst_t * inst,uint8_t pin_num,uint32_t pin_role)
{
    uint8_t idx = inst->idx;
    switch(pin_role)
    {
    case SPIM_CLK_PIN:
        sysctrl_io_config(pin_num, SPIM_CLK);
        if(idx)
        {
            sysc_cpu_spim1_en_setf(1);
        }else
        {
            sysc_cpu_spim0_en_setf(1);
        }
    break;
    case SPIS_CLK_PIN:
        sysctrl_io_config(pin_num,SPIS_CLK);
        sysc_cpu_spis_en_setf(1);
    break;
    case MOSI_PIN:
        if(inst->slave)
        {
            sysctrl_io_config(pin_num, SPIS_MOSI);
            sysc_cpu_spis_en_setf(1);
        }else
        {
            sysctrl_io_config(pin_num, SPIM_MOSI);
            if(idx)
            {
                sysc_cpu_spim1_en_setf(1);
            }else
            {
                sysc_cpu_spim0_en_setf(1);
            }
        }
    break;
    case MISO_PIN:
        if(inst->slave)
        {
            sysctrl_io_config(pin_num, SPIS_MISO);
            sysc_cpu_spis_en_setf(1);
        }else
        {
            sysctrl_io_config(pin_num, SPIM_MISO);
            if(idx)
            {
                sysc_cpu_spim1_en_setf(1);
            }else
            {
                sysc_cpu_spim0_en_setf(1);
            }
        }
    break;
    case SPIS_CS_PIN:
        sysctrl_io_config(pin_num,SPIS_CS);
        sysc_cpu_spis_en_setf(1);
    break;
    case SPIM_CS0_PIN:
        sysctrl_io_config(pin_num, SPIM_CS);
        if(idx)
        {
            sysc_cpu_spim1_en_setf(1);
        }else
        {
            sysc_cpu_spim0_en_setf(1);
        }
    break;
    case SPIM_CS1_PIN:
        sysctrl_io_config(pin_num, SPIM_CS);
        if(idx)
        {
            sysc_cpu_spim1_cs1_en_setf(1);
        }else
        {
            sysc_cpu_spim0_cs1_en_setf(1);
        }  
    break;
    default:
        BX_ASSERT(0);
    break;
    }    
}

static void spi_sys_stat(app_spi_inst_t *inst,uint32_t sys_stat)
{
    uint8_t idx = inst->idx;
    switch(sys_stat)
    {
    case SPI_INIT:
        recovery_list_add(&periph_domain_recovery_mngt, &inst->inst);
    break;
    case SPI_UNINIT:
        recovery_list_remove(&periph_domain_recovery_mngt,&inst->inst);
    break;
    case SPI_TX_START:
    case SPI_RX_START:
    case SPI_TXRX_START:
        if(inst->slave)
        {
           periph_domain_stat.spis = 1; 
        }else
        {
            if(idx)
            {
                periph_domain_stat.spim1 = 1;
            }else
            {
                periph_domain_stat.spim0 = 1;
            }           
        }
    break;
    case SPI_TX_DONE:
    case SPI_RX_DONE:
    case SPI_TXRX_DONE:
        if(inst->slave)
        {
           periph_domain_stat.spis = 0; 
        }else
        {
            if(idx)
            {
                periph_domain_stat.spim1 = 0;
            }else
            {
                periph_domain_stat.spim0 = 0;
            }           
        }
    break;
    default:
        LOG(LOG_LVL_WARN,"unexpected sys stat: %d\n",sys_stat);
    break;
    }
}

const periph_universal_func_set_t spi_universal_func = {
    .intr_op_func = (intr_op_func_t)spi_intr_op,
    .sw_rst_func = (sw_rst_func_t)spi_sw_rst,
    .clk_src_cfg_func = NULL,
    .clk_gate_func = (clk_gate_func_t)spi_clk_gate,
    .pin_cfg_func = (pin_cfg_func_t)spi_pin_cfg,
    .sys_stat_func = (sys_stat_func_t)spi_sys_stat,
};

uint8_t spi_get_fifo_depth(app_spi_inst_t *inst)
{
    uint8_t fifo_depth;
    if(inst->slave)
    {
        fifo_depth = SPIS_FIFO_DEPTH;
    }else
    {
        fifo_depth = inst->idx ? SPIM1_FIFO_DEPTH : SPIM0_FIFO_DEPTH;
    }
    return fifo_depth;
}

void SPIM0_IRQHandler()
{
    app_spi_isr(spim_inst_env[0]);
}

void SPIM1_IRQHandler()
{
    app_spi_isr(spim_inst_env[1]);
}

void SPIS_IRQHandler()
{
    app_spi_isr(spis_inst_env);
}
