#ifndef IDIP_STREAM_PD_H_INCLUDED
#define IDIP_STREAM_PD_H_INCLUDED

#include <idip.h>
#include <idip/details/idip_stream_base.h>

struct idip_session;

/// Structure that represents arguments of persistent_data.set method
typedef struct pd_query_args
{
    void* pdata;
    size_t pdata_size;
}
pd_query_args_t;

/// Data writeback implementation method type.
/// The handler proxying data readed from user to libidip sender.
typedef idip_status_code_t
(*idip_stream_persistent_data_write_back_impl_t)(idip_stream_persistent_data_t* stream,
                                                 const void* pdata,
                                                 size_t pdata_size,
                                                 size_t pdata_max_size,
                                                 long timeout_ms);

///
struct idip_stream_persistent_data
{
    // idip_stream_persistence_data : public idip_stream_base
    struct idip_stream_base     base;
    // pointer to data put method
    idip_stream_persistent_data_write_back_impl_t  do_write_back;
    // persistence_data specific
    idip_stream_persistent_data_get_handler_t get_handler;
    idip_stream_persistent_data_set_handler_t set_handler;
    pd_query_args_t             query_args;
    void*                       user_data;
    // These fields will set to malloc()'ed buffer with a readed data.
    // Its using only for internal calls.
    char*                       b64_storage_data;
    size_t                      b64_storage_data_size;
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
idip_stream_pd_get_init(idip_stream_persistent_data_t* self, uint64_t request_id,
                        const idip_stream_creation_params_t *init_params,
                        const pd_query_args_t *query_args);

int
idip_stream_pd_set_init(idip_stream_persistent_data_t* self, uint64_t request_id,
                        const idip_stream_creation_params_t *init_params,
                        const pd_query_args_t* query_args);

struct base64_buf_t {
    char* buf;
    size_t len;
};

struct base64_buf_t
idip_stream_pd_move_base64_data(idip_stream_persistent_data_t* self);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IDIP_STREAM_PD_H_INCLUDED */
