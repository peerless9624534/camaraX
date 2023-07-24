#ifndef IDIP_SYSTEM_PROXY_FILTER_H
#define IDIP_SYSTEM_PROXY_FILTER_H

#include <idip/idip_iovec.h>
#include <idip/details/system_proxy/fwds.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/// Result of filter operation
struct idip_sys_proxy_filter_res {
    /// Success flag
    bool ok;
    /// Data chunks vector (scatter/gather)
    idip_iovec_t * data;
    /// Len of data vector (count of chunks in data vector)
    size_t len;
};

typedef idip_sys_proxy_filter_res_t (*idip_sys_proxy_filter_func)(void * /*data*/,
    void * /*in_data*/, size_t /*in_size*/
);

/// Filter of a proxy stream. Each filter function accepts the input and produces
/// the output. The input can be empty, which means the filter function must return
/// data ready at the time of the function call (if any). The output also can be
/// empty, which means it is not ready yet. The output must remain valid either
/// until the next call of the filter function or until the call of stop() function.
/// If the filter function returns 'idip_proxy_filter_res_t.ok == false' then
/// session begins to terminate
/// All functions are called sequentially, i.e in a thread-safe serial manner
struct idip_sys_proxy_filter {
    /// Filter input data received from a service, i.e. processing chain looks
    /// as service -> |filter| -> peer. Function can return empty data
    idip_sys_proxy_filter_func from_svc;

    /// Filter input data received from a peer, i.e. processing chain looks as
    /// peer -> |filter| -> service
    idip_sys_proxy_filter_func to_svc;

    /// This function will be called once to setup the filter
    void (*setup)(void * /*data*/, idip_sys_proxy_session_t * /*sess*/);

    /// This function will be called once to stop the filter. It's guaranteed
    /// that at the time of the function call the output generated from the
    /// from_svc() and to_svc() functions is no longer used and can be safely
    /// deallocated
    void (*stop)(void * /*data*/);

    /// Filter own data. It is passed to filter functions as own_data param
    void * data;
};

void idip_sys_proxy_session_post_drain_filter_to_svc(idip_sys_proxy_session_t * self);
void idip_sys_proxy_session_post_drain_filter_from_svc(idip_sys_proxy_session_t * self);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // IDIP_SYSTEM_PROXY_FILTER_H
