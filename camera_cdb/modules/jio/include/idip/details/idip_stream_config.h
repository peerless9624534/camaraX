#ifndef IDIP_STREAM_CONFIG_H
#define IDIP_STREAM_CONFIG_H

#include <idip.h>
#include <idip/details/idip_stream_base.h>
#include <idip/idip_vector.h>

#include <stdint.h>
#include <msgpack.h>

struct idip_session;

///
struct config_get_set_args {
    char* target;
    idip_ptr_vector_t* config; // map to idip_cfg_param_t**
};
typedef struct config_get_set_args config_get_set_args_t;

///
struct idip_stream_config
{
    struct idip_stream_base             base;
    idip_stream_config_get_handler_t    get_handler;
    idip_stream_config_update_handler_t set_handler;
    void*                               user_data;
    idip_ptr_vector_t*                  result;
    config_get_set_args_t               query_args;
};

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int idip_stream_config_get_init(idip_stream_config_t* self,
                            uint64_t request_id,
                            idip_stream_creation_params_t* init_params,
                            const config_get_set_args_t* query_args);

int idip_stream_config_set_init(idip_stream_config_t* self,
                                uint64_t request_id,
                                idip_stream_creation_params_t* init_params,
                                const config_get_set_args_t* query_args);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // IDIP_STREAM_CONFIG_H
