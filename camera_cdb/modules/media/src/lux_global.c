#include "lux_global.h"
#include "lux_record.h"

#include <stdio.h>
#include <stdarg.h>

#include <jwhead.h>

#ifndef DISCOVERY_ONLY
#include <imp_log.h>
#include <lux_base.h>
#include <lux_config.h>
#include <idip.h>
#include <lux_iniparse.h>
#endif


DWORD_X bPlayingCryMusic = 0;
BOOL_X g_audio_onoff_status = LUX_FALSE;
DWORD_X handletype_of_playmusic = 0;
float g_cryDetectSensitive = JIO_SOUND_DET_SENSI_LOW;
idip_server_t* m_idip_server;
BOOL_X  g_bcryDetctAlarmSong = LUX_FALSE;
bool  push_live_stream_enable_main = 1;
bool  push_live_stream_enable_sub = 1;
bool  Semaphore_Pend_flag_sub = 0;
bool  Semaphore_Pend_flag_main = 0;
bool  LuxTestEnbale = 0;
lux_record_t* g_lux_record = NULL;
LUX_VIDEO_ENC_ATTR_ST* g_VEncAttr = NULL;
LUX_FSRC_ATTR_ST* g_framSrcAttr = NULL;
LUX_CONFIG_DATA_ST* g_configData = NULL;
live_streamer_args_t* args_main = NULL;
live_streamer_args_t* args_sub = NULL;
//LUX_CONFIG_JIO_CFG_ST* g_JioConfig = NULL;



char g_sensor_name[128] = { "imx327" };
char g_serial_str[128] = { "HMY12345678" };
char g_device_model[128] = {"JJOH201"};
char g_device_type[128] = { "bullet"};
char g_deivce_model_detail[128] = { "bullet JJOH201" };

char* led_color = "red";
int LED_FlashTime;

int fd_79;
int fd_116;
int fd_122;
int fd_123;

int g_sensor_h_flip = 0;
int g_sensor_v_flip = 0;

// // static instance of configuration model
// config_model_t cfg =
// {
//     .online_mode = 1,
//     .timezone = "+08:00",
//     .timezone_offset_s = 28800,
//     // "microphone/enabled"
//     .microphone_enabled = 1,
//     // "led/enabled"
//     .led_enabled = 0,
//     // "night_vision/mode"
//     .night_vision_mode = "off",
//     // "general/power_freq"
//     .power_freq = "50 HZ",

//     // "general/flip_mode"
//     .flip_mode = 0,

//     // Detectors
//     // "sound_detection/enabled"
//     .sound_detection = 0,
//     // "sound_detection/sensitivity"
//     .sound_detection_sensitivity = 64,
//     // "pir_detection/enabled"
//     .pir_detection = 0,
//     // "motion_detector/enabled"
//     .motion_detector = 0,
//     // "motion_detector/N/sensitivity"
//     .motion_detector_0_sensitivity = 0,
//     // "motion_detector/N/threshold"
//     .motion_detector_0_threshold = 0,

//     .motion_detector_0_zone = {0},

//     // current resolution
//     .resolution_0 = "1920x1080",
//     .resolution_1 = "720x576",
//     // current bitrate
//     .bitrate_0 = 1536,
//     .bitrate_1 = 256,
//     // current fps
//     .fps_0 = 25,
//     .fps_1 = 25,
//     // osd/show_time
//     .osd_show_time = 1,
//     // video codecs
//     .codec_0 = "H264",
//     .codec_1 = "H264",
//     // audio codecs
//     .acodec_0 = "AAC",
//     .acodec_1 = "AAC",
//     .online = 0,

//     .face_detection = 0,
//     .privacy_mask = 0,
//     .wdr = 0
// };

