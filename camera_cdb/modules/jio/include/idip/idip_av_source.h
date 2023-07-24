#ifndef IDIP_AV_SOURCE_H
#define IDIP_AV_SOURCE_H

#include <idip.h>
#include <idip/idip_av_packet.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct idip_byte_storage idip_byte_storage_t;
struct idip_av_buffer;
struct idip_av_source;

/// An opaque @c idip_av_source_t data reader
typedef struct idip_av_source_reader idip_av_source_reader_t;

/**
 * @brief Creates an @c idip_av_source_t object.
 * @param[out] self A pointer to an @c idip_av_source_t pointer
 *                  where the address of the new object will be put.
 *                  Must not be @c NULL.
 * @param[in] params Initialization parameters.
 *                   Must not be @c NULL.
 *                   Following fields in @a params are mandatory:
 *                   - @a target_name - name of the target device;
 *                   - @a profile_name - name of the target profile;
 *                   - @a v_info.codec must not be @ref VIDEO_CODEC_NONE;
 *                   - @a packet_count - number of packets to keep in buffer
 *                                       (GOP size plus audio packet number for this GOP is recommended);
 *                   - @a buffer_capacity - number of bytes for packet payloads
 *                                          (number of bytes corresponding the @a packet_count total payload
 *                                          is recommended).
 *                   If @a a_info.codec_type is @ref AUDIO_CODEC_NONE, then no audio is avalable.
 *                   @a packet_count is recommended to be equal to at least the GOP size.
 *                   @a buffer_capacity is recommended to be set the value that could fit in the GOP
 *                   (if @a packet_count equals the GOP size).
 * @return @c 0 if successful,
 *         @c EINVAL if invalid arguments are passed,
 *         @c ENOMEM if allocations failed.
 *
 * On error the @a self argument is intact.
 */
int idip_av_source_create(idip_av_source_t** self, const idip_av_source_params_t* params);

idip_av_source_t* idip_av_source_new(const idip_av_source_params_t* params);

idip_av_source_t* idip_av_source_add_ref(idip_av_source_t* self);

void idip_av_source_unref(idip_av_source_t* self);

/**
 * @brief Updates an @c idip_av_source_t object parameters.
 * @param[in,out] self A pointer to an @c idip_av_source_t pointer to update.
 *                     Must not be @c NULL.
 * @param v_info Video stream specs.
 *               <tt>v_info.codec</tt> must not be @ref VIDEO_CODEC_NONE.
 * @param a_info Audio stream specs.
 *               If @a a_info.codec_type is @ref AUDIO_CODEC_NONE, no audio is avalable.
 * @param packet_count Number of packets to keep.
 *                     Could be reached before the @a buffer_capacity.
 *                     Must not be @c 0.
 *                     It's recommended to set it to the GOP size.
 * @param buffer_capacity Total number of bytes to keep.
 *                        Could be reached before the @a packet_count.
 *                        Must not be @c 0.
 *                        It's recommended to set it to the GOP byte size.
 * @return @c 0 if successful,
 *         @c EINVAL if invalid arguments are passed,
 *         @c ENOMEM if allocations failed.
 */
int idip_av_source_update(idip_av_source_t* self, video_stream_info_t v_info,
                          audio_stream_info_t a_info, idip_av_limits_t limits);

/**
 * @brief Stops an @c idip_av_source_t object byt setting the EOS state and waiting for all readers to quit.
 * @param self A pointer to an @c idip_av_source_t object to stop.
 *             Must not be @c NULL.
 */
void idip_av_source_stop(idip_av_source_t* self);

/**
 * @brief Reads an @c idip_av_source_t object video specs.
 * @param[in] self A pointer an @c idip_av_source_t object to read video specs from.
 *                 Must not be @c NULL.
 * @param[out] v_info A pointer to an @c video_stream_info_t structure to store the video specs to.
 *                    Must not be @c NULL.
 * @return @c 0 if susccessful,
 *         @c EINVAL if @a self or @a v_info is @c NULL.
 */
int idip_av_source_video_info(const idip_av_source_reader_t* self, video_stream_info_t* v_info);

/**
 * @brief Reads an @c idip_av_source_t object audio specs.
 * @param[in] self A pointer an @c idip_av_source_t object to read the audio specs from.
 *                 Must not be @c NULL.
 * @param[out] a_info A pointer to an @c audio_stream_info_t structure to store the audio specs to.
 *                    Must not be @c NULL.
 * @return @c 0 if susccessful,
 *         @c EINVAL if @a self or @a a_info is @c NULL.
 */
