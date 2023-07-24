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
#define STR_LEN_IP_ADDRESS 16
/******************************************************************************\
 *                                                                            *
 * Client-Side Call Stub Functions                                            *
 *                                                                            *
\******************************************************************************/

    
//     /** Web service synchronous operation 'soap_call___tds__GetServices' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetServices(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetServices *tds__GetServices, struct _tds__GetServicesResponse *tds__GetServicesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetServices' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetServices(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetServices *tds__GetServices){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetServices' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetServices(struct soap *soap, struct _tds__GetServicesResponse *tds__GetServicesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetServiceCapabilities' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetServiceCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetServiceCapabilities *tds__GetServiceCapabilities, struct _tds__GetServiceCapabilitiesResponse *tds__GetServiceCapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetServiceCapabilities' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetServiceCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetServiceCapabilities *tds__GetServiceCapabilities){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetServiceCapabilities' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetServiceCapabilities(struct soap *soap, struct _tds__GetServiceCapabilitiesResponse *tds__GetServiceCapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetDeviceInformation' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetDeviceInformation(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDeviceInformation *tds__GetDeviceInformation, struct _tds__GetDeviceInformationResponse *tds__GetDeviceInformationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetDeviceInformation' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetDeviceInformation(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDeviceInformation *tds__GetDeviceInformation){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetDeviceInformation' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetDeviceInformation(struct soap *soap, struct _tds__GetDeviceInformationResponse *tds__GetDeviceInformationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetSystemDateAndTime' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetSystemDateAndTime(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetSystemDateAndTime *tds__SetSystemDateAndTime, struct _tds__SetSystemDateAndTimeResponse *tds__SetSystemDateAndTimeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetSystemDateAndTime' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetSystemDateAndTime(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetSystemDateAndTime *tds__SetSystemDateAndTime){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetSystemDateAndTime' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetSystemDateAndTime(struct soap *soap, struct _tds__SetSystemDateAndTimeResponse *tds__SetSystemDateAndTimeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetSystemDateAndTime' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetSystemDateAndTime(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetSystemDateAndTime *tds__GetSystemDateAndTime, struct _tds__GetSystemDateAndTimeResponse *tds__GetSystemDateAndTimeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetSystemDateAndTime' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetSystemDateAndTime(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetSystemDateAndTime *tds__GetSystemDateAndTime){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetSystemDateAndTime' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetSystemDateAndTime(struct soap *soap, struct _tds__GetSystemDateAndTimeResponse *tds__GetSystemDateAndTimeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetSystemFactoryDefault' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetSystemFactoryDefault(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault, struct _tds__SetSystemFactoryDefaultResponse *tds__SetSystemFactoryDefaultResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetSystemFactoryDefault' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetSystemFactoryDefault(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetSystemFactoryDefault' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetSystemFactoryDefault(struct soap *soap, struct _tds__SetSystemFactoryDefaultResponse *tds__SetSystemFactoryDefaultResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__UpgradeSystemFirmware' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__UpgradeSystemFirmware(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__UpgradeSystemFirmware *tds__UpgradeSystemFirmware, struct _tds__UpgradeSystemFirmwareResponse *tds__UpgradeSystemFirmwareResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__UpgradeSystemFirmware' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__UpgradeSystemFirmware(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__UpgradeSystemFirmware *tds__UpgradeSystemFirmware){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__UpgradeSystemFirmware' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__UpgradeSystemFirmware(struct soap *soap, struct _tds__UpgradeSystemFirmwareResponse *tds__UpgradeSystemFirmwareResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SystemReboot' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SystemReboot(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SystemReboot *tds__SystemReboot, struct _tds__SystemRebootResponse *tds__SystemRebootResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SystemReboot' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SystemReboot(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SystemReboot *tds__SystemReboot){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SystemReboot' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SystemReboot(struct soap *soap, struct _tds__SystemRebootResponse *tds__SystemRebootResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__RestoreSystem' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__RestoreSystem(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__RestoreSystem *tds__RestoreSystem, struct _tds__RestoreSystemResponse *tds__RestoreSystemResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__RestoreSystem' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__RestoreSystem(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__RestoreSystem *tds__RestoreSystem){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__RestoreSystem' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__RestoreSystem(struct soap *soap, struct _tds__RestoreSystemResponse *tds__RestoreSystemResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetSystemBackup' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetSystemBackup(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetSystemBackup *tds__GetSystemBackup, struct _tds__GetSystemBackupResponse *tds__GetSystemBackupResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetSystemBackup' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetSystemBackup(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetSystemBackup *tds__GetSystemBackup){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetSystemBackup' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetSystemBackup(struct soap *soap, struct _tds__GetSystemBackupResponse *tds__GetSystemBackupResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetSystemLog' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetSystemLog(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetSystemLog *tds__GetSystemLog, struct _tds__GetSystemLogResponse *tds__GetSystemLogResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetSystemLog' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetSystemLog(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetSystemLog *tds__GetSystemLog){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetSystemLog' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetSystemLog(struct soap *soap, struct _tds__GetSystemLogResponse *tds__GetSystemLogResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetSystemSupportInformation' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetSystemSupportInformation(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetSystemSupportInformation *tds__GetSystemSupportInformation, struct _tds__GetSystemSupportInformationResponse *tds__GetSystemSupportInformationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetSystemSupportInformation' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetSystemSupportInformation(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetSystemSupportInformation *tds__GetSystemSupportInformation){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetSystemSupportInformation' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetSystemSupportInformation(struct soap *soap, struct _tds__GetSystemSupportInformationResponse *tds__GetSystemSupportInformationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetScopes' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetScopes(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetScopes *tds__GetScopes, struct _tds__GetScopesResponse *tds__GetScopesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetScopes' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetScopes(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetScopes *tds__GetScopes){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetScopes' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetScopes(struct soap *soap, struct _tds__GetScopesResponse *tds__GetScopesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetScopes' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetScopes(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetScopes *tds__SetScopes, struct _tds__SetScopesResponse *tds__SetScopesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetScopes' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetScopes(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetScopes *tds__SetScopes){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetScopes' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetScopes(struct soap *soap, struct _tds__SetScopesResponse *tds__SetScopesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__AddScopes' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__AddScopes(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__AddScopes *tds__AddScopes, struct _tds__AddScopesResponse *tds__AddScopesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__AddScopes' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__AddScopes(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__AddScopes *tds__AddScopes){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__AddScopes' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__AddScopes(struct soap *soap, struct _tds__AddScopesResponse *tds__AddScopesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__RemoveScopes' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__RemoveScopes(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__RemoveScopes *tds__RemoveScopes, struct _tds__RemoveScopesResponse *tds__RemoveScopesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__RemoveScopes' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__RemoveScopes(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__RemoveScopes *tds__RemoveScopes){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__RemoveScopes' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__RemoveScopes(struct soap *soap, struct _tds__RemoveScopesResponse *tds__RemoveScopesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetDiscoveryMode' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetDiscoveryMode(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDiscoveryMode *tds__GetDiscoveryMode, struct _tds__GetDiscoveryModeResponse *tds__GetDiscoveryModeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetDiscoveryMode' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetDiscoveryMode(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDiscoveryMode *tds__GetDiscoveryMode){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetDiscoveryMode' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetDiscoveryMode(struct soap *soap, struct _tds__GetDiscoveryModeResponse *tds__GetDiscoveryModeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetDiscoveryMode' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetDiscoveryMode(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetDiscoveryMode *tds__SetDiscoveryMode, struct _tds__SetDiscoveryModeResponse *tds__SetDiscoveryModeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetDiscoveryMode' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetDiscoveryMode(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetDiscoveryMode *tds__SetDiscoveryMode){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetDiscoveryMode' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetDiscoveryMode(struct soap *soap, struct _tds__SetDiscoveryModeResponse *tds__SetDiscoveryModeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetRemoteDiscoveryMode' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetRemoteDiscoveryMode(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetRemoteDiscoveryMode *tds__GetRemoteDiscoveryMode, struct _tds__GetRemoteDiscoveryModeResponse *tds__GetRemoteDiscoveryModeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetRemoteDiscoveryMode' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetRemoteDiscoveryMode(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetRemoteDiscoveryMode *tds__GetRemoteDiscoveryMode){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetRemoteDiscoveryMode' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetRemoteDiscoveryMode(struct soap *soap, struct _tds__GetRemoteDiscoveryModeResponse *tds__GetRemoteDiscoveryModeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetRemoteDiscoveryMode' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetRemoteDiscoveryMode(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetRemoteDiscoveryMode *tds__SetRemoteDiscoveryMode, struct _tds__SetRemoteDiscoveryModeResponse *tds__SetRemoteDiscoveryModeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetRemoteDiscoveryMode' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetRemoteDiscoveryMode(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetRemoteDiscoveryMode *tds__SetRemoteDiscoveryMode){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetRemoteDiscoveryMode' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetRemoteDiscoveryMode(struct soap *soap, struct _tds__SetRemoteDiscoveryModeResponse *tds__SetRemoteDiscoveryModeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetDPAddresses' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetDPAddresses(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDPAddresses *tds__GetDPAddresses, struct _tds__GetDPAddressesResponse *tds__GetDPAddressesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetDPAddresses' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetDPAddresses(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDPAddresses *tds__GetDPAddresses){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetDPAddresses' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetDPAddresses(struct soap *soap, struct _tds__GetDPAddressesResponse *tds__GetDPAddressesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetEndpointReference' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetEndpointReference(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetEndpointReference *tds__GetEndpointReference, struct _tds__GetEndpointReferenceResponse *tds__GetEndpointReferenceResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetEndpointReference' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetEndpointReference(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetEndpointReference *tds__GetEndpointReference){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetEndpointReference' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetEndpointReference(struct soap *soap, struct _tds__GetEndpointReferenceResponse *tds__GetEndpointReferenceResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetRemoteUser' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetRemoteUser(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetRemoteUser *tds__GetRemoteUser, struct _tds__GetRemoteUserResponse *tds__GetRemoteUserResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetRemoteUser' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetRemoteUser(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetRemoteUser *tds__GetRemoteUser){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetRemoteUser' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetRemoteUser(struct soap *soap, struct _tds__GetRemoteUserResponse *tds__GetRemoteUserResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetRemoteUser' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetRemoteUser(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetRemoteUser *tds__SetRemoteUser, struct _tds__SetRemoteUserResponse *tds__SetRemoteUserResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetRemoteUser' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetRemoteUser(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetRemoteUser *tds__SetRemoteUser){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetRemoteUser' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetRemoteUser(struct soap *soap, struct _tds__SetRemoteUserResponse *tds__SetRemoteUserResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetUsers' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetUsers(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetUsers *tds__GetUsers, struct _tds__GetUsersResponse *tds__GetUsersResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetUsers' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetUsers(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetUsers *tds__GetUsers){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetUsers' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetUsers(struct soap *soap, struct _tds__GetUsersResponse *tds__GetUsersResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__CreateUsers' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__CreateUsers(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__CreateUsers *tds__CreateUsers, struct _tds__CreateUsersResponse *tds__CreateUsersResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__CreateUsers' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__CreateUsers(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__CreateUsers *tds__CreateUsers){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__CreateUsers' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__CreateUsers(struct soap *soap, struct _tds__CreateUsersResponse *tds__CreateUsersResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__DeleteUsers' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__DeleteUsers(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__DeleteUsers *tds__DeleteUsers, struct _tds__DeleteUsersResponse *tds__DeleteUsersResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__DeleteUsers' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__DeleteUsers(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__DeleteUsers *tds__DeleteUsers){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__DeleteUsers' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__DeleteUsers(struct soap *soap, struct _tds__DeleteUsersResponse *tds__DeleteUsersResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetUser' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetUser(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetUser *tds__SetUser, struct _tds__SetUserResponse *tds__SetUserResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetUser' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetUser(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetUser *tds__SetUser){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetUser' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetUser(struct soap *soap, struct _tds__SetUserResponse *tds__SetUserResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetWsdlUrl' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetWsdlUrl(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetWsdlUrl *tds__GetWsdlUrl, struct _tds__GetWsdlUrlResponse *tds__GetWsdlUrlResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetWsdlUrl' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetWsdlUrl(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetWsdlUrl *tds__GetWsdlUrl){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetWsdlUrl' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetWsdlUrl(struct soap *soap, struct _tds__GetWsdlUrlResponse *tds__GetWsdlUrlResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetPasswordComplexityOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetPasswordComplexityOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetPasswordComplexityOptions *tds__GetPasswordComplexityOptions, struct _tds__GetPasswordComplexityOptionsResponse *tds__GetPasswordComplexityOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetPasswordComplexityOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetPasswordComplexityOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetPasswordComplexityOptions *tds__GetPasswordComplexityOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetPasswordComplexityOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetPasswordComplexityOptions(struct soap *soap, struct _tds__GetPasswordComplexityOptionsResponse *tds__GetPasswordComplexityOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetPasswordComplexityConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetPasswordComplexityConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetPasswordComplexityConfiguration *tds__GetPasswordComplexityConfiguration, struct _tds__GetPasswordComplexityConfigurationResponse *tds__GetPasswordComplexityConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetPasswordComplexityConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetPasswordComplexityConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetPasswordComplexityConfiguration *tds__GetPasswordComplexityConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetPasswordComplexityConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetPasswordComplexityConfiguration(struct soap *soap, struct _tds__GetPasswordComplexityConfigurationResponse *tds__GetPasswordComplexityConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetPasswordComplexityConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetPasswordComplexityConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetPasswordComplexityConfiguration *tds__SetPasswordComplexityConfiguration, struct _tds__SetPasswordComplexityConfigurationResponse *tds__SetPasswordComplexityConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetPasswordComplexityConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetPasswordComplexityConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetPasswordComplexityConfiguration *tds__SetPasswordComplexityConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetPasswordComplexityConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetPasswordComplexityConfiguration(struct soap *soap, struct _tds__SetPasswordComplexityConfigurationResponse *tds__SetPasswordComplexityConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetPasswordHistoryConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetPasswordHistoryConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetPasswordHistoryConfiguration *tds__GetPasswordHistoryConfiguration, struct _tds__GetPasswordHistoryConfigurationResponse *tds__GetPasswordHistoryConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetPasswordHistoryConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetPasswordHistoryConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetPasswordHistoryConfiguration *tds__GetPasswordHistoryConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetPasswordHistoryConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetPasswordHistoryConfiguration(struct soap *soap, struct _tds__GetPasswordHistoryConfigurationResponse *tds__GetPasswordHistoryConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetPasswordHistoryConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetPasswordHistoryConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetPasswordHistoryConfiguration *tds__SetPasswordHistoryConfiguration, struct _tds__SetPasswordHistoryConfigurationResponse *tds__SetPasswordHistoryConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetPasswordHistoryConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetPasswordHistoryConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetPasswordHistoryConfiguration *tds__SetPasswordHistoryConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetPasswordHistoryConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetPasswordHistoryConfiguration(struct soap *soap, struct _tds__SetPasswordHistoryConfigurationResponse *tds__SetPasswordHistoryConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetAuthFailureWarningOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetAuthFailureWarningOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetAuthFailureWarningOptions *tds__GetAuthFailureWarningOptions, struct _tds__GetAuthFailureWarningOptionsResponse *tds__GetAuthFailureWarningOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetAuthFailureWarningOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetAuthFailureWarningOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetAuthFailureWarningOptions *tds__GetAuthFailureWarningOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetAuthFailureWarningOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetAuthFailureWarningOptions(struct soap *soap, struct _tds__GetAuthFailureWarningOptionsResponse *tds__GetAuthFailureWarningOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetAuthFailureWarningConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetAuthFailureWarningConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetAuthFailureWarningConfiguration *tds__GetAuthFailureWarningConfiguration, struct _tds__GetAuthFailureWarningConfigurationResponse *tds__GetAuthFailureWarningConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetAuthFailureWarningConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetAuthFailureWarningConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetAuthFailureWarningConfiguration *tds__GetAuthFailureWarningConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetAuthFailureWarningConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetAuthFailureWarningConfiguration(struct soap *soap, struct _tds__GetAuthFailureWarningConfigurationResponse *tds__GetAuthFailureWarningConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetAuthFailureWarningConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetAuthFailureWarningConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetAuthFailureWarningConfiguration *tds__SetAuthFailureWarningConfiguration, struct _tds__SetAuthFailureWarningConfigurationResponse *tds__SetAuthFailureWarningConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetAuthFailureWarningConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetAuthFailureWarningConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetAuthFailureWarningConfiguration *tds__SetAuthFailureWarningConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetAuthFailureWarningConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetAuthFailureWarningConfiguration(struct soap *soap, struct _tds__SetAuthFailureWarningConfigurationResponse *tds__SetAuthFailureWarningConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetCapabilities' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetCapabilities *tds__GetCapabilities, struct _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetCapabilities' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetCapabilities *tds__GetCapabilities){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetCapabilities' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetCapabilities(struct soap *soap, struct _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetDPAddresses' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetDPAddresses(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetDPAddresses *tds__SetDPAddresses, struct _tds__SetDPAddressesResponse *tds__SetDPAddressesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetDPAddresses' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetDPAddresses(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetDPAddresses *tds__SetDPAddresses){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetDPAddresses' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetDPAddresses(struct soap *soap, struct _tds__SetDPAddressesResponse *tds__SetDPAddressesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetHostname' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetHostname(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetHostname *tds__GetHostname, struct _tds__GetHostnameResponse *tds__GetHostnameResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetHostname' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetHostname(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetHostname *tds__GetHostname){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetHostname' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetHostname(struct soap *soap, struct _tds__GetHostnameResponse *tds__GetHostnameResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetHostname' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetHostname(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetHostname *tds__SetHostname, struct _tds__SetHostnameResponse *tds__SetHostnameResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetHostname' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetHostname(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetHostname *tds__SetHostname){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetHostname' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetHostname(struct soap *soap, struct _tds__SetHostnameResponse *tds__SetHostnameResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetHostnameFromDHCP' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetHostnameFromDHCP(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP, struct _tds__SetHostnameFromDHCPResponse *tds__SetHostnameFromDHCPResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetHostnameFromDHCP' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetHostnameFromDHCP(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetHostnameFromDHCP' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetHostnameFromDHCP(struct soap *soap, struct _tds__SetHostnameFromDHCPResponse *tds__SetHostnameFromDHCPResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetDNS' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetDNS(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDNS *tds__GetDNS, struct _tds__GetDNSResponse *tds__GetDNSResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetDNS' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetDNS(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDNS *tds__GetDNS){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetDNS' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetDNS(struct soap *soap, struct _tds__GetDNSResponse *tds__GetDNSResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetDNS' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetDNS(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetDNS *tds__SetDNS, struct _tds__SetDNSResponse *tds__SetDNSResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetDNS' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetDNS(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetDNS *tds__SetDNS){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetDNS' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetDNS(struct soap *soap, struct _tds__SetDNSResponse *tds__SetDNSResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetNTP' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetNTP(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetNTP *tds__GetNTP, struct _tds__GetNTPResponse *tds__GetNTPResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetNTP' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetNTP(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetNTP *tds__GetNTP){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetNTP' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetNTP(struct soap *soap, struct _tds__GetNTPResponse *tds__GetNTPResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetNTP' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetNTP(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetNTP *tds__SetNTP, struct _tds__SetNTPResponse *tds__SetNTPResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetNTP' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetNTP(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetNTP *tds__SetNTP){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetNTP' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetNTP(struct soap *soap, struct _tds__SetNTPResponse *tds__SetNTPResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetDynamicDNS' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetDynamicDNS(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDynamicDNS *tds__GetDynamicDNS, struct _tds__GetDynamicDNSResponse *tds__GetDynamicDNSResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetDynamicDNS' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetDynamicDNS(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDynamicDNS *tds__GetDynamicDNS){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetDynamicDNS' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetDynamicDNS(struct soap *soap, struct _tds__GetDynamicDNSResponse *tds__GetDynamicDNSResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetDynamicDNS' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetDynamicDNS(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetDynamicDNS *tds__SetDynamicDNS, struct _tds__SetDynamicDNSResponse *tds__SetDynamicDNSResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetDynamicDNS' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetDynamicDNS(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetDynamicDNS *tds__SetDynamicDNS){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetDynamicDNS' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetDynamicDNS(struct soap *soap, struct _tds__SetDynamicDNSResponse *tds__SetDynamicDNSResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetNetworkInterfaces' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetNetworkInterfaces(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetNetworkInterfaces *tds__GetNetworkInterfaces, struct _tds__GetNetworkInterfacesResponse *tds__GetNetworkInterfacesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetNetworkInterfaces' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetNetworkInterfaces(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetNetworkInterfaces *tds__GetNetworkInterfaces){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetNetworkInterfaces' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetNetworkInterfaces(struct soap *soap, struct _tds__GetNetworkInterfacesResponse *tds__GetNetworkInterfacesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetNetworkInterfaces' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetNetworkInterfaces(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetNetworkInterfaces *tds__SetNetworkInterfaces, struct _tds__SetNetworkInterfacesResponse *tds__SetNetworkInterfacesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetNetworkInterfaces' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetNetworkInterfaces(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetNetworkInterfaces *tds__SetNetworkInterfaces){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetNetworkInterfaces' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetNetworkInterfaces(struct soap *soap, struct _tds__SetNetworkInterfacesResponse *tds__SetNetworkInterfacesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetNetworkProtocols' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetNetworkProtocols(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetNetworkProtocols *tds__GetNetworkProtocols, struct _tds__GetNetworkProtocolsResponse *tds__GetNetworkProtocolsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetNetworkProtocols' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetNetworkProtocols(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetNetworkProtocols *tds__GetNetworkProtocols){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetNetworkProtocols' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetNetworkProtocols(struct soap *soap, struct _tds__GetNetworkProtocolsResponse *tds__GetNetworkProtocolsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetNetworkProtocols' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetNetworkProtocols(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetNetworkProtocols *tds__SetNetworkProtocols, struct _tds__SetNetworkProtocolsResponse *tds__SetNetworkProtocolsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetNetworkProtocols' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetNetworkProtocols(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetNetworkProtocols *tds__SetNetworkProtocols){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetNetworkProtocols' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetNetworkProtocols(struct soap *soap, struct _tds__SetNetworkProtocolsResponse *tds__SetNetworkProtocolsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetNetworkDefaultGateway' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetNetworkDefaultGateway(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway, struct _tds__GetNetworkDefaultGatewayResponse *tds__GetNetworkDefaultGatewayResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetNetworkDefaultGateway' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetNetworkDefaultGateway(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetNetworkDefaultGateway' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetNetworkDefaultGateway(struct soap *soap, struct _tds__GetNetworkDefaultGatewayResponse *tds__GetNetworkDefaultGatewayResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetNetworkDefaultGateway' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetNetworkDefaultGateway(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway, struct _tds__SetNetworkDefaultGatewayResponse *tds__SetNetworkDefaultGatewayResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetNetworkDefaultGateway' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetNetworkDefaultGateway(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetNetworkDefaultGateway' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetNetworkDefaultGateway(struct soap *soap, struct _tds__SetNetworkDefaultGatewayResponse *tds__SetNetworkDefaultGatewayResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetZeroConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetZeroConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetZeroConfiguration *tds__GetZeroConfiguration, struct _tds__GetZeroConfigurationResponse *tds__GetZeroConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetZeroConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetZeroConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetZeroConfiguration *tds__GetZeroConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetZeroConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetZeroConfiguration(struct soap *soap, struct _tds__GetZeroConfigurationResponse *tds__GetZeroConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetZeroConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetZeroConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetZeroConfiguration *tds__SetZeroConfiguration, struct _tds__SetZeroConfigurationResponse *tds__SetZeroConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetZeroConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetZeroConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetZeroConfiguration *tds__SetZeroConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetZeroConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetZeroConfiguration(struct soap *soap, struct _tds__SetZeroConfigurationResponse *tds__SetZeroConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetIPAddressFilter' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetIPAddressFilter(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetIPAddressFilter *tds__GetIPAddressFilter, struct _tds__GetIPAddressFilterResponse *tds__GetIPAddressFilterResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetIPAddressFilter' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetIPAddressFilter(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetIPAddressFilter *tds__GetIPAddressFilter){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetIPAddressFilter' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetIPAddressFilter(struct soap *soap, struct _tds__GetIPAddressFilterResponse *tds__GetIPAddressFilterResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetIPAddressFilter' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetIPAddressFilter(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetIPAddressFilter *tds__SetIPAddressFilter, struct _tds__SetIPAddressFilterResponse *tds__SetIPAddressFilterResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetIPAddressFilter' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetIPAddressFilter(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetIPAddressFilter *tds__SetIPAddressFilter){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetIPAddressFilter' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetIPAddressFilter(struct soap *soap, struct _tds__SetIPAddressFilterResponse *tds__SetIPAddressFilterResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__AddIPAddressFilter' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__AddIPAddressFilter(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__AddIPAddressFilter *tds__AddIPAddressFilter, struct _tds__AddIPAddressFilterResponse *tds__AddIPAddressFilterResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__AddIPAddressFilter' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__AddIPAddressFilter(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__AddIPAddressFilter *tds__AddIPAddressFilter){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__AddIPAddressFilter' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__AddIPAddressFilter(struct soap *soap, struct _tds__AddIPAddressFilterResponse *tds__AddIPAddressFilterResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__RemoveIPAddressFilter' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__RemoveIPAddressFilter(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter, struct _tds__RemoveIPAddressFilterResponse *tds__RemoveIPAddressFilterResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__RemoveIPAddressFilter' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__RemoveIPAddressFilter(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__RemoveIPAddressFilter' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__RemoveIPAddressFilter(struct soap *soap, struct _tds__RemoveIPAddressFilterResponse *tds__RemoveIPAddressFilterResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetAccessPolicy' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetAccessPolicy(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetAccessPolicy *tds__GetAccessPolicy, struct _tds__GetAccessPolicyResponse *tds__GetAccessPolicyResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetAccessPolicy' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetAccessPolicy(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetAccessPolicy *tds__GetAccessPolicy){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetAccessPolicy' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetAccessPolicy(struct soap *soap, struct _tds__GetAccessPolicyResponse *tds__GetAccessPolicyResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetAccessPolicy' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetAccessPolicy(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetAccessPolicy *tds__SetAccessPolicy, struct _tds__SetAccessPolicyResponse *tds__SetAccessPolicyResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetAccessPolicy' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetAccessPolicy(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetAccessPolicy *tds__SetAccessPolicy){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetAccessPolicy' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetAccessPolicy(struct soap *soap, struct _tds__SetAccessPolicyResponse *tds__SetAccessPolicyResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__CreateCertificate' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__CreateCertificate(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__CreateCertificate *tds__CreateCertificate, struct _tds__CreateCertificateResponse *tds__CreateCertificateResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__CreateCertificate' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__CreateCertificate(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__CreateCertificate *tds__CreateCertificate){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__CreateCertificate' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__CreateCertificate(struct soap *soap, struct _tds__CreateCertificateResponse *tds__CreateCertificateResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetCertificates' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetCertificates(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetCertificates *tds__GetCertificates, struct _tds__GetCertificatesResponse *tds__GetCertificatesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetCertificates' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetCertificates(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetCertificates *tds__GetCertificates){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetCertificates' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetCertificates(struct soap *soap, struct _tds__GetCertificatesResponse *tds__GetCertificatesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetCertificatesStatus' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetCertificatesStatus(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetCertificatesStatus *tds__GetCertificatesStatus, struct _tds__GetCertificatesStatusResponse *tds__GetCertificatesStatusResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetCertificatesStatus' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetCertificatesStatus(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetCertificatesStatus *tds__GetCertificatesStatus){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetCertificatesStatus' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetCertificatesStatus(struct soap *soap, struct _tds__GetCertificatesStatusResponse *tds__GetCertificatesStatusResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetCertificatesStatus' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetCertificatesStatus(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetCertificatesStatus *tds__SetCertificatesStatus, struct _tds__SetCertificatesStatusResponse *tds__SetCertificatesStatusResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetCertificatesStatus' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetCertificatesStatus(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetCertificatesStatus *tds__SetCertificatesStatus){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetCertificatesStatus' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetCertificatesStatus(struct soap *soap, struct _tds__SetCertificatesStatusResponse *tds__SetCertificatesStatusResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__DeleteCertificates' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__DeleteCertificates(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__DeleteCertificates *tds__DeleteCertificates, struct _tds__DeleteCertificatesResponse *tds__DeleteCertificatesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__DeleteCertificates' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__DeleteCertificates(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__DeleteCertificates *tds__DeleteCertificates){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__DeleteCertificates' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__DeleteCertificates(struct soap *soap, struct _tds__DeleteCertificatesResponse *tds__DeleteCertificatesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetPkcs10Request' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetPkcs10Request(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetPkcs10Request *tds__GetPkcs10Request, struct _tds__GetPkcs10RequestResponse *tds__GetPkcs10RequestResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetPkcs10Request' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetPkcs10Request(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetPkcs10Request *tds__GetPkcs10Request){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetPkcs10Request' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetPkcs10Request(struct soap *soap, struct _tds__GetPkcs10RequestResponse *tds__GetPkcs10RequestResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__LoadCertificates' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__LoadCertificates(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__LoadCertificates *tds__LoadCertificates, struct _tds__LoadCertificatesResponse *tds__LoadCertificatesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__LoadCertificates' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__LoadCertificates(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__LoadCertificates *tds__LoadCertificates){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__LoadCertificates' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__LoadCertificates(struct soap *soap, struct _tds__LoadCertificatesResponse *tds__LoadCertificatesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetClientCertificateMode' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetClientCertificateMode(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetClientCertificateMode *tds__GetClientCertificateMode, struct _tds__GetClientCertificateModeResponse *tds__GetClientCertificateModeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetClientCertificateMode' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetClientCertificateMode(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetClientCertificateMode *tds__GetClientCertificateMode){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetClientCertificateMode' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetClientCertificateMode(struct soap *soap, struct _tds__GetClientCertificateModeResponse *tds__GetClientCertificateModeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetClientCertificateMode' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetClientCertificateMode(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetClientCertificateMode *tds__SetClientCertificateMode, struct _tds__SetClientCertificateModeResponse *tds__SetClientCertificateModeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetClientCertificateMode' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetClientCertificateMode(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetClientCertificateMode *tds__SetClientCertificateMode){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetClientCertificateMode' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetClientCertificateMode(struct soap *soap, struct _tds__SetClientCertificateModeResponse *tds__SetClientCertificateModeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetRelayOutputs' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetRelayOutputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetRelayOutputs *tds__GetRelayOutputs, struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetRelayOutputs' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetRelayOutputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetRelayOutputs *tds__GetRelayOutputs){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetRelayOutputs' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetRelayOutputs(struct soap *soap, struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetRelayOutputSettings' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetRelayOutputSettings(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetRelayOutputSettings *tds__SetRelayOutputSettings, struct _tds__SetRelayOutputSettingsResponse *tds__SetRelayOutputSettingsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetRelayOutputSettings' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetRelayOutputSettings(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetRelayOutputSettings *tds__SetRelayOutputSettings){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetRelayOutputSettings' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetRelayOutputSettings(struct soap *soap, struct _tds__SetRelayOutputSettingsResponse *tds__SetRelayOutputSettingsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetRelayOutputState' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetRelayOutputState(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetRelayOutputState *tds__SetRelayOutputState, struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetRelayOutputState' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetRelayOutputState(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetRelayOutputState *tds__SetRelayOutputState){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetRelayOutputState' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetRelayOutputState(struct soap *soap, struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SendAuxiliaryCommand' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SendAuxiliaryCommand(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand, struct _tds__SendAuxiliaryCommandResponse *tds__SendAuxiliaryCommandResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SendAuxiliaryCommand' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SendAuxiliaryCommand(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SendAuxiliaryCommand' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SendAuxiliaryCommand(struct soap *soap, struct _tds__SendAuxiliaryCommandResponse *tds__SendAuxiliaryCommandResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetCACertificates' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetCACertificates(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetCACertificates *tds__GetCACertificates, struct _tds__GetCACertificatesResponse *tds__GetCACertificatesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetCACertificates' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetCACertificates(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetCACertificates *tds__GetCACertificates){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetCACertificates' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetCACertificates(struct soap *soap, struct _tds__GetCACertificatesResponse *tds__GetCACertificatesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__LoadCertificateWithPrivateKey' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__LoadCertificateWithPrivateKey(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__LoadCertificateWithPrivateKey *tds__LoadCertificateWithPrivateKey, struct _tds__LoadCertificateWithPrivateKeyResponse *tds__LoadCertificateWithPrivateKeyResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__LoadCertificateWithPrivateKey' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__LoadCertificateWithPrivateKey(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__LoadCertificateWithPrivateKey *tds__LoadCertificateWithPrivateKey){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__LoadCertificateWithPrivateKey' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__LoadCertificateWithPrivateKey(struct soap *soap, struct _tds__LoadCertificateWithPrivateKeyResponse *tds__LoadCertificateWithPrivateKeyResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetCertificateInformation' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetCertificateInformation(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetCertificateInformation *tds__GetCertificateInformation, struct _tds__GetCertificateInformationResponse *tds__GetCertificateInformationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetCertificateInformation' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetCertificateInformation(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetCertificateInformation *tds__GetCertificateInformation){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetCertificateInformation' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetCertificateInformation(struct soap *soap, struct _tds__GetCertificateInformationResponse *tds__GetCertificateInformationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__LoadCACertificates' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__LoadCACertificates(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__LoadCACertificates *tds__LoadCACertificates, struct _tds__LoadCACertificatesResponse *tds__LoadCACertificatesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__LoadCACertificates' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__LoadCACertificates(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__LoadCACertificates *tds__LoadCACertificates){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__LoadCACertificates' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__LoadCACertificates(struct soap *soap, struct _tds__LoadCACertificatesResponse *tds__LoadCACertificatesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__CreateDot1XConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__CreateDot1XConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__CreateDot1XConfiguration *tds__CreateDot1XConfiguration, struct _tds__CreateDot1XConfigurationResponse *tds__CreateDot1XConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__CreateDot1XConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__CreateDot1XConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__CreateDot1XConfiguration *tds__CreateDot1XConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__CreateDot1XConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__CreateDot1XConfiguration(struct soap *soap, struct _tds__CreateDot1XConfigurationResponse *tds__CreateDot1XConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetDot1XConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetDot1XConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetDot1XConfiguration *tds__SetDot1XConfiguration, struct _tds__SetDot1XConfigurationResponse *tds__SetDot1XConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetDot1XConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetDot1XConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetDot1XConfiguration *tds__SetDot1XConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetDot1XConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetDot1XConfiguration(struct soap *soap, struct _tds__SetDot1XConfigurationResponse *tds__SetDot1XConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetDot1XConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetDot1XConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDot1XConfiguration *tds__GetDot1XConfiguration, struct _tds__GetDot1XConfigurationResponse *tds__GetDot1XConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetDot1XConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetDot1XConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDot1XConfiguration *tds__GetDot1XConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetDot1XConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetDot1XConfiguration(struct soap *soap, struct _tds__GetDot1XConfigurationResponse *tds__GetDot1XConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetDot1XConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetDot1XConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDot1XConfigurations *tds__GetDot1XConfigurations, struct _tds__GetDot1XConfigurationsResponse *tds__GetDot1XConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetDot1XConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetDot1XConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDot1XConfigurations *tds__GetDot1XConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetDot1XConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetDot1XConfigurations(struct soap *soap, struct _tds__GetDot1XConfigurationsResponse *tds__GetDot1XConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__DeleteDot1XConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__DeleteDot1XConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__DeleteDot1XConfiguration *tds__DeleteDot1XConfiguration, struct _tds__DeleteDot1XConfigurationResponse *tds__DeleteDot1XConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__DeleteDot1XConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__DeleteDot1XConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__DeleteDot1XConfiguration *tds__DeleteDot1XConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__DeleteDot1XConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__DeleteDot1XConfiguration(struct soap *soap, struct _tds__DeleteDot1XConfigurationResponse *tds__DeleteDot1XConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetDot11Capabilities' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetDot11Capabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDot11Capabilities *tds__GetDot11Capabilities, struct _tds__GetDot11CapabilitiesResponse *tds__GetDot11CapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetDot11Capabilities' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetDot11Capabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDot11Capabilities *tds__GetDot11Capabilities){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetDot11Capabilities' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetDot11Capabilities(struct soap *soap, struct _tds__GetDot11CapabilitiesResponse *tds__GetDot11CapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetDot11Status' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetDot11Status(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDot11Status *tds__GetDot11Status, struct _tds__GetDot11StatusResponse *tds__GetDot11StatusResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetDot11Status' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetDot11Status(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDot11Status *tds__GetDot11Status){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetDot11Status' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetDot11Status(struct soap *soap, struct _tds__GetDot11StatusResponse *tds__GetDot11StatusResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__ScanAvailableDot11Networks' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__ScanAvailableDot11Networks(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__ScanAvailableDot11Networks *tds__ScanAvailableDot11Networks, struct _tds__ScanAvailableDot11NetworksResponse *tds__ScanAvailableDot11NetworksResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__ScanAvailableDot11Networks' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__ScanAvailableDot11Networks(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__ScanAvailableDot11Networks *tds__ScanAvailableDot11Networks){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__ScanAvailableDot11Networks' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__ScanAvailableDot11Networks(struct soap *soap, struct _tds__ScanAvailableDot11NetworksResponse *tds__ScanAvailableDot11NetworksResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetSystemUris' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetSystemUris(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetSystemUris *tds__GetSystemUris, struct _tds__GetSystemUrisResponse *tds__GetSystemUrisResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetSystemUris' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetSystemUris(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetSystemUris *tds__GetSystemUris){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetSystemUris' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetSystemUris(struct soap *soap, struct _tds__GetSystemUrisResponse *tds__GetSystemUrisResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__StartFirmwareUpgrade' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__StartFirmwareUpgrade(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__StartFirmwareUpgrade *tds__StartFirmwareUpgrade, struct _tds__StartFirmwareUpgradeResponse *tds__StartFirmwareUpgradeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__StartFirmwareUpgrade' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__StartFirmwareUpgrade(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__StartFirmwareUpgrade *tds__StartFirmwareUpgrade){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__StartFirmwareUpgrade' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__StartFirmwareUpgrade(struct soap *soap, struct _tds__StartFirmwareUpgradeResponse *tds__StartFirmwareUpgradeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__StartSystemRestore' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__StartSystemRestore(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__StartSystemRestore *tds__StartSystemRestore, struct _tds__StartSystemRestoreResponse *tds__StartSystemRestoreResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__StartSystemRestore' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__StartSystemRestore(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__StartSystemRestore *tds__StartSystemRestore){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__StartSystemRestore' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__StartSystemRestore(struct soap *soap, struct _tds__StartSystemRestoreResponse *tds__StartSystemRestoreResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetStorageConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetStorageConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetStorageConfigurations *tds__GetStorageConfigurations, struct _tds__GetStorageConfigurationsResponse *tds__GetStorageConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetStorageConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetStorageConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetStorageConfigurations *tds__GetStorageConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetStorageConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetStorageConfigurations(struct soap *soap, struct _tds__GetStorageConfigurationsResponse *tds__GetStorageConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__CreateStorageConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__CreateStorageConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__CreateStorageConfiguration *tds__CreateStorageConfiguration, struct _tds__CreateStorageConfigurationResponse *tds__CreateStorageConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__CreateStorageConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__CreateStorageConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__CreateStorageConfiguration *tds__CreateStorageConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__CreateStorageConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__CreateStorageConfiguration(struct soap *soap, struct _tds__CreateStorageConfigurationResponse *tds__CreateStorageConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetStorageConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetStorageConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetStorageConfiguration *tds__GetStorageConfiguration, struct _tds__GetStorageConfigurationResponse *tds__GetStorageConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetStorageConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetStorageConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetStorageConfiguration *tds__GetStorageConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetStorageConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetStorageConfiguration(struct soap *soap, struct _tds__GetStorageConfigurationResponse *tds__GetStorageConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetStorageConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetStorageConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetStorageConfiguration *tds__SetStorageConfiguration, struct _tds__SetStorageConfigurationResponse *tds__SetStorageConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetStorageConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetStorageConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetStorageConfiguration *tds__SetStorageConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetStorageConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetStorageConfiguration(struct soap *soap, struct _tds__SetStorageConfigurationResponse *tds__SetStorageConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__DeleteStorageConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__DeleteStorageConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__DeleteStorageConfiguration *tds__DeleteStorageConfiguration, struct _tds__DeleteStorageConfigurationResponse *tds__DeleteStorageConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__DeleteStorageConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__DeleteStorageConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__DeleteStorageConfiguration *tds__DeleteStorageConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__DeleteStorageConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__DeleteStorageConfiguration(struct soap *soap, struct _tds__DeleteStorageConfigurationResponse *tds__DeleteStorageConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__GetGeoLocation' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetGeoLocation(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetGeoLocation *tds__GetGeoLocation, struct _tds__GetGeoLocationResponse *tds__GetGeoLocationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__GetGeoLocation' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__GetGeoLocation(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetGeoLocation *tds__GetGeoLocation){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__GetGeoLocation' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__GetGeoLocation(struct soap *soap, struct _tds__GetGeoLocationResponse *tds__GetGeoLocationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetGeoLocation' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetGeoLocation(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetGeoLocation *tds__SetGeoLocation, struct _tds__SetGeoLocationResponse *tds__SetGeoLocationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetGeoLocation' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetGeoLocation(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetGeoLocation *tds__SetGeoLocation){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetGeoLocation' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetGeoLocation(struct soap *soap, struct _tds__SetGeoLocationResponse *tds__SetGeoLocationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__DeleteGeoLocation' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__DeleteGeoLocation(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__DeleteGeoLocation *tds__DeleteGeoLocation, struct _tds__DeleteGeoLocationResponse *tds__DeleteGeoLocationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__DeleteGeoLocation' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__DeleteGeoLocation(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__DeleteGeoLocation *tds__DeleteGeoLocation){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__DeleteGeoLocation' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__DeleteGeoLocation(struct soap *soap, struct _tds__DeleteGeoLocationResponse *tds__DeleteGeoLocationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tds__SetHashingAlgorithm' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetHashingAlgorithm(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetHashingAlgorithm *tds__SetHashingAlgorithm, struct _tds__SetHashingAlgorithmResponse *tds__SetHashingAlgorithmResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tds__SetHashingAlgorithm' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tds__SetHashingAlgorithm(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetHashingAlgorithm *tds__SetHashingAlgorithm){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tds__SetHashingAlgorithm' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tds__SetHashingAlgorithm(struct soap *soap, struct _tds__SetHashingAlgorithmResponse *tds__SetHashingAlgorithmResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetServiceCapabilities' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetServiceCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetServiceCapabilities *tmd__GetServiceCapabilities, struct _tmd__GetServiceCapabilitiesResponse *tmd__GetServiceCapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetServiceCapabilities' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetServiceCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetServiceCapabilities *tmd__GetServiceCapabilities){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetServiceCapabilities' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetServiceCapabilities(struct soap *soap, struct _tmd__GetServiceCapabilitiesResponse *tmd__GetServiceCapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetRelayOutputOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetRelayOutputOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetRelayOutputOptions *tmd__GetRelayOutputOptions, struct _tmd__GetRelayOutputOptionsResponse *tmd__GetRelayOutputOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetRelayOutputOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetRelayOutputOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetRelayOutputOptions *tmd__GetRelayOutputOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetRelayOutputOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetRelayOutputOptions(struct soap *soap, struct _tmd__GetRelayOutputOptionsResponse *tmd__GetRelayOutputOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetAudioSources' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetAudioSources(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct tmd__Get *tmd__GetAudioSources, struct tmd__GetResponse *tmd__GetAudioSourcesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetAudioSources' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetAudioSources(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct tmd__Get *tmd__GetAudioSources){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetAudioSources' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetAudioSources(struct soap *soap, struct tmd__GetResponse *tmd__GetAudioSourcesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetAudioOutputs' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetAudioOutputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct tmd__Get *tmd__GetAudioOutputs, struct tmd__GetResponse *tmd__GetAudioOutputsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetAudioOutputs' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetAudioOutputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct tmd__Get *tmd__GetAudioOutputs){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetAudioOutputs' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetAudioOutputs(struct soap *soap, struct tmd__GetResponse *tmd__GetAudioOutputsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetVideoSources' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetVideoSources(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct tmd__Get *tmd__GetVideoSources, struct tmd__GetResponse *tmd__GetVideoSourcesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetVideoSources' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetVideoSources(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct tmd__Get *tmd__GetVideoSources){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetVideoSources' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetVideoSources(struct soap *soap, struct tmd__GetResponse *tmd__GetVideoSourcesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetVideoOutputs' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetVideoOutputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetVideoOutputs *tmd__GetVideoOutputs, struct _tmd__GetVideoOutputsResponse *tmd__GetVideoOutputsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetVideoOutputs' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetVideoOutputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetVideoOutputs *tmd__GetVideoOutputs){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetVideoOutputs' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetVideoOutputs(struct soap *soap, struct _tmd__GetVideoOutputsResponse *tmd__GetVideoOutputsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetVideoSourceConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetVideoSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetVideoSourceConfiguration *tmd__GetVideoSourceConfiguration, struct _tmd__GetVideoSourceConfigurationResponse *tmd__GetVideoSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetVideoSourceConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetVideoSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetVideoSourceConfiguration *tmd__GetVideoSourceConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetVideoSourceConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetVideoSourceConfiguration(struct soap *soap, struct _tmd__GetVideoSourceConfigurationResponse *tmd__GetVideoSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetVideoOutputConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetVideoOutputConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetVideoOutputConfiguration *tmd__GetVideoOutputConfiguration, struct _tmd__GetVideoOutputConfigurationResponse *tmd__GetVideoOutputConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetVideoOutputConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetVideoOutputConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetVideoOutputConfiguration *tmd__GetVideoOutputConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetVideoOutputConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetVideoOutputConfiguration(struct soap *soap, struct _tmd__GetVideoOutputConfigurationResponse *tmd__GetVideoOutputConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetAudioSourceConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetAudioSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetAudioSourceConfiguration *tmd__GetAudioSourceConfiguration, struct _tmd__GetAudioSourceConfigurationResponse *tmd__GetAudioSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetAudioSourceConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetAudioSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetAudioSourceConfiguration *tmd__GetAudioSourceConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetAudioSourceConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetAudioSourceConfiguration(struct soap *soap, struct _tmd__GetAudioSourceConfigurationResponse *tmd__GetAudioSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetAudioOutputConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetAudioOutputConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetAudioOutputConfiguration *tmd__GetAudioOutputConfiguration, struct _tmd__GetAudioOutputConfigurationResponse *tmd__GetAudioOutputConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetAudioOutputConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetAudioOutputConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetAudioOutputConfiguration *tmd__GetAudioOutputConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetAudioOutputConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetAudioOutputConfiguration(struct soap *soap, struct _tmd__GetAudioOutputConfigurationResponse *tmd__GetAudioOutputConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__SetVideoSourceConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__SetVideoSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__SetVideoSourceConfiguration *tmd__SetVideoSourceConfiguration, struct _tmd__SetVideoSourceConfigurationResponse *tmd__SetVideoSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__SetVideoSourceConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__SetVideoSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__SetVideoSourceConfiguration *tmd__SetVideoSourceConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__SetVideoSourceConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__SetVideoSourceConfiguration(struct soap *soap, struct _tmd__SetVideoSourceConfigurationResponse *tmd__SetVideoSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__SetVideoOutputConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__SetVideoOutputConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__SetVideoOutputConfiguration *tmd__SetVideoOutputConfiguration, struct _tmd__SetVideoOutputConfigurationResponse *tmd__SetVideoOutputConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__SetVideoOutputConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__SetVideoOutputConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__SetVideoOutputConfiguration *tmd__SetVideoOutputConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__SetVideoOutputConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__SetVideoOutputConfiguration(struct soap *soap, struct _tmd__SetVideoOutputConfigurationResponse *tmd__SetVideoOutputConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__SetAudioSourceConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__SetAudioSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__SetAudioSourceConfiguration *tmd__SetAudioSourceConfiguration, struct _tmd__SetAudioSourceConfigurationResponse *tmd__SetAudioSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__SetAudioSourceConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__SetAudioSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__SetAudioSourceConfiguration *tmd__SetAudioSourceConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__SetAudioSourceConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__SetAudioSourceConfiguration(struct soap *soap, struct _tmd__SetAudioSourceConfigurationResponse *tmd__SetAudioSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__SetAudioOutputConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__SetAudioOutputConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__SetAudioOutputConfiguration *tmd__SetAudioOutputConfiguration, struct _tmd__SetAudioOutputConfigurationResponse *tmd__SetAudioOutputConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__SetAudioOutputConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__SetAudioOutputConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__SetAudioOutputConfiguration *tmd__SetAudioOutputConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__SetAudioOutputConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__SetAudioOutputConfiguration(struct soap *soap, struct _tmd__SetAudioOutputConfigurationResponse *tmd__SetAudioOutputConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetVideoSourceConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetVideoSourceConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetVideoSourceConfigurationOptions *tmd__GetVideoSourceConfigurationOptions, struct _tmd__GetVideoSourceConfigurationOptionsResponse *tmd__GetVideoSourceConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetVideoSourceConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetVideoSourceConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetVideoSourceConfigurationOptions *tmd__GetVideoSourceConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetVideoSourceConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetVideoSourceConfigurationOptions(struct soap *soap, struct _tmd__GetVideoSourceConfigurationOptionsResponse *tmd__GetVideoSourceConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetVideoOutputConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetVideoOutputConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetVideoOutputConfigurationOptions *tmd__GetVideoOutputConfigurationOptions, struct _tmd__GetVideoOutputConfigurationOptionsResponse *tmd__GetVideoOutputConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetVideoOutputConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetVideoOutputConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetVideoOutputConfigurationOptions *tmd__GetVideoOutputConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetVideoOutputConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetVideoOutputConfigurationOptions(struct soap *soap, struct _tmd__GetVideoOutputConfigurationOptionsResponse *tmd__GetVideoOutputConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetAudioSourceConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetAudioSourceConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetAudioSourceConfigurationOptions *tmd__GetAudioSourceConfigurationOptions, struct _tmd__GetAudioSourceConfigurationOptionsResponse *tmd__GetAudioSourceConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetAudioSourceConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetAudioSourceConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetAudioSourceConfigurationOptions *tmd__GetAudioSourceConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetAudioSourceConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetAudioSourceConfigurationOptions(struct soap *soap, struct _tmd__GetAudioSourceConfigurationOptionsResponse *tmd__GetAudioSourceConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetAudioOutputConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetAudioOutputConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetAudioOutputConfigurationOptions *tmd__GetAudioOutputConfigurationOptions, struct _tmd__GetAudioOutputConfigurationOptionsResponse *tmd__GetAudioOutputConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetAudioOutputConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetAudioOutputConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetAudioOutputConfigurationOptions *tmd__GetAudioOutputConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetAudioOutputConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetAudioOutputConfigurationOptions(struct soap *soap, struct _tmd__GetAudioOutputConfigurationOptionsResponse *tmd__GetAudioOutputConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetRelayOutputs' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetRelayOutputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetRelayOutputs *tds__GetRelayOutputs, struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetRelayOutputs' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetRelayOutputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetRelayOutputs *tds__GetRelayOutputs){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetRelayOutputs' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetRelayOutputs(struct soap *soap, struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__SetRelayOutputSettings' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__SetRelayOutputSettings(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__SetRelayOutputSettings *tmd__SetRelayOutputSettings, struct _tmd__SetRelayOutputSettingsResponse *tmd__SetRelayOutputSettingsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__SetRelayOutputSettings' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__SetRelayOutputSettings(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__SetRelayOutputSettings *tmd__SetRelayOutputSettings){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__SetRelayOutputSettings' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__SetRelayOutputSettings(struct soap *soap, struct _tmd__SetRelayOutputSettingsResponse *tmd__SetRelayOutputSettingsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__SetRelayOutputState' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__SetRelayOutputState(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetRelayOutputState *tds__SetRelayOutputState, struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__SetRelayOutputState' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__SetRelayOutputState(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetRelayOutputState *tds__SetRelayOutputState){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__SetRelayOutputState' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__SetRelayOutputState(struct soap *soap, struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetDigitalInputs' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetDigitalInputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetDigitalInputs *tmd__GetDigitalInputs, struct _tmd__GetDigitalInputsResponse *tmd__GetDigitalInputsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetDigitalInputs' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetDigitalInputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetDigitalInputs *tmd__GetDigitalInputs){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetDigitalInputs' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetDigitalInputs(struct soap *soap, struct _tmd__GetDigitalInputsResponse *tmd__GetDigitalInputsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetDigitalInputConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetDigitalInputConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetDigitalInputConfigurationOptions *tmd__GetDigitalInputConfigurationOptions, struct _tmd__GetDigitalInputConfigurationOptionsResponse *tmd__GetDigitalInputConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetDigitalInputConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetDigitalInputConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetDigitalInputConfigurationOptions *tmd__GetDigitalInputConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetDigitalInputConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetDigitalInputConfigurationOptions(struct soap *soap, struct _tmd__GetDigitalInputConfigurationOptionsResponse *tmd__GetDigitalInputConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__SetDigitalInputConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__SetDigitalInputConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__SetDigitalInputConfigurations *tmd__SetDigitalInputConfigurations, struct _tmd__SetDigitalInputConfigurationsResponse *tmd__SetDigitalInputConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__SetDigitalInputConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__SetDigitalInputConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__SetDigitalInputConfigurations *tmd__SetDigitalInputConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__SetDigitalInputConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__SetDigitalInputConfigurations(struct soap *soap, struct _tmd__SetDigitalInputConfigurationsResponse *tmd__SetDigitalInputConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetSerialPorts' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetSerialPorts(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetSerialPorts *tmd__GetSerialPorts, struct _tmd__GetSerialPortsResponse *tmd__GetSerialPortsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetSerialPorts' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetSerialPorts(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetSerialPorts *tmd__GetSerialPorts){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetSerialPorts' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetSerialPorts(struct soap *soap, struct _tmd__GetSerialPortsResponse *tmd__GetSerialPortsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetSerialPortConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetSerialPortConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetSerialPortConfiguration *tmd__GetSerialPortConfiguration, struct _tmd__GetSerialPortConfigurationResponse *tmd__GetSerialPortConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetSerialPortConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetSerialPortConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetSerialPortConfiguration *tmd__GetSerialPortConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetSerialPortConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetSerialPortConfiguration(struct soap *soap, struct _tmd__GetSerialPortConfigurationResponse *tmd__GetSerialPortConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__SetSerialPortConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__SetSerialPortConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__SetSerialPortConfiguration *tmd__SetSerialPortConfiguration, struct _tmd__SetSerialPortConfigurationResponse *tmd__SetSerialPortConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__SetSerialPortConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__SetSerialPortConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__SetSerialPortConfiguration *tmd__SetSerialPortConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__SetSerialPortConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__SetSerialPortConfiguration(struct soap *soap, struct _tmd__SetSerialPortConfigurationResponse *tmd__SetSerialPortConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__GetSerialPortConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__GetSerialPortConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetSerialPortConfigurationOptions *tmd__GetSerialPortConfigurationOptions, struct _tmd__GetSerialPortConfigurationOptionsResponse *tmd__GetSerialPortConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__GetSerialPortConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__GetSerialPortConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__GetSerialPortConfigurationOptions *tmd__GetSerialPortConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__GetSerialPortConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__GetSerialPortConfigurationOptions(struct soap *soap, struct _tmd__GetSerialPortConfigurationOptionsResponse *tmd__GetSerialPortConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tmd__SendReceiveSerialCommand' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tmd__SendReceiveSerialCommand(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__SendReceiveSerialCommand *tmd__SendReceiveSerialCommand, struct _tmd__SendReceiveSerialCommandResponse *tmd__SendReceiveSerialCommandResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tmd__SendReceiveSerialCommand' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tmd__SendReceiveSerialCommand(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tmd__SendReceiveSerialCommand *tmd__SendReceiveSerialCommand){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tmd__SendReceiveSerialCommand' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tmd__SendReceiveSerialCommand(struct soap *soap, struct _tmd__SendReceiveSerialCommandResponse *tmd__SendReceiveSerialCommandResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

