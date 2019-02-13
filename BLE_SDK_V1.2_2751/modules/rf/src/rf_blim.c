/**
****************************************************************************************
*
* @file rf_blim.c
*
* @brief Blim radio initialization and specific functions
*
* Copyright (C) RivieraWaves 2009-2015
*
* $Rev: $
*
****************************************************************************************
*/

/**
****************************************************************************************
* @addtogroup RF_BLIM
* @ingroup RF
* @brief Blim Radio Driver
*
* This is the driver block for Blim radio
* @{
****************************************************************************************
*/

/**
 *****************************************************************************************
 * INCLUDE FILES
 *****************************************************************************************
 */
#include <string.h>        // for memcpy
#include "co_utils.h"      // common utility definition
#include "co_math.h"       // common math functions
#include "co_endian.h"     // endian definitions
#include "rf.h"            // RF interface
#include "em_map.h"        // RF area mapping

#include "rwip.h"          // for RF API structure definition
#include "reg_blecore.h"   // ble core registers
#include "reg_ble_em_cs.h" // control structure definitions

// Blim register definitions and access functions
static uint32_t rf_blim_reg_rd (uint16_t addr);
static void rf_blim_reg_wr (uint16_t addr, uint32_t value);

#define REG_BLIM_RD        rf_blim_reg_rd
#define REG_BLIM_WR        rf_blim_reg_wr

/**
 ****************************************************************************************
 * DEFINES
 ****************************************************************************************
 **/

#define RF_EM_SPI_ADRESS        (EM_BASE_ADDR + EM_RF_SW_SPI_OFFSET)

#define BLIM_SPIRD              0x00
#define BLIM_SPIWR              0x80

#define BLIM_MAX_BURST_SIZE     0x80
#define ICY31_INIT_TBL_SIZE     0xBD
#define ICY32_INIT_TBL_SIZE     0xBE

// TX max power
#define BLIM_POWER_MAX          0x06


/**
****************************************************************************************
* MACROS
*****************************************************************************************
*/

/// IcyTRx EM Write Macro for HW driven SPI chained structures
#define RF_ICTRX_EM_BLE_WR(addr, val) \
    EM_BLE_WR((((uint32_t) (addr)) + REG_BLE_EM_CS_BASE_ADDR), (val))


// Max burst register
static uint8_t rf_blim_reg_buf[BLIM_MAX_BURST_SIZE + 2]; // max burst size + buffer controls

/**
 ****************************************************************************************
 * GLOBAL VARIABLE DEFINITIONS
 *****************************************************************************************
 **/

/* Icytrx v0x31 static settings - CS2 - 1Mbps table */
static const uint8_t RF_BLIM_REG_TBL_31_1MBPS[ICY31_INIT_TBL_SIZE] =
{ 0x22, 0x08, 0x00, 0x00,   0x00, 0x23, 0x0b, 0x14,    0x05, 0x20, 0x00, 0x10,   0x03, 0x00, 0xff, 0xff, // 0x0*
  0x0f, 0x00, 0x00, 0x80,   0x23, 0x82, 0x1b, 0xc7,    0x15, 0x82, 0x00, 0x00,   0x01, 0x01, 0x72, 0x1c, // 0x1*
  0x07, 0x00, 0x82, 0xff,   0x01, 0x55, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00,   0x29, 0x41, 0x76, 0x71, // 0x2*
  0x03, 0x2d, 0x03, 0x80,   0x00, 0x55, 0x55, 0x55,    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, // 0x3*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01,    0x02, 0x07, 0x10, 0x20,   0x37, 0x50, 0x66, 0x76, // 0x4*
  0x7e, 0x2b, 0x10, 0xc0,   0x21, 0x74, 0xca, 0xfe,    0x3f, 0x00, 0x02, 0x50,   0x0f, 0x1b, 0x30, 0x44, // 0x5*
  0x0b, 0x55, 0x15, 0x11,   0x1b, 0x09, 0x0b, 0x15,    0x0c, 0xfe, 0x50, 0x5c,   0x84, 0x00, 0x00, 0x04, // 0x6*
  0x24, 0x30, 0x81, 0x11,   0x9c, 0x00, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0xdc, 0xc8, // 0x7*
  0x0d, 0x00, 0x00, 0x00,   0x00, 0x33, 0xc0, 0x22,    0x61, 0x01, 0x00, 0x00,   0x00, 0x73, 0x66, 0x72, // 0x8*
  0x99, 0x75, 0x9a, 0x77,   0x37, 0x72, 0x13, 0x68,    0x25, 0x00, 0x00, 0x08,   0x0f, 0xc2, 0xc2, 0x16, // 0x9*
  0x18, 0x00, 0x00, 0x07,   0x7f, 0x80, 0x00, 0x00,    0x00, 0x3a, 0xd0, 0xc3,   0x78, 0x04, 0x04, 0xf0, // 0xa*
  0x00, 0x03, 0x00, 0x4b,   0x11, 0x00, 0x00, 0x40,    0x00, 0x00, 0x86, 0x25,   0x00
  };

