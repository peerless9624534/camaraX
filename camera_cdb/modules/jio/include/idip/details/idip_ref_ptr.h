#ifndef IDIP_REF_PTR_H
#define IDIP_REF_PTR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

struct weak_ptr;
struct ref_block;
typedef struct ref_block ref_block_t;
typedef struct weak_ptr weak_ptr_t;

/**
  *  The ref_block should be placed at the start of user data structure:
  *  struct data {
  *      ref_block_t * rb;
  *      // data fields
  *      int id;
  *      const char * name;
  *  };
*/

/** Release user data pointer */
typedef void(*ref_ptr_deleter)(void *);

void idip_init_ref_block(void * ref_data, ref_ptr_deleter deleter);
void * idip_ref(void * ref_data);
void * idip_ref_print(void * ref_data, const char* fn);
void idip_unref(void * ref_data);
void idip_unref_print(void * ref_data, const char* fn);
weak_ptr_t * idip_ref_weak(void * ref_data);
void idip_unref_weak(weak_ptr_t * weak_data_ptr);
void * idip_ref_weak_to_shared(weak_ptr_t * weak_data_ptr);
void * idip_ref_weak_to_shared_print(weak_ptr_t * weak_data_ptr, const char* fn);
bool idip_weak_expired(weak_ptr_t * weak_data_ptr);
bool idip_ref_same_data(void * ref_data, weak_ptr_t * weak_data_ptr);
int idip_get_use_count(void * ref_data);

#ifdef __cplusplus
}
#endif

#endif // IDIP_REF_PTR_H
