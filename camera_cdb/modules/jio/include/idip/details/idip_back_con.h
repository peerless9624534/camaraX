#ifndef IDIP_BACKCON_H
#define IDIP_BACKCON_H

#include <libwebsockets.h>

#define IDIP_HTTP_READ_BUFFER_SIZE 1024

/// An opaque data structure that represents a backward connection client.
/// It's talking with idip throught WebSocket client.
struct idip_bc_client;
enum idip_rc_t;
typedef struct idip_bc_client idip_bcc_t;

/// To use http handler as a part of protocols[] array in LWS.
extern struct lws_protocols idip_http_cli_protocol;

#ifdef __cplusplus
extern "C" {
#endif

const char* lws_reason_str(int reason);

idip_bcc_t* idip_bcc_new(struct idip_server* owner, struct lws_context* context);

idip_bcc_t* idip_bcc_add_ref(idip_bcc_t* self);

void idip_bcc_unref(idip_bcc_t* self);

enum idip_rc_t idip_bcc_start(idip_bcc_t* self, const char* uri_str);

void idip_bcc_disable(idip_bcc_t* self);

/// Return client uuid string (that generated on client creation) or NULL.
const char* idip_bcc_uuid(const idip_bcc_t* self);

void idip_bcc_set_main(idip_bcc_t* self, int is_main);

#ifdef __cplusplus
}
#endif

#endif // IDIP_BACKCON_H
