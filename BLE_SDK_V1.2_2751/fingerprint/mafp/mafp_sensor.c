#include "mafp_sensor.h"
#include "sensor_sal.h"
#include "policy.h"
#include <string.h>

uint32_t flash_key = 0;

int32_t mafp_init(int32_t *bad_blocks)
{
	int32_t ret = 0;
	
	LOGD("start.");
        
	if (bad_blocks) *bad_blocks = 0;
	
	ret = sensor_create(bad_blocks);
	if (ret < 0) {
		LOGE("sensor_create failed.");                     // failed, mode switch
	}

#if DETECT_MODE_ENABLE == 1
	if (mafp_detect_mode() < 0) {
		LOGE("enter detect_mode failed.");                 // failed, mode switch
		return ERROR_MODE_SWITCH;
	}
#endif

	LOGD("end.");

	return ret;
}


int32_t mafp_fina(void)
{
	LOGD("start.");

	sensor_destroy();

	LOGD("end.");
        
	return 0;
}


int32_t mafp_get_chipid(void)
{
	uint8_t id = 0, ret = 0;
	
	ret = sensor_get_chipid(&id);
	if (ret) {
		LOGE("Get chipid FAILED");
		return ret;
	}
	
	return id;
}


int32_t mafp_interrupt_calibrate(void)
{
#if DETECT_MODE_ENABLE == 1
	int32_t r;

	LOGD("start.");

	r = sensor_detect_touch();
	if (r == NORMAL_UNPRESSED) {
		sensor_reset_interrupt_parameters();
		r = sensor_calc_interrupt_parameters();
	}

	LOGD("end.");

	return r;
#else
	return ERROR_MODE_NO_DETECT;
#endif
}



int32_t mafp_detect_mode(void)
{
#if DETECT_MODE_ENABLE == 1
	return sensor_detect_mode();
#else
	return ERROR_MODE_NO_DETECT;
#endif	
}



int32_t mafp_powerdown_mode(void)
{
	return sensor_powerdown_mode();
}


int32_t mafp_pre_enroll(void)
{
	int32_t ret = 0;

	LOGD("start.");

	ret = sensor_detect_touch();
	if (ret != NORMAL_PRESSED) {
		ret = ERROR_ENROLL_NO_TOUCH;                            // failed, Finger leaved
		goto out;
	}

	ret = sensor_get_enroll_image();
	if (ret < 0) {
		LOGE("get_enroll_image failed.");
		ret = ERROR_GET_IMAGE;                                  // failed, Get Image
		goto out;
	}

#if DETECT_MODE_ENABLE == 1
	if (mafp_detect_mode() < 0) {
		LOGE("enter detect_mode failed.");                     // failed, mode switch
		return ERROR_MODE_SWITCH;
	}
#endif

	ret = sensor_characteristic_extract(0);
	if (ret < 0) {
		LOGE("characteristic_extract failed.");
		return ERROR_BAD_IMAGE;                                // failed, Bad Image
	}
	
	if (sensor_check_movement() < 0) {
		LOGE("No movement with latest.");                      // failed, no movement with latest
		return ERROR_BAD_IMAGE;
	}

	LOGD("end.");
	return 0;

out:
#if DETECT_MODE_ENABLE == 1
	if (mafp_detect_mode() < 0) {
		LOGE("enter detect_mode failed.");                     // failed, mode switch
		return ERROR_MODE_SWITCH;
	}
	platform_msleep(15);
#endif
	return ret;
}


int32_t mafp_enroll(uint8_t enroll_id)
{
	int32_t ret = 0;
	
	LOGD("start.");

	if (enroll_id >= FINGER_NUM) {
		LOGE("invalid enroll id %d", enroll_id);
		return ERROR_BAD_PARAMETERS;
	}

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
		LOGE("failed, ret = %d.", ret);                        // failed, algo failed                    
	}

	LOGD("end.");

	return ret;
}


int32_t mafp_post_enroll(void)
{
	int32_t ret = 0;
	
	LOGD("start.");
        
	ret = sensor_do_enroll_finish();      //  save template

	LOGD("end.");
        
	return ret;
}


int32_t mafp_cancel_enroll(void)
{
	int32_t ret = 0;
	
	LOGD("start.");

	ret = sensor_do_enroll_cancel();
	
	LOGD("end.");

	return ret;
}


