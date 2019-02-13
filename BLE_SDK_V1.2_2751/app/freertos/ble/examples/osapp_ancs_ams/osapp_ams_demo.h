/*
 * osapp_ams_demo.h
 *
 *  Created on: 2018Äê11ÔÂ1ÈÕ
 *      Author: mingzhou
 */

#ifndef OSAPP_AMS_DEMO_H_
#define OSAPP_AMS_DEMO_H_
#include <stdint.h>

#define AMSC_SERVICE_UUID_128 {0xDC, 0xF8, 0x55, 0xAD, 0x02, 0xC5, \
    0xF4, 0x8E, 0x3A, 0x43, 0x36, 0x0F, 0x2B, 0x50, 0xD3, 0x89}

#define AMSC_REMOTE_COMMAND_UUID_128 {0xC2, 0x51, 0xCA, 0xF7, 0x56, 0x0E, \
    0xDF, 0xB8, 0x8A, 0x4A, 0xB1, 0x57, 0xD8, 0x81, 0x3C, 0x9B}

#define AMSC_ENTITY_UPDATE_UUID_128 {0x02, 0xC1, 0x96, 0xBA, 0x92, 0xBB, \
    0x0C, 0x9A, 0x1F, 0x41, 0x8D, 0x80, 0xCE, 0xAB, 0x7C, 0x2F}

#define AMSC_ENTITY_ATTRIBUTE_UUID_128 {0xD7, 0xD5, 0xBB, 0x70, 0xA8, 0xA3, \
    0xAB, 0xA6, 0xD8, 0x46, 0xAB, 0x23, 0x8C, 0xF3, 0xB2, 0xC6}

enum
{
    AMS_REMOTE_CMD_ID_PLAY = 0,
    AMS_REMOTE_CMD_ID_PAUSE,
    AMS_REMOTE_CMD_ID_TOOGLE_PLAY_PAUSE,
    AMS_REMOTE_CMD_ID_NEXT_TRACK,
    AMS_REMOTE_CMD_ID_PREVIOUS_TRACK,
    AMS_REMOTE_CMD_ID_VOLUME_UP, // 5
    AMS_REMOTE_CMD_ID_VOLUME_DOWN,
    AMS_REMOTE_CMD_ID_ADVANCE_REPEAT_MODE,
    AMS_REMOTE_CMD_ID_ADVANCE_SHUFFLE_MODE,
    AMS_REMOTE_CMD_ID_SKIPFORWARD,
    AMS_REMOTE_CMD_ID_SKIPBACKWARD, // 10
    AMS_REMOTE_CMD_ID_LIKE_TRACK,
    AMS_REMOTE_CMD_ID_DISLIKE_TRACK,
    AMS_REMOTE_CMD_ID_BOOKMARK_TRACK,
    AMS_REMOTE_CMD_ID_MAX, //14
};

enum
{
    AMS_ENTITY_ID_PLAYER = 0,
    AMS_ENTITY_ID_QUEUE,
    AMS_ENTITY_ID_TRACK,
    AMS_ENTITY_ID_MAX, // 3
};

enum
{
    AMS_ENTITY_UPDATE_FLAG_TRUNCATED = 1 << 0,
};

enum
{
    AMS_PLAYER_ATTRIBUTE_ID_NAME = 0,
    AMS_PLAYER_ATTRIBUTE_ID_PLAYBACKINFO,
    AMS_PLAYER_ATTRIBUTE_ID_VOLUME,
    AMS_PLAYER_ATTRIBUTE_ID_MAX, // 3
};

enum
{
    AMS_QUEUE_ATTRIBUTE_ID_INDEX = 0,
    AMS_QUEUE_ATTRIBUTE_ID_COUNT,
    AMS_QUEUE_ATTRIBUTE_ID_SHUFFLE_MODE,
    AMS_QUEUE_ATTRIBUTE_ID_REPEAT_MODE,
    AMS_QUEUE_ATTRIBUTE_ID_MAX, // 4
};

