/**
 * idip.h
 * IDIP Server API
 *
 * This file is a part of Ivideon Embedded SDK
 */
#ifndef LIBIDIP_H_INCLUDED
#define LIBIDIP_H_INCLUDED

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <idip/ivesdk_version.h>
#include <idip/idip_enum.h>
#include <idip/idip_utils.h>
#include <idip/idip_data_types.h>
#include <idip/idip_server.h>
#include <idip/idip_custom_arg.h>
#include <idip/idip_cfg_param.h>
#include <idip/idip_av_limits.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//------------------------------------------------------------------------------

/// Supported video codec constants
typedef enum video_codec
{
    /// Video codec is undefined
    VIDEO_CODEC_NONE = 0,
    /// Video is raw images in BGR24
    VIDEO_CODEC_RAW_BGR24,
    /// Video is raw images in RGB24
    VIDEO_CODEC_RAW_RGB24,
    /// Frames are jpeg images
    VIDEO_CODEC_MJPEG,
    /// Video encoded with h264 codec
    VIDEO_CODEC_H264,
    /// Video encoded with h265 codec
    VIDEO_CODEC_H265,
    /// Video encoded with mpeg4 codec
    VIDEO_CODEC_MPEG4,
    /// Pseudo codec for passing text information
    VIDEO_CODEC_TEXT,
    /// Video is raw images in YUV420p
    VIDEO_CODEC_RAW_YUV420P,
    /// Video is raw images in ARGB32
    VIDEO_CODEC_RAW_ARGB32,

    _VIDEO_CODEC_MAX ///< Count of supported codecs
}
video_codec_t;

/// Supported audio codec constants
typedef enum audio_codec
{
    AUDIO_CODEC_NONE = 0,   ///< "none"
    AUDIO_CODEC_AMR_NB,     ///< "amr_nb"
    AUDIO_CODEC_AMR_WB,     ///< "amr_wb"
    AUDIO_CODEC_AAC,        ///< "aac"
    AUDIO_CODEC_MP3,        ///< "mp3"
    AUDIO_CODEC_G726_16,    ///< "g726_16"
    AUDIO_CODEC_G726_24,    ///< "g726_24"
    AUDIO_CODEC_G726_32,    ///< "g726_32"
    AUDIO_CODEC_G726_40,    ///< "g726_40"
    AUDIO_CODEC_PCMU,       ///< "pcmu"
    AUDIO_CODEC_PCMA,       ///< "pcma"
    AUDIO_CODEC_PCM16LE,    ///< "pcmu16le"
    AUDIO_CODEC_PCMS16LE,   ///< "pcms16le"
    AUDIO_CODEC_PCMS16BE,   ///< "pcms16be"
    AUDIO_CODEC_PCM16,      ///< "pcm16"
    AUDIO_CODEC_ADPCM,      ///< "adpcm"
    AUDIO_CODEC_IMA_WAV,    ///< "imawav"

    _AUDIO_CODEC_MAX
}
audio_codec_t;

/// Supported formats of audio samples
typedef enum audio_sample_format
{
    AUDIO_SAMPLE_FORMAT_NONE = 0,  ///< "none"
    AUDIO_SAMPLE_FORMAT_INT8,      ///< "int8"
    AUDIO_SAMPLE_FORMAT_UINT8,     ///< "uint8"
    AUDIO_SAMPLE_FORMAT_INT16,     ///< "int16"
    AUDIO_SAMPLE_FORMAT_INT32,     ///< "int32"
    AUDIO_SAMPLE_FORMAT_FLOAT,     ///< "float"
    AUDIO_SAMPLE_FORMAT_DOUBLE,    ///< "double"

    AUDIO_SAMPLE_FORMAT_UINT8P,    ///< "uint8p"
    AUDIO_SAMPLE_FORMAT_INT16P,    ///< "int16p"
    AUDIO_SAMPLE_FORMAT_INT32P,    ///< "int32p"
    AUDIO_SAMPLE_FORMAT_FLOATP,    ///< "floatp"
    AUDIO_SAMPLE_FORMAT_DOUBLEP,   ///< "doublep"

    _AUDIO_SAMPLE_FORMAT_MAX
}
audio_sample_format_t;

/// Useful audio sampling frequencies
typedef enum audio_sampling_freq
{
    IDIP_AUDIO_SAMPLING_FREQ_48K    = 48000, ///< 48 kHz
    IDIP_AUDIO_SAMPLING_FREQ_44K1   = 44100, ///< 44.1 kHz
    IDIP_AUDIO_SAMPLING_FREQ_32K    = 32000, ///< 32 kHz
    IDIP_AUDIO_SAMPLING_FREQ_16K    = 16000, ///< 16 kHz
    IDIP_AUDIO_SAMPLING_FREQ_8K     = 8000,  ///< 8 kHz
}
audio_sampling_freq_t;

/// Useful audio bitrates
typedef enum idip_audio_bitrate
{
    IDIP_AUDIO_BITRATE_32_KBI  = 32000,
    IDIP_AUDIO_BITRATE_40_KBI  = 40000,
    IDIP_AUDIO_BITRATE_48_KBI  = 48000,
    IDIP_AUDIO_BITRATE_128_KBI = 128000,
    IDIP_AUDIO_BITRATE_192_KBI = 192000,
    IDIP_AUDIO_BITRATE_256_KBI = 256000,
    IDIP_AUDIO_BITRATE_320_KBI = 320000,
}
idip_audio_bitrate_t;

/// Enum that describes supported audio channels counts
typedef enum idip_audio_channels
{
    IDIP_AUDIO_NONE = 0,    ///< no audio channels
    IDIP_AUDIO_MONO = 1,    ///< one channel, mono
    IDIP_AUDIO_STEREO = 2,  ///< two channels, stereo
}
idip_audio_channels_t;

/// Archive play speed factors
typedef enum archive_play_speed
{
    ARCHIVE_PLAY_SPEED_MINUS_64X = 0,   ///< -64x
    ARCHIVE_PLAY_SPEED_MINUS_32X,       ///< -32x
    ARCHIVE_PLAY_SPEED_MINUS_16X,       ///< -16x
    ARCHIVE_PLAY_SPEED_MINUS_8X,        ///< -8x
    ARCHIVE_PLAY_SPEED_MINUS_4X,        ///< -4x
    ARCHIVE_PLAY_SPEED_MINUS_2X,        ///< -2x, backward playing with double speed
    ARCHIVE_PLAY_SPEED_MINUS_1X,        ///< -1x, backward playing with normal speed
    ARCHIVE_PLAY_SPEED_MINUS_1X2,       ///< -1/2x, backward playing with half speed
    ARCHIVE_PLAY_SPEED_MINUS_1X4,       ///< -1/4x
    ARCHIVE_PLAY_SPEED_MINUS_1X8,       ///< -1/8x
    ARCHIVE_PLAY_SPEED_MINUS_1X16,      ///< -1/16x
    ARCHIVE_PLAY_SPEED_MINUS_1X32,      ///< -1/32x
    ARCHIVE_PLAY_SPEED_MINUS_1X64,      ///< -1/64x
    ARCHIVE_PLAY_SPEED_1X64,            ///< 1/64x
    ARCHIVE_PLAY_SPEED_1X32,            ///< 1/32x
    ARCHIVE_PLAY_SPEED_1X16,            ///< 1/16x
    ARCHIVE_PLAY_SPEED_1X8,             ///< 1/8x
    ARCHIVE_PLAY_SPEED_1X4,             ///< 1/4x, forward playing with a quarter speed
    ARCHIVE_PLAY_SPEED_1X2,             ///< 1/2x, forward playing with a half speed
    ARCHIVE_PLAY_SPEED_1X,              ///< 1x, normal speed
    ARCHIVE_PLAY_SPEED_2X,              ///< 2x, forward playing with double speed
    ARCHIVE_PLAY_SPEED_4X,              ///< 4x
    ARCHIVE_PLAY_SPEED_8X,              ///< 8x
    ARCHIVE_PLAY_SPEED_16X,             ///< 16x
    ARCHIVE_PLAY_SPEED_32X,             ///< 32x
    ARCHIVE_PLAY_SPEED_64X,             ///< 64x

    _ARCHIVE_PLAY_SPEED_MAX
}
archive_play_speed_t;

/// Archive playing mode
typedef enum archive_play_mode
{
    /// Play one record. Playing stops when end of record reached.
    ARCHIVE_PLAY_MODE_SINGLE = 0,
    /// Continuous playing mode. Plays records one by one.
    ARCHIVE_PLAY_MODE_CONTINOUS,
    /// Play record from start time to first key frame or only one frame (for jpeg)
    ARCHIVE_PLAY_MODE_ONESHOT,

    _ARCHIVE_PLAY_MODE_MAX
}
archive_play_mode_t;

