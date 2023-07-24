/*
 * lux_audio.c
 *
 * 基于Ingenic平台封装的音频模块
 * 
 */

#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/prctl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <imp_audio.h>
#include <imp_log.h>
#include <imp_ivs.h>
#include <lux_iniparse.h>
#include "lux_base.h"
#include "comm_def.h"
#include "comm_error_code.h"
#include "lux_audio.h"
#include "ivs_common.h"
#include "ivs_inf_cryDet.h"
#include "ivs_interface.h"
#include "iaac.h"
#include "lux_hal_misc.h"

#include "lux_summary.h"

#include "lux_base.h"
#include "lux_ivs.h"
#include "lux_pwm.h"
#include "lux_global.h"

//#include "tuya_ipc_cloud_storage.h"
// #include "tuya_ipc_stream_storage.h"
// #include "tuya_ipc_common_demo.h"
// #include "tuya_ipc_dp_handler.h"
#include "lux_ipc_control.h"
#include "idip.h"
#include "idip/idip_custom_events.h"
#include "jwhead.h"
#include "lux_config.h" //hy add 23/2/20


#define TAG "lux_audio"

#define LUX_AUDIO_AI_DEV_ID     1
#define LUX_AUDIO_AI_CHAN_ID    0
#define LUX_AUDIO_AO_DEV_ID    0
#define LUX_AUDIO_AO_CHAN_ID   0

#define LUX_AUDIO "audio"
#define LUX_AUDIO_FRAMERATE 40      /* 音频 输入&输出 帧率 */
#define LUX_AO_PCM_BUF_LEN  2048    /* ao解码缓存大小 */
#define LUX_AUDIO_NUM_PER_FRM 640   /* 每帧的采样点个数 */
#define LUX_CRADLESONG_DURATION 20  /* 自定义音频最大时长 单位(s) */

/* audio common parse ini */
#define LUX_AUDIO_AEC "audio_aec_en"

/* audio in parse ini */
#define LUX_AUDIO_IN_TYPE "audio_in_type"
#define LUX_AUDIO_IN_SAMPLE_RATE "audio_in_sample_rate"
#define LUX_AI_VOLUME "ai_volume"
#define LUX_AI_GAIN   "ai_gain"
#define LUX_AI_NS       "ai_ns_en"
#define LUX_AI_NS_LENEL "ai_ns_level"
#define LUX_AI_HPF "ai_hpf_en"
#define LUX_AI_AGC       "ai_agc_en"
#define LUX_AI_AGC_LEVEL "ai_agc_level"
#define LUX_AI_AGC_GAIN  "ai_agc_gain"

/* audio in cry detect, define in /system/init/tuya_config.ini, section: [function] */
#define LUX_AI_CRY_SECTION "algorithm_config"
#define LUX_AI_CRY_ONOFF "cry_onoff"
//#define LUX_AI_CRY_SENSI "cry_sensitive"

/* audio out parse ini */
#define LUX_AUDIO_OUT_TYPE "audio_out_type"
#define LUX_AUDIO_OUT_SAMPLE_RATE "audio_out_sample_rate"
#define LUX_AO_VOLUME "ao_volume"
#define LUX_AO_GAIN   "ao_gain"
#define LUX_AO_HPF  "ao_hpf_en"
#define LUX_AO_AGC       "ao_agc_en"
#define LUX_AO_AGC_LEVEL "ao_agc_level"
#define LUX_AO_AGC_GAIN  "ao_agc_gain"
#define LUX_SLEEP_MODE   "privacy_mask"

//#define LUX_AUDIO_ENC   /* 开启音频ai编码功能,暂未开发 */

FILE *cradlesongFp = NULL;
// extern INT_T gcustomed_action_get;
// extern DWORD_T gstart_record_time;
/* customed_voice_action status */
#define LUX_CUSTOMED_VOICE_ACTION "customed_voice_action"

INT_X LUX_Customed_Voice_Action(void);
static void* thread_play_file(void* arg);

//for test
// #define LUX_AI_DUMP_DATA
//#define LUX_AO_DUMP_DATA

typedef struct
{
    BOOL_X bInit;
    UINT_X devId;
    OSI_MUTEX mutex;

    UINT_X frmIdx;                               /* 帧全局结构索引 */
    IMPAudioFrame chnFrame[MAX_AUDIO_FRAME_NUM]; /* ai channel获取的音频帧 */
    LUX_AI_CHN_ATTR_ST chnAttr; /* audio in 通道属性，只支持一个通道 */

    /* 音频输入设备属性（编码器参数） */
    IMPAudioIOAttr      impAttr; /* imp_ai 属性 */
    IMPAudioIChnParam   aiParam;

} LUX_AUDIO_AI_ST;

typedef struct
{
    BOOL_X bInit;
    UINT_X devId;
    OSI_MUTEX mutex;

    PVOID_X pPcmBuf;    /* 解码音频数据后的临时缓存 */
    INT_X   frmSeq;     /* frame 序号 */
    LUX_AO_CHN_ATTR_ST chnAttr; /* audio out 通道属性，只支持一个通道 */

    /* 音频输出设备属性（解码器参数） */
    IMPAudioIOAttr impAttr; /* imp_ao 属性 */
} LUX_AUDIO_AO_ST;

typedef struct 
{
    BOOL_X    bInit;
    OSI_MUTEX mutex;

    BOOL_X bAecEnable;      /* 回声消除 */

    LUX_AUDIO_AI_ST aiDevAttr;  /* audio in 设备属性，只支持一个 device */
    LUX_AUDIO_AO_ST aoDevAttr;  /* audio out 设备属性，只支持一个 device */
} LUX_AUDIO_ATTR_ST;

static LUX_AUDIO_ATTR_ST g_audioAttr =
{
    .aiDevAttr = {
        .devId = LUX_AUDIO_AI_DEV_ID,
        .aiParam.usrFrmDepth = LUX_AUDIO_FRAMERATE,
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .chnAttr = {
            .chnId = LUX_AUDIO_AI_CHAN_ID,
            .payloadType = PT_PCM,
            .gain = 28,
            .volume = 60, /* 音量的取值范围为[-30 ~ 120] */
        },
        .impAttr = {
            .bitwidth = AUDIO_BIT_WIDTH_16,
            .samplerate = AUDIO_SAMPLE_RATE_16000,
            .soundmode = AUDIO_SOUND_MODE_MONO,
            .frmNum = LUX_AUDIO_FRAMERATE,
            .numPerFrm = LUX_AUDIO_NUM_PER_FRM, //AUDIO_SAMPLE_RATE_16000 / LUX_AUDIO_FRAMERATE,
            .chnCnt = 1,
        },
    },
    .aoDevAttr = {
        .devId = LUX_AUDIO_AO_DEV_ID,
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .chnAttr = {
            .chnId = LUX_AUDIO_AO_DEV_ID,
            .payloadType = PT_PCM,
            .gain = 28,
            .volume = 80,
        },
        .impAttr = {
            .bitwidth = AUDIO_BIT_WIDTH_16,
            .samplerate = AUDIO_SAMPLE_RATE_16000,
            .soundmode = AUDIO_SOUND_MODE_MONO,
            .frmNum = LUX_AUDIO_FRAMERATE,
            .numPerFrm = LUX_AUDIO_NUM_PER_FRM, //AUDIO_SAMPLE_RATE_16000 / LUX_AUDIO_FRAMERATE,
            .chnCnt = 1,
        },
    },
    .mutex = PTHREAD_MUTEX_INITIALIZER,
};
typedef struct 
{    /* 哭声检测算法 */
    OSI_MUTEX mutex;

    BOOL_X bCryInit;
    BOOL_X bCryOpen;
    INT_X cryCount;
    IMPIVSInterface *interface;
    IAACInfo iaacInfo;
    crydet_param_input_t crydetParam;
    UINT_X FrmSeq;
    LUX_AI_CRY_DET_RESULT *pCryResult;  /* 哭声检测结果，功能未开启返回空 */
}LUX_AUDIO_CRY_ST;

static LUX_AUDIO_CRY_ST g_cryAttr =
{
    .bCryInit = false,
    .bCryOpen = false,
    .cryCount = 0,
    .iaacInfo.license_path = INGENIT_ALG_FILE_NAME,
    .iaacInfo.sn = "fc99a05479571355d2b9da163308fa07",
    .crydetParam.stride = 3,
    //.crydetParam.skip_num = 0,  //此参数为算法内部使用，无须操作
    .crydetParam.numPerFrm = LUX_AUDIO_NUM_PER_FRM, //AUDIO_SAMPLE_RATE_16000 / LUX_AUDIO_FRAMERATE,
};

/* My G711 Encoder */
#define SIGN_BIT (0x80)  /* Sign bit for a A-law byte. */
#define QUANT_MASK (0xf) /* Quantization field mask. */
#define NSEGS (8)        /* Number of A-law segments. */
#define SEG_SHIFT (4)    /* Left shift for segment number. */
#define SEG_MASK (0x70)  /* Segment field mask. */
#define BIAS (0x84)      /* Bias for linear code. */

#ifdef LUX_AUDIO_ENC
static short seg_end[8] = {0xFF,0x1FF,0x3FF,0x7FF,0xFFF,0x1FFF,0x3FFF,0x7FFF};


static int search(int val, short *table, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        if (val <= *table++)
            return (i);
    }
    return (size);
}

static unsigned char linear2alaw(int pcm_val)
{
    int mask;
    int seg;
    unsigned char aval;

    if (pcm_val >= 0)
    {
        mask = 0xD5; /* sign (7th) bit = 1 */
    }
    else
    {
        mask = 0x55; /* sign bit = 0 */
        pcm_val = -pcm_val - 8;
    }

    seg = search(pcm_val, seg_end, 8);
    if (seg >= 8) /* out of range, return maximum value. */
        return (0x7F ^ mask);
    else
    {
        aval = seg << SEG_SHIFT;
        if (seg < 2)
            aval |= (pcm_val >> 4) & QUANT_MASK;
        else
            aval |= (pcm_val >> (seg + 3)) & QUANT_MASK;
        return (aval ^ mask);
    }
}

