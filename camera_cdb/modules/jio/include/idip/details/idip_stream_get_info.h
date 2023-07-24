#ifndef IDIP_STREAM_GET_INFO_H
#define IDIP_STREAM_GET_INFO_H

#include <idip.h>
#include <idip/details/idip_stream_base.h>

struct idip_session;

///
struct idip_stream_system_get_info
{
    // idip_stream_system_get_interfaces: public idip_stream_base
    struct idip_stream_base            base;
    // PAY ATTENTION! The handler IS SYSTEM_GET_INTERFACES type!
    // This because we have to get correct network information in the get_info data.
    idip_system_get_interfaces_handler_t handler;
    void*                                user_data;
};
typedef struct idip_stream_system_get_info idip_stream_system_get_info_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
idip_stream_system_get_info_init(idip_stream_system_get_info_t* self,
                                 uint64_t request_id,
                                 idip_stream_creation_params_t* init_params);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif // IDIP_STREAM_GET_INFO_H
