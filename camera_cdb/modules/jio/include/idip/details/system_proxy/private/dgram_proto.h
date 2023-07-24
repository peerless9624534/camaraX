#ifndef IDIP_SYSTEM_PROXY_PRIVATE_DGRAM_PROTO_H
#define IDIP_SYSTEM_PROXY_PRIVATE_DGRAM_PROTO_H

/// Header-only helper
#include <idip/details/system_proxy/private/base_proto.h>
#include <idip/details/system_proxy/private/misc.h>
#include <idip/details/idip_byte_ringbuf.h>
#include <idip/idip_deque.h>
#include <idip/idip_wrappers.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct dgram_data {
    void * data;
    size_t size;
    size_t ack_size; // for ack purposes
} dgram_data_t;

typedef struct dgram_window_buf {
    window_buf_base_t base;
    idip_deque_t * data;  // dgram_data_t objects
    pthread_mutex_t mutex;
    bool eos;
} dgram_window_buf_t;

/* dgram_window_buf */
static inline void dgram_data_destroy(void * self_raw)
{
    dgram_data_t * self = (dgram_data_t *) self_raw;
    free(self->data);
}

static inline void dgram_window_buf_destroy(dgram_window_buf_t * self)
{
    if (self->data)
        idip_deque_destroy(self->data);

    idip_mutex_destroy(&self->mutex);
    memset(self, 0, sizeof(*self));
}

static inline bool dgram_window_buf_write(window_buf_base_t * self_base,
                                          const void * data, size_t size,
                                          size_t ack_size, bool eos)
{
    dgram_window_buf_t * self = (dgram_window_buf_t *) self_base;
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

    dgram_data_t d = {.data = malloc(size), .size = size, .ack_size = ack_size};
    memcpy(d.data, data, size);
    idip_deque_push_back(self->data, &d);
    idip_mutex_unlock(&self->mutex);
    return true;
}

static inline void dgram_window_buf_consume(window_buf_base_t * self_base,
                                            size_t size, size_t * ack_size)
{
    dgram_window_buf_t * self = (dgram_window_buf_t *) self_base;
    *ack_size = 0;
    idip_mutex_lock(&self->mutex);

    if (idip_deque_is_empty(self->data))
        return idip_mutex_unlock(&self->mutex);

    dgram_data_t * d = (dgram_data_t *) idip_deque_peek_front(self->data);
    *ack_size = d->ack_size;
    assert(size == d->size);
    idip_deque_pop_front(self->data);
    idip_mutex_unlock(&self->mutex);
}

static inline void dgram_window_buf_peek(window_buf_base_t * self_base,
                                         uint8_t ** data, size_t * size,
                                         bool * eos)
{
    dgram_window_buf_t * self = (dgram_window_buf_t *) self_base;
    idip_mutex_lock(&self->mutex);
    if (idip_deque_is_empty(self->data)) {
        *data = NULL;
        *size = 0;
        *eos = self->eos;
        return idip_mutex_unlock(&self->mutex);
    }

    dgram_data_t * d = (dgram_data_t *) idip_deque_peek_front(self->data);
    *data = d->data;
    *size = d->size;
    *eos = false;
    idip_mutex_unlock(&self->mutex);
}

static inline bool dgram_window_buf_create(dgram_window_buf_t * self, size_t size)
{
    IDIP_UNUSED(size);
    memset(self, 0, sizeof(dgram_window_buf_t));
    idip_mutex_init(&self->mutex);
    self->data = idip_deque_create(sizeof(dgram_data_t), dgram_data_destroy);
    if (!self->data)
        goto fail;

    self->base.consume = dgram_window_buf_consume;
    self->base.peek = dgram_window_buf_peek;
    self->base.write = dgram_window_buf_write;
    return true;

fail:
    dgram_window_buf_destroy(self);
    return false;
}

/* dgram_proto */

typedef struct dgram_proto {
    base_proto_t base;
    struct sockaddr_storage peer_addr_storage;
    size_t peer_addr_len;
    dgram_window_buf_t window_buf_storage;
    uint32_t max_dgram_size;
} dgram_proto_t;

