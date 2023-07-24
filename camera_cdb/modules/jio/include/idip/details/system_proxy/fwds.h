#ifndef IDIP_SYSTEM_PROXY_FWDS_H
#define IDIP_SYSTEM_PROXY_FWDS_H

struct idip_server;
struct idip_vector;
struct idip_session;
struct msgpack_packer;
struct idip_stream_base;
struct idip_sys_proxy_core;
struct idip_sys_proxy_filter;
struct idip_sys_proxy_session;
struct idip_sys_proxy_svc_info;
struct idip_sys_proxy_core_post;
struct idip_sys_proxy_filter_res;
union idip_sys_proxy_core_post_data;
struct idip_sys_proxy_session_params;

typedef struct idip_server idip_server_t;
typedef struct idip_vector idip_vector_t;
typedef struct idip_session idip_session_t;
typedef struct msgpack_packer msgpack_packer;
typedef struct idip_stream_base idip_stream_base_t;
typedef struct idip_sys_proxy_core idip_sys_proxy_core_t;
typedef struct idip_sys_proxy_filter idip_sys_proxy_filter_t;
typedef struct idip_sys_proxy_session idip_sys_proxy_session_t;
typedef struct idip_sys_proxy_svc_info idip_sys_proxy_svc_info_t;
typedef struct idip_sys_proxy_core_post idip_sys_proxy_core_post_t;
typedef struct idip_sys_proxy_filter_res idip_sys_proxy_filter_res_t;
typedef union idip_sys_proxy_core_post_data idip_sys_proxy_core_post_data_t;
typedef struct idip_sys_proxy_session_params idip_sys_proxy_session_params_t;

typedef enum idip_sys_proxy_proto {
    IDIP_SYS_PROXY_PROTO_NONE,
    IDIP_SYS_PROXY_PROTO_TCP,
    IDIP_SYS_PROXY_PROTO_UDP,
    IDIP_SYS_PROXY_PROTO_UNIX
} idip_sys_proxy_proto_t;

#endif // IDIP_SYSTEM_PROXY_FWDS_H
