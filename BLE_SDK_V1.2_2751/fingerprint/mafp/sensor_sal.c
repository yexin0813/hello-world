#include "sensor_utils.h"
#include "sensor_sal.h"
#include "sensor_reg.h"
#include "sensor_identify.h"
#include "policy.h"
#include "afs121.h"
#include <string.h>
#include <stdlib.h>
#include "platform.h"
#include "algorithm.h"
#include "malib.h"
#include "m_timer.h"
#include "tpls_mngr.h"
#include "type_def.h"


sensor_context_t sen_ctx;
alg_buf_t        g_buf;
uint8_t bmp_file_buffer[16*1024] = {0x00};

img_file_t      *img_file = (img_file_t *)bmp_file_buffer;


static int32_t sensor_get_bkg(void);
static int32_t sensor_alg_init(void);
static int32_t get_proper_image(uint8_t *image, uint32_t len);


int32_t sensor_create(int32_t *dead_block_num)
{
	int32_t i;

#if GRAY_CODE_VERIFICATION == 1
	int32_t seed = 0;
	uint8_t * image = g_buf.tmp.u.imgs.buf2;
#endif
	uint8_t id = 0;
        
	LOGD("start.");

	platform_create();

#if GRAY_CODE_VERIFICATION == 1
	if (get_proper_image(image, IMAGE_SIZE)) {
		LOGE("get_proper_image error.");
		return ERROR_GET_IMAGE;
	}
	seed = getHashIndex(image);

	if (fp121_current_line_identify(seed)) {
		LOGE("identify 1 failed.");
		return ERROR_CHIP_IDENTIFY;
	}

	if (fp121_image_mark_identify(seed)) {
		LOGE("identify 2 failed.");
		return ERROR_CHIP_IDENTIFY;
	}

	if (fp121_gray_identify(seed)) {
		LOGE("identify 3 failed.");
		return ERROR_CHIP_IDENTIFY;
	}
#endif

	memset(&sen_ctx, 0, sizeof(sen_ctx));
	memset(&g_buf, 0, sizeof(g_buf));

	sen_ctx.work_mode = POWERDOWN_MODE;
	sen_ctx.interrupt_delay_ms = 15;

	// load properties from storage
	if (policy_load_properties((uint8_t *)&sen_ctx.prop, sizeof(sensor_property_t))) {
		LOGE("policy_load_properties failed.");
		return ERROR_LOAD_PROPERTIES;
	}

	if (sen_ctx.prop.ver != PROPERTY_VERSION) {
		LOGE("property version check failed.");
		return ERROR_LOAD_PROPERTIES;
	}

	if (policy_init_templates()) {
		LOGE("Init templates failed.");
		return ERROR_INIT_TEMPLATES;
	}

	LOGE("Properties : %#x, %#x, %#x, %x, [%d, %d, %d], [%d, %d]",
	     sen_ctx.prop.capacity, sen_ctx.prop.refctrl, sen_ctx.prop.volt, sen_ctx.prop.para_calibrated,
	     sen_ctx.prop.a, sen_ctx.prop.n, sen_ctx.prop.int_calibrated,
	     sen_ctx.tpls_mngr.subtpls_used, sen_ctx.tpls_mngr.subtpls_block_dead);

	LOGE("------> CorrFull = %d", sen_ctx.prop.corr_full);
	LOGE("------> CorrList :");
	for (i = 0; i < 6; i++) {
		LOGE("[%d] %6d %6d %6d %6d %6d %6d", i, sen_ctx.prop.corr_list[i * 6 + 0], sen_ctx.prop.corr_list[i * 6 + 1],
		     sen_ctx.prop.corr_list[i * 6 + 2], sen_ctx.prop.corr_list[i * 6 + 3], sen_ctx.prop.corr_list[i * 6 + 4],
		     sen_ctx.prop.corr_list[i * 6 + 5]);
	}
	
	if (sen_ctx.prop.corr_full > 100000) {
		LOGE("sensor_alg_init failed, corr_full > 100000.");
		return ERROR_INIT_ALGO;
	}

	policy_init_matched_record(&sen_ctx.rcrd_cntr);

	if (dead_block_num) *dead_block_num = sen_ctx.tpls_mngr.subtpls_block_dead;

	sensor_hw_get_chipid(&id);
	if (id != AFS121_CHIPID) {
		LOGE("get chip id error, %x", id);
		return ERROR_CHIP_ID;
	}
	
	LOGD("end.");
        
	return 0;
}


int32_t sensor_destroy(void)
{
	sen_ctx.work_mode = POWERDOWN_MODE;

	platform_destroy();
        
	return 0;
}


int32_t sensor_reset_mode(void)
{
	int32_t ret = 0;

	sen_ctx.work_mode = RESET_MODE;
	
	ret = sensor_hw_set_mode(RESET_MODE);
	if (ret) {
		return ret;
	}

	return 0;
}


int32_t sensor_capture_mode(void)
{
	sensor_property_t *prop = &sen_ctx.prop;
	int32_t ret = 0;

	LOGD("start.");

#if DETECT_MODE_ENABLE == 1
	if (sen_ctx.work_mode == CAPTURE_MODE) {
		LOGD("already capture mode.");

		return 0;
	}
#endif
        
	LOGD("capacity=%#x, volt=%#x, refctrl=%#x, work_mode=%#x",
	     prop->capacity, prop->volt, prop->refctrl, sen_ctx.work_mode);

	if (sen_ctx.work_mode == DETECT_MODE) {
		if (sensor_hw_set_mode(CAPTURE_MODE)) {
			ret = sensor_hw_set_mode(RESET_MODE);
			if (ret) return ret;
		}
	} else {
		ret = sensor_hw_set_mode(RESET_MODE); 
		if (ret) return ret;
	}

	sensor_hw_set_register(MA_INTERRUPT_REG, 0x80);
    

	sensor_hw_set_register(MA_MAIN_CFG_REG, 0xf0);
    
        
	sensor_hw_set_register(MA_START_LINE_REG, 0x01);
    
	sensor_hw_set_register(MA_END_LINE_REG, 0x79);
    

	sensor_hw_set_register(MA_CAPACITY_CFG_REG, 0x9c);
    
	sensor_hw_set_register(MA_GREY_OFST_REG, GREY_OFFSET);
    

	sensor_hw_set_register(MA_VOLTAGE_CFG_REG, prop->volt);
	platform_msleep(3);

	sensor_hw_set_register(MA_REF_CTRL_REG, prop->refctrl);
    
	sensor_hw_set_register(MA_CAPACITY_CFG_REG, prop->capacity);
	platform_msleep(3);
	
	sen_ctx.work_mode = CAPTURE_MODE;

	LOGD("end.");

	return 0;
}


