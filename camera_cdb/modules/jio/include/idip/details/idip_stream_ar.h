#ifndef IDIP_STREAM_AR_H
#define IDIP_STREAM_AR_H

#include <idip.h>
#include <idip/details/idip_stream_base.h>

#include <stdint.h>

struct idip_session;
///
struct archive_get_args {
    char*                           target;
    idip_archive_play_params_t      play_params;
};
typedef struct archive_get_args archive_get_args_t;

///
struct archive_get_init_params {
    IDIP_STREAM_BASE_INIT_PARAMS_DECL
    idip_stream_archive_handler_t handler;
    void *user_data;
};
typedef struct archive_get_init_params archive_get_init_params_t;

///
struct idip_stream_archive
{
    // idip_stream_ar : public idip_stream_base
    struct idip_stream_base         base;
    // video.get specific
    idip_stream_archive_handler_t   handler;
    void*                           user_data;
    // query args
    archive_get_args_t              query_args;
};

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int idip_stream_archive_init(idip_stream_archive_t* self,
                             uint64_t request_id,
                             idip_stream_creation_params_t* init_params,
                             const archive_get_args_t* query_args);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // IDIP_STREAM_AR_H