static unsigned char linear2ulaw(int pcm_val)
{
    int mask;
    int seg;
    unsigned char uval;

    if (pcm_val < 0)
    {
        pcm_val = BIAS - pcm_val;
        mask = 0x7F;
    }
    else
    {
        pcm_val += BIAS;
        mask = 0xFF;
    }

    seg = search(pcm_val, seg_end, 8);
    if (seg >= 8) /* out of range, return maximum value. */
        return (0x7F ^ mask);
    else
    {
        uval = (seg << 4) | ((pcm_val >> (seg + 3)) & 0xF);
        return (uval ^ mask);
    }
}

static int my_g711a_encode(unsigned char g711_data[], const short amp[], int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        g711_data[i] = linear2alaw(amp[i]);
    }

    return len;
}

static int my_g711u_encode(unsigned char g711_data[], const short amp[], int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        g711_data[i] = linear2ulaw(amp[i]);
    }

    return len;
}

static int LUX_AUDIO_PCM2G711a(char *InAudioData, char *OutAudioData, int DataLen)
{
    int Retaen = 0;

    if ((NULL == InAudioData) || (NULL == OutAudioData) || (0 == DataLen))
    {
        IMP_LOG_ERR(TAG, "Error, empty data or transmit failed, exit !\n");
        return -1;
    }

    Retaen = my_g711a_encode((unsigned char *)OutAudioData, (short *)InAudioData, DataLen / 2);

    return Retaen;
}

static int LUX_AUDIO_PCM2G711u(char *InAudioData, char *OutAudioData, int DataLen)
{
    int Retuen = 0;

    if ((NULL == InAudioData) || (NULL == OutAudioData) || (0 == DataLen))
    {
        IMP_LOG_ERR(TAG, "Error, empty data or transmit failed, exit !\n");
        return -1;
    }

    Retuen = my_g711u_encode((unsigned char *)OutAudioData, (short *)InAudioData, DataLen / 2);

    return Retuen;
}

#endif

static int alaw2linear(unsigned char a_val)
{
    int t;
    int seg;

    a_val ^= 0x55;

    t = (a_val & QUANT_MASK) << 4;
    seg = ((unsigned)a_val & SEG_MASK) >> SEG_SHIFT;
    switch (seg)
    {
    case 0:
        t += 8;
        break;
    case 1:
        t += 0x108;
        break;
    default:
        t += 0x108;
        t <<= seg - 1;
    }
    return ((a_val & SIGN_BIT) ? t : -t);
}

static int ulaw2linear(unsigned char u_val)
{
    int t;

    u_val = ~u_val;

    t = ((u_val & QUANT_MASK) << 3) + BIAS;
    t <<= ((unsigned)u_val & SEG_MASK) >> SEG_SHIFT;

    return ((u_val & SIGN_BIT) ? (BIAS - t) : (t - BIAS));
}

static int my_g711a_decode(short amp[], const unsigned char g711a_data[], int g711a_bytes)
{
    int i;
    int samples;
    unsigned char code;
    int sl;

    for (samples = i = 0;;)
    {
        if (i >= g711a_bytes)
            break;
        code = g711a_data[i++];

        sl = alaw2linear(code);

        amp[samples++] = (short)sl;
    }
    return samples * 2;
}

static int my_g711u_decode(short amp[], const unsigned char g711u_data[], int g711u_bytes)
{
    int i;
    int samples;
    unsigned char code;
    int sl;

    for (samples = i = 0;;)
    {
        if (i >= g711u_bytes)
            break;
        code = g711u_data[i++];

        sl = ulaw2linear(code);

        amp[samples++] = (short)sl;
    }
    return samples * 2;
}

static int LUX_AUDIO_G711a2PCM(char *InAudioData, char *OutAudioData, int DataLen)
{
    int Retade = 0;

    if ((NULL == InAudioData) || (NULL == OutAudioData) || (0 == DataLen))
    {
        IMP_LOG_ERR(TAG, "Error, empty data or transmit failed, exit !\n");
        return -1;
    }

    Retade = my_g711a_decode((short *)OutAudioData, (unsigned char *)InAudioData, DataLen);

    return Retade;
}

static int LUX_AUDIO_G711u2PCM(char *InAudioData, char *OutAudioData, int DataLen)
{
    int Retude = 0;

    if ((NULL == InAudioData) || (NULL == OutAudioData) || (0 == DataLen))
    {
        IMP_LOG_ERR(TAG, "Error, empty data or transmit failed, exit !\n");
        return -1;
    }

    Retude = my_g711u_decode((short *)OutAudioData, (unsigned char *)InAudioData, DataLen);

    return Retude;
}

/**
 * @description: 从media配置文件中读取音频参数
 * @return 0
 */
