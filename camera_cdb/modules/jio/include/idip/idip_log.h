#ifndef IDIP_LOG_H
#define IDIP_LOG_H

#include <inttypes.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

struct msgpack_object;
struct transport_message;

enum {
    IDIP_LOG_FATAL = 0,
    IDIP_LOG_ERROR,
    IDIP_LOG_WARNING,
    IDIP_LOG_INFO,
    IDIP_LOG_DEBUG,
    IDIP_LOG_TRACE,
};

#ifdef __GNUC__
    #define __is_printf_like __attribute__ ((format (printf, 1, 2)))
    #define __is_printf_like_2 __attribute__ ((format (printf, 2, 3)))
#else
    #define __is_printf_like
    #define __is_printf_like_2
#endif

/// Log record metadata
typedef struct idip_log_metadata {
    struct timeval timestamp;   ///< record creation time
    int priority;               ///< log priority
    char* scope;                ///< or prefix for a log string
} idip_log_metadata_t;

/// log formatter function type
typedef size_t (*log_backend_fmt_t)(void* udata,
                                    char* buf, size_t buf_size,
                                    const idip_log_metadata_t* metadata,
                                    const char* fmt, va_list ap);
/// log write function type
typedef void (*log_backend_put_t)(void* udata,
                                  const idip_log_metadata_t* metadata,
                                  const char* renderred_msg);
/// log close function type
typedef void  (*log_backend_close_t)(void* udata);


typedef bool (*log_backend_allow_colors_t)(void* udata);

typedef struct idip_log_backend {
    log_backend_fmt_t                   fmt;
    log_backend_put_t                   put;
    log_backend_close_t                 close;
    log_backend_allow_colors_t          test_colors_fn;
    int                                 max_priority;
    void*                               udata;
}
idip_log_backend_t;

void idip_log_set_backend(idip_log_backend_t* backend);

void idip_log_fatal(const char* fmt, ...) __is_printf_like;
void idip_log_error(const char* fmt, ...) __is_printf_like;
void idip_log_warning(const char* fmt, ...) __is_printf_like;
void idip_log_info(const char* fmt, ...) __is_printf_like;
void idip_log_debug(const char* fmt, ...) __is_printf_like;
void idip_log_trace(const char* fmt, ...) __is_printf_like;

void idip_log_xfatal(const char* scope, const char* fmt, ...) __is_printf_like_2;
void idip_log_xerror(const char* scope, const char* fmt, ...) __is_printf_like_2;
void idip_log_xwarning(const char* scope, const char* fmt, ...) __is_printf_like_2;
void idip_log_xinfo(const char* scope, const char* fmt, ...) __is_printf_like_2;
void idip_log_xdebug(const char* scope, const char* fmt, ...) __is_printf_like_2;
void idip_log_xtrace(const char* scope, const char* fmt, ...) __is_printf_like_2;

void idip_log_mpk_query(const struct msgpack_object* obj, int fd);
// Log outgoing transport message and v2 data from packed msgpack'ed buffer
void idip_log_mpk_transport_tx(const void* buf, size_t size, int fd);
void idip_log_mpk_transport_parsed(const struct transport_message* header,
                                   const struct msgpack_object system,
                                   size_t packet_size, int fd);

int idip_log_time2str(char* buf, size_t buf_size, struct timeval tv);

void idip_log_fatal_invalid_arguments(const char* func);

bool idip_log_supports_color();

int idip_log_priority_from_string(const char* priority, int failback);

#ifdef __cplusplus
}
#endif

#endif // IDIP_LOG_H
