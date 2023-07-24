#ifndef IDIP_STREAM_SYSTEM_FS_H
#define IDIP_STREAM_SYSTEM_FS_H

#include <idip.h>
#include <idip/details/idip_stream_base.h>
#include <idip/details/idip_thread_queue.h>

struct idip_session;

typedef enum system_fs_command_status_code
{
    IDIP_SYSTEM_FS_STATUS_OK,       ///< system fs command ok
    IDIP_SYSTEM_FS_STATUS_ERROR,    ///< Error on system fs command execution
    _IDIP_SYSTEM_FS_STATUS_MAX      ///< count of system fs command stats codes. Prefer for internal use
}
system_fs_command_status_code_t;

typedef struct idip_system_fs_command {
    char* path;                      ///< PATH on FS system
    char* new_path;                  ///< New PATH on FS system
    method_t method;                 ///< command id
    uint64_t mode;                   ///< permissions in format "01777"
    uint64_t offset;                 ///< File offset
    uint64_t size;                   ///< requested data size
    char* user;                      ///< user name for change own
    char* group;                     ///< group name for change own
    char* data;                      ///< buffer to write
    uint64_t data_size;              ///< buffer size
}
idip_system_fs_command_t;

// for each request_id we create its own reply context.
typedef struct idip_stream_filesystem
{
    // idip_stream_filesystem : public idip_stream_base
    struct idip_stream_base     base;
    void*                       user_data;
} idip_stream_system_fs_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

bool
idip_system_fs_check_method(method_t method);

void
idip_system_fs_command_delete_cmd(idip_system_fs_command_t* cmd);

int
idip_stream_system_fs_init(idip_stream_system_fs_t* self, uint64_t request_id,
                           idip_stream_creation_params_t* init_params,
                           idip_system_fs_command_t* query_args, size_t hlp_size,
                           bool eos);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // IDIP_STREAM_SYSTEM_FS_H
