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

  
//     /** Web service synchronous operation 'soap_call___tad__GetServiceCapabilities' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tad__GetServiceCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetServiceCapabilities *tad__GetServiceCapabilities, struct _tad__GetServiceCapabilitiesResponse *tad__GetServiceCapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tad__GetServiceCapabilities' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tad__GetServiceCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetServiceCapabilities *tad__GetServiceCapabilities){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tad__GetServiceCapabilities' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tad__GetServiceCapabilities(struct soap *soap, struct _tad__GetServiceCapabilitiesResponse *tad__GetServiceCapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tad__DeleteAnalyticsEngineControl' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tad__DeleteAnalyticsEngineControl(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__DeleteAnalyticsEngineControl *tad__DeleteAnalyticsEngineControl, struct _tad__DeleteAnalyticsEngineControlResponse *tad__DeleteAnalyticsEngineControlResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tad__DeleteAnalyticsEngineControl' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tad__DeleteAnalyticsEngineControl(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__DeleteAnalyticsEngineControl *tad__DeleteAnalyticsEngineControl){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tad__DeleteAnalyticsEngineControl' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tad__DeleteAnalyticsEngineControl(struct soap *soap, struct _tad__DeleteAnalyticsEngineControlResponse *tad__DeleteAnalyticsEngineControlResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tad__CreateAnalyticsEngineControl' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tad__CreateAnalyticsEngineControl(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__CreateAnalyticsEngineControl *tad__CreateAnalyticsEngineControl, struct _tad__CreateAnalyticsEngineControlResponse *tad__CreateAnalyticsEngineControlResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tad__CreateAnalyticsEngineControl' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tad__CreateAnalyticsEngineControl(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__CreateAnalyticsEngineControl *tad__CreateAnalyticsEngineControl){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tad__CreateAnalyticsEngineControl' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tad__CreateAnalyticsEngineControl(struct soap *soap, struct _tad__CreateAnalyticsEngineControlResponse *tad__CreateAnalyticsEngineControlResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tad__SetAnalyticsEngineControl' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tad__SetAnalyticsEngineControl(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__SetAnalyticsEngineControl *tad__SetAnalyticsEngineControl, struct _tad__SetAnalyticsEngineControlResponse *tad__SetAnalyticsEngineControlResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tad__SetAnalyticsEngineControl' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tad__SetAnalyticsEngineControl(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__SetAnalyticsEngineControl *tad__SetAnalyticsEngineControl){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tad__SetAnalyticsEngineControl' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tad__SetAnalyticsEngineControl(struct soap *soap, struct _tad__SetAnalyticsEngineControlResponse *tad__SetAnalyticsEngineControlResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tad__GetAnalyticsEngineControl' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tad__GetAnalyticsEngineControl(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetAnalyticsEngineControl *tad__GetAnalyticsEngineControl, struct _tad__GetAnalyticsEngineControlResponse *tad__GetAnalyticsEngineControlResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tad__GetAnalyticsEngineControl' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tad__GetAnalyticsEngineControl(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetAnalyticsEngineControl *tad__GetAnalyticsEngineControl){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tad__GetAnalyticsEngineControl' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tad__GetAnalyticsEngineControl(struct soap *soap, struct _tad__GetAnalyticsEngineControlResponse *tad__GetAnalyticsEngineControlResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tad__GetAnalyticsEngineControls' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tad__GetAnalyticsEngineControls(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetAnalyticsEngineControls *tad__GetAnalyticsEngineControls, struct _tad__GetAnalyticsEngineControlsResponse *tad__GetAnalyticsEngineControlsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tad__GetAnalyticsEngineControls' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tad__GetAnalyticsEngineControls(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetAnalyticsEngineControls *tad__GetAnalyticsEngineControls){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tad__GetAnalyticsEngineControls' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tad__GetAnalyticsEngineControls(struct soap *soap, struct _tad__GetAnalyticsEngineControlsResponse *tad__GetAnalyticsEngineControlsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tad__GetAnalyticsEngine' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tad__GetAnalyticsEngine(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetAnalyticsEngine *tad__GetAnalyticsEngine, struct _tad__GetAnalyticsEngineResponse *tad__GetAnalyticsEngineResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tad__GetAnalyticsEngine' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tad__GetAnalyticsEngine(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetAnalyticsEngine *tad__GetAnalyticsEngine){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tad__GetAnalyticsEngine' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tad__GetAnalyticsEngine(struct soap *soap, struct _tad__GetAnalyticsEngineResponse *tad__GetAnalyticsEngineResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tad__GetAnalyticsEngines' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tad__GetAnalyticsEngines(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetAnalyticsEngines *tad__GetAnalyticsEngines, struct _tad__GetAnalyticsEnginesResponse *tad__GetAnalyticsEnginesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tad__GetAnalyticsEngines' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tad__GetAnalyticsEngines(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetAnalyticsEngines *tad__GetAnalyticsEngines){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tad__GetAnalyticsEngines' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tad__GetAnalyticsEngines(struct soap *soap, struct _tad__GetAnalyticsEnginesResponse *tad__GetAnalyticsEnginesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tad__SetVideoAnalyticsConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tad__SetVideoAnalyticsConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__SetVideoAnalyticsConfiguration *tad__SetVideoAnalyticsConfiguration, struct _tad__SetVideoAnalyticsConfigurationResponse *tad__SetVideoAnalyticsConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tad__SetVideoAnalyticsConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tad__SetVideoAnalyticsConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__SetVideoAnalyticsConfiguration *tad__SetVideoAnalyticsConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tad__SetVideoAnalyticsConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tad__SetVideoAnalyticsConfiguration(struct soap *soap, struct _tad__SetVideoAnalyticsConfigurationResponse *tad__SetVideoAnalyticsConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tad__SetAnalyticsEngineInput' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tad__SetAnalyticsEngineInput(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__SetAnalyticsEngineInput *tad__SetAnalyticsEngineInput, struct _tad__SetAnalyticsEngineInputResponse *tad__SetAnalyticsEngineInputResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tad__SetAnalyticsEngineInput' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tad__SetAnalyticsEngineInput(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__SetAnalyticsEngineInput *tad__SetAnalyticsEngineInput){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tad__SetAnalyticsEngineInput' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tad__SetAnalyticsEngineInput(struct soap *soap, struct _tad__SetAnalyticsEngineInputResponse *tad__SetAnalyticsEngineInputResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tad__GetAnalyticsEngineInput' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tad__GetAnalyticsEngineInput(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetAnalyticsEngineInput *tad__GetAnalyticsEngineInput, struct _tad__GetAnalyticsEngineInputResponse *tad__GetAnalyticsEngineInputResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tad__GetAnalyticsEngineInput' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tad__GetAnalyticsEngineInput(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetAnalyticsEngineInput *tad__GetAnalyticsEngineInput){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tad__GetAnalyticsEngineInput' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tad__GetAnalyticsEngineInput(struct soap *soap, struct _tad__GetAnalyticsEngineInputResponse *tad__GetAnalyticsEngineInputResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tad__GetAnalyticsEngineInputs' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tad__GetAnalyticsEngineInputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetAnalyticsEngineInputs *tad__GetAnalyticsEngineInputs, struct _tad__GetAnalyticsEngineInputsResponse *tad__GetAnalyticsEngineInputsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tad__GetAnalyticsEngineInputs' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tad__GetAnalyticsEngineInputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetAnalyticsEngineInputs *tad__GetAnalyticsEngineInputs){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tad__GetAnalyticsEngineInputs' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tad__GetAnalyticsEngineInputs(struct soap *soap, struct _tad__GetAnalyticsEngineInputsResponse *tad__GetAnalyticsEngineInputsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tad__GetAnalyticsDeviceStreamUri' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tad__GetAnalyticsDeviceStreamUri(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetAnalyticsDeviceStreamUri *tad__GetAnalyticsDeviceStreamUri, struct _tad__GetAnalyticsDeviceStreamUriResponse *tad__GetAnalyticsDeviceStreamUriResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tad__GetAnalyticsDeviceStreamUri' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tad__GetAnalyticsDeviceStreamUri(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetAnalyticsDeviceStreamUri *tad__GetAnalyticsDeviceStreamUri){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tad__GetAnalyticsDeviceStreamUri' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tad__GetAnalyticsDeviceStreamUri(struct soap *soap, struct _tad__GetAnalyticsDeviceStreamUriResponse *tad__GetAnalyticsDeviceStreamUriResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tad__GetVideoAnalyticsConfiguration' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tad__GetVideoAnalyticsConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetVideoAnalyticsConfiguration *tad__GetVideoAnalyticsConfiguration, struct _tad__GetVideoAnalyticsConfigurationResponse *tad__GetVideoAnalyticsConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tad__GetVideoAnalyticsConfiguration' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tad__GetVideoAnalyticsConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetVideoAnalyticsConfiguration *tad__GetVideoAnalyticsConfiguration){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tad__GetVideoAnalyticsConfiguration' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tad__GetVideoAnalyticsConfiguration(struct soap *soap, struct _tad__GetVideoAnalyticsConfigurationResponse *tad__GetVideoAnalyticsConfigurationResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tad__CreateAnalyticsEngineInputs' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tad__CreateAnalyticsEngineInputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__CreateAnalyticsEngineInputs *tad__CreateAnalyticsEngineInputs, struct _tad__CreateAnalyticsEngineInputsResponse *tad__CreateAnalyticsEngineInputsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tad__CreateAnalyticsEngineInputs' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tad__CreateAnalyticsEngineInputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__CreateAnalyticsEngineInputs *tad__CreateAnalyticsEngineInputs){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tad__CreateAnalyticsEngineInputs' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tad__CreateAnalyticsEngineInputs(struct soap *soap, struct _tad__CreateAnalyticsEngineInputsResponse *tad__CreateAnalyticsEngineInputsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tad__DeleteAnalyticsEngineInputs' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tad__DeleteAnalyticsEngineInputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__DeleteAnalyticsEngineInputs *tad__DeleteAnalyticsEngineInputs, struct _tad__DeleteAnalyticsEngineInputsResponse *tad__DeleteAnalyticsEngineInputsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tad__DeleteAnalyticsEngineInputs' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tad__DeleteAnalyticsEngineInputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__DeleteAnalyticsEngineInputs *tad__DeleteAnalyticsEngineInputs){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tad__DeleteAnalyticsEngineInputs' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tad__DeleteAnalyticsEngineInputs(struct soap *soap, struct _tad__DeleteAnalyticsEngineInputsResponse *tad__DeleteAnalyticsEngineInputsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___tad__GetAnalyticsState' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___tad__GetAnalyticsState(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetAnalyticsState *tad__GetAnalyticsState, struct _tad__GetAnalyticsStateResponse *tad__GetAnalyticsStateResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___tad__GetAnalyticsState' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___tad__GetAnalyticsState(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tad__GetAnalyticsState *tad__GetAnalyticsState){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___tad__GetAnalyticsState' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___tad__GetAnalyticsState(struct soap *soap, struct _tad__GetAnalyticsStateResponse *tad__GetAnalyticsStateResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    /** Web service operation '__tad__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tad__GetServiceCapabilities(struct soap *soap, struct _tad__GetServiceCapabilities *tad__GetServiceCapabilities, struct _tad__GetServiceCapabilitiesResponse *tad__GetServiceCapabilitiesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tad__DeleteAnalyticsEngineControl' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tad__DeleteAnalyticsEngineControl(struct soap *soap, struct _tad__DeleteAnalyticsEngineControl *tad__DeleteAnalyticsEngineControl, struct _tad__DeleteAnalyticsEngineControlResponse *tad__DeleteAnalyticsEngineControlResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tad__CreateAnalyticsEngineControl' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tad__CreateAnalyticsEngineControl(struct soap *soap, struct _tad__CreateAnalyticsEngineControl *tad__CreateAnalyticsEngineControl, struct _tad__CreateAnalyticsEngineControlResponse *tad__CreateAnalyticsEngineControlResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tad__SetAnalyticsEngineControl' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tad__SetAnalyticsEngineControl(struct soap *soap, struct _tad__SetAnalyticsEngineControl *tad__SetAnalyticsEngineControl, struct _tad__SetAnalyticsEngineControlResponse *tad__SetAnalyticsEngineControlResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tad__GetAnalyticsEngineControl' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tad__GetAnalyticsEngineControl(struct soap *soap, struct _tad__GetAnalyticsEngineControl *tad__GetAnalyticsEngineControl, struct _tad__GetAnalyticsEngineControlResponse *tad__GetAnalyticsEngineControlResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tad__GetAnalyticsEngineControls' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tad__GetAnalyticsEngineControls(struct soap *soap, struct _tad__GetAnalyticsEngineControls *tad__GetAnalyticsEngineControls, struct _tad__GetAnalyticsEngineControlsResponse *tad__GetAnalyticsEngineControlsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tad__GetAnalyticsEngine' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tad__GetAnalyticsEngine(struct soap *soap, struct _tad__GetAnalyticsEngine *tad__GetAnalyticsEngine, struct _tad__GetAnalyticsEngineResponse *tad__GetAnalyticsEngineResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tad__GetAnalyticsEngines' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tad__GetAnalyticsEngines(struct soap *soap, struct _tad__GetAnalyticsEngines *tad__GetAnalyticsEngines, struct _tad__GetAnalyticsEnginesResponse *tad__GetAnalyticsEnginesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tad__SetVideoAnalyticsConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tad__SetVideoAnalyticsConfiguration(struct soap *soap, struct _tad__SetVideoAnalyticsConfiguration *tad__SetVideoAnalyticsConfiguration, struct _tad__SetVideoAnalyticsConfigurationResponse *tad__SetVideoAnalyticsConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tad__SetAnalyticsEngineInput' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tad__SetAnalyticsEngineInput(struct soap *soap, struct _tad__SetAnalyticsEngineInput *tad__SetAnalyticsEngineInput, struct _tad__SetAnalyticsEngineInputResponse *tad__SetAnalyticsEngineInputResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tad__GetAnalyticsEngineInput' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tad__GetAnalyticsEngineInput(struct soap *soap, struct _tad__GetAnalyticsEngineInput *tad__GetAnalyticsEngineInput, struct _tad__GetAnalyticsEngineInputResponse *tad__GetAnalyticsEngineInputResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tad__GetAnalyticsEngineInputs' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tad__GetAnalyticsEngineInputs(struct soap *soap, struct _tad__GetAnalyticsEngineInputs *tad__GetAnalyticsEngineInputs, struct _tad__GetAnalyticsEngineInputsResponse *tad__GetAnalyticsEngineInputsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tad__GetAnalyticsDeviceStreamUri' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tad__GetAnalyticsDeviceStreamUri(struct soap *soap, struct _tad__GetAnalyticsDeviceStreamUri *tad__GetAnalyticsDeviceStreamUri, struct _tad__GetAnalyticsDeviceStreamUriResponse *tad__GetAnalyticsDeviceStreamUriResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tad__GetVideoAnalyticsConfiguration' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tad__GetVideoAnalyticsConfiguration(struct soap *soap, struct _tad__GetVideoAnalyticsConfiguration *tad__GetVideoAnalyticsConfiguration, struct _tad__GetVideoAnalyticsConfigurationResponse *tad__GetVideoAnalyticsConfigurationResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tad__CreateAnalyticsEngineInputs' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tad__CreateAnalyticsEngineInputs(struct soap *soap, struct _tad__CreateAnalyticsEngineInputs *tad__CreateAnalyticsEngineInputs, struct _tad__CreateAnalyticsEngineInputsResponse *tad__CreateAnalyticsEngineInputsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tad__DeleteAnalyticsEngineInputs' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tad__DeleteAnalyticsEngineInputs(struct soap *soap, struct _tad__DeleteAnalyticsEngineInputs *tad__DeleteAnalyticsEngineInputs, struct _tad__DeleteAnalyticsEngineInputsResponse *tad__DeleteAnalyticsEngineInputsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tad__GetAnalyticsState' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tad__GetAnalyticsState(struct soap *soap, struct _tad__GetAnalyticsState *tad__GetAnalyticsState, struct _tad__GetAnalyticsStateResponse *tad__GetAnalyticsStateResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}

