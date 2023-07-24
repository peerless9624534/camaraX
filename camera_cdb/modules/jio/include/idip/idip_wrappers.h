/**
 * A collection of pthread wrappers/adaptors to use in libidip implementation.
 */
#ifndef IDIP_WRAPPERS_H
#define IDIP_WRAPPERS_H

#include <pthread.h>
#include <stdint.h>
#include <idip/idip_iovec.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void idip_mutex_init(pthread_mutex_t* mutex);
void idip_recursive_mutex_init(pthread_mutex_t* mutex);
void idip_mutex_destroy(pthread_mutex_t* mutex);
void idip_mutex_lock(pthread_mutex_t* mutex);
int idip_mutex_trylock(pthread_mutex_t* mutex);
void idip_mutex_unlock(pthread_mutex_t* mutex);

/**
 * @brief Initialize a condition variable and configure it to use CLOCK_MONOTONIC
 * instead CLOCK_REALTIME that used by default in pthreads.
 * @param cond - pointer to condition variable
 *
 * @note Because the condition variable uses CLOCK_MONOTONIC you have to use
 * an adaptor @ref idip_monotonic_ms_from_now() with the @ref idip_cond_timedwait()
 *
 * @note this function call abort() on initialization errors.
 */
void idip_cond_init(pthread_cond_t* cond);

/**
 * @brief Destroy @a cond variable that was initialized with @ref idip_cond_init()
 * @param cond - pointer to a condition variable
 */
void idip_cond_destroy(pthread_cond_t* cond);

int idip_cond_signal(pthread_cond_t *cond);
int idip_cond_broadcast(pthread_cond_t *cond);
void idip_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

/**
 * @brief Acts like pthread_cond_timedwait() for conditions that were initalized
 * with the @ref idip_cond_init(), but call abort() on errors.
 *
 * @param cond pointer to a condition variable to wait to.
 * @param mutex pointer to a mutex
 * @param abstime_monotonic - pointer to a struct timespec, that @b INITIALIZED
 * WITH CLOCK_MONOTONIC TIME!
 *
 * @note use the adaptor @ref idip_monotonic_ms_from_now() to initialize
 * the @a abstime_monotonic
 *
 * @note NEVER use this function with conditions initialized with PTHREAD_COND_INITIALIZER.
 *
 * @retval 0 on success (a conditin is in signalled state)
 * @retval EINTR when the waiting wass interrupted by a signal (spirious wakeup)
 * @retval ETIMEDOUT when a tmeout was done
 */
int idip_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex,
                        const struct timespec *abstime_monotonic);

/**
 * The same as pthread_set_name_np() if is available on the platform.
 * Else does nothing.
 */
int idip_pthread_setname(pthread_t thread_id, const char* name_max_16);

int idip_pthread_getname(pthread_t thread_id, char* name, size_t len);

const char* idip_thread_get_name(pthread_t thread_id, char* name, size_t len);

const char* idip_thread_make_name(char* buf, size_t len, uint64_t request_id, int method);

typedef struct idip_sendmsg_res {
    size_t sent;    /// bytes sent
    int ec;         /// errno
} idip_sendmsg_res_t;

struct sockaddr;

idip_sendmsg_res_t idip_sendmsg(int sock, struct sockaddr* addr,
                                size_t addr_len, idip_iovec_t* msg_iov,
                                size_t msg_iovlen, int flags);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // IDIP_WRAPPERS_H
