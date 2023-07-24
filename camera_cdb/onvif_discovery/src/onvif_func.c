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
//#include "stdsoap2.h"
//#include "soapH.h"
//#include "authenticate.h"
#include "lux_global.h"
//#include <AppVersion.h>
#include "func_log.h"
#include "func_network.h"
#include "func_com.h"
#include "func_conf.h"
//#include "lux_base.h"
#include "lux_config.h"

#ifndef DISCOVERY_ONLY
#include "func_video.h"
#include "func_osd.h"
#include "lux_osd.h"
//#include "lux_time.h"
#include <imp_isp.h>
#endif

#define LOG_TAG "ONVIF"

char HOST_IP[20] = {0};
extern unsigned char HOST_MAC[6];
#define STR_LEN_IP_ADDRESS 16

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
 * Server-Side Operations                                                     *
 *                                                                            *
\******************************************************************************/

/** Web service operation 'SOAP_ENV__Fault' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 SOAP_ENV__Fault(struct soap* soap, char *faultcode, char *faultstring, char *faultactor, struct SOAP_ENV__Detail *detail, struct SOAP_ENV__Code *SOAP_ENV__Code, struct SOAP_ENV__Reason *SOAP_ENV__Reason, char *SOAP_ENV__Node, char *SOAP_ENV__Role, struct SOAP_ENV__Detail *SOAP_ENV__Detail)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__wsdd__Hello' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Hello(struct soap* soap, struct wsdd__HelloType *wsdd__Hello)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__wsdd__Bye' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Bye(struct soap* soap, struct wsdd__ByeType *wsdd__Bye)
{
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

    ret = device_mac_get((char *)"eth0", HOST_MAC);
    if (ret < 0) {
	LOG_ERR(LOG_TAG, "ERROR: can't get host mac address!\n");
	return SOAP_ERR;
    }
    //sprintf(_HwId,"urn:uuid:2419d68a-2dd2-21b2-a205-%02X%02X%02X%02X%02X%02X", HOST_MAC[0], HOST_MAC[1], HOST_MAC[2], HOST_MAC[3], HOST_MAC[4], HOST_MAC[5]);
    //sprintf(_HwId,"urn:uuid:2419d6-21b2-a205-%02X%02X%02X%02X%02X%02X", HOST_MAC[0], HOST_MAC[1], HOST_MAC[2], HOST_MAC[3], HOST_MAC[4], HOST_MAC[5]);
    sprintf(_HwId, "IPC-%s", g_serial_str);

    ret = device_ip_get((char *)"eth0", HOST_IP);
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

/** Web service operation '__wsdd__ProbeMatches' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__ProbeMatches(struct soap* soap, struct wsdd__ProbeMatchesType *wsdd__ProbeMatches)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__wsdd__Resolve' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Resolve(struct soap* soap, struct wsdd__ResolveType *wsdd__Resolve)
{
    //LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__wsdd__ResolveMatches' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__ResolveMatches(struct soap* soap, struct wsdd__ResolveMatchesType *wsdd__ResolveMatches)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation 'SOAP_ENV__Fault_alex' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 SOAP_ENV__Fault_alex(struct soap* soap, char *faultcode, char *faultstring, char *faultactor, struct SOAP_ENV__Detail *detail, struct SOAP_ENV__Code *SOAP_ENV__Code, struct SOAP_ENV__Reason *SOAP_ENV__Reason, char *SOAP_ENV__Node, char *SOAP_ENV__Role, struct SOAP_ENV__Detail *SOAP_ENV__Detail)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

#ifndef DISCOVERY_ONLY
/** Web service operation '__dn__Hello' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __dn__Hello(struct soap* soap, struct wsdd__HelloType dn__Hello, struct wsdd__ResolveType *dn__HelloResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__dn__Bye' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __dn__Bye(struct soap* soap, struct wsdd__ByeType dn__Bye, struct wsdd__ResolveType *dn__ByeResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__dn__Probe' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __dn__Probe(struct soap* soap, struct wsdd__ProbeType dn__Probe, struct wsdd__ProbeMatchesType *dn__ProbeResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetServices' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServices(struct soap* soap, struct _tds__GetServices *tds__GetServices, struct _tds__GetServicesResponse *tds__GetServicesResponse){

    int ret = 0;
    char ip[20];
    char url[MAX_XADDR_LEN];

	/* get host ip */
    ret = device_ip_get((char *)"eth0", ip);
    if (ret < 0) {
		LOG_ERR(LOG_TAG, "ERROR: can't get host ip address!\n");
		return SOAP_ERR;
    }

    struct tds__Service *Service = (struct tds__Service *)soap_zalloc(soap, sizeof(struct tds__Service));
    Service->Namespace = (char *)soap_zalloc(soap, MAX_NAMESPACE_LEN);
    Service->XAddr = (char *)soap_zalloc(soap, MAX_XADDR_LEN);
    Service->Version = (struct tt__OnvifVersion *)soap_zalloc(soap, sizeof(struct tt__OnvifVersion));
    Service->Version->Major = 2;
    Service->Version->Minor = 40;

    Service->Capabilities = NULL;
    tds__GetServicesResponse->__sizeService = 1;

    /* set Analytics service */
    strcpy(Service->Namespace, "http://www.onvif.org/ver20/analytics/wsdl");
    sprintf(url, "http://%s:%d/onvif/device_service",ip, SERVER_LISTEN_PORT);

    //set XAddr value
    strcpy(Service->XAddr, url);

    /* set value */
    tds__GetServicesResponse->Service = Service;

    return SOAP_OK;
}

/** Web service operation '__tds__GetServiceCapabilities' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServiceCapabilities(struct soap* soap, struct _tds__GetServiceCapabilities *tds__GetServiceCapabilities, struct _tds__GetServiceCapabilitiesResponse *tds__GetServiceCapabilitiesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetDeviceInformation' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDeviceInformation(struct soap* soap, struct _tds__GetDeviceInformation *tds__GetDeviceInformation, struct _tds__GetDeviceInformationResponse *tds__GetDeviceInformationResponse)
{
    int ret = 0;
    LOG_DBG(LOG_TAG, "\n");

    //ACCESS_CONTROL;

    /* malloc */
    tds__GetDeviceInformationResponse->Manufacturer = (char *)soap_zalloc(soap, INFO_STRING_LEN);
    tds__GetDeviceInformationResponse->Model = (char *)soap_zalloc(soap, INFO_STRING_LEN);
    tds__GetDeviceInformationResponse->FirmwareVersion = (char *)soap_zalloc(soap, INFO_STRING_LEN);
    tds__GetDeviceInformationResponse->SerialNumber = (char *)soap_zalloc(soap, INFO_STRING_LEN);
    tds__GetDeviceInformationResponse->HardwareId = (char *)soap_zalloc(soap, INFO_STRING_LEN);

    strcpy(tds__GetDeviceInformationResponse->Manufacturer, TDS_DEVICE_INFO_MANUFACTURER);
    sprintf(tds__GetDeviceInformationResponse->Model, "%s %s", g_device_type, g_device_model);
    sprintf(tds__GetDeviceInformationResponse->FirmwareVersion, "%s(%s)", APP_DETAIL_VERSION, BUILD_DATE);

    strcpy(tds__GetDeviceInformationResponse->SerialNumber, g_serial_str);
    ret = device_mac_get_by_str((char *)"eth0", (unsigned char *)tds__GetDeviceInformationResponse->HardwareId); //eth0
    if (ret < 0) {
	    LOG_ERR(LOG_TAG, "ERROR: can't get host mac address!\n");
	    return SOAP_ERR;
    }

    return SOAP_OK;
}

/** Web service operation '__tds__SetSystemDateAndTime' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemDateAndTime(struct soap* soap, struct _tds__SetSystemDateAndTime *tds__SetSystemDateAndTime, struct _tds__SetSystemDateAndTimeResponse *tds__SetSystemDateAndTimeResponse)
{
    LOG_DBG(LOG_TAG, "\n");

    char dateBuf[64];
    char cmd[128];

    sprintf(dateBuf, "%04d-%02d-%02d %02d:%02d:%02d", 
        tds__SetSystemDateAndTime->UTCDateTime->Date->Year,
        tds__SetSystemDateAndTime->UTCDateTime->Date->Month, 
        tds__SetSystemDateAndTime->UTCDateTime->Date->Day, 
        tds__SetSystemDateAndTime->UTCDateTime->Time->Hour,
        tds__SetSystemDateAndTime->UTCDateTime->Time->Minute,
        tds__SetSystemDateAndTime->UTCDateTime->Time->Second);
    sprintf(cmd, "date -s \"%s\"", dateBuf);     
    LUX_BASE_System_CMD(cmd);

    printf("============= onvif synchronization time: [%s] \n", dateBuf);

    if (tds__SetSystemDateAndTime->TimeZone && tds__SetSystemDateAndTime->TimeZone->TZ)
    {
        printf("============= onvif timezone:[%s] \n", tds__SetSystemDateAndTime->TimeZone->TZ);
    }
    else
    {
        printf("============= onvif timezone is null \n");
    }

    return SOAP_OK;
}

/** Web service operation '__tds__GetSystemDateAndTime' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemDateAndTime(struct soap* soap, struct _tds__GetSystemDateAndTime *tds__GetSystemDateAndTime, struct _tds__GetSystemDateAndTimeResponse *tds__GetSystemDateAndTimeResponse)
{
    LOG_DBG(LOG_TAG, "\n");

    struct tt__SystemDateTime *SystemDateAndTime = (struct tt__SystemDateTime *)soap_zalloc(soap, sizeof(struct tt__SystemDateTime));
    SystemDateAndTime->TimeZone = (struct tt__TimeZone *)soap_zalloc(soap, sizeof(struct tt__TimeZone));
    SystemDateAndTime->TimeZone->TZ = (char *)soap_zalloc(soap, 100);
    SystemDateAndTime->UTCDateTime = (struct tt__DateTime *)soap_zalloc(soap, sizeof(struct tt__DateTime));
    SystemDateAndTime->UTCDateTime->Time = (struct tt__Time *)soap_zalloc(soap, sizeof(struct tt__Time));
    SystemDateAndTime->UTCDateTime->Date = (struct tt__Date *)soap_zalloc(soap, sizeof(struct tt__Date));
    SystemDateAndTime->LocalDateTime = (struct tt__DateTime *)soap_zalloc(soap, sizeof(struct tt__DateTime));
    SystemDateAndTime->LocalDateTime->Time = (struct tt__Time *)soap_zalloc(soap, sizeof(struct tt__Time));
    SystemDateAndTime->LocalDateTime->Date = (struct tt__Date *)soap_zalloc(soap, sizeof(struct tt__Date));
    SystemDateAndTime->Extension = (struct tt__SystemDateTimeExtension *)soap_zalloc(soap, sizeof(struct tt__SystemDateTimeExtension));

    SystemDateAndTime->DateTimeType = tt__SetDateTimeType__Manual;//0
    SystemDateAndTime->DaylightSavings = xsd__boolean__false_;//0

    strcpy(SystemDateAndTime->TimeZone->TZ, g_JioConfig->timezone);

    time_t timev;

    timev = time(NULL);
    struct tm mytm = *localtime(&timev);

    SystemDateAndTime->UTCDateTime->Date->Year  = mytm.tm_year+1900;
    SystemDateAndTime->UTCDateTime->Date->Month = mytm.tm_mon + 1;
    SystemDateAndTime->UTCDateTime->Date->Day   = mytm.tm_mday;

    SystemDateAndTime->UTCDateTime->Time->Hour   = mytm.tm_hour;
    SystemDateAndTime->UTCDateTime->Time->Minute = mytm.tm_min;
    SystemDateAndTime->UTCDateTime->Time->Second = mytm.tm_sec;

    timev += g_configData_st->g_JioConfig.timezone_offset_s;
    mytm = *localtime(&timev);

    SystemDateAndTime->LocalDateTime->Date->Year  = mytm.tm_year + 1900;
    SystemDateAndTime->LocalDateTime->Date->Month = mytm.tm_mon + 1;
    SystemDateAndTime->LocalDateTime->Date->Day   = mytm.tm_mday;

    SystemDateAndTime->LocalDateTime->Time->Hour   = mytm.tm_hour;
    SystemDateAndTime->LocalDateTime->Time->Minute = mytm.tm_min;
    SystemDateAndTime->LocalDateTime->Time->Second = mytm.tm_sec;

    tds__GetSystemDateAndTimeResponse->SystemDateAndTime = SystemDateAndTime;

    return SOAP_OK;
}

/** Web service operation '__tds__SetSystemFactoryDefault' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemFactoryDefault(struct soap* s, struct _tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault, struct _tds__SetSystemFactoryDefaultResponse *tds__SetSystemFactoryDefaultResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    LUX_BASE_System_CMD("/tmp/factory_reset.sh");
    return SOAP_OK;
}

/** Web service operation '__tds__UpgradeSystemFirmware' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__UpgradeSystemFirmware(struct soap* soap, struct _tds__UpgradeSystemFirmware *tds__UpgradeSystemFirmware, struct _tds__UpgradeSystemFirmwareResponse *tds__UpgradeSystemFirmwareResponse)
{
    LOG_DBG(LOG_TAG, "\n");
	return SOAP_OK;
}

/** Web service operation '__tds__SystemReboot' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SystemReboot(struct soap* soap, struct _tds__SystemReboot *tds__SystemReboot, struct _tds__SystemRebootResponse *tds__SystemRebootResponse)
{
    LOG_DBG(LOG_TAG, "\n");

	tds__SystemRebootResponse->Message = (char *)soap_zalloc(soap, INFO_STRING_LEN);
    strcpy(tds__SystemRebootResponse->Message, "Rebooting in 10 seconds");

    sleep(10);
    sync();
    reboot(RB_AUTOBOOT);
    return SOAP_OK;
}

/** Web service operation '__tds__RestoreSystem' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__RestoreSystem(struct soap* soap, struct _tds__RestoreSystem *tds__RestoreSystem, struct _tds__RestoreSystemResponse *tds__RestoreSystemResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetSystemBackup' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemBackup(struct soap* soap, struct _tds__GetSystemBackup *tds__GetSystemBackup, struct _tds__GetSystemBackupResponse *tds__GetSystemBackupResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetSystemLog' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemLog(struct soap* soap, struct _tds__GetSystemLog *tds__GetSystemLog, struct _tds__GetSystemLogResponse *tds__GetSystemLogResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetSystemSupportInformation' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemSupportInformation(struct soap* soap, struct _tds__GetSystemSupportInformation *tds__GetSystemSupportInformation, struct _tds__GetSystemSupportInformationResponse *tds__GetSystemSupportInformationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetScopes' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetScopes(struct soap* soap, struct _tds__GetScopes *tds__GetScopes, struct _tds__GetScopesResponse *tds__GetScopesResponse)
{
    printf("===========__tds__GetScopes \n");
    tds__GetScopesResponse->__sizeScopes = 7;
    struct tt__Scope *Scopes = (struct tt__Scope *)soap_zalloc(soap, sizeof(struct tt__Scope)*tds__GetScopesResponse->__sizeScopes);
    Scopes->ScopeItem = (char *)soap_zalloc(soap, INFO_STRING_LEN);

    Scopes->ScopeDef = tt__ScopeDefinition__Fixed;
    Scopes->ScopeItem = "onvif://www.onvif.org/type/NetworkVideoTransmitter";

    (Scopes+1)->ScopeDef = tt__ScopeDefinition__Fixed;
    (Scopes+1)->ScopeItem = "onvif://www.onvif.org/type/audio_encoder";

    (Scopes+2)->ScopeDef = tt__ScopeDefinition__Fixed;
    (Scopes+2)->ScopeItem = "onvif://www.onvif.org/type/video";

    (Scopes+3)->ScopeDef = tt__ScopeDefinition__Fixed;
    (Scopes+3)->ScopeItem = "onvif://www.onvif.org/name/Ingenic-IPC";

    (Scopes+4)->ScopeDef = tt__ScopeDefinition__Configurable;
    (Scopes+4)->ScopeItem = "onvif://www.onvif.org/location";

    (Scopes+5)->ScopeDef = tt__ScopeDefinition__Fixed;
    (Scopes+5)->ScopeItem = "onvif://www.onvif.org/hardware";

    (Scopes+6)->ScopeDef = tt__ScopeDefinition__Fixed;
    (Scopes+6)->ScopeItem = "onvif://www.onvif.org/hardware/JZ-IPCAM";

    tds__GetScopesResponse->Scopes = Scopes;

    return SOAP_OK;
}

/** Web service operation '__tds__SetScopes' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetScopes(struct soap* soap, struct _tds__SetScopes *tds__SetScopes, struct _tds__SetScopesResponse *tds__SetScopesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__AddScopes' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__AddScopes(struct soap* soap, struct _tds__AddScopes *tds__AddScopes, struct _tds__AddScopesResponse *tds__AddScopesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__RemoveScopes' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveScopes(struct soap* soap, struct _tds__RemoveScopes *tds__RemoveScopes, struct _tds__RemoveScopesResponse *tds__RemoveScopesResponse)
{LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetDiscoveryMode' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDiscoveryMode(struct soap* soap, struct _tds__GetDiscoveryMode *tds__GetDiscoveryMode, struct _tds__GetDiscoveryModeResponse *tds__GetDiscoveryModeResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetDiscoveryMode' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDiscoveryMode(struct soap* soap, struct _tds__SetDiscoveryMode *tds__SetDiscoveryMode, struct _tds__SetDiscoveryModeResponse *tds__SetDiscoveryModeResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetRemoteDiscoveryMode' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteDiscoveryMode(struct soap* soap, struct _tds__GetRemoteDiscoveryMode *tds__GetRemoteDiscoveryMode, struct _tds__GetRemoteDiscoveryModeResponse *tds__GetRemoteDiscoveryModeResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetRemoteDiscoveryMode' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteDiscoveryMode(struct soap* soap, struct _tds__SetRemoteDiscoveryMode *tds__SetRemoteDiscoveryMode, struct _tds__SetRemoteDiscoveryModeResponse *tds__SetRemoteDiscoveryModeResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetDPAddresses' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDPAddresses(struct soap* soap, struct _tds__GetDPAddresses *tds__GetDPAddresses, struct _tds__GetDPAddressesResponse *tds__GetDPAddressesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetEndpointReference' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetEndpointReference(struct soap* soap, struct _tds__GetEndpointReference *tds__GetEndpointReference, struct _tds__GetEndpointReferenceResponse *tds__GetEndpointReferenceResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetRemoteUser' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteUser(struct soap* soap, struct _tds__GetRemoteUser *tds__GetRemoteUser, struct _tds__GetRemoteUserResponse *tds__GetRemoteUserResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetRemoteUser' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteUser(struct soap* soap, struct _tds__SetRemoteUser *tds__SetRemoteUser, struct _tds__SetRemoteUserResponse *tds__SetRemoteUserResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetUsers' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetUsers(struct soap* soap, struct _tds__GetUsers *tds__GetUsers, struct _tds__GetUsersResponse *tds__GetUsersResponse)
{
    tds__GetUsersResponse->__sizeUser = 1;
    tds__GetUsersResponse->User = (struct tt__User *)soap_zalloc(soap, sizeof(struct tt__User));

    tds__GetUsersResponse->User->Username = "admin";
    tds__GetUsersResponse->User->Password = "admin";
    tds__GetUsersResponse->User->UserLevel = tt__UserLevel__User;

    return SOAP_OK;
}

/** Web service operation '__tds__CreateUsers' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateUsers(struct soap* soap, struct _tds__CreateUsers *tds__CreateUsers, struct _tds__CreateUsersResponse *tds__CreateUsersResponse)
{
    if (tds__CreateUsers != NULL) {
        LOG_DBG(LOG_TAG, "Username: %s\n", tds__CreateUsers->User->Username ? tds__CreateUsers->User->Username : "");
        LOG_DBG(LOG_TAG, "Password: %s\n", tds__CreateUsers->User->Password ? tds__CreateUsers->User->Password : "");
        LOG_DBG(LOG_TAG, "UserLevel: %d\n", tds__CreateUsers->User->UserLevel);
    }

    return SOAP_OK;
}

/** Web service operation '__tds__DeleteUsers' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteUsers(struct soap* soap, struct _tds__DeleteUsers *tds__DeleteUsers, struct _tds__DeleteUsersResponse *tds__DeleteUsersResponse)
{
    int i = 0;
    if (tds__DeleteUsers != NULL) {
        for(i = 0; i < tds__DeleteUsers->__sizeUsername; i++) {
            LOG_DBG(LOG_TAG, "Username : %s\n", tds__DeleteUsers->Username[i]);
        }
    }

    return SOAP_OK;
}

/** Web service operation '__tds__SetUser' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetUser(struct soap* soap, struct _tds__SetUser *tds__SetUser, struct _tds__SetUserResponse *tds__SetUserResponse)
{
    int i = 0;
    if (tds__SetUser != NULL) {
        for(i = 0;i < tds__SetUser->__sizeUser; i++) {
            LOG_DBG(LOG_TAG, "Username: %s\n", (tds__SetUser->User + i)->Username);
            LOG_DBG(LOG_TAG, "Password: %s\n", (tds__SetUser->User + i)->Password);
            LOG_DBG(LOG_TAG, "UserLevel: %d\n", (tds__SetUser->User + i)->UserLevel);
        }
    }

    return SOAP_OK;
}

/** Web service operation '__tds__GetWsdlUrl' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetWsdlUrl(struct soap* soap, struct _tds__GetWsdlUrl *tds__GetWsdlUrl, struct _tds__GetWsdlUrlResponse *tds__GetWsdlUrlResponse)
{
    int ret = 0;
    char ip[20];
    /* get host ip */
    ret = device_ip_get((char *)"eth0", ip); //eth0
    if (ret < 0) {
		LOG_DBG(LOG_TAG, "ERROR: can't get host ip address!\n");
		return SOAP_ERR;
    }

    tds__GetWsdlUrlResponse->WsdlUrl = (char *)soap_zalloc(soap, sizeof(char)*128);
    sprintf(tds__GetWsdlUrlResponse->WsdlUrl, "http://%s/wsdl", ip);

    return SOAP_OK;
}

