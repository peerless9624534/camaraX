#ifndef IDIP_AV_LIMITS_H_INCLUDED
#define IDIP_AV_LIMITS_H_INCLUDED

#include <stddef.h>

/// A data structure to set/get buffering limits.
typedef struct idip_av_limits
{
    size_t      size_limit;     ///< A buffer hard size limit in bytes
    long long   time_limit_ms;  ///< A buffer frame hold time limit in milliseconds.
}
idip_av_limits_t;

#define IDIP_AV_LIMITS_INIT(vbitrate_kbps, abitrate_kbps, buffer_time_sec) \
    (idip_av_limits_t){ \
        .time_limit_ms = (buffer_time_sec) * 1000, \
        .size_limit = (((vbitrate_kbps) + (abitrate_kbps)) * 1024L * (buffer_time_sec) / 8) * 1.1 \
    }

#ifdef __cplusplus
extern "C" {
#endif

int idip_av_limits_init_helper(idip_av_limits_t* limits,
                               unsigned vbitrate, unsigned abitrate,
                               unsigned long buffer_time_sec);

#ifdef __cplusplus
}
#endif

#endif /* IDIP_AV_LIMITS_H_INCLUDED */

