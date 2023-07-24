#ifndef IDIP_STREAM_PASSWD_H_INCLUDED
#define IDIP_STREAM_PASSWD_H_INCLUDED

#include <idip.h>
#include <idip/details/idip_stream_base.h>

///
struct idip_stream_passwd
{
    /// idip_stream_passwd : public idip_stream_base
    struct idip_stream_base         base;
    /// password change specific
    idip_stream_passwd_handler_t    handler;
    void*                           user_data;
    idip_user_credentials_t       args;
};
typedef struct idip_stream_passwd idip_stream_passwd_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
idip_stream_passwd_init(idip_stream_passwd_t* self,
                        uint64_t request_id,
                        idip_stream_creation_params_t *init_params,
                        idip_user_credentials_t query_args);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IDIP_STREAM_PASSWD_H_INCLUDED */