static inline bool dgram_proto_connect(base_proto_t * self_base, session_t * sess,
                                       struct lws_context * ctx, struct lws_vhost * vhost,
                                       const char * proto_id)
{
    IDIP_UNUSED(ctx);

    dgram_proto_t * self = (dgram_proto_t *) self_base;
    uint16_t any_port_to_bind = 0;
    if (!lws_create_adopt_udp(vhost, self->base.addr, any_port_to_bind, LWS_CAUDP_BIND,
                              proto_id, NULL, NULL, sess, NULL, NULL)) {
        idip_log_xerror(__func__, "Connection failed: %s", idip_sys_proxy_session_get_log_info(sess));
        return false;
    }

    return true;
}


static inline void dgram_proto_on_lws_event(base_proto_t * self_base, session_t * sess,
                                            struct lws * wsi, enum lws_callback_reasons reason,
                                            void * user, void * data, size_t size)
{
    IDIP_UNUSED(user);

    dgram_proto_t * self = (dgram_proto_t *) self_base;
    switch (reason) {
        case LWS_CALLBACK_RAW_ADOPT: {
            // This event occurs directly in the lws_client_connect_via_info() function
            // Saving peer address
            const struct lws_udp * udp_info = lws_get_udp(wsi);
            self->peer_addr_len = sa46_socklen(&udp_info->sa46);
            memcpy(&self->peer_addr_storage, sa46_sockaddr(&udp_info->sa46), self->peer_addr_len);
            if (self->peer_addr_storage.ss_family == AF_INET) {
                struct sockaddr_in * sa = (struct sockaddr_in *) (&self->peer_addr_storage);
                sa->sin_port = self->base.port;
            } else if (self->peer_addr_storage.ss_family == AF_INET6) {
                struct sockaddr_in6 * sa = (struct sockaddr_in6 *) (&self->peer_addr_storage);
                sa->sin6_port = self->base.port;
            }

            idip_sys_proxy_session_on_conn_setup(sess, wsi);
            schedule_sending(wsi);
            break;
        }
        case LWS_CALLBACK_RAW_RX:
            if (size > self->max_dgram_size) {
                idip_sys_proxy_session_on_error(sess, IDIP_RC_INTERNAL_ERROR, "max dgram size exceeded");
                return;
            }
            break;
        default:
            break;
    }

    base_proto_on_lws_event(sess, wsi, reason, user, data, size);
}

static inline idip_sendmsg_res_t dgram_proto_sendmsg(base_proto_t * self_base,
                                                     struct lws * wsi,
                                                     idip_iovec_t * msg_iov,
                                                     size_t msg_iovlen)
{
    msg_iovlen = 1; // send one datagram at time
    dgram_proto_t * self = (dgram_proto_t *) self_base;
    idip_sendmsg_res_t res = idip_sendmsg(lws_get_socket_fd(wsi),
        (struct sockaddr *)&self->peer_addr_storage, self->peer_addr_len,
        msg_iov, msg_iovlen, 0
    );

    if (res.ec == 0) {
        if (res.sent < msg_iov[0].iov_len) {
            // We've got a partial sending, it's an error
            res.ec = EMSGSIZE;
            res.sent = 0;
        }
    } else if (res.ec == EAGAIN || res.ec == EWOULDBLOCK) {
        res.ec = 0;
        res.sent = 0;
    }

    return res;
}

static inline void dgram_proto_close_sending(base_proto_t * self_base, struct lws * wsi)
{
    IDIP_UNUSED(self_base);
    IDIP_UNUSED(wsi);
}

static inline void dgram_proto_destroy(base_proto_t * self_base)
{
    if (self_base->window_buf)
        dgram_window_buf_destroy((dgram_window_buf_t *) self_base->window_buf);

    base_proto_destroy(self_base);
}

static inline bool dgram_proto_create(dgram_proto_t * self, const char * addr,
                                      uint16_t port, uint32_t max_dgram_size,
                                      size_t window_size)
{
    memset(self, 0, sizeof(*self));
    if (!dgram_window_buf_create(&self->window_buf_storage, window_size))
        return false;

    base_proto_init(&self->base, IDIP_SYS_PROXY_PROTO_UDP,
        &self->window_buf_storage.base, addr, port
    );
    self->max_dgram_size = max_dgram_size;
    self->base.connect = dgram_proto_connect;
    self->base.on_lws_event = dgram_proto_on_lws_event;
    self->base.destroy = dgram_proto_destroy;
    self->base.sendmsg = dgram_proto_sendmsg;
    self->base.close_sending = dgram_proto_close_sending;
    return true;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // IDIP_SYSTEM_PROXY_PRIVATE_DGRAM_PROTO_H
