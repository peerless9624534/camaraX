#ifndef IDIP_LOG_BACKEND_SYSLOG_H
#define IDIP_LOG_BACKEND_SYSLOG_H

#include <idip/idip_log.h>

#if defined(IVESDK_WITH_LOG_SYSLOG)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize log backend structure, open syslog subsystem and set log identifier.
 * The string pointed to by @a logname is prepended to every message, and is
 * typically set to the program name. If logname is NULL, the program name is used.
 *
 * @param backend - pointer to a @ref idip_log_backend_t instance to initialize.
 * Can't be NULL;
 * @param max_priority - a maximal priority of log messages.
 * @param logname - a string that will be prefixed to every log message.
 *
 * @note The function passes dynamic copy of @a logname to the openlog(3) and stores
 * the pointer to backend.udata field. Then no side effects with the prefix will occur.
 * If you pass the initialized packend to the @ref idip_log_set_backend() then
 * you don't have to call free(), the log subsystem will care about the action.
 */
void idip_log_backend_syslog_init(idip_log_backend_t* backend,
                                  int max_priority,
                                  const char *logname);

#ifdef __cplusplus
}
#endif

#endif // IVESDK_WITH_LOG_SYSLOG

#endif // IDIP_LOG_BACKEND_SYSLOG_H
