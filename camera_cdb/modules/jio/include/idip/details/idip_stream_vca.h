#ifndef IDIP_STREAM_VCA_H
#define IDIP_STREAM_VCA_H

#include <idip.h>
#include <idip/idip_vca_handler.h>
#include <idip/details/idip_stream_base.h>

struct idip_session;

typedef struct idip_stream_vca_args {
    idip_vca_processor_args_t processor;
    char* shared_memory_name;
}
idip_stream_vca_args_t;

// for each request_id we create its own reply context.
///
struct idip_stream_vca
{
    // idip_stream_av : public idip_stream_base
    struct idip_stream_base     base;
    // user specific
    idip_stream_vca_handler_t   handler;
    void*                       user_data;
    idip_vca_reader_t * reader;
    // query args
    idip_stream_vca_args_t      query_args;
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
idip_stream_vca_init(idip_stream_vca_t* self, uint64_t request_id,
                     idip_stream_creation_params_t* init_params,
                     idip_stream_vca_args_t* query_args);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // IDIP_STREAM_VCA_H
