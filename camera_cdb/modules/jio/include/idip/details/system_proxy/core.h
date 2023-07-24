#ifndef IDIP_SYSTEM_PROXY_CORE_H
#define IDIP_SYSTEM_PROXY_CORE_H

#include <idip/idip_rc.h>
#include <idip/details/idip_session.h>
#include <idip/details/system_proxy/fwds.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/// Creates proxy which connects a remote client (peer) of idip-server to the
/// service accessible from idip-server side
idip_sys_proxy_core_t * idip_sys_proxy_core_create(struct idip_vector * methods_db);

/// Destroys proxy core
void idip_sys_proxy_core_destroy(idip_sys_proxy_core_t ** self);

/// Starts proxy session
idip_rc_t idip_sys_proxy_core_start_session(idip_sys_proxy_core_t * self,
                                            idip_session_t * cmd_session,
                                            idip_server_t * server,
                                            const msgpack_object_kv * data,
                                            size_t ack_size,
                                            const idip_sys_proxy_session_params_t * params);

/// Either creates, attaches to proxy_ctl and starts a new proxy session or passes
/// data to an existing proxy session
idip_rc_t idip_sys_proxy_core_on_data_from_peer(idip_sys_proxy_core_t * self,
                                                idip_session_t * cmd_session,
                                                idip_server_t * server,
                                                idip_stream_base_t * stream,
                                                uint64_t request_id,
                                                const msgpack_object_kv * req_args,
                                                size_t tx_window, void * hlp_raw,
                                                size_t hlp_size,
                                                bool eos);

/// Dump services info into the pk
void idip_sys_proxy_core_dump_services(idip_sys_proxy_core_t * self, msgpack_packer * pk);


/// Add or update service info
bool idip_sys_proxy_core_set_service_info(idip_sys_proxy_core_t * self,
                                          const idip_sys_proxy_svc_info_t * svc_info);

/// Remove service info
void idip_sys_proxy_core_remove_service_info(idip_sys_proxy_core_t * self,
                                             const char * svc_name);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // IDIP_SYSTEM_PROXY_CORE_H
