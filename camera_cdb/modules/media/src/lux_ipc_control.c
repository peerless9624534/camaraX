/*******************************************************************************
*文件名   : lux_ipc_control.c
*作用       : 人形 图片压缩等功能实现
*作者       : 唐兆伟 cxj
*******************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#include <imp_log.h>
#include <imp_common.h>
#include <imp_system.h>
#include <imp_framesource.h>
#include <imp_encoder.h>
#include <imp_log.h>
#include <comm_def.h>
#include <comm_error_code.h>
#include <comm_func.h>
#include <lux_isp.h>
#include <lux_fsrc.h>
#include <lux_video.h>
#include <lux_base.h>
#include "lux_audio.h"
#include <lux_iniparse.h>
#include <lux_hal_led.h>
#include <lux_hal_lightsns.h>
#include <lux_hal_misc.h>
#include <lux_daynight.h>
#include <lux_ivs.h>
#include <lux_osd.h>
#include <lux_pwm.h>

#include <ivs_common.h>
#include <ivs_inf_move.h>
#include <ivs_interface.h>

#include <lux_nv2jpeg.h>

#include <lux_event.h>
#include <idip_custom_arg.h>
#include <idip_server.h>
#include <demo_main.h>
#include <lux_ipc_control.h>
#include<lux_event_object.h>

#include "lux_event.h"
#include "lux_iniparse.h"

#include "imp_audio.h"
//#include "tuya_ipc_api.h"
//#include "tuya_ipc_media.h"
//#include "tuya_cloud_com_defs.h"
//#include "tuya_cloud_types.h"
#include "dirent.h"
#include "lux_config.h"
#include "lux_global.h"
#include "lux_person_det.h"


#define IPC_APP_AUDIO_WAIT_CONN_INTERVAL 60 /* 循环播放“等待连接”音频间隔，单位：秒 */
#define AUDIO_BEEP_SOUND_BUF_LEN 164 * 1024
#define AUDIO_PCM_BYTES_PER_PACK 640
#define LUX_CRADLESONG_PATH     "/system/sdcard2/Music/CradleSong"
#define LUX_WHITENOISE_PATH     "/system/sdcard2/Music/WhiteNoise"
#define LUX_POETRYSONG_PATH     "/system/sdcard2/Music/PoetrySong"
#define LUX_ENGLISHSONG_PATH    "/system/sdcard2/Music/EnglishSong"
#define LUX_CUSTOMEDSONG_PATH   "/tmp/Mycradlesong.pcm"
#define LUX_SONGLEN                      10
#define NURSERY_AUDIO_PCM_BYTES_PER_PACK 320
extern BOOL_X b_newselectsong;
extern BOOL_X g_audio_onoff_status;
extern DWORD_X songplay_pieces;
extern DWORD_X handletype_of_playmusic;
extern DWORD_X bPlayingCryMusic;

BOOL_X start_audio_status_reported = LUX_FALSE;

INT_X error_number_response = 1000;

typedef struct
{
    //IPC_APP_AUDIO_BEEP_TYPE type;
    char name[64];
    void *pData;
    unsigned int size;  //文件大小
    int  len;           //读到的数据大小
    LUX_AUDIO_PAYLOAD_TYPE encType;
} IPC_APP_NURSERY_MEMORY;

static IPC_APP_NURSERY_MEMORY g_nursery_choose;
IPC_APP_NURSERY_MEMORY *pNursery_Choose = &g_nursery_choose;
//pNursery_Choose = &g_nursery_choose;
typedef struct 
{
    IPC_APP_AUDIO_BEEP_TYPE type;
    char name[64];
    void *pData;
    unsigned int size;  //文件大小
    int  len;           //读到的数据大小
    LUX_AUDIO_PAYLOAD_TYPE encType;
} IPC_APP_AUDIO_BEEP_ATTR;

typedef struct 
{
    OSI_MUTEX mux;
    UINT_X    cnt;  /* 临时记录变量，用来统计高并发任务个数 */
    BOOL_X    bPlay; /* true：保持播放，false：停止播放 */
    IPC_APP_AUDIO_BEEP_TYPE type;
} IPC_APP_AUDIO_BEEP_CTRL;

static IPC_APP_AUDIO_BEEP_CTRL g_audioBeepCtrl;
static IPC_APP_AUDIO_BEEP_ATTR g_audioBeepAttr[IPC_APP_AUDIO_MAX];

