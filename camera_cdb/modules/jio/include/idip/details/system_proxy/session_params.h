#ifndef IDIP_SYSTEM_PROXY_SESSION_PARAMS_H
#define IDIP_SYSTEM_PROXY_SESSION_PARAMS_H

#include <idip/idip_iovec.h>
#include <idip/details/system_proxy/fwds.h>
#include <idip/details/system_proxy/filter.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct idip_sys_proxy_session_params {
    char * addr;                    /// address to connect to
    idip_sys_proxy_proto_t proto;   /// connection protocol (see enum values)
    uint16_t port;                  /// port to connect to (makes sense only for tcp and udp protocols)
    char * app_proto;               /// (optional) application level protocol (f.e.: "rtsp")
    size_t tx_window;               /// peer (client) window size in bytes
    uint64_t request_id;            /// idip request id
    idip_sys_proxy_filter_t filter; /// (optional) filter for sending to/receiving from the service data

    // UDP specific
    uint32_t max_dgram_size;
} idip_sys_proxy_session_params_t;

void idip_sys_proxy_session_params_init(idip_sys_proxy_session_params_t * self,
                                        size_t tx_window, uint64_t request_id,
                                        const msgpack_object_kv * args);

void idip_sys_proxy_session_params_clear(idip_sys_proxy_session_params_t * self);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // IDIP_SYSTEM_PROXY_SESSION_H
