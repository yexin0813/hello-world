/** @addtogroup DMAC_WRAPPER
 *    @ingroup DMAC
 *  @{
 */
#ifndef APP_DMAC_WRAPPER_H_
#define APP_DMAC_WRAPPER_H_
#include "app_dmac.h"

/** @brief Initialize DMAC */
void app_dmac_init_wrapper(void);

/** @brief De initialize DMAC */
void app_dmac_uninit_wrapper(void);

/**
 * @brief Start a dma transfer
 * @param[in] param parameter for the transfer
 * @return channel index used in this transfer
 */
uint8_t app_dmac_start_wrapper(app_dmac_transfer_param_t *param);

/**
 * @brief Cancel an ongoing dma trasfer
 * @param[in] ch_idx channel index to cancel
 * @return remaining length that the dma hasn't transferred
 */
uint32_t app_dmac_transfer_cancel_wrapper(uint8_t ch_idx);

/**
 * @brief Wait for a dma transfer done (can only be applied to non-interrupt transfer)
 * @param[in] ch_idx channel index for which to wait
 */
void app_dmac_transfer_wait_wrapper(uint8_t ch_idx);

#endif
/** @}*/