int32_t sensor_powerdown_mode(void)
{
	int32_t ret;
        
	if (sen_ctx.work_mode == POWERDOWN_MODE) return 0;

	sen_ctx.work_mode = POWERDOWN_MODE;

	ret = sensor_hw_set_mode(POWERDOWN_MODE);
	if (ret) return ret;
        
	return 0;
}


#if DETECT_MODE_ENABLE == 1
static int32_t detect_mode(uint8_t n, uint8_t a, uint8_t int_type)
{
	int32_t ret = 0;

	LOGD("start.");

	if (sen_ctx.work_mode != CAPTURE_MODE) {
		ret = sensor_capture_mode();
		if (ret) return ret;
	}

	sen_ctx.work_mode = DETECT_MODE;

	sensor_hw_set_register(MA_START_LINE_REG, DETECT_START_LINE);
	sensor_hw_set_register(MA_CURRENT_LINE_REG, DETECT_CURRENT_LINE);
	sensor_hw_set_register(MA_END_LINE_REG, DETECT_END_LINE);
	sensor_hw_set_register(MA_LINE_STEP_REG, DETECT_LINE_STEP);
	sensor_hw_set_register(MA_FRAME_ROWS_REG, DETECT_FRAME_ROW);

	sensor_hw_set_register(MA_INTERRUPT_REG, int_type);
	sensor_hw_set_register(MA_FINGER_DET_PARA_N, n);
	sensor_hw_set_register(MA_FINGER_DET_PARA_A, a);
	
	ret = sensor_hw_set_mode(DETECT_MODE);
	if (ret) return ret;

	LOGD("end");
        
	return 0;
}
#endif


int32_t sensor_detect_mode(void)
{
	int32_t ret = 0;
	
#if DETECT_MODE_ENABLE == 1	
	LOGD("start.");

	LOGD("---> detect mode, n=%#x, a=%#x", sen_ctx.prop.n, sen_ctx.prop.a);
	
	ret = detect_mode(sen_ctx.prop.n, sen_ctx.prop.a, 0xa0);
		
	LOGD("end.");
#endif

	return ret;
}


int32_t sensor_get_chipid(uint8_t *id)
{
	int32_t ret = 0;

	LOGD("start.");

	ret = sensor_capture_mode();
	if (ret) {
		LOGE("mode switch failed.\r\n");
		return ret;
	}
	
	ret = sensor_hw_get_register(MA_PRODU_CODE_2_REG, id);
	LOGD("sensor id is: %#x", *id);
        
	LOGD("end.");

	return ret;
}


int32_t sensor_read_image(uint8_t *rawBuf, uint32_t len)
{
	int32_t i, ret = 0;
	uint8_t *outBuf = rawBuf;
	uint8_t *inBuf = g_buf.tmp.u.imgs.buf2;

	LOGD("start.");

	sensor_hw_set_register(MA_CURRENT_LINE_REG, 0x01);
    
	sensor_hw_set_register(MA_LINE_STEP_REG, 0x01);
    
	sensor_hw_set_register(MA_START_LINE_REG, 0x01);
    
	sensor_hw_set_register(MA_END_LINE_REG, 0x7a);

    
	ret = sensor_hw_read_image( inBuf, RAW_IMAGE_SIZE );
    
	if (ret) {
		LOGE("read image failed.");
		return ret;
	}

	for ( i = 0, inBuf+=RAW_IMAGE_LINE; i < IMAGE_H; i++, outBuf+=IMAGE_W, inBuf+=RAW_IMAGE_LINE )
	{
		memcpy(outBuf, inBuf, IMAGE_W);
	}

	LOGD("end.");

	return 0;
}


int32_t sensor_calc_init_parameters(void)
{
	int32_t i = 0, ret = 0, cap_value = 6, temp_grey = 0;
	uint8_t *raw = g_buf.tmp.u.imgs.buf2;
	int32_t min = 0, max = 31, middle = (min + max) >> 1;
	int32_t refctrl, volt;

	ret = sensor_hw_set_mode(RESET_MODE);
	if ( ret ) return ret;

	sensor_hw_set_register(MA_MAIN_CFG_REG, 0xf0);
	sensor_hw_set_register(MA_CAPACITY_CFG_REG, 0xdc);
	sensor_hw_set_register(MA_GREY_OFST_REG, 0xff);
	sensor_hw_set_register(MA_VOLTAGE_CFG_REG, 0x00);
	while ( max - min > 1 ) {
		LOGD("middle %d", middle);
		sensor_hw_set_register(MA_REF_CTRL_REG, middle << 3);
		sensor_read_image( raw, IMAGE_SIZE );
		temp_grey = sensor_get_grey( raw, IMAGE_SIZE );
		if ( temp_grey > IMAGE_SIZE * 100 ) {
			min = middle;
		} else {
			max = middle;
		}
		middle = (max + min) >> 1;
		LOGD("temp_grey_avg %d", temp_grey / 14400);
	}

	cap_value = max;
	LOGD("cap_value = %d", cap_value);
	cap_value++;
	do {
		cap_value--;
		sensor_hw_set_register(MA_REF_CTRL_REG, cap_value << 3);
		for (i = 0; i < 16; i++) {
			sensor_hw_set_register(MA_CAPACITY_CFG_REG, 0x9c);
			sensor_hw_set_register(MA_VOLTAGE_CFG_REG, i);
			platform_msleep(3);
			sensor_hw_set_register(MA_CAPACITY_CFG_REG, 0xdc);
			sensor_read_image( raw, IMAGE_SIZE );

			if ( !sensor_bkg_check( raw, IMAGE_SIZE, 48, IMAGE_H ) ) {
				break;
			}
		}
	} while( i == 16 );
	LOGD("cap_value now = %d", cap_value);
	refctrl = cap_value << 3;
	volt = i;
	if ( i==16 ) {
		LOGE("failed.");
		return ERROR_CALC_INIT_PARAM;
	}
	LOGD("refctr = 0x%x, volt = 0x%x, grey= %d.",
	     refctrl, volt, sensor_get_grey(raw, IMAGE_SIZE));

	sen_ctx.prop.refctrl = refctrl;
	sen_ctx.prop.volt = volt;
	sen_ctx.prop.para_calibrated = 1;

	return 0;
}


