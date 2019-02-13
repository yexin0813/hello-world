#ifndef __MAFP_POLICY_H__
#define __MAFP_POLICY_H__

#include "mafp_defs.h"
#include "tpls_mngr.h"


#define MAGIC_MATCHED_RECORD	0x5A5A

typedef struct match_record_s {
	uint16_t magic;
	uint8_t  rsv;
	uint8_t  fid;
} match_record_t;

extern int32_t policy_load_properties(uint8_t *buf, uint32_t size);
extern int32_t policy_save_properties(uint8_t *buf, uint32_t size, uint32_t key);
extern int32_t policy_clear_properties(uint32_t key);

extern int32_t policy_load_bkg(uint8_t **buf, uint32_t size);
extern int32_t policy_save_bkg(uint8_t *buf, uint32_t size, uint32_t key);

extern int32_t policy_init_templates(void);
extern int32_t policy_save_templates(uint32_t key);

extern int32_t policy_load_subtpl_header(uint8_t fid, uint8_t idx, subtpl_t *subtpl);
extern int32_t policy_load_subtpl_body(uint8_t fid, uint8_t idx, subtpl_t *subtpl);
extern int32_t policy_load_subtpl(uint8_t fid, uint8_t idx, subtpl_t *subtpl);
extern int32_t policy_save_subtpl(uint8_t fid, uint8_t idx, subtpl_t *subtpl);
extern int32_t policy_get_subtpls_amount(uint8_t fid);
extern int32_t policy_get_subtpl_updatetimes(uint8_t fid, uint8_t idx, int32_t *val);

extern int32_t policy_init_matched_record(uint8_t *cntr);
extern int32_t policy_clear_matched_record(uint32_t key);
extern int32_t policy_save_matched_fid(uint8_t fid, uint8_t *cntr, uint32_t key);
extern int32_t policy_load_latest_matched(uint8_t fids[16], uint8_t cntr);

extern int32_t policy_enroll_cancel(uint8_t fid);


extern int32_t policy_load_latest_modified_subtpl(subtpl_t *subtpl);

#endif
