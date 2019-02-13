#ifndef BOOT_RAM_H_
#define BOOT_RAM_H_
#include <stdint.h>
#include "boot_typedef.h"
#define IMAGE_OFFSET_BASE 0x1000
#define FLASH_IMAGE_HEADER_SIZE 128
#define CRC_BLOCK_SIZE_MAX 0x1000

typedef enum{
    Std_Read_Mode,
    Dual_Output_Read_Mode,
    Quad_Output_Read_Mode,
    Dual_IO_Read_Mode,
    Quad_IO_Read_Mode,
    Read_Mode_Max,
}flash_op_mode_t;

typedef struct{
    uint32_t valid :1;
    uint32_t obsolete :1;
    uint32_t flash_op_mode:3;
    uint32_t reserved: 27;
}image_feature_t;
typedef struct{
        uint16_t crc[40];
        image_feature_t feature;
        uint32_t reserved;
        uint32_t image_ram_size;
        uint32_t image_total_size;
}image_header_t;
typedef union{
    uint8_t a[FLASH_IMAGE_HEADER_SIZE];
    image_header_t s;
}flash_image_header_t;
typedef struct{
    boot_header_t header;
    volatile uint32_t flash_total_size;
    volatile uint32_t nvds_base;
    volatile uint32_t image_limit;
}boot_ram_header_t;

#endif
