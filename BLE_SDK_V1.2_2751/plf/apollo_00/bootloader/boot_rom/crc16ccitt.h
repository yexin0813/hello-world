#ifndef CRC16CCITT_H_
#define CRC16CCITT_H_
#include <stdint.h>
#define CRC_FIELD_LENGTH sizeof(uint16_t)
uint16_t crc16ccitt(uint16_t crc,const void *in_data,uint32_t len);

#endif

