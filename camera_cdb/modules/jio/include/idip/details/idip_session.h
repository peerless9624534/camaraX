#ifndef IDIP_SESSION_H
#define IDIP_SESSION_H

#include <stdint.h>
#include <sys/socket.h>
#include <libwebsockets.h>
#include <idip.h>
#include <idip/details/idip_methods.h>
#include <idip/details/write_op_item.h>
#include <idip/details/idip_ref_ptr.h>

struct idip_server;
struct idip_stream_base;
struct idip_session;
struct idip_event_notifier;
struct write_op_item;
struct msgpack_sbuffer;
struct idip_thread_strategy;
typedef struct idip_session idip_session_t;
typedef struct idip_stream_base idip_stream_base_t;

/// For internal use only
typedef struct idip_session_stat {
    long long create_ts;                // TL
    long long last_ts;                  // LTS
    unsigned long streams_created;      // SC
    unsigned long streams_deleted;      // SD
    unsigned long events_sended;        // EV
    unsigned long long bytes_readed;    // R
    unsigned long long bytes_sent;    // W
    unsigned long tlv_count;            // TLV
    unsigned long requests;             // RQ
    unsigned long requests_with_errors; // RQE
    int           last_method;          // M
} idip_session_stat_t;

/// libwebsockets compatible protocol handler helper
extern struct lws_protocols idip_protocol;
extern struct lws_protocols idip2ws_protocol;

#ifdef __cplusplus
extern "C" {
#endif

/// Creates new instance of idip session
/// @param owner - pointer to libev's event loop
/// @param fd - IO handler
/// @param sa - socket address information
/// @param slen - length of sa
/// @param timeout_ms - timeout in milliseconds. Client will close connection
/// when no network activity will be in the time.
/// @return pointer to new instance of idip session
///
/// @note The instance is counted. It supports shared_ptr and weak_ptr semantic.
/// This method returns an instance with reference counter equals to 1.
/// Then it should be deleted using @ref idip_session_unref()
idip_session_t*
idip_session_new(struct idip_server *owner, int fd,
                 struct sockaddr* sa, socklen_t slen,
                 uint64_t timeout_ms, struct lws* wsi);

/// Increments a count of strong referenced (shared_ptr) to the object.
idip_session_t*
idip_session_add_ref(idip_session_t* self);

/// Derements a count of strong references to the object. When the counter
/// becomes to zero then the object will destroyed.
void
idip_session_unref(idip_session_t* self);

const char* idip_session_get_name(const idip_session_t* self);
const char* idip_session_get_uid(const idip_session_t* self);

struct idip_server* idip_session_get_server(const idip_session_t* self);

struct idip_event_notifier* idip_session_get_event_notifier(idip_session_t* self);

void
idip_session_send_event(idip_session_t* self, uint64_t request_id,
                        uint64_t sequence_id, const char* event_name,
                        const char *target, uint64_t timestamp,
                        const void* mpk_arg, size_t mpk_arg_size);

void
idip_session_stop(idip_session_t* self);

struct idip_ptr_vector;
void idip_session_collect_active_streams(idip_session_t* self,
                                         struct idip_ptr_vector* streams);

/// Send End-Of-Stream packet
void
idip_session_send_eos(idip_session_t* self, uint64_t request_id,
                      uint64_t sequence_id);

/// A video stream request handler implementation. Uses registered @ref idip_av_source_t
/// objects to read audio/video data.
///
/// @param stream - pointer to stream instance. Do NOT cache it!
/// @param args - pointer to structure with handler's arguments (parameters)
/// @param udata - pointer to user data
/// @return IDIP_OK - if stream was successfully finished,
///         another values of @ref idip_status_code_t if errors was occurred
///
/// @ref idip_av_source_t object are registered with @ref idip_server_av_source_register()
/// function.
idip_status_code_t
idip_av_handler(idip_stream_av_t* stream,
                const idip_stream_av_args_t* args,
                void* udata);

bool
idip_session_enqueue_wop_item(idip_session_t* self, write_op_item_t* wop,
                              int priority);

struct write_op_item*
idip_session_async_write_start(idip_session_t *self,
                               struct msgpack_sbuffer* frame,
                               int priority,
                               write_op_complete_t on_complete,
                               void *on_complete_data);

int idip_session_async_write_local(idip_session_t *self,
                             struct msgpack_sbuffer *frame,
                             void (*on_complete)(void *, size_t, int),
                             void *on_complete_data);

int idip_session_stream_finished_notify(idip_session_t* self,
                                        idip_stream_base_t* stream,
                                        int status_code);

const idip_session_stat_t*
idip_session_get_stat(const idip_session_t* self, idip_session_stat_t *stat);

const char*
idip_session_get_client_id(const idip_session_t* self);

const char*
idip_session_get_peer_name(const idip_session_t* self);

int
idip_session_get_fd(const idip_session_t* self);

size_t
idip_session_get_mtu(const idip_session_t* self);

void
idip_session_stat_update_last_access_time(idip_session_t *self, long long now);

bool
idip_session_has_the_same_weak_ptr(idip_session_t* self, weak_ptr_t* weak);

int
idip_session_do_write_from_wscli(idip_session_t* self, struct lws *wsi);

// TODO: idip_session_DO_read_data()
int
idip_session_read_data(idip_session_t* self, const void* buf, size_t size);

// TODO: idip_session_stop_from_lws()
void
idip_lws_stop_session(idip_session_t** self);

void
idip_session_handle_lws_cancel_event(idip_session_t **p_self);

int
idip_session_register_stream(idip_session_t* self, weak_ptr_t* stream_weak);

#ifdef __cplusplus
}
#endif

#endif // IDIP_SESSION_H
