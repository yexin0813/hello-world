#ifndef __REG_RTC_H__
#define __REG_RTC_H__
#include <stdint.h>

typedef struct
{
    volatile uint32_t RTC_CCVR;
    volatile uint32_t RTC_CMR;
    volatile uint32_t RTC_CLR;
    volatile uint32_t RTC_CCR;
    volatile uint32_t RTC_STAT;
    volatile uint32_t RTC_RSTAT;
    volatile uint32_t RTC_EOI;
    volatile uint32_t RTC_COMP_VERSION;
}reg_rtc_t;

enum RTC_RTC_CCVR_FIELD
{
    RTC_CURRENT_COUNTER_VALUE_MASK = (int)0xffffffffL,
    RTC_CURRENT_COUNTER_VALUE_POS = 0,
};

enum RTC_RTC_CMR_FIELD
{
    RTC_COUNTER_MATCH_MASK = (int)0xffffffffL,
    RTC_COUNTER_MATCH_POS = 0,
};

enum RTC_RTC_CLR_FIELD
{
    RTC_COUNTER_LOAD_MASK = (int)0xffffffffL,
    RTC_COUNTER_LOAD_POS = 0,
};

enum RTC_RTC_CCR_FIELD
{
    RTC_RTC_WEN_MASK = 0x8,
    RTC_RTC_WEN_POS = 3,
    RTC_RTC_EN_MASK = 0x4,
    RTC_RTC_EN_POS = 2,
    RTC_RTC_MASK_MASK = 0x2,
    RTC_RTC_MASK_POS = 1,
    RTC_RTC_IEN_MASK = 0x1,
    RTC_RTC_IEN_POS = 0,
};

enum RTC_RTC_STAT_FIELD
{
    RTC_RTC_STAT_MASK = 0x1,
    RTC_RTC_STAT_POS = 0,
};

enum RTC_RTC_RSTAT_FIELD
{
    RTC_RTC_RSTAT_MASK = 0x1,
    RTC_RTC_RSTAT_POS = 0,
};

enum RTC_RTC_EOI_FIELD
{
    RTC_RTC_EOI_MASK = 0x1,
    RTC_RTC_EOI_POS = 0,
};

enum RTC_RTC_COMP_VERSION_FIELD
{
    RTC_RTC_COMP_VERSION_MASK = (int) 0xffffffffL,
    RTC_RTC_COMP_VERSION_POS = 0,
};

#endif
