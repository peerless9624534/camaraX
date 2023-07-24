/*
 * comm_func.h
 *
 * 公共函数头文件，提供接口给上层调用
 * 
 */

#ifndef __COMM_FUNC_H__
#define __COMM_FUNC_H__

#include "imp_common.h"
#include "imp_osd.h"
#include "imp_framesource.h"
#include "imp_isp.h"
#include "imp_encoder.h"

#include "comm_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* 码流获取通道，包括主子码流、NV12格式帧数据、JPEG图片，NV21数据 */
typedef enum
{
    LUX_STREAM_MAIN,
    LUX_STREAM_SUB,
    LUX_STREAM_THRD,
    LUX_STREAM_JPEG,
    LUX_STREAM_NV12,
    LUX_STREAM_BUTT,
} LUX_STREAM_CHANNEL_EN;

/* 视频编码类型 */
typedef enum
{
    LUX_ENC_TYPE_H264,
    LUX_ENC_TYPE_H265,
    LUX_ENC_TYPE_JPEG,
} LUX_VIDEO_ENC_TYPE_EN;

/* 设备层区分出来的模块 */
typedef enum
{
    LUX_MODULS_FSRC,
    LUX_MODULS_IVS,
    LUX_MODULS_OSD,
    LUX_MODULS_VENC,
    LUX_MODULS_VDEC,
} LUX_MODULS_DEV_EN;

/* 绑定功能结构 */
typedef struct
{
    BOOL_X bUsed;   /* 当前数组资源是否被使用 */
    INT_X srcChan;  /* 绑定源通道 */
    LUX_MODULS_DEV_EN srcModule; /* 绑定源模块 */

    INT_X dstChan;               /* 绑定目标通道 */
    LUX_MODULS_DEV_EN dstModule; /* 绑定目标模块 */
} LUX_MODULES_BIND_ST;

/* 编码器码率控制模式 */
typedef enum
{
    LUX_VENC_RC_MODE_CBR, /* 定码率 */
    LUX_VENC_RC_MODE_VBR, /* 编码率 */
} LUX_VENC_RC_MODE_EN;

/* 编码器通道参数 */
typedef struct
{
    LUX_STREAM_CHANNEL_EN channel;  /* 码流通道 */
    BOOL_X bEnable;                 /* 通道使能状态 */
    UINT_X fps;                     /* 帧率 */
    UINT_X gop;                     /* gop结构 */
    UINT_X width;                   /* 通道宽、高 */
    UINT_X height;
    UINT_X bitrate;                 /* 通道编码码率 */
    LUX_VENC_RC_MODE_EN rcMode;     /* 码率控制模式，CBR/VBR */
    LUX_VIDEO_ENC_TYPE_EN format;   /* 通道编码格式，H.264/H.265/JPEG */
} LUX_VENC_CHN_PARAM_ST;

/*sensor的宽高信息*/
typedef struct 
{
    UINT_X      width;
    UINT_X      height; 

}LUX_COMM_FUNC_SNS_WH;

#define LUX_MODULES_BIND_NUM 32
/* 公共数据结构 */
typedef struct
{
    UINT_X bindNum;
    LUX_MODULES_BIND_ST binds[LUX_MODULES_BIND_NUM];
} LUX_COMM_ATTR_ST;

/**
 * @description: 添加模块绑定参数到全局信息表，模块初始化完成后一次性绑定
 *              各个模块需要从源端绑定时，调用此接口配置绑定信息
 * @param [in] pBind 模块绑定参数
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_COMM_Bind_AddModule(LUX_MODULES_BIND_ST *pBind);

/**
 * @description: 建立模块绑定连接
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_COMM_Bind_Connect();

/**
  * @description: 解除模块绑定连接
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_COMM_Bind_DisConnect();

/**
 * @description: 二维码解析
 * @return 0 成功，-1 失败
 */
INT_X LUX_COMM_Zbar_Parse(void *y8data, int w, int h, char *pqrcode);

/**
 * @description: 根据sensor的名字找到对应的sensor配置参数
 * @param [in] sensorName  sensor的名字
 * @param [out] pOutSensorWH  sensor的宽高
 * @return 0 成功， 非零失败，返回错误码
 */
 INT_X LUX_COMM_GetSensorWH(PCHAR_X sensorName, LUX_COMM_FUNC_SNS_WH *pOutSensorWH);

/**
 * @description: 解析报警区域字符串
 * @param [in] str  报警区域字符串
 * @param [in] key  键
 * @param [out] value  解析出的键值
 * @return 0 成功， 非零失败，返回错误码
 */
int LUX_COMM_ParseStr(char *str, char *key, int *value);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __COMM_FUNC_H__ */