INT_X IPC_APP_PutAudioPcm(IPC_APP_AUDIO_BEEP_TYPE type);

/* 播放音频提示音 */
int IPC_APP_AUDIO_PlayBeep(IPC_APP_AUDIO_BEEP_TYPE type)
{
    FILE *fp = NULL;
    CHAR_X tmp[4] = "\0";
    if(IPC_APP_AUDIO_BLESSING == type)
    {
        //不播放欢迎语
        fp = fopen(UNNORMAL_REBOOT_FILE_NAME,"r");
        if(NULL != fp)
        {
            fgets(tmp,sizeof(tmp),fp);
            printf("\r\n[%s]%d: tmp = %s\n", __func__, __LINE__, tmp);
            if(NULL != strstr(tmp,"1"))
            {
                fclose(fp);
                fp = NULL;
                fp = fopen(UNNORMAL_REBOOT_FILE_NAME,"w");
                if(NULL != fp)
                {
                    fprintf(fp,"2");
                }
                fclose(fp);

                printf("\r\n[%s]%d: reboot system success!\n", __func__, __LINE__);
                return 0;
            }
            else
            {
                fclose(fp);
                remove(UNNORMAL_REBOOT_FILE_NAME);
            }
        }

        // if(LUX_OK == access(UNNORMAL_REBOOT_FILE_NAME,F_OK))
        // {
        //     printf("[SKIP]unNormalReboot IPC_APP_AUDIO_BLESSING\n");
        //     return 0;
        // }
    }
    if(IPC_APP_AUDIO_CONN_OK == type)
    {
        //不播放连接成功并且删除标识文件
        if(LUX_OK == access(UNNORMAL_REBOOT_FILE_NAME,F_OK))
        {
            printf("[SKIP]unNormalReboot IPC_APP_AUDIO_CONN_OK\n");
            remove(UNNORMAL_REBOOT_FILE_NAME);
            return 0;
        }
    }

    Thread_Mutex_Lock(&g_audioBeepCtrl.mux);

    g_audioBeepCtrl.type = type;
    g_audioBeepCtrl.bPlay = LUX_TRUE;
    g_audioBeepCtrl.cnt++;

    Thread_Mutex_UnLock(&g_audioBeepCtrl.mux);

    return 0;
}

/* 停止音频提示音 */
int IPC_APP_AUDIO_StopBeep()
{
    //Thread_Mutex_Lock(&g_audioBeepCtrl.mux);

    g_audioBeepCtrl.bPlay = LUX_FALSE;

    //Thread_Mutex_UnLock(&g_audioBeepCtrl.mux);

    return 0;
}

/* 音频提示音处理线程 */ 
void IPC_APP_AUDIO_BeepProcess()
{
    while (1)
    {
        Thread_Mutex_Lock(&g_audioBeepCtrl.mux);

        if (g_audioBeepCtrl.cnt)
        {
            if ((g_audio_onoff_status == 0) && (handletype_of_playmusic != LUX_SONG_PLAY))
            {
                IPC_APP_PutAudioPcm(g_audioBeepCtrl.type);                
            }
            g_audioBeepCtrl.cnt = 0;
        }

        Thread_Mutex_UnLock(&g_audioBeepCtrl.mux);

        usleep(100*1000);
    }

    return ;
}

unsigned long get_file_size(const char *filename)  
{  
    struct stat buf;  
    if(stat(filename, &buf)<0)  
    {  
        return 0;  
    }

    return (unsigned long)buf.st_size;
}

