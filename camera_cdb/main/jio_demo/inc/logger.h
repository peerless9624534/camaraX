#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <idip/idip_log.h>

#undef LOG_NOISE
#undef LOG_INFO
#undef LOG_DEBUG
#undef LOG_WARN
#undef LOG_ERROR
#undef LOG_FATAL


#ifdef __cplusplus
extern "C" {
#endif


#if defined(RELEASE_TARGET)
#define LOG_DEBUG(fmt, ...)   ((void)0)
#else

#define LOG_DEBUG(fmt, ...)    idip_log_xdebug( __func__, fmt , ##__VA_ARGS__ )
#endif

#define LOG_INFO(fmt, ...)     idip_log_xinfo( __func__, fmt , ##__VA_ARGS__ )
#define LOG_NOTICE(fmt, ...)   idip_log_xnotice( __func__, fmt , ##__VA_ARGS__ )
#define LOG_WARN(fmt, ...)     idip_log_xwarning( __func__, fmt , ##__VA_ARGS__ )
#define LOG_ERROR(fmt, ...)    idip_log_xerror( __func__, fmt , ##__VA_ARGS__ )
#define LOG_FATAL(fmt, ...)    idip_log_xfatal( __func__, fmt , ##__VA_ARGS__ )


#ifdef __cplusplus
}
#endif

#endif // __LOGGER_H__

