/**
 * IDIP server private implementation header file.
 */
#ifndef IDIP_SERVER_P_H
#define IDIP_SERVER_P_H

#include <idip.h>
#include <idip/idip_av_source.h>
#include <idip/details/idip_methods.h>

enum idip_rc_t;

#define IDIP_PROTOCOL_VERSION 2

// Custom information for system.get_info method implementation
typedef struct idip_server_custom_info
{
    /// Vendor name (required)
    char* vendor;
    /// Device model name (required)
    char* model;
    /// Device serial number (required)
    char* serial_number;
    /// Current firmware version (required)
    char* firmware_version;
    /// SSL stuff path (used only with mbedtls and wolfssl)
    char* client_ssl_ca_filepath;
    /// A key string for backward connection (required for back connection only)
    char* dev_key;
    /// An URI string to connect to a cloud service (when the backwad connection is using)
    char* client_uri_str;
    /// A token that using during device attachment (to the cloud) procedure
    char* attachment_token;
    /// A cache of list of interfaces (they're goes from user handler)
    idip_iface_t* ifaces;
    /// Count of items in the cahced @a ifaces list.
    size_t ifaces_count;
    /// Primary hardware (MAC) addresses that can be used for attach-to-cloud procedure
    idip_hwaddr_t primary_hwaddr;
    ///
    bool primary_hwaddr_is_set;
}
idip_server_custom_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct lws;
struct idip_session;
struct idip_stream_base;
struct msgpack_packer;
struct idip_thread_strategy;
typedef struct idip_session idip_session_t;
typedef struct idip_thmap idip_thmap_t;

idip_thmap_t* idip_server_get_impl(idip_server_t* self);

int idip_server_attach_session(struct idip_server* self, idip_session_t* session);

void idip_server_detach_session(struct idip_server* self, idip_session_t* session);

void idip_server_p2t_enqueue(idip_server_t* self,
                             const char* target,
                             const audio_stream_info_t* info,
                             const void* data, size_t size);

void idip_server_pack_custom_info(idip_server_t* self, struct msgpack_packer* pk);

idip_av_source_reader_t* idip_server_av_source_find(idip_server_t* self,
                                                    const char* target,
                                                    const char* profile,
                                                    uint64_t play_start_ts);

int idip_server_initialize_threading_strategy(idip_server_t *self, int method,
                                              struct idip_thread_strategy* strategy);

struct idip_gc* idip_server_get_gc(idip_server_t* self);

void idip_server_append_wop_stat(idip_server_t* self, pthread_t thread,
                                 long long creation_time_ms, size_t writed);

void idip_server_statistics_remove(idip_server_t* self, pthread_t thread);

int idip_server_statistics_setup(idip_server_t* self, const char* uristr);

void idip_server_targets_map_lock(idip_server_t* self);

void idip_server_targets_map_unlock(idip_server_t* self);

bool idip_server_test_cancel(idip_server_t* self);

void idip_server_stat_stream_created(idip_server_t* self);

void idip_server_stat_stream_removed(idip_server_t* self);

void idip_server_stat_register_request(idip_server_t* self,
                                       int method, uint64_t rid);

void idip_server_wscli_disable(idip_server_t* self);


const idip_server_custom_info_t*
idip_server_get_custom_info(const idip_server_t* self);

/// For WebSocket client. Creates a new session and stores it to the
/// @a new_session_ptr if success.
///
/// @param self - pointer to a server instance,
/// @param new_session_ptr - pointer to topinter to store a new idip_session_t
/// @param wsi - pointer to wsi connection object that has WS connection established
/// @return 0 on succes, -1 otherwise
int
idip_server_create_and_attach_new_session(idip_server_t* self,
                                          idip_session_t** new_session_pptr,
                                          struct lws* wsi);

char*
idip_server_get_base64_persistent_storage_data(idip_server_t* self);

int
idip_server_set_base64_persistent_storage_data(idip_server_t* self,
                                               const char* data_base64);

/// Schedule a new connection to a given url. The current backward connection
/// will close if exists.
enum idip_rc_t idip_server_connect_to(idip_server_t* self, const char* urlstr,
                                      const char **puuid);

/// Creates a copy of @a iflist in server custom info.
/// @return 0 if success, -1 on errors
int idip_server_update_ifaces_info(idip_server_t* self,
                                   const idip_iface_t * const iflist,
                                   size_t iflist_size);

int
idip_server_method_wireless_setup(idip_server_t* self,
                                  const idip_ap_connect_parameters_t* args);

int
idip_server_method_wireless_setup_and_wait(idip_server_t* self,
                                           const idip_ap_connect_parameters_t* args);

int
idip_server_method_system_notify(idip_server_t* self,
                                 const idip_system_notify_query_args_t* args);

int
idip_server_method_system_token(idip_server_t* self,
                                idip_parsed_token_t *token);

int idip_server_set_token_for_uplink_connection(idip_server_t* server, const char* token);

int idip_server_filter_notify(idip_server_t* self, const char* notify);

const char* idip_server_method_name(const idip_server_t* self, method_t method);
bool idip_server_method_is_internal(const idip_server_t* self, method_t method);
bool idip_server_method_has_input(const idip_server_t *self, method_t method);
bool idip_server_method_has_args_in_all_hlp(const idip_server_t *self, method_t method);
bool idip_server_method_rx_window_enabled(const idip_server_t *self, method_t method);
size_t idip_server_method_rx_window(const idip_server_t* self, method_t method);


void idip_server_pack_methods_info(idip_server_t* self, struct msgpack_packer* pk);
void idip_server_schedule_bcc_remove(idip_server_t* self, const char* uuid);

struct idip_sys_proxy_core;
struct idip_sys_proxy_core * idip_server_get_sys_proxy_core(idip_server_t* self);

#ifdef __cplusplus
}
#endif

#endif // IDIP_SERVER_P_H
