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
/******************************************************************************\
 *                                                                            *
 * Client-Side Call Stub Functions                                            *
 *                                                                            *
\******************************************************************************/

    
//     /** Web service synchronous operation 'soap_call___timg__GetServiceCapabilities' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__GetServiceCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetServiceCapabilities *timg__GetServiceCapabilities, struct _timg__GetServiceCapabilitiesResponse *timg__GetServiceCapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___timg__GetServiceCapabilities' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___timg__GetServiceCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetServiceCapabilities *timg__GetServiceCapabilities){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___timg__GetServiceCapabilities' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___timg__GetServiceCapabilities(struct soap *soap, struct _timg__GetServiceCapabilitiesResponse *timg__GetServiceCapabilitiesResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___timg__GetImagingSettings' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__GetImagingSettings(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetImagingSettings *timg__GetImagingSettings, struct _timg__GetImagingSettingsResponse *timg__GetImagingSettingsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___timg__GetImagingSettings' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___timg__GetImagingSettings(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetImagingSettings *timg__GetImagingSettings){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___timg__GetImagingSettings' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___timg__GetImagingSettings(struct soap *soap, struct _timg__GetImagingSettingsResponse *timg__GetImagingSettingsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___timg__SetImagingSettings' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__SetImagingSettings(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__SetImagingSettings *timg__SetImagingSettings, struct _timg__SetImagingSettingsResponse *timg__SetImagingSettingsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___timg__SetImagingSettings' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___timg__SetImagingSettings(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__SetImagingSettings *timg__SetImagingSettings){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___timg__SetImagingSettings' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___timg__SetImagingSettings(struct soap *soap, struct _timg__SetImagingSettingsResponse *timg__SetImagingSettingsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___timg__GetOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__GetOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetOptions *timg__GetOptions, struct _timg__GetOptionsResponse *timg__GetOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___timg__GetOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___timg__GetOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetOptions *timg__GetOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___timg__GetOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___timg__GetOptions(struct soap *soap, struct _timg__GetOptionsResponse *timg__GetOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___timg__Move' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__Move(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__Move *timg__Move, struct _timg__MoveResponse *timg__MoveResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___timg__Move' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___timg__Move(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__Move *timg__Move){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___timg__Move' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___timg__Move(struct soap *soap, struct _timg__MoveResponse *timg__MoveResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___timg__Stop' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__Stop(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__Stop *timg__Stop, struct _timg__StopResponse *timg__StopResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___timg__Stop' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___timg__Stop(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__Stop *timg__Stop){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___timg__Stop' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___timg__Stop(struct soap *soap, struct _timg__StopResponse *timg__StopResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___timg__GetStatus' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__GetStatus(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetStatus *timg__GetStatus, struct _timg__GetStatusResponse *timg__GetStatusResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___timg__GetStatus' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___timg__GetStatus(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetStatus *timg__GetStatus){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___timg__GetStatus' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___timg__GetStatus(struct soap *soap, struct _timg__GetStatusResponse *timg__GetStatusResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___timg__GetMoveOptions' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__GetMoveOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetMoveOptions *timg__GetMoveOptions, struct _timg__GetMoveOptionsResponse *timg__GetMoveOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___timg__GetMoveOptions' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___timg__GetMoveOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetMoveOptions *timg__GetMoveOptions){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___timg__GetMoveOptions' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___timg__GetMoveOptions(struct soap *soap, struct _timg__GetMoveOptionsResponse *timg__GetMoveOptionsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___timg__GetPresets' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__GetPresets(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetPresets *timg__GetPresets, struct _timg__GetPresetsResponse *timg__GetPresetsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___timg__GetPresets' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___timg__GetPresets(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetPresets *timg__GetPresets){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___timg__GetPresets' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___timg__GetPresets(struct soap *soap, struct _timg__GetPresetsResponse *timg__GetPresetsResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___timg__GetCurrentPreset' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__GetCurrentPreset(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetCurrentPreset *timg__GetCurrentPreset, struct _timg__GetCurrentPresetResponse *timg__GetCurrentPresetResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___timg__GetCurrentPreset' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___timg__GetCurrentPreset(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetCurrentPreset *timg__GetCurrentPreset){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___timg__GetCurrentPreset' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___timg__GetCurrentPreset(struct soap *soap, struct _timg__GetCurrentPresetResponse *timg__GetCurrentPresetResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
    
