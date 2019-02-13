/** @addtogroup HWECC_WRAPPER
 *    @ingroup HWECC
 *  @{
 */

#ifndef APP_HWECC_WRAPPER_H_
#define APP_HWECC_WRAPPER_H_
#include "app_hwecc.h"

/**
 * @brief Initialize HWECC
 */
void app_hwecc_init_wrapper(void);

/**
 * @brief De initialize HWECC
 */
void app_hwecc_uninit_wrapper(void);

/**
 * @brief Start ECC Calculation Using HWECC
 * @param[in] param parameters for ecc calculation
 */
void app_hwecc_calculate_wrapper(ecc_queue_t *param);

#endif
/** @}*/

