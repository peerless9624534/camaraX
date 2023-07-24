#ifndef IDIP_STREAM_TOKEN_H
#define IDIP_STREAM_TOKEN_H

#include <idip.h>
#include <idip/details/idip_stream_base.h>

///
struct idip_stream_system_token {
    idip_stream_base_t          base;
    idip_system_token_handler_t handler;
    void*                       user_data;
    idip_parsed_token_t         token; ///< a token copy from user
                                       ///< and WiFi credentials
    int                         retcode; /// handler return code
};

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int idip_stream_system_token_init(idip_stream_system_token_t* self,
                                  uint64_t request_id,
                                  idip_stream_creation_params_t* init_params);

/// "Move" paparsed token from the stream.
/// Return a structure with fields that are pointing to malloc'ed strings and
/// set to NULL internal values of the object.
/// @note Use @ref idip_parsed_token_destroy() to clean the returned structure.
idip_parsed_token_t
idip_stream_system_token_move_data(idip_stream_system_token_t* self);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // IDIP_STREAM_TOKEN_H