int32_t mafp_pre_authenticate(void)
{
	int32_t ret = 0;


	LOGD("start.");




	ret = sensor_detect_touch();

    


    
	if (ret != NORMAL_PRESSED) {
        
		ret =  ERROR_AUTH_NO_TOUCH;
		goto out;
	}
	
	if (sensor_get_auth_image()) {
		LOGE("get_auth_image failed.");
		ret = ERROR_GET_IMAGE;                                 // failed, Get Image
		goto out;
	}

    

#if DETECT_MODE_ENABLE == 1	
	if (mafp_detect_mode() < 0) {
		LOGE("enter detect_mode failed.");                     // failed, mode switch
		return ERROR_MODE_SWITCH;
	}
#endif

    

	ret = sensor_characteristic_extract(0);

    
	if (ret < 0) {
		LOGE("characteristic_extract failed.");
		return ERROR_BAD_IMAGE;                                // failed, Bad Image
	}


    
	if (sensor_check_movement() < 0) {
        
		LOGE("No movement with latest.");                      // failed, no movement with latest
		return ERROR_BAD_IMAGE;                                
	}



	LOGD("end.");

	return 0;

out:
#if DETECT_MODE_ENABLE == 1	
	if (mafp_detect_mode() < 0) {
		LOGE("enter detect_mode failed.");                     // failed, mode switch
		return ERROR_MODE_SWITCH;
	}
	platform_msleep(15);
#endif
	return ret;
}


int32_t mafp_authenticate(uint8_t expected_fid)
{
	int32_t ret = ERROR_AUTH_NO_MATCH;

	LOGD("start.");

	SET_KEY_0();

	ret = sensor_do_authenticate(AUTH_THRESHOLD, expected_fid);

	LOGD("end.");
	
	return ret;                             // finger ID, or NO match, algo failed or bad image
}


int32_t mafp_post_authenticate(void)
{
	int32_t ret = 0;

	ret = sensor_do_authenticate_finish();  // update template if needed
	
	return ret;
}


int32_t mafp_finger_leave(void)
{
	int32_t status = 0, ret = 0;

	LOGD("start.");
        
	ret = sensor_detect_touch();
	if (ret == NORMAL_UNPRESSED) {
		status = 1;	// OK, finger leaved
		goto out;
	}

	LOGD("end.");

out :
#if DETECT_MODE_ENABLE == 1
	if (mafp_detect_mode() < 0) {
		LOGE("enter detect_mode failed.");
		return ERROR_MODE_SWITCH;
	}
	platform_msleep(15);
#endif
	
	return status;
}


int32_t mafp_finger_touch(void)
{
	int32_t status = 0, ret = 0;

	LOGD("start.");
        
	ret = sensor_detect_touch();
	if (ret == NORMAL_PRESSED) {
		status = 1;    // OK, finger touched
		goto out;
	}

	LOGD("end.");

out :
#if DETECT_MODE_ENABLE == 1
	if (mafp_detect_mode() < 0) {
		LOGE("enter detect_mode failed.");
		return ERROR_MODE_SWITCH;
	}
	platform_msleep(15);
#endif
	
	return status;
}


int32_t mafp_get_enrollid_status(uint8_t enroll_id)
{
	int32_t ret = 0;
	
	LOGD("start.");

	if (enroll_id >= FINGER_NUM) {
		LOGE("invalid enroll id %d", enroll_id);
		return ERROR_BAD_PARAMETERS;
	}
	
	ret = sensor_fid_is_used(enroll_id);
	if (ret == 1) {
		LOGD("already exist enroll id %d", enroll_id);
	}
	
	LOGD("end.");

	return ret;
}


int32_t mafp_remove_enrollid(uint8_t enroll_id)
{
	int32_t ret = 0;
	
	LOGD("start.");

	if (enroll_id >= FINGER_NUM) {
		LOGE("invalid enroll id %d", enroll_id);
		return ERROR_BAD_PARAMETERS;
	}
	
	ret = sensor_remove_fid(enroll_id);	
	if (ret) {
		LOGE("remove id %d failed, ret = %d", enroll_id, ret);
		return ERROR_REMOVE_FID;
	}
	
	LOGD("end.");

	return 0;
}


int32_t mafp_clear_all_enrollids(void)
{
	int32_t ret = 0;
	
	ret = sensor_clear_used_fids();
	if (ret) {
		LOGE("clear fids failed, ret = %d", ret);
		return ERROR_CLEAR_FIDS;
	}
	
	return 0;
}


