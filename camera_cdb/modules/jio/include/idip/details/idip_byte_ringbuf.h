#ifndef IDIP_BYTE_RINGBUF_H
#define IDIP_BYTE_RINGBUF_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <idip/idip_data_types.h>
#include <idip/idip_iovec.h>

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/**
 * @brief A byte ring container type.
 * @warning Doesn't support bytes invalidation so it should be tracked outside.
 */
typedef struct idip_byte_ringbuf idip_byte_ringbuf_t;

/**
 * @brief An @c idip_byte_ringbuf_t forward const iterator.
 * @warning The stored position validity should be tracked outside.
 */
typedef struct idip_byte_ringbuf_iterator idip_byte_ringbuf_iterator_t;

/**
 * @brief Allocates a new @a idip_byte_ringbuf_t container capable to store @a capacity number of bytes.
 * @param[out] self A pointer to an @c idip_byte_ringbuf_t pointer to store the address of the container object to.
 *                  Must not be @c NULL.
 * @param capacity Byte capacity of the new @a idip_byte_ringbuf_t container object.
 *                 Must not be @c 0
 * @return @c 0 if successful,
 *         @c EINVAL if invalid arguments are passed,
 *         @c ENOMEM in case of allocaton failures.
 */
int idip_byte_ringbuf_new(idip_byte_ringbuf_t** self, size_t capacity);

/**
 * @brief Destroys and frees an @a idip_byte_ringbuf_t container.
 * @param[in] self A pointer to an @a idip_byte_ringbuf_t container to destroy and free.
 *                 If @a NULL, the function is no-op.
 */
void idip_byte_ringbuf_delete(idip_byte_ringbuf_t* self);

/**
 * @brief Swaps two @c idip_byte_ringbuf_t objects.
 * @param self A pointer to an @c idip_byte_ringbuf_t object to swap.
 *             Must not be @c NULL.
 * @param other A pointer to an @c idip_byte_ringbuf_t object to swap with.
 *              Must not be @c NULL.
 * @return @c 0 if successful,
 *         @c EINVAL if @a self or @a other is @c NULL.
 */
int idip_byte_ringbuf_swap(idip_byte_ringbuf_t* self, idip_byte_ringbuf_t* other);

/**
 * @brief Resizes an @a idip_byte_ringbuf_t container capable to a @a capacity number of bytes.
 * @param[in] self A pointer to an @c idip_byte_ringbuf_t container to resize.
 *                 Must not be @c NULL.
 * @param capacity New byte capacity of the @a idip_byte_ringbuf_t container.
 *                 Must not be @c 0
 * @return @c 0 if successful,
 *         @c EINVAL if invalid arguments are passed,
 *         @c ENOMEM in case of allocaton failures.
 */
int idip_byte_ringbuf_resize(idip_byte_ringbuf_t* self, size_t capacity);

/**
 * @brief Stores @a size number of bytes in an @a idip_byte_ringbuf_t container.
 * @param[in] self A pointer to an @a idip_byte_ringbuf_t container to store bytes in.
 *                 Must not be @a NULL.
 * @param[in] data A pointer to bytes to store.
 *                 Must not be @a NULL.
 * @param size Number of bytes to store.
 *             Must be less than the capacity of the container object.
 * @return @a 0 if successful,
 *         @a EINVAL if @a self, or @a data is @a NULL, or @a size exceeds the capacity of the container,
 *         @a EAGAIN if container is full.
 *
 * Doesn't rewrite unread data to keep user iterators valid.
 */
int idip_byte_ringbuf_write(idip_byte_ringbuf_t* self, const void* data, size_t size);

/**
 * @brief Reads and pops stored bytes from an @a idip_byte_ringbuf_t container.
 * @param[in] self A pointer to an @a idip_byte_ringbuf_t container to read and pop bytes from.
 *                 Must not be @a NULL.
 * @param[out] buffer A pointer to an object to store bytes to.
 *                    Must not be @a NULL if the @a size argument greater than @a 0.
 * @param size Number of bytes to read.
 *             Must be less than or equal to the capacity of the container object.
 * @return @a 0 if successful,
 *         @a EINVAL if @a self or @a buffer argument is @a NULL,
 *         @a EAGAIN if container is empty or contains less than @a size number of bytes.
 */
int idip_byte_ringbuf_read(idip_byte_ringbuf_t* self, idip_byte_t* buffer, size_t size);

/**
 * @brief Reads stored bytes from an @a idip_byte_ringbuf_t container.
 * @param self A pointer to an @a idip_byte_ringbuf_t container to read bytes from.
 *             Must not be @a NULL.
 * @param buffer A pointer to an object to store bytes to.
 *               Must not be @a NULL.
 * @param size Number of bytes to peek.
 *             mUST be less than or equal to the capacity of the container object.
 * @return @a 0 if successful,
 *         @a EINVAL if @a self or @a buffer argument is @a NULL,
 *         @a EAGAIN if container is empty or contains less than @a size number of bytes.
 *
 * Doesn't pop bytes from the container object.
 */