enum
{
    AMS_SHUFFLE_MODE_OFF = 0,
    AMS_SHUFFLE_MODE_ONE,
    AMS_SHUFFLE_MODE_ALL,
    AMS_SHUFFLE_MODE_MAX, // 3
};

enum
{
    AMS_REPEAT_MODE_OFF = 0,
    AMS_REPEAT_MODE_ONE,
    AMS_REPEAT_MODE_ALL,
    AMS_REPEAT_MODE_MAX, // 3
};

enum
{
    AMS_TRACK_ATTRIBUTE_ID_ARTIST = 0,
    AMS_TRACK_ATTRIBUTE_ID_ALBUM,
    AMS_TRACK_ATTRIBUTE_ID_TITLE,
    AMS_TRACK_ATTRIBUTE_ID_DURATION,
    AMS_TRACK_ATTRIBUTE_ID_MAX, //4
};

enum
{
    AMS_ERROR_INVALID_STATE = 0xA0,
    AMS_ERROR_INVALID_COMMAND,
    AMS_ABSENT_ATTRIBUTE, // 0xA2
};

enum amsc_chars
{
    AMSC_CHAR_REMOTE_CMD = 0,
    AMSC_CHAR_ENTITY_UPDATE,
    AMSC_CHAR_ENTITY_ATT,
    AMSC_CHAR_MAX,
};

enum amsc_descs
{
    AMSC_DESC_REMOTE_CMD_CFG = 0,
    AMSC_DESC_ENTITY_UPDATE_CFG,
    AMSC_DESC_MAX,
    AMSC_DESC_MASK = 0x10,
};

enum amsc_state
{
    AMSC_STATE_FREE = 0,
    AMSC_STATE_IDLE,
    AMSC_STATE_ENCRYPT,
    AMSC_STATE_DISCOVERING,
    AMSC_STATE_NOTIF_INIT,
    AMSC_STATE_BUSY,

    AMSC_STATUS_MAX
};

enum amsc_op
{
    AMSC_OP_NULL = 0,
    AMSC_OP_ENCRYPT, // GPA encryption
    AMSC_OP_SRV_DISC, // AMSC service discovering
    AMSC_OP_EN_RMT_CMD_NTF, // Enable remote command notification
    AMSC_OP_EN_ET_UDT_NTF, // Enable entity update notification
    AMSC_OP_WR_RMT_CMD, // Write remote command(execute command)
    AMSC_OP_WR_ET_UDT, // Write entity update(enable entity/attribute pair notification)
    AMSC_OP_RD_ET_ATT, // Read entity attribute
    AMSC_OP_WR_ET_ATT, // Write entity attribute

    AMSC_OP_MAX,
};

struct amsc_entity_notf
{
    uint8_t entityID;
    uint8_t attID;
    uint8_t entityUpdateFlags;
    uint8_t value[__ARRAY_EMPTY];
};

/**
 * @brief struct to store information related to link key
 */
struct store_gapc_ltk
{
    /// Long Term Key
    struct gap_sec_key ltk;
    /// Encryption Diversifier
    uint16_t ediv;
    /// Random Number
    rand_nb_t randnb;
    /// Encryption key size (7 to 16)
    uint8_t key_size;
};

/**
 * @brief struct to store bond information
 */
struct bond_info_t
{
    /// valid or not
    uint8_t valid;
    /// store information related to link key
    struct store_gapc_ltk ltk_store;
    /// store csrk, which will use in encryption later
    struct gap_sec_key csrk;
    /// store auth, which will use in encryption later
    struct gapc_bond_auth auth;
    /// store the master address, should compare with conn_info_t.device_addr when bonding
    bd_addr_t device_addr;
};

struct amsc_content
{
    uint8_t operation;
    uint8_t amsc_state;
    struct prf_svc svc;
    struct prf_char_inf chars[AMSC_CHAR_MAX];
    struct prf_char_desc_inf descs[AMSC_DESC_MAX];

};

#endif /* OSAPP_AMS_DEMO_H_ */
