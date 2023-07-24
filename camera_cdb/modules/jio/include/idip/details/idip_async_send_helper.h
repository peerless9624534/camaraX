#ifndef ASYNC_SEND_HELPER_H
#define ASYNC_SEND_HELPER_H

#include <stdint.h>
#include <stdbool.h>
#include <idip/idip_rc.h>
#include <idip/idip_iovec.h>
#include <msgpack/sbuffer.h>

#ifdef __cplusplus
extern "C" {
#endif

struct write_op_item;
struct idip_stream_base;
struct idip_asend_helper;
struct idip_asend_buffer;
typedef struct idip_asend_helper idip_asend_helper_t;
typedef struct idip_asend_buffer idip_asend_buffer_t;
typedef bool (*idip_asend_on_complete_cb)(void * /*data*/,
                                          struct idip_stream_base * /*stream*/,
                                          idip_rc_t /*ec*/, bool /*last*/);
typedef bool (*idip_asend_post_fn)(void * /*data*/,
                                   struct idip_stream_base * /*stream*/,
                                   struct write_op_item * /*wop*/);

void idip_asend_helper_init_globals();

/// In order to keep consistency the client code must use only one sending
/// functional
idip_asend_helper_t * idip_asend_helper_create(struct idip_stream_base * stream,
                                               idip_asend_on_complete_cb complete_cb,
                                               void * complete_cb_data,
                                               bool segment_on);

idip_asend_helper_t * idip_asend_helper_create_ex(struct idip_stream_base * stream,
                                                  idip_asend_on_complete_cb complete_cb,
                                                  void * complete_cb_data,
                                                  bool segment_on,
                                                  idip_asend_post_fn post_fn,
                                                  void * post_fn_data);

/// It's guarenteed that the complete callback will never be called again after
/// this function returns
void idip_asend_helper_destroy(idip_asend_helper_t * self);

typedef enum idip_asend_helper_error_code {
    IDIP_ASEND_HLP_EC_NO_ERROR,
    IDIP_ASEND_HLP_EC_BUSY,
    IDIP_ASEND_HLP_EC_WINDOW_OVERFLOW,
    IDIP_ASEND_HLP_EC_NO_DATA,
    IDIP_ASEND_HLP_EC_STOPPED
} idip_asend_helper_ec_t;

idip_asend_helper_ec_t idip_asend_helper_send_vec(idip_asend_helper_t * self,
                                                  const idip_iovec_t * vec,
                                                  size_t len, bool last);
idip_asend_helper_ec_t idip_asend_helper_send_data(idip_asend_helper_t * self,
                                                   const uint8_t * data, size_t size,
                                                   bool last);

idip_asend_helper_ec_t idip_asend_helper_send_buf(idip_asend_helper_t * self,
                                                  idip_asend_buffer_t * buf,
                                                  bool last);

idip_asend_helper_ec_t idip_asend_helper_send_ack(idip_asend_helper_t * self);
idip_asend_helper_ec_t idip_asend_helper_send_eos(idip_asend_helper_t * self);
idip_asend_helper_ec_t idip_asend_helper_send_error(idip_asend_helper_t * self,
                                                    idip_rc_t error,
                                                    const char * desc);

/// It's guarenteed that the complete callback will never be called again after
/// this function returns
void idip_asend_helper_cancel(idip_asend_helper_t * self);
idip_asend_helper_ec_t idip_asend_helper_resume_payload_sending(idip_asend_helper_t * self);

idip_asend_helper_ec_t idip_asend_helper_take_buffer(idip_asend_helper_t * self,
                                                     idip_asend_buffer_t * buffer);

/// Buffer for payload. Use only special functions to work with it, do not access
/// its fields directly!
typedef struct idip_asend_buffer {
    // private
    msgpack_sbuffer buf;
} idip_asend_buffer_t;

idip_iovec_t idip_asend_buffer_try_reserve(idip_asend_buffer_t * self, size_t size);
void idip_asend_buffer_consume(idip_asend_buffer_t * self, size_t size);
void idip_asend_buffer_write(idip_asend_buffer_t * self, void * data, size_t size);
void * idip_asend_buffer_get_data(idip_asend_buffer_t * self);
size_t idip_asend_buffer_get_size(idip_asend_buffer_t * self);
void idip_asend_buffer_destroy(idip_asend_buffer_t * self);

#ifdef __cplusplus
}
#endif

#endif // ASYNC_SEND_HELPER_H
