#ifndef IDIP_STREAM_BASE_H
#define IDIP_STREAM_BASE_H

#include <stdint.h>
#include <pthread.h>
#include <idip.h>
#include <idip/details/idip_thread_manager.h>
#include <idip/details/idip_transport.h>
#include <idip/details/idip_ref_ptr.h>
#include <idip/idip_av_packet.h>
#include <idip/idip_vca_handler.h>
#include <idip/details/idip_targets_map.h>
#include <idip/idip_rc.h>
#include <idip/details/idip_queue.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct idip_session;
struct write_op;
struct msgpack_sbuffer;
struct iovec;
struct idip_gc;

#ifndef IDIP_STAT_AVG_ITEMS
#define IDIP_STAT_AVG_ITEMS 50
#endif /* IDIP_STAT_AVG_ITEMS */

#ifndef IDIP_WOP_FINAL_TIMEOUT
/// Last thread write operation timeout.
#define IDIP_WOP_FINAL_TIMEOUT 5000
#endif /* IDIP_WOP_FINAL_TIMEOUT */

/// request id for internal requests
#define INTERNAL_RID UINT64_MAX

/// For internal use only
typedef struct idip_stream_stat {
    long long create_ts;            // TL
    long long gc_ts;                // TGC
    long long last_ts;              // TLA (last access)
    unsigned long long bytes_readed;// R
    unsigned long long bytes_sended;// W
    unsigned long long write_ops;   // WOP
    uint64_t  last_vts;             // last video frame time stamp
    uint64_t  last_ats;             // last audio frame time stamp
    uint64_t  first_vts;            // first video frame time stamp
    uint64_t  first_ats;            // first audio frame time stamp
    size_t    vframes_count;        // count of successfuly snded video frames
    size_t    aframes_count;        // --""-- audio frames
} idip_stream_stat_t;

typedef enum {
    STREAM_STATE_INITIALIZED = 0,
    STREAM_STATE_QUEUED,
    STREAM_STATE_ACTIVE,
    STREAM_STATE_FINISHED
}
idip_stream_state_t;


enum {
    IDIP_STREAM_PRIORITY_HIGH = 0,
    IDIP_STREAM_PRIORITY_MEDIUM = 1,
    IDIP_STREAM_PRIORITY_LOW = 2,

    IDIP_STREAM_PRIORITY_DEFAULT = IDIP_STREAM_PRIORITY_HIGH
};

typedef struct idip_stream_tx_data
{
    void*           buf;
    struct iovec*   iov;
    size_t          iov_size;
    long            timeout_ms;
    int             transport_flags; ///< additional transport flags
    idip_v2_transport_sysext_t* system_map;
}
idip_stream_tx_data_t;

typedef int (*idip_stream_run_handler_t)(void* self);
typedef void (*idip_stream_destroy_handler_t)(void* self);
typedef void (*idip_stream_queue_item_deleter_t)(void* self);
typedef void (*idip_stream_can_send_handler_t)(struct idip_stream_base* self);
typedef void (*idip_stream_done_handler_t)(struct idip_stream_base*);

/// For each request_id we create its own reply context.
/// The struct is a base class of contexts.
struct idip_stream_base {
    // 0. Reference counter
    ref_block_t* rb;
    // 1. read-only shared data
    // 1.1. stream name
    char* name;
    // 1.2.1 owner session
    struct idip_session*        session;
    // 1.2.2 owner server (weak). Only for statistics
    struct idip_server*         server;
    // 1.3. some statistic
    idip_stream_stat_t  stat;
    pthread_mutex_t     stat_lock;
    // 1.4. to identify context in set of contexts
    uint64_t            request_id;
    uint64_t            sequence_id;
    // 1.5. Write priority for all packets. Set statically for each stream type
    // 0 - max (default), 1 - medium (video.get), 2 - low (archive.get)
    int                 priority;
    // 1.6. file descriptor
    int                 fd;
    // 1.7. method type
    method_t            method;

