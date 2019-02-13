#ifndef __SENSOR_IDENTIFY_H__
#define __SENSOR_IDENTIFY_H__

#include "mafp_defs.h"

typedef struct current_line_identify {
	unsigned char start_line;
	unsigned char end_line;
	unsigned char line_step;
	unsigned char current_line;
}current_line_identify_t;

typedef struct image_mark_identify {
	unsigned char start_line;
	unsigned char end_line;
	unsigned char line_step;
	unsigned char current_line;
	unsigned char gray_disable;
}image_mark_identify_t;

/*格雷码验证
 * @seed随机数种子
 */
extern uint32_t fp121_gray_identify(int seed);

/*当前行验证
 * @seed随机数种子
 */
extern uint32_t fp121_current_line_identify(int seed);

/*图像mark验证
 * @seed随机数种子
 */
extern uint32_t fp121_image_mark_identify(int seed);


#endif 