static INT_X LUX_AUDIO_ReadIni()
{

    INT_X ret = LUX_ERR;
#if 1
    LUX_AUDIO_AI_ST* pAiAttr = &g_audioAttr.aiDevAttr;
    LUX_AUDIO_AO_ST* pAoAttr = &g_audioAttr.aoDevAttr;
   // LUX_AUDIO_CRY_ST *pCry = &g_cryAttr;
    //BOOL_X  sleepMode = LUX_FALSE;

    pAiAttr->chnAttr.payloadType = (LUX_AUDIO_PAYLOAD_TYPE)g_configData->media_config.audio_config.audio_in_type;
    pAiAttr->impAttr.samplerate = (LUX_AUDIO_PAYLOAD_TYPE)g_configData->media_config.audio_config.audio_in_sample_rate;
    pAoAttr->chnAttr.payloadType = (LUX_AUDIO_PAYLOAD_TYPE)g_configData->media_config.audio_config.audio_out_type;
    pAoAttr->impAttr.samplerate = (LUX_AUDIO_PAYLOAD_TYPE)g_configData->media_config.audio_config.audio_out_sample_rate;
    g_audioAttr.bAecEnable = g_configData->media_config.audio_config.audio_aec_en;
    pAiAttr->chnAttr.volume = (INT_X)g_configData->media_config.audio_config.ai_volume;
    pAiAttr->chnAttr.gain = (INT_X)g_configData->media_config.audio_config.ai_gain;
    pAiAttr->chnAttr.bNsEnable = g_configData->media_config.audio_config.ai_ns_en;
    pAiAttr->chnAttr.nsLevel = (UINT_X)g_configData->media_config.audio_config.ai_ns_level;
    pAiAttr->chnAttr.bHpfEnable = g_configData->media_config.audio_config.ai_hpf_en;
    pAiAttr->chnAttr.bAgcEnable = g_configData->media_config.audio_config.ai_agc_en;
    pAiAttr->chnAttr.agcParam.targetLevelDbfs = (INT_X)g_configData->media_config.audio_config.ai_agc_level;
    pAiAttr->chnAttr.agcParam.compressionGaindB = (INT_X)g_configData->media_config.audio_config.ai_agc_gain;
    pAoAttr->chnAttr.gain = (INT_X)g_configData->media_config.audio_config.ao_gain;
    pAoAttr->chnAttr.bHpfEnable = g_configData->media_config.audio_config.ao_hpf_en;
    pAoAttr->chnAttr.bAgcEnable = g_configData->media_config.audio_config.ao_agc_en;
    pAoAttr->chnAttr.agcParam.targetLevelDbfs = (INT_X)g_configData->media_config.audio_config.ao_agc_level;
    pAoAttr->chnAttr.agcParam.compressionGaindB = (INT_X)g_configData->media_config.audio_config.ao_agc_gain;

    //free(media_parameter);
#endif
// #pragma region 原有读取配置文件
#if 0
   BOOL_X  sleepMode = LUX_FALSE;
   LUX_AUDIO_CRY_ST *pCry = &g_cryAttr;
    LUX_AUDIO_AI_ST* pAiAttr = &g_audioAttr.aiDevAttr;
    LUX_AUDIO_AO_ST* pAoAttr = &g_audioAttr.aoDevAttr;   
   /* COMMON */
   ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, LUX_AUDIO, LUX_AUDIO_IN_TYPE, (int*)&pAiAttr->chnAttr.payloadType);
   if (LUX_OK != ret)
       IMP_LOG_ERR(TAG, "LUX_AUDIO_IN_TYPE failed,error [0x%x]\n", ret);
    
   ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, LUX_AUDIO, LUX_AUDIO_IN_SAMPLE_RATE, (int *)&pAiAttr->impAttr.samplerate);
   if (LUX_OK != ret)
       IMP_LOG_ERR(TAG, "LUX_AUDIO_IN_SAMPLE_RATE failed,error [0x%x]\n", ret);
    
   ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, LUX_AUDIO, LUX_AUDIO_OUT_TYPE, (int *)&pAoAttr->chnAttr.payloadType);
   if (LUX_OK != ret)
       IMP_LOG_ERR(TAG, "LUX_AUDIO_OUT_TYPE failed,error [0x%x]\n", ret);
   
   ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, LUX_AUDIO, LUX_AUDIO_OUT_SAMPLE_RATE, (int *)&pAoAttr->impAttr.samplerate);
   if (LUX_OK != ret)
       IMP_LOG_ERR(TAG, "LUX_AUDIO_OUT_SAMPLE_RATE failed,error [0x%x]\n", ret);
    
   ret = LUX_INIParse_GetBool(LUX_CONFIG_FILE, LUX_AUDIO, LUX_AUDIO_AEC, &g_audioAttr.bAecEnable);
   if (LUX_OK != ret)
       IMP_LOG_ERR(TAG, "LUX_AUDIO_AEC failed,error [0x%x]\n", ret);

   /* AUDIO IN */
    
   ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, LUX_AUDIO, LUX_AI_VOLUME, &pAiAttr->chnAttr.volume);
   if (LUX_OK != ret)
       IMP_LOG_ERR(TAG, "LUX_AI_VOLUME failed,error [0x%x]\n", ret);
    
   ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, LUX_AUDIO, LUX_AI_GAIN, &pAiAttr->chnAttr.gain);
   if (LUX_OK != ret)
       IMP_LOG_ERR(TAG, "LUX_AI_GAIN failed,error [0x%x]\n", ret);
   
   ret = LUX_INIParse_GetBool(LUX_CONFIG_FILE, LUX_AUDIO, LUX_AI_NS, &pAiAttr->chnAttr.bNsEnable);
   if (LUX_OK != ret)
       IMP_LOG_ERR(TAG, "LUX_AI_NS failed,error [0x%x]\n", ret);
    
   ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, LUX_AUDIO, LUX_AI_NS_LENEL, (int*)&pAiAttr->chnAttr.nsLevel);
   if (LUX_OK != ret)
       IMP_LOG_ERR(TAG, "LUX_AI_NS_LENEL failed,error [0x%x]\n", ret);
    
   ret = LUX_INIParse_GetBool(LUX_CONFIG_FILE, LUX_AUDIO, LUX_AI_HPF, &pAiAttr->chnAttr.bHpfEnable);
   if (LUX_OK != ret)
       IMP_LOG_ERR(TAG, "LUX_AI_HPF failed,error [0x%x]\n", ret);
    
   ret = LUX_INIParse_GetBool(LUX_CONFIG_FILE, LUX_AUDIO, LUX_AI_AGC, &pAiAttr->chnAttr.bAgcEnable);
   if (LUX_OK != ret)
       IMP_LOG_ERR(TAG, "LUX_AI_AGC failed,error [0x%x]\n", ret);
    
   ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, LUX_AUDIO, LUX_AI_AGC_LEVEL, &pAiAttr->chnAttr.agcParam.targetLevelDbfs);
   if (LUX_OK != ret)
       IMP_LOG_ERR(TAG, "LUX_AI_AGC_LEVEL failed,error [0x%x]\n", ret);

   ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, LUX_AUDIO, LUX_AI_AGC_GAIN, &pAiAttr->chnAttr.agcParam.compressionGaindB);
   if (LUX_OK != ret)
       IMP_LOG_ERR(TAG, "LUX_AI_AGC_GAIN failed,error [0x%x]\n", ret);

   /* AUDIO OUT */
   #ifdef BABY_PRO
   INT_X value = 0;
   ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "algorithm_config", LUX_AO_VOLUME, &value);
   if ((LUX_OK != ret) || (value > 10))
   {
       value = 9;
       char tmp[16] = {'\0'};
       sprintf(tmp, "%d", value);
       ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "algorithm_config", LUX_AO_VOLUME, tmp);
       if (LUX_OK != ret)
       {
           IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
       }
       ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "algorithm_config", LUX_AO_VOLUME, &value);
       if (LUX_OK != ret)
       {
           IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
       }
   }
   if(10 == value)
   {
       pAoAttr->chnAttr.volume = 48;
   }
   else
   {
       pAoAttr->chnAttr.volume = value * 5;
   }
   
   #else
   ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, LUX_AUDIO, LUX_AO_VOLUME, &pAoAttr->chnAttr.volume);
   if (LUX_OK != ret)
       IMP_LOG_ERR(TAG, "LUX_AO_VOLUME failed,error [0x%x]\n", ret);
   #endif
   
   ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, LUX_AUDIO, LUX_AO_GAIN, &pAoAttr->chnAttr.gain);
   if (LUX_OK != ret)
       IMP_LOG_ERR(TAG, "LUX_AO_GAIN failed,error [0x%x]\n", ret);
    
   ret = LUX_INIParse_GetBool(LUX_CONFIG_FILE, LUX_AUDIO, LUX_AO_HPF, &pAoAttr->chnAttr.bHpfEnable);
   if (LUX_OK != ret)
       IMP_LOG_ERR(TAG, "LUX_AO_HPF failed,error [0x%x]\n", ret);
  
   ret = LUX_INIParse_GetBool(LUX_CONFIG_FILE, LUX_AUDIO, LUX_AO_AGC, &pAoAttr->chnAttr.bAgcEnable);
   if (LUX_OK != ret)
       IMP_LOG_ERR(TAG, "LUX_AO_AGC failed,error [0x%x]\n", ret);

   ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, LUX_AUDIO, LUX_AO_AGC_LEVEL, &pAoAttr->chnAttr.agcParam.targetLevelDbfs);
   if (LUX_OK != ret)
       IMP_LOG_ERR(TAG, "LUX_AO_AGC_LEVEL failed,error [0x%x]\n", ret);

   ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, LUX_AUDIO, LUX_AO_AGC_GAIN, &pAoAttr->chnAttr.agcParam.compressionGaindB);
   if (LUX_OK != ret)
       IMP_LOG_ERR(TAG, "LUX_AO_AGC_GAIN failed,error [0x%x]\n", ret);

   ret = LUX_INIParse_GetBool(LUX_CONFIG_FILE, LUX_AI_CRY_SECTION, LUX_SLEEP_MODE, &sleepMode);
   
   if (LUX_OK != ret)
   IMP_LOG_ERR(TAG, "cry_onoff failed,error [0x%x]\n", ret);

   if(LUX_FALSE == sleepMode)
   {
       ret = LUX_INIParse_GetBool(LUX_CONFIG_FILE, LUX_AI_CRY_SECTION, LUX_AI_CRY_ONOFF, &pCry->bCryOpen);
       if (LUX_OK != ret)
           IMP_LOG_ERR(TAG, "cry_onoff failed,error [0x%x]\n", ret);

   }

   else /*隐私模式下不打开哭声检测*/
   {
       pCry->bCryOpen = LUX_FALSE;
   }
// #pragma endregion
 #endif   
    
    return LUX_OK;
}

/**
 * @description: 将参数写入到media配置文件中
 * @return 0
 */
INT_X LUX_AUDIO_WriteIni()
{
    return LUX_OK;
}

/******************************************************************
 *                                                                *
 *                         AI操作相关函数                          *
 *                                                                *
 ******************************************************************/

INT_X LUX_AI_GetParam()
{
    return LUX_OK;
}

INT_X LUX_AI_SetParam()
{
    return LUX_OK;
}

/**
 * @description: 获取 audio in 音频数据
 * @param [out] pFrame：音频帧结构
 * @return 0：成功 其他：错误码
 */
INT_X LUX_AI_GetStream(LUX_AUDIO_FRAME_ST *pFrame)
{
    if (NULL == pFrame)
    {
        IMP_LOG_ERR(TAG, "error!!! null ptr!\n");
        return LUX_AUDIO_NULL_PRT;
    }
    INT_X ret = LUX_ERR;
    INT_X devID = LUX_AUDIO_AI_DEV_ID;
    INT_X chnID = LUX_AUDIO_AI_CHAN_ID;
    IMPAudioFrame *pImpFrm = NULL;
    LUX_AUDIO_AI_ST *pAttr = &g_audioAttr.aiDevAttr;
    //MYWARN("pAttr->bInit:[%d] pAttr->chnAttr.bStart:[%d] \n", pAttr->bInit, pAttr->chnAttr.bStart);
    if (!pAttr->bInit || !pAttr->chnAttr.bStart)
        return LUX_OK;

    Thread_Mutex_Lock(&pAttr->mutex);
    do
    {
        ret = IMP_AI_PollingFrame(devID, chnID, 1000);
        if (ret != 0)
        {
            MYERROR("Audio Polling Frame Data error\n");
            ret = LUX_AUDIO_GET_STREAM_ERR;
            break;
        }
        /* 从全局拿帧缓存 */
        pImpFrm = &pAttr->chnFrame[pAttr->frmIdx % MAX_AUDIO_FRAME_NUM];
        pAttr->frmIdx++;

        memset(pImpFrm, 0, sizeof(IMPAudioFrame));
        ret = IMP_AI_GetFrame(devID, chnID, pImpFrm, BLOCK);
        if (ret != 0)
        {
            MYERROR("Audio Get Frame Data error\n");
            ret = LUX_AUDIO_GET_STREAM_ERR;
            break;
        }

        pFrame->type = pAttr->chnAttr.payloadType;
        pFrame->pData = pImpFrm->virAddr;
        pFrame->len = pImpFrm->len;
        pFrame->seq = pImpFrm->seq;
        pFrame->timeStamp = pImpFrm->timeStamp;
        pFrame->priv = (VOID_X *)pImpFrm;        

        if (LuxTestEnbale == 1)/*工厂测试使用，保存音频文件*/
        {
            static FILE* aiFp = NULL;
            static unsigned int nWriteNum = 0;
            if (NULL == aiFp)
            {
                aiFp = fopen("/tmp/audio_in_1.pcm", "wb");
            }
            if (NULL != aiFp && nWriteNum <= 3000 * 1024)
            {
                fwrite(pFrame->pData, 1, pFrame->len, aiFp);
                nWriteNum += pFrame->len;
                fflush(aiFp);
            }
        }
        else
        {
            if (access("/tmp/audiotest", 0) == 0)
            {
                static FILE* aiFp = NULL;
                static unsigned int nWriteNum = 0;
                if (NULL == aiFp)
                {
                    aiFp = fopen("/tmp/audio_in_1.pcm", "wb");
                }
                if (NULL != aiFp && nWriteNum <= 2000 * 1024)
                {
                    fwrite(pFrame->pData, 1, pFrame->len, aiFp);
                    nWriteNum += pFrame->len;
                    fflush(aiFp);
                }
            }
        }
    }
    while (0);

    Thread_Mutex_UnLock(&pAttr->mutex);

    return ret;
}

