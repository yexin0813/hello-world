#include "sensor_reg.h"
#include "sensor_hal.h"
#include <string.h>
#include "platform.h"
#include "sensor_operation_afs121n.h"

int32_t sensor_hw_get_register(uint8_t reg, uint8_t *val)
{
	uint8_t old = 0, new_2 = 0, data = 0;

	platform_fp_rw_reg(reg, data, &old, &new_2);
	platform_fp_rw_reg(reg, old, &data, &new_2);

	if (new_2 != old) {
		LOGE("******get register failed******");
		return ERROR_SPI_READ;
	}
	*val = old;

	return 0;
}


int32_t sensor_hw_set_register(uint8_t reg, uint8_t val)
{
	uint8_t old, new_1;

	//LOGD(">>>>>> %x = %x", reg, val);
	platform_fp_rw_reg(reg, val, &old, &new_1);

	if ((new_1 != val) && (reg != MA_INTERRUPT_REG)) {
		LOGE("******set register %x=%x, %x, %x failed******", reg, val, old, new_1);
		return ERROR_SPI_WRITE;
	}
        
	return 0;
}


int32_t sensor_hw_get_chipid(uint8_t *id)
{
	int32_t ret = 0;
        
	LOGD("start.");

	if (sensor_hw_set_mode(RESET_MODE)) {
		LOGE("sensor_hw_set_mode failed.");
		return ERROR_MODE_SWITCH;
	}

	platform_msleep(8);
        
	ret = sensor_hw_get_register(MA_PRODU_CODE_2_REG, id);
	LOGE("sensor id is: %#x", *id);
        
	LOGD("end.");

	return ret;
}


#define ENUM_TO_STR(x) case x: return #x;
char* enum2str(int cmdnum)
{
	switch (cmdnum) {
		ENUM_TO_STR(POWERDOWN_MODE);
		ENUM_TO_STR(DETECT_MODE);
		ENUM_TO_STR(CAPTURE_MODE);
		ENUM_TO_STR(RESET_MODE);
	default:
		return "UNKNOW COMMAND";
	}
}


int32_t sensor_hw_set_mode(uint8_t mode)
{
	int32_t i, j, time = 0;
	uint8_t reg = 0, val = 0;

	LOGD("set to %s.", enum2str(mode));
        
	switch( mode )
	{
	case POWERDOWN_MODE:
		reg = MA_POWER_DOWN;
		break;
	case DETECT_MODE:
		reg = MA_DETECT_MODE;
		break;
	case CAPTURE_MODE:
		time = 3;
		reg = MA_CAPTURE_MODE;
		break;
	case RESET_MODE:
		time = 8;
		reg = MA_RESET_MODE;
		break;
	default:
		return ERROR_BAD_PARAMETERS;
	}

	for (i = 0; i < SENSOR_MODE_SET_RETRY_TIME; i++) {
		platform_fp_set_mode(reg);

		if (!time) break;
                
		platform_msleep(time);

		for (j = 0; j < SENSOR_MODE_SET_RETRY_TIME; j++) {
			sensor_hw_get_register(MA_PRODU_CODE_1_REG, &val);
			if ( val == 0x41 ) break;
		}

		if (j != SENSOR_MODE_SET_RETRY_TIME) break;
	}

	if (i == SENSOR_MODE_SET_RETRY_TIME) {
		LOGE("%s failed.", enum2str(mode));
		return ERROR_MODE_SWITCH;
	}

	return 0;
}


int32_t sensor_hw_read_image(uint8_t *img, int size)
{
	platform_fp_read_frame(img, size);
        
	return 0;
}


int32_t sensor_hw_read_line(uint8_t *line, uint8_t size)
{
	platform_fp_read_line(line, size);
        
	return 0;
}


