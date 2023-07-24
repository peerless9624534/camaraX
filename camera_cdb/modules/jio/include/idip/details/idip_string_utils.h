#ifndef IDIP_DETAILS_STRING_UTILS_H
#define IDIP_DETAILS_STRING_UTILS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <idip/idip_seq_finder.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Buffer offset correction helper for (v)snprintf()-like functions.
/// Calculates actual length of written bytes in a buffer after a (v)snptinrf()
/// call.
/// @param snp_result - a value returned from (v)snprintf()
/// @param buf_max - size of rendering buffer that's passed to
/// correspond (v)snprintf() call.
size_t idip_snp_written_bytes(int snp_result, size_t buf_max);

/// Check if \param str is null or its size is zero
bool idip_string_empty(const char * str);

/// Allocates formatted string
char* idip_alloc_fmt_str(const char* fmt, ...);

/// Skips whitespaces both from the left and right sides
void idip_skip_whitespaces(const char ** str, size_t * len);

/// Skips whitespaces from the left side
void idip_left_skip_whitespaces(const char ** str, size_t * len);

/// Skips whitespaces from the right side
void idip_right_skip_whitespaces(const char ** str, size_t * len);

/// Converts string to unsigned integer value
/// \return true if successful
bool idip_str_to_uint(const char * str, size_t len, unsigned long * res);

/// Converts string signed integer value
/// \return true if successful
bool idip_str_to_int(const char * str, size_t len, long * res);

/// Representation of a string without terminating null character
typedef struct idip_str_view {
    const char * data;
    size_t len;
} idip_str_view_t;

/// Converts c-string (with terminating null character) to the idip_str_view
idip_str_view_t idip_str_view_from_cstr(const char * cstr);

/// Macro for initialization of the static constants
#define IDIP_STR_VIEW_INIT_FROM_CSTR(cstr)  {.data = cstr, .len = strlen(cstr)}

/// Returns true if data is null or len is 0
bool idip_str_view_empty(const idip_str_view_t * str);

/// Returns a pointer to the next after the last character
const char * idip_str_view_end(const idip_str_view_t * str);

#ifdef __cplusplus
}
#endif

#endif // IDIP_DETAILS_STRING_UTILS_H
