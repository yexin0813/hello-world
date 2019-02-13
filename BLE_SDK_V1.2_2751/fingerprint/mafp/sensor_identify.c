/*
 * sensor_identify.c
 *
 *  Created on: Mar 15, 2017
 *      Author: zhl
 */
#include "sensor_identify.h"
#include "sensor_reg.h"
#include "sensor_hal.h"
#include "malib.h"
#include "platform.h"
#include <string.h>


/**
 **
 **  NOTE : For AFS121N ONLY
 **
 **/
 

#define sensor_get_register sensor_hw_get_register
#define sensor_set_register sensor_hw_set_register
#define sensor_read_line    sensor_hw_read_line

#define LINE_WIDTH  120

const unsigned char light_bin[LINE_WIDTH] = {
	0x4c, 0x4d, 0x4f, 0x4e, 0x4a, 0x4b, 0x49, 0x48, 0x58, 0x59,
	0x5b, 0x5a, 0x5e, 0x5f, 0x5d, 0x5c, 0x54, 0x55, 0x57, 0x56,
	0x52, 0x53, 0x51, 0x50, 0x70, 0x71, 0x73, 0x72, 0x76, 0x77,
	0x75, 0x74, 0x7c, 0x7d, 0x7f, 0x7e, 0x7a, 0x7b, 0x79, 0x78,
	0x68, 0x69, 0x6b, 0x6a, 0x6e, 0x6f, 0x6d, 0x6c, 0x64, 0x65,
	0x67, 0x66, 0x62, 0x63, 0x61, 0x60, 0x20, 0x21, 0x23, 0x22,
	0x26, 0x27, 0x25, 0x24, 0x2c, 0x2d, 0x2f, 0x2e, 0x2a, 0x2b,
	0x29, 0x28, 0x38, 0x39, 0x3b, 0x3a, 0x3e, 0x3f, 0x3d, 0x3c,
	0x34, 0x35, 0x37, 0x36, 0x32, 0x33, 0x31, 0x30, 0x10, 0x11,
	0x13, 0x12, 0x16, 0x17, 0x15, 0x14, 0x1c, 0x1d, 0x1f, 0x1e,
	0x1a, 0x1b , 0x19, 0x18, 0x8, 0x9, 0xb, 0xa, 0xe, 0xf,
	0xd, 0xc, 0x4, 0x5, 0x7, 0x6, 0x2, 0x3, 0x1, 0x0
};

const unsigned char dark_gray[LINE_WIDTH] = {
	0x5a, 0x5b, 0x59, 0x58, 0x5d, 0x5c, 0x5e, 0x5f, 0x4a, 0x4b,
	0x49, 0x48, 0x4d, 0x4c, 0x4e, 0x4f, 0x45, 0x44, 0x46, 0x47,
	0x42, 0x43, 0x41, 0x40, 0x6a, 0x6b, 0x69, 0x68, 0x6d, 0x6c,
	0x6e, 0x6f, 0x65, 0x64, 0x66, 0x67, 0x62, 0x63, 0x61, 0x60,
	0x75, 0x74, 0x76, 0x77, 0x72, 0x73, 0x71, 0x70, 0x7a, 0x7b,
	0x79, 0x78, 0x7d, 0x7c, 0x7e, 0x7f, 0x2a, 0x2b, 0x29, 0x28,
	0x2d, 0x2c, 0x2e, 0x2f, 0x25, 0x24, 0x26, 0x27, 0x22, 0x23,
	0x21, 0x20, 0x35, 0x34, 0x36, 0x37, 0x32, 0x33, 0x31, 0x30,
	0x3a, 0x3b, 0x39, 0x38, 0x3d, 0x3c, 0x3e, 0x3f, 0x15, 0x14,
	0x16, 0x17, 0x12, 0x13, 0x11, 0x10, 0x1a, 0x1b, 0x19, 0x18,
	0x1d, 0x1c, 0x1e, 0x1f, 0xa, 0xb, 0x9, 0x8, 0xd, 0xc,
	0xe, 0xf, 0x5, 0x4, 0x6, 0x7, 0x2, 0x3, 0x1, 0x0
};

