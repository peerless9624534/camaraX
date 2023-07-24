/**
 * IDIP server public API header file.
 */
#ifndef IDIP_SERVER_H_INCLUDED
#define IDIP_SERVER_H_INCLUDED

#include <idip/idip_enum.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

//----- idip server instance --------------------------------------------------------

/// Opaque data structure that represents IDIP server instance.
struct idip_server;
/// IDIP server instance type
typedef struct idip_server idip_server_t;

struct idip_server_impl;
typedef struct idip_server_impl idip_server_impl_t;

/// Idip server global configuration
typedef struct idip_server_conf
{
    /// A HTTP(S) addres to connect to. For agentless scheme.
    /// If enable_local_listener = 0 [requiried] else [optional, default = NULL]
    /// @example Example values:
    ///     "http://example.com/ivideon" - notencrypted, port 80
    ///     "http://example.com:1234/ivideon" - notencrypted, port 1234
    ///     "https://example.com/ivideon" - encrypted, port 443
    ///     "https://example.com:1234/ivideon" - encreyped, port 1234
    /// @note Only "http" and "https" urls are valid here.
    char* cloud_enpoint_url;
    /// A special Device Key string from Ivideon to identify on a cloud agent.
    /// If enable_local_listener = 0 [requiried] else [optional, default = NULL].
    char* dev_key;

    /// Vendor name. [optional, default = NULL]
    char* vendor;
    /// Device model name. [optional, default = NULL]
    char* model;
    /// Device serial number. [optional, default = NULL]
    char* serial_number;
    /// Current firmware version. [optional, default = NULL]
    char* firmware_version;
    /// Hardware address of device. [optional, default = NULL]
    char* mac;

    /// Path to CA (http/wss cloud server certificate) file.
    /// @note Only for Mbedtls and WolfSSL. Ignored for OpenSSL backend.
    char* cloud_ssl_ca_filepath;

    /// Option flags
    /// Disable ipv6. [optional, default = 0]
    unsigned disable_ipv6;
    /// Explicitly enables local IDIP server listener. [optional, default = 0]
    /// @note The setting enables to use @a host_or_path and @a port values.
    unsigned enable_local_listener;

    /// Host name or path in local file system.
    /// If enable_local_listener = 1 [requiried] else [optional, default = NULL].
    char* host_or_path;
    /// Port number. [optional, default = 0]
    /// Set to 0 if local path is used.
    int port;

    /// Keep-alive interval in milliseconds.
    /// [optional, default = IDIP_KEEP_ALIVE_AUTO (0)].
    unsigned keep_alive_ms;
}
idip_server_conf_t;

enum {
    /// The implementation of the idip_server_t will choose the timeout value.
    IDIP_KEEP_ALIVE_AUTO  = 0
};

/// Face detector event parameters
struct idip_face_detector_event_params {
    int track_begin;
    int track_end;
    struct best_shot_tag {
        uint64_t ts;
        const void* data;
        size_t data_size;
        unsigned int width;
        unsigned int height;
        struct position_tag {
            unsigned int x;
            unsigned int y;
            unsigned int width;
            unsigned int height;
        } position;
    } best_shot;
};
/// Face detector event parameters
typedef struct idip_face_detector_event_params idip_face_detector_event_params_t;

/// User verification callback type
typedef int (*idip_auth_user_t)(void* uder_data, const char* user, const char* password);

#ifndef IDIP_MTU_MIN
#define IDIP_MTU_MIN 1024
#endif /* IDIP_MTU_MIN */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/// Create new instance of IDIP server. The server is stopped but it is ready
/// for capabilities setup.
/// @param conf - pointer to a server configuration structure
/// (@a idip_server_conf_t)
/// @return a new instance of @a idip_server_t if success or NULL if failure.
idip_server_t*
idip_server_new(const idip_server_conf_t* conf);

/// Delete instance of @a idip_server_t which was created by
/// @ref idip_server_new() method.
/// @param server - pointer to idip server instance. NULL value is ignored.
void
idip_server_delete(idip_server_t* server);

/// Start a configured idip server instance. The server is opening connections
/// and run its own loop. This method replaces a part of @ref idip_server_start()
/// function.
/// @param server - pointer to a idip server instance which was created by
/// @ref idip_server_new() functions.
/// @return
idip_status_code_t
idip_server_run(idip_server_t* server);

/// Stop IDIP server instance and wait for server's complete termination
///
/// @param self - pointer to idip server's instance
idip_status_code_t
idip_server_stop(idip_server_t* self);

/// Return keep alive time value given on initialization
uint64_t idip_server_get_keep_alive_ms(idip_server_t* self);

bool idip_server_is_inside_network_thread(idip_server_t* self);

void idip_server_set_app_stop_notify(idip_server_t* self, void (*handler)(void));

bool idip_server_ipv6_disabled(const idip_server_t* self);
bool idip_server_listener_enabled(const idip_server_t* self);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IDIP_SERVER_H_INCLUDED */

