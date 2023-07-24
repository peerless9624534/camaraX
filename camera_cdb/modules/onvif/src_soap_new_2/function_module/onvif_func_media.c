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
// /******************************************************************************\
//  *                                                                            *
//  * Client-Side Call Stub Functions                                            *
//  *                                                                            *
// \******************************************************************************/

    
//     /** Web service synchronous operation 'soap_call___trt__GetServiceCapabilities' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetServiceCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetServiceCapabilities *trt__GetServiceCapabilities, struct _trt__GetServiceCapabilitiesResponse *trt__GetServiceCapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetServiceCapabilities' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetServiceCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetServiceCapabilities *trt__GetServiceCapabilities){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetServiceCapabilities' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetServiceCapabilities(struct soap *soap, struct _trt__GetServiceCapabilitiesResponse *trt__GetServiceCapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetVideoSources' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetVideoSources(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoSources *trt__GetVideoSources, struct _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetVideoSources' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetVideoSources(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoSources *trt__GetVideoSources){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetVideoSources' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetVideoSources(struct soap *soap, struct _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetAudioSources' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetAudioSources(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioSources *trt__GetAudioSources, struct _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetAudioSources' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetAudioSources(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioSources *trt__GetAudioSources){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetAudioSources' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetAudioSources(struct soap *soap, struct _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetAudioOutputs' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetAudioOutputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioOutputs *trt__GetAudioOutputs, struct _trt__GetAudioOutputsResponse *trt__GetAudioOutputsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetAudioOutputs' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetAudioOutputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioOutputs *trt__GetAudioOutputs){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetAudioOutputs' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetAudioOutputs(struct soap *soap, struct _trt__GetAudioOutputsResponse *trt__GetAudioOutputsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__CreateProfile' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__CreateProfile(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__CreateProfile *trt__CreateProfile, struct _trt__CreateProfileResponse *trt__CreateProfileResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__CreateProfile' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__CreateProfile(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__CreateProfile *trt__CreateProfile){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__CreateProfile' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__CreateProfile(struct soap *soap, struct _trt__CreateProfileResponse *trt__CreateProfileResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetProfile' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetProfile(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetProfile *trt__GetProfile, struct _trt__GetProfileResponse *trt__GetProfileResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetProfile' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetProfile(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetProfile *trt__GetProfile){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetProfile' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetProfile(struct soap *soap, struct _trt__GetProfileResponse *trt__GetProfileResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetProfiles' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetProfiles(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetProfiles *trt__GetProfiles, struct _trt__GetProfilesResponse *trt__GetProfilesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetProfiles' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetProfiles(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetProfiles *trt__GetProfiles){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetProfiles' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetProfiles(struct soap *soap, struct _trt__GetProfilesResponse *trt__GetProfilesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__AddVideoEncoderConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__AddVideoEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__AddVideoEncoderConfiguration *trt__AddVideoEncoderConfiguration, struct _trt__AddVideoEncoderConfigurationResponse *trt__AddVideoEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__AddVideoEncoderConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__AddVideoEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__AddVideoEncoderConfiguration *trt__AddVideoEncoderConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__AddVideoEncoderConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__AddVideoEncoderConfiguration(struct soap *soap, struct _trt__AddVideoEncoderConfigurationResponse *trt__AddVideoEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__AddVideoSourceConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__AddVideoSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__AddVideoSourceConfiguration *trt__AddVideoSourceConfiguration, struct _trt__AddVideoSourceConfigurationResponse *trt__AddVideoSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__AddVideoSourceConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__AddVideoSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__AddVideoSourceConfiguration *trt__AddVideoSourceConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__AddVideoSourceConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__AddVideoSourceConfiguration(struct soap *soap, struct _trt__AddVideoSourceConfigurationResponse *trt__AddVideoSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__AddAudioEncoderConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__AddAudioEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__AddAudioEncoderConfiguration *trt__AddAudioEncoderConfiguration, struct _trt__AddAudioEncoderConfigurationResponse *trt__AddAudioEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__AddAudioEncoderConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__AddAudioEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__AddAudioEncoderConfiguration *trt__AddAudioEncoderConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__AddAudioEncoderConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__AddAudioEncoderConfiguration(struct soap *soap, struct _trt__AddAudioEncoderConfigurationResponse *trt__AddAudioEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__AddAudioSourceConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__AddAudioSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__AddAudioSourceConfiguration *trt__AddAudioSourceConfiguration, struct _trt__AddAudioSourceConfigurationResponse *trt__AddAudioSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__AddAudioSourceConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__AddAudioSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__AddAudioSourceConfiguration *trt__AddAudioSourceConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__AddAudioSourceConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__AddAudioSourceConfiguration(struct soap *soap, struct _trt__AddAudioSourceConfigurationResponse *trt__AddAudioSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__AddPTZConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__AddPTZConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__AddPTZConfiguration *trt__AddPTZConfiguration, struct _trt__AddPTZConfigurationResponse *trt__AddPTZConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__AddPTZConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__AddPTZConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__AddPTZConfiguration *trt__AddPTZConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__AddPTZConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__AddPTZConfiguration(struct soap *soap, struct _trt__AddPTZConfigurationResponse *trt__AddPTZConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__AddVideoAnalyticsConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__AddVideoAnalyticsConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration, struct _trt__AddVideoAnalyticsConfigurationResponse *trt__AddVideoAnalyticsConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__AddVideoAnalyticsConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__AddVideoAnalyticsConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__AddVideoAnalyticsConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__AddVideoAnalyticsConfiguration(struct soap *soap, struct _trt__AddVideoAnalyticsConfigurationResponse *trt__AddVideoAnalyticsConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__AddMetadataConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__AddMetadataConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__AddMetadataConfiguration *trt__AddMetadataConfiguration, struct _trt__AddMetadataConfigurationResponse *trt__AddMetadataConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__AddMetadataConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__AddMetadataConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__AddMetadataConfiguration *trt__AddMetadataConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__AddMetadataConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__AddMetadataConfiguration(struct soap *soap, struct _trt__AddMetadataConfigurationResponse *trt__AddMetadataConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__AddAudioOutputConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__AddAudioOutputConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__AddAudioOutputConfiguration *trt__AddAudioOutputConfiguration, struct _trt__AddAudioOutputConfigurationResponse *trt__AddAudioOutputConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__AddAudioOutputConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__AddAudioOutputConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__AddAudioOutputConfiguration *trt__AddAudioOutputConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__AddAudioOutputConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__AddAudioOutputConfiguration(struct soap *soap, struct _trt__AddAudioOutputConfigurationResponse *trt__AddAudioOutputConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__AddAudioDecoderConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__AddAudioDecoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__AddAudioDecoderConfiguration *trt__AddAudioDecoderConfiguration, struct _trt__AddAudioDecoderConfigurationResponse *trt__AddAudioDecoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__AddAudioDecoderConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__AddAudioDecoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__AddAudioDecoderConfiguration *trt__AddAudioDecoderConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__AddAudioDecoderConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__AddAudioDecoderConfiguration(struct soap *soap, struct _trt__AddAudioDecoderConfigurationResponse *trt__AddAudioDecoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__RemoveVideoEncoderConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__RemoveVideoEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__RemoveVideoEncoderConfiguration *trt__RemoveVideoEncoderConfiguration, struct _trt__RemoveVideoEncoderConfigurationResponse *trt__RemoveVideoEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__RemoveVideoEncoderConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__RemoveVideoEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__RemoveVideoEncoderConfiguration *trt__RemoveVideoEncoderConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__RemoveVideoEncoderConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__RemoveVideoEncoderConfiguration(struct soap *soap, struct _trt__RemoveVideoEncoderConfigurationResponse *trt__RemoveVideoEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__RemoveVideoSourceConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__RemoveVideoSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__RemoveVideoSourceConfiguration *trt__RemoveVideoSourceConfiguration, struct _trt__RemoveVideoSourceConfigurationResponse *trt__RemoveVideoSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__RemoveVideoSourceConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__RemoveVideoSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__RemoveVideoSourceConfiguration *trt__RemoveVideoSourceConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__RemoveVideoSourceConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__RemoveVideoSourceConfiguration(struct soap *soap, struct _trt__RemoveVideoSourceConfigurationResponse *trt__RemoveVideoSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__RemoveAudioEncoderConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__RemoveAudioEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__RemoveAudioEncoderConfiguration *trt__RemoveAudioEncoderConfiguration, struct _trt__RemoveAudioEncoderConfigurationResponse *trt__RemoveAudioEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__RemoveAudioEncoderConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__RemoveAudioEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__RemoveAudioEncoderConfiguration *trt__RemoveAudioEncoderConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__RemoveAudioEncoderConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__RemoveAudioEncoderConfiguration(struct soap *soap, struct _trt__RemoveAudioEncoderConfigurationResponse *trt__RemoveAudioEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__RemoveAudioSourceConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__RemoveAudioSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__RemoveAudioSourceConfiguration *trt__RemoveAudioSourceConfiguration, struct _trt__RemoveAudioSourceConfigurationResponse *trt__RemoveAudioSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__RemoveAudioSourceConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__RemoveAudioSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__RemoveAudioSourceConfiguration *trt__RemoveAudioSourceConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__RemoveAudioSourceConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__RemoveAudioSourceConfiguration(struct soap *soap, struct _trt__RemoveAudioSourceConfigurationResponse *trt__RemoveAudioSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__RemovePTZConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__RemovePTZConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__RemovePTZConfiguration *trt__RemovePTZConfiguration, struct _trt__RemovePTZConfigurationResponse *trt__RemovePTZConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__RemovePTZConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__RemovePTZConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__RemovePTZConfiguration *trt__RemovePTZConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__RemovePTZConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__RemovePTZConfiguration(struct soap *soap, struct _trt__RemovePTZConfigurationResponse *trt__RemovePTZConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__RemoveVideoAnalyticsConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__RemoveVideoAnalyticsConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__RemoveVideoAnalyticsConfiguration *trt__RemoveVideoAnalyticsConfiguration, struct _trt__RemoveVideoAnalyticsConfigurationResponse *trt__RemoveVideoAnalyticsConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__RemoveVideoAnalyticsConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__RemoveVideoAnalyticsConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__RemoveVideoAnalyticsConfiguration *trt__RemoveVideoAnalyticsConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__RemoveVideoAnalyticsConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__RemoveVideoAnalyticsConfiguration(struct soap *soap, struct _trt__RemoveVideoAnalyticsConfigurationResponse *trt__RemoveVideoAnalyticsConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__RemoveMetadataConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__RemoveMetadataConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__RemoveMetadataConfiguration *trt__RemoveMetadataConfiguration, struct _trt__RemoveMetadataConfigurationResponse *trt__RemoveMetadataConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__RemoveMetadataConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__RemoveMetadataConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__RemoveMetadataConfiguration *trt__RemoveMetadataConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__RemoveMetadataConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__RemoveMetadataConfiguration(struct soap *soap, struct _trt__RemoveMetadataConfigurationResponse *trt__RemoveMetadataConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__RemoveAudioOutputConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__RemoveAudioOutputConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__RemoveAudioOutputConfiguration *trt__RemoveAudioOutputConfiguration, struct _trt__RemoveAudioOutputConfigurationResponse *trt__RemoveAudioOutputConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__RemoveAudioOutputConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__RemoveAudioOutputConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__RemoveAudioOutputConfiguration *trt__RemoveAudioOutputConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__RemoveAudioOutputConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__RemoveAudioOutputConfiguration(struct soap *soap, struct _trt__RemoveAudioOutputConfigurationResponse *trt__RemoveAudioOutputConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__RemoveAudioDecoderConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__RemoveAudioDecoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__RemoveAudioDecoderConfiguration *trt__RemoveAudioDecoderConfiguration, struct _trt__RemoveAudioDecoderConfigurationResponse *trt__RemoveAudioDecoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__RemoveAudioDecoderConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__RemoveAudioDecoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__RemoveAudioDecoderConfiguration *trt__RemoveAudioDecoderConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__RemoveAudioDecoderConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__RemoveAudioDecoderConfiguration(struct soap *soap, struct _trt__RemoveAudioDecoderConfigurationResponse *trt__RemoveAudioDecoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__DeleteProfile' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__DeleteProfile(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__DeleteProfile *trt__DeleteProfile, struct _trt__DeleteProfileResponse *trt__DeleteProfileResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__DeleteProfile' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__DeleteProfile(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__DeleteProfile *trt__DeleteProfile){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__DeleteProfile' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__DeleteProfile(struct soap *soap, struct _trt__DeleteProfileResponse *trt__DeleteProfileResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetVideoSourceConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetVideoSourceConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoSourceConfigurations *trt__GetVideoSourceConfigurations, struct _trt__GetVideoSourceConfigurationsResponse *trt__GetVideoSourceConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetVideoSourceConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetVideoSourceConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoSourceConfigurations *trt__GetVideoSourceConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetVideoSourceConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetVideoSourceConfigurations(struct soap *soap, struct _trt__GetVideoSourceConfigurationsResponse *trt__GetVideoSourceConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetVideoEncoderConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetVideoEncoderConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations, struct _trt__GetVideoEncoderConfigurationsResponse *trt__GetVideoEncoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetVideoEncoderConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetVideoEncoderConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetVideoEncoderConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetVideoEncoderConfigurations(struct soap *soap, struct _trt__GetVideoEncoderConfigurationsResponse *trt__GetVideoEncoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetAudioSourceConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetAudioSourceConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioSourceConfigurations *trt__GetAudioSourceConfigurations, struct _trt__GetAudioSourceConfigurationsResponse *trt__GetAudioSourceConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetAudioSourceConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetAudioSourceConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioSourceConfigurations *trt__GetAudioSourceConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetAudioSourceConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetAudioSourceConfigurations(struct soap *soap, struct _trt__GetAudioSourceConfigurationsResponse *trt__GetAudioSourceConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetAudioEncoderConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetAudioEncoderConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioEncoderConfigurations *trt__GetAudioEncoderConfigurations, struct _trt__GetAudioEncoderConfigurationsResponse *trt__GetAudioEncoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetAudioEncoderConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetAudioEncoderConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioEncoderConfigurations *trt__GetAudioEncoderConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetAudioEncoderConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetAudioEncoderConfigurations(struct soap *soap, struct _trt__GetAudioEncoderConfigurationsResponse *trt__GetAudioEncoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetVideoAnalyticsConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetVideoAnalyticsConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoAnalyticsConfigurations *trt__GetVideoAnalyticsConfigurations, struct _trt__GetVideoAnalyticsConfigurationsResponse *trt__GetVideoAnalyticsConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetVideoAnalyticsConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetVideoAnalyticsConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoAnalyticsConfigurations *trt__GetVideoAnalyticsConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetVideoAnalyticsConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetVideoAnalyticsConfigurations(struct soap *soap, struct _trt__GetVideoAnalyticsConfigurationsResponse *trt__GetVideoAnalyticsConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetMetadataConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetMetadataConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetMetadataConfigurations *trt__GetMetadataConfigurations, struct _trt__GetMetadataConfigurationsResponse *trt__GetMetadataConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetMetadataConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetMetadataConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetMetadataConfigurations *trt__GetMetadataConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetMetadataConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetMetadataConfigurations(struct soap *soap, struct _trt__GetMetadataConfigurationsResponse *trt__GetMetadataConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetAudioOutputConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetAudioOutputConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioOutputConfigurations *trt__GetAudioOutputConfigurations, struct _trt__GetAudioOutputConfigurationsResponse *trt__GetAudioOutputConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetAudioOutputConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetAudioOutputConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioOutputConfigurations *trt__GetAudioOutputConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetAudioOutputConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetAudioOutputConfigurations(struct soap *soap, struct _trt__GetAudioOutputConfigurationsResponse *trt__GetAudioOutputConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetAudioDecoderConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetAudioDecoderConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioDecoderConfigurations *trt__GetAudioDecoderConfigurations, struct _trt__GetAudioDecoderConfigurationsResponse *trt__GetAudioDecoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetAudioDecoderConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetAudioDecoderConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioDecoderConfigurations *trt__GetAudioDecoderConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetAudioDecoderConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetAudioDecoderConfigurations(struct soap *soap, struct _trt__GetAudioDecoderConfigurationsResponse *trt__GetAudioDecoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetVideoSourceConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetVideoSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoSourceConfiguration *trt__GetVideoSourceConfiguration, struct _trt__GetVideoSourceConfigurationResponse *trt__GetVideoSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetVideoSourceConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetVideoSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoSourceConfiguration *trt__GetVideoSourceConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetVideoSourceConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetVideoSourceConfiguration(struct soap *soap, struct _trt__GetVideoSourceConfigurationResponse *trt__GetVideoSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetVideoEncoderConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetVideoEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration, struct _trt__GetVideoEncoderConfigurationResponse *trt__GetVideoEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetVideoEncoderConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetVideoEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetVideoEncoderConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetVideoEncoderConfiguration(struct soap *soap, struct _trt__GetVideoEncoderConfigurationResponse *trt__GetVideoEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetAudioSourceConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetAudioSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioSourceConfiguration *trt__GetAudioSourceConfiguration, struct _trt__GetAudioSourceConfigurationResponse *trt__GetAudioSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetAudioSourceConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetAudioSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioSourceConfiguration *trt__GetAudioSourceConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetAudioSourceConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetAudioSourceConfiguration(struct soap *soap, struct _trt__GetAudioSourceConfigurationResponse *trt__GetAudioSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetAudioEncoderConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetAudioEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioEncoderConfiguration *trt__GetAudioEncoderConfiguration, struct _trt__GetAudioEncoderConfigurationResponse *trt__GetAudioEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetAudioEncoderConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetAudioEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioEncoderConfiguration *trt__GetAudioEncoderConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetAudioEncoderConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetAudioEncoderConfiguration(struct soap *soap, struct _trt__GetAudioEncoderConfigurationResponse *trt__GetAudioEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetVideoAnalyticsConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetVideoAnalyticsConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoAnalyticsConfiguration *trt__GetVideoAnalyticsConfiguration, struct _trt__GetVideoAnalyticsConfigurationResponse *trt__GetVideoAnalyticsConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetVideoAnalyticsConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetVideoAnalyticsConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoAnalyticsConfiguration *trt__GetVideoAnalyticsConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetVideoAnalyticsConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetVideoAnalyticsConfiguration(struct soap *soap, struct _trt__GetVideoAnalyticsConfigurationResponse *trt__GetVideoAnalyticsConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetMetadataConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetMetadataConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetMetadataConfiguration *trt__GetMetadataConfiguration, struct _trt__GetMetadataConfigurationResponse *trt__GetMetadataConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetMetadataConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetMetadataConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetMetadataConfiguration *trt__GetMetadataConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetMetadataConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetMetadataConfiguration(struct soap *soap, struct _trt__GetMetadataConfigurationResponse *trt__GetMetadataConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetAudioOutputConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetAudioOutputConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioOutputConfiguration *trt__GetAudioOutputConfiguration, struct _trt__GetAudioOutputConfigurationResponse *trt__GetAudioOutputConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetAudioOutputConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetAudioOutputConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioOutputConfiguration *trt__GetAudioOutputConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetAudioOutputConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetAudioOutputConfiguration(struct soap *soap, struct _trt__GetAudioOutputConfigurationResponse *trt__GetAudioOutputConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetAudioDecoderConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetAudioDecoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioDecoderConfiguration *trt__GetAudioDecoderConfiguration, struct _trt__GetAudioDecoderConfigurationResponse *trt__GetAudioDecoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetAudioDecoderConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetAudioDecoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioDecoderConfiguration *trt__GetAudioDecoderConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetAudioDecoderConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetAudioDecoderConfiguration(struct soap *soap, struct _trt__GetAudioDecoderConfigurationResponse *trt__GetAudioDecoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetCompatibleVideoEncoderConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetCompatibleVideoEncoderConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations, struct _trt__GetCompatibleVideoEncoderConfigurationsResponse *trt__GetCompatibleVideoEncoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetCompatibleVideoEncoderConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetCompatibleVideoEncoderConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetCompatibleVideoEncoderConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetCompatibleVideoEncoderConfigurations(struct soap *soap, struct _trt__GetCompatibleVideoEncoderConfigurationsResponse *trt__GetCompatibleVideoEncoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetCompatibleVideoSourceConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetCompatibleVideoSourceConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetCompatibleVideoSourceConfigurations *trt__GetCompatibleVideoSourceConfigurations, struct _trt__GetCompatibleVideoSourceConfigurationsResponse *trt__GetCompatibleVideoSourceConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetCompatibleVideoSourceConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetCompatibleVideoSourceConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetCompatibleVideoSourceConfigurations *trt__GetCompatibleVideoSourceConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetCompatibleVideoSourceConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetCompatibleVideoSourceConfigurations(struct soap *soap, struct _trt__GetCompatibleVideoSourceConfigurationsResponse *trt__GetCompatibleVideoSourceConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetCompatibleAudioEncoderConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetCompatibleAudioEncoderConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetCompatibleAudioEncoderConfigurations *trt__GetCompatibleAudioEncoderConfigurations, struct _trt__GetCompatibleAudioEncoderConfigurationsResponse *trt__GetCompatibleAudioEncoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetCompatibleAudioEncoderConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetCompatibleAudioEncoderConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetCompatibleAudioEncoderConfigurations *trt__GetCompatibleAudioEncoderConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetCompatibleAudioEncoderConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetCompatibleAudioEncoderConfigurations(struct soap *soap, struct _trt__GetCompatibleAudioEncoderConfigurationsResponse *trt__GetCompatibleAudioEncoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetCompatibleAudioSourceConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetCompatibleAudioSourceConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetCompatibleAudioSourceConfigurations *trt__GetCompatibleAudioSourceConfigurations, struct _trt__GetCompatibleAudioSourceConfigurationsResponse *trt__GetCompatibleAudioSourceConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetCompatibleAudioSourceConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetCompatibleAudioSourceConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetCompatibleAudioSourceConfigurations *trt__GetCompatibleAudioSourceConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetCompatibleAudioSourceConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetCompatibleAudioSourceConfigurations(struct soap *soap, struct _trt__GetCompatibleAudioSourceConfigurationsResponse *trt__GetCompatibleAudioSourceConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetCompatibleVideoAnalyticsConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetCompatibleVideoAnalyticsConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations, struct _trt__GetCompatibleVideoAnalyticsConfigurationsResponse *trt__GetCompatibleVideoAnalyticsConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetCompatibleVideoAnalyticsConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetCompatibleVideoAnalyticsConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetCompatibleVideoAnalyticsConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetCompatibleVideoAnalyticsConfigurations(struct soap *soap, struct _trt__GetCompatibleVideoAnalyticsConfigurationsResponse *trt__GetCompatibleVideoAnalyticsConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetCompatibleMetadataConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetCompatibleMetadataConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetCompatibleMetadataConfigurations *trt__GetCompatibleMetadataConfigurations, struct _trt__GetCompatibleMetadataConfigurationsResponse *trt__GetCompatibleMetadataConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetCompatibleMetadataConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetCompatibleMetadataConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetCompatibleMetadataConfigurations *trt__GetCompatibleMetadataConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetCompatibleMetadataConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetCompatibleMetadataConfigurations(struct soap *soap, struct _trt__GetCompatibleMetadataConfigurationsResponse *trt__GetCompatibleMetadataConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetCompatibleAudioOutputConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetCompatibleAudioOutputConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetCompatibleAudioOutputConfigurations *trt__GetCompatibleAudioOutputConfigurations, struct _trt__GetCompatibleAudioOutputConfigurationsResponse *trt__GetCompatibleAudioOutputConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetCompatibleAudioOutputConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetCompatibleAudioOutputConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetCompatibleAudioOutputConfigurations *trt__GetCompatibleAudioOutputConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetCompatibleAudioOutputConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetCompatibleAudioOutputConfigurations(struct soap *soap, struct _trt__GetCompatibleAudioOutputConfigurationsResponse *trt__GetCompatibleAudioOutputConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetCompatibleAudioDecoderConfigurations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetCompatibleAudioDecoderConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetCompatibleAudioDecoderConfigurations *trt__GetCompatibleAudioDecoderConfigurations, struct _trt__GetCompatibleAudioDecoderConfigurationsResponse *trt__GetCompatibleAudioDecoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetCompatibleAudioDecoderConfigurations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetCompatibleAudioDecoderConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetCompatibleAudioDecoderConfigurations *trt__GetCompatibleAudioDecoderConfigurations){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetCompatibleAudioDecoderConfigurations' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetCompatibleAudioDecoderConfigurations(struct soap *soap, struct _trt__GetCompatibleAudioDecoderConfigurationsResponse *trt__GetCompatibleAudioDecoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__SetVideoSourceConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__SetVideoSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetVideoSourceConfiguration *trt__SetVideoSourceConfiguration, struct _trt__SetVideoSourceConfigurationResponse *trt__SetVideoSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__SetVideoSourceConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__SetVideoSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetVideoSourceConfiguration *trt__SetVideoSourceConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__SetVideoSourceConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__SetVideoSourceConfiguration(struct soap *soap, struct _trt__SetVideoSourceConfigurationResponse *trt__SetVideoSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__SetVideoEncoderConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__SetVideoEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration, struct _trt__SetVideoEncoderConfigurationResponse *trt__SetVideoEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__SetVideoEncoderConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__SetVideoEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__SetVideoEncoderConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__SetVideoEncoderConfiguration(struct soap *soap, struct _trt__SetVideoEncoderConfigurationResponse *trt__SetVideoEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__SetAudioSourceConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__SetAudioSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetAudioSourceConfiguration *trt__SetAudioSourceConfiguration, struct _trt__SetAudioSourceConfigurationResponse *trt__SetAudioSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__SetAudioSourceConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__SetAudioSourceConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetAudioSourceConfiguration *trt__SetAudioSourceConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__SetAudioSourceConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__SetAudioSourceConfiguration(struct soap *soap, struct _trt__SetAudioSourceConfigurationResponse *trt__SetAudioSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__SetAudioEncoderConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__SetAudioEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetAudioEncoderConfiguration *trt__SetAudioEncoderConfiguration, struct _trt__SetAudioEncoderConfigurationResponse *trt__SetAudioEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__SetAudioEncoderConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__SetAudioEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetAudioEncoderConfiguration *trt__SetAudioEncoderConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__SetAudioEncoderConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__SetAudioEncoderConfiguration(struct soap *soap, struct _trt__SetAudioEncoderConfigurationResponse *trt__SetAudioEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__SetVideoAnalyticsConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__SetVideoAnalyticsConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetVideoAnalyticsConfiguration *trt__SetVideoAnalyticsConfiguration, struct _trt__SetVideoAnalyticsConfigurationResponse *trt__SetVideoAnalyticsConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__SetVideoAnalyticsConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__SetVideoAnalyticsConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetVideoAnalyticsConfiguration *trt__SetVideoAnalyticsConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__SetVideoAnalyticsConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__SetVideoAnalyticsConfiguration(struct soap *soap, struct _trt__SetVideoAnalyticsConfigurationResponse *trt__SetVideoAnalyticsConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__SetMetadataConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__SetMetadataConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetMetadataConfiguration *trt__SetMetadataConfiguration, struct _trt__SetMetadataConfigurationResponse *trt__SetMetadataConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__SetMetadataConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__SetMetadataConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetMetadataConfiguration *trt__SetMetadataConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__SetMetadataConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__SetMetadataConfiguration(struct soap *soap, struct _trt__SetMetadataConfigurationResponse *trt__SetMetadataConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__SetAudioOutputConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__SetAudioOutputConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetAudioOutputConfiguration *trt__SetAudioOutputConfiguration, struct _trt__SetAudioOutputConfigurationResponse *trt__SetAudioOutputConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__SetAudioOutputConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__SetAudioOutputConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetAudioOutputConfiguration *trt__SetAudioOutputConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__SetAudioOutputConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__SetAudioOutputConfiguration(struct soap *soap, struct _trt__SetAudioOutputConfigurationResponse *trt__SetAudioOutputConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__SetAudioDecoderConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__SetAudioDecoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetAudioDecoderConfiguration *trt__SetAudioDecoderConfiguration, struct _trt__SetAudioDecoderConfigurationResponse *trt__SetAudioDecoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__SetAudioDecoderConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__SetAudioDecoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetAudioDecoderConfiguration *trt__SetAudioDecoderConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__SetAudioDecoderConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__SetAudioDecoderConfiguration(struct soap *soap, struct _trt__SetAudioDecoderConfigurationResponse *trt__SetAudioDecoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetVideoSourceConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetVideoSourceConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoSourceConfigurationOptions *trt__GetVideoSourceConfigurationOptions, struct _trt__GetVideoSourceConfigurationOptionsResponse *trt__GetVideoSourceConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetVideoSourceConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetVideoSourceConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoSourceConfigurationOptions *trt__GetVideoSourceConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetVideoSourceConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetVideoSourceConfigurationOptions(struct soap *soap, struct _trt__GetVideoSourceConfigurationOptionsResponse *trt__GetVideoSourceConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetVideoEncoderConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetVideoEncoderConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions, struct _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetVideoEncoderConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetVideoEncoderConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetVideoEncoderConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetVideoEncoderConfigurationOptions(struct soap *soap, struct _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetAudioSourceConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetAudioSourceConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioSourceConfigurationOptions *trt__GetAudioSourceConfigurationOptions, struct _trt__GetAudioSourceConfigurationOptionsResponse *trt__GetAudioSourceConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetAudioSourceConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetAudioSourceConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioSourceConfigurationOptions *trt__GetAudioSourceConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetAudioSourceConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetAudioSourceConfigurationOptions(struct soap *soap, struct _trt__GetAudioSourceConfigurationOptionsResponse *trt__GetAudioSourceConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetAudioEncoderConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetAudioEncoderConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions, struct _trt__GetAudioEncoderConfigurationOptionsResponse *trt__GetAudioEncoderConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetAudioEncoderConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetAudioEncoderConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetAudioEncoderConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetAudioEncoderConfigurationOptions(struct soap *soap, struct _trt__GetAudioEncoderConfigurationOptionsResponse *trt__GetAudioEncoderConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetMetadataConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetMetadataConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetMetadataConfigurationOptions *trt__GetMetadataConfigurationOptions, struct _trt__GetMetadataConfigurationOptionsResponse *trt__GetMetadataConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetMetadataConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetMetadataConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetMetadataConfigurationOptions *trt__GetMetadataConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetMetadataConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetMetadataConfigurationOptions(struct soap *soap, struct _trt__GetMetadataConfigurationOptionsResponse *trt__GetMetadataConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetAudioOutputConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetAudioOutputConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioOutputConfigurationOptions *trt__GetAudioOutputConfigurationOptions, struct _trt__GetAudioOutputConfigurationOptionsResponse *trt__GetAudioOutputConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetAudioOutputConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetAudioOutputConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioOutputConfigurationOptions *trt__GetAudioOutputConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetAudioOutputConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetAudioOutputConfigurationOptions(struct soap *soap, struct _trt__GetAudioOutputConfigurationOptionsResponse *trt__GetAudioOutputConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetAudioDecoderConfigurationOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetAudioDecoderConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioDecoderConfigurationOptions *trt__GetAudioDecoderConfigurationOptions, struct _trt__GetAudioDecoderConfigurationOptionsResponse *trt__GetAudioDecoderConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetAudioDecoderConfigurationOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetAudioDecoderConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetAudioDecoderConfigurationOptions *trt__GetAudioDecoderConfigurationOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetAudioDecoderConfigurationOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetAudioDecoderConfigurationOptions(struct soap *soap, struct _trt__GetAudioDecoderConfigurationOptionsResponse *trt__GetAudioDecoderConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetGuaranteedNumberOfVideoEncoderInstances' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetGuaranteedNumberOfVideoEncoderInstances(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetGuaranteedNumberOfVideoEncoderInstances *trt__GetGuaranteedNumberOfVideoEncoderInstances, struct _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse *trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetGuaranteedNumberOfVideoEncoderInstances' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetGuaranteedNumberOfVideoEncoderInstances(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetGuaranteedNumberOfVideoEncoderInstances *trt__GetGuaranteedNumberOfVideoEncoderInstances){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetGuaranteedNumberOfVideoEncoderInstances' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetGuaranteedNumberOfVideoEncoderInstances(struct soap *soap, struct _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse *trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetStreamUri' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetStreamUri(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetStreamUri *trt__GetStreamUri, struct _trt__GetStreamUriResponse *trt__GetStreamUriResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetStreamUri' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetStreamUri(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetStreamUri *trt__GetStreamUri){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetStreamUri' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetStreamUri(struct soap *soap, struct _trt__GetStreamUriResponse *trt__GetStreamUriResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__StartMulticastStreaming' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__StartMulticastStreaming(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__StartMulticastStreaming *trt__StartMulticastStreaming, struct _trt__StartMulticastStreamingResponse *trt__StartMulticastStreamingResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__StartMulticastStreaming' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__StartMulticastStreaming(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__StartMulticastStreaming *trt__StartMulticastStreaming){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__StartMulticastStreaming' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__StartMulticastStreaming(struct soap *soap, struct _trt__StartMulticastStreamingResponse *trt__StartMulticastStreamingResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__StopMulticastStreaming' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__StopMulticastStreaming(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__StopMulticastStreaming *trt__StopMulticastStreaming, struct _trt__StopMulticastStreamingResponse *trt__StopMulticastStreamingResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__StopMulticastStreaming' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__StopMulticastStreaming(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__StopMulticastStreaming *trt__StopMulticastStreaming){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__StopMulticastStreaming' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__StopMulticastStreaming(struct soap *soap, struct _trt__StopMulticastStreamingResponse *trt__StopMulticastStreamingResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__SetSynchronizationPoint' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__SetSynchronizationPoint(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetSynchronizationPoint *trt__SetSynchronizationPoint, struct _trt__SetSynchronizationPointResponse *trt__SetSynchronizationPointResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__SetSynchronizationPoint' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__SetSynchronizationPoint(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetSynchronizationPoint *trt__SetSynchronizationPoint){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__SetSynchronizationPoint' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__SetSynchronizationPoint(struct soap *soap, struct _trt__SetSynchronizationPointResponse *trt__SetSynchronizationPointResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetSnapshotUri' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetSnapshotUri(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetSnapshotUri *trt__GetSnapshotUri, struct _trt__GetSnapshotUriResponse *trt__GetSnapshotUriResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetSnapshotUri' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetSnapshotUri(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetSnapshotUri *trt__GetSnapshotUri){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetSnapshotUri' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetSnapshotUri(struct soap *soap, struct _trt__GetSnapshotUriResponse *trt__GetSnapshotUriResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetVideoSourceModes' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetVideoSourceModes(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoSourceModes *trt__GetVideoSourceModes, struct _trt__GetVideoSourceModesResponse *trt__GetVideoSourceModesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetVideoSourceModes' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetVideoSourceModes(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoSourceModes *trt__GetVideoSourceModes){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetVideoSourceModes' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetVideoSourceModes(struct soap *soap, struct _trt__GetVideoSourceModesResponse *trt__GetVideoSourceModesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__SetVideoSourceMode' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__SetVideoSourceMode(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetVideoSourceMode *trt__SetVideoSourceMode, struct _trt__SetVideoSourceModeResponse *trt__SetVideoSourceModeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__SetVideoSourceMode' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__SetVideoSourceMode(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetVideoSourceMode *trt__SetVideoSourceMode){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__SetVideoSourceMode' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__SetVideoSourceMode(struct soap *soap, struct _trt__SetVideoSourceModeResponse *trt__SetVideoSourceModeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetOSDs' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetOSDs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetOSDs *trt__GetOSDs, struct _trt__GetOSDsResponse *trt__GetOSDsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetOSDs' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetOSDs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetOSDs *trt__GetOSDs){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetOSDs' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetOSDs(struct soap *soap, struct _trt__GetOSDsResponse *trt__GetOSDsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetOSD' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetOSD(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetOSD *trt__GetOSD, struct _trt__GetOSDResponse *trt__GetOSDResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetOSD' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetOSD(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetOSD *trt__GetOSD){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetOSD' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetOSD(struct soap *soap, struct _trt__GetOSDResponse *trt__GetOSDResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__GetOSDOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetOSDOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetOSDOptions *trt__GetOSDOptions, struct _trt__GetOSDOptionsResponse *trt__GetOSDOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__GetOSDOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__GetOSDOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetOSDOptions *trt__GetOSDOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__GetOSDOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__GetOSDOptions(struct soap *soap, struct _trt__GetOSDOptionsResponse *trt__GetOSDOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__SetOSD' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__SetOSD(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetOSD *trt__SetOSD, struct _trt__SetOSDResponse *trt__SetOSDResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__SetOSD' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__SetOSD(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetOSD *trt__SetOSD){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__SetOSD' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__SetOSD(struct soap *soap, struct _trt__SetOSDResponse *trt__SetOSDResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__CreateOSD' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__CreateOSD(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__CreateOSD *trt__CreateOSD, struct _trt__CreateOSDResponse *trt__CreateOSDResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__CreateOSD' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__CreateOSD(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__CreateOSD *trt__CreateOSD){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__CreateOSD' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__CreateOSD(struct soap *soap, struct _trt__CreateOSDResponse *trt__CreateOSDResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    
//     /** Web service synchronous operation 'soap_call___trt__DeleteOSD' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__DeleteOSD(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__DeleteOSD *trt__DeleteOSD, struct _trt__DeleteOSDResponse *trt__DeleteOSDResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_send___trt__DeleteOSD' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___trt__DeleteOSD(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__DeleteOSD *trt__DeleteOSD){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }    /** Web service asynchronous operation 'soap_recv___trt__DeleteOSD' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___trt__DeleteOSD(struct soap *soap, struct _trt__DeleteOSDResponse *trt__DeleteOSDResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
/******************************************************************************\
 *                                                                            *
 * Server-Side Operations                                                     *
 *                                                                            *
\******************************************************************************/

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
    ret = LUX_Network_getip2((char *)"eth0", ip); //eth0
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
    LOG_DBG(LOG_TAG, "\n");

    ACCESS_CONTROL

    int ret = 0;
    char ip[20];
    /* get host ip */
    ret = LUX_Network_getip2((char *)"eth0", ip);
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
    /** Web service operation '__trt__AddVideoEncoderConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoEncoderConfiguration(struct soap* soap, struct _trt__AddVideoEncoderConfiguration *trt__AddVideoEncoderConfiguration, struct _trt__AddVideoEncoderConfigurationResponse *trt__AddVideoEncoderConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__AddVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoSourceConfiguration(struct soap* soap, struct _trt__AddVideoSourceConfiguration *trt__AddVideoSourceConfiguration, struct _trt__AddVideoSourceConfigurationResponse *trt__AddVideoSourceConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__AddAudioEncoderConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioEncoderConfiguration(struct soap* soap, struct _trt__AddAudioEncoderConfiguration *trt__AddAudioEncoderConfiguration, struct _trt__AddAudioEncoderConfigurationResponse *trt__AddAudioEncoderConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__AddAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioSourceConfiguration(struct soap* soap, struct _trt__AddAudioSourceConfiguration *trt__AddAudioSourceConfiguration, struct _trt__AddAudioSourceConfigurationResponse *trt__AddAudioSourceConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__AddPTZConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__AddPTZConfiguration(struct soap* soap, struct _trt__AddPTZConfiguration *trt__AddPTZConfiguration, struct _trt__AddPTZConfigurationResponse *trt__AddPTZConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__AddVideoAnalyticsConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoAnalyticsConfiguration(struct soap* soap, struct _trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration, struct _trt__AddVideoAnalyticsConfigurationResponse *trt__AddVideoAnalyticsConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__AddMetadataConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__AddMetadataConfiguration(struct soap* soap, struct _trt__AddMetadataConfiguration *trt__AddMetadataConfiguration, struct _trt__AddMetadataConfigurationResponse *trt__AddMetadataConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__AddAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioOutputConfiguration(struct soap* soap, struct _trt__AddAudioOutputConfiguration *trt__AddAudioOutputConfiguration, struct _trt__AddAudioOutputConfigurationResponse *trt__AddAudioOutputConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__AddAudioDecoderConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioDecoderConfiguration(struct soap* soap, struct _trt__AddAudioDecoderConfiguration *trt__AddAudioDecoderConfiguration, struct _trt__AddAudioDecoderConfigurationResponse *trt__AddAudioDecoderConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__RemoveVideoEncoderConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoEncoderConfiguration(struct soap* soap, struct _trt__RemoveVideoEncoderConfiguration *trt__RemoveVideoEncoderConfiguration, struct _trt__RemoveVideoEncoderConfigurationResponse *trt__RemoveVideoEncoderConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__RemoveVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoSourceConfiguration(struct soap* soap, struct _trt__RemoveVideoSourceConfiguration *trt__RemoveVideoSourceConfiguration, struct _trt__RemoveVideoSourceConfigurationResponse *trt__RemoveVideoSourceConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__RemoveAudioEncoderConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioEncoderConfiguration(struct soap* soap, struct _trt__RemoveAudioEncoderConfiguration *trt__RemoveAudioEncoderConfiguration, struct _trt__RemoveAudioEncoderConfigurationResponse *trt__RemoveAudioEncoderConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__RemoveAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioSourceConfiguration(struct soap* soap, struct _trt__RemoveAudioSourceConfiguration *trt__RemoveAudioSourceConfiguration, struct _trt__RemoveAudioSourceConfigurationResponse *trt__RemoveAudioSourceConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__RemovePTZConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__RemovePTZConfiguration(struct soap* soap, struct _trt__RemovePTZConfiguration *trt__RemovePTZConfiguration, struct _trt__RemovePTZConfigurationResponse *trt__RemovePTZConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__RemoveVideoAnalyticsConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoAnalyticsConfiguration(struct soap* soap, struct _trt__RemoveVideoAnalyticsConfiguration *trt__RemoveVideoAnalyticsConfiguration, struct _trt__RemoveVideoAnalyticsConfigurationResponse *trt__RemoveVideoAnalyticsConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__RemoveMetadataConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveMetadataConfiguration(struct soap* soap, struct _trt__RemoveMetadataConfiguration *trt__RemoveMetadataConfiguration, struct _trt__RemoveMetadataConfigurationResponse *trt__RemoveMetadataConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__RemoveAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioOutputConfiguration(struct soap* soap, struct _trt__RemoveAudioOutputConfiguration *trt__RemoveAudioOutputConfiguration, struct _trt__RemoveAudioOutputConfigurationResponse *trt__RemoveAudioOutputConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__RemoveAudioDecoderConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioDecoderConfiguration(struct soap* soap, struct _trt__RemoveAudioDecoderConfiguration *trt__RemoveAudioDecoderConfiguration, struct _trt__RemoveAudioDecoderConfigurationResponse *trt__RemoveAudioDecoderConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__DeleteProfile' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__DeleteProfile(struct soap* soap, struct _trt__DeleteProfile *trt__DeleteProfile, struct _trt__DeleteProfileResponse *trt__DeleteProfileResponse){
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
    LOG_DBG(LOG_TAG, "\n");

    int ret = 0;
    char ip[20];
    /* get host ip */
    ret = LUX_Network_getip2((char *)"eth0", ip); //eth0
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
	Configurations[0].H264->H264Profile = tt__H264Profile__Main; //tt__H264Profile__Main;

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
    ret = LUX_Network_getip2((char*)"eth0", ip); //eth0
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
    LOG_DBG(LOG_TAG, "\n");
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
	ret = LUX_Network_getip2((char *)"eth0", IpAddr); //eth0
	if (ret < 0) {
		LOG_ERR(LOG_TAG, "Get host IP address fail.\n");
		return SOAP_ERR;
	}

	//获取帧率
	ret = LUX_Video_Func_Get_Fps(&iFrmRateNum, &iFrmRateDen);
	if(ret < 0)
	{
		LOG_ERR(LOG_TAG,"Call func_video_get_fps fail.\n");
		return SOAP_ERR;
	}


	//获取码率
	ret = LUX_Video_Func_Get_Bitrate(iEncChnNum, &iBitRate);
	if(ret < 0)
	{
		LOG_ERR(LOG_TAG,"Call func_video_get_bitrate fail.\n");
		return SOAP_ERR;
	}

	//获取分辨率
	ret = LUX_Video_Fnuc_Get_Resoulution(iFsChnNum, &iPicWidth, &iPicHeigth);
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

	if (strcmp(trt__GetVideoEncoderConfiguration->ConfigurationToken, "MainVideoEncoderConfigToken") == 0)
	{
        LOG_DBG(LOG_TAG, "Token is MainVideoEncoderConfigToken\n");

        Configuration->Encoding = tt__VideoEncoding__H264;
        Configuration->UseCount = 1;
        Configuration->Quality = 30;
        Configuration->SessionTimeout = "10000";
        Configuration->RateControl->FrameRateLimit = 25;
        Configuration->RateControl->EncodingInterval = 1;
        Configuration->RateControl->BitrateLimit = 2048;
        Configuration->H264->GovLength = 50;
        Configuration->H264->H264Profile = tt__H264Profile__Main ;
        Configuration->Multicast->Port = 18888;
        Configuration->Multicast->TTL = 255;
        Configuration->Multicast->AutoStart = xsd__boolean__false_;
        Configuration->Multicast->Address->Type = tt__IPType__IPv4;
        sprintf(Configuration->Multicast->Address->IPv4Address, "%s", IpAddr);
        Configuration->Multicast->Address->IPv6Address = NULL;

		sprintf(Configuration->Name, "%s", "MainVideoEncoderConfig");
		sprintf(Configuration->token, "%s", "MainVideoEncoderConfigToken");

		Configuration->Resolution->Width = iPicWidth;
		Configuration->Resolution->Height = iPicHeigth;

	}
	else if(strcmp(trt__GetVideoEncoderConfiguration->ConfigurationToken, "SubVideoEncoderConfigToken") == 0)
	{
        LOG_DBG(LOG_TAG, "Token is SubVideoEncoderConfigToken\n");

        Configuration->Encoding = tt__VideoEncoding__H264;
        Configuration->UseCount = 1;
        Configuration->Quality = 30;
        Configuration->SessionTimeout = "10000";
        Configuration->RateControl->FrameRateLimit = 25;
        Configuration->RateControl->EncodingInterval = 1;
        Configuration->RateControl->BitrateLimit = 2048;
        Configuration->H264->GovLength = 50;
        Configuration->H264->H264Profile = tt__H264Profile__Main;
        Configuration->Multicast->Port = 18888;
        Configuration->Multicast->TTL = 255;
        Configuration->Multicast->AutoStart = xsd__boolean__false_;
        Configuration->Multicast->Address->Type = tt__IPType__IPv4;
        sprintf(Configuration->Multicast->Address->IPv4Address, "%s", IpAddr);
        Configuration->Multicast->Address->IPv6Address = NULL;

		sprintf(Configuration->Name, "%s", "SubVideoEncoderConfig");
		sprintf(Configuration->token, "%s", "SubVideoEncoderConfigToken");

		Configuration->Resolution->Width = 640;
		Configuration->Resolution->Height = 360;
	}

    trt__GetVideoEncoderConfigurationResponse->Configuration = Configuration;

    return SOAP_OK;
}
    /** Web service operation '__trt__GetAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfiguration(struct soap* soap, struct _trt__GetAudioSourceConfiguration *trt__GetAudioSourceConfiguration, struct _trt__GetAudioSourceConfigurationResponse *trt__GetAudioSourceConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__GetAudioEncoderConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfiguration(struct soap* soap, struct _trt__GetAudioEncoderConfiguration *trt__GetAudioEncoderConfiguration, struct _trt__GetAudioEncoderConfigurationResponse *trt__GetAudioEncoderConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__GetVideoAnalyticsConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoAnalyticsConfiguration(struct soap* soap, struct _trt__GetVideoAnalyticsConfiguration *trt__GetVideoAnalyticsConfiguration, struct _trt__GetVideoAnalyticsConfigurationResponse *trt__GetVideoAnalyticsConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__GetMetadataConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfiguration(struct soap* soap, struct _trt__GetMetadataConfiguration *trt__GetMetadataConfiguration, struct _trt__GetMetadataConfigurationResponse *trt__GetMetadataConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__GetAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfiguration(struct soap* soap, struct _trt__GetAudioOutputConfiguration *trt__GetAudioOutputConfiguration, struct _trt__GetAudioOutputConfigurationResponse *trt__GetAudioOutputConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__GetAudioDecoderConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfiguration(struct soap* soap, struct _trt__GetAudioDecoderConfiguration *trt__GetAudioDecoderConfiguration, struct _trt__GetAudioDecoderConfigurationResponse *trt__GetAudioDecoderConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__GetCompatibleVideoEncoderConfigurations' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoEncoderConfigurations(struct soap* soap, struct _trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations, struct _trt__GetCompatibleVideoEncoderConfigurationsResponse *trt__GetCompatibleVideoEncoderConfigurationsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__GetCompatibleVideoSourceConfigurations' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoSourceConfigurations(struct soap* soap, struct _trt__GetCompatibleVideoSourceConfigurations *trt__GetCompatibleVideoSourceConfigurations, struct _trt__GetCompatibleVideoSourceConfigurationsResponse *trt__GetCompatibleVideoSourceConfigurationsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__GetCompatibleAudioEncoderConfigurations' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioEncoderConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioEncoderConfigurations *trt__GetCompatibleAudioEncoderConfigurations, struct _trt__GetCompatibleAudioEncoderConfigurationsResponse *trt__GetCompatibleAudioEncoderConfigurationsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__GetCompatibleAudioSourceConfigurations' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioSourceConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioSourceConfigurations *trt__GetCompatibleAudioSourceConfigurations, struct _trt__GetCompatibleAudioSourceConfigurationsResponse *trt__GetCompatibleAudioSourceConfigurationsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__GetCompatibleVideoAnalyticsConfigurations' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoAnalyticsConfigurations(struct soap* soap, struct _trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations, struct _trt__GetCompatibleVideoAnalyticsConfigurationsResponse *trt__GetCompatibleVideoAnalyticsConfigurationsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__GetCompatibleMetadataConfigurations' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleMetadataConfigurations(struct soap* soap, struct _trt__GetCompatibleMetadataConfigurations *trt__GetCompatibleMetadataConfigurations, struct _trt__GetCompatibleMetadataConfigurationsResponse *trt__GetCompatibleMetadataConfigurationsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__GetCompatibleAudioOutputConfigurations' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioOutputConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioOutputConfigurations *trt__GetCompatibleAudioOutputConfigurations, struct _trt__GetCompatibleAudioOutputConfigurationsResponse *trt__GetCompatibleAudioOutputConfigurationsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__GetCompatibleAudioDecoderConfigurations' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioDecoderConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioDecoderConfigurations *trt__GetCompatibleAudioDecoderConfigurations, struct _trt__GetCompatibleAudioDecoderConfigurationsResponse *trt__GetCompatibleAudioDecoderConfigurationsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__SetVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoSourceConfiguration(struct soap* soap, struct _trt__SetVideoSourceConfiguration *trt__SetVideoSourceConfiguration, struct _trt__SetVideoSourceConfigurationResponse *trt__SetVideoSourceConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    

/** Web service operation '__trt__SetVideoEncoderConfiguration' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoEncoderConfiguration(struct soap* soap, struct _trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration, struct _trt__SetVideoEncoderConfigurationResponse *trt__SetVideoEncoderConfigurationResponse)
{
    LOG_DBG(LOG_TAG, "\n");
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
        LOG_ERR(LOG_TAG, "Call func_video_set_resolution,error number is %d\n", ret);
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
    /** Web service operation '__trt__SetAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioSourceConfiguration(struct soap* soap, struct _trt__SetAudioSourceConfiguration *trt__SetAudioSourceConfiguration, struct _trt__SetAudioSourceConfigurationResponse *trt__SetAudioSourceConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__SetAudioEncoderConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioEncoderConfiguration(struct soap* soap, struct _trt__SetAudioEncoderConfiguration *trt__SetAudioEncoderConfiguration, struct _trt__SetAudioEncoderConfigurationResponse *trt__SetAudioEncoderConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__SetVideoAnalyticsConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoAnalyticsConfiguration(struct soap* soap, struct _trt__SetVideoAnalyticsConfiguration *trt__SetVideoAnalyticsConfiguration, struct _trt__SetVideoAnalyticsConfigurationResponse *trt__SetVideoAnalyticsConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__SetMetadataConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__SetMetadataConfiguration(struct soap* soap, struct _trt__SetMetadataConfiguration *trt__SetMetadataConfiguration, struct _trt__SetMetadataConfigurationResponse *trt__SetMetadataConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__SetAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioOutputConfiguration(struct soap* soap, struct _trt__SetAudioOutputConfiguration *trt__SetAudioOutputConfiguration, struct _trt__SetAudioOutputConfigurationResponse *trt__SetAudioOutputConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__SetAudioDecoderConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioDecoderConfiguration(struct soap* soap, struct _trt__SetAudioDecoderConfiguration *trt__SetAudioDecoderConfiguration, struct _trt__SetAudioDecoderConfigurationResponse *trt__SetAudioDecoderConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__GetVideoSourceConfigurationOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfigurationOptions(struct soap* soap, struct _trt__GetVideoSourceConfigurationOptions *trt__GetVideoSourceConfigurationOptions, struct _trt__GetVideoSourceConfigurationOptionsResponse *trt__GetVideoSourceConfigurationOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}   
/** Web service operation '__trt__GetAudioSourceConfigurationOptions' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurationOptions(struct soap* soap, struct _trt__GetAudioSourceConfigurationOptions *trt__GetAudioSourceConfigurationOptions, struct _trt__GetAudioSourceConfigurationOptionsResponse *trt__GetAudioSourceConfigurationOptionsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
} 
/** Web service operation '__trt__GetVideoEncoderConfigurationOptions' (returns SOAP_OK or error code) */
SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurationOptions(struct soap* soap, struct _trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions, struct _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    LOG_DBG(LOG_TAG, "Token is [%s]\n", trt__GetVideoEncoderConfigurationOptions->ConfigurationToken);
    struct tt__VideoEncoderConfigurationOptions *Options = (struct tt__VideoEncoderConfigurationOptions *)soap_zalloc(soap, sizeof(struct tt__VideoEncoderConfigurationOptions));
    Options->QualityRange = (struct tt__IntRange *)soap_zalloc(soap, sizeof(struct tt__IntRange));
    Options->H264 = (struct tt__H264Options *)soap_zalloc(soap, sizeof(struct tt__H264Options));
    Options->Extension =  (struct tt__VideoEncoderOptionsExtension*)soap_zalloc(soap, sizeof(struct tt__VideoEncoderOptionsExtension));
    Options->H264->ResolutionsAvailable = (struct tt__VideoResolution *)soap_zalloc(soap, sizeof(struct tt__VideoResolution) * 4);
    Options->H264->GovLengthRange = (struct tt__IntRange *)soap_zalloc(soap, sizeof(struct tt__IntRange));
    Options->H264->FrameRateRange = (struct tt__IntRange *)soap_zalloc(soap, sizeof(struct tt__IntRange));
    Options->H264->EncodingIntervalRange = (struct tt__IntRange *)soap_zalloc(soap, sizeof(struct tt__IntRange));
    Options->H264->H264ProfilesSupported = (enum tt__H264Profile *)soap_zalloc(soap, sizeof(enum tt__H264Profile));

    Options->Extension->H264 = (struct tt__H264Options2*)soap_zalloc(soap, sizeof(struct tt__H264Options2));
    Options->Extension->H264->BitrateRange = (struct tt__IntRange*)soap_zalloc(soap, sizeof(struct tt__IntRange));


    if (strcmp(trt__GetVideoEncoderConfigurationOptions->ConfigurationToken, "MainVideoEncoderConfigToken") == 0)
    {
        Options->QualityRange->Min = 0;
        Options->QualityRange->Max = 100;
        Options->H264->__sizeH264ProfilesSupported = 1;
       *Options->H264->H264ProfilesSupported = tt__H264Profile__Main;
        Options->H264->__sizeResolutionsAvailable = 2;
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
        Options->Extension->H264->BitrateRange->Max = 4096;
    }
    else if (strcmp(trt__GetVideoEncoderConfigurationOptions->ConfigurationToken, "SubVideoEncoderConfigToken") == 0)
    {
        Options->QualityRange->Min = 0;
        Options->QualityRange->Max = 100;
        Options->H264->__sizeH264ProfilesSupported = 1;
        *Options->H264->H264ProfilesSupported = tt__H264Profile__Main;

        Options->H264->__sizeResolutionsAvailable = 4;
        Options->H264->ResolutionsAvailable[0].Width = 800;
        Options->H264->ResolutionsAvailable[0].Height = 600;

        Options->H264->ResolutionsAvailable[1].Width = 720;
        Options->H264->ResolutionsAvailable[1].Height = 576;

        Options->H264->ResolutionsAvailable[2].Width = 640;
        Options->H264->ResolutionsAvailable[2].Height = 480;

        Options->H264->ResolutionsAvailable[3].Width = 320;
        Options->H264->ResolutionsAvailable[3].Height = 240;

        Options->H264->GovLengthRange->Min = 1;
        Options->H264->GovLengthRange->Max = 360;

        Options->H264->FrameRateRange->Min = 5;
        Options->H264->FrameRateRange->Max = 30;

        Options->H264->EncodingIntervalRange->Min = 1;
        Options->H264->EncodingIntervalRange->Max = 100;


        Options->Extension->H264->BitrateRange->Min = 5;
        Options->Extension->H264->BitrateRange->Max = 4096;
    }
    else
    {
        LOG_ERR(LOG_TAG, "GetVideoEncoderConfiguration Token error\n");
        return SOAP_ERR;
    }

    trt__GetVideoEncoderConfigurationOptionsResponse->Options = Options;
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
    /** Web service operation '__trt__GetMetadataConfigurationOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfigurationOptions(struct soap* soap, struct _trt__GetMetadataConfigurationOptions *trt__GetMetadataConfigurationOptions, struct _trt__GetMetadataConfigurationOptionsResponse *trt__GetMetadataConfigurationOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__GetAudioOutputConfigurationOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfigurationOptions(struct soap* soap, struct _trt__GetAudioOutputConfigurationOptions *trt__GetAudioOutputConfigurationOptions, struct _trt__GetAudioOutputConfigurationOptionsResponse *trt__GetAudioOutputConfigurationOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__GetAudioDecoderConfigurationOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfigurationOptions(struct soap* soap, struct _trt__GetAudioDecoderConfigurationOptions *trt__GetAudioDecoderConfigurationOptions, struct _trt__GetAudioDecoderConfigurationOptionsResponse *trt__GetAudioDecoderConfigurationOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__GetGuaranteedNumberOfVideoEncoderInstances' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetGuaranteedNumberOfVideoEncoderInstances(struct soap* soap, struct _trt__GetGuaranteedNumberOfVideoEncoderInstances *trt__GetGuaranteedNumberOfVideoEncoderInstances, struct _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse *trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse){
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
	ret = LUX_Network_getip2((char *)"eth0", ip); //eth0
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
    /** Web service operation '__trt__StartMulticastStreaming' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__StartMulticastStreaming(struct soap* soap, struct _trt__StartMulticastStreaming *trt__StartMulticastStreaming, struct _trt__StartMulticastStreamingResponse *trt__StartMulticastStreamingResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__StopMulticastStreaming' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__StopMulticastStreaming(struct soap* soap, struct _trt__StopMulticastStreaming *trt__StopMulticastStreaming, struct _trt__StopMulticastStreamingResponse *trt__StopMulticastStreamingResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__SetSynchronizationPoint' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__SetSynchronizationPoint(struct soap* soap, struct _trt__SetSynchronizationPoint *trt__SetSynchronizationPoint, struct _trt__SetSynchronizationPointResponse *trt__SetSynchronizationPointResponse){
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
    ret = LUX_Network_getip2((char *)"eth0", ip); //eth0
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
    /** Web service operation '__trt__GetVideoSourceModes' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceModes(struct soap* soap, struct _trt__GetVideoSourceModes *trt__GetVideoSourceModes, struct _trt__GetVideoSourceModesResponse *trt__GetVideoSourceModesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}    /** Web service operation '__trt__SetVideoSourceMode' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoSourceMode(struct soap* soap, struct _trt__SetVideoSourceMode *trt__SetVideoSourceMode, struct _trt__SetVideoSourceModeResponse *trt__SetVideoSourceModeResponse){
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
//     /** Web service operation '__ns5__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetServiceCapabilities(struct soap *soap, struct _ns5__GetServiceCapabilities *ns5__GetServiceCapabilities, struct _ns5__GetServiceCapabilitiesResponse *ns5__GetServiceCapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__CreateProfile' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__CreateProfile(struct soap *soap, struct _ns5__CreateProfile *ns5__CreateProfile, struct _ns5__CreateProfileResponse *ns5__CreateProfileResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetProfiles' implementation, should return SOAP_OK or error code */
// SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetProfiles(struct soap *soap, struct _ns5__GetProfiles *ns5__GetProfiles, struct _ns5__GetProfilesResponse *ns5__GetProfilesResponse){
 
//  LOG_DBG(LOG_TAG, "\n");

//     ACCESS_CONTROL

//     int ret = 0;
//     char ip[20];
//     /* get host ip */
//     ret = LUX_Network_getip2((char *)"eth0", ip);
//     if (ret < 0) {
// 		LOG_ERR(LOG_TAG, "ERROR: can't get host ip address!\n");
// 		return SOAP_ERR;
//     }

//     ns5__GetProfilesResponse->__sizeProfiles = 2;
//     /* Profile */
//     struct ns5__MediaProfile *Profiles = (struct ns5__MediaProfile *)soap_zalloc(soap, sizeof(struct ns5__MediaProfile) * ns5__GetProfilesResponse->__sizeProfiles);

//     /* Main stream config */
//     Profiles[0].Name = (char *)soap_zalloc(soap, 100);
//     strcpy(Profiles[0].Name, "MainStream");

//     Profiles[0].token = (char *)soap_zalloc(soap, 100);
//     strcpy(Profiles[0].token, "MainStreamToken");

//     Profiles[0].fixed = (enum xsd__boolean *)soap_zalloc(soap, sizeof(enum xsd__boolean));
//     *Profiles[0].fixed = xsd__boolean__true_;

//     Profiles[0].Configurations->VideoSource = (struct tt__VideoSourceConfiguration *)soap_zalloc(soap, sizeof(struct tt__VideoSourceConfiguration));
//     Profiles[0].Configurations->VideoSource->Name = (char *)soap_zalloc(soap, 100);
// 	Profiles[0].Configurations->VideoSource->token = (char *)soap_zalloc(soap, 100);
//     Profiles[0].Configurations->VideoSource->SourceToken = (char *)soap_zalloc(soap, 100);
//     Profiles[0].Configurations->VideoSource->Bounds = (struct tt__IntRectangle *)soap_zalloc(soap, sizeof(struct tt__IntRectangle));

//     sprintf(Profiles[0].Configurations->VideoSource->Name, "%s", "XcamVideoSourceConfig");
//     sprintf(Profiles[0].Configurations->VideoSource->token, "%s", "XcamVideoSourceConfigToken");
//     sprintf(Profiles[0].Configurations->VideoSource->SourceToken, "%s", "XcamVideoSourceConfigToken");
//     Profiles[0].Configurations->VideoSource->UseCount = 2;

// 	Profiles[0].Configurations->VideoSource->Bounds->x = 0;
// 	Profiles[0].Configurations->VideoSource->Bounds->y = 0;
// 	Profiles[0].Configurations->VideoSource->Bounds->width = 1920;
// 	Profiles[0].Configurations->VideoSource->Bounds->height = 1080;

// 	Profiles[0].Configurations->VideoEncoder = (struct tt__VideoEncoder2Configuration *)soap_zalloc(soap, sizeof(struct tt__VideoEncoder2Configuration));

// 	Profiles[0].Configurations->VideoEncoder->Name = (char *)soap_zalloc(soap, 100);
// 	sprintf(Profiles[0].Configurations->VideoEncoder->Name, "%s", "MainVideoEncoderConfig");
// 	Profiles[0].Configurations->VideoEncoder->UseCount = 1;
// 	Profiles[0].Configurations->VideoEncoder->token = (char *)soap_zalloc(soap, 100);
// 	sprintf(Profiles[0].Configurations->VideoEncoder->token, "%s", "MainVideoEncoderConfigToken");
//     Profiles[0].Configurations->VideoEncoder->Encoding = tt__VideoEncoding__H264;
//     Profiles[0].Configurations->VideoEncoder->Resolution = (struct tt__VideoResolution *)soap_zalloc(soap, sizeof(struct tt__VideoResolution));
//     Profiles[0].Configurations->VideoEncoder->Resolution->Width = 1920;
//     Profiles[0].Configurations->VideoEncoder->Resolution->Height = 1080;
//     Profiles[0].Configurations->VideoEncoder->Quality = 40;//5

//     Profiles[0].Configurations->VideoEncoder->Multicast = (struct tt__MulticastConfiguration *)soap_zalloc(soap, sizeof(struct tt__MulticastConfiguration));
//     Profiles[0].Configurations->VideoEncoder->Multicast->Address = (struct tt__IPAddress *)soap_zalloc(soap, sizeof(struct tt__IPAddress));
//     Profiles[0].Configurations->VideoEncoder->Multicast->Address->Type = tt__IPType__IPv4;
//     Profiles[0].Configurations->VideoEncoder->Multicast->Address->IPv4Address = (char *)soap_zalloc(soap, 100);
//     sprintf(Profiles[0].Configurations->VideoEncoder->Multicast->Address->IPv4Address, "%s", ip);
//     Profiles[0].Configurations->VideoEncoder->Multicast->Address->IPv6Address = NULL;
//     Profiles[0].Configurations->VideoEncoder->Multicast->Port = 18888;
//     Profiles[0].Configurations->VideoEncoder->Multicast->TTL = 255;
//     Profiles[0].Configurations->VideoEncoder->Multicast->AutoStart = xsd__boolean__false_;

//     // Profiles[0].Configurations->VideoEncoder->SessionTimeout = "PT300S";

//     // Profiles[0].Configurations->VideoEncoder->H264 = (struct tt__H264Configuration *)soap_zalloc(soap, sizeof(struct tt__H264Configuration));
//     // Profiles[0].Configurations->VideoEncoder->H264->GovLength = 50;//30
//     // Profiles[0].Configurations->VideoEncoder->H264->H264Profile = tt__H264Profile__High; //tt__H264Profile__Main;

//     Profiles[0].Configurations->VideoEncoder->RateControl = (struct tt__VideoRateControl *)soap_zalloc(soap, sizeof(struct tt__VideoRateControl));
//     Profiles[0].Configurations->VideoEncoder->RateControl->FrameRateLimit = 25;
//     // Profiles[0].Configurations->VideoEncoder->RateControl->EncodingInterval = 1;
//     Profiles[0].Configurations->VideoEncoder->RateControl->BitrateLimit = 2048;
//     Profiles[0].Configurations->VideoEncoder->RateControl = NULL;


//     /* Second stream config */
//     Profiles[1].Name = (char *)soap_zalloc(soap, 100);
//     strcpy(Profiles[1].Name, "SubStream");

//     Profiles[1].token = (char *)soap_zalloc(soap, 100);
//     strcpy(Profiles[1].token, "SubStreamToken");

//     Profiles[1].fixed = (enum xsd__boolean *)soap_zalloc(soap, sizeof(enum xsd__boolean));
//     *Profiles[1].fixed = xsd__boolean__true_;

//     Profiles[1].Configurations->VideoSource = (struct tt__VideoSourceConfiguration *)soap_zalloc(soap, sizeof(struct tt__VideoSourceConfiguration));
//     Profiles[1].Configurations->VideoSource->Name = (char *)soap_zalloc(soap, 100);
//     Profiles[1].Configurations->VideoSource->token = (char *)soap_zalloc(soap, 100);
//     Profiles[1].Configurations->VideoSource->SourceToken = (char *)soap_zalloc(soap, 100);
//     Profiles[1].Configurations->VideoSource->Bounds = (struct tt__IntRectangle *)soap_zalloc(soap, sizeof(struct tt__IntRectangle));

//     sprintf(Profiles[1].Configurations->VideoSource->Name, "%s", "XcamVideoSourceConfig");
//     sprintf(Profiles[1].Configurations->VideoSource->token, "%s", "XcamVideoSourceConfigToken");
//     sprintf(Profiles[1].Configurations->VideoSource->SourceToken, "%s", "XcamVideoSourceConfigToken");
//     Profiles[1].Configurations->VideoSource->UseCount = 2;

//     Profiles[1].Configurations->VideoSource->Bounds->x = 0;
//     Profiles[1].Configurations->VideoSource->Bounds->y = 0;
//     Profiles[1].Configurations->VideoSource->Bounds->width = 640;
//     Profiles[1].Configurations->VideoSource->Bounds->height = 360;

//     Profiles[1].Configurations->VideoEncoder = (struct tt__VideoEncoder2Configuration *)soap_zalloc(soap, sizeof(struct tt__VideoEncoder2Configuration));

//     Profiles[1].Configurations->VideoEncoder->Name = (char *)soap_zalloc(soap, 100);
//     sprintf(Profiles[1].Configurations->VideoEncoder->Name, "%s", "SubVideoEncoderConfig");
//     Profiles[1].Configurations->VideoEncoder->UseCount = 1;
//     Profiles[1].Configurations->VideoEncoder->token = (char *)soap_zalloc(soap, 100);
//     sprintf(Profiles[1].Configurations->VideoEncoder->token, "%s", "SubVideoEncoderConfigToken");
//     Profiles[1].Configurations->VideoEncoder->Encoding = tt__VideoEncoding__H264;
//     Profiles[1].Configurations->VideoEncoder->Resolution = (struct tt__VideoResolution *)soap_zalloc(soap, sizeof(struct tt__VideoResolution));
//     Profiles[1].Configurations->VideoEncoder->Resolution->Width = 640;
//     Profiles[1].Configurations->VideoEncoder->Resolution->Height = 360;
//     Profiles[1].Configurations->VideoEncoder->Quality = 40;//5
//     Profiles[1].Configurations->VideoEncoder->Multicast = (struct tt__MulticastConfiguration *)soap_zalloc(soap, sizeof(struct tt__MulticastConfiguration));
//     Profiles[1].Configurations->VideoEncoder->Multicast->Address = (struct tt__IPAddress *)soap_zalloc(soap, sizeof(struct tt__IPAddress));
//     Profiles[1].Configurations->VideoEncoder->Multicast->Address->Type = tt__IPType__IPv4;
//     Profiles[1].Configurations->VideoEncoder->Multicast->Address->IPv4Address = (char *)soap_zalloc(soap, 100);
//     sprintf(Profiles[1].Configurations->VideoEncoder->Multicast->Address->IPv4Address, "%s", ip);
//     Profiles[1].Configurations->VideoEncoder->Multicast->Address->IPv6Address = NULL;
//     Profiles[1].Configurations->VideoEncoder->Multicast->Port = 18888;
//     Profiles[1].Configurations->VideoEncoder->Multicast->TTL = 255;
//     Profiles[1].Configurations->VideoEncoder->Multicast->AutoStart = xsd__boolean__false_;

//     // Profiles[1].Configurations->VideoEncoder->SessionTimeout = "PT300S";

//     // Profiles[1].Configurations->VideoEncoder->H264 = (struct tt__H264Configuration *)soap_zalloc(soap, sizeof(struct tt__H264Configuration));
//     // Profiles[1].Configurations->VideoEncoder->H264->GovLength = 50;//30
//     // Profiles[1].Configurations->VideoEncoder->H264->H264Profile = tt__H264Profile__High; //tt__H264Profile__Main;

//     Profiles[1].Configurations->VideoEncoder->RateControl = (struct tt__VideoRateControl *)soap_zalloc(soap, sizeof(struct tt__VideoRateControl));
//     Profiles[1].Configurations->VideoEncoder->RateControl->FrameRateLimit = 25;
//     // Profiles[1].Configurations->VideoEncoder->RateControl->EncodingInterval = 1;
//     Profiles[1].Configurations->VideoEncoder->RateControl->BitrateLimit = 2048;
//     Profiles[1].Configurations->VideoEncoder->RateControl = NULL;

//     /* set trt__GetProfileResponse */
//     ns5__GetProfilesResponse->Profiles = Profiles;

//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__AddConfiguration' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__AddConfiguration(struct soap *soap, struct _ns5__AddConfiguration *ns5__AddConfiguration, struct _ns5__AddConfigurationResponse *ns5__AddConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__RemoveConfiguration' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__RemoveConfiguration(struct soap *soap, struct _ns5__RemoveConfiguration *ns5__RemoveConfiguration, struct _ns5__RemoveConfigurationResponse *ns5__RemoveConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__DeleteProfile' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__DeleteProfile(struct soap *soap, struct _ns5__DeleteProfile *ns5__DeleteProfile, struct _ns5__DeleteProfileResponse *ns5__DeleteProfileResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetVideoSourceConfigurations' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetVideoSourceConfigurations(struct soap *soap, struct ns5__GetConfiguration *ns5__GetVideoSourceConfigurations, struct _ns5__GetVideoSourceConfigurationsResponse *ns5__GetVideoSourceConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetVideoEncoderConfigurations' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetVideoEncoderConfigurations(struct soap *soap, struct ns5__GetConfiguration *ns5__GetVideoEncoderConfigurations, struct _ns5__GetVideoEncoderConfigurationsResponse *ns5__GetVideoEncoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetAudioSourceConfigurations' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetAudioSourceConfigurations(struct soap *soap, struct ns5__GetConfiguration *ns5__GetAudioSourceConfigurations, struct _ns5__GetAudioSourceConfigurationsResponse *ns5__GetAudioSourceConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetAudioEncoderConfigurations' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetAudioEncoderConfigurations(struct soap *soap, struct ns5__GetConfiguration *ns5__GetAudioEncoderConfigurations, struct _ns5__GetAudioEncoderConfigurationsResponse *ns5__GetAudioEncoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetAnalyticsConfigurations' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetAnalyticsConfigurations(struct soap *soap, struct ns5__GetConfiguration *ns5__GetAnalyticsConfigurations, struct _ns5__GetAnalyticsConfigurationsResponse *ns5__GetAnalyticsConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetMetadataConfigurations' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetMetadataConfigurations(struct soap *soap, struct ns5__GetConfiguration *ns5__GetMetadataConfigurations, struct _ns5__GetMetadataConfigurationsResponse *ns5__GetMetadataConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetAudioOutputConfigurations' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetAudioOutputConfigurations(struct soap *soap, struct ns5__GetConfiguration *ns5__GetAudioOutputConfigurations, struct _ns5__GetAudioOutputConfigurationsResponse *ns5__GetAudioOutputConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetAudioDecoderConfigurations' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetAudioDecoderConfigurations(struct soap *soap, struct ns5__GetConfiguration *ns5__GetAudioDecoderConfigurations, struct _ns5__GetAudioDecoderConfigurationsResponse *ns5__GetAudioDecoderConfigurationsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__SetVideoSourceConfiguration' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__SetVideoSourceConfiguration(struct soap *soap, struct _ns5__SetVideoSourceConfiguration *ns5__SetVideoSourceConfiguration, struct ns5__SetConfigurationResponse *ns5__SetVideoSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__SetVideoEncoderConfiguration' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__SetVideoEncoderConfiguration(struct soap *soap, struct _ns5__SetVideoEncoderConfiguration *ns5__SetVideoEncoderConfiguration, struct ns5__SetConfigurationResponse *ns5__SetVideoEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__SetAudioSourceConfiguration' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__SetAudioSourceConfiguration(struct soap *soap, struct _ns5__SetAudioSourceConfiguration *ns5__SetAudioSourceConfiguration, struct ns5__SetConfigurationResponse *ns5__SetAudioSourceConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__SetAudioEncoderConfiguration' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__SetAudioEncoderConfiguration(struct soap *soap, struct _ns5__SetAudioEncoderConfiguration *ns5__SetAudioEncoderConfiguration, struct ns5__SetConfigurationResponse *ns5__SetAudioEncoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__SetMetadataConfiguration' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__SetMetadataConfiguration(struct soap *soap, struct _ns5__SetMetadataConfiguration *ns5__SetMetadataConfiguration, struct ns5__SetConfigurationResponse *ns5__SetMetadataConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__SetAudioOutputConfiguration' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__SetAudioOutputConfiguration(struct soap *soap, struct _ns5__SetAudioOutputConfiguration *ns5__SetAudioOutputConfiguration, struct ns5__SetConfigurationResponse *ns5__SetAudioOutputConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__SetAudioDecoderConfiguration' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__SetAudioDecoderConfiguration(struct soap *soap, struct _ns5__SetAudioDecoderConfiguration *ns5__SetAudioDecoderConfiguration, struct ns5__SetConfigurationResponse *ns5__SetAudioDecoderConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetVideoSourceConfigurationOptions' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetVideoSourceConfigurationOptions(struct soap *soap, struct ns5__GetConfiguration *ns5__GetVideoSourceConfigurationOptions, struct _ns5__GetVideoSourceConfigurationOptionsResponse *ns5__GetVideoSourceConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetVideoEncoderConfigurationOptions' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetVideoEncoderConfigurationOptions(struct soap *soap, struct ns5__GetConfiguration *ns5__GetVideoEncoderConfigurationOptions, struct _ns5__GetVideoEncoderConfigurationOptionsResponse *ns5__GetVideoEncoderConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetAudioSourceConfigurationOptions' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetAudioSourceConfigurationOptions(struct soap *soap, struct ns5__GetConfiguration *ns5__GetAudioSourceConfigurationOptions, struct _ns5__GetAudioSourceConfigurationOptionsResponse *ns5__GetAudioSourceConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetAudioEncoderConfigurationOptions' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetAudioEncoderConfigurationOptions(struct soap *soap, struct ns5__GetConfiguration *ns5__GetAudioEncoderConfigurationOptions, struct _ns5__GetAudioEncoderConfigurationOptionsResponse *ns5__GetAudioEncoderConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetMetadataConfigurationOptions' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetMetadataConfigurationOptions(struct soap *soap, struct ns5__GetConfiguration *ns5__GetMetadataConfigurationOptions, struct _ns5__GetMetadataConfigurationOptionsResponse *ns5__GetMetadataConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetAudioOutputConfigurationOptions' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetAudioOutputConfigurationOptions(struct soap *soap, struct ns5__GetConfiguration *ns5__GetAudioOutputConfigurationOptions, struct _ns5__GetAudioOutputConfigurationOptionsResponse *ns5__GetAudioOutputConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetAudioDecoderConfigurationOptions' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetAudioDecoderConfigurationOptions(struct soap *soap, struct ns5__GetConfiguration *ns5__GetAudioDecoderConfigurationOptions, struct _ns5__GetAudioDecoderConfigurationOptionsResponse *ns5__GetAudioDecoderConfigurationOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetVideoEncoderInstances' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetVideoEncoderInstances(struct soap *soap, struct _ns5__GetVideoEncoderInstances *ns5__GetVideoEncoderInstances, struct _ns5__GetVideoEncoderInstancesResponse *ns5__GetVideoEncoderInstancesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetStreamUri' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetStreamUri(struct soap *soap, struct _ns5__GetStreamUri *ns5__GetStreamUri, struct _ns5__GetStreamUriResponse *ns5__GetStreamUriResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__StartMulticastStreaming' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__StartMulticastStreaming(struct soap *soap, struct ns5__StartStopMulticastStreaming *ns5__StartMulticastStreaming, struct ns5__SetConfigurationResponse *ns5__StartMulticastStreamingResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__StopMulticastStreaming' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__StopMulticastStreaming(struct soap *soap, struct ns5__StartStopMulticastStreaming *ns5__StopMulticastStreaming, struct ns5__SetConfigurationResponse *ns5__StopMulticastStreamingResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__SetSynchronizationPoint' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__SetSynchronizationPoint(struct soap *soap, struct _ns5__SetSynchronizationPoint *ns5__SetSynchronizationPoint, struct _ns5__SetSynchronizationPointResponse *ns5__SetSynchronizationPointResponse){
//     LOG_DBG(LOG_TAG, "\n");
 
//     struct tt__SystemDateTime *SystemDateAndTime = (struct tt__SystemDateTime *)soap_zalloc(soap, sizeof(struct tt__SystemDateTime));
//     SystemDateAndTime->TimeZone = (struct tt__TimeZone *)soap_zalloc(soap, sizeof(struct tt__TimeZone));
//     SystemDateAndTime->TimeZone->TZ = (char *)soap_zalloc(soap, 100);
//     SystemDateAndTime->UTCDateTime = (struct tt__DateTime *)soap_zalloc(soap, sizeof(struct tt__DateTime));
//     SystemDateAndTime->UTCDateTime->Time = (struct tt__Time *)soap_zalloc(soap, sizeof(struct tt__Time));
//     SystemDateAndTime->UTCDateTime->Date = (struct tt__Date *)soap_zalloc(soap, sizeof(struct tt__Date));
//     SystemDateAndTime->LocalDateTime = (struct tt__DateTime *)soap_zalloc(soap, sizeof(struct tt__DateTime));
//     SystemDateAndTime->LocalDateTime->Time = (struct tt__Time *)soap_zalloc(soap, sizeof(struct tt__Time));
//     SystemDateAndTime->LocalDateTime->Date = (struct tt__Date *)soap_zalloc(soap, sizeof(struct tt__Date));
//     SystemDateAndTime->Extension = (struct tt__SystemDateTimeExtension *)soap_zalloc(soap, sizeof(struct tt__SystemDateTimeExtension));

//     SystemDateAndTime->DateTimeType = tt__SetDateTimeType__Manual;//0
//     SystemDateAndTime->DaylightSavings = xsd__boolean__false_;//0

//     strcpy(SystemDateAndTime->TimeZone->TZ, g_configData->system_config.timezone);

//     time_t timev;

//     timev = time(NULL);
//     struct tm mytm = *localtime(&timev);

//     SystemDateAndTime->UTCDateTime->Date->Year  = mytm.tm_year+1900;
//     SystemDateAndTime->UTCDateTime->Date->Month = mytm.tm_mon + 1;
//     SystemDateAndTime->UTCDateTime->Date->Day   = mytm.tm_mday;

//     SystemDateAndTime->UTCDateTime->Time->Hour   = mytm.tm_hour;
//     SystemDateAndTime->UTCDateTime->Time->Minute = mytm.tm_min;
//     SystemDateAndTime->UTCDateTime->Time->Second = mytm.tm_sec;

//     timev += g_configData->system_config.timezone_offset_s;
//     mytm = *localtime(&timev);

//     SystemDateAndTime->LocalDateTime->Date->Year  = mytm.tm_year + 1900;
//     SystemDateAndTime->LocalDateTime->Date->Month = mytm.tm_mon + 1;
//     SystemDateAndTime->LocalDateTime->Date->Day   = mytm.tm_mday;

//     SystemDateAndTime->LocalDateTime->Time->Hour   = mytm.tm_hour;
//     SystemDateAndTime->LocalDateTime->Time->Minute = mytm.tm_min;
//     SystemDateAndTime->LocalDateTime->Time->Second = mytm.tm_sec;

//     ns5__SetSynchronizationPointResponse->SystemDateAndTime = SystemDateAndTime;


//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetSnapshotUri' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetSnapshotUri(struct soap *soap, struct _ns5__GetSnapshotUri *ns5__GetSnapshotUri, struct _ns5__GetSnapshotUriResponse *ns5__GetSnapshotUriResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetVideoSourceModes' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetVideoSourceModes(struct soap *soap, struct _ns5__GetVideoSourceModes *ns5__GetVideoSourceModes, struct _ns5__GetVideoSourceModesResponse *ns5__GetVideoSourceModesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__SetVideoSourceMode' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__SetVideoSourceMode(struct soap *soap, struct _ns5__SetVideoSourceMode *ns5__SetVideoSourceMode, struct _ns5__SetVideoSourceModeResponse *ns5__SetVideoSourceModeResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetOSDs' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetOSDs(struct soap *soap, struct _ns5__GetOSDs *ns5__GetOSDs, struct _ns5__GetOSDsResponse *ns5__GetOSDsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetOSDOptions' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetOSDOptions(struct soap *soap, struct _ns5__GetOSDOptions *ns5__GetOSDOptions, struct _ns5__GetOSDOptionsResponse *ns5__GetOSDOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__SetOSD' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__SetOSD(struct soap *soap, struct _ns5__SetOSD *ns5__SetOSD, struct ns5__SetConfigurationResponse *ns5__SetOSDResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__CreateOSD' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__CreateOSD(struct soap *soap, struct _ns5__CreateOSD *ns5__CreateOSD, struct _ns5__CreateOSDResponse *ns5__CreateOSDResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__DeleteOSD' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__DeleteOSD(struct soap *soap, struct _ns5__DeleteOSD *ns5__DeleteOSD, struct ns5__SetConfigurationResponse *ns5__DeleteOSDResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetMasks' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetMasks(struct soap *soap, struct _ns5__GetMasks *ns5__GetMasks, struct _ns5__GetMasksResponse *ns5__GetMasksResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__GetMaskOptions' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__GetMaskOptions(struct soap *soap, struct _ns5__GetMaskOptions *ns5__GetMaskOptions, struct _ns5__GetMaskOptionsResponse *ns5__GetMaskOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__SetMask' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__SetMask(struct soap *soap, struct _ns5__SetMask *ns5__SetMask, struct ns5__SetConfigurationResponse *ns5__SetMaskResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__CreateMask' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__CreateMask(struct soap *soap, struct _ns5__CreateMask *ns5__CreateMask, struct _ns5__CreateMaskResponse *ns5__CreateMaskResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service operation '__ns5__DeleteMask' implementation, should return SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 __ns5__DeleteMask(struct soap *soap, struct _ns5__DeleteMask *ns5__DeleteMask, struct ns5__SetConfigurationResponse *ns5__DeleteMaskResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
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
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetServiceCapabilities(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetVideoSources(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetAudioSources(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetAudioOutputs(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__CreateProfile(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetProfile(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetProfiles(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__AddVideoEncoderConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__AddVideoSourceConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__AddAudioEncoderConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__AddAudioSourceConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__AddPTZConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__AddVideoAnalyticsConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__AddMetadataConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__AddAudioOutputConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__AddAudioDecoderConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__RemoveVideoEncoderConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__RemoveVideoSourceConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__RemoveAudioEncoderConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__RemoveAudioSourceConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__RemovePTZConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__RemoveVideoAnalyticsConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__RemoveMetadataConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__RemoveAudioOutputConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__RemoveAudioDecoderConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__DeleteProfile(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetVideoSourceConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetVideoEncoderConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetAudioSourceConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetAudioEncoderConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetVideoAnalyticsConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetMetadataConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetAudioOutputConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetAudioDecoderConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetVideoSourceConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetVideoEncoderConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetAudioSourceConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetAudioEncoderConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetVideoAnalyticsConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetMetadataConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetAudioOutputConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetAudioDecoderConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetCompatibleVideoEncoderConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetCompatibleVideoSourceConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetCompatibleAudioEncoderConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetCompatibleAudioSourceConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetCompatibleVideoAnalyticsConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetCompatibleMetadataConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetCompatibleAudioOutputConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetCompatibleAudioDecoderConfigurations(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__SetVideoSourceConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__SetVideoEncoderConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__SetAudioSourceConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__SetAudioEncoderConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__SetVideoAnalyticsConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__SetMetadataConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__SetAudioOutputConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__SetAudioDecoderConfiguration(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetVideoSourceConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetVideoEncoderConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetAudioSourceConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetAudioEncoderConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetMetadataConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetAudioOutputConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetAudioDecoderConfigurationOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetGuaranteedNumberOfVideoEncoderInstances(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetStreamUri(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__StartMulticastStreaming(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__StopMulticastStreaming(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__SetSynchronizationPoint(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetSnapshotUri(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetVideoSourceModes(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__SetVideoSourceMode(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetOSDs(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetOSD(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetOSDOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__SetOSD(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__CreateOSD(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__DeleteOSD(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
