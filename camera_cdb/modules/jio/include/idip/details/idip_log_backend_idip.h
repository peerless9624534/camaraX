#ifndef IDIP_LOG_BACKEND_IDIP_H
#define IDIP_LOG_BACKEND_IDIP_H

#include <idip/idip_log.h>
#include <idip/details/idip_stream_base.h>
#include <idip/idip_vector.h>
#include <pthread.h>

typedef struct idip_stream_log_query_args
{
    /// Enable or disable proxying logs throught IDIP to the current peer.
    bool proxying_enabled;
    /// A value of proxying_enabled was set by the request.
    bool proxying_enabled_is_set;
    /// log priority. 0 (higest) ... 6 (lowest).
    /// Log strings with a priority geather than the given will rejected.
    int priority;
    /// Buffering time in milliseconds. The logging subsystem will not wait
    /// buffer overflow and will send the buffer each time.
    long long buffer_time_ms;
    /// Maximal amount of log lines in the buffer.
    size_t buffer_limit;
}
idip_stream_log_query_args_t;


typedef struct idip_stream_log_t
{
    struct idip_stream_base      base;
    idip_stream_log_query_args_t query_args;
    pthread_mutex_t msg_lock;
    pthread_cond_t  msg_cond;
    idip_vector_t* items; ///< log items (input)
    idip_vector_t* items2send; ///< log items to send
    size_t count_limit;     ///< maximal amount of log lines in the queue
    size_t current_size;    ///< allocated data size

} idip_stream_log_t;

#ifdef __cplusplus
extern "C" {
#endif


int idip_stream_log_init(idip_stream_log_t* self,
                         uint64_t request_id,
                         idip_stream_creation_params_t* init_params,
                         const idip_stream_log_query_args_t* query_args);

const idip_log_backend_t* idip_log_backend_idip_get();

#ifdef __cplusplus
}
#endif

#endif // IDIP_LOG_BACKEND_IDIP_H
