#ifndef __SENSOR_UTILS_H__
#define __SENSOR_UTILS_H__

#include "mafp_defs.h"

extern int32_t sensor_get_grey(uint8_t* buf, uint32_t len);
extern int32_t sensor_bkg_check_num(uint8_t* buf, uint32_t len);
extern int32_t sensor_bkg_check(uint8_t* buf, uint32_t len, uint8_t grey_val, uint32_t th);
extern uint8_t sensor_calc_greyoffset(uint8_t *buf, uint32_t len, uint8_t default_offset);
extern int32_t sensor_is_exposed(uint8_t *buf, uint32_t len, uint8_t greyoffset);

#endif