int idip_byte_ringbuf_peek(const idip_byte_ringbuf_t* self, idip_byte_t* buffer, size_t size);

/**
 * @brief Returns an array of continuous byte areas available to read from an
 *        @a idip_byte_ringbuf_t container.
 * @param self A pointer to an @a idip_byte_ringbuf_t container to read bytes from.
 *             Must not be @a NULL.
 * @param vec  A result array
 *
 * Doesn't pop or copy bytes from the container object.
 */
void idip_byte_ringbuf_peek_iovec(const idip_byte_ringbuf_t* self, idip_iovec_t vec[2]);

/**
 * @brief Clears an @a idip_byte_ringbuf_t container.
 * @param[in] self A pointer to an @a idip_byte_ringbuf_t object to clear.
 *                 Must not be @a NULL.
 * @return @a 0 if successful,
 *         @a EINVAL if @a self is @a NULL.
 *
 * Effectively, it sets inner reading and writing positions to @a 0.
 */
int idip_byte_ringbuf_clear(idip_byte_ringbuf_t* self);

/**
 * @brief Returns number of bytes stored in an @a idip_byte_ringbuf_t container.
 * @param[in] self A pointer to an @a idip_byte_ringbuf_t container to return the size of.
 *                 Must not be @a NULL.
 * @param size Pointer to a @a size_t variable to store the size to.
 *             Must not be @a NULL.
 * @return @a 0 if successful,
 *         @a EINVAL if @a self or @a size is @a NULL.
 */
int idip_byte_ringbuf_size(const idip_byte_ringbuf_t* self, size_t* size);

/**
 * @brief Returns number of bytes stored in an @a idip_byte_ringbuf_t container.
 * @param[in] self A pointer to an @a idip_byte_ringbuf_t container to return the capacity of.
 *                 Must not be @a NULL.
 * @param[out] capacity Pointer to a @a size_t variable to store the capacity of container to.
 *                      Must not be @a NULL.
 * @return @a 0 if successful,
 *         @a EINVAL if @a self or @a capacity is @a NULL.
 */
int idip_byte_ringbuf_capacity(const idip_byte_ringbuf_t* self, size_t* capacity);

/**
 * @brief Returns actual capacity (allocated data size) in bytes
 * @param[in] self A pointer to an @a idip_byte_ringbuf_t container to return the capacity of.
 *                 Must not be @a NULL.
 * @return actual buffer capacity in bytes
 * @note The method does the same as @ref idip_byte_ringbuf_capacity() but
 * has simple interface.
 */
size_t idip_byte_ringbuf_get_capacity(const idip_byte_ringbuf_t* self);

/**
 * @brief Tests whether an @a idip_byte_ringbuf_t object is empty.
 * @param[in] self A pointer to an @a idip_byte_ringbuf_t object to test.
 * @return @a true if @a self is not @a NULL and the pointed object contains no items,
 *         @a false, otherwise.
 */
bool idip_byte_ringbuf_empty(const idip_byte_ringbuf_t* self);

/**
 * @brief Tests whether an @a idip_byte_ringbuf_t object could store @a size bytes.
 * @param[in] self A pointer to an @ref idip_byte_ringbuf_t object to test.
 * @param[out] size A pointer to a @a size_t variable to store the container size to.
 * @return @a true if @a self is not @a NULL and the pointed object is full,
 *         @a false, otherwise.
 */
bool idip_byte_ringbuf_could_store(const idip_byte_ringbuf_t* self, size_t size);

/**
 * @brief Returns a count of bytes that could be stored in the buffer
 * @param[in] self A pointer to an @ref idip_byte_ringbuf_t object to test.
 * @return Capacity of free (available) space to push data.
 */
size_t idip_byte_ringbuf_available(const idip_byte_ringbuf_t* self);

/**
 * @brief Shifts an @a idip_byte_ringbuf_t object current reading position forward by @a size bytes.
 * @param[in] self A pointer to an @a idip_byte_ringbuf_t object to shift the reading position of.
 *                 Must not be @a NULL.
 * @param size Number of bytes to shift the reading position by.
 *             Must be less than or equal to the size of the container.
 * @return @a 0 if successful,
 *         @a EINVAL if @a self is @a NULL,
 *         @a EAGAIN if @a size exceeds the size of the container.
 */
int idip_byte_ringbuf_pop_front(idip_byte_ringbuf_t* self, size_t size);

/**
 * @brief Shifts an @a idip_byte_ringbuf_t object current writing position backward by @a size bytes.
 * @param[in] self A pointer to an @a idip_byte_ringbuf_t object to shift the writing position of.
 *                 Must not be @a NULL.
 * @param size Number of bytes to shift the writing position by.
 *             Must be less than or equal to the size of the container.
 * @return @a 0 if successful,
 *         @a EINVAL if @a self is @a NULL,
 *         @a EAGAIN if @a size exceeds the size of the container.
 */