/* Icytrx v0x31 static settings - CS2 - 2Mbps table */
static const uint8_t RF_BLIM_REG_TBL_31_2MBPS[ICY31_INIT_TBL_SIZE] =
{ 0x22, 0x08, 0x00, 0x00,   0x00, 0x23, 0x0b, 0x14,    0x05, 0x20, 0x00, 0x10,   0x03, 0x00, 0xff, 0xff, // 0x0*
  0x0f, 0x00, 0x00, 0x80,   0x23, 0x82, 0x1b, 0xc7,    0x15, 0x82, 0x01, 0x00,   0x00, 0x00, 0x72, 0x1c, // 0x1*
  0x07, 0x00, 0x82, 0xff,   0x01, 0x55, 0x01, 0x00,    0x00, 0x00, 0x00, 0x00,   0x29, 0x41, 0x76, 0x71, // 0x2*
  0x03, 0x2d, 0x03, 0x80,   0x00, 0x55, 0x55, 0x55,    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, // 0x3*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01,    0x02, 0x07, 0x10, 0x20,   0x37, 0x50, 0x66, 0x76, // 0x4*
  0x7e, 0x3b, 0x00, 0xc0,   0x21, 0x74, 0xca, 0xfe,    0x3f, 0x80, 0x02, 0x70,   0x0f, 0x1b, 0x30, 0x44, // 0x5*
  0x0b, 0xaa, 0x2a, 0x11,   0x15, 0x07, 0x0b, 0x15,    0x0c, 0xfe, 0x50, 0x5c,   0x84, 0x00, 0x00, 0x04, // 0x6*
  0x24, 0x30, 0x81, 0x11,   0x9c, 0x00, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0xdc, 0xc8, // 0x7*
  0x0d, 0x00, 0x00, 0x00,   0x00, 0x33, 0xc0, 0x22,    0x61, 0x01, 0x00, 0x00,   0x00, 0x73, 0x66, 0x72, // 0x8*
  0x99, 0x75, 0x9a, 0x77,   0x37, 0x72, 0x13, 0x68,    0x25, 0x00, 0x00, 0x08,   0x0f, 0xc2, 0xc2, 0x16, // 0x9*
  0x18, 0x00, 0x00, 0x07,   0x7f, 0x80, 0x00, 0x00,    0x00, 0x3a, 0xd0, 0xc3,   0x78, 0x04, 0x04, 0xf0, // 0xa*
  0x00, 0x03, 0x00, 0x4b,   0x11, 0x00, 0x00, 0x40,    0x00, 0x00, 0x86, 0x25,   0x00
  };

/* Icytrx v0x32 static settings - CS3 - 1Mbps table */
static const uint8_t RF_BLIM_REG_TBL_32_1MBPS[ICY32_INIT_TBL_SIZE] =
{ 0x22, 0x08, 0x00, 0x00,   0x00, 0x22, 0x0b, 0x14,    0x05, 0x20, 0x00, 0x10,   0x03, 0x00, 0xff, 0xff, // 0x0*
  0x0f, 0x00, 0x00, 0x80,   0x23, 0x82, 0x1b, 0xc7,    0x15, 0x82, 0x00, 0x00,   0x01, 0x01, 0x72, 0x1c, // 0x1*
  0x07, 0x00, 0x82, 0xff,   0x01, 0x55, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00,   0x29, 0x41, 0x76, 0x71, // 0x2*
  0x03, 0x2d, 0x03, 0x80,   0x00, 0x55, 0x55, 0x55,    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, // 0x3*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01,    0x02, 0x07, 0x10, 0x20,   0x37, 0x50, 0x66, 0x76, // 0x4*
  0x7e, 0x2b, 0x10, 0xc0,   0x21, 0x74, 0xca, 0xfe,    0x3f, 0x00, 0x02, 0x50,   0x2f, 0x1b, 0x30, 0x44, // 0x5*
  0x0b, 0x55, 0x15, 0x10,   0x1b, 0x09, 0x3a, 0x0b,    0x17, 0x0c, 0xfe, 0x60,   0x60, 0x99, 0x00, 0x00, // 0x6*
  0x01, 0x88, 0x48, 0xc0,   0x02, 0x1e, 0xf0, 0x02,    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0xae, // 0x7*
  0xcd, 0x02, 0x00, 0x00,   0xf1, 0x00, 0x33, 0xc0,    0x22, 0x61, 0x01, 0x00,   0x00, 0x00, 0x73, 0x66, // 0x8*
  0x28, 0x77, 0x75, 0x5a,   0x77, 0x66, 0x92, 0xb4,    0x0c, 0x79, 0x67, 0x00,   0x00, 0x08, 0x0f, 0xc2, // 0x9*
  0xc2, 0x16, 0x18, 0x00,   0x00, 0x07, 0x4a, 0x41,    0x00, 0x00, 0x00, 0x35,   0x84, 0xc3, 0x80, 0x04, // 0xa*
  0x04, 0xf0, 0x00, 0x03,   0x4b, 0x11, 0x00, 0x00,    0x40, 0x00, 0x00, 0x88,   0x25, 0x00 
 };

