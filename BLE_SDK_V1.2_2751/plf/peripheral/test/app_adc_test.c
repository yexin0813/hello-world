/*
 * app_adc_test.c
 *
 *  Created on: 2018Äê6ÔÂ28ÈÕ
 *      Author: jiachuang
 */

#include "app_adc.h"
#include "string.h"
#include "plf.h"
#include "clk_gate.h"// use 32K RC calibration result as seed
#include "LOG.h"


#define ADC_CHANNEL_SUM_CNT     6
#define ADC_PER_CHANNEL_SAMPLE  100
uint16_t adc_all_buff[ADC_CHANNEL_SUM_CNT][ADC_PER_CHANNEL_SAMPLE];
uint16_t error_rate[ADC_CHANNEL_SUM_CNT + 1];
volatile uint16_t adc_test_all_dma_ok = 0;

#define ADC_TEST_GOAL   430
#define ADC_TEST_TOL    50
#define OUT_RANGE(val) ((val < (ADC_TEST_GOAL-ADC_TEST_TOL)) || (val > (ADC_TEST_GOAL+ADC_TEST_TOL)))

#define ADC_TEST_LDO_FORCE_ON   1
#define ADC_TEST_LDO_DELAY      8

uint32_t test_all_dma_sample_rate[3] = {1000,30000,100000};
uint32_t test_all_delay_rate[3]      = {900 ,30   ,2};


app_adc_inst_t adc = ADC_INSTANCE(0);





uint8_t adc_test_verify(void)
{
    uint32_t ch = 0;
    uint32_t times = 0;
    uint16_t err_sum=0;
    //calc errors
    memset(error_rate,0,sizeof(error_rate));
    for(times = 0 ; times <  ADC_PER_CHANNEL_SAMPLE; times ++)
    {
        for(ch = 0 ; ch <  ADC_CHANNEL_SUM_CNT; ch ++)
        {
            if(OUT_RANGE(adc_all_buff[ch][times]))
            {
                error_rate[ch]++;
            }
        }
    }
    //calc sum error
    for(ch = 0 ; ch < ADC_CHANNEL_SUM_CNT ; ch ++) 
    {
        err_sum += error_rate[ch];
    }
    error_rate[ADC_CHANNEL_SUM_CNT] = (err_sum + ADC_CHANNEL_SUM_CNT - 1) / ADC_CHANNEL_SUM_CNT;
    return err_sum;

}


uint8_t adc_test_without_dma(app_adc_inst_t *inst , uint8_t sample_rate_index)
{
    uint32_t ch = 0;
    uint32_t times = 0;
    //init
    adc.param.ldo_force_on = ADC_TEST_LDO_FORCE_ON;
    adc.param.ldo_delay_us = ADC_TEST_LDO_DELAY;
    adc.param.use_dma = 0;
    app_adc_init(&inst->inst);
    //sample
    memset(adc_all_buff,0,sizeof(adc_all_buff));
    for(times = 0 ; times < ADC_PER_CHANNEL_SAMPLE ; times ++)
    {
        for(ch = 0 ; ch < ADC_CHANNEL_SUM_CNT ; ch ++)
        {
            adc_all_buff[ch][times] = app_adc_read_without_dma(&inst->inst , ch);
            BX_DELAY_US(test_all_delay_rate[sample_rate_index]);
        }
    }
    //uninit
    app_adc_uninit(&inst->inst);
    return adc_test_verify();
}



void app_adc_rx_finish(void* ptr , uint8_t dummy)
{
	adc_test_all_dma_ok = 1;
}



uint8_t adc_test_with_dma(app_adc_inst_t *inst , uint8_t sample_rate_index)
{
    uint32_t ch = 0;
    adc.param.ldo_force_on = ADC_TEST_LDO_FORCE_ON;
    adc.param.ldo_delay_us = ADC_TEST_LDO_DELAY;
    adc.param.use_dma = 1;
    //sample
    memset(adc_all_buff,0,sizeof(adc_all_buff));
    for(ch = 0 ; ch < ADC_CHANNEL_SUM_CNT ; ch ++)
    {
        BX_DELAY_US(100);
        adc_test_all_dma_ok = 0;
        adc.param.dma_bufptr = adc_all_buff[ch];
        adc.param.dma_size = ADC_PER_CHANNEL_SAMPLE;
        adc.param.dma_delay = APB_CLK / test_all_dma_sample_rate[sample_rate_index];
        //init
        app_adc_init(&inst->inst);
        app_adc_read_with_dma(&inst->inst , ch , app_adc_rx_finish , 0);
        while(adc_test_all_dma_ok == 0);
        //uninit
        app_adc_uninit(&inst->inst);
    }    
    return adc_test_verify();
}




void app_adc_test(void)
{
    uint8_t i=0;
    uint8_t result = 0;
    BX_DELAY_US(100000);//wait a moment
    while(*(__IO uint32_t *)0 != 0)
    {
        for(i=0;i<3;i++)
        {
            //no dma
            result = adc_test_without_dma(&adc , i);
            if(result > 0) LOG(3,"ADC NODMA ERROR! -- speed=%d\n",i);
            else           LOG(3,"ADC NODMA OK!    -- speed=%d\n",i);
            //use dma
            result = adc_test_with_dma(&adc , i);
            if(result > 0) LOG(3,"ADC DMA   ERROR! -- speed=%d\n",i);
            else           LOG(3,"ADC DMA   OK!    -- speed=%d\n",i);
        }
    }

}