char* trim_left(char* buf)
{
    char* p = buf;

    do
    {
        if (*p == 9 || *p == 32 || *p == 10 || *p == 13)
        {
            p++;

            if (*p == '\0')
            {
                buf[0] = '\0';
                break;
            }

            continue;
        }
        else {
            if (p != buf)
            {
                size_t mv_len = strlen(buf) - (p - buf);
                memcpy(buf, p, mv_len);
                buf[mv_len] = '\0';
            }
            break;
        }
    } while (1);

    return buf;
}

char* trim_right(char* buf)
{
    int strlen1 = strlen(buf);
    char* pEnd = buf + strlen1;
    char* p = pEnd;

    do
    {
        --p;

        if (*p != 9 && *p != 32 && *p != 10 && *p != 13)
        {
            *++p = '\0';
            break;
        }

        if (p == buf)
        {
            buf[0] = '\0';
            break;
        }
    } while (1);

    return buf;
}

char* trim_pure(char* buf)
{
    char* p = buf;
    char* pOut = buf;

    do
    {
        if (*p == '\x1b')
        {
            // find '\x6d'
            p++;
            do
            {
                if (*p == '\x6d')
                {
                    p++;
                    break;
                }
                else if (*p == '\x1b')
                {
                    break;
                }
                else {
                    p++;
                    continue;
                }
            } while (*p);
        }
        else {
            *pOut++ = *p++;
        }
    } while (*p);

    *pOut = '\x0';
    return buf;
}

char* trim_all(char* buf)
{
    trim_left(buf);
    trim_right(buf);
    return trim_pure(buf);
}

int GetSensorName(char *sensorName)
{
    int ret = 0;
    int fd = 0;
    int data = -1;

    /* open device file */
    fd = open("/proc/jz/sinfo/info", O_RDONLY);
    if (-1 == fd) {
        MYERROR("err: open failed[%s]\n", "/proc/jz/sinfo/info");
        return -1;
    }

    char buf[200] = { 0 };

    int readlen = read(fd, buf, sizeof(buf) - 1);

    if (readlen > 3)
    {
        char *ptr = strstr(buf, ":");
        if (NULL != ptr)
        {
            ptr++;
            if (!isalnum((ptr[0]))) ptr++;

            if (ptr[strlen(ptr) - 1] == '\r' || ptr[strlen(ptr) - 1] == '\n')
            {
                ptr[strlen(ptr) - 1] = '\0';
            }

            if (ptr[strlen(ptr) - 1] == '\r' || ptr[strlen(ptr) - 1] == '\n')
            {
                ptr[strlen(ptr) - 1] = '\0';
            }

            if (strlen(ptr) < 50)
            {
                strcpy(sensorName, ptr);
            }
        }
    }

    /* close device file */
    close(fd);

    return ret;
}

int GetKeyStrFromDeviceInfo(char* pTag, char* value)
{
    if (NULL == pTag || NULL == value)
    {
        MYERROR("[pTag] or [value] is null ptr \n");
        return 0;
    }
    int read = 0;
    int len = 0;
    FILE* fp = NULL;
    char* line = NULL;

    char key[200];

    fp = fopen("/tmp/device_info.ini", "r");
    if (NULL == fp)
    {
        MYERROR("open fd[%s] error!\n", "/tmp/device_info.ini");
        return 0;
    }

    while (((read = getline(&line, (size_t*)&len, fp)) != -1) && !feof(fp))
    {
        trim_left(line);
        trim_right(line);
        if (0 == strncmp(pTag, line, strlen(pTag)))
        {
            printf("line:[%s]\n", line);
            /* 获取字符=和\n之间的有效字符串 */
            //sscanf(line, "%*[^=]=%[^\n]", value);
            char temp[256] = { 0 };

            char* p = strtok(line, "=");
            if (p)
            {
                strcpy(key, p);
                p = strtok(NULL, "=");
                if (!p)
                {
                    break;
                }
            }
            else
            {
                break;
            }
            
            strcpy(value, p);
            trim_left(value);
            trim_right(value);

            printf("key:[%s] value:[%s] \n", key, value);

            break;
        }
        len = 0;
    }

    if (line)
        free(line);

    return strlen(value);
}