#if DETECT_MODE_ENABLE == 1
static int32_t calc_paraN(uint8_t avggrey)
{
	uint8_t cnt = 0, n = 0;

	LOGD("start.");

	if (detect_mode(0, avggrey, 0xe0)) {
		LOGE("set detect mode failed.");
		return ERROR_MODE_SWITCH;
	}

	platform_msleep(sen_ctx.interrupt_delay_ms);

	sensor_hw_set_mode(CAPTURE_MODE);
	sensor_hw_get_register(MA_FINGER_DET_PARA_L, &cnt);

	if (cnt < 20) {
		n =cnt + 8;
	}

	if (n == 0) {
		n = 20;
	}

	sen_ctx.prop.n = n;
	sen_ctx.prop.a = avggrey;
        
	LOGD("n = 0x%x, a = 0x%x, cnt=0x%x.", n, avggrey, cnt);
        
	return 0;
}


int32_t sensor_calc_interrupt_parameters(void)
{
	int32_t avg = 0, ret = 0;
	uint8_t line[RAW_IMAGE_LINE] = { 0 };

	LOGD("start.");

	if (detect_mode(0, 0, 0xe0)) {
		LOGE("set detect mode failed.");
		return ERROR_MODE_SWITCH;
	}

	platform_msleep(sen_ctx.interrupt_delay_ms);

	sensor_hw_set_mode(CAPTURE_MODE);
	
	ret = sensor_hw_read_line(line, RAW_IMAGE_LINE);
	if (ret) {
		LOGE("read line failed.");
		return ret;
	}

	avg = sensor_get_grey(line, IMAGE_W) / IMAGE_W;
	LOGD("avg=0x%x.", avg);

	ret = (avg > 0) ? 0 : ERROR_CALC_INT_PARAM;
	if (avg > 0) {
		if ( sen_ctx.prop.a > (avg - 2)
		     || (sen_ctx.prop.a == 0)) {
			if (calc_paraN(avg - 2)) {
				LOGE("calc_paraN failed.");
				return ERROR_CALC_INT_PARAM;
			} else {
				sensor_set_interrupt_calibrated();
			}
		}
	}

	LOGD("end.");
        
	return ret;
}


int32_t sensor_is_interrupt_calibrated(void)
{
	return (!sen_ctx.prop.int_calibrated)? 0 : 1;
}


int32_t sensor_set_interrupt_calibrated(void)
{
	sen_ctx.prop.int_calibrated = 1;
	policy_save_properties((uint8_t *)&sen_ctx.prop, sizeof(sensor_property_t), GET_KEY_ALL());
	
	return 0;
}


int32_t sensor_reset_interrupt_parameters(void)
{
	sen_ctx.prop.n = 0;
	sen_ctx.prop.a = 0;
	sen_ctx.prop.int_calibrated = 0;
        
	return 0;
}
#endif


int32_t sensor_is_para_calibrated(void)
{
	return (!sen_ctx.prop.para_calibrated)? 0 : 1;
}


int32_t sensor_reinit_parameters(void)
{
	int32_t ret = 0, i;

	policy_load_properties((uint8_t *)&sen_ctx.prop, sizeof(sensor_property_t));
	policy_clear_properties(GET_KEY_ALL());

	LOGE("===== !!! Reinit Parameters Now. !!! =====");
	memset(&sen_ctx.prop, 0, sizeof(sensor_property_t));
		
	sen_ctx.prop.ver = PROPERTY_VERSION;
		
	sen_ctx.prop.capacity = 0xdc;
	sen_ctx.prop.bkg_energy = 0xffff;
		
	policy_load_bkg(&sen_ctx.prop.bkg, IMAGE_SIZE);

	if (sensor_calc_init_parameters()) {
		LOGE("calc init parameters failed.");
		return ERROR_CALC_INIT_PARAM;
	}

#if DETECT_MODE_ENABLE == 1
	if (sensor_calc_interrupt_parameters()) {
		LOGE("calc interrupt parameters failed.");
		return ERROR_CALC_INT_PARAM;
	}
#endif
	
	ret = sensor_get_bkg();
	if (ret) {
		LOGE("sensor get bkg failed.");
		return ERROR_GET_IMAGE;
	}

	if (sensor_alg_init()) {
		LOGE("sensor_alg_init failed.");
		return ERROR_INIT_ALGO;
	}

	LOGE("Properties : %#x, %#x, %#x, %x, [%d, %d, %d], [%d, %d]",
	     sen_ctx.prop.capacity, sen_ctx.prop.refctrl, sen_ctx.prop.volt, sen_ctx.prop.para_calibrated,
	     sen_ctx.prop.a, sen_ctx.prop.n, sen_ctx.prop.int_calibrated,
	     sen_ctx.tpls_mngr.subtpls_used, sen_ctx.tpls_mngr.subtpls_block_dead);

	LOGE("------> CorrFull = %d", sen_ctx.prop.corr_full);
	LOGE("------> CorrList :");
	for (i = 0; i < 6; i++) {
		LOGE("[%d] %6d %6d %6d %6d %6d %6d", i, sen_ctx.prop.corr_list[i * 6 + 0], sen_ctx.prop.corr_list[i * 6 + 1],
		     sen_ctx.prop.corr_list[i * 6 + 2], sen_ctx.prop.corr_list[i * 6 + 3], sen_ctx.prop.corr_list[i * 6 + 4],
		     sen_ctx.prop.corr_list[i * 6 + 5]);
	}

	if (sen_ctx.prop.corr_full > 100000) {
		LOGE("sensor_alg_init failed, corr_full > 100000.");
		return ERROR_INIT_ALGO;
	}

	policy_save_properties((uint8_t *)&sen_ctx.prop, sizeof(sensor_property_t), GET_KEY_ALL());

	return 0;
}


