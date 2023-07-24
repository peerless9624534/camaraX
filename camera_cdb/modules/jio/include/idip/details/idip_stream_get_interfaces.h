#ifndef IDIP_STREAM_GET_INTERFACES_H
#define IDIP_STREAM_GET_INTERFACES_H

#include <idip.h>
#include <idip/details/idip_stream_base.h>

struct idip_session;
struct msgpack_packer;

///
struct idip_stream_system_get_interfaces
{
    // idip_stream_system_get_interfaces: public idip_stream_base
    struct idip_stream_base            base;
    // method specific
    idip_system_get_interfaces_handler_t handler;
    void*                                       user_data;
};
typedef struct idip_stream_system_get_interfaces idip_stream_system_get_interfaces_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
idip_stream_system_get_interfaces_init(idip_stream_system_get_interfaces_t* self,
                                       uint64_t request_id,
                                       idip_stream_creation_params_t* init_params);


void idip_pack_ifaces_info(const idip_iface_t* const iflist,
                           size_t iflist_size, struct msgpack_packer* pk);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif // IDIP_STREAM_GET_INTERFACES_H