const char* format_time(int64_t t)
{
    static char buf[32];
    struct tm* tmStart = gmtime(&t);
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", tmStart->tm_year + 1900, tmStart->tm_mon + 1, tmStart->tm_mday,
        tmStart->tm_hour, tmStart->tm_min, tmStart->tm_sec);
    return buf;
}

uint64_t time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

#if 0
LUX_VIDEO_ENC_ATTR_ST g_VEnd..cAttr =
{
    .bInit = LUX_FALSE,
    .encGrpAttr[0] =
    {
        .grpId = 0,
        .bindSrcModule = LUX_MODULS_FSRC,
        .bindSrcChan = 0,
        .encChnAttr[0] = /* Group 0,channel 0 视频主码流：1080p */
        {
            .chnId = CH0_INDEX,
            .bValid = CHN0_EN,
            .streamChn = LUX_STREAM_MAIN,
            .chnMux = PTHREAD_MUTEX_INITIALIZER,
        },
    },
    .encGrpAttr[1] =
    {
        .grpId = 1,
        .bindSrcModule = LUX_MODULS_IVS,
        .bindSrcChan = 0,
        .encChnAttr[0] = /* Group 1 channel 0 视频子码流：360p */
        {
            .chnId = CH1_INDEX,
            .bValid = CHN1_EN,
            .streamChn = LUX_STREAM_SUB,
            .chnMux = PTHREAD_MUTEX_INITIALIZER,

        },
        .encChnAttr[1] = /* Group 1 channel 1 抓图通道：360p */
        {
            .chnId = CH2_INDEX,
            .bValid = CHN2_EN,     //修改为0禁用图片流
            .streamChn = LUX_STREAM_JPEG,
            .chnMux = PTHREAD_MUTEX_INITIALIZER,

        },
    }
};
#endif