/* Icytrx v0x32 static settings - CS3 - 2Mbps table */
// [TODO] => Add 2Mbps table

/* Frequency Table for v0x31 (CS2) and v0x32 (CS3) */
static const uint32_t RF_BLIM_FREQ_TBL[EM_RF_FREQ_TABLE_LEN/4] =
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
static uint8_t RF_BLIM_VCO_TBL[EM_RF_VCO_TABLE_LEN] =
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
    // 0x0C00 offset of icytrx register in BLIM
    BLIM_VERSION               = 0x0CFF,
    BLIM_ICY31_CENTER_FREQ     = 0x0C16, 
    BLIM_ICY31_FSM_MODE        = 0x0CC0,
    BLIM_ICY31_SW_CAP_FSM      = 0x0CA2,
    BLIM_ICY31_TIMEOUT         = 0x0C8B,
    BLIM_ICY31_BANK            = 0x0C1A,
    BLIM_ICY32_CENTER_FREQ     = 0x0C16,
    BLIM_ICY32_FSM_MODE        = 0x0CC0,
    BLIM_ICY32_SW_CAP_FSM      = 0x0CA4,
    BLIM_ICY32_TIMEOUT         = 0x0C8C,
    BLIM_ICY32_BANK            = 0x0C1A
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
static void rf_blim_spi_tf(void)
{
    //launch SPI transfer
    ble_spigo_setf(1);

    //wait for transfer to be completed
    while (!ble_spicomp_getf());
}

/**
 ****************************************************************************************
 * @brief Blim specific read access
 *
 * @param[in] addr    register address
 *
 * @return uint32_t value
 *****************************************************************************************
 */
static uint32_t rf_blim_reg_rd (uint16_t addr)
{
    // Next Pointr to 0x0
    rf_blim_reg_buf[0] = (uint8_t)(0);  // Next Pointer set to 0x0000 to stop the SPI Chained access
    rf_blim_reg_buf[1] = (uint8_t)(0);

    //copy control and number of u32 to send
    rf_blim_reg_buf[2] = (uint8_t)(BLIM_SPIRD + 1);

    //copy address
    rf_blim_reg_buf[3] = (uint8_t) (addr >> 8);
    rf_blim_reg_buf[4] = (uint8_t) addr;

    memcpy((void *)RF_EM_SPI_ADRESS, rf_blim_reg_buf, 5);

    //do the transfer
    rf_blim_spi_tf();

    return (uint32_t)(*((uint8_t *)(RF_EM_SPI_ADRESS + 5)));
}

/**
 ****************************************************************************************
 * @brief Blim specific write access
 *
 * @param[in] addr    register address
 * @param[in] value   value to write
 *
 * @return uint32_t value
 ****************************************************************************************
 */
static void rf_blim_reg_wr (uint16_t addr, uint32_t value)
{
    rf_blim_reg_buf[0] = (uint8_t)(0);  // Next Pointer set to 0x0000 to stop the SPI Chained access
    rf_blim_reg_buf[1] = (uint8_t)(0);

    //inversion for EM reading by U8 on blim SPI side
    //copy control and number of u32 to send
    rf_blim_reg_buf[2] = (uint8_t)(BLIM_SPIWR + 1);

    //copy address
    rf_blim_reg_buf[3] = (uint8_t) (addr >> 8);
    rf_blim_reg_buf[4] = (uint8_t) (addr);
	
    //on old implementations (BT core 3.0, BLE core 1.0) swap the data
    rf_blim_reg_buf[5]= (uint8_t)value;

    memcpy((void *)RF_EM_SPI_ADRESS, rf_blim_reg_buf, 6);

    //do the transfer
    rf_blim_spi_tf();
}

/**
 ****************************************************************************************
 * @brief Blim specific read access
 *
 * @param[in] addr    register address
 * @param[in] size    transfer size
 * @param[in] data    pointer to the data array
 *
 * @return uint32_t value
 ****************************************************************************************
 **/
