#ifndef IDIP_SYSTEM_PROXY_PRIVATE_MISC_H
#define IDIP_SYSTEM_PROXY_PRIVATE_MISC_H

/// Header-only helper
#include <idip/details/system_proxy/private/fwds.h>
#include <idip/idip_wrappers.h>
#include <idip/details/message_pack_helpers.h>

#define MAX_UNSIGNED_SIZE_OF(type) ((type) ~ (type) 0)
static const uint16_t DEFAULT_MAX_DGRAM_SIZE = 2048;

/* sync_state */
typedef struct sync_state {
    uint32_t s;
    pthread_mutex_t mutex;
} sync_state_t;

static inline void sync_state_init(sync_state_t * self, uint32_t value)
{
    idip_mutex_init(&self->mutex);
    self->s = value;
}

static inline void sync_state_destroy(sync_state_t * self)
{
    idip_mutex_destroy(&self->mutex);
}

static inline uint32_t sync_state_get(sync_state_t * self)
{
    idip_mutex_lock(&self->mutex);
    uint32_t res = self->s;
    idip_mutex_unlock(&self->mutex);
    return res;
}

static inline uint32_t sync_state_set(sync_state_t * self, uint32_t value)
{
    idip_mutex_lock(&self->mutex);
    uint32_t prev = self->s;
    self->s = value;
    idip_mutex_unlock(&self->mutex);
    return prev;
}

static inline uint32_t sync_state_cas(sync_state_t * self, uint32_t expected, uint32_t desired)
{
    idip_mutex_lock(&self->mutex);
    uint32_t prev_or_curr = self->s;
    if (self->s == expected)
        self->s = desired;
    idip_mutex_unlock(&self->mutex);
    return prev_or_curr;
}

/* utils */
static inline size_t get_iovec_total_size(idip_iovec_t * msg_iov, size_t msg_iovlen)
{
    size_t res = 0;
    for (size_t i = 0; i < msg_iovlen; ++i)
        res += msg_iov[i].iov_len;

    return res;
}

static inline bool extract_proto(const msgpack_object_kv * args, proto_t * proto)
{
    char proto_name[10];
    size_t len = 0;
    *proto = IDIP_SYS_PROXY_PROTO_NONE;
    memset(&proto_name, 0, sizeof(proto_name));
    if (0 != mpk_get_string(args, "proto", proto_name, sizeof(proto_name) - 1, &len))
        return false;

    if (strncmp("tcp", proto_name, len) == 0) {
        *proto = IDIP_SYS_PROXY_PROTO_TCP;
        return true;
    }

    if (strncmp("udp", proto_name, len) == 0) {
        *proto = IDIP_SYS_PROXY_PROTO_UDP;
        return true;
    }

    if (strncmp("unix", proto_name, len) == 0) {
        *proto = IDIP_SYS_PROXY_PROTO_UNIX;
        return true;
    }

    return false;
}

static inline bool extract_port(const msgpack_object_kv * args, proto_t proto, uint16_t * port)
{
    if (proto == IDIP_SYS_PROXY_PROTO_UNIX)
        return true;


    uint64_t tmp = 0;
    if (mpk_get_uint(args, "port", &tmp) != 0)
        return false;

    if (tmp > MAX_UNSIGNED_SIZE_OF(uint16_t))
        return false;

    *port = (uint16_t) tmp;
    return true;
}

static inline void extract_max_dgram_size(const msgpack_object_kv * args, uint32_t * max_dgram_size)
{
    uint64_t tmp = 0;
    if (mpk_get_uint(args, "max_dgram_size", &tmp) != 0
            || tmp > MAX_UNSIGNED_SIZE_OF(uint32_t)) {
        *max_dgram_size = DEFAULT_MAX_DGRAM_SIZE;
        return;
    }

    *max_dgram_size = (uint32_t) tmp;
}

#endif // IDIP_SYSTEM_PROXY_PRIVATE_MISC_H
