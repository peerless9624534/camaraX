#ifndef IDIP_SYSTEM_PROXY_PRIVATE_STREAM_PROTO_H
#define IDIP_SYSTEM_PROXY_PRIVATE_STREAM_PROTO_H

/// Header-only helper
#include <idip/details/system_proxy/private/base_proto.h>
#include <idip/details/idip_byte_ringbuf.h>
#include <idip/details/idip_string_utils.h>
#include <idip/idip_deque.h>
#include <idip/idip_wrappers.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct stream_chunk_meta {
    size_t size;
    size_t ack_size; // for ack purposes
} stream_chunk_meta_t;

typedef struct stream_window_buf {
    window_buf_base_t base;
    idip_byte_ringbuf_t * data;
    idip_deque_t * metas;  // chunk_meta_t objects
    pthread_mutex_t mutex;
    bool eos;
} stream_window_buf_t;

/* stream_window_buf */
static inline void stream_window_buf_destroy(stream_window_buf_t * self)
{
    if (self->data)
        idip_byte_ringbuf_delete(self->data);
    if (self->metas)
        idip_deque_destroy(self->metas);

    idip_mutex_destroy(&self->mutex);
    memset(self, 0, sizeof(*self));
}

static inline bool stream_window_buf_write(window_buf_base_t * self_base,
                                           const void * data, size_t size,
                                           size_t ack_size, bool eos)
{
    stream_window_buf_t * self = (stream_window_buf_t *) self_base;
    idip_mutex_lock(&self->mutex);
    if (self->eos) {
        idip_mutex_unlock(&self->mutex);
        return false;
    }

    self->eos = eos;
    if (!data || size == 0) {
        idip_mutex_unlock(&self->mutex);
        return true;
    }

    // doesn't rewrite ringbuf in case of insufficient capacity
    if (idip_byte_ringbuf_write(self->data, data, size) != 0) {
        idip_mutex_unlock(&self->mutex);
        return false;
    }

    stream_chunk_meta_t meta = {.size = size, .ack_size = ack_size};
    idip_deque_push_back(self->metas, &meta);
    idip_mutex_unlock(&self->mutex);
    return true;
}

static inline void stream_window_buf_consume(window_buf_base_t * self_base,
                                             size_t size, size_t * ack_size)
{
    stream_window_buf_t * self = (stream_window_buf_t *) self_base;
    *ack_size = 0;
    idip_mutex_lock(&self->mutex);
    if (idip_byte_ringbuf_pop_front(self->data, size) != 0)
        return idip_mutex_unlock(&self->mutex);

    size_t cons_size = size;
    while (!idip_deque_is_empty(self->metas) && cons_size > 0) {
        stream_chunk_meta_t * meta = (stream_chunk_meta_t *) idip_deque_peek_front(self->metas);
        if (meta->size <= cons_size) {
            // a chunk was consumed completely
            cons_size -= meta->size;
            *ack_size += meta->ack_size;
            idip_deque_pop_front(self->metas);
        } else {
            // only part of a chunk was consumed, so we should shrink it
            meta->size -= cons_size;
            meta->ack_size -= cons_size;
            *ack_size += cons_size;
            break;
        }
    }

    idip_mutex_unlock(&self->mutex);
}

static inline void stream_window_buf_peek(window_buf_base_t * self_base,
                                          uint8_t ** data, size_t * size,
                                          bool * eos)
{
    stream_window_buf_t * self = (stream_window_buf_t *) self_base;
    idip_iovec_t vec[2];
    memset(&vec, 0, sizeof(vec));

    idip_mutex_lock(&self->mutex);
    idip_byte_ringbuf_peek_iovec(self->data, vec);
    *data = vec[0].iov_base;
    *size = vec[0].iov_len;
    *eos = false;
    if (*size == 0 || *data == NULL) {
        *data = NULL;
        *size = 0;
        *eos = self->eos;
    }

    idip_mutex_unlock(&self->mutex);
}

static inline bool stream_window_buf_create(stream_window_buf_t * self, size_t size)
{
    memset(self, 0, sizeof(stream_window_buf_t));
    idip_mutex_init(&self->mutex);
    idip_byte_ringbuf_new(&self->data, size);
    if (!self->data)
        goto fail;

    self->metas = idip_deque_create(sizeof(stream_chunk_meta_t), NULL);
    if (!self->metas)
        goto fail;

    self->base.consume = stream_window_buf_consume;
    self->base.peek = stream_window_buf_peek;
    self->base.write = stream_window_buf_write;
    return true;

fail:
    stream_window_buf_destroy(self);
    return false;
}