/* 音频提示音功能初始化 */
void IPC_APP_AUDIO_BeepInit()
{
    int i = 0;
    FILE *fp = NULL;
    IPC_APP_AUDIO_BEEP_ATTR *pHandle = NULL;
    LUX_AUDIO_PARAM_ST param;

    memset(&param, 0, sizeof(param));
    LUX_AUDIO_GetParam(&param);

    for (i = 0; i < IPC_APP_AUDIO_MAX; i++)
    {
        pHandle = &g_audioBeepAttr[i];
        switch (i)
        {
            case IPC_APP_AUDIO_WAIT_CONN:
                strcpy(pHandle->name, "/system/bin/audio/wait_conn.pcm");
                fp = fopen(pHandle->name, "r");
                pHandle->type = IPC_APP_AUDIO_WAIT_CONN;
                pHandle->size = get_file_size(pHandle->name);
                break;
            case IPC_APP_AUDIO_SCAN_OK:
                strcpy(pHandle->name, "/system/bin/audio/scan_ok.pcm");
                fp = fopen(pHandle->name, "r");
                pHandle->type = IPC_APP_AUDIO_SCAN_OK;
                pHandle->size = get_file_size(pHandle->name);
                break;
            case IPC_APP_AUDIO_CONN_OK:
                strcpy(pHandle->name, "/system/bin/audio/connect_ok.pcm");
                fp = fopen(pHandle->name, "r");
                pHandle->type = IPC_APP_AUDIO_CONN_OK;
                pHandle->size = get_file_size(pHandle->name);
                break;
            case IPC_APP_AUDIO_MUSIC:
                if(0 == access("/system/bin/audio/Mycradlesong.pcm",F_OK))
                {
                    strcpy(pHandle->name, "/system/bin/audio/Mycradlesong.pcm");
                }
                else
                {
                    strcpy(pHandle->name, "/system/bin/audio/cradlesong.pcm");
                }
                fp = fopen(pHandle->name, "r");
                pHandle->type = IPC_APP_AUDIO_MUSIC;
                pHandle->size = get_file_size(pHandle->name);
                break;
            case IPC_APP_AUDIO_BLESSING:
                strcpy(pHandle->name, "/system/bin/audio/blessing.pcm");
                fp = fopen(pHandle->name, "r");
                pHandle->type = IPC_APP_AUDIO_BLESSING;
                pHandle->size = get_file_size(pHandle->name);
                break;
            case IPC_APP_AUDIO_DING:
                strcpy(pHandle->name, "/system/bin/audio/ding.pcm");
                fp = fopen(pHandle->name, "r");
                pHandle->type = IPC_APP_AUDIO_DING;
                pHandle->size = get_file_size(pHandle->name);
                break;
            default:
                printf("%s:%d switch-case error!\n", __func__, __LINE__);
                break;
        }

        if (NULL != fp)
        {
            /* 申请保存音频数据的内存 */
            pHandle->pData = malloc(pHandle->size);
            pHandle->len = fread(pHandle->pData, 1, pHandle->size, fp);
            pHandle->encType = param.type;
            fclose(fp);
        }
    }

    Thread_Mutex_Create(&g_audioBeepCtrl.mux);
    Task_CreateThread(IPC_APP_AUDIO_BeepProcess, NULL);
    return;
}


/* IVS 报警提示音+上传报警信息+图片 */
void IPC_APP_IVS_AlarmNotify(IN CONST CHAR_T *snap_buffer, IN CONST UINT_T snap_size, IN CONST LUX_NOTIFICATION_NAME_E type)
{
    //tuya_ipc_notify_motion_detect(snap_buffer, snap_size, (NOTIFICATION_CONTENT_TYPE_E)type);
    switch (type)
    {
        case LUX_NOTIFICATION_NAME_MOTION:
            // IPC_APP_report_move_alarm("move alarmd");
            printf("\n***************************move alarmd***************************\n");
            break;
    #ifdef CONFIG_BABY
        case LUX_NOTIFICATION_NAME_DEV_LINK:
            // IPC_APP_report_smile_snap("smile snap");
            printf("\n***************************smile snap***************************\n");
            break;
        case LUX_NOTIFICATION_NAME_FACE:
            // IPC_APP_report_cover_face_alarm("cover alarmd");
            printf("\n***************************cover alarmd***************************\n");
            break;
        case LUX_NOTIFICATION_NAME_LINGER:
            // IPC_APP_report_perm_alarm("perm alarmd");
            printf("\n***************************perm alarmd***************************\n");
            break;
        #ifdef TUYA_DP_ALBUM_ONOFF
        case LUX_NOTIFICATION_NAME_HUMAN:
            // IPC_APP_report_album_alarm("album alarmd");
            printf("\n***************************album alarmd***************************\n");
            break;
        #endif
        #ifdef TUYA_DP_SLEEP_DET_ONOFF
        case LUX_NOTIFICATION_NAME_CALL_ACCEPT:
            // IPC_APP_report_sleep_det_alarm("sleep start");
            printf("\n***************************sleep start***************************\n");
            break;
        case LUX_NOTIFICATION_NAME_CALL_NOT_ACCEPT:
            // IPC_APP_report_sleep_det_alarm("sleep End");
            printf("\n***************************sleep End***************************\n");
            break;
        #endif

    #endif
        default:
            printf("%s:%d invalid alarm\n", __func__, __LINE__);
            break;
    }
    // tuya_ipc_notify_with_event(snap_buffer, snap_size, NOTIFICATION_CONTENT_JPEG, (NOTIFICATION_NAME_E)type);

    return;
}

