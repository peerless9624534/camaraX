#ifndef IDIP_LWS_HELPERS_H
#define IDIP_LWS_HELPERS_H

#include <libwebsockets.h>

#ifdef __cplusplus
extern "C" {
#endif

const char* lws_reason_str(int reason);

const char* lejp_clbk_reason_str(enum lejp_callbacks reason);

#ifdef __cplusplus
}
#endif

#endif // IDIP_LWS_HELPERS_H
