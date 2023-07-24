#ifndef AVPKT_SOURCE_H
#define AVPKT_SOURCE_H

#include <stddef.h>
#include <stdint.h>

#include <idip.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct avpkt {
    /// is key video frame flag
    int is_key_frame;
    /// Pointer to read-only video buffer data. NULL if data is absent.
    const idip_byte_t*  video;
    /// Video buffer size in bytes.
    size_t video_size;
    /// Pointer to read-only audio buffer data. NULL if data is absent.
    const idip_byte_t*  audio;
    /// Audio data size in bytes.
    size_t audio_size;
    /// Packet time stamp
    int64_t ts_ms;
} avpkt_t;

struct avpkt_source;
typedef struct avpkt_source avpkt_source_t;

avpkt_source_t* avpkt_source_new_from_file(const char* file_path, bool loop);

const char * avpkt_source_get_filename(avpkt_source_t* self);

int avpkt_source_start_play(avpkt_source_t* self, int64_t start_ts);

void avpkt_source_request_key_frame_safe(avpkt_source_t *self);

int64_t avpkt_source_get_next_frame_send_mon_time(avpkt_source_t *self);

int avpkt_source_get_next_packet(avpkt_source_t* self, avpkt_t* packet);

unsigned avpkt_source_get_fps(avpkt_source_t* self);

int64_t avpkt_source_get_duration_ms(const avpkt_source_t* self);

void avpkt_source_delete(avpkt_source_t* self);

int avpkt_source_video_info(avpkt_source_t* self, video_stream_info_t* v_info);

int avpkt_source_audio_info(avpkt_source_t* self, audio_stream_info_t* a_info);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // AVPKT_SOURCE_H