int idip_av_source_audio_info(const idip_av_source_reader_t* self, audio_stream_info_t* a_info);

/**
 * @brief Creates an initialized @c idip_av_source_reader_t object associated
 *        with an @c idip_av_source_t object.
 * @param[out] self A pointer to an unitialized @c idip_av_source_reader_t pointer
 *                  to assign the a new @c idip_av_source_reader_t object to.
 *                  Must not be @c NULL.
 * @param[in] source A pointer to an @c idip_av_source_t object to associate with.
 *                   Must not be @c NULL.
 * @return @c 0 if successful,
 *         @c EINVAL if @a new_reader or @a source id @c NULL,
 *         @c EBUSY if @a source is in EOS state.
 */
int idip_av_source_reader_new(idip_av_source_reader_t** self,
                              idip_av_source_t* source,
                              uint64_t play_start_ms);

/**
 * @brief Destroys and frees an initialized @c idip_av_source_reader_t object.
 * @param self A pointer to an @c idip_av_source_reader_t object to destroy and free.
 *             Must not be @c NULL.
 * @return @c 0 if successful,
 *         @c EINVAL if @a self is @c NULL.
 */
int idip_av_source_reader_delete(idip_av_source_reader_t* self);

//------------------------------------------------------------------------------
typedef enum idip_avs_read_result {
    IDIP_AVS_READ_OK = 0,       // all ok, frame was got
    IDIP_AVS_READ_TIMEOUT,      // no frame, timeout is reaced
    IDIP_AVS_READ_EOS,          // no frame, end-of-stream detected
    IDIP_AVS_READ_INVALIDATED,  // no frame, iterator was invalidated, try again with a new one
    IDIP_AVS_READ_EMPTY,        // no frame, iterator is at end
    IDIP_AVS_READ_ERROR,        // no frame, some error (details in log)
} idip_avs_read_result_t;

/**
 * @brief Makes an @c idip_av_source_reader_t object to fetch next packet from the associated source.
 * @param reader A pointer to an @c idip_av_source_t object to read a packet from.
 *               Must not be @c NULL.
 * @param[out] header A pointer an @c idip_av_packet_t to fill the packet header data to.
 *                    Must not be @ NULL.
 * @param[out] payload_buffer A pointer to an @c idip_byte_storage_t to put the packet payload to.
 *                            Must not be @c NULL.
 *                            Extended on demand.
 * @param timeout_ms A reading timeout in milliseconds.
 *                   Use @c IDIP_INFINITE_TIMEOUT to wait infinitely.
 * @return @c 0 if successful,
 *         @c EAGAIN if @a timeout_ms is @c 0 and the source invalidated data at the current reading position,
 *         @c ETIMEDOUT if @a timeout_ms is not @c 0 and this timeout has passed,
 *         @c EINVAL if @a reader, @a header or @a payload_buffer is @c NULL,
 *         @c ENOMEM if inner reallocations failed during the reading.
 * @warning After successful reading @a header will hold references to data in the @a payload_buffer container,
 *          so the @a payload_buffer lifetime must exceed the lifetime of the @a header.
 */
idip_avs_read_result_t
idip_av_source_reader_read(idip_av_source_reader_t* reader, idip_av_packet_t* header,
                           idip_byte_storage_t* payload_buffer, int64_t timeout_ms, bool ignore_invalidation);

/**
 * @brief Makes an @c idip_av_source_reader_t object to fetch packets from the associated source until
 *        the key frame is fetched.
 * @param reader A pointer to an @c idip_av_source_t object to read packets from.
 *               Must not be @c NULL.
 * @param[out] header A pointer an @c idip_av_packet_t to fill the packet header data to.
 *                    Must not be @ NULL.
 * @param[out] payload_buffer A pointer to an @c idip_byte_storage_t to put the packet payload to.
 *                            Must not be @c NULL.
 *                            Extended on demand.
 * @param timeout_ms A reading timeout in milliseconds.
 *                   Use @c IDIP_INFINITE_TIMEOUT to wait infinitely.
 * @return @c 0 if successful,
 *         @c ETIMEDOUT if @a timeout_ms is not @c 0 and this timeout has passed,
 *         @c EINVAL if @a reader, @a header or @a payload_buffer is @c NULL,
 *         @c ENOMEM if inner reallocations failed during the reading.
 * @warning After successful reading @a header will hold references to data in the @a payload_buffer container,
 *          so the @a payload_buffer lifetime must exceed the lifetime of the @a header.
 *
 * Blocks until a key frame can be fetched from the source via @a reader or the @a timeout_ms has passed.
 */
