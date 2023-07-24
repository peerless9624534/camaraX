#ifndef IDIP_STREAM_ARCHIVE_LIST_H
#define IDIP_STREAM_ARCHIVE_LIST_H

#include <idip.h>
#include <idip/details/idip_stream_base.h>

///
struct archive_list_query_args {
    char* target;
    archive_interval_t interval;
};
typedef struct archive_list_query_args archive_list_args_t;

///
struct idip_stream_archive_list {
    idip_stream_base_t          base;
    idip_stream_archive_list_handler_t handler;
    void*                       user_data;
    archive_list_args_t   query_args;
};

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int idip_stream_archive_list_init(idip_stream_archive_list_t* self,
                                  uint64_t request_id,
                                  idip_stream_creation_params_t* init_params,
                                  archive_list_args_t* query_args);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // IDIP_STREAM_ARCHIVE_LIST_H
