/** @file idip_utils.h */

#ifndef IDIP_UTILS_H
#define IDIP_UTILS_H

#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#define IDIP_STRINGIFY( s ) #s

#define IDIP_UNUSED( x ) ((void)(x))

#define IDIP_ARRAY_SIZE( x ) (sizeof(x)/sizeof((x)[0]))

// object-orientired macros

/// Allocate memory for object type. Memory filled to 0.
#define IDIP_NEW( type ) calloc(1, sizeof(type))

/// Allocate memory for object and padding space after it
#define IDIP_NEW_PAD( type, padding ) calloc(1, sizeof(type) + (padding))

/**
 * @brief Frees the @a object pointer and sets it to @c NULL.
 * @param[in,out] object A pointer to memory to free.
 *                       Will be set to @c NULL.
 */
#define IDIP_FREE( object ) \
    if( object ) { \
        free(object); \
        object = NULL; \
    }

/**
 * @brief Passes an @a object pointer to a destructor @a dtor and sets the @a object to @c NULL.
 * @param[in,out] object A pointer to an object to destroy and free.
 *                       Will be set to @c NULL.
 * @param[in] dtor The destructor to pass the @a object to.
 *                 Must free after the desctuction.
 */
#define IDIP_DELETE( object, dtor ) \
    if( object ) { \
        dtor(object); \
        object = NULL; \
    }

/**
 * @brief Passes an @a object pointer to a destructor @a dtor with an extra argument @a dtor_arg
 *        and sets the @a object to @c NULL.
 * @param[in,out] object A pointer to an object to destroy and free.
 *                       Will be set to @c NULL.
 * @param[in] dtor The destructor to pass the @a object to.
 *                 Must free the @a object.
 * @param[in] dtor_arg A @a dtor extra argument to pass after the @a object.
 */
#define IDIP_DELETE2( object, dtor, dtor_arg ) \
    if( object ) { \
        dtor(object, dtor_arg); \
        object = NULL; \
    }

#if !defined(TRUE) && !defined(FALSE)
    #define TRUE (1==1)
    #define FALSE (!TRUE)
#endif

#define IDIP_CONTAINER_OF(FIELD_PTR, CONT_TYPE, FIELD_NAME) ((CONT_TYPE *)((char *)(FIELD_PTR) - offsetof(CONT_TYPE, FIELD_NAME)))

#define IDIP_RETURN_VAL_IF_FAIL( stmt, val ) \
if( !(stmt) ) { \
    idip_log_fatal_invalid_arguments(__func__); \
    return (val); \
}

#define IDIP_RETURN_IF_FAIL( stmt ) \
if( !(stmt) ) { \
    idip_log_fatal_invalid_arguments(__func__); \
    return; \
}

#define IDIP_ABORT_IF_FAIL( stmt ) \
if( !(stmt) ) { \
    idip_log_fatal_invalid_arguments(__func__); \
    abort(); \
}

#ifndef __cplusplus

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif /* MIN */

// STATIC_ASSERT( test-expr , message-if-fail );
#ifndef IDIP_STATIC_ASSERT
    #define ASSERT_CONCAT_(a, b) a##b
    #define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)
    /* These can't be used after statements in c89. */
    #ifdef __COUNTER__
        #define IDIP_STATIC_ASSERT(e, m) enum { ASSERT_CONCAT(static_assert_, __COUNTER__) = 1/(!!(e)) }
    #else
        /* This can't be used twice on the same line so ensure if using in headers
         * that the headers are not included twice (by wrapping in #ifndef...#endif)
         * Note it doesn't cause an issue when used on same line of separate modules
         * compiled with gcc -combine -fwhole-program.  */
        #define IDIP_STATIC_ASSERT(e, m) enum { ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(e)) }
    #endif
#endif /* IDIP_STATIC_ASSERT */

#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

uint64_t idip_timeval_to_ms(const struct timeval* tv);
uint64_t get_time_utc_ms();
long long idip_monotonic_ms_now();
struct timespec idip_monotonic_ms_from_now(long long diff_ms);

/// Replace string pointed by @a *cur to copy of string pointer by @a replace.
/// If the original string is exists, then it will be freed by using free() call.
///
/// @param cur - pointer to pointer to original string. Cannot be NULL.
/// @param replace - pointer to replace string. Can be NULL. If it is NULL
/// then the original string pointer will be freeing automatically.
/// @retval 0 the string was replaced successful
/// @retval -1 there ware errors. See @a errno value.
int str_replace(char** cur, const char* replace);

void idip_print_callstack(const char * prefix, void (*printf_like_fn)(const char*, const char*, ...));

/// Fills a buffer for random bytes
int idip_get_random_bytes(void* buf, size_t size);

/// Typical string buffer size for UUID
#define IDIP_UUID_STR_SIZE 40

/// Generates a random uuid string in a cloud format.
/// Based on RFC 4122.
int idip_get_random_uuid(char* uuid, size_t uuid_size);

/// Delay (sleep) process for a given milliseconds.
/// @param delay Sleep time in milliseconds. With a negative value
/// exits immediately (doesn't delay).
void idip_delay_ms(int64_t delay);

#ifdef __cplusplus
}
#endif


#endif // IDIP_UTILS_H
