#ifndef IDIP_DEQUE_H
#define IDIP_DEQUE_H

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct idip_deque;
typedef struct idip_deque idip_deque_t;

/// Create a new deque.
/// \param item_size - the stored item size
/// \param item_dtor - optional item destructor, can be NULL
/// \return A new created deque or NULL in case of any errors
idip_deque_t * idip_deque_create(size_t item_size, void (item_dtor)(void * /*item*/));

/// \return a pointer to the stored item at the front of the deque. If the deque
/// is empty then behaviour is undefined, so you should check it firstly
void * idip_deque_peek_front(idip_deque_t * self);

/// \return a pointer to the stored item at the back of the deque. If the deque
/// is empty then behaviour is undefined, so you should check it firstly
void * idip_deque_peek_back(idip_deque_t * self);

/// Push item to the front of the deque
/// \return true if succeeded or false in case of failure
bool idip_deque_push_front(idip_deque_t * self, const void * data);

/// Push item to the back of the deque
/// \return true if succeeded or false in case of failure
bool idip_deque_push_back(idip_deque_t * self, const void * data);

/// Pop item from the front of the deque and copy it to the \param data location.
/// If the deque is empty then behaviour is undefined, so you should check it firstly
void idip_deque_pop_front_and_copy(idip_deque_t * self, void * data);

/// Pop item from the back of the deque and copy it to the \param data location.
/// If the deque is empty then behaviour is undefined, so you should check it firstly
void idip_deque_pop_back_and_copy(idip_deque_t * self, void * data);

/// Pop item from the front of the deque and destroy it via item_dtor (if it was
/// set in the idip_deque_create() call). If the deque is empty then behaviour is
/// undefined, so you should check it firstly
void idip_deque_pop_front(idip_deque_t * self);

/// Pop item from the back of the deque and destroy it via item_dtor (if it was
/// set in the idip_deque_create() call). If the deque is empty then behaviour
/// is undefined, so you should check it firstly
void idip_deque_pop_back(idip_deque_t * self);

/// \return whether the deque is empty
bool idip_deque_is_empty(idip_deque_t * self);

/// \return the deque size, i.e. the count of stored items
size_t idip_deque_get_size(idip_deque_t * self);

/// Remove all the items, calling item_dtor on them (if it was set in the
/// idip_deque_create() call)
void idip_deque_clear(idip_deque_t * self);

/// Destroy the deque
void idip_deque_destroy(idip_deque_t * self);

#ifdef __cplusplus
}
#endif

#endif // IDIP_DEQUE_H
