/**
****************************************************************************************
*
* @file rf_atlas.c
*
* @brief Atlas radio initialization and specific functions
*
* Copyright (C) RivieraWaves 2009-2015
*
* $Rev: $
*
****************************************************************************************
*/

/**
****************************************************************************************
* @addtogroup RF_ATLAS
* @ingroup RF
* @brief Atlas Radio Driver
*
* This is the driver block for Atlas radio
* @{
****************************************************************************************
*/

/**
 *****************************************************************************************
 * INCLUDE FILES
 *****************************************************************************************
 */ 
#define __RAM_CODE__
#include <string.h>        // for memcpy
#include "co_utils.h"      // common utility definition
#include "co_math.h"       // common math functions
#include "co_endian.h"     // endian definitions
#include "rf.h"            // RF interface
#include "em_map.h"        // RF area mapping

#include "rwip.h"          // for RF API structure definition
#include "reg_blecore.h"   // ble core registers
#include "reg_ble_em_cs.h" // control structure definitions

#include "plf.h"           // Platform register

// Atlas register definitions and access functions
static uint32_t rf_atl_reg_rd (uint16_t addr);
static void rf_atl_reg_wr (uint16_t addr, uint32_t value);

#define REG_ATL_RD                rf_atl_reg_rd
#define REG_ATL_WR                rf_atl_reg_wr

/**
 ****************************************************************************************
 * DEFINES
 ****************************************************************************************
 **/

#define RF_EM_SPI_ADRESS        (EM_BASE_ADDR + EM_RF_SW_SPI_OFFSET)

#define ATL_SPIRD                   0x00
#define ATL_SPIWR                   0x80

#define ATL_MAX_BURST_SIZE          0x80
#define ICY20_INIT_TBL_SIZE         0x98
#define ICY30_INIT_TBL_SIZE         0xA8
#define ICY31_INIT_TBL_SIZE         0xBD
#define ICY40_INIT_TBL_SIZE         0xC0

// TX max power
#define ATL_POWER_MAX               0x06


/**
****************************************************************************************
* MACROS
*****************************************************************************************
*/

/// IcyTRx EM Write Macro for HW driven SPI chained structures
#define RF_ICTRX_EM_BLE_WR(addr, val) \
    EM_BLE_WR((((uint32_t) (addr)) + REG_BLE_EM_CS_BASE_ADDR), (val))


// Max burst register
static uint8_t rf_atl_reg_buf[ATL_MAX_BURST_SIZE + 2]; // max burst size + buffer controls

/**
 ****************************************************************************************
 * GLOBAL VARIABLE DEFINITIONS
 *****************************************************************************************
 **/

/* Icytrx v0x20 static settings - 65nm */
static const uint8_t RF_ATL_REG_TBL_20[ICY20_INIT_TBL_SIZE] =
{ 0x22, 0x08, 0x00, 0x81,   0x80, 0x82, 0xff, 0x01,    0xaa, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x1e, //0x0*
  0x3c, 0x5a, 0x78, 0x02,   0x10, 0x88, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00,   0x00, 0x23, 0x12, 0x14, //0x1*
  0x03, 0x20, 0x00, 0x00,   0x03, 0x00, 0xff, 0xf0,    0x0f, 0x00, 0x00, 0x00,   0xf5, 0x49, 0x17, 0x02, //0x2*
  0x00, 0x01, 0x01, 0x00,   0x00, 0x00, 0x00, 0x00,    0x00, 0x01, 0x02, 0x07,   0x10, 0x20, 0x37, 0x50, //0x3*
  0x66, 0x76, 0x7e, 0x2b,   0x11, 0xa0, 0x21, 0x74,    0xca, 0xfe, 0x3f, 0x05,   0x04, 0x57, 0x0b, 0x1b, //0x4*
  0x30, 0x44, 0x0b, 0xaa,   0x2a, 0x11, 0x17, 0x17,    0x08, 0x0e, 0x0c, 0x08,   0xc8, 0x54, 0x80, 0x00, //0x5*
  0x40, 0xa0, 0x42, 0x86,   0x15, 0x3d, 0x7a, 0xf4,    0xdb, 0xb6, 0x0d, 0x00,   0x22, 0x40, 0x42, 0x44, //0x6*
  0x14, 0x00, 0x00, 0x00,   0x73, 0x66, 0xb0, 0x99,    0x79, 0x07, 0x76, 0x47,   0x93, 0x78, 0x56, 0x00, //0x7*
  0x00, 0x02, 0x0f, 0xc3,   0x82, 0x06, 0x00, 0x00,    0x00, 0x07, 0x1f, 0x00,   0x00, 0x00, 0x00, 0x35, //0x8*
  0x80, 0xc3, 0x69, 0x44,   0x00, 0xf0, 0x00, 0x00                                                       //0x9*
};

/* Icytrx v0x30 static settings - CS1 */
static const uint8_t RF_ATL_REG_TBL_30[ICY30_INIT_TBL_SIZE] =
{ 0x22, 0x08, 0x01, 0x00,   0x00, 0x23, 0x0b, 0x14,    0x05, 0x20, 0x00, 0x10,   0x03, 0x43, 0xff, 0xff, // 0x0*
  0x0f, 0x00, 0x00, 0x42,   0x46, 0xc8, 0x1c, 0xc7,    0x15, 0x82, 0x00, 0x00,   0x01, 0x01, 0x72, 0x1c, // 0x1*
  0x07, 0x00, 0xa2, 0xff,   0x01, 0x55, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00,   0x29, 0x41, 0x76, 0x71, // 0x2*
  0x03, 0x2d, 0x03, 0x80,   0x00, 0x55, 0x55, 0x55,    0x55, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, // 0x3*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01,    0x02, 0x07, 0x10, 0x20,   0x37, 0x50, 0x66, 0x76, // 0x4*
  0x7e, 0x2b, 0x01, 0xa0,   0x21, 0x74, 0xca, 0xfe,    0x3f, 0x80, 0x00, 0x50,   0x0f, 0x1b, 0x30, 0x44, // 0x5*
  0x0b, 0xaa, 0x2a, 0x11,   0x1b, 0x17, 0x0b, 0x15,    0x0c, 0x0c, 0xe8, 0x5c,   0x84, 0x00, 0x20, 0x60, // 0x6*
  0xc0, 0x84, 0x31, 0x63,   0xc6, 0x8c, 0xdb, 0xb6,    0x0d, 0x00, 0x24, 0xb0,   0x42, 0xb4, 0x14, 0x00, // 0x7*
  0x00, 0x00, 0x73, 0x66,   0x72, 0x99, 0x75, 0x99,    0x97, 0x39, 0x72, 0x68,   0x25, 0x00, 0x00, 0x02, // 0x8*
  0x0f, 0xc2, 0xc2, 0x06,   0x00, 0x00, 0x00, 0x07,    0x33, 0x00, 0x00, 0x00,   0x00, 0x35, 0x80, 0xc3, // 0x9*
  0x80, 0x04, 0x00, 0xf0,   0x00, 0x03, 0x00
};


