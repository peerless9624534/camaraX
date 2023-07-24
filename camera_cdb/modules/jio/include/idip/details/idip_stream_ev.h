#ifndef IDIP_STREAM_EV_H
#define IDIP_STREAM_EV_H

#include <idip.h>
#include <idip/idip_vector.h>

struct idip_session;
struct idip_event_notifier {
    idip_vector_t* contexts;
    pthread_mutex_t contexts_lock;
};
typedef struct idip_event_notifier idip_event_notifier_t;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int idip_event_notifier_init(idip_event_notifier_t* self);

void idip_event_notifier_destroy(idip_event_notifier_t* self);


/// Send an event if the session has subscriptions for the event type.
///
/// @param self - a pointer to the @ref idip_event_notifier_t context. Must not be NULL.
/// @param event_name - event name string. Must not be NULL.
/// @param target - target name string. Can be NULL for device-wide events.
/// @param timestamp - event register time, milliseconds in UTC.
/// @param mpk_arg - optional event arguments.
/// @param mpk_arg_size - size of @a mpk_arg.
/// @param session - pointer to session/connection.
///
/// @return a number of sended events
int idip_event_notifier_notify(idip_event_notifier_t* self,
                                const char* event_name,
                                const char *target,
                                uint64_t timestamp,
                                const void *mpk_arg, 
                                size_t mpk_arg_size,
                                struct idip_session* session);

int idip_event_notify_add_watcher(idip_event_notifier_t* self,
                                  idip_ptr_vector_t *target_list,
                                  idip_ptr_vector_t *event_list,
                                  uint64_t request_id);

void idip_event_notify_remove_watchers(idip_event_notifier_t* self,
                                       uint64_t request_id,
                                       struct idip_session *session);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // IDIP_STREAM_EV_H
