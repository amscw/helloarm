/*
 * common.hpp
 *
 *  Created on: 1 апр. 2020 г.
 *      Author: moskvin
 */

#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include <string.h>
#include "trace.h"

#ifdef __cplusplus
extern "C" {
#endif

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define TRACE_PRINT(fmt,args...) trace_printf("%s(%s)-" fmt "\r\n", __FILENAME__, __FUNCTION__, ##args)
#define TRACE_CHAR(c) trace_putchar(c)
#define TRACE_BYTE(n) trace_printf("%02x ", n)
#define TRACE_WORD(n) trace_printf("%08x ", n)
#define TRACE_STR(s) trace_puts(s)

#define RUNTIME_ASSERT(expr) {\
	if (expr) {\
		/*TRACE_PRINT("ok");*/\
	} else {\
		TRACE_PRINT("fail");\
		return false;\
	}\
}

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_COMMON_H_ */
