/*
 * charger.h
 *
 *  Created on: 2018Äê8ÔÂ6ÈÕ
 *      Author: mingzhou
 */

#ifndef CHARGER_H_
#define CHARGER_H_

#include <stdint.h>

#define CHARGER_PIN 22 // GPIO22 for charger plug-in wakeup

enum
{
    Charger_Init,
    Charger_Uinit
};

enum
{
    BX_Charger_Cur_5mA = 0,
    BX_Charger_Cur_10mA,
    BX_Charger_Cur_30mA,
    BX_Charger_Cur_45mA,
    BX_Charger_Cur_60mA,
    BX_Charger_Cur_90mA, //5
    BX_Charger_Cur_120mA,
    BX_Charger_Cur_150mA,
    BX_Charger_Cur_180mA,
    BX_Charger_Cur_210mA,
    BX_Charger_Cur_270mA, // 10
};

/**
 * @brief Change charger system status.
 * @param[in] inst. Always NULL.
 * @param[in] sys_stat. System status.
 */
static void charger_sys_stat(void *inst,uint32_t sys_stat);
/**
 * @brief Initialize & enable charger.
 */
void charger_init(void);
/**
 * @brief Un-initialize & disable charger.
 */
void charger_uninit(void);
/**
 * @brief Set charger current.
 * @param[in] cur. Current.
 */
void charger_setCur(uint8_t cur);
/**
 * @brief Charger interrupt handler.
 */
void charger_isr(void);
/**
 * @brief Configure charger.
 * @param[in] cur. Current.
 * @param[in] callback. User callback function.
 */
void charger_config(uint8_t cur, void (*callback)());
#endif /* CHARGER_INTEGRATION_H_ */