/// OTA states
typedef enum idip_ota_state
{
    /// Data part. The implementation should store the data.
    IDIP_OTA_DATA,
    /// End of data, all chunks were sent, no more chunks left.
    /// The implementation should start checking the downloaded file and
    /// upgrade procedure.
    IDIP_OTA_DATA_END,
    /// There were some errors during firmware download (for example,
    /// the network is down). The implementation should invalidate all previous
    /// data chunks. OTA failed.
    IDIP_OTA_CANCEL,
}
idip_ota_state_t;

/// Constant to apply infinite timeout to send/put operations
#define IDIP_INFINITE_TIMEOUT  (-1l)

//----- video.get --------------------------------------------------------------

/// Information about video stream
typedef struct video_stream_info
{
    /// Codec type
    video_codec_t codec;
    /// Image width in pixels. Can be set to 0 for mjpeg/h264/h265 codecs.
    /// The field is used prefer for @ref VIDEO_CODEC_NONE. Other codecs are contains
    /// resolution data, then the value is hint for decoder.
    unsigned int width;
    /// Image height in pixels. Can be set to 0 for mjpeg/h264/h265 codecs.
    /// The field is used prefer for @ref VIDEO_CODEC_NONE. Other codecs are contains
    /// resolution data, then the value is hint for decoder.
    unsigned int height;
}
video_stream_info_t;

/// Information about audio stream
typedef struct audio_stream_info
{
    audio_codec_t codec_type;       ///< Codec type
    audio_sample_format_t sample_format;   ///< Sample format
    unsigned int channels;          ///< Number of channels
    unsigned int sample_rate;       ///< Sample rate (samples per second)
}
audio_stream_info_t;

/// A helper macro that initializes an @ref audio_stream_info_t instance
/// with values that describes a stream without audio data (no audio).
#define IDIP_AUDIO_STREAM_INFO_NO_AUDIO \
    (audio_stream_info_t) { \
        .codec_type = AUDIO_CODEC_NONE, \
        .sample_format = AUDIO_SAMPLE_FORMAT_NONE, \
        .channels = 0, \
        .sample_rate = 0 \
    }

/// Opaque data structure that represents IDIP audio and video stream
/// input interface.
struct idip_stream_av;
typedef struct idip_stream_av idip_stream_av_t;

/// An audio/video source opaque handle.
struct idip_av_source;
typedef struct idip_av_source idip_av_source_t;

/// @ref idip_av_source_t event types
typedef enum {
    /// A first media consumer (client) appeared.
    /// The system should enable encoder for the channel if it was suspended.
    IDIP_AV_EVENT_FIRST_CLIENT_ENTER,
    /// No more media consumers that requesting live video stream. The system can
    /// suspend encoder for the channel for power consumption reasons.
    IDIP_AV_EVENT_LAST_CLIENT_LEAVE,
    /// A media consumer (client) appeared.
    IDIP_AV_EVENT_CLIENT_ENTER,
    /// A media consumer (client) left. But there are some other consumers reading
    /// the stream. In most cases this event can be safely ignored.
    IDIP_AV_EVENT_CLIENT_LEAVE,
    /// A key frame requested for the channel. Force the encoder to push
    /// new key frame.
    IDIP_AV_EVENT_KEYFRAME_REQUEST,
} idip_av_source_event_t;

/// An callback to signal audio/video encoder about channel events (see
/// @ref idip_av_source_event_t).
/// Could be supplied upon @c idip_av_source_t registering.
///
/// @param event - event type to handle
/// @param source - pointer to @ref idip_av_source_t that generates the event.
/// @param event_data - pointer to extended event data, depend on event type.
/// At now is always NULL.
/// @param udata - a pointer to user data that was got on the handler registration
typedef void
(*idip_av_source_event_handler_t)(idip_av_source_event_t event,
                                  const idip_av_source_t* source,
                                  const void* event_data, void* udata);

/// Parameters to initiate video/audio stream translation
typedef struct idip_stream_av_args {
    /// Target identifier (name of target)
    char* target;
    /// Profile name. Can be used for differentiate between cameras into DVR
    char* profile;
    /// Video/audio buffering time hint
    uint64_t buffer_time;
}
idip_stream_av_args_t;

/// Video stream request handler type. Inside this callback type user must
/// sends video and audio frames to stream.
///
/// @param stream - pointer to stream instance. Do NOT cache it!
/// @param args - pointer to structure with handler's arguments (parameters)
/// @param udata - pointer to user data
/// @return IDIP_OK - if stream was successfully finished,
///         another values of @ref idip_status_code_t if errors was occurred
typedef
idip_status_code_t (*idip_stream_av_handler_t)(idip_stream_av_t* stream,
                                               const idip_stream_av_args_t* args,
                                               void* udata);

/// Allows user to send video stream information.
///
/// @param stream - pointer to idip stream instance
/// @param info - pointer to video information structure
/// @param timeout_ms - operation timeout in milliseconds.
/// You can use IDIP_INFINITE_TIMEOUT here.
/// @return IDIP_OK - if video information was set success
/// or IDIP_GENERIC_ERROR in another case. idip server require to terminate stream
/// (return from @ref idip_stream_av_handler_t)
idip_status_code_t
idip_stream_av_put_video_info(idip_stream_av_t* stream,
                              const video_stream_info_t* info,
                              int timeout_ms);

/// Allows user to send audio stream information
///
/// @param stream - pointer to idip stream instance
/// @param info - pointer to audio stream info
/// @param timeout_ms - operation timeout in milliseconds.
/// You can use IDIP_INFINITE_TIMEOUT here.
idip_status_code_t
idip_stream_av_put_audio_info(idip_stream_av_t* stream,
                              const audio_stream_info_t* info,
                              long timeout_ms);

/// Allows user to send video frame.
/// This call blocks user until data will be send or stream not stopped.
/// Function DO NOT create copy of buffer's data.
///
/// @param stream - pointer to stream interface instance that serve current request
/// @param is_key_frame - key frame attribute (1 - key frame, 0 - basic)
/// @param time_stamp_ms - frame creation time stamp (UTC)
/// @param buffer - pointer to buffer
/// @param size - size of the buffer
/// @param timeout_ms - operation timeout in milliseconds.
/// You can use IDIP_INFINITE_TIMEOUT here.
idip_status_code_t
idip_stream_av_put_video_frame(idip_stream_av_t* stream,
                               int is_key_frame,
                               int64_t time_stamp_ms,
                               const void* buffer,
                               size_t size,
                               long timeout_ms);

/// Send audio data piece. Function DO NOT creates copy of user data. This function
/// blocks until data will be send or stream ended
///
/// @param stream - pointer to stream instance
/// @param time_stamp_ms - frame creation time stamp (UTC)
/// @param buffer - pointer to audio samples buffer
/// @param size - audio buffer size (in bytes)
/// @param timeout_ms - operation timeout in milliseconds.
/// You can use IDIP_INFINITE_TIMEOUT here.
idip_status_code_t
idip_stream_av_put_audio(idip_stream_av_t* stream,
                         int64_t time_stamp_ms,
                         const void* buffer,
                         size_t size,
                         long timeout_ms);

/// @brief Pushes data to an @c idip_av_source_t object.
/// @param self A pointer to an @c idip_av_source_t object to push data to.
///             Must not be @c NULL.
/// @param ts_ms Data timestamp, in milliseconds from the Epoch.
///              Must not be @c 0.
/// @param audio_data A pointer to audio data.
/// @param audio_size The @c audio_data size.
/// @param video_data A pointer to a video data.
/// @param video_size The @c video_data size.
/// @param video_is_key_frame Indicates a key video frame.
/// @return @c IDIP_OK if successful,
///         @c IDIP_INVALID_ARGUMENTS if invalid arguments are passed,
///         @c IDIP_INTERNAL_ERROR in case of internal error.
/// @warning Either @a audio_data or @a video_data could be @c NULL, but not both -
///          or @c IDIP_INVALID_ARGUMENTS will be returned.

idip_status_code_t
idip_av_source_push(idip_av_source_t* self,
                    uint64_t ts_ms,
                    const void* audio_data,
                    size_t audio_size,
                    const void* video_data,
                    size_t video_size,
                    bool video_is_key_frame);

/// @brief Pushes video data to an @c idip_av_source_t object.
/// @param self self A pointer to an @c idip_av_source_t object to push data to.
///             Must not be @c NULL.
/// @param ts_ms Data timestamp, in milliseconds from the Epoch.
/// @param data A pointer to video data.
///             Must not be @c NULL.
/// @param size @a data size.
///             Must not be @c 0.
/// @param is_key_frame @c true indicates a key video frame.
/// @return @c IDIP_OK if successful,
///         @c IDIP_INVALID_ARGUMENTS if invalid arguments are passed,
///         @c IDIP_INTERNAL_ERROR in case of internal error.

