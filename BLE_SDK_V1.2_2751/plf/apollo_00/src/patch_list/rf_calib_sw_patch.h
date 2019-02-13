#ifndef RF_CALIB_SW_PATCH_H_
#define RF_CALIB_SW_PATCH_H_
#include "lld.h"
#include "lld_pdu.h"
#include "lld_evt.h"

struct Rf_calib_sw_patch_struct
{
	uint32_t rf_reg_b;
	uint32_t rf_reg_e;
	uint32_t mdm_reg_0x20110044;
	uint32_t mdm_reg_0x20110048;
	uint32_t mdm_reg_0x2011004c;
	uint32_t mdm_reg_0x20110050;
	uint32_t rf_reg_f[40];
};
void rf_calib_sw_patch(struct lld_evt_tag* evt);
void lld_evt_end_print_ch(struct ea_elt_tag *elt);


void rf_calib_sw_patch(struct lld_evt_tag* evt);
void patch_hopping(void);
void patch_channel_all37(void);
void patch_hopping_log(void);
void patch_hopping_huami(void);

#endif
