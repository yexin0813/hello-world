#include "periph_common.h"
#include "app_iic.h"
#include "periph_mngt.h"
#include "bx_config.h"
#include "apollo_00.h"
#include "bx_dbg.h"
#include "rst_gen.h"
#include "clk_gate.h"
#include "reg_sysc_per.h"
#include "pshare.h"
#include "log.h"
#include "sysctrl.h"
#include "ll.h"


static app_iic_inst_t *iic_inst_env[2];

struct iic_hlcnt_rxtxhold_Config
{
    uint32_t lcnt;    // low level count
    uint32_t hcnt;    // high level count
    uint32_t rx_hold; // rx hold
    uint32_t tx_hold; // tx hold
};
static struct iic_hlcnt_rxtxhold_Config iic_hlcnt_array[IIC_HIGH_SPEED_MODE + 1] =
{
    {0,   0,   0, 0 }, //speed mode is 1-3,array[0] is no use.
    {160, 145, 3, 10}, // STANDARD_100K_32M
    {40,  30,  3, 6 }, // FAST_400K_32M
    {16,  8,   3, 3 }, // HIGH_1M_32M
};

static void iic_intr_op(app_iic_inst_t * inst,intr_operation_t op)
{
    IRQn_Type iic_irq = inst->idx ? IIC1_IRQn : IIC0_IRQn;
    if(op == INTR_ENABLE)
    {
        iic_inst_env[inst->idx] = inst;
        NVIC_EnableIRQ(iic_irq);
    }else if(op == INTR_DISABLE)
    {
        NVIC_DisableIRQ(iic_irq);
    }else if(op == INTR_CLR)
    {
        NVIC_ClearPendingIRQ(iic_irq);
    }else
    {
        BX_ASSERT(0);
    }
}

static void iic_sw_rst(app_iic_inst_t * inst)
{
    uint32_t rst_mask = inst->idx ? IIC1_SRST_PER : IIC0_SRST_PER;
    srst_per(rst_mask);
}

//parameter is speed mode
static void iic_clk_src_cfg(app_iic_inst_t * inst,uint32_t clk_cfg)
{
    uint8_t idx =inst->idx;
    reg_iic_t *reg = inst->reg;
    app_iic_speed_mode_t speed_mode = (app_iic_speed_mode_t)clk_cfg;
    
    //set clock source
    if(idx)
    {
        #if USE_PLL == 1
//            clk_src_iic1_FromPllPD_N4_PER(2);
            GLOBAL_INT_DISABLE();
            sysc_per_clkg0_set(1<<7);
            sysc_per_clk_div_iic1_para_m1_setf(2&0xf);
            sysc_per_clk_sel_iic1_setf(3);
            sysc_per_clkg0_set(1<<6);
            GLOBAL_INT_RESTORE();
        #else
//            clk_src_iic1_From16MPD_PER();
                GLOBAL_INT_DISABLE();
                sysc_per_clkg0_set(1<<5);
                sysc_per_clk_sel_iic1_setf(0);
                sysc_per_clkg0_set(1<<4);
                GLOBAL_INT_RESTORE();
            clk_gate_per_g1(PER_CLKG0_SET_IIC0);
        #endif
    }else
    {
        #if USE_PLL == 1
//            clk_src_iic0_FromPllPD_N4_PER(2);
                GLOBAL_INT_DISABLE();
                sysc_per_clkg0_set(1<<3);
                sysc_per_clk_div_iic0_para_m1_setf(2&0xf);
                sysc_per_clk_sel_iic0_setf(3);
                sysc_per_clkg0_set(1<<2);
                GLOBAL_INT_RESTORE();
        #else
//            clk_src_iic0_From16MPD_PER();
                GLOBAL_INT_DISABLE();
                sysc_per_clkg0_set(1<<1);
                sysc_per_clk_sel_iic0_setf(0);
                sysc_per_clkg0_set(1<<0);
                GLOBAL_INT_RESTORE();
            clk_gate_per_g0(PER_CLKG0_SET_IIC0);
        #endif
    }
    
    //set speed mode
    FIELD_WR(reg,IC_CON , IIC_SPEED , speed_mode);
    //set h/l cmt
    if(speed_mode == IIC_STANDARD_MODE)
    {
        FIELD_WR(reg , IC_SS_SCL_HCNT , IIC_SS_SCL_HCNT , iic_hlcnt_array[speed_mode].hcnt);
        FIELD_WR(reg , IC_SS_SCL_LCNT , IIC_SS_SCL_LCNT , iic_hlcnt_array[speed_mode].lcnt);
    }
    else if(speed_mode == IIC_FAST_MODE)
    {
        FIELD_WR(reg , IC_FS_SCL_HCNT , IIC_FS_SCL_HCNT , iic_hlcnt_array[speed_mode].hcnt);
        FIELD_WR(reg , IC_FS_SCL_LCNT , IIC_FS_SCL_LCNT , iic_hlcnt_array[speed_mode].lcnt);
    }
    else if(speed_mode == IIC_HIGH_SPEED_MODE)
    {
        FIELD_WR(reg , IC_HS_SCL_HCNT , IIC_HS_SCL_HCNT , iic_hlcnt_array[speed_mode].hcnt);
        FIELD_WR(reg , IC_HS_SCL_LCNT , IIC_HS_SCL_LCNT , iic_hlcnt_array[speed_mode].lcnt);
    }
    //set tx/rx hold
    FIELD_WR(reg , IC_SDA_HOLD , IIC_IC_SDA_RX_HOLD , iic_hlcnt_array[speed_mode].rx_hold);
    FIELD_WR(reg , IC_SDA_HOLD , IIC_IC_SDA_TX_HOLD , iic_hlcnt_array[speed_mode].tx_hold);
}

