#ifndef LOCAL_WRITE_OP_H
#define LOCAL_WRITE_OP_H

#include <msgpack/sbuffer.h>

struct idip_session;

//------------------------------------------------------------------------------
// helper structure
struct local_reply_task {
    struct msgpack_sbuffer* v2_frame;
    struct idip_session*    session;
    int                     close;
    long long               create_ts;
};
typedef struct local_reply_task local_reply_task_t;

local_reply_task_t* local_reply_task_new(struct idip_session* session, int close);

void local_reply_task_delete(local_reply_task_t* self);

#endif // LOCAL_WRITE_OP_H