/* Icytrx v0x31 static settings - CS2 - 1Mbps table */
static const uint8_t RF_ATL_REG_TBL_31_1MBPS[ICY31_INIT_TBL_SIZE] =
{ 0x22, 0x08, 0x00, 0x00,   0x00, 0x22, 0x0b, 0x14,    0x05, 0x20, 0x00, 0x10,   0x03, 0x00, 0xff, 0xff, // 0x0*
  0x0f, 0x00, 0x00, 0x80,   0x23, 0x82, 0x1b, 0xc7,    0x15, 0x82, 0x00, 0x00,   0x01, 0x01, 0x72, 0x1c, // 0x1*
  0x07, 0x00, 0x82, 0xff,   0x01, 0x55, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00,   0x29, 0x41, 0x76, 0x71, // 0x2*
  0x03, 0x2d, 0x03, 0x80,   0x00, 0x55, 0x55, 0x55,    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, // 0x3*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01,    0x02, 0x07, 0x10, 0x20,   0x37, 0x50, 0x66, 0x76, // 0x4*
  0x7e, 0x2b, 0x10, 0xc0,   0x21, 0x74, 0xca, 0xfe,    0x3f, 0x00, 0x02, 0x50,   0x3f, 0x1b, 0x30, 0x44, // 0x5*
  0x0b, 0x55, 0x15, 0x11,   0x1b, 0x09, 0x0b, 0x15,    0x0c, 0xfe, 0x50, 0x5c,   0x84, 0x00, 0x00, 0x04, // 0x6*
  0x24, 0x30, 0x81, 0x11,   0x9c, 0x00, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0xdc, 0xc8, // 0x7*
  0x0d, 0x00, 0x00, 0x00,   0x00, 0x33, 0xc0, 0x22,    0x61, 0x01, 0x00, 0x00,   0x00, 0x73, 0x66, 0x72, // 0x8*
  0x99, 0x75, 0x9a, 0x77,   0x37, 0x72, 0x13, 0x68,    0x25, 0x00, 0x00, 0x08,   0x0f, 0xc2, 0xc2, 0x16, // 0x9*
  0x18, 0x00, 0x00, 0x07,   0x7f, 0x80, 0x00, 0x00,    0x00, 0x3a, 0xd0, 0xc3,   0x78, 0x04, 0x04, 0xf0, // 0xa*
  0x00, 0x03, 0x00, 0x4b,   0x11, 0x87, 0x00, 0x40,    0x00, 0x00, 0x8b, 0x25,   0x00
};

/* Icytrx v0x31 static settings - CS2 - 2Mbps table */
static const uint8_t RF_ATL_REG_TBL_31_2MBPS[ICY31_INIT_TBL_SIZE] =
{ 0x22, 0x08, 0x00, 0x00,   0x00, 0x22, 0x0b, 0x14,    0x05, 0x20, 0x00, 0x10,   0x03, 0x00, 0xff, 0xff, // 0x0*
  0x0f, 0x00, 0x00, 0x80,   0x23, 0x82, 0x1b, 0xc7,    0x15, 0x82, 0x01, 0x00,   0x00, 0x00, 0x72, 0x1c, // 0x1*
  0x07, 0x00, 0x82, 0xff,   0x01, 0x55, 0x01, 0x00,    0x00, 0x00, 0x00, 0x00,   0x29, 0x41, 0x76, 0x71, // 0x2*
  0x03, 0x2d, 0x03, 0x80,   0x00, 0x55, 0x55, 0x55,    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, // 0x3*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01,    0x02, 0x07, 0x10, 0x20,   0x37, 0x50, 0x66, 0x76, // 0x4*
  0x7e, 0x3b, 0x00, 0xc0,   0x21, 0x74, 0xca, 0xfe,    0x3f, 0x80, 0x02, 0x70,   0x3f, 0x1b, 0x30, 0x44, // 0x5*
  0x0b, 0xaa, 0x2a, 0x11,   0x15, 0x07, 0x0b, 0x15,    0x0c, 0xfe, 0x50, 0x5c,   0x84, 0x00, 0x00, 0x04, // 0x6*
  0x24, 0x30, 0x81, 0x11,   0x9c, 0x00, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0xdc, 0xc8, // 0x7*
  0x0d, 0x00, 0x00, 0x00,   0x00, 0x33, 0xc0, 0x22,    0x61, 0x01, 0x00, 0x00,   0x00, 0x73, 0x66, 0x72, // 0x8*
  0x99, 0x75, 0x9a, 0x77,   0x37, 0x72, 0x13, 0x68,    0x25, 0x00, 0x00, 0x08,   0x0f, 0xc2, 0xc2, 0x16, // 0x9*
  0x18, 0x00, 0x00, 0x07,   0x7f, 0x80, 0x00, 0x00,    0x00, 0x3a, 0xd0, 0xc3,   0x78, 0x04, 0x04, 0xf0, // 0xa*
  0x00, 0x03, 0x00, 0x4b,   0x11, 0x87, 0x00, 0x40,    0x00, 0x00, 0x8b, 0x25,   0x00 
};


/* Icytrx v0x40 static settings - GCS1 - 1Mbps table */
static  const uint8_t RF_ATL_REG_TBL_40_1MBPS[ICY40_INIT_TBL_SIZE] =
//{ 0x22, 0x08, 0x00, 0x00,   0x00, 0x23, 0x0b, 0x14,    0x05, 0x20, 0x00, 0x10,   0x03, 0x00, 0xff, 0xff, // 0x0*
//  0x0f, 0x00, 0x00, 0x80,   0x23, 0x82, 0x00, 0x00,    0x1b, 0xc7, 0x15, 0x82,   0x01, 0x01, 0x72, 0x1c, // 0x1*
//  0x07, 0x00, 0x82, 0xff,   0x01, 0x55, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00,   0x29, 0x41, 0x76, 0x71, // 0x2*
//  0x03, 0x00, 0x00, 0x00,   0x2d, 0x03, 0x80, 0x00,    0x55, 0x55, 0x55, 0x00,   0x00, 0x00, 0x00, 0x00, // 0x3*
//  0x00, 0x2b, 0x10, 0xc0,   0x00, 0x00, 0x00, 0x00,    0x00, 0x00, 0x01, 0x02,   0x07, 0x10, 0x20, 0x37, // 0x4*
//  0x50, 0x66, 0x76, 0x7e,   0x21, 0x74, 0xca, 0xfe,    0x3f, 0x1b, 0x00, 0x02,   0x50, 0x3f, 0x30, 0x44, // 0x5*
//  0x0b, 0x11, 0x55, 0x15,   0x1b, 0x0c, 0x09, 0x3a,    0x10, 0x17, 0xfe, 0x50,   0x5c, 0x84, 0x00, 0x00, // 0x6*
//  0x00, 0x00, 0x04, 0x24,   0x30, 0x81, 0x11, 0x9c,    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, // 0x7*
//  0x00, 0x33, 0xc0, 0x22,   0x6e, 0xdd, 0x0d, 0x00,    0x02, 0x61, 0x01, 0x00,   0x00, 0x00, 0x73, 0x66, // 0x8*
//  0x72, 0x99, 0x73, 0x9a,   0x77, 0x37, 0x72, 0x13,    0x00, 0x68, 0x25, 0x0f,   0x00, 0x00, 0x08, 0x7f, // 0x9*
//  0x18, 0x00, 0x00, 0x80,   0xc2, 0xc2, 0x16, 0x80,    0x77, 0x00, 0x00, 0x00,   0x00, 0x35, 0x84, 0xc3, // 0xa*
//  0x04, 0x04, 0xf0, 0x00,   0x03, 0x4b, 0x11, 0x00,    0x00, 0x40, 0x00, 0x00,   0x86, 0x25, 0x00, 0x03
//};
{
  0x22, 0x08, 0x00, 0x00,   0x00, 0x23, 0x0b, 0x14,    0x05, 0x20, 0x00, 0x10,   0x03, 0x00, 0xff, 0xff, // 0x0*
  0x0f, 0x00, 0x00, 0x80,   0x23, 0x82, 0x00, 0x00,    0x1b, 0xc7, 0x15, 0x82,   0x01, 0x01, 0x72, 0x1c, // 0x1*
  0x07, 0x00, 0x82, 0xff,   0x01, 0x55, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00,   0x29, 0x41, 0x76, 0x71, // 0x2*
  0x03, 0x00, 0x00, 0x00,   0x2d, 0x03, 0x80, 0x00,    0x55, 0x55, 0x55, 0x00,   0x00, 0x00, 0x00, 0x00, // 0x3*
  0x00, 0x2b, 0x10, 0xc0,   0x00, 0x00, 0x00, 0x00,    0x00, 0x00, 0x01, 0x02,   0x07, 0x10, 0x20, 0x37, // 0x4*
  0x50, 0x66, 0x76, 0x7e,   0x21, 0x74, 0xca, 0xfe,    0x3f, 0x1b, 0x00, 0x02,   0x50, 0x2f, 0x30, 0x44, // 0x5*// reg0x5d =original value 0x3f
  0x0b, 0x11, 0x55, 0x15,   0x1b, 0x0c, 0x09, 0x3a,    0x10, 0x17, 0xfe, 0x50,   0x5c, 0x84, 0x00, 0x00, // 0x6*
  0x00, 0x00, 0x04, 0x24,   0x30, 0x81, 0x11, 0x9c,    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, // 0x7*
  0x00, 0x33, 0xc0, 0x22,   0x6e, 0xdd, 0x0d, 0x00,    0x02, 0x61, 0x01, 0x00,   0x00, 0x00, 0x73, 0x66, // 0x8*
  0x72, 0x99, 0x73, 0xcc,   0x77, 0x37, 0x72, 0x13,    0x00, 0x68, 0x25, 0x0f,   0x00, 0x00, 0x08, 0x6f, // 0x9*
  0x18, 0x00, 0x00, 0x80,   0xc2, 0xc2, 0x16, 0x80,    0x77, 0x00, 0x00, 0x00,   0x00, 0x35, 0x84, 0xc3, // 0xa*
  0x04, 0x04, 0xf0, 0x00,   0x03, 0x4b, 0x11, 0x00,    0x00, 0x40, 0x00, 0x00,   0x86, 0x25
};


