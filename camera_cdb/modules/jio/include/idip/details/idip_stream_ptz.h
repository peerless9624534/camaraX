#ifndef IDIP_STREAM_PTZ_H_INCLUDED
#define IDIP_STREAM_PTZ_H_INCLUDED

#include <idip.h>
#include <idip/details/idip_stream_base.h>

struct idip_session;

///
struct idip_stream_ptz
{
    /// idip_stream_ptz : public idip_stream_base
    struct idip_stream_base     base;
    /// method specific
    idip_ptz_handler_t handler;
    void*              user_data;
    char*              target;
    idip_ptz_command_t cmd;
};
typedef struct idip_stream_ptz idip_stream_ptz_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
idip_stream_ptz_init(idip_stream_ptz_t* self,
                     uint64_t request_id,
                     idip_stream_creation_params_t* init_params,
                     const idip_ptz_command_t *query_args);

ptz_command_type_t ptz_command_type_from_str(const char* cmd);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IDIP_STREAM_PTZ_H_INCLUDED */
