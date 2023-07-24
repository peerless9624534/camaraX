/** @file idip_vector.h */

#ifndef IDIP_VECTOR_H
#define IDIP_VECTOR_H

#include <stddef.h>
#include <stdbool.h>

struct idip_vector
{
    /// pointer to vector's data
    void*  data;
    /// current length
    size_t length;
};
typedef struct  idip_vector idip_vector_t;

struct idip_ptr_vector
{
    /// pointer to vector's data
    void**  data;
    /// current length
    size_t length;
};
typedef struct  idip_ptr_vector idip_ptr_vector_t;

/// A type for an @ref idip_vector_t item destructor.
typedef void (*idip_destroy_fn_t)(void*);

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/**
 * @brief Creates an @ref idip_vector_t container to store items of size @a item_size
 *        and destroy them with @a destroy upon removing.
 * @param item_size A size of an item in the new @c idip_vector_t container.
 *                  Must not be @c 0.
 * @param destroy A destructor for items in the new @c idip_vector_t container to call
 *                upon item removing (@see idip_vector_delete(), @see idip_vector_pop_back(),
 *                @see idip_vector_clear()).
 *                If @c NULL, no item destruction is performed.
 * @return A pointer to the new @ref idip_vector_t object if successful,
 *         @c NULL if allocation has failed or @a item_size is @c 0.
 */
idip_vector_t* idip_vector_new_with_destroy_func(size_t item_size,
                                                 idip_destroy_fn_t destroy);

/**
 * @brief Creates an @ref idip_vector_t container to store items of size @a item_size.
 * @param item_size A size of an item in the new @c idip_vector_t container.
 *                  Must not be @c 0.
 * @return A pointer to the new @ref idip_vector_t object if successful,
 *         @c NULL if an allocation failed or invalid arguments are passed.
 */
idip_vector_t* idip_vector_new(size_t item_size);

/**
 * @brief Creates an @ref idip_vector_t container with preallocated capacity
 *        to store items of size @a item_size.
 * @param item_size A size of an item in the new @c idip_vector_t container.
 *                  Must not be @c 0.
 * @param items Number of items to preallocate.
 * @return A pointer to the new @ref idip_vector_t object if successful,
 *         @c NULL if an allocation failed or invalid arguments are passed.
 */
idip_vector_t* idip_vector_new_sized(size_t item_size, size_t items);

/**
 * @brief Creates an @ref idip_vector_t container with preallocated capacity
 *        to store items of size @a item_size and destroy them with @a destroy upon removing.
 * @param item_size A size of an item in the new @c idip_vector_t container.
 *                  Must not be @c 0.
 * @param alloc_items Number of items to preallocate capacity for.
 * @param destroy A destructor for items in the new @c idip_vector_t container to call
 *                upon item removing (@see idip_vector_delete(), @see idip_vector_pop_back(),
 *                @see idip_vector_clear()).
 *                If @c NULL, no item destruction is performed.
 * @return A pointer to the new @ref idip_vector_t object if successful,
 *         @c NULL if allocation has failed or @a item_size is @c 0.
 */
idip_vector_t* idip_vector_new_full(size_t item_size,
                                    size_t alloc_items,
                                    idip_destroy_fn_t destroy);

idip_vector_t* idip_vector_new_from_array(size_t item_size,
                                          size_t items_count,
                                          const void* src_array);

/**
 * @brief Destroys and frees an @ref idip_vector_t container.
 * @param self A pointer to an @ref idip_vector_t container to destroy and free.
 *             Must not be @c NULL.
 */
void idip_vector_delete(idip_vector_t* self);

/**
 * @brief Pushes an @a item back to an @ref idip_vector_t container.
 *        Item is @c memmove'd.
 * @param self A pointer to an @ref idip_vector_t container to push to.
 *             Must not be @c NULL.
 * @param item A pointer to an item to push.
 *             Must not be @c NULL.
 * @return @c 1 if successful,
 *         @c 0 if invalid arguments are passed or reallocation has failed.
 */
