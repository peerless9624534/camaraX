#ifndef LUX_IPC_CONTROL_H
#define LUX_IPC_CONTROL_H

#include<lux_event_object.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    IPC_BOOTUP_FINISH,
    IPC_START_WIFI_CFG,
    IPC_REV_WIFI_CFG,
    IPC_CONNECTING_WIFI,
    IPC_MQTT_ONLINE,
    IPC_RESET_SUCCESS,
}IPC_APP_NOTIFY_EVENT_E;

/* 音频播放类型 */
typedef enum
{
    IPC_APP_AUDIO_WAIT_CONN,
    IPC_APP_AUDIO_SCAN_OK,
    IPC_APP_AUDIO_CONN_OK,
    IPC_APP_AUDIO_MUSIC,
    IPC_APP_AUDIO_BLESSING,
    IPC_APP_AUDIO_DING,
    IPC_APP_AUDIO_MAX,
} IPC_APP_AUDIO_BEEP_TYPE;

/* 音频文件操作 */
typedef enum
{
    LUX_SONG_STOP,
    LUX_SONG_PAUSE,
    LUX_SONG_PLAY,
    LUX_SONG_LIST_PLAY,
    LUX_SONG_SINGLE_CYCLE,
    LUX_SONG_SELECT_FAILED = 98,
    LUX_SONG_OPENSD_FAILED,
} LUX_SONG_HANDLE;

//#define TAG_LOG_TEST    //log日志信息

/*定义事件对象*/
 extern struct_event_info event_info;		

/*函数申明*/
void tag_ipc_app_init(void);

void IPC_APP_AUDIO_BeepInit();

int IPC_APP_AUDIO_PlayBeep(IPC_APP_AUDIO_BEEP_TYPE type);

void IPC_APP_Notify_LED_Sound_Status_CB(IPC_APP_NOTIFY_EVENT_E notify_event);

#ifdef __cplusplus
}
#endif
#endif