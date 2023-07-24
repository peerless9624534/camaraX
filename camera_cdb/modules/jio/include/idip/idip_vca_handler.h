#ifndef IDIP_VCA_HANDLER_H
#define IDIP_VCA_HANDLER_H

#include <idip.h>

/// Opaque struct of vca stream that represent idip command
struct idip_stream_vca;
typedef struct idip_stream_vca idip_stream_vca_t;

/// Opaque struct of vca reader. Used to get vca frames from shared memory.
struct idip_vca_reader;
typedef struct idip_vca_reader idip_vca_reader_t;

/// Arguments passed to processor factory
typedef struct idip_vca_processor_args {
    /// Target identifier (name of target)
    char* target;
}
idip_vca_processor_args_t;

/// Pointer for user defined handler of vca stream.
/// Would be called from idip server.
/// User defined code should call reader function in it to get frames.
typedef
idip_status_code_t (*idip_stream_vca_handler_t)(idip_stream_vca_t* stream,
                                                const idip_vca_processor_args_t * proc_args,
                                                idip_vca_reader_t * reader,
                                                void* udata);

/// Frame struct to be processed by vca analytics.
typedef struct idip_vca_frame {
    /// parameterss of video frame
    video_stream_info_t v_info;
    /// Data of video frame.
    /// It's a pointer to shared memory, not owned
    void* data;
    /// Size of video frame in bytes
    size_t dataSize;
    /// timestamp of video frame in milliseconds
    int64_t timestamp_ms;
}
idip_vca_frame_t;

/// Status returned from @ref idip_vca_reader_get_locked_frame
typedef enum idip_vca_reader_status_code {
    /// Failed to get frame, stream is broken.
    IDIP_VCA_READER_STATUS_ERROR,
    /// Got video frame
    IDIP_VCA_READER_STATUS_LOCKED,
    /// Faled to get frame, but stream was closed explicity
    IDIP_VCA_READER_STATUS_EOF,
}
idip_vca_reader_status_code_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/// Get shared memory name used for frames reading, only for debug purposes.
/// returned string valid only inside function @ref idip_stream_vca_handler_t and must be read only
const char* idip_stream_vca_get_channel_name(idip_stream_vca_t* stream);

/// Get locked video frame from shared memory.
/// After using locked frame caller must unlock shared memory with @ref idip_vca_reader_unlock_frame.
/// Second call without unlocking is an error.
/// During the time while frame is locked producer can't write new frame.
/// @param reader pointer to reader passed by idip server to read frames, must be not NULL.
/// @param frame pointer to be filled, it's value is valid only if returned status is @ref IDIP_VCA_READER_STATUS_LOCKED.
/// @return value from the enum @ref idip_vca_reader_status_code_t
idip_vca_reader_status_code_t idip_vca_reader_get_locked_frame(idip_vca_reader_t* reader, idip_vca_frame_t* frame);

/// Unlock last locked frame.
void idip_vca_reader_unlock_frame(idip_vca_reader_t* reader);

/// Set up or replace vca handler for given target.
///
/// @param server - pointer to initialized idip server instance. Cannot be NULL.
/// @param target - target name. If NULL or empty string the function set up
/// a default vca handler.
/// @param handler - pointer to @ref idip_stream_vca_handler_t function
/// @param user_data - pointer that will be passed to @a handler.
/// @return 0 if success, -1 if errors.
///
int idip_server_setup_vca_feature(idip_server_t* server,
                                  const char* target,
                                  idip_stream_vca_handler_t handler,
                                  void *user_data);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // IDIP_VCA_HANDLER_H