static int32_t get_proper_image(uint8_t *image, uint32_t len)
{
	int32_t ret = 0;

	LOGD("start.");
	
	ret = sensor_capture_mode();
	if (ret) {
		LOGE("set capture_mode error");
		return ret;
	}

	ret = sensor_read_image(image, len);
	if (0 != sensor_bkg_check(image, IMAGE_SIZE, 16, IMAGE_W)) {
		
		sensor_calc_init_parameters();
		ret = sensor_capture_mode();
		if ( ret ) {
			LOGE("set capture_mode error");
		}
		ret = sensor_read_image(image, len);
	}

	LOGD("end. ret=%d",  ret);
	return ret;
}


static int32_t sensor_get_bkg(void)
{
	uint8_t *raw = g_buf.tmp.u.imgs.buf2;
        
	if (get_proper_image( raw, IMAGE_SIZE )) {
		LOGE("get_proper_image error.");
		return ERROR_GET_IMAGE;
	}

	LOGE("--> bkg updating...");
					
	sen_ctx.prop.bkg_energy = 100;

	policy_save_bkg(raw, IMAGE_SIZE, GET_KEY_ALL());
	policy_save_properties((uint8_t *)&sen_ctx.prop, sizeof(sensor_property_t), GET_KEY_ALL());
#if PROCESS_IMAGE_UPLOAD_EN
	save_as_img("bkg.bmp", raw, IMAGE_W, IMAGE_H);
#endif

	return 0;
}


static int32_t sensor_alg_init(void)
{
	uint8_t *image = g_buf.tmp.u.imgs.buf2;
	
again:
	if (get_proper_image(image, IMAGE_SIZE)) {
		LOGE("get_proper_image error");
		return ERROR_GET_IMAGE;
	}
		
	sen_ctx.prop.corr_full = calCorrFull(sen_ctx.prop.bkg, image);
	if (!sen_ctx.prop.corr_full) {
		LOGE("Failed.");
		goto again;
	}
	
	calCorrList(sen_ctx.prop.bkg, image, sen_ctx.prop.corr_list);

	sen_ctx.prop.corr_inited = 1;
	
	policy_save_properties((uint8_t *)&sen_ctx.prop, sizeof(sensor_property_t), GET_KEY_ALL());
	
	return 0;
}


int32_t sensor_detect_touch(void)
{
	int32_t dtc;
	uint8_t *raw = g_buf.tmp.u.imgs.buf2;

	if (get_proper_image( raw, IMAGE_SIZE )) {
		LOGE("get_proper_image error.");
		return ERROR_GET_IMAGE;
	}

	dtc = checkPress(sen_ctx.prop.bkg, raw, sen_ctx.prop.corr_full);

	return ( dtc == 1 ) ? NORMAL_PRESSED : NORMAL_UNPRESSED;
}



#define ENROLL_DEC_PER_SECOND           (4)
#define ENROLL_AREA_BASELINE            (30)
#define ENROLL_STATIC_TIME              (500)
#define ENROLL_TIMEOUT                  (3000)  // ms

#define AUTH_DEC_PER_SECOND             (6)
#define AUTH_AREA_BASELINE              (20)
#define MATCH_STATIC_TIME               (200)
#define AUTH_TIMEOUT                    (3000)  // ms


int32_t sensor_get_enroll_image(void)
{
	uint8_t *raw = g_buf.tmp.u.imgs.buf1;
	uint8_t *src = g_buf.tmp.u.imgs.buf2;
	int32_t cur_area = 0;
	uint32_t start = 0, current = 0;

	memset(src, 0, IMAGE_SIZE);
	platform_get_jiffies(&start);

	while (1) {
		platform_get_jiffies(&current);
		if (current - start >= ENROLL_TIMEOUT) {
			LOGE("failed, time out.");
			return ERROR_GET_IMAGE;
		}

		if (get_proper_image(raw, IMAGE_SIZE)) {
			LOGE("get_proper_image error");
			return ERROR_GET_IMAGE;
		}
        
		cur_area = getTouchArea( sen_ctx.prop.bkg, raw, sen_ctx.prop.corr_list );

		LOGE("area = %d, time = %d", cur_area, current-start);

		if ( (current - start < ENROLL_STATIC_TIME) && (cur_area < 36) ) {
			continue;
		}
		
		if ( ( cur_area >= 36 )
		     || ( ( cur_area >= ENROLL_AREA_BASELINE )
			  && ( cur_area >= ( 36 - ( current - start - ENROLL_STATIC_TIME ) * ENROLL_DEC_PER_SECOND / 1000 ) ) )) {
			LOGD("Done, Elapsed %d, getTouchArea = %d", current - start, cur_area);
			fp121_rmvBkg_Expand( raw, sen_ctx.prop.bkg, src, g_buf.tmp.u.proc_buf, g_buf.tmp.limit_buf);

#if PROCESS_IMAGE_UPLOAD_EN
			snprintf((char *)img_file->name, 100, "enroll_src%d.bmp", sen_ctx.enroll_steps);
			save_as_img((char *)img_file->name, src, IMAGE_W, IMAGE_H);
#endif

			// copy to buf1, for extract
			memcpy(g_buf.tmp.u.imgs.buf1, src, IMAGE_SIZE);
			
			return 0;
		}
	}
}