/** Web service operation '__tds__GetServices' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServices(struct soap* soap, struct _tds__GetServices *tds__GetServices, struct _tds__GetServicesResponse *tds__GetServicesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    int ret = 0;
    char ip[20];
    char url[MAX_XADDR_LEN];

	/* get host ip */
    ret = LUX_Network_getip2((char *)"eth0", ip);
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

    ACCESS_CONTROL;

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

    ret = LUX_Network_GetMACStr((char *)"eth0", (unsigned char *)tds__GetDeviceInformationResponse->HardwareId); //eth0
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
    ACCESS_CONTROL;
    char dateBuf[64];
    char cmd[128];

    if (tds__SetSystemDateAndTime->DateTimeType == tt__SetDateTimeType__NTP)
    {
        lux_time_ntp_sync("00:00");
    }
    else if (tds__SetSystemDateAndTime->DateTimeType == tt__SetDateTimeType__Manual)
    {
        if (tds__SetSystemDateAndTime->UTCDateTime)
        {
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
        }
    }

    if (tds__SetSystemDateAndTime->TimeZone && tds__SetSystemDateAndTime->TimeZone->TZ)
    {
        printf("============= onvif timezone:[%s] \n", tds__SetSystemDateAndTime->TimeZone->TZ);
        set_time_zone(tds__SetSystemDateAndTime->TimeZone->TZ);
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

    strcpy(SystemDateAndTime->TimeZone->TZ, g_configData->system_config.timezone);

    time_t timev;

    timev = time(NULL);
    struct tm mytm = *localtime(&timev);

    SystemDateAndTime->UTCDateTime->Date->Year  = mytm.tm_year+1900;
    SystemDateAndTime->UTCDateTime->Date->Month = mytm.tm_mon + 1;
    SystemDateAndTime->UTCDateTime->Date->Day   = mytm.tm_mday;

    SystemDateAndTime->UTCDateTime->Time->Hour   = mytm.tm_hour;
    SystemDateAndTime->UTCDateTime->Time->Minute = mytm.tm_min;
    SystemDateAndTime->UTCDateTime->Time->Second = mytm.tm_sec;

    timev += g_configData->system_config.timezone_offset_s;
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
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemFactoryDefault(struct soap* soap, struct _tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault, struct _tds__SetSystemFactoryDefaultResponse *tds__SetSystemFactoryDefaultResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    LUX_BASE_System_CMD("/tmp/factory_reset.sh");
    return SOAP_OK;
}

/** Web service operation '__tds__UpgradeSystemFirmware' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__UpgradeSystemFirmware(struct soap* soap, struct _tds__UpgradeSystemFirmware *tds__UpgradeSystemFirmware, struct _tds__UpgradeSystemFirmwareResponse *tds__UpgradeSystemFirmwareResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
	return SOAP_OK;
}

/** Web service operation '__tds__SystemReboot' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SystemReboot(struct soap* soap, struct _tds__SystemReboot *tds__SystemReboot, struct _tds__SystemRebootResponse *tds__SystemRebootResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
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
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetSystemBackup' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemBackup(struct soap* soap, struct _tds__GetSystemBackup *tds__GetSystemBackup, struct _tds__GetSystemBackupResponse *tds__GetSystemBackupResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetSystemLog' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemLog(struct soap* soap, struct _tds__GetSystemLog *tds__GetSystemLog, struct _tds__GetSystemLogResponse *tds__GetSystemLogResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetSystemSupportInformation' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemSupportInformation(struct soap* soap, struct _tds__GetSystemSupportInformation *tds__GetSystemSupportInformation, struct _tds__GetSystemSupportInformationResponse *tds__GetSystemSupportInformationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetScopes' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetScopes(struct soap* soap, struct _tds__GetScopes *tds__GetScopes, struct _tds__GetScopesResponse *tds__GetScopesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
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
    (Scopes+3)->ScopeItem = "onvif://www.onvif.org/name/Luxshare-IPC";

    (Scopes+4)->ScopeDef = tt__ScopeDefinition__Configurable;
    (Scopes+4)->ScopeItem = "onvif://www.onvif.org/location";

    (Scopes+5)->ScopeDef = tt__ScopeDefinition__Fixed;
    (Scopes+5)->ScopeItem = "onvif://www.onvif.org/hardware";

    (Scopes+6)->ScopeDef = tt__ScopeDefinition__Fixed;
    (Scopes+6)->ScopeItem = "onvif://www.onvif.org/hardware/Luxshare-IPCAM";

    tds__GetScopesResponse->Scopes = Scopes;

    return SOAP_OK;
}

/** Web service operation '__tds__SetScopes' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetScopes(struct soap* soap, struct _tds__SetScopes *tds__SetScopes, struct _tds__SetScopesResponse *tds__SetScopesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__AddScopes' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__AddScopes(struct soap* soap, struct _tds__AddScopes *tds__AddScopes, struct _tds__AddScopesResponse *tds__AddScopesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__RemoveScopes' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveScopes(struct soap* soap, struct _tds__RemoveScopes *tds__RemoveScopes, struct _tds__RemoveScopesResponse *tds__RemoveScopesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetDiscoveryMode' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDiscoveryMode(struct soap* soap, struct _tds__GetDiscoveryMode *tds__GetDiscoveryMode, struct _tds__GetDiscoveryModeResponse *tds__GetDiscoveryModeResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__SetDiscoveryMode' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDiscoveryMode(struct soap* soap, struct _tds__SetDiscoveryMode *tds__SetDiscoveryMode, struct _tds__SetDiscoveryModeResponse *tds__SetDiscoveryModeResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
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
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__SetRemoteUser' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteUser(struct soap* soap, struct _tds__SetRemoteUser *tds__SetRemoteUser, struct _tds__SetRemoteUserResponse *tds__SetRemoteUserResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetUsers' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetUsers(struct soap* soap, struct _tds__GetUsers *tds__GetUsers, struct _tds__GetUsersResponse *tds__GetUsersResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    int num = 0;
    int i = 0;
    struct LUX_USER_INFO_ST User[MAX_USER_NUM];
    
    //struct LUX_USER_INFO_ST* User = (struct LUX_USER_INFO_ST*)soap_zalloc(soap, sizeof(struct LUX_USER_INFO_ST));
    LUX_UserMngr_ListUsers(User, &num);

    tds__GetUsersResponse->__sizeUser = num;
    MYTRACE(" user number is %d\n",tds__GetUsersResponse->__sizeUser);
    struct tt__User* Res_User = (struct tt__User*)soap_zalloc(soap, sizeof(struct tt__User)*tds__GetUsersResponse->__sizeUser);

    for (i=0;i<num;i++)
    {
        Res_User[i].Username = (char*)soap_zalloc(soap, 100);
        Res_User[i].Password = (char*)soap_zalloc(soap, 100);
        strcpy(Res_User[i].Username, User[i].user_name);
        strcpy(Res_User[i].Password, User[i].password);
        Res_User[i].UserLevel = User[i].role;
    }

    tds__GetUsersResponse->User = Res_User;

    return SOAP_OK;
}

/** Web service operation '__tds__CreateUsers' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateUsers(struct soap* soap, struct _tds__CreateUsers *tds__CreateUsers, struct _tds__CreateUsersResponse *tds__CreateUsersResponse)
{
    ACCESS_CONTROL;

    int ret = 0;
    int i = 0; 
    struct LUX_USER_INFO_ST* user = (struct LUX_USER_INFO_ST*)soap_zalloc(soap, sizeof(struct LUX_USER_INFO_ST));

    if (tds__CreateUsers != NULL) {
        for (i = 0; i < tds__CreateUsers->__sizeUser; i++) {

            LOG_DBG(LOG_TAG, "Username: %s\n", tds__CreateUsers->User->Username ? tds__CreateUsers->User->Username : "");
            LOG_DBG(LOG_TAG, "Password: %s\n", tds__CreateUsers->User->Password ? tds__CreateUsers->User->Password : "");
            LOG_DBG(LOG_TAG, "UserLevel: %d\n", tds__CreateUsers->User->UserLevel);
            strcpy(user->user_id, "admin");
            strcpy(user->user_name, tds__CreateUsers->User[i].Username);
            strcpy(user->password, tds__CreateUsers->User[i].Password);
            user->role = tds__CreateUsers->User[i].UserLevel;

            ret = LUX_UserMngr_AddUser(user);
            if (ret < 0) {
                LOG_DBG(LOG_TAG, "ERROR: CreateUsers Fault!\n");
                return SOAP_ERR;
            }
        }
    }
  
    LOG_DBG(LOG_TAG, "CreateUsers is successfully\n");
    return SOAP_OK;
}

/** Web service operation '__tds__DeleteUsers' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteUsers(struct soap* soap, struct _tds__DeleteUsers *tds__DeleteUsers, struct _tds__DeleteUsersResponse *tds__DeleteUsersResponse)
{
    ACCESS_CONTROL;
    LOG_DBG(LOG_TAG, "\n");
    int i = 0 ,ret = 0;
    char* buf = NULL;
    if (tds__DeleteUsers != NULL) {
        for(i = 0; i < tds__DeleteUsers->__sizeUsername; i++) {
            LOG_DBG(LOG_TAG, "Username : %s\n", tds__DeleteUsers->Username[i]);
            buf = tds__DeleteUsers->Username[i];
            LOG_DBG(LOG_TAG, "Username : %s\n", buf);
            ret = LUX_UserMngr_DelUser(buf);
            if (ret < 0) {
                LOG_DBG(LOG_TAG, "delete_user failed,Invalid parameter!\n");
                return SOAP_ERR;
            }
        }
    }

    return SOAP_OK;
}

/** Web service operation '__tds__SetUser' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetUser(struct soap* soap, struct _tds__SetUser *tds__SetUser, struct _tds__SetUserResponse *tds__SetUserResponse)
{
    ACCESS_CONTROL;
    int i = 0;
    int ret = 0;
    struct LUX_USER_INFO_ST* user = (struct LUX_USER_INFO_ST*)soap_zalloc(soap, sizeof(struct LUX_USER_INFO_ST));
    if (tds__SetUser != NULL) {
        for(i = 0;i < tds__SetUser->__sizeUser; i++) {
            LOG_DBG(LOG_TAG, "Username: %s\n", (tds__SetUser->User + i)->Username);
            LOG_DBG(LOG_TAG, "Password: %s\n", (tds__SetUser->User + i)->Password);
            LOG_DBG(LOG_TAG, "UserLevel: %d\n", (tds__SetUser->User + i)->UserLevel);

            strcpy(user->user_id, "admin");
            strcpy(user->user_name, (tds__SetUser->User + i)->Username);
            strcpy(user->password, (tds__SetUser->User + i)->Password);
            user->role = (tds__SetUser->User + i)->UserLevel;

            ret = LUX_UserMngr_ModifyUser(user);
            if (ret < 0) {
                LOG_DBG(LOG_TAG, "ModifyUser failed,Invalid parameter!\n");
                return SOAP_ERR;
            }
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
    ret = LUX_Network_getip2((char *)"eth0", ip); //eth0
    if (ret < 0) {
		LOG_DBG(LOG_TAG, "ERROR: can't get host ip address!\n");
		return SOAP_ERR;
    }

    tds__GetWsdlUrlResponse->WsdlUrl = (char *)soap_zalloc(soap, sizeof(char)*128);
    sprintf(tds__GetWsdlUrlResponse->WsdlUrl, "http://%s/wsdl", ip);

    return SOAP_OK;
}

/** Web service operation '__tds__GetPasswordComplexityOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPasswordComplexityOptions(struct soap* soap, struct _tds__GetPasswordComplexityOptions* tds__GetPasswordComplexityOptions, struct _tds__GetPasswordComplexityOptionsResponse* tds__GetPasswordComplexityOptionsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
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
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetPasswordHistoryConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPasswordHistoryConfiguration(struct soap* soap, struct _tds__GetPasswordHistoryConfiguration* tds__GetPasswordHistoryConfiguration, struct _tds__GetPasswordHistoryConfigurationResponse* tds__GetPasswordHistoryConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__SetPasswordHistoryConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetPasswordHistoryConfiguration(struct soap* soap, struct _tds__SetPasswordHistoryConfiguration* tds__SetPasswordHistoryConfiguration, struct _tds__SetPasswordHistoryConfigurationResponse* tds__SetPasswordHistoryConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetAuthFailureWarningOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetAuthFailureWarningOptions(struct soap* soap, struct _tds__GetAuthFailureWarningOptions* tds__GetAuthFailureWarningOptions, struct _tds__GetAuthFailureWarningOptionsResponse* tds__GetAuthFailureWarningOptionsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetAuthFailureWarningConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetAuthFailureWarningConfiguration(struct soap* soap, struct _tds__GetAuthFailureWarningConfiguration* tds__GetAuthFailureWarningConfiguration, struct _tds__GetAuthFailureWarningConfigurationResponse* tds__GetAuthFailureWarningConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__SetAuthFailureWarningConfiguration' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetAuthFailureWarningConfiguration(struct soap* soap, struct _tds__SetAuthFailureWarningConfiguration* tds__SetAuthFailureWarningConfiguration, struct _tds__SetAuthFailureWarningConfigurationResponse* tds__SetAuthFailureWarningConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetCapabilities' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCapabilities(struct soap* soap, struct _tds__GetCapabilities *tds__GetCapabilities, struct _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    int ret = 0;
    char ip[20];
    char url[MAX_XADDR_LEN];

    ret = LUX_Network_getip2((char *)"eth0", ip); //eth0
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
    strcpy(HostnameInformation->Name, "Luxshare-IPC");

    tds__GetHostnameResponse->HostnameInformation = HostnameInformation;

    return SOAP_OK;
}

/** Web service operation '__tds__SetHostname' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostname(struct soap* soap, struct _tds__SetHostname *tds__SetHostname, struct _tds__SetHostnameResponse *tds__SetHostnameResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__SetHostnameFromDHCP' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostnameFromDHCP(struct soap* soap, struct _tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP, struct _tds__SetHostnameFromDHCPResponse *tds__SetHostnameFromDHCPResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetDNS' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDNS(struct soap* soap, struct _tds__GetDNS *tds__GetDNS, struct _tds__GetDNSResponse *tds__GetDNSResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL
    int ret = 0;
    char ip[20];
    /* get host ip */
    ret = LUX_Network_GetDNS(ip); //eth0
    if (ret < 0) {
		MYERROR("can't get host ip address!\n");
		return SOAP_ERR;
    }

    MYTRACEING("dns:%s \n", ip);

    struct tt__DNSInformation *DNSInformation = (struct tt__DNSInformation *)soap_zalloc(soap, sizeof(struct tt__DNSInformation));

    DNSInformation->FromDHCP = xsd__boolean__false_;

    DNSInformation->__sizeDNSManual = 1;
    struct tt__IPAddress *DNSManual = (struct tt__IPAddress *)soap_zalloc(soap, sizeof(struct tt__IPAddress));
    DNSManual->IPv4Address = (char*)soap_zalloc(soap, INFO_STRING_LEN);
    DNSManual->Type = tt__IPType__IPv4;
    strcpy(DNSManual->IPv4Address, ip);

    DNSInformation->DNSManual = DNSManual;
    tds__GetDNSResponse->DNSInformation = DNSInformation;

    return SOAP_OK;
}

