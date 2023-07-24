#ifndef IDIP_SYSTEM_PROXY_SESSION_H
#define IDIP_SYSTEM_PROXY_SESSION_H

#include <idip.h>
#include <idip/idip_iovec.h>
#include <idip/details/idip_session.h>
#include <idip/details/idip_stream_base.h>
#include <idip/details/system_proxy/filter.h>
#include <idip/details/system_proxy/session_params.h>
#include <libwebsockets.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

idip_sys_proxy_session_t * idip_sys_proxy_session_create(idip_sys_proxy_core_t * proxy_ctl,
                                                         idip_session_t * cmd_session,
                                                         idip_server_t * server,
                                                         const msgpack_object_kv * data,
                                                         size_t ack_size,
                                                         const idip_sys_proxy_session_params_t * params);

idip_rc_t idip_sys_proxy_session_on_data_from_peer(idip_sys_proxy_session_t * self,
                                                   const void * hlp_raw, size_t hlp_size,
                                                   bool eos);

bool idip_sys_proxy_session_start(idip_sys_proxy_session_t * self, struct lws_context * ctx,
                                  struct lws_vhost * vhost, const char * proto_id);
idip_sys_proxy_session_t * idip_sys_proxy_session_from_wsi(struct lws * wsi);
void idip_sys_proxy_session_on_lws_event(idip_sys_proxy_session_t * self, struct lws * wsi,
                                         enum lws_callback_reasons reason, void * user,
                                         void * data, size_t size);

void idip_sys_proxy_session_on_conn_setup(idip_sys_proxy_session_t * self, struct lws * wsi);
void idip_sys_proxy_session_on_data_received_from_svc(idip_sys_proxy_session_t * self,
                                                      void * data, size_t size);
void idip_sys_proxy_session_on_can_send_to_svc(idip_sys_proxy_session_t * self);
void idip_sys_proxy_session_on_conn_disposed(idip_sys_proxy_session_t * self);
void idip_sys_proxy_session_on_unreg(idip_sys_proxy_session_t * self);
void idip_sys_proxy_session_on_error(idip_sys_proxy_session_t * self, idip_rc_t ec,
                                     const char * reason);
const char * idip_sys_proxy_session_get_log_info(idip_sys_proxy_session_t * self);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // IDIP_SYSTEM_PROXY_SESSION_H