idip_status_code_t
idip_av_source_push_video(idip_av_source_t* self,
                          uint64_t ts_ms,
                          const void* data,
                          size_t size,
                          bool is_key_frame);

/// @brief Pushes audio data to an @c idip_av_source_t object.
/// @param self A pointer to an @c idip_av_source_t object to push data to.
///             Must not be @c NULL.
/// @param ts_ms Data timestamp, in milliseconds from the Epoch.
/// @param data A pointer to audio data.
///             Must not be @c NULL.
/// @param size @a data size.
///             Must not be @c 0.
/// @return @c IDIP_OK if successful,
///         @c IDIP_INVALID_ARGUMENTS if invalid arguments are passed,
///         @c IDIP_INTERNAL_ERROR in case of internal error.

idip_status_code_t
idip_av_source_push_audio(idip_av_source_t* self,
                          uint64_t ts_ms,
                          const void* data,
                          size_t size);

//----- audio.put --------------------------------------------------------------
/// Play audio stream request handler type
///
/// @param target - target identifier
/// @param info - audio stream characteristics
/// @param data - audio frame data
/// @param size - audio data size
/// @param udata - pointer to user data
typedef
idip_status_code_t (*idip_audio_playback_handler_t)(const char* target,
                                                    const audio_stream_info_t* info,
                                                    const void* data,
                                                    size_t size,
                                                    void* udata);

//----- archive.get ----------------------------------------------------------------

/// Opaque data structure that represents archive stream output interface
struct idip_stream_archive;
typedef struct idip_stream_archive idip_stream_archive_t;

/// Play archive parameters (request)
typedef struct idip_archive_play_params
{
    uint64_t start_ms;              ///< Record's start time. Milliseconds in UTC.
    archive_play_speed_t speed;     ///< Play speed factor
    archive_play_mode_t play_mode;  ///< Play mode
}
idip_archive_play_params_t;

/// Description of archive record. All values are milliseconds in UTC.
typedef struct archive_record
{
    uint64_t start_ms;      ///< record's start time
    uint64_t duration_ms;   ///< record's duration
}
archive_record_t;

/// Request parameters for handler. All values are milliseconds in UTC.
typedef struct archive_interval
{
    uint64_t start_ms;  ///< begin of interval
    uint64_t end_ms;    ///< end of interval
}
archive_interval_t;

/// Request to play archive
///
/// @param stream - pointer to output stream instance
/// @param target - target identifier
/// @param params - playing parameters
/// @param udata - pointer to user's data
typedef
idip_status_code_t (*idip_stream_archive_handler_t)(
            idip_stream_archive_t* stream,
            const char* target,
            const idip_archive_play_params_t* params,
            void* udata);

/// Put video information to output stream.
/// This call blocks until data will not send.
///
/// @param stream - pointer to idip stream instance
/// @param info - pointer to video information structure
/// @param timeout_ms - operation timeout in milliseconds.
/// You can use IDIP_INFINITE_TIMEOUT here.
idip_status_code_t
idip_stream_ar_put_video_info(idip_stream_archive_t* stream,
                              const video_stream_info_t* info,
                              long timeout_ms);

/// Put audio information to output stream
///
/// @param stream - pointer to idip stream instance
/// @param info - pointer to audio stream info
/// @param timeout_ms - operation timeout in milliseconds.
/// You can use IDIP_INFINITE_TIMEOUT here.
idip_status_code_t
idip_stream_ar_put_audio_info(idip_stream_archive_t* stream,
                              const audio_stream_info_t* info,
                              long timeout_ms);

/// @param self - pointer to stream instance
/// @param v_info - pointer to structure with video meta information. Cannot be NULL.
/// @param a_info - pointer to audio stream description. Can be NULL if no audio info is available.
/// @param d_info - pointer to structure that contains time information. Cannot be NULL.
/// @param timeout_ms - operation timeout in milliseconds.
/// Can be set to @ref IDIP_INFINITE_TIMEOUT
/// @note v_info, a_info and d_info cannot be NULL simultaneously
idip_status_code_t
idip_stream_ar_put_meta_info(idip_stream_archive_t* self,
                             const video_stream_info_t* v_info,
                             const audio_stream_info_t* a_info,
                             const archive_record_t* d_info,
                             long timeout_ms);

/// Allows user to send video frame from archive.
/// This call blocks user until data will be send or stream not stopped.
/// Function DO NOT create copy of buffer's data.
///
/// @param stream - pointer to stream interface instance that serve current request
/// @param is_key_frame - key frame attribute (1 - key frame, 0 - basic)
/// @param time_stamp_ms - frame creation time stamp (UTC)
/// @param buffer - pointer to buffer
/// @param size - size of the buffer
/// @param timeout_ms - operation timeout in milliseconds.
/// You can use IDIP_INFINITE_TIMEOUT here.
idip_status_code_t
idip_stream_ar_put_video_frame(idip_stream_archive_t* stream,
                                int is_key_frame,
                                uint64_t time_stamp_ms,
                                const void* buffer,
                                size_t size,
                                long timeout_ms);

/// Send audio data piece. Function DO NOT creates copy of user data. It blocks
/// until data will be send or stream ended
///
/// @param stream - pointer to stream instance
/// @param time_stamp_ms - milliseconds in UTC
/// @param buffer - pointer to audio samples buffer
/// @param size - audio buffer size (in bytes)
/// @param timeout_ms - operation timeout in milliseconds.
/// You can use IDIP_INFINITE_TIMEOUT here.
idip_status_code_t
idip_stream_ar_put_audio(idip_stream_archive_t* stream,
                         uint64_t time_stamp_ms,
                         const void* buffer,
                         size_t size,
                         long timeout_ms);

/// A synonym for the @ref idip_stream_ar_put_audio
#define idip_stream_ar_put_audio_frame idip_stream_ar_put_audio

//----- archive.list -----------------------------------------------------------

/// Opaque data structure that represents output stream for archive.list request
struct idip_stream_archive_list;
typedef struct idip_stream_archive_list idip_stream_archive_list_t;

/// Request to find records in archive
///
/// @param stream - output stream instance
/// @param target - target identifier
/// @param params - search options
/// @param udata - pointer to user's data
typedef
idip_status_code_t (*idip_stream_archive_list_handler_t)(
        idip_stream_archive_list_t* stream,
        const char* target,
        const archive_interval_t* params,
        void* udata);

/// Put information about founded records to output stream
///
/// @param stream - pointer to output stream instance
/// @param records - pointer to array of records
/// @param records_count - count of records array
/// @param timeout_ms - operation timeout in milliseconds.
/// You can use IDIP_INFINITE_TIMEOUT here.
idip_status_code_t
idip_stream_ar_list_put_items(idip_stream_archive_list_t* stream,
                              const archive_record_t* records,
                              size_t records_count,
                              long timeout_ms);

//----- archive.days -----------------------------------------------------------
/// Opaque data structure that represents output stream for archive.days request
struct idip_stream_archive_days;
typedef struct idip_stream_archive_days idip_stream_archive_days_t;

/// Request to search first-of-day records in archive
///
/// @param stream - pointer to output stream instance
/// @param target - target identifier
/// @param params - search options
/// @param udata - pointer to user's data
typedef
idip_status_code_t (*idip_stream_archive_days_handler_t)(
        idip_stream_archive_days_t* stream,
        const char* target,
        const archive_interval_t* params,
        void* udata);


/// Put information about founded records to output stream
///
/// @param stream - pointer to output stream instance
/// @param records - pointer to array of records
/// @param records_count - count of records array
/// @param timeout_ms - operation timeout in milliseconds.
/// You can use IDIP_INFINITE_TIMEOUT here.
idip_status_code_t
idip_stream_ar_days_put_items(idip_stream_archive_days_t* stream,
                              const archive_record_t* records,
                              size_t records_count,
                              long timeout_ms);

//----- config.get, config.put -------------------------------------------------

/// Opaque data structure that represents configuration stream interface
struct idip_stream_config;
typedef struct idip_stream_config idip_stream_config_t;

/// Request to send all configuration parameters to stream
///
/// @param stream - pointer to output stream instance
/// @param target - target identifier
/// @param udata - pointer to user's data
typedef
idip_status_code_t (*idip_stream_config_get_handler_t)(
                    idip_stream_config_t* stream,
                    const char* target,
                    void* udata);



/// Request to update configuration value. User must accept configuration
/// parameters from this callback and put all parameters that values has change.
///
/// @param stream - pointer to output stream instance
/// @param target - target identifier
/// @param config - array of pointers to instances of configuration parameters
/// @ref idip_cfg_param_t (see idip_cfg.h)
/// @param config_items_count - count of items in the above array
/// @param udata - pointer to user's data
typedef
idip_status_code_t (*idip_stream_config_update_handler_t)(
                    idip_stream_config_t* stream,
                    const char* target,
                    const idip_cfg_param_t** config,
                    size_t config_items_count,
                    void* udata);

