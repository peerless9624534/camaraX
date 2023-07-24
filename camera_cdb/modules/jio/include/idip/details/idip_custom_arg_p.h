#ifndef IDIP_CUSTOM_ARG_P_H_INCLUDED
#define IDIP_CUSTOM_ARG_P_H_INCLUDED

#include <msgpack.h>
#include <idip/idip_vector.h>

//------------------------------------------------------------------------------
struct idip_custom_arg
{
    struct msgpack_object* root;
    struct msgpack_object* cur;
    idip_ptr_vector_t*     parent_stack;
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void idip_custom_arg_make_message(const struct idip_custom_arg* self, struct msgpack_sbuffer* sbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IDIP_CUSTOM_ARG_P_H_INCLUDED */