/** Web service operation '__tds__GetCapabilities' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCapabilities(struct soap* soap, struct _tds__GetCapabilities *tds__GetCapabilities, struct _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse)
{
    int ret = 0;
    char ip[20];
    char url[MAX_XADDR_LEN];

    ret = device_ip_get((char *)"eth0", ip); //eth0
    if (ret < 0) {
		LOG_ERR(LOG_TAG, "ERROR: can't get host ip address!\n");
		return SOAP_ERR;
    }

    /*----------------- Category ------------------*/
    if (tds__GetCapabilities->Category == NULL) {
		tds__GetCapabilities->Category = (enum tt__CapabilityCategory *)soap_zalloc(soap, sizeof(enum tt__CapabilityCategory));
		*tds__GetCapabilities->Category = tt__CapabilityCategory__Media;//5
    }

    /*---------------- Analytics -----------------*/
    struct tt__AnalyticsCapabilities *Analytics = (struct tt__AnalyticsCapabilities *)soap_zalloc(soap, sizeof(struct tt__AnalyticsCapabilities));

    Analytics->XAddr = (char *)soap_zalloc(soap, MAX_XADDR_LEN);
    sprintf(url, "http://%s:%d/onvif/analytics_service", ip, SERVER_LISTEN_PORT);

    strcpy(Analytics->XAddr, url);
    Analytics->RuleSupport = xsd__boolean__true_;
    Analytics->AnalyticsModuleSupport = xsd__boolean__true_;

    /*----------------- Device ----------------*/
    struct tt__DeviceCapabilities *Device = (struct tt__DeviceCapabilities *)soap_zalloc(soap, sizeof(struct tt__DeviceCapabilities));

    Device->XAddr = (char *)soap_zalloc(soap, MAX_XADDR_LEN);
    Device->Network = (struct tt__NetworkCapabilities *)soap_zalloc(soap, sizeof(struct tt__NetworkCapabilities));
    Device->Network->IPFilter = (enum xsd__boolean *)soap_zalloc(soap, sizeof(enum xsd__boolean));
    Device->Network->ZeroConfiguration = (enum xsd__boolean *)soap_zalloc(soap, sizeof(enum xsd__boolean));
    Device->Network->IPVersion6 = (enum xsd__boolean *)soap_zalloc(soap, sizeof(enum xsd__boolean));
    Device->Network->DynDNS = (enum xsd__boolean *)soap_zalloc(soap, sizeof(enum xsd__boolean));
    Device->Network->Extension = (struct tt__NetworkCapabilitiesExtension *)soap_zalloc(soap, sizeof(struct tt__NetworkCapabilitiesExtension));
    Device->Network->Extension->Dot11Configuration = (enum xsd__boolean *)soap_zalloc(soap, sizeof(enum xsd__boolean));
    sprintf(url, "http://%s:%d/onvif/device_service",ip ,SERVER_LISTEN_PORT);

    strcpy(Device->XAddr, url);
    *Device->Network->IPFilter = xsd__boolean__false_;//true
    *Device->Network->ZeroConfiguration = xsd__boolean__false_;
    *Device->Network->IPVersion6 = xsd__boolean__false_;
    *Device->Network->DynDNS = xsd__boolean__false_;//true
    *Device->Network->Extension->Dot11Configuration = xsd__boolean__false_;

    struct tt__SystemCapabilities *System =
	(struct tt__SystemCapabilities *)soap_zalloc(soap, sizeof(struct tt__SystemCapabilities));
    System->DiscoveryResolve = xsd__boolean__false_;
    System->DiscoveryBye = xsd__boolean__false_;//true
    System->RemoteDiscovery = xsd__boolean__true_;//false
    System->SystemBackup = xsd__boolean__false_;
    System->SystemLogging = xsd__boolean__false_;
    System->FirmwareUpgrade = xsd__boolean__false_;//true

    struct tt__OnvifVersion *SupportedVersions =
	(struct tt__OnvifVersion *)soap_zalloc(soap, sizeof(struct tt__OnvifVersion));
    SupportedVersions->Major = 2;
    SupportedVersions->Minor = 40;
    System->__sizeSupportedVersions = 1;
    System->SupportedVersions = SupportedVersions;

    Device->System = System;

   /* struct tt__IOCapabilities *IO =
	(struct tt__IOCapabilities *)soap_zalloc(soap, sizeof(struct tt__IOCapabilities));
	IO->InputConnectors =
	(int *)soap_zalloc(soap, sizeof(int));
    *IO->InputConnectors = 2;//1

    IO->RelayOutputs = (int *)soap_zalloc(soap, sizeof(int));
    *IO->RelayOutputs = 2;//1

    Device->IO = IO;
*/
    struct tt__SecurityCapabilities *Security =
	(struct tt__SecurityCapabilities *)soap_zalloc(soap, sizeof(struct tt__SecurityCapabilities));
    Security->TLS1_x002e1 = xsd__boolean__false_;//true
    Security->TLS1_x002e2 = xsd__boolean__false_;//true
    Security->OnboardKeyGeneration = xsd__boolean__false_;//true
    Security->AccessPolicyConfig = xsd__boolean__false_;
    Security->X_x002e509Token = xsd__boolean__false_;
    Security->SAMLToken = xsd__boolean__false_;
    Security->KerberosToken = xsd__boolean__false_;
    Security->RELToken = xsd__boolean__false_;

    Device->Security = Security;

    struct tt__DeviceCapabilitiesExtension *Extension1 =
	(struct tt__DeviceCapabilitiesExtension *)soap_zalloc(soap, sizeof(struct tt__DeviceCapabilitiesExtension));

    Device->Extension = Extension1;

    /*------------------------- Events ------------------*/
    struct tt__EventCapabilities *Events = (struct tt__EventCapabilities *)soap_zalloc(soap, sizeof(struct tt__EventCapabilities));

    Events->XAddr = (char *)soap_zalloc(soap, MAX_XADDR_LEN);
    sprintf(url, "http://%s:%d/onvif/event_service", ip, SERVER_LISTEN_PORT);

    strcpy(Events->XAddr, url);
    Events->WSSubscriptionPolicySupport = xsd__boolean__true_;
    Events->WSPullPointSupport = xsd__boolean__true_;
    Events->WSPausableSubscriptionManagerInterfaceSupport = xsd__boolean__false_;//true

    /*---------------------- Imaging --------------------*/
    struct tt__ImagingCapabilities *Imaging = (struct tt__ImagingCapabilities *)soap_zalloc(soap, sizeof(struct tt__ImagingCapabilities));

    Imaging->XAddr = (char *)soap_zalloc(soap, MAX_XADDR_LEN);
    sprintf(url, "http://%s:%d/onvif/imaging_service", ip, SERVER_LISTEN_PORT);

    strcpy(Imaging->XAddr, url);

    /*--------------------- Media ----------------------*/
    struct tt__MediaCapabilities *Media = (struct tt__MediaCapabilities *)soap_zalloc(soap, sizeof(struct tt__MediaCapabilities));

    Media->XAddr = (char *)soap_zalloc(soap, MAX_XADDR_LEN);
    Media->StreamingCapabilities = (struct tt__RealTimeStreamingCapabilities *)soap_zalloc(soap, sizeof(struct tt__RealTimeStreamingCapabilities));
    Media->StreamingCapabilities->RTPMulticast = (enum xsd__boolean *)soap_zalloc(soap, sizeof(enum xsd__boolean));
    Media->StreamingCapabilities->RTP_USCORETCP = (enum xsd__boolean *)soap_zalloc(soap, sizeof(enum xsd__boolean));
    Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP = (enum xsd__boolean *)soap_zalloc(soap, sizeof(enum xsd__boolean));

    sprintf(url, "http://%s:%d/onvif/media_service", ip, SERVER_LISTEN_PORT);
    strcpy(Media->XAddr, url);

    *Media->StreamingCapabilities->RTPMulticast = xsd__boolean__true_;
    *Media->StreamingCapabilities->RTP_USCORETCP = xsd__boolean__true_;//true
    *Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP = xsd__boolean__true_;

    /*----------------------- PTZ -----------------------*/
    struct tt__PTZCapabilities *PTZ = (struct tt__PTZCapabilities *)soap_zalloc(soap, sizeof(struct tt__PTZCapabilities));

    PTZ->XAddr = (char *)soap_zalloc(soap, MAX_XADDR_LEN);
    sprintf(PTZ->XAddr, "http://%s:%d/onvif/ptz_service", ip, SERVER_LISTEN_PORT);

    /*------------------- Extension --------------------*/
    struct tt__CapabilitiesExtension *Extension = (struct tt__CapabilitiesExtension *)soap_zalloc(soap, sizeof(struct tt__CapabilitiesExtension));

    Extension->DeviceIO = (struct tt__DeviceIOCapabilities *)soap_zalloc(soap, sizeof(struct tt__DeviceIOCapabilities));
    Extension->DeviceIO->XAddr = (char *)soap_zalloc(soap, MAX_XADDR_LEN);

    sprintf(url, "http://%s:%d/onvif/deviceio_service", ip, SERVER_LISTEN_PORT);
    strcpy(Extension->DeviceIO->XAddr, url);

    Extension->DeviceIO->VideoSources = xsd__boolean__true_;
    Extension->DeviceIO->VideoOutputs = xsd__boolean__false_;
    Extension->DeviceIO->AudioSources = xsd__boolean__false_;
    Extension->DeviceIO->AudioOutputs = xsd__boolean__false_;
    Extension->DeviceIO->RelayOutputs = xsd__boolean__false_;
    Extension->DeviceIO->XAddr = NULL;

    /*-------------------- Capabilities -----------------------*/
    tds__GetCapabilitiesResponse->Capabilities = (struct tt__Capabilities *)soap_zalloc(soap, sizeof(struct tt__Capabilities));
    tds__GetCapabilitiesResponse->Capabilities->Analytics = Analytics; /* Must support */
    tds__GetCapabilitiesResponse->Capabilities->Device = Device;
    tds__GetCapabilitiesResponse->Capabilities->Events = Events;
    tds__GetCapabilitiesResponse->Capabilities->Imaging = Imaging;	/* Must support */
    tds__GetCapabilitiesResponse->Capabilities->Media = Media;	/* Must support */
    tds__GetCapabilitiesResponse->Capabilities->PTZ = PTZ;
    tds__GetCapabilitiesResponse->Capabilities->Extension = Extension;

    return SOAP_OK;
}

/** Web service operation '__tds__SetDPAddresses' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDPAddresses(struct soap* soap, struct _tds__SetDPAddresses *tds__SetDPAddresses, struct _tds__SetDPAddressesResponse *tds__SetDPAddressesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetHostname' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetHostname(struct soap* soap, struct _tds__GetHostname *tds__GetHostname, struct _tds__GetHostnameResponse *tds__GetHostnameResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    struct tt__HostnameInformation *HostnameInformation = (struct tt__HostnameInformation *)soap_zalloc(soap, sizeof(struct tt__HostnameInformation));
    HostnameInformation->Name = (char *)soap_zalloc(soap, INFO_STRING_LEN);

    HostnameInformation->FromDHCP = xsd__boolean__false_;
    strcpy(HostnameInformation->Name, "JZ-IPC");

    tds__GetHostnameResponse->HostnameInformation = HostnameInformation;

    return SOAP_OK;
}

/** Web service operation '__tds__SetHostname' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostname(struct soap* soap, struct _tds__SetHostname *tds__SetHostname, struct _tds__SetHostnameResponse *tds__SetHostnameResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetHostnameFromDHCP' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostnameFromDHCP(struct soap* soap, struct _tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP, struct _tds__SetHostnameFromDHCPResponse *tds__SetHostnameFromDHCPResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetDNS' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDNS(struct soap* soap, struct _tds__GetDNS *tds__GetDNS, struct _tds__GetDNSResponse *tds__GetDNSResponse)
{
    int ret = 0;
    char ip[20];
    /* get host ip */
    ret = device_ip_get((char *)"eth0", ip); //eth0
    if (ret < 0) {
		LOG_ERR(LOG_TAG, "ERROR: can't get host ip address!\n");
		return SOAP_ERR;
    }

    struct tt__DNSInformation *DNSInformation = (struct tt__DNSInformation *)soap_zalloc(soap, sizeof(struct tt__DNSInformation));

    DNSInformation->FromDHCP = xsd__boolean__false_;

    struct tt__IPAddress *DNSManual =
	(struct tt__IPAddress *)soap_zalloc(soap, sizeof(struct tt__IPAddress));
    DNSManual->Type = tt__IPType__IPv4;
    DNSManual->IPv4Address = ip;

    DNSInformation->DNSManual = DNSManual;
    tds__GetDNSResponse->DNSInformation = DNSInformation;

    return SOAP_OK;
}

/** Web service operation '__tds__SetDNS' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDNS(struct soap* soap, struct _tds__SetDNS *tds__SetDNS, struct _tds__SetDNSResponse *tds__SetDNSResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetNTP' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNTP(struct soap* soap, struct _tds__GetNTP *tds__GetNTP, struct _tds__GetNTPResponse *tds__GetNTPResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetNTP' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNTP(struct soap* soap, struct _tds__SetNTP *tds__SetNTP, struct _tds__SetNTPResponse *tds__SetNTPResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetDynamicDNS' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDynamicDNS(struct soap* soap, struct _tds__GetDynamicDNS *tds__GetDynamicDNS, struct _tds__GetDynamicDNSResponse *tds__GetDynamicDNSResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetDynamicDNS' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDynamicDNS(struct soap* soap, struct _tds__SetDynamicDNS *tds__SetDynamicDNS, struct _tds__SetDynamicDNSResponse *tds__SetDynamicDNSResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetNetworkInterfaces' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkInterfaces(struct soap* soap, struct _tds__GetNetworkInterfaces *tds__GetNetworkInterfaces, struct _tds__GetNetworkInterfacesResponse *tds__GetNetworkInterfacesResponse)
{
    LOG_DBG(LOG_TAG, "\n");

    int ret = 0;
    char ip[20];

    /* get host ip */
    ret = device_ip_get((char *)"eth0", ip);
    if (ret < 0) {
		LOG_ERR(LOG_TAG, "ERROR: can't get host ip address!\n");
		return SOAP_ERR;
    }

    ret = device_mac_get((char *)"eth0", HOST_MAC);
    if (ret < 0) {
		LOG_ERR(LOG_TAG, "ERROR: can't get host mac address!\n");
		return SOAP_ERR;
    }
    struct tt__NetworkInterface *NetworkInterfaces = (struct tt__NetworkInterface *)soap_zalloc(soap, sizeof(struct tt__NetworkInterface));

    NetworkInterfaces->token = "1";
    NetworkInterfaces->Enabled = xsd__boolean__true_;

    struct tt__NetworkInterfaceInfo *Info =	(struct tt__NetworkInterfaceInfo *)soap_zalloc(soap, sizeof(struct tt__NetworkInterfaceInfo));
    Info->Name = "Eth0";
    Info->HwAddress = (char *)soap_zalloc(soap, 100);
    sprintf(Info->HwAddress, "%02x-%02x-%02x-%02x-%02x-%02x", HOST_MAC[0], HOST_MAC[1], HOST_MAC[2], HOST_MAC[3], HOST_MAC[4], HOST_MAC[5]);

    Info->MTU = (int *)soap_zalloc(soap, sizeof(int));
    *Info->MTU = 1514;

    struct tt__NetworkInterfaceLink *Link =	(struct tt__NetworkInterfaceLink *)soap_zalloc(soap, sizeof(struct tt__NetworkInterfaceLink));
    struct tt__NetworkInterfaceConnectionSetting *AdminSettings = (struct tt__NetworkInterfaceConnectionSetting *)soap_zalloc(soap, sizeof(struct tt__NetworkInterfaceConnectionSetting));
    struct tt__NetworkInterfaceConnectionSetting *OperSettings = (struct tt__NetworkInterfaceConnectionSetting *)soap_zalloc(soap, sizeof(struct tt__NetworkInterfaceConnectionSetting));

    AdminSettings->AutoNegotiation = xsd__boolean__true_;
    AdminSettings->Speed = 100;
    AdminSettings->Duplex = tt__Duplex__Full;

    OperSettings->AutoNegotiation = xsd__boolean__true_;
    OperSettings->Speed = 100;
    OperSettings->Duplex = tt__Duplex__Full;

    Link->AdminSettings = AdminSettings;
    Link->OperSettings = OperSettings;
    Link->InterfaceType = 6;

    struct tt__IPv4NetworkInterface *IPv4 =	(struct tt__IPv4NetworkInterface *)soap_zalloc(soap, sizeof(struct tt__IPv4NetworkInterface));
    IPv4->Enabled = xsd__boolean__true_;
    struct tt__IPv4Configuration *Config = (struct tt__IPv4Configuration *)soap_zalloc(soap, sizeof(struct tt__IPv4Configuration));
    Config->__sizeManual = 1;
    Config->FromDHCP = (struct tt__PrefixedIPv4Address *)0;
    struct tt__PrefixedIPv4Address *Manual = (struct tt__PrefixedIPv4Address *)soap_zalloc(soap, sizeof(struct tt__PrefixedIPv4Address));

    Manual->Address = soap_zalloc(soap, STR_LEN_IP_ADDRESS);
    device_ip_get("eth0", Manual->Address);

    Manual->PrefixLength = 24;
    Config->Manual = Manual;
    IPv4->Config = Config;
    NetworkInterfaces->Info = Info;
    NetworkInterfaces->Link = Link;
    NetworkInterfaces->IPv4 = IPv4;

    tds__GetNetworkInterfacesResponse->__sizeNetworkInterfaces = 1;
    tds__GetNetworkInterfacesResponse->NetworkInterfaces = NetworkInterfaces;

    return SOAP_OK;
}

