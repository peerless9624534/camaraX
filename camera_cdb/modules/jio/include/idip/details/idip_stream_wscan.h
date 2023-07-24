#ifndef IDIP_STREAM_WSCAN_H_INCLUDED
#define IDIP_STREAM_WSCAN_H_INCLUDED

#include <idip.h>
#include <idip/details/idip_stream_base.h>

struct idip_session;
struct idip_vector;

///
struct idip_stream_wireless_scan
{
    /// idip_stream_wscan : public idip_stream_base
    struct idip_stream_base     base;
    /// wireless scan specific
    idip_wireless_scan_handler_t handler;
    void*                       user_data;
    /// results of scan
    struct idip_vector*         results;
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int
idip_stream_wscan_init(idip_stream_wireless_scan_t* self,
                       uint64_t request_id,
                       idip_stream_creation_params_t* init_params);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IDIP_STREAM_WSCAN_H_INCLUDED */
