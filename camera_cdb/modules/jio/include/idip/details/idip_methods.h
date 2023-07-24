#ifndef IDIP_METHODS_H_INCLUDED
#define IDIP_METHODS_H_INCLUDED

#include <msgpack/object.h>
#include <idip/idip_enum.h>

typedef enum method_t {
    METHOD_SYSTEM_INIT,
    METHOD_SYSTEM_GET_INFO,
    METHOD_SYSTEM_CANCEL,
    METHOD_SYSTEM_PING,
    METHOD_SYSTEM_NOTIFY,
    METHOD_SYSTEM_UPDATE_VERSION,
    METHOD_SYSTEM_PASSWD,
    METHOD_SYSTEM_GET_USERS,
    METHOD_SYSTEM_LOG_SETUP,
    METHOD_SYSTEM_CONNECT_TO,
    METHOD_SYSTEM_GET_INTERFACES,
    METHOD_SYSTEM_TOKEN,
    METHOD_VIDEO_GET,
    METHOD_SNAPSHOT,
    METHOD_AUDIO_PUT,
    METHOD_EVENTS_SUBSCRIBE,
    METHOD_ARCHIVE_LIST,
    METHOD_ARCHIVE_GET,
    METHOD_ARCHIVE_DAYS,
    METHOD_CONFIG_GET,
    METHOD_CONFIG_SET,

    METHOD_PERSISTENT_DATA_GET,
    METHOD_PERSISTENT_DATA_SET,
    METHOD_WIRELESS_SCAN,
    METHOD_WIRELESS_SETUP,
    METHOD_PTZ_CONTROL,

    METHOD_SYSTEM_STAT_SETUP,
    METHOD_SYSTEM_STAT,

    METHOD_VCA_PROCESSOR_RUN,

    METHOD_SYSTEM_FS_LIST,
    METHOD_SYSTEM_FS_STAT,
    METHOD_SYSTEM_FS_RM,
    METHOD_SYSTEM_FS_MV,
    METHOD_SYSTEM_FS_MKDIR,
    METHOD_SYSTEM_FS_CHMOD,
    METHOD_SYSTEM_FS_CHOWN,
    METHOD_SYSTEM_FS_READ,
    METHOD_SYSTEM_FS_WRITE,
    METHOD_SYSTEM_OS_EXEC,
    METHOD_SYSTEM_PROXY,

    METHOD_UNKNOWN
} method_t;

/// Method run policy
typedef enum idip_run_policy_t {
    IDIP_POLICY_NONE, ///< for inline implemented methods
    IDIP_POLICY_RUN_IN_CURRENT_THREAD,
    IDIP_POLICY_RUN_IN_SEPARATE_THREAD,
    IDIP_POLICY_RUN_IN_QUEUE, ///< run in a queue of a dedicated thread
} idip_run_policy_t;

/// Method trait flags
typedef enum idip_mt_flag_t {
    IDIP_MTF_NO_FLAGS = 0, ///< no flags specified for the method
                           ///< Then it doesn't support windowing and multiple requests.
    IDIP_MTF_IS_INTERNAL = (1 << 0), ///< method is implemented in libidip
                                     ///< and cannot be overrided by user
    IDIP_MTF_RX_WIN = (1 << 1), ///< method allows rx window feature
    IDIP_MTF_INPUT  = (1 << 2), ///< method allows multiple input HLPs
    IDIP_MTF_ARGS_HLP = (1 << 3), ///< method with multiple input requires
                                  ///< looking for "args" of each incoming HLP.
                                  ///< Instead the handler will parse the first HLP only.
} idip_mt_flag_t;

/// Method traits
typedef struct idip_method_traits_t {
    method_t            method;
    const char*         method_name;
    idip_run_policy_t   run_policy;
    const char*         queue_name;
    size_t              rx_window;
    uint32_t            flags;
} idip_method_traits_t;

