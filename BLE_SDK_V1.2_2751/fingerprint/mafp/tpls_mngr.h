#ifndef __TEMPLATES_MANAGER_H__
#define __TEMPLATES_MANAGER_H__

#include "mafp_defs.h"
#include "platform.h"
#include "sysCfg.h"

#define FINGER_NUM_G0                   2
#define SUBTPLS_AMOUNT_G0               12

#define FINGER_NUM_G1                   1
#define SUBTPLS_AMOUNT_G1               3

#define POOL_GROUP_NUM	                2
#define FINGER_NUM                      (FINGER_NUM_G0 + FINGER_NUM_G1)

#define FID_SUBTPLS_MAX_NUM             SUBTPLS_AMOUNT_G0
#define FID_SUBTPLS_MIN_NUM             SUBTPLS_AMOUNT_G1


#define MINIMUM_REQIRED_NUM             (FINGER_NUM_G0 * SUBTPLS_AMOUNT_G0 + FINGER_NUM_G1 * SUBTPLS_AMOUNT_G1)


typedef struct subtpl_attribute_s {
	uint32_t magic : 8;
	uint32_t write_times : 24;

	uint32_t valid_flag;
	
	uint32_t update_times;
	uint16_t rsv;
	uint8_t  fid;
	uint8_t  crc;
} subtpl_attr_t;


#define MAGIC_UPDATED_RECORD                    (0x500A)

typedef struct updated_record_s {
	uint16_t magic;
	uint16_t tid;
} updated_record_t;


#define TEMPLATE_SIZE                           (TEMPLATE_SZ)


///////////////////////////// following calculated automatically ////////////////////////////////

#define TPLS_EFLASH1_SIZE                       (TPLS_EFLASH1_END - TPLS_EFLASH1_START - EFLASH_PAGESIZE)
#define TPLS_EFLASH2_SIZE                       (TPLS_EFLASH2_END - TPLS_EFLASH2_START)

#define SUBTPL_SIZE                             ROUND_UP(TEMPLATE_SIZE, EFLASH1_PAGESIZE)
#define SUBTPLS_TOTAL                           ((TPLS_EFLASH1_SIZE + TPLS_EFLASH2_SIZE) / SUBTPL_SIZE)

#define SUBTPLS_EFLASH1_START                   (TPLS_EFLASH2_END - SUBTPLS_TOTAL * SUBTPL_SIZE)

#define SUBTPL_POS(i)                           (SUBTPLS_EFLASH1_START + (i) * SUBTPL_SIZE)
#define SUBTPL_ATTR(i)                          (SUBTPL_POS(i) + TEMPLATE_SIZE)




#define SUBTPLS_START_ADDR                      (SUBTPLS_EFLASH1_START - EFLASH_PAGESIZE)
#define SUBTPLS_UPDATE_RECORD_TOTAL             (EFLASH_PAGESIZE / 4)

/////////////////////////////// above calculated automatically /////////////////////////////////

#if ((SUBTPLS_AMOUNT_G0 > FID_SUBTPLS_MAX_NUM) || (SUBTPLS_AMOUNT_G1 > SUBTPLS_AMOUNT_G0) ||  (SUBTPLS_AMOUNT_G1 < FID_SUBTPLS_MIN_NUM))
#error "Configuration Wrong, please check. !!!!!"
#endif

#if (SUBTPLS_TOTAL <= MINIMUM_REQIRED_NUM)
#error "Configuration Wrong, too few subtpls.!!!!!"
#endif

#define SUBTPL_MAGIC                            0x55
#define SUBTPL_HEADER_SIZE                      256
#define SUBTPL_VALID_MAGIC                      (0xC4AA217F)


typedef struct fidtpl_s {
	uint32_t amount : 8;
	uint32_t update_times : 24;
	uint16_t subtpls[ FID_SUBTPLS_MAX_NUM ];
} fidtpl_t;


typedef struct subtpl_s {
	uint8_t data[ TEMPLATE_SIZE ];
	subtpl_attr_t attr;
} subtpl_t;


typedef struct tpls_mngr_s {
	fidtpl_t  fids_pool[ FINGER_NUM ];
	uint16_t  fids_used;
	uint8_t   fids_bitsmap[ (FINGER_NUM + 7) / 8 ];
	uint8_t   sorted_fids_list[ FINGER_NUM ];

	uint16_t  subtpls_block_dead;
	uint16_t  subtpls_used;
	uint8_t   subtpls_bitsmap[ (SUBTPLS_TOTAL + 7) / 8 ];

	uint16_t  updated_cntr;
} tpls_mngr_t;

extern int32_t tpls_mngr_init(tpls_mngr_t *tpl_mngr);

extern int32_t fidtpl_read_subtpl_header(tpls_mngr_t *tpl_mngr, uint8_t fid, uint8_t idx, subtpl_t *subtpl);
extern int32_t fidtpl_read_subtpl_body(tpls_mngr_t *tpl_mngr, uint8_t fid, uint8_t idx, subtpl_t *subtpl);
extern int32_t fidtpl_read_subtpl(tpls_mngr_t *tpl_mngr, uint8_t fid, uint8_t idx, subtpl_t *subtpl);
extern int32_t fidtpl_update_subtpl(tpls_mngr_t *tpl_mngr, uint8_t fid, uint8_t idx, subtpl_t *subtpl);

extern int32_t fidtpl_get_subtpls_amount(tpls_mngr_t *tpl_mngr, uint8_t fid);
extern int32_t fidtpl_get_subtpl_tid(tpls_mngr_t *tpl_mngr, uint8_t fid, uint8_t idx, uint16_t *tid);
extern int32_t fidtpl_get_subtpl_writetimes(tpls_mngr_t *tpl_mngr, uint8_t fid, uint8_t idx, uint16_t *tid, uint32_t *write_times);
extern int32_t fidtpl_get_subtpl_updatetimes(tpls_mngr_t *tpl_mngr, uint8_t fid, uint8_t idx, int32_t *val);

extern int32_t fids_get_total_used(tpls_mngr_t *tpl_mngr);
extern int32_t fid_is_used(tpls_mngr_t *tpl_mngr, uint8_t fid);
extern int32_t fid_clear(tpls_mngr_t *tpl_mngr, uint8_t fid);

extern int32_t fids_get_sorted_list(tpls_mngr_t *tpl_mngr, uint8_t sorted_list[ FINGER_NUM ]);
extern int32_t tids_get_sorted_list(tpls_mngr_t *tpl_mngr, uint8_t fid, uint16_t sorted_list[ FID_SUBTPLS_MAX_NUM ]);

extern int32_t subtpls_exchange(tpls_mngr_t *tpl_mngr, uint16_t tid1, uint16_t tid2, subtpl_t *subtpl1, subtpl_t *subtpl2);
extern int32_t subtpls_coolarea_get(void);

extern int32_t subtpl_get_write_times(uint16_t tid);
extern int32_t subtpls_show_write_times(void);

extern int32_t subtpls_blocks_reset(void);

extern int32_t subtpl_load_latest_modified(tpls_mngr_t *tpl_mngr, subtpl_t * subtpl);

extern void fid_show_status(tpls_mngr_t *tpl_mngr, uint8_t fid);

#endif