/// Puts boolean configuration parameter to output stream
///
/// @param stream - pointer to output stream instance
/// @param name - parameter's name
/// @param val - value of parameter. 0 equals false, other values equals true
idip_status_code_t
idip_stream_config_put_bool(idip_stream_config_t* stream,
                            const char* name,
                            int val);

/// Put integer configuration value with its actual range
///
/// @param stream - pointer to output stream instance
/// @param name - parameter's name
/// @param value - value of parameter
/// @param min - value's lower bound
/// @param max - value's upper bound
/// @param step - step of possible increasing or decreasing of value
idip_status_code_t
idip_stream_config_put_int(idip_stream_config_t* stream,
                           const char* name,
                           int64_t value,
                           int64_t min,
                           int64_t max,
                           int64_t step);

/// Put double configuration value with its actual range
///
/// @param stream - pointer to output stream instance
/// @param name - parameter's name
/// @param value - value of parameter
idip_status_code_t
idip_stream_config_put_double(idip_stream_config_t* stream,
                              const char* name,
                              double value);

/// Put string configuration value with its actual range
///
/// @param stream - pointer to output stream instance
/// @param name - parameter's name
/// @param value - pointer to string buffer
/// @param min_length - minimum value's string length
/// @param max_length - maximum value's string length
idip_status_code_t
idip_stream_config_put_string(idip_stream_config_t* stream,
                              const char* name,
                              const char* value,
                              uint32_t min_length,
                              uint32_t max_length);

/// Put position configuration value with its actual range
///
/// @param stream - pointer to output stream instance
/// @param name - parameter's name
/// @param value - value of parameter
/// @param width - maximal value of position's X
/// @param height - maximal value of position's Y
idip_status_code_t
idip_stream_config_put_pos(idip_stream_config_t* stream,
                           const char* name,
                           const idip_pos_t* value,
                           int64_t width,
                           int64_t height);

/// Put grid configuration value with its actual range
///
/// @param stream - pointer to output stream instance
/// @param name - parameter's name
/// @param value - value of parameter
/// @param width - grid width (count of elements)
/// @param height - grid height
idip_status_code_t
idip_stream_config_put_grid(idip_stream_config_t* stream,
                            const char* name,
                            const char* value,
                            uint64_t width,
                            uint64_t height);

/// Put string value with its actual range
///
/// @param stream - pointer to output stream instance
/// @param name - parameter's name
/// @param value - value of parameter
/// @param range - array of strings, all acceptable values of parameter
/// @param range_length - count of items in range array
idip_status_code_t
idip_stream_config_put_choice_str(idip_stream_config_t* stream,
                                  const char* name,
                                  const char* value,
                                  const char* const* range,
                                  size_t range_length);

/// Put integer value with its actual range
///
/// @param stream - pointer to output stream instance
/// @param name - parameter's name
/// @param value - value of parameter
/// @param range - array of integers, all acceptable values of parameter
/// @param range_length - count of items in range array
idip_status_code_t
idip_stream_config_put_choice_int(idip_stream_config_t* stream,
                                  const char* name,
                                  int64_t value,
                                  const int64_t* range,
                                  size_t range_length);

/// Put polyline value with its range.
///
/// @param stream - pointer to output stream instance
/// @param name - parameter's name
/// @param value - value of parameter (array of points: vertexes of polyline)
/// @param value_length - count of elements of value. Must be 2 points or more.
/// @param range_x - X-axis range
/// @param range_y - Y-axis range
/// @param range_length - maximum points of polyline. This value limits a maximal
/// count of points in polyline that can be received correctly by current
/// implementation of idip server
idip_status_code_t
idip_stream_config_put_polyline(idip_stream_config_t* stream,
                                const char* name,
                                const idip_pos_t* value,
                                size_t value_length,
                                idip_axis_range_t range_x, idip_axis_range_t range_y,
                                size_t range_length);

/// Put polygon value with its range.
///
/// @param stream - pointer to output stream instance
/// @param name - parameter's name
/// @param value - value of parameter (array of points: vertexes of polygon)
/// @param value_length - count of elements of value. Must be 3 points or more.
/// @param range_x - X-axis range
/// @param range_y - Y-axis range
/// @param range_length - maximum points of polygon. This value limits a maximal
/// count of points in polygon that can be received correctly by current
/// implementation of idip server
idip_status_code_t
idip_stream_config_put_polygon(idip_stream_config_t* stream,
                               const char* name,
                               const idip_pos_t* value,
                               size_t value_length,
                               idip_axis_range_t range_x, idip_axis_range_t range_y,
                               size_t range_length);

/// Put rectangle value with its range.
///
/// @param stream - pointer to output stream instance
/// @param name - parameter's name
/// @param value_a - first point of rectangle
/// @param value_b - second point of rectangle that is opposite to first
/// @param range_x - X-axis range
/// @param range_y - Y-axis range
idip_status_code_t
idip_stream_config_put_rect(idip_stream_config_t* stream,
                            const char* name,
                            idip_pos_t value_a, idip_pos_t value_b,
                            idip_axis_range_t range_x, idip_axis_range_t range_y);


//----- persistance_data.get, persistance_data.put -----------------------------
/// Opaque data structure that represents stream interface for send
/// persistence data object
struct idip_stream_persistent_data;
typedef struct idip_stream_persistent_data idip_stream_persistent_data_t;

/// Callback type for read persistent data object.
///
/// @param stream - pointer to output stream instance
/// @param udata - pointer to user's data
typedef
idip_status_code_t (*idip_stream_persistent_data_get_handler_t)(
                    idip_stream_persistent_data_t* stream,
                    void* udata);

/// Put persistence data to output stream and send it.
///
/// @param stream - - pointer to output stream instance
/// @param pdata - pointer to persistent data
/// @param pdata_size - size of the data block (in bytes)
/// @param pdata_max_size - size limit to persistent data (in bytes)
/// @param timeout_ms - timeout for put operation in milliseconds
idip_status_code_t
idip_stream_persistence_data_put(idip_stream_persistent_data_t* stream,
                                 const void* pdata,
                                 size_t pdata_size,
                                 size_t pdata_max_size,
                                 long timeout_ms);

/// Callback type to store persistent data on device
///
/// @param pdata - pointer to block of persistent data
/// @param pdata_size - persistent data size
/// @param udata - pointer to user's data
typedef
idip_status_code_t (*idip_stream_persistent_data_set_handler_t)(
                    const void* pdata, size_t pdata_size,
                    void* udata);

//----- wireless.scan ----------------------------------------------------------
/// Opaque data structure that represents interface to send
/// results of wireless network scanning
struct idip_stream_wireless_scan;
typedef struct idip_stream_wireless_scan idip_stream_wireless_scan_t;

typedef enum idip_ap_security_type {
    AP_SEC_TYPE_UNKNOWN, ///< type can't be determined
    AP_SEC_TYPE_OPEN, ///< no encryption
    AP_SEC_TYPE_WEP,  ///< WEP encryption
    AP_SEC_TYPE_WPA,  ///< WPA
    AP_SEC_TYPE_WPA2, ///< WPA2
    AP_SEC_TYPE_AUTO, ///< The user handler should determine the actual encryption method by itself.

    _AP_SEC_TYPE_MAX  ///< Count of security types. For internal use
}
idip_ap_security_type_t;

/// Wireless Access Point (AP) description
typedef struct idip_ap_description
{
    /// Network's SSID (AP name). AP can hide its own ssid.
    /// Set the field to NULL in this case.
    char* ssid;
    /// Signal level, percent (0..100)
    int rssi;
    /// Type of AP
    idip_ap_security_type_t security;
}
idip_ap_description_t;

/// Callback type for scanning wireless networks near the device.
/// Send scanning results with @ref idip_stream_wireless_scan_put_ap_description()
/// inside the callback.
/// @param stream - pointer to output stream instance
/// @param udata - pointer to user's data
typedef idip_status_code_t
(*idip_wireless_scan_handler_t)(idip_stream_wireless_scan_t* stream, void* udata);

/// Send (put) description of AP to output stream
///
/// @param stream - pointer to output stream instance
/// @param ap_descr - pointer to AP's description. All fields MUST be set!
idip_status_code_t
idip_stream_wireless_scan_put_ap_description(idip_stream_wireless_scan_t* stream,
                                             const idip_ap_description_t* ap_descr);

//----- wireless.setup ---------------------------------------------------------
/// Access Point (AP) connection parameters
typedef struct idip_ap_connect_parameters
{
    /// Network's SSID (AP name). AP can hide its own ssid.
    /// Set the field to NULL in this case.
    char* ssid;
    /// Type of AP
    idip_ap_security_type_t security;
    /// Password for connection when AP type is WEP or WPA/WPA2. For open AP's
    /// password is empty string
    char* password;
}
idip_ap_connect_parameters_t;