/* 音频提示音功能退出 */
void IPC_APP_AUDIO_BeepExit()
{
    INT_T i = 0;

    for (i = 0; i < IPC_APP_AUDIO_MAX; i++)
    {
        if (g_audioBeepAttr[i].pData)
        {
            free(g_audioBeepAttr[i].pData);
            g_audioBeepAttr[i].len = 0;
        }
    }

    return ;
}

#if 0
/* 
Callback when the user clicks on the APP to remove the device
*/
void IPC_APP_Reset_System_CB(int type)
{
    printf("reset ipc success. please restart the ipc %d\n", type);
    IPC_APP_Notify_LED_Sound_Status_CB(IPC_RESET_SUCCESS);
    
    /* 删除设备要将设备恢复出厂设置 */
    //system("/system/init/backup/factory_reset.sh");
    LUX_BASE_System_CMD("/system/init/backup/factory_reset.sh");
    sleep(1);

    /* Developers need to restart IPC operations */
    LUX_Event_Reboot();

    return ;
}
#endif

void IPC_APP_Restart_Process_CB(void)
{
    printf("sdk internal restart request. please restart the ipc\n");
    
    LUX_Event_Reboot();

    return ;
    /* Developers need to implement restart operations. Restart the process or restart the device. */
}

int IPC_APP_PutAudioPcm(IPC_APP_AUDIO_BEEP_TYPE type)
{
    bPlayingCryMusic = 0;
    if (type >= IPC_APP_AUDIO_MAX || type < 0)
    {
        printf("%s:%d invalid type:%d\n", __func__, __LINE__, type);
        return -1;
    }
    if( LUX_ERR == LUX_AO_ClearBuffer())
    {
        printf("%s:%d LUX_AO_ClearBuffer Error\n", __func__, __LINE__);
        return -1;
    }
    if(IPC_APP_AUDIO_MUSIC == type)
    {
        bPlayingCryMusic = 1;
    }
    LUX_AUDIO_FRAME_ST aoStream;
    UINT_X loopTimes = 0, len = 0;
    LUX_AUDIO_PAYLOAD_TYPE encType;
    void *pBuf = NULL;
    INT_X i = 0;
    INT_X j = 0;
    if (0 == g_audioBeepAttr[type].len || NULL == g_audioBeepAttr[type].pData)
    {
        printf("%s:%d invalid param type:%d len:%d p:%p\n", __func__, __LINE__, 
            type, g_audioBeepAttr[type].len, g_audioBeepAttr[type].pData);
        return -1;
    }
    pBuf = g_audioBeepAttr[type].pData;
    len = g_audioBeepAttr[type].len;
    encType = g_audioBeepAttr[type].encType;
    loopTimes = len / AUDIO_PCM_BYTES_PER_PACK;

    /* 分包播放音频数据 */
    memset(&aoStream, 0, sizeof(aoStream));
    for (i = 0; i < loopTimes; i++)
    {
        if ((g_audio_onoff_status == TRUE) || (handletype_of_playmusic == LUX_SONG_PLAY))
        {
            return 0;
        }
        aoStream.pData = (pBuf + AUDIO_PCM_BYTES_PER_PACK * i);
        aoStream.len = AUDIO_PCM_BYTES_PER_PACK;
        LUX_AO_PutStream(&aoStream);
    }

    /* 播放剩余尾部 */
    if (len - (loopTimes * AUDIO_PCM_BYTES_PER_PACK))
    {
        if ((g_audio_onoff_status == TRUE) || (handletype_of_playmusic == LUX_SONG_PLAY))
        {
            bPlayingCryMusic = 0;
            return 0;
        }
        aoStream.pData = pBuf + (AUDIO_PCM_BYTES_PER_PACK * loopTimes);
        aoStream.len = len - (loopTimes * AUDIO_PCM_BYTES_PER_PACK);
        aoStream.type = encType; /* 下行音频编码格式 */
        LUX_AO_PutStream(&aoStream);
    }
    if(IPC_APP_AUDIO_DING == type)
    {
        /* 小于缓存大小的音频 用0补足*/
        IMPAudioOChnState status;
        IMP_AO_QueryChnStat(0,0,&status);
        printf("AO buffer status status.chnTotalNum = %d ,status.chnBusyNum = %d ,status.chnFreeNum =%d \n",status.chnTotalNum,status.chnBusyNum,status.chnFreeNum);
        char tmp[AUDIO_PCM_BYTES_PER_PACK];
        memset(tmp,0,AUDIO_PCM_BYTES_PER_PACK);
        for(j = 0 ; j < status.chnTotalNum / status.chnBusyNum; j++)
        {
            for (i = 0; i < loopTimes; i++)
            {
                aoStream.pData = tmp;
                aoStream.len = AUDIO_PCM_BYTES_PER_PACK;
                LUX_AO_PutStream(&aoStream);
            }
        }
    }

    //如果需要播放音乐，则再播放一次
    if( IPC_APP_AUDIO_MUSIC == type )
    {   
        for(j = 0 ; j < LUX_BABY_CRY_MUSIC_EXTRO_TIMES ; j++)
        {
            memset(&aoStream, 0, sizeof(aoStream));
            for (i = 0; i < loopTimes; i++)
            {
                if ((g_audio_onoff_status == TRUE) || (handletype_of_playmusic == LUX_SONG_PLAY))
                {
                    return 0;
                }
                aoStream.pData = (pBuf + AUDIO_PCM_BYTES_PER_PACK * i);
                aoStream.len = AUDIO_PCM_BYTES_PER_PACK;
                LUX_AO_PutStream(&aoStream);
            }

            /* 播放剩余尾部 */
            if (len - (loopTimes * AUDIO_PCM_BYTES_PER_PACK))
            {
                if ((g_audio_onoff_status == TRUE) || (handletype_of_playmusic == LUX_SONG_PLAY))
                {
                    return 0;
                }
                aoStream.pData = pBuf + (AUDIO_PCM_BYTES_PER_PACK * loopTimes);
                aoStream.len = len - (loopTimes * AUDIO_PCM_BYTES_PER_PACK);
                aoStream.type = encType; /* 下行音频编码格式 */
                LUX_AO_PutStream(&aoStream);
            }
        }
        bPlayingCryMusic = 0;
    }
    return 0;
}

