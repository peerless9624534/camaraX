#ifndef IDIP_IOVEC_H
#define IDIP_IOVEC_H

#ifdef _WIN32

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct idip_iovec {
    void *   iov_base;
    size_t   iov_len;
} idip_iovec_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#else

#include <sys/uio.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct iovec idip_iovec_t;

void idip_plain_buf_adv(const void** buf, size_t* size, ptrdiff_t delta);
void idip_buf_adv_to_end(const void** buf, size_t* size);

bool idip_iovec_equal(const idip_iovec_t* left, const idip_iovec_t* right);
void idip_iovec_adv(idip_iovec_t* self, ptrdiff_t delta);
void idip_iovec_adv_to_end(idip_iovec_t* self);

#endif // _WIN32

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // IDIP_IOVEC_H