/**
 * @description: 释放 audio in 音频数据
 * @param [in] pFrame：帧结构
 * @return 0：成功 其他：错误码
 */
INT_X LUX_AI_ReleaseStream(LUX_AUDIO_FRAME_ST *pFrame)
{
    if (NULL == pFrame || NULL == pFrame->priv)
    {
        IMP_LOG_ERR(TAG, "LUX_AI_ReleaseStream error null ptr!\n");
        return LUX_AUDIO_NULL_PRT;
    }
    INT_X ret = LUX_ERR;
    INT_X devID = LUX_AUDIO_AI_DEV_ID;
    INT_X chnID = LUX_AUDIO_AI_CHAN_ID;

    /* Step 8: release the audio record frame. */
    ret = IMP_AI_ReleaseFrame(devID, chnID, (IMPAudioFrame *)pFrame->priv);
    if (ret != 0)
    {
        IMP_LOG_ERR(TAG, "Audio release frame data error\n");
        return LUX_AUDIO_RLS_STREAM_ERR;
    }

    return ret;
}


/**
 * @description: 开启 audio in 算法，包括降噪、滤波、声音增强
 * @param [in] devId：ai 设备ID
 * @param [in] pChnAttr：ai 通道属性
 * @return 0：成功 其他：错误码
 */
static INT_X LUX_AI_StartgAlg(UINT_X devId, LUX_AI_CHN_ATTR_ST *pChnAttr)
{
    if (NULL == pChnAttr)
    {
        MYERRORING("ERR: null ptr\n");
        return LUX_AUDIO_NULL_PRT;
    }
    INT_X ret = LUX_OK;
    IMPAudioIOAttr *pImpAttr = &g_audioAttr.aiDevAttr.impAttr;

    /* 回声抑制功能与此函数下的功能互斥 */
    if (g_audioAttr.bAecEnable)
    {
        MYWARNING("config alg: mutex with aec!\n");
        return LUX_OK;
    }
    
    /* 开启增益功能 */
    if (pChnAttr->bAgcEnable)
    {
        IMPAudioAgcConfig impAgcParam;

        impAgcParam.TargetLevelDbfs = pChnAttr->agcParam.targetLevelDbfs;
        impAgcParam.CompressionGaindB = pChnAttr->agcParam.compressionGaindB;
        ret = IMP_AI_EnableAgc(pImpAttr, impAgcParam);
        if (ret != 0)
        {
            MYERRORING("enable audio agc error.\n");
            return LUX_AUDIO_CONF_ALG_ERR;
        }
    }

    /* 开启降噪功能 */
    if (pChnAttr->bNsEnable)
    {
        ret = IMP_AI_EnableNs(pImpAttr, pChnAttr->nsLevel);
        if (ret != 0)
        {
            MYERRORING("enable audio ns error.\n");
            return LUX_AUDIO_CONF_ALG_ERR;
        }
    }
    /* 开启高通滤波功能 */
    if (pChnAttr->bHpfEnable)
    {
        ret = IMP_AI_EnableHpf(pImpAttr);
        if (ret != 0)
        {
            MYERRORING("enable audio hpf error.\n");
            return LUX_AUDIO_CONF_ALG_ERR;
        }
    }

    return ret;
}

/**
 * @description: 关闭 audio in 算法，包括降噪、滤波、声音增强
 * @param [in] devId：ai 设备ID
 * @param [in] pChnAttr：ai 通道属性
 * @return 0：成功 其他：错误码
 */
static INT_X LUX_AI_StopAlg(UINT_X devId, LUX_AI_CHN_ATTR_ST *pChnAttr)
{
    if (NULL == pChnAttr)
    {
        IMP_LOG_WARN(TAG, "ERR: null ptr\n");
        return LUX_AUDIO_NULL_PRT;
    }
    INT_X ret = LUX_OK;

    /* 回声抑制功能与此函数下的功能互斥 */
    if (g_audioAttr.bAecEnable)
    {
        IMP_LOG_WARN(TAG, "config alg: mutex with aec!\n");
        return LUX_OK;
    }

    /* 开启降噪功能 */
    if (pChnAttr->bNsEnable)
    {
        ret = IMP_AI_DisableNs();
        if (ret != 0)
        {
            IMP_LOG_INFO(TAG, "enable audio ns error.\n");
            return LUX_AUDIO_CONF_ALG_ERR;
        }
    }

    /* 开启高通滤波功能 */
    if (pChnAttr->bHpfEnable)
    {
        ret = IMP_AI_DisableHpf();
        if (ret != 0)
        {
            IMP_LOG_INFO(TAG, "enable audio ns error.\n");
            return LUX_AUDIO_CONF_ALG_ERR;
        }
    }

    /* 开启声音增强功能 */
    if (pChnAttr->bAgcEnable)
    {
        ret = IMP_AI_DisableAgc();
        if (ret != 0)
        {
            IMP_LOG_INFO(TAG, "enable audio ns error.\n");
            return LUX_AUDIO_CONF_ALG_ERR;
        }
    }

    return ret;
}

/**
 * @description: 开启 audio in 模块
 * @return 0：成功 其他：错误码
 */
INT_X LUX_AI_Start()
{
    INT_X ret = LUX_ERR;
    INT_X devID = LUX_AUDIO_AI_DEV_ID;
    INT_X chnID = LUX_AUDIO_AI_CHAN_ID;
    LUX_AUDIO_AI_ST *pAttr = &g_audioAttr.aiDevAttr;

    if (pAttr->chnAttr.bStart)
    {
        IMP_LOG_ERR(TAG, "Audio Start repeat!\n");
        return LUX_OK;
    }

    Thread_Mutex_Lock(&pAttr->mutex);

    do {
        /* Step 4: enable AI channel. */
        ret = IMP_AI_EnableChn(devID, chnID);
        if (ret != LUX_OK)
        {
            MYERRORING("Audio Record enable channel failed \n");
            ret = LUX_AUDIO_CHAN_START_ERR;
            break;
        }

        /* start agc ns hpf */
        ret = LUX_AI_StartgAlg(devID, &pAttr->chnAttr);
        if (ret != LUX_OK)
        {
            MYERRORING("Audio Record Set Gain failed\n");
            ret = LUX_AUDIO_CHAN_START_ERR;
            break;
        }

        /* Enable Aec */
        if (g_audioAttr.bAecEnable)
        {
            ret = IMP_AI_EnableAec(devID, chnID, LUX_AUDIO_AO_DEV_ID, LUX_AUDIO_AO_CHAN_ID);
            if (ret != 0)
            {
                MYERROR("Audio enable aec failed\n");
            }
        }

        /* Step 5: Set audio channel volume. */
        ret = IMP_AI_SetVol(devID, chnID, pAttr->chnAttr.volume);
        if (ret != LUX_OK)
        {
            MYERROR("Audio Record set volume failed\n");
        }
        MYTRACE("Set AI Volume, ret:%d, vol: %d\n", ret, pAttr->chnAttr.volume);      

        ret = IMP_AI_SetGain(devID, chnID, pAttr->chnAttr.gain);
        if (ret != LUX_OK)
        {
            MYERROR("Audio Record Set Gain failed\n");
        }

        pAttr->chnAttr.bStart = LUX_TRUE;
    } while (0);

    Thread_Mutex_UnLock(&pAttr->mutex);

    return ret;
}

/**
 * @description: 停止 audio in 模块
 * @return 0：成功 其他：错误码
 */
INT_X LUX_AI_Stop()
{
    if (!g_audioAttr.aiDevAttr.chnAttr.bStart)
    {
        Thread_Mutex_Lock(&g_audioAttr.aiDevAttr.mutex);

        LUX_AI_StopAlg(g_audioAttr.aiDevAttr.devId, &g_audioAttr.aiDevAttr.chnAttr);

        if (IMP_AI_DisableChn(g_audioAttr.aiDevAttr.devId, g_audioAttr.aiDevAttr.chnAttr.chnId) != LUX_OK)
        {
            IMP_LOG_ERR(TAG, "Audio channel disable error\n");
        }

        g_audioAttr.aiDevAttr.chnAttr.bStart = LUX_FALSE;

        Thread_Mutex_UnLock(&g_audioAttr.aiDevAttr.mutex);
    }

    return LUX_OK;
}

/**
 * @description: 初始化 audio in 模块
 * @return 0：成功 其他：错误码
 */