#if 0
LUX_FSRC_ATTR_ST g_framSrcAttr =
{
    .bInit = LUX_FALSE,
    .fsChn =
    {
        {
            /* 通道0（主码流）视频源参数 */
            .index = CH0_INDEX,
            .enable = CHN0_EN,
            .fsChnAttr =
            {
                .pixFmt = PIX_FMT_NV12,
                .picWidth = SENSOR_WIDTH_DEFAULT,
                .picHeight = SENSOR_HEIGHT_DEFAULT,
                .outFrmRateNum = SENSOR_FRAME_RATE_NUM,
                .outFrmRateDen = SENSOR_FRAME_RATE_DEN,
                .nrVBs = 3,
                .type = FS_PHY_CHANNEL,
           },
        },
        {
            /* 通道1（子码流）视频源参数 */
            .index = CH1_INDEX,
            .enable = CHN1_EN,
            .fsChnAttr =
            {
                .pixFmt = PIX_FMT_NV12,
                .picWidth = SENSOR_WIDTH_SECOND,
                .picHeight = SENSOR_HEIGHT_SECOND,
                .outFrmRateNum = SENSOR_FRAME_RATE_NUM,
                .outFrmRateDen = SENSOR_FRAME_RATE_DEN,
                .nrVBs = 3,
                .type = FS_PHY_CHANNEL,

                /*	//不需要裁剪
                .crop.enable = 0,
                .crop.top = 0,
                .crop.left = 0,
                .crop.width = SENSOR_WIDTH_SECOND,
                .crop.height = SENSOR_HEIGHT_SECOND,
                */

                .scaler.enable = LUX_TRUE,
                .scaler.outwidth = SENSOR_WIDTH_SECOND,
                .scaler.outheight = SENSOR_HEIGHT_SECOND,
           },
        },
    }
};
#endif
int LUX_Global_Init()
{
    int ret;

    //读取序列号
    if (access("/system/debug", 0) != 0)  //非调试模式，从KEY分区中读取
    {
        //序列号
        ret = GetKeyStrFromDeviceInfo("app_uuid", g_serial_str);
        if (ret <= 0)
        {
            MYERROR("ERR: get app_uuid, ret:%#x \n", ret);
        }
        else
        {
            MYTRACEING("read app_uuid:[%s] read len:[%d]\n", g_serial_str, ret);
        }
    }
    else  //调试模式，从文件中读取
    {
        memset(g_serial_str, 0, sizeof(g_serial_str));
        FILE* in = fopen("/system/jio/serial_num", "r");
        if (in)
        {
            fread(g_serial_str, 1, 50, in);
            fclose(in);
        }
    }

    //型号
    if (access("/system/debug_model", 0) != 0)
    {
        ret = GetKeyStrFromDeviceInfo("device_model", g_device_model);
        if (ret <= 0)
        {
            MYERROR("ERR: get device_model, ret:%#x \n", ret);
        }
        else
        {
            MYTRACEING("read device_model:[%s] read len:[%d]\n", g_device_model, ret);
        }
    }
    else
    {
        memset(g_device_model, 0, sizeof(g_device_model));
        FILE* in = fopen("/system/jio/device_model", "r");
        if (in)
        {
            fread(g_device_model, 1, 50, in);
            fclose(in);
        }
    }

    //类型
    if (access("/system/debug_type", 0) != 0)
    {
        ret = GetKeyStrFromDeviceInfo("device_type", g_device_type);
        if (ret <= 0)
        {
            MYERROR("ERR: get device_type, ret:%#x \n", ret);
        }
        else
        {
            MYTRACEING("read device_type:[%s] read len:[%d]\n", g_device_type, ret);
        }
    }
    else
    {
        memset(g_device_type, 0, sizeof(g_device_type));
        FILE* in = fopen("/system/jio/device_type", "r");
        if (in)
        {
            fread(g_device_type, 1, 50, in);
            fclose(in);
        }
    }

    //处理字符串中的特殊控制字符　前后空格　前后的回车换行符
    trim_left(g_serial_str);
    trim_right(g_serial_str);
    trim_pure(g_serial_str);

    trim_left(g_device_model);
    trim_right(g_device_model);
    trim_pure(g_device_model);

    trim_left(g_device_type);
    trim_right(g_device_type);
    trim_pure(g_device_type);

    sprintf(g_deivce_model_detail, "%s %s", g_device_type, g_device_model);

    //读取SENSOR名称
    if (0 == GetSensorName(g_sensor_name))
    {
        MYTRACE("get sensor name:[%s] \n", g_sensor_name);
    }
    else
    {
        MYWARN("set sensor name failed \n");
    }

#ifndef DISCOVERY_ONLY
    g_lux_record = (lux_record_t*)malloc(sizeof(lux_record_t));
    memset(g_lux_record, 0, sizeof(lux_record_t));

    g_VEncAttr = (LUX_VIDEO_ENC_ATTR_ST*)malloc(sizeof(LUX_VIDEO_ENC_ATTR_ST));
    memset(g_VEncAttr, 0, sizeof(LUX_VIDEO_ENC_ATTR_ST));

    g_configData = (LUX_CONFIG_DATA_ST*)malloc(sizeof(LUX_CONFIG_DATA_ST));
    memset(g_configData, 0, sizeof(LUX_CONFIG_DATA_ST));

    g_VEncAttr->bInit = LUX_FALSE;
    g_VEncAttr->encGrpAttr[0].grpId = 0;
    g_VEncAttr->encGrpAttr[0].bindSrcModule = LUX_MODULS_FSRC;
    g_VEncAttr->encGrpAttr[0].bindSrcChan = 0;

    /* Group 0,channel 0 视频主码流：1080p */
    g_VEncAttr->encGrpAttr[0].encChnAttr[0].chnId = CH0_INDEX;
    g_VEncAttr->encGrpAttr[0].encChnAttr[0].bValid = CHN0_EN;
    g_VEncAttr->encGrpAttr[0].encChnAttr[0].streamChn = LUX_STREAM_MAIN;
    g_VEncAttr->encGrpAttr[0].encChnAttr[0].chnMux = (OSI_MUTEX)PTHREAD_MUTEX_INITIALIZER;

    /* Group 1 */
    g_VEncAttr->encGrpAttr[1].grpId = 1;
    g_VEncAttr->encGrpAttr[1].bindSrcModule = LUX_MODULS_IVS;
    g_VEncAttr->encGrpAttr[1].bindSrcChan = 0;

    /* Group 1 channel 0 视频子码流：360p */
    g_VEncAttr->encGrpAttr[1].encChnAttr[0].chnId = CH1_INDEX;
    g_VEncAttr->encGrpAttr[1].encChnAttr[0].bValid = CHN1_EN;
    g_VEncAttr->encGrpAttr[1].encChnAttr[0].streamChn = LUX_STREAM_SUB;
    g_VEncAttr->encGrpAttr[1].encChnAttr[0].chnMux = (OSI_MUTEX)PTHREAD_MUTEX_INITIALIZER;

    /* Group 1 channel 1 抓图通道：360p */
    g_VEncAttr->encGrpAttr[1].encChnAttr[1].chnId = CH2_INDEX;
    g_VEncAttr->encGrpAttr[1].encChnAttr[1].bValid = CHN2_EN;
    g_VEncAttr->encGrpAttr[1].encChnAttr[1].streamChn = LUX_STREAM_JPEG;
    g_VEncAttr->encGrpAttr[1].encChnAttr[1].chnMux = (OSI_MUTEX)PTHREAD_MUTEX_INITIALIZER;


    /*视频源的值初始化*/
    g_framSrcAttr = (LUX_FSRC_ATTR_ST*)malloc(sizeof(LUX_FSRC_ATTR_ST));
    memset(g_framSrcAttr->fsChn, 0, FS_CHN_NUM*sizeof(LUX_FSRC_CHN_ATTR_ST));
    g_framSrcAttr->bInit = LUX_FALSE;

    /* 通道0（主码流）视频源参数 */
    g_framSrcAttr->fsChn[0].index = CH0_INDEX;
    g_framSrcAttr->fsChn[0].enable = CHN0_EN;
    g_framSrcAttr->fsChn[0].fsChnAttr.pixFmt = PIX_FMT_NV12;
    g_framSrcAttr->fsChn[0].fsChnAttr.picWidth = SENSOR_WIDTH_DEFAULT;
    g_framSrcAttr->fsChn[0].fsChnAttr.picHeight = SENSOR_HEIGHT_DEFAULT;
    g_framSrcAttr->fsChn[0].fsChnAttr.outFrmRateNum = SENSOR_FRAME_RATE_NUM;
    g_framSrcAttr->fsChn[0].fsChnAttr.outFrmRateDen = SENSOR_FRAME_RATE_DEN;
    g_framSrcAttr->fsChn[0].fsChnAttr.nrVBs = 3;
    g_framSrcAttr->fsChn[0].fsChnAttr.type = FS_PHY_CHANNEL;

    /* 通道1（子码流）视频源参数 */
    g_framSrcAttr->fsChn[1].index = CH1_INDEX;
    g_framSrcAttr->fsChn[1].enable = CHN1_EN;
    g_framSrcAttr->fsChn[1].fsChnAttr.pixFmt = PIX_FMT_NV12;
    g_framSrcAttr->fsChn[1].fsChnAttr.picWidth = SENSOR_WIDTH_SECOND;
    g_framSrcAttr->fsChn[1].fsChnAttr.picHeight = SENSOR_HEIGHT_SECOND;
    g_framSrcAttr->fsChn[1].fsChnAttr.outFrmRateNum = SENSOR_FRAME_RATE_NUM;
    g_framSrcAttr->fsChn[1].fsChnAttr.outFrmRateDen = SENSOR_FRAME_RATE_DEN;
    g_framSrcAttr->fsChn[1].fsChnAttr.nrVBs = 3;
    g_framSrcAttr->fsChn[1].fsChnAttr.type = FS_PHY_CHANNEL;
    g_framSrcAttr->fsChn[1].fsChnAttr.scaler.enable = LUX_TRUE;
    g_framSrcAttr->fsChn[1].fsChnAttr.scaler.outwidth = SENSOR_WIDTH_SECOND;
    g_framSrcAttr->fsChn[1].fsChnAttr.scaler.outheight = SENSOR_HEIGHT_SECOND;

    args_main = (live_streamer_args_t*)malloc(sizeof(live_streamer_args_t));
    args_sub  = (live_streamer_args_t*)malloc(sizeof(live_streamer_args_t));
    
    args_main->av_source = NULL;
    args_main->channel = LUX_STREAM_MAIN;

    args_sub->av_source = NULL;
    args_sub->channel = LUX_STREAM_SUB;

    char szTemp[24] = { 0 };
    if (GetKeyStrFromDeviceInfo("init_sensor_h_flip", szTemp) > 0)
    {
        g_sensor_h_flip = atoi(szTemp);
    }

    if (GetKeyStrFromDeviceInfo("init_sensor_v_flip", szTemp) > 0)
    {
        g_sensor_v_flip = atoi(szTemp);
    }

    MYTRACEING("read init_sensor_h_flip:[%d] \n", g_sensor_h_flip);
    MYTRACEING("read init_sensor_v_flip:[%d] \n", g_sensor_v_flip);

#endif


    return 0;
}

