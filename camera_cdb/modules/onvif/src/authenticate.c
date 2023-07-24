#include "authenticate.h"
#include <jwhead.h>
#include <lux_users.h>
#include <httpda.h>

int onvif_access_control(struct soap* soap)
{
    int ret = 0;
    char passwd[32] = {0};
    const char* username = soap_wsse_get_Username(soap);
    if (username == NULL)
    {
        MYERROR("username is null\n");
        return 401;
    }
    MYTRACE("WS-Username token username is [%s]\n", username);

    if (LUX_UserMngr_GetPasswd((char*)username, passwd) != 0)
    {
        MYERROR("Username[%s] is not exists\n", username);
        return 401;
    }

    MYTRACE("WS-Username token passwd is [%s]\n", passwd);
    if (ret = soap_wsse_verify_Password(soap, passwd))
    {
        soap_wsse_delete_Security(soap);
        MYERROR("verify Username[%s] Password[%s] is fault,Error Number is [%d]\n", username, passwd, ret);
        return 401;
    }
    soap_wsse_delete_Security(soap);
    MYTRACE("WS-Username token Authentication is pass\n");
    return SOAP_OK;
}

int http_digest_auth(struct soap* soap)
{
    printf("enter http_digest_auth() \n");
    
    printf("soap->authrealm:[%s] userid:[%s] passwd:[%s]\n", soap->authrealm, soap->userid, soap->passwd);

    char passwd[32] = { 0 };

    if (soap->authrealm && soap->userid)
    {
        MYTRACE("WS-Username token userid is [%s]\n", (char*)soap->userid);
        int retGetPasswd = LUX_UserMngr_GetPasswd((char*)soap->userid, passwd);
        MYTRACE("WS-Username token passwd is [%s]\n", passwd);
        if (0 == retGetPasswd)
        {
            if (!strcmp(soap->authrealm, AUTHREALM))
            {
                if (!http_da_verify_post(soap, passwd))
                {
                    MYTRACE("Digest Authentication is pass\n");
                    return SOAP_OK;
                }
            }
        }
    }
    soap->authrealm = AUTHREALM;
    
    return 401;
}
