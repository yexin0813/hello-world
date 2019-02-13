/** @addtogroup HWECC
 *    @ingroup PERIPHERAL
 *  @{
 */

#ifndef APP_HWECC_H_
#define APP_HWECC_H_
#include <stdint.h>
#include <stdbool.h>

#include "periph_common.h"
#include "_reg_base_addr.h"
#include "reg_ecc.h"
#include "hwecc_integration.h"
#include "bx_ring_queue.h"
#define ECC_BLOCK_SIZE 32


enum hwecc_sys_stat_config
{
    HWECC_INIT,
    HWECC_UNINIT,
    HWECC_START,
    HWECC_DONE,
    
};

typedef struct {
    uint8_t content[ECC_BLOCK_SIZE]; 
}ecc_ram_field_t;
typedef struct {
    ecc_ram_field_t block[16];
}ecc_ram_t;

typedef struct {
    uint8_t const *secret_key;
    uint8_t const *public_key[2];
}ecc_in_t;

typedef struct {
    uint8_t *key[2];
}ecc_out_t;

typedef void (*ecc_cb_t)(void *);

/**@brief ecc calculation parameters */
typedef struct{
    ecc_in_t in;    /**< input data for ecc.  */
    ecc_out_t out;  /**< ecc output buffer.  */
    ecc_cb_t cb;    /**< callback function pointer.  */
    void *dummy; /**< callback parameter.  */
}ecc_queue_t;

typedef struct
{
    ring_queue_t *rq;
    bool busy;
}app_hwecc_env_t;

/**@brief HWECC instance structure */
typedef struct
{
    periph_inst_t inst; /**< Peripheral common instance for HWECC.  */
    app_hwecc_env_t env; /**< HWECC operating environment.  */
    reg_ecc_t *reg;            /**< HWECC register base address.  */
}app_hwecc_inst_t; 

/**@brief macro for HWECC instantiation. */
#define HWECC_INSTANCE(id)\
{\
    .inst = {.init_func = app_hwecc_init,},\
    .reg = (reg_ecc_t *)REG_ECC##id##_BASE,\
    .env = {.rq = &ecc_q,.busy = false},\
}

/**
 * @brief Initialize APP_HWECC module
 * @param[in] hdl the peripheral common instance handler(pointer) for the HWECC instance
 */
void app_hwecc_init(periph_inst_handle_t hdl);

/**
 * @brief De initialize APP_HWECC module
 * @param[in] hdl the peripheral common instance handler(pointer) for the HWECC instance
 */
void app_hwecc_uninit(periph_inst_handle_t hdl);

/**
 * @brief Start ECC Calculation Using HWECC
 * @param[in] hdl the peripheral common instance handler(pointer) for the HWECC instance
 * @param[in] param parameters for ecc calculation
 */
void app_hwecc_calculate(periph_inst_handle_t hdl,ecc_queue_t *param);

void app_hwecc_isr(app_hwecc_inst_t *inst);

#endif
/** @}*/

