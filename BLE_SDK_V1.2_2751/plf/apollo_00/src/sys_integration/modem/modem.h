
/**
 ****************************************************************************************
 *
 * @file modem.h
 *
 * @brief Declaration of modem functions
 *
 * Copyright (C) Apollo
 *
 *
 ****************************************************************************************
 */

#ifndef MODEM_H
#define MODEM_H

#include "reg_ble_mdm.h"

/**
 ****************************************************************************************
 * @defgroup MODEM
 * @ingroup DRIVERS
 *
 * @{
 ****************************************************************************************
 */

//#define DEV_PATCH_V3
#ifdef DEV_PATCH_V3
void devi_rxtx_on_patch(void);
void devi_rxtx_off_patch(void);
void devi_minus_val(void);
#endif

/**
 * @brief Derivation calibration in main.
 */
void modem_dev_calib_in_main(void);

/**
 * @brief initialize the modem.
 */
void modem_init(void);
/**
 * @brief start vco calibration, the calibration should be finished before ble start work.
 * It takes about 7.5ms from vco calibration start to vco calibration finished. So modem_vcocali_start should be used as early as possible.
 */
void modem_vcocali_start(void);
/**
 * @brief wait vco calibration finish and save the result for future use.
 */
void modem_vcocali_value_save(void);        // put it in appropriate place 7.5ms after modem_vcocali_start

/// @}

#endif
