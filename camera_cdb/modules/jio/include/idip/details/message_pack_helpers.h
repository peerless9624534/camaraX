#ifndef MESSAGE_PACK_HELPERS_H
#define MESSAGE_PACK_HELPERS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <msgpack.h>

typedef enum idip_reply_has_data {
    REPLY_NO_MORE_DATA = 0,
    REPLY_HAS_MORE_DATA = 1
} idip_reply_has_data_t;

/// Compares message pack string object and c-string.
/// @param o - msgpack string object
/// @param str - string to compare
/// @return 1 if values are equals, and 0 otherwize. Non-string objects do not
/// comparable with c-strings and therefore they are equals never.
/// @note Be aware, internally function computes lengh of str.
int mpk_str_equals_to(const msgpack_object o, const char* str);

/// This function is like strdup() from libc, but allocate (with malloc())
/// and copies string from messagepack string object. Result string is always
/// zero-terminated. This is an opposite to message packs's strings where
/// termination character can be or can't be exists.
/// @param o - message pack string object. Another types of objects DO NOT
/// acceptable.
/// @return malloc()'s allocated copy of source string or NULL
char* mpk_strdup(const msgpack_object o);

/// Copies the string from message pack string object to the buffer pointed by
/// dest. The function is like strncpy() but accepts message pack string object
/// instead c-string as source.
/// @return pointer to destination staring dest
char* mpk_strncpy(char* dest, size_t dest_len, const msgpack_object o);

/// Makes a deep copy of \param o. If \param zone is NULL, then the result
/// object will be allocated via the standart malloc() function and thus it
/// should be destroyed by the mpk_reset_object() or mpk_reset_map() function.
/// If \param zone is not NULL, then the result object will be allocated in this
/// zone and thus you should use zone clear functions in order to destroy the
/// result object
msgpack_object mpk_copy_object(const msgpack_object* o, msgpack_zone* zone);
msgpack_object mpk_copy_map(const msgpack_object_map* map, msgpack_zone* zone);

/// Frees an \param o allocated by the mpk_copy_object()/mpk_copy_map() functions
/// with NULL zone. Scalar objects become nil and vector objects become cleared
/// (ptr == NULL and size == 0) after funcion returns
void mpk_reset_object(msgpack_object* o);
void mpk_reset_map(msgpack_object_map* map);

int mpk_get_uint64(uint64_t* dest, msgpack_object o);

int mpk_get_int64(int64_t* dest, msgpack_object o);

int mpk_get_int8(int8_t* dest, msgpack_object o);

int mpk_get_item_at(msgpack_object array, size_t index, msgpack_object* result);

#define MPK_ARRAY_ITEM_AT(arrObj, index) (*((arrObj).via.array.ptr + (index)))

/// Look ups in the given key-value object a value object by it's key.
/// @param o - pointer to key-value message pack's object
/// @param key - a key to search
/// @return opinter to value object or NULL if not found
msgpack_object* mpk_find(const msgpack_object_kv* o, const char* key);

int mpk_get_bool(const msgpack_object_kv* o, const char* key, int* val);

int mpk_get_int(const msgpack_object_kv* o, const char* key, int64_t* val);

int mpk_get_uint(const msgpack_object_kv* o, const char* key, uint64_t* val);

int mpk_get_string(const msgpack_object_kv* o, const char* key,
                   char* val, size_t max_len, size_t* len);

int mpk_get_string_dup(const msgpack_object_kv* o, const char* key,
                       char** val, size_t* len);

/// Find MSGPACK_OBJECT_BIN for given key and save pointer to it and its size
int mpk_get_blob(const msgpack_object_kv* o, const char* key,
                 const void **blob, size_t* blob_size);

/// Packs string pointed by s with msgpack_packer instance.
/// Original message pack API don't contains simple methods to do it. Instead
/// API exports 2 methods: msgpack_pack_str() and msgpack_pack_str_body().
/// The function aggregates these two to shrink the code.
/// @param pk - pointer to msgpack_packer instance
/// @param s - pointer to source string
int msgpack_pack_string(msgpack_packer* pk, const char* s);

/// Packs boolean value with msgpack_packer instance.
/// Original msgpack API exports 2 functions to pack boolean values:
/// msgpack_pack_true() and msgpack_pack_false(). User must explicitly checks
/// boolean value and call these methods.
/// THis helper function aggregates two methods to pack boolean value,
/// contained in variable.
int msgpack_pack_bool(msgpack_packer* pk, int b);

void mpk_pack_status(msgpack_packer* pk, int ec, const char* descr);


msgpack_object*
mpk_find_in_map_object(const msgpack_object* o, const char* key);

msgpack_object_kv*
mpk_find_kv_in_map_object(const msgpack_object* o, const char* key);

/// The function actions like msgpack_object_print() but limits number of
/// printing bytes of blobs. If @a bin_size_limit is @c 0, the function
/// is the same as @ref msgpack_object_print().
void mpk_object_print(FILE* out, msgpack_object o, size_t bin_size_limit);

/// The function actions like msgpack_object_print_buffer() but limits number of
/// printing bytes of blobs to 16 bytes.
size_t mpk_object_print_buffer(char *buffer, size_t buffer_size, msgpack_object o);


struct idip_v2_transport_sysext_t;

int
mpk_pack_transport_v2(msgpack_packer* pk,
                      uint64_t request_id, uint64_t seq_id,
                      const struct idip_v2_transport_sysext_t* system);

msgpack_object
mpk_lookup_idip_method_name(const char* buf, size_t buf_size);

/// Looks for a idip method name pattern in message pack buffer in the unpacker.
msgpack_object
mpk_unpacker_lookup_idip_method_name(const msgpack_unpacker* unp);

/// Creates new empty buffer without allocated data
msgpack_sbuffer mpk_sbuffer_create();

/// Does the same writing as the msgpack_sbuffer_write() function except that
/// the initial writing doesn't allocate MSGPACK_SBUFFER_INIT_SIZE bytes, instead
/// it allocates exact \param len bytes
/// \return 0 if successful, non-zero value otherwise
int mpk_sbuffer_write(void* raw_sbuf, const char* buf, size_t len);

/// Reserves at least \param size free bytes
/// \return 0 if successful, non-zero value otherwise
int mpk_sbuffer_reserve(msgpack_sbuffer* sbuf, size_t size);

/// Deallocates and reinit \param sbuf buffer. After this function returns
/// \param sbuf can be used again as a just created buffer via msgpack_sbuffer_init()
void mpk_sbuffer_reset(msgpack_sbuffer* sbuf);

/// Deallocates \param to buffer, moves all the contents of \param from to
/// \param to and resets \param from
void mpk_sbuffer_move(msgpack_sbuffer* to, msgpack_sbuffer* from);

/// Moves all the contents of \param from to \param to_ptr and \param to_size
/// and resets \param from
void mpk_sbuffer_move_plain(void ** to_ptr, size_t * to_size, msgpack_sbuffer* from);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // MESSAGE_PACK_HELPERS_H