void debug_printf(int level, const char* file, const char* fun, const int line, const char* fmt, ...)
{
    struct tm* pTempTm;
    struct timeval time;

    static char buffer[1024] = { 0 };

    gettimeofday(&time, NULL);
    pTempTm = localtime(&time.tv_sec);

    va_list arg_ptr;
    va_list arg_tmp;
    va_start(arg_ptr, fmt);
    va_copy(arg_tmp, arg_ptr);

    switch (level)
    {
    case MY_LOG_LEVEL_TRACE:
        printf("[%s][%02d:%02d:%02d.%03d %s %s %d]:", "INFO ", pTempTm->tm_hour, pTempTm->tm_min, pTempTm->tm_sec, time.tv_usec / 1000, file, fun, line);
        vsnprintf(buffer, sizeof(buffer) - 1, fmt, arg_ptr);
#ifndef DISCOVERY_ONLY
        imp_log_fun(IMP_LOG_LEVEL_INFO, 0, IMP_LOG_OUT_SERVER, "LUX_AV", NULL, 0, NULL, buffer);
#endif
        break;

    case MY_LOG_LEVEL_TRACEING:
        printf("\033[5m\033[40;42m[%s]\033[0m[%02d:%02d:%02d.%03d %s %s %d] ", "INFO ", pTempTm->tm_hour, pTempTm->tm_min, pTempTm->tm_sec, time.tv_usec / 1000, file, fun, line);
        vsnprintf(buffer, sizeof(buffer) - 1, fmt, arg_ptr);
#ifndef DISCOVERY_ONLY
        imp_log_fun(IMP_LOG_LEVEL_INFO, 0, IMP_LOG_OUT_SERVER, "LUX_AV", NULL, 0, NULL, buffer);
#endif
        break;

    case MY_LOG_LEVEL_WARN:
        printf("\033[1m\033[40;33m[%s]\033[0m[%02d:%02d:%02d.%03d %s %s %d] ", "WARN ", pTempTm->tm_hour, pTempTm->tm_min, pTempTm->tm_sec, time.tv_usec / 1000, file, fun, line);
        vsnprintf(buffer, sizeof(buffer) - 1, fmt, arg_ptr);
#ifndef DISCOVERY_ONLY
        imp_log_fun(IMP_LOG_LEVEL_WARN, 0, IMP_LOG_OUT_SERVER, "LUX_AV", NULL, 0, NULL, buffer);
#endif
        break;

    case MY_LOG_LEVEL_WARNING:
        printf("\033[5m\033[40;33m[%s]\033[0m[%02d:%02d:%02d.%03d %s %s %d] ", "WARN ", pTempTm->tm_hour, pTempTm->tm_min, pTempTm->tm_sec, time.tv_usec / 1000, file, fun, line);
        vsnprintf(buffer, sizeof(buffer) - 1, fmt, arg_ptr);
#ifndef DISCOVERY_ONLY
        imp_log_fun(IMP_LOG_LEVEL_WARN, 0, IMP_LOG_OUT_SERVER, "LUX_AV", NULL, 0, NULL, buffer);
#endif
        break;

    case MY_LOG_LEVEL_ERROR:
        printf("\033[1m\033[40;31m[%s]\033[0m[%02d:%02d:%02d.%03d %s %s %d] ", "ERROR", pTempTm->tm_hour, pTempTm->tm_min, pTempTm->tm_sec, time.tv_usec / 1000, file, fun, line);
        vsnprintf(buffer, sizeof(buffer) - 1, fmt, arg_ptr);
#ifndef DISCOVERY_ONLY
        imp_log_fun(IMP_LOG_LEVEL_ERROR, 0, IMP_LOG_OUT_SERVER, "LUX_AV", NULL, 0, NULL, buffer);
#endif
        break;

    case MY_LOG_LEVEL_ERRORING:
        printf("\033[5m\033[40;31m[%s]\033[0m[%02d:%02d:%02d.%03d %s %s %d] ", "ERROR", pTempTm->tm_hour, pTempTm->tm_min, pTempTm->tm_sec, time.tv_usec / 1000, file, fun, line);
        vsnprintf(buffer, sizeof(buffer) - 1, fmt, arg_ptr);
#ifndef DISCOVERY_ONLY
        imp_log_fun(IMP_LOG_LEVEL_ERROR, 0, IMP_LOG_OUT_SERVER, "LUX_AV", NULL, 0, NULL, buffer);
#endif
        break;

    default:
        break;
    }

    vprintf(fmt, arg_ptr);

    va_end(arg_ptr);
}
#ifndef DISCOVERY_ONLY
int checkip(const char *ip)
{
    int ipvalue[4] = {0,0,0,0};

    sscanf(ip, "%d.%d.%d.%d", &ipvalue[0], &ipvalue[1], &ipvalue[2], &ipvalue[3]);

    int i;
    for (i = 0; i < 4; i++)
    {
        if (ipvalue[i] > 0 && ipvalue[i] < 255)
        {
        }
        else
        {
            return -1;
        }
    }

    return 0;
}

