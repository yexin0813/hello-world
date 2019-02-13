/*
 * internal_log.h
 *
 *  Created on: 2018Äê7ÔÂ9ÈÕ
 *      Author: jiachuang
 */

#ifndef PLF_BX_DEBUG_INTERNAL_LOG_INTERNAL_LOG_H_
#define PLF_BX_DEBUG_INTERNAL_LOG_INTERNAL_LOG_H_

#include "log.h"
#include "string.h"


#if(USE_INTERNAL_LOG)
//declare
void internal_log_write(uint8_t *buf , uint32_t length);
void internal_log_print(int8_t level, const char * format, ...);

#endif


#endif /* PLF_BX_DEBUG_INTERNAL_LOG_INTERNAL_LOG_H_ */
