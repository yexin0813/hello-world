/** @addtogroup DMAC
 *    @ingroup PERIPHERAL
 *  @{
 */
#ifndef APP_DMAC_H_
#define APP_DMAC_H_
#include <stdint.h>
#include "periph_common.h"
#include "_reg_base_addr.h"
#include "reg_dmac.h"
#include "co_utils.h"
#include "dmac_integration.h"
#include "dmac_typedef.h"
enum dmac_sys_stat_config
{
    DMAC_INIT,
    DMAC_UNINIT,
    DMAC_TRANSFER_START,
    DMAC_TRANSFER_DONE,
};

typedef struct
{
    dmac_ch_env_t *ch;
    uint8_t ch_num;
    uint8_t ch_stat;
}dmac_env_t;

/**@brief DMAC transfer parameters structure */
typedef struct
{
    void (*callback)(void *); /**< callback function pointer.  */
    void *callback_param;   /**< parameter pass to callback function.  */
    uint8_t *src;                       /**< source address.  */
    uint8_t *dst;                      /**< destination address.  */
    uint32_t length;               /**< transfer length,count by src_tr_width.  */
    uint32_t src_tr_width:3,/**< source transfer width.  */
            dst_tr_width:3,         /**< destination transfer width.  */
            src_msize:3,               /**< source burst size.  */
            dst_msize:3,              /**< destination burst size.  */
            tt_fc:3,                        /**< transfer type and flow control.  */
            src_per:4,                  /**< source hardware handshake. */
            dst_per:4,                  /**< destination hardware handshake. */
            int_en:1;                   /**< interrupt enable. */
}app_dmac_transfer_param_t;

/**@brief DMAC instance structure */
typedef struct
{
    //public:
    periph_inst_t inst; /**< Peripheral common instance for DMAC.  */
    //private:
    dmac_env_t env; /**< DMAC operating environment.  */
    reg_dmac_t *reg; /**< DMAC register base address.  */
    uint8_t idx; /**< DMAC instance index.  */
}app_dmac_inst_t;

/**@brief macro for DMAC instantiation. */
#define DMAC_INSTANCE(id) \
{   \
            .inst = {.init_func = app_dmac_init,},\
            .reg = (reg_dmac_t *)REG_DMAC##id##_BASE,\
            .env = {.ch = dmac_ch_env, .ch_num =DMAC_CHANNEL_NUM,},\
}

/**
 * @brief Initialize APP_DMAC module
 * @param[in] hdl the peripheral common instance handler(pointer) for the DMAC instance
 */
void app_dmac_init(periph_inst_handle_t hdl);

/**
 * @brief De initialize APP_DMAC module
 * @param[in] hdl the peripheral common instance handler(pointer) for the DMAC instance
 */
void app_dmac_uninit(periph_inst_handle_t hdl);

/**
 * @brief Start a dma transfer
 * @param[in] hdl the peripheral common instance handler(pointer) for the DMAC instance
 * @param[in] param parameter for the transfer
 * @return channel index used in this transfer
 */
uint8_t app_dmac_start(periph_inst_handle_t hdl,app_dmac_transfer_param_t *param);

/**
 * @brief Cancel an ongoing dma trasfer
 * @param[in] hdl the peripheral common instance handler(pointer) for the DMAC instance
 * @param[in] ch_idx channel index to cancel
 * @return remaining length that the dma hasn't transferred
 */
uint32_t app_dmac_transfer_cancel(periph_inst_handle_t hdl,uint8_t ch_idx);

/**
 * @brief Wait for a dma transfer done (can only be applied to non-interrupt transfer)
 * @param[in] hdl the peripheral common instance handler(pointer) for the DMAC instance
 * @param[in] ch_idx channel index for which to wait
 */
void app_dmac_transfer_wait(periph_inst_handle_t hdl,uint8_t ch_idx);

/**
 * @brief Get enumeration of burst transaction size
 * @param[in] msize Burst Size in bytes
 */
uint8_t dmac_get_burst_transaction_size_enum(uint16_t msize);

void app_dmac_isr(app_dmac_inst_t *inst);


#endif
/** @}*/

