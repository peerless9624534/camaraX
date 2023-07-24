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

// /******************************************************************************\
//  *                                                                            *
//  * Client-Side Call Stub Functions                                            *
//  *                                                                            *
// \******************************************************************************/

    
//     /** Web service synchronous operation 'soap_call___tls__GetServiceCapabilities' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tls__GetServiceCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__GetServiceCapabilities *tls__GetServiceCapabilities, struct _tls__GetServiceCapabilitiesResponse *tls__GetServiceCapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tls__GetServiceCapabilities' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tls__GetServiceCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__GetServiceCapabilities *tls__GetServiceCapabilities){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tls__GetServiceCapabilities' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tls__GetServiceCapabilities(struct soap *soap, struct _tls__GetServiceCapabilitiesResponse *tls__GetServiceCapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tls__GetLayout' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tls__GetLayout(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__GetLayout *tls__GetLayout, struct _tls__GetLayoutResponse *tls__GetLayoutResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tls__GetLayout' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tls__GetLayout(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__GetLayout *tls__GetLayout){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tls__GetLayout' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tls__GetLayout(struct soap *soap, struct _tls__GetLayoutResponse *tls__GetLayoutResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tls__SetLayout' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tls__SetLayout(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__SetLayout *tls__SetLayout, struct _tls__SetLayoutResponse *tls__SetLayoutResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tls__SetLayout' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tls__SetLayout(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__SetLayout *tls__SetLayout){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tls__SetLayout' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tls__SetLayout(struct soap *soap, struct _tls__SetLayoutResponse *tls__SetLayoutResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tls__GetDisplayOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tls__GetDisplayOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__GetDisplayOptions *tls__GetDisplayOptions, struct _tls__GetDisplayOptionsResponse *tls__GetDisplayOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tls__GetDisplayOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tls__GetDisplayOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__GetDisplayOptions *tls__GetDisplayOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tls__GetDisplayOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tls__GetDisplayOptions(struct soap *soap, struct _tls__GetDisplayOptionsResponse *tls__GetDisplayOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tls__GetPaneConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tls__GetPaneConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__GetPaneConfigurations *tls__GetPaneConfigurations, struct _tls__GetPaneConfigurationsResponse *tls__GetPaneConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tls__GetPaneConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tls__GetPaneConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__GetPaneConfigurations *tls__GetPaneConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tls__GetPaneConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tls__GetPaneConfigurations(struct soap *soap, struct _tls__GetPaneConfigurationsResponse *tls__GetPaneConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tls__GetPaneConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tls__GetPaneConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__GetPaneConfiguration *tls__GetPaneConfiguration, struct _tls__GetPaneConfigurationResponse *tls__GetPaneConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tls__GetPaneConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tls__GetPaneConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__GetPaneConfiguration *tls__GetPaneConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tls__GetPaneConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tls__GetPaneConfiguration(struct soap *soap, struct _tls__GetPaneConfigurationResponse *tls__GetPaneConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tls__SetPaneConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tls__SetPaneConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__SetPaneConfigurations *tls__SetPaneConfigurations, struct _tls__SetPaneConfigurationsResponse *tls__SetPaneConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tls__SetPaneConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tls__SetPaneConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__SetPaneConfigurations *tls__SetPaneConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tls__SetPaneConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tls__SetPaneConfigurations(struct soap *soap, struct _tls__SetPaneConfigurationsResponse *tls__SetPaneConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tls__SetPaneConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tls__SetPaneConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__SetPaneConfiguration *tls__SetPaneConfiguration, struct _tls__SetPaneConfigurationResponse *tls__SetPaneConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tls__SetPaneConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tls__SetPaneConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__SetPaneConfiguration *tls__SetPaneConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tls__SetPaneConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tls__SetPaneConfiguration(struct soap *soap, struct _tls__SetPaneConfigurationResponse *tls__SetPaneConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tls__CreatePaneConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tls__CreatePaneConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__CreatePaneConfiguration *tls__CreatePaneConfiguration, struct _tls__CreatePaneConfigurationResponse *tls__CreatePaneConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tls__CreatePaneConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tls__CreatePaneConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__CreatePaneConfiguration *tls__CreatePaneConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tls__CreatePaneConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tls__CreatePaneConfiguration(struct soap *soap, struct _tls__CreatePaneConfigurationResponse *tls__CreatePaneConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tls__DeletePaneConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tls__DeletePaneConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__DeletePaneConfiguration *tls__DeletePaneConfiguration, struct _tls__DeletePaneConfigurationResponse *tls__DeletePaneConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tls__DeletePaneConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tls__DeletePaneConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tls__DeletePaneConfiguration *tls__DeletePaneConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tls__DeletePaneConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tls__DeletePaneConfiguration(struct soap *soap, struct _tls__DeletePaneConfigurationResponse *tls__DeletePaneConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

