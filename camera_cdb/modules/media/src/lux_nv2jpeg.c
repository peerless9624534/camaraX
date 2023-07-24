#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "imp_system.h"
#include "imp_common.h"
#include "imp_encoder.h"
#include "imp_log.h"

#include "comm_def.h"
#include "comm_error_code.h"
#include "comm_func.h"

#include "icommon.h"
#include "lib_jpeg_codec.h"
#include "lux_nv2jpeg.h"
#include "lux_iniparse.h"
#include "lux_ivs.h"
#include <lux_global.h>

#include "lux_event.h"
//#include "tuya_ipc_system_control_demo.h"

#define TAG "lux_nv2jpeg"

extern void *video_vbm_malloc(int size, int align);
extern void video_vbm_free(void *vaddr);
extern intptr_t video_vbm_virt_to_phys(intptr_t vaddr);
extern intptr_t video_vbm_phys_to_virt(intptr_t paddr);

typedef struct
{
	void *h;
	streamInfo stream;
    INT_X width;
    INT_X height;
    INT_X initQp;
    uint8_t *src_buf;
    IMPFrameInfo frame;
    INT_X frmSize;

    OSI_MUTEX   mutex;
}LUX_NV12_TO_JPEG_ST;


typedef struct
{
    LUX_NV12_TO_JPEG_ST g_jpegAttr[2];
}LUX_NV12_GROUP;

LUX_NV12_GROUP gGroup =
{
    .g_jpegAttr[0] = 
    {
        .src_buf = NULL,
        .initQp = 45,
    },
    .g_jpegAttr[1] = 
    {
        .src_buf = NULL,
        .initQp = 45,
        .width = SENSOR_WIDTH_SECOND,
        .height = SENSOR_HEIGHT_SECOND
    }
    
};

// INT_X LUX_Jpeg_Init(INT_X Chn_Id)
// {	
//     CHAR_X sensorName[64] = {0};
// 	LUX_COMM_FUNC_SNS_WH sensorWH;
    
//     LUX_NV12_TO_JPEG_ST *jpegSt = &gGroup.g_jpegAttr[Chn_Id];
//     int ret = LUX_OK;
//     if(LUX_STREAM_MAIN == Chn_Id)
//     {
//     	/*读取配置文件的sensor名字*/
//         ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "sensor", "name", sensorName);
//         if(LUX_OK != ret){
//             IMP_LOG_ERR(TAG, "LUX_INIParse_GetString failed,error [0x%x]\n",ret);
//             return LUX_ERR;
//         }

//         /*获取镜头分辨率大小*/
//         ret = LUX_COMM_GetSensorWH(sensorName, &sensorWH);
//         if(LUX_OK != ret){
//             IMP_LOG_ERR(TAG, "LUX_COMM_GetSensorWH failed,error [0x%x]\n",ret);
//             return LUX_ERR;
//         }
//         jpegSt->width = sensorWH.width;
//         jpegSt->height = sensorWH.height;
//     }
//     ret = alcodec_jpege_init(&jpegSt->h, jpegSt->width, jpegSt->height, jpegSt->initQp);
//     if (ret < 0) {
//         IMP_LOG_ERR(TAG, "alcodec_jpege_init failed,error [0x%x]\n",ret);
//         return LUX_ERR;
//     }
//     Thread_Mutex_Create(&jpegSt->mutex);

//     return LUX_OK;
// }

// INT_X LUX_Jpeg_Deinit(INT_X Chn_Id)
// {
//     LUX_NV12_TO_JPEG_ST *jpegSt = &gGroup.g_jpegAttr[Chn_Id];

//     if(NULL != jpegSt->stream.streamAddr)
//     {
//         free(jpegSt->stream.streamAddr);
//         jpegSt->stream.streamAddr = NULL;
//     }
//     if(NULL != jpegSt->src_buf)
//     {
//         video_vbm_free(jpegSt->src_buf);
//         jpegSt->src_buf = NULL;
//     }
    
//     alcodec_jpege_deinit(jpegSt->h);
//     Thread_Mutex_Destroy(&jpegSt->mutex);
//     return LUX_OK;
// }


INT_X LUX_Jpeg_Init(void)
{	
    CHAR_X sensorName[64] = {0};
	LUX_COMM_FUNC_SNS_WH sensorWH;
    LUX_NV12_TO_JPEG_ST *jpegSt;
    int ret = LUX_OK;
    int Chn_Id;
    for (Chn_Id = 0; Chn_Id < LUX_STREAM_THRD; Chn_Id++)
    {     
        jpegSt = &gGroup.g_jpegAttr[Chn_Id]; 
        if(LUX_STREAM_MAIN == Chn_Id)
        {
        	/*读取配置文件的sensor名字*/
            strcpy(sensorName, g_sensor_name);
            //ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "sensor", "name", sensorName);
            //if(LUX_OK != ret){
            //    IMP_LOG_ERR(TAG, "LUX_INIParse_GetString failed,error [0x%x]\n",ret);
            //    return LUX_ERR;
            //}

            /*获取镜头分辨率大小*/
            ret = LUX_COMM_GetSensorWH(sensorName, &sensorWH);
            if(LUX_OK != ret){
                IMP_LOG_ERR(TAG, "LUX_COMM_GetSensorWH failed,error [0x%x]\n",ret);
                return LUX_ERR;
            }
            jpegSt->width = sensorWH.width;
            jpegSt->height = sensorWH.height;
        }
        ret = /*alcodec_jpege_init(&jpegSt->h, jpegSt->width, jpegSt->height, jpegSt->initQp)*/0;
        if (ret < 0) {
            IMP_LOG_ERR(TAG, "alcodec_jpege_init failed,error [0x%x]\n",ret);
            return LUX_ERR;
        }
        Thread_Mutex_Create(&jpegSt->mutex);
    }
    return LUX_OK;
}

