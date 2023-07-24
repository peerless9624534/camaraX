#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/reboot.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/route.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdbool.h>
#include <signal.h>
#include "soapStub.h"    // server stubs, serializers, etc.
#include "stdsoap2.h"
#include "soapH.h"
#include "authenticate.h"
#include "lux_global.h"
#include <AppVersion.h>
#include "func_log.h"
#include "func_network.h"
#include "func_com.h"
#include "func_conf.h"
#include "lux_base.h"

#ifndef DISCOVERY_ONLY
#include "func_video.h"
#include "func_osd.h"
#include "lux_osd.h"
#include "lux_time.h"
#include <imp_isp.h>
#endif
#include "lux_config.h"
#include "lux_users.h"
#define LOG_TAG "ONVIF"
static void *soap_zalloc(struct soap *soap, size_t n)
{
    void *p = soap_malloc(soap, n);
    if( p == NULL ) {
	LOG_ERR(LOG_TAG, " soap_zalloc error \n");
	return NULL;
    }
    memset(p, 0x00, n);
    return p;
}    
    /** Web service operation '__ns3__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns3__GetServiceCapabilities(struct soap *soap, struct _ns3__GetServiceCapabilities *ns3__GetServiceCapabilities, struct _ns3__GetServiceCapabilitiesResponse *ns3__GetServiceCapabilitiesResponse){
    LOG_DBG(LOG_TAG, "\n");

    int i;
    struct ns3__ServiceCapabilities *Capabilities = (struct ns3__ServiceCapabilities *)soap_zalloc(soap, sizeof(struct ns3__ServiceCapabilities));    
    Capabilities->__any = (char *)soap_zalloc(soap, 128);
    Capabilities->__size = 30;
    Capabilities->MaxLimit = 20;//某种限制或上限
    Capabilities->MaxAuthenticationProfiles = 20;//最大认证配置文件的数量
    Capabilities->MaxPoliciesPerAuthenticationProfile = 20;//每个认证配置文件中的最大策略数量
    Capabilities->MaxSecurityLevels = 20;//最大安全级别的数量
    Capabilities->MaxRecognitionGroupsPerSecurityLevel = 20;//每个安全级别中的最大识别组数量
    Capabilities->MaxRecognitionMethodsPerRecognitionGroup =20;//表示每个识别组中的最大识别方法数量
    Capabilities->ClientSuppliedTokenSupported =xsd__boolean__true_;//是否支持客户端提供的令牌
    Capabilities->SupportedAuthenticationModes = (char *)soap_zalloc(soap, 100);//
    Capabilities->__anyAttribute =  (char *)soap_zalloc(soap, 100);
    ns3__GetServiceCapabilitiesResponse->Capabilities = Capabilities;

    return SOAP_OK;
}
    /** Web service operation '__ns3__GetAuthenticationProfileInfo' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns3__GetAuthenticationProfileInfo(struct soap *soap, struct _ns3__GetAuthenticationProfileInfo *ns3__GetAuthenticationProfileInfo, struct _ns3__GetAuthenticationProfileInfoResponse *ns3__GetAuthenticationProfileInfoResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns3__GetAuthenticationProfileInfoList' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns3__GetAuthenticationProfileInfoList(struct soap *soap, struct _ns3__GetAuthenticationProfileInfoList *ns3__GetAuthenticationProfileInfoList, struct _ns3__GetAuthenticationProfileInfoListResponse *ns3__GetAuthenticationProfileInfoListResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns3__GetAuthenticationProfiles' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns3__GetAuthenticationProfiles(struct soap *soap, struct _ns3__GetAuthenticationProfiles *ns3__GetAuthenticationProfiles, struct _ns3__GetAuthenticationProfilesResponse *ns3__GetAuthenticationProfilesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns3__GetAuthenticationProfileList' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns3__GetAuthenticationProfileList(struct soap *soap, struct _ns3__GetAuthenticationProfileList *ns3__GetAuthenticationProfileList, struct _ns3__GetAuthenticationProfileListResponse *ns3__GetAuthenticationProfileListResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns3__CreateAuthenticationProfile' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns3__CreateAuthenticationProfile(struct soap *soap, struct _ns3__CreateAuthenticationProfile *ns3__CreateAuthenticationProfile, struct _ns3__CreateAuthenticationProfileResponse *ns3__CreateAuthenticationProfileResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns3__SetAuthenticationProfile' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns3__SetAuthenticationProfile(struct soap *soap, struct _ns3__SetAuthenticationProfile *ns3__SetAuthenticationProfile, struct _ns3__SetAuthenticationProfileResponse *ns3__SetAuthenticationProfileResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns3__ModifyAuthenticationProfile' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns3__ModifyAuthenticationProfile(struct soap *soap, struct _ns3__ModifyAuthenticationProfile *ns3__ModifyAuthenticationProfile, struct _ns3__ModifyAuthenticationProfileResponse *ns3__ModifyAuthenticationProfileResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns3__DeleteAuthenticationProfile' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns3__DeleteAuthenticationProfile(struct soap *soap, struct _ns3__DeleteAuthenticationProfile *ns3__DeleteAuthenticationProfile, struct _ns3__DeleteAuthenticationProfileResponse *ns3__DeleteAuthenticationProfileResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns3__GetSecurityLevelInfo' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns3__GetSecurityLevelInfo(struct soap *soap, struct _ns3__GetSecurityLevelInfo *ns3__GetSecurityLevelInfo, struct _ns3__GetSecurityLevelInfoResponse *ns3__GetSecurityLevelInfoResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns3__GetSecurityLevelInfoList' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns3__GetSecurityLevelInfoList(struct soap *soap, struct _ns3__GetSecurityLevelInfoList *ns3__GetSecurityLevelInfoList, struct _ns3__GetSecurityLevelInfoListResponse *ns3__GetSecurityLevelInfoListResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns3__GetSecurityLevels' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns3__GetSecurityLevels(struct soap *soap, struct _ns3__GetSecurityLevels *ns3__GetSecurityLevels, struct _ns3__GetSecurityLevelsResponse *ns3__GetSecurityLevelsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns3__GetSecurityLevelList' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns3__GetSecurityLevelList(struct soap *soap, struct _ns3__GetSecurityLevelList *ns3__GetSecurityLevelList, struct _ns3__GetSecurityLevelListResponse *ns3__GetSecurityLevelListResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns3__CreateSecurityLevel' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns3__CreateSecurityLevel(struct soap *soap, struct _ns3__CreateSecurityLevel *ns3__CreateSecurityLevel, struct _ns3__CreateSecurityLevelResponse *ns3__CreateSecurityLevelResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns3__SetSecurityLevel' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns3__SetSecurityLevel(struct soap *soap, struct _ns3__SetSecurityLevel *ns3__SetSecurityLevel, struct _ns3__SetSecurityLevelResponse *ns3__SetSecurityLevelResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns3__ModifySecurityLevel' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns3__ModifySecurityLevel(struct soap *soap, struct _ns3__ModifySecurityLevel *ns3__ModifySecurityLevel, struct _ns3__ModifySecurityLevelResponse *ns3__ModifySecurityLevelResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns3__DeleteSecurityLevel' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns3__DeleteSecurityLevel(struct soap *soap, struct _ns3__DeleteSecurityLevel *ns3__DeleteSecurityLevel, struct _ns3__DeleteSecurityLevelResponse *ns3__DeleteSecurityLevelResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