/* Icytrx v0x40 static settings - GCS1 - 2Mbps table */
static  const uint8_t RF_ATL_REG_TBL_40_2MBPS[ICY40_INIT_TBL_SIZE] =
{ 0x22, 0x08, 0x00, 0x00,   0x00, 0x23, 0x0b, 0x14,    0x05, 0x20, 0x00, 0x10,   0x03, 0x00, 0xff, 0xff, // 0x0*
  0x0f, 0x00, 0x00, 0x80,   0x23, 0x82, 0x01, 0x00,    0x1b, 0xc7, 0x15, 0x82,   0x00, 0x00, 0x72, 0x1c, // 0x1*
  0x07, 0x00, 0x82, 0xff,   0x01, 0x55, 0x01, 0x00,    0x00, 0x00, 0x00, 0x00,   0x29, 0x41, 0x76, 0x71, // 0x2*
  0x03, 0x00, 0x00, 0x00,   0x2d, 0x03, 0x80, 0x00,    0x55, 0x55, 0x55, 0x00,   0x00, 0x00, 0x00, 0x00, // 0x3*
  0x00, 0x3b, 0x00, 0xc0,   0x00, 0x00, 0x00, 0x00,    0x00, 0x00, 0x01, 0x02,   0x07, 0x10, 0x20, 0x37, // 0x4*
  0x50, 0x66, 0x76, 0x7e,   0x21, 0x74, 0xca, 0xfe,    0x3f, 0x1b, 0x80, 0x02,   0x70, 0x3f, 0x30, 0x44, // 0x5*
  0x0b, 0x11, 0xaa, 0x2a,   0x15, 0x0c, 0x07, 0x3a,    0x10, 0x17, 0xfe, 0x50,   0x5c, 0x84, 0x00, 0x00, // 0x6*
  0x00, 0x00, 0x04, 0x24,   0x30, 0x81, 0x11, 0x9c,    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, // 0x7*
  0x00, 0x33, 0xc0, 0x22,   0x6e, 0xdd, 0x0d, 0x00,    0x02, 0x61, 0x01, 0x00,   0x00, 0x00, 0x73, 0x66, // 0x8*
  0x72, 0x99, 0x73, 0x9a,   0x77, 0x37, 0x72, 0x13,    0x00, 0x68, 0x25, 0x0f,   0x00, 0x00, 0x08, 0x7f, // 0x9*
  0x18, 0x00, 0x00, 0x80,   0xc2, 0xc2, 0x16, 0x80,    0x77, 0x00, 0x00, 0x00,   0x00, 0x35, 0x84, 0xc3, // 0xa*
  0x04, 0x04, 0xf0, 0x00,   0x03, 0x4b, 0x11, 0x00,    0x00, 0x40, 0x00, 0x00,   0x86, 0x25, 0x00, 0x03
};

/* Frequency Table for IcyTRx65 v0x20 */
static const uint32_t RF_ATL_FREQ_TBL20[EM_RF_FREQ_TABLE_LEN/4] =
{
  [0x00] = 0x821638e3,    // 2404MHz
  [0x01] = 0x8216aaaa,    // 2406MHz
  [0x02] = 0x82171c71,    // 2408MHz
  [0x03] = 0x82178e38,    // 2410MHz
  [0x04] = 0x82180000,    // 2412MHz
  [0x05] = 0x821871c7,    // 2414MHz
  [0x06] = 0x8218e38e,    // 2416MHz
  [0x07] = 0x82195555,    // 2418MHz
  [0x08] = 0x8219c71c,    // 2420MHz
  [0x09] = 0x821a38e3,    // 2422MHz
  [0x0a] = 0x821aaaaa,    // 2424MHz
  [0x0b] = 0x821b8e38,    // 2428MHz
  [0x0c] = 0x821c0000,    // 2430MHz
  [0x0d] = 0x821c71c7,    // 2432MHz
  [0x0e] = 0x821ce38e,    // 2434MHz
  [0x0f] = 0x821d5555,    // 2436MHz
  [0x10] = 0x821dc71c,    // 2438MHz
  [0x11] = 0x821e38e3,    // 2440MHz
  [0x12] = 0x821eaaaa,    // 2442MHz
  [0x13] = 0x821f1c71,    // 2444MHz
  [0x14] = 0x821f8e38,    // 2446MHz
  [0x15] = 0x82200000,    // 2448MHz
  [0x16] = 0x822071c7,    // 2450MHz
  [0x17] = 0x8220e38e,    // 2452MHz
  [0x18] = 0x82215555,    // 2454MHz
  [0x19] = 0x8221c71c,    // 2456MHz
  [0x1a] = 0x822238e3,    // 2458MHz
  [0x1b] = 0x8222aaaa,    // 2460MHz
  [0x1c] = 0x82231c71,    // 2462MHz
  [0x1d] = 0x82238e38,    // 2464MHz
  [0x1e] = 0x82240000,    // 2466MHz
  [0x1f] = 0x822471c7,    // 2468MHz
  [0x20] = 0x8224e38e,    // 2470MHz
  [0x21] = 0x82255555,    // 2472MHz
  [0x22] = 0x8225c71c,    // 2474MHz
  [0x23] = 0x822638e3,    // 2476MHz
  [0x24] = 0x8226aaaa,    // 2478MHz
  [0x25] = 0x8215c71c,    // 2402MHz
  [0x26] = 0x821b1c71,    // 2426MHz
  [0x27] = 0x82271c71,    // 2480MHz
};

/* Frequency Table for IcyTRx65 v0x30 - CS1 */
static const uint32_t RF_ATL_FREQ_TBL30[EM_RF_FREQ_TABLE_LEN/4] =
{
  [0x00] = 0x821638e3,    // 2404MHz
  [0x01] = 0x8216aaaa,    // 2406MHz
  [0x02] = 0x82171c71,    // 2408MHz
  [0x03] = 0x82178e38,    // 2410MHz
  [0x04] = 0x82180000,    // 2412MHz
  [0x05] = 0x821871c7,    // 2414MHz
  [0x06] = 0x8218e38e,    // 2416MHz
  [0x07] = 0x82195555,    // 2418MHz
  [0x08] = 0x8219c71c,    // 2420MHz
  [0x09] = 0x821a38e3,    // 2422MHz
  [0x0a] = 0x821aaaaa,    // 2424MHz
  [0x0b] = 0x821b8e38,    // 2428MHz
  [0x0c] = 0x821c0000,    // 2430MHz
  [0x0d] = 0x821c71c7,    // 2432MHz
  [0x0e] = 0x821ce38e,    // 2434MHz
  [0x0f] = 0x821d5555,    // 2436MHz
  [0x10] = 0x821dc71c,    // 2438MHz
  [0x11] = 0x821e38e3,    // 2440MHz
  [0x12] = 0x821eaaaa,    // 2442MHz
  [0x13] = 0x821f1c71,    // 2444MHz
  [0x14] = 0x821f8e38,    // 2446MHz
  [0x15] = 0x82200000,    // 2448MHz
  [0x16] = 0x822071c7,    // 2450MHz
  [0x17] = 0x8220e38e,    // 2452MHz
  [0x18] = 0x82215555,    // 2454MHz
  [0x19] = 0x8221c71c,    // 2456MHz
  [0x1a] = 0x822238e3,    // 2458MHz
  [0x1b] = 0x8222aaaa,    // 2460MHz
  [0x1c] = 0x82231c71,    // 2462MHz
  [0x1d] = 0x82238e38,    // 2464MHz
  [0x1e] = 0x82240000,    // 2466MHz
  [0x1f] = 0x822471c7,    // 2468MHz
  [0x20] = 0x8224e38e,    // 2470MHz
  [0x21] = 0x82255555,    // 2472MHz
  [0x22] = 0x8225c71c,    // 2474MHz
  [0x23] = 0x822638e3,    // 2476MHz
  [0x24] = 0x8226aaaa,    // 2478MHz
  [0x25] = 0x8215c71c,    // 2402MHz
  [0x26] = 0x821b1c71,    // 2426MHz
  [0x27] = 0x82271c71,    // 2480MHz
};

