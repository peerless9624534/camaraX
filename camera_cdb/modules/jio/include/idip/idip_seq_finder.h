#ifndef IDIP_SEQ_FINDER_H
#define IDIP_SEQ_FINDER_H

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct idip_seq_finder;
typedef struct idip_seq_finder idip_seq_finder_t;

typedef struct idip_seq_finder_res {
    const void * unhand_data;
    size_t unhand_size;
} idip_seq_finder_res_t;

/// Create finder that is searching for the sequence \param seq of size \param
/// size in the chunked stream
idip_seq_finder_t * idip_seq_finder_create(const void * seq, size_t size);
bool idip_seq_finder_scan(idip_seq_finder_t * self, const void * data,
                          size_t size, idip_seq_finder_res_t * res);

void idip_seq_finder_destroy(idip_seq_finder_t * self);

#ifdef __cplusplus
}
#endif

#endif // IDIP_SEQ_FINDER_H