INT_X LUX_AI_Init()
{
    INT_X ret = LUX_ERR;
    LUX_AUDIO_AI_ST *pAttr = NULL;

    pAttr = &g_audioAttr.aiDevAttr;
    pAttr->impAttr.numPerFrm = LUX_AUDIO_NUM_PER_FRM;//pAttr->impAttr.samplerate / LUX_AUDIO_FRAMERATE;

    printf("IMP_AI_SetPubAttr devid:%d \n", pAttr->devId);
    ret = IMP_AI_SetPubAttr(pAttr->devId, &pAttr->impAttr);
    if (ret != LUX_OK)
    {
        IMP_LOG_ERR(TAG, "set ai %d attr err: %d\n", pAttr->devId, ret);
        return LUX_AUDIO_INIT_ERR;
    }

    ret = IMP_AI_GetPubAttr(pAttr->devId, &pAttr->impAttr);
    if (ret != 0)
    {
        IMP_LOG_ERR(TAG, "get ai %d attr err: %d\n", pAttr->devId, ret);
        return LUX_AUDIO_INIT_ERR;
    }

    IMP_LOG_INFO(TAG, "Audio In GetPubAttr samplerate : %d\n", pAttr->impAttr.samplerate);
    IMP_LOG_INFO(TAG, "Audio In GetPubAttr   bitwidth : %d\n", pAttr->impAttr.bitwidth);
    IMP_LOG_INFO(TAG, "Audio In GetPubAttr  soundmode : %d\n", pAttr->impAttr.soundmode);
    IMP_LOG_INFO(TAG, "Audio In GetPubAttr     frmNum : %d\n", pAttr->impAttr.frmNum);
    IMP_LOG_INFO(TAG, "Audio In GetPubAttr  numPerFrm : %d\n", pAttr->impAttr.numPerFrm);
    IMP_LOG_INFO(TAG, "Audio In GetPubAttr     chnCnt : %d\n", pAttr->impAttr.chnCnt);

    /* Step 2: enable AI device. */
    ret = IMP_AI_Enable(pAttr->devId);
    if (ret != 0)
    {
        IMP_LOG_ERR(TAG, "enable ai %d err\n", pAttr->devId);
        return LUX_AUDIO_INIT_ERR;
    }

    /* Step 3: set audio channel attribute of AI device. */
    ret = IMP_AI_SetChnParam(pAttr->devId, pAttr->chnAttr.chnId, &pAttr->aiParam);
    if (ret != 0)
    {
        IMP_LOG_ERR(TAG, "set ai %d channel %d attr err: %d\n", pAttr->devId, pAttr->chnAttr.chnId, ret);
        return LUX_AUDIO_INIT_ERR;
    }

    /* Enable Aec */
    if (g_audioAttr.bAecEnable)
    {
        ret = IMP_AI_EnableAec(pAttr->devId, pAttr->chnAttr.chnId, LUX_AUDIO_AO_DEV_ID, LUX_AUDIO_AO_CHAN_ID);
        if (ret != 0)
        {
            IMP_LOG_ERR(TAG, "Audio enable aec failed\n");
        }
    }

#if 1 //for test
    IMPAudioIChnParam chnParam;
    memset(&chnParam, 0x0, sizeof(chnParam));
    ret = IMP_AI_GetChnParam(pAttr->devId, pAttr->chnAttr.chnId, &chnParam);
    if (ret != 0)
    {
        IMP_LOG_ERR(TAG, "get ai %d channel %d attr err: %d\n", pAttr->devId, pAttr->chnAttr.chnId, ret);
        return LUX_AUDIO_INIT_ERR;
    }

    IMP_LOG_INFO(TAG, "Audio In GetChnParam usrFrmDepth : %d\n", chnParam.usrFrmDepth);
#endif

    g_audioAttr.aiDevAttr.bInit = LUX_TRUE;

    return ret;
}

/**
 * @description: 退出 audio in 模块
 * @return 0：成功 其他：错误码
 */
VOID_X LUX_AI_Exit()
{
    INT_X ret = LUX_ERR;

    if (g_audioAttr.aiDevAttr.bInit)
    {
        if (g_audioAttr.bAecEnable)
        {
            ret = IMP_AI_DisableAec(LUX_AUDIO_AI_DEV_ID, LUX_AUDIO_AI_CHAN_ID);
            if (ret != 0)
            {
                IMP_LOG_ERR(TAG, "Audio enable aec failed\n");
            }
        }

        /* disable the audio channel. */
        ret = IMP_AI_Disable(g_audioAttr.aiDevAttr.devId);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "Audio channel disable error\n");
            return ;
        }

        g_audioAttr.aiDevAttr.bInit = LUX_FALSE;
    }

    return ;
}
/******************************************************************
 *                                                                *
 *                         AO操作相关函数                          *
 *                                                                *
 ******************************************************************/
/**
 * @description: 开启 audio out 算法，包括滤波、声音增强
 * @param [in] devId：ao 设备ID
 * @param [in] pChnAttr：ao 通道属性
 * @return 0：成功 其他：错误码
 */
static INT_X LUX_AO_StartAlg(UINT_X devId, LUX_AO_CHN_ATTR_ST *pChnAttr)
{
    if (NULL == pChnAttr)
    {
        IMP_LOG_WARN(TAG, "ERR: null ptr\n");
        return LUX_AUDIO_NULL_PRT;
    }
    INT_X ret = LUX_OK;
    IMPAudioIOAttr *pImpAttr = &g_audioAttr.aiDevAttr.impAttr;

    /* 回声抑制功能与此函数下的功能互斥 */
    if (g_audioAttr.bAecEnable)
    {
        IMP_LOG_WARN(TAG, "config alg: mutex with aec!\n");
        return LUX_OK;
    }

    /* 开启声音增强功能 */
    if (pChnAttr->bAgcEnable)
    {
        IMPAudioAgcConfig impAgcParam;

        impAgcParam.TargetLevelDbfs = pChnAttr->agcParam.targetLevelDbfs;
        impAgcParam.CompressionGaindB = pChnAttr->agcParam.compressionGaindB;
        ret = IMP_AO_EnableAgc(pImpAttr, impAgcParam);
        if (ret != 0)
        {
            IMP_LOG_INFO(TAG, "enable audio ns error.\n");
            return LUX_AUDIO_CONF_ALG_ERR;
        }
    }

    /* 开启高通滤波功能 */
    if (pChnAttr->bHpfEnable)
    {
        ret = IMP_AO_EnableHpf(pImpAttr);
        if (ret != 0)
        {
            IMP_LOG_INFO(TAG, "enable audio ns error.\n");
            return LUX_AUDIO_CONF_ALG_ERR;
        }
    }

    return ret;
}

/**
 * @description: 停止 audio out 算法
 * @param [in] devId：ao 设备ID
 * @param [in] pChnAttr：ao 通道属性
 * @return 0：成功 其他：错误码
 */
static INT_X LUX_AO_StopAlg(UINT_X devId, LUX_AO_CHN_ATTR_ST *pChnAttr)
{
    if (NULL == pChnAttr)
    {
        IMP_LOG_WARN(TAG, "ERR: null ptr\n");
        return LUX_AUDIO_NULL_PRT;
    }
    INT_X ret = LUX_OK;

    /* 回声抑制功能与此函数下的功能互斥 */
    if (g_audioAttr.bAecEnable)
    {
        IMP_LOG_WARN(TAG, "config alg: mutex with aec!\n");
        return LUX_OK;
    }

    /* 开启高通滤波功能 */
    if (pChnAttr->bHpfEnable)
    {
        ret = IMP_AO_DisableHpf();
        if (ret != 0)
        {
            IMP_LOG_INFO(TAG, "enable audio ns error.\n");
            return LUX_AUDIO_CONF_ALG_ERR;
        }
    }

    /* 开启声音增强功能 */
    if (pChnAttr->bAgcEnable)
    {
        ret = IMP_AO_DisableAgc();
        if (ret != 0)
        {
            IMP_LOG_ERR(TAG, "enable audio ns error.\n");
            return LUX_AUDIO_CONF_ALG_ERR;
        }
    }

    return ret;
}

/**
 * @description: 开启 audio out
 * @return 0：成功 其他：错误码
 */
INT_X LUX_AO_Start()
{
    INT_X ret = LUX_ERR;
    INT_X devID = LUX_AUDIO_AO_DEV_ID;
    INT_X chnID = LUX_AUDIO_AO_CHAN_ID;
    LUX_AUDIO_AO_ST *pAttr = &g_audioAttr.aoDevAttr;

    if (pAttr->chnAttr.bStart)
    {
        IMP_LOG_ERR(TAG, "Audio Start repeat!\n");
        return LUX_OK;
    }
    if(0 == pAttr->chnAttr.volume)
    {
        IMP_LOG_ERR(TAG, "Audio not Start. volume = 0!\n");
        return LUX_OK;
    }
    Thread_Mutex_Lock(&pAttr->mutex);

    do
    {
        /* Step 4: enable AI channel. */
        ret = IMP_AO_EnableChn(devID, chnID);
        if (ret != LUX_OK)
        {
            IMP_LOG_ERR(TAG, "Audio Record enable channel failed\n");
            ret = LUX_AUDIO_CHAN_START_ERR;
            break;
        }

        /* 开启ao算法 */
        ret = LUX_AO_StartAlg(devID, &pAttr->chnAttr);
        if (ret != LUX_OK)
        {
            IMP_LOG_ERR(TAG, "Audio Record Set Gain failed\n");
        }

        /* Step 5: Set audio channel volume. */
        ret = IMP_AO_SetVol(devID, chnID, pAttr->chnAttr.volume);
        if (ret != LUX_OK)
        {
            IMP_LOG_ERR(TAG, "Audio Record set volume failed\n");
        }

        ret = IMP_AO_SetGain(devID, chnID, pAttr->chnAttr.gain);
        if (ret != LUX_OK)
        {
            IMP_LOG_ERR(TAG, "Audio Record Set Gain failed\n");
        }

        /* 开启扬声器io，不可提前，此处解决设备启动时的bobo声 */
        ret = LUX_Hal_Misc_SpeakerOnOff(LUX_TRUE);
        if (ret != LUX_OK)
        {
            IMP_LOG_ERR(TAG, "LUX_Hal_Misc_SpeakerOnOff failed\n");
        }

        pAttr->chnAttr.bStart = LUX_TRUE;
    } while (0);

    Thread_Mutex_UnLock(&pAttr->mutex);

    return ret;
}

/**
 * @description: 停止 audio out
 * @return 0：成功 其他：错误码
 */
INT_X LUX_AO_Stop()
{
    INT_X ret = LUX_ERR;
    LUX_AUDIO_AO_ST *pAttr = &g_audioAttr.aoDevAttr;

    if (pAttr->chnAttr.bStart)
    {
        Thread_Mutex_Lock(&pAttr->mutex);

        LUX_AO_StopAlg(pAttr->devId, &pAttr->chnAttr);

        /* AO 退出前清掉通道中缓存的数据 */
        ret = IMP_AO_ClearChnBuf(pAttr->devId, pAttr->chnAttr.chnId);
        if (ret != 0)
        {
            IMP_LOG_ERR(TAG, "IMP_AO_ClearChnBuf error\n");
        }

        ret = IMP_AO_DisableChn(pAttr->devId, pAttr->chnAttr.chnId);
        if (ret != LUX_OK)
        {
            IMP_LOG_ERR(TAG, "Audio channel disable error\n");
        }
        pAttr->frmSeq = 0;
        pAttr->chnAttr.bStart = LUX_FALSE;

        Thread_Mutex_UnLock(&pAttr->mutex);
    }

    return LUX_OK;
}
/**
 * @description: 用于测试播报提示音时有未播放完的语言
 * 
 * @return INT_X 
 */