/* Frequency Table for IcyTRx65 v0x31 - CS2 */
static const uint32_t RF_ATL_FREQ_TBL31[EM_RF_FREQ_TABLE_LEN/4] =
{
  [0x00] = 0x821638e3,    // 2404MHz
  [0x01] = 0x8216aaaa,    // 2406MHz
  [0x02] = 0x82171c71,    // 2408MHz
  [0x03] = 0x82178e38,    // 2410MHz
  [0x04] = 0x82180000,    // 2412MHz
  [0x05] = 0x821871c7,    // 2414MHz
  [0x06] = 0x8218e38e,    // 2416MHz
  [0x07] = 0x82195555,    // 2418MHz
  [0x08] = 0x8219c71c,    // 2420MHz
  [0x09] = 0x821a38e3,    // 2422MHz
  [0x0a] = 0x821aaaaa,    // 2424MHz
  [0x0b] = 0x821b8e38,    // 2428MHz
  [0x0c] = 0x821c0000,    // 2430MHz
  [0x0d] = 0x821c71c7,    // 2432MHz
  [0x0e] = 0x821ce38e,    // 2434MHz
  [0x0f] = 0x821d5555,    // 2436MHz
  [0x10] = 0x821dc71c,    // 2438MHz
  [0x11] = 0x821e38e3,    // 2440MHz
  [0x12] = 0x821eaaaa,    // 2442MHz
  [0x13] = 0x821f1c71,    // 2444MHz
  [0x14] = 0x821f8e38,    // 2446MHz
  [0x15] = 0x82200000,    // 2448MHz
  [0x16] = 0x822071c7,    // 2450MHz
  [0x17] = 0x8220e38e,    // 2452MHz
  [0x18] = 0x82215555,    // 2454MHz
  [0x19] = 0x8221c71c,    // 2456MHz
  [0x1a] = 0x822238e3,    // 2458MHz
  [0x1b] = 0x8222aaaa,    // 2460MHz
  [0x1c] = 0x82231c71,    // 2462MHz
  [0x1d] = 0x82238e38,    // 2464MHz
  [0x1e] = 0x82240000,    // 2466MHz
  [0x1f] = 0x822471c7,    // 2468MHz
  [0x20] = 0x8224e38e,    // 2470MHz
  [0x21] = 0x82255555,    // 2472MHz
  [0x22] = 0x8225c71c,    // 2474MHz
  [0x23] = 0x822638e3,    // 2476MHz
  [0x24] = 0x8226aaaa,    // 2478MHz
  [0x25] = 0x8215c71c,    // 2402MHz
  [0x26] = 0x821b1c71,    // 2426MHz
  [0x27] = 0x82271c71,    // 2480MHz
};

/* Frequency Table for IcyTRx55 v0x40 and v0x41 - GCS1 */
static const uint32_t RF_ATL_FREQ_TBL40[EM_RF_FREQ_TABLE_LEN/4] =
{
  [0x00] = 0x821638e3,    // 2404MHz
  [0x01] = 0x8216aaaa,    // 2406MHz
  [0x02] = 0x82171c71,    // 2408MHz
  [0x03] = 0x82178e38,    // 2410MHz
  [0x04] = 0x82180000,    // 2412MHz
  [0x05] = 0x821871c7,    // 2414MHz
  [0x06] = 0x8218e38e,    // 2416MHz
  [0x07] = 0x82195555,    // 2418MHz
  [0x08] = 0x8219c71c,    // 2420MHz
  [0x09] = 0x821a38e3,    // 2422MHz
  [0x0a] = 0x821aaaaa,    // 2424MHz
  [0x0b] = 0x821b8e38,    // 2428MHz
  [0x0c] = 0x821c0000,    // 2430MHz
  [0x0d] = 0x821c71c7,    // 2432MHz
  [0x0e] = 0x821ce38e,    // 2434MHz
  [0x0f] = 0x821d5555,    // 2436MHz
  [0x10] = 0x821dc71c,    // 2438MHz
  [0x11] = 0x821e38e3,    // 2440MHz
  [0x12] = 0x821eaaaa,    // 2442MHz
  [0x13] = 0x821f1c71,    // 2444MHz
  [0x14] = 0x821f8e38,    // 2446MHz
  [0x15] = 0x82200000,    // 2448MHz
  [0x16] = 0x822071c7,    // 2450MHz
  [0x17] = 0x8220e38e,    // 2452MHz
  [0x18] = 0x82215555,    // 2454MHz
  [0x19] = 0x8221c71c,    // 2456MHz
  [0x1a] = 0x822238e3,    // 2458MHz
  [0x1b] = 0x8222aaaa,    // 2460MHz
  [0x1c] = 0x82231c71,    // 2462MHz
  [0x1d] = 0x82238e38,    // 2464MHz
  [0x1e] = 0x82240000,    // 2466MHz
  [0x1f] = 0x822471c7,    // 2468MHz
  [0x20] = 0x8224e38e,    // 2470MHz
  [0x21] = 0x82255555,    // 2472MHz
  [0x22] = 0x8225c71c,    // 2474MHz
  [0x23] = 0x822638e3,    // 2476MHz
  [0x24] = 0x8226aaaa,    // 2478MHz
  [0x25] = 0x8215c71c,    // 2402MHz
  [0x26] = 0x821b1c71,    // 2426MHz
  [0x27] = 0x82271c71,    // 2480MHz
};

/* VCO sub-band init value Table for IcyTRx / All 0xFF before VCO calibration */
static const uint8_t RF_ATL_VCO_TBL[EM_RF_VCO_TABLE_LEN] =
{
  [0x00] = 0xFF,   // 2404 MHz
  [0x01] = 0xFF,   // 2406 MHz
  [0x02] = 0xFF,   // 2408 MHz
  [0x03] = 0xFF,   // 2410 MHz
  [0x04] = 0xFF,   // 2412 MHz
  [0x05] = 0xFF,   // 2414 MHz
  [0x06] = 0xFF,   // 2416 MHz
  [0x07] = 0xFF,   // 2418 MHz
  [0x08] = 0xFF,   // 2420 MHz
  [0x09] = 0xFF,   // 2422 MHz
  [0x0A] = 0xFF,   // 2426 MHz
  [0x0B] = 0xFF,   // 2428 MHz
  [0x0C] = 0xFF,   // 2430 MHz
  [0x0D] = 0xFF,   // 2432 MHz
  [0x0E] = 0xFF,   // 2434 MHz
  [0x0F] = 0xFF,   // 2436 MHz
  [0x10] = 0xFF,   // 2438 MHz
  [0x11] = 0xFF,   // 2440 MHz
  [0x12] = 0xFF,   // 2442 MHz
  [0x13] = 0xFF,   // 2444 MHz
  [0x14] = 0xFF,   // 2446 MHz
  [0x15] = 0xFF,   // 2448 MHz
  [0x16] = 0xFF,   // 2450 MHz
  [0x17] = 0xFF,   // 2452 MHz
  [0x18] = 0xFF,   // 2454 MHz
  [0x19] = 0xFF,   // 2456 MHz
  [0x1A] = 0xFF,   // 2458 MHz
  [0x1B] = 0xFF,   // 2460 MHz
  [0x1C] = 0xFF,   // 2462 MHz
  [0x1D] = 0xFF,   // 2464 MHz
  [0x1E] = 0xFF,   // 2466 MHz
  [0x1F] = 0xFF,   // 2468 MHz
  [0x20] = 0xFF,   // 2470 MHz
  [0x21] = 0xFF,   // 2472 MHz
  [0x22] = 0xFF,   // 2474 MHz
  [0x23] = 0xFF,   // 2476 MHz
  [0x24] = 0xFF,   // 2478 MHz
  [0x25] = 0xFF,   // 2402 MHz
  [0x26] = 0xFF,   // 2426 MHz
  [0x27] = 0xFF    // 2480 MHz
};