/******************************************************************************\
 *                                                                            *
 * Server-Side Operations                                                     *
 *                                                                            *
\******************************************************************************/

    /** Web service operation '__tls__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tls__GetServiceCapabilities(struct soap* soap, struct _tls__GetServiceCapabilities *tls__GetServiceCapabilities, struct _tls__GetServiceCapabilitiesResponse *tls__GetServiceCapabilitiesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tls__GetLayout' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tls__GetLayout(struct soap* soap, struct _tls__GetLayout *tls__GetLayout, struct _tls__GetLayoutResponse *tls__GetLayoutResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tls__SetLayout' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tls__SetLayout(struct soap* soap, struct _tls__SetLayout *tls__SetLayout, struct _tls__SetLayoutResponse *tls__SetLayoutResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tls__GetDisplayOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tls__GetDisplayOptions(struct soap* soap, struct _tls__GetDisplayOptions *tls__GetDisplayOptions, struct _tls__GetDisplayOptionsResponse *tls__GetDisplayOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tls__GetPaneConfigurations' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tls__GetPaneConfigurations(struct soap* soap, struct _tls__GetPaneConfigurations *tls__GetPaneConfigurations, struct _tls__GetPaneConfigurationsResponse *tls__GetPaneConfigurationsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tls__GetPaneConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tls__GetPaneConfiguration(struct soap* soap, struct _tls__GetPaneConfiguration *tls__GetPaneConfiguration, struct _tls__GetPaneConfigurationResponse *tls__GetPaneConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tls__SetPaneConfigurations' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tls__SetPaneConfigurations(struct soap* soap, struct _tls__SetPaneConfigurations *tls__SetPaneConfigurations, struct _tls__SetPaneConfigurationsResponse *tls__SetPaneConfigurationsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tls__SetPaneConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tls__SetPaneConfiguration(struct soap* soap, struct _tls__SetPaneConfiguration *tls__SetPaneConfiguration, struct _tls__SetPaneConfigurationResponse *tls__SetPaneConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tls__CreatePaneConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tls__CreatePaneConfiguration(struct soap* soap, struct _tls__CreatePaneConfiguration *tls__CreatePaneConfiguration, struct _tls__CreatePaneConfigurationResponse *tls__CreatePaneConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tls__DeletePaneConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tls__DeletePaneConfiguration(struct soap* soap, struct _tls__DeletePaneConfiguration *tls__DeletePaneConfiguration, struct _tls__DeletePaneConfigurationResponse *tls__DeletePaneConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

// /******************************************************************************\
//  *                                                                            *
//  * Server-Side Skeletons to Invoke Service Operations                         *
//  *                                                                            *
// \******************************************************************************/

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve_request(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tls__GetServiceCapabilities(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tls__GetLayout(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tls__SetLayout(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tls__GetDisplayOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tls__GetPaneConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tls__GetPaneConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tls__SetPaneConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tls__SetPaneConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tls__CreatePaneConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tls__DeletePaneConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
