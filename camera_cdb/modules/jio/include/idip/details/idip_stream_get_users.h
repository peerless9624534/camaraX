#ifndef IDIP_STREAM_GET_USERS_H_INCLUDED
#define IDIP_STREAM_GET_USERS_H_INCLUDED

#include <idip.h>
#include <idip/details/idip_stream_base.h>

///
struct idip_stream_system_get_users
{
    // idip_stream_system_get_users : public idip_stream_base
    struct idip_stream_base                 base;
    // system.get_users specific
    idip_stream_system_get_users_handler_t  handler;
    void*                                   user_data;
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
idip_stream_get_users_init(idip_stream_system_get_users_t *self,
                           uint64_t request_id,
                           idip_stream_creation_params_t* init_params);

idip_ap_security_type_t security_from_str(const char* sec_type_str);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IDIP_STREAM_GET_USERS_H_INCLUDED */