idip_status_code_t idip_av_source_reader_wait_for_key(idip_av_source_reader_t* reader, idip_av_packet_t* header,
                                               idip_byte_storage_t* payload_buffer, int64_t timeout_ms, const char *stream_name);

/**
 * @brief Signals that an @c idip_av_source_reader_t object requested a key frame.
 * @param self A pointer to an @c idip_av_source_reader_t object to signal.
 *             Must not be @c NULL.
 */
void idip_av_source_reader_request_key_frame(idip_av_source_reader_t* self);

/**
 * @brief Rewinds internal buffer pointer to a first key video frame
 * that's in interval [now - rewind_time_ms, now].
 * @param self A pointer to an @c idip_av_source_t object to read packets from.
 *             Must not be @c NULL.
 * @param rewind_time_ms time difference from now in millisecinds
 * @return @c 0 if successful,
 *         @c EINVAL if @a self is @c NULL.
 */
int idip_av_source_reader_rewind_for_time(idip_av_source_reader_t* self,
                                          uint64_t rewind_time_ms);

/**
 * @brief Sets an End-Of-Stream flag to @c false.
 * @param self A pointer to an @c idip_av_source_reader_t object to reset the EOS state of.
 *             Must not be @c NULL.
 */
void idip_av_source_reset_eos(idip_av_source_t* self);

/**
 * @brief Tests whether an End-Of-Stream is reached.
 * @param self A pointer to an @c idip_av_source_reader_t object to test.
 *             Must not be @c NULL.
 * @return @c true if self is not @c NULL and the End-Of-Stream is reached,
 *         @c false otherwise.
 */
bool idip_av_source_eos(idip_av_source_reader_t* self);

/**
 * @brief Tests whether an @c idip_av_source_t object target name matches a @a target string.
 * @param[in] self A pointer to an @c idip_av_source_t object to test.
 *                 Must not be @c NULL.
 * @param[in] target A target name to match against.
 *                   Must not be @c NULL.
 * @return @c true if @a self and @a target are not @c NULL
 *         and the @a self target name matches the @a target string,
 *         @c false, otherwise.
 */
bool idip_av_source_target_equal(const idip_av_source_t* self, const char* target);

/**
 * @brief Tests whether an @c idip_av_source_t object profile name matches a @a profile string.
 * @param[in] self A pointer to an @c idip_av_source_t object to test.
 *                 Must not be @c NULL.
 * @param[in] profile A profile name to match against.
 *                    Must not be @c NULL.
 * @return @c true if @a self and @a profile are not @c NULL
 *         and the @a self profile name matches the @a profile string,
 *         @c false, otherwise.
 */
bool idip_av_source_profile_equal(const idip_av_source_t* self, const char* profile);

/**
 * @brief Tests whether an @c idip_av_source_t object target name matches a @a target string
 * and opbect profile name matches a @a profile string.
 * @param[in] self A pointer to an @c idip_av_source_t object to test.
 *                 Must not be @c NULL.
 * @param[in] target A target name to match against.
 *                   Must not be @c NULL.
 * @param[in] profile A profile name to match against.
 *                    Must not be @c NULL.
 * @return @c true if @a self and @a profile are not @c NULL,
 *         the @a self target name matches the @a target string,
 *         and the @a self profile name matches the @a profile string,
 *         @c false, otherwise.
 * @note the function dose the same as @ref idip_av_source_target_equal(self, target) &&
 * @ref idip_av_source_profile_equal(self, profile) with less locking overhead.
 */
bool idip_av_source_ids_equal(const idip_av_source_t* self,
                              const char* target,
                              const char* profile);

/**
 * @brief Makes a copy of the target name of the source.
 * @param self pointer to a @ref idip_av_source_t object.
 * @return A pointer to malloc()'ed string with the target name or NULL.
 * @note Use free() to release the result.
 */
char* idip_av_source_get_target(const idip_av_source_t* self);

/**
 * @brief Makes a copy of the profile name of the source.
 * @param self pointer to a @ref idip_av_source_t object.
 * @return A pointer to malloc()'ed string with the profile name or NULL.
 * @note Use free() to release the result.
 */
char* idip_av_source_get_profile(const idip_av_source_t* self);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // IDIP_AV_SOURCE_H