/** Web service operation '__tds__SetDNS' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDNS(struct soap* soap, struct _tds__SetDNS *tds__SetDNS, struct _tds__SetDNSResponse *tds__SetDNSResponse)
{
    int ret = 0;
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    if (tds__SetDNS && tds__SetDNS->DNSManual && tds__SetDNS->DNSManual->IPv4Address)
    {
        ret = LUX_Network_SetDNS(tds__SetDNS->DNSManual->IPv4Address);
        if (ret != 0)
        {
            MYERROR("set dns[%s] failed\n", tds__SetDNS->DNSManual->IPv4Address);
        }
    }

    return SOAP_OK;
}

/** Web service operation '__tds__GetNTP' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNTP(struct soap* soap, struct _tds__GetNTP *tds__GetNTP, struct _tds__GetNTPResponse *tds__GetNTPResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL
    return SOAP_OK;
}

/** Web service operation '__tds__SetNTP' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNTP(struct soap* soap, struct _tds__SetNTP *tds__SetNTP, struct _tds__SetNTPResponse *tds__SetNTPResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetDynamicDNS' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDynamicDNS(struct soap* soap, struct _tds__GetDynamicDNS *tds__GetDynamicDNS, struct _tds__GetDynamicDNSResponse *tds__GetDynamicDNSResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__SetDynamicDNS' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDynamicDNS(struct soap* soap, struct _tds__SetDynamicDNS *tds__SetDynamicDNS, struct _tds__SetDynamicDNSResponse *tds__SetDynamicDNSResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetNetworkInterfaces' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkInterfaces(struct soap* soap, struct _tds__GetNetworkInterfaces *tds__GetNetworkInterfaces, struct _tds__GetNetworkInterfacesResponse *tds__GetNetworkInterfacesResponse)
{
    LOG_DBG(LOG_TAG, "\n");

    ACCESS_CONTROL;
    int ret = 0;

    char ip2[20];
    char netmask[20];
    char mac[20];
    int hostmac[6] = { 0 };

    LUX_Network_getip(mac, ip2, netmask);
    MYTRACEING("IP:[%s] mask:[%s] mac:[%s] \n", ip2, netmask, mac);

    sscanf(mac, "%02x:%02x:%02x:%02x:%02x:%02x", &hostmac[0], &hostmac[1], &hostmac[2], &hostmac[3], &hostmac[4], &hostmac[5]);

    struct tt__NetworkInterface *NetworkInterfaces = (struct tt__NetworkInterface *)soap_zalloc(soap, sizeof(struct tt__NetworkInterface));

    NetworkInterfaces->token = "1";
    NetworkInterfaces->Enabled = xsd__boolean__true_;

    struct tt__NetworkInterfaceInfo *Info =	(struct tt__NetworkInterfaceInfo *)soap_zalloc(soap, sizeof(struct tt__NetworkInterfaceInfo));
    Info->Name = "eth0";
    Info->HwAddress = (char *)soap_zalloc(soap, 100);
    sprintf(Info->HwAddress, "%02x-%02x-%02x-%02x-%02x-%02x", hostmac[0], hostmac[1], hostmac[2], hostmac[3], hostmac[4], hostmac[5]);

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
    strcpy(Manual->Address, ip2);

    unsigned int maskValue = inet_addr(netmask);

    Manual->PrefixLength = 0;

    int i;
    for (i = 0; i < 32; i++)
    {
        if ((maskValue >> i) & 1)
        {
            Manual->PrefixLength++;
        }
        else
        {
            break;
        }
    }

    Config->Manual = Manual;
    IPv4->Config = Config;
    NetworkInterfaces->Info = Info;
    NetworkInterfaces->Link = Link;
    NetworkInterfaces->IPv4 = IPv4;

    tds__GetNetworkInterfacesResponse->__sizeNetworkInterfaces = 1;
    tds__GetNetworkInterfacesResponse->NetworkInterfaces = NetworkInterfaces;

    return SOAP_OK;
}

/** Web service operation '__tds__SetNetworkInterfaces' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkInterfaces(struct soap* soap, struct _tds__SetNetworkInterfaces *tds__SetNetworkInterfaces, struct _tds__SetNetworkInterfacesResponse *tds__SetNetworkInterfacesResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
	char subnetaddr[20] = {0};
    char ip[20] = { 0 };
	int netmask_len = 0;
	memcpy(ip, tds__SetNetworkInterfaces->NetworkInterface->IPv4->Manual->Address, strlen(tds__SetNetworkInterfaces->NetworkInterface->IPv4->Manual->Address));

	netmask_len = tds__SetNetworkInterfaces->NetworkInterface->IPv4->Manual->PrefixLength;

    unsigned maskValue = 0;
    int i;
    for (i = 0; i < netmask_len; i++)
    {
        maskValue = maskValue << 1 | 1;
    }

    unsigned char valueTmp[4] = { ((char*)&maskValue)[0],((char*)&maskValue)[1], ((char*)&maskValue)[2], ((char*)&maskValue)[3] };
    sprintf(subnetaddr, "%d.%d.%d.%d", valueTmp[0], valueTmp[1], valueTmp[2], valueTmp[3]);
	printf("-------- netmask = %s\n", subnetaddr);

    LUX_Network_setip(ip, subnetaddr);

	tds__SetNetworkInterfacesResponse->RebootNeeded = xsd__boolean__false_;

	return SOAP_OK;
}

/** Web service operation '__tds__GetNetworkProtocols' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkProtocols(struct soap* soap, struct _tds__GetNetworkProtocols *tds__GetNetworkProtocols, struct _tds__GetNetworkProtocolsResponse *tds__GetNetworkProtocolsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__SetNetworkProtocols' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkProtocols(struct soap* soap, struct _tds__SetNetworkProtocols *tds__SetNetworkProtocols, struct _tds__SetNetworkProtocolsResponse *tds__SetNetworkProtocolsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetNetworkDefaultGateway' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkDefaultGateway(struct soap* soap, struct _tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway, struct _tds__GetNetworkDefaultGatewayResponse *tds__GetNetworkDefaultGatewayResponse)
{
    LOG_DBG(LOG_TAG, "Get network default gateway.\n");
    ACCESS_CONTROL;
	struct tt__NetworkGateway* NetworkGateway = (struct tt__NetworkGateway*)soap_zalloc(soap, sizeof(struct tt__NetworkGateway));
	char **IPv4Address = (char*)soap_zalloc(soap, 20);
	char *tmp = (char *)soap_zalloc(soap, 20);
    IPv4Address[0] = tmp;
    NetworkGateway->__sizeIPv4Address = 1;

	//获取网关
    LUX_Network_getGateway(tmp);

	printf("Gateway:%s\n", tmp);

	NetworkGateway->IPv4Address = IPv4Address;
	tds__GetNetworkDefaultGatewayResponse->NetworkGateway = NetworkGateway;

	return SOAP_OK;
}

/** Web service operation '__tds__SetNetworkDefaultGateway' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkDefaultGateway(struct soap* soap, struct _tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway, struct _tds__SetNetworkDefaultGatewayResponse *tds__SetNetworkDefaultGatewayResponse)
{
    LOG_DBG(LOG_TAG, "Set network default gateway.\n");
    ACCESS_CONTROL;
	printf("-------- __tds__SetNetworkDefaultGateway...\n");

	if ( tds__SetNetworkDefaultGateway == NULL ) {
		printf("------- tds__SetNetworkDefaultGateway == NULL\n");
		return SOAP_ERR;
	}

	printf("__sizeIPv4Address = %d.\n", tds__SetNetworkDefaultGateway->__sizeIPv4Address);
	printf("IPv4Address = %s\n", tds__SetNetworkDefaultGateway->IPv4Address[0]);
    if (tds__SetNetworkDefaultGateway->IPv4Address[0] != NULL)
    {
        LUX_Network_setGateway(tds__SetNetworkDefaultGateway->IPv4Address[0]);
    }

	return SOAP_OK;
}

/** Web service operation '__tds__GetZeroConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetZeroConfiguration(struct soap* soap, struct _tds__GetZeroConfiguration *tds__GetZeroConfiguration, struct _tds__GetZeroConfigurationResponse *tds__GetZeroConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__SetZeroConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetZeroConfiguration(struct soap* soap, struct _tds__SetZeroConfiguration *tds__SetZeroConfiguration, struct _tds__SetZeroConfigurationResponse *tds__SetZeroConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetIPAddressFilter' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetIPAddressFilter(struct soap* soap, struct _tds__GetIPAddressFilter *tds__GetIPAddressFilter, struct _tds__GetIPAddressFilterResponse *tds__GetIPAddressFilterResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__SetIPAddressFilter' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetIPAddressFilter(struct soap* soap, struct _tds__SetIPAddressFilter *tds__SetIPAddressFilter, struct _tds__SetIPAddressFilterResponse *tds__SetIPAddressFilterResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__AddIPAddressFilter' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__AddIPAddressFilter(struct soap* soap, struct _tds__AddIPAddressFilter *tds__AddIPAddressFilter, struct _tds__AddIPAddressFilterResponse *tds__AddIPAddressFilterResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__RemoveIPAddressFilter' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveIPAddressFilter(struct soap* soap, struct _tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter, struct _tds__RemoveIPAddressFilterResponse *tds__RemoveIPAddressFilterResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetAccessPolicy' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetAccessPolicy(struct soap* soap, struct _tds__GetAccessPolicy *tds__GetAccessPolicy, struct _tds__GetAccessPolicyResponse *tds__GetAccessPolicyResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__SetAccessPolicy' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetAccessPolicy(struct soap* soap, struct _tds__SetAccessPolicy *tds__SetAccessPolicy, struct _tds__SetAccessPolicyResponse *tds__SetAccessPolicyResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
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
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__SetRelayOutputSettings' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputSettings(struct soap* soap, struct _tds__SetRelayOutputSettings *tds__SetRelayOutputSettings, struct _tds__SetRelayOutputSettingsResponse *tds__SetRelayOutputSettingsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__SetRelayOutputState' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputState(struct soap* soap, struct _tds__SetRelayOutputState *tds__SetRelayOutputState, struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__SendAuxiliaryCommand' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SendAuxiliaryCommand(struct soap* soap, struct _tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand, struct _tds__SendAuxiliaryCommandResponse *tds__SendAuxiliaryCommandResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
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
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetDot11Status' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Status(struct soap* soap, struct _tds__GetDot11Status *tds__GetDot11Status, struct _tds__GetDot11StatusResponse *tds__GetDot11StatusResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__ScanAvailableDot11Networks' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__ScanAvailableDot11Networks(struct soap* soap, struct _tds__ScanAvailableDot11Networks *tds__ScanAvailableDot11Networks, struct _tds__ScanAvailableDot11NetworksResponse *tds__ScanAvailableDot11NetworksResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetSystemUris' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemUris(struct soap* soap, struct _tds__GetSystemUris *tds__GetSystemUris, struct _tds__GetSystemUrisResponse *tds__GetSystemUrisResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__StartFirmwareUpgrade' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__StartFirmwareUpgrade(struct soap* soap, struct _tds__StartFirmwareUpgrade *tds__StartFirmwareUpgrade, struct _tds__StartFirmwareUpgradeResponse *tds__StartFirmwareUpgradeResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__StartSystemRestore' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__StartSystemRestore(struct soap* soap, struct _tds__StartSystemRestore *tds__StartSystemRestore, struct _tds__StartSystemRestoreResponse *tds__StartSystemRestoreResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetStorageConfigurations' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetStorageConfigurations(struct soap* soap, struct _tds__GetStorageConfigurations *tds__GetStorageConfigurations, struct _tds__GetStorageConfigurationsResponse *tds__GetStorageConfigurationsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__CreateStorageConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateStorageConfiguration(struct soap* soap, struct _tds__CreateStorageConfiguration *tds__CreateStorageConfiguration, struct _tds__CreateStorageConfigurationResponse *tds__CreateStorageConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
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
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__DeleteStorageConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteStorageConfiguration(struct soap* soap, struct _tds__DeleteStorageConfiguration *tds__DeleteStorageConfiguration, struct _tds__DeleteStorageConfigurationResponse *tds__DeleteStorageConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__GetGeoLocation' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__GetGeoLocation(struct soap* soap, struct _tds__GetGeoLocation *tds__GetGeoLocation, struct _tds__GetGeoLocationResponse *tds__GetGeoLocationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__SetGeoLocation' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__SetGeoLocation(struct soap* soap, struct _tds__SetGeoLocation *tds__SetGeoLocation, struct _tds__SetGeoLocationResponse *tds__SetGeoLocationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

/** Web service operation '__tds__DeleteGeoLocation' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteGeoLocation(struct soap* soap, struct _tds__DeleteGeoLocation *tds__DeleteGeoLocation, struct _tds__DeleteGeoLocationResponse *tds__DeleteGeoLocationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHashingAlgorithm(struct soap* soap, struct _tds__SetHashingAlgorithm* tds__SetHashingAlgorithm, struct _tds__SetHashingAlgorithmResponse* tds__SetHashingAlgorithmResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    ACCESS_CONTROL;
    return SOAP_OK;
}

    /** Web service operation '__tmd__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetServiceCapabilities(struct soap* soap, struct _tmd__GetServiceCapabilities *tmd__GetServiceCapabilities, struct _tmd__GetServiceCapabilitiesResponse *tmd__GetServiceCapabilitiesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__GetRelayOutputOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetRelayOutputOptions(struct soap* soap, struct _tmd__GetRelayOutputOptions *tmd__GetRelayOutputOptions, struct _tmd__GetRelayOutputOptionsResponse *tmd__GetRelayOutputOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__GetAudioSources' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetAudioSources(struct soap* soap, struct tmd__Get *tmd__GetAudioSources, struct tmd__GetResponse *tmd__GetAudioSourcesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__GetAudioOutputs' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetAudioOutputs(struct soap* soap, struct tmd__Get *tmd__GetAudioOutputs, struct tmd__GetResponse *tmd__GetAudioOutputsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__GetVideoSources' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetVideoSources(struct soap* soap, struct tmd__Get *tmd__GetVideoSources, struct tmd__GetResponse *tmd__GetVideoSourcesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__GetVideoOutputs' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetVideoOutputs(struct soap* soap, struct _tmd__GetVideoOutputs *tmd__GetVideoOutputs, struct _tmd__GetVideoOutputsResponse *tmd__GetVideoOutputsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__GetVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetVideoSourceConfiguration(struct soap* soap, struct _tmd__GetVideoSourceConfiguration *tmd__GetVideoSourceConfiguration, struct _tmd__GetVideoSourceConfigurationResponse *tmd__GetVideoSourceConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__GetVideoOutputConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetVideoOutputConfiguration(struct soap* soap, struct _tmd__GetVideoOutputConfiguration *tmd__GetVideoOutputConfiguration, struct _tmd__GetVideoOutputConfigurationResponse *tmd__GetVideoOutputConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__GetAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetAudioSourceConfiguration(struct soap* soap, struct _tmd__GetAudioSourceConfiguration *tmd__GetAudioSourceConfiguration, struct _tmd__GetAudioSourceConfigurationResponse *tmd__GetAudioSourceConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__GetAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetAudioOutputConfiguration(struct soap* soap, struct _tmd__GetAudioOutputConfiguration *tmd__GetAudioOutputConfiguration, struct _tmd__GetAudioOutputConfigurationResponse *tmd__GetAudioOutputConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__SetVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetVideoSourceConfiguration(struct soap* soap, struct _tmd__SetVideoSourceConfiguration *tmd__SetVideoSourceConfiguration, struct _tmd__SetVideoSourceConfigurationResponse *tmd__SetVideoSourceConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__SetVideoOutputConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetVideoOutputConfiguration(struct soap* soap, struct _tmd__SetVideoOutputConfiguration *tmd__SetVideoOutputConfiguration, struct _tmd__SetVideoOutputConfigurationResponse *tmd__SetVideoOutputConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__SetAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetAudioSourceConfiguration(struct soap* soap, struct _tmd__SetAudioSourceConfiguration *tmd__SetAudioSourceConfiguration, struct _tmd__SetAudioSourceConfigurationResponse *tmd__SetAudioSourceConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__SetAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetAudioOutputConfiguration(struct soap* soap, struct _tmd__SetAudioOutputConfiguration *tmd__SetAudioOutputConfiguration, struct _tmd__SetAudioOutputConfigurationResponse *tmd__SetAudioOutputConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__GetVideoSourceConfigurationOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetVideoSourceConfigurationOptions(struct soap* soap, struct _tmd__GetVideoSourceConfigurationOptions *tmd__GetVideoSourceConfigurationOptions, struct _tmd__GetVideoSourceConfigurationOptionsResponse *tmd__GetVideoSourceConfigurationOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__GetVideoOutputConfigurationOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetVideoOutputConfigurationOptions(struct soap* soap, struct _tmd__GetVideoOutputConfigurationOptions *tmd__GetVideoOutputConfigurationOptions, struct _tmd__GetVideoOutputConfigurationOptionsResponse *tmd__GetVideoOutputConfigurationOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__GetAudioSourceConfigurationOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetAudioSourceConfigurationOptions(struct soap* soap, struct _tmd__GetAudioSourceConfigurationOptions *tmd__GetAudioSourceConfigurationOptions, struct _tmd__GetAudioSourceConfigurationOptionsResponse *tmd__GetAudioSourceConfigurationOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__GetAudioOutputConfigurationOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetAudioOutputConfigurationOptions(struct soap* soap, struct _tmd__GetAudioOutputConfigurationOptions *tmd__GetAudioOutputConfigurationOptions, struct _tmd__GetAudioOutputConfigurationOptionsResponse *tmd__GetAudioOutputConfigurationOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__GetRelayOutputs' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetRelayOutputs(struct soap* soap, struct _tds__GetRelayOutputs *tds__GetRelayOutputs, struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__SetRelayOutputSettings' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetRelayOutputSettings(struct soap* soap, struct _tmd__SetRelayOutputSettings *tmd__SetRelayOutputSettings, struct _tmd__SetRelayOutputSettingsResponse *tmd__SetRelayOutputSettingsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__SetRelayOutputState' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetRelayOutputState(struct soap* soap, struct _tds__SetRelayOutputState *tds__SetRelayOutputState, struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__GetDigitalInputs' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetDigitalInputs(struct soap* soap, struct _tmd__GetDigitalInputs *tmd__GetDigitalInputs, struct _tmd__GetDigitalInputsResponse *tmd__GetDigitalInputsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__GetDigitalInputConfigurationOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetDigitalInputConfigurationOptions(struct soap* soap, struct _tmd__GetDigitalInputConfigurationOptions *tmd__GetDigitalInputConfigurationOptions, struct _tmd__GetDigitalInputConfigurationOptionsResponse *tmd__GetDigitalInputConfigurationOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__SetDigitalInputConfigurations' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetDigitalInputConfigurations(struct soap* soap, struct _tmd__SetDigitalInputConfigurations *tmd__SetDigitalInputConfigurations, struct _tmd__SetDigitalInputConfigurationsResponse *tmd__SetDigitalInputConfigurationsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__GetSerialPorts' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetSerialPorts(struct soap* soap, struct _tmd__GetSerialPorts *tmd__GetSerialPorts, struct _tmd__GetSerialPortsResponse *tmd__GetSerialPortsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__GetSerialPortConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetSerialPortConfiguration(struct soap* soap, struct _tmd__GetSerialPortConfiguration *tmd__GetSerialPortConfiguration, struct _tmd__GetSerialPortConfigurationResponse *tmd__GetSerialPortConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__SetSerialPortConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__SetSerialPortConfiguration(struct soap* soap, struct _tmd__SetSerialPortConfiguration *tmd__SetSerialPortConfiguration, struct _tmd__SetSerialPortConfigurationResponse *tmd__SetSerialPortConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__GetSerialPortConfigurationOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__GetSerialPortConfigurationOptions(struct soap* soap, struct _tmd__GetSerialPortConfigurationOptions *tmd__GetSerialPortConfigurationOptions, struct _tmd__GetSerialPortConfigurationOptionsResponse *tmd__GetSerialPortConfigurationOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tmd__SendReceiveSerialCommand' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tmd__SendReceiveSerialCommand(struct soap* soap, struct _tmd__SendReceiveSerialCommand *tmd__SendReceiveSerialCommand, struct _tmd__SendReceiveSerialCommandResponse *tmd__SendReceiveSerialCommandResponse){
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

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetServices(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetServiceCapabilities(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetDeviceInformation(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetSystemDateAndTime(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetSystemDateAndTime(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetSystemFactoryDefault(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__UpgradeSystemFirmware(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SystemReboot(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__RestoreSystem(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetSystemBackup(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetSystemLog(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetSystemSupportInformation(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetScopes(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetScopes(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__AddScopes(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__RemoveScopes(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetDiscoveryMode(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetDiscoveryMode(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetRemoteDiscoveryMode(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetRemoteDiscoveryMode(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetDPAddresses(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetEndpointReference(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetRemoteUser(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetRemoteUser(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetUsers(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__CreateUsers(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__DeleteUsers(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetUser(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetWsdlUrl(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetPasswordComplexityOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetPasswordComplexityConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetPasswordComplexityConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetPasswordHistoryConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetPasswordHistoryConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetAuthFailureWarningOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetAuthFailureWarningConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetAuthFailureWarningConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetCapabilities(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetDPAddresses(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetHostname(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetHostname(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetHostnameFromDHCP(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetDNS(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetDNS(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetNTP(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetNTP(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetDynamicDNS(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetDynamicDNS(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetNetworkInterfaces(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetNetworkInterfaces(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetNetworkProtocols(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetNetworkProtocols(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetNetworkDefaultGateway(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetNetworkDefaultGateway(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetZeroConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetZeroConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetIPAddressFilter(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetIPAddressFilter(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__AddIPAddressFilter(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__RemoveIPAddressFilter(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetAccessPolicy(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetAccessPolicy(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__CreateCertificate(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetCertificates(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetCertificatesStatus(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetCertificatesStatus(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__DeleteCertificates(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetPkcs10Request(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__LoadCertificates(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetClientCertificateMode(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetClientCertificateMode(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetRelayOutputs(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetRelayOutputSettings(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetRelayOutputState(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SendAuxiliaryCommand(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetCACertificates(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__LoadCertificateWithPrivateKey(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetCertificateInformation(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__LoadCACertificates(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__CreateDot1XConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetDot1XConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetDot1XConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetDot1XConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__DeleteDot1XConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetDot11Capabilities(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetDot11Status(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__ScanAvailableDot11Networks(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetSystemUris(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__StartFirmwareUpgrade(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__StartSystemRestore(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetStorageConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__CreateStorageConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetStorageConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetStorageConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__DeleteStorageConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetGeoLocation(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetGeoLocation(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__DeleteGeoLocation(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SetHashingAlgorithm(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetServiceCapabilities(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetRelayOutputOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetAudioSources(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetAudioOutputs(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetVideoSources(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetVideoOutputs(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetVideoSourceConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetVideoOutputConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetAudioSourceConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetAudioOutputConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__SetVideoSourceConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__SetVideoOutputConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__SetAudioSourceConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__SetAudioOutputConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetVideoSourceConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetVideoOutputConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetAudioSourceConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetAudioOutputConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetRelayOutputs(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__SetRelayOutputSettings(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__SetRelayOutputState(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetDigitalInputs(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetDigitalInputConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__SetDigitalInputConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetSerialPorts(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetSerialPortConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__SetSerialPortConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__GetSerialPortConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tmd__SendReceiveSerialCommand(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

