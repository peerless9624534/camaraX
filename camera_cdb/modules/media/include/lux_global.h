#ifndef __LUX_GLOBAL_H__
#define __LUX_GLOBAL_H__

#include <jwhead.h>
#include <stdint.h>
#include <sys/time.h>

#include <comm_def.h>

#include <comm_func.h>

#include "lux_hal_led.h"
#include "lux_base.h"

#define JIO_SOUND_DET_SENSI_LOW  (0.90f)
#define JIO_SOUND_DET_SENSI_HIGH (0.80f)



struct idip_server;
/// IDIP server instance type
typedef struct idip_server idip_server_t;
typedef struct lux_record lux_record_t;

typedef struct LUX_VIDEO_ENC_ATTR LUX_VIDEO_ENC_ATTR_ST;
// typedef struct LUX_FSRC_ATTR LUX_FSRC_ATTR_ST;

typedef struct LUX_CONFIG_DATA LUX_CONFIG_DATA_ST;

//typedef struct live_streamer_args live_streamer_args_t;

typedef struct idip_av_source idip_av_source_t;

typedef struct live_streamer_args {
    idip_av_source_t* av_source;
    LUX_STREAM_CHANNEL_EN channel;
} live_streamer_args_t;

//临时使用 后面会删掉
//typedef struct config_model_t;

extern char g_serial_str[128];
extern char g_sensor_name[128];
extern char g_device_model[128];
extern char g_device_type[128];
extern char g_deivce_model_detail[128];

extern DWORD_X bPlayingCryMusic;
extern BOOL_X g_audio_onoff_status;
extern DWORD_X handletype_of_playmusic;
extern float g_cryDetectSensitive;
extern idip_server_t* m_idip_server;
extern BOOL_X  g_bcryDetctAlarmSong;
extern bool  push_live_stream_enable_sub;
extern bool  push_live_stream_enable_main;
extern bool  Semaphore_Pend_flag_main;
extern bool  Semaphore_Pend_flag_sub;
extern bool  LuxTestEnbale;
extern lux_record_t* g_lux_record;
extern char* led_color;
extern int LED_FlashTime;
extern int fd_79;
extern int fd_116;
extern int fd_122;
extern int fd_123;
//extern config_model_t cfg;
extern int g_sensor_h_flip;
extern int g_sensor_v_flip;

extern OSI_SEM g_liveAudioSem;
extern OSI_SEM g_liveVideoMainSem;
extern OSI_SEM g_liveVideoSubSem;


extern int LUX_Global_Init();

extern uint64_t time_ms(void);

char* trim_left(char* buf);
char* trim_right(char* buf);
char* trim_pure(char* buf);
char* trim_all(char* buf);

//void load_jio_config();
//extern LUX_CONFIG_DATA_ST g_configData_st;
//void init_jio_config(void);
extern  live_streamer_args_t *args_sub;
//extern  LUX_CONFIG_JIO_CFG_ST* g_JioConfig;
//extern LUX_CONFIG_DATA_ST g_configData_st;

void set_time_zone(const char* timezone_info);

int LUX_Network_setip(const char* ip, const char *mask);
int LUX_Network_setipEx(const char* ip, const char* mask, const char *gateway, const char *dns);
int LUX_Network_setDHCP(int enable);
int LUX_Network_getip(char* mac, char* ip, char* mask);
int LUX_Network_getip2(const char* eth, char* gw);
int LUX_Network_setGateway(const char* gw);
int LUX_Network_getGateway(char* ip);
int LUX_Network_SetDNS(const char* dns);
int LUX_Network_GetDNS(char* dns);
int LUX_Network_GetMAC(char* interface, unsigned char* pMac);
int LUX_Network_GetMACStr(char* interface, unsigned char* pMac);

const char* format_time(int64_t t);
LUX_VIDEO_ENC_ATTR_ST* g_VEncAttr;
extern LUX_CONFIG_DATA_ST *g_configData;

live_streamer_args_t *args_main;
live_streamer_args_t *args_sub;
//config_model_t* g_JioConfig;


//struct _timg__GetImagingSettingsResponse;	
#ifndef SOAP_TYPE__timg__GetImagingSettingsResponse
#define SOAP_TYPE__timg__GetImagingSettingsResponse (1908)
/* complex XML schema type 'timg:GetImagingSettingsResponse': */
struct _timg__GetImagingSettingsResponse {
        /** Required element 'timg:ImagingSettings' of XML schema type 'tt:ImagingSettings20' */
        struct tt__ImagingSettings20 *ImagingSettings;
};
#endif

#ifndef SOAP_TYPE__timg__SetImagingSettings
#define SOAP_TYPE__timg__SetImagingSettings (1909)
/* complex XML schema type 'timg:SetImagingSettings': */
struct _timg__SetImagingSettings {
    /** Required element 'timg:VideoSourceToken' of XML schema type 'tt:ReferenceToken' */
    char* VideoSourceToken;
    /** Required element 'timg:ImagingSettings' of XML schema type 'tt:ImagingSettings20' */
    struct tt__ImagingSettings20* ImagingSettings;
    /** Optional element 'timg:ForcePersistence' of XML schema type 'xsd:boolean' */
    enum xsd__boolean* ForcePersistence;
};
#endif



// timezone offsets
// https://en.wikipedia.org/wiki/List_of_tz_database_time_zones


#endif