INT_X LUX_AO_ClearBuffer()
{
    INT_X ret = LUX_ERR;
    LUX_AUDIO_AO_ST *pAttr = &g_audioAttr.aoDevAttr;
    IMPAudioOChnState status;
    if (!pAttr->bInit || !pAttr->chnAttr.bStart)
    {
        IMP_LOG_ERR(TAG, "bInit:%d bStart:%d\n", pAttr->bInit, pAttr->chnAttr.bStart);
        return LUX_ERR;
    }
    ret = IMP_AO_ClearChnBuf(pAttr->devId, pAttr->chnAttr.chnId);
    if (ret != 0)
    {
        IMP_LOG_ERR(TAG, "IMP_AO_ClearChnBuf error\n");
    }
    IMP_AO_QueryChnStat(pAttr->devId,pAttr->chnAttr.chnId,&status);
    printf("AO buffer status status.chnTotalNum = %d ,status.chnBusyNum = %d ,status.chnFreeNum =%d \n",status.chnTotalNum,status.chnBusyNum,status.chnFreeNum);

    return ret;
}

//#ifdef BABY_PRO
INT_X LUX_AO_Get_Volume()
{
    INT_X ret = LUX_ERR;
    // INT_X value = 0;
    // LUX_AUDIO_AO_ST *pAttr = &g_audioAttr.aoDevAttr;

    // ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "media_config_audio", "ao_volume", &value);
    // if (LUX_OK != ret)
    //     IMP_LOG_ERR(TAG, "ao_volume failed,error [0x%x]\n", ret);
    if(10 ==  g_configData->media_config.audio_config.ai_volume)
    {
         g_configData->media_config.audio_config.ai_volume = 48;

    }
    else
    {
         g_configData->media_config.audio_config.ai_volume =  g_configData->media_config.audio_config.ai_volume * 5;
    }
    printf("%s:%d Get AO Volume: %d\n",__func__,__LINE__, g_configData->media_config.audio_config.ai_volume);

    return  g_configData->media_config.audio_config.ai_volume;
}

INT_X LUX_AO_Set_Vloume(INT_X value)
{
    INT_X ret = LUX_ERR;
    INT_X devID = LUX_AUDIO_AO_DEV_ID;
    INT_X chnID = LUX_AUDIO_AO_CHAN_ID;
    LUX_AUDIO_AO_ST *pAttr = &g_audioAttr.aoDevAttr;
    char tmp[16] = { 0 };

    if(value <= 0) 
    {
        value = 0;
        ret = LUX_AO_Stop();
        if (ret != LUX_OK)
        {
            IMP_LOG_ERR(TAG, "Audio Stop failed\n");
        }
        //return ret;
    }
    else
    {
        if(value >10) value = 10;
        if(10 == value )
        {
            pAttr->chnAttr.volume = 48;
        }
        else
        {
            pAttr->chnAttr.volume = value * 5;
        }
        ret = LUX_AO_Start();
        if (ret != LUX_OK)
        {
            IMP_LOG_ERR(TAG, "Audio Start failed\n");
            return ret;
        }
        //Thread_Mutex_Lock(&pAttr->mutex);
        ret = IMP_AO_SetVol(devID,chnID,pAttr->chnAttr.volume);
        if (ret != LUX_OK)
        {
            IMP_LOG_ERR(TAG, "Audio Record Get Volume failed\n");
        }
        printf("========*********======= \n");
        printf("========*********=======%s:%d Set AO Volume: %d\n",__func__,__LINE__,pAttr->chnAttr.volume);
        printf("========*********======= \n");

        //Thread_Mutex_UnLock(&pAttr->mutex);
    }
    g_configData->media_config.audio_config.ao_volume = value;
    // sprintf(tmp, "%d", value);//将整数转换为字符串，范围是0～50,两位数
    // ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "media_config_audio", "ao_volume", tmp);
    // if (0 != ret)
    // {
    //     IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
    // }
    return ret;
}
//#endif
/**
 * @description: 初始化 audio out 资源
 * @return 0：成功 其他：错误码
 */
INT_X LUX_AO_Init()
{
    INT_X ret = LUX_ERR;
    LUX_AUDIO_AO_ST *pAttr = &g_audioAttr.aoDevAttr;
    IMPAudioIOAttr *pImpAttr = &g_audioAttr.aoDevAttr.impAttr;

    pAttr->pPcmBuf = malloc(LUX_AO_PCM_BUF_LEN);
    if (NULL == pAttr->pPcmBuf)
    {
        IMP_LOG_ERR(TAG, "malloc memory size:%d failed!\n", LUX_AO_PCM_BUF_LEN);
        return LUX_AUDIO_INIT_ERR;
    }

    pImpAttr->numPerFrm = LUX_AUDIO_NUM_PER_FRM;//pImpAttr->samplerate / LUX_AUDIO_FRAMERATE;
    ret = IMP_AO_SetPubAttr(pAttr->devId, pImpAttr);
    if (ret != LUX_OK)
    {
        IMP_LOG_ERR(TAG, "set ao %d attr err: %d\n", pAttr->devId, ret);
        return LUX_AUDIO_INIT_ERR;
    }

    ret = IMP_AO_GetPubAttr(pAttr->devId, pImpAttr);
    if (ret != 0)
    {
        IMP_LOG_ERR(TAG, "get ao %d attr err: %d\n", pAttr->devId, ret);
        return LUX_AUDIO_INIT_ERR;
    }

    IMP_LOG_INFO(TAG, "Audio OUT GetPubAttr samplerate : %d\n", pImpAttr->samplerate);
    IMP_LOG_INFO(TAG, "Audio OUT GetPubAttr   bitwidth : %d\n", pImpAttr->bitwidth);
    IMP_LOG_INFO(TAG, "Audio OUT GetPubAttr  soundmode : %d\n", pImpAttr->soundmode);
    IMP_LOG_INFO(TAG, "Audio OUT GetPubAttr     frmNum : %d\n", pImpAttr->frmNum);
    IMP_LOG_INFO(TAG, "Audio OUT GetPubAttr  numPerFrm : %d\n", pImpAttr->numPerFrm);
    IMP_LOG_INFO(TAG, "Audio OUT GetPubAttr     chnCnt : %d\n", pImpAttr->chnCnt);

    /* Step 2: enable AI device. */
    ret = IMP_AO_Enable(pAttr->devId);
    if (ret != 0)
    {
        IMP_LOG_ERR(TAG, "enable ai %d err\n", pAttr->devId);
        return LUX_AUDIO_INIT_ERR;
    }
    pAttr->bInit = LUX_TRUE;

    return ret;
}

/**
 * @description: 退出 audio out
 * @return 0：成功 其他：错误码
 */
VOID_X LUX_AO_Exit()
{
    if (g_audioAttr.aoDevAttr.bInit)
    {
        /* disable the audio channel. */
        if (LUX_OK != IMP_AO_Disable(g_audioAttr.aiDevAttr.devId))
        {
            IMP_LOG_ERR(TAG, "Audio channel disable error\n");
            return;
        }

        free(g_audioAttr.aoDevAttr.pPcmBuf);
        g_audioAttr.aoDevAttr.bInit = LUX_FALSE;
    }

    return;
}

/**
 * @description: 送帧到 audio out 输出
 * @param [in] pFrame：帧结构
 * @return 0：成功 其他：错误码
 */
INT_X LUX_AO_PutStream(LUX_AUDIO_FRAME_ST *pFrame)
{
    if (NULL == pFrame)
    {
        IMP_LOG_ERR(TAG, "NULL PTR!\n");
        return LUX_AUDIO_NULL_PRT;
    }
    INT_X ret = LUX_ERR;
    IMPAudioFrame frm;
    INT_X devID = LUX_AUDIO_AO_DEV_ID;
    INT_X chnID = LUX_AUDIO_AO_CHAN_ID;
    LUX_AUDIO_AO_ST *pAttr = &g_audioAttr.aoDevAttr;
    PVOID_X pData = NULL;
    INT_X   dataLen = 0;

    if (!pAttr->bInit || !pAttr->chnAttr.bStart)
    {
        IMP_LOG_ERR(TAG, "bInit:%d bStart:%d\n", pAttr->bInit, pAttr->chnAttr.bStart);
        return LUX_AUDIO_SND_FRAME_ERR;
    }
#if 0//cxj test
#ifdef LUX_AO_DUMP_DATA
    static FILE *aoFp = NULL;
    
    if (NULL == aoFp)
    {
        aoFp = fopen("/tmp/audio_out_0.pcm", "w+");
    }
    if (NULL != aoFp)
    {
        fwrite(pFrame->pData, 1, pFrame->len, aoFp);
        fflush(aoFp);
    }
#endif

    
    if (gcustomed_action_get == LUX_AUDIO_RECORD_START)
    {
        int ret = getTime_getS() - gstart_record_time;
        if (ret >= LUX_CRADLESONG_DURATION)
        {
            CHAR_X tmp_str[2] = {0};
            tmp_str[0] = '0';
            IPC_APP_set_customed_voice_action(tmp_str);

            return ret;
        }

        if (NULL == cradlesongFp)
        {
            cradlesongFp = fopen("/tmp/Mycradlesong.pcm", "w+");
        }

        if (NULL != cradlesongFp)
        {
            fwrite(pFrame->pData, 1, pFrame->len, cradlesongFp);
            fflush(cradlesongFp);
        }
        return ret;
    }
#endif
    Thread_Mutex_Lock(&pAttr->mutex);

    if (LUX_G711U == pAttr->chnAttr.payloadType)
    {
        pData = pAttr->pPcmBuf;
        dataLen = LUX_AUDIO_G711u2PCM(pFrame->pData, pData, pFrame->len);
    }
    else if (LUX_G711A == pAttr->chnAttr.payloadType)
    {
        pData = pAttr->pPcmBuf;
        dataLen = LUX_AUDIO_G711a2PCM(pFrame->pData, pData, pFrame->len);
    }
    else
    {
        pData = pFrame->pData;
        dataLen = pFrame->len;
    }

    memset(&frm, 0, sizeof(frm));
    frm.virAddr = pData;
    frm.len = dataLen;
    frm.seq = pAttr->frmSeq++;
    frm.timeStamp = getTime_getUs();
    frm.bitwidth = pAttr->impAttr.bitwidth;
    frm.soundmode = pAttr->impAttr.soundmode;

    ret = IMP_AO_SendFrame(devID, chnID, &frm, BLOCK);
    if (ret != 0)
    {
        IMP_LOG_ERR(TAG, "send Frame Data error\n");
        ret = LUX_AUDIO_SND_FRAME_ERR;
    }

    Thread_Mutex_UnLock(&pAttr->mutex);

    return ret;
}