int32_t sensor_get_auth_image(void)
{
	uint8_t *raw = g_buf.tmp.u.imgs.buf1;
	uint8_t *src = g_buf.tmp.u.imgs.buf2;
	int32_t last = 0, cur_area = 0;
	uint32_t start = 0, current = 0;
	int32_t areaTH = 0;


	memset(src, 0, IMAGE_SIZE);
	
	platform_get_jiffies(&start);
	
	while (1) {
		platform_get_jiffies(&current);
		if ( current - start >= AUTH_TIMEOUT ) {
			LOGE("failed, time out.");
			return ERROR_GET_IMAGE;
		}



		if (get_proper_image( raw, IMAGE_SIZE )) {
			LOGE("get_proper_image error");
			return ERROR_GET_IMAGE;
		}
        

		cur_area = getTouchArea( sen_ctx.prop.bkg, raw, sen_ctx.prop.corr_list );




        sen_ctx.touch_area = cur_area;                // touchArea para for match to decide if update

		LOGE("area = %d, last = %d, time = %d", cur_area, last, current-start);

		if ( cur_area >= 36) {
			LOGD("Done 1, area = %d", cur_area);
			break;
		}

		if ( cur_area + 4 < last ) {
			LOGE("Leaving, %d < %d.", cur_area, last);
			return ERROR_GET_IMAGE;
		}

		if ( cur_area <= last )  {
			if (current - start < MATCH_STATIC_TIME ) {
				areaTH = 32;
			} else {
				areaTH = MAX( AUTH_AREA_BASELINE, 32 - ( current - start - MATCH_STATIC_TIME ) * AUTH_DEC_PER_SECOND / 1000 );
			}

			LOGD("areaTH = %d", areaTH);

			if ( (cur_area >= areaTH) && (last >= areaTH) ) {
				LOGD("Done 2, area = %d", cur_area);
				break;
			}
		}

		last = cur_area;
	}

#if PROCESS_IMAGE_UPLOAD_EN
	save_as_img("auth_raw.bmp", raw, IMAGE_W, IMAGE_H);
#endif




	fp121_rmvBkg_Expand( raw, sen_ctx.prop.bkg, src, g_buf.tmp.u.proc_buf, g_buf.tmp.limit_buf);



    
	// copy to buf1, for extract
	memcpy(g_buf.tmp.u.imgs.buf1, src, IMAGE_SIZE);

#if PROCESS_IMAGE_UPLOAD_EN
	save_as_img("auth_src.bmp", src, IMAGE_W, IMAGE_H);
#endif



	return 0;
}


int32_t sensor_capture_image(void)
{
	uint8_t *raw = g_buf.tmp.u.imgs.buf1;
	uint8_t *src = g_buf.tmp.u.imgs.buf2;
	int32_t cur_area = 0, last = 0;
	uint32_t start = 0, current = 0;

	memset(src, 0, IMAGE_SIZE);
	platform_get_jiffies(&start);

	while (1) {
		platform_get_jiffies(&current);
		if (current - start >= ENROLL_TIMEOUT) {
			LOGE("failed, time out.");
			return ERROR_GET_IMAGE;
		}

		if (get_proper_image(raw, IMAGE_SIZE)) {
			LOGE("get_proper_image error");
			return ERROR_GET_IMAGE;
		}
        
		cur_area = getTouchArea( sen_ctx.prop.bkg, raw, sen_ctx.prop.corr_list );
		sen_ctx.touch_area = cur_area;                // touchArea para for match to decide if update

		LOGE("area = %d, time = %d", cur_area, current-start);

		if ( cur_area + 4 < last ) {
			LOGE("Leaving, %d < %d.", cur_area, last);
			return ERROR_GET_IMAGE;
		}

		last = cur_area;

		if ( (current - start < ENROLL_STATIC_TIME) && (cur_area < 36) ) {
			continue;
		}
		
		if ( ( cur_area >= 36 )
		     || ( ( cur_area >= ENROLL_AREA_BASELINE )
			  && ( cur_area >= ( 36 - ( current - start - ENROLL_STATIC_TIME ) * ENROLL_DEC_PER_SECOND / 1000 ) ) )) {
			LOGD("Done, Elapsed %d, getTouchArea = %d", current - start, cur_area);
			fp121_rmvBkg_Expand( raw, sen_ctx.prop.bkg, src, g_buf.tmp.u.proc_buf, g_buf.tmp.limit_buf);

#if PROCESS_IMAGE_UPLOAD_EN
			snprintf((char *)img_file->name, 100, "enroll_src%d.bmp", sen_ctx.enroll_steps);
			save_as_img((char *)img_file->name, src, IMAGE_W, IMAGE_H);
#endif

			// copy to buf1, for extract
			memcpy(g_buf.tmp.u.imgs.buf1, src, IMAGE_SIZE);

			return 0;
		}
	}
}


int32_t sensor_check_movement(void)
{
	int32_t movement = -1;
	
	movement = afs_checkMove((int *)sen_ctx.subtpls_buf, (int *)&g_buf );
	if ( movement >= 0 && movement <= 256 ) {
		LOGE("--> Note : Matched with latest updated template.");
		return ERROR_CHECK_MOVEMENT;
	}
	
	return 0;
}



