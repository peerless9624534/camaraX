#ifndef IDIP_STREAM_WSETUP_H_INCLUDED
#define IDIP_STREAM_WSETUP_H_INCLUDED

#include <idip.h>
#include <idip/details/idip_stream_base.h>

struct idip_session;

///
struct idip_stream_wireless_setup
{
    /// idip_stream_wireless_setup : public idip_stream_base
    struct idip_stream_base     base;
    /// wireless setup specific
    idip_wireless_setup_handler_t handler;
    void*                       user_data;
    idip_ap_connect_parameters_t connect_opts;
    // mutex and contition to wait for result
    pthread_mutex_t             result_lock;
    pthread_cond_t              result_cond;
    int                         result_ready;
    int                         result;
};
typedef struct idip_stream_wireless_setup idip_stream_wireless_setup_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
idip_stream_wsetup_init(idip_stream_wireless_setup_t* self,
                        uint64_t request_id,
                        idip_stream_creation_params_t* init_params,
                        const idip_ap_connect_parameters_t *query_args);

idip_ap_security_type_t security_from_str(const char* sec_type_str);

int idip_stream_wsetup_wait_for_result(idip_stream_wireless_setup_t* self);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IDIP_STREAM_WSETUP_H_INCLUDED */
