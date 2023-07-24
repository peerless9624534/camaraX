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

    
//     /** Web service synchronous operation 'soap_call___ns1__GetServiceCapabilities' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetServiceCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetServiceCapabilities *ns1__GetServiceCapabilities, struct _ns1__GetServiceCapabilitiesResponse *ns1__GetServiceCapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetServiceCapabilities' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetServiceCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetServiceCapabilities *ns1__GetServiceCapabilities){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetServiceCapabilities' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetServiceCapabilities(struct soap *soap, struct _ns1__GetServiceCapabilitiesResponse *ns1__GetServiceCapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__CreateProfile' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__CreateProfile(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__CreateProfile *ns1__CreateProfile, struct _ns1__CreateProfileResponse *ns1__CreateProfileResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__CreateProfile' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__CreateProfile(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__CreateProfile *ns1__CreateProfile){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__CreateProfile' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__CreateProfile(struct soap *soap, struct _ns1__CreateProfileResponse *ns1__CreateProfileResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetProfiles' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetProfiles(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetProfiles *ns1__GetProfiles, struct _ns1__GetProfilesResponse *ns1__GetProfilesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetProfiles' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetProfiles(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetProfiles *ns1__GetProfiles){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetProfiles' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetProfiles(struct soap *soap, struct _ns1__GetProfilesResponse *ns1__GetProfilesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__AddConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__AddConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__AddConfiguration *ns1__AddConfiguration, struct _ns1__AddConfigurationResponse *ns1__AddConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__AddConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__AddConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__AddConfiguration *ns1__AddConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__AddConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__AddConfiguration(struct soap *soap, struct _ns1__AddConfigurationResponse *ns1__AddConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__RemoveConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__RemoveConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__RemoveConfiguration *ns1__RemoveConfiguration, struct _ns1__RemoveConfigurationResponse *ns1__RemoveConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__RemoveConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__RemoveConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__RemoveConfiguration *ns1__RemoveConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__RemoveConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__RemoveConfiguration(struct soap *soap, struct _ns1__RemoveConfigurationResponse *ns1__RemoveConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__DeleteProfile' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__DeleteProfile(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__DeleteProfile *ns1__DeleteProfile, struct _ns1__DeleteProfileResponse *ns1__DeleteProfileResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__DeleteProfile' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__DeleteProfile(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__DeleteProfile *ns1__DeleteProfile){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__DeleteProfile' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__DeleteProfile(struct soap *soap, struct _ns1__DeleteProfileResponse *ns1__DeleteProfileResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetVideoSourceConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetVideoSourceConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetVideoSourceConfigurations, struct _ns1__GetVideoSourceConfigurationsResponse *ns1__GetVideoSourceConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetVideoSourceConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetVideoSourceConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetVideoSourceConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetVideoSourceConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetVideoSourceConfigurations(struct soap *soap, struct _ns1__GetVideoSourceConfigurationsResponse *ns1__GetVideoSourceConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetVideoEncoderConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetVideoEncoderConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetVideoEncoderConfigurations, struct _ns1__GetVideoEncoderConfigurationsResponse *ns1__GetVideoEncoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetVideoEncoderConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetVideoEncoderConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetVideoEncoderConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetVideoEncoderConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetVideoEncoderConfigurations(struct soap *soap, struct _ns1__GetVideoEncoderConfigurationsResponse *ns1__GetVideoEncoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetAudioSourceConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetAudioSourceConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetAudioSourceConfigurations, struct _ns1__GetAudioSourceConfigurationsResponse *ns1__GetAudioSourceConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetAudioSourceConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetAudioSourceConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetAudioSourceConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetAudioSourceConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetAudioSourceConfigurations(struct soap *soap, struct _ns1__GetAudioSourceConfigurationsResponse *ns1__GetAudioSourceConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetAudioEncoderConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetAudioEncoderConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetAudioEncoderConfigurations, struct _ns1__GetAudioEncoderConfigurationsResponse *ns1__GetAudioEncoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetAudioEncoderConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetAudioEncoderConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetAudioEncoderConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetAudioEncoderConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetAudioEncoderConfigurations(struct soap *soap, struct _ns1__GetAudioEncoderConfigurationsResponse *ns1__GetAudioEncoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetAnalyticsConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetAnalyticsConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetAnalyticsConfigurations, struct _ns1__GetAnalyticsConfigurationsResponse *ns1__GetAnalyticsConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetAnalyticsConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetAnalyticsConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetAnalyticsConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetAnalyticsConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetAnalyticsConfigurations(struct soap *soap, struct _ns1__GetAnalyticsConfigurationsResponse *ns1__GetAnalyticsConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetMetadataConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetMetadataConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetMetadataConfigurations, struct _ns1__GetMetadataConfigurationsResponse *ns1__GetMetadataConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetMetadataConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetMetadataConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetMetadataConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetMetadataConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetMetadataConfigurations(struct soap *soap, struct _ns1__GetMetadataConfigurationsResponse *ns1__GetMetadataConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetAudioOutputConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetAudioOutputConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetAudioOutputConfigurations, struct _ns1__GetAudioOutputConfigurationsResponse *ns1__GetAudioOutputConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetAudioOutputConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetAudioOutputConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetAudioOutputConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetAudioOutputConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetAudioOutputConfigurations(struct soap *soap, struct _ns1__GetAudioOutputConfigurationsResponse *ns1__GetAudioOutputConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetAudioDecoderConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetAudioDecoderConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetAudioDecoderConfigurations, struct _ns1__GetAudioDecoderConfigurationsResponse *ns1__GetAudioDecoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetAudioDecoderConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetAudioDecoderConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetAudioDecoderConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetAudioDecoderConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetAudioDecoderConfigurations(struct soap *soap, struct _ns1__GetAudioDecoderConfigurationsResponse *ns1__GetAudioDecoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__SetVideoSourceConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__SetVideoSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetVideoSourceConfiguration *ns1__SetVideoSourceConfiguration, struct ns1__SetConfigurationResponse *ns1__SetVideoSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__SetVideoSourceConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__SetVideoSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetVideoSourceConfiguration *ns1__SetVideoSourceConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__SetVideoSourceConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__SetVideoSourceConfiguration(struct soap *soap, struct ns1__SetConfigurationResponse *ns1__SetVideoSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__SetVideoEncoderConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__SetVideoEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetVideoEncoderConfiguration *ns1__SetVideoEncoderConfiguration, struct ns1__SetConfigurationResponse *ns1__SetVideoEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__SetVideoEncoderConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__SetVideoEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetVideoEncoderConfiguration *ns1__SetVideoEncoderConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__SetVideoEncoderConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__SetVideoEncoderConfiguration(struct soap *soap, struct ns1__SetConfigurationResponse *ns1__SetVideoEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__SetAudioSourceConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__SetAudioSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetAudioSourceConfiguration *ns1__SetAudioSourceConfiguration, struct ns1__SetConfigurationResponse *ns1__SetAudioSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__SetAudioSourceConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__SetAudioSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetAudioSourceConfiguration *ns1__SetAudioSourceConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__SetAudioSourceConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__SetAudioSourceConfiguration(struct soap *soap, struct ns1__SetConfigurationResponse *ns1__SetAudioSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__SetAudioEncoderConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__SetAudioEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetAudioEncoderConfiguration *ns1__SetAudioEncoderConfiguration, struct ns1__SetConfigurationResponse *ns1__SetAudioEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__SetAudioEncoderConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__SetAudioEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetAudioEncoderConfiguration *ns1__SetAudioEncoderConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__SetAudioEncoderConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__SetAudioEncoderConfiguration(struct soap *soap, struct ns1__SetConfigurationResponse *ns1__SetAudioEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__SetMetadataConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__SetMetadataConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetMetadataConfiguration *ns1__SetMetadataConfiguration, struct ns1__SetConfigurationResponse *ns1__SetMetadataConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__SetMetadataConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__SetMetadataConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetMetadataConfiguration *ns1__SetMetadataConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__SetMetadataConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__SetMetadataConfiguration(struct soap *soap, struct ns1__SetConfigurationResponse *ns1__SetMetadataConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__SetAudioOutputConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__SetAudioOutputConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetAudioOutputConfiguration *ns1__SetAudioOutputConfiguration, struct ns1__SetConfigurationResponse *ns1__SetAudioOutputConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__SetAudioOutputConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__SetAudioOutputConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetAudioOutputConfiguration *ns1__SetAudioOutputConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__SetAudioOutputConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__SetAudioOutputConfiguration(struct soap *soap, struct ns1__SetConfigurationResponse *ns1__SetAudioOutputConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__SetAudioDecoderConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__SetAudioDecoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetAudioDecoderConfiguration *ns1__SetAudioDecoderConfiguration, struct ns1__SetConfigurationResponse *ns1__SetAudioDecoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__SetAudioDecoderConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__SetAudioDecoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetAudioDecoderConfiguration *ns1__SetAudioDecoderConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__SetAudioDecoderConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__SetAudioDecoderConfiguration(struct soap *soap, struct ns1__SetConfigurationResponse *ns1__SetAudioDecoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetVideoSourceConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetVideoSourceConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetVideoSourceConfigurationOptions, struct _ns1__GetVideoSourceConfigurationOptionsResponse *ns1__GetVideoSourceConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetVideoSourceConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetVideoSourceConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetVideoSourceConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetVideoSourceConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetVideoSourceConfigurationOptions(struct soap *soap, struct _ns1__GetVideoSourceConfigurationOptionsResponse *ns1__GetVideoSourceConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetVideoEncoderConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetVideoEncoderConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetVideoEncoderConfigurationOptions, struct _ns1__GetVideoEncoderConfigurationOptionsResponse *ns1__GetVideoEncoderConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetVideoEncoderConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetVideoEncoderConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetVideoEncoderConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetVideoEncoderConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetVideoEncoderConfigurationOptions(struct soap *soap, struct _ns1__GetVideoEncoderConfigurationOptionsResponse *ns1__GetVideoEncoderConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetAudioSourceConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetAudioSourceConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetAudioSourceConfigurationOptions, struct _ns1__GetAudioSourceConfigurationOptionsResponse *ns1__GetAudioSourceConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetAudioSourceConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetAudioSourceConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetAudioSourceConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetAudioSourceConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetAudioSourceConfigurationOptions(struct soap *soap, struct _ns1__GetAudioSourceConfigurationOptionsResponse *ns1__GetAudioSourceConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetAudioEncoderConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetAudioEncoderConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetAudioEncoderConfigurationOptions, struct _ns1__GetAudioEncoderConfigurationOptionsResponse *ns1__GetAudioEncoderConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetAudioEncoderConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetAudioEncoderConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetAudioEncoderConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetAudioEncoderConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetAudioEncoderConfigurationOptions(struct soap *soap, struct _ns1__GetAudioEncoderConfigurationOptionsResponse *ns1__GetAudioEncoderConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetMetadataConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetMetadataConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetMetadataConfigurationOptions, struct _ns1__GetMetadataConfigurationOptionsResponse *ns1__GetMetadataConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetMetadataConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetMetadataConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetMetadataConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetMetadataConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetMetadataConfigurationOptions(struct soap *soap, struct _ns1__GetMetadataConfigurationOptionsResponse *ns1__GetMetadataConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetAudioOutputConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetAudioOutputConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetAudioOutputConfigurationOptions, struct _ns1__GetAudioOutputConfigurationOptionsResponse *ns1__GetAudioOutputConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetAudioOutputConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetAudioOutputConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetAudioOutputConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetAudioOutputConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetAudioOutputConfigurationOptions(struct soap *soap, struct _ns1__GetAudioOutputConfigurationOptionsResponse *ns1__GetAudioOutputConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetAudioDecoderConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetAudioDecoderConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetAudioDecoderConfigurationOptions, struct _ns1__GetAudioDecoderConfigurationOptionsResponse *ns1__GetAudioDecoderConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetAudioDecoderConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetAudioDecoderConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__GetConfiguration *ns1__GetAudioDecoderConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetAudioDecoderConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetAudioDecoderConfigurationOptions(struct soap *soap, struct _ns1__GetAudioDecoderConfigurationOptionsResponse *ns1__GetAudioDecoderConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetVideoEncoderInstances' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetVideoEncoderInstances(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetVideoEncoderInstances *ns1__GetVideoEncoderInstances, struct _ns1__GetVideoEncoderInstancesResponse *ns1__GetVideoEncoderInstancesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetVideoEncoderInstances' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetVideoEncoderInstances(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetVideoEncoderInstances *ns1__GetVideoEncoderInstances){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetVideoEncoderInstances' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetVideoEncoderInstances(struct soap *soap, struct _ns1__GetVideoEncoderInstancesResponse *ns1__GetVideoEncoderInstancesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetStreamUri' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetStreamUri(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetStreamUri *ns1__GetStreamUri, struct _ns1__GetStreamUriResponse *ns1__GetStreamUriResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetStreamUri' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetStreamUri(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetStreamUri *ns1__GetStreamUri){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetStreamUri' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetStreamUri(struct soap *soap, struct _ns1__GetStreamUriResponse *ns1__GetStreamUriResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__StartMulticastStreaming' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__StartMulticastStreaming(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__StartStopMulticastStreaming *ns1__StartMulticastStreaming, struct ns1__SetConfigurationResponse *ns1__StartMulticastStreamingResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__StartMulticastStreaming' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__StartMulticastStreaming(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__StartStopMulticastStreaming *ns1__StartMulticastStreaming){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__StartMulticastStreaming' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__StartMulticastStreaming(struct soap *soap, struct ns1__SetConfigurationResponse *ns1__StartMulticastStreamingResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__StopMulticastStreaming' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__StopMulticastStreaming(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__StartStopMulticastStreaming *ns1__StopMulticastStreaming, struct ns1__SetConfigurationResponse *ns1__StopMulticastStreamingResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__StopMulticastStreaming' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__StopMulticastStreaming(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct ns1__StartStopMulticastStreaming *ns1__StopMulticastStreaming){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__StopMulticastStreaming' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__StopMulticastStreaming(struct soap *soap, struct ns1__SetConfigurationResponse *ns1__StopMulticastStreamingResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__SetSynchronizationPoint' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__SetSynchronizationPoint(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetSynchronizationPoint *ns1__SetSynchronizationPoint, struct _ns1__SetSynchronizationPointResponse *ns1__SetSynchronizationPointResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__SetSynchronizationPoint' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__SetSynchronizationPoint(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetSynchronizationPoint *ns1__SetSynchronizationPoint){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__SetSynchronizationPoint' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__SetSynchronizationPoint(struct soap *soap, struct _ns1__SetSynchronizationPointResponse *ns1__SetSynchronizationPointResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetSnapshotUri' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetSnapshotUri(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetSnapshotUri *ns1__GetSnapshotUri, struct _ns1__GetSnapshotUriResponse *ns1__GetSnapshotUriResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetSnapshotUri' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetSnapshotUri(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetSnapshotUri *ns1__GetSnapshotUri){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetSnapshotUri' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetSnapshotUri(struct soap *soap, struct _ns1__GetSnapshotUriResponse *ns1__GetSnapshotUriResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetVideoSourceModes' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetVideoSourceModes(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetVideoSourceModes *ns1__GetVideoSourceModes, struct _ns1__GetVideoSourceModesResponse *ns1__GetVideoSourceModesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetVideoSourceModes' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetVideoSourceModes(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetVideoSourceModes *ns1__GetVideoSourceModes){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetVideoSourceModes' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetVideoSourceModes(struct soap *soap, struct _ns1__GetVideoSourceModesResponse *ns1__GetVideoSourceModesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__SetVideoSourceMode' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__SetVideoSourceMode(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetVideoSourceMode *ns1__SetVideoSourceMode, struct _ns1__SetVideoSourceModeResponse *ns1__SetVideoSourceModeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__SetVideoSourceMode' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__SetVideoSourceMode(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetVideoSourceMode *ns1__SetVideoSourceMode){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__SetVideoSourceMode' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__SetVideoSourceMode(struct soap *soap, struct _ns1__SetVideoSourceModeResponse *ns1__SetVideoSourceModeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetOSDs' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetOSDs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetOSDs *ns1__GetOSDs, struct _ns1__GetOSDsResponse *ns1__GetOSDsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetOSDs' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetOSDs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetOSDs *ns1__GetOSDs){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetOSDs' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetOSDs(struct soap *soap, struct _ns1__GetOSDsResponse *ns1__GetOSDsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetOSDOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetOSDOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetOSDOptions *ns1__GetOSDOptions, struct _ns1__GetOSDOptionsResponse *ns1__GetOSDOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetOSDOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetOSDOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetOSDOptions *ns1__GetOSDOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetOSDOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetOSDOptions(struct soap *soap, struct _ns1__GetOSDOptionsResponse *ns1__GetOSDOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__SetOSD' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__SetOSD(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetOSD *ns1__SetOSD, struct ns1__SetConfigurationResponse *ns1__SetOSDResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__SetOSD' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__SetOSD(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetOSD *ns1__SetOSD){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__SetOSD' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__SetOSD(struct soap *soap, struct ns1__SetConfigurationResponse *ns1__SetOSDResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__CreateOSD' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__CreateOSD(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__CreateOSD *ns1__CreateOSD, struct _ns1__CreateOSDResponse *ns1__CreateOSDResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__CreateOSD' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__CreateOSD(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__CreateOSD *ns1__CreateOSD){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__CreateOSD' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__CreateOSD(struct soap *soap, struct _ns1__CreateOSDResponse *ns1__CreateOSDResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__DeleteOSD' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__DeleteOSD(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__DeleteOSD *ns1__DeleteOSD, struct ns1__SetConfigurationResponse *ns1__DeleteOSDResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__DeleteOSD' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__DeleteOSD(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__DeleteOSD *ns1__DeleteOSD){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__DeleteOSD' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__DeleteOSD(struct soap *soap, struct ns1__SetConfigurationResponse *ns1__DeleteOSDResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetMasks' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetMasks(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetMasks *ns1__GetMasks, struct _ns1__GetMasksResponse *ns1__GetMasksResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetMasks' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetMasks(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetMasks *ns1__GetMasks){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetMasks' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetMasks(struct soap *soap, struct _ns1__GetMasksResponse *ns1__GetMasksResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__GetMaskOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__GetMaskOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetMaskOptions *ns1__GetMaskOptions, struct _ns1__GetMaskOptionsResponse *ns1__GetMaskOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__GetMaskOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__GetMaskOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__GetMaskOptions *ns1__GetMaskOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__GetMaskOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__GetMaskOptions(struct soap *soap, struct _ns1__GetMaskOptionsResponse *ns1__GetMaskOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__SetMask' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__SetMask(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetMask *ns1__SetMask, struct ns1__SetConfigurationResponse *ns1__SetMaskResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__SetMask' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__SetMask(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__SetMask *ns1__SetMask){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__SetMask' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__SetMask(struct soap *soap, struct ns1__SetConfigurationResponse *ns1__SetMaskResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__CreateMask' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__CreateMask(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__CreateMask *ns1__CreateMask, struct _ns1__CreateMaskResponse *ns1__CreateMaskResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__CreateMask' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__CreateMask(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__CreateMask *ns1__CreateMask){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__CreateMask' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__CreateMask(struct soap *soap, struct _ns1__CreateMaskResponse *ns1__CreateMaskResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___ns1__DeleteMask' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___ns1__DeleteMask(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__DeleteMask *ns1__DeleteMask, struct ns1__SetConfigurationResponse *ns1__DeleteMaskResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___ns1__DeleteMask' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___ns1__DeleteMask(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _ns1__DeleteMask *ns1__DeleteMask){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___ns1__DeleteMask' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___ns1__DeleteMask(struct soap *soap, struct ns1__SetConfigurationResponse *ns1__DeleteMaskResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