int32_t sensor_do_authenticate(int32_t threshold, uint8_t expected_fid)
{
	int32_t match_score = 0, i;
	uint8_t sorted_fids_list[ FINGER_NUM ], used_fids = 0;
	uint8_t latest_matched[16], n, j, flag = 0, fid;

    


	SET_KEY_1();
	
	LOGD("start ************************************************* ");

	sen_ctx.match_score = 0;




	// STEP 0: expected fid
	if (expected_fid != 0xff) {
		fid = expected_fid;
		if (sensor_fid_is_used(fid)) {
			match_score = afs_match( fid, (int *)sen_ctx.subtpls_buf, (int *)&g_buf, (sen_ctx.touch_area>=32)?1:0);
			LOGD("0. FID =%3d, Score = %d", fid, match_score);

			if (match_score < 0) {
				LOGE("match fail %d.", match_score);
				return ERROR_AUTH_ALGO_FAIL;
			}

			if (match_score >= threshold) {
				sen_ctx.match_score = match_score;
			
				LOGD("--> Step 0 : matched fid=%d ", fid);
				policy_save_matched_fid(fid, &sen_ctx.rcrd_cntr, GET_KEY_ALL());
				return fid;
			}
		}

		return ERROR_AUTH_NO_MATCH;
	}	



	// STEP 1: search latest matched list
	n = policy_load_latest_matched(latest_matched, sen_ctx.rcrd_cntr);



	for (i = 0; i < n; i++) {
		fid = latest_matched[i];
		if (!sensor_fid_is_used(fid)) continue;

        
		match_score = afs_match( fid, (int *)sen_ctx.subtpls_buf, (int *)&g_buf, (sen_ctx.touch_area>=32)?1:0);
		LOGD("1. FID =%3d, Score = %d", fid, match_score);

        
		if (match_score < 0) {
			LOGE("match fail %d.", match_score);
			return ERROR_AUTH_ALGO_FAIL;
		}

		if (match_score >= threshold) {
			sen_ctx.match_score = match_score;

			LOGD("--> Step 1 : matched fid=%d ", fid);
			policy_save_matched_fid(fid, &sen_ctx.rcrd_cntr, GET_KEY_ALL());
			
			return fid;
		}
	}

    
	
	// STEP 2: search all, kickout STEP 1 items
	used_fids = fids_get_sorted_list(&sen_ctx.tpls_mngr, sorted_fids_list);
	LOGD("Valid Fingers : %d", used_fids);

	for (i = 0; i < used_fids; i++) {
		fid = sorted_fids_list[i];
		if (!sensor_fid_is_used(fid)) continue;

		// kick out STEP 1 Items
		for (flag = 0, j = 0; j < n; j++) {
			if (fid == latest_matched[j]) {
				flag = 1;
				break;
			}
		}
		if (flag) continue;

        
		match_score = afs_match( fid, (int *)sen_ctx.subtpls_buf, (int *)&g_buf, (sen_ctx.touch_area>=32)?1:0);
		LOGD("2. FID =%3d, Score = %d", fid, match_score);


        
		if (match_score < 0) {
			LOGE("match fail %d.", match_score);
			return ERROR_AUTH_ALGO_FAIL;
		}

		if (match_score >= threshold) {
			sen_ctx.match_score = match_score;

			LOGD("--> Step 2 : matched fid=%d ", fid);

            
			policy_save_matched_fid(fid, &sen_ctx.rcrd_cntr, GET_KEY_ALL());
            


			return fid;
		}
	}

	LOGD("No match.");

	return ERROR_AUTH_NO_MATCH;
}


int32_t sensor_do_authenticate_finish(void)
{
	//if (sen_ctx.match_score >= 3000) {
	//	LOGE("FID = %d, Amount = %d, Score = %d\r\n",
	//	     sen_ctx.match_fid,
	//	     fidtpl_get_subtpls_amount(&sen_ctx.tpls_mngr, sen_ctx.match_fid),
	//	     sen_ctx.match_score);
	//}
		
	//if ((sen_ctx.match_fid != FINGER_NUM) && (sen_ctx.match_score >= 5000)) {
	//	// show updated template
	//	fid_show_status(&sen_ctx.tpls_mngr, sen_ctx.match_fid);
	//}
	
	return 0;
}


int32_t sensor_get_enroll_steps(uint8_t fid)
{
	if (!sen_ctx.enroll_steps) {
		sen_ctx.enroll_fid = fid;
		sen_ctx.enroll_steps = 0;
		
	} else if (sen_ctx.enroll_fid != fid) {
		LOGE("incomplete enroll process %d.", sen_ctx.enroll_fid);
		
		sensor_do_enroll_cancel();

		return ERROR_ENROLL_LAST_INCOMPLETE;
	}

	return sen_ctx.enroll_steps;
}


int32_t sensor_do_enroll(void)
{
	int32_t ret = 0;

	LOGD("start.");

	SET_KEY_1();

	ret = afs_enroll(sen_ctx.enroll_fid, (int *)sen_ctx.subtpls_buf, (int *)&g_buf);
	if (ret < 0) {
		LOGE("afs_enroll failed, ret = %d.", ret);
		return ERROR_ENROLL_ALGO_FAIL;
	}

	sen_ctx.enroll_steps++;
	LOGD("step = %d", sen_ctx.enroll_steps);

	LOGD("end.");
        
	return sen_ctx.enroll_steps;
}


int32_t sensor_do_enroll_finish(void)
{
	int32_t ret = 0;
	
	LOGD("start.");

	//fid_show_status(&sen_ctx.tpls_mngr, sen_ctx.enroll_fid);

	if (sen_ctx.enroll_steps && (sen_ctx.enroll_fid != FINGER_NUM)) {
		sensor_set_fid_used(sen_ctx.enroll_fid);
	} else {
		LOGE("no such progress");
		ret = ERROR_ENROLL_NO_PROGRESS;
	}
	
	sen_ctx.enroll_steps = 0;
	sen_ctx.enroll_fid = FINGER_NUM;

	LOGD("end.");
        
	return ret;
}


int32_t sensor_do_enroll_cancel(void)
{
	LOGD("start.");

	if (sen_ctx.enroll_steps && (sen_ctx.enroll_fid != FINGER_NUM)) {
		policy_enroll_cancel(sen_ctx.enroll_fid);
	}

	sen_ctx.enroll_steps = 0;
	sen_ctx.enroll_fid = FINGER_NUM;
	
	LOGD("end.");
        
	return 0;
}



int32_t sensor_fid_is_used(uint8_t fid)
{
	if (fid >= FINGER_NUM) {
		LOGE("invalid finger id %d.", fid);
		return ERROR_BAD_PARAMETERS;
	}

	if (sen_ctx.templates.fid_status[fid / 8] & (1 << (fid & 0x7))) {
		LOGD("enroll id %d already used.", fid);
		return 1;
	}

	return 0;
}


int32_t sensor_set_fid_used(uint8_t fid)
{
	if (fid >= FINGER_NUM) {
		LOGE("invalid finger id %d.", fid);
		return -1;
	}

	if (fid_is_used(&sen_ctx.tpls_mngr, fid)) {
		if (!sensor_fid_is_used(fid)) {
			sen_ctx.templates.fid_status[fid / 8] |= 1 << (fid & 0x7);
			//LOGE("--> %x, fid %d.", sen_ctx.templates.fid_status[fid / 8], fid);
			policy_save_templates(GET_KEY_ALL());
		}
	}
	
	return 0;
}


int32_t sensor_remove_fid(uint8_t fid)
{
	if (fid >= FINGER_NUM) {
		LOGE("invalid finger id %d.", fid);
		return ERROR_BAD_PARAMETERS;
	}

	if (fid_clear(&sen_ctx.tpls_mngr, fid)) {
		LOGE("remove finger id %d failed.", fid);
		return ERROR_REMOVE_FID;
	}

	if (sen_ctx.templates.fid_status[fid / 8] & (1 << (fid & 0x7))) {
		sen_ctx.templates.fid_status[fid / 8] &= ~(1 << (fid & 0x7));
		policy_save_templates(GET_KEY_ALL());
	}

	return 0;
}


