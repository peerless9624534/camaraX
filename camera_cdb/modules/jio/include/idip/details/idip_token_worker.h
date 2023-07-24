#ifndef IDIP_TOKEN_WORKER_H
#define IDIP_TOKEN_WORKER_H

struct idip_server;
struct idip_token_worker;
typedef struct idip_token_worker idip_token_worker_t;

#ifdef __cplusplus
extern "C" {
#endif

extern const long long kStopWaitMsAfterUplink;

idip_token_worker_t* idip_token_worker_new(long long wait_uplink_ms);

void idip_token_worker_delete(idip_token_worker_t* self);

void idip_token_worker_start(idip_token_worker_t* self,
                             struct idip_server *server);

void idip_token_worker_stop(idip_token_worker_t* self);

#ifdef __cplusplus
}
#endif

#endif // IDIP_TOKEN_WORKER_H