int LUX_Network_setip(const char* ip, const char* mask)
{
    char buf[128] = { 0 };
    
    sprintf(buf, "/system/init/ip.sh set %s %s", ip, mask);

    return LUX_BASE_System_CMD(buf);
}

int LUX_Network_setipEx(const char* ip, const char* mask, const char* gateway, const char* dns)
{
    char buf[128] = { 0 };

    sprintf(buf, "/system/init/ip.sh set %s %s %s %s", ip, mask, gateway, dns);

    return LUX_BASE_System_CMD(buf);
}

int LUX_Network_setDHCP(int enable)
{
    char buf[64] = { 0 };

    sprintf(buf, "/system/init/ip.sh set dhcp");

    return LUX_BASE_System_CMD(buf);
}

int LUX_Network_getip(char *mac, char* ip, char* mask)
{
    LUX_BASE_System_CMD_Ex("set +v;ifconfig eth0 | grep \"HWaddr\" | grep -v \"grep\" | awk '{print $5}';set -v", mac);

    //ifconfig eth0 | grep "inet addr" | grep -v "grep" | awk -F':' '{print $2}' | awk '{print $1}'
    LUX_BASE_System_CMD_Ex("set +v;ifconfig eth0 | grep \"inet addr\" | grep -v \"grep\" | awk -F':' '{print $2}' | awk '{print $1}';set -v", ip);

    //ifconfig eth0 | grep "Mask:" | grep -v "grep" | awk -F':' '{print $4}'
    LUX_BASE_System_CMD_Ex("set +v;ifconfig eth0 | grep \"Mask:\" | grep -v \"grep\" | awk -F':' '{print $4}';set -v", mask);

    return 0;
}