int32_t sensor_clear_used_fids(void)
{
	int8_t i;

	for (i = 0; i < FINGER_NUM; i++) {
		fid_clear(&sen_ctx.tpls_mngr, i);
	}
	
	policy_clear_matched_record(GET_KEY_ALL());
	sen_ctx.rcrd_cntr = 0;

	memset(sen_ctx.templates.fid_status, 0, sizeof(sen_ctx.templates.fid_status));

	return policy_save_templates(GET_KEY_ALL());
}


int32_t sensor_get_used_fids(void)
{
	int32_t i = 0, n = 0;

	for (i = 0; i < FINGER_NUM; i++) {
		if (sen_ctx.templates.fid_status[i / 8] & (1 << (i & 0x7))) n++;
	}
    
	LOGD("Stored Valid Fids : %d", n);

	return n;
}


int32_t sensor_get_total_fids(void)
{
	return FINGER_NUM;
}


int32_t sensor_reset_templates_storage(void)
{
	sensor_clear_used_fids();
	subtpls_blocks_reset();

	return 0;
}

/* Add by HansHuang 09/28/18 */
int32_t whole_bmpfile_size = 0;
uint8_t *bmp_src_ptr = 0;



int32_t sensor_get_upload_image_buf(uint32_t idx, uint8_t **dst_addr)
{
	uint8_t *raw = g_buf.tmp.u.imgs.buf1;
	uint8_t *src = g_buf.tmp.u.imgs.buf2, *dst = img_file->buf;
	uint32_t size = sizeof(img_file->buf);
	uint32_t start = 0, current = 0;

    
	
	switch (idx) {
	case 0:		// flashÖÐ±³¾°Í¼
		LOGE("-->  Upload Flash backgroud image.");
		src = sen_ctx.prop.bkg;
		img_file->w = IMAGE_W;
		img_file->h = IMAGE_H;
		break;
		
	case 1:		// È¥±³¾°Í¼
		LOGE("-->  Upload Realtime rmv_bkg image.");
		
		if (get_proper_image( raw, IMAGE_SIZE )) {
			LOGE("get_proper_image error.");
			return 0;
		}
		fp121_rmvBkg_Expand( raw, sen_ctx.prop.bkg, src, g_buf.tmp.u.proc_buf, g_buf.tmp.limit_buf );
		memcpy(g_buf.tmp.u.imgs.buf1, src, IMAGE_SIZE);

		src = g_buf.tmp.u.imgs.buf1;
		img_file->w = IMAGE_W;
		img_file->h = IMAGE_H;
		break;
		
	case 2:		// ÊµÊ±Í¼
		LOGE("-->  Upload Realtime Capture image.");

    
		if (get_proper_image(raw, IMAGE_SIZE)) {
			LOGE("get_proper_image error.");
			return 0;
		}

		src = g_buf.tmp.u.imgs.buf1;
		img_file->w = IMAGE_W;
		img_file->h = IMAGE_H;
		break;
		
	case 3:
		*dst_addr = img_file->buf;
		LOGE("-->  Upload Saved BMP: w = %d, h = %d", img_file->w, img_file->h);

		return img_file->size;
		
	case 4:       // °´Ñ¹×¢²áÈ¥±³¾°Í¼
		LOGE("-->  Upload Realtime rmv_bkg image with detect_touch, Please finger left, then touch.");
		
		platform_get_jiffies(&start);
		while (1) {
			platform_get_jiffies(&current);
			if (current - start >= 5000) {
				LOGE("failed, time out, Finger Touched, please moveout.");
				return 0;
			}

			if (sensor_detect_touch() == NORMAL_UNPRESSED) break;
		}
		
		platform_get_jiffies(&start);
		while (1) {
			platform_get_jiffies(&current);
			if (current - start >= 5000) {
				LOGE("failed, time out, no finger touch detected, please touch.");
				return 0;
			}

			if (sensor_detect_touch() == NORMAL_PRESSED) break;
		}

		if (sensor_get_enroll_image() < 0) return 0;

		src = g_buf.tmp.u.imgs.buf1;
		img_file->w = IMAGE_W;
		img_file->h = IMAGE_H;
		break;

	case 5:       // °´Ñ¹Æ¥ÅäÈ¥±³¾°Í¼
		LOGE("-->  Upload Realtime image with detect_touch, Please finger left, then touch.");
		
		platform_get_jiffies(&start);
		while (1) {
			platform_get_jiffies(&current);
			if (current - start >= 5000) {
				LOGE("failed, time out, Finger Touched, please moveout.");
				return 0;
			}

			if (sensor_detect_touch() == NORMAL_UNPRESSED) break;
		}
		
		platform_get_jiffies(&start);
		while (1) {
			platform_get_jiffies(&current);
			if (current - start >= 5000) {
				LOGE("failed, time out, no finger touch detected, please touch.");
				return 0;
			}

			if (sensor_detect_touch() == NORMAL_PRESSED) break;
		}

		if (sensor_get_auth_image() < 0) return 0;

		src = g_buf.tmp.u.imgs.buf1;
		img_file->w = IMAGE_W;
		img_file->h = IMAGE_H;
		break;
		
	default:
		return 0;
	}

	*dst_addr = dst;
	LOGD("-->  Upload BMP: w = %d, h = %d", img_file->w, img_file->h);



	whole_bmpfile_size = save_bmp_file(dst, size, src, img_file->w, img_file->h);
    bmp_src_ptr = dst;


    LOGE("whole bmpfile size:%d",whole_bmpfile_size);
    return whole_bmpfile_size;

}


int32_t sensor_get_upload_template_buf(uint32_t idx, uint8_t **dst_addr)
{
	// to do later
	return 0;
}


int32_t sensor_save_download_template(uint8_t idx)
{
	// to do later
	return 0;
}