char tmp_buf[20];
/** Web service operation '__tds__SetNetworkInterfaces' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkInterfaces(struct soap* soap, struct _tds__SetNetworkInterfaces *tds__SetNetworkInterfaces, struct _tds__SetNetworkInterfacesResponse *tds__SetNetworkInterfacesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
	char subnetaddr[20] = {0};
//	char tmp_buf[20] = {0};
	int netmask_len = 0;
	memset(tmp_buf, 0x00, sizeof(unsigned char) * 20);
	memcpy(tmp_buf, tds__SetNetworkInterfaces->NetworkInterface->IPv4->Manual->Address, strlen(tds__SetNetworkInterfaces->NetworkInterface->IPv4->Manual->Address));

	netmask_len = tds__SetNetworkInterfaces->NetworkInterface->IPv4->Manual->PrefixLength;
	printf("--------- netmask_len = %d\n", netmask_len);

	int count = netmask_len / 8;
	int count_b = netmask_len % 8;
	int j;

	netmask_len = 0;
	if (subnetaddr == NULL)
		return -1;
	if (count == 0) {
		for (j = 0 ; j < count_b ; j++) {
			netmask_len += 1 << (7-j);
		}
		printf("subnet mask: %d.0.0.0\n", netmask_len);
		sprintf(subnetaddr, "%d.0.0.0", netmask_len);

	} else if (count == 1) {
		for (j = 0 ; j < count_b ; j++) {
			netmask_len += 1 << (7-j);
		}
		printf("subnet mask: 255.%d.0.0\n", netmask_len);
		sprintf(subnetaddr, "255.%d.0.0", netmask_len);
	} else if (count == 2) {
		for (j = 0 ; j < count_b ; j++) {
			netmask_len += 1 << (7-j);
		}
		printf("subnet mask: 255.255.%d.0\n", netmask_len);
		sprintf(subnetaddr, "255.255.%d.0", netmask_len);
	} else if (count == 3) {
		for (j = 0 ; j < count_b ; j++) {
			netmask_len += 1 << (7-j);
		}
		printf("subnet mask: 255.255.255.%d\n", netmask_len);
		sprintf(subnetaddr, "255.255.255.%d", netmask_len);
	} else if (count == 4) {
		printf("subnet mask: 255.255.255.255\n");
		sprintf(subnetaddr, "255.255.255.255");
	}
	printf("-------- netmask = %s\n", subnetaddr);

	//这个地方需要添加设置掩码的接口
	(void)device_network_set_ip_mask("eth0",subnetaddr);

	char ipTemp[20] = {0};
	int ret = 0;
	ret = device_ip_get("eth0",ipTemp);
	if ((ret == 0) && (strcmp(ipTemp,tmp_buf) != 0)) {
		signal(SIGALRM, device_ip_set);
		alarm(2);
	}

	tds__SetNetworkInterfacesResponse->RebootNeeded = xsd__boolean__false_;

	return SOAP_OK;
}

/** Web service operation '__tds__GetNetworkProtocols' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkProtocols(struct soap* soap, struct _tds__GetNetworkProtocols *tds__GetNetworkProtocols, struct _tds__GetNetworkProtocolsResponse *tds__GetNetworkProtocolsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetNetworkProtocols' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkProtocols(struct soap* soap, struct _tds__SetNetworkProtocols *tds__SetNetworkProtocols, struct _tds__SetNetworkProtocolsResponse *tds__SetNetworkProtocolsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetNetworkDefaultGateway' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkDefaultGateway(struct soap* soap, struct _tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway, struct _tds__GetNetworkDefaultGatewayResponse *tds__GetNetworkDefaultGatewayResponse)
{
    LOG_DBG(LOG_TAG, "Get network default gateway.\n");
	printf("-------- __tds__GetNetworkDefaultGateway...\n");
	struct tt__NetworkGateway* NetworkGateway = (struct tt__NetworkGateway*)soap_zalloc(soap, sizeof(struct tt__NetworkGateway));
	char **IPv4Address = (char*)soap_zalloc(soap, 20);
	char *tmp = (char *)soap_zalloc(soap, 20);

	//get_gateway(tmp),获取网关
	(void)device_network_get_ip_gateway("eth0", tmp);

	printf("Gateway:%s\n", tmp);
	printf("------ IPv4Address : %s\n", tmp);

	IPv4Address[0] = tmp;
	NetworkGateway->__sizeIPv4Address = 1;
	NetworkGateway->IPv4Address = IPv4Address;
	tds__GetNetworkDefaultGatewayResponse->NetworkGateway = NetworkGateway;

	return SOAP_OK;
}

/** Web service operation '__tds__SetNetworkDefaultGateway' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkDefaultGateway(struct soap* soap, struct _tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway, struct _tds__SetNetworkDefaultGatewayResponse *tds__SetNetworkDefaultGatewayResponse)
{
    LOG_DBG(LOG_TAG, "Set network default gateway.\n");
	printf("-------- __tds__SetNetworkDefaultGateway...\n");

	if ( tds__SetNetworkDefaultGateway == NULL ) {
		printf("------- tds__SetNetworkDefaultGateway == NULL\n");
		return SOAP_ERR;
	}

	printf("__sizeIPv4Address = %d.\n", tds__SetNetworkDefaultGateway->__sizeIPv4Address);
	printf("IPv4Address = %s\n", tds__SetNetworkDefaultGateway->IPv4Address[0]);
	if (tds__SetNetworkDefaultGateway->IPv4Address[0] != NULL)
		device_network_set_ip_gateway(tds__SetNetworkDefaultGateway->IPv4Address[0]);

	return SOAP_OK;
}

/** Web service operation '__tds__GetZeroConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetZeroConfiguration(struct soap* soap, struct _tds__GetZeroConfiguration *tds__GetZeroConfiguration, struct _tds__GetZeroConfigurationResponse *tds__GetZeroConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetZeroConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetZeroConfiguration(struct soap* soap, struct _tds__SetZeroConfiguration *tds__SetZeroConfiguration, struct _tds__SetZeroConfigurationResponse *tds__SetZeroConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetIPAddressFilter' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetIPAddressFilter(struct soap* soap, struct _tds__GetIPAddressFilter *tds__GetIPAddressFilter, struct _tds__GetIPAddressFilterResponse *tds__GetIPAddressFilterResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetIPAddressFilter' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetIPAddressFilter(struct soap* soap, struct _tds__SetIPAddressFilter *tds__SetIPAddressFilter, struct _tds__SetIPAddressFilterResponse *tds__SetIPAddressFilterResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__AddIPAddressFilter' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__AddIPAddressFilter(struct soap* soap, struct _tds__AddIPAddressFilter *tds__AddIPAddressFilter, struct _tds__AddIPAddressFilterResponse *tds__AddIPAddressFilterResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__RemoveIPAddressFilter' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveIPAddressFilter(struct soap* soap, struct _tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter, struct _tds__RemoveIPAddressFilterResponse *tds__RemoveIPAddressFilterResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetAccessPolicy' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetAccessPolicy(struct soap* soap, struct _tds__GetAccessPolicy *tds__GetAccessPolicy, struct _tds__GetAccessPolicyResponse *tds__GetAccessPolicyResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetAccessPolicy' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetAccessPolicy(struct soap* soap, struct _tds__SetAccessPolicy *tds__SetAccessPolicy, struct _tds__SetAccessPolicyResponse *tds__SetAccessPolicyResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__CreateCertificate' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateCertificate(struct soap* soap, struct _tds__CreateCertificate *tds__CreateCertificate, struct _tds__CreateCertificateResponse *tds__CreateCertificateResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetCertificates' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificates(struct soap* soap, struct _tds__GetCertificates *tds__GetCertificates, struct _tds__GetCertificatesResponse *tds__GetCertificatesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetCertificatesStatus' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificatesStatus(struct soap* soap, struct _tds__GetCertificatesStatus *tds__GetCertificatesStatus, struct _tds__GetCertificatesStatusResponse *tds__GetCertificatesStatusResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetCertificatesStatus' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetCertificatesStatus(struct soap* soap, struct _tds__SetCertificatesStatus *tds__SetCertificatesStatus, struct _tds__SetCertificatesStatusResponse *tds__SetCertificatesStatusResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__DeleteCertificates' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteCertificates(struct soap* soap, struct _tds__DeleteCertificates *tds__DeleteCertificates, struct _tds__DeleteCertificatesResponse *tds__DeleteCertificatesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetPkcs10Request' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPkcs10Request(struct soap* soap, struct _tds__GetPkcs10Request *tds__GetPkcs10Request, struct _tds__GetPkcs10RequestResponse *tds__GetPkcs10RequestResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__LoadCertificates' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificates(struct soap* soap, struct _tds__LoadCertificates *tds__LoadCertificates, struct _tds__LoadCertificatesResponse *tds__LoadCertificatesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetClientCertificateMode' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetClientCertificateMode(struct soap* soap, struct _tds__GetClientCertificateMode *tds__GetClientCertificateMode, struct _tds__GetClientCertificateModeResponse *tds__GetClientCertificateModeResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetClientCertificateMode' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetClientCertificateMode(struct soap* soap, struct _tds__SetClientCertificateMode *tds__SetClientCertificateMode, struct _tds__SetClientCertificateModeResponse *tds__SetClientCertificateModeResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetRelayOutputs' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRelayOutputs(struct soap* soap, struct _tds__GetRelayOutputs *tds__GetRelayOutputs, struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetRelayOutputSettings' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputSettings(struct soap* soap, struct _tds__SetRelayOutputSettings *tds__SetRelayOutputSettings, struct _tds__SetRelayOutputSettingsResponse *tds__SetRelayOutputSettingsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetRelayOutputState' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputState(struct soap* soap, struct _tds__SetRelayOutputState *tds__SetRelayOutputState, struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SendAuxiliaryCommand' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SendAuxiliaryCommand(struct soap* soap, struct _tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand, struct _tds__SendAuxiliaryCommandResponse *tds__SendAuxiliaryCommandResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetCACertificates' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCACertificates(struct soap* soap, struct _tds__GetCACertificates *tds__GetCACertificates, struct _tds__GetCACertificatesResponse *tds__GetCACertificatesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__LoadCertificateWithPrivateKey' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificateWithPrivateKey(struct soap* soap, struct _tds__LoadCertificateWithPrivateKey *tds__LoadCertificateWithPrivateKey, struct _tds__LoadCertificateWithPrivateKeyResponse *tds__LoadCertificateWithPrivateKeyResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetCertificateInformation' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificateInformation(struct soap* soap, struct _tds__GetCertificateInformation *tds__GetCertificateInformation, struct _tds__GetCertificateInformationResponse *tds__GetCertificateInformationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__LoadCACertificates' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCACertificates(struct soap* soap, struct _tds__LoadCACertificates *tds__LoadCACertificates, struct _tds__LoadCACertificatesResponse *tds__LoadCACertificatesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__CreateDot1XConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateDot1XConfiguration(struct soap* soap, struct _tds__CreateDot1XConfiguration *tds__CreateDot1XConfiguration, struct _tds__CreateDot1XConfigurationResponse *tds__CreateDot1XConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetDot1XConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDot1XConfiguration(struct soap* soap, struct _tds__SetDot1XConfiguration *tds__SetDot1XConfiguration, struct _tds__SetDot1XConfigurationResponse *tds__SetDot1XConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetDot1XConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfiguration(struct soap* soap, struct _tds__GetDot1XConfiguration *tds__GetDot1XConfiguration, struct _tds__GetDot1XConfigurationResponse *tds__GetDot1XConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetDot1XConfigurations' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfigurations(struct soap* soap, struct _tds__GetDot1XConfigurations *tds__GetDot1XConfigurations, struct _tds__GetDot1XConfigurationsResponse *tds__GetDot1XConfigurationsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__DeleteDot1XConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteDot1XConfiguration(struct soap* soap, struct _tds__DeleteDot1XConfiguration *tds__DeleteDot1XConfiguration, struct _tds__DeleteDot1XConfigurationResponse *tds__DeleteDot1XConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetDot11Capabilities' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Capabilities(struct soap* soap, struct _tds__GetDot11Capabilities *tds__GetDot11Capabilities, struct _tds__GetDot11CapabilitiesResponse *tds__GetDot11CapabilitiesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetDot11Status' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Status(struct soap* soap, struct _tds__GetDot11Status *tds__GetDot11Status, struct _tds__GetDot11StatusResponse *tds__GetDot11StatusResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__ScanAvailableDot11Networks' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__ScanAvailableDot11Networks(struct soap* soap, struct _tds__ScanAvailableDot11Networks *tds__ScanAvailableDot11Networks, struct _tds__ScanAvailableDot11NetworksResponse *tds__ScanAvailableDot11NetworksResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetSystemUris' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemUris(struct soap* soap, struct _tds__GetSystemUris *tds__GetSystemUris, struct _tds__GetSystemUrisResponse *tds__GetSystemUrisResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__StartFirmwareUpgrade' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__StartFirmwareUpgrade(struct soap* soap, struct _tds__StartFirmwareUpgrade *tds__StartFirmwareUpgrade, struct _tds__StartFirmwareUpgradeResponse *tds__StartFirmwareUpgradeResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__StartSystemRestore' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__StartSystemRestore(struct soap* soap, struct _tds__StartSystemRestore *tds__StartSystemRestore, struct _tds__StartSystemRestoreResponse *tds__StartSystemRestoreResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetStorageConfigurations' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetStorageConfigurations(struct soap* soap, struct _tds__GetStorageConfigurations *tds__GetStorageConfigurations, struct _tds__GetStorageConfigurationsResponse *tds__GetStorageConfigurationsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__CreateStorageConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateStorageConfiguration(struct soap* soap, struct _tds__CreateStorageConfiguration *tds__CreateStorageConfiguration, struct _tds__CreateStorageConfigurationResponse *tds__CreateStorageConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetStorageConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetStorageConfiguration(struct soap* soap, struct _tds__GetStorageConfiguration *tds__GetStorageConfiguration, struct _tds__GetStorageConfigurationResponse *tds__GetStorageConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetStorageConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetStorageConfiguration(struct soap* soap, struct _tds__SetStorageConfiguration *tds__SetStorageConfiguration, struct _tds__SetStorageConfigurationResponse *tds__SetStorageConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__DeleteStorageConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteStorageConfiguration(struct soap* soap, struct _tds__DeleteStorageConfiguration *tds__DeleteStorageConfiguration, struct _tds__DeleteStorageConfigurationResponse *tds__DeleteStorageConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetGeoLocation' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetGeoLocation(struct soap* soap, struct _tds__GetGeoLocation *tds__GetGeoLocation, struct _tds__GetGeoLocationResponse *tds__GetGeoLocationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetGeoLocation' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetGeoLocation(struct soap* soap, struct _tds__SetGeoLocation *tds__SetGeoLocation, struct _tds__SetGeoLocationResponse *tds__SetGeoLocationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__DeleteGeoLocation' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteGeoLocation(struct soap* soap, struct _tds__DeleteGeoLocation *tds__DeleteGeoLocation, struct _tds__DeleteGeoLocationResponse *tds__DeleteGeoLocationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetServiceCapabilities' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetServiceCapabilities(struct soap* soap, struct _trt__GetServiceCapabilities *trt__GetServiceCapabilities, struct _trt__GetServiceCapabilitiesResponse *trt__GetServiceCapabilitiesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoSources' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSources(struct soap* soap, struct _trt__GetVideoSources *trt__GetVideoSources, struct _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse)
{
    struct tt__VideoSource *VideoSources = (struct tt__VideoSource *)soap_zalloc(soap, sizeof(struct tt__VideoSource) * 1);
    VideoSources->token = (char *)soap_zalloc(soap, INFO_STRING_LEN);
    VideoSources->Resolution = (struct tt__VideoResolution *)soap_zalloc(soap, sizeof(struct tt__VideoResolution));
    sprintf(VideoSources->token, "%s", "XcamVideoSourceConfigToken");
    VideoSources->Framerate = 25;

    VideoSources->Resolution->Width = 1920;
    VideoSources->Resolution->Height = 1080;
    //VideoSources->Extension = NULL;//my
    /* set value */
    trt__GetVideoSourcesResponse->__sizeVideoSources = 1;
    trt__GetVideoSourcesResponse->VideoSources = VideoSources;

    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioSources' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSources(struct soap* soap, struct _trt__GetAudioSources *trt__GetAudioSources, struct _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioOutputs' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputs(struct soap* soap, struct _trt__GetAudioOutputs *trt__GetAudioOutputs, struct _trt__GetAudioOutputsResponse *trt__GetAudioOutputsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__CreateProfile' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__CreateProfile(struct soap* soap, struct _trt__CreateProfile *trt__CreateProfile, struct _trt__CreateProfileResponse *trt__CreateProfileResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetProfile' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfile(struct soap* soap, struct _trt__GetProfile *trt__GetProfile, struct _trt__GetProfileResponse *trt__GetProfileResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    int ret = 0;
    char ip[20];
    /* get host ip */
    ret = device_ip_get((char *)"eth0", ip); //eth0
    if (ret < 0) {
		LOG_ERR(LOG_TAG, "ERROR: can't get host ip address!\n");
		return SOAP_ERR;
    }

    struct tt__Profile *Profile = (struct tt__Profile *)soap_zalloc(soap, sizeof(struct tt__Profile));
    Profile->Name = (char *)soap_zalloc(soap, 100);
    strcpy(Profile->Name, "MainStream");

    Profile->token = (char *)soap_zalloc(soap, 100);
    strcpy(Profile->token, "MainStreamToken");

    Profile->fixed = (enum xsd__boolean *)soap_zalloc(soap, sizeof(enum xsd__boolean));
    *Profile->fixed = xsd__boolean__true_;

    Profile->VideoSourceConfiguration = (struct tt__VideoSourceConfiguration *)soap_zalloc(soap, sizeof(struct tt__VideoSourceConfiguration));
    Profile->VideoSourceConfiguration->Name = (char *)soap_zalloc(soap, 100);
    Profile->VideoSourceConfiguration->token = (char *)soap_zalloc(soap, 100);
    Profile->VideoSourceConfiguration->SourceToken = (char *)soap_zalloc(soap, 100);
    Profile->VideoSourceConfiguration->Bounds = (struct tt__IntRectangle *)soap_zalloc(soap, sizeof(struct tt__IntRectangle));

    sprintf(Profile->VideoSourceConfiguration->Name, "%s", "XcamVideoSourceConfig");
    sprintf(Profile->VideoSourceConfiguration->token, "%s", "XcamVideoSourceConfigToken");
    sprintf(Profile->VideoSourceConfiguration->SourceToken, "%s", "XcamVideoSourceConfigToken");
    Profile->VideoSourceConfiguration->UseCount = 3;

    Profile->VideoSourceConfiguration->Bounds->x = 0;
    Profile->VideoSourceConfiguration->Bounds->y = 0;
    Profile->VideoSourceConfiguration->Bounds->width = 1920;
    Profile->VideoSourceConfiguration->Bounds->height = 1080;

    Profile->VideoEncoderConfiguration = (struct tt__VideoEncoderConfiguration *)soap_zalloc(soap, sizeof(struct tt__VideoEncoderConfiguration));

    Profile->VideoEncoderConfiguration->Name = (char *)soap_zalloc(soap, 100);
    sprintf(Profile->VideoEncoderConfiguration->Name, "%s", "MainVideoEncoderConfig");
    Profile->VideoEncoderConfiguration->UseCount = 1;
    Profile->VideoEncoderConfiguration->token = (char *)soap_zalloc(soap, 100);
    sprintf(Profile->VideoEncoderConfiguration->token, "%s", "MainVideoEncoderConfigToken");
    Profile->VideoEncoderConfiguration->Encoding = tt__VideoEncoding__H264;
    Profile->VideoEncoderConfiguration->Resolution = (struct tt__VideoResolution *)soap_zalloc(soap, sizeof(struct tt__VideoResolution));
    Profile->VideoEncoderConfiguration->Resolution->Width = 1920;
    Profile->VideoEncoderConfiguration->Resolution->Height = 1080;
    Profile->VideoEncoderConfiguration->Quality = 40;//5

    Profile->VideoEncoderConfiguration->Multicast = (struct tt__MulticastConfiguration *)soap_zalloc(soap, sizeof(struct tt__MulticastConfiguration));
    Profile->VideoEncoderConfiguration->Multicast->Address = (struct tt__IPAddress *)soap_zalloc(soap, sizeof(struct tt__IPAddress));
    Profile->VideoEncoderConfiguration->Multicast->Address->Type = tt__IPType__IPv4;
    Profile->VideoEncoderConfiguration->Multicast->Address->IPv4Address = (char *)soap_zalloc(soap, 100);
    sprintf(Profile->VideoEncoderConfiguration->Multicast->Address->IPv4Address, "%s", ip);
    Profile->VideoEncoderConfiguration->Multicast->Address->IPv6Address = NULL;
    Profile->VideoEncoderConfiguration->Multicast->Port = 18888;
    Profile->VideoEncoderConfiguration->Multicast->TTL = 255;
    Profile->VideoEncoderConfiguration->Multicast->AutoStart = xsd__boolean__false_;

    Profile->VideoEncoderConfiguration->SessionTimeout = "PT300S";

    Profile->VideoEncoderConfiguration->H264 = (struct tt__H264Configuration *)soap_zalloc(soap, sizeof(struct tt__H264Configuration));
    Profile->VideoEncoderConfiguration->H264->GovLength = 50;//30
    Profile->VideoEncoderConfiguration->H264->H264Profile = tt__H264Profile__High; //tt__H264Profile__Main;

    Profile->VideoEncoderConfiguration->RateControl = (struct tt__VideoRateControl *)soap_zalloc(soap, sizeof(struct tt__VideoRateControl));
    Profile->VideoEncoderConfiguration->RateControl->FrameRateLimit = 25;
    Profile->VideoEncoderConfiguration->RateControl->EncodingInterval = 1;
    Profile->VideoEncoderConfiguration->RateControl->BitrateLimit = 2048;
    Profile->VideoEncoderConfiguration->RateControl = NULL;
    /* set trt__GetProfileResponse */
    trt__GetProfileResponse->Profile = Profile;

    return SOAP_OK;
}

/** Web service operation '__trt__GetProfiles' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfiles(struct soap* soap, struct _trt__GetProfiles *trt__GetProfiles, struct _trt__GetProfilesResponse *trt__GetProfilesResponse)
{
    int ret = 0;
    char ip[20];
    /* get host ip */
    ret = device_ip_get((char *)"eth0", ip);
    if (ret < 0) {
		LOG_ERR(LOG_TAG, "ERROR: can't get host ip address!\n");
		return SOAP_ERR;
    }

    trt__GetProfilesResponse->__sizeProfiles = 2;
    /* Profile */
    struct tt__Profile *Profiles = (struct tt__Profile *)soap_zalloc(soap, sizeof(struct tt__Profile) * trt__GetProfilesResponse->__sizeProfiles);

    /* Main stream config */
    Profiles[0].Name = (char *)soap_zalloc(soap, 100);
    strcpy(Profiles[0].Name, "MainStream");

    Profiles[0].token = (char *)soap_zalloc(soap, 100);
    strcpy(Profiles[0].token, "MainStreamToken");

    Profiles[0].fixed = (enum xsd__boolean *)soap_zalloc(soap, sizeof(enum xsd__boolean));
    *Profiles[0].fixed = xsd__boolean__true_;

    Profiles[0].VideoSourceConfiguration = (struct tt__VideoSourceConfiguration *)soap_zalloc(soap, sizeof(struct tt__VideoSourceConfiguration));
    Profiles[0].VideoSourceConfiguration->Name = (char *)soap_zalloc(soap, 100);
	Profiles[0].VideoSourceConfiguration->token = (char *)soap_zalloc(soap, 100);
    Profiles[0].VideoSourceConfiguration->SourceToken = (char *)soap_zalloc(soap, 100);
    Profiles[0].VideoSourceConfiguration->Bounds = (struct tt__IntRectangle *)soap_zalloc(soap, sizeof(struct tt__IntRectangle));

    sprintf(Profiles[0].VideoSourceConfiguration->Name, "%s", "XcamVideoSourceConfig");
    sprintf(Profiles[0].VideoSourceConfiguration->token, "%s", "XcamVideoSourceConfigToken");
    sprintf(Profiles[0].VideoSourceConfiguration->SourceToken, "%s", "XcamVideoSourceConfigToken");
    Profiles[0].VideoSourceConfiguration->UseCount = 2;

	Profiles[0].VideoSourceConfiguration->Bounds->x = 0;
	Profiles[0].VideoSourceConfiguration->Bounds->y = 0;
	Profiles[0].VideoSourceConfiguration->Bounds->width = 1920;
	Profiles[0].VideoSourceConfiguration->Bounds->height = 1080;

	Profiles[0].VideoEncoderConfiguration = (struct tt__VideoEncoderConfiguration *)soap_zalloc(soap, sizeof(struct tt__VideoEncoderConfiguration));

	Profiles[0].VideoEncoderConfiguration->Name = (char *)soap_zalloc(soap, 100);
	sprintf(Profiles[0].VideoEncoderConfiguration->Name, "%s", "MainVideoEncoderConfig");
	Profiles[0].VideoEncoderConfiguration->UseCount = 1;
	Profiles[0].VideoEncoderConfiguration->token = (char *)soap_zalloc(soap, 100);
	sprintf(Profiles[0].VideoEncoderConfiguration->token, "%s", "MainVideoEncoderConfigToken");
    Profiles[0].VideoEncoderConfiguration->Encoding = tt__VideoEncoding__H264;
    Profiles[0].VideoEncoderConfiguration->Resolution = (struct tt__VideoResolution *)soap_zalloc(soap, sizeof(struct tt__VideoResolution));
    Profiles[0].VideoEncoderConfiguration->Resolution->Width = 1920;
    Profiles[0].VideoEncoderConfiguration->Resolution->Height = 1080;
    Profiles[0].VideoEncoderConfiguration->Quality = 40;//5

    Profiles[0].VideoEncoderConfiguration->Multicast = (struct tt__MulticastConfiguration *)soap_zalloc(soap, sizeof(struct tt__MulticastConfiguration));
    Profiles[0].VideoEncoderConfiguration->Multicast->Address = (struct tt__IPAddress *)soap_zalloc(soap, sizeof(struct tt__IPAddress));
    Profiles[0].VideoEncoderConfiguration->Multicast->Address->Type = tt__IPType__IPv4;
    Profiles[0].VideoEncoderConfiguration->Multicast->Address->IPv4Address = (char *)soap_zalloc(soap, 100);
    sprintf(Profiles[0].VideoEncoderConfiguration->Multicast->Address->IPv4Address, "%s", ip);
    Profiles[0].VideoEncoderConfiguration->Multicast->Address->IPv6Address = NULL;
    Profiles[0].VideoEncoderConfiguration->Multicast->Port = 18888;
    Profiles[0].VideoEncoderConfiguration->Multicast->TTL = 255;
    Profiles[0].VideoEncoderConfiguration->Multicast->AutoStart = xsd__boolean__false_;

    Profiles[0].VideoEncoderConfiguration->SessionTimeout = "PT300S";

    Profiles[0].VideoEncoderConfiguration->H264 = (struct tt__H264Configuration *)soap_zalloc(soap, sizeof(struct tt__H264Configuration));
    Profiles[0].VideoEncoderConfiguration->H264->GovLength = 50;//30
    Profiles[0].VideoEncoderConfiguration->H264->H264Profile = tt__H264Profile__High; //tt__H264Profile__Main;

    Profiles[0].VideoEncoderConfiguration->RateControl = (struct tt__VideoRateControl *)soap_zalloc(soap, sizeof(struct tt__VideoRateControl));
    Profiles[0].VideoEncoderConfiguration->RateControl->FrameRateLimit = 25;
    Profiles[0].VideoEncoderConfiguration->RateControl->EncodingInterval = 1;
    Profiles[0].VideoEncoderConfiguration->RateControl->BitrateLimit = 2048;
    Profiles[0].VideoEncoderConfiguration->RateControl = NULL;


    /* Second stream config */
    Profiles[1].Name = (char *)soap_zalloc(soap, 100);
    strcpy(Profiles[1].Name, "SubStream");

    Profiles[1].token = (char *)soap_zalloc(soap, 100);
    strcpy(Profiles[1].token, "SubStreamToken");

    Profiles[1].fixed = (enum xsd__boolean *)soap_zalloc(soap, sizeof(enum xsd__boolean));
    *Profiles[1].fixed = xsd__boolean__true_;

    Profiles[1].VideoSourceConfiguration = (struct tt__VideoSourceConfiguration *)soap_zalloc(soap, sizeof(struct tt__VideoSourceConfiguration));
    Profiles[1].VideoSourceConfiguration->Name = (char *)soap_zalloc(soap, 100);
    Profiles[1].VideoSourceConfiguration->token = (char *)soap_zalloc(soap, 100);
    Profiles[1].VideoSourceConfiguration->SourceToken = (char *)soap_zalloc(soap, 100);
    Profiles[1].VideoSourceConfiguration->Bounds = (struct tt__IntRectangle *)soap_zalloc(soap, sizeof(struct tt__IntRectangle));

    sprintf(Profiles[1].VideoSourceConfiguration->Name, "%s", "XcamVideoSourceConfig");
    sprintf(Profiles[1].VideoSourceConfiguration->token, "%s", "XcamVideoSourceConfigToken");
    sprintf(Profiles[1].VideoSourceConfiguration->SourceToken, "%s", "XcamVideoSourceConfigToken");
    Profiles[1].VideoSourceConfiguration->UseCount = 2;

    Profiles[1].VideoSourceConfiguration->Bounds->x = 0;
    Profiles[1].VideoSourceConfiguration->Bounds->y = 0;
    Profiles[1].VideoSourceConfiguration->Bounds->width = 640;
    Profiles[1].VideoSourceConfiguration->Bounds->height = 360;

    Profiles[1].VideoEncoderConfiguration = (struct tt__VideoEncoderConfiguration *)soap_zalloc(soap, sizeof(struct tt__VideoEncoderConfiguration));

    Profiles[1].VideoEncoderConfiguration->Name = (char *)soap_zalloc(soap, 100);
    sprintf(Profiles[1].VideoEncoderConfiguration->Name, "%s", "SubVideoEncoderConfig");
    Profiles[1].VideoEncoderConfiguration->UseCount = 1;
    Profiles[1].VideoEncoderConfiguration->token = (char *)soap_zalloc(soap, 100);
    sprintf(Profiles[1].VideoEncoderConfiguration->token, "%s", "SubVideoEncoderConfigToken");
    Profiles[1].VideoEncoderConfiguration->Encoding = tt__VideoEncoding__H264;
    Profiles[1].VideoEncoderConfiguration->Resolution = (struct tt__VideoResolution *)soap_zalloc(soap, sizeof(struct tt__VideoResolution));
    Profiles[1].VideoEncoderConfiguration->Resolution->Width = 640;
    Profiles[1].VideoEncoderConfiguration->Resolution->Height = 360;
    Profiles[1].VideoEncoderConfiguration->Quality = 40;//5
    Profiles[1].VideoEncoderConfiguration->Multicast = (struct tt__MulticastConfiguration *)soap_zalloc(soap, sizeof(struct tt__MulticastConfiguration));
    Profiles[1].VideoEncoderConfiguration->Multicast->Address = (struct tt__IPAddress *)soap_zalloc(soap, sizeof(struct tt__IPAddress));
    Profiles[1].VideoEncoderConfiguration->Multicast->Address->Type = tt__IPType__IPv4;
    Profiles[1].VideoEncoderConfiguration->Multicast->Address->IPv4Address = (char *)soap_zalloc(soap, 100);
    sprintf(Profiles[1].VideoEncoderConfiguration->Multicast->Address->IPv4Address, "%s", ip);
    Profiles[1].VideoEncoderConfiguration->Multicast->Address->IPv6Address = NULL;
    Profiles[1].VideoEncoderConfiguration->Multicast->Port = 18888;
    Profiles[1].VideoEncoderConfiguration->Multicast->TTL = 255;
    Profiles[1].VideoEncoderConfiguration->Multicast->AutoStart = xsd__boolean__false_;

    Profiles[1].VideoEncoderConfiguration->SessionTimeout = "PT300S";

    Profiles[1].VideoEncoderConfiguration->H264 = (struct tt__H264Configuration *)soap_zalloc(soap, sizeof(struct tt__H264Configuration));
    Profiles[1].VideoEncoderConfiguration->H264->GovLength = 50;//30
    Profiles[1].VideoEncoderConfiguration->H264->H264Profile = tt__H264Profile__High; //tt__H264Profile__Main;

    Profiles[1].VideoEncoderConfiguration->RateControl = (struct tt__VideoRateControl *)soap_zalloc(soap, sizeof(struct tt__VideoRateControl));
    Profiles[1].VideoEncoderConfiguration->RateControl->FrameRateLimit = 25;
    Profiles[1].VideoEncoderConfiguration->RateControl->EncodingInterval = 1;
    Profiles[1].VideoEncoderConfiguration->RateControl->BitrateLimit = 2048;
    Profiles[1].VideoEncoderConfiguration->RateControl = NULL;

    /* set trt__GetProfileResponse */
    trt__GetProfilesResponse->Profiles = Profiles;

    return SOAP_OK;
}