/// Callback type for setup wireless connection
/// @param params - pointer to structure where connection options are contains
/// @param udata - pointer to user's data
typedef idip_status_code_t
(*idip_wireless_setup_handler_t)(const idip_ap_connect_parameters_t* params,
                                 void* udata);

//----- ptz --------------------------------------------------------------------
/// PTZ commands
typedef enum ptz_command_type
{
    IDIP_PTZ_HOME, ///< Reset ptz module to default settings

    IDIP_PTZ_ZOOM_IN,  ///< Increase zoom factor
    IDIP_PTZ_ZOOM_OUT, ///< Decrease zoom factor
    IDIP_PTZ_LEFT,     ///< yaw ckw
    IDIP_PTZ_RIGHT,    ///< yaw ccw
    IDIP_PTZ_UP,       ///< pitch up
    IDIP_PTZ_DOWN,     ///< pitch down

    _IDIP_PTZ_MAX      ///< count of command types. Prefer for internal use
}
ptz_command_type_t;

/// Arguments of ptz method.
typedef struct idip_ptz_command
{
    char*              target;
    ptz_command_type_t command;
}
idip_ptz_command_t;

/// Callback type for PTZ operations
typedef idip_status_code_t
(*idip_ptz_handler_t)(const char* target, const idip_ptz_command_t* cmd, void* udata);

//----- video.snapshot ----------------------------------------------------------
/// Opaque data structure that represents interface to send
/// snapshots (snapshots)
struct idip_stream_snapshot;
typedef struct idip_stream_snapshot idip_stream_snapshot_t;

/// Parameters to initiate snapshot translation
typedef struct idip_stream_snapshot_args {
    /// Target identifier (name of target)
    char* target;
    /// Profile name. Can be used for differentiate between cameras into DVR
    char* profile;
}
idip_stream_snapshot_args_t;

/// Video stream request handler type. Inside this callback type user must
/// sends video and audio frames to stream.
///
/// @param stream - pointer to stream instance. Do NOT cache it!
/// @param args - pointer to structure with handler's arguments (parameters)
/// @param udata - pointer to user data
/// @return IDIP_OK - if stream was successfully finished,
///         another values of @ref idip_status_code_t if errors was occurred
typedef idip_status_code_t
(*idip_stream_snapshot_handler_t)(idip_stream_snapshot_t* stream,
                                 const idip_stream_snapshot_args_t* args,
                                 void* udata);

/// Allows user to send snapshots
///
/// @param stream - pointer to idip output stream instance
/// @param info - pointer to video information structure
/// @param timeout_ms - operation timeout in milliseconds.
/// You can use IDIP_INFINITE_TIMEOUT here.
/// @return IDIP_OK - if video information was set success
/// or IDIP_GENERIC_ERROR in another case. idip server require to terminate stream
/// (return from @ref idip_stream_snapshot_handler_t)
idip_status_code_t
idip_stream_snapshot_set_video_info(idip_stream_snapshot_t* stream,
                                   const video_stream_info_t* info,
                                   int timeout_ms);

/// Allows user to send video frame or jpeg/raw image.
/// Call @ref idip_stream_snapshot_set_video_info() before to set snapshot's
/// parameters (e.g. resolution for raw image or codec type)
///
/// This call blocks user until data will be send or stream not stopped.
/// Function DO NOT create copy of buffer's data.
///
/// @param stream - pointer to stream interface instance that serve current request
/// @param time_stamp_ms - frame capture time (milliseconds in UTC)
/// @param buffer - pointer to buffer
/// @param size - size of the buffer
/// @param timeout_ms - operation timeout in milliseconds.
/// You can use IDIP_INFINITE_TIMEOUT here.
idip_status_code_t
idip_stream_snapshot_put_video_frame(idip_stream_snapshot_t* stream,
                                    int64_t time_stamp_ms,
                                    const void* buffer,
                                    size_t size,
                                    long timeout_ms);

//----- system.notify ----------------------------------------------------
/// Structure that represents arguments of system.notify method
typedef struct system_notify_query_args
{
    /// target name
    char* target;
    /// notify name. There are the following names are supported:
    ///   "bind" - the camera is waiting for a QR code or binding using its MAC address.
    ///   "bound" - the camera was attached to the cloud.
    ///   "connect" - connection to the cloud is in progress.
    ///   "online" - camera is online and is connected to the cloud.
    ///   "offline" - connection to the cloud has lost.
    ///   "upgrade" - software upgrade process is started. The camera is
    ///               offline, all network interfaces may be down.
    char* name;
}
idip_system_notify_query_args_t;

/// Callback type to notify user about system state changes. User's
/// implementation must show state on hardware indicators
/// @param args - pointer to structure with arguments
/// @param udata - pointer to user's data
typedef idip_status_code_t
(*idip_stream_system_notify_handler_t)(
        const idip_system_notify_query_args_t* args, void* udata);

//----- system.update_version --------------------------------------------------
/// Data type to pass parameters to @ref idip_stream_update_version_handler_t
typedef struct system_update_query_args
{
    /// new software version string.
    char* version;
    /// An URL to fetch new firmware.
    char* url;
}
idip_system_update_query_args_t;


/// Callback type to notify user about new software version available
/// on ivideon's update site.
/// @param args - Pointer to helper data structure that contains pointer to a
/// new firmware version string, and a pointer to an URL to fetch the firmware.
/// @param udata - pointer to user's data
typedef idip_status_code_t
(*idip_stream_update_version_handler_t)(const idip_system_update_query_args_t* args,
                                        void* udata);

/// Arguments for @ref idip_stream_ota_handler_t
typedef struct idip_ota_args
{
    /// Pointer to data buffer. The buffer is allocated in the RAM by libidip.
    /// You have to set up a chunk size limit by @ref idip_server_setup_ota_handler().
    /// May be NULL when @a state equals to @ref IDIP_OTA_CANCEL.
    void*               chunk;
    /// Data size (in bytes).
    size_t              chunk_size;
    /// Chunk data offset from start of a firmware file.
    size_t              chunk_offset;
    /// Chunk receiving state
    idip_ota_state_t    state;
}
idip_ota_args_t;

/// Callback type to store part of firmware upgrade file.
/// @note This is an experimental interface. It is partially implemented.
/// Use @ref idip_stream_update_version_handler_t instead.
///
/// @param args - pointer to @ref idip_ota_args_t structure
/// @param user_data - pointer to user data
typedef idip_status_code_t
(*idip_stream_ota_handler_t)(const idip_ota_args_t* args, void* user_data);

//----- system.passwd ----------------------------------------------------------
/// Arguments for @ref idip_stream_passwd_handler_t
typedef struct idip_user_credentials
{
    char* password;     /// new password to set. Cannot be NULL.
    char* user;         /// user name. Not required (can be NULL).
}
idip_user_credentials_t;

/// Handler to update a camera password
///
/// @param argx - pointer to @ref idip_system_passwd_args_t structure
/// @param user_data - pointer to user data
typedef idip_status_code_t
(*idip_stream_passwd_handler_t)(const idip_user_credentials_t* args, void* user_data);

//----- system.get_users -------------------------------------------------------
/// An opaque data structure that implements an output stream for a
/// system.get_users method.
typedef struct idip_stream_system_get_users idip_stream_system_get_users_t;

/// Handler to get all accounts
///
/// @param stream - a pointer to an output stream object
/// @param user_data - pointer to user data
typedef idip_status_code_t
(*idip_stream_system_get_users_handler_t)(idip_stream_system_get_users_t* stream,
                                          void* user_data);

/// Send a list of user credentials. The method could be called from
/// @ref idip_stream_system_get_users_handler_t handler only.
///
/// @param stream - pointer to an output stream.
/// @param cred - pointer to a first user credentials array item. Cannot be NULL!
/// @param cred_size - counter of items in @a cred array.
/// @param timeout_ms - operation timeout in milliseconds.
/// You can use IDIP_INFINITE_TIMEOUT here.
idip_status_code_t
idip_stream_system_get_users_put_credentials(idip_stream_system_get_users_t* stream,
                                             const idip_user_credentials_t* cred,
                                             size_t cred_size,
                                             long timeout_ms);

//----- system.get_interfaces -------------------------------------------------------
/// An opaque data structure that implements an output stream for a
/// system.get_interfaces method.
typedef struct idip_stream_system_get_interfaces idip_stream_system_get_interfaces_t;

/// Handler to get information about all system interfaces
///
/// @param stream - a pointer to an output stream object
/// @param user_data - pointer to user data
typedef idip_status_code_t
(*idip_system_get_interfaces_handler_t)(idip_stream_system_get_interfaces_t* stream,
                                        void* user_data);
/// Interface types (wired or wireless)
typedef enum idip_iftype
{
    IDIP_IFACE_WIRED,
    IDIP_IFACE_WIRELESS,
}
idip_iftype_t;