/******************************************************************************\
 *                                                                            *
 * Server-Side Operations                                                     *
 *                                                                            *
\******************************************************************************/

    /** Web service operation '__ns1__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetServiceCapabilities(struct soap* soap, struct _ns1__GetServiceCapabilities *ns1__GetServiceCapabilities, struct _ns1__GetServiceCapabilitiesResponse *ns1__GetServiceCapabilitiesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__CreateProfile' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__CreateProfile(struct soap* soap, struct _ns1__CreateProfile *ns1__CreateProfile, struct _ns1__CreateProfileResponse *ns1__CreateProfileResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetProfiles' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetProfiles(struct soap* soap, struct _ns1__GetProfiles *ns1__GetProfiles, struct _ns1__GetProfilesResponse *ns1__GetProfilesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__AddConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__AddConfiguration(struct soap* soap, struct _ns1__AddConfiguration *ns1__AddConfiguration, struct _ns1__AddConfigurationResponse *ns1__AddConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__RemoveConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__RemoveConfiguration(struct soap* soap, struct _ns1__RemoveConfiguration *ns1__RemoveConfiguration, struct _ns1__RemoveConfigurationResponse *ns1__RemoveConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__DeleteProfile' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__DeleteProfile(struct soap* soap, struct _ns1__DeleteProfile *ns1__DeleteProfile, struct _ns1__DeleteProfileResponse *ns1__DeleteProfileResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetVideoSourceConfigurations' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetVideoSourceConfigurations(struct soap* soap, struct ns1__GetConfiguration *ns1__GetVideoSourceConfigurations, struct _ns1__GetVideoSourceConfigurationsResponse *ns1__GetVideoSourceConfigurationsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetVideoEncoderConfigurations' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetVideoEncoderConfigurations(struct soap* soap, struct ns1__GetConfiguration *ns1__GetVideoEncoderConfigurations, struct _ns1__GetVideoEncoderConfigurationsResponse *ns1__GetVideoEncoderConfigurationsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetAudioSourceConfigurations' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAudioSourceConfigurations(struct soap* soap, struct ns1__GetConfiguration *ns1__GetAudioSourceConfigurations, struct _ns1__GetAudioSourceConfigurationsResponse *ns1__GetAudioSourceConfigurationsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetAudioEncoderConfigurations' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAudioEncoderConfigurations(struct soap* soap, struct ns1__GetConfiguration *ns1__GetAudioEncoderConfigurations, struct _ns1__GetAudioEncoderConfigurationsResponse *ns1__GetAudioEncoderConfigurationsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetAnalyticsConfigurations' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAnalyticsConfigurations(struct soap* soap, struct ns1__GetConfiguration *ns1__GetAnalyticsConfigurations, struct _ns1__GetAnalyticsConfigurationsResponse *ns1__GetAnalyticsConfigurationsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetMetadataConfigurations' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetMetadataConfigurations(struct soap* soap, struct ns1__GetConfiguration *ns1__GetMetadataConfigurations, struct _ns1__GetMetadataConfigurationsResponse *ns1__GetMetadataConfigurationsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetAudioOutputConfigurations' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAudioOutputConfigurations(struct soap* soap, struct ns1__GetConfiguration *ns1__GetAudioOutputConfigurations, struct _ns1__GetAudioOutputConfigurationsResponse *ns1__GetAudioOutputConfigurationsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetAudioDecoderConfigurations' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAudioDecoderConfigurations(struct soap* soap, struct ns1__GetConfiguration *ns1__GetAudioDecoderConfigurations, struct _ns1__GetAudioDecoderConfigurationsResponse *ns1__GetAudioDecoderConfigurationsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__SetVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetVideoSourceConfiguration(struct soap* soap, struct _ns1__SetVideoSourceConfiguration *ns1__SetVideoSourceConfiguration, struct ns1__SetConfigurationResponse *ns1__SetVideoSourceConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__SetVideoEncoderConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetVideoEncoderConfiguration(struct soap* soap, struct _ns1__SetVideoEncoderConfiguration *ns1__SetVideoEncoderConfiguration, struct ns1__SetConfigurationResponse *ns1__SetVideoEncoderConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__SetAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetAudioSourceConfiguration(struct soap* soap, struct _ns1__SetAudioSourceConfiguration *ns1__SetAudioSourceConfiguration, struct ns1__SetConfigurationResponse *ns1__SetAudioSourceConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__SetAudioEncoderConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetAudioEncoderConfiguration(struct soap* soap, struct _ns1__SetAudioEncoderConfiguration *ns1__SetAudioEncoderConfiguration, struct ns1__SetConfigurationResponse *ns1__SetAudioEncoderConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__SetMetadataConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetMetadataConfiguration(struct soap* soap, struct _ns1__SetMetadataConfiguration *ns1__SetMetadataConfiguration, struct ns1__SetConfigurationResponse *ns1__SetMetadataConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__SetAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetAudioOutputConfiguration(struct soap* soap, struct _ns1__SetAudioOutputConfiguration *ns1__SetAudioOutputConfiguration, struct ns1__SetConfigurationResponse *ns1__SetAudioOutputConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__SetAudioDecoderConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetAudioDecoderConfiguration(struct soap* soap, struct _ns1__SetAudioDecoderConfiguration *ns1__SetAudioDecoderConfiguration, struct ns1__SetConfigurationResponse *ns1__SetAudioDecoderConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetVideoSourceConfigurationOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetVideoSourceConfigurationOptions(struct soap* soap, struct ns1__GetConfiguration *ns1__GetVideoSourceConfigurationOptions, struct _ns1__GetVideoSourceConfigurationOptionsResponse *ns1__GetVideoSourceConfigurationOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetVideoEncoderConfigurationOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetVideoEncoderConfigurationOptions(struct soap* soap, struct ns1__GetConfiguration *ns1__GetVideoEncoderConfigurationOptions, struct _ns1__GetVideoEncoderConfigurationOptionsResponse *ns1__GetVideoEncoderConfigurationOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetAudioSourceConfigurationOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAudioSourceConfigurationOptions(struct soap* soap, struct ns1__GetConfiguration *ns1__GetAudioSourceConfigurationOptions, struct _ns1__GetAudioSourceConfigurationOptionsResponse *ns1__GetAudioSourceConfigurationOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetAudioEncoderConfigurationOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAudioEncoderConfigurationOptions(struct soap* soap, struct ns1__GetConfiguration *ns1__GetAudioEncoderConfigurationOptions, struct _ns1__GetAudioEncoderConfigurationOptionsResponse *ns1__GetAudioEncoderConfigurationOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetMetadataConfigurationOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetMetadataConfigurationOptions(struct soap* soap, struct ns1__GetConfiguration *ns1__GetMetadataConfigurationOptions, struct _ns1__GetMetadataConfigurationOptionsResponse *ns1__GetMetadataConfigurationOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetAudioOutputConfigurationOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAudioOutputConfigurationOptions(struct soap* soap, struct ns1__GetConfiguration *ns1__GetAudioOutputConfigurationOptions, struct _ns1__GetAudioOutputConfigurationOptionsResponse *ns1__GetAudioOutputConfigurationOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetAudioDecoderConfigurationOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetAudioDecoderConfigurationOptions(struct soap* soap, struct ns1__GetConfiguration *ns1__GetAudioDecoderConfigurationOptions, struct _ns1__GetAudioDecoderConfigurationOptionsResponse *ns1__GetAudioDecoderConfigurationOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetVideoEncoderInstances' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetVideoEncoderInstances(struct soap* soap, struct _ns1__GetVideoEncoderInstances *ns1__GetVideoEncoderInstances, struct _ns1__GetVideoEncoderInstancesResponse *ns1__GetVideoEncoderInstancesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetStreamUri' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetStreamUri(struct soap* soap, struct _ns1__GetStreamUri *ns1__GetStreamUri, struct _ns1__GetStreamUriResponse *ns1__GetStreamUriResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__StartMulticastStreaming' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__StartMulticastStreaming(struct soap* soap, struct ns1__StartStopMulticastStreaming *ns1__StartMulticastStreaming, struct ns1__SetConfigurationResponse *ns1__StartMulticastStreamingResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__StopMulticastStreaming' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__StopMulticastStreaming(struct soap* soap, struct ns1__StartStopMulticastStreaming *ns1__StopMulticastStreaming, struct ns1__SetConfigurationResponse *ns1__StopMulticastStreamingResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__SetSynchronizationPoint' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetSynchronizationPoint(struct soap* soap, struct _ns1__SetSynchronizationPoint *ns1__SetSynchronizationPoint, struct _ns1__SetSynchronizationPointResponse *ns1__SetSynchronizationPointResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetSnapshotUri' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetSnapshotUri(struct soap* soap, struct _ns1__GetSnapshotUri *ns1__GetSnapshotUri, struct _ns1__GetSnapshotUriResponse *ns1__GetSnapshotUriResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetVideoSourceModes' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetVideoSourceModes(struct soap* soap, struct _ns1__GetVideoSourceModes *ns1__GetVideoSourceModes, struct _ns1__GetVideoSourceModesResponse *ns1__GetVideoSourceModesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__SetVideoSourceMode' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetVideoSourceMode(struct soap* soap, struct _ns1__SetVideoSourceMode *ns1__SetVideoSourceMode, struct _ns1__SetVideoSourceModeResponse *ns1__SetVideoSourceModeResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetOSDs' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetOSDs(struct soap* soap, struct _ns1__GetOSDs *ns1__GetOSDs, struct _ns1__GetOSDsResponse *ns1__GetOSDsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetOSDOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetOSDOptions(struct soap* soap, struct _ns1__GetOSDOptions *ns1__GetOSDOptions, struct _ns1__GetOSDOptionsResponse *ns1__GetOSDOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__SetOSD' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetOSD(struct soap* soap, struct _ns1__SetOSD *ns1__SetOSD, struct ns1__SetConfigurationResponse *ns1__SetOSDResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__CreateOSD' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__CreateOSD(struct soap* soap, struct _ns1__CreateOSD *ns1__CreateOSD, struct _ns1__CreateOSDResponse *ns1__CreateOSDResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__DeleteOSD' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__DeleteOSD(struct soap* soap, struct _ns1__DeleteOSD *ns1__DeleteOSD, struct ns1__SetConfigurationResponse *ns1__DeleteOSDResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetMasks' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetMasks(struct soap* soap, struct _ns1__GetMasks *ns1__GetMasks, struct _ns1__GetMasksResponse *ns1__GetMasksResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__GetMaskOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__GetMaskOptions(struct soap* soap, struct _ns1__GetMaskOptions *ns1__GetMaskOptions, struct _ns1__GetMaskOptionsResponse *ns1__GetMaskOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__SetMask' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__SetMask(struct soap* soap, struct _ns1__SetMask *ns1__SetMask, struct ns1__SetConfigurationResponse *ns1__SetMaskResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__CreateMask' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__CreateMask(struct soap* soap, struct _ns1__CreateMask *ns1__CreateMask, struct _ns1__CreateMaskResponse *ns1__CreateMaskResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__ns1__DeleteMask' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __ns1__DeleteMask(struct soap* soap, struct _ns1__DeleteMask *ns1__DeleteMask, struct ns1__SetConfigurationResponse *ns1__DeleteMaskResponse){
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

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetServiceCapabilities(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__CreateProfile(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetProfiles(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__AddConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__RemoveConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__DeleteProfile(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetVideoSourceConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetVideoEncoderConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetAudioSourceConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetAudioEncoderConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetAnalyticsConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetMetadataConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetAudioOutputConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetAudioDecoderConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__SetVideoSourceConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__SetVideoEncoderConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__SetAudioSourceConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__SetAudioEncoderConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__SetMetadataConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__SetAudioOutputConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__SetAudioDecoderConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetVideoSourceConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetVideoEncoderConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetAudioSourceConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetAudioEncoderConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetMetadataConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetAudioOutputConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetAudioDecoderConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetVideoEncoderInstances(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetStreamUri(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__StartMulticastStreaming(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__StopMulticastStreaming(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__SetSynchronizationPoint(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetSnapshotUri(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetVideoSourceModes(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__SetVideoSourceMode(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetOSDs(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetOSDOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__SetOSD(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__CreateOSD(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__DeleteOSD(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetMasks(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__GetMaskOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__SetMask(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__CreateMask(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___ns1__DeleteMask(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
