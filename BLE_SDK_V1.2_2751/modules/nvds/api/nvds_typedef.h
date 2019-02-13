#ifndef NVDS_TYPEDEF_H_
#define NVDS_TYPEDEF_H_
#include <stdbool.h>   
#include <stdint.h>

/*
 * DEFINES
 ****************************************************************************************
 */

/// NVDS is defined as read-write
#define NVDS_READ_WRITE          1

/// NVDS is defined as packed
#define NVDS_PACKED              0  //default value 1

/// NVDS has 8-bit length tags
#define NVDS_8BIT_TAGLENGTH      1

/// Type of the tag length (8 or 16 bits)
#if (NVDS_8BIT_TAGLENGTH)
typedef uint8_t  nvds_tag_len_t;
#else
typedef uint16_t nvds_tag_len_t;
#endif // NVDS_8BIT_TAGLENGTH


/*
 * ENUMERATION DEFINITIONS
 ****************************************************************************************
 */

/// Possible Returned Status
enum NVDS_STATUS
{
    /// NVDS status OK
    NVDS_OK,
    /// generic NVDS status KO
    NVDS_FAIL,
    /// NVDS TAG unrecognized
    NVDS_TAG_NOT_DEFINED,
    /// No space for NVDS
    NVDS_NO_SPACE_AVAILABLE,
    /// Length violation
    NVDS_LENGTH_OUT_OF_RANGE,
    /// NVDS parameter locked
    NVDS_PARAM_LOCKED,
    /// NVDS corrupted
    NVDS_CORRUPT
};

/// List of NVDS TAG identifiers
enum NVDS_TAG
{
    /// Definition of the tag associated to each parameters
    /// Local Bd Address
    NVDS_TAG_BD_ADDRESS                 = 0x01,
    /// Device Name
    NVDS_TAG_DEVICE_NAME                = 0x02,
    /// Radio Drift
    NVDS_TAG_LPCLK_DRIFT                = 0x07,
    /// Radio Jitter
    NVDS_TAG_LPCLK_JITTER               = 0x08,
    /// Radio Class
    NVDS_TAG_RADIO_CLASS                = 0x09,
    /// Bluejay specific Settings
    NVDS_TAG_BJ_TXCNTL1                 = 0x0A,
    /// External wake-up time
    NVDS_TAG_EXT_WAKEUP_TIME            = 0x0D,
    /// Oscillator wake-up time
    NVDS_TAG_OSC_WAKEUP_TIME            = 0x0E,
    /// Radio wake-up time
    NVDS_TAG_RM_WAKEUP_TIME             = 0x0F,
    /// UART baudrate
    NVDS_TAG_UART_BAUDRATE              = 0x10,
    /// Enable sleep mode
    NVDS_TAG_SLEEP_ENABLE               = 0x11,
    /// Enable External Wakeup
    NVDS_TAG_EXT_WAKEUP_ENABLE          = 0x12,
    /// SP Private Key 192
    NVDS_TAG_SP_PRIVATE_KEY_P192        = 0x13,
    /// SP Public Key 192
    NVDS_TAG_SP_PUBLIC_KEY_P192         = 0x14,
    /// Errata adopted check
    NVDS_TAG_ERRATA_ADOPTED             = 0x15,
    /// CQDDR Tags
    NVDS_TAG_BASIC_THRESHOLD            = 0x16,
    NVDS_TAG_EDR_THRESHOLD              = 0x17,
    NVDS_TAG_BASIC_ALGORITHM            = 0x18,
    NVDS_TAG_EDR_ALGORITHM              = 0x19,
    NVDS_TAG_BASIC_PACKET_LUT           = 0x2A,
    NVDS_TAG_EDR_PACKET_LUT             = 0x2B,
    /// Synchronous links configuration
    NVDS_TAG_SYNC_CONFIG                = 0x2C,
    /// PCM Settings
    NVDS_TAG_PCM_SETTINGS               = 0x2D,
    /// Sleep algorithm duration
    NVDS_TAG_SLEEP_ALGO_DUR             = 0x2E,

    /// Diagport configuration
    NVDS_TAG_DIAG_BT_HW                 = 0x30,
    NVDS_TAG_DIAG_BLE_HW                = 0x31,
    NVDS_TAG_DIAG_SW                    = 0x32,
    NVDS_TAG_DIAG_PLF                   = 0x34,

    /// RSSI threshold tags
	NVDS_TAG_RSSI_HIGH_THR              = 0x3A,
	NVDS_TAG_RSSI_LOW_THR               = 0x3B,
	NVDS_TAG_RSSI_INTERF_THR            = 0x3C,

    /// BLE Channel Assessment tags
    NVDS_TAG_BLE_CA_TIMER_DUR           = 0x40,
    NVDS_TAG_BLE_CRA_TIMER_CNT          = 0x41,
    NVDS_TAG_BLE_CA_MIN_THR             = 0x42,
    NVDS_TAG_BLE_CA_MAX_THR             = 0x43,
    NVDS_TAG_BLE_CA_NOISE_THR           = 0x44,

    /// AFH algorithm tags
    NVDS_TAG_AFH_REASS_NBCH             = 0x51,
    NVDS_TAG_AFH_WINLGTH                = 0x52,
    NVDS_TAG_AFH_RSSIMIN                = 0x53,
    NVDS_TAG_AFH_PERTHRESBAD            = 0x54,
    NVDS_TAG_AFH_REASS_INT              = 0x55,
    NVDS_TAG_AFH_NMIN                   = 0x56,
    NVDS_TAG_AFH_MAXADAPT               = 0x57,
    NVDS_TAG_AFH_THSMIN                 = 0x58,


