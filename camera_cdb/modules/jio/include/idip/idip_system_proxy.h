#ifndef IDIP_SYSTEM_PROXY_H
#define IDIP_SYSTEM_PROXY_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/// Get receive window for system.proxy command
size_t idip_sys_proxy_get_rx_window();

typedef struct idip_sys_proxy_svc_info {
    char * name;
    char * url;
} idip_sys_proxy_svc_info_t;

/// Clear fields of the idip_sys_proxy_svc_info object
void idip_sys_proxy_svc_info_clear(idip_sys_proxy_svc_info_t * self);

struct idip_server;

/// Add or update service info
/// \return 0 in case of success
int idip_sys_proxy_set_service_info(struct idip_server * server,
                                     const idip_sys_proxy_svc_info_t * svc_info);

/// Remove service info
void idip_sys_proxy_remove_service_info(struct idip_server * server,
                                        const char * svc_name);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // IDIP_SYSTEM_PROXY_H
