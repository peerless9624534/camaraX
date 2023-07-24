#ifndef IDIP_STREAM_NOTIFY_STATE_H_INCLUDED
#define IDIP_STREAM_NOTIFY_STATE_H_INCLUDED

#include <idip.h>
#include <idip/details/idip_stream_base.h>

struct idip_session;

///
struct idip_stream_system_notify
{
    // idip_stream_sotify_state : public idip_stream_base
    struct idip_stream_base            base;
    // method specific
    idip_stream_system_notify_handler_t       handler;
    idip_system_notify_query_args_t    query_args;
    void*                              user_data;
};
typedef struct idip_stream_system_notify idip_stream_system_notify_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
idip_stream_system_notify_init(idip_stream_system_notify_t* self,
                               uint64_t request_id,
                               idip_stream_creation_params_t* init_params,
                               const idip_system_notify_query_args_t *query_args);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IDIP_STREAM_NOTIFY_STATE_H_INCLUDED */
