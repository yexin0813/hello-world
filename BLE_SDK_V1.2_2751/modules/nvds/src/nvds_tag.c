#include "bx_config.h"
#include "nvds_tag.h"
#include "compiler_flag.h"
#define NVDS_DATA_SECTION __attribute__((section("nvds_area")))
#define TAG_INIT()                             \
    .head =                                                     \
        {                                                             \
            .magic_flag = {'N','V','D','S'},     \
            .ver = 1,                                          \
        },                                                          \
    .tail = 0xff
#define DEFINE_TAG(tag,...)           \
    .tag##_env =                              \
    {                                           \
        .label = {                              \
            .valid = 1,                 \
            .reserved = 0x7f,   \
        },                                      \
        .struct_size_r = (uint8_t)(~sizeof(nvds_data.tag##_env)),\
        .name = tag,                            \
        .data = __VA_ARGS__,                        \
    }

const nvds_data_t nvds_data NVDS_DATA_SECTION USED_FLAG = {
    TAG_INIT(),
    DEFINE_TAG(NVDS_TAG_DEVICE_NAME,BX_DEV_NAME),
    DEFINE_TAG(NVDS_TAG_BD_ADDRESS,BX_DEV_ADDR),
    DEFINE_TAG(NVDS_TAG_EXT_WAKEUP_TIME,BLE_WAKEUP_TIME),
    DEFINE_TAG(NVDS_TAG_OSC_WAKEUP_TIME,BLE_WAKEUP_TIME),
    DEFINE_TAG(NVDS_TAG_RM_WAKEUP_TIME,BLE_WAKEUP_TIME),
    DEFINE_TAG(NVDS_TAG_SLEEP_ENABLE,DEEP_SLEEP_ENABLE),
    DEFINE_TAG(NVDS_TAG_EXT_WAKEUP_ENABLE,EXT_WAKE_UP_ENABLE),
    //DEFINE_TAG(NVDS_TAG_DIAG_BLE_HW,{}},
    
    DEFINE_TAG(NEW_TAG_1,{0}),
    
};
    