const unsigned char dark_bin[LINE_WIDTH] = {
	0xcc, 0xcd, 0xcf, 0xce, 0xca, 0xcb, 0xc9, 0xc8, 0xd8, 0xd9,
	0xdb, 0xda, 0xde, 0xdf, 0xdd, 0xdc, 0xd4, 0xd5, 0xd7, 0xd6,
	0xd2, 0xd3, 0xd1, 0xd0, 0xf0, 0xf1, 0xf3, 0xf2, 0xf6, 0xf7,
	0xf5, 0xf4, 0xfc, 0xfd, 0xff, 0xfe, 0xfa, 0xfb, 0xf9, 0xf8,
	0xe8, 0xe9, 0xeb, 0xea, 0xee, 0xef, 0xed, 0xec, 0xe4, 0xe5,
	0xe7, 0xe6, 0xe2, 0xe3, 0xe1, 0xe0, 0xa0, 0xa1, 0xa3, 0xa2,
	0xa6, 0xa7, 0xa5, 0xa4, 0xac, 0xad, 0xaf, 0xae, 0xaa, 0xab,
	0xa9, 0xa8, 0xb8, 0xb9, 0xbb, 0xba, 0xbe, 0xbf, 0xbd, 0xbc,
	0xb4, 0xb5, 0xb7, 0xb6, 0xb2, 0xb3, 0xb1, 0xb0, 0x90, 0x91,
	0x93, 0x92, 0x96, 0x97, 0x95, 0x94, 0x9c, 0x9d, 0x9f, 0x9e,
	0x9a, 0x9b, 0x99, 0x98, 0x88, 0x89, 0x8b, 0x8a, 0x8e, 0x8f,
	0x8d, 0x8c, 0x84, 0x85, 0x87, 0x86, 0x82, 0x83, 0x81, 0x80
};

const unsigned char light_gray[LINE_WIDTH] = {
	0xf0, 0xf1, 0xf3, 0xf2, 0xf7, 0xf6, 0xf4, 0xf5, 0xe0, 0xe1,
	0xe3, 0xe2, 0xe7, 0xe6, 0xe4, 0xe5, 0xef, 0xee, 0xec, 0xed,
	0xe8, 0xe9, 0xeb, 0xea, 0xc0, 0xc1, 0xc3, 0xc2, 0xc7, 0xc6,
	0xc4, 0xc5, 0xcf, 0xce, 0xcc, 0xcd, 0xc8, 0xc9, 0xcb, 0xca,
	0xdf, 0xde, 0xdc, 0xdd, 0xd8, 0xd9, 0xdb, 0xda, 0xd0, 0xd1,
	0xd3, 0xd2, 0xd7, 0xd6, 0xd4, 0xd5, 0x80, 0x81, 0x83, 0x82,
	0x87, 0x86, 0x84, 0x85, 0x8f, 0x8e, 0x8c, 0x8d, 0x88, 0x89,
	0x8b, 0x8a, 0x9f, 0x9e, 0x9c, 0x9d, 0x98, 0x99, 0x9b, 0x9a,
	0x90, 0x91, 0x93, 0x92, 0x97, 0x96, 0x94, 0x95, 0xbf, 0xbe,
	0xbc, 0xbd, 0xb8, 0xb9, 0xbb, 0xba, 0xb0, 0xb1, 0xb3, 0xb2,
	0xb7, 0xb6, 0xb4, 0xb5, 0xa0, 0xa1, 0xa3, 0xa2, 0xa7, 0xa6,
	0xa4, 0xa5, 0xaf, 0xae, 0xac, 0xad, 0xa8, 0xa9, 0xab, 0xaa
};


/*格雷码验证
 * @seed随机数种子
 */
