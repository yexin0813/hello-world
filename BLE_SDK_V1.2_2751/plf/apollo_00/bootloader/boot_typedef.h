#ifndef BOOT_TYPEDEF_H_
#define BOOT_TYPEDEF_H_
#include <stdint.h>
#define FLAG_CONST_SIZE 4
#define BX_FLAG_CONST {0x42,0x58,0x32,0x34}
#define BX_UART_BOOT_ACK "BX_UART_BOOT"
typedef void (*entry_point_t)();
typedef struct{
    uint8_t bx_flag[FLAG_CONST_SIZE];
    uint8_t *base_addr;
    uint32_t length;
    entry_point_t entry_point;
}boot_header_t;
typedef enum{
    BOOT_FROM_SPI_FLASH = 0,
    BOOT_FROM_UART
}boot_option_t;
typedef enum{
    VOLTAGE_1V8 = 0,
    VOLTAGE_3V
}pad_voltage_t;
#endif