int LUX_Network_getip2(const char* eth, char* ip)
{
    return LUX_BASE_System_CMD_Ex("set +v;ifconfig eth0 | grep \"inet addr\" | grep -v \"grep\" | awk -F':' '{print $2}' | awk '{print $1}';set -v", ip);
}

int LUX_Network_setGateway(const char* gw)
{
    char buf[128] = { 0 };
    //sprintf(buf, "route delete default;route add default gw %s", gw);
    char ip[20], mask[20], dns[20];
    LUX_Network_getip(NULL, ip, mask);
    LUX_Network_GetDNS(dns);
    sprintf(buf, "/system/init/ip.sh set %s %s %s %s", ip, mask, gw, dns);
    return LUX_BASE_System_CMD(buf);
}

int LUX_Network_getGateway(char* gw)
{
    return LUX_BASE_System_CMD_Ex("route -n | grep eth0 |  awk '{print $2;exit;}'", gw);
}

int LUX_Network_SetDNS(const char* dns)
{
    char buf[128] = { 0 };

    char ip[20], mask[20], gw[20];

    //sprintf(buf, "echo \"nameserver %s #eth0\" > /tmp/resolv.conf", ip);

    LUX_Network_getip(NULL, ip, mask);
    LUX_Network_getGateway(gw);
    sprintf(buf, "/system/init/ip.sh set %s %s %s %s", ip, mask, gw, dns);
    return LUX_BASE_System_CMD(buf);
}

