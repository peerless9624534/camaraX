/**
 * IDIP custom events creation API.
 */
#ifndef IDIP_CUSTOM_ARG_H_INCLUDED
#define IDIP_CUSTOM_ARG_H_INCLUDED

#include <stddef.h>
#include <stdint.h>

//----- custom events ----------------------------------------------------------

/// Opaque data structure for create custom event arguments
struct idip_custom_arg;
/// Data type that represents custom event creation service.
/// API for the type seems like as json object creation. By default it is
/// an empty dictionary object. User can append simple key-value pairs here
/// or create sub-objects or sub-arrays.
/// @note All of functions DO NOT copy any string data, they just saves pointers to strings.
/// You don't have to delete given strings all time while instance of @ref idip_custom_arg_t
/// is alive.
typedef struct idip_custom_arg idip_custom_arg_t;

/// Status codes for operations on @ref idip_custom_arg_t;
enum idip_custom_arg_status_t {
    /// Operation was success
    CUSTOM_ARG_OK = 0,
    /// An invalid argument was passed to method
    CUSTOM_ARG_INVALID_ARGUMENT = -1,
    /// There are no available memory available during method call.
    CUSTOM_ARG_NO_MEM = -2,
    /// Call of method requires not empty and not null key.
    CUSTOM_ARG_KEY_REQUIRED = -3,
    /// Mismatch call of idip_custom_arg_close_array() when current object
    /// is a dictionary, or mismatching call of idip_custom_arg_close_dict()
    /// when current object is array
    CUSTOM_ARG_REACH_OF_ORDER = -4,
    /// Number of idip_custom_arg_close_xxx() calls is greater than number
    /// of idip_custom_arg_begin_xxx() calls
    CUSTOM_ARG_UNDERFLOW = -5,
};

/// Create empty instance of @ref idip_custom_arg_t.
/// Users should append structured data by using idip_custom_arg_xxx functions.
/// By default empty instance is equivalent to empty dictionary object {}.
idip_custom_arg_t* idip_custom_arg_new(void);

/// Delete object and free resources that was bound to it.
void idip_custom_arg_delete(idip_custom_arg_t* self);

/// Create empty dictionary object inside current object (a sub-object) with
/// given name and set is as current object.
/// Dictionary is key-value storage like json object {}. It can
/// contains simple key-value pairs, sub named objects and named arrays.
/// Sub-objects and sub-arrays can contains other sub-objects and so on.
/// After than sub object was created with this one call, the sub object
/// sets as current. All of idip_custom_arg_append_xxx() methods are append
/// values to current object. The main rule: each sub-object inside dictionary
/// must have a name (a key). To finalize current dictionary you must call
/// @ref idip_custom_arg_end_dict().
/// @note Set name to NULL when you append new objects to array.
/// @param self - pointer to instance of @ref idip_custom_arg_t
/// @param name - name of new dictionary object. Set it to NULL when you append
/// new dictionary inside arrays.
/// @return one of @ref idip_custom_arg_status_t
int idip_custom_arg_begin_dict(idip_custom_arg_t* self,
                               const char* name);

/// Finalize current dictionary object and go to previous upper level object
/// (parent object).
/// @return one of @ref idip_custom_arg_status_t
int idip_custom_arg_end_dict(idip_custom_arg_t* self);

/// Create empty sub-array inside current object and set it as current object.
/// Array is a sequence of unnamed objects, arrays or plain values.
/// @note Set name to NULL when you append values into array using
/// idip_custom_arg_append_xxx() functions family.
/// @param self - pointer to instance of @ref idip_custom_arg_t
/// @param name - name of new array object. Set it to NULL when you append
/// new array inside arrays and some pointer to string when you create an array
/// inside dictionary. ( { "array": [ ] } - inside objects, [ [ ] ] - inside arrays)
/// @return one of @ref idip_custom_arg_status_t
int idip_custom_arg_begin_array(idip_custom_arg_t* self,
                                const char* name);

/// Finalize current array object and go to previous upper level object
/// (parent object).
/// @return one of @ref idip_custom_arg_status_t
int idip_custom_arg_end_array(idip_custom_arg_t* self);

// These functions are append pair of "name": value to current map or
// map { "name": value } to current array.
/// Append named or unnamed null value into current object.
/// @param self - pointer to instance of @ref idip_custom_arg_t
/// @param name - name of new null value. Set to NULL when you append value into array.
/// @return one of @ref idip_custom_arg_status_t
int idip_custom_arg_append_null(idip_custom_arg_t* self,
                                const char* name);

/// Append named or unnamed boolean value into current object.
/// @param self - pointer to instance of @ref idip_custom_arg_t
/// @param name - name of value. Set to NULL when you append value into array.
/// @param val - value to append
/// @return one of @ref idip_custom_arg_status_t
int idip_custom_arg_append_bool(idip_custom_arg_t* self,
                                const char* name, int val);

/// Append named or unnamed integer value into current object.
/// @param self - pointer to instance of @ref idip_custom_arg_t
/// @param name - name of value. Set to NULL when you append value into array.
/// @param val - value to append
/// @return one of @ref idip_custom_arg_status_t
int idip_custom_arg_append_int(idip_custom_arg_t* self,
                               const char* name, int val);

/// Append named or unnamed integer value into current object.
/// @param self - pointer to instance of @ref idip_custom_arg_t
/// @param name - name of value. Set to NULL when you append value into array.
/// @param val - value to append
/// @return one of @ref idip_custom_arg_status_t
int idip_custom_arg_append_int64(idip_custom_arg_t* self,
                                 const char* name, int64_t val);

/// Append named or unnamed double value into current object.
/// @param self - pointer to instance of @ref idip_custom_arg_t
/// @param name - name of value. Set to NULL when you append value into array.
/// @param val - value to append
/// @return one of @ref idip_custom_arg_status_t
int idip_custom_arg_append_double(idip_custom_arg_t* self,
                                  const char* name, double val);

/// Append named or unnamed string value into current object.
/// @param self - pointer to instance of @ref idip_custom_arg_t
/// @param name - name of value. Set to NULL when you append value into array.
/// @param val - pointer to sting value
/// @return one of @ref idip_custom_arg_status_t
/// @note This one function DO NOT copy any string data, it just save pointer to string.
/// You don't have to delete the string all time while instance of @ref idip_custom_arg_t
/// is alive.
int idip_custom_arg_append_string(idip_custom_arg_t* self,
                                  const char* name, const char* val);

/// Append named or unnamed binary data (blob) value into current object.
/// @param self - pointer to instance of @ref idip_custom_arg_t
/// @param name - name of value. Set to NULL when you append value into array.
/// @param val - pointer to binary data
/// @param val_size - binary data size (in bytes)
/// @return one of @ref idip_custom_arg_status_t
/// @note The function DO NOT copy val, just store the pointer.
int idip_custom_arg_append_blob(idip_custom_arg_t* self,
                                const char* name,
                                const void* val, size_t val_size);

#endif /* IDIP_CUSTOM_ARG_H_INCLUDED */