uint32_t fp121_gray_identify(int seed)
{
	int i = 0;
	const unsigned char *p_org = 0;
	unsigned char *p_current = 0, line[122];
	uint32_t ret = 0;
	unsigned char bkg_light = 0x02, bkg_dark = 0x00,
		open_gray = 0x00, close_gray = 0x01, storage = 0x04;

	(void) seed;
	sensor_set_register(MA_MAIN_CFG_REG, storage | bkg_light | open_gray);
	sensor_set_register(MA_REF_CTRL_REG, 0x50);
	//sensor_set_register(MA_GREY_EXPF_REG, 0x40);
	sensor_set_register(MA_FINGER_DET_PARA_N, 0x00);
	sensor_set_register(MA_FINGER_DET_PARA_A, 0x00);
	sensor_set_register(MA_FINGER_DET_PARA_D, 0x10);
	sensor_read_line(line, sizeof(line));
	LOGD("light_bin================================================================");
	sensor_set_register(MA_MAIN_CFG_REG, storage | bkg_light | close_gray);
	memset(line, 0, sizeof(line));
	sensor_read_line(line, sizeof(line));
	p_org = light_bin;
	p_current = line;
	//printn(line, LINE_WIDTH);
	for(i = 0; i < LINE_WIDTH; i++, p_current++, p_org++) {
		if(*p_current != *p_org) {
			LOGE("light binary not equal, i = %d, current = 0x%x, org = 0x%x", i, p_current[0], p_org[0]);
			ret = 1;
			break;
		}
	}
	if(0 == ret) {
		LOGD("dark_gray================================================================");
		sensor_set_register(MA_MAIN_CFG_REG, storage | bkg_dark | open_gray);
		sensor_set_register(MA_REF_CTRL_REG, 0x51);
		//sensor_set_register(MA_REF_DMY_REG, 0x41);
		sensor_set_register(MA_FINGER_DET_PARA_N, 0x01);
		sensor_set_register(MA_FINGER_DET_PARA_A, 0x01);
		sensor_set_register(MA_FINGER_DET_PARA_D, 0x11);
		memset(line, 0, sizeof(line));
		sensor_read_line(line, sizeof(line));
		p_org = dark_gray;
		p_current = line;
		//printn(line, LINE_WIDTH);
		for(i = 0; i < LINE_WIDTH; i++, p_current++, p_org++) {
			if(*p_current != *p_org) {
				LOGE("dark gray not equal, i = %d, current = 0x%x, org = 0x%x", i, p_current[0], p_org[0]);
				ret = 2;
				break;
			}
		}
		if(0 == ret) {
			LOGD("dark_bin================================================================");
			sensor_set_register(MA_MAIN_CFG_REG, storage | bkg_dark | close_gray);
			sensor_set_register(MA_REF_CTRL_REG, 0x52);
			//sensor_set_register(MA_REF_DMY_REG, 0x42);
			sensor_set_register(MA_FINGER_DET_PARA_N, 0x02);
			sensor_set_register(MA_FINGER_DET_PARA_A, 0x02);
			sensor_set_register(MA_FINGER_DET_PARA_D, 0x12);
			memset(line, 0, sizeof(line));
			sensor_read_line(line, sizeof(line));
			p_org = dark_bin;
			p_current = line;
			//printn(line, LINE_WIDTH);
			for(i = 0; i < LINE_WIDTH; i++, p_current++, p_org++) {
				if(*p_current != *p_org) {
					LOGE("dark binary not equal, i = %d, current = 0x%x, org = 0x%x", i, p_current[0], p_org[0]);
					ret = 3;
					break;
				}
			}
			if(0 == ret) {
				LOGD("light_gray================================================================");
				sensor_set_register(MA_MAIN_CFG_REG, storage | bkg_light | open_gray);
				sensor_set_register(MA_REF_CTRL_REG, 0x53);
				//sensor_set_register(MA_REF_DMY_REG, 0x43);
				sensor_set_register(MA_FINGER_DET_PARA_N, 0x03);
				sensor_set_register(MA_FINGER_DET_PARA_A, 0x03);
				sensor_set_register(MA_FINGER_DET_PARA_D, 0x13);
				memset(line, 0, sizeof(line));
				sensor_read_line(line, sizeof(line));
				p_org = light_gray;
				p_current = line;
				//printn(line, LINE_WIDTH);
				for(i = 0; i < LINE_WIDTH; i++, p_current++, p_org++) {
					if(*p_current != *p_org) {
						LOGE("light gray not equal, i = %d, current = 0x%x, org = 0x%x", i, p_current[0], p_org[0]);
						ret = 4;
						break;
					}
				}
			}
		}
	}
	LOGD("ret = %d", ret);
	return ret;
}


