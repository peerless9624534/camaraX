#ifndef IDIP_STREAM_SYSTEM_OS_EXEC_H_INCLUDED
#define IDIP_STREAM_SYSTEM_OS_EXEC_H_INCLUDED

#include <idip.h>
#include <idip/details/idip_stream_base.h>
#include <idip/idip_vector.h>
#include <sys/types.h>

struct idip_stream_system_os_exec_args
{
    idip_ptr_vector_t* argv;
    void* input;
    size_t input_size;
    int silent;
};
typedef struct idip_stream_system_os_exec_args idip_stream_system_os_exec_args_t;

// Data structure for passing stdin buffers to a running process
typedef struct idip_os_exec_stdin_data
{
    void* input;
    size_t input_size;
}
idip_os_exec_stdin_data_t;

struct idip_stream_system_os_exec
{
    idip_stream_base_t                  base;
    idip_stream_system_os_exec_args_t   query_args;
    pid_t                               child_pid;
    int                                 exit_code; // child exit code
    int                                 child_status; // child done check flag
    int                                 stdin_pipe[2];
    int                                 stdout_pipe[2];
    int                                 stderr_pipe[2];
};
typedef struct idip_stream_system_os_exec idip_stream_system_os_exec_t;

#ifdef __cplusplus
extern "C" {
#endif

/// @note this function movies query_args->argv to the object if success
/// and does query_args->argv = NULL.
int idip_stream_system_os_exec_init(idip_stream_system_os_exec_t *self,
                                    uint64_t request_id,
                                    idip_stream_creation_params_t *init_params,
                                    idip_stream_system_os_exec_args_t *query_args);

void idip_stream_system_os_exec_args_destroy(idip_stream_system_os_exec_args_t* self);

#ifdef __cplusplus
}
#endif

#endif /* IDIP_STREAM_SYSTEM_OS_EXEC_H_INCLUDED */
