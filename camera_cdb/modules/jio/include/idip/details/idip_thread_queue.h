#ifndef IDIP_THREAD_QUEUE_H
#define IDIP_THREAD_QUEUE_H

#include <stddef.h>

struct idip_thread_queue;
typedef struct idip_thread_queue idip_thread_queue_t;

typedef void (*queue_item_deleter_t)(void *item);

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Creates new thread save queue instance with given size hint.
 * @param items_per_chunk - count of items per chunk. Internally queue
 * does allocate blocks (chunks) of pointers to store queue items instead
 * per-item allocations. It does this for optimization purposes. If you don't
 * know how large chunks you need, use IDIP_QUEUE_ITEMS_PER_CHUNK_DEFAULT value.
 * See @ref idip_thread_queue_new().
 * @return Pointer to new queue's instance or NULL if errors.
 */
idip_thread_queue_t* idip_thread_queue_new_with_options(size_t items_per_chunk, queue_item_deleter_t item_deleter);

/**
 * @brief Does the same as @ref idip_queue_new_with_options() with
 * items_per_chunk = IDIP_QUEUE_ITEMS_PER_CHUNK_DEFAULT.
 * @return Pointer to new queue's instance or NULL if errors.
 */
idip_thread_queue_t* idip_thread_queue_new(queue_item_deleter_t item_deleter);

/**
 * @brief Delete queue
 * @param self - pointer to instance of idip_queue_t
 */
void idip_thread_queue_delete(idip_thread_queue_t* self);

/**
 * @brief Stop thread queue.
 * @param self - pointer to instance of queue
 *
 * @note Call of this method on empty queue is correct
 *
 */
void idip_thread_queue_stop(idip_thread_queue_t* self);

/**
 * @brief Checks that given queue is empty (contains no elements).
 * @param self - pointer to queue instance. Can't be NULL. With NULL value you
 * get an undefined behavior.
 * @retval 0 (FALSE) if queue is not empty
 * @retval 1 (TRUE) if queue is empty
 */
int idip_thread_queue_is_empty(idip_thread_queue_t* self);

/**
 * @brief Put new item into head of queue (enqueue).
 * @param self - pointer to instance of queue. Can't be NULL.
 * @param ptr - pointer to store in queue
 * @return count of inserted items. When function returns 0 no elements
 * was inserted.
 */
int idip_thread_queue_enqueue(idip_thread_queue_t* self, void* ptr);

/**
 * @brief Remove item from tail of queue and get it.
 * @param self - pointer to instance of queue
 *
 * @note Call of this method on empty queue is an undefined behavior and can
 * destroy queue's invariants. Use the method only with nonempty queue!
 *
 * @return pointer from tail of queue
 */
void *idip_thread_queue_dequeue(idip_thread_queue_t* self);

/**
 * @brief Wait item from tail of queue and get it.
 * @param self - pointer to instance of queue
 *
 * @note Call of this method on empty queue is correct wait new item
 *
 * @retval 0 (FALSE) if queue is empty
 * @retval 1 (TRUE) if queue is not empty
 */
int idip_thread_queue_wait(idip_thread_queue_t* self, long long wait_time_ms);

/**
 * @brief Replace items in the queue that are matches to a @a match criteria.
 * @param self - pointer to queue instance
 * @param replace - value to replace
 * @param match - pointer to function that determines a criteria.
 * @param udata - pointer to some user data. The pointer will be pass to
 * second argument of @a match function. The function must return nonzero
 * values for matched items, and zero otherwise.
 * @param release - pointer to replaced item release function.
 * @note Complexity is O(n).
 * @retval 0 if success, -1 if errors were occurred.
 */
int idip_thread_queue_replace_matched(idip_thread_queue_t* self,
                                     void* replace,
                                     int (*match)(void* item, void* udata),
                                     void* udata,
                                     void (*release)(void* item));

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // IDIP_THREAD_QUEUE_H
