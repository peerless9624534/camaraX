#ifndef IDIP_TIMESPEC_H
#define IDIP_TIMESPEC_H

#include <time.h>
#include <assert.h>

#include <stdint.h>

static inline void idip_timespec_add_ms(struct timespec* ts, int64_t ms)
{
    assert(ts);
    assert(ms >= 0);
    if (ts && ms >= 0) {
        ts->tv_nsec += ms * 1000000l;
        if (ts->tv_nsec >= 1000000000l) {
            ts->tv_sec++;
            ts->tv_nsec -= 1000000000l;
        }
    }
}

#endif // IDIP_TIMESPEC_H