/// IPv4 address and mask representaion
typedef struct idip_ipv4_addr
{
    uint8_t addr[4];    ///< address octets, e.g. { 192, 168, 0, 1 }
    uint8_t mask_length;///< net mask length in byes (e.g. 24)
}
idip_ipv4_addr_t;

/// IPv6 address and mask representaion
/// @a addr items are in host format
typedef struct idip_ipv6_addr
{
    uint16_t addr[8];   // address octets, e.g. { 192, 168, 0, 1 }
    uint8_t  mask_length;
}
idip_ipv6_addr_t;

/// Maximum amount of addresses of an interface
#define IDIP_MAX_IFADDRESSES 4

/// MAC address length
#define IDIP_HWADDR_LEN 6

typedef struct idip_hwaddr
{
    uint8_t mac[IDIP_HWADDR_LEN];
}
idip_hwaddr_t;

/// A data structure that represents a network interface settings
typedef struct idip_iface
{
    /// interface type. @ref idip_iface_type_t
    idip_iftype_t       type;
    /// The mac address of the interface is using for attach to the cloud.
    uint8_t             attach_iface;
    /// interface name
    char*               ifname;
    /// current interface mac address (octets)
    idip_hwaddr_t       mac_addr;
    /// array of ipv4 addresses of the interface with masks
    idip_ipv4_addr_t    ipv4_addr[IDIP_MAX_IFADDRESSES];
    /// Count of valued items in the @a ipv4_addr array
    uint8_t             ipv4_addr_count;
    /// array of ipv6 addresses of the interface with masks
    idip_ipv6_addr_t    ipv6_addr[IDIP_MAX_IFADDRESSES];
    /// Count of valued items in the @a ipv6_addr array
    uint8_t             ipv6_addr_count;
    struct {
        char* ssid;         ///< SSID for wireless interface. Set to NULL for
                            /// wired or not connected wireless interfaces
        idip_ap_security_type_t auth_type;    ///< bss auth type
    };
}
idip_iface_t;

/// An option to retrieve address information for an interface
typedef enum idip_get_interface_info_option {
    IDIP_GET_IFINFO_ALL,        ///< Get both IPv4 and IPv6 addresses
    IDIP_GET_IFINFO_IPV4_ONLY,  ///< Get IPv4 addresses only
    IDIP_GET_IFINFO_IPV6_ONLY,  ///< Get IPv6 addresses only
} idip_get_interface_info_option_t;

/// Retrieve information about network addresses for an interface.
/// This is a helper function for implementations of the
/// @ref idip_system_get_interfaces_handler_t.
///
/// @param ifname - a string, a name of the interface. E.g. "eth0", "wlan0".
/// Must not be NULL.
/// @param option - a switch that allows to retreive only ipv4/v6 addresses or both
/// @param info - [out] a pointer to filling data. Must not be NULL.
///
/// @note The function DOESN'N CHANGE these fields: info.ssid,
/// info.auth_type and info.attach_iface. A user MUST setup these fields by itself.
///
/// @note The function just assign @a ifname the info.ifname, not a copy.
///
/// @return 0 on success, -1 on invalid parameters or errors
int idip_get_interface_info(const char* ifname,
                            idip_get_interface_info_option_t option,
                            idip_iface_t* info);

/// Send a list wint the system interfaces infrmation. The method could be
/// called from @ref idip_stream_system_get_interfaces_handler_t handler only.
///
/// @note PAY ATTENTION! The first item of @a iflist should describe an active
/// network interface that is using for network operations.
///
/// @param stream - pointer to an output stream.
/// @param iflist - pointer to a first item of interfaces description. Cannot be NULL!
/// @param iflist_size - counter of items in @a iflist array.
/// @param timeout_ms - operation timeout in milliseconds.
/// You can use IDIP_INFINITE_TIMEOUT here.
idip_status_code_t
idip_stream_system_get_interfaces_put_iface_data(idip_stream_system_get_interfaces_t* stream,
                                                 const idip_iface_t * const iflist,
                                                 size_t iflist_size,
                                                 long timeout_ms);

//----- token.get --------------------------------------------------------------
/// An opaque data structure that can pass a token from QR-code/sound/etc to libidip
typedef struct idip_stream_system_token idip_stream_system_token_t;

/// A handler type that can be called when libidip is need for a token.
/// @param stream - a pointer to an output stream object
/// @param user_data - pointer to user data
typedef idip_status_code_t
(*idip_system_token_handler_t)(idip_stream_system_token_t* stream, void* user_data);

/// A structure to pass a token and WiFi credenials from QR-code/sound code/etc
/// to the libidip @ref idip_stream_system_token_put_token().
typedef struct idip_parsed_token
{
    /// a cloud attach token value. Required.
    char* token;
    /// WiFi SSID. Optional. Can be NULL.
    char* ssid;
    /// WiFi AP password. Optional. Can be NULL.
    char* passwd;
}
idip_parsed_token_t;

/// All fields of @ref idip_parsed_token_t are malloc()'ed strings. This
/// conveinient function allows to free all fields of that struct.
void idip_parsed_token_destroy(idip_parsed_token_t* self);

/// Put QR-code/etc token to a @ref idip_stream_system_token_t object.
/// @note The function can be called only from @ref idip_system_token_handler_t.
idip_status_code_t
idip_stream_system_token_put_token(idip_stream_system_token_t* stream,
                                   const idip_parsed_token_t * const token);

//----- idip server ------------------------------------------------------------

/// Initialize library.
///
/// @return IDIP_OK if success, IDIP_GENERIC_ERROR if errors was occurred.
idip_status_code_t idip_init(void);

/// Cleanup library resources
void idip_cleanup(void);

//------------------------------------------------------------------------------

/// Set up or replace live stream handler for given target.
///
/// @param server - pointer to initialized idip server instance. Cannot be NULL.
/// @param target - target name. If NULL or empty string the function set up
/// a default live stream handler.
/// @param handler - pointer to @ref idip_stream_av_handler_t function
/// If both @a target and @a handler are NULL, then the server will not support
/// live streams translation. All requests for live videos will be rejected.
/// @param pprofiles - a NULL-terminated array of pointers to all allowed profile
/// names (strings) for the given @a target. Can be NULL when you don't want to
/// handle profile names.
/// @param user_data - pointer that will be passed to @a handler.
/// @return 0 if success, -1 if errors.
///
/// @note This function should not be used when you're using @ref idip_av_source_t.
int idip_server_setup_live_feature(idip_server_t* server,
                                   const char* target, const char **pprofiles,
                                   idip_stream_av_handler_t handler,
                                   void *user_data);

/// Set up or replace a snapshot handler for given target.
///
/// @param server - pointer to initialized idip server instance. Cannot be NULL.
/// @param target - target name. If NULL or empty string the function set up
/// a default snapshot handler.
/// @param handler - pointer to snapshot handler implementation.
/// @param udata - pointer which will be passed to @a handler.
/// @return 0 if success, -1 if errors.
int idip_server_setup_snapshot_feature(idip_server_t* server,
                                       const char* target,
                                       idip_stream_snapshot_handler_t handler,
                                       void *udata);

/// Archive feature capabilities type
typedef struct idip_archive_capabilities
{
    /// Array of supported archive speed factors. Cannot be NULL. In a simplest
    /// case an implementation can support only one speed factor
    /// @ref ARCHIVE_PLAY_SPEED_1X.
    archive_play_speed_t* speed_factors;
    /// Archive speed factors array length (in items).
    size_t speed_factors_count;
    /// IDIPv2 Window Size Limit in bytes. 0 - Windowing algorithm isn't used.
    size_t window_size;
}
idip_archive_capabilities_t;

/// Setup an archive capabilities and handlers for given target.
///
/// @param server - pointer to idip server instance. Cannot be NULL.
/// @param target - target name. If NULL then the function set up
/// archive default handlers. All of three handlers should be valid.
/// @param handler -
/// @param handler_udata - pointer which will be pass to @a handler
/// @param list_handler -
/// @param list_handler_udata - pointer which will be pass to @a list_handler
/// @param days_handler -
/// @param days_handler_udata - pointer which will be pass to @a days_handler
/// @param capabilities - pointer to instance of @a idip_archive_capabilities_t.
/// capabilities.speed_factors must contains one or more values when handlers
/// are set.
/// @note A correct archive feature implementation must contain all of three callbacks.
/// @return 0 if success, -1 if errors.
int idip_server_setup_archive_feature(idip_server_t* server,
                                      const char *target,
                                      idip_stream_archive_handler_t handler,
                                      void *handler_udata,
                                      idip_stream_archive_list_handler_t list_handler,
                                      void *list_handler_udata,
                                      idip_stream_archive_days_handler_t days_handler,
                                      void *days_handler_udata,
                                      idip_archive_capabilities_t capabilities);