/******************************************************************
 *                                                                *
 *                         音频模块相关函数                        *
 *                                                                *
 ******************************************************************/
/**
 * @description: 获取音频模块参数
 * @return 0：成功 其他：错误码
 */
INT_X LUX_AUDIO_GetParam(LUX_AUDIO_PARAM_ST *pParam)
{
    if (NULL == pParam)
    {
        IMP_LOG_ERR(TAG, "ERROR: null ptr!\n");
        return LUX_AUDIO_NULL_PRT;
    }
    IMPAudioIOAttr *pImpAttr = &g_audioAttr.aiDevAttr.impAttr;

    Thread_Mutex_Lock(&g_audioAttr.mutex);

    pParam->bitWidth   = pImpAttr->bitwidth;
    pParam->sampleRate = pImpAttr->samplerate;
    pParam->soundMode  = pImpAttr->soundmode;
    pParam->fps        = LUX_AUDIO_FRAMERATE;//pImpAttr->samplerate / pImpAttr->numPerFrm;
    pParam->type = g_audioAttr.aoDevAttr.chnAttr.payloadType;

    Thread_Mutex_UnLock(&g_audioAttr.mutex);

    return LUX_OK;
}



typedef struct
{
    //IPC_APP_AUDIO_BEEP_TYPE type;
    char name[64];
    void *pData;
    unsigned int size;  //文件大小
    int  len;           //读到的数据大小
    LUX_AUDIO_PAYLOAD_TYPE encType;
} IPC_APP_NURSERY_MEMORY;

IPC_APP_NURSERY_MEMORY g_audio_file_data;
IPC_APP_NURSERY_MEMORY* pg_audio_data = &g_audio_file_data;

unsigned long LUX_SYS_GetFileSize(const char *filename)  
{  
    struct stat buf1;  
    if(stat(filename, &buf1)<0)  
    {  
        return 0;  
    }
    return (unsigned long)buf1.st_size;
}


static void* thread_play_file(void* arg)
{
    LUX_AUDIO_FRAME_ST aoStream;
    UINT_X loopTimes = 0, len = 0;
    //LUX_AUDIO_PAYLOAD_TYPE encType;
    void *pBuf = NULL;
    int i = 0;
    char* path = arg;
    //#define PATH "/tmp/test.pcm"
    #define NURSERY_AUDIO_PCM_BYTES_PER_PACK 320
    sleep(1);
    strncpy(pg_audio_data->name, path, strlen(path));
    pg_audio_data->size = LUX_SYS_GetFileSize(path);
    FILE* fp = fopen(pg_audio_data->name, "r");
    if(fp == NULL)
    {
        return (void*)0;
    }
    pg_audio_data->pData = malloc(pg_audio_data->size);
    pg_audio_data->len = fread(pg_audio_data->pData, 1, pg_audio_data->size, fp);
    pg_audio_data->encType = LUX_PCM;//测试结果对应LUX_AUDIO_PAYLOAD_TYPE是0
    fclose(fp);

    pBuf = pg_audio_data->pData;
    len = pg_audio_data->len;
    loopTimes = len / NURSERY_AUDIO_PCM_BYTES_PER_PACK;

    printf("start the song, loopTimes = %d\n", loopTimes);

    memset(&aoStream, 0, sizeof(aoStream));
    for (i = 0; i < loopTimes; i++)
    {
        aoStream.pData = (pBuf + NURSERY_AUDIO_PCM_BYTES_PER_PACK * i);
        aoStream.len = NURSERY_AUDIO_PCM_BYTES_PER_PACK;
        aoStream.type = LUX_PCM;
        LUX_AO_PutStream(&aoStream);
    }
    free(pg_audio_data->pData);
    return (void*)0;
}

INT_X LUX_AO_PlayFile(const char* path)
{
    pthread_t live_audio_thid;
    pthread_create(&live_audio_thid, NULL, thread_play_file, path);
    pthread_detach(live_audio_thid);
}

/**
 * @description: 配置音频模块参数
 * @return 0：成功 其他：错误码
 */
INT_X LUX_AUDIO_SetParam(LUX_AUDIO_PARAM_ST *pParam)
{
    if (NULL == pParam)
    {
        IMP_LOG_ERR(TAG, "ERROR: null ptr!\n");
        return LUX_AUDIO_NULL_PRT;
    }
    INT_X ret = LUX_ERR;

    Thread_Mutex_Lock(&g_audioAttr.mutex);

    //todo

    Thread_Mutex_UnLock(&g_audioAttr.mutex);

    return ret;
}

INT_X LUX_AUDIO_Init()
{
    //printf("setp -1");
    INT_X ret = LUX_ERR;

    /* 读取配置文件中的配置参数 */
     LUX_AUDIO_ReadIni();

#if 0 //T40移植暂时屏蔽  humy
    /* 开启扬声器io前，去使能io，start时再开启，此处解决设备启动时的bobo声 */
    ret = LUX_Hal_Misc_SpeakerOnOff(LUX_FALSE);
    if (ret != LUX_OK)
    {
        IMP_LOG_ERR(TAG, "LUX_Hal_Misc_SpeakerOnOff failed\n");
    }
#endif

    /* AUDIO IN INIT */
    ret = LUX_AI_Init();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "ERROR: LUX_AUDIO_Init!\n");
        return LUX_AUDIO_INIT_ERR;
    }

#if 0 //T40移植暂时屏蔽  humy
    /* AUDIO OUT INIT */
    ret = LUX_AO_Init();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "ERROR: LUX_AUDIO_Init!\n");
        return LUX_AUDIO_INIT_ERR;
    }
#endif

    g_audioAttr.bInit = LUX_TRUE;

    return ret;
}

VOID_X LUX_AUDIO_Exit()
{
    if (g_audioAttr.bInit)
    {
        /* 退出ai模块 */
        LUX_AI_Exit();

        /* 退出ao模块 */
        LUX_AO_Exit();

        g_audioAttr.bInit = LUX_FALSE;
    }

    return ;
}

INT_X LUX_AI_SetCryDet(BOOL_X bOpen)
{    
    //Thread_Mutex_Lock(&g_cryAttr.mutex);

    do {
        g_cryAttr.bCryOpen = bOpen;

    } while(0);

    //Thread_Mutex_UnLock(&g_cryAttr.mutex);

    return LUX_OK;
}

/**
 * @description: 哭声检测处理
 * @param [in] interface：算法handle
 * @param [in] pFrame：音频帧结
 * @param [out] pResult：吐出算法结果
 * @return 0：成功 其他：错误码
 */
static INT_X LUX_AI_CryDetProc(IMPIVSInterface *interface, IMPAudioFrame *pFrame, void **ppResult)
{
    INT_X ret = LUX_OK;

    if (interface->preProcessSync && ((ret = interface->preProcessSync(interface, (IMPFrameInfo*)pFrame)) < 0))
    {
        IMP_LOG_ERR(TAG, "inteface->preProcessSync failed,ret=%d preProcessSync:%p\n", ret, interface->preProcessSync);
        return -1;
    }
    if (interface->processAsync && ((ret = interface->processAsync(interface, (IMPFrameInfo *)pFrame)) < 0))
    {
        IMP_LOG_ERR(TAG, "interface->processAsync failed,ret=%d\n", ret);
        return -1;
    }
    if (interface->getResult && ((ret = interface->getResult(interface, ppResult)) < 0))
    {
        IMP_LOG_ERR(TAG, "inteface->getResult failed,ret=%d\n", ret);
        return -1;
    }

    return LUX_OK;
}

/**
 * @description: 释放哭声检测结果
 * @return 0：成功 其他：错误码
 */
INT_X LUX_AI_Cry_ReleaseStream()
{        
    INT_X ret = LUX_OK;
    
    //释放哭声检测结果
    LUX_AUDIO_CRY_ST *pAttr = &g_cryAttr;
    if (pAttr->interface && pAttr->interface->releaseResult && pAttr->pCryResult)
    {
        if (pAttr->interface->releaseResult(pAttr->interface, (void *)pAttr->pCryResult))
        {
            IMP_LOG_ERR(TAG, "inteface->releaseResult failed ret=%d\n", ret);
            ret = LUX_ERR;
        }
    }
    return ret;
}
extern float g_cryDetectSensitive; /* 哭声检测灵敏度 */
extern LUX_IVS_TIMEINTERVAL_EN g_alarmInterval; /* 报警时间间隔 */
extern BOOL_X g_bcryDetctAlarmSong;
extern BOOL_X g_pwmBlubEnable;

