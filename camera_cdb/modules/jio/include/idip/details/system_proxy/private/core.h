#ifndef IDIP_SYSTEM_PROXY_PRIVATE_CORE_H
#define IDIP_SYSTEM_PROXY_PRIVATE_CORE_H

#include <idip/details/system_proxy/core.h>
#include <idip/details/system_proxy/fwds.h>
#include <idip/idip_utils.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// Well aligned storage for posts. Can hold 3 pointers. The minimum size is
// explicitly set to 24 bytes
union idip_sys_proxy_core_post_data {
    void * ptr[3];
    uint8_t bytes[24];
};

#define IDIP_SYS_PROXY_CORE_POST_DATA_CHECK_FIT(type) \
    IDIP_STATIC_ASSERT(sizeof(type) <= sizeof(idip_sys_proxy_core_post_data_t), "doesn't fit");

typedef struct idip_sys_proxy_core_post {
    void (*handle)(idip_sys_proxy_core_post_data_t * /*data*/);
    void (*dispose)(idip_sys_proxy_core_post_data_t * /*data*/);
    idip_sys_proxy_core_post_data_t data;
} idip_sys_proxy_core_post_t;

bool idip_sys_proxy_core_post(idip_sys_proxy_core_t * self, idip_sys_proxy_core_post_t * post);
void idip_sys_proxy_core_post_dispose(idip_sys_proxy_core_post_t * self);
bool idip_sys_proxy_core_post_session_unreg(idip_sys_proxy_core_t * self, idip_sys_proxy_session_t * session);

idip_rc_t idip_sys_proxy_core_start_session(idip_sys_proxy_core_t * self,
                                            idip_session_t * cmd_session,
                                            idip_server_t * server,
                                            const msgpack_object_kv * data,
                                            size_t ack_size,
                                            const idip_sys_proxy_session_params_t * params);

idip_rc_t idip_sys_proxy_core_pass_session_data(idip_sys_proxy_core_t * self,
                                                idip_stream_base_t * stream,
                                                const void * hlp_raw, size_t hlp_size,
                                                bool eos);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // IDIP_SYSTEM_PROXY_PRIVATE_CORE_H
