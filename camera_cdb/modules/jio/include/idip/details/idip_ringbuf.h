#ifndef IDIP_RINGBUF_H
#define IDIP_RINGBUF_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct idip_ringbuf idip_ringbuf_t;

/**
 * All items in the buffer should inherit the sturcture as a first field!
 * @example
 * struct my_ringbuf_item {
 *      idip_ringbuf_item_base_t base;
 *      // other fields
 *      ...
 * };
 */
typedef struct idip_ringbuf_item_base 
{
    uint64_t  id;
} 
idip_ringbuf_item_base_t;

/**
 * @brief An @c idip_ringbuf_t forward const iterator.
 */
typedef struct idip_ringbuf_iterator idip_ringbuf_iterator_t;

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/**
 * @brief Allocates a new @a idip_ringbuf_t container object capable to store
 *        a @a capacity number of objects of @a item_size bytes each.
 * @param[out] self A pointer to an @c idip_ringbuf_t pointer to assign a new container object address to.
 *                  Must not be @c NULL.
 * @param capacity Number of objects to store.
 *                 Must not be @c 0.
 * @param item_size Size of a single object to store.
 *                  Must not be @c 0.
 * @return @c 0 if successful,
 *         @c EINVAL if invalid arguments are passed,
 *         @c ENOMEM if memory allocation has failed.
 *
 * @note The @ref idip_ringbuf_t object is reference counted. Remove the
 * object using @ref idip_ringbuf_unref().
 * @note Each iterator contains a reference to the buffer.
 */
int idip_ringbuf_new(idip_ringbuf_t** self, size_t capacity, size_t item_size);

/**
 * @brief Increases reference counter to the object.
 * @param self A pointer to an @c idip_ringbuf_t object.
 * Can be NULL (does nothing in the case).
 * @return The same pointes as @a self.
 */
idip_ringbuf_t* idip_ringbuf_add_ref(idip_ringbuf_t* self);

/**
 * @brief Decreases reference counter to the underline object. When the counter
 * reaches 0 value, then the object will destroyed.
 * @param self A pointer to an @c idip_ringbuf_t object.
 */
void idip_ringbuf_unref(idip_ringbuf_t* self);

/**
 * @brief Resizes an @a idip_ringbuf_t container object to a @a capacity number of items keeping the @a item_size.
 * @param[in] self A pointer to an @c idip_ringbuf_t pointer to assign a new container object address to.
 *                 Must not be @c NULL.
 * @param capacity New item capacity.
 *                 Must not be @c 0.
 * @return @c 0 if successful,
 *         @c EINVAL if invalid arguments are passed,
 *         @c ENOMEM if memory allocation has failed.
 */
int idip_ringbuf_resize(idip_ringbuf_t* self, size_t capacity);

/**
 * @brief Puts a new item to an @a idip_ringbuf_t container.
 * @param[in] self A pointer to an @a idip_ringbuf_t container to put the item to.
 *                 Must not be @a NULL.
 * @param[in] item A pointer to the item to put.
 *                 Must not be @a NULL.
 * @return @a 0 if successful,
 *         @a EAGAIN if container is full,
 *         @a EINVAL if @a self or @a item is @a NULL.
 *
 * Successful writing marks the corresponding item slot occupied.
 */
int idip_ringbuf_write(idip_ringbuf_t* self, const void *item);

/**
 * @brief Reads and pops a stored item from an @a idip_ringbuf_t container.
 * @param[in] self A pointer to the @a idip_ringbuf_t container to get the item from.
 *                 Must not be @a NULL.
 * @param[out] item A pointer to a buffer to read the item into.
 *                  Must not be @a NULL.
 *                  Must be of @a item_size bytes size (@see idip_ringbuf_new()).
 * @return @a 0 if successful,
 *         @a EAGAIN if container is empty,
 *         @a EINVAL if @a self or @a item is @a NULL.
 *
 * Successful reading marks the corresponding item slot vacant.
 */
int idip_ringbuf_read(idip_ringbuf_t* self, void *item);

/**
 * @brief Reads a stored item from an @a idip_ringbuf_t container.
 * @param[in] self A pointer to the @a idip_ringbuf_t container to peek the item from.
 *                 Must not be @c NULL.
 * @param[out] item A pointer to a buffer to read the item into.
 *                  Must not be @a NULL.
 *                  Must be of @a item_size bytes size (@see idip_ringbuf_new()).
 * @return @a 0 if successful,
 *         @a EAGAIN if container is empty,
 *         @a EINVAL if @a self or @a item is @a NULL.
 *
 * Successful peeking keeps the corresponding item slot occupied.
 */