static void iic_clk_gate(app_iic_inst_t * inst,clk_gate_operation_t op)
{
    uint8_t idx = inst->idx;
    #if (defined(CFG_ON_CHIP)&&(CFG_ON_CHIP==1))
    if(op == SET_CLK )
    {
        if(idx)
        {
            clk_gate_per_g0(PER_CLKG0_SET_IIC1|PER_CLKG1_SET_IIC1);
        }else
        {
            clk_gate_per_g0(PER_CLKG0_SET_IIC0|PER_CLKG1_SET_IIC0);
        }
    }else
    {
        if(idx)
        {
            clk_gate_per_g0(PER_CLKG0_SET_IIC1|PER_CLKG1_SET_IIC1);
        }else
        {
            clk_gate_per_g0(PER_CLKG0_SET_IIC0|PER_CLKG1_SET_IIC0);
        }
    }   
    #endif
}

static void iic_pin_cfg(app_iic_inst_t * inst,uint8_t pin_num,uint32_t pin_role)
{
    uint8_t idx = inst->idx;
    switch(pin_role)
    {
    case IIC_SCL_PIN:
        sysctrl_io_config(pin_num, IIC_SCL);
        if(idx)
        {
            pshare_funcio_set(pin_num - 2,IO_IIC1_SCL, ENABLE);
        }else
        {
            pshare_funcio_set(pin_num - 2,IO_IIC0_SCL, ENABLE);
        }
        break;
    case IIC_SDA_PIN:
        sysctrl_io_config(pin_num, IIC_SDA);
        if(idx)
        {
            pshare_funcio_set(pin_num - 2,IO_IIC1_SDA, ENABLE);
        }else
        {
            pshare_funcio_set(pin_num - 2,IO_IIC0_SDA, ENABLE);
        }
        break;
    default:
        BX_ASSERT(0);
    break;
    }
    //set pull up
    hwp_sysc_awo->gpio_ps.val |= (1 << pin_num);//pull up select
    hwp_sysc_awo->gpio_pe.val |= (1 << pin_num);//pull up enable
}

static void iic_sys_stat(app_iic_inst_t *inst,uint32_t sys_stat)
{
    uint8_t idx = inst->idx;
    switch(sys_stat)
    {
    case IIC_INIT:
        recovery_list_add(&periph_domain_recovery_mngt, &inst->inst);
    break;
    case IIC_UNINIT:
        recovery_list_remove(&periph_domain_recovery_mngt,&inst->inst);
    break;
    case IIC_READ_START:
    case IIC_WRITE_START:
        if(idx)
        {
            periph_domain_stat.iic1 = 1;
        }else
        {
            periph_domain_stat.iic0 = 1;
        }
    break;
    case IIC_READ_DONE:
    case IIC_WRITE_DONE:
        if(idx)
        {
            periph_domain_stat.iic1 = 0;
        }else
        {
            periph_domain_stat.iic0 = 0;
        }
    break;
    default:
        LOG(LOG_LVL_WARN,"unexpected sys stat: %d\n",sys_stat);
    break;
    }
}

const periph_universal_func_set_t iic_universal_func = 
{
    .intr_op_func     = (intr_op_func_t    )iic_intr_op,
    .sw_rst_func      = (sw_rst_func_t     )iic_sw_rst,
    .clk_src_cfg_func = (clk_src_cfg_func_t)iic_clk_src_cfg,
    .clk_gate_func    = (clk_gate_func_t   )iic_clk_gate,
    .pin_cfg_func     = (pin_cfg_func_t    )iic_pin_cfg,
    .sys_stat_func    = (sys_stat_func_t   )iic_sys_stat,
};



void IIC0_IRQHandler(void)
{
    app_iic_isr(iic_inst_env[0]);
}

void IIC1_IRQHandler(void)
{
    app_iic_isr(iic_inst_env[1]);
}