int idip_vector_push_back(idip_vector_t* self, const void *item);

/**
 * @brief Pops an item from an @ref idip_vector_t container.
 * @param self A pointer to an @ref idip_vector_t container to pop from.
 *             Must not be @c NULL.
 */
void idip_vector_pop_back(idip_vector_t* self);

/**
 * @brief Returns a pointer to an item at certain index in an @ref idip_vector_t
 *        container.
 * @param vec A pointer to an @ref idip_vector_t container to get an item pointer to.
 *            Must not be @c NULL.
 * @param type A type of the item being pointed.
 * @param index A integer item position in the @a vec.
 *              Must be less than <tt>vec->length</tt>.
 * @return A pointer of type <tt>type*</t> to the item in @a vec at position @a index.
 * @warning No argument check is performed.
 */
#define idip_vector_index(vec, type, index) \
        ( ((type*) (vec)->data) [(index)] )

/**
 * brief Removes an item at certain index from an @ref idip_vector_t container by
 *       calling @a destroy_fn for it if it's set and overwriting with the last item.
 *       Removing the last item just destroys it.
 * @param self A poitner to an @ref idip_vector_t container to remove from.
 *             Must not be @c NULL.
 * @param ix Index of the item to remove.
 *           Must be less than @a length of the @a self.
 */
void idip_vector_remove_fast(idip_vector_t* self, size_t ix);

/**
 * @brief Removes all items from an @ref idip_vector_t container,
 *        calling @a destroy_fn if it's set.
 * @param self A pointer to an @ref idip_vector_t container to clear.
 *             Must not be @c NULL.
 */
void idip_vector_clear(idip_vector_t* self);

/**
 * @brief Tests if an @ref idip_vector_t container is empty.
 * @param self A pointer to an @ref idip_vector_t container to clear.
 *             Must not be @c NULL.
 * @return @c true if @a self is not @c NULL and it's empty,
 *         @c false, otherwise.
 */
bool idip_vector_empty(idip_vector_t* self);

/**
 * @brief Reserve @a reserved items in the vector.
 * @param A pointer to an @ref idip_vector_t container to clear.
 *        Must not be @c NULL.
 * @param reserved count of items to be reserver in the vector
 * @return true on success, false when failure.
 */
bool idip_vector_reserve(idip_vector_t* self, size_t reserved);

typedef void (*idip_free_fn_t)(void*);

idip_ptr_vector_t* idip_ptr_vector_new();

idip_ptr_vector_t* idip_ptr_vector_new_with_free_func( idip_free_fn_t fn );

void idip_ptr_vector_delete(idip_ptr_vector_t* self);

int idip_ptr_vector_push_back(idip_ptr_vector_t* self, void *item);

void idip_ptr_vector_pop_back(idip_ptr_vector_t* self);

int idip_ptr_vector_back(idip_ptr_vector_t *self, void **back_item);

int idip_ptr_vector_front(idip_ptr_vector_t* self, void **front_item);

#define idip_ptr_vector_index(vec, index) ( ((vec)->data)[(index)] )

#define idip_ptr_vector_indext(vec, index, ptr_type) ( (ptr_type*)idip_ptr_vector_index(vec, index) )

void idip_ptr_vector_remove_index_fast(idip_ptr_vector_t* self, size_t ix);

void idip_ptr_vector_remove_fast(idip_ptr_vector_t* self, const void* ptr);

/// Remove @a n items from the vector starts from index @a ix
/// @note invariant: ix + n < self->length
void idip_ptr_vector_remove(idip_ptr_vector_t* self, size_t ix, size_t n);

void idip_ptr_vector_clear(idip_ptr_vector_t* self);

bool idip_ptr_vector_empty(idip_ptr_vector_t* self);

bool idip_ptr_vector_find(const idip_ptr_vector_t* self, void* ptr, size_t* index);

void idip_ptr_vector_qsort(idip_ptr_vector_t* self,
                           int (*compar)(const void *, const void *));

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // IDIP_VECTOR_H
