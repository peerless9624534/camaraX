#ifndef AVPKT_STREAM_SOURCE_H
#define AVPKT_STREAM_SOURCE_H

#include "avpkt_source.h"

#include <idip.h>
#include <idip/idip_av_source.h>

#include <stdint.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief @a avpkt_stream_t wrapper that reads file and puts packets by calling @ref idip_av_source_push().
 */
typedef struct avpkt_stream_source
{
    idip_server_t* server;           ///< A non-owning pointer to an @ref idip_server_t object to associate with.

    pthread_mutex_t source_mutex;    ///< A @a source reading/writing mutex.
    avpkt_source_t* source;          ///< An owning pointer to an @ref avpkt_source_t object that reads an input file.

    video_stream_info_t v_info;      ///< Source video info.
    audio_stream_info_t a_info;      ///< Source audio info.
    idip_av_limits_t buffer_limits;  ///< Limits for frame buffers

    pthread_mutex_t av_sink_mutex;   ///< An audio/video source handle mutex.
    idip_av_source_t* av_sink;       ///< An non-owning pointer to a live streamer (sink) interface.
                                     /// Don't confuse the name. For an idip_server_t instance it is a frame source,
                                     /// but it's a sink for the ffmpeg-based reader.

    pthread_t thread; ///< A stream source thread id.
    bool async; ///< A flag indicating that sending was run in a separate thread

    uint8_t stopping;               ///< Atomic, indicates whether stopping is triggered.
} avpkt_stream_source_t;

/**
 * @brief Initializes an @a avpkt_stream_source_t object with a server context and a filename to stream from.
 * @param self An @a avpkt_stream_source_t object to initialize.
 *             Must not be @a NULL.
 * @param filename A pointer to a filename to stream from.
 *                 Must not be @a NULL.
 *                 Will be copied so it's safe to free after use.
 * @param repeat_forever Repeat the file from the beginning.
 * @return 0 if successful,
 *         @a ENOMEM if inner allocations failed,
 *         @a ENOENT if the @a filename path doesn't exist,
 *         @a EACCESS if access to the @a filename path denied.
 */
// 使用服务器上下文和要从中流式传输的文件名初始化avpkt_stream_source_t对象。
// 参数:
// self–要初始化的avpkt_stream_source_t对象。不得为NULL。
// filename–指向要从中流式传输的文件名的指针。不得为NULL。将被复制，以便在使用后安全释放。
// repeat_forever–从头开始重复该文件。
// 返回:
// 如果成功，返回0；如果内部分配失败，返回ENOMEM；如果文件名路径不存在，返回ENOENT；如果拒绝访问文件名路径，返回EACCESS。
int avpkt_stream_source_init(avpkt_stream_source_t* self, const char* filename,
                             bool repeat_forever, bool async);

/**
 * @brief Destroys an @a avpkt_stream_source_t object.
 * @param self A pointer to an initialized @a avpkt_stream_source_t object to destroy.
 *             Must not be @a NULL.
 *             Passing an uninitialized object results in undefined behavior.
 */
// 销毁avpkt_stream_source_t对象。
// 参数:
// self–指向要销毁的已初始化avpkt_stream_source_t对象的指针。不得为NULL。传递未初始化的对象会导致未定义的行为。
void avpkt_stream_source_destroy(avpkt_stream_source_t* self);

/**
 * @brief An @c avpkt_stream_source_t work loop body.
 *        Fetches packets from a @c avpkt_source_t object that was created upon
 *        initialization and put in an @c idip_stream_av_t handle.
 * @param[in] avpkt_av_stream_source A pointer to an @c avpkt_av_stream_source_t object
 *                                holding both @c avpkt_source_t objects
 *                                and packet generation state.
 * @param[in] stream_av A pointer to an @c idip_stream_av_t object.
 * @return An @c idip_status_code_t value casted to @c void* indicating error:
 *         @c IDIP_INVALID_ARGUMENTS if invalid arguments usage datected
 *         @c IDIP_GENERIC_ERROR in case of recursive locking attempt detected or packet
 *         fetching has failed.
 */
idip_status_code_t avpkt_stream_source_stream_play_loop(avpkt_stream_source_t * avpkt_stream_source, idip_stream_av_t *stream_av);

/**
 * @brief Sets an @c avpkt_stream_source_t object's @a av_source handle.
 * @param[in] self A pointer to an @c avpkt_stream_source_t object to register.
 *                 Must not be @c NULL.
 * @param[in] server A pointer to an @c idip_server_t object where av_source is registered.
 *                   Must not be @c NULL.
 * @param[in] source A pointer to an @c idip_av_source_t object.
 *                   Must not be @c NULL.
 * @return @c 0 if successful,
 *         @c EINVAL if invalid arguents are passed.
 */
// 设置avpkt_stream_source_t对象的av_source句柄。
// 参数:
// self–指向要注册的avpkt_stream_source_t对象的指针。不得为NULL。
// server–指向注册了av_source的idip_server_t对象的指针。不得为NULL。
// source–指向idip_av_source_t对象的指针。不得为NULL。
// 返回:
// 如果成功，则为0；如果传递了无效的参数，则为EINVAL。
int avpkt_stream_source_set(avpkt_stream_source_t* self, idip_server_t* server, idip_av_source_t* av_source);

/**
 * @brief Releases an @c avpkt_stream_source_t object's @a av_source handle.
 * @param[in] self A pointer to an @c avpkt_stream_source_t object to release.
 *                 Must not be @c NULL.
 * @param[in] server A pointer to an @c idip_server_t object where av_source is registered.
 *                   Must not be @c NULL.
 * @param[in] source A pointer to an @c idip_av_source_t object.
 *                   Must not be @c NULL.
 * @return @c 0 if successful,
 *         @c EINVAL if invalid arguents are passed.
 */
int avpkt_stream_source_release(avpkt_stream_source_t* self, idip_server_t* server, idip_av_source_t* av_source);

/**
 * @brief Emulates reregistering an @c avpkt_stream_source_t object after reconfiguration.
 * @param[in] self A pointer to an @c avpkt_stream_source_t object to reconfigure.
 *                 Must not be @c NULL.
 * @return @c 0 if successful,
 *         @c EINVAL if @a self is @c NULL.
 */
int avpkt_stream_source_reconfigure(avpkt_stream_source_t* self);

/**
 * @brief Request to generate the next video frame as a key one.
 * @param self A pointer to @c avpkt_stream_source_t object.
 *             Must not be @c NULL.
 * @return @c 0 if successful,
 *         @c EINVAL if @a self is @c NULL,
 *         @c EFAULT on stream error.
 */
int avpkt_stream_source_force_video_key_frame(avpkt_stream_source_t * self);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // AVPKT_STREAM_SOURCE_H
