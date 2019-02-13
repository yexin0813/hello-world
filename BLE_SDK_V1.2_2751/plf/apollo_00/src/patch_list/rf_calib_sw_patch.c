#include <stdint.h>
#include <stdbool.h>
#include "rf_calib_sw_patch.h"
#include "reg_ble_em_cs.h"
#include "awo.h"
#include "patch.h"
#include "log.h"

#define TARGET_BOARD_FIB3_HUAMI_PATCH
//#define TARGET_BOARD_FIB3
//#define TARGET_BOARD_FIB2
//#define TARGET_BOARD_PACKAGE_NOFIB
//#define TARGET_BOARD_GENERAL

#undef SW_HOPPING_STRUCT


#ifdef TARGET_BOARD_FIB2
#define SW_HOPPING_STRUCT sw_hopping_struct_fib2
static struct Rf_calib_sw_patch_struct sw_hopping_struct_fib2 =
{
	.rf_reg_b = 0x00005500,
	.rf_reg_e = 0x38ffffff,
	.mdm_reg_0x20110044 = 0x912,
	.mdm_reg_0x20110048 = 0x912,
	.mdm_reg_0x2011004c = 0x891,
	.mdm_reg_0x20110050 = 0x8d2,
	.rf_reg_f = {
        0x02ec0000,
        0x02ec0000,
        0x02ec0000,
        0x02ec0000,
        0x02ec0000,
        0x02e40000,
        0x02e40000,
        0x02e40000,
        0x02dc0000,
        0x02d40000,
        0x02d40000,
        0x02d40000,
        0x02d40000,
        0x02d40000,
        0x02d40000,
        0x02cc0000,
        0x02cc0000,
        0x02cc0000,
        0x02c40000,
        0x02c40000,
        0x02c40000,
        0x02b40000,
        0x02b40000,
        0x02b40000,
        0x02b40000,
        0x02b40000,
        0x02b40000,
        0x02b40000,
        0x02a40000,
        0x02a40000,
        0x02a40000,
        0x02a40000,
        0x02a40000,
        0x02a40000,
        0x02a40000,
        0x02940000,
        0x02940000,
        0x02940000,
        0x02940000,
        0x028c0000	
	}
};

#elif defined TARGET_BOARD_FIB3
#define SW_HOPPING_STRUCT sw_hopping_struct_fib3

static struct Rf_calib_sw_patch_struct sw_hopping_struct_fib3 =
{
	.rf_reg_b = 0x00005500,
	.rf_reg_e = 0x38ffffff,
	.mdm_reg_0x20110044 = 0x912,
	.mdm_reg_0x20110048 = 0x952,
	.mdm_reg_0x2011004c = 0x912,
	.mdm_reg_0x20110050 = 0x8d1,
	.rf_reg_f = {
        0x02ec0000,
        0x02ec0000,
        0x02ec0000,
        0x02ec0000,
        0x02e40000,
        0x02e40000,
        0x02e40000,
        0x02dc0000,
        0x02d40000,
        0x02d40000,
        0x02d40000,
        0x02d40000,
        0x02d40000,
        0x02cc0000,
        0x02cc0000,
        0x02cc0000,
        0x02c40000,
        0x02c40000,
        0x02c40000,
        0x02b40000,
        0x02b40000,
        0x02b40000,
        0x02b40000,
        0x02b40000,
        0x02b40000,
        0x02b40000,
        0x02a40000,
        0x02a40000,
        0x02a40000,
        0x02a40000,
        0x02a40000,
        0x02a40000,
        0x02a40000,
        0x02940000,
        0x02940000,
        0x02940000,
        0x02940000,
        0x02ec0000,			//37
        0x02d40000,			//38
        0x028c0000			//39
		
	}
};
#elif defined TARGET_BOARD_FIB3_HUAMI_PATCH
#define SW_HOPPING_STRUCT sw_hopping_struct_fib_huami_patch

static struct Rf_calib_sw_patch_struct sw_hopping_struct_fib_huami_patch =
{
    .rf_reg_b = 0x00005500,
    .rf_reg_e = 0x38ffffff,
    .mdm_reg_0x20110044 = 0x912,
    .mdm_reg_0x20110048 = 0x952,
    .mdm_reg_0x2011004c = 0x912,
    .mdm_reg_0x20110050 = 0x8d1,
    .rf_reg_f = {
            0x02c46db1,
            0x02c46db1,
            0x02bc6db1,
            0x02bc6db1,
            0x02bc6db1,
            0x02b46db1,
            0x02b46db1,
            0x02b46db1,
            0x02b46db1,
            0x02b46db1,
            0x02ac6db1,
            0x02ac6db1,
            0x02ac6db1,
            0x02a46db1,
            0x02a46db1,
            0x02a46db1,
            0x02a46db1,
            0x029c6db1,
            0x029c6db1,
            0x029c6db1,
            0x02946db1,
            0x02946db1,
            0x02946db1,
            0x02946db1,
            0x028c6db1,
            0x028c6db1,
            0x028c6db1,
            0x028c6db1,
            0x02846db1,
            0x02846db1,
            0x02846db1,
            0x02846db1,
            0x027c6db1,
            0x027c6db1,
            0x02746db1,
            0x02746db1,
            0x02746db1,
            0x02cc6db1, //37
            0x02ac6db1, //38
            0x02746db1  //39

    }
};
#elif defined TARGET_BOARD_PACKAGE_NOFIB
#define SW_HOPPING_STRUCT sw_hopping_struct_package3_nofib

