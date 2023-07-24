#ifndef IDIP_GC_H
#define IDIP_GC_H

typedef struct gc_finalizer
{
    void* finalize_data;
    void (*finalize_fn)(void* data);
}
gc_finalizer_t;

struct idip_stream_base;
typedef struct idip_gc idip_gc_t;


idip_gc_t* idip_gc_new();

int idip_gc_start(idip_gc_t* self);

int idip_gc_stop(idip_gc_t* self);

void idip_gc_delete(idip_gc_t* self);


int idip_gc_stream_thread(idip_gc_t* self,
                   const char* session_name,
                   struct idip_stream_base* stream);

#endif // IDIP_GC_H
