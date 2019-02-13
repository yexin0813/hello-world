#ifndef RTC_H_
#define RTC_H_
#include "rtc_integration.h"
#include "reg_rtc.h"
#include "_reg_base_addr.h"
#include "field_manipulate.h"
#define RTC_REG ((reg_rtc_t *)REG_RTC_BASE)

#define RTC_MATCHCNT_SET(val) RTC_REG->RTC_CMR = (val)
#define RTC_LOADCNT_SET(val) RTC_REG->RTC_CLR = (val)
#define RTC_EN_WITH_INT() RTC_REG->RTC_CCR = FIELD_BUILD(RTC_RTC_EN,1) | FIELD_BUILD(RTC_RTC_IEN,1)
#define RTC_EN_WITHOUT_INT() RTC_REG->RTC_CCR = FIELD_BUILD(RTC_RTC_EN,1)
#define RTC_CURRENTCNT_GET() RTC_REG->RTC_CCVR
#define RTC_INT_CLR() RTC_REG->RTC_EOI
#define RTC_INT_STAT_GET() RTC_REG->RTC_STAT
#endif