static struct Rf_calib_sw_patch_struct sw_hopping_struct_package3_nofib =
{
	.rf_reg_b = 0x00005500,
	.rf_reg_e = 0x38ffffff,
	.mdm_reg_0x20110044 = 0x912,
	.mdm_reg_0x20110048 = 0x952,
	.mdm_reg_0x2011004c = 0x912,
	.mdm_reg_0x20110050 = 0x8d1,
	.rf_reg_f = {
	    0x02e40000,
	    0x02ec0000,
	    0x02ec0000,
	    0x02ec0000,
	    0x02ec0000,
	    0x02ec0000,
	    0x02e40000,
	    0x02e40000,
	    0x02dc0000,
	    0x02d40000,
	    0x02d40000,
	    0x02d40000,
	    0x02d40000,
	    0x02d40000,
	    0x02d40000,
	    0x02d40000,
	    0x02cc0000,
	    0x02cc0000,
	    0x02cc0000,
	    0x02c40000,
	    0x02c40000,
	    0x02c40000,
	    0x02b40000,
	    0x02b40000,
	    0x02b40000,
	    0x02b40000,
	    0x02b40000,
	    0x02b40000,
	    0x02b40000,
	    0x02b40000,
	    0x02a40000,
	    0x02a40000,
	    0x02a40000,
	    0x02a40000,
	    0x02a40000,
	    0x02a40000,
	    0x02a40000,
	    0x02940000,
	    0x02940000,
	    0x02940000
	}
};
#elif defined TARGET_BOARD_GENERAL
#define SW_HOPPING_STRUCT sw_hopping_struct_general

static struct Rf_calib_sw_patch_struct sw_hopping_struct_general =
{
	.rf_reg_b = 0x00005500,
	.rf_reg_e = 0x38ffffff,
	.mdm_reg_0x20110044 = 0x912,
	.mdm_reg_0x20110048 = 0x952,
	.mdm_reg_0x2011004c = 0x912,
	.mdm_reg_0x20110050 = 0x8d1,
	.rf_reg_f = {
	    0x02ec0000,
	    0x02ec0000,
	    0x02ec0000,
	    0x02ec0000,
	    0x02ec0000,
	    0x02ec0000,
	    0x02e40000,
	    0x02e40000,
	    0x02dc0000,
	    0x02d40000,
	    0x02d40000,
	    0x02d40000,
	    0x02d40000,
	    0x02d40000,
	    0x02d40000,
	    0x02d40000,
	    0x02cc0000,
	    0x02cc0000,
	    0x02cc0000,
	    0x02c40000,
	    0x02c40000,
	    0x02c40000,
	    0x02b40000,
	    0x02b40000,
	    0x02b40000,
	    0x02b40000,
	    0x02b40000,
	    0x02b40000,
	    0x02b40000,
	    0x02b40000,
	    0x02a40000,
	    0x02a40000,
	    0x02a40000,
	    0x02a40000,
	    0x02a40000,
	    0x02a40000,
	    0x02a40000,
	    0x02940000,
	    0x02940000,
	    0x02940000
	}
};
#else
#error "target board  should be configured!!!"
#endif