/** Web service operation '__trt__AddVideoEncoderConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoEncoderConfiguration(struct soap* soap, struct _trt__AddVideoEncoderConfiguration *trt__AddVideoEncoderConfiguration, struct _trt__AddVideoEncoderConfigurationResponse *trt__AddVideoEncoderConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__AddVideoSourceConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoSourceConfiguration(struct soap* soap, struct _trt__AddVideoSourceConfiguration *trt__AddVideoSourceConfiguration, struct _trt__AddVideoSourceConfigurationResponse *trt__AddVideoSourceConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__AddAudioEncoderConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioEncoderConfiguration(struct soap* soap, struct _trt__AddAudioEncoderConfiguration *trt__AddAudioEncoderConfiguration, struct _trt__AddAudioEncoderConfigurationResponse *trt__AddAudioEncoderConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__AddAudioSourceConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioSourceConfiguration(struct soap* soap, struct _trt__AddAudioSourceConfiguration *trt__AddAudioSourceConfiguration, struct _trt__AddAudioSourceConfigurationResponse *trt__AddAudioSourceConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__AddPTZConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddPTZConfiguration(struct soap* soap, struct _trt__AddPTZConfiguration *trt__AddPTZConfiguration, struct _trt__AddPTZConfigurationResponse *trt__AddPTZConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__AddVideoAnalyticsConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoAnalyticsConfiguration(struct soap* soap, struct _trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration, struct _trt__AddVideoAnalyticsConfigurationResponse *trt__AddVideoAnalyticsConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__AddMetadataConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddMetadataConfiguration(struct soap* soap, struct _trt__AddMetadataConfiguration *trt__AddMetadataConfiguration, struct _trt__AddMetadataConfigurationResponse *trt__AddMetadataConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__AddAudioOutputConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioOutputConfiguration(struct soap* soap, struct _trt__AddAudioOutputConfiguration *trt__AddAudioOutputConfiguration, struct _trt__AddAudioOutputConfigurationResponse *trt__AddAudioOutputConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__AddAudioDecoderConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioDecoderConfiguration(struct soap* soap, struct _trt__AddAudioDecoderConfiguration *trt__AddAudioDecoderConfiguration, struct _trt__AddAudioDecoderConfigurationResponse *trt__AddAudioDecoderConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__RemoveVideoEncoderConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoEncoderConfiguration(struct soap* soap, struct _trt__RemoveVideoEncoderConfiguration *trt__RemoveVideoEncoderConfiguration, struct _trt__RemoveVideoEncoderConfigurationResponse *trt__RemoveVideoEncoderConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__RemoveVideoSourceConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoSourceConfiguration(struct soap* soap, struct _trt__RemoveVideoSourceConfiguration *trt__RemoveVideoSourceConfiguration, struct _trt__RemoveVideoSourceConfigurationResponse *trt__RemoveVideoSourceConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__RemoveAudioEncoderConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioEncoderConfiguration(struct soap* soap, struct _trt__RemoveAudioEncoderConfiguration *trt__RemoveAudioEncoderConfiguration, struct _trt__RemoveAudioEncoderConfigurationResponse *trt__RemoveAudioEncoderConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__RemoveAudioSourceConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioSourceConfiguration(struct soap* soap, struct _trt__RemoveAudioSourceConfiguration *trt__RemoveAudioSourceConfiguration, struct _trt__RemoveAudioSourceConfigurationResponse *trt__RemoveAudioSourceConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__RemovePTZConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemovePTZConfiguration(struct soap* soap, struct _trt__RemovePTZConfiguration *trt__RemovePTZConfiguration, struct _trt__RemovePTZConfigurationResponse *trt__RemovePTZConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__RemoveVideoAnalyticsConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoAnalyticsConfiguration(struct soap* soap, struct _trt__RemoveVideoAnalyticsConfiguration *trt__RemoveVideoAnalyticsConfiguration, struct _trt__RemoveVideoAnalyticsConfigurationResponse *trt__RemoveVideoAnalyticsConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__RemoveMetadataConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveMetadataConfiguration(struct soap* soap, struct _trt__RemoveMetadataConfiguration *trt__RemoveMetadataConfiguration, struct _trt__RemoveMetadataConfigurationResponse *trt__RemoveMetadataConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__RemoveAudioOutputConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioOutputConfiguration(struct soap* soap, struct _trt__RemoveAudioOutputConfiguration *trt__RemoveAudioOutputConfiguration, struct _trt__RemoveAudioOutputConfigurationResponse *trt__RemoveAudioOutputConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__RemoveAudioDecoderConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioDecoderConfiguration(struct soap* soap, struct _trt__RemoveAudioDecoderConfiguration *trt__RemoveAudioDecoderConfiguration, struct _trt__RemoveAudioDecoderConfigurationResponse *trt__RemoveAudioDecoderConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__DeleteProfile' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__DeleteProfile(struct soap* soap, struct _trt__DeleteProfile *trt__DeleteProfile, struct _trt__DeleteProfileResponse *trt__DeleteProfileResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoSourceConfigurations' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfigurations(struct soap* soap, struct _trt__GetVideoSourceConfigurations *trt__GetVideoSourceConfigurations, struct _trt__GetVideoSourceConfigurationsResponse *trt__GetVideoSourceConfigurationsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    trt__GetVideoSourceConfigurationsResponse->__sizeConfigurations = 2;
    struct tt__VideoSourceConfiguration *Configurations = (struct tt__VideoSourceConfiguration *)soap_zalloc(soap, sizeof(struct tt__VideoSourceConfiguration) * trt__GetVideoSourceConfigurationsResponse->__sizeConfigurations);

    /* Main stream config */
	Configurations[0].Name = (char *)soap_zalloc(soap, 100);
	Configurations[0].token = (char *)soap_zalloc(soap, 100);
	Configurations[0].SourceToken = (char *)soap_zalloc(soap, 100);
	Configurations[0].Bounds = (struct tt__IntRectangle *)soap_zalloc(soap, sizeof(struct tt__IntRectangle));

	sprintf(Configurations[0].Name, "%s", "XcamVideoSourceConfig");
	sprintf(Configurations[0].token, "%s", "XcamVideoSourceConfigToken");
	sprintf(Configurations[0].SourceToken, "%s", "XcamVideoSourceConfigToken");
	Configurations[0].UseCount = 3;

	Configurations[0].Bounds->x = 0;
	Configurations[0].Bounds->y = 0;
	Configurations[0].Bounds->width = 1920;
	Configurations[0].Bounds->height = 1080;
   //Configurations[0].Extension = NULL;//my

    /* Second stream config */
	Configurations[1].Name = (char *)soap_zalloc(soap, 100);
	Configurations[1].token = (char *)soap_zalloc(soap, 100);
	Configurations[1].SourceToken = (char *)soap_zalloc(soap, 100);
	Configurations[1].Bounds = (struct tt__IntRectangle *)soap_zalloc(soap, sizeof(struct tt__IntRectangle));

	sprintf(Configurations[1].Name, "%s", "XcamVideoSourceConfig");
	sprintf(Configurations[1].token, "%s", "XcamVideoSourceConfigToken");
	sprintf(Configurations[1].SourceToken, "%s", "XcamVideoSourceConfigToken");
	Configurations[1].UseCount = 3;

	Configurations[1].Bounds->x = 0;
	Configurations[1].Bounds->y = 0;
	Configurations[1].Bounds->width = 640;
	Configurations[1].Bounds->height = 360;
    //Configurations[0].Extension = NULL;//my

    trt__GetVideoSourceConfigurationsResponse->Configurations = Configurations;

    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoEncoderConfigurations' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurations(struct soap* soap, struct _trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations, struct _trt__GetVideoEncoderConfigurationsResponse *trt__GetVideoEncoderConfigurationsResponse)
{
    int ret = 0;
    char ip[20];
    /* get host ip */
    ret = device_ip_get((char *)"eth0", ip); //eth0
    if (ret < 0) {
	LOG_ERR(LOG_TAG, "ERROR: can't get host ip address!\n");
	return SOAP_ERR;
    }

    trt__GetVideoEncoderConfigurationsResponse->__sizeConfigurations = 2;
    struct tt__VideoEncoderConfiguration *Configurations = (struct tt__VideoEncoderConfiguration *)soap_zalloc(soap, sizeof(struct tt__VideoEncoderConfiguration) * trt__GetVideoEncoderConfigurationsResponse->__sizeConfigurations);

    /* Main stream config */
	Configurations[0].Name = (char *)soap_zalloc(soap, 100);
	Configurations[0].token = (char *)soap_zalloc(soap, 100);
	Configurations[0].Resolution = (struct tt__VideoResolution *)soap_zalloc(soap, sizeof(struct tt__VideoResolution));
	Configurations[0].RateControl = (struct tt__VideoRateControl *)soap_zalloc(soap, sizeof(struct tt__VideoRateControl));
	Configurations[0].H264 = (struct tt__H264Configuration *)soap_zalloc(soap, sizeof(struct tt__H264Configuration));
	Configurations[0].Multicast = (struct tt__MulticastConfiguration *)soap_zalloc(soap, sizeof(struct tt__MulticastConfiguration));
	Configurations[0].Multicast->Address = (struct tt__IPAddress *)soap_zalloc(soap, sizeof(struct tt__IPAddress));
	Configurations[0].Multicast->Address->IPv4Address = (char *)soap_zalloc(soap, 100);
	Configurations[0].__any = (char *)soap_zalloc(soap, 100);

	sprintf(Configurations[0].Name, "%s", "MainVideoEncoderConfig");
	sprintf(Configurations[0].token, "%s", "MainVideoEncoderConfigToken");
	Configurations[0].Encoding = tt__VideoEncoding__H264;
	Configurations[0].UseCount = 2;
	Configurations[0].Quality = 30;//5
	Configurations[0].SessionTimeout = "10000";
	Configurations[0].Resolution->Width = 1920;
	Configurations[0].Resolution->Height = 1080;

    Configurations[0].RateControl->FrameRateLimit = 25;
	Configurations[0].RateControl->EncodingInterval = 1;
	Configurations[0].RateControl->BitrateLimit = 2048;

	Configurations[0].H264->GovLength = 50;//30
	Configurations[0].H264->H264Profile = tt__H264Profile__High; //tt__H264Profile__Main;

    Configurations[0].Multicast->Port = 18888;
	Configurations[0].Multicast->TTL = 255;
	Configurations[0].Multicast->AutoStart = xsd__boolean__false_;

	Configurations[0].Multicast->Address->Type = tt__IPType__IPv4;
	sprintf(Configurations[0].Multicast->Address->IPv4Address, "%s", ip);
	Configurations[0].Multicast->Address->IPv6Address = NULL;

    /* Second stream config */
	Configurations[1].Name = (char *)soap_zalloc(soap, 100);
	Configurations[1].token = (char *)soap_zalloc(soap, 100);
	Configurations[1].Resolution = (struct tt__VideoResolution *)soap_zalloc(soap, sizeof(struct tt__VideoResolution));
	Configurations[1].RateControl = (struct tt__VideoRateControl *)soap_zalloc(soap, sizeof(struct tt__VideoRateControl));
	Configurations[1].H264 = (struct tt__H264Configuration *)soap_zalloc(soap, sizeof(struct tt__H264Configuration));
	Configurations[1].Multicast = (struct tt__MulticastConfiguration *)soap_zalloc(soap, sizeof(struct tt__MulticastConfiguration));
	Configurations[1].Multicast->Address = (struct tt__IPAddress *)soap_zalloc(soap, sizeof(struct tt__IPAddress));
	Configurations[1].Multicast->Address->IPv4Address = (char *)soap_zalloc(soap, 100);
	Configurations[1].__any = (char *)soap_zalloc(soap, 100);

	sprintf(Configurations[1].Name, "%s", "SubVideoEncoderConfig");
	sprintf(Configurations[1].token, "%s", "SubVideoEncoderConfigToken");
	Configurations[1].Encoding = tt__VideoEncoding__H264;
	Configurations[1].UseCount = 2;
	Configurations[1].Quality = 30;//5
	Configurations[1].SessionTimeout = "10000";
	Configurations[1].Resolution->Width = 640;
	Configurations[1].Resolution->Height = 360;

    Configurations[1].RateControl->FrameRateLimit = 25;
	Configurations[1].RateControl->EncodingInterval = 1;
	Configurations[1].RateControl->BitrateLimit = 2048;

	Configurations[1].H264->GovLength = 50;//30
	Configurations[1].H264->H264Profile = tt__H264Profile__High; //tt__H264Profile__Main;

    Configurations[1].Multicast->Port = 18888;
	Configurations[1].Multicast->TTL = 255;
	Configurations[1].Multicast->AutoStart = xsd__boolean__false_;

	Configurations[1].Multicast->Address->Type = tt__IPType__IPv4;
	sprintf(Configurations[1].Multicast->Address->IPv4Address, "%s", ip);
	Configurations[1].Multicast->Address->IPv6Address = NULL;

    trt__GetVideoEncoderConfigurationsResponse->Configurations = Configurations;

    LOG_DBG(LOG_TAG, "\n");

    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioSourceConfigurations' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurations(struct soap* soap, struct _trt__GetAudioSourceConfigurations *trt__GetAudioSourceConfigurations, struct _trt__GetAudioSourceConfigurationsResponse *trt__GetAudioSourceConfigurationsResponse)
{
    
    LOG_DBG(LOG_TAG, "\n");

    //trt__GetAudioSourceConfigurationsResponse->__sizeConfigurations = 1;
    //struct tt__AudioSourceConfiguration* Configurations = (struct tt__AudioSourceConfiguration*)soap_zalloc(soap, sizeof(struct tt__AudioSourceConfiguration) * trt__GetAudioSourceConfigurationsResponse->__sizeConfigurations);

    ///* Audio stream config */
    //Configurations[0].Name = (char*)soap_zalloc(soap, 100);
    //Configurations[0].token = (char*)soap_zalloc(soap, 100);
    //Configurations[0].SourceToken = (char*)soap_zalloc(soap, 100);

    //sprintf(Configurations[0].Name, "%s", "XcamAudioSourceConfig");
    //sprintf(Configurations[0].token, "%s", "XcamAudioSourceConfigToken");
    //sprintf(Configurations[0].SourceToken, "%s", "XcamAudioSourceConfigToken");
    //Configurations[0].UseCount = 3;

    //trt__GetAudioSourceConfigurationsResponse->Configurations = Configurations;

    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioEncoderConfigurations' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurations(struct soap* soap, struct _trt__GetAudioEncoderConfigurations *trt__GetAudioEncoderConfigurations, struct _trt__GetAudioEncoderConfigurationsResponse *trt__GetAudioEncoderConfigurationsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
#if 0
    int ret = 0;
    char ip[20];
    /* get host ip */
    ret = device_ip_get((char*)"eth0", ip); //eth0
    if (ret < 0) {
        LOG_ERR(LOG_TAG, "ERROR: can't get host ip address!\n");
        return SOAP_ERR;
    }

    trt__GetAudioEncoderConfigurationsResponse->__sizeConfigurations = 1;
    struct tt__AudioEncoderConfiguration* Configurations = (struct tt__AudioEncoderConfiguration*)soap_zalloc(soap, sizeof(struct tt__AudioEncoderConfiguration) * trt__GetAudioEncoderConfigurationsResponse->__sizeConfigurations);

    /* Main stream config */
    Configurations[0].Name = (char*)soap_zalloc(soap, 100);
    Configurations[0].token = (char*)soap_zalloc(soap, 100);
    Configurations[0].Multicast = (struct tt__MulticastConfiguration*)soap_zalloc(soap, sizeof(struct tt__MulticastConfiguration));
    Configurations[0].Multicast->Address = (struct tt__IPAddress*)soap_zalloc(soap, sizeof(struct tt__IPAddress));
    Configurations[0].Multicast->Address->IPv4Address = (char*)soap_zalloc(soap, 100);
    Configurations[0].__any = (char*)soap_zalloc(soap, 100);

    sprintf(Configurations[0].Name, "%s", "AudioEncoderConfig");
    sprintf(Configurations[0].token, "%s", "AudioEncoderConfigToken");
    Configurations[0].Encoding = tt__AudioEncoding__AAC;
    Configurations[0].Bitrate = 200;
    Configurations[0].SampleRate =16000 ;
    Configurations[0].UseCount = 2;
    Configurations[0].SessionTimeout = "10000";

    Configurations[0].Multicast->Port = 18888;
    Configurations[0].Multicast->TTL = 255;
    Configurations[0].Multicast->AutoStart = xsd__boolean__false_;

    Configurations[0].Multicast->Address->Type = tt__IPType__IPv4;
    sprintf(Configurations[0].Multicast->Address->IPv4Address, "%s", ip);
    Configurations[0].Multicast->Address->IPv6Address = NULL;

    trt__GetAudioEncoderConfigurationsResponse->Configurations = Configurations;
#endif

    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoAnalyticsConfigurations' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoAnalyticsConfigurations(struct soap* soap, struct _trt__GetVideoAnalyticsConfigurations *trt__GetVideoAnalyticsConfigurations, struct _trt__GetVideoAnalyticsConfigurationsResponse *trt__GetVideoAnalyticsConfigurationsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetMetadataConfigurations' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfigurations(struct soap* soap, struct _trt__GetMetadataConfigurations *trt__GetMetadataConfigurations, struct _trt__GetMetadataConfigurationsResponse *trt__GetMetadataConfigurationsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioOutputConfigurations' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfigurations(struct soap* soap, struct _trt__GetAudioOutputConfigurations *trt__GetAudioOutputConfigurations, struct _trt__GetAudioOutputConfigurationsResponse *trt__GetAudioOutputConfigurationsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioDecoderConfigurations' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfigurations(struct soap* soap, struct _trt__GetAudioDecoderConfigurations *trt__GetAudioDecoderConfigurations, struct _trt__GetAudioDecoderConfigurationsResponse *trt__GetAudioDecoderConfigurationsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoSourceConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfiguration(struct soap* soap, struct _trt__GetVideoSourceConfiguration *trt__GetVideoSourceConfiguration, struct _trt__GetVideoSourceConfigurationResponse *trt__GetVideoSourceConfigurationResponse)
{
    struct tt__VideoSourceConfiguration *Configuration = (struct tt__VideoSourceConfiguration *)soap_zalloc(soap, sizeof(struct tt__VideoSourceConfiguration));


    Configuration->Name = (char *)soap_zalloc(soap, 100);
    Configuration->token = (char *)soap_zalloc(soap, 100);
    Configuration->SourceToken = (char *)soap_zalloc(soap, 100);
    Configuration->Bounds = (struct tt__IntRectangle *)soap_zalloc(soap, sizeof(struct tt__IntRectangle));

    sprintf(Configuration->Name, "%s", "XcamVideoSourceConfig");
    sprintf(Configuration->token, "%s", "XcamVideoSourceConfigToken");
    sprintf(Configuration->SourceToken, "%s", "XcamVideoSourceConfigToken");
    Configuration->UseCount = 3;

    Configuration->Bounds->x = 0;
    Configuration->Bounds->y = 0;
    Configuration->Bounds->width = 1920;
    Configuration->Bounds->height = 1080;
    //Configuration->Extension = NULL;//my

    trt__GetVideoSourceConfigurationResponse->Configuration = Configuration;

    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoEncoderConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfiguration(struct soap* soap, struct _trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration, struct _trt__GetVideoEncoderConfigurationResponse *trt__GetVideoEncoderConfigurationResponse)
{
	int ret = 0;
	char IpAddr[20];
	int iFsChnNum = 0;
	uint32_t iEncChnNum = 0;
	uint32_t iFrmRateNum = 0;
	uint32_t  iFrmRateDen = 0;
	int iBitRate = 0;
	int iPicWidth = 0;
	int iPicHeigth = 0;

	memset(IpAddr, 0, 20);

	if (strcmp(trt__GetVideoEncoderConfiguration->ConfigurationToken, "MainVideoEncoderConfigToken") == 0)
	{
		iFsChnNum = 0;
		iEncChnNum = 0;
	}
	else if(strcmp(trt__GetVideoEncoderConfiguration->ConfigurationToken, "SubVideoEncoderConfigToken") == 0)
	{
		iFsChnNum = 1;
		iEncChnNum = 1;
	}
	else
	{
		LOG_ERR(LOG_TAG,"GetVideoEncoderConfiguration Token error\n");
		return SOAP_ERR;
	}

	//获取IP地址
	ret = device_ip_get((char *)"eth0", IpAddr); //eth0
	if (ret < 0) {
		LOG_ERR(LOG_TAG, "Get host IP address fail.\n");
		return SOAP_ERR;
	}

	//获取帧率
	ret = func_video_get_fps(&iFrmRateNum, &iFrmRateDen);
	if(ret < 0)
	{
		LOG_ERR(LOG_TAG,"Call func_video_get_fps fail.\n");
		return SOAP_ERR;
	}

	//获取码率
	ret = func_video_get_bitrate(iEncChnNum, &iBitRate);
	if(ret < 0)
	{
		LOG_ERR(LOG_TAG,"Call func_video_get_bitrate fail.\n");
		return SOAP_ERR;
	}

	//获取分辨率
	ret = func_video_get_resoulution(iFsChnNum, &iPicWidth, &iPicHeigth);
	if(ret < 0)
	{
		LOG_ERR(LOG_TAG,"Call func_video_get_resolution fail.\n");
		return SOAP_ERR;
	}

	struct tt__VideoEncoderConfiguration *Configuration = (struct tt__VideoEncoderConfiguration *)soap_zalloc(soap, sizeof(struct tt__VideoEncoderConfiguration));
    Configuration->Name = (char *)soap_zalloc(soap, 100);
    Configuration->token = (char *)soap_zalloc(soap, 100);
    Configuration->Resolution = (struct tt__VideoResolution *)soap_zalloc(soap, sizeof(struct tt__VideoResolution));
    Configuration->RateControl = (struct tt__VideoRateControl *)soap_zalloc(soap, sizeof(struct tt__VideoRateControl));
    Configuration->H264 = (struct tt__H264Configuration *)soap_zalloc(soap, sizeof(struct tt__H264Configuration));
    Configuration->Multicast = (struct tt__MulticastConfiguration *)soap_zalloc(soap, sizeof(struct tt__MulticastConfiguration));
    Configuration->Multicast->Address = (struct tt__IPAddress *)soap_zalloc(soap, sizeof(struct tt__IPAddress));
    Configuration->Multicast->Address->IPv4Address = (char *)soap_zalloc(soap, 100);

	Configuration->Encoding = tt__VideoEncoding__H264;
	Configuration->UseCount = 1;
	Configuration->Quality = 30;
	Configuration->SessionTimeout = "10000";
	Configuration->RateControl->FrameRateLimit = 25;
	Configuration->RateControl->EncodingInterval = 1;
	Configuration->RateControl->BitrateLimit = 2048;
	Configuration->H264->GovLength = 50;
	Configuration->H264->H264Profile = tt__H264Profile__High;
	Configuration->Multicast->Port = 18888;
	Configuration->Multicast->TTL = 255;
	Configuration->Multicast->AutoStart = xsd__boolean__false_;
	Configuration->Multicast->Address->Type = tt__IPType__IPv4;
	sprintf(Configuration->Multicast->Address->IPv4Address, "%s", IpAddr);
	Configuration->Multicast->Address->IPv6Address = NULL;

	if (strcmp(trt__GetVideoEncoderConfiguration->ConfigurationToken, "MainVideoEncoderConfigToken") == 0)
	{
		sprintf(Configuration->Name, "%s", "MainVideoEncoderConfig");
		sprintf(Configuration->token, "%s", "MainVideoEncoderConfigToken");

		Configuration->Resolution->Width = iPicWidth;
		Configuration->Resolution->Height = iPicHeigth;
	}
	else
	{
		sprintf(Configuration->Name, "%s", "SubVideoEncoderConfig");
		sprintf(Configuration->token, "%s", "SubVideoEncoderConfigToken");

		Configuration->Resolution->Width = 640;
		Configuration->Resolution->Height = 360;
	}

    trt__GetVideoEncoderConfigurationResponse->Configuration = Configuration;

    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioSourceConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfiguration(struct soap* soap, struct _trt__GetAudioSourceConfiguration *trt__GetAudioSourceConfiguration, struct _trt__GetAudioSourceConfigurationResponse *trt__GetAudioSourceConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioEncoderConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfiguration(struct soap* soap, struct _trt__GetAudioEncoderConfiguration *trt__GetAudioEncoderConfiguration, struct _trt__GetAudioEncoderConfigurationResponse *trt__GetAudioEncoderConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoAnalyticsConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoAnalyticsConfiguration(struct soap* soap, struct _trt__GetVideoAnalyticsConfiguration *trt__GetVideoAnalyticsConfiguration, struct _trt__GetVideoAnalyticsConfigurationResponse *trt__GetVideoAnalyticsConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetMetadataConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfiguration(struct soap* soap, struct _trt__GetMetadataConfiguration *trt__GetMetadataConfiguration, struct _trt__GetMetadataConfigurationResponse *trt__GetMetadataConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioOutputConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfiguration(struct soap* soap, struct _trt__GetAudioOutputConfiguration *trt__GetAudioOutputConfiguration, struct _trt__GetAudioOutputConfigurationResponse *trt__GetAudioOutputConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioDecoderConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfiguration(struct soap* soap, struct _trt__GetAudioDecoderConfiguration *trt__GetAudioDecoderConfiguration, struct _trt__GetAudioDecoderConfigurationResponse *trt__GetAudioDecoderConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetCompatibleVideoEncoderConfigurations' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoEncoderConfigurations(struct soap* soap, struct _trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations, struct _trt__GetCompatibleVideoEncoderConfigurationsResponse *trt__GetCompatibleVideoEncoderConfigurationsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetCompatibleVideoSourceConfigurations' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoSourceConfigurations(struct soap* soap, struct _trt__GetCompatibleVideoSourceConfigurations *trt__GetCompatibleVideoSourceConfigurations, struct _trt__GetCompatibleVideoSourceConfigurationsResponse *trt__GetCompatibleVideoSourceConfigurationsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetCompatibleAudioEncoderConfigurations' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioEncoderConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioEncoderConfigurations *trt__GetCompatibleAudioEncoderConfigurations, struct _trt__GetCompatibleAudioEncoderConfigurationsResponse *trt__GetCompatibleAudioEncoderConfigurationsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetCompatibleAudioSourceConfigurations' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioSourceConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioSourceConfigurations *trt__GetCompatibleAudioSourceConfigurations, struct _trt__GetCompatibleAudioSourceConfigurationsResponse *trt__GetCompatibleAudioSourceConfigurationsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetCompatibleVideoAnalyticsConfigurations' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoAnalyticsConfigurations(struct soap* soap, struct _trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations, struct _trt__GetCompatibleVideoAnalyticsConfigurationsResponse *trt__GetCompatibleVideoAnalyticsConfigurationsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetCompatibleMetadataConfigurations' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleMetadataConfigurations(struct soap* soap, struct _trt__GetCompatibleMetadataConfigurations *trt__GetCompatibleMetadataConfigurations, struct _trt__GetCompatibleMetadataConfigurationsResponse *trt__GetCompatibleMetadataConfigurationsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetCompatibleAudioOutputConfigurations' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioOutputConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioOutputConfigurations *trt__GetCompatibleAudioOutputConfigurations, struct _trt__GetCompatibleAudioOutputConfigurationsResponse *trt__GetCompatibleAudioOutputConfigurationsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetCompatibleAudioDecoderConfigurations' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioDecoderConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioDecoderConfigurations *trt__GetCompatibleAudioDecoderConfigurations, struct _trt__GetCompatibleAudioDecoderConfigurationsResponse *trt__GetCompatibleAudioDecoderConfigurationsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__SetVideoSourceConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoSourceConfiguration(struct soap* soap, struct _trt__SetVideoSourceConfiguration *trt__SetVideoSourceConfiguration, struct _trt__SetVideoSourceConfigurationResponse *trt__SetVideoSourceConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__SetVideoEncoderConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoEncoderConfiguration(struct soap* soap, struct _trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration, struct _trt__SetVideoEncoderConfigurationResponse *trt__SetVideoEncoderConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "__trt__SetVideoEncoderConfiguration\n");
	int ret = 0;
	uint32_t frmRateNum = 0;
	uint32_t frmRateDen = 0;
    uint32_t impvi_num = 0;
	int frmChnNum = 0;
	int encChnNum = 0;
	int targetBitRate = 0;
	int picWidth = 0;
	int picHeigth = 0;

	if(strcmp(trt__SetVideoEncoderConfiguration->Configuration->token,"MainVideoEncoderConfigToken") == 0)
	{
		LOG_DBG(LOG_TAG,"Main Streamn Set Video Encoder Configuration.\n");
		encChnNum = 0;
		frmChnNum = 0;
	}
	else if(strcmp(trt__SetVideoEncoderConfiguration->Configuration->token,"SubVideoEncoderConfigToken") == 0)
	{
		LOG_DBG(LOG_TAG,"Sub Stream Set Video Encoder Configuration.\n");
		encChnNum = 1;
		frmChnNum = 1;
	}
	else
	{
		LOG_ERR(LOG_TAG,"SetVideoEncoderConfiguration Token error\n");
		return SOAP_ERR;
	}

#if 0
	测试设置编码模式
	printf("set func:%d\n",trt__SetVideoEncoderConfiguration->Configuration->Encoding);
	int mode = 0;
	if(trt__SetVideoEncoderConfiguration->Configuration->Encoding == 2)
	{
		mode = 0;
	}
	else
	{
		mode = 1;
	}

	ret = func_video_set_enc_mode(encChnNum, mode);
	if(ret < 0)
	{
		LOG_ERR(LOG_TAG, "Call func_video_set_mode fial.\n");
		return SOAP_ERR;
	}
#endif
#if 1
	//设置分辨率
	picWidth = trt__SetVideoEncoderConfiguration->Configuration->Resolution->Width;
	picHeigth = trt__SetVideoEncoderConfiguration->Configuration->Resolution->Height;
    //picWidth = 640;
    //picHeigth = 360;
    ret = LUX_Video_Change_Resolution(frmChnNum, picWidth, picHeigth);
    if (ret < 0)
    {
        LOG_ERR(LOG_TAG, "Call func_video_set_resolution.\n");
        return SOAP_ERR;
    }
    LUX_Video_Fnuc_Get_Resoulution(frmRateNum, &picWidth, &picHeigth);
    LOG_DBG(LOG_TAG, "Current resloution_picWidth_value = %d\n", picWidth);
    LOG_DBG(LOG_TAG, "Current resloution_picHeigth_value = %d\n", picHeigth);

	//设置帧率
	frmRateNum = trt__SetVideoEncoderConfiguration->Configuration->RateControl->FrameRateLimit;
	frmRateDen = 1;
    impvi_num = 0;
    ret = LUX_Video_Func_Set_Fps(impvi_num,&frmRateNum, &frmRateDen);
	if(ret < 0)
	{
		LOG_ERR(LOG_TAG,"Call func_video_set_fps fail.\n");
		return SOAP_ERR;
	}
    LUX_Video_Func_Get_Fps(&frmRateNum, &frmRateDen);
    LOG_DBG(LOG_TAG,"Current frmateNum = %d\n", frmRateNum);

    //设置码率
    targetBitRate = trt__SetVideoEncoderConfiguration->Configuration->RateControl->BitrateLimit;
    printf("targetBitRate = %d", targetBitRate);
    ret = LUX_Video_Func_Set_Bitrate(encChnNum, targetBitRate);
    if (ret < 0)
    {
        LOG_ERR(LOG_TAG, "Call fun_video_set_bitrate fail\n");
        return SOAP_ERR;
    }
    LUX_Video_Get_Bitrate(encChnNum, &targetBitRate);
    LOG_DBG(LOG_TAG, "Current BitRate = %d\n", targetBitRate);

#endif
	return SOAP_OK;
}

/** Web service operation '__trt__SetAudioSourceConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioSourceConfiguration(struct soap* soap, struct _trt__SetAudioSourceConfiguration *trt__SetAudioSourceConfiguration, struct _trt__SetAudioSourceConfigurationResponse *trt__SetAudioSourceConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__SetAudioEncoderConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioEncoderConfiguration(struct soap* soap, struct _trt__SetAudioEncoderConfiguration *trt__SetAudioEncoderConfiguration, struct _trt__SetAudioEncoderConfigurationResponse *trt__SetAudioEncoderConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__SetVideoAnalyticsConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoAnalyticsConfiguration(struct soap* soap, struct _trt__SetVideoAnalyticsConfiguration *trt__SetVideoAnalyticsConfiguration, struct _trt__SetVideoAnalyticsConfigurationResponse *trt__SetVideoAnalyticsConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__SetMetadataConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetMetadataConfiguration(struct soap* soap, struct _trt__SetMetadataConfiguration *trt__SetMetadataConfiguration, struct _trt__SetMetadataConfigurationResponse *trt__SetMetadataConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__SetAudioOutputConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioOutputConfiguration(struct soap* soap, struct _trt__SetAudioOutputConfiguration *trt__SetAudioOutputConfiguration, struct _trt__SetAudioOutputConfigurationResponse *trt__SetAudioOutputConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__SetAudioDecoderConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioDecoderConfiguration(struct soap* soap, struct _trt__SetAudioDecoderConfiguration *trt__SetAudioDecoderConfiguration, struct _trt__SetAudioDecoderConfigurationResponse *trt__SetAudioDecoderConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoSourceConfigurationOptions' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfigurationOptions(struct soap* soap, struct _trt__GetVideoSourceConfigurationOptions *trt__GetVideoSourceConfigurationOptions, struct _trt__GetVideoSourceConfigurationOptionsResponse *trt__GetVideoSourceConfigurationOptionsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoEncoderConfigurationOptions' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurationOptions(struct soap* soap, struct _trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions, struct _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse)
{
    struct tt__VideoEncoderConfigurationOptions *Options = (struct tt__VideoEncoderConfigurationOptions *)soap_zalloc(soap, sizeof(struct tt__VideoEncoderConfigurationOptions));
    Options->QualityRange = (struct tt__IntRange *)soap_zalloc(soap, sizeof(struct tt__IntRange));
    Options->H264 = (struct tt__H264Options *)soap_zalloc(soap, sizeof(struct tt__H264Options));
    Options->Extension =  (struct tt__VideoEncoderOptionsExtension*)soap_zalloc(soap, sizeof(struct tt__VideoEncoderOptionsExtension));
    Options->H264->ResolutionsAvailable = (struct tt__VideoResolution *)soap_zalloc(soap, sizeof(struct tt__VideoResolution) * 2);
    Options->H264->GovLengthRange = (struct tt__IntRange *)soap_zalloc(soap, sizeof(struct tt__IntRange));
    Options->H264->FrameRateRange = (struct tt__IntRange *)soap_zalloc(soap, sizeof(struct tt__IntRange));
    Options->H264->EncodingIntervalRange = (struct tt__IntRange *)soap_zalloc(soap, sizeof(struct tt__IntRange));
    Options->H264->H264ProfilesSupported = (enum tt__H264Profile *)soap_zalloc(soap, sizeof(enum tt__H264Profile));

    Options->Extension->H264 = (struct tt__H264Options2*)soap_zalloc(soap, sizeof(struct tt__H264Options2));
    Options->Extension->H264->BitrateRange = (struct tt__IntRange*)soap_zalloc(soap, sizeof(struct tt__IntRange));

    Options->QualityRange->Min = 0;
    Options->QualityRange->Max = 100;

    Options->H264->__sizeResolutionsAvailable = 2;
    Options->H264->__sizeH264ProfilesSupported = 1;
    *Options->H264->H264ProfilesSupported = tt__H264Profile__Main;

    Options->H264->ResolutionsAvailable[0].Width = 1920;
    Options->H264->ResolutionsAvailable[0].Height = 1080;

    Options->H264->ResolutionsAvailable[1].Width = 1280;
    Options->H264->ResolutionsAvailable[1].Height = 720;

    Options->H264->GovLengthRange->Min = 1;
    Options->H264->GovLengthRange->Max = 360;

    Options->H264->FrameRateRange->Min = 5;
    Options->H264->FrameRateRange->Max = 30;

    Options->H264->EncodingIntervalRange->Min = 1;
    Options->H264->EncodingIntervalRange->Max = 100;


    Options->Extension->H264->BitrateRange->Min = 5;
    Options->Extension->H264->BitrateRange->Max = 250;

    trt__GetVideoEncoderConfigurationOptionsResponse->Options = Options;
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioSourceConfigurationOptions' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurationOptions(struct soap* soap, struct _trt__GetAudioSourceConfigurationOptions *trt__GetAudioSourceConfigurationOptions, struct _trt__GetAudioSourceConfigurationOptionsResponse *trt__GetAudioSourceConfigurationOptionsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioEncoderConfigurationOptions' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurationOptions(struct soap* soap, struct _trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions, struct _trt__GetAudioEncoderConfigurationOptionsResponse *trt__GetAudioEncoderConfigurationOptionsResponse)
{
    struct tt__AudioEncoderConfigurationOptions *Options_1 = (struct tt__AudioEncoderConfigurationOptions *)soap_zalloc(soap, sizeof(struct tt__AudioEncoderConfigurationOptions));
    Options_1->Options = (struct tt__AudioEncoderConfigurationOption*)soap_zalloc(soap, sizeof(struct tt__AudioEncoderConfigurationOption));
    Options_1->__sizeOptions = 1;
    Options_1->Options->BitrateList = (struct tt__IntItems *)soap_zalloc(soap, sizeof(struct tt__IntItems));
    Options_1->Options->BitrateList->Items = (int*)soap_zalloc(soap, sizeof(int));
    Options_1->Options->SampleRateList = (struct tt__IntItems*)soap_zalloc(soap, sizeof(struct tt__IntItems));
    Options_1->Options->SampleRateList->Items = (int*)soap_zalloc(soap, sizeof(int));

    Options_1->Options->Encoding = tt__AudioEncoding__AAC;
    Options_1->Options->BitrateList->__sizeItems = 1;
    Options_1->Options->SampleRateList->__sizeItems = 1;
    Options_1->Options->BitrateList->Items[0] = 200;
    Options_1->Options->SampleRateList->Items[0] = 16000;

    Options_1->Options->BitrateList->Items[1] = 100;
    Options_1->Options->SampleRateList->Items[1] = 15000;

    trt__GetAudioEncoderConfigurationOptionsResponse->Options = Options_1;
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetMetadataConfigurationOptions' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfigurationOptions(struct soap* soap, struct _trt__GetMetadataConfigurationOptions *trt__GetMetadataConfigurationOptions, struct _trt__GetMetadataConfigurationOptionsResponse *trt__GetMetadataConfigurationOptionsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioOutputConfigurationOptions' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfigurationOptions(struct soap* soap, struct _trt__GetAudioOutputConfigurationOptions *trt__GetAudioOutputConfigurationOptions, struct _trt__GetAudioOutputConfigurationOptionsResponse *trt__GetAudioOutputConfigurationOptionsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetAudioDecoderConfigurationOptions' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfigurationOptions(struct soap* soap, struct _trt__GetAudioDecoderConfigurationOptions *trt__GetAudioDecoderConfigurationOptions, struct _trt__GetAudioDecoderConfigurationOptionsResponse *trt__GetAudioDecoderConfigurationOptionsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetGuaranteedNumberOfVideoEncoderInstances' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetGuaranteedNumberOfVideoEncoderInstances(struct soap* soap, struct _trt__GetGuaranteedNumberOfVideoEncoderInstances *trt__GetGuaranteedNumberOfVideoEncoderInstances, struct _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse *trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetStreamUri' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetStreamUri(struct soap* soap, struct _trt__GetStreamUri *trt__GetStreamUri, struct _trt__GetStreamUriResponse *trt__GetStreamUriResponse)
{
	int ret = 0;
	char ip[20];
	char url[MAX_XADDR_LEN];

	memset(url, 0, MAX_XADDR_LEN);

	/* get host ip */
	ret = device_ip_get((char *)"eth0", ip); //eth0
	if (ret < 0)
	{
		LOG_ERR(LOG_TAG, "ERROR: can't get host ip address!\n");
		return SOAP_ERR;
	}

    struct tt__MediaUri *MediaUri = (struct tt__MediaUri *)soap_zalloc(soap, sizeof(struct tt__MediaUri));
    MediaUri->Uri = (char *)soap_zalloc(soap, MAX_XADDR_LEN);
    if (NULL == trt__GetStreamUri->ProfileToken)
    {
        sprintf(url, "rtsp://%s:%d/%s", ip, 554, TT_MEDIA_0_NAME);
    }
    else
    {
        if (0 == strcmp(trt__GetStreamUri->ProfileToken, "MainStreamToken"))
        {
            sprintf(url, "rtsp://%s:%d/%s", ip, 554, TT_MEDIA_0_NAME);
        }
        else if (0 == strcmp(trt__GetStreamUri->ProfileToken, "SubStreamToken"))
        {
            sprintf(url, "rtsp://%s:%d/%s", ip, 554, TT_MEDIA_1_NAME);
        }
        else
        {
            LOG_ERR(LOG_TAG, "ERROR: Get Stream Uri ProfileToken error.\n");
            return SOAP_ERR;
        }
    }

	strncpy(MediaUri->Uri, url, MAX_XADDR_LEN);
	LOG_DBG(LOG_TAG, "MediaUri->Uri: [%s]\n", MediaUri->Uri);
	MediaUri->InvalidAfterConnect = xsd__boolean__false_;
	MediaUri->InvalidAfterReboot = xsd__boolean__false_;
	MediaUri->Timeout = "PT300S";

	trt__GetStreamUriResponse->MediaUri = MediaUri;

	return SOAP_OK;
}

/** Web service operation '__trt__StartMulticastStreaming' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__StartMulticastStreaming(struct soap* soap, struct _trt__StartMulticastStreaming *trt__StartMulticastStreaming, struct _trt__StartMulticastStreamingResponse *trt__StartMulticastStreamingResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__StopMulticastStreaming' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__StopMulticastStreaming(struct soap* soap, struct _trt__StopMulticastStreaming *trt__StopMulticastStreaming, struct _trt__StopMulticastStreamingResponse *trt__StopMulticastStreamingResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__SetSynchronizationPoint' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetSynchronizationPoint(struct soap* soap, struct _trt__SetSynchronizationPoint *trt__SetSynchronizationPoint, struct _trt__SetSynchronizationPointResponse *trt__SetSynchronizationPointResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetSnapshotUri' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetSnapshotUri(struct soap* soap, struct _trt__GetSnapshotUri *trt__GetSnapshotUri, struct _trt__GetSnapshotUriResponse *trt__GetSnapshotUriResponse)
{
    int ret = 0;
    char ip[20];
    char url[MAX_XADDR_LEN];

    /* get host ip */
    ret = device_ip_get((char *)"eth0", ip); //eth0
    if (ret < 0) {
	LOG_ERR(LOG_TAG, "ERROR: can't get host ip address!\n");
	return SOAP_ERR;
    }

    sprintf(url, "rtsp://%s:%d/main", ip, 554);
    struct tt__MediaUri *MediaUri = (struct tt__MediaUri *)soap_zalloc(soap, sizeof(struct tt__MediaUri));
    MediaUri->Uri = (char *)soap_zalloc(soap, 100);

    strcpy(MediaUri->Uri, url);
    LOG_DBG(LOG_TAG, "MediaUri->Uri: [%s]\n", MediaUri->Uri);
    MediaUri->InvalidAfterConnect = xsd__boolean__false_;
    MediaUri->InvalidAfterReboot = xsd__boolean__true_;
    MediaUri->Timeout = 0;

    trt__GetSnapshotUriResponse->MediaUri = MediaUri;

	LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetVideoSourceModes' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceModes(struct soap* soap, struct _trt__GetVideoSourceModes *trt__GetVideoSourceModes, struct _trt__GetVideoSourceModesResponse *trt__GetVideoSourceModesResponse)
{
	LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__SetVideoSourceMode' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoSourceMode(struct soap* soap, struct _trt__SetVideoSourceMode *trt__SetVideoSourceMode, struct _trt__SetVideoSourceModeResponse *trt__SetVideoSourceModeResponse)
{
	LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trt__GetOSDs' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetOSDs(struct soap* soap, struct _trt__GetOSDs *trt__GetOSDs, struct _trt__GetOSDsResponse *trt__GetOSDsResponse)
{
	LOG_DBG(LOG_TAG, "\n");

	int osdnumber = 0;
	int streamnum = 0;

	//获取总共已经有了多少个osd
	osdnumber = func_osd_get_osdrgns_num(streamnum);
	if (osdnumber < 0) {
		LOG_ERR(LOG_TAG,"call func_get_osdrgns_num error.\n");
	}

	//获取每个osd的配置
	trt__GetOSDsResponse->__sizeOSDs = 2;
	struct tt__OSDConfiguration *OSD = (struct tt__OSDConfiguration *)soap_zalloc(soap,sizeof(struct tt__OSDConfiguration)*(trt__GetOSDsResponse->__sizeOSDs));

	OSD[0].token = "osdstream0_0";
	OSD[0].Type =tt__OSDType__Text;
	OSD[0].Position = (struct tt__OSDPosConfiguration *)soap_zalloc(soap, sizeof(struct tt__OSDPosConfiguration ));
	OSD[0].Position->Type = "custom";
	OSD[0].Position->Pos = (struct tt__Vector *)soap_zalloc(soap,sizeof(struct tt__Vector));

	OSD[0].Position->Pos->x = (float *)soap_zalloc(soap, sizeof(float));
	OSD[0].Position->Pos->y = (float *)soap_zalloc(soap, sizeof(float));
	*(OSD[0].Position->Pos->x) = 1.0;
	*(OSD[0].Position->Pos->y) = -1.0;

	OSD[0].TextString = (struct tt__OSDTextConfiguration *)soap_zalloc(soap,sizeof(struct tt__OSDTextConfiguration));

	OSD[0].TextString->Type = "DateAndTime";
	OSD[0].TextString->DateFormat = "M/d/yyyy";
	OSD[0].TextString->TimeFormat = "h:mm:ss tt";

	trt__GetOSDsResponse->OSDs = OSD;

	return SOAP_OK;
}

/** Web service operation '__trt__GetOSD' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetOSD(struct soap* soap, struct _trt__GetOSD *trt__GetOSD, struct _trt__GetOSDResponse *trt__GetOSDResponse)
{
	LOG_DBG(LOG_TAG, "\n");
	return SOAP_OK;
}

/** Web service operation '__trt__GetOSDOptions' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetOSDOptions(struct soap* soap, struct _trt__GetOSDOptions *trt__GetOSDOptions, struct _trt__GetOSDOptionsResponse *trt__GetOSDOptionsResponse)
{
	LOG_DBG(LOG_TAG, "\n");

    struct tt__OSDConfigurationOptions *OSDOptions = (struct tt__OSDConfigurationOptions *)soap_zalloc(soap, sizeof(struct tt__OSDConfigurationOptions));

	struct tt__MaximumNumberOfOSDs *MaximumNumberOfOSDs = (struct tt__MaximumNumberOfOSDs *)soap_zalloc(soap,sizeof(struct tt__MaximumNumberOfOSDs));

	struct tt__OSDTextOptions *TextOption = (struct tt__OSDTextOptions *)soap_zalloc(soap,sizeof(struct tt__OSDTextOptions));

	memset(OSDOptions,0,sizeof(struct tt__OSDConfigurationOptions));
	memset(MaximumNumberOfOSDs,0,sizeof(struct tt__MaximumNumberOfOSDs));
	memset(TextOption,0,sizeof(struct tt__OSDTextOptions));

	MaximumNumberOfOSDs->Total = 2;
	MaximumNumberOfOSDs->PlainText = (int *)soap_zalloc(soap,sizeof(int));
	MaximumNumberOfOSDs->DateAndTime = (int *)soap_zalloc(soap,sizeof(int));
	*(MaximumNumberOfOSDs->PlainText) = 1;
	*(MaximumNumberOfOSDs->DateAndTime) = 1;

	OSDOptions->__sizeType = 2;
	OSDOptions->Type = (enum tt__OSDType *)soap_zalloc(soap,sizeof(enum tt__OSDType)*(OSDOptions->__sizeType));
	OSDOptions->Type[0] = tt__OSDType__Text;
	OSDOptions->Type[1] = tt__OSDType__Text;

	OSDOptions->__sizePositionOption = 2;
	char* position_0 = "Custom";
	char* position_1 = "Custom";
	//0号
	static char *c[2];
	c[0] = position_0;
	//1号
	c[1] = position_1;

	OSDOptions->PositionOption = c;

	TextOption->__sizeType = 2;
	char* type_0 = "PlainText";
	char* type_1 = "DateAndTime";

	static char* a_rhq[2];
	//0
	a_rhq[0]=type_0;
	//1
	a_rhq[1]=type_1;
	TextOption->Type = a_rhq;

	TextOption->__sizeTimeFormat = 1;
	char* timetype = "h:mm:ss tt";
	static char *b_rhq[1];
	b_rhq[0] = timetype;
	TextOption->TimeFormat = b_rhq;

	TextOption->__sizeDateFormat = 1;
	 char* datetype = "M/d/yyyy";
	static char* d_rhq[1];
	d_rhq[0] = datetype;
	TextOption->DateFormat = d_rhq;

	OSDOptions->TextOption = TextOption;
	OSDOptions->MaximumNumberOfOSDs = MaximumNumberOfOSDs;

	trt__GetOSDOptionsResponse->OSDOptions = OSDOptions;

	return SOAP_OK;
}

/** Web service operation '__trt__SetOSD' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetOSD(struct soap* soap, struct _trt__SetOSD *trt__SetOSD, struct _trt__SetOSDResponse *trt__SetOSDResponse)
{
	LOG_DBG(LOG_TAG, "\n");
	int streamnum =0;
	int ret = 0;
	int picWidth = 1920;
	int picHeigth = 1080;
	float pos_x = *(trt__SetOSD->OSD->Position->Pos->x);
	float pos_y = *(trt__SetOSD->OSD->Position->Pos->y);
	int x = 0;
	int y = 0;

	if (strcmp(trt__SetOSD->OSD->VideoSourceConfigurationToken->__item, "MainStreamToken") == 0) {
		streamnum = 0;
	} else {
		streamnum = 0;
	}

//	ret = func_video_get_resoulution(streamnum, picWidth, picHeigth);
	if (ret < 0) {
		LOG_ERR(LOG_TAG,"Call func_video_set_resolution fail.\n");
		return ret;
	}

	//trt__SetOSD->OSD->Position->Type这个看是不是需要转换一下
	//坐标转换
	x = (1.0 + pos_x) * ((float)picWidth) / 2.0;
	y = (1.0 - pos_y) * ((float)picHeigth) / 2.0;

	ret = func_osd_set_text(streamnum, trt__SetOSD->OSD->token, x, y, trt__SetOSD->OSD->TextString->PlainText);
	if (ret < 0) {
		return SOAP_ERR;
	}

	return SOAP_OK;
}

/** Web service operation '__trt__CreateOSD' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__CreateOSD(struct soap* soap, struct _trt__CreateOSD *trt__CreateOSD, struct _trt__CreateOSDResponse *trt__CreateOSDResponse)
{
	LOG_DBG(LOG_TAG, "\n");
	int streamnum = 0;
	int osdRgnType = 0;
//	char* type = NULL;
	char* text = NULL;
	int ret = 0;
	int picWidth = 1920;
	int picHeigth = 1080;
	float pos_x = 0;
	float pos_y = 0;
	int x = 0;
	int y = 0;

	if (strcmp(trt__CreateOSD->OSD->token, "MainStreamToken")== 0) {
		streamnum = 0;
	} else {
		streamnum = 0;
	}

	osdRgnType = trt__CreateOSD->OSD->Type;
//	type = trt__CreateOSD->OSD->Position->Type;//这个type有四个类型左上，右上左下右下，在这里处理
	text = trt__CreateOSD->OSD->TextString->PlainText;
	pos_x = *(trt__CreateOSD->OSD->Position->Pos->x);
	pos_y = *(trt__CreateOSD->OSD->Position->Pos->y);

    printf("Token : %s\n", trt__CreateOSD->OSD->token);
    printf("osdRgnType : %d\n", osdRgnType);
    //printf("type : %s\n", type);
    printf("text : %s\n", text);
    printf("pos_x : %d\n", pos_x);
    printf("pos_y : %d\n", pos_y);
	//坐标转换
	x = (1.0 + pos_x) * ((float)picWidth) / 2.0;
	y = (1.0 - pos_y) * ((float)picHeigth) / 2.0;
    printf("pos_x_1 : %d\n", x);
    printf("pos_y_1 : %d\n", y);
	char token[20] = {0};

	ret = func_osd_create_text(streamnum, token, x, y, text);
	if (ret < 0) {
		return SOAP_ERR;
	}

	trt__CreateOSDResponse = (struct _trt__CreateOSDResponse *)soap_zalloc(soap, sizeof(struct _trt__CreateOSDResponse));

	trt__CreateOSDResponse->OSDToken = "osdstream0_4";

	return SOAP_OK;
}

/** Web service operation '__trt__DeleteOSD' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__DeleteOSD(struct soap* soap, struct _trt__DeleteOSD *trt__DeleteOSD, struct _trt__DeleteOSDResponse *trt__DeleteOSDResponse)
{
	LOG_DBG(LOG_TAG, "\n");
	int ret = 0;
	int bshow = 0;
	char* token = trt__DeleteOSD->OSDToken;

    LUX_OSD_Show(bshow);

    return SOAP_OK;
}

/** Web service operation '__tptz__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetServiceCapabilities(struct soap* soap, struct _tptz__GetServiceCapabilities* tptz__GetServiceCapabilities, struct _tptz__GetServiceCapabilitiesResponse* tptz__GetServiceCapabilitiesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__GetConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetConfigurations(struct soap* soap, struct _tptz__GetConfigurations* tptz__GetConfigurations, struct _tptz__GetConfigurationsResponse* tptz__GetConfigurationsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__GetPresets' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresets(struct soap* soap, struct _tptz__GetPresets* tptz__GetPresets, struct _tptz__GetPresetsResponse* tptz__GetPresetsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__SetPreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetPreset(struct soap* soap, struct _tptz__SetPreset* tptz__SetPreset, struct _tptz__SetPresetResponse* tptz__SetPresetResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__RemovePreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__RemovePreset(struct soap* soap, struct _tptz__RemovePreset* tptz__RemovePreset, struct _tptz__RemovePresetResponse* tptz__RemovePresetResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__GotoPreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GotoPreset(struct soap* soap, struct _tptz__GotoPreset* tptz__GotoPreset, struct _tptz__GotoPresetResponse* tptz__GotoPresetResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__GetStatus' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetStatus(struct soap* soap, struct _tptz__GetStatus* tptz__GetStatus, struct _tptz__GetStatusResponse* tptz__GetStatusResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__GetConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetConfiguration(struct soap* soap, struct _tptz__GetConfiguration* tptz__GetConfiguration, struct _tptz__GetConfigurationResponse* tptz__GetConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__GetNodes' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetNodes(struct soap* soap, struct _tptz__GetNodes* tptz__GetNodes, struct _tptz__GetNodesResponse* tptz__GetNodesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__GetNode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetNode(struct soap* soap, struct _tptz__GetNode* tptz__GetNode, struct _tptz__GetNodeResponse* tptz__GetNodeResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__SetConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetConfiguration(struct soap* soap, struct _tptz__SetConfiguration* tptz__SetConfiguration, struct _tptz__SetConfigurationResponse* tptz__SetConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__GetConfigurationOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetConfigurationOptions(struct soap* soap, struct _tptz__GetConfigurationOptions* tptz__GetConfigurationOptions, struct _tptz__GetConfigurationOptionsResponse* tptz__GetConfigurationOptionsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__GotoHomePosition' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GotoHomePosition(struct soap* soap, struct _tptz__GotoHomePosition* tptz__GotoHomePosition, struct _tptz__GotoHomePositionResponse* tptz__GotoHomePositionResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__SetHomePosition' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetHomePosition(struct soap* soap, struct _tptz__SetHomePosition* tptz__SetHomePosition, struct _tptz__SetHomePositionResponse* tptz__SetHomePositionResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__ContinuousMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__ContinuousMove(struct soap* soap, struct _tptz__ContinuousMove* tptz__ContinuousMove, struct _tptz__ContinuousMoveResponse* tptz__ContinuousMoveResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__RelativeMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__RelativeMove(struct soap* soap, struct _tptz__RelativeMove* tptz__RelativeMove, struct _tptz__RelativeMoveResponse* tptz__RelativeMoveResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__SendAuxiliaryCommand' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__SendAuxiliaryCommand(struct soap* soap, struct _tptz__SendAuxiliaryCommand* tptz__SendAuxiliaryCommand, struct _tptz__SendAuxiliaryCommandResponse* tptz__SendAuxiliaryCommandResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__AbsoluteMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__AbsoluteMove(struct soap* soap, struct _tptz__AbsoluteMove* tptz__AbsoluteMove, struct _tptz__AbsoluteMoveResponse* tptz__AbsoluteMoveResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__GeoMove' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GeoMove(struct soap* soap, struct _tptz__GeoMove* tptz__GeoMove, struct _tptz__GeoMoveResponse* tptz__GeoMoveResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__Stop' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__Stop(struct soap* soap, struct _tptz__Stop* tptz__Stop, struct _tptz__StopResponse* tptz__StopResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__GetPresetTours' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresetTours(struct soap* soap, struct _tptz__GetPresetTours* tptz__GetPresetTours, struct _tptz__GetPresetToursResponse* tptz__GetPresetToursResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__GetPresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresetTour(struct soap* soap, struct _tptz__GetPresetTour* tptz__GetPresetTour, struct _tptz__GetPresetTourResponse* tptz__GetPresetTourResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__GetPresetTourOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresetTourOptions(struct soap* soap, struct _tptz__GetPresetTourOptions* tptz__GetPresetTourOptions, struct _tptz__GetPresetTourOptionsResponse* tptz__GetPresetTourOptionsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__CreatePresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__CreatePresetTour(struct soap* soap, struct _tptz__CreatePresetTour* tptz__CreatePresetTour, struct _tptz__CreatePresetTourResponse* tptz__CreatePresetTourResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__ModifyPresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__ModifyPresetTour(struct soap* soap, struct _tptz__ModifyPresetTour* tptz__ModifyPresetTour, struct _tptz__ModifyPresetTourResponse* tptz__ModifyPresetTourResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__OperatePresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__OperatePresetTour(struct soap* soap, struct _tptz__OperatePresetTour* tptz__OperatePresetTour, struct _tptz__OperatePresetTourResponse* tptz__OperatePresetTourResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__RemovePresetTour' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__RemovePresetTour(struct soap* soap, struct _tptz__RemovePresetTour* tptz__RemovePresetTour, struct _tptz__RemovePresetTourResponse* tptz__RemovePresetTourResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__GetCompatibleConfigurations' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetCompatibleConfigurations(struct soap* soap, struct _tptz__GetCompatibleConfigurations* tptz__GetCompatibleConfigurations, struct _tptz__GetCompatibleConfigurationsResponse* tptz__GetCompatibleConfigurationsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tptz__MoveAndStartTracking' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tptz__MoveAndStartTracking(struct soap* soap, struct _tptz__MoveAndStartTracking* tptz__MoveAndStartTracking, struct _tptz__MoveAndStartTrackingResponse* tptz__MoveAndStartTrackingResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetPasswordComplexityOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPasswordComplexityOptions(struct soap* soap, struct _tds__GetPasswordComplexityOptions* tds__GetPasswordComplexityOptions, struct _tds__GetPasswordComplexityOptionsResponse* tds__GetPasswordComplexityOptionsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetPasswordComplexityConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPasswordComplexityConfiguration(struct soap* soap, struct _tds__GetPasswordComplexityConfiguration* tds__GetPasswordComplexityConfiguration, struct _tds__GetPasswordComplexityConfigurationResponse* tds__GetPasswordComplexityConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetPasswordComplexityConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetPasswordComplexityConfiguration(struct soap* soap, struct _tds__SetPasswordComplexityConfiguration* tds__SetPasswordComplexityConfiguration, struct _tds__SetPasswordComplexityConfigurationResponse* tds__SetPasswordComplexityConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetPasswordHistoryConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPasswordHistoryConfiguration(struct soap* soap, struct _tds__GetPasswordHistoryConfiguration* tds__GetPasswordHistoryConfiguration, struct _tds__GetPasswordHistoryConfigurationResponse* tds__GetPasswordHistoryConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetPasswordHistoryConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetPasswordHistoryConfiguration(struct soap* soap, struct _tds__SetPasswordHistoryConfiguration* tds__SetPasswordHistoryConfiguration, struct _tds__SetPasswordHistoryConfigurationResponse* tds__SetPasswordHistoryConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetAuthFailureWarningOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetAuthFailureWarningOptions(struct soap* soap, struct _tds__GetAuthFailureWarningOptions* tds__GetAuthFailureWarningOptions, struct _tds__GetAuthFailureWarningOptionsResponse* tds__GetAuthFailureWarningOptionsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__GetAuthFailureWarningConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetAuthFailureWarningConfiguration(struct soap* soap, struct _tds__GetAuthFailureWarningConfiguration* tds__GetAuthFailureWarningConfiguration, struct _tds__GetAuthFailureWarningConfigurationResponse* tds__GetAuthFailureWarningConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tds__SetAuthFailureWarningConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetAuthFailureWarningConfiguration(struct soap* soap, struct _tds__SetAuthFailureWarningConfiguration* tds__SetAuthFailureWarningConfiguration, struct _tds__SetAuthFailureWarningConfigurationResponse* tds__SetAuthFailureWarningConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHashingAlgorithm(struct soap* soap, struct _tds__SetHashingAlgorithm* tds__SetHashingAlgorithm, struct _tds__SetHashingAlgorithmResponse* tds__SetHashingAlgorithmResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tdn__Hello' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tdn__Hello(struct soap* soap, struct wsdd__HelloType tdn__Hello, struct wsdd__ResolveType* tdn__HelloResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tdn__Bye' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tdn__Bye(struct soap* soap, struct wsdd__ByeType tdn__Bye, struct wsdd__ResolveType* tdn__ByeResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__tdn__Probe' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tdn__Probe(struct soap* soap, struct wsdd__ProbeType tdn__Probe, struct wsdd__ProbeMatchesType* tdn__ProbeResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__timg__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetServiceCapabilities(struct soap* soap, struct _timg__GetServiceCapabilities* timg__GetServiceCapabilities, struct _timg__GetServiceCapabilitiesResponse* timg__GetServiceCapabilitiesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__timg__GetImagingSettings' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetImagingSettings(struct soap* soap, struct _timg__GetImagingSettings* timg__GetImagingSettings, struct _timg__GetImagingSettingsResponse* timg__GetImagingSettingsResponse)
{
    /* Main stream settings */
    struct tt__ImagingSettings20* Settings = (struct tt__ImagingSettings20*)soap_zalloc(soap, sizeof(struct tt__ImagingSettings20));
    Settings[0].BacklightCompensation = (struct tt__BacklightCompensation20*)soap_zalloc(soap, sizeof(struct tt__BacklightCompensation20));
    Settings->Brightness = (float*)soap_zalloc(soap, sizeof(float));
    Settings[0].ColorSaturation = (float*)soap_zalloc(soap, sizeof(float));
    Settings[0].Contrast = (float*)soap_zalloc(soap, sizeof(float));
    Settings[0].Exposure = (struct tt__Exposure20*)soap_zalloc(soap, sizeof(struct tt__Exposure20));
    Settings[0].Sharpness = (float*)soap_zalloc(soap, sizeof(float));
    Settings[0].WhiteBalance = (struct tt__WhiteBalance20*)soap_zalloc(soap, sizeof(struct tt__WhiteBalance20));
    Settings[0].WideDynamicRange = (struct tt__WideDynamicRange20*)soap_zalloc(soap, sizeof(struct tt__WideDynamicRange20));
    Settings[0].Focus = (struct tt__FocusConfiguration20*)soap_zalloc(soap, sizeof(struct tt__FocusConfiguration20));

    unsigned char Brightness = 0;
    unsigned char Contrast = 0;
    unsigned char Sharpness = 0;
    unsigned char ColorSaturation = 0;
    IMPISPTuningOpsMode WideDynamicRange_Mode;/*WDR模式*/

#ifdef PLATFORM_T40
    IMP_ISP_Tuning_GetBrightness(IMPVI_MAIN, &Brightness);
    IMP_ISP_Tuning_GetSharpness(IMPVI_MAIN, &Contrast);
    IMP_ISP_Tuning_GetSaturation(IMPVI_MAIN, &Sharpness);
    IMP_ISP_Tuning_GetContrast(IMPVI_MAIN, &ColorSaturation);
    IMP_ISP_WDR_ENABLE_GET(IMPVI_MAIN, &WideDynamicRange_Mode);
#else
    IMP_ISP_Tuning_GetBrightness(&Brightness);
    IMP_ISP_Tuning_GetSharpness(&Contrast);
    IMP_ISP_Tuning_GetSaturation(&Sharpness);
    IMP_ISP_Tuning_GetContrast(&ColorSaturation);
    IMP_ISP_WDR_ENABLE_GET(&WideDynamicRange_Mode);
#endif

    /*测试log，程序出错打开调试用*/
   /* printf("Get Brightness value = %d\n", Brightness);
    printf("Get Contrast value = %d\n", Contrast);
    printf("Get Sharpness value = %d\n", Sharpness);
    printf("Get ColorSaturation value = %d\n", ColorSaturation);
    printf("Get WideDynamicRange_Mode value = %d\n", WideDynamicRange_Mode);*/

    *Settings->Brightness = (float)Brightness;
    *Settings->Contrast = (float)Contrast;
    *Settings->Sharpness = (float)Sharpness;
    *Settings->ColorSaturation = (float)ColorSaturation;
    Settings->WideDynamicRange->Mode = WideDynamicRange_Mode;

    /*测试log，程序出错打开调试用*/
    /*printf("Get_1 Brightness value = %f\n", *Settings->Brightness);
    printf("Get_1 Contrast value = %f\n", *Settings[0].Contrast);
    printf("Get_1 Sharpness value = %f\n", *Settings[0].Sharpness);
    printf("Get_1 ColorSaturation value = %f\n", *Settings[0].ColorSaturation);
    printf("Get_1 WideDynamicRange_Mode value = %d\n", Settings->WideDynamicRange->Mode);*/

    timg__GetImagingSettingsResponse->ImagingSettings = Settings;
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
/** Web service operation '__timg__SetImagingSettings' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__SetImagingSettings(struct soap* soap, struct _timg__SetImagingSettings* timg__SetImagingSettings, struct _timg__SetImagingSettingsResponse* timg__SetImagingSettingsResponse)
{
    unsigned char Brightness = 0;
    unsigned char Contrast = 0;
    unsigned char Sharpness = 0;
    unsigned char ColorSaturation = 0;
    IMPISPTuningOpsMode WideDynamicRange_Mode; 

    Brightness = (unsigned char)(*timg__SetImagingSettings->ImagingSettings->Brightness);
    Contrast = (unsigned char)(*timg__SetImagingSettings->ImagingSettings->Contrast);
    Sharpness = (unsigned char)(*timg__SetImagingSettings->ImagingSettings->Sharpness);
    ColorSaturation = (unsigned char)(*timg__SetImagingSettings->ImagingSettings->ColorSaturation);
   
    if (timg__SetImagingSettings->ImagingSettings->WideDynamicRange->Mode < IMPISP_TUNING_OPS_MODE_BUTT)
    {
        WideDynamicRange_Mode = (IMPISPTuningOpsMode)timg__SetImagingSettings->ImagingSettings->WideDynamicRange->Mode;
    }

    /*测试log，程序出错打开调试用*/
    //printf("Rcv token = %s\n", timg__SetImagingSettings->VideoSourceToken);
    //printf("Rcv Brightness value = %d\n", Brightness);
    //printf("Rcv Contrast value = %d\n", Contrast);
    //printf("Rcv Sharpness value = %d\n", Sharpness);
    //printf("Rcv ColorSaturation value = %d\n", ColorSaturation);
    //printf("Rcv WideDynamicRange_Mode value = %d\n", WideDynamicRange_Mode);


    /*设置亮度、对比度、锐度、颜色饱和度、WDR开关等参数*/
#ifdef PLATFORM_T40
    IMP_ISP_Tuning_SetBrightness(IMPVI_MAIN, &Brightness);
    IMP_ISP_Tuning_SetContrast(IMPVI_MAIN, &Contrast);
    IMP_ISP_Tuning_SetSharpness(IMPVI_MAIN, &Sharpness);
    IMP_ISP_Tuning_SetSaturation(IMPVI_MAIN, &ColorSaturation);
    IMP_ISP_WDR_ENABLE(IMPVI_MAIN, &WideDynamicRange_Mode);//0 = DIABLE;1 = ENBALE;
    //IMP_ISP_Tuning_SetBcshHue();
#else
    IMP_ISP_Tuning_SetBrightness(&Brightness);
    IMP_ISP_Tuning_SetContrast(&Contrast);
    IMP_ISP_Tuning_SetSharpness(&Sharpness);
    IMP_ISP_Tuning_SetSaturation(&ColorSaturation);
    IMP_ISP_WDR_ENABLE(&WideDynamicRange_Mode);//0 = DIABLE;1 = ENBALE;
    //IMP_ISP_Tuning_SetBcshHue();
#endif
 
    LOG_DBG(LOG_TAG, "\n"); 
    return SOAP_OK;
}
/** Web service operation '__timg__GetOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetOptions(struct soap* soap, struct _timg__GetOptions* timg__GetOptions, struct _timg__GetOptionsResponse* timg__GetOptionsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
/** Web service operation '__timg__Move' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__Move(struct soap* soap, struct _timg__Move* timg__Move, struct _timg__MoveResponse* timg__MoveResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
/** Web service operation '__timg__Stop' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__Stop(struct soap* soap, struct _timg__Stop* timg__Stop, struct _timg__StopResponse* timg__StopResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
/** Web service operation '__timg__GetStatus' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetStatus(struct soap* soap, struct _timg__GetStatus* timg__GetStatus, struct _timg__GetStatusResponse* timg__GetStatusResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
/** Web service operation '__timg__GetMoveOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetMoveOptions(struct soap* soap, struct _timg__GetMoveOptions* timg__GetMoveOptions, struct _timg__GetMoveOptionsResponse* timg__GetMoveOptionsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
/** Web service operation '__timg__GetPresets' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetPresets(struct soap* soap, struct _timg__GetPresets* timg__GetPresets, struct _timg__GetPresetsResponse* timg__GetPresetsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
/** Web service operation '__timg__GetCurrentPreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetCurrentPreset(struct soap* soap, struct _timg__GetCurrentPreset* timg__GetCurrentPreset, struct _timg__GetCurrentPresetResponse* timg__GetCurrentPresetResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
/** Web service operation '__timg__SetCurrentPreset' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__SetCurrentPreset(struct soap* soap, struct _timg__SetCurrentPreset* timg__SetCurrentPreset, struct _timg__SetCurrentPresetResponse* timg__SetCurrentPresetResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__GetServiceCapabilities(struct soap* soap, struct _trc__GetServiceCapabilities* trc__GetServiceCapabilities, struct _trc__GetServiceCapabilitiesResponse* trc__GetServiceCapabilitiesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__CreateRecording' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__CreateRecording(struct soap* soap, struct _trc__CreateRecording* trc__CreateRecording, struct _trc__CreateRecordingResponse* trc__CreateRecordingResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__DeleteRecording' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__DeleteRecording(struct soap* soap, struct _trc__DeleteRecording* trc__DeleteRecording, struct _trc__DeleteRecordingResponse* trc__DeleteRecordingResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__GetRecordings' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__GetRecordings(struct soap* soap, struct _trc__GetRecordings* trc__GetRecordings, struct _trc__GetRecordingsResponse* trc__GetRecordingsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__SetRecordingConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__SetRecordingConfiguration(struct soap* soap, struct _trc__SetRecordingConfiguration* trc__SetRecordingConfiguration, struct _trc__SetRecordingConfigurationResponse* trc__SetRecordingConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__GetRecordingConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__GetRecordingConfiguration(struct soap* soap, struct _trc__GetRecordingConfiguration* trc__GetRecordingConfiguration, struct _trc__GetRecordingConfigurationResponse* trc__GetRecordingConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__GetRecordingOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__GetRecordingOptions(struct soap* soap, struct _trc__GetRecordingOptions* trc__GetRecordingOptions, struct _trc__GetRecordingOptionsResponse* trc__GetRecordingOptionsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__CreateTrack' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__CreateTrack(struct soap* soap, struct _trc__CreateTrack* trc__CreateTrack, struct _trc__CreateTrackResponse* trc__CreateTrackResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__DeleteTrack' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__DeleteTrack(struct soap* soap, struct _trc__DeleteTrack* trc__DeleteTrack, struct _trc__DeleteTrackResponse* trc__DeleteTrackResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__GetTrackConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__GetTrackConfiguration(struct soap* soap, struct _trc__GetTrackConfiguration* trc__GetTrackConfiguration, struct _trc__GetTrackConfigurationResponse* trc__GetTrackConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__SetTrackConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__SetTrackConfiguration(struct soap* soap, struct _trc__SetTrackConfiguration* trc__SetTrackConfiguration, struct _trc__SetTrackConfigurationResponse* trc__SetTrackConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__CreateRecordingJob' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__CreateRecordingJob(struct soap* soap, struct _trc__CreateRecordingJob* trc__CreateRecordingJob, struct _trc__CreateRecordingJobResponse* trc__CreateRecordingJobResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__DeleteRecordingJob' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__DeleteRecordingJob(struct soap* soap, struct _trc__DeleteRecordingJob* trc__DeleteRecordingJob, struct _trc__DeleteRecordingJobResponse* trc__DeleteRecordingJobResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__GetRecordingJobs' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__GetRecordingJobs(struct soap* soap, struct _trc__GetRecordingJobs* trc__GetRecordingJobs, struct _trc__GetRecordingJobsResponse* trc__GetRecordingJobsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__SetRecordingJobConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__SetRecordingJobConfiguration(struct soap* soap, struct _trc__SetRecordingJobConfiguration* trc__SetRecordingJobConfiguration, struct _trc__SetRecordingJobConfigurationResponse* trc__SetRecordingJobConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__GetRecordingJobConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__GetRecordingJobConfiguration(struct soap* soap, struct _trc__GetRecordingJobConfiguration* trc__GetRecordingJobConfiguration, struct _trc__GetRecordingJobConfigurationResponse* trc__GetRecordingJobConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__SetRecordingJobMode' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__SetRecordingJobMode(struct soap* soap, struct _trc__SetRecordingJobMode* trc__SetRecordingJobMode, struct _trc__SetRecordingJobModeResponse* trc__SetRecordingJobModeResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__GetRecordingJobState' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__GetRecordingJobState(struct soap* soap, struct _trc__GetRecordingJobState* trc__GetRecordingJobState, struct _trc__GetRecordingJobStateResponse* trc__GetRecordingJobStateResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__ExportRecordedData' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__ExportRecordedData(struct soap* soap, struct _trc__ExportRecordedData* trc__ExportRecordedData, struct _trc__ExportRecordedDataResponse* trc__ExportRecordedDataResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__StopExportRecordedData' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__StopExportRecordedData(struct soap* soap, struct _trc__StopExportRecordedData* trc__StopExportRecordedData, struct _trc__StopExportRecordedDataResponse* trc__StopExportRecordedDataResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

/** Web service operation '__trc__GetExportRecordedDataState' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __trc__GetExportRecordedDataState(struct soap* soap, struct _trc__GetExportRecordedDataState* trc__GetExportRecordedDataState, struct _trc__GetExportRecordedDataStateResponse* trc__GetExportRecordedDataStateResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
#endif //DISCOVERY_ONLY