struct idip_vector; // forward

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern const char* const kMETHOD_SYSTEM_INIT_NAME;
extern const char* const kMETHOD_SYSTEM_GET_INFO_NAME;
extern const char* const kMETHOD_SYSTEM_CANCEL_NAME;
extern const char* const kMETHOD_SYSTEM_PING_NAME;
extern const char* const kMETHOD_SYSTEM_NOTIFY_NAME;
extern const char* const kMETHOD_SYSTEM_UPDATE_VERSION_NAME;
extern const char* const kMETHOD_SYSTEM_PASSWD_NAME;
extern const char* const kMETHOD_SYSTEM_GET_USERS_NAME;
extern const char* const kMETHOD_SYSTEM_STAT_SETUP_NAME;
extern const char* const kMETHOD_SYSTEM_STAT_NAME;
extern const char* const kMETHOD_SYSTEM_LOG_SETUP_NAME;
extern const char* const kMETHOD_SYSTEM_TOKEN_NAME;
extern const char* const kMETHOD_VIDEO_GET_NAME;
extern const char* const kMETHOD_SNAPSHOT_NAME;
extern const char* const kMETHOD_AUDIO_PUT_NAME;
extern const char* const kMETHOD_EVENTS_SUBSCRIBE_NAME;
extern const char* const kMETHOD_ARCHIVE_LIST_NAME;
extern const char* const kMETHOD_ARCHIVE_GET_NAME;
extern const char* const kMETHOD_ARCHIVE_DAYS_NAME;
extern const char* const kMETHOD_CONFIG_GET_NAME;
extern const char* const kMETHOD_CONFIG_SET_NAME;
extern const char* const kMETHOD_PERSISTENT_DATA_GET_NAME;
extern const char* const kMETHOD_PERSISTENT_DATA_SET_NAME;
extern const char* const kMETHOD_WIRELESS_SCAN_NAME;
extern const char* const kMETHOD_WIRELESS_SETUP_NAME;
extern const char* const kMETHOD_PTZ_CONTROL_NAME;
extern const char* const kMETHOD_VCA_PROCESSOR_RUN_NAME;
extern const char* const kMETHOD_SYSTEM_GET_INTERFACES_NAME;
extern const char* const kMETHOD_SYSTEM_FS_LIST_NAME;
extern const char* const kMETHOD_SYSTEM_FS_STAT_NAME;
extern const char* const kMETHOD_SYSTEM_FS_RM_NAME;
extern const char* const kMETHOD_SYSTEM_FS_MV_NAME;
extern const char* const kMETHOD_SYSTEM_FS_MKDIR_NAME;
extern const char* const kMETHOD_SYSTEM_FS_CHMOD_NAME;
extern const char* const kMETHOD_SYSTEM_FS_CHOWN_NAME;
extern const char* const kMETHOD_SYSTEM_FS_READ_NAME;
extern const char* const kMETHOD_SYSTEM_FS_WRITE_NAME;
extern const char* const kMETHOD_SYSTEM_OS_EXEC_NAME;
extern const char* const kMETHOD_PROXY_NAME;

// internal dedicated thread names
extern const char* const kUPDTH;
extern const char* const kARCHTH;
extern const char* const kCFGTH;
extern const char* const kPTZTH;

const char* method_str(enum method_t m, const char* deflt);

enum method_t mpk_detect_method(const msgpack_object o);

/**
 * Creates a vector of @ref idip_method_properites_t that contains a
 * database of method properties: db[ method_as_index ] -> { properties }
 * @return an instance of @ref idip_vector_t <idip_method_properites_t> or NULL.
 */
struct idip_vector* idip_methods_db_create();

const char*         idip_method_method_name(const struct idip_vector* db, method_t method);
idip_run_policy_t   idip_method_run_policy(const struct idip_vector* db, method_t method);
size_t              idip_method_rx_window(const struct idip_vector* db, method_t method);
bool                idip_method_rx_window_enabled(const struct idip_vector* db, method_t method);
bool                idip_method_is_internal(const struct idip_vector* db, method_t method);
bool                idip_method_has_input(const struct idip_vector* db, method_t method);
bool                idip_method_has_args_in_all_hlp(const struct idip_vector* db, method_t method);
const char*         idip_method_queue(const struct idip_vector *db, method_t method);

void idip_method_set_rx_window(struct idip_vector* db, method_t method, size_t rx_window);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IDIP_METHODS_H_INCLUDED */

