
/**
 ****************************************************************************************
 *
 * @file pshare.c
 *
 * @brief pin share driver interface
 *
 * Copyright (C) Apollo
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup pin share
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "bx_dbg.h"
#include "pshare.h"
#include "reg_sysc_cpu.h"
/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * STRUCT DEFINITIONS
 ****************************************************************************************
 */


/*
 * GLOBAL VARIABLE DECLARATION
 ****************************************************************************************
 */



/*
 * LOCAL FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */





/*
 * EXPORTED FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */
 
/**
 * @brief set the function of function I/O
 * @param io_num: choose from FUNC_IO_0 to FUNC_IO_21. Find definition in pshare.h
 * @param idx: choose from IO_UART0_TXD to IO_PWM_4. Find definition in pshare.h
 * @param en: enable the function io setting or not.
 */

#if HW_BX_VERSION == 00
void pshare_funcio_set(uint8_t io_num, uint8_t idx, uint8_t en)
{
	uint32_t enable_map;
	BX_ASSERT((io_num <= 21));
	BX_ASSERT((idx <= 14));
	BX_ASSERT((en == ENABLE) || (en == DISABLE));

	enable_map = sysc_cpu_func_io_en_r_getf();
	if(en == DISABLE)
	{
		enable_map &= (~(1 << io_num));
		sysc_cpu_func_io_en_r_setf(enable_map);
		return;
	}
	switch(io_num)
	{
		case FUNC_IO_0:
			BX_ASSERT(sysc_cpu_spim0_cs1_en_getf() == 0);
			sysc_cpu_func_io00_sel_setf(idx);
			break;
		case FUNC_IO_1:
			BX_ASSERT(sysc_cpu_spim0_en_getf() == 0);
			BX_ASSERT(sysc_cpu_spis_en_getf() == 0);
			sysc_cpu_func_io01_sel_setf(idx);
			break;
		case FUNC_IO_2:
			BX_ASSERT(sysc_cpu_spim0_en_getf() == 0);
			BX_ASSERT(sysc_cpu_spis_en_getf() == 0);
			sysc_cpu_func_io02_sel_setf(idx);
			break;
		case FUNC_IO_3:
			BX_ASSERT(sysc_cpu_spim0_en_getf() == 0);
			BX_ASSERT(sysc_cpu_spis_en_getf() == 0);
			sysc_cpu_func_io03_sel_setf(idx);
			break;
		case FUNC_IO_4:
			BX_ASSERT(sysc_cpu_spim0_en_getf() == 0);
			BX_ASSERT(sysc_cpu_spis_en_getf() == 0);
			sysc_cpu_func_io04_sel_setf(idx);
			break;
		case FUNC_IO_5:
			BX_ASSERT(sysc_cpu_spim1_cs1_en_getf() == 0);
            BX_ASSERT((sysc_cpu_ble_mac_dbg_en_getf() & 0x01) == 0);
			sysc_cpu_func_io05_sel_setf(idx);
			break;
		case FUNC_IO_6:
			BX_ASSERT(sysc_cpu_spim1_en_getf() == 0);
            BX_ASSERT((sysc_cpu_ble_mac_dbg_en_getf() & 0x02) == 0);
			sysc_cpu_func_io06_sel_setf(idx);
			break;
		case FUNC_IO_7:
			BX_ASSERT(sysc_cpu_spim1_en_getf() == 0);
            BX_ASSERT((sysc_cpu_ble_mac_dbg_en_getf() & 0x04) == 0);
			sysc_cpu_func_io07_sel_setf(idx);
			break;
		case FUNC_IO_8:
			BX_ASSERT(sysc_cpu_spim1_en_getf() == 0);
            BX_ASSERT((sysc_cpu_ble_mac_dbg_en_getf() & 0x08) == 0);
			sysc_cpu_func_io08_sel_setf(idx);
			break;
		case FUNC_IO_9:
			BX_ASSERT(sysc_cpu_spim1_en_getf() == 0);
            BX_ASSERT((sysc_cpu_ble_mac_dbg_en_getf() & 0x10) == 0);
			sysc_cpu_func_io09_sel_setf(idx);
			break;
		case FUNC_IO_10:
			BX_ASSERT(sysc_cpu_spim1_en_getf() == 0);
            BX_ASSERT((sysc_cpu_ble_mac_dbg_en_getf() & 0x20) == 0);
			sysc_cpu_func_io10_sel_setf(idx);
			break;
		case FUNC_IO_11:
			BX_ASSERT(sysc_cpu_spim1_en_getf() == 0);
            BX_ASSERT((sysc_cpu_ble_mac_dbg_en_getf() & 0x40) == 0);
			sysc_cpu_func_io11_sel_setf(idx);
			break;
		case FUNC_IO_12:
			BX_ASSERT(sysc_cpu_gpio14_en_getf() == 0);
            BX_ASSERT((sysc_cpu_ble_mac_dbg_en_getf() & 0x80) == 0);
			sysc_cpu_func_io12_sel_setf(idx);
			break;
		case FUNC_IO_13:
			BX_ASSERT(sysc_cpu_gpio15_en_getf() == 0);
			sysc_cpu_func_io13_sel_setf(idx);
			break;
		case FUNC_IO_14:
			sysc_cpu_func_io14_sel_setf(idx);
			break;
		case FUNC_IO_15:
			sysc_cpu_func_io15_sel_setf(idx);
			break;
		case FUNC_IO_16:
			sysc_cpu_func_io16_sel_setf(idx);
			break;
		case FUNC_IO_17:
			sysc_cpu_func_io17_sel_setf(idx);
			break;
		case FUNC_IO_18:
			sysc_cpu_func_io18_sel_setf(idx);
			break;
		case FUNC_IO_19:
			sysc_cpu_func_io19_sel_setf(idx);
			break;
		case FUNC_IO_20:
			sysc_cpu_func_io20_sel_setf(idx);
			break;
		case FUNC_IO_21:
			sysc_cpu_func_io21_sel_setf(idx);
			break;
		default:
			break;

	}
	enable_map |= (1 << io_num);
	sysc_cpu_func_io_en_r_setf(enable_map);
}
#elif HW_BX_VERSION == 01
void pshare_funcio_set(uint8_t io_num, uint8_t idx, uint8_t en)
{
	uint32_t enable_map;

	enable_map = sysc_cpu_func_io_en_getf();
	if(en == DISABLE)
	{
		enable_map &= (~(1 << io_num));
		sysc_cpu_func_io_en_setf(enable_map);
		return;
	}
	switch(io_num)
	{
		case FUNC_IO_0:
			sysc_cpu_func_io00_sel_setf(idx);
			break;
		case FUNC_IO_1:
			sysc_cpu_func_io01_sel_setf(idx);
			break;
		case FUNC_IO_2:
			sysc_cpu_func_io02_sel_setf(idx);
			break;
		case FUNC_IO_3:
			sysc_cpu_func_io03_sel_setf(idx);
			break;
		case FUNC_IO_4:
			sysc_cpu_func_io04_sel_setf(idx);
			break;
		case FUNC_IO_5:
			sysc_cpu_func_io05_sel_setf(idx);
			break;
		case FUNC_IO_6:
			sysc_cpu_func_io06_sel_setf(idx);
			break;
		case FUNC_IO_7:
			sysc_cpu_func_io07_sel_setf(idx);
			break;
		case FUNC_IO_8:
			sysc_cpu_func_io08_sel_setf(idx);
			break;
		case FUNC_IO_9:
			sysc_cpu_func_io09_sel_setf(idx);
			break;
		case FUNC_IO_10:
			sysc_cpu_func_io10_sel_setf(idx);
			break;
		case FUNC_IO_11:
			sysc_cpu_func_io11_sel_setf(idx);
			break;
		case FUNC_IO_12:
			sysc_cpu_func_io12_sel_setf(idx);
			break;
		case FUNC_IO_13:
			sysc_cpu_func_io13_sel_setf(idx);
			break;
		case FUNC_IO_14:
			sysc_cpu_func_io14_sel_setf(idx);
			break;
		case FUNC_IO_15:
			sysc_cpu_func_io15_sel_setf(idx);
			break;
		case FUNC_IO_16:
			sysc_cpu_func_io16_sel_setf(idx);
			break;
		case FUNC_IO_17:
			sysc_cpu_func_io17_sel_setf(idx);
			break;
		case FUNC_IO_18:
			sysc_cpu_func_io18_sel_setf(idx);
			break;
		case FUNC_IO_19:
			sysc_cpu_func_io19_sel_setf(idx);
			break;
		case FUNC_IO_20:
			sysc_cpu_func_io20_sel_setf(idx);
			break;
		case FUNC_IO_21:
			sysc_cpu_func_io21_sel_setf(idx);
			break;
		default:
			break;

	}
	enable_map |= (1 << io_num);
	sysc_cpu_func_io_en_setf(enable_map);
}