static BOOL_X APP_IsAlarmByInterval(UINT_X lastTime)
{
    UINT_X timeInterval = 0;
    UINT_X curTime = 0;
    BOOL_X bAlarm = FALSE;

    switch (g_alarmInterval)
    {
        case LUX_IVS_TIMEINTERVAL_1MIN:
            timeInterval = 60;
            break;
        case LUX_IVS_TIMEINTERVAL_3MIN:
            timeInterval = 3 * 60;
            break;
        case LUX_IVS_TIMEINTERVAL_5MIN:
            timeInterval = 5 * 60;
            break;
        default:
            timeInterval = 60;
            break;
    }

    curTime = getTime_getS();
    if (curTime - lastTime > timeInterval)
    {
        bAlarm = TRUE;
    }
    //printf("INFO: %s:%d timeInterval:%d curTime:%d lastTime:%d-%d bAlarm:%d\n",
    //       __func__, __LINE__, timeInterval, curTime, lastTime, *pLastTime, bAlarm);

    return bAlarm;
}

extern idip_server_t* m_idip_server;
void LUX_AI_CryDet_Process()
{
    int ret = 0;
    g_cryAttr.FrmSeq = 0;
    //UINT_X lastSeq = 0;
    UINT_X lastTime = 0;
    UINT_X timerId = 0;
    UINT_X ssTimerId = 0;
    IMPAudioFrame *pImpFrm = NULL;
    IMPAudioFrame ImpFrm;
    uint32_t frmVirAddr[1298];
    LUX_ENCODE_STREAM_ST capPic[2];
    UINT_X  event_id = 0;
    LUX_AUDIO_AI_ST *pAttr = &g_audioAttr.aiDevAttr;
    LUX_BASE_TIMER_PARAM timerParam_event;
    LUX_BASE_TIMER_PARAM timerParam_pwm;
    while (1)
    {
        if (!g_cryAttr.bCryInit || !g_cryAttr.bCryOpen)
        {
            usleep(100 * 1000);
            continue;
        }
        //将数据copy出来使用，以免数据被覆盖
        Thread_Mutex_Lock(&pAttr->mutex);
        pImpFrm = &pAttr->chnFrame[g_cryAttr.FrmSeq % MAX_AUDIO_FRAME_NUM];
        memcpy(&ImpFrm,pImpFrm,sizeof(IMPAudioFrame));
        memcpy(frmVirAddr,pImpFrm->virAddr,ImpFrm.len);
        ImpFrm.virAddr = frmVirAddr;
        Thread_Mutex_UnLock(&pAttr->mutex);

        //处理本地缓存中帧序的同步问题
        if(g_cryAttr.FrmSeq < ImpFrm.seq)   //音频输入速度快与分析速度，会舍弃一部分数据
        {
            //printf("[%d] [sync AAAA] FrmSeq g_cryAttr.FrmSeq = %d ImpFrm.seq = %d ImpFrm.timeStamp = %lld\n",getTime_getMs(),g_cryAttr.FrmSeq,ImpFrm.seq ,ImpFrm.timeStamp);
            g_cryAttr.FrmSeq = ImpFrm.seq;
        }
        if(g_cryAttr.FrmSeq != ImpFrm.seq)
        {
            //printf("[%d] [sync BBBB] FrmSeq g_cryAttr.FrmSeq = %d ImpFrm.seq = %d ImpFrm.timeStamp = %lld\n",getTime_getMs(),g_cryAttr.FrmSeq,ImpFrm.seq ,ImpFrm.timeStamp);
            usleep(500*1000);
            continue;
        }
        g_cryAttr.FrmSeq++;

#ifdef LUX_AI_DUMP_DATA
            static FILE *aiFp = NULL;
            if (NULL == aiFp)
            {
                aiFp = fopen("/tmp/audio_in_CryDet.pcm", "w+");
            }
            if (NULL != aiFp)
            {
                fwrite(ImpFrm.virAddr, 1, ImpFrm.len, aiFp);
                fflush(aiFp);
            }
#endif
        Thread_Mutex_Lock(&g_cryAttr.mutex);
        do
        {
            ret = LUX_AI_CryDetProc(g_cryAttr.interface, &ImpFrm, (void**)&g_cryAttr.pCryResult);
            if (ret != 0)
            {
                IMP_LOG_ERR(TAG, "LUX_AI_CryDetProc error ret:%#x\n", ret);
                LUX_AI_Cry_ReleaseStream();
                break;
            }
            
            if(g_cryAttr.pCryResult->confidence > 0.1)
            {
                printf("[%d] [INFO][[[[CryDet pCryResult confidence = %f   Sensitive = %f]]]]\n",getTime_getMs(), g_cryAttr.pCryResult->confidence ,g_cryDetectSensitive);
            }
            else
            {
                LUX_AI_Cry_ReleaseStream();
                break;
            }
            if(g_cryAttr.pCryResult->confidence > g_cryDetectSensitive)
            {
                if (APP_IsAlarmByInterval(lastTime)) /* unnit:s */
                {
                    g_cryAttr.cryCount++;
                    printf("%s:%d cryCount:[%d] \n", __func__, __LINE__,  g_cryAttr.cryCount);
                    LUX_SUM_Update_Event(LUX_SUM_EVENT_BABY_CRY,gettimeStampS() ,NULL);
                    memset(&capPic[LUX_STREAM_SUB], 0, sizeof(capPic[LUX_STREAM_SUB]));
                    ret = LUX_BASE_CapturePic_HD(LUX_STREAM_SUB,&capPic[LUX_STREAM_SUB]);
                    if (0 != ret)
                    {
                        LUX_BASE_ReleasePic_HD(LUX_STREAM_SUB,&capPic[LUX_STREAM_SUB]);
                        LUX_AI_Cry_ReleaseStream();
                        break;
                    }
                    /* 抓图成功，上报婴儿哭声报警信息 */
                    //jio event
                    // int rc = idip_server_send_sound_event(m_idip_server,
                    //                                     "camera0",// target name is an example
                    //                                     (int64_t)getTime_getMs());   // event time stamp (milliseconds UTC)
                    int rc = idip_server_send_event(m_idip_server,
                                                    "camera0",// target name is an example
                                                    "sound",   // event name
                                                    (int64_t)getTime_getMs(),    // event time stamp (milliseconds UTC)
                                                    NULL);     // there is no data bound to the event
                    if( rc != IDIP_OK ) {
                        // Handle the error
                        printf("\n**************************************** idip_server send_sound_event report failed *********************************\n");
                    }


                    /* 播放安眠曲 */
                    if (g_bcryDetctAlarmSong)
                    {
                        IPC_APP_AUDIO_PlayBeep(IPC_APP_AUDIO_MUSIC);
                    }
                    /* 打开呼吸灯，1min后熄灭 */
                    if (g_pwmBlubEnable)
                    {
                        /* 创建呼吸灯处理定时器 */
                        memset(&timerParam_pwm, 0, sizeof(timerParam_pwm));
                        timerParam_pwm.bTimeStartEnable = LUX_TRUE;
                        timerParam_pwm.timeStartValue = LUX_BABY_CRY_PWM_BLUB_TIME;
                        timerParam_pwm.pTimerProcess = (void*)LUX_PWM_Stop;
                        
                        timerId = LUX_BASE_TimerCreate(&timerParam_pwm);
                        LUX_BASE_TimerStart(timerId);

                        LUX_PWM_Start();
                    }
                    LUX_BASE_ReleasePic_HD(LUX_STREAM_SUB,&capPic[LUX_STREAM_SUB]);
                    //LUX_BASE_ReleasePic(LUX_STREAM_JPEG, &capPic);
                    lastTime = getTime_getS();
                } 
            }
            LUX_AI_Cry_ReleaseStream();
        } while (0);
        Thread_Mutex_UnLock(&g_cryAttr.mutex);
        //usleep(20*1000);
        continue;
    }
    LUX_BASE_TimerDestroy(ssTimerId);
    LUX_BASE_TimerDestroy(timerId);
}

#if 0  //暂时不使用哭声检测  humy
/**
 * @description: 初始化哭声检测算法资源
 * @return 0：成功 其他：错误码
 */
INT_X LUX_AI_CryDet_Init()
{
    INT_X ret = LUX_ERR;
    /* 哭声检测算法初始化 */
    do
    {
        g_cryAttr.interface = CryDetInterfaceInit(&g_cryAttr.crydetParam);
        if (NULL == g_cryAttr.interface)
        {
            IMP_LOG_ERR(TAG, "CryDetInterfaceInit failed\n");
            break;
        }

        if (g_cryAttr.interface->init && ((ret = g_cryAttr.interface->init(g_cryAttr.interface)) < 0))
        {
            IMP_LOG_ERR(TAG, "inteface->init failed, ret=%d\n", ret);
            break;
        }

        Thread_Mutex_Create(&g_cryAttr.mutex);

        Task_CreateThread(LUX_AI_CryDet_Process, NULL);
        g_cryAttr.bCryInit = LUX_TRUE;
        
    } while (0);

    return ret;
}

/**
 * @description: 去初始化哭声检测算法资源
 * @return 0：成功 其他：错误码
 */
INT_X LUX_AI_CryDet_DeInit()
{
    /* 哭声检测算法去初始化 */
    if (g_cryAttr.interface && g_cryAttr.interface->exit)
    {
        g_cryAttr.interface->exit(g_cryAttr.interface);
    }
    //IAAC_DeInit();

    if (g_cryAttr.interface)
    {
        CryDetInterfaceExit(g_cryAttr.interface);
    }
    Thread_Mutex_Destroy(&g_cryAttr.mutex);
    return LUX_OK;
}
#endif