int idip_ringbuf_peek(const idip_ringbuf_t* self, void *item, uint64_t *node_id);

/**
 * @brief Clears an @a idip_ringbuf_t container.
 * @param[in] self A pointer to the @a idip_ringbuf_t container to clear.
 *                 Must not be @a NULL.
 * @return @a 0 if successful,
 *         @a EINVAL if @a self is @a NULL.
 *
 * Effectively, it sets inner reading and writing positions to @a 0,
 * so any deallocations must be done outside.
 */
int idip_ringbuf_clear(idip_ringbuf_t* self);

/**
 * @brief Returns a number of items stored in an @a idip_ringbuf_t container.
 * @param[in] self A pointer to an @a idip_ringbuf_t container to return the size of.
 *                 Must not be @a NULL.
 * @param[out] size A pointer to an @a size_t variable to store the container size to.
 *                  Must not be @a NULL.
 * @return @a 0 if successful,
 *         @a EINVAL if @a self or @a size is @a NULL.
 */
int idip_ringbuf_size(const idip_ringbuf_t* self, size_t* size);

/**
 * @brief Returns a maximal number of items tat could be stored in an @a idip_ringbuf_t container.
 * @param[in] self A pointer to an @a idip_ringbuf_t container to return the size of.
 *                 Must not be @a NULL.
 * @param[out] size A pointer to an @a size_t variable to store the container size to.
 *                  Must not be @a NULL.
 * @return @a 0 if successful,
 *         @a EINVAL if @a self or @a size is @a NULL.
 */
int idip_ringbuf_capacity(const idip_ringbuf_t* self, size_t* size);

/**
 * @brief Tests whether an @a idip_ringbuf_t container is empty.
 * @param[in] self A pointer to an @a idip_ringbuf_t container to test.
 *                 Must not be @a NULL.
 * @return @a true if @a self is not @a NULL an the pointed object contains no items,
 *         @a false, otherwise.
 */
bool idip_ringbuf_empty(const idip_ringbuf_t* self);

/**
 * @brief Tests whether an @a idip_ringbuf_t container is full.
 * @param[in] self A pointer to an @a idip_ringbuf_t container to test.
 *                 Must not be @a NULL.
 * @return @a true if @a self is not @a NULL and the pointed object is full,
 *         @a false, otherwise.
 */
bool idip_ringbuf_full(const idip_ringbuf_t* self);

/**
 * @brief Shifts an @a idip_ringbuf_t object current item reading position by 1 item.
 * @param[in] self A pointer to an @a idip_ringbuf_t container to pop an item from.
 *                 Must not be @a NULL.
 * @return @a 0 if successful,
 *         @a EAGAIN if container is empty,
 *         @a EINVAL if @a self is @a NULL.
 */
int idip_ringbuf_pop(idip_ringbuf_t* self);

/**
 * @brief Creates and initializes an @c idip_ringbuf_iterator_t object pointing to an @c idip_ringbuf_t begin.
 * @param[in] self A pointer to an @c idip_ringbuf_t container to get the begin iterator to.
 *                 Must not be @c NULL.
 * @param[out] iterator A pointer to an @c idip_ringbuf_iterator_t pointer to store the address
 *                      of a new iterator to.
 *                      Must not be @c NULL.
 * @return @c 0 if successful,
 *         @c EINVAL if @a self or @a iterator is @c NULL,
 *         @c ENOMEM if allocation of a new iterator object has failed.
 */
int idip_ringbuf_begin(idip_ringbuf_t *self, idip_ringbuf_iterator_t** iterator);

/**
 * @brief Creates and initializes an @c idip_ringbuf_iterator_t object pointing to an @c idip_ringbuf_t end.
 * @param[in] self A pointer to an @c idip_ringbuf_t container to get the end iterator to.
 *                 Must not be @c NULL.
 * @param[out] iterator A pointer to an @c idip_ringbuf_iterator_t pointer to store the address
 *                      of a new iterator to.
 *                      Must not be @c NULL.
 * @return @c 0 if successful,
 *         @c EINVAL if @a self or @a iterator is @c NULL,
 *         @c ENOMEM if allocation of a new iterator object has failed.
 */