/* IcyTRx dynamic register */ 
enum
{
    ICY2x_CENTER_FREQ     = 0x2C,
    ICY3x_CENTER_FREQ     = 0x16,
    ICY_FSM_MODE          = 0xC0,
    ICY_TIMING            = 0x6d,
    ICY2x_SW_CAP_FSM      = 0x88,
    ICY30_TIMING          = 0x7b,
    ICY30_SW_CAP_FSM      = 0x96,
    ICY31_SW_CAP_FSM      = 0xa2,
    ICY31_TIMING          = 0x8B,
    ICY40_CENTER_FREQ     = 0x18,
    ICY40_SW_CAP_FSM      = 0xa2,
    ICY40_TIMING          = 0x8C
};

/**
 ****************************************************************************************
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief SPI access
 ***************************************************************************************
 */
static void rf_atl_spi_tf(void)
{
    //launch SPI transfer
    ble_spigo_setf(1);

    //wait for transfer to be completed
    while (!ble_spicomp_getf());
}

/**
 ****************************************************************************************
 * @brief Atlas specific read access
 *
 * @param[in] addr    register address
 *
 * @return uint32_t value
 *****************************************************************************************
 */
static uint32_t rf_atl_reg_rd (uint16_t addr)
{
    // Next Pointr to 0x0
    rf_atl_reg_buf[0] = (uint8_t)(0);  // Next Pointer set to 0x0000 to stop the SPI Chained access
    rf_atl_reg_buf[1] = (uint8_t)(0);

    //copy control and number of u32 to send
    rf_atl_reg_buf[2] = (uint8_t)(ATL_SPIRD + 1);

    //copy address
    rf_atl_reg_buf[3] = addr;

    memcpy((void *)RF_EM_SPI_ADRESS, rf_atl_reg_buf, 4);

    //do the transfer
    rf_atl_spi_tf();

    return (uint32_t)(*((uint8_t *)(RF_EM_SPI_ADRESS + 4)));
}

/**
 ****************************************************************************************
 * @brief Atlas specific write access
 *
 * @param[in] addr    register address
 * @param[in] value   value to write
 *
 * @return uint32_t value
 ****************************************************************************************
 */
static void rf_atl_reg_wr (uint16_t addr, uint32_t value)
{
    rf_atl_reg_buf[0] = (uint8_t)(0);  // Next Pointer set to 0x0000 to stop the SPI Chained access
    rf_atl_reg_buf[1] = (uint8_t)(0);

    //inversion for EM reading by U8 on ATL SPI side
    //copy control and number of u32 to send
    rf_atl_reg_buf[2] = (uint8_t)(ATL_SPIWR + 1);

    //copy address
    rf_atl_reg_buf[3] = (uint8_t)(addr);

    //on old implementations (BT core 3.0, BLE core 1.0) swap the data
    rf_atl_reg_buf[4]= (uint8_t)value;

    memcpy((void *)RF_EM_SPI_ADRESS, rf_atl_reg_buf, 5);

    //do the transfer
    rf_atl_spi_tf();
}

/**
 ****************************************************************************************
 * @brief Atlas specific read access
 *
 * @param[in] addr    register address
 * @param[in] size    transfer size
 * @param[in] data    pointer to the data array
 *
 * @return uint32_t value
 ****************************************************************************************
 **/
static void rf_atl_reg_burst_wr (uint16_t addr, uint8_t size, uint8_t *data)
{
    rf_atl_reg_buf[0] = (uint8_t)(0);  // Next Pointer set to 0x0000 to stop the SPI Chained access
    rf_atl_reg_buf[1] = (uint8_t)(0);

    //copy control and number of u8 to send
    rf_atl_reg_buf[2] = (uint8_t)(ATL_SPIWR + size);

    //copy address
    rf_atl_reg_buf[3] = (uint8_t)(addr);

    for(int i =0;i < size + 2;i++)
    {
        rf_atl_reg_buf[i + 4] = *(data + i);
    }

    memcpy((void *)RF_EM_SPI_ADRESS, rf_atl_reg_buf, 4 + size);

    //do the transfer
    rf_atl_spi_tf();
}

/**
 *****************************************************************************************
 * @brief Init RF sequence after reset.
 *****************************************************************************************
 */
static void rf_reset(void)
{
}

/**
 ****************************************************************************************
 * @brief ISR to be called in BLE ISR routine when RF Interrupt occurs.
 *****************************************************************************************
 */
static void rf_force_agc_enable(bool en)
{
}

/**
 *****************************************************************************************
 * @brief Get TX power in dBm from the index in the control structure
 *
 * @param[in] txpwr_idx  Index of the TX power in the control structure
 * @param[in] modulation Modulation: 1 or 2 or 3 MBPS
 *
 * @return The TX power in dBm
 *
 *****************************************************************************************
 */
static uint8_t rf_txpwr_dbm_get(uint8_t txpwr_idx, uint8_t modulation)
{
    // Power table should be provided
    return(0);
}

/**
 *****************************************************************************************
 * @brief Sleep function for Atlas RF.
 *****************************************************************************************
 */
static void rf_sleep(void)
{
    #if defined(CFG_BLE)
    ble_deepslcntl_set(ble_deepslcntl_get() |
                      BLE_DEEP_SLEEP_ON_BIT |    // RW BLE Core sleep
                      BLE_RADIO_SLEEP_EN_BIT |   // Radio sleep
                      BLE_OSC_SLEEP_EN_BIT);     // Oscillator sleep
    #endif // CFG_BLE
}

/**
 *****************************************************************************************
 * @brief Convert RSSI to dBm
 *
 * @param[in] rssi_reg RSSI read from the HW registers
 *
 * @return The converted RSSI
 *
 *****************************************************************************************
 */
static int8_t rf_rssi_convert (uint8_t rssi_reg)
{
    uint8_t RssidBm = 0; 

    RssidBm = ((rssi_reg) >> 1) - 118;

    return(RssidBm);
}

/**
 ****************************************************************************************
 * RADIO FUNCTION INTERFACE
 ****************************************************************************************
 **/

