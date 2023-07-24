#ifndef IDIP_STREAM_SNAPSHOT_H
#define IDIP_STREAM_SNAPSHOT_H

#include <idip.h>
#include <idip/details/idip_stream_base.h>

struct idip_session;

// for each request_id we create its own reply context.
///
struct idip_stream_snapshot
{
    // idip_stream_snapshot : public idip_stream_base
    struct idip_stream_base     base;
    // method specific
    idip_stream_snapshot_handler_t    handler;
    void*                       user_data;
    // query args
    idip_stream_snapshot_args_t  query_args;
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
idip_stream_snapshot_init(idip_stream_snapshot_t* self, uint64_t request_id,
                         idip_stream_creation_params_t* init_params,
                         idip_stream_snapshot_args_t* query_args);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // IDIP_STREAM_SNAPSHOT_H
