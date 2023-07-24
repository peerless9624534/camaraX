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


//     /** Web service synchronous operation 'soap_call___tan__GetSupportedRules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tan__GetSupportedRules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetSupportedRules *tan__GetSupportedRules, struct _tan__GetSupportedRulesResponse *tan__GetSupportedRulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tan__GetSupportedRules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tan__GetSupportedRules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetSupportedRules *tan__GetSupportedRules){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tan__GetSupportedRules' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tan__GetSupportedRules(struct soap *soap, struct _tan__GetSupportedRulesResponse *tan__GetSupportedRulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tan__CreateRules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tan__CreateRules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__CreateRules *tan__CreateRules, struct _tan__CreateRulesResponse *tan__CreateRulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tan__CreateRules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tan__CreateRules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__CreateRules *tan__CreateRules){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tan__CreateRules' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tan__CreateRules(struct soap *soap, struct _tan__CreateRulesResponse *tan__CreateRulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tan__DeleteRules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tan__DeleteRules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__DeleteRules *tan__DeleteRules, struct _tan__DeleteRulesResponse *tan__DeleteRulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tan__DeleteRules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tan__DeleteRules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__DeleteRules *tan__DeleteRules){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tan__DeleteRules' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tan__DeleteRules(struct soap *soap, struct _tan__DeleteRulesResponse *tan__DeleteRulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tan__GetRules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tan__GetRules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetRules *tan__GetRules, struct _tan__GetRulesResponse *tan__GetRulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tan__GetRules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tan__GetRules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetRules *tan__GetRules){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tan__GetRules' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tan__GetRules(struct soap *soap, struct _tan__GetRulesResponse *tan__GetRulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tan__GetRuleOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tan__GetRuleOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetRuleOptions *tan__GetRuleOptions, struct _tan__GetRuleOptionsResponse *tan__GetRuleOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tan__GetRuleOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tan__GetRuleOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetRuleOptions *tan__GetRuleOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tan__GetRuleOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tan__GetRuleOptions(struct soap *soap, struct _tan__GetRuleOptionsResponse *tan__GetRuleOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tan__ModifyRules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tan__ModifyRules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__ModifyRules *tan__ModifyRules, struct _tan__ModifyRulesResponse *tan__ModifyRulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tan__ModifyRules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tan__ModifyRules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__ModifyRules *tan__ModifyRules){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tan__ModifyRules' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tan__ModifyRules(struct soap *soap, struct _tan__ModifyRulesResponse *tan__ModifyRulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tan__GetServiceCapabilities' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tan__GetServiceCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetServiceCapabilities *tan__GetServiceCapabilities, struct _tan__GetServiceCapabilitiesResponse *tan__GetServiceCapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tan__GetServiceCapabilities' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tan__GetServiceCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetServiceCapabilities *tan__GetServiceCapabilities){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tan__GetServiceCapabilities' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tan__GetServiceCapabilities(struct soap *soap, struct _tan__GetServiceCapabilitiesResponse *tan__GetServiceCapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tan__GetSupportedAnalyticsModules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tan__GetSupportedAnalyticsModules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetSupportedAnalyticsModules *tan__GetSupportedAnalyticsModules, struct _tan__GetSupportedAnalyticsModulesResponse *tan__GetSupportedAnalyticsModulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tan__GetSupportedAnalyticsModules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tan__GetSupportedAnalyticsModules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetSupportedAnalyticsModules *tan__GetSupportedAnalyticsModules){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tan__GetSupportedAnalyticsModules' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tan__GetSupportedAnalyticsModules(struct soap *soap, struct _tan__GetSupportedAnalyticsModulesResponse *tan__GetSupportedAnalyticsModulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tan__CreateAnalyticsModules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tan__CreateAnalyticsModules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__CreateAnalyticsModules *tan__CreateAnalyticsModules, struct _tan__CreateAnalyticsModulesResponse *tan__CreateAnalyticsModulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tan__CreateAnalyticsModules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tan__CreateAnalyticsModules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__CreateAnalyticsModules *tan__CreateAnalyticsModules){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tan__CreateAnalyticsModules' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tan__CreateAnalyticsModules(struct soap *soap, struct _tan__CreateAnalyticsModulesResponse *tan__CreateAnalyticsModulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tan__DeleteAnalyticsModules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tan__DeleteAnalyticsModules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__DeleteAnalyticsModules *tan__DeleteAnalyticsModules, struct _tan__DeleteAnalyticsModulesResponse *tan__DeleteAnalyticsModulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tan__DeleteAnalyticsModules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tan__DeleteAnalyticsModules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__DeleteAnalyticsModules *tan__DeleteAnalyticsModules){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tan__DeleteAnalyticsModules' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tan__DeleteAnalyticsModules(struct soap *soap, struct _tan__DeleteAnalyticsModulesResponse *tan__DeleteAnalyticsModulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tan__GetAnalyticsModules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tan__GetAnalyticsModules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetAnalyticsModules *tan__GetAnalyticsModules, struct _tan__GetAnalyticsModulesResponse *tan__GetAnalyticsModulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tan__GetAnalyticsModules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tan__GetAnalyticsModules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetAnalyticsModules *tan__GetAnalyticsModules){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tan__GetAnalyticsModules' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tan__GetAnalyticsModules(struct soap *soap, struct _tan__GetAnalyticsModulesResponse *tan__GetAnalyticsModulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tan__GetAnalyticsModuleOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tan__GetAnalyticsModuleOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetAnalyticsModuleOptions *tan__GetAnalyticsModuleOptions, struct _tan__GetAnalyticsModuleOptionsResponse *tan__GetAnalyticsModuleOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tan__GetAnalyticsModuleOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tan__GetAnalyticsModuleOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetAnalyticsModuleOptions *tan__GetAnalyticsModuleOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tan__GetAnalyticsModuleOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tan__GetAnalyticsModuleOptions(struct soap *soap, struct _tan__GetAnalyticsModuleOptionsResponse *tan__GetAnalyticsModuleOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tan__ModifyAnalyticsModules' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tan__ModifyAnalyticsModules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__ModifyAnalyticsModules *tan__ModifyAnalyticsModules, struct _tan__ModifyAnalyticsModulesResponse *tan__ModifyAnalyticsModulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tan__ModifyAnalyticsModules' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tan__ModifyAnalyticsModules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__ModifyAnalyticsModules *tan__ModifyAnalyticsModules){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tan__ModifyAnalyticsModules' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tan__ModifyAnalyticsModules(struct soap *soap, struct _tan__ModifyAnalyticsModulesResponse *tan__ModifyAnalyticsModulesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tan__GetSupportedMetadata' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tan__GetSupportedMetadata(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetSupportedMetadata *tan__GetSupportedMetadata, struct _tan__GetSupportedMetadataResponse *tan__GetSupportedMetadataResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tan__GetSupportedMetadata' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tan__GetSupportedMetadata(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetSupportedMetadata *tan__GetSupportedMetadata){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tan__GetSupportedMetadata' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tan__GetSupportedMetadata(struct soap *soap, struct _tan__GetSupportedMetadataResponse *tan__GetSupportedMetadataResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    /** Web service operation '__tan__GetSupportedRules' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tan__GetSupportedRules(struct soap *soap, struct _tan__GetSupportedRules *tan__GetSupportedRules, struct _tan__GetSupportedRulesResponse *tan__GetSupportedRulesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tan__CreateRules' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tan__CreateRules(struct soap *soap, struct _tan__CreateRules *tan__CreateRules, struct _tan__CreateRulesResponse *tan__CreateRulesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tan__DeleteRules' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tan__DeleteRules(struct soap *soap, struct _tan__DeleteRules *tan__DeleteRules, struct _tan__DeleteRulesResponse *tan__DeleteRulesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tan__GetRules' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tan__GetRules(struct soap *soap, struct _tan__GetRules *tan__GetRules, struct _tan__GetRulesResponse *tan__GetRulesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tan__GetRuleOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tan__GetRuleOptions(struct soap *soap, struct _tan__GetRuleOptions *tan__GetRuleOptions, struct _tan__GetRuleOptionsResponse *tan__GetRuleOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tan__ModifyRules' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tan__ModifyRules(struct soap *soap, struct _tan__ModifyRules *tan__ModifyRules, struct _tan__ModifyRulesResponse *tan__ModifyRulesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tan__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tan__GetServiceCapabilities(struct soap *soap, struct _tan__GetServiceCapabilities *tan__GetServiceCapabilities, struct _tan__GetServiceCapabilitiesResponse *tan__GetServiceCapabilitiesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tan__GetSupportedAnalyticsModules' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tan__GetSupportedAnalyticsModules(struct soap *soap, struct _tan__GetSupportedAnalyticsModules *tan__GetSupportedAnalyticsModules, struct _tan__GetSupportedAnalyticsModulesResponse *tan__GetSupportedAnalyticsModulesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tan__CreateAnalyticsModules' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tan__CreateAnalyticsModules(struct soap *soap, struct _tan__CreateAnalyticsModules *tan__CreateAnalyticsModules, struct _tan__CreateAnalyticsModulesResponse *tan__CreateAnalyticsModulesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tan__DeleteAnalyticsModules' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tan__DeleteAnalyticsModules(struct soap *soap, struct _tan__DeleteAnalyticsModules *tan__DeleteAnalyticsModules, struct _tan__DeleteAnalyticsModulesResponse *tan__DeleteAnalyticsModulesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tan__GetAnalyticsModules' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tan__GetAnalyticsModules(struct soap *soap, struct _tan__GetAnalyticsModules *tan__GetAnalyticsModules, struct _tan__GetAnalyticsModulesResponse *tan__GetAnalyticsModulesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tan__GetAnalyticsModuleOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tan__GetAnalyticsModuleOptions(struct soap *soap, struct _tan__GetAnalyticsModuleOptions *tan__GetAnalyticsModuleOptions, struct _tan__GetAnalyticsModuleOptionsResponse *tan__GetAnalyticsModuleOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tan__ModifyAnalyticsModules' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tan__ModifyAnalyticsModules(struct soap *soap, struct _tan__ModifyAnalyticsModules *tan__ModifyAnalyticsModules, struct _tan__ModifyAnalyticsModulesResponse *tan__ModifyAnalyticsModulesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tan__GetSupportedMetadata' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tan__GetSupportedMetadata(struct soap *soap, struct _tan__GetSupportedMetadata *tan__GetSupportedMetadata, struct _tan__GetSupportedMetadataResponse *tan__GetSupportedMetadataResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