    // 2. windowing
    // 2.1. MTU size in bytes. Initialized at object creation and never changes.
    const size_t        mtu;
    // 2.2. Window size. Initialized at object creation. Zero mean that
    // windowing algorithm isn't used.
    const size_t        tx_window;
    // 2.3. Mine window size.
    const size_t        rx_window;
    // 2.3. What data piece we can send without acknowledge and optional callback
    // to notify about this
    size_t              available_to_send; // access under lock
    idip_stream_can_send_handler_t on_available_to_send_cb; // is called out of lock
    // 2.4. number of last accepted byte
    uint64_t            last_accepted;     // access under lock
    // 2.5. amount of received data
    uint64_t            received; // access under lock
    // 2.6. input queue
    idip_queue_t*        incoming_queue; // access under lock
    // accumulated size of data that hasn't been acknowledged yet
    size_t               pending_ack;

    // 3. state data
    // 3.1. thread done flag and optional callback to notify about this
    int                 done;
    idip_stream_done_handler_t on_stream_done_cb; // is called out of lock
    // 3.2. current write operation complete flag
    int                 write_completed;
    // 3.3. running state. When the state is "active" then the object
    // is accessed by a runner thread. In another states it can be deleted safely.
    idip_stream_state_t state;


    // 4. thread and synchronization
    // 4.1. lock state data
    pthread_mutex_t     lock;
    // 4.2. condition to wakeup threads that are waiting state/window data changes.
    pthread_cond_t      wakeup;
    // 4.3. A bound (owner/user) thread attaching/join methods and data.
    // Will initialized separately.
    idip_thread_strategy_t  thread_strategy;
    // 4.4. A callback that will be called on wakeup methods. Can be NULL.
    // The function allows being woken up conditions inside inherited streams.
    void (*advanced_wakeup_notice)(struct idip_stream_base* self);

    // 5. Pointer to a derived class destructor.
    idip_stream_destroy_handler_t destroy_handler;

    // 6. Real stream handler implementation
    idip_stream_run_handler_t handler_impl;
    // 6.2. Internal incoming queue item destory method (can be NULL)
    idip_stream_queue_item_deleter_t qitem_deleter;

    // 7. server's GC
    struct idip_gc*        gc;

    bool                   internal;
    // Overrided handler exit result. If set then the libidip will send
    // this code and this error message in system field.
    bool                    override;
    idip_rc_t               override_result;
    char*                   override_emsg;
};
typedef struct idip_stream_base idip_stream_base_t;

#define IDIP_STREAM_BASE_INIT_PARAMS_DECL \
    struct idip_server*         server; \
    struct idip_session*        owner; \
    size_t window;

typedef struct idip_stream_creation_params {
    struct idip_server*         server;    ///< pointer to the server instance
    struct idip_session*        owner;     ///< owned session (optional)
    bool                        no_owner;  ///< the session has no owner (for internal use only)
    bool                        is_internal; ///< The method is internally implemented
    size_t                      tx_window; ///< peer window size
    size_t                      rx_window; ///< stream receiver window size
    size_t                      mtu;       ///< maximum transfer unit for tx?
    idip_method_handler_t       handler;   ///< handler with type and udata
}
idip_stream_creation_params_t;

/**
 * @brief Policy for create stream's name
 * @param owner - pointer to owner
 * @param request_id - request identifier
 * @param method_name - method name
 * @return
 */
char* idip_stream_make_name(struct idip_session *owner,
                            uint64_t request_id,
                            const char* method_name);

/**
 * @brief Assertion helper to use in stream constructors. Checks that
 * the @ref params isn't NULL, the params->server is set, a handler is set
 * and has a compatible type.
 * @param params - pointer to structire to check
 * @param method - metod type that should had a handler in the params
 * @return In DEBUG mode uses assert() call, with NDEBUG return -1 when fail
 * and 0 otherwise.
 */
int idip_stream_creation_params_assert(const idip_stream_creation_params_t* params,
                                       method_t method);

/**
 * @brief Initialize base part of stream instance.
 * @param self - pointer to base part of stream instance
 * @param request_id - current request identifier
 * @param handler_impl - pointer to user handler call wrapper
 * @param destroy_handler - pointer to function that will be call before freeing
 * the instance, e.g. child's "destructor".
 * @return 0 if success, -1 otherwise.
 */
int
idip_stream_base_init(idip_stream_base_t* self,
                      uint64_t request_id,
                      const idip_stream_creation_params_t *params,
                      idip_stream_run_handler_t handler_impl,
                      idip_stream_destroy_handler_t destroy_handler,
                      idip_stream_queue_item_deleter_t qitem_destroy);