int idip_byte_ringbuf_pop_back(idip_byte_ringbuf_t* self, size_t size);

/**
 * @brief Creates and initializes an @c idip_byte_ringbuf_iterator_t object pointing to an @c idip_byte_ringbuf_t begin.
 * @param[in] self A pointer to an @c idip_byte_ringbuf_t container to get the begin iterator to.
 *                 Must not be @c NULL.
 * @param[out] iterator A pointer to an @c idip_byte_ringbuf_iterator_t pointer to store the address
 *                      of a new iterator to.
 *                      Must not be @c NULL.
 * @return @c 0 if successful,
 *         @c EINVAL if @a self or @a iterator is @c NULL,
 *         @c ENOMEM if allocation of a new iterator object has failed.
 */
int idip_byte_ringbuf_begin(const idip_byte_ringbuf_t* self, idip_byte_ringbuf_iterator_t** iterator);

/**
 * @brief Creates and initializes an @c idip_byte_ringbuf_iterator_t object pointing to an @c idip_byte_ringbuf_t end.
 * @param[in] self A pointer to an @c idip_byte_ringbuf_t container to get the end iterator to.
 *                 Must not be @c NULL.
 * @param[out] iterator A pointer to an @c idip_byte_ringbuf_iterator_t pointer to store the address
 *                      of a new iterator to.
 *                      Must not be @c NULL.
 * @return @c 0 if successful,
 *         @c EINVAL if @a self or @a iterator is @c NULL,
 *         @c ENOMEM if allocation of a new iterator object has failed.
 */
int idip_byte_ringbuf_end(const idip_byte_ringbuf_t* self, idip_byte_ringbuf_iterator_t** iterator);

/**
 * @brief Destroys and frees an @c idip_byte_ringbuf_iterator_t object.
 * @param[in] self A pointer to the @c idip_byte_ringbuf_iterator_t object to destroy and free.
 *                 If @c NULL, the function is no-op.
 */
void idip_byte_ringbuf_iterator_delete(idip_byte_ringbuf_iterator_t* self);

/**
 * @brief Returns a pointer to the first byte under an @a idip_byte_ringbuf_iterator_t object.
 * @param[in] self A pointer to the @a idip_byte_ringbuf_iterator_t object to get the first byte address under.
 *                 Must not be @c NULL.
 * @param[out] ptr A pointer to pointer to store the first byte address to.
 *                 Must not be @a NULL.
 * @return @c 0 if successful,
 *         @c EAGAIN if container is empty,
 *         @c EINVAL if @a self or @a item is @a NULL,
 *         @c EFAULT if @a self is invalid.
 */
int idip_byte_ringbuf_iterator_get_pointer(const idip_byte_ringbuf_iterator_t* self, const void **ptr);

/**
 * @brief Copies bytes pointed by an @a idip_byte_ringbuf_iterator_t object.
 * @param[in] self A pointer to the @a idip_byte_ringbuf_iterator_t object.
 *                 Must not be @c NULL.
 * @param[out] buffer A pointer to a buffer to copy bytes to.
 *                    Must not be @a NULL.
 *                    Must be of at least @a size bytes.
 * @param size A number of bytes to copy.
 * @return @c 0 if successful,
 *         @c EAGAIN if container is empty,
 *         @c EINVAL if @a self or @a item is @a NULL.
 */
int idip_byte_ringbuf_iterator_dereference(const idip_byte_ringbuf_iterator_t* self, void* buffer, size_t size);

/**
 * @brief Tests whether two @c idip_byte_ringbuf_iterator_t object point to the same item.
 * @param[in] lhs A pointer to the first @c idip_byte_ringbuf_iterator_t object.
 *                Must not be @c NULL.
 * @param[in] rhs A pointer to the second @c idip_byte_ringbuf_iterator_t object.
 *                Must not be @c NULL.
 * @return @c true if @a lhs and @a rhs are not @c NULL and point to the same item,
 *         @c false, otherwise.
 */
bool idip_byte_ringbuf_iterator_equal(const idip_byte_ringbuf_iterator_t* lhs, const idip_byte_ringbuf_iterator_t* rhs);

/**
 * @brief Tests whether an @c idip_byte_ringbuf_iterator_t object points to an @c idip_byte_ringbuf_t container's end.
 * @param self A pointer to an @c idip_byte_ringbuf_iterator_t to test.
 * @return @c true if @c self is not @c NULL and points to the container's end,
 *         @c false, otherwise.
 */
bool idip_byte_ringbuf_iterator_is_at_end(const idip_byte_ringbuf_iterator_t* self);

void idip_byte_ringbuf_iterator_set_pos(idip_byte_ringbuf_iterator_t* self, size_t pos);

#ifdef __cplusplus
} // extern "C"
#endif //__cplusplus


#endif // IDIP_BYTE_RINGBUF_H
