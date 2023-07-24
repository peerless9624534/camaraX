/*
 * lux_audio.h
 *
 * 音频头文件
 */

#ifndef __LUX_AUDIO_H__
#define __LUX_AUDIO_H__

#include "comm_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

/**
 * 定义AGC增益结构体.
 */
typedef struct
{
    INT_X targetLevelDbfs;   /**< 增益级别,取值为[0, 31], 这指目标音量级别,单位为db,为负值.值越小,音量越大. */
    INT_X compressionGaindB; /**< 设置最大的增益值,[0, 90],0代表无增益,值越大,增益越高. */
} LUX_AUDIO_AGC_PARAM_ST;

/**
 * 定义音频净荷类型枚举.
 */
typedef enum
{
    LUX_PCM = 0,
    LUX_G711A = 1,
    LUX_G711U = 2,
    LUX_G726 = 3,
    LUX_AUDIO_TYPE_MAX,
} LUX_AUDIO_PAYLOAD_TYPE;

/**
 * 自定义语音定义.
 */
typedef enum
{
    LUX_AUDIO_RECORD_STOP,
    LUX_AUDIO_RECORD_START,
    LUX_AUDIO_RECORD_UPDATE = 3,
} LUX_AUDIO_RECORD_TYPE;
/**
 * 音频采样率定义.
 */
typedef enum
{
    LUX_AUDIO_SAMPLE_RATE_8000 = 8000,   /**< 8KHz采样率 */
    LUX_AUDIO_SAMPLE_RATE_16000 = 16000, /**< 16KHz采样率 */
    LUX_AUDIO_SAMPLE_RATE_24000 = 24000, /**< 24KHz采样率 */
    LUX_AUDIO_SAMPLE_RATE_32000 = 32000, /**< 32KHz采样率 */
    LUX_AUDIO_SAMPLE_RATE_44100 = 44100, /**< 44.1KHz采样率 */
    LUX_AUDIO_SAMPLE_RATE_48000 = 48000, /**< 48KHz采样率 */
    LUX_AUDIO_SAMPLE_RATE_96000 = 96000, /**< 96KHz采样率 */
} LUX_AUDIO_SAMPLE_RATE;
/**
 * 音频采样精度定义.
 */
typedef enum
{
    LUX_AUDIO_BIT_WIDTH_16 = 16, /**< 16bit采样精度 */
} LUX_AUDIO_BIT_WIDTH;
/**
 * 音频声道模式定义.
 */
typedef enum
{
    LUX_AUDIO_SOUND_MODE_MONO = 1,   /**< 单声道 */
    LUX_AUDIO_SOUND_MODE_STEREO = 2, /**< 双声道 */
} LUX_AUDIO_SOUND_MODE;

/**
 * 音频输入输出设备属性.
 */
typedef struct
{
    INT_X  fps;                        /**< 帧率 */
    LUX_AUDIO_PAYLOAD_TYPE type;       /**< 音频编码格式 */
    LUX_AUDIO_SAMPLE_RATE  sampleRate; /**< 音频采样率 */
    LUX_AUDIO_BIT_WIDTH    bitWidth;   /**< 音频采样精度 */
    LUX_AUDIO_SOUND_MODE   soundMode;  /**< 音频声道模式 */
} LUX_AUDIO_PARAM_ST;

typedef struct
{
    UINT_X chnId;
    BOOL_X bStart;

    INT_X volume; /* 设备音量：数字增益 [-30 ~ 120] */
    INT_X gain;   /* 模拟增益 [0 ~ 31]*/

    BOOL_X bNsEnable; /* 降噪，若开启回声消除（Aec），关闭此选项 */
    UINT_X nsLevel;

    BOOL_X bHpfEnable; /* 高通滤波，若开启回声消除（Aec），关闭此选项 */
    BOOL_X bAgcEnable; /* 自动增益，若开启回声消除（Aec），关闭此选项 */
    LUX_AUDIO_AGC_PARAM_ST agcParam;

    LUX_AUDIO_PAYLOAD_TYPE payloadType; /* 音频流负载类型 */
} LUX_AI_CHN_ATTR_ST;

typedef struct
{
    UINT_X chnId;
    BOOL_X bStart;

    INT_X  volume;  /* 设备音量：数字增益 [-30 ~ 120] */
    INT_X  gain;    /* 模拟增益 [0 ~ 31]*/
    BOOL_X bMute;

    BOOL_X bHpfEnable; /* 高通滤波，若开启回声消除（Aec），关闭此选项 */
    BOOL_X bAgcEnable; /* 自动增益，若开启回声消除（Aec），关闭此选项 */
    LUX_AUDIO_AGC_PARAM_ST agcParam;
    
    LUX_AUDIO_PAYLOAD_TYPE payloadType; /* 音频流负载类型 */
} LUX_AO_CHN_ATTR_ST;


/**
 * 音频帧结构体.
 */
typedef struct
{
    VOID_X *pData;           /**< 音频帧数据虚拟地址 */
    INT_X len;                 /**< 音频帧长度 */
    INT_X seq;                 /**< 音频帧序号 */
    UINT64_X timeStamp;        /**< 音频帧数据时间戳 */
    LUX_AUDIO_PAYLOAD_TYPE type;/* 编码格式 */
    VOID_X *priv;
} LUX_AUDIO_FRAME_ST;

/* 哭声检测 */
typedef struct 
{
    FLOAT_X confidence; /* 检测结果的置信度 */
    INT64_X timeStamp;  /* 时间戳 */
    INT_X reserv;       /* 保留 */
} LUX_AI_CRY_DET_RESULT;

INT_X LUX_AI_GetStream(LUX_AUDIO_FRAME_ST *pFrame);
INT_X LUX_AI_ReleaseStream(LUX_AUDIO_FRAME_ST *pFrame);
INT_X LUX_AI_Start();
INT_X LUX_AI_Stop();

BOOL_X LUX_AI_GetCryDet(void);
INT_X LUX_AI_SetCryDet(BOOL_X bOpen);

INT_X LUX_AO_PutStream(LUX_AUDIO_FRAME_ST *pFrame);
INT_X LUX_AO_Start();
INT_X LUX_AO_Stop();
INT_X LUX_AO_ClearBuffer();

//#ifdef BABY_PRO
INT_X LUX_AO_Get_Volume(void);
INT_X LUX_AO_Set_Vloume(INT_X value);
//#endif

INT_X LUX_AO_PlayFile(const char* path);

INT_X LUX_AUDIO_GetParam(LUX_AUDIO_PARAM_ST *pParam);
INT_X LUX_AUDIO_SetParam(LUX_AUDIO_PARAM_ST *pParam);
INT_X LUX_AUDIO_Init();
VOID_X LUX_AUDIO_Exit();

INT_X LUX_AI_CryDet_Init();
INT_X LUX_AI_CryDet_DeInit();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __LUX_AUDIO_H__ */
