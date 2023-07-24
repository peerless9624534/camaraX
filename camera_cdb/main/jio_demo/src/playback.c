#if defined(USE_ALSA_BACKEND)

#include "playback.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <alsa/asoundlib.h>

#include <idip.h>
#include <idip/idip_utils.h>

struct playback
{
    char* device_name;
    snd_output_t *output;
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *hwparams;
    snd_pcm_sw_params_t *swparams;
    snd_pcm_format_t format;    ///< sample format
    unsigned int rate;          ///< stream rate
    unsigned int channels;      ///< count of channels
    unsigned int buffer_time;   ///< ring buffer length in us
    unsigned int period_time;   ///< period time in us
    int resample;               ///< enable alsa-lib resampling
    int period_event;           ///< produce poll event after each period
    snd_pcm_sframes_t buffer_size;
    snd_pcm_sframes_t period_size;
    audio_stream_info_t idip_info;
};

//------------------------------------------------------------------------------
static int
playback_set_hwparams(playback_t* self, snd_pcm_access_t access)
{
    unsigned int rrate;
    snd_pcm_uframes_t size;
    int err, dir;
    /* choose all parameters */
    err = snd_pcm_hw_params_any(self->handle, self->hwparams);
    if (err < 0) {
        printf("Broken configuration for playback: no configurations available: %s\n", snd_strerror(err));
        return err;
    }
    /* set hardware resampling */
    err = snd_pcm_hw_params_set_rate_resample(self->handle, self->hwparams, self->resample);
    if (err < 0) {
        printf("Resampling setup failed for playback: %s\n", snd_strerror(err));
        return err;
    }
    /* set the interleaved read/write format */
    err = snd_pcm_hw_params_set_access(self->handle, self->hwparams, access);
    if (err < 0) {
        printf("Access type not available for playback: %s\n", snd_strerror(err));
        return err;
    }
    /* set the sample format */
    err = snd_pcm_hw_params_set_format(self->handle, self->hwparams, self->format);
    if (err < 0) {
        printf("Sample format not available for playback: %s\n", snd_strerror(err));
        return err;
    }
    /* set the count of channels */
    err = snd_pcm_hw_params_set_channels(self->handle, self->hwparams, self->channels);
    if (err < 0) {
        printf("Channels count (%u) not available for playbacks: %s\n", self->channels, snd_strerror(err));
        return err;
    }
    /* set the stream rate */
    rrate = self->rate;
    err = snd_pcm_hw_params_set_rate_near(self->handle, self->hwparams, &rrate, 0);
    if (err < 0) {
        printf("Rate %uHz not available for playback: %s\n", self->rate, snd_strerror(err));
        return err;
    }
    if (rrate != self->rate) {
        printf("Rate doesn't match (requested %uHz, get %iHz)\n", self->rate, err);
        return -EINVAL;
    }
    /* set the buffer time */
    err = snd_pcm_hw_params_set_buffer_time_near(self->handle, self->hwparams, &self->buffer_time, &dir);
    if (err < 0) {
        printf("Unable to set buffer time %u for playback: %s\n", self->buffer_time, snd_strerror(err));
        return err;
    }
    err = snd_pcm_hw_params_get_buffer_size(self->hwparams, &size);
    if (err < 0) {
        printf("Unable to get buffer size for playback: %s\n", snd_strerror(err));
        return err;
    }
    self->buffer_size = size;
    /* set the period time */
    err = snd_pcm_hw_params_set_period_time_near(self->handle, self->hwparams, &self->period_time, &dir);
    if (err < 0) {
        printf("Unable to set period time %u for playback: %s\n", self->period_time, snd_strerror(err));
        return err;
    }
    err = snd_pcm_hw_params_get_period_size(self->hwparams, &size, &dir);
    if (err < 0) {
        printf("Unable to get period size for playback: %s\n", snd_strerror(err));
        return err;
    }
    self->period_size = size;
    /* write the parameters to device */
    err = snd_pcm_hw_params(self->handle, self->hwparams);
    if (err < 0) {
        printf("Unable to set hw params for playback: %s\n", snd_strerror(err));
        return err;
    }
    return 0;
}
//------------------------------------------------------------------------------
static int
playback_set_swparams(playback_t* self)
{
    int err;
    /* get the current swparams */
    err = snd_pcm_sw_params_current(self->handle, self->swparams);
    if (err < 0) {
        printf("Unable to determine current swparams for playback: %s\n", snd_strerror(err));
        return err;
    }
    /* start the transfer when the buffer is almost full: */
    /* (buffer_size / avail_min) * avail_min */
    err = snd_pcm_sw_params_set_start_threshold(self->handle, self->swparams,
                                                (self->buffer_size / self->period_size) * self->period_size);
    if (err < 0) {
        printf("Unable to set start threshold mode for playback: %s\n", snd_strerror(err));
        return err;
    }
    /* allow the transfer when at least period_size samples can be processed */
    /* or disable this mechanism when period event is enabled (aka interrupt like style processing) */
    err = snd_pcm_sw_params_set_avail_min(self->handle, self->swparams,
                                          self->period_event ? self->buffer_size : self->period_size);
    if (err < 0) {
        printf("Unable to set avail min for playback: %s\n", snd_strerror(err));
        return err;
    }
    /* enable period events when requested */
    if (self->period_event) {
        err = snd_pcm_sw_params_set_period_event(self->handle, self->swparams, 1);
        if (err < 0) {
            printf("Unable to set period event: %s\n", snd_strerror(err));
            return err;
        }
    }
    /* write the parameters to the playback device */
    err = snd_pcm_sw_params(self->handle, self->swparams);
    if (err < 0) {
        printf("Unable to set sw params for playback: %s\n", snd_strerror(err));
        return err;
    }
    return 0;
}
//------------------------------------------------------------------------------
/// Underrun and suspend recovery
static int
xrun_recovery(snd_pcm_t *handle, int err)
{
    printf("stream recovery\n");
    if (err == -EPIPE) {    /* under-run */
        err = snd_pcm_prepare(handle);
        if (err < 0)
            printf("Can't recovery from underrun, prepare failed: %s\n", snd_strerror(err));
        return 0;
    } else if (err == -ESTRPIPE) {
        while ((err = snd_pcm_resume(handle)) == -EAGAIN)
            sleep(1);   /* wait until the suspend flag is released */
        if (err < 0) {
            err = snd_pcm_prepare(handle);
            if (err < 0)
                printf("Can't recovery from suspend, prepare failed: %s\n", snd_strerror(err));
        }
        return 0;
    }
    return err;
}
//------------------------------------------------------------------------------
/// Transfer method - write only
static int
playback_write_loop(playback_t* self, const signed short *ptr, int nsamples)
{
    int err, cptr = nsamples;
    while (cptr > 0) {
        err = snd_pcm_writei(self->handle, ptr, cptr);
        if (err == -EAGAIN)
            continue;
        if (err < 0) {
            if (xrun_recovery(self->handle, err) < 0) {
                printf("Write error: %s\n", snd_strerror(err));
                return -1;
            }
            break;  /* skip one period */
        }
        ptr += err * self->channels;
        cptr -= err;
    }
    return 0;
}