static int g_waitConnOngoing = 1;

void IPC_APP_AUDIO_WaitConn_Stop()
{
    g_waitConnOngoing = 0;

    return ;
}

/* 循环播放“等待连接”提示音处理线程 */
void IPC_APP_AUDIO_WaitConnProcess()
{
    UINT_X timeBegin = (0 - IPC_APP_AUDIO_WAIT_CONN_INTERVAL);
    UINT_X timeNow = 0;

    while (g_waitConnOngoing)
    {
        timeNow = getTime_getS();
        if (timeNow - timeBegin >= IPC_APP_AUDIO_WAIT_CONN_INTERVAL)
        {
            timeBegin = timeNow;
            IPC_APP_AUDIO_PlayBeep(IPC_APP_AUDIO_WAIT_CONN);
        }
        usleep(100*1000);
    }
    printf("%s:%d thread exit! g_waitConnOngoing:%d\n", __func__, __LINE__, g_waitConnOngoing);

    return ;
}

/* Developers need to implement the corresponding prompt sound playback and LED prompts,
   you can refer to the SDK attached files, using TUYA audio files. */
void IPC_APP_Notify_LED_Sound_Status_CB(IPC_APP_NOTIFY_EVENT_E notify_event)
{
    // BOOL_X led_enabled = 0;
    
    // int err = LUX_INIParse_GetBool(LUX_CONFIG_FILE, "hal", "led", &led_enabled);
    // if(err)
    //     IMP_LOG_WARN("IPC", "get led config failed");
    // printf("curr event:%d \r\n", notify_event);
    switch (notify_event)
    {
        case IPC_BOOTUP_FINISH: /* Startup success */
        {
            IPC_APP_AUDIO_PlayBeep(IPC_APP_AUDIO_BLESSING);
            break;
        }
        case IPC_START_WIFI_CFG: /* Start configuring the network */
        {
            Task_CreateThread(IPC_APP_AUDIO_WaitConnProcess, NULL);
            if(g_configData->system_config.led_enabled)
            {
                LUX_HAL_LedSetStatus(LUX_HAL_LED_GREEN, LUX_HAL_LED_BLINK);
                LUX_HAL_LedSetStatus(LUX_HAL_LED_RED, LUX_HAL_LED_OFF);
            }
            break;
        }
        case IPC_REV_WIFI_CFG: /* Receive network configuration information */
        {
            break;
        }
        case IPC_CONNECTING_WIFI: /* Start Connecting WIFI */
        {
            IPC_APP_AUDIO_PlayBeep(IPC_APP_AUDIO_SCAN_OK);
            if(g_configData->system_config.led_enabled)
                LUX_HAL_LedSetStatus(LUX_HAL_LED_GREEN, LUX_HAL_LED_BLINK);
            break;
        }
        case IPC_MQTT_ONLINE: /* MQTT on-line */
        {
            IPC_APP_AUDIO_PlayBeep(IPC_APP_AUDIO_CONN_OK);
            if(g_configData->system_config.led_enabled)
            {
                LUX_HAL_LedSetStatus(LUX_HAL_LED_GREEN, LUX_HAL_LED_ON);
                LUX_HAL_LedSetStatus(LUX_HAL_LED_RED, LUX_HAL_LED_OFF);
            }
            break;
        }
        case IPC_RESET_SUCCESS: /* Reset completed */
        {
            LUX_INIParse_SetKey(LUX_CONFIG_FILE, "algorithm_config", "connect_status", "0");
            usleep(200*1000);
            break;
        }
        default:
        {
            break;
        }
    }
}