void
idip_stream_base_destroy(idip_stream_base_t* self);

void
idip_stream_base_lock(idip_stream_base_t* self);

void
idip_stream_base_unlock(idip_stream_base_t* self);

void
idip_stream_base_wakeup(idip_stream_base_t* self);

int
idip_stream_base_wait(idip_stream_base_t* self,
                      struct timespec* abstime_monotonic);

void
idip_stream_base_set_queued(idip_stream_base_t* self);

void
idip_stream_base_set_active(idip_stream_base_t* self);

void
idip_stream_base_set_finished(idip_stream_base_t* self);

void
idip_stream_base_windowing_set_accepted(idip_stream_base_t* self, uint64_t accepted);

int
idip_stream_base_call_handler(idip_stream_base_t* self);


const char*
idip_stream_base_get_name(const idip_stream_base_t* self);

/**
 * @brief idip_stream_start - Creates streamer thread and run thread_proc in it.
 * @param self - pointer to stream instance
 * @param thread_proc - pointer to function which runs in streamer thread
 * @param thread_arg - pointer to thread_proc's argument
 * @return 0 if success, -1 otherwise
 */
int idip_stream_base_start(idip_stream_base_t* self);

/**
 * @brief Basic send operation for msgpack_sbuffer
 * @param self - pointer to stream instance
 * @param sbuf - pointer to data buffer
 * @param timeout_ms - send operation timeout in milliseconds.
 * Set -1 to infinite timeout.
 * @param done_mask - done mask. see @ref idip_stream_base_done_mask_t
 * @return IDIP_OK if success or IDIP_GENERIC_ERROR in thread wants to stop case
 * or IDIP_SEND_TIMEOUT if timeout was occurred.
 */
int idip_stream_do_send_sbuf(idip_stream_base_t* self,
                             struct msgpack_sbuffer *sbuf, long timeout_ms,
                             bool log_enabled);

int idip_stream_do_send_sbuf_with_ack(idip_stream_base_t* self,
                                      struct msgpack_sbuffer *sbuf,
                                      long timeout_ms,
                                      bool log_enabled);

int idip_stream_do_send_iovec(idip_stream_base_t* self,
                              const struct iovec *iov,
                              size_t iov_count, long timeout_ms,
                              bool log_enabled);
/**
 * @brief Send status code with has_no_more_data = false flag
 * @param self - pointer to stream instance
 * @param status - status code to send
 * @param timeout_ms - operation timeout in milliseconds.
 * @return IDIP_OK if success or IDIP_GENERIC_ERROR in thread wants to stop case
 * @note This function disables a "graceful" thread termination. Only "hard"
 * termination can cancel this write operation!
 */
void idip_stream_send_status(idip_stream_base_t* self, idip_rc_t status, long timeout_ms);

/**
 * @brief Do notify owner of stream about stream's thread is finish.
 * @param self - pointer to stream instance
 * @param status_code - stream exit status code
 */
void idip_stream_thread_finished_notify(idip_stream_base_t* self, int status_code);

/**
 * Sends stop signal to stream's thread. It cancels any write operations.
 */
void idip_stream_stop_notify(idip_stream_base_t* self);

idip_rc_t idip_stream_thread_status_code_to_packet_status(int rc);

/**
 * @param self - pointer to stream instance
 * @param info - pointer to structure with video's properties
 * @param timeout_ms - operation timeout in milliseconds.
 * Can be set to @ref IDIP_INFINITE_TIMEOUT
 */
idip_status_code_t
idip_stream_base_put_video_info(idip_stream_base_t* self,
                                const video_stream_info_t* info,
                                long timeout_ms);

idip_status_code_t
idip_stream_base_put_audio_info(idip_stream_base_t* self,
                                const audio_stream_info_t* info,
                                long timeout_ms);

/**
 * @param self - pointer to stream instance
 * @param v_info - pointer to structure with video meta information. Cannot be NULL.
 * @param a_info - pointer to audio stream description. Can be NULL if no audio info is available.
 * @param d_info - pointer to structure that contains time information. Cannot be NULL.
 * @param timeout_ms - operation timeout in milliseconds.
 * Can be set to @ref IDIP_INFINITE_TIMEOUT
 * @return
 */