static void rf_blim_reg_burst_wr (uint16_t addr, uint8_t size, uint8_t *data)
{
    rf_blim_reg_buf[0] = (uint8_t)(0);  // Next Pointer set to 0x0000 to stop the SPI Chained access
    rf_blim_reg_buf[1] = (uint8_t)(0);

    //copy control and number of u8 to send
    rf_blim_reg_buf[2] = (uint8_t)(BLIM_SPIWR + size);

    //copy address
    rf_blim_reg_buf[3] = (uint8_t) (addr >> 8);
    rf_blim_reg_buf[4] = (uint8_t) (addr);
    
    for(int i =0;i < size + 2;i++)
    {
        rf_blim_reg_buf[i + 5] = *(data + i);
    }

    memcpy((void *)RF_EM_SPI_ADRESS, rf_blim_reg_buf, 5 + size);

    //do the transfer
    rf_blim_spi_tf();
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
 * @brief Sleep function for Blim RF.
 *****************************************************************************************
 */
static void rf_sleep(void)
{
    #if defined(CFG_BLE)
    ble_deepslcntl_set(ble_deepslcntl_get() |
                      BLE_DEEP_SLEEP_ON_BIT |    // RW BLE Core sleep
                      BLE_RADIO_SLEEP_EN_BIT|    // Radio sleep
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
    uint32_t icy_version = 0; // Default version is Blim
    uint8_t idx = 0;

#if !defined(RP_HWSIM_BYPASS)
    uint8_t vco_sub_read = 0;
#endif // RP_HWSIM_BYPASS

    // Initialize the RF driver API structure
    api->reg_rd = rf_blim_reg_rd;
    api->reg_wr = rf_blim_reg_wr;
    api->txpwr_dbm_get = rf_txpwr_dbm_get;
    api->txpwr_max = BLIM_POWER_MAX;
    api->sleep = rf_sleep;
    api->reset = rf_reset;
    api->force_agc_enable = rf_force_agc_enable;
    api->rssi_convert = rf_rssi_convert;

    // --------------- RADIOCNTL0 register ---------------
    // Set pointer SW SPI Drive access Pointer
    ble_spiptr_setf(EM_RF_SW_SPI_OFFSET);

    // --------------- RADIOCNTL1 register ---------------
    // RF Selection
    ble_xrfsel_setf(0x04);

    // Select Jitter elimination FIFO
    ble_jef_select_setf(0x1);

    // select sync Pulse Mode
    ble_sync_pulse_mode_setf(0x1);

#if !defined(RP_HWSIM_BYPASS)
    // Detect the RF version
    icy_version = rf_blim_reg_rd(BLIM_VERSION);

    switch(icy_version)
    {
        case(0x31): // CS2
        {
            ble_subversion_setf(0x1);
        }
        break;
        case(0x32): // CS3
        {
            ble_subversion_setf(0x2);
        }
        break;
        default:
        {
            ASSERT_ERR(0); // [TODO] Asser with read value of icytrx
        }
        break;
    }
#else
    // Stick to CS2 version to prevent ASSERT
    icy_version = 0x31;
#endif

    // Blim4sme Register Initialization
    rf_blim_reg_wr(0x0D00,0x01);  // Blim Dig Reg: MODE
    rf_blim_reg_wr(0x0D1B, 0xFF); // Blim Dig Reg: PAD_PULLDOWN_ENABLE
    rf_blim_reg_wr(0x0D12,0x12); // Blim Dig Reg: GPIO_CONF_x
    rf_blim_reg_wr(0x0D13,0x12);
    rf_blim_reg_wr(0x0D14,0x12);
    rf_blim_reg_wr(0x0D15,0x12);
    rf_blim_reg_wr(0x0D16,0x12);
    rf_blim_reg_wr(0x0D17,0x12);
    rf_blim_reg_wr(0x0D18,0x12);
    rf_blim_reg_wr(0x0D19,0x12);
    // [TODO] check if below access are needed
    rf_blim_reg_wr(0x0D02, 0x00);
    rf_blim_reg_wr(0x0D03, 0x00);

  // ------------ BLIMSME RF - Tx/Rx Power up/down and Timings -------------------
  switch(icy_version)
  {
  case(0x31):
  case(0x32):
    // 1Mbps settings
    ble_rxpwrup0_setf(106);
    ble_txpwrdn0_setf(15);
    ble_txpwrup0_setf(42);
    ble_txpathdly0_setf(11);
    ble_rxpathdly0_setf(45);
    ble_rfrxtmda0_setf(8);
    ble_sync_position0_setf(0);

    // 2Mbps settings
    ble_rxpwrup1_setf(106);
    ble_txpwrdn1_setf(15);
    ble_txpwrup1_setf(42);
    ble_txpathdly1_setf(10);
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
  case(0x31):

    // 1Mbps config setting [TODO] Burst of 8 bytes check if can be optimize
    rf_blim_reg_wr(0x0C1A,0x00);
    rf_blim_reg_burst_wr(0x0C00, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0]);
    rf_blim_reg_burst_wr(0x0C08, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0x08]);
    rf_blim_reg_burst_wr(0x0C10, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0x10]);
    rf_blim_reg_burst_wr(0x0C18, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0x18]);
    rf_blim_reg_burst_wr(0x0C20, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0x20]);
    rf_blim_reg_burst_wr(0x0C28, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0x28]);
    rf_blim_reg_burst_wr(0x0C30, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0x30]);
    rf_blim_reg_burst_wr(0x0C38, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0x38]);
    rf_blim_reg_burst_wr(0x0C40, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0x40]);
    rf_blim_reg_burst_wr(0x0C48, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0x48]);
    rf_blim_reg_burst_wr(0x0C50, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0x50]);
    rf_blim_reg_burst_wr(0x0C58, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0x58]);
    rf_blim_reg_burst_wr(0x0C60, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0x60]);
    rf_blim_reg_burst_wr(0x0C68, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0x68]);
    rf_blim_reg_burst_wr(0x0C70, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0x70]);
    rf_blim_reg_burst_wr(0x0C78, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0x78]);
    rf_blim_reg_burst_wr(0x0C80, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0x80]);
    rf_blim_reg_burst_wr(0x0C88, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0x88]);
    rf_blim_reg_burst_wr(0x0C90, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0x90]);
    rf_blim_reg_burst_wr(0x0C98, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0x98]);
    rf_blim_reg_burst_wr(0x0CA0, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0xA0]);
    rf_blim_reg_burst_wr(0x0CA8, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0xA8]);
    rf_blim_reg_burst_wr(0x0CB0, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0xB0]);
    rf_blim_reg_burst_wr(0x0CB8, 0x05, (uint8_t *) &RF_BLIM_REG_TBL_31_1MBPS[0xB8]);

    // 2Mbps config settings
    rf_blim_reg_wr(0x0C1A,0x01);
    rf_blim_reg_burst_wr(0x0C00, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0]);
    rf_blim_reg_burst_wr(0x0C08, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0x08]);
    rf_blim_reg_burst_wr(0x0C10, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0x10]);
    rf_blim_reg_burst_wr(0x0C18, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0x18]);
    rf_blim_reg_burst_wr(0x0C20, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0x20]);
    rf_blim_reg_burst_wr(0x0C28, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0x28]);
    rf_blim_reg_burst_wr(0x0C30, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0x30]);
    rf_blim_reg_burst_wr(0x0C38, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0x38]);
    rf_blim_reg_burst_wr(0x0C40, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0x40]);
    rf_blim_reg_burst_wr(0x0C48, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0x48]);
    rf_blim_reg_burst_wr(0x0C50, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0x50]);
    rf_blim_reg_burst_wr(0x0C58, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0x58]);
    rf_blim_reg_burst_wr(0x0C60, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0x60]);
    rf_blim_reg_burst_wr(0x0C68, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0x68]);
    rf_blim_reg_burst_wr(0x0C70, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0x70]);
    rf_blim_reg_burst_wr(0x0C78, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0x78]);
    rf_blim_reg_burst_wr(0x0C80, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0x80]);
    rf_blim_reg_burst_wr(0x0C88, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0x88]);
    rf_blim_reg_burst_wr(0x0C90, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0x90]);
    rf_blim_reg_burst_wr(0x0C98, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0x98]);
    rf_blim_reg_burst_wr(0x0CA0, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0xA0]);
    rf_blim_reg_burst_wr(0x0CA8, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0xA8]);
    rf_blim_reg_burst_wr(0x0CB0, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0xB0]);
    rf_blim_reg_burst_wr(0x0CB8, 0x05, (uint8_t *) &RF_BLIM_REG_TBL_31_2MBPS[0xB8]);

    // Switch bank back
    rf_blim_reg_wr(0x0C1A,0x00);

	break;
  case(0x32):

    // 1Mbps config setting [TODO] Burst of 8 bytes check if can be optimize
    rf_blim_reg_wr(0x0C1A,0x00);
    rf_blim_reg_burst_wr(0x0C00, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0]);
    rf_blim_reg_burst_wr(0x0C08, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0x08]);
    rf_blim_reg_burst_wr(0x0C10, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0x10]);
    rf_blim_reg_burst_wr(0x0C18, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0x18]);
    rf_blim_reg_burst_wr(0x0C20, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0x20]);
    rf_blim_reg_burst_wr(0x0C28, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0x28]);
    rf_blim_reg_burst_wr(0x0C30, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0x30]);
    rf_blim_reg_burst_wr(0x0C38, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0x38]);
    rf_blim_reg_burst_wr(0x0C40, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0x40]);
    rf_blim_reg_burst_wr(0x0C48, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0x48]);
    rf_blim_reg_burst_wr(0x0C50, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0x50]);
    rf_blim_reg_burst_wr(0x0C58, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0x58]);
    rf_blim_reg_burst_wr(0x0C60, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0x60]);
    rf_blim_reg_burst_wr(0x0C68, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0x68]);
    rf_blim_reg_burst_wr(0x0C70, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0x70]);
    rf_blim_reg_burst_wr(0x0C78, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0x78]);
    rf_blim_reg_burst_wr(0x0C80, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0x80]);
    rf_blim_reg_burst_wr(0x0C88, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0x88]);
    rf_blim_reg_burst_wr(0x0C90, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0x90]);
    rf_blim_reg_burst_wr(0x0C98, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0x98]);
    rf_blim_reg_burst_wr(0x0CA0, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0xA0]);
    rf_blim_reg_burst_wr(0x0CA8, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0xA8]);
    rf_blim_reg_burst_wr(0x0CB0, 0x08, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0xB0]);
    rf_blim_reg_burst_wr(0x0CB8, 0x07, (uint8_t *) &RF_BLIM_REG_TBL_32_1MBPS[0xB8]);

    // [TODO] => Add 2Mbps table

    break;
  default:
    ASSERT_ERR(0);
  break;
  }

    // Frequency Table Programming
    uint32_t *ft_ptr=(uint32_t *)(EM_BASE_ADDR+EM_FT_OFFSET);
    for(idx=0; idx < EM_RF_FREQ_TABLE_LEN/4; idx++)
    {
      *ft_ptr++=RF_BLIM_FREQ_TBL[idx];
    }

    // VCO sub-band table Reset
    uint8_t *vco_ptr=(uint8_t *)(EM_BASE_ADDR+EM_FT_OFFSET+EM_RF_FREQ_TABLE_LEN);
    for(idx=0; idx < EM_RF_VCO_TABLE_LEN; idx++)
    {
      *vco_ptr++=RF_BLIM_VCO_TBL[idx];
    }

