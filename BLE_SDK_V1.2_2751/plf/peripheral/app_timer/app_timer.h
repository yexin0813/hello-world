/** @addtogroup TIMER
 *  @ingroup PERIPHERAL
 *  @brief TIMER driver
 *  @{
 */
/*
 * app_timer.h
 *
 *  Created on: 2018Äê6ÔÂ26ÈÕ
 *      Author: jiachuang
 */

#ifndef PLF_PERIPHERAL_APP_TIMER_APP_TIMER_H_
#define PLF_PERIPHERAL_APP_TIMER_APP_TIMER_H_
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>
#include <stdbool.h>
#include "periph_common.h"
#include "timer_integration.h"
#include "_reg_base_addr.h"
#include "reg_timer.h"
#include "bx_config.h"
#include "field_manipulate.h"


/*
 * DEFINES
 ****************************************************************************************
 */
/// This is all channel timer tick clock dividor                <p>
/// timer tick clock = 32MHz/(ALL_CHANNEL_TIMER_CLK_DIV+1)      <p>
/// This value range from 0 to 0xFF
#define ALL_CHANNEL_TIMER_CLK_DIV     15



/*
 * ENUMERATIONS
 ****************************************************************************************
 */
/// Internal system state , user don't care.
enum timer_sys_stat_config
{
    TIMER_INIT,
    TIMER_UNINIT,
    TIMER_START,
    TIMER_STOP,
};



/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/**@brief TIMER parameters structure  */
typedef struct
{
    uint32_t    auto_reload_value;              /**< auto reloat value , 24-bit. */
    void        (*callback)(void*);             /**< timer interrupt callback function */
    void        *callback_param;                /**< timer interrupt callback parameter */
}app_timer_comm_params_t;

/**@brief TIMER instance structure */
typedef struct
{
    //public:
    periph_inst_t inst;                 /**< Peripheral common instance for TIMER.  */
    app_timer_comm_params_t param;      /**< TIMER initialization parameters.  */
    //private:
    reg_timer_t *reg;                   /**< TIMER register base address.  */
    uint8_t idx;                        /**< TIMER instance index.  */
}app_timer_inst_t;



/*
 * MACROS
 ****************************************************************************************
 */
/**@brief macro for TIMER instantiation. */
#define TIMER_INSTANCE(id)  \
        {\
            .inst = {.init_func = app_timer_init,},\
            .idx = (id),\
            .reg = (reg_timer_t *)REG_TIMER_BASE,\
        }

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */


void app_timer_isr(app_timer_inst_t *inst);


/**
 * @brief Initialize APP_TIMER module
 * @param[in] hdl the peripheral common instance handler(pointer) for the TIMER instance
 */
void app_timer_init(periph_inst_handle_t hdl);


/**
 * @brief De initialize APP_TIMER module
 * @param[in] hdl the peripheral common instance handler(pointer) for the TIMER instance
 */
void app_timer_uninit(periph_inst_handle_t hdl);


/**
 * @brief Start APP_TIMER module
 * @param[in] hdl the peripheral common instance handler(pointer) for the TIMER instance
 */
void app_timer_start(periph_inst_handle_t hdl);


/**
 * @brief Stop APP_TIMER module
 * @param[in] hdl the peripheral common instance handler(pointer) for the TIMER instance
 */
void app_timer_stop(periph_inst_handle_t hdl);

/**
 * @brief Get current value
 * @param[in] hdl the peripheral common instance handler(pointer) for the TIMER instance
 *
 * @return[out] Current value of timer
 */
uint32_t app_timer_getValue(periph_inst_handle_t hdl);


/** @}*/
#endif /* PLF_PERIPHERAL_APP_TIMER_APP_TIMER_H_ */
