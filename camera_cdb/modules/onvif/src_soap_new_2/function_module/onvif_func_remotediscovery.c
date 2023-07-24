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
char HOST_IP[20] = {0};
extern unsigned char HOST_MAC[6];
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
/******************************************************************************\
 *                                                                            *
 * Client-Side Call Stub Functions                                            *
 *                                                                            *
\******************************************************************************/

    /** Web service one-way asynchronous operation 'soap_send_SOAP_ENV__Fault' */
    SOAP_FMAC5 int SOAP_FMAC6 soap_send_SOAP_ENV__Fault(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *faultcode, char *faultstring, char *faultactor, struct SOAP_ENV__Detail *detail, struct SOAP_ENV__Code *SOAP_ENV__Code, struct SOAP_ENV__Reason *SOAP_ENV__Reason, char *SOAP_ENV__Node, char *SOAP_ENV__Role, struct SOAP_ENV__Detail *SOAP_ENV__Detail){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service one-way asynchronous operation 'soap_recv_SOAP_ENV__Fault' */
    SOAP_FMAC5 int SOAP_FMAC6 soap_recv_SOAP_ENV__Fault(struct soap *soap, struct SOAP_ENV__Fault *_param_1){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service one-way asynchronous operation 'soap_send___wsdd__Hello' */
    SOAP_FMAC5 int SOAP_FMAC6 soap_send___wsdd__Hello(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsdd__HelloType *wsdd__Hello){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service one-way asynchronous operation 'soap_recv___wsdd__Hello' */
    SOAP_FMAC5 int SOAP_FMAC6 soap_recv___wsdd__Hello(struct soap *soap, struct __wsdd__Hello *_param_1){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service one-way asynchronous operation 'soap_send___wsdd__Bye' */
    SOAP_FMAC5 int SOAP_FMAC6 soap_send___wsdd__Bye(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsdd__ByeType *wsdd__Bye){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service one-way asynchronous operation 'soap_recv___wsdd__Bye' */
    SOAP_FMAC5 int SOAP_FMAC6 soap_recv___wsdd__Bye(struct soap *soap, struct __wsdd__Bye *_param_1){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service one-way asynchronous operation 'soap_send___wsdd__Probe' */
    SOAP_FMAC5 int SOAP_FMAC6 soap_send___wsdd__Probe(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsdd__ProbeType *wsdd__Probe){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service one-way asynchronous operation 'soap_recv___wsdd__Probe' */
    SOAP_FMAC5 int SOAP_FMAC6 soap_recv___wsdd__Probe(struct soap *soap, struct __wsdd__Probe *_param_1){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__wsdd__Probe' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 soap_send___wsdd__ProbeMatches(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsdd__ProbeMatchesType *wsdd__ProbeMatches)
{	struct __wsdd__ProbeMatches soap_tmp___wsdd__ProbeMatches;
    if (soap_action == NULL)
	soap_action = "http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/ProbeMatches";
    soap_tmp___wsdd__ProbeMatches.wsdd__ProbeMatches = wsdd__ProbeMatches;
    soap_begin(soap);
    soap->encodingStyle = NULL; /* use SOAP literal style */
    soap_serializeheader(soap);
    soap_serialize___wsdd__ProbeMatches(soap, &soap_tmp___wsdd__ProbeMatches);
    if (soap_begin_count(soap))
	return soap->error;
    if ((soap->mode & SOAP_IO_LENGTH))
    {	if (soap_envelope_begin_out(soap)
	    || soap_putheader(soap)
	    || soap_body_begin_out(soap)
	    || soap_put___wsdd__ProbeMatches(soap, &soap_tmp___wsdd__ProbeMatches, "-wsdd:ProbeMatches", "")
	    || soap_body_end_out(soap)
	    || soap_envelope_end_out(soap))
	    return soap->error;
    }
    if (soap_end_count(soap))
	return soap->error;
    if (soap_connect(soap, soap_endpoint, soap_action)
	|| soap_envelope_begin_out(soap)
	|| soap_putheader(soap)
	|| soap_body_begin_out(soap)
	|| soap_put___wsdd__ProbeMatches(soap, &soap_tmp___wsdd__ProbeMatches, "-wsdd:ProbeMatches", "")
	|| soap_body_end_out(soap)
	|| soap_envelope_end_out(soap)
	|| soap_end_send(soap))
	return soap_closesock(soap);
    return SOAP_OK;
}
    /** Web service one-way asynchronous operation 'soap_recv___wsdd__ProbeMatches' */
    SOAP_FMAC5 int SOAP_FMAC6 soap_recv___wsdd__ProbeMatches(struct soap *soap, struct __wsdd__ProbeMatches *_param_1){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service one-way asynchronous operation 'soap_send___wsdd__Resolve' */
    SOAP_FMAC5 int SOAP_FMAC6 soap_send___wsdd__Resolve(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsdd__ResolveType *wsdd__Resolve){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service one-way asynchronous operation 'soap_recv___wsdd__Resolve' */
    SOAP_FMAC5 int SOAP_FMAC6 soap_recv___wsdd__Resolve(struct soap *soap, struct __wsdd__Resolve *_param_1){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service one-way asynchronous operation 'soap_send___wsdd__ResolveMatches' */
    SOAP_FMAC5 int SOAP_FMAC6 soap_send___wsdd__ResolveMatches(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsdd__ResolveMatchesType *wsdd__ResolveMatches){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service one-way asynchronous operation 'soap_recv___wsdd__ResolveMatches' */
    SOAP_FMAC5 int SOAP_FMAC6 soap_recv___wsdd__ResolveMatches(struct soap *soap, struct __wsdd__ResolveMatches *_param_1){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    
    /** Web service synchronous operation 'soap_call___tdn__Hello' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 soap_call___tdn__Hello(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsdd__HelloType tdn__Hello, struct wsdd__ResolveType *tdn__HelloResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service asynchronous operation 'soap_send___tdn__Hello' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 soap_send___tdn__Hello(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsdd__HelloType tdn__Hello){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service asynchronous operation 'soap_recv___tdn__Hello' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tdn__Hello(struct soap *soap, struct wsdd__ResolveType *tdn__HelloResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    
    /** Web service synchronous operation 'soap_call___tdn__Bye' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 soap_call___tdn__Bye(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsdd__ByeType tdn__Bye, struct wsdd__ResolveType *tdn__ByeResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service asynchronous operation 'soap_send___tdn__Bye' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 soap_send___tdn__Bye(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsdd__ByeType tdn__Bye){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service asynchronous operation 'soap_recv___tdn__Bye' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tdn__Bye(struct soap *soap, struct wsdd__ResolveType *tdn__ByeResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    
    /** Web service synchronous operation 'soap_call___tdn__Probe' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 soap_call___tdn__Probe(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsdd__ProbeType tdn__Probe, struct wsdd__ProbeMatchesType *tdn__ProbeResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service asynchronous operation 'soap_send___tdn__Probe' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 soap_send___tdn__Probe(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsdd__ProbeType tdn__Probe){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service asynchronous operation 'soap_recv___tdn__Probe' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tdn__Probe(struct soap *soap, struct wsdd__ProbeMatchesType *tdn__ProbeResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/******************************************************************************\
 *                                                                            *
 * Server-Side Operations                                                     *
 *                                                                            *
\******************************************************************************/

    /** Web service one-way operation 'SOAP_ENV__Fault' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
    SOAP_FMAC5 int SOAP_FMAC6 SOAP_ENV__Fault(struct soap* soap, char *faultcode, char *faultstring, char *faultactor, struct SOAP_ENV__Detail *detail, struct SOAP_ENV__Code *SOAP_ENV__Code, struct SOAP_ENV__Reason *SOAP_ENV__Reason, char *SOAP_ENV__Node, char *SOAP_ENV__Role, struct SOAP_ENV__Detail *SOAP_ENV__Detail){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service one-way operation '__wsdd__Hello' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
    SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Hello(struct soap* soap, struct wsdd__HelloType *wsdd__Hello){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service one-way operation '__wsdd__Bye' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
    SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Bye(struct soap* soap, struct wsdd__ByeType *wsdd__Bye){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Probe(struct soap* soap, struct wsdd__ProbeType *wsdd__Probe)
{
    LOG_DBG(LOG_TAG, "\n");

	#define MACH_ADDR_LENGTH 6
	#define INFO_LENGTH 512
	#define LARGE_INFO_LENGTH 1024
	#define SMALL_INFO_LENGTH 512

    int ret = 0;
    char _IPAddr[INFO_LENGTH] = {0};
    char _HwId[1024] = {0};

    wsdd__ProbeMatchesType ProbeMatches;

    LOG_DBG(LOG_TAG, "wsdd__Probe->Types=%s\n",wsdd__Probe->Types);

    ProbeMatches.ProbeMatch =
	(struct wsdd__ProbeMatchType *)soap_zalloc(soap, sizeof(struct wsdd__ProbeMatchType));
    ProbeMatches.ProbeMatch->XAddrs =
	(char *)soap_zalloc(soap, sizeof(char) * INFO_LENGTH);
    ProbeMatches.ProbeMatch->Types =
	(char *)soap_zalloc(soap, sizeof(char) * INFO_LENGTH);
    ProbeMatches.ProbeMatch->Scopes =
	(struct wsdd__ScopesType*)soap_zalloc(soap,sizeof(struct wsdd__ScopesType));
#if 0
    ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties =
	(struct wsa__ReferencePropertiesType*)soap_zalloc(soap,sizeof(struct wsa__ReferencePropertiesType));
    ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters =
	(struct wsa__ReferenceParametersType*)soap_zalloc(soap,sizeof(struct wsa__ReferenceParametersType));
    ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName =
	(struct wsa__ServiceNameType*)soap_zalloc(soap,sizeof(struct wsa__ServiceNameType));
    ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType =
	(char **)soap_zalloc(soap, sizeof(char *) * SMALL_INFO_LENGTH);

    ProbeMatches.ProbeMatch->wsa__EndpointReference.__any =
	(char **)soap_zalloc(soap, sizeof(char*) * SMALL_INFO_LENGTH);
    ProbeMatches.ProbeMatch->wsa__EndpointReference.__anyAttribute =
	(char *)soap_zalloc(soap, sizeof(char) * SMALL_INFO_LENGTH);
#endif
    ProbeMatches.ProbeMatch->wsa__EndpointReference.Address =
	(char *)soap_zalloc(soap, sizeof(char) * INFO_LENGTH);

    //ret = device_mac_get((char *)"eth0", HOST_MAC);
    //if (ret < 0) {
	//LOG_ERR(LOG_TAG, "ERROR: can't get host mac address!\n");
	//return SOAP_ERR;
    //}
    //sprintf(_HwId,"urn:uuid:2419d68a-2dd2-21b2-a205-%02X%02X%02X%02X%02X%02X", HOST_MAC[0], HOST_MAC[1], HOST_MAC[2], HOST_MAC[3], HOST_MAC[4], HOST_MAC[5]);
    //sprintf(_HwId,"urn:uuid:2419d6-21b2-a205-%02X%02X%02X%02X%02X%02X", HOST_MAC[0], HOST_MAC[1], HOST_MAC[2], HOST_MAC[3], HOST_MAC[4], HOST_MAC[5]);
    sprintf(_HwId, "IPC-%s", g_serial_str);

    ret = LUX_Network_getip2((char *)"eth0", HOST_IP);
    if (ret < 0) {
	LOG_ERR(LOG_TAG, "ERROR: can't get host ip address!\n");
	return SOAP_ERR;
    }

    sprintf(_IPAddr, "http://%s:%d/onvif/device_service",HOST_IP,SERVER_LISTEN_PORT);
    LOG_DBG(LOG_TAG, "Onvif Server Address: %s\n", _IPAddr);

    ProbeMatches.__sizeProbeMatch = 1;
    ProbeMatches.ProbeMatch->Scopes->__item = (char *)soap_zalloc(soap, 1024);

    //Scopes MUST BE

    strcat(ProbeMatches.ProbeMatch->Scopes->__item, "onvif://www.onvif.org/location/country/china ");
    //strcat(ProbeMatches.ProbeMatch->Scopes->__item, "onvif://www.onvif.org/name/Dahua ");
    //strcat(ProbeMatches.ProbeMatch->Scopes->__item, "onvif://www.onvif.org/hardware/IPC-HFW2225B ");
    strcat(ProbeMatches.ProbeMatch->Scopes->__item, "onvif://www.onvif.org/name/Xcam ");
    strcat(ProbeMatches.ProbeMatch->Scopes->__item, "onvif://www.onvif.org/hardware/IPC-Xcam ");

    strcat(ProbeMatches.ProbeMatch->Scopes->__item, "onvif://www.onvif.org/Profile/Streaming ");
    strcat(ProbeMatches.ProbeMatch->Scopes->__item, "onvif://www.onvif.org/type/Network_Video_Transmitter ");
    strcat(ProbeMatches.ProbeMatch->Scopes->__item, "onvif://www.onvif.org/extension/unique_identifier ");
    strcat(ProbeMatches.ProbeMatch->Scopes->__item, "onvif://www.onvif.org/Profile/Q/Operational");
    //strcat(ProbeMatches.ProbeMatch->Scopes->__item, "onvif://www.onvif.org/type/Video\n");
    //strcat(ProbeMatches.ProbeMatch->Scopes->__item, "onvif://www.onvif.org/name/XCamera_IPC\n");

    ProbeMatches.ProbeMatch->Scopes->MatchBy = NULL;
    strcpy(ProbeMatches.ProbeMatch->XAddrs, _IPAddr);
    strcpy(ProbeMatches.ProbeMatch->Types, wsdd__Probe->Types);

    LOG_DBG(LOG_TAG, "wsdd__Probe->Types=%s\n",wsdd__Probe->Types);
    ProbeMatches.ProbeMatch->MetadataVersion = 1;
    
    //ws-discovery
#if 0
    ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties->__size = 0;
    ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties->__any = NULL;
    ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters->__size = 0;
    ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters->__any = NULL;

    ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType[0] = (char *)soap_zalloc(soap, sizeof(char) * SMALL_INFO_LENGTH);
    //ws-discovery
    strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType[0], "xsd:ttl");
    ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->__item = NULL;
    ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->PortName = NULL;
    ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->__anyAttribute = NULL;
    ProbeMatches.ProbeMatch->wsa__EndpointReference.__any[0] = (char *)soap_zalloc(soap, sizeof(char) * SMALL_INFO_LENGTH);
    strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.__any[0], "Any");
    strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.__anyAttribute, "Attribute");
    ProbeMatches.ProbeMatch->wsa__EndpointReference.__size = 0;
#endif
    strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.Address, _HwId);

    soap->header->wsa__To = "http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous";
    soap->header->wsa__Action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches";
    soap->header->wsa__RelatesTo = (struct wsa__Relationship*)soap_zalloc(soap, sizeof(struct wsa__Relationship));
    soap->header->wsa__RelatesTo->__item = soap->header->wsa__MessageID;
    soap->header->wsa__RelatesTo->RelationshipType = NULL;
    soap->header->wsa__RelatesTo->__anyAttribute = NULL;

    soap->header->wsa__MessageID = (char *)soap_zalloc(soap, sizeof(char) * INFO_LENGTH);
    strcpy(soap->header->wsa__MessageID,_HwId+4);

    if (SOAP_OK == soap_send___wsdd__ProbeMatches(soap, "http://", NULL, &ProbeMatches))
    {
	LOG_DBG(LOG_TAG, "send ProbeMatches success !\n");
	return SOAP_OK;
    }

    return soap->error;
}
    /** Web service one-way operation '__wsdd__ProbeMatches' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
    SOAP_FMAC5 int SOAP_FMAC6 __wsdd__ProbeMatches(struct soap* soap, struct wsdd__ProbeMatchesType *wsdd__ProbeMatches){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service one-way operation '__wsdd__Resolve' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
    SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Resolve(struct soap* soap, struct wsdd__ResolveType *wsdd__Resolve){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service one-way operation '__wsdd__ResolveMatches' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
    SOAP_FMAC5 int SOAP_FMAC6 __wsdd__ResolveMatches(struct soap* soap, struct wsdd__ResolveMatchesType *wsdd__ResolveMatches){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tdn__Hello' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tdn__Hello(struct soap* soap, struct wsdd__HelloType tdn__Hello, struct wsdd__ResolveType *tdn__HelloResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tdn__Bye' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tdn__Bye(struct soap* soap, struct wsdd__ByeType tdn__Bye, struct wsdd__ResolveType *tdn__ByeResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tdn__Probe' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tdn__Probe(struct soap* soap, struct wsdd__ProbeType tdn__Probe, struct wsdd__ProbeMatchesType *tdn__ProbeResponse){
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

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve_SOAP_ENV__Fault(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsdd__Hello(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsdd__Bye(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsdd__Probe(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsdd__ProbeMatches(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsdd__Resolve(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsdd__ResolveMatches(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tdn__Hello(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tdn__Bye(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tdn__Probe(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

