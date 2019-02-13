/** @addtogroup ADC
 *  @ingroup PERIPHERAL
 *  @brief ADC driver
 *  @{
 */
/*
 * app_adc.h
 *
 *  Created on: 2018Äê6ÔÂ26ÈÕ
 *      Author: jiachuang
 */
#ifndef PLF_PERIPHERAL_APP_ADC_APP_ADC_H_
#define PLF_PERIPHERAL_APP_ADC_APP_ADC_H_
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>
#include <stdbool.h>
#include "periph_common.h"
#include "adc_integration.h"
#include "_reg_base_addr.h"
#include "reg_adc.h"
#include "bx_config.h"
#include "field_manipulate.h"


/*
 * DEFINES
 ****************************************************************************************
 */
/// ADC have 6 channels max
#define ADC_CHANNEL_SUM_CNT 6

/*
 * ENUMERATIONS
 ****************************************************************************************
 */
/// Internal system state , user don't care.
enum adc_sys_stat_config
{
    ADC_INIT,
    ADC_UNINIT,
    ADC_READ_START,
    ADC_READ_DONE,
};


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/// ADC parameters
typedef struct
{
    /// LDO delay time (us)
    uint16_t  ldo_delay_us;
    /// LDO force on enable
    uint8_t   ldo_force_on;
    /// Use DMA operate
    uint8_t   use_dma;
    /// DAM transmit size
    uint16_t  dma_size;
    /// DMA buffer pointer
    uint16_t* dma_bufptr;
    /// DMA delay per sample.
    uint16_t  dma_delay;
}app_adc_comm_params_t;

/// ADC environment structure
struct adc_env_tag
{
    /// call back function pointer
    void (*callback) (void*,uint8_t);
    /// call back function parameter
    void *dummy;
};

/**@brief ADC instance structure */
typedef struct
{
    //public:
    periph_inst_t inst;             /**< Peripheral common instance for ADC.  */
    app_adc_comm_params_t param;    /**< ADC initialization parameters.  */
    //private:
    struct adc_env_tag      env;    /**< ADC initialization parameters.  */
    reg_adc_t *reg;                 /**< ADC register base address.  */
    uint8_t idx;                    /**< ADC instance index.  */
}app_adc_inst_t;



/*
 * MACROS
 ****************************************************************************************
 */
/**@brief macro for ADC instantiation. */
#define ADC_INSTANCE(id)  \
        {\
            .inst = {.init_func = app_adc_init,},\
            .idx = (id),\
            .reg = (reg_adc_t *)REG_ADC_CTRL_BASE,\
        }



/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */


/**
 * @brief Initialize APP_ADC module
 * @param[in] hdl the peripheral common instance handler(pointer) for the ADC instance
 */
void app_adc_init(periph_inst_handle_t hdl);


/**
 * @brief De initialize APP_ADC module
 * @param[in] hdl the peripheral common instance handler(pointer) for the ADC instance
 */
void app_adc_uninit(periph_inst_handle_t hdl);


/**
 * @brief To read ADC value without DMA.
 * @param[in] hdl the peripheral common instance handler(pointer) for the ADC instance
 * @param[in] channel Which adc channel will be read
 * @retval uint16_t The Data conversion value.
 */
uint16_t app_adc_read_without_dma(periph_inst_handle_t hdl , uint8_t channel);


/**
 * @brief To read ADC value with DMA.
 * @param[in] hdl the peripheral common instance handler(pointer) for the ADC instance
 * @param[in] channel Which adc channel will be read
 * @param[in] callback function pointer that will be called when all data has been read
 * @param[in] dummy parameter that will be pass to callback function
 */
void app_adc_read_with_dma(periph_inst_handle_t hdl , uint8_t channel , void (*callback) (void*,uint8_t),void* dummy);

/**
 * @brief ADC sample battery.
 * @return uint32_t The battery value.
 */
uint32_t app_adc_battery(void);

/**
 * @brief ADC sample temperature.
 * @return uint32_t The temperature value.
 */
uint32_t app_adc_tempSensor(void);

/**
 * @brief ADC sample gpadc input in single end mode.
 * @param[in] channel Which adc channel will be read
 * @return uint32_t The gpadc value.
 */
uint32_t app_adc_gpadc_single_end(uint8_t channel);

/**
 * @brief ADC sample gpadc input in differential mode.
 * @param[in] channel Which adc channel will be read
 * @return uint32_t The gpadc value.
 */
uint32_t app_adc_gpadc_differential(uint8_t channel);


/** @}*/
#endif /* PLF_PERIPHERAL_APP_ADC_APP_ADC_H_ */
