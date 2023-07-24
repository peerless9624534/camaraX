#ifndef WRITE_OP_ITEM_P_H
#define WRITE_OP_ITEM_P_H

#include <sys/uio.h> // iovec
#include <pthread.h> // pthread_t
#include <idip/details/write_op_item.h>

struct write_op_item
{
    // complete handler
    write_op_complete_t on_complete;
    // complete handler data
    void*               on_complete_data;
    // A mutex for locking on_complete call for call once.
    pthread_mutex_t     on_complete_lock;

    // Real written size
    size_t              written;
    // A own data buffer
    struct msgpack_sbuffer* v2_frame;

    // Creation monotonic timestamp
    long long           creation_time;

    // a reference counter
    int                 ref_count;
};

#endif // WRITE_OP_ITEM_P_H