idip_status_code_t
idip_stream_base_put_meta_info(idip_stream_base_t* self,
                               const video_stream_info_t* v_info,
                               const audio_stream_info_t* a_info,
                               const archive_record_t* d_info,
                               long timeout_ms);


idip_status_code_t
idip_stream_base_put_av_packet(idip_stream_base_t* self,
                               const idip_av_packet_t* packet,
                               long timeout_ms);

void
idip_stream_base_log_pts(idip_stream_base_t* self);

/**
 * @brief Initializes an instance of @ref idip_stream_creation_params_t,
 * set a suitable handler to call, initialize fields to a consistant state.
 *
 * @param params - pointer to an object to initialize
 * @param server - pointer to a @ref idip_server_t instance
 * @param session - pointer to a bound @ref idip_session_t object that owns the stream.
 * Can be NULL for streams, that would be called directly (e.g. from server).
 * @param target - a target name or NULL for system-wide streams.
 * @param method - method type. The stream handler will initialized with this method type handler.
 *
 * @return 0 on success, -1 on errors.
 *
 * @note The function increases a session reference counter (if it isn't NULL)
 * on success, but does not on errors.
 */
idip_rc_t
idip_stream_creation_params_init(idip_stream_creation_params_t* params,
                                 idip_server_t* server,
                                 struct idip_session* session,
                                 const char* target,
                                 method_t method);

typedef enum idip_waitp_rc_t
{
    IDIP_WAITP_DONE, // wait done, stream done flag is signalled
    IDIP_WAITP_PRED, // wait done, a wait predicate condition occured
    IDIP_WAITP_TIMEOUT, // wait timeout
    IDIP_WAITP_ERROR, // syscall error during wait
}
idip_waitp_rc_t;

/// Addidtional wait predicate callback for stream wait function.
/// @return 0 if nonsignalled (waiting), 1 if signaled (wait done)
typedef int(*idip_wait_predicate_t)(void* udata);

idip_waitp_rc_t
idip_stream_base_waitp(idip_stream_base_t* self, long long timeout_ms,
                        idip_wait_predicate_t predicate, void* predicate_data);

/**
 * @brief Set up or update handler exit code and status. A handler exit code
 * will ignored and replaced by the given code and message. Multiple calls
 * of the function will store the latest call data.
 *
 * @param self - pointer to stream instance
 * @param ec - error code to send after a handler finished
 * @param emsg - error message to send after a handler finished
 * @param move - do "move" @a emsg to the @a self instead copying. The @a self
 * will own the string and will free it by tiselves. PAY ATTENTION: the @a emsg
 * should be a malloc()'ed string.
 */
void idip_stream_base_set_result(idip_stream_base_t* self, idip_rc_t ec, const char* emsg, bool move);

/**
 * @brief Appends an item to a internal incoming_queue (MT-safe).
 * @param self - pointer to stream instance
 * @param data - pointer to data object
 * @param hlp_size - size of High-Level Packet (HLP), which contained the @param data.
 *                   In the general case it isn't equal to the size of the @param data
 * @param eos - the sign of the last packet (end-of-stream)
 * @return 0 if succes, -1 on error
 */
int idip_stream_base_enqueue_data(idip_stream_base_t* self, void* data, size_t hlp_size, bool eos);

/**
 * @brief Remove an object from tail of incoming queue if it exists (MT-safe).
 * @return pointer to an object or NULL if the incoming queue is empty or an error occured
 */
void* idip_stream_base_dequeue_data(idip_stream_base_t* self, bool * eos);

/**
 * @brief Deletes @a item using stream queue item deleter
 * @param self - pointer to a stream instance
 * @param item - pointer to an item returned from @ref idip_stream_base_dequeue_data()
 */
void idip_stream_base_delete_queue_item(idip_stream_base_t* self, void* item);

/**
 * @brief Extracts the current accumulated ack value and then resets it (MT-safe).
*/
size_t idip_stream_consume_ack(idip_stream_base_t* self);

/**
 * @brief Adds ack_size to the pending ack (not yet acknowledged) (MT-safe).
 * Normally, you don't need this function, so use it carefully!
*/
void idip_stream_add_pending_ack(idip_stream_base_t* self, size_t ack_size);

bool idip_stream_base_test_done(idip_stream_base_t *self);
void idip_stream_base_advanced_wakeup(idip_stream_base_t* self);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // IDIP_STREAM_BASE_H