/* stream_proto */

typedef struct stream_proto {
    base_proto_t base;
    stream_window_buf_t window_buf_storage;
} stream_proto_t;

static inline bool stream_proto_connect(base_proto_t * self_base, session_t * sess,
                                        struct lws_context * ctx, struct lws_vhost * vhost,
                                        const char * proto_id)
{
    stream_proto_t * self = (stream_proto_t *) self_base;
    struct lws_client_connect_info info;
    memset(&info, 0, sizeof(info));
    info.context = ctx;
    info.port = self->base.port;
    info.address = self->base.addr;
    info.local_protocol_name = proto_id;
    info.vhost = vhost;
    info.method = "RAW";
    info.opaque_user_data = sess;
    if (!lws_client_connect_via_info(&info)) {
        idip_log_xerror(__func__, "Connection failed: %s", idip_sys_proxy_session_get_log_info(sess));
        return false;
    }

    return true;
}

static inline void stream_proto_on_lws_event(base_proto_t * self_base, session_t * sess,
                                             struct lws * wsi, enum lws_callback_reasons reason,
                                             void * user, void * data, size_t size)
{
    IDIP_UNUSED(self_base);
    IDIP_UNUSED(user);

    switch (reason) {
        case LWS_CALLBACK_CONNECTING:
            // This event occurs directly in the lws_client_connect_via_info() function
            idip_sys_proxy_session_on_conn_setup(sess, wsi);
            break;
        case LWS_CALLBACK_RAW_CONNECTED:
            idip_log_xinfo(__func__, "Connected: %s", idip_sys_proxy_session_get_log_info(sess));
            schedule_sending(wsi);
            break;
        case LWS_CALLBACK_RAW_CLOSE:
            // This event occurs only if the connection was established
            idip_log_xinfo(__func__, "Disconnected: %s", idip_sys_proxy_session_get_log_info(sess));
            break;
        default:
            break;
    }

    base_proto_on_lws_event(sess, wsi, reason, user, data, size);
}

static inline idip_sendmsg_res_t stream_proto_sendmsg(base_proto_t * self_base,
                                                      struct lws * wsi,
                                                      idip_iovec_t * msg_iov,
                                                      size_t msg_iovlen)
{
    IDIP_UNUSED(self_base);
    idip_sendmsg_res_t res = idip_sendmsg(lws_get_socket_fd(wsi), NULL, 0, msg_iov, msg_iovlen, 0);
    if (res.ec == 0)
        return res;
    if (res.ec == EAGAIN || res.ec == EWOULDBLOCK) {
        // it's ok, there's no space in socket buffer for now
        res.ec = 0;
        res.sent = 0;
        return res;
    }

    return res;
}

static inline void stream_proto_close_sending(base_proto_t * self_base, struct lws * wsi)
{
    IDIP_UNUSED(self_base);
    shutdown(lws_get_socket_fd(wsi), SHUT_WR);
}


static inline void stream_proto_destroy(base_proto_t * self_base)
{
    if (self_base->window_buf)
        stream_window_buf_destroy((stream_window_buf_t *) self_base->window_buf);

    base_proto_destroy(self_base);
}

static inline bool stream_proto_create(stream_proto_t * self, const char * addr,
                                       uint16_t port, idip_sys_proxy_proto_t type,
                                       size_t window_size)
{
    if (type != IDIP_SYS_PROXY_PROTO_TCP && type != IDIP_SYS_PROXY_PROTO_UNIX)
        return false;

    memset(self, 0, sizeof(*self));
    if (!stream_window_buf_create(&self->window_buf_storage, window_size))
        return false;

    if (type == IDIP_SYS_PROXY_PROTO_TCP) {
        base_proto_init(&self->base, type, &self->window_buf_storage.base, addr, port);
    } else {
        // lws specific: we should prepend '+' to the addr
        base_proto_init(&self->base, type, &self->window_buf_storage.base, NULL, port);
        self->base.addr = idip_alloc_fmt_str("+%s", addr);
        if (!self->base.addr)
            goto fail;
    }

    self->base.connect = stream_proto_connect;
    self->base.on_lws_event = stream_proto_on_lws_event;
    self->base.destroy = stream_proto_destroy;
    self->base.sendmsg = stream_proto_sendmsg;
    self->base.close_sending = stream_proto_close_sending;
    return true;

fail:
    stream_proto_destroy(&self->base);
    return false;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // IDIP_SYSTEM_PROXY_PRIVATE_STREAM_PROTO_H