#else
#error ERROR: ERROR to define HW_BX_VERSION
#endif



/**
 * @brief disable all the function io settings
 */
void pshare_funcio_disable_all()
{
    #if HW_BX_VERSION == 00
	sysc_cpu_func_io_en_r_setf(0);
    #elif HW_BX_VERSION == 01
    sysc_cpu_func_io_en_setf(0);
    #else
    #error ERROR: ERROR to define HW_BX_VERSION
    #endif
    
}


/**
 * @brief set the pinshare. Read the pinshare WORD document for detail information.
 */
#if HW_BX_VERSION == 00
void pshare_pin_share_en(uint8_t ble_mac_dbg_en, uint8_t rfif_en, uint8_t gpio15_en, uint8_t gpio14_en, uint8_t gpio01_en, uint8_t gpio00_en, 
				uint8_t spis_en, uint8_t spim1_cs1_en, uint8_t spim1_en, uint8_t spim0_cs1_en, uint8_t spim0_en, uint8_t qspi_en)
{
	BX_ASSERT(gpio15_en <= 1);
	BX_ASSERT(gpio14_en <= 1);
	BX_ASSERT(gpio01_en <= 1);
	BX_ASSERT(gpio00_en <= 1);
	BX_ASSERT(spis_en <= 1);
	BX_ASSERT(spim1_cs1_en <= 1);
	BX_ASSERT(spim1_en <= 1);
	BX_ASSERT(spim0_cs1_en <= 1);
	BX_ASSERT(spim0_en <= 1);
	BX_ASSERT(qspi_en <= 0xf);
    BX_ASSERT(rfif_en <= 1);
    
	BX_ASSERT((spim0_en & spis_en) == 0);					// see spec for reason
	BX_ASSERT((spim1_cs1_en & rfif_en) == 0);
    BX_ASSERT((spim1_en & rfif_en) == 0);
    BX_ASSERT((gpio14_en & rfif_en) == 0);
    BX_ASSERT((gpio15_en & rfif_en) == 0);

    BX_ASSERT((rfif_en && ble_mac_dbg_en) == 0 );
	
	sysc_cpu_pin_share_en_pack(ble_mac_dbg_en, rfif_en, gpio15_en, gpio14_en, gpio01_en, gpio00_en, spis_en, spim1_cs1_en, spim1_en, spim0_cs1_en, spim0_en, qspi_en);

}
#elif HW_BX_VERSION == 2401
void pshare_pin_share_en(uint8_t ble_mac_dbg_en, uint8_t rfif_en, uint8_t gpio01_en, uint8_t gpio00_en, 
				uint8_t spis_en, uint8_t spim1_cs1_en, uint8_t spim1_en, uint8_t spim0_cs1_en, uint8_t spim0_en, uint8_t qspi_en)
{
	sysc_cpu_pin_share_en_pack(ble_mac_dbg_en, rfif_en, gpio01_en, gpio00_en, spis_en, spim1_cs1_en, spim1_en, spim0_cs1_en, spim0_en, qspi_en);
}
#else
#error ERROR: ERROR to define HW_BX_VERSION
#endif

void pshare_reset_to_gpio()
{   // except swd
    sysc_cpu_func_io_en_set(0);
    sysc_cpu_pin_share_en_set(0x1800);
}

/// @} pin share

