#ifndef IDIP_LOG_P_H
#define IDIP_LOG_P_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
    #define __is_printf_like_3_4 __attribute__ ((format (printf, 3, 4)))
    #define __is_printf_like_3_0 __attribute__ ((format (printf, 3, 0)))
#else
    #define __is_printf_like_3_4
    #define __is_printf_like_3_0
#endif

void idip_log(int level, const char* scope, const char* fmt, va_list ap) __is_printf_like_3_0;
void idip_log_generic(int level, const char* scope, const char* fmt, ...) __is_printf_like_3_4;

#ifdef __cplusplus
}
#endif

#endif // IDIP_LOG_P_H