int idip_ringbuf_end(idip_ringbuf_t *self, idip_ringbuf_iterator_t** iterator);

/**
 * @brief Destroys and frees an @c idip_ringbuf_iterator_t object.
 * @param[in] self A pointer to the @c idip_ringbuf_iterator_t object to destroy and free.
 *                 If @c NULL, the function is no-op.
 */
void idip_ringbuf_iterator_delete(idip_ringbuf_iterator_t* self);

/**
 * @brief Test whether an @c idip_ringbuf_iterator_t object points to a valid
 * READ position, that can be dereferenced.
 * So an iterator to an empty buffer isn't valid.
 * @param self A pointer to an @c idip_ringbuf_iterator_t object to test.
 *             Must not be @c NULL.
 * @return @c true if @a self is not @c NULL and points to a valid @c idip_ringbuf_t position,
 *         @c false, otherwise.
 */
bool idip_ringbuf_iterator_is_valid(const idip_ringbuf_iterator_t* self);

/**
 * @brief Returns a pointer to an item under an @a idip_ringbuf_iterator_t object.
 * @param[in] self A pointer to the @a idip_ringbuf_iterator_t object to use to get an item pointer.
 *                 Must not be @c NULL.
 * @param[out] item A pointer to pointer to store the item address.
 *                  Must not be @a NULL.
 * @return @c 0 if successful,
 *         @c EAGAIN if container is empty,
 *         @c EINVAL if @a self or @a item is @a NULL,
 *         @c EFAULT if @a self is invalid.
 */
int idip_ringbuf_iterator_get_pointer(const idip_ringbuf_iterator_t* self, const void **item);

/**
 * @brief Reads a item pointed by an @a idip_ringbuf_iterator_t object.
 * @param[in] self A pointer to the @a idip_ringbuf_iterator_t object.
 *                 Must not be @c NULL.
 * @param[out] item A pointer to a buffer to read the item into.
 *                  Must not be @a NULL.
 *                  Must be of @a item_size bytes size (@see idip_ringbuf_new()).
 * @param [out] uid An item serial identifier in the queue. Can be NULL.
 * @return @c 0 if successful,
 *         @c EAGAIN if container is empty,
 *         @c EFAULT if @a self was invalidated since the last access,
 *         @c EINVAL if @a self or @a item is @a NULL.
 */
int idip_ringbuf_iterator_dereference(const idip_ringbuf_iterator_t *self, void *item, uint64_t* uid);

/**
 * @brief Advances a valid @c idip_ringbuf_iterator_t object position.
 * @param[in,out] self A pointer to an @c idip_ringbuf_iterator_t to advance.
 *                     Must not be @c NULL.
 * @return @c 0 if successful,
 *         @c EINVAL if @a self is @c NULL,
 *         @c EAGAIN if @a self is already at end.
 * @warning The new position could point to a container end, test it before dereferencing
 *          (@see idip_ringbuf_iterator_is_at_end()).
 */
int idip_ringbuf_iterator_advance(idip_ringbuf_iterator_t* self);

/**
 * @brief Tests whether two @c idip_ringbuf_iterator_t object point to the same item.
 * @param[in] lhs A pointer to the first @c idip_ringbuf_iterator_t object.
 *                Must not be @c NULL.
 * @param[in] rhs A pointer to the second @c idip_ringbuf_iterator_t object.
 *                Must not be @c NULL.
 * @return @c true if @a lhs and @a rhs are not @c NULL and point to the same item,
 *         @c false, otherwise.
 */
bool idip_ringbuf_iterator_equal(const idip_ringbuf_iterator_t* lhs, const idip_ringbuf_iterator_t* rhs);

/**
 * @brief Tests whether an @c idip_ringbuf_iterator_t object points to an @c idip_ringbuf_t container's end.
 * @param self A pointer to an @c idip_ringbuf_iterator_t to test.
 * @return @c true if @c self is not @c NULL and points to the container's end,
 *         @c false, otherwise.
 */
bool idip_ringbuf_iterator_is_at_end(const idip_ringbuf_iterator_t* self);


int idip_ringbuf_find_if(const idip_ringbuf_t* self,
                         idip_ringbuf_iterator_t** iterator,
                         int (*compare)(void* udata, const void* item),
                         void *udata);

size_t idip_ringbuf_item_size(const idip_ringbuf_t* self);

void idip_ringbuf_iterator_log_numbers(const idip_ringbuf_iterator_t* self);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // IDIP_RINGBUF_H