int32_t save_as_img(char *name, uint8_t *buf, int32_t width, int32_t hight)
{
	memset(img_file->buf, 0, sizeof(img_file->buf));

	LOGD("==> Save %s, buffer : %x, and uploading...", name, buf);

	img_file->size = save_bmp_file(img_file->buf, sizeof(img_file->buf), buf, width, hight);
	img_file->w = width;
	img_file->h = hight;

	extern U8 SendData(U8 *name,U8 *buf,U32 len);
	SendData((U8 *)name, img_file->buf, img_file->size);

	return img_file->size;
}



int32_t sensor_get_download_buf(uint8_t *name, uint8_t **dst_addr, uint32_t size)
{
	uint32_t sz = 0;

	LOGD("--> Name : %s", name);

	*dst_addr = img_file->buf;
	sz = sizeof(img_file->buf);
	
	if (size > sz) {
		LOGD("!!!!! download buffer is too small %d, %d, Please Check. !!!!!",
		     size, sz);
		*dst_addr = 0;
		return 0;
	}
	
	strncpy((char *)img_file->name, (char *)name, 99);
	img_file->name[99] = '\0';

	return sizeof(img_file->buf);
}


int32_t sensor_save_download_buf(void)
{
	uint8_t *raw = g_buf.tmp.u.imgs.buf2;
	uint8_t *src = g_buf.tmp.u.imgs.buf1;
	uint8_t *p;
	int32_t ret = 0, idx = 0, id;

	LOGD("--> Name : %s", img_file->name);

	p = (uint8_t *)strstr((char *)img_file->name, "template");
	if (p != NULL) {
		idx = 1;
		id = atoi((char *)(p + 8));
	}
	
	switch (idx) {
	case 0:
		ret = load_bmp_body(img_file->buf, raw, IMAGE_SIZE, &img_file->w, &img_file->h);
		LOGE("%d : Save %s @ raw_image, size %d, %s.",
		     idx, img_file->name, img_file->w * img_file->h, (ret < 0)? "failed" : "OK");
		LOGE("BMP : w = %d, h = %d.", img_file->w, img_file->h);

		if (ret < 0) {
			LOGE("Load Image Failed.");
			return ERROR_BAD_IMAGE;
		}

		if (strstr((char *)img_file->name, "background.bmp")) { // save to flash
			policy_save_bkg(raw, IMAGE_SIZE, GET_KEY_ALL());
		} else {
			fp121_rmvBkg_Expand( raw, sen_ctx.prop.bkg, src, g_buf.tmp.u.proc_buf, g_buf.tmp.limit_buf);

			if ( afs_extract( (int *)src, (int *)&sen_ctx.subtpls_buf[0], (int *)&g_buf ) ) {
				LOGE("afs_extract failed.");
				return ERROR_BAD_IMAGE;
			}
		}

		break;
	case 1:
		sensor_save_download_template(id);
		return 0;
	default:
		LOGE("%d : unknown idx.", idx);
	}
	
	return 0;
}


int32_t sensor_enroll_image_testing(uint8_t enroll_id)
{
	int32_t ret = 0;
	
	ret = sensor_get_enroll_steps(enroll_id);
	if (ret < 0) {
		LOGE("Invalid enroll_id %d", enroll_id);
		return ERROR_ENROLL_LAST_INCOMPLETE;                   // failed, incompletemt last enrollment
	}

	if (ret >= TEMPLATE_NUM) {
		LOGE("Invalid enrollment : ID %d, step %d, exceed %d.",
		     enroll_id, ret, TEMPLATE_NUM);
		return ERROR_ENROLL_EXCEED_MAX_TIMES;                  // failed, exceed max times
	}

	SET_KEY_0();
	
	ret = sensor_do_enroll();
	if (ret <= 0) {
		LOGE("failed, ret = %d.", ret);                        // failed, ERROR_BAD_IMAGE, ERROR_ENROLL_ALGO_FAIL
		return ret;
	}

	return ret;	// done, steps
}


int32_t sensor_authenticate_image_testing(void)
{
	int32_t ret = 0;

	SET_KEY_0();
        
	ret = sensor_do_authenticate(AUTH_THRESHOLD, 0xff);

	return ret;
}


int32_t sensor_characteristic_extract(uint8_t buff_id)
{
	uint8_t *src = g_buf.tmp.u.imgs.buf1;

	if (buff_id >= SUBTPL_BUF_NUM) {
		LOGE("invalid buffer id %d.", buff_id);
		return ERROR_BAD_PARAMETERS;
	}

	if (sensor_get_grey(src, IMAGE_SIZE) < 10000) {
		LOGE("invalid image.");
		return ERROR_INVALID_IMAGE;
	}

	if (afs_extract( (int *)src, (int *)&sen_ctx.subtpls_buf[buff_id], (int *)&g_buf)) {
		LOGE("afs_extract failed");
		return ERROR_BAD_IMAGE;
	}

	memset(src, 0, IMAGE_SIZE);
		
	return 0;
}


int32_t sensor_clear_temp_buffers(void)
{
	int32_t i;
	
	memset(&g_buf, 0, sizeof(g_buf));

	for (i = 0; i < SUBTPL_BUF_NUM; i++) {
		memset(&sen_ctx.subtpls_buf[i], 0, sizeof(sen_ctx.subtpls_buf[i]));
	}
	
	return 0;
}


int32_t sensor_save_finger_template(uint8_t fid)
{
	int32_t amnt = 0, i;
	uint8_t src[8] = {0};
	
	//SET_KEY_0();
	SET_KEY_1();
	SET_KEY_2();

	if (sensor_fid_is_used(fid)) {
		LOGE("fid is used.");
		return ERROR_BAD_PARAMETERS;
	}

	for (i = 0; i < SUBTPL_BUF_NUM; i++) {
		if (memcmp(&sen_ctx.subtpls_buf[i], src, 8) &&
		    (policy_save_subtpl(fid, amnt++, &sen_ctx.subtpls_buf[i]) < 0)) {
			LOGE("save_subtpl %d failed.", i);
			return ERROR_ENROLL_SAVE_TEMPLATE;
		}

		SET_KEY_0();
		SET_KEY_1();
		SET_KEY_2();
	}

	if (sensor_set_fid_used(fid)) {
		LOGE("save template failed.");
		return ERROR_ENROLL_SAVE_TEMPLATE;
	}

	return 0;
}


