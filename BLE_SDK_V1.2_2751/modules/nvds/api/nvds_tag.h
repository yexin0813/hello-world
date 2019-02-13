#ifndef NVDS_TAG_H_
#define NVDS_TAG_H_
#include "nvds_typedef.h"
#define NVDS_DATA_SECTION_MAX_SIZE 0x1000
#if (NVDS_DATA_SECTION_MAX_SIZE % 0x1000)
#error NVDS_DATA_SECTION_MAX_SIZE must be multiple of 4KB
#endif
#define NVDS_MAGIC_FLAG_SIZE 4
#define TAG_HEAD    uint8_t struct_size_r;    \
                    uint8_t name;           \
                    tag_label_t label
#define TAG_HEAD_INIT(head,name,data_length)                                                               \
                        do{                                                                                                                                 \
                            (head)->struct_size_r = ~((data_length) + sizeof(nvds_tag_head_t));            \
                            (head)->name = (name);                                                                                      \
                            *(uint8_t *)&((head)->label) = 0xff;                                                                \
                        }while(0)
#define DECL_TAG(tag,array_type,total_length)   \
        struct{                                 \
            TAG_HEAD;                           \
            array_type data[(total_length)/sizeof(array_type)]  __attribute__((packed));      \
        } tag##_env __attribute__((packed))
#define DECL_END() uint8_t tail
#define NVDS_TAG_STRUCT_SIZE(tag) (~(tag)->struct_size_r)
typedef struct{
    uint8_t valid :1;
    uint8_t reserved: 7;
}tag_label_t;
typedef struct{
    TAG_HEAD;
}nvds_tag_head_t;

enum VALID_BIT
{
    INVALID = 0,
    VALID 
};

enum NVDS_USR_TAG {
    NEW_TAG_1 = NVDS_TAG_APP_SPECIFIC_FIRST,
    NEW_TAG_2,
    NEW_TAG_LAST = NVDS_TAG_APP_SPECIFIC_LAST,
};

typedef struct{
    uint8_t magic_flag[NVDS_MAGIC_FLAG_SIZE];
    uint32_t ver;
}nvds_data_head_t;
typedef struct {
    __attribute__((packed)) nvds_data_head_t head;
    DECL_TAG(NVDS_TAG_DEVICE_NAME,uint8_t,NVDS_LEN_DEVICE_NAME);
    DECL_TAG(NVDS_TAG_BD_ADDRESS,uint8_t,NVDS_LEN_BD_ADDRESS);
    DECL_TAG(NVDS_TAG_EXT_WAKEUP_TIME,uint16_t,NVDS_LEN_EXT_WAKEUP_TIME);
    DECL_TAG(NVDS_TAG_OSC_WAKEUP_TIME,uint16_t,NVDS_LEN_OSC_WAKEUP_TIME);
    DECL_TAG(NVDS_TAG_RM_WAKEUP_TIME,uint16_t,NVDS_LEN_RM_WAKEUP_TIME);
    DECL_TAG(NVDS_TAG_SLEEP_ENABLE,uint8_t,NVDS_LEN_SLEEP_ENABLE);
    DECL_TAG(NVDS_TAG_EXT_WAKEUP_ENABLE,uint8_t,NVDS_LEN_EXT_WAKEUP_ENABLE);
    //DECL_TAG(NVDS_TAG_DIAG_BLE_HW,uint8_t,NVDS_LEN_DIAG_BLE_HW);
    
    DECL_TAG(NEW_TAG_1,uint32_t,4);
    DECL_END();
}nvds_data_t;


extern const nvds_data_t nvds_data;
#endif
