#ifndef IDIP_LOG_BACKEND_RSYSLOG_H
#define IDIP_LOG_BACKEND_RSYSLOG_H

#include <idip/idip_log.h>

typedef struct idip_rsyslog_conf_t {
    /// Syslog Message Facilities MUST be in the range of 0 to 23 inclusive
    unsigned int facility;
    /// field identifies the machine that originally sent the syslog message.
    char* host_name;
    /// field SHOULD identify the device or application that originated the message.
    /// It is a string without further semantics.
    char* app_name;
    /// value that is included in the message, having no
    /// interoperable meaning, except that a change in the value indicates
    /// there has been a discontinuity in syslog reporting.  The field does
    /// not have any specific syntax or semantic
    char* proc_id;
    /// SHOULD identify the type of message
    char* msg_id;
    /// provides a mechanism to express information in a well
    /// defined, easily parseable and interpretable data format.
    char* structured_data;
    /// Switch compact/full log message header
    char use_compact_msg;
} idip_rsyslog_conf_t;

/// An opaque data structure that represent a UDP syslog backend for idip_log_xxx() functions
typedef struct idip_log_backend_rsyslog_udp_t idip_log_backend_rsyslog_udp_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new syslog backen that can
 * send messages in syslog format via UDP
 * @param conf pointer to syslog static configuration
 * @param host host name to connect to
 * @param port UDP port unmber to connect to
 * @return new instance of @ref idip_rsyslog_udp_backend_t
 */
idip_log_backend_rsyslog_udp_t*
idip_log_backend_rsyslog_udp_new(const idip_rsyslog_conf_t* conf,
                                 const char* host, int port);

/**
 * Delete rsyslog backend.
 * @param self pointer to an object created by @ref idip_rsyslog_udp_backend_new()
 *
 * @note The function is compatible with the type @ref log_backend_close_t.
 */
void idip_log_backend_rsyslog_udp_delete(idip_log_backend_rsyslog_udp_t* self);


size_t idip_log_rsyslog_formatter(void* udata,
                                  char* buf, size_t buf_size,
                                  const idip_log_metadata_t* item,
                                  const char* fmt, va_list ap);

void idip_log_rsyslog_put(void* udata,
                          const idip_log_metadata_t *metadata,
                          const char* buf);

void idip_log_rsyslog_close(void* udata);


bool idip_log_backend_rsyslog_init(idip_log_backend_t *backend,
                                   int max_priority,
                                   const idip_rsyslog_conf_t *conf,
                                   const char *host, int port);

#ifdef __cplusplus
}
#endif

#endif // IDIP_LOG_BACKEND_RSYSLOG_H
