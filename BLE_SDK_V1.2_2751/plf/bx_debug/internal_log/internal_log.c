/*
 * internal_log.c
 *
 *  Created on: 2018Äê7ÔÂ9ÈÕ
 *      Author: jiachuang
 */
#include "internal_log.h"
#if(USE_INTERNAL_LOG)

typedef struct
{
    char    *current_pointer;   //sprintf buffer pointer
    uint32_t current_offset;    //offset in internal_log_buf
    uint32_t log_cnt;           //log counter
    uint32_t log_line_cnt;      //new line log counter
}ram_log_t;


char internal_log_buf[INTERNAL_LOG_DEEPTH + INTERNAL_LOG_MAX];

ram_log_t ram_log=
{
        .current_pointer = internal_log_buf,
        .current_offset  = 0,
        .log_cnt = 0,
        .log_line_cnt = 0,
};


//increase offset and counter
static void internal_log_increase(uint32_t current_length)
{
    //increase offset
    ram_log.current_offset += current_length;
    if(ram_log.current_offset > INTERNAL_LOG_DEEPTH)
    {
        ram_log.current_offset = 0;
    }

    //current pointer value
    ram_log.current_pointer = internal_log_buf + ram_log.current_offset;

    //increase counter
    ram_log.log_cnt ++;
    if(internal_log_buf[ram_log.current_offset - 1] == '\n')
    {
        ram_log.log_line_cnt ++;
    }
}


void internal_log_write(uint8_t *buf , uint32_t length)
{
    //copy to buffer
    memcpy(ram_log.current_pointer , buf , length);
    //increase offset and counter
    internal_log_increase(length);
}


void internal_log_print(int8_t level, const char * format, ...)
{
    uint32_t current_length = 0;

    //print
    va_list args;
    va_start(args,format);
    current_length = vsprintf(ram_log.current_pointer,format,args);
    va_end(args);

    //increase offset and counter
    internal_log_increase(current_length);
}


#endif


