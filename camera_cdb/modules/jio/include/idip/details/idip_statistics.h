#ifndef IDIP_STATISTICS_H
#define IDIP_STATISTICS_H

#include <stddef.h>
#include <pthread.h>
#include <idip/details/idip_methods.h>

typedef struct idip_statistics idip_statistics_t;

#ifdef __cplusplus
extern "C" {
#endif

idip_statistics_t* idip_statistics_new();

void idip_statistics_delete(idip_statistics_t* self);

void idip_statistics_append_wop_stat(idip_statistics_t* self,
                                     pthread_t thread,
                                     long long creation_ts,
                                     long long now,
                                     size_t writed);

void idip_statistics_append_event(idip_statistics_t* self);

void idip_statistics_session_created(idip_statistics_t* self);

void idip_statistics_session_removed(idip_statistics_t* self);

void idip_statistics_stream_created(idip_statistics_t* self);

void idip_statistics_stream_removed(idip_statistics_t* self);

void idip_statistics_clear(idip_statistics_t* self);

void idip_statistics_remove(idip_statistics_t* self, pthread_t thread);

void idip_statistics_register_request(idip_statistics_t* self, long long now,
                                      enum method_t method, uint64_t rid);

int idip_statistics_print(idip_statistics_t* self, char* buf, size_t buf_size,
                          long long now);

#ifdef __cplusplus
}
#endif

#endif // IDIP_STATISTICS_H