//------------------------------------------------------------------------------
static int playback_close(playback_t* self)
{
    if(!self->handle)
        return 0;

    snd_pcm_drain(self->handle);
    IDIP_DELETE(self->handle, snd_pcm_close);
    return 0;
}
//------------------------------------------------------------------------------
static int playback_open(playback_t* self)
{
    playback_close(self);

    int err = 0;
    if ((err = snd_pcm_open(&self->handle, self->device_name, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        printf("Playback open error: %s\n", snd_strerror(err));
        return 0;
    }

    if ((err = playback_set_hwparams(self, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        printf("Setting of hwparams failed: %s\n", snd_strerror(err));
        return -1;
    }
    if ((err = playback_set_swparams(self)) < 0) {
        printf("Setting of swparams failed: %s\n", snd_strerror(err));
        return -1;
    }
    snd_pcm_dump(self->handle, self->output);

//    size_t samples_size = (self->period_size * self->channels * snd_pcm_format_physical_width(self->format)) / 8;

    return 0;
}
//------------------------------------------------------------------------------
static inline void playback_setup_defaults(playback_t* self)
{
    self->format = SND_PCM_FORMAT_S16; /* sample format */
    self->rate = 44100;                /* stream rate */
    self->channels = 1;
    self->buffer_time = 500000;        /* ring buffer length in us */
    self->period_time = 100000;        /* period time in us */
    self->resample = 1;                /* enable alsa-lib resampling */
    self->period_event = 0;            /* produce poll event after each period */
}
//------------------------------------------------------------------------------
playback_t *playback_new(const char* device_name)
{
    playback_t* self = IDIP_NEW(*self);
    if( !self ) {
        printf("Can't allocate memory for playback: %s\n", strerror(errno));
        return NULL;
    }
    playback_setup_defaults(self);

    self->device_name = strdup(device_name);
    if( !self->device_name ) {
        printf("Can't copy device name: %s\n", strerror(errno));
        playback_delete(self);
        return NULL;
    }

    int err = snd_output_stdio_attach(&self->output, stdout, 0);
    if (err < 0) {
        printf("Output failed: %s\n", snd_strerror(err));
        playback_delete(self);
        return NULL;
    }
    if( (err = snd_pcm_hw_params_malloc(&self->hwparams)) < 0) {
        printf("Can't create playback's HW params: %s\n", snd_strerror(err));
        playback_delete(self);
        return NULL;
    }
    if( (err = snd_pcm_sw_params_malloc(&self->swparams)) < 0) {
        printf("Can't create playback's SW params: %s\n", snd_strerror(err));
        playback_delete(self);
        return NULL;
    }

    return self;
}
//------------------------------------------------------------------------------
void playback_delete(playback_t *self)
{
    if( !self ) return;

    playback_close(self);

    IDIP_DELETE(self->hwparams, snd_pcm_hw_params_free);
    IDIP_DELETE(self->swparams, snd_pcm_sw_params_free);
    IDIP_DELETE(self->output, snd_output_close);
    IDIP_FREE(self->device_name);
    free(self);
}
//------------------------------------------------------------------------------
static int is_supported_codec_type(audio_codec_t codec)
{
    switch(codec) {
    case AUDIO_CODEC_PCMU:
    case AUDIO_CODEC_PCMA:
    case AUDIO_CODEC_PCM16LE:
    case AUDIO_CODEC_PCMS16LE:
    case AUDIO_CODEC_PCMS16BE:
    case AUDIO_CODEC_PCM16:
        return 1;
    default:
        return 0;
    }
}
//------------------------------------------------------------------------------
static snd_pcm_format_t idip_sample_format_to_snd_format(audio_sample_format_t format)
{
    switch(format) {
//    case AUDIO_SAMPLE_FORMAT_NONE = 0,  ///< "none"
    case AUDIO_SAMPLE_FORMAT_INT8:  return SND_PCM_FORMAT_S8;
    case AUDIO_SAMPLE_FORMAT_UINT8: return SND_PCM_FORMAT_U8;
    case AUDIO_SAMPLE_FORMAT_INT16: return SND_PCM_FORMAT_S16;
    case AUDIO_SAMPLE_FORMAT_INT32: return SND_PCM_FORMAT_S32;
    case AUDIO_SAMPLE_FORMAT_FLOAT: return SND_PCM_FORMAT_FLOAT;
//    case AUDIO_SAMPLE_FORMAT_DOUBLE:

    case AUDIO_SAMPLE_FORMAT_UINT8P:    return SND_PCM_FORMAT_U8;
    case AUDIO_SAMPLE_FORMAT_INT16P:    return SND_PCM_FORMAT_S16;
    case AUDIO_SAMPLE_FORMAT_INT32P:    return SND_PCM_FORMAT_S32;
    case AUDIO_SAMPLE_FORMAT_FLOATP:    return SND_PCM_FORMAT_FLOAT;
//    case AUDIO_SAMPLE_FORMAT_DOUBLEP:
    default:
        return SND_PCM_FORMAT_UNKNOWN;
    }
}
//------------------------------------------------------------------------------
int playback_play(playback_t *self,
                  const audio_stream_info_t *info,
                  const void *buf,
                  unsigned long buflen)
{
    if( !self || !info || !buf || !buflen ) {
        errno = EINVAL;
        return -1;
    }

    int need_update = (0 != memcmp(&self->idip_info, info, sizeof(*info)));
    if( need_update ) {
        playback_close(self);

        if( !is_supported_codec_type(info->codec_type) ) {
            errno = EINVAL;
            return -1;
        }

        self->channels = info->channels;
        self->rate = info->sample_rate;
        self->format = idip_sample_format_to_snd_format(info->sample_format);

        if( playback_open(self) < 0 ) {
            return -1;
        }
        memcpy(&self->idip_info, info, sizeof(*info));
    }

    if( !self->handle ) {
        if( playback_open(self) < 0 ) {
            return -1;
        }
    }

    unsigned k = self->channels * 2;
    playback_write_loop(self, (const signed short*)buf, (int)(buflen/k));

    return 0;
}

#endif // USE_ALSA_BACKEND