void rf_init(struct rwip_rf_api *api)
{
    uint32_t icy_version = 0; // Default version is Atlas
    uint8_t idx = 0;

#if !defined(RP_HWSIM_BYPASS)
    uint8_t vco_sub_read = 0;
#endif // RP_HWSIM_BYPASS

    // Initialize the RF driver API structure
    api->reg_rd = rf_atl_reg_rd;
    api->reg_wr = rf_atl_reg_wr;
    api->txpwr_dbm_get = rf_txpwr_dbm_get;
    api->txpwr_max = ATL_POWER_MAX;
    api->sleep = rf_sleep;
    api->reset = rf_reset;
    api->force_agc_enable = rf_force_agc_enable;
    api->rssi_convert = rf_rssi_convert;

    // --------------- RADIOCNTL0 register ---------------
    // Set pointer SW SPI Drive access Pointer
    ble_spiptr_setf(EM_RF_SW_SPI_OFFSET);

    // --------------- RADIOCNTL1 register ---------------
    // RF Selection
    ble_xrfsel_setf(0x03);

    // Select Jitter elimination FIFO
    ble_jef_select_setf(0x1);

    // select sync Pulse Mode
    ble_sync_pulse_mode_setf(0x1);

#if !defined(RP_HWSIM_BYPASS)
    // Detect the RF version
    icy_version = rf_atl_reg_rd(0xFF);
    // Select proper sub-version of IcyTRx Radio Controller / Need BLE Core xls update 1st
    switch(icy_version)
    {
        case(0x30):  // CS1
        {
            ble_subversion_setf(0x1);
            // Set Platform RF selection register
            //plf_rf_interface_sel(RF_INTF_V3X_V4X);
        }
        break;
        case(0x31):  // CS2
        {
            ble_subversion_setf(0x2);
            // Set Platform RF selection register
            //plf_rf_interface_sel(RF_INTF_V3X_V4X);
        }
        break;
        case(0x40):
        case(0x41):// GCS1
        {
            ble_subversion_setf(0x3);
            // Set Platform RF selection register
            //plf_rf_interface_sel(RF_INTF_V3X_V4X);
        }
        break;
        default:     // v0x2-
        {
            ble_subversion_setf(0x0);
            // Reset Platform RF selection register
            //plf_rf_interface_sel(RF_INTF_V1X_V2X);
        }
        break;
    }
#else
    // Stick to CS2 version to prevent ASSERT
    icy_version = 0x31;
#endif

  // ------------ Tx/Rx Power up/down and Timings -------------------
  switch(icy_version)
  {
  case(0x20): // Aligned with OT driver - // To be verified
    // 1Mbps settings
    ble_rxpwrup0_setf(113);
    ble_txpwrdn0_setf(15);
    ble_txpwrup0_setf(33);
    ble_txpathdly0_setf(8);
    ble_rxpathdly0_setf(30);
    ble_rfrxtmda0_setf(8);
    ble_sync_position0_setf(0);
    // 2Mbps settings
    ble_rxpwrup1_setf(113);
    ble_txpwrdn1_setf(15);
    ble_txpwrup1_setf(31);
    ble_txpathdly1_setf(4);
    ble_rxpathdly1_setf(15);
    ble_rfrxtmda1_setf(4);
    ble_sync_position1_setf(0);
	break;

  case(0x30): // To be verified
    // 1Mbps settings
    ble_rxpwrup0_setf(113);
    ble_txpwrdn0_setf(15);
    ble_txpwrup0_setf(49);
    ble_txpathdly0_setf(14);
    ble_rxpathdly0_setf(38);
    ble_rfrxtmda0_setf(8);
    ble_sync_position0_setf(0);
    // 2Mbps settings
    ble_rxpwrup1_setf(113);
    ble_txpwrdn1_setf(15);
    ble_txpwrup1_setf(38);
    ble_txpathdly1_setf(7);
    ble_rxpathdly1_setf(19);
    ble_rfrxtmda1_setf(4);
    ble_sync_position1_setf(0);

	break;
  case(0x31): // To be verified
    // 1Mbps settings
    ble_rxpwrup0_setf(111);
    ble_txpwrdn0_setf(15);
    ble_txpwrup0_setf(45);
    ble_txpathdly0_setf(14);
    ble_rxpathdly0_setf(45);
    ble_rfrxtmda0_setf(8);
    ble_sync_position0_setf(0);
    // 2Mbps settings
    ble_rxpwrup1_setf(113);
    ble_txpwrdn1_setf(15);
    ble_txpwrup1_setf(42);
    ble_txpathdly1_setf(7);
    ble_rxpathdly1_setf(26);
    ble_rfrxtmda1_setf(4);
    ble_sync_position1_setf(0);
	break;

  case(0x40):
  case(0x41):// To be verified
    // 1Mbps settings
    ble_rxpwrup0_setf(106);
    ble_txpwrdn0_setf(15);
    ble_txpwrup0_setf(42);
    ble_txpathdly0_setf(8);
    ble_rxpathdly0_setf(45);
    ble_rfrxtmda0_setf(8);
    ble_sync_position0_setf(0);
    // 2Mbps settings
    ble_rxpwrup1_setf(106);
    ble_txpwrdn1_setf(15);
    ble_txpwrup1_setf(42);
    ble_txpathdly1_setf(6);
    ble_rxpathdly1_setf(21);
    ble_rfrxtmda1_setf(4);
    ble_sync_position1_setf(0);
    break;

  default:
   ASSERT_ERR(0);
  break;
  }

  // IcyTRx Register Initialization
  switch(icy_version)
  {
  case(0x20):
		rf_atl_reg_burst_wr(0x00, 0x70, (uint8_t *) &RF_ATL_REG_TBL_20[0]);
		rf_atl_reg_burst_wr(0x70, 0x27, (uint8_t *) &RF_ATL_REG_TBL_20[0x70]);
	break;
  case(0x30): // CS1
		rf_atl_reg_burst_wr(0x00, 0x70, (uint8_t *) &RF_ATL_REG_TBL_30[0]);
		rf_atl_reg_burst_wr(0x70, 0x37, (uint8_t *) &RF_ATL_REG_TBL_30[0x70]);
	break;
  case(0x31): // CS2
    // 1Mbps config setting
    rf_atl_reg_wr(0x1A,0x00);
    rf_atl_reg_burst_wr(0x00, 0x70, (uint8_t *) &RF_ATL_REG_TBL_31_1MBPS[0]);
    rf_atl_reg_burst_wr(0x70, 0x4C, (uint8_t *) &RF_ATL_REG_TBL_31_1MBPS[0x70]);
    // 2Mbps config settings
    rf_atl_reg_wr(0x1A,0x01);
    rf_atl_reg_burst_wr(0x00, 0x70, (uint8_t *) &RF_ATL_REG_TBL_31_2MBPS[0]);
    rf_atl_reg_burst_wr(0x70, 0x4C, (uint8_t *) &RF_ATL_REG_TBL_31_2MBPS[0x70]);
    rf_atl_reg_wr(0x1A,0x00);
	break;
  case(0x40): // GCS1
  case(0x41):
    // 1Mbps config setting
    rf_atl_reg_wr(0x16,0x00);
    rf_atl_reg_burst_wr(0x00, 0x70, (uint8_t *) &RF_ATL_REG_TBL_40_1MBPS[0]);
    rf_atl_reg_burst_wr(0x70, 0x4E, (uint8_t *) &RF_ATL_REG_TBL_40_1MBPS[0x70]);
    rf_atl_reg_wr(0xBE, RF_ATL_REG_TBL_40_1MBPS[0xBE]);
    // 2Mbps config setting
    rf_atl_reg_wr(0x16,0x01);
    rf_atl_reg_burst_wr(0x00, 0x70, (uint8_t *) &RF_ATL_REG_TBL_40_2MBPS[0]);
    rf_atl_reg_burst_wr(0x70, 0x4E, (uint8_t *) &RF_ATL_REG_TBL_40_2MBPS[0x70]);
    rf_atl_reg_wr(0xBE, RF_ATL_REG_TBL_40_2MBPS[0xBE]);
    rf_atl_reg_wr(0x16,0x00);
    break;
  default:
    ASSERT_ERR(0);
  break;
  }

  // Frequency Table Programming
  uint32_t *ft_ptr=(uint32_t *)(EM_BASE_ADDR+EM_FT_OFFSET);
  for(idx=0; idx < EM_RF_FREQ_TABLE_LEN/4; idx++)
  {
    switch(icy_version)
    {
    case(0x20):
      *ft_ptr++=RF_ATL_FREQ_TBL20[idx];
	  break;
    case(0x30):
      *ft_ptr++=RF_ATL_FREQ_TBL30[idx];
      break;
    case(0x31):
      *ft_ptr++=RF_ATL_FREQ_TBL31[idx];
	  break;
    case(0x40):
    case(0x41):
      *ft_ptr++=RF_ATL_FREQ_TBL40[idx];
      break;
    default:
      ASSERT_ERR(0);
    break;
    }
  }
  
  // VCO sub-band table Reset
  uint8_t *vco_ptr=(uint8_t *)(EM_BASE_ADDR+EM_FT_OFFSET+EM_RF_FREQ_TABLE_LEN);
  for(idx=0; idx < EM_RF_VCO_TABLE_LEN; idx++)
    {
    *vco_ptr++=RF_ATL_VCO_TBL[idx];
    }

  #if !defined(RP_HWSIM_BYPASS)
  // VCO Sub-band Calibration process / bypassed in HW simulations
  switch(icy_version)
  {
  case(0x20):
    rf_atl_reg_wr(ICY_TIMING,0x90);
	break;
  case(0x30):
    rf_atl_reg_wr(ICY30_TIMING,0xB0);
    break;
  case(0x31):
	rf_atl_reg_wr(ICY31_TIMING,0x81);
	break;
  case(0x40):
  case(0x41):
    //rf_atl_reg_wr(ICY40_TIMING,0x81);
    break;
  default:
  break;
  }

  for(idx=0; idx < EM_RF_VCO_TABLE_LEN; idx++)
    {
    // Initialize VCO sub Table pointer in EM
    uint8_t *ptr_vcosub=(uint8_t *)(EM_BASE_ADDR+EM_FT_OFFSET+EM_RF_FREQ_TABLE_LEN+idx);

    // Program PLLCNTL values / take value from RF_ATL_FREQ_TBL
    switch(icy_version)
    {
    case(0x20):
      rf_atl_reg_burst_wr(ICY2x_CENTER_FREQ, 4, (uint8_t *) &RF_ATL_FREQ_TBL20[idx]);
	  break;
    case(0x30):
      rf_atl_reg_burst_wr(ICY3x_CENTER_FREQ, 4, (uint8_t *) &RF_ATL_FREQ_TBL30[idx]);
	  break;
    case(0x31):
      rf_atl_reg_burst_wr(ICY3x_CENTER_FREQ, 4, (uint8_t *) &RF_ATL_FREQ_TBL31[idx]);
    break;
    case(0x40):
    case(0x41):
       rf_atl_reg_burst_wr(ICY40_CENTER_FREQ, 4, (uint8_t *) &RF_ATL_FREQ_TBL40[idx]);
    break;
    default:
    break;
    }

    // TxEN + Enable VCO Calibration in RF for v0x1-,  v0x2- and CS1 / @0xC0 write 0x6
    // RxEN + Enable VCO Calibration in RF for CS2 / @0xC0 write 0x3
    if(icy_version!=0x31)
       rf_atl_reg_wr(ICY_FSM_MODE,0x06);
    else
       rf_atl_reg_wr(ICY_FSM_MODE,0x03);

    // Poll on @0xC0 until it is equal to 0x0
    while (rf_atl_reg_rd(ICY_FSM_MODE) != 0x00);

    // WARNING !! Problem of SW dynamic resolved by adding this loop in order to prevent from erasing VCO table @ index 0
    for(uint32_t cpt=0;cpt<1000;cpt++);

    // Store VCO sub-band read value
    switch(icy_version)
      {
      case(0x30):
        vco_sub_read = rf_atl_reg_rd((uint16_t)ICY30_SW_CAP_FSM);
        // Invert half byte as Rx VCO sub-band programming is using bit [7:4]
        *ptr_vcosub = (vco_sub_read << 4) | (vco_sub_read >> 4);
      break;
      case(0x31):
        vco_sub_read = rf_atl_reg_rd((uint16_t)ICY31_SW_CAP_FSM);
        *ptr_vcosub = vco_sub_read;
      break;
      case(0x40):
      case(0x41):
        vco_sub_read = rf_atl_reg_rd((uint16_t)ICY40_SW_CAP_FSM);
        *ptr_vcosub = (vco_sub_read << 4) | (vco_sub_read >> 4);
      break;
      default:
        vco_sub_read = rf_atl_reg_rd((uint16_t)ICY2x_SW_CAP_FSM);
        // Invert half byte as Rx VCO sub-band programming is using bit [7:4]
        *ptr_vcosub = (vco_sub_read << 4) | (vco_sub_read >> 4);
      break;
      }
    }

    switch(icy_version)
    {
    case(0x20):
      rf_atl_reg_wr(ICY_TIMING,0x40);
	break;
    case(0x30):
      rf_atl_reg_wr(ICY30_TIMING,0xB0);
    break;
    case(0x31):
      rf_atl_reg_wr(ICY31_TIMING,0x00);
    break;
    case(0x40):
    case(0x41):
     // rf_atl_reg_wr(ICY40_TIMING,0xB0);
	break;
    default:
    break;
    }

  #endif // RP_HWSIM_BYPASS

  /* *************************************************************************************** */
  /* Initialize HW SPI Chains Pointers  */
  /* *************************************************************************************** */
  uint16_t txonptr  = (EM_RF_HW_SPI_OFFSET);
  uint16_t txoffptr = (0);
  uint16_t rxonptr  = (EM_RF_HW_SPI_OFFSET+0x16);
  uint16_t rxoffptr = (EM_RF_HW_SPI_OFFSET+0x16+0x26);
  uint16_t rssiptr  = (EM_RF_HW_SPI_OFFSET+0x16+0x26+0x8);

  /* TxOn Sequence start pointer */
  ble_txonptr_setf(txonptr);

  /* TxOff Sequence start pointer */
  ble_txoffptr_setf(txoffptr);

  /* RxOn Sequence start pointer */
  ble_rxonptr_setf(rxonptr);

  /* RxOff Sequence start pointer */
  ble_rxoffptr_setf(rxoffptr);

  /* RSSI Sequence start pointer */
  ble_rssiptr_setf(rssiptr);

  /* *************************************************************************************** */
  /* Initialize HW SPI Tx On Chained list  -> 2 structures of 8 bytes*/
  /* *************************************************************************************** */
  /*   TxON Access 1 -> RF Bank Selection
               -> Next Pointer = txonptr+0x6
               -> Write Address @0x30 / 1 byte for v0x2-
               -> Write Address @0x1A / 1 byte for CS1 and CS2
               -> Write Address @0x16 / 1 byte for GCS1
               -> Write data = 0x0000 -> Will be replaced by CS-TXRATE or CS-RXRATE accordingly */
  RF_ICTRX_EM_BLE_WR(txonptr,     txonptr+0x6);
  switch(icy_version)
    {
    case(0x30):   // CS1
  	  RF_ICTRX_EM_BLE_WR(txonptr+0x2, 0x1A81);
    break;
    case(0x31):   // CS2
  	  RF_ICTRX_EM_BLE_WR(txonptr+0x2, 0x1A81);
    break;
    case(0x40):   // GCS1
    case(0x41):
      RF_ICTRX_EM_BLE_WR(txonptr+0x2, 0x1681);
    break;
    default:
  	  RF_ICTRX_EM_BLE_WR(txonptr+0x2, 0x3081);
    break;
    }
  RF_ICTRX_EM_BLE_WR(txonptr+0x4, 0x0000);


  /*   TxON Access 2 -> PLL word
               -> Next Pointer = txonptr+0xE
               -> Write Address @0x2C / 4 byte for v0x2-
               -> Write Address @0x16 / 4 byte for CS1 and CS2
               -> Write data = 0x00000000 -> Will be replaced by PLL word value from table above */
  RF_ICTRX_EM_BLE_WR(txonptr+0x6,   txonptr+0xE);
  switch(icy_version)
  {
  case(0x30):   // CS1
	RF_ICTRX_EM_BLE_WR(txonptr+0x8, 0x1684);
  break;
  case(0x31):   // CS2
	RF_ICTRX_EM_BLE_WR(txonptr+0x8, 0x1684);
  break;
  case(0x40):   // GCS1
  case(0x41):
    RF_ICTRX_EM_BLE_WR(txonptr+0x8, 0x1884);
  break;
  default:
	RF_ICTRX_EM_BLE_WR(txonptr+0x8, 0x2C84);
  break;
  }
  RF_ICTRX_EM_BLE_WR(txonptr+0xA, 0x0000);
  RF_ICTRX_EM_BLE_WR(txonptr+0xC, 0x0000);

  /*   TxON Access 2 -> Tx enable
               -> Next Pointer = 0x0000 -> end of Sequence
               -> Write Address @0xC0 / 1 byte / Valid for all RF version at the moment
               -> Write data = 0x07 */
  RF_ICTRX_EM_BLE_WR(txonptr+0xE, 0x0000);
  RF_ICTRX_EM_BLE_WR(txonptr+0x10, 0xC081);
  RF_ICTRX_EM_BLE_WR(txonptr+0x12, 0x0007);
  RF_ICTRX_EM_BLE_WR(txonptr+0x14, 0x0000);

  /* *************************************************************************************** */
  /* Initialize HW SPI Tx Off Chained list -> Nothing here at the moment */
  /* *************************************************************************************** */

  /* *************************************************************************************** */
  /* Initialize HW SPI Rx On Chained list -> 4 structures of 8 bytes */
  /* *************************************************************************************** */
  /* Initialize HW SPI Rx On Chained list  */
  /*   RxON Access 1 -> RF Bank Selection
               -> Next Pointer = txonptr+0x6
               -> Write Address @0x30 / 1 byte for v0x2-
               -> Write Address @0x1A / 1 byte for CS1 and CS2
               -> Write Address @0x16 / 1 byte for GCS1
               -> Write data = 0x0000 -> Will be replaced by CS-TXRATE or CS-RXRATE accordingly */
  RF_ICTRX_EM_BLE_WR(rxonptr,     rxonptr+0x6);
  switch(icy_version)
    {
    case(0x30):   // CS1
  	  RF_ICTRX_EM_BLE_WR(rxonptr+0x2, 0x1A81);
    break;
    case(0x31):   // CS2
  	  RF_ICTRX_EM_BLE_WR(rxonptr+0x2, 0x1A81);
    break;
    case(0x40):   // GCS1
    case(0x41):
      RF_ICTRX_EM_BLE_WR(rxonptr+0x2, 0x1681);
    break;
    default:
  	  RF_ICTRX_EM_BLE_WR(rxonptr+0x2, 0x3081);
    break;
    }
  RF_ICTRX_EM_BLE_WR(rxonptr+0x4, 0x0000);

  /*   RxON Access 2 -> PLL word
               -> Next Pointer = rxonptr+0xE
               -> Write Address @0x2C / 4 byte for v0x2-
               -> Write Address @0x16 / 4 byte for CS1 and CS2
                -> Write data = 0x00000000 -> Will be replaced by PLL word value from table in EM*/
  RF_ICTRX_EM_BLE_WR(rxonptr+0x6,  rxonptr+0xE);
  switch(icy_version)
  {
  case(0x30):   // CS1
	RF_ICTRX_EM_BLE_WR(rxonptr+0x8, 0x1684);
  break;
  case(0x31):   // CS2
	RF_ICTRX_EM_BLE_WR(rxonptr+0x8, 0x1684);
  break;
  case(0x40):   // GCS1
  case(0x41):
    RF_ICTRX_EM_BLE_WR(rxonptr+0x8, 0x1884);
  break;
  default:
	RF_ICTRX_EM_BLE_WR(rxonptr+0x8, 0x2C84);
  break;
  }
  RF_ICTRX_EM_BLE_WR(rxonptr+0xA,  0x0000);
  RF_ICTRX_EM_BLE_WR(rxonptr+0xC,  0x0000);

  /*   RxON Access 3 -> VCO SW Cap (v0x2-) or VCO SW Cap auto Calibration (v0x3-)
               -> Next Pointer = rxonptr+0x16
               -> Write Address @0x88 / 1 byte / for v0x1- to v0x2-
               -> Write Address @0x96 / 1 byte / for CS1
               -> Write Address @0xA2 / 1 byte / for CS2 -> replaced below
               -> Write data = 0x00 -> will be replaced by VCO sub-band value from table in EM */
  RF_ICTRX_EM_BLE_WR(rxonptr+0xE,  rxonptr+0x16);
  switch(icy_version)
  {
  case(0x30):   // CS1
	RF_ICTRX_EM_BLE_WR(rxonptr+0x10,  0x9681);
    RF_ICTRX_EM_BLE_WR(rxonptr+0x12,  0x0000);
  break;
  case(0x31):   // CS2
	RF_ICTRX_EM_BLE_WR(rxonptr+0x10,  0xA281);
    RF_ICTRX_EM_BLE_WR(rxonptr+0x12,  0x0000);
  break;
  case(0x40):   // GCS1
  case(0x41):
    RF_ICTRX_EM_BLE_WR(rxonptr+0x10,  0xA281);
    RF_ICTRX_EM_BLE_WR(rxonptr+0x12,  0x0000);
  break;
  default:
	RF_ICTRX_EM_BLE_WR(rxonptr+0x10,  0x8881);
	RF_ICTRX_EM_BLE_WR(rxonptr+0x12,  0x0000);
  break;
  }

  RF_ICTRX_EM_BLE_WR(rxonptr+0x14,  0x0000);

  /*   RxON Access 4 -> Rx enable
               -> Next Pointer = rxonptr+0x1E
               -> Write Address @0xC0 / 1 byte / Valid for all RF version at the moment
               -> Write data = 0x01 (for v0x2-) or 0x03 (for CS2) */
  RF_ICTRX_EM_BLE_WR(rxonptr+0x16, rxonptr+0x1E);
  RF_ICTRX_EM_BLE_WR(rxonptr+0x18, 0xC081);
  switch(icy_version)
  {
  case(0x31):   // CS2
	RF_ICTRX_EM_BLE_WR(rxonptr+0x1A, 0x0001);
  break;
  case(0x40):   // GCS1
  case(0x41):
    RF_ICTRX_EM_BLE_WR(rxonptr+0x1A, 0x0001);
  break;
  default:
	RF_ICTRX_EM_BLE_WR(rxonptr+0x1A, 0x0001);
  break;
  }
  RF_ICTRX_EM_BLE_WR(rxonptr+0x1C, 0x0000);
    
  /*   RxON Access 5 -> Access Address Write
               -> Next Pointer = 0x0000 -> end of Sequence
               -> Write Address @0x0F / 4 byte / for v0x2-
               -> Write Address @0x2C / 4 byte / for CS1 and CS2
               -> Write data = 0x00 -> will be replaced by 32bit Access Address by the Radio Controller */
  RF_ICTRX_EM_BLE_WR(rxonptr+0x1E, 0x0000);
  switch(icy_version)
  {
  case(0x30):   // CS1
	RF_ICTRX_EM_BLE_WR(rxonptr+0x20, 0x2C84);
  break;
  case(0x31):   // CS2
	RF_ICTRX_EM_BLE_WR(rxonptr+0x20, 0x2C84);
  break;
  case(0x40):   // GCS1
  case(0x41):
    RF_ICTRX_EM_BLE_WR(rxonptr+0x20, 0x2C84);
  break;
  default:
    RF_ICTRX_EM_BLE_WR(rxonptr+0x20, 0x0F84);
  break;
  }
  RF_ICTRX_EM_BLE_WR(rxonptr+0x22, 0x0000);
  RF_ICTRX_EM_BLE_WR(rxonptr+0x24, 0x0000);

  /* *************************************************************************************** */
  /* Initialize HW SPI Rx Off Chained list -> 1 structure of 8 bytes */
  /* *************************************************************************************** */
  /*    RxOFF Access 1  -> Rx Disable
                -> Next Pointer = 0x0000 -> end of Sequence
                -> Write Address @0xC0 / 1 byte
                -> Write data = 0x08 */

  RF_ICTRX_EM_BLE_WR(rxoffptr,     0x0000);
  RF_ICTRX_EM_BLE_WR(rxoffptr+0x2, 0xC081);
  RF_ICTRX_EM_BLE_WR(rxoffptr+0x4, 0x0008);
  RF_ICTRX_EM_BLE_WR(rxoffptr+0x6, 0x0000);

  /* *************************************************************************************** */
  /* Initialize HW SPI RSSI Chained list  -> 1 structure of 8 bytes*/
  /* *************************************************************************************** */
  /*    RSSI Access   -> RSSI read
               -> Next Pointer = 0x0000 -> end of the sequence
               -> Write Address @0xCA / 1 byte / Valid for all RF version at the moment
               -> Write data = 0x000000 -> Read data to replace this / provided to Packet Controller */
  RF_ICTRX_EM_BLE_WR(rssiptr,     0x0000);
  RF_ICTRX_EM_BLE_WR(rssiptr+0x2, 0xCA01);
  RF_ICTRX_EM_BLE_WR(rssiptr+0x4, 0x0000);
  RF_ICTRX_EM_BLE_WR(rssiptr+0x6, 0x0000);

};
///@} RF_ATLAS