INT_X LUX_Jpeg_Deinit(void)
{
    int Chn_Id;
    LUX_NV12_TO_JPEG_ST *jpegSt;
    for (Chn_Id = 0; Chn_Id < LUX_STREAM_THRD; Chn_Id++)
    {     
        jpegSt = &gGroup.g_jpegAttr[Chn_Id];

        if(NULL != jpegSt->stream.streamAddr)
        {
            free(jpegSt->stream.streamAddr);
            jpegSt->stream.streamAddr = NULL;
        }
        if(NULL != jpegSt->src_buf)
        {
            video_vbm_free(jpegSt->src_buf);
            jpegSt->src_buf = NULL;
        }

        //alcodec_jpege_deinit(jpegSt->h);
        Thread_Mutex_Destroy(&jpegSt->mutex);
    }
    return LUX_OK;
}

INT_X LUX_Jpeg_Encode(INT_X Chn_Id,uint8_t *pollingBuffer,UINT_X Buffersize,LUX_ENCODE_STREAM_ST *picInfo)
{
    int ret = LUX_OK;

    LUX_NV12_TO_JPEG_ST *jpegSt = &gGroup.g_jpegAttr[Chn_Id];

	if(NULL == pollingBuffer || NULL == picInfo)
	{
		IMP_LOG_ERR(TAG, "pollingBuffer or picInfo point  error.\n");
		return LUX_ERR;
	}

    Thread_Mutex_Lock(&jpegSt->mutex);
    //===========================================================

    jpegSt->frmSize = jpegSt->width * jpegSt->height * 3 / 2;
	if((jpegSt->src_buf = (uint8_t*)video_vbm_malloc(jpegSt->frmSize, C_VB_ALIGN)) == NULL){
		printf("video_vbm_malloc src_buf failed\n");
		return LUX_ERR;
	}
    jpegSt->frame.width = jpegSt->width;
    jpegSt->frame.height = jpegSt->height;
    jpegSt->frame.size = jpegSt->frmSize;
	
    jpegSt->frame.phyAddr = (uint32_t)video_vbm_virt_to_phys((intptr_t)jpegSt->src_buf);
    jpegSt->frame.virAddr = (uint32_t)jpegSt->src_buf;
    
	jpegSt->stream.streamAddr = (uint8_t *)malloc(jpegSt->frmSize);
	if(jpegSt->stream.streamAddr == NULL) {
		printf("steamAddr malloc failed\n");
		return LUX_ERR;
	}

    memcpy(jpegSt->src_buf,pollingBuffer,jpegSt->frmSize);
    ret = /*alcodec_jpege_encode(jpegSt->h, jpegSt->frame, &jpegSt->stream)*/0;
    if (ret < 0) {
        printf("alcodec_jpege_encode failed\n");
        return LUX_ERR;
    }
    picInfo->pAddr = jpegSt->stream.streamAddr;
    picInfo->streamLen = jpegSt->stream.streamLen;
    printf("CH%d Picture Buffer size:[%d] \n",Chn_Id, picInfo->streamLen);

    return LUX_OK;
}

INT_X LUX_Jpeg_FreeStream(INT_X Chn_Id,LUX_ENCODE_STREAM_ST *pPicInfo)
{
    LUX_NV12_TO_JPEG_ST *jpegSt = &gGroup.g_jpegAttr[Chn_Id];
    if(NULL != jpegSt->stream.streamAddr)
    {
        free(jpegSt->stream.streamAddr);
        jpegSt->stream.streamAddr = NULL;
    }
    if(NULL != jpegSt->src_buf)
    {
        video_vbm_free(jpegSt->src_buf);
        jpegSt->src_buf = NULL;
    }
    printf("LUX_Jpeg_FreeStream Done\n");
    Thread_Mutex_UnLock(&jpegSt->mutex);

    return LUX_OK;
}


/*用于影集抓拍*/
#define LUX_ABLUM_TOTAL_MS 60 * 60 * 1000
#define LUX_ABLUM_INTERVAL_MS 1 * 60 * 1000
typedef struct
{
    BOOL_X bInited;
    BOOL_X bStarted;
    INT_X picCounts;
    UINT_X endTime;
    UINT_X curTime;
    UINT_X lastTime; 
    LUX_EVENT_ATTR_ST alarmCbk;
    OSI_MUTEX   mutex;

}LUX_Jpeg_Album_ST;