/// Push-To-Talk playback capabilities type.
typedef struct idip_p2t_playback_capabilities
{
    /// List of supported audio codecs which are using for playing on a device.
    audio_codec_t* acodecs;
    /// Audio codecs list size. In items.
    size_t acodecs_size;
    /// IDIPv2 Window Size Limit in bytes. 0 - Windowing algorithm isn't used.
    size_t window_size;
}
idip_p2t_playback_capabilities_t;

/// Setup a playback feature handler and capabilities for given target.
///
/// @param server - pointer to initialized idip server instance.
/// @param target - target name. If NULL or an empty string then the functions
/// setup a default p2t handler.
/// @param handler - user handler which implements p2t feature. If NULL
/// then the device/target don't support the feature.
/// @param udata - pointer to user data which will be passed to @a handler.
/// @param capabilities - data which are bound to the p2t feature. They must
/// contains a valid data when the handler is set.
/// @return 0 if success, -1 if errors.
int idip_server_setup_p2t_feature(idip_server_t* server,
                                  const char* target,
                                  idip_audio_playback_handler_t handler,
                                  void *udata,
                                  idip_p2t_playback_capabilities_t capabilities);

/// Cloud configuration capabilities type.
typedef struct idip_config_capabilities
{
    /// Pointer to device configuration transmit handler implementation.
    /// Cannot be NULL.
    idip_stream_config_get_handler_t    get_handler;
    /// Pointer to device configuration update handler implementation.
    /// Cannot be NULL.
    idip_stream_config_update_handler_t update_handler;
}
idip_config_capabilities_t;

/// Set up a cloud configuration handlers for given target.
///
/// @param server - pointer to an initialized idip server instance. Cannot be NULL.
/// @param target - target name. If NULL the function set up default
/// configuration handlers instead default handlers overriding.
/// @param get_handler - configuration read handler
/// @param get_handler_udata - pointer to user data which will be passed
/// to @a get_handler
/// @param update_handler - configuration values update handler
/// @param update_handler_udata - pointer to user data which will be passed
/// to @a update_handler
/// @note All of two handlers must be implemented or nothing.
/// @return 0 if success, -1 if errors.
int idip_server_setup_config_feature(idip_server_t* server,
                                     const char* target,
                                     idip_stream_config_get_handler_t    get_handler,
                                     void *get_handler_udata,
                                     idip_stream_config_update_handler_t update_handler,
                                     void *update_handler_udata);

/// Setup handlers which implements persistent storage capabilities.
/// The feature is device-wide then no target is needed.
///
/// @param server - pointer to initialized idip server instance.
/// @param get_handler - Pointer to a handler implementation which is reading
/// data from a persistent data storage.
/// @param get_handler_udata - pointer which will be passed to @a get_handler.
/// @param put_handler - Pointer to a handler implementation which is writing
/// data to a persistent data storage.
/// @param put_handler_udata - pointer which will be passed to @a put_handler.
/// @param data_max_size - Maximum data amount which is able to store.
/// @note All of two handlers must be implemented.
/// @return 0 if success, -1 if errors.
int idip_server_setup_persistent_storage_feature(idip_server_t* server,
                                                 idip_stream_persistent_data_get_handler_t get_handler,
                                                 void* get_handler_udata,
                                                 idip_stream_persistent_data_set_handler_t put_handler,
                                                 void* put_handler_udata,
                                                 size_t data_max_size);

/// A handler that implements getting data from file based persistent storage.
/// @note The handler assumes that the @a udata is a pointer to a string
/// with path to file (const char*).
idip_status_code_t
idip_file_persistent_storage_get_handler(idip_stream_persistent_data_t* stream,
                                    void* udata);

/// A handler that implements writing data from file based persistent storage.
/// @note The handler assumes that the @a udata is a pointer to a string
/// with path to file (const char*).
idip_status_code_t
idip_file_persistent_storage_set_handler(const void* data, size_t size, void* udata);

/// Setup 2 handlers that implements persistent storage on a file.
/// It's a helper function that can be used instead
/// the @ref idip_server_setup_persistent_storage_feature() on most cases.
/// It setups @ref idip_file_persistent_storage_get_handler(),
/// @ref idip_file_persistent_storage_set_handler() and bound them with
/// the given @a file path.
///
/// @param server - pointer to initialized idip server instance.
/// @return 0 if success, -1 if errors.
int idip_server_setup_file_persistent_storage(idip_server_t* server,
                                              const char* file_path);

int idip_server_setup_get_interfaces_feature(idip_server_t* server,
                                             idip_system_get_interfaces_handler_t handler,
                                             void* udata);

/// A simple handler implementation to use on devices with a single interface.
/// @note The handler assumes the @a udata is a pointer to an interface name
/// string (const char*).
idip_status_code_t
idip_single_interface_handler(idip_stream_system_get_interfaces_t* stream,
                              void* udata);

/// A helper method that setups a simple handler that can get the libidip
/// information about one network interface. The function setups
/// the @ref idip_single_interface_handler as a handler for the
/// @ref idip_server_setup_get_interfaces_feature.
///
/// @note DO NOT use the function when a device has more than one network interface.
///
/// @param server - pointer to initialized idip server instance.
/// @param ifname - name of network interface (e.g. "eth0")
/// @return 0 on sccess, -1 on errors.
int idip_server_setup_single_interface_handler(idip_server_t* server,
                                               const char* ifname);

/// Set up device wireless access feature handlers.
/// The feature is device-wide then no target is needed.
///
/// @param server - pointer to initialized idip server instance.
/// @param scan_handler - Pointer to a wireless network scan handler
/// implementation.
/// @param scan_handler_udata - pointer that will be passed to @a scan handler.
/// @param setup_handler - Pointer to a handler implementation which is able
/// to connect the device to a wireless network.
/// @param setup_handler_udata - pointer that will be passed
/// to @a setup_handler.
/// @note All of two handlers must be implemented.
/// @return 0 if success, -1 if errors.
int idip_server_setup_wireless_feature(idip_server_t* server,
                                       idip_wireless_scan_handler_t  scan_handler,
                                       void *scan_handler_udata,
                                       idip_wireless_setup_handler_t setup_handler,
                                       void *setup_handler_udata);

/// PTZ feature capabilities type
typedef struct idip_ptz_capabilities
{
    /// Array of supported directions
    ptz_command_type_t* commands;
    /// Directions array size (in items)
    size_t commands_count;
}
idip_ptz_capabilities_t;

/// Set up PTZ feature capabilities.
///
/// @param server - pointer to initialized idip server instance.
/// @param target - target name. If NULL then the function setup a
/// default ptz handler.
/// @param handler - Pointer to PTZ handler implementation. Cannot be NULL.
/// @param udata - pointer to user data which will be passed to @a handler
/// @param capabilities - @a idip_ptz_capabilities_t structure.
/// @return 0 if success, -1 if errors.
int idip_server_setup_ptz_feature(idip_server_t* server,
                                  const char* target,
                                  idip_ptz_handler_t handler,
                                  void *udata,
                                  idip_ptz_capabilities_t capabilities);

/// Set up a handler which is calling by libidip on each system notify message.
///
/// @param server - pointer to initialized idip server instance.
/// @param handler - pointer to a handler
/// @param udata - pointer to user data which will be passed to @a handler
/// @return 0 if success, -1 if errors.
int idip_server_setup_system_notify_handler(idip_server_t* server,
                                            idip_stream_system_notify_handler_t handler,
                                            void *udata);

/// Set up a handler which will be called if a new software version is available
/// on a software update server.
///
/// @param server - pointer to initialized idip server instance.
/// @param handler - pointer to a handler
/// @param udata - pointer to user data which will be passed to @a handler
/// @return 0 if success, -1 if errors.
int idip_server_setup_update_handler(idip_server_t* server,
                                     idip_stream_update_version_handler_t handler,
                                     void *udata);

/// Set up a handler which will be called when a part of firmware upgrade file
/// was downloaded from the cloud.
/// @note This is an experimental interface. It might be partially implemented.
/// Use @ref idip_server_setup_update_handler instead.
///
/// @param server - pointer to initialized idip server instance.
/// @param handler - pointer to a handler
/// @param maximum_chunk_size - maximum chunk size (in bytes) that the
/// implementation could accept. This value is a hint to libidip which allocates
/// a buffer for data chunks.
/// @param udata - pointer to user data which will be passed to @a handler
/// @return 0 if success, -1 if errors.
int idip_server_setup_ota_handler(idip_server_t* server,
                                  idip_stream_ota_handler_t handler,
                                  size_t maximum_chunk_size,
                                  void* udata);

/// Setup a handler which will be called for each protocol authentication query.
/// If @a handler is NULL, then the authentication is disabled.
///
/// @param server - pointer to @a idip_server_t instance
/// @param handler - pointer to a handler implementation
/// @param udata - pointer to user data which will be passed to @a handler
/// @return 0 if success, -1 if errors.
int idip_server_setup_auth_user_handler(idip_server_t* server,
                                        idip_auth_user_t handler,
                                        void *udata);