static void get_current_line_para(int seed, current_line_identify_t *para)
{
	int tmpseed = 0;
	
	tmpseed = platform_rand();
	//    tmpseed = seed;
	LOGD("seed = %d", tmpseed);
	para->current_line = platform_rand_with_seed(tmpseed)%122;//0到121
	para->end_line = platform_rand_with_seed(para->current_line)%120 + 2;//2到121
	para->start_line = platform_rand_with_seed(para->end_line)%(para->end_line) ;//0到end_line-1
	para->line_step = platform_rand_with_seed(para->start_line)%121 + 1;//1-121
	//    platform_msleep(1000);
}


static unsigned char get_expect_next_line(unsigned char startline, unsigned char endline,
					  unsigned char current_line, unsigned char line_step)
{
	unsigned char ret = 0;
	unsigned short sum = current_line + line_step;

	LOGD("startline=%d, endline=%d, current_line=%d, line_step=%d",
	     startline, endline, current_line, line_step);

	if(sum <= endline) {
		ret = sum;
	} else {
		if (sum < 256) {
			ret = startline;
		} else {
			if((sum & 0x0ff) <= endline) {
				ret = (sum & 0x0ff);
			} else {
				ret = startline;
			}
		}
	}
	//    LOGD("ret = %d", ret);
	return ret;
}

static uint32_t special_current_line_identify(void * para)
{
	uint32_t ret = 0;
	//unsigned short next_line_1st = 0, next_line_2nd = 0;
	unsigned char currentln_1st = 0, currentln_2nd = 0, expect_currentln_1st = 0, expect_currentln_2nd = 0;
	unsigned char line[112];
	current_line_identify_t *data = (current_line_identify_t *)para;
	
	LOGD("start_line = %d, end_line = %d, curln = %d, line_step = %d\n",
	     data->start_line, data->end_line, data->current_line, data->line_step);

	sensor_set_register(MA_MAIN_CFG_REG, 0x30);//window_en & currentline_en
	sensor_set_register(MA_START_LINE_REG, data->start_line);
	sensor_set_register(MA_END_LINE_REG, data->end_line);
	sensor_set_register(MA_LINE_STEP_REG, data->line_step);
	sensor_set_register(MA_CURRENT_LINE_REG, data->current_line);
	sensor_read_line(line, sizeof(line));
	sensor_set_register(MA_MAIN_CFG_REG, 0x20);
	sensor_get_register(MA_CURRENT_LINE_REG, &currentln_1st);

	//next_line_1st = data->current_line + data->line_step;
	expect_currentln_1st = get_expect_next_line(data->start_line, data->end_line,
						    data->current_line, data->line_step);
	LOGD("%s expect_currentln_1st=%d, currentln_1st=%d",
	     __func__, expect_currentln_1st, currentln_1st);
	if(expect_currentln_1st != currentln_1st) {
		LOGE("%s 1st error, expect_currentln_1st=%d, currentln_1st=%d",
		     __func__, expect_currentln_1st, currentln_1st);
		ret = 1;
	}
	if(ret == 0) {
		sensor_read_line(line, sizeof(line));
		sensor_get_register(MA_CURRENT_LINE_REG, &currentln_2nd);
		expect_currentln_2nd = get_expect_next_line(data->start_line, data->end_line,
							    currentln_1st, data->line_step);
		LOGD("%s expect_currentln_2nd=%d, currentln_2nd=%d",
		     __func__, expect_currentln_2nd, currentln_2nd);
		if(expect_currentln_2nd != currentln_2nd) {
			LOGE("%s 2nd error, expect_currentln_2nd=%d, currentln_2nd=%d",
			     __func__, expect_currentln_2nd, currentln_2nd);
			ret = 2;
		}
	}
	LOGD("ret = %d", ret);
	return ret;
}