LUX_Jpeg_Album_ST g_Album = 
{

    .bInited = LUX_FALSE,
    .bStarted = LUX_FALSE
};

void LUX_Jpeg_Album_Process()
{
    INT_X ret = LUX_ERR;
    LUX_ENCODE_STREAM_ST picInfo;


    while (true)
    {
        //printf("%s:%d Album stat bStarted %d  bInited %d!\n", __func__, __LINE__,g_Album.bStarted,g_Album.bInited);
        if(!g_Album.bStarted || !g_Album.bInited)
        {
            usleep(200*1000);
            continue;
        }
        Thread_Mutex_Lock(&g_Album.mutex);
        g_Album.curTime = getTime_getMs();

        //printf("%s:%d Album stat curTime [%d] lastTime [%d] endTime [%d]\n", __func__, __LINE__,g_Album.curTime,g_Album.lastTime,g_Album.endTime);

        if(g_Album.curTime - g_Album.lastTime > LUX_ABLUM_INTERVAL_MS)
        {
            printf("%s:%d Start CapturePic\n", __func__, __LINE__);
            g_Album.lastTime = g_Album.curTime;
            do
            {
                ret = LUX_BASE_CapturePic_HD(LUX_STREAM_MAIN,&picInfo);
                if (0 != ret)
                {
                    IMP_LOG_ERR(TAG, " LUX_BASE_CapturePic failed\n");
                    break;
                }
                if (NULL != g_Album.alarmCbk.pEvntReportFuncCB)
                {                                                                                                       //LUX_NOTIFICATION_NAME_BABY_CRY  用于验证是否可用
                    g_Album.alarmCbk.pEvntReportFuncCB(picInfo.pAddr, picInfo.streamLen, LUX_NOTIFICATION_NAME_HUMAN);//9 LUX_NOTIFICATION_NAME_HUMAN 设置为相册抓拍 
                    g_Album.picCounts++;
                    printf("%s:%d Album[%d] report success!\n", __func__, __LINE__,g_Album.picCounts);
                }
                LUX_BASE_ReleasePic_HD(LUX_STREAM_MAIN,&picInfo);
            } while (0);
            //printf("%s:%d CapturePic Done\n", __func__, __LINE__);
        }
        Thread_Mutex_UnLock(&g_Album.mutex);
        //一小时时间结束后，主动关闭
        if(g_Album.curTime >g_Album.endTime && g_Album.bStarted)
        {
            printf("%s:%d Over 1 hour Stop\n", __func__, __LINE__);
            LUX_Jpeg_Album_Stop();
            //IPC_APP_ALBUM_Stat(LUX_FALSE);        //暂时屏蔽上报关闭
        }


        usleep(1000 * 1000);
    }
    //DP上报状态
}
INT_X LUX_Jpeg_Album_Init()
{
    //INT_X ret = LUX_ERR;
    memset(&g_Album,0,sizeof(LUX_Jpeg_Album_ST));
    LUX_Event_GetAlarmCbk(&g_Album.alarmCbk);

    if(g_Album.bInited)
    {
        IMP_LOG_ERR(TAG, "Warnning, Album already Inited\n", LUX_IVS_PERSON_DET_EN);
        return LUX_ERR;
    }
    Thread_Mutex_Create(&g_Album.mutex);

    Task_CreateThread(LUX_Jpeg_Album_Process, NULL);
    g_Album.bInited = LUX_TRUE;

    return LUX_OK;
}

INT_X LUX_Jpeg_Album_DeInit()
{
    //INT_X ret = LUX_ERR;
    g_Album.bInited = LUX_FALSE;

    Thread_Mutex_Destroy(&g_Album.mutex);
    return LUX_OK;
}

INT_X LUX_Jpeg_Album_Start()
{
    Thread_Mutex_Lock(&g_Album.mutex);
    if (g_Album.bStarted)
    {
        IMP_LOG_ERR(TAG, "Warnning, Album already Started\n", LUX_IVS_PERSON_DET_EN);
        return LUX_OK;
    }

    g_Album.picCounts = 0;
    g_Album.curTime = getTime_getMs();
    g_Album.endTime = g_Album.curTime + LUX_ABLUM_TOTAL_MS;
    g_Album.bStarted = LUX_TRUE;
    Thread_Mutex_UnLock(&g_Album.mutex);

    return LUX_OK;
}

INT_X LUX_Jpeg_Album_Stop()
{
    Thread_Mutex_Lock(&g_Album.mutex);
    g_Album.bStarted = LUX_FALSE;
    Thread_Mutex_UnLock(&g_Album.mutex);

    return LUX_OK;
}
BOOL_X LUX_Jpeg_Album_IsStart()
{
    BOOL_X isStart;
    Thread_Mutex_Lock(&g_Album.mutex);
    isStart = g_Album.bStarted;
    Thread_Mutex_UnLock(&g_Album.mutex);

    return isStart;
}
