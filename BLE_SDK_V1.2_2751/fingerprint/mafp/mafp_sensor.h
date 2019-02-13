#ifndef __MAFP_SENSOR_H__
#define __MAFP_SENSOR_H__

#include "mafp_defs.h"



extern int32_t mafp_init(int32_t *bad_blocks);
extern int32_t mafp_fina(void);

extern int32_t mafp_get_chipid(void);

extern int32_t mafp_interrupt_calibrate(void);

extern int32_t mafp_detect_mode(void);
extern int32_t mafp_powerdown_mode(void);

extern int32_t mafp_pre_enroll(void);
extern int32_t mafp_enroll(uint8_t enroll_id);
extern int32_t mafp_post_enroll(void);
extern int32_t mafp_cancel_enroll(void);

extern int32_t mafp_pre_authenticate(void);
extern int32_t mafp_authenticate(uint8_t expected_fid);
extern int32_t mafp_post_authenticate(void);

extern int32_t mafp_finger_leave(void);
extern int32_t mafp_finger_touch(void);

extern int32_t mafp_get_enrollid_status(uint8_t enroll_id);
extern int32_t mafp_remove_enrollid(uint8_t enroll_id);
extern int32_t mafp_get_used_enrollids(void);
extern int32_t mafp_get_total_enrollids(void);
extern int32_t mafp_clear_all_enrollids(void);
extern int32_t mafp_reset_templates_storage(void);

extern int32_t mafp_sensor_parameters_reinit(void);
extern int32_t mafp_factory_init(void);


////// for SYNO protocol //////

extern int32_t mafp_capture_image(void);
extern int32_t mafp_extract_characteristic(uint8_t buff_id);

extern int32_t mafp_enroll_save_template(uint8_t fid);

extern int32_t mafp_match_range(uint8_t buff_id, uint8_t start, uint8_t end);

////// SYNO protocol end //////

#endif

