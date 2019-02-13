#ifndef __SENSOR_SAL_H__
#define __SENSOR_SAL_H__

#include "mafp_defs.h"
#include "sysCfg.h"
#include "algApi.h"
#include "sensor_hal.h"
#include "tpls_mngr.h"


#define IMAGE_W                 120
#define IMAGE_H                 120
#define IMAGE_SIZE              (IMAGE_W * IMAGE_H)

#define GREY_OFFSET             0xff
                        	
#define RAW_IMAGE_LINE          (IMAGE_W + 1)
#define RAW_IMAGE_SIZE          (RAW_IMAGE_LINE * (IMAGE_H + 1))

                        	
#define DETECT_START_LINE       (IMAGE_H / 2)
#define DETECT_END_LINE         (IMAGE_H / 2)
#define DETECT_CURRENT_LINE     (IMAGE_H / 2)
#define DETECT_LINE_STEP        1
#define DETECT_FRAME_ROW        1

#define AFS121_CHIPID           (0x79)

#define INTERRUPT_DELAY	        1

#define PROPERTY_VERSION        25


#define TEMPLATE_NUM		FID_SUBTPLS_MAX_NUM


#define SUBTPL_BUF_NUM          2


typedef struct sensor_property_s {
	uint8_t ver;

	uint8_t capacity;
	uint8_t refctrl;
	uint8_t volt;
	uint8_t para_calibrated;

	uint8_t a;
	uint8_t n;
	uint8_t int_calibrated;

	int32_t  corr_inited;
	int32_t  corr_full;
	int32_t  corr_list[36];
	
	uint32_t bkg_energy;
	uint8_t *bkg;
} sensor_property_t;


typedef struct templates_s {
	uint32_t subtpls_start;
	uint16_t subtpls_total;
	uint16_t subtpl_size;
	uint8_t  fid_status[ (FINGER_NUM + 7) / 8 ];
} template_t;


typedef struct sensor_context_s {
	sensor_property_t prop;
	template_t templates;
	
	int32_t  match_score;            

	int8_t   enroll_fid;
	int8_t   enroll_steps;           

	int8_t   work_mode;
	int8_t   interrupt_delay_ms;     

	uint8_t  rcrd_cntr;
	uint8_t  touch_area;

	tpls_mngr_t tpls_mngr;

	subtpl_t subtpls_buf[SUBTPL_BUF_NUM];
} sensor_context_t;


typedef struct {
	union {
		int16_t proc_buf[IMAGE_SIZE];
		struct {
			uint8_t buf1[IMAGE_SIZE];
			uint8_t buf2[IMAGE_SIZE];
		} imgs;
	} u;
	int limit_buf[512];
} buf_t;


typedef union {
	buf_t      tmp;
	extractBuf extractProcBuf;        
	matchBuf   matchProcBuf;            
} alg_buf_t;


typedef struct img_file_s {
	int8_t  name[100];
	int32_t size;
	uint8_t buf[IMAGE_SIZE + 1200];
	int32_t w, h;
} img_file_t;


extern sensor_context_t sen_ctx;


extern int32_t sensor_create(int32_t *dead_block_num);
extern int32_t sensor_destroy(void);
extern int32_t sensor_reset_mode(void);
extern int32_t sensor_capture_mode(void);
extern int32_t sensor_powerdown_mode(void);
extern int32_t sensor_detect_mode(void);
extern int32_t sensor_get_chipid(uint8_t *id);
extern int32_t sensor_calc_init_parameters(void);
extern int32_t sensor_is_para_calibrated(void);
extern int32_t sensor_is_interrupt_calibrated(void);
extern int32_t sensor_set_interrupt_calibrated(void);
extern int32_t sensor_reset_interrupt_parameters(void);
extern int32_t sensor_calc_interrupt_parameters(void);
extern int32_t sensor_reinit_parameters(void);
	
extern int32_t sensor_detect_touch(void);
extern int32_t sensor_get_energy(void);

extern int32_t sensor_get_auth_image(void);
extern int32_t sensor_get_enroll_image(void);

extern int32_t sensor_do_authenticate(int32_t threshold, uint8_t expected_fid);
extern int32_t sensor_do_authenticate_finish(void);

extern int32_t sensor_get_enroll_steps(uint8_t user_id);
extern int32_t sensor_do_enroll(void);
extern int32_t sensor_do_enroll_finish(void);
extern int32_t sensor_do_enroll_cancel(void);


extern int32_t sensor_fid_is_used(uint8_t fid);
extern int32_t sensor_set_fid_used(uint8_t fid);
extern int32_t sensor_remove_fid(uint8_t fid);
extern int32_t sensor_clear_used_fids(void);
extern int32_t sensor_get_used_fids(void);
extern int32_t sensor_get_total_fids(void);
extern int32_t sensor_reset_templates_storage(void);


extern int32_t sensor_get_upload_image_buf(uint32_t idx, uint8_t **dst_addr);
extern int32_t sensor_get_upload_template_buf(uint32_t idx, uint8_t **dst_addr);

extern int32_t sensor_get_download_buf(uint8_t *name, uint8_t **dst_addr, uint32_t size);
extern int32_t sensor_save_download_buf(void);

extern int32_t sensor_enroll_image_testing(uint8_t fid);
extern int32_t sensor_authenticate_image_testing(void);

extern int32_t sensor_clear_properties(void);

extern int32_t sensor_check_movement(void);

extern int32_t sensor_clear_temp_buffers(void);

extern int32_t sensor_capture_image(void);
extern int32_t sensor_characteristic_extract(uint8_t buff_id);
extern int32_t sensor_save_finger_template(uint8_t fid);

extern int32_t save_as_img(char *name, uint8_t *buf, int32_t width, int32_t hight);


#endif