//     /** Web service synchronous operation 'soap_call___timg__SetCurrentPreset' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__SetCurrentPreset(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__SetCurrentPreset *timg__SetCurrentPreset, struct _timg__SetCurrentPresetResponse *timg__SetCurrentPresetResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_send___timg__SetCurrentPreset' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_send___timg__SetCurrentPreset(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__SetCurrentPreset *timg__SetCurrentPreset){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }
//     /** Web service asynchronous operation 'soap_recv___timg__SetCurrentPreset' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
//     SOAP_FMAC5 int SOAP_FMAC6 soap_recv___timg__SetCurrentPreset(struct soap *soap, struct _timg__SetCurrentPresetResponse *timg__SetCurrentPresetResponse){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

/******************************************************************************\
 *                                                                            *
 * Server-Side Operations                                                     *
 *                                                                            *
\******************************************************************************/

    /** Web service operation '__timg__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __timg__GetServiceCapabilities(struct soap* soap, struct _timg__GetServiceCapabilities *timg__GetServiceCapabilities, struct _timg__GetServiceCapabilitiesResponse *timg__GetServiceCapabilitiesResponse){
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
    LOG_DBG(LOG_TAG, "\n");

    unsigned char Brightness = 0;
    unsigned char Contrast = 0;
    unsigned char ColorSaturation = 0;

    int Brightness_int = 0;
    int Contrast_int = 0;
    int ColorSaturation_int = 0;

    Brightness_int = (int)(*timg__SetImagingSettings->ImagingSettings->Brightness);
    Contrast_int = (int)(*timg__SetImagingSettings->ImagingSettings->Contrast);
    ColorSaturation_int = (int)(*timg__SetImagingSettings->ImagingSettings->ColorSaturation);

    if (Brightness_int >= 0 && Brightness_int <100)
    {
        Brightness_int = (Brightness_int * 256) / 100;
    }
    else if (100 == Brightness_int)
    {
        Brightness_int = 255;
    }
    else
    {
        MYTRACE("arguments fault!\n");
    }

    if (Contrast_int >= 0 && Contrast_int < 100)
    {
        Contrast_int = (Contrast_int * 256) / 100;
    }
    else if (Contrast_int == 100)
    {
        Contrast_int = 255;
    }
    else
    {
        MYTRACE("arguments fault!\n");
    }

    if (ColorSaturation_int >= 0 && ColorSaturation_int < 100)
    {
        ColorSaturation_int = (ColorSaturation_int * 256) / 100;
    }
    else if (ColorSaturation_int == 100)
    {
        ColorSaturation_int = 255;
    }
    else
    {
        MYTRACE("arguments fault!\n");
    }

    Brightness = (unsigned char)Brightness_int;
    Contrast = (unsigned char)Contrast_int;
    ColorSaturation = (unsigned char)ColorSaturation_int;

    /*NVR设备暂时没有锐度的参数，后续如果有需要再行打开--Haokaijun*/
    /*
    unsigned char Sharpness = 0;
    int Sharpness_int = 0;
    IMPISPTuningOpsMode WideDynamicRange_Mode;

    if (Sharpness_int >= 0 && Sharpness_int < 100)
    {
        Sharpness_int = (Sharpness_int * 256) / 100;
    }
    else if (Sharpness_int == 100)
    {
        Sharpness_int = 255;
    }
    else
    {
        MYTRACE("arguments fault!\n");
    }
    Sharpness = (unsigned char)Sharpness_int;*/


    /*NVR设备暂时没有WDR的参数，后续如果有需要再行打开--Haokaijun*/
    /*if (timg__SetImagingSettings->ImagingSettings->WideDynamicRange->Mode < IMPISP_TUNING_OPS_MODE_BUTT)
    {
        WideDynamicRange_Mode = (IMPISPTuningOpsMode)timg__SetImagingSettings->ImagingSettings->WideDynamicRange->Mode;
    }*/


    /*测试log，程序出错打开调试用*/
    /*printf("Rcv token = %s\n", timg__SetImagingSettings->VideoSourceToken);
    printf("Rcv Brightness value = %d\n", Brightness);
    printf("Rcv Contrast value = %d\n", Contrast);
    printf("Rcv ColorSaturation value = %d\n", ColorSaturation);*/

    /*设置亮度、对比度、锐度、颜色饱和度、WDR开关等参数*/
#ifdef PLATFORM_T40
    IMP_ISP_Tuning_SetBrightness(IMPVI_MAIN, &Brightness);
    IMP_ISP_Tuning_SetContrast(IMPVI_MAIN, &Contrast);
    IMP_ISP_Tuning_SetSaturation(IMPVI_MAIN, &ColorSaturation);
    /*NVR设备暂时没有锐度的参数，后续如果有需要再行打开--Haokaijun*/
    /*IMP_ISP_Tuning_SetSharpness(IMPVI_MAIN, &Sharpness);*/
    /*NVR设备暂时没有WDR的参数，后续如果有需要再行打开--Haokaijun*/
    /*IMP_ISP_WDR_ENABLE(IMPVI_MAIN, &WideDynamicRange_Mode);
    IMP_ISP_Tuning_SetBcshHue();*/