/* Callback of talkback mode,turn on or off speaker hardware*/
void JIO_APP_Enable_Speaker_CB(BOOL_X enabled)
{
    printf("enable speaker %d \r\n", enabled);
    //TODO
    /* Developers need to turn on or off speaker hardware operations. 
    If IPC hardware features do not need to be explicitly turned on, the function can be left blank. */
}

#if 0
/* Callback of talkback mode,turn on or off the sound */
void JIO_APP_Rev_Audio_CB(IN CONST MEDIA_FRAME_S *p_audio_frame,
                           TUYA_AUDIO_SAMPLE_E audio_sample,
                           TUYA_AUDIO_DATABITS_E audio_databits,
                           TUYA_AUDIO_CHANNEL_E audio_channel)
{
    LUX_AUDIO_FRAME_ST frame;

    frame.pData = p_audio_frame->p_buf;
    frame.len = p_audio_frame->size;

    LUX_AO_PutStream(&frame);

    /* Developers need to implement the operations of voice playback*/
    return ;
}
#endif

/**
 定义事件对象
 说明：   demo_main.c中轮训事件发生
                后续用队列方式实现轮训事件处理
**/
 struct_event_info event_info;		

/**
 * 描述	:   处理人形事件
 * 输入参数  : 无
 * 返回参数  : 无
 * 说明：
**/
static void tag_ipc_ivs_alarm_hander(void)
{
    #ifdef TAG_LOG_TEST
        printf("\n\n######################## tag_ivs_person_alarm_hander ########################\n\n");
    #endif
    event_info.pset_value(&event_info,VCA_CROWD);		//设置值 
}

/**
 * 描述 :   初始化
 * 输入参数  : 无
 * 返回参数  : 无
 * 说明：
**/
void tag_ipc_app_init(void)
{
    char i=0;
     char ret = -1;
     LUX_EVENT_ATTR_ST IVSFuncCB;

      /*IVS算法注册,需要连网*/
    ret = LUX_IVS_Iaac_Init();
    if(ret)
    {
         printf("\n\n######################## LUX_IVS_Iaac_Init   failed ########################\n\n");
    }

    /*ivs算法初始化*/
    for (i = 0; i < LUX_IVS_ALGO_NUM; i++)
    {
        Semaphore_Post(&g_ivsAlgSem);
    }

     /*注册回调处理人形*/
    IVSFuncCB.pEvntReportFuncCB = (pAlarmEventReportCB *)tag_ipc_ivs_alarm_hander;
     ret = LUX_EVENT_IvsRegistCB(&IVSFuncCB);
     if(ret)
    {
        printf("\n\n########################LUX_EVENT_IvsRegistCB  failed########################\n\n");
    }

    /*默认开启侦测*/
   LUX_IVS_PD_StartAlgo();    //后续嵌入到app通知

    /*构造对象，用于对事件的处理*/
 	constructor(&event_info);			
}
