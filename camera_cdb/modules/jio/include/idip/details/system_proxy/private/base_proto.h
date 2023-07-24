#ifndef IDIP_SYSTEM_PROXY_PRIVATE_BASE_PROTO_H
#define IDIP_SYSTEM_PROXY_PRIVATE_BASE_PROTO_H

/// Header-only helper
#include <idip/details/system_proxy/private/fwds.h>
#include <idip/details/system_proxy/private/session.h>
#include <idip/idip_wrappers.h>
#include <idip/idip_log.h>
#include <libwebsockets.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct window_buf_base;
typedef struct window_buf_base window_buf_base_t;

struct window_buf_base {
    bool (*write)(window_buf_base_t * /*self*/, const void * /*data*/, size_t /*size*/,
                 size_t /*ack_size*/, bool /*eos*/);
    void (*peek)(window_buf_base_t * /*self*/, uint8_t ** /*data*/, size_t * /*size*/,
                 bool * /*eos*/);
    void (*consume)(window_buf_base_t * /*self*/, size_t /*size*/, size_t * /*ack_size*/);
};

static inline void schedule_sending(struct lws * wsi)
{
    lws_callback_on_writable(wsi);
}

static inline void enable_receiving(struct lws * wsi)
{
    lws_rx_flow_control(wsi, 1);
}

static inline void disable_receiving(struct lws * wsi)
{
    lws_rx_flow_control(wsi, 0);
}

struct base_proto;
typedef struct base_proto base_proto_t;

struct base_proto {
    bool (*connect)(base_proto_t * /*self*/, session_t * /*sess*/,
        struct lws_context * /*ctx*/, struct lws_vhost * /*vhost*/, const char * /*proto_id*/
    );
    void (*on_lws_event)(base_proto_t * /*self*/, session_t * /*sess*/,
        struct lws * /*wsi*/, enum lws_callback_reasons /*reason*/, void * /*user*/,
        void * /*data*/, size_t /*size*/
    );
    idip_sendmsg_res_t (*sendmsg)(base_proto_t * /*self*/, struct lws * /*wsi*/,
        idip_iovec_t * /*msg_iov*/, size_t /*msg_iovlen*/
    );

    void (*close_sending)(base_proto_t * /*self*/, struct lws * /*wsi*/);
    void (*destroy)(base_proto_t * /*self*/);

    idip_sys_proxy_proto_t type;
    window_buf_base_t * window_buf;
    char * addr;
    uint16_t port;
};

static inline void base_proto_on_lws_event(session_t * sess, struct lws * wsi,
                                           enum lws_callback_reasons reason, void * user,
                                           void * data, size_t size)
{
    IDIP_UNUSED(user);
    IDIP_UNUSED(wsi);

    switch (reason) {
        case LWS_CALLBACK_RAW_RX:
            idip_sys_proxy_session_on_data_received_from_svc(sess, data, size);
            return;

        case LWS_CALLBACK_RAW_WRITEABLE:
            idip_sys_proxy_session_on_can_send_to_svc(sess);
            return;

        case LWS_CALLBACK_WSI_DESTROY:
            // This event occurs when connection is being destroyed
            idip_log_xinfo(__func__, "Connection was finalized. %s",
                idip_sys_proxy_session_get_log_info(sess)
            );
            idip_sys_proxy_session_on_conn_disposed(sess);
            return;
        default:
            return;
    }
}

static inline void base_proto_init(base_proto_t * self, idip_sys_proxy_proto_t type,
                                   window_buf_base_t * window_buf, const char * addr,
                                   uint16_t port)
{
    self->type = type;
    self->window_buf = window_buf;
    self->port = port;
    if (addr)
        self->addr = strdup(addr);
}

static inline void base_proto_destroy(base_proto_t * self)
{
    free(self->addr);
    memset(self, 0, sizeof(*self));
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // IDIP_SYSTEM_PROXY_PRIVATE_BASE_PROTO_H
