#include <string.h>
#include "log.h"
#include "app_spi.h"
#include "co_math.h"
app_spi_inst_t spim1_inst = SPIM_INSTANCE(1);
app_spi_inst_t spis_inst = SPIS_INSTANCE();
#define DATA_LENGTH 50
uint32_t spis_tx_data[DATA_LENGTH];
uint32_t spim_tx_data[DATA_LENGTH];
uint32_t spis_rx_buf[DATA_LENGTH];
uint32_t spim_rx_buf[DATA_LENGTH];
static void spis_transfer_cb(void *cb_param)
{
    if(memcmp(spim_tx_data,spis_rx_buf,sizeof(uint32_t)*DATA_LENGTH)==0)
    {
        LOG(LOG_LVL_INFO,"spis rx spim tx ok\n");
    }else
    {
        LOG(LOG_LVL_INFO,"spis rx spim tx fail\n");
    }
}

static void spim_transfer_cb(void *cb_param)
{
    if(memcmp(spis_tx_data,spim_rx_buf,sizeof(uint32_t)*DATA_LENGTH)==0)
    {
        LOG(LOG_LVL_INFO,"spim rx spis tx ok\n");
    }else
    {
        LOG(LOG_LVL_INFO,"spim rx spis tx fail\n");
    }
}

void spim1_init()
{
    spim1_inst.param.u.master.clk_div = 12;
    spim1_inst.param.dfs_32 = DFS_32_32_bits;
    spim1_inst.param.cpol = Inactive_Low;
    spim1_inst.param.cph = SCLK_Toggle_In_Middle;
    spim1_inst.param.clk_pin_no = 9;
    spim1_inst.param.mosi_pin_no = 11;
    spim1_inst.param.miso_pin_no = 10;
    spim1_inst.param.u.master.cs_pin_no[1] = 7;
    spim1_inst.param.u.master.hw_cs_mask = 0x2;
    app_spi_init(&spim1_inst.inst);
}

void spis_init()
{

    spis_inst.param.dfs_32 = DFS_32_32_bits;
    spis_inst.param.cpol = Inactive_Low;
    spis_inst.param.cph = SCLK_Toggle_In_Middle;
    spis_inst.param.clk_pin_no = 4;
    spis_inst.param.mosi_pin_no = 6;
    spis_inst.param.miso_pin_no = 5;
    spis_inst.param.u.slave.cs_pin_no = 3;
    app_spi_init(&spis_inst.inst);
}

void spim_spis_test()
{

    uint8_t i;
    for(i=0;i<DATA_LENGTH;++i)
    {
        spim_tx_data[i] = co_rand_word();
        spis_tx_data[i] = co_rand_word();
    }
//    app_spi_transmit(&spis_inst.inst,0,spis_tx_data,DATA_LENGTH,spis_transfer_cb, &spis_inst);
    app_spi_transmit_receive(&spis_inst.inst,0,spis_tx_data,spis_rx_buf,DATA_LENGTH,spis_transfer_cb,&spis_inst);
    app_spi_transmit_receive(&spim1_inst.inst,0x2,spim_tx_data,spim_rx_buf,DATA_LENGTH,spim_transfer_cb,&spim1_inst);


}