/// Setup a pair of handlers to get all accounts and change password for camera
/// account.
///
/// @param server - pointer to initialized idip server instance.
/// @param passwd_handler - set password handler.
/// @param passwd_handler_udata - pointer to @a passwd_handler data.
/// @param get_users_handler - get all users handler.
/// @param get_users_handler_udata - pointer to @a get_users_handler data.
/// @return 0 if success, -1 if errors.
int idip_server_setup_account_management_handlers(idip_server_t* server,
                    idip_stream_passwd_handler_t passwd_handler,
                    void* passwd_handler_udata,
                    idip_stream_system_get_users_handler_t get_users_handler,
                    void* get_users_handler_udata);

int idip_server_setup_token_handler(idip_server_t* server,
                                    idip_system_token_handler_t token_handler,
                                    void* token_handler_udata);

//------------------------------------------------------------------------------

/// Notify DIDP server instance about event
///
/// @param self - pointer to IDIP server instance
/// @param target - target name
/// @param event_name - event name
/// @param event_time_stamp_ms - UTC timestamp in milliseconds
/// @param arg - pointer to optional argument instance. Can be null
///
/// @return IDIP_OK if success or one of @ref idip_status_code_t if server
/// requests stop the stream
idip_status_code_t
idip_server_send_event(idip_server_t* self,
                      const char* target,
                      const char* event_name,
                      uint64_t event_time_stamp_ms,
                      const idip_custom_arg_t *arg);

//------------------------------------------------------------------------------

/// Audio/video source parameters
typedef struct idip_av_source_params
{
    const char* target_name;                                   ///< Target name
    const char* profile_name;                                  ///< Profile name
    video_stream_info_t v_info;                                ///< Video parameters
    audio_stream_info_t a_info;                                ///< Audio parameters
    idip_av_limits_t limits;                                   ///< Buffersite and time limits
    idip_av_source_event_handler_t  event_handler;		///< A callback to enforce a key video frame and handle other events from idip_av_source_t
    void* event_handler_data;					///< A @a event_handler private data
} idip_av_source_params_t;

/// Creates and registers an audio/video source.
///
/// @param[in] self A pointer to an @c idip_server_t object to register the source in.
///                 Must not be @c NULL.
/// @param[in] params A pointer to an @ref idip_av_source_params_t object holding the
///                   source initialization parameters.
///                   Must not be @c NULL.
/// @param[out] source A pointer to an @c idip_av_source_t handle to associate the registered source with.
///                    Must not be @c NULL.
/// @return @c IDIP_OK if successful,
///         @c IDIP_INVALID_ARGUMENTS if invalid arguments are passed,
///         @c IDIP_GENERIC_ERROR if internal allocations has failed.
///
/// A source handle returned in the @a source argument is to be used to put audio/video data from the specified
/// target and profile in the @a target_name and the @a profile_name fields of @a params, respectively.
/// Following fields in @a params are mandatory:
/// - @a target_name;
/// - @a profile_name;
/// - @a v_info.codec must not be @ref VIDEO_CODEC_NONE;
/// - @a packet_count;
/// - @a buffer_capacity.
///
/// If @a a_info.codec_type is @ref AUDIO_CODEC_NONE, then no audio is avalable.
/// @a packet_count is recommended to be equal to at least the GOP size.
/// @a buffer_capacity is recommended to be set the value that could fit in the GOP
/// (if @a packet_count equals the GOP size).
idip_status_code_t
idip_server_av_source_register(idip_server_t* self,
                               const idip_av_source_params_t* params,
                               idip_av_source_t** source);

/// Unregisters and destroys an audio/video source associated with an @c idip_av_source_t handle.
///
/// @param[in] self A pointer to an @c idip_server_t object to unregister the source from.
///                 Must not be @c NULL.
/// @param[in, out] source A pointer to an @c idip_av_source_t handle associated with the source
///                        to unregister and destroy.
///                        Must not be @c NULL.
///                        Will be set to @c NULL on success.
/// @return @c IDIP_OK if successful,
///         @c IDIP_INVALID_ARGUMENTS if invalid arguments are passed.
///
/// This call blocks until active audio/video streams exit.
idip_status_code_t
idip_server_av_source_unregister(idip_server_t* self, idip_av_source_t **source);

/// Replaces an existing audio/video source with a new one.
///
/// @param[in] self A pointer to an @c idip_server_t object to register the source in.
///                 Must not be @c NULL.
/// @param v_info Video stream specs.
///               <tt>v_info.codec</tt> must not be @ref VIDEO_CODEC_NONE.
/// @param a_info Audio stream specs.
///               If <tt>a_info.codec_type</tt> is @ref AUDIO_CODEC_NONE, no audio is avalable.
/// @param packet_count Number of packets to keep.
///                     Could be reached before the @a buffer_capacity.
///                     Must not be @c 0.
///                     It's recommended to set it to the GOP size.
/// @param buffer_capacity Total number of bytes to keep.
///                        Could be reached before the @a packet_count.
///                        Must not be @c 0.
///                        It's recommended to set it to a value that could fit in a GOP.
/// @param[in,out] source A pointer to a registered @c idip_av_source_t handle to replace.
///                       Must not be @c NULL.
///                       @c *source must point to an existing @c idip_av_source_t object
///                       which is registered by a previous call to @ref idip_server_av_source_register().
/// @return @c IDIP_OK if successful,
///         @c IDIP_INVALID_ARGUMENTS if invalid arguments are passed,
///         @c IDIP_GENERIC_ERROR if internal allocations has failed.
///
/// A source handle returned in the @a source argument is to be used to put audio/video data from the specified
/// target and profile in the @a target_name and the @a profile_name argument, respectively.
/// Replacing is performed by removing the old source from the source list and adding the new one
/// so existing audio/video receiving sessions could find it right after the receiving an EOS of
/// the removed source.

idip_status_code_t
idip_server_av_source_update(idip_server_t* self,
                             idip_av_source_t* source,
                             video_stream_info_t v_info,
                             audio_stream_info_t a_info,
                             idip_av_limits_t limits);


/// A simple file-based helper to implement a @ref idip_stream_persistent_data_get_handler_t.
///
/// @param[in] stream A pointer to stream to write persistent storage data.
/// Cannot be null.
/// @param persistent_file_path A path in a filesystem where the persistent storage
/// file will be.
/// @param storage_max A maximal amount of bytes that yo're allow to use
/// for persistent data storage.
///
/// @note The helper may be not optimal or not usable in some cases (like
/// read-only file systems). It implemented here to allow to fast create a
/// server prototype. We don't recommend to use this implementation
/// in production code.
idip_status_code_t
idip_persistent_data_get_default_implementation_helper(idip_stream_persistent_data_t* stream,
                                                const char* persistent_file_path,
                                                const size_t storage_max);

/// A simple file-based helper to implement a @ref idip_stream_persistent_data_set_handler_t
///
/// @param persistent_file_path A path in a filesystem where the persistent storage
/// file will be.
/// @param data A pointer to data blob to write to a file.
/// @param size Data blob size in bytes.
///
/// @note The helper may be not optimal or not usable in some cases (like
/// read-only file systems). It implemented here to allow to fast create a
/// server prototype. We don't recommend to use this implementation
/// in production code.
idip_status_code_t
idip_persistent_data_set_default_implementation_helper(const char* persistent_file_path,
                                                       const void *data, size_t size);

/// Helper function. Allows to parse json data from QR code by Ivideon Cloud.
/// JSON from QR-code is an array of 6 strings. String by index are:
/// [0] - cloud token
/// [1] - ssid
/// [2] - wifi password
/// [3] - camera name
/// [4,5,6] - unused
///
/// @param json - a string with json array.
/// @param json_len - length of @a json.
/// @param result - a pointer to a structure where the result will be placed.
///
/// @note all filelds od @a result are malloc()'ed strings. They they will be
/// freed after use to avoid memory leaks.
/// There is a convenient function @ref idip_parsed_token_destroy() for that.
int
idip_parse_cloud_qr_code_token_json(const char* json, int json_len,
                                    idip_parsed_token_t* result);


/// Helper function. Requires non empty value for an environment fariable.
/// If the variable wasn't set or contain an empty string the function
/// DOES abort() of the program. When the value exists the function
/// acts like gentenv(3).
///
/// @param envname - enviromnent variable name. Must not be NULL.
///
/// @note DO NOT use the function in production code. It is only for
/// Ivideon Embedded SDK examples.
char*
idip_requires_env(const char* envname, const char *description);

/// Helper function. Gets a value for an environment name
/// like the getenv(3) but prints additional warinig when
/// the environment wariable is emoty or isn't set.
char*
idip_wants_env(const char* envname, const char *description);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIBIDIP_H_INCLUDED */