int LUX_Network_GetDNS(char* dns)
{
//cat /tmp/resolv.conf | awk '{print $2;exit;}'
    char buf[128] = { 0 };

    sprintf(buf, "cat /tmp/resolv.conf | awk '{print $2;exit;}'");

    return LUX_BASE_System_CMD_Ex(buf, dns);

}

int LUX_Network_GetMAC(char* interface, unsigned char* pMac)
{
    struct ifreq ifreq;
    int sockfd = 0;
    unsigned char mac[6] = { 0 };

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    strcpy(ifreq.ifr_name, interface);

    if (ioctl(sockfd, SIOCGIFHWADDR, &ifreq) < 0)
    {
        close(sockfd);
        return -2;
    }

    memcpy(mac, ifreq.ifr_hwaddr.sa_data, 6);
    printf("MAC:%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    if (pMac != NULL)
    {
        memcpy(pMac, mac, 6);
    }

    close(sockfd);

    return 0;
}

int LUX_Network_GetMACStr(char *interface, unsigned char *pMac)
{
	struct ifreq ifreq;
	int sockfd = 0;
	unsigned char mac[6] = {0};

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;

	strcpy(ifreq.ifr_name, interface);

	if(ioctl(sockfd, SIOCGIFHWADDR, &ifreq) < 0)
	{
		close(sockfd);
		return -2;
	}

	memcpy(mac, ifreq.ifr_hwaddr.sa_data, 6);
	printf("MAC:%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	sprintf((char*)pMac, "%02X-%02X-%02X-%02X-%02X-%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	close(sockfd);

	return 0;
}
#endif
