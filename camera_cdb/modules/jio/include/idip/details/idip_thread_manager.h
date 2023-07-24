#ifndef IDIP_THREAD_MANAGER_H
#define IDIP_THREAD_MANAGER_H

#include <pthread.h>
#include <idip/details/idip_methods.h>

struct idip_stream_base;

struct idip_thread_manager;
typedef struct idip_thread_manager idip_thread_manager_t;

typedef struct idip_thread_strategy {
    // Thread id
    pthread_t           thread_id;
    // Thread was joined flag (used by GC only)
    int thread_joined;
    // These members are initialized by idip_thread_manager factory.
    // Start an asynchronous operation in a thread
    int (*start_fn)(struct idip_stream_base *self,
                    const char* thread_name,
                    void *(*start_routine)(void *),
                    void *arg);
    // Wait for asynchronous operation done. After this call
    // the object is able to free.
    int (*join_fn)(struct idip_thread_strategy* self);
    // Gets actual thread_id
    int (*thread_id_fn)(struct idip_thread_strategy* self, pthread_t* id);
    // Some thread manager data
    void* tm_data;
}
idip_thread_strategy_t;

#ifdef __cplusplus
extern "C" {
#endif

idip_thread_manager_t *idip_thread_manager_new();

void idip_thread_manager_delete(idip_thread_manager_t* self);

int idip_thread_manager_start(idip_thread_manager_t *self);

int idip_thread_manager_stop(idip_thread_manager_t* self);

int
idip_thread_manager_initialize_threading_strategy_p(idip_thread_manager_t *self,
                                                    idip_run_policy_t policy,
                                                    const char *queue_name,
                                                    idip_thread_strategy_t *thread_strategy);

int idip_thread_strategy_start_thread(struct idip_stream_base *stream,
                                      const char *thread_name,
                                      void *(*task_routine)(void *),
                                      void *task_arg);

int idip_thread_strategy_join(idip_thread_strategy_t *self, uint64_t request_id, uint64_t sequence_id);

int idip_thread_strategy_get_thread_id(idip_thread_strategy_t* self, pthread_t* id_ptr);

#ifdef __cplusplus
}
#endif

#endif // IDIP_THREAD_MANAGER_H