#else
    IMP_ISP_Tuning_SetBrightness(&Brightness);
    IMP_ISP_Tuning_SetContrast(&Contrast);
    IMP_ISP_Tuning_SetSaturation(&ColorSaturation);
    /*NVR设备暂时没有锐度的参数，后续如果有需要再行打开--Haokaijun*/
    /*IMP_ISP_Tuning_SetSharpness(&Sharpness);*/
    /*NVR设备暂时没有WDR的参数，后续如果有需要再行打开--Haokaijun*/
    /*IMP_ISP_WDR_ENABLE(&WideDynamicRange_Mode); 
    IMP_ISP_Tuning_SetBcshHue();*/
#endif
 
    return SOAP_OK;
}
/** Web service operation '__timg__GetOptions' implementation, should return SOAP_OK or error code */
SOAP_FMAC5 int SOAP_FMAC6 __timg__GetOptions(struct soap* soap, struct _timg__GetOptions* timg__GetOptions, struct _timg__GetOptionsResponse* timg__GetOptionsResponse)
{
    LOG_DBG(LOG_TAG, "\n");
    struct tt__ImagingOptions20* ImagingOptions = (struct tt__ImagingOptions20*)soap_zalloc(soap, sizeof(struct tt__ImagingOptions20));
    ImagingOptions->BacklightCompensation = (struct tt__BacklightCompensationOptions20*)soap_zalloc(soap, sizeof(struct tt__BacklightCompensationOptions20));
    ImagingOptions->Brightness = (struct tt__FloatRange*)soap_zalloc(soap, sizeof(struct tt__FloatRange));
    ImagingOptions->ColorSaturation = (struct tt__FloatRange*)soap_zalloc(soap, sizeof(struct tt__FloatRange));
    ImagingOptions->Contrast = (struct tt__FloatRange*)soap_zalloc(soap, sizeof(struct tt__FloatRange));
   

    ImagingOptions->Brightness->Min = 0;
    ImagingOptions->Brightness->Max = 255;
    ImagingOptions->ColorSaturation->Min = 0;
    ImagingOptions->ColorSaturation->Max = 255;
    ImagingOptions->Contrast->Min = 0;
    ImagingOptions->Contrast->Max = 255;

    timg__GetOptionsResponse->ImagingOptions = ImagingOptions;
    return SOAP_OK;
}
    /** Web service operation '__timg__Move' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __timg__Move(struct soap* soap, struct _timg__Move *timg__Move, struct _timg__MoveResponse *timg__MoveResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__timg__Stop' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __timg__Stop(struct soap* soap, struct _timg__Stop *timg__Stop, struct _timg__StopResponse *timg__StopResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__timg__GetStatus' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __timg__GetStatus(struct soap* soap, struct _timg__GetStatus *timg__GetStatus, struct _timg__GetStatusResponse *timg__GetStatusResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__timg__GetMoveOptions' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __timg__GetMoveOptions(struct soap* soap, struct _timg__GetMoveOptions *timg__GetMoveOptions, struct _timg__GetMoveOptionsResponse *timg__GetMoveOptionsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__timg__GetPresets' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __timg__GetPresets(struct soap* soap, struct _timg__GetPresets *timg__GetPresets, struct _timg__GetPresetsResponse *timg__GetPresetsResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__timg__GetCurrentPreset' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __timg__GetCurrentPreset(struct soap* soap, struct _timg__GetCurrentPreset *timg__GetCurrentPreset, struct _timg__GetCurrentPresetResponse *timg__GetCurrentPresetResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__timg__SetCurrentPreset' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __timg__SetCurrentPreset(struct soap* soap, struct _timg__SetCurrentPreset *timg__SetCurrentPreset, struct _timg__SetCurrentPresetResponse *timg__SetCurrentPresetResponse){
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

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___timg__GetServiceCapabilities(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___timg__GetImagingSettings(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___timg__SetImagingSettings(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___timg__GetOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___timg__Move(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___timg__Stop(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___timg__GetStatus(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___timg__GetMoveOptions(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___timg__GetPresets(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___timg__GetCurrentPreset(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }

// SOAP_FMAC5 int SOAP_FMAC6 soap_serve___timg__SetCurrentPreset(struct soap*){
//     LOG_DBG(LOG_TAG, "\n");
//     return SOAP_OK;
// }