/*当前行验证
 * @seed随机数种子
 */
uint32_t fp121_current_line_identify(int seed)
{
	uint32_t ret = 0;
	current_line_identify_t para;
	
	//    LOGD("seed = %d\n", seed);
	get_current_line_para(seed, &para);
	ret = special_current_line_identify((void *)(&para));
	LOGD("ret = %d", ret);
	return ret;
}


static void get_image_mark_para(int seed, image_mark_identify_t *para)
{
	int tmpseed = 0;
	
	tmpseed = platform_rand();
	//    tmpseed = seed;
	LOGD("seed = %d", tmpseed);
	para->current_line = platform_rand_with_seed(tmpseed)%12 + 110; //110-121之间
	para->end_line = platform_rand_with_seed(para->current_line)%7 + 115;//115-121之间
	para->start_line = platform_rand_with_seed(para->end_line)%(para->end_line - 110) + 110;//110-(endline-1)之间
	para->line_step = platform_rand_with_seed(para->start_line)%121+1;//1-121之间
	para->gray_disable = (platform_rand_with_seed(para->end_line))%2;
	//    platform_msleep(1000);
}


static uint32_t special_image_mark_identify(void *para)
{
	uint32_t ret = 0;
	unsigned char curentln = 0;
	unsigned char line[124];
	image_mark_identify_t *data = (image_mark_identify_t *)para;
	
	LOGD("start_line = %d, end_line = %d, linestep = %d, curln = %d\n",
	     data->start_line, data->end_line, data->line_step, data->current_line);

	sensor_set_register(MA_MAIN_CFG_REG, (0x30 | (data->gray_disable & 0x01)));//打开行选使能+开关雷码
	sensor_set_register(MA_START_LINE_REG, data->start_line);
	sensor_set_register(MA_END_LINE_REG, data->end_line);
	sensor_set_register(MA_LINE_STEP_REG, data->line_step);
	sensor_set_register(MA_CURRENT_LINE_REG, data->current_line);
	sensor_set_register(MA_MAIN_CFG_REG, (0x00 | (data->gray_disable & 0x01)));//关闭行选使能+开关雷码
	sensor_get_register(MA_CURRENT_LINE_REG, &curentln);
	//    LOGD("curentln = %d\n", curentln);
	sensor_read_line(line, sizeof(line));
	sensor_set_register(MA_MAIN_CFG_REG, (0x30 | (data->gray_disable & 0x01)));//打开行选使能+开关雷码
	sensor_set_register(MA_CURRENT_LINE_REG, data->current_line);
	sensor_set_register(MA_MAIN_CFG_REG, (0x00 | (data->gray_disable & 0x01)));//关闭行选使能+开关雷码
	sensor_get_register(MA_CURRENT_LINE_REG, &curentln);
	//    LOGD("curentln = %d\n", curentln);
	sensor_read_line(line, sizeof(line));
	//printn(line, w);

	if((data->gray_disable & 0x01) == 0x01) {//关闭格雷码
		LOGD("gray disable, line[122]=0x%x, line[123]=0x%x", line[122], line[123]);
		if(120 == curentln) {
			if(line[122] != 0xff) {
				ret = 1;
			}
		} else {
			if(line[122] != 0x00) {
				ret = 2;
			}
		}
	} else {//打开格雷码
		LOGD("gray enable, line[122]=0x%x, line[123]=0x%x", line[122], line[123]);
		if(120 == curentln) {
			if(line[122] != 0xaa) {
				ret = 3;
			}
		} else {
			if(line[122] != 0x00) {
				ret = 4;
			}
		}
	}
	LOGD("ret = %d", ret);
	return ret;
}

/*图像mark验证
 * @seed随机数种子
 */
uint32_t fp121_image_mark_identify(int seed)
{
	uint32_t ret = 0;
	image_mark_identify_t para;
	
	get_image_mark_para(seed, &para);//gray开关是否有问题? by zhangli
	ret = special_image_mark_identify((void *)(&para));
	LOGD("ret = %d", ret);
	return ret;
}

