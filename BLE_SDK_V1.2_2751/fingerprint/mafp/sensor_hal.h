#ifndef __SENSOR_HAL_H__
#define __SENSOR_HAL_H__

#include "mafp_defs.h"

enum sensor_hw_mode
{
	POWERDOWN_MODE,
	DETECT_MODE,
	CAPTURE_MODE,
	RESET_MODE,
};


#define SENSOR_MODE_SET_RETRY_TIME      10


extern int32_t sensor_hw_get_register(uint8_t reg, uint8_t *val);
extern int32_t sensor_hw_set_register(uint8_t reg, uint8_t val);
extern int32_t sensor_hw_get_chipid(uint8_t *id);
extern int32_t sensor_hw_read_line(uint8_t *line, uint8_t size);
extern int32_t sensor_hw_read_image(uint8_t *img, int size);
extern int32_t sensor_hw_set_mode(uint8_t mode);

#endif

