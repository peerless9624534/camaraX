#ifndef PLAYBACK_H
#define PLAYBACK_H

#if defined(USE_ALSA_BACKEND)

#include <alsa/asoundlib.h>

struct playback;
typedef struct playback playback_t;

#ifdef __cplusplus
extern "C" {
#endif


typedef struct audio_stream_info audio_stream_info_t;

playback_t* playback_new(const char *device_name);

void playback_delete(playback_t* self);

int playback_play(playback_t* self, const audio_stream_info_t* info,
                  const void* buf, unsigned long buflen);

#ifdef __cplusplus
}
#endif

#endif // USE_ALSA_BACKEND

#endif // PLAYBACK_H