#if !defined(RP_HWSIM_BYPASS)
    // VCO Sub-band Calibration process / bypassed in HW simulations
    switch(icy_version)
    {
    case(0x31):
      rf_blim_reg_wr(BLIM_ICY31_TIMEOUT,0x81);
      break;
    case(0x32):
  	  rf_blim_reg_wr(BLIM_ICY32_TIMEOUT,0x81);
	  break;
    default:
    break;
    }

    for(idx=0; idx < EM_RF_VCO_TABLE_LEN; idx++)
    {
      // Initialize VCO sub Table pointer in EM
      uint8_t *ptr_vcosub=(uint8_t *)(EM_BASE_ADDR+EM_FT_OFFSET+EM_RF_FREQ_TABLE_LEN+idx);

      // Program PLLCNTL values / take value from RF_BLIM_FREQ_TBL
      switch(icy_version)
      {
      case(0x31):
        rf_blim_reg_burst_wr(BLIM_ICY31_CENTER_FREQ, 4, (uint8_t *) &RF_BLIM_FREQ_TBL[idx]);
	    break;
      case(0x32):
        rf_blim_reg_burst_wr(BLIM_ICY32_CENTER_FREQ, 4, (uint8_t *) &RF_BLIM_FREQ_TBL[idx]);
        break;
      default:
      break;
      }

      // RxEN + Enable VCO Calibration in RF 
      // Then Poll on until end
      switch(icy_version)
      {
      case(0x31):
        rf_blim_reg_wr(BLIM_ICY31_FSM_MODE,0x03);
        while (rf_blim_reg_rd(BLIM_ICY31_FSM_MODE) != 0x00);
	    break;
      case(0x32):
        rf_blim_reg_wr(BLIM_ICY32_FSM_MODE,0x03);
        while (rf_blim_reg_rd(BLIM_ICY32_FSM_MODE) != 0x00);
        break;
      default:
      break;
      }

      // WARNING !! Problem of SW dynamic resolved by adding this loop in order to prevent from erasing VCO table @ index 0
      for(uint32_t cpt=0;cpt<1000;cpt++);

      // Store VCO sub-band read value
      switch(icy_version)
      {
      case(0x31):
        vco_sub_read = rf_blim_reg_rd((uint16_t)BLIM_ICY31_SW_CAP_FSM);
       *ptr_vcosub = vco_sub_read;
	    break;
      case(0x32):
        vco_sub_read = rf_blim_reg_rd((uint16_t)BLIM_ICY32_SW_CAP_FSM);
        *ptr_vcosub = vco_sub_read;
        break;
      default:
      break;
      }
    }

    switch(icy_version)
    {
    case(0x31):
      rf_blim_reg_wr(BLIM_ICY31_TIMEOUT,0x00);
    break;
    case(0x32):
      rf_blim_reg_wr(BLIM_ICY32_TIMEOUT,0x00);
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
  uint16_t rxoffptr = (EM_RF_HW_SPI_OFFSET+0x16+0x28);
  uint16_t rssiptr  = (EM_RF_HW_SPI_OFFSET+0x16+0x28+0x8);

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
               -> Write Address @0x0C1A / 1 byte for CS2
               -> Write data = 0x0000 -> Will be replaced by CS-TXRATE or CS-RXRATE accordingly */
  RF_ICTRX_EM_BLE_WR(txonptr,     txonptr+0x6);
  switch(icy_version)
  {
    case(0x31):   // CS2
    case(0x32):   // CS3
      RF_ICTRX_EM_BLE_WR(txonptr+0x2, 0x0C81);
      RF_ICTRX_EM_BLE_WR(txonptr+0x4, 0x001A);
    break;
    default:
    break;
  }

  /*   TxON Access 2 -> PLL word (center freq)
               -> Next Pointer = txonptr+0x10
               -> Write Address @0x0C16 / 4 byte for CS2
               -> Write data = 0x00000000 -> Will be replaced by PLL word value from table above */
  RF_ICTRX_EM_BLE_WR(txonptr+0x6,   txonptr+0x10);
  switch(icy_version)
  {
    case(0x31):   // CS2
    case(0x32):   // CS3
      RF_ICTRX_EM_BLE_WR(txonptr+0x8, 0x0C84);
      RF_ICTRX_EM_BLE_WR(txonptr+0xA, 0x0016);
      RF_ICTRX_EM_BLE_WR(txonptr+0xC, 0x0000);
      RF_ICTRX_EM_BLE_WR(txonptr+0xE, 0x0000);
    break;
    default:
    break;
  }

  /*   TxON Access 2 -> Tx enable (FSM MODE)
               -> Next Pointer = 0x0000 -> end of Sequence
               -> Write Address @0x0CC0 / 1 byte / Valid for all RF version at the moment
               -> Write data = 0x07 */
  RF_ICTRX_EM_BLE_WR(txonptr+0x10, 0x0000);
  switch(icy_version)
    {
    case(0x31):   // CS2
    case(0x32):   // CS3
      RF_ICTRX_EM_BLE_WR(txonptr+0x12, 0x0C81);
      RF_ICTRX_EM_BLE_WR(txonptr+0x14, 0x07C0);
    break;
    default:
    break;
    }

  /* *************************************************************************************** */
  /* Initialize HW SPI Tx Off Chained list -> Nothing here at the moment */
  /* *************************************************************************************** */

  /* *************************************************************************************** */
  /* Initialize HW SPI Rx On Chained list -> 4 structures of 8 bytes */
  /* *************************************************************************************** */
  /* Initialize HW SPI Rx On Chained list  */
  /*   RxON Access 1 -> RF Bank Selection
               -> Next Pointer = txonptr+0x6
               -> Write Address @0x0C1A / 1 byte for CS2
               -> Write data = 0x00 -> Will be replaced by CS-TXRATE or CS-RXRATE accordingly */
  RF_ICTRX_EM_BLE_WR(rxonptr,     rxonptr+0x6);
  switch(icy_version)
    {
    case(0x31):   // CS2
    case(0x32):   // CS3
      RF_ICTRX_EM_BLE_WR(rxonptr+0x2, 0x0C81);
      RF_ICTRX_EM_BLE_WR(rxonptr+0x4, 0x001A);
    break;
    default:
    break;
    }

  /*   RxON Access 2 -> PLL word (centerfreq)
               -> Next Pointer = rxonptr+0x10
               -> Write Address @0x0C16 / 4 byte for CS2
                -> Write data = 0x00000000 -> Will be replaced by PLL word value from table in EM*/
  RF_ICTRX_EM_BLE_WR(rxonptr+0x6,  rxonptr+0x10);
  switch(icy_version)
    {
    case(0x31):   // CS2
    case(0x32):   // CS3
      RF_ICTRX_EM_BLE_WR(rxonptr+0x8,  0x0C84);
      RF_ICTRX_EM_BLE_WR(rxonptr+0xA,  0x0016);
      RF_ICTRX_EM_BLE_WR(rxonptr+0xC,  0x0000);
      RF_ICTRX_EM_BLE_WR(rxonptr+0xE,  0x0000);
    break;
    default:
    break;
    }

  /*   RxON Access 3 -> VCO SW Cap (v0x2-) or VCO SW Cap auto Calibration (v0x3-)
               -> Next Pointer = rxonptr+0x16
               -> Write Address @0x0CA2 / 1 byte / for CS2 -> replaced below
               -> Write data = 0x00 -> will be replaced by VCO sub-band value from table in EM */
  RF_ICTRX_EM_BLE_WR(rxonptr+0x10,  rxonptr+0x16);
  switch(icy_version)
    {
    case(0x31):   // CS2
      RF_ICTRX_EM_BLE_WR(rxonptr+0x12,  0x0C81);
      RF_ICTRX_EM_BLE_WR(rxonptr+0x14,  0x00A2);
    break;
    case(0x32):   // CS3
      RF_ICTRX_EM_BLE_WR(rxonptr+0x12,  0x0C81);
      RF_ICTRX_EM_BLE_WR(rxonptr+0x14,  0x00A4);
    break;
    default:
    break;
    }

  /*   RxON Access 4 -> Rx enable (FSM Mode)
               -> Next Pointer = rxonptr+0x1E
               -> Write Address @0x0CC0 / 1 byte / Valid for all RF version at the moment
               -> Write data = 0x01 (for v0x2-) or 0x03 (for CS2) */
  RF_ICTRX_EM_BLE_WR(rxonptr+0x16, rxonptr+0x1E);
  switch(icy_version)
    {
    case(0x31):   // CS2
    case(0x32):   // CS3
      RF_ICTRX_EM_BLE_WR(rxonptr+0x18, 0x0C81);
      RF_ICTRX_EM_BLE_WR(rxonptr+0x1A, 0x01C0);
      RF_ICTRX_EM_BLE_WR(rxonptr+0x1C, 0x0000);
    break;
    default:
    break;
    }
    
  /*   RxON Access 5 -> Access Address Write (PATTERN) 
               -> Next Pointer = 0x0000 -> end of Sequence
               -> Write Address @0x0C2C / 4 byte / for CS2
               -> Write data = 0x00 -> will be replaced by 32bit Access Address by the Radio Controller */
  RF_ICTRX_EM_BLE_WR(rxonptr+0x1E, 0x0000);
  switch(icy_version)
    {
    case(0x31):   // CS2
    case(0x32):   // CS2
      RF_ICTRX_EM_BLE_WR(rxonptr+0x20, 0x0C84);
      RF_ICTRX_EM_BLE_WR(rxonptr+0x22, 0x002C);
      RF_ICTRX_EM_BLE_WR(rxonptr+0x24, 0x0000);
      RF_ICTRX_EM_BLE_WR(rxonptr+0x26, 0x0000);
    break;
    default:
    break;
    }

  /* *************************************************************************************** */
  /* Initialize HW SPI Rx Off Chained list -> 1 structure of 8 bytes */
  /* *************************************************************************************** */
  /*    RxOFF Access 1  -> Rx Disable (FSM Mode)
                -> Next Pointer = 0x0000 -> end of Sequence
                -> Write Address @0x0CC0 / 1 byte
                -> Write data = 0x08 */

  RF_ICTRX_EM_BLE_WR(rxoffptr,     0x0000);
  switch(icy_version)
    {
    case(0x31):   // CS2
    case(0x32):   // CS3
      RF_ICTRX_EM_BLE_WR(rxoffptr+0x2, 0x0C81);
      RF_ICTRX_EM_BLE_WR(rxoffptr+0x4, 0x08C0);
      RF_ICTRX_EM_BLE_WR(rxoffptr+0x6, 0x0000);
    break;
    default:
    break;
    }

  /* *************************************************************************************** */
  /* Initialize HW SPI RSSI Chained list  -> 1 structure of 8 bytes*/
  /* *************************************************************************************** */
  /*    RSSI Access   -> RSSI read
               -> Next Pointer = 0x0000 -> end of the sequence
               -> Write Address @0x0CCA / 1 byte / Valid for all RF version at the moment
               -> Write data = 0x000000 -> Read data to replace this / provided to Packet Controller */
  RF_ICTRX_EM_BLE_WR(rssiptr,     0x0000);
  switch(icy_version)
    {
    case(0x31):   // CS2
    case(0x32):   // CS3
      RF_ICTRX_EM_BLE_WR(rssiptr+0x2, 0x0C01);
      RF_ICTRX_EM_BLE_WR(rssiptr+0x4, 0x00CA);
      RF_ICTRX_EM_BLE_WR(rssiptr+0x6, 0x0000);
    break;
    default:
    break;
    }

};
///@} RF_BLIM
