#ifndef __MAFP_DEFS_H__
#define __MAFP_DEFS_H__

#include <stdio.h>
#include <string.h>
#include "m_uart.h"
#include "config.h"


typedef unsigned char           uint8_t;
typedef unsigned short          uint16_t;
typedef unsigned int            uint32_t;       
typedef signed char             int8_t;
typedef short                   int16_t;
typedef int                     int32_t;
typedef unsigned long long      uint64_t;


enum {
	NORMAL_PRESSED   = 1,
	NORMAL_UNPRESSED = 0,

	ERROR_BASE       = -10000,
	ERROR_BAD_PARAMETERS,
	
	ERROR_LOAD_PROPERTIES,
	ERROR_INIT_TEMPLATES,
	ERROR_INIT_ALGO,
	ERROR_CHIP_ID,
	ERROR_CHIP_IDENTIFY,
	ERROR_MODE_SWITCH,
	ERROR_MODE_NO_DETECT,
	ERROR_CALC_INIT_PARAM,
	ERROR_CALC_INT_PARAM,

	ERROR_GET_IMAGE,
	ERROR_BAD_IMAGE,
	ERROR_INVALID_IMAGE,
	ERROR_CHECK_MOVEMENT,
	
	ERROR_SPI_READ,
	ERROR_SPI_WRITE,

	ERROR_AUTH_NO_TOUCH,
	ERROR_AUTH_ALGO_FAIL,
	ERROR_AUTH_NO_MATCH,
	
	ERROR_ENROLL_LAST_INCOMPLETE,
	ERROR_ENROLL_ALGO_FAIL,
	ERROR_ENROLL_NO_TOUCH,
	ERROR_ENROLL_EXCEED_MAX_TIMES,
	ERROR_ENROLL_NO_PROGRESS,
	ERROR_ENROLL_SAVE_TEMPLATE,

	ERROR_REMOVE_FID,
	ERROR_CLEAR_FIDS,
};


#define AUTH_THRESHOLD                      (3000)
					    
#define DETECT_MODE_ENABLE                  0
					    
#define GRAY_CODE_VERIFICATION              0


