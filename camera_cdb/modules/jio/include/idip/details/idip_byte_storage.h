#ifndef IDIP_BYTE_STORAGE_H
#define IDIP_BYTE_STORAGE_H

#include <idip/idip_data_types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Raw data holder with size and capacity.
 *
 * Technically, it's a byte vector.
 */
typedef struct idip_byte_storage
{
    idip_byte_t* data;
    size_t capacity;
    size_t size;
} idip_byte_storage_t;

/**
 * @brief Initializes an @a idip_byte_storage_t object by allocating its inner buffer with specified @a capacity.
 * @param self A pointer to an @a idip_byte_storage_t object to initialize. Must not be @a NULL.
 * @param capacity Initial storage capacity. @ 0 causes 1-byte preallocation.
 * @return 0 if successful,
 *         @a ENOMEM in case of allocation failure.
 *
 * Attempting to initialize an already initialized @a idip_byte_storage_t object results in memory leak.
 */
int idip_byte_storage_init(idip_byte_storage_t* self, size_t capacity);


int idip_byte_storage_init_default(idip_byte_storage_t* self);

/**
 * @brief Frees the inner buffer.
 * @param self A pointer to an @a idip_byte_storage_t to destroy. Must not be @a NULL.
 *
 * Attempting to destroy an uninitialized @a idip_byte_storage_t object results in undefined behavior.
 */
void idip_byte_storage_destroy(idip_byte_storage_t* self);

/**
 * @brief Preallocates memory for a @a idip_byte_storage_t object with a @a capacity number of bytes.
 * @param self A pointer to an @a idip_byte_storage_t object to preallocate memory for. Must not be @a NULL
 *             and must be initialized.
 * @param capacity Number of bytes to preallocate.
 * @return @a 0 if successful,
 *         @a ENOMEM if preallocation failed.
 *
 * If @a capacity is less than the object's current capacity leaves memory untouched.
 * Attempting to reserve for an unitialized @a idip_byte_storage_t object results in undefined behavior.
 */
int idip_byte_storage_reserve(idip_byte_storage_t* self, size_t capacity);

/**
 * @brief Copies @a size number of bytes from the specified @a data pointer. May reallocate.
 * @param self A pointer to an @a idip_byte_storage_t object to copy bytes to. Must not be @a NULL.
 * @param data A pointer to data to copy bytes from. Must not be @a NULL.
 * @param size Number of bytes to copy.
 * @return @a 0 if successful,
 *         @a ENOMEM if reallocation failed.
 * @warning @a data must not point to data inside the storage since @a memcpy is used.
 */
int idip_byte_storage_copy_data(idip_byte_storage_t* self, const idip_byte_t* data, size_t size);

/**
 * @brief Sets an inner data pointer to @a data and size and capacity to @a size.
 * @param self A pointer to an @a idip_byte_storage_t object to set the inner buffer of. Must no be @a NULL.
 * @param data A pointer to data to set to. Must not be @a NULL.
 * @param size Number of bytes stored at the @a data pointer.
 */
void idip_byte_storage_move_data(idip_byte_storage_t* self, idip_byte_t* data, size_t size);

/**
 * @brief Appends data to an @a idip_byte_storage_t object.
 * @param self A pointer to an @a idip_byte_storage_t object to push data back to. Must no be @a NULL.
 * @param data A pointer to data to push back. Must not be @a NULL.
 * @param size Number of bytes to push back.
 * @return 0 if successful,
 *         @a ENOMEM if reallocation is required and it has failed.
 */
int idip_byte_storage_push_back_data(idip_byte_storage_t* self, const idip_byte_t* data, size_t size);

/**
 * @brief Clears an @a idip_byte_storage_t object.
 * @param self A pointer to an @a idip_byte_storage_t object to clear. Must not be @a NULL.
 *
 * Effectively, it sets the inner @a size field to @a 0 and leaves memory untouched.
 */
void idip_byte_storage_clear(idip_byte_storage_t* self);

/**
 * @brief Tests whether an @a idip_byte_storage_t object holds no data.
 * @return @a true if the object's size is @a 0,
 *         @a false otherwise.
 *
 * Capacity of the @a self is not tested.
 */
bool idip_byte_storage_empty(idip_byte_storage_t* self);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // IDIP_BYTE_STORAGE_H