static void rf_calib_params_set(int16_t channel)
{
    //awo_rf_reg(0xf,reg_f_fib3[channel]);
    awo_rf_reg(0xf,SW_HOPPING_STRUCT.rf_reg_f[channel]);
}
static bool check_ch_good(uint8_t ch,uint32_t llchmap_l,uint8_t llchmap_h)
{
    
    uint32_t mask;
    bool ch_good;
    if(ch<32)
    {
        mask = 1<<ch;
        ch_good = mask&llchmap_l;
    }else
    {
        mask = 1<<(ch-32);
        ch_good = mask&llchmap_h;
    }
    return ch_good;
}
/* patch this func to 0x000164c4 */
void rf_calib_sw_patch(struct lld_evt_tag* evt)
{

    lld_pdu_tx_prog(evt); //execute the original code in this address
       
    uint16_t conhdl = evt->conhdl;
    int16_t nb_inc = 1;

    uint16_t hopcntl;
    uint16_t last_ch;
    uint16_t hop;
    int16_t next_ch;

    if(conhdl != LLD_ADV_HDL)
    {
    /* The following code is copied from lld_evt_channel_next() */  
      
        // Get hopping information from the CS
        hopcntl = ble_hopcntl_get(conhdl);
        // Get last channel
        last_ch  = hopcntl & BLE_CH_IDX_MASK;
        // Get hopping value
        hop     = (hopcntl & BLE_HOP_INT_MASK) >> BLE_HOP_INT_LSB;
        // Perform requested number of hopping
        next_ch  = (last_ch + (nb_inc * hop)) % 37;

        if (next_ch < 0)
        {
            next_ch += 37;
        }
    /* End */

        uint32_t llchmap_l = (ble_chmap1_get(conhdl)<<16) | ble_chmap0_get(conhdl);
        uint8_t llchmap_h = ble_llchmap3_getf(conhdl);
        uint8_t nbchgood = ble_nbchgood_getf(conhdl);
        if(nbchgood&&!check_ch_good(next_ch,llchmap_l,llchmap_h))
        {
            uint8_t remapping_index = next_ch % nbchgood;
            uint8_t i=0;
            uint8_t j=0;
            while(i!=remapping_index+1)
            {
                if(check_ch_good(j++,llchmap_l,llchmap_h))
                {
                    ++i;
                }
            }
            next_ch = j - 1;
        }
    }
    else
    {
        if(evt->mode == LLD_EVT_TEST_MODE)
        {
            next_ch = ble_ch_idx_getf(LLD_ADV_HDL);
        }else
        {
            next_ch = 37;
        }
    }
    rf_calib_params_set(next_ch);
    if(ble_format_getf(LLD_ADV_HDL)==LLD_TXTEST_MODE)
    {
        *(uint32_t *)0x202010ac &= 0xfdffffff; // rf reg f auto band
    }
//    LOG(LOG_LVL_INFO,"ch_calc:%d",next_ch);
}

void lld_evt_end_print_ch(struct ea_elt_tag *elt)
{
    lld_evt_end(elt);
//    struct lld_evt_tag *evt = LLD_EVT_ENV_ADDR_GET(elt);
//    LOG(LOG_LVL_INFO,",ch:%d\n",ble_ch_idx_getf(evt->conhdl));
}

void patch_hopping()
{
    uint32_t patch_addr = 0x000164c4+1;     //thumb 2 code should +1 
    static uint32_t patch1_jmp = 0;
    awo_rf_reg(0xb,SW_HOPPING_STRUCT.rf_reg_b);
    awo_rf_reg(0xe,SW_HOPPING_STRUCT.rf_reg_e);
    
    *( ( volatile uint32_t * )  0x20110044 ) = SW_HOPPING_STRUCT.mdm_reg_0x20110044;
    *( ( volatile uint32_t * )  0x20110048 ) = SW_HOPPING_STRUCT.mdm_reg_0x20110048;
    *( ( volatile uint32_t * )  0x2011004c ) = SW_HOPPING_STRUCT.mdm_reg_0x2011004c;
    *( ( volatile uint32_t * )  0x20110050 ) = SW_HOPPING_STRUCT.mdm_reg_0x20110050;

    volatile uint32_t code;
	patch1_jmp = cal_patch_bl((uint32_t)patch_addr,(uint32_t)rf_calib_sw_patch);                 // test1 mod 4 == 0 use +4
    patch_entrance_exit_addr(0,(uint32_t)patch_addr,patch1_jmp);			//  bl

    patch_enable_set(patch_status_get() | (1<<0));
}

void patch_hopping_huami(void)
{
    *(uint32_t *)0x20201094 = 0x070e3480; // rf_reg_9 rx filter bandwitdh
    uint32_t patch_addr = 0x00016798+1;     //thumb 2 code should +1
    static uint32_t patch1_jmp = 0;

    patch1_jmp = cal_patch_bl((uint32_t)patch_addr,(uint32_t)rf_calib_sw_patch);                 // test1 mod 4 == 0 use +4
    patch_entrance_exit_addr(0,(uint32_t)patch_addr,patch1_jmp);            //  bl

    patch_enable_set(patch_status_get() | (1<<0));
}

void patch_channel_all37()
{
    volatile uint32_t code;
    code = 0xf0002000;                 //MOVS     r0,#0   = 2000
    patch_entrance_exit_addr(1,0x0001804c,code);
    patch_enable_set(patch_status_get() | (1<<1));

}

void patch_hopping_log()			// use patch 2 & 3
{
	uint32_t patch_addr = 0x0001583a+1;		//thumb 2 code should +1
	uint32_t patch_jmp = 0;

	patch_jmp = cal_patch_bl((uint32_t)patch_addr,(uint32_t)lld_evt_end_print_ch);                 // test1 mod 4 == 0 use +4

	patch_entrance_exit_addr(2,(uint32_t)patch_addr-2,(((uint32_t)patch_jmp & 0xffff)<<16) | 0x4620 );
	patch_enable_set(patch_status_get() | (1<<2));
	patch_entrance_exit_addr(3,(uint32_t)patch_addr+2,(((uint32_t)patch_jmp & 0xffff0000)>>16) | ((uint32_t)0xbd70<<16) );
	patch_enable_set(patch_status_get() | (1<<3));
}
