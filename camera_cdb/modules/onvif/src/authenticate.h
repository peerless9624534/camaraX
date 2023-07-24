#ifndef __AUTHENTICATE_H__
#define __AUTHENTICATE_H__

#include <stdio.h>
#include "stdsoap2.h"

#define AUTHREALM "IPC_CAM"

#define ACCESS_CONTROL                                                                  \
    do                                                                                  \
    {                                                                                   \
        if (onvif_access_control(soap) != SOAP_OK && http_digest_auth(soap) != SOAP_OK) \
            return 401;                                                                 \
    } while (0);

/* HTTP Digest Authentication */
int http_digest_auth(struct soap* soap);
/* WS-Username Token */
int onvif_access_control(struct soap* soap);

#endif /* __AUTHENTICATE_H__ */