    NVDS_TAG_BT_LINK_KEY_FIRST          = 0x60,
    NVDS_TAG_BT_LINK_KEY_LAST           = 0x67,

    NVDS_TAG_BLE_LINK_KEY_FIRST         = 0x70,
    NVDS_TAG_BLE_LINK_KEY_LAST          = 0x7F,
    /// SP Private Key (Low Energy)
    NVDS_TAG_LE_PRIVATE_KEY_P256        = 0x80,
    /// SP Public Key (Low Energy)
    NVDS_TAG_LE_PUBLIC_KEY_P256         = 0x81,
    /// SP Debug: Used Fixed Private Key from NVDS (Low Energy)
    NVDS_TAG_LE_DBG_FIXED_P256_KEY_EN   = 0x82,
    /// SP Private Key (classic BT)
    NVDS_TAG_SP_PRIVATE_KEY_P256        = 0x83,
    /// SP Public Key (classic BT)
    NVDS_TAG_SP_PUBLIC_KEY_P256         = 0x84,

    /// Application specific
    NVDS_TAG_APP_SPECIFIC_FIRST         = 0x90,
    NVDS_TAG_APP_SPECIFIC_LAST          = 0xAF,
};

/// List of NVDS Tag lengths
enum NVDS_LEN
{
     // Definition of length associated to each parameters
     /// Local Bd Address
     NVDS_LEN_BD_ADDRESS                 = 6,
     /// Device Name
     NVDS_LEN_DEVICE_NAME                = 248,
     /// Low power clock drift
     NVDS_LEN_LPCLK_DRIFT                = 2,
     /// Low power clock jitter
     NVDS_LEN_LPCLK_JITTER               = 1,
     /// Radio Class
     NVDS_LEN_RADIO_CLASS                = 1,
     /// Bluejay specific Settings
     NVDS_LEN_BJ_TXCNTL1                 = 4,


     /// External wake-up time
     NVDS_LEN_EXT_WAKEUP_TIME            = 2,
     /// Oscillator wake-up time
     NVDS_LEN_OSC_WAKEUP_TIME            = 2,
     /// Radio wake-up time
     NVDS_LEN_RM_WAKEUP_TIME             = 2,
     /// UART baudrate
     NVDS_LEN_UART_BAUDRATE              = 4,
     /// Enable sleep mode
     NVDS_LEN_SLEEP_ENABLE               = 1,
     /// Enable External Wakeup
     NVDS_LEN_EXT_WAKEUP_ENABLE          = 1,
     /// SP Private Key 192
     NVDS_LEN_SP_PRIVATE_KEY_P192        = 24,
     /// SP Public Key 192
     NVDS_LEN_SP_PUBLIC_KEY_P192         = 48,
     /// Errata adopted check
     NVDS_LEN_ERRATA_ADOPTED             = 1,
     /// CQDDR Tags
     NVDS_LEN_BASIC_THRESHOLD            = 70,
     NVDS_LEN_EDR_THRESHOLD              = 70,
     NVDS_LEN_BASIC_ALGORITHM            = 21,
     NVDS_LEN_EDR_ALGORITHM              = 21,
     NVDS_LEN_BASIC_PACKET_LUT           = 16,
     NVDS_LEN_EDR_PACKET_LUT             = 16,
     /// Synchronous links configuration
     NVDS_LEN_SYNC_CONFIG                = 2,
     /// PCM Settings
     NVDS_LEN_PCM_SETTINGS               = 8,
     /// Sleep algorithm duration
     NVDS_LEN_SLEEP_ALGO_DUR             = 2,
     /// Diagport configuration
     NVDS_LEN_DIAG_BT_HW                 = 4,
     NVDS_LEN_DIAG_BLE_HW                = 4,
     NVDS_LEN_DIAG_SW                    = 4,
     NVDS_LEN_DIAG_PLF                   = 4,
	 /// RSSI thresholds
	 NVDS_LEN_RSSI_THR                   = 1,


     NVDS_LEN_BLE_CA_TIMER_DUR           = 2,
     NVDS_LEN_BLE_CRA_TIMER_CNT          = 1,
     NVDS_LEN_BLE_CA_MIN_THR             = 1,
     NVDS_LEN_BLE_CA_MAX_THR             = 1,
     NVDS_LEN_BLE_CA_NOISE_THR           = 1,

     /// AFH algorithm tags
     NVDS_LEN_AFH_REASS_NBCH             = 1,
     NVDS_LEN_AFH_WINLGTH                = 1,
     NVDS_LEN_AFH_RSSIMIN                = 1,
     NVDS_LEN_AFH_PERTHRESBAD            = 1,
     NVDS_LEN_AFH_REASS_INT              = 1,
     NVDS_LEN_AFH_NMIN                   = 1,
     NVDS_LEN_AFH_MAXADAPT               = 1,
     NVDS_LEN_AFH_THSMIN                 = 1,
     /// Link keys
     NVDS_LEN_BT_LINK_KEY                = 22,
     NVDS_LEN_BLE_LINK_KEY               = 48,

     /// P256
     NVDS_LEN_LE_PRIVATE_KEY_P256        = 32,
     NVDS_LEN_LE_PUBLIC_KEY_P256         = 64,
     NVDS_LEN_LE_DBG_FIXED_P256_KEY_EN   = 1,
     NVDS_LEN_SP_PRIVATE_KEY_P256        = 32,
     NVDS_LEN_SP_PUBLIC_KEY_P256         = 64,
};

#endif
