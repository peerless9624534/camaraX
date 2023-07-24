#ifndef WRITE_OP_ITEM_H
#define WRITE_OP_ITEM_H

#include <stddef.h>
#include <idip/idip_iovec.h>

//------------------------------------------------------------------------------
/// callback type that called after operation complete
typedef void (*write_op_complete_t)(void* stream, size_t writed, int ec);

//------------------------------------------------------------------------------
struct write_op_item;
struct idip_server;
struct msgpack_sbuffer;
typedef struct write_op_item write_op_item_t;

/// Interface for async data sending
struct write_op_item {
    /// ref object
    write_op_item_t* (*ref)(write_op_item_t* /*self*/);

    /// uref object
    void (*unref)(write_op_item_t* /*self*/);

    /// get ref count
    int (*get_use_count)(write_op_item_t* /*self*/);

    /// return data to send
    idip_iovec_t (*prepare_data)(write_op_item_t* /*self*/);

    /// return of data size
    size_t (*get_data_size)(write_op_item_t* /*self*/);

    /// the callback that is called after sending data
    void (*complete)(write_op_item_t* /*self*/, int /*ec*/);

    /// add written bytes in order to accumulate the sum
    void (*add_written_bytes)(write_op_item_t* /*self*/, size_t /*written*/);

    /// get accumulate writen bytes sum
    size_t (*get_witten_bytes)(write_op_item_t* /*self*/);

    /// get time of object creation
    long long (*get_creat_time)(write_op_item_t* /*self*/);
};

write_op_item_t*
write_op_item_new(struct msgpack_sbuffer* frame,
                  write_op_complete_t on_complete,
                  void* on_complete_data);

#endif // WRITE_OP_ITEM_H
