/** @addtogroup PWM
 *  @ingroup PERIPHERAL
 *  @brief PWM driver
 *  @{
 */
/*
 * app_pwm.h
 *
 *  Created on: 2018Äê6ÔÂ26ÈÕ
 *      Author: jiachuang
 */

#ifndef PLF_PERIPHERAL_APP_PWM_APP_PWM_H_
#define PLF_PERIPHERAL_APP_PWM_APP_PWM_H_
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>
#include <stdbool.h>
#include "periph_common.h"
#include "pwm_integration.h"
#include "_reg_base_addr.h"
#include "reg_pwm.h"
#include "bx_config.h"
#include "field_manipulate.h"

/*
 * DEFINES
 ****************************************************************************************
 */
/// This is all channel pwm tick clock dividor              <p>
/// PWM tick clock = 16MHz/(ALL_CHANNEL_PWM_CLK_DIV+1)      <p>
/// This value range from 0 to 0xFF
#define ALL_CHANNEL_PWM_CLK_DIV     1


/*
 * ENUMERATIONS
 ****************************************************************************************
 */
/// enable or disable a timer
enum pwm_pin_role_config
{
    /// enable the timer
    PWM_ENABLE,
    /// disable the timer
    PWM_DISABLE,
};

/// Internal system state , user don't care.
enum pwm_sys_stat_config
{
    PWM_INIT,
    PWM_UNINIT,
    PWM_OUTPUT_START,
    PWM_OUTPUT_STOP,
};



/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/**@brief PWM parameters structure  */
typedef struct
{
    uint8_t         pin_num;            /**< select which pin output the pwm. */
    uint16_t        high_time;          /**< high time of a period <p> base clock tick = 16MHz/(ALL_CHANNEL_PWM_CLK_DIV+1) */
    uint16_t        low_time;           /**< low  time of a period <p> base clock tick = 16MHz/(ALL_CHANNEL_PWM_CLK_DIV+1) */
}app_pwm_comm_params_t;


/**@brief PWM instance structure */
typedef struct
{
    //public:
    periph_inst_t inst;             /**< Peripheral common instance for PWM.  */
    app_pwm_comm_params_t param;    /**< PWM initialization parameters.  */
    //private:
    reg_pwm_t *reg;                 /**< PWM register base address.  */
    uint8_t idx;                    /**< PWM instance index.  */
}app_pwm_inst_t;



/*
 * MACROS
 ****************************************************************************************
 */
/**@brief macro for PWM instantiation. */
#define PWM_INSTANCE(id)  \
        {\
            .inst = {.init_func = app_pwm_init,},\
            .idx = (id),\
            .reg = (reg_pwm_t *)REG_PWM_BASE,\
        }


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 * @brief Initialize APP_PWM module
 * @param[in] hdl the peripheral common instance handler(pointer) for the PWM instance
 */
void app_pwm_init(periph_inst_handle_t hdl);


/**
 * @brief De initialize APP_PWM module
 * @param[in] hdl the peripheral common instance handler(pointer) for the PWM instance
 */
void app_pwm_uninit(periph_inst_handle_t hdl);


/**
 * @brief Start APP_PWM module
 * @param[in] hdl the peripheral common instance handler(pointer) for the PWM instance
 */
void app_pwm_start(periph_inst_handle_t hdl);


/**
 * @brief Stop APP_PWM module
 * @param[in] hdl the peripheral common instance handler(pointer) for the PWM instance
 */
void app_pwm_stop(periph_inst_handle_t hdl);


/** @}*/
#endif /* PLF_PERIPHERAL_APP_PWM_APP_PWM_H_ */