int32_t mafp_get_used_enrollids(void)
{
	int32_t ret = 0;
	
	LOGD("start.");
	
	ret = sensor_get_used_fids();
	
	LOGD("end.");

	return ret;
}


int32_t mafp_get_total_enrollids(void)
{
	return sensor_get_total_fids();
}


int32_t mafp_reset_templates_storage(void)
{
	int32_t ret = 0;
	
	LOGD("start.");
	
	ret = sensor_reset_templates_storage();

	LOGD("end, ret = %d.", ret);

	return ret;
}


int32_t mafp_sensor_parameters_reinit(void)
{
	int32_t ret = 0;
	
	ret = sensor_reinit_parameters();
	if (ret < 0) {
		LOGE("reinit parameters failed.");                 // failed, mode switch
	}

#if DETECT_MODE_ENABLE == 1
	if (mafp_detect_mode() < 0) {
		LOGE("enter detect_mode failed.");                 // failed, mode switch
		return ERROR_MODE_SWITCH;
	}
#endif

	return ret;
}


int32_t mafp_factory_init(void)
{
	if (sensor_clear_used_fids()) {
		LOGE("clear fids failed");
		return ERROR_CLEAR_FIDS;
	}

	return mafp_sensor_parameters_reinit();
}


///////////////////////////////////// for SYNO protocol /////////////////////////////////////

int32_t mafp_capture_image(void)
{
	int32_t ret = 0;

	LOGD("start.");

	ret = sensor_detect_touch();
	if (ret != NORMAL_PRESSED) {
		ret = ERROR_ENROLL_NO_TOUCH;                            // failed, Finger leaved
		goto out;
	}

	ret = sensor_capture_image();
	if (ret < 0) {
		LOGE("capture_image failed.");
		ret = ERROR_GET_IMAGE;                                  // failed, Get Image
		goto out;
	}

out:
#if DETECT_MODE_ENABLE == 1
	if (mafp_detect_mode() < 0) {
		LOGE("enter detect_mode failed.");                      // failed, mode switch
		return ERROR_MODE_SWITCH;
	}
	platform_msleep(15);
#endif

	LOGD("end.");

	return ret;
}


// buff_id : 0 ~ SUBTPL_BUF_NUM - 1
int32_t mafp_extract_characteristic(uint8_t buff_id)
{
	int32_t ret = 0;

	LOGD("start.");
	
	ret = sensor_characteristic_extract(buff_id);
	if (ret < 0) {
		LOGE("characteristic_extract failed.");
	}

	LOGD("end.");
	
	return ret;
}


int32_t mafp_enroll_save_template(uint8_t fid)
{
	int32_t ret = 0;
	
	LOGD("start.");

	SET_KEY_0();
	
	ret = sensor_save_finger_template(fid);

	if (ret) sensor_do_enroll_cancel();

	sensor_clear_temp_buffers();

	LOGD("end.");
	
	return ret;
}


int32_t mafp_match_range(uint8_t buff_id, uint8_t start, uint8_t end)
{
	int32_t ret = ERROR_AUTH_NO_MATCH; //, i;

	LOGD("start.");

	if ((start > end) || (buff_id >= SUBTPL_BUF_NUM)) {
		LOGE("Params error : buff_id %d, start %d, end %d.", buff_id, start, end);
		ret =  ERROR_BAD_PARAMETERS;
		goto out;
	}

	if (buff_id != 0) {
		memcpy(&sen_ctx.subtpls_buf[0], &sen_ctx.subtpls_buf[buff_id], sizeof(sen_ctx.subtpls_buf[0]));
	}
	
	SET_KEY_0();

	if (sensor_check_movement() < 0) {
		LOGE("No movement with latest.");                // failed, no movement with latest
		ret = ERROR_AUTH_NO_MATCH;
		goto out;
	}

	//if (start == 0xff) {
		ret = sensor_do_authenticate(AUTH_THRESHOLD, 0xff);
	//} else {
	//	for (i = start; i <= end; i++) {
	//		ret = sensor_do_authenticate(AUTH_THRESHOLD, i);
	//		if (ret != ERROR_AUTH_NO_MATCH) break;
	//	}
	//}

out:
	sensor_clear_temp_buffers();

	LOGD("end.");
	
	return ret;                                             // finger ID, or NO match, algo failed
}

///////////////////////////////////// SYNO protocol end /////////////////////////////////////

