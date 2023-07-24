#ifndef IDIP_TO_STR_H
#define IDIP_TO_STR_H

#include <idip.h>

#ifdef __cplusplus
extern "C" {
#endif

const char* video_codec_str(video_codec_t codec, const char* def);

const char* audio_codec_str(audio_codec_t codec, const char* def);

audio_codec_t audio_codec_from_str(const char* str);

const char* audio_sample_format_str(audio_sample_format_t format, const char* def);

audio_sample_format_t audio_sample_format_from_str(const char* str);

const char* speed_factor_str(archive_play_speed_t speed, const char* def);

archive_play_speed_t speed_factor_from_str(const char* str);

archive_play_mode_t play_mode_from_str(const char* str);

const char* ptz_command_type_str(ptz_command_type_t cmd, const char* def);

ptz_command_type_t ptz_command_type_from_str(const char* cmd);

const char* ap_security_to_str(idip_ap_security_type_t type, const char* stub);

#ifdef __cplusplus
}
#endif

#endif // IDIP_TO_STR_H