#if  (GRAY_CODE_VERIFICATION == 1)
#define platform_create                     mafp_0001
#define platform_destroy                    mafp_0002
#define platform_fp_rw_reg                  mafp_0003
#define platform_fp_set_mode                mafp_0004
#define platform_fp_read_line               mafp_0005
#define platform_fp_read_frame              mafp_0006
#define platform_msleep                     mafp_0007
#define platform_fs_read                    mafp_0008
#define platform_fs_write                   mafp_0009
#define platform_fs_page_size               mafp_0010
#define platform_fs_erase_page              mafp_0011
#define platform_fs_write_dword             mafp_0012
#define policy_init_properties              mafp_0013
#define policy_save_properties              mafp_0014
#define policy_clear_properties             mafp_0015
#define policy_load_bkg                     mafp_0016
#define policy_save_bkg                     mafp_0017
#define policy_init_templates               mafp_0018
#define policy_save_templates               mafp_0019
#define policy_load_subtpl_header           mafp_0020
#define policy_load_subtpl_body             mafp_0021
#define policy_load_subtpl                  mafp_0022
#define policy_save_subtpl                  mafp_0023
#define policy_get_subtpls_amount           mafp_0024
#define policy_get_subtpl_tid               mafp_0025
#define policy_get_subtpl_write_times       mafp_0026
#define policy_init_matched_record          mafp_0027
#define policy_clear_matched_record         mafp_0028
#define policy_save_matched_fid             mafp_0029
#define policy_load_latest_matched          mafp_0030
#define policy_enroll_cancel                mafp_0031
#define policy_subtpls_exchange             mafp_0032
#define policy_subtpl_2coolarea             mafp_0033
#define sensor_hw_get_register              mafp_0034
#define sensor_hw_set_register              mafp_0035
#define sensor_hw_get_chipid                mafp_0036
#define sensor_hw_read_line                 mafp_0037
#define sensor_hw_read_image                mafp_0038
#define sensor_hw_set_mode                  mafp_0039
#define fp121_gray_identify                 mafp_0040
#define sensor_create                       mafp_0041
#define sensor_destroy                      mafp_0042
#define sensor_capture_mode                 mafp_0043
#define sensor_powerdown_mode               mafp_0044
#define sensor_detect_mode                  mafp_0045
#define sensor_get_chipid                   mafp_0046
#define sensor_calc_init_parameters         mafp_0047
#define sensor_is_para_calibrated           mafp_0048
#define sensor_is_interrupt_calibrated      mafp_0049
#define sensor_set_interrupt_calibrated     mafp_0050
#define sensor_reset_interrupt_parameters   mafp_0051
#define sensor_calc_interrupt_parameters    mafp_0052
#define sensor_detect_touch                 mafp_0053
#define sensor_get_energy                   mafp_0054
#define sensor_get_auth_image               mafp_0055
#define sensor_get_enroll_image             mafp_0056
#define sensor_do_authenticate              mafp_0057
#define sensor_do_authenticate_finish       mafp_0058
#define sensor_get_enroll_steps             mafp_0059
#define sensor_do_enroll                    mafp_0060
#define sensor_do_enroll_finish             mafp_0061
#define sensor_do_enroll_cancel             mafp_0062
#define sensor_fid_is_used                  mafp_0063
#define sensor_set_fid_used                 mafp_0064
#define sensor_remove_fid                   mafp_0065
#define sensor_clear_used_fids              mafp_0066
#define sensor_get_used_fids                mafp_0067
#define sensor_get_total_fids               mafp_0068
#define sensor_get_upload_image_buf         mafp_0069
#define sensor_get_upload_template_buf      mafp_0070
#define sensor_get_download_buf             mafp_0071
#define sensor_save_download_buf            mafp_0072
#define sensor_clear_properties             mafp_0073
#define save_as_img                         mafp_0074
#define sensor_get_grey                     mafp_0075
#define sensor_bkg_check_num                mafp_0076
#define sensor_bkg_check                    mafp_0077
#define sensor_calc_greyoffset              mafp_0078
#define sensor_is_exposed                   mafp_0079
#define tpls_mngr_init                      mafp_0080
#define fidtpl_read_subtpl_header           mafp_0081
#define fidtpl_read_subtpl_body             mafp_0082
#define fidtpl_read_subtpl                  mafp_0083
#define fidtpl_update_subtpl                mafp_0084
#define fidtpl_get_subtpls_amount           mafp_0085
#define fidtpl_get_subtpl_tid               mafp_0086
#define fidtpl_get_subtpl_writetimes        mafp_0087
#define fids_get_total_used                 mafp_0088
#define fid_is_used                         mafp_0089
#define fid_clear                           mafp_0090
#define fids_get_sorted_list                mafp_0091
#define tids_get_sorted_list                mafp_0092
#define subtpls_exchange                    mafp_0093
#define subtpls_coolarea_get                mafp_0094
#define subtpl_get_write_times              mafp_0095
#define subtpls_show_write_times            mafp_0096
#define fid_show_status                     mafp_0097
#define platform_rand_with_seed             mafp_0098
#define fp121_current_line_identify         mafp_0099
#define fp121_image_mark_identify           mafp_0100

#define afs_extract                         mafp_90000
#define afs_enroll                          mafp_90001
#define afs_match                           mafp_90002

#define fp121_rmvBkg_Expand                 mafp_1000100
#define fp121_tell_bkg                      mafp_1000101
#define calCorrFull                         mafp_1000102
#define calCorrList                         mafp_1000103
#define getTouchArea                        mafp_1000104
#define checkPress                          mafp_1000105


#endif


extern uint32_t flash_key;

#define SET_KEY_0()                         {flash_key |= getKeyLL();}
#define SET_KEY_1()                         {flash_key |= getKeyLH() << 8;}
#define SET_KEY_2()                         {flash_key |= getKeyHL() << 16;}
#define SET_KEY_3()                         {flash_key |= getKeyHH() << 24;}
						
#define GET_KEY_ALL()                       EFLASH_OPERATION_KEY
#define GET_KEY()                           flash_key
						
#define CLEAR_KEY()                         {flash_key = 0;}


extern void fsmDebugPrint(const char *fmt, ...);

#define LOGE(format, ...) fsmDebugPrint("[%s:%d] " format "\r\n", __func__, __LINE__, ##__VA_ARGS__ )
//#define LOGE(format, ...) debugPrint("[%s:%d] " format "\r\n", __func__, __LINE__, ##__VA_ARGS__ )

#define LOGD(format, ...) //debugPrint("[%s:%d] " format "\r\n", __func__, __LINE__, ##__VA_ARGS__ )


#endif




