#ifndef IDIP_DEQUE_P_H
#define IDIP_DEQUE_P_H

#include <idip/idip_deque.h>

/// Note: this header is intended only for private usage in the idip_deque.c and tests

#ifdef __cplusplus
extern "C" {
#endif

struct node;
typedef struct node node_t;

struct node {
    node_t * next;
    node_t * prev;
    void * chunk[0];    // a pointer guarantees the correct alignment for all stored data types
};

typedef struct side_desc {
    node_t * node;
    int rpos;   // the value can be negative in special edge cases (see deque_pop_back() and idip_deque_push_front())
} side_desc_t;

typedef struct idip_deque {
    side_desc_t front;
    side_desc_t back;
    size_t size;
    size_t item_size;
    size_t chunk_capacity;
    void (*item_dtor)(void * /*item*/);
} idip_deque_t;

static const size_t MIN_CHUNK_SIZE = 1024;      // min chunk size in bytes
static const size_t MIN_CHUNK_CAPACITY = 16;    // min chunk capacity in items

#ifdef __cplusplus
}
#endif

#endif // IDIP_DEQUE_P_H
