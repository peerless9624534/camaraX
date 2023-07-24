/*
* lux_video.c
*
* 基于Ingenic平台编解码模块封装的视频功能，主要使用编码encoder
*
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "comm_def.h"
#include "comm_error_code.h"
#include "comm_func.h"

#include "imp_encoder.h"
#include "imp_log.h"
#include "lux_base.h"
#include "lux_video.h"
#include "lux_fsrc.h"

#include <lux_iniparse.h>

#include <lux_global.h>
#include <jwhead.h>
#include "lux_sys.h"
#include "lux_config.h"
#define TAG "lux_video"

extern LUX_COMM_ATTR_ST g_commAttr;
static const IMPEncoderRcMode S_RC_METHOD = IMP_ENC_RC_MODE_CBR;

typedef struct
{
    INT_X eProfile;
    INT_X uLevel;
    INT_X uTier;
    INT_X ePicFormat;
    INT_X eEncOptions;
    INT_X eEncTools;

}LUX_VIDEO_ENCATTR_PARAM;

typedef struct
{
    INT_X rcMode;
    INT_X uTargetBitRate;
    INT_X uMaxBitRate;
    INT_X iInitialQP;
    INT_X iMinQP;
    INT_X iMaxQP;
    INT_X iIPDelta;
    INT_X iPBDelta;
    INT_X eRcOptions;
    INT_X uMaxPictureSize;
    INT_X uMaxPSNR;
    INT_X frmRateDen;

}LUX_VIDEO_RCATTR_PARAM;


typedef struct
{
    INT_X uGopCtrlMode;
    INT_X uGopLength;
    INT_X uNumB;
    INT_X uMaxSameSenceCnt;
    INT_X bEnableLT;
    INT_X uFreqLT;
    INT_X bLTRC;

}LUX_VIDEO_GOP_PARAM;

typedef struct
{
    LUX_VIDEO_ENCATTR_PARAM encAttr;
    LUX_VIDEO_RCATTR_PARAM rcAttr;
    LUX_VIDEO_GOP_PARAM gopAttr;

}LUX_VIDEO_PARAM_ST;

#define  CHANGE_RESOLUTION_ENBALE 0

#if 0
/**
 * @description: 内部函数，配置编码器低码率编码属性
 * @param [in]  ChnAttr 通道属性
 * @param [in] uTargetBitRate：目标码率值
 * @return 成功：0，失败：错误码；
 */
static INT_X LUX_Video_Encoder_LowBitStream(IMPEncoderChnAttr* pChnAttr, UINT_X uTargetBitRate)
{
    IMPEncoderRcAttr* rcAttr = &pChnAttr->rcAttr;
    //uTargetBitRate /= 2;

    switch (rcAttr->attrRcMode.rcMode)
    {
    case IMP_ENC_RC_MODE_FIXQP:
        rcAttr->attrRcMode.attrFixQp.iInitialQP = 38;
        break;
    case IMP_ENC_RC_MODE_CBR:
        rcAttr->attrRcMode.attrCbr.uTargetBitRate = uTargetBitRate;
        rcAttr->attrRcMode.attrCbr.iInitialQP = -1;
        rcAttr->attrRcMode.attrCbr.iMinQP = 34;
        rcAttr->attrRcMode.attrCbr.iMaxQP = 51;
        rcAttr->attrRcMode.attrCbr.iIPDelta = -1;
        rcAttr->attrRcMode.attrCbr.iPBDelta = -1;
        rcAttr->attrRcMode.attrCbr.eRcOptions = IMP_ENC_RC_SCN_CHG_RES | IMP_ENC_RC_OPT_SC_PREVENTION;
        rcAttr->attrRcMode.attrCbr.uMaxPictureSize = uTargetBitRate * 4 / 3;
        break;
    case IMP_ENC_RC_MODE_VBR:
        rcAttr->attrRcMode.attrVbr.uTargetBitRate = uTargetBitRate;
        rcAttr->attrRcMode.attrVbr.uMaxBitRate = uTargetBitRate * 4 / 3;
        rcAttr->attrRcMode.attrVbr.iInitialQP = -1;
        rcAttr->attrRcMode.attrVbr.iMinQP = 34;
        rcAttr->attrRcMode.attrVbr.iMaxQP = 51;
        rcAttr->attrRcMode.attrVbr.iIPDelta = -1;
        rcAttr->attrRcMode.attrVbr.iPBDelta = -1;
        rcAttr->attrRcMode.attrVbr.eRcOptions = IMP_ENC_RC_SCN_CHG_RES | IMP_ENC_RC_OPT_SC_PREVENTION;
        rcAttr->attrRcMode.attrVbr.uMaxPictureSize = uTargetBitRate * 4 / 3;
        break;
    case IMP_ENC_RC_MODE_CAPPED_VBR:
        rcAttr->attrRcMode.attrCappedVbr.uTargetBitRate = uTargetBitRate;
        rcAttr->attrRcMode.attrCappedVbr.uMaxBitRate = uTargetBitRate * 4 / 3;
        rcAttr->attrRcMode.attrCappedVbr.iInitialQP = -1;
        rcAttr->attrRcMode.attrCappedVbr.iMinQP = 34;
        rcAttr->attrRcMode.attrCappedVbr.iMaxQP = 51;
        rcAttr->attrRcMode.attrCappedVbr.iIPDelta = -1;
        rcAttr->attrRcMode.attrCappedVbr.iPBDelta = -1;
        rcAttr->attrRcMode.attrCappedVbr.eRcOptions = IMP_ENC_RC_SCN_CHG_RES | IMP_ENC_RC_OPT_SC_PREVENTION;
        rcAttr->attrRcMode.attrCappedVbr.uMaxPictureSize = uTargetBitRate * 4 / 3;
        rcAttr->attrRcMode.attrCappedVbr.uMaxPSNR = 42;
        break;
    case IMP_ENC_RC_MODE_CAPPED_QUALITY:
        rcAttr->attrRcMode.attrCappedQuality.uTargetBitRate = uTargetBitRate;
        rcAttr->attrRcMode.attrCappedQuality.uMaxBitRate = uTargetBitRate * 4 / 3;
        rcAttr->attrRcMode.attrCappedQuality.iInitialQP = -1;
        rcAttr->attrRcMode.attrCappedQuality.iMinQP = 34;
        rcAttr->attrRcMode.attrCappedQuality.iMaxQP = 51;
        rcAttr->attrRcMode.attrCappedQuality.iIPDelta = -1;
        rcAttr->attrRcMode.attrCappedQuality.iPBDelta = -1;
        rcAttr->attrRcMode.attrCappedQuality.eRcOptions = IMP_ENC_RC_SCN_CHG_RES | IMP_ENC_RC_OPT_SC_PREVENTION;
        rcAttr->attrRcMode.attrCappedQuality.uMaxPictureSize = uTargetBitRate * 4 / 3;
        rcAttr->attrRcMode.attrCappedQuality.uMaxPSNR = 42;
        break;
    case IMP_ENC_RC_MODE_INVALID:
        IMP_LOG_ERR(TAG, "unsupported rcmode:%d, we only support fixqp, cbr vbr and capped vbr\n", rcAttr->attrRcMode.rcMode);
        return LUX_VIDEO_INVALID_PARM;
    }

    return LUX_OK;
}
#endif


/**
 * @description: 内部函数，配置编码通道默认参数
 * @param [in] pChnAttr: 通道属性
 * @param [in] pImpChnAttr ：通道属性
 * @return 成功：0，失败：错误码；
 */
INT_X LUX_Video_Encoder_SetDefaultParam(ENCODER_CHN_ATTR_ST* pChnAttr, IMPEncoderChnAttr* pImpChnAttr)
{
    if (NULL == pChnAttr || NULL == pImpChnAttr)
    {
        IMP_LOG_ERR(TAG, "LUX_Video_Encoder_SetDefaultParam, null ptr!\n");
        return LUX_VIDEO_NULL_PRT;
    }
    INT_X ret = LUX_ERR;
    FLOAT_X ratio = 1.00;
    UINT_X uTargetBitRate = 0;
    UINT_X frmRateNum = 25;
    UINT_X frmRateDen = 1;
    INT_X chnWidth = 0;
    INT_X chnHeight = 0;

    /* 校验通道宽高 */
    chnWidth = (INT_X)((0 == pChnAttr->chnAttr.encAttr.uWidth) ? SENSOR_WIDTH_DEFAULT : pChnAttr->chnAttr.encAttr.uWidth);
    chnHeight = (INT_X)((0 == pChnAttr->chnAttr.encAttr.uHeight) ? SENSOR_HEIGHT_DEFAULT : pChnAttr->chnAttr.encAttr.uHeight);

    if (((uint64_t)chnWidth * chnHeight) > (1280 * 720))
    {
        ratio = log10f(((uint64_t)chnWidth * chnHeight) / (1280 * 720.0)) + 1;
    }
    else
    {
        ratio = 1.0 / (log10f((1280 * 720.0) / ((uint64_t)chnWidth * chnHeight)) + 1);
    }
    ratio = ratio > 0.1 ? ratio : 0.1;
    uTargetBitRate = BITRATE_720P_Kbs * ratio;

    frmRateNum = pChnAttr->chnAttr.rcAttr.outFrmRate.frmRateNum ? pChnAttr->chnAttr.rcAttr.outFrmRate.frmRateNum : 15;
    frmRateDen = pChnAttr->chnAttr.rcAttr.outFrmRate.frmRateDen ? pChnAttr->chnAttr.rcAttr.outFrmRate.frmRateDen : 1;

    /* 设置通道参数 */
    ret = IMP_Encoder_SetDefaultParam(pImpChnAttr, pChnAttr->chnAttr.encAttr.eProfile, pChnAttr->chnAttr.rcAttr.attrRcMode.rcMode,
                                      chnWidth, chnHeight, frmRateNum, frmRateDen, frmRateNum / frmRateDen, 2,
                                      (pChnAttr->chnAttr.rcAttr.attrRcMode.rcMode == IMP_ENC_RC_MODE_FIXQP) ? 35 : -1,
                                      uTargetBitRate);
    if (ret < 0)
    {
        IMP_LOG_ERR(TAG, "IMP_Encoder_SetDefaultParam error !\n");
        return LUX_VIDEO_CHAN_PARAM_ERR;
    }

#if 0    
    ret = LUX_Video_Encoder_LowBitStream(pImpChnAttr, uTargetBitRate);
    if (ret < 0)
    {
        IMP_LOG_ERR(TAG, "LUX_Video_Encoder_LowBitStream error ret:%#x\n", ret);
        return LUX_VIDEO_CHAN_PARAM_ERR;
    }
#endif
    return ret;
}

static VOID_X LUX_Video_Encoder_ChnAttr_DebugInfo(INT_X chnID, LUX_VIDEO_PARAM_ST* pChnConfig)
{
    printf("--------------------------------Video Encoder Attr--------------------------------------\n");
    printf("chn[%d]_encAttr_eProfile = [%d]\n", chnID, pChnConfig->encAttr.eProfile);
    printf("chn[%d]_encAttr_uLevel = [%d]\n", chnID, pChnConfig->encAttr.uLevel);
    printf("chn[%d]_encAttr_uTier = [%d]\n", chnID, pChnConfig->encAttr.uTier);
    printf("chn[%d]_encAttr_ePicFormat = [%d]\n", chnID, pChnConfig->encAttr.ePicFormat);
    printf("chn[%d]_encAttr_eEncOptions = [%d]\n", chnID, pChnConfig->encAttr.eEncOptions);
    printf("chn[%d]_encAttr_eEncTools = [%d]\n", chnID, pChnConfig->encAttr.eEncTools);
    printf("chn[%d]_rcAttr_rcMode = [%d]\n", chnID, pChnConfig->rcAttr.rcMode);
    printf("chn[%d]_rcAttr_uTargetBitRate = [%d]\n", chnID, pChnConfig->rcAttr.uTargetBitRate);
    printf("chn[%d]_rcAttr_uMaxBitRate = [%d]\n", chnID, pChnConfig->rcAttr.uMaxBitRate);
    printf("chn[%d]_rcAttr_iInitialQ = [%d]\n", chnID, pChnConfig->rcAttr.iInitialQP);
    printf("chn[%d]_rcAttr_iMinQP = [%d]\n", chnID, pChnConfig->rcAttr.iMinQP);
    printf("chn[%d]_rcAttr_iMaxQP = [%d]\n", chnID, pChnConfig->rcAttr.iMaxQP);
    printf("chn[%d]_rcAttr_iIPDelta = [%d]\n", chnID, pChnConfig->rcAttr.iPBDelta);
    printf("chn[%d]_rcAttr_iPBDelta = [%d]\n", chnID, pChnConfig->rcAttr.iPBDelta);
    printf("chn[%d]_rcAttr_eRcOptions = [%d]\n", chnID, pChnConfig->rcAttr.eRcOptions);
    printf("chn[%d]_rcAttr_uMaxPictureSize = [%d]\n", chnID, pChnConfig->rcAttr.uMaxPictureSize);
    printf("chn[%d]_rcAttr_uMaxPSNR = [%d]\n", chnID, pChnConfig->rcAttr.uMaxPSNR);
    printf("chn[%d]_rcAttr_frmRateDen = [%d]\n", chnID, pChnConfig->rcAttr.frmRateDen);
    printf("chn[%d]_gopAttr_uGopCtrlMode = [%d]\n", chnID, pChnConfig->gopAttr.uGopCtrlMode);
    printf("chn[%d]_gopAttr_uGopLength = [%d]\n", chnID, pChnConfig->gopAttr.uGopLength);
    //printf("chn[%d]_gopAttr_uNumB = [%d]\n", chnID, pChnConfig->gopAttr.uNumB);
    printf("chn[%d]_gopAttr_uMaxSameSenceCnt = [%d]\n", chnID, pChnConfig->gopAttr.uMaxSameSenceCnt);
    printf("chn[%d]_gopAttr_bEnableLT = [%d]\n", chnID, pChnConfig->gopAttr.bEnableLT);
    printf("chn[%d]_gopAttr_uFreqLT = [%d]\n", chnID, pChnConfig->gopAttr.uFreqLT);
    printf("chn[%d]_gopAttr_bLTRC = [%d]\n", chnID, pChnConfig->gopAttr.bLTRC);

    return;
}


/**
 * @description: 获取配置编码器编码属性
 *  @param [in] chnID：通道号
 * @param [in]  ChnAttr 通道属性
 * @return 成功：0，失败：错误码；
 */
static INT_X LUX_Video_Encoder_GetChnConfig(INT_X chnID, LUX_VIDEO_PARAM_ST* pChnConfig)
{
    if (NULL == pChnConfig)
    {
        IMP_LOG_ERR(TAG, "LUX_Video_Encoder_SetChnParam, null ptr!\n");
        return LUX_VIDEO_NULL_PRT;
    }

    switch (chnID)
    {
    case CH0_INDEX:

             pChnConfig->encAttr.eProfile =    g_configData->media_config.video_config.chn0_encAttr_eProfile;  //AVC_BASELINE_H264=66
             pChnConfig->encAttr.uLevel =    g_configData->media_config.video_config.chn0_encAttr_uLevel;
             pChnConfig->encAttr.uTier =    g_configData->media_config.video_config.chn0_encAttr_uTier;
             pChnConfig->encAttr.ePicFormat =    g_configData->media_config.video_config.chn0_encAttr_ePicFormat;
             pChnConfig->encAttr.eEncOptions =    g_configData->media_config.video_config.chn0_encAttr_eEncOptions;
             pChnConfig->encAttr.eEncTools =    g_configData->media_config.video_config.chn0_encAttr_eEncTools;
             pChnConfig->rcAttr.rcMode =    g_configData->media_config.video_config.chn0_rcAttr_rcMode;
             pChnConfig->rcAttr.uTargetBitRate =    g_configData->media_config.video_config.chn0_rcAttr_uTargetBitRate;
             pChnConfig->rcAttr.uMaxBitRate =    g_configData->media_config.video_config.chn0_rcAttr_uMaxBitRate;
             pChnConfig->rcAttr.iInitialQP =    g_configData->media_config.video_config.chn0_rcAttr_iInitialQP;
             pChnConfig->rcAttr.iMinQP =    g_configData->media_config.video_config.chn0_rcAttr_iMinQP;
             pChnConfig->rcAttr.iMaxQP =    g_configData->media_config.video_config.chn0_rcAttr_iMaxQP;
             pChnConfig->rcAttr.iIPDelta =    g_configData->media_config.video_config.chn0_rcAttr_iIPDelta;
             pChnConfig->rcAttr.iPBDelta =    g_configData->media_config.video_config.chn0_rcAttr_iPBDelta;
             pChnConfig->rcAttr.eRcOptions =    g_configData->media_config.video_config.chn0_rcAttr_eRcOptions;
             pChnConfig->rcAttr.uMaxPictureSize =    g_configData->media_config.video_config.chn0_rcAttr_uMaxPictureSize;
             pChnConfig->rcAttr.uMaxPSNR =    g_configData->media_config.video_config.chn0_rcAttr_uMaxPSNR;
             pChnConfig->rcAttr.frmRateDen =    g_configData->media_config.video_config.chn0_rcAttr_frmRateDen;
             pChnConfig->gopAttr.uGopCtrlMode =    g_configData->media_config.video_config.chn0_gopAttr_uGopCtrlMode;
             pChnConfig->gopAttr.uGopLength =    g_configData->media_config.video_config.chn0_gopAttr_uGopLength;
             pChnConfig->gopAttr.uNumB =    g_configData->media_config.video_config.chn0_gopAttr_uNumB;
             pChnConfig->gopAttr.uMaxSameSenceCnt =    g_configData->media_config.video_config.chn0_gopAttr_uMaxSameSenceCnt;
             pChnConfig->gopAttr.bEnableLT =    g_configData->media_config.video_config.chn0_gopAttr_bEnableLT;
             pChnConfig->gopAttr.uFreqLT =   g_configData->media_config.video_config.chn0_gopAttr_uFreqLT;
             pChnConfig->gopAttr.bLTRC =    g_configData->media_config.video_config.chn0_gopAttr_bLTRC;

         break;

    case CH1_INDEX:
             pChnConfig->encAttr.eProfile =    g_configData->media_config.video_config.chn1_encAttr_eProfile;  //AVC_BASELINE_H264=66
             pChnConfig->encAttr.uLevel =    g_configData->media_config.video_config.chn1_encAttr_uLevel;
             pChnConfig->encAttr.uTier =    g_configData->media_config.video_config.chn1_encAttr_uTier;
             pChnConfig->encAttr.ePicFormat =    g_configData->media_config.video_config.chn1_encAttr_ePicFormat;
             pChnConfig->encAttr.eEncOptions =    g_configData->media_config.video_config.chn1_encAttr_eEncOptions;
             pChnConfig->encAttr.eEncTools =    g_configData->media_config.video_config.chn1_encAttr_eEncTools;
             pChnConfig->rcAttr.rcMode =    g_configData->media_config.video_config.chn1_rcAttr_rcMode;
             pChnConfig->rcAttr.uTargetBitRate =    g_configData->media_config.video_config.chn1_rcAttr_uTargetBitRate;
             pChnConfig->rcAttr.uMaxBitRate =    g_configData->media_config.video_config.chn1_rcAttr_uMaxBitRate;
             pChnConfig->rcAttr.iInitialQP =    g_configData->media_config.video_config.chn1_rcAttr_iInitialQP;
             pChnConfig->rcAttr.iMinQP =    g_configData->media_config.video_config.chn1_rcAttr_iMinQP;
             pChnConfig->rcAttr.iMaxQP =    g_configData->media_config.video_config.chn1_rcAttr_iMaxQP;
             pChnConfig->rcAttr.iIPDelta =    g_configData->media_config.video_config.chn1_rcAttr_iIPDelta;
             pChnConfig->rcAttr.iPBDelta =    g_configData->media_config.video_config.chn1_rcAttr_iPBDelta;
             pChnConfig->rcAttr.eRcOptions =    g_configData->media_config.video_config.chn1_rcAttr_eRcOptions;
             pChnConfig->rcAttr.uMaxPictureSize =    g_configData->media_config.video_config.chn1_rcAttr_uMaxPictureSize;
             pChnConfig->rcAttr.uMaxPSNR =    g_configData->media_config.video_config.chn1_rcAttr_uMaxPSNR;
             pChnConfig->rcAttr.frmRateDen =    g_configData->media_config.video_config.chn1_rcAttr_frmRateDen;
             pChnConfig->gopAttr.uGopCtrlMode =    g_configData->media_config.video_config.chn1_gopAttr_uGopCtrlMode;
             pChnConfig->gopAttr.uGopLength =    g_configData->media_config.video_config.chn1_gopAttr_uGopLength;
             pChnConfig->gopAttr.uNumB =    g_configData->media_config.video_config.chn1_gopAttr_uNumB;
             pChnConfig->gopAttr.uMaxSameSenceCnt =    g_configData->media_config.video_config.chn1_gopAttr_uMaxSameSenceCnt;
             pChnConfig->gopAttr.bEnableLT =    g_configData->media_config.video_config.chn1_gopAttr_bEnableLT;
             pChnConfig->gopAttr.uFreqLT =   g_configData->media_config.video_config.chn1_gopAttr_uFreqLT;
             pChnConfig->gopAttr.bLTRC =    g_configData->media_config.video_config.chn1_gopAttr_bLTRC;
        break;

    case CH2_INDEX:
             pChnConfig->encAttr.eProfile =    g_configData->media_config.video_config.chn1_encAttr_eProfile;  //AVC_BASELINE_H264=66
             pChnConfig->encAttr.uLevel =    g_configData->media_config.video_config.chn2_encAttr_uLevel;
             pChnConfig->encAttr.uTier =    g_configData->media_config.video_config.chn2_encAttr_uTier;
             pChnConfig->encAttr.ePicFormat =    g_configData->media_config.video_config.chn2_encAttr_ePicFormat;
             pChnConfig->encAttr.eEncOptions =    g_configData->media_config.video_config.chn2_encAttr_eEncOptions;
             pChnConfig->encAttr.eEncTools =    g_configData->media_config.video_config.chn2_encAttr_eEncTools;
             pChnConfig->rcAttr.rcMode =    g_configData->media_config.video_config.chn2_rcAttr_rcMode;
             pChnConfig->rcAttr.iInitialQP =    g_configData->media_config.video_config.chn2_rcAttr_iInitialQP;
             pChnConfig->rcAttr.frmRateDen =    g_configData->media_config.video_config.chn2_rcAttr_frmRateDen;
             pChnConfig->gopAttr.uGopCtrlMode =    g_configData->media_config.video_config.chn2_gopAttr_uGopCtrlMode;
             pChnConfig->gopAttr.uGopLength =    g_configData->media_config.video_config.chn2_gopAttr_uGopLength;
             pChnConfig->gopAttr.uNumB =    g_configData->media_config.video_config.chn2_gopAttr_uNumB;
             pChnConfig->gopAttr.uMaxSameSenceCnt =    g_configData->media_config.video_config.chn2_gopAttr_uMaxSameSenceCnt;
             pChnConfig->gopAttr.bEnableLT =    g_configData->media_config.video_config.chn2_gopAttr_bEnableLT;
             pChnConfig->gopAttr.uFreqLT =   g_configData->media_config.video_config.chn2_gopAttr_uFreqLT;
             pChnConfig->gopAttr.bLTRC =    g_configData->media_config.video_config.chn2_gopAttr_bLTRC;
        break;

    default:
        IMP_LOG_ERR(TAG, " invalid chnnal(%d)\n", chnID);
        return LUX_VIDEO_INVALID_PARM;
    }

    LUX_Video_Encoder_ChnAttr_DebugInfo(chnID, pChnConfig);

    return LUX_OK;
}

/**
 * @description: 配置编码器编码属性
 *  @param [in] chnID：通道号
 * @param [in]  ChnAttr 通道属性
 * @return 成功：0，失败：错误码；
 */
static INT_X LUX_Video_Encoder_SetChnParam(INT_X chnID, IMPEncoderChnAttr* pChnAttr)
{

    if (NULL == pChnAttr)
    {
        IMP_LOG_ERR(TAG, "LUX_Video_Encoder_SetChnParam, null ptr!\n");
        return LUX_VIDEO_NULL_PRT;
    }


#if 0 //默认配置
    INT_X ret = LUX_ERR;
    IMPEncoderChnAttr encChnAttr;
#endif
    int ret = -1;
    IMPFSChnAttr chnAttr = { 0 };
    LUX_VIDEO_PARAM_ST chnAttrParam;
    memset(&chnAttrParam, 0, sizeof(LUX_VIDEO_PARAM_ST));

    /*获取视频源的通道属性*/
    ret = IMP_FrameSource_GetChnAttr(chnID, &chnAttr);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_Video_Encoder_SetChnParam, null ptr!\n");
    }

    LUX_Video_Encoder_GetChnConfig(chnID, &chnAttrParam);

    switch (chnID)
    {
    case CH0_INDEX:
        pChnAttr->encAttr.uWidth = (0 != chnAttr.picWidth) ? chnAttr.picWidth : SENSOR_WIDTH_DEFAULT;
        pChnAttr->encAttr.uHeight = (0 != chnAttr.picHeight) ? chnAttr.picHeight : SENSOR_HEIGHT_DEFAULT;
        pChnAttr->rcAttr.outFrmRate.frmRateNum = VIDEO_ENC_MAIN_FRAMERATE;
        pChnAttr->encAttr.eProfile = chnAttrParam.encAttr.eProfile;
        pChnAttr->encAttr.uLevel = chnAttrParam.encAttr.uLevel;
        pChnAttr->encAttr.uTier = chnAttrParam.encAttr.uTier;
        pChnAttr->encAttr.ePicFormat = chnAttrParam.encAttr.ePicFormat;
        pChnAttr->encAttr.eEncOptions = chnAttrParam.encAttr.eEncOptions;
        pChnAttr->encAttr.eEncTools = chnAttrParam.encAttr.eEncTools;

        pChnAttr->rcAttr.attrRcMode.rcMode = chnAttrParam.rcAttr.rcMode;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.uTargetBitRate = chnAttrParam.rcAttr.uTargetBitRate;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.uMaxBitRate = chnAttrParam.rcAttr.uMaxBitRate;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.iInitialQP = chnAttrParam.rcAttr.iInitialQP;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.iMinQP = chnAttrParam.rcAttr.iMinQP;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.iMaxQP = chnAttrParam.rcAttr.iMaxQP;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.iIPDelta = chnAttrParam.rcAttr.iIPDelta;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.iPBDelta = chnAttrParam.rcAttr.iPBDelta;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.eRcOptions = chnAttrParam.rcAttr.eRcOptions;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.uMaxPictureSize = chnAttrParam.rcAttr.uMaxPictureSize;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.uMaxPSNR = chnAttrParam.rcAttr.uMaxPSNR;
        pChnAttr->rcAttr.outFrmRate.frmRateDen = chnAttrParam.rcAttr.frmRateDen;

        pChnAttr->gopAttr.uGopCtrlMode = chnAttrParam.gopAttr.uGopCtrlMode;
        pChnAttr->gopAttr.uGopLength = chnAttrParam.gopAttr.uGopLength;
        //pChnAttr->gopAttr.uNumB = chnAttrParam.gopAttr.uNumB;
        pChnAttr->gopAttr.uMaxSameSenceCnt = chnAttrParam.gopAttr.uMaxSameSenceCnt;
        pChnAttr->gopAttr.bEnableLT = chnAttrParam.gopAttr.bEnableLT;
        pChnAttr->gopAttr.uFreqLT = chnAttrParam.gopAttr.uFreqLT;
        pChnAttr->gopAttr.bLTRC = chnAttrParam.gopAttr.bLTRC;
        break;

    case CH1_INDEX:
        pChnAttr->encAttr.uWidth = (0 != chnAttr.picWidth) ? chnAttr.picWidth : SENSOR_WIDTH_SECOND;
        pChnAttr->encAttr.uHeight = (0 != chnAttr.picHeight) ? chnAttr.picHeight : SENSOR_HEIGHT_SECOND;
        pChnAttr->rcAttr.outFrmRate.frmRateNum = VIDEO_ENC_MAIN_FRAMERATE;
        pChnAttr->encAttr.eProfile = chnAttrParam.encAttr.eProfile;
        pChnAttr->encAttr.uLevel = chnAttrParam.encAttr.uLevel;
        pChnAttr->encAttr.uTier = chnAttrParam.encAttr.uTier;
        pChnAttr->encAttr.ePicFormat = chnAttrParam.encAttr.ePicFormat;
        pChnAttr->encAttr.eEncOptions = chnAttrParam.encAttr.eEncOptions;
        pChnAttr->encAttr.eEncTools = chnAttrParam.encAttr.eEncTools;

        pChnAttr->rcAttr.attrRcMode.rcMode = chnAttrParam.rcAttr.rcMode;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.uTargetBitRate = chnAttrParam.rcAttr.uTargetBitRate;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.uMaxBitRate = chnAttrParam.rcAttr.uMaxBitRate;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.iInitialQP = chnAttrParam.rcAttr.iInitialQP;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.iMinQP = chnAttrParam.rcAttr.iMinQP;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.iMaxQP = chnAttrParam.rcAttr.iMaxQP;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.iIPDelta = chnAttrParam.rcAttr.iIPDelta;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.iPBDelta = chnAttrParam.rcAttr.iPBDelta;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.eRcOptions = chnAttrParam.rcAttr.eRcOptions;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.uMaxPictureSize = chnAttrParam.rcAttr.uMaxPictureSize;
        pChnAttr->rcAttr.attrRcMode.attrCappedQuality.uMaxPSNR = chnAttrParam.rcAttr.uMaxPSNR;
        pChnAttr->rcAttr.outFrmRate.frmRateDen = chnAttrParam.rcAttr.frmRateDen;

        pChnAttr->gopAttr.uGopCtrlMode = chnAttrParam.gopAttr.uGopCtrlMode;
        pChnAttr->gopAttr.uGopLength = chnAttrParam.gopAttr.uGopLength;
        //pChnAttr->gopAttr.uNumB = chnAttrParam.gopAttr.uNumB;
        pChnAttr->gopAttr.uMaxSameSenceCnt = chnAttrParam.gopAttr.uMaxSameSenceCnt;
        pChnAttr->gopAttr.bEnableLT = chnAttrParam.gopAttr.bEnableLT;
        pChnAttr->gopAttr.uFreqLT = chnAttrParam.gopAttr.uFreqLT;
        pChnAttr->gopAttr.bLTRC = chnAttrParam.gopAttr.bLTRC;
        break;

    case CH2_INDEX:
        pChnAttr->encAttr.uWidth = (0 != chnAttr.picWidth) ? chnAttr.picWidth : SENSOR_WIDTH_SECOND;
        pChnAttr->encAttr.uHeight = (0 != chnAttr.picHeight) ? chnAttr.picHeight : SENSOR_HEIGHT_SECOND;

        pChnAttr->encAttr.eProfile = chnAttrParam.encAttr.eProfile;
        pChnAttr->encAttr.ePicFormat = chnAttrParam.encAttr.ePicFormat;
        pChnAttr->encAttr.eEncOptions = chnAttrParam.encAttr.eEncOptions;
        pChnAttr->encAttr.eEncTools = chnAttrParam.encAttr.eEncTools;

        pChnAttr->rcAttr.attrRcMode.rcMode = chnAttrParam.rcAttr.rcMode;
        pChnAttr->rcAttr.attrRcMode.attrFixQp.iInitialQP = chnAttrParam.rcAttr.iInitialQP;
        pChnAttr->rcAttr.outFrmRate.frmRateNum = 5;
        pChnAttr->rcAttr.outFrmRate.frmRateDen = chnAttrParam.rcAttr.frmRateDen;

        pChnAttr->gopAttr.uGopCtrlMode = chnAttrParam.gopAttr.uGopCtrlMode;
        pChnAttr->gopAttr.uGopLength = chnAttrParam.gopAttr.uGopLength;
        //pChnAttr->gopAttr.uNumB = chnAttrParam.gopAttr.uNumB;
        pChnAttr->gopAttr.uMaxSameSenceCnt = chnAttrParam.gopAttr.uMaxSameSenceCnt;
        pChnAttr->gopAttr.bEnableLT = chnAttrParam.gopAttr.bEnableLT;
        pChnAttr->gopAttr.uFreqLT = chnAttrParam.gopAttr.uFreqLT;
        pChnAttr->gopAttr.bLTRC = chnAttrParam.gopAttr.bLTRC;
        break;

#if 0 //默认配置
        memset(&encChnAttr, 0, sizeof(encChnAttr));
        ret = LUX_Video_Encoder_SetDefaultParam(&g_VEncAttr.encGrpAttr[1].encChnAttr[1], &encChnAttr);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_Encoder_CreateChn(%d) error ret：%#x!\n", ret);
            return LUX_VIDEO_CHAN_CREATE_ERR;
        }
        memcpy(pChnAttr, &encChnAttr, sizeof(IMPEncoderChnAttr));
        break;


#endif
    default:
        IMP_LOG_ERR(TAG, " invalid chnnal(%d)\n", chnID);
        return LUX_VIDEO_INVALID_PARM;
    }

    return LUX_OK;
}



/**
 * @description: 内部函数，创建编码器通道
 * @param [in] grpId：编码器组ID
 * @return 成功：0，失败：错误码；
 */
static INT_X LUX_Video_Encoder_CreateChn(UINT_X grpId)
{
    if (grpId >= LUX_VENC_GRP_NUM)
    {
        IMP_LOG_ERR(TAG, "LUX_Video_Encoder_CreateChn, param error grpId:%d!\n", grpId);
        return LUX_VIDEO_NULL_PRT;
    }

    UINT_X i = 0;
    INT_X ret = LUX_ERR;
    ENCODER_CHN_ATTR_ST* pChnAttr = NULL;

    for (i = 0; i < LUX_VENC_CHN_NUM; i++)
    {
        pChnAttr = &g_VEncAttr->encGrpAttr[grpId].encChnAttr[i];
        if (pChnAttr->bValid)
        {

#if 0
            /* 设置编码通道参数 */
            //memset(&encChnAttr, 0, sizeof(encChnAttr));
            ret = LUX_Video_Encoder_SetDefaultParam(pChnAttr, &g_VEncAttr->encGrpAttr[grpId].encChnAttr[i].chnAttr);
            if (LUX_OK != ret)
            {
                IMP_LOG_ERR(TAG, "IMP_Encoder_CreateChn(%d) error ret：%#x!\n", ret);
                return LUX_VIDEO_CHAN_CREATE_ERR;
            }
#endif
            /* 设置编码通道参数 */
            ret = LUX_Video_Encoder_SetChnParam(pChnAttr->chnId, &pChnAttr->chnAttr);
            if (LUX_OK != ret)
            {
                MYERRORING("IMP_Encoder_CreateChn(%d) error ret%#x!\n", pChnAttr->chnId, ret);
                return LUX_VIDEO_CHAN_CREATE_ERR;
            }
            else {
                MYTRACE("grpID:%d,chnID:%d,LUX_Video_Encoder_SetChnParam is success\n", grpId, pChnAttr->chnId);
            }

            /* 创建编码通道 */
            ret = IMP_Encoder_CreateChn(pChnAttr->chnId, &pChnAttr->chnAttr);
            if (ret < 0)
            {
                MYERRORING("IMP_Encoder_CreateChn(%d,%d) error:%#x !\n", grpId, pChnAttr->chnId, ret);
                return LUX_VIDEO_CHAN_CREATE_ERR;
            }
            else {
                MYTRACE("grpID:%d,chnID:%d,IMP_Encoder_CreateChn is success\n", grpId, pChnAttr->chnId);
            }

            /* 注册编码通道 */
            ret = IMP_Encoder_RegisterChn(grpId, pChnAttr->chnId);
            if (ret < 0)
            {
                IMP_LOG_ERR(TAG, "IMP_Encoder_RegisterChn(%d,%d) error: %d\n", grpId, pChnAttr->chnId, ret);
                return LUX_VIDEO_CHAN_CREATE_ERR;
            }
            else {
                MYTRACE("grpID:%d,chnID:%d,IMP_Encoder_RegisterChn is success\n", grpId, pChnAttr->chnId);
            }
#if 0
            /* gop属性 */
            if (pChnAttr->chnAttr.gopAttr.uGopLength)
            {
                ret = IMP_Encoder_SetChnGopLength(pChnAttr->chnId, pChnAttr->chnAttr.gopAttr.uGopLength);
                if (ret < 0)
                {
                    IMP_LOG_ERR(TAG, "IMP_Encoder_SetChnGopLength error !\n");
                }
                encChnAttr.gopAttr.uGopLength = pChnAttr->chnAttr.gopAttr.uGopLength;
            }
#endif
            /* 申请 LUX_VENC_CHN_TMPBUF_LEN 大小的通道码流缓存数据 */
            pChnAttr->pTmpBuf = malloc(LUX_VENC_CHN_TMPBUF_LEN);
            if (NULL == pChnAttr->pTmpBuf)
            {
                IMP_LOG_ERR(TAG, "malloc memory error !!!\n");
                return LUX_VIDEO_MALLOC_ERR;
            }
            else {
                MYTRACE("grpID:%d,chnID:%d,malloc LUX_VENC_CHN_TMPBUF is success\n", grpId, i);
            }
        }
    }
    return ret;
}

/**
 * @description: 内部函数，销毁编码器通道
 * @param [in] grpId：编码器组ID
 * @return 成功：0，失败：错误码；
 */
static INT_X LUX_Video_Encoder_DestroyChn(UINT_X grpId)
{
    if (grpId >= LUX_VENC_GRP_NUM)
    {
        IMP_LOG_ERR(TAG, "LUX_Video_Encoder_DestroyChn, param error grpId:%d!\n", grpId);
        return LUX_VIDEO_INVALID_PARM;
    }

    INT_X i = 0;
    INT_X ret = LUX_OK;
    IMPEncoderChnStat chnStat;
    ENCODER_CHN_ATTR_ST* pChnAttr = NULL;

    for (i = 0; i < LUX_VENC_CHN_NUM; i++)
    {
        pChnAttr = &g_VEncAttr->encGrpAttr[grpId].encChnAttr[i];

        /* 通道已经使能，释放掉资源 */
        if (pChnAttr->bValid)
        {
            /* 释放临时缓存 */
            if (pChnAttr->pTmpBuf)
            {
                free(pChnAttr->pTmpBuf);
            }

            memset(&chnStat, 0, sizeof(chnStat));
            ret = IMP_Encoder_Query(pChnAttr->chnId, &chnStat);
            if (ret < 0)
            {
                IMP_LOG_ERR(TAG, "IMP_Encoder_Query(%d) error: %d\n", grpId, ret);
                continue;
            }

            if (chnStat.registered)
            {
                ret = IMP_Encoder_UnRegisterChn(pChnAttr->chnId);
                if (ret < 0)
                {
                    IMP_LOG_ERR(TAG, "IMP_Encoder_UnRegisterChn(%d) error: %d\n", grpId, ret);
                    continue;
                }

                ret = IMP_Encoder_DestroyChn(pChnAttr->chnId);
                if (ret < 0)
                {
                    IMP_LOG_ERR(TAG, "IMP_Encoder_DestroyChn(%d) error: %d\n", grpId, ret);
                    continue;
                }
            }
        }
    }

    return ret;
}

#ifdef SAVE_STREAM  /* 测试代码 */
static int save_stream(int chnId, IMPEncoderStream* stream)
{
    int ret = 0;
    int i = 0;
    int nr_pack = stream->packCount;
    IMPEncoderPack* pack = NULL;
    uint32_t remSize = 0;
    char saveName[32] = { 0 };
    char* formatTable[3] = { "h264", "h264", "jpg" };
    static FILE* fp[VIDEO_ENC_CHN_MAX] = { NULL };

    if (!fp[chnId])
    {
        sprintf(saveName, "video_stream_chn%d.%s", chnId, formatTable[chnId]);

        fp[chnId] = fopen(saveName, "wb");
        if (NULL == fp[chnId])
            return -1;
    }

    for (i = 0; i < nr_pack; i++)
    {
        pack = &stream->pack[i];

        if (pack->length)
        {
            remSize = stream->streamSize - pack->offset;
            if (remSize < pack->length)
            {
                ret = fwrite((void*)(stream->virAddr + pack->offset), 1, remSize, fp[chnId]);
                if (ret != remSize)
                {
                    IMP_LOG_ERR(TAG, "stream write ret(%d) != pack[%d].remSize(%d) len:%d\n", ret, i, remSize, ret);
                    return -1;
                }
                ret = fwrite((void*)stream->virAddr, 1, pack->length - remSize, fp[chnId]);
                if (ret != (pack->length - remSize))
                {
                    IMP_LOG_ERR(TAG, "stream write ret(%d) != pack[%d].(length-remSize)(%d) len:%d\n", ret, i, (pack->length - remSize), ret);
                    return -1;
                }
            }
            else
            {
                ret = fwrite((void*)(stream->virAddr + pack->offset), 1, pack->length, fp[chnId]);
                if (ret != pack->length)
                {
                    IMP_LOG_ERR(TAG, "stream write ret(%d) != pack[%d].length(%d) len:%d\n", ret, i, pack->length, ret);
                    return -1;
                }
            }
        }
    }

    return 0;
}

/* 测试代码，用来保存拼包后的stream */
static int save_pack_stream(UINT_X chnId, LUX_ENCODE_STREAM_ST* pStream)
{
    if (NULL == pStream)
    {
        return 0;
    }
    int ret = 0;
    char saveName[32] = { 0 };
    char* formatTable[3] = { "h265", "h265", "jpg" };
    static FILE* fp[VIDEO_ENC_CHN_MAX] = { NULL };
    if (0 == chnId)
    {
        if (!fp[chnId])
        {
            sprintf(saveName, "/tmp/video_pack_stream_chn%d.%s", chnId, formatTable[chnId]);

            fp[chnId] = fopen(saveName, "wb");
            if (NULL == fp[chnId])
            {
                return -1;
            }
        }

        ret = fwrite(pStream->pAddr, 1, pStream->streamLen, fp[chnId]);
        if (ret != pStream->streamLen)
        {
            IMP_LOG_ERR(TAG, "stream write ret(%d) streamLen(%d)\n", ret, pStream->streamLen);
            return -1;
        }
        fflush(fp[chnId]);
    }
    return 0;
}
#endif

/**
 * @description: 编码码流拼包
 * @param [in]  pStream：sdk获取的码流结构
 * @param [out] pOutStream：输出拼包转换后的码流结构
 * @return 成功：0，失败：错误码；
 */
static INT_X LUX_Video_Encoder_PackStream(IMPEncoderStream* pStream, LUX_ENCODE_STREAM_ST* pOutStream, VOID_X* pTmpBuf)
{
    if ((NULL == pStream) || (NULL == pOutStream) || (NULL == pTmpBuf))
    {
        IMP_LOG_ERR(TAG, "NULL PTR!\n");
        return LUX_VIDEO_NULL_PRT;
    }
    UINT_X i = 0;
    UINT_X streamLen = 0;
    BOOL_X bNeedCpyTemp = LUX_FALSE;
    UINT_X reminSize = 0;
    IMPEncoderPack* pPack = NULL;

    /* 计算是否需要拼包 */
    for (i = 0; i < pStream->packCount; i++)
    {
        if ((pStream->streamSize - pStream->pack[i].offset) < pStream->pack[i].length)
        {
            bNeedCpyTemp = LUX_TRUE;
            printf("================= bNeedCpyTemp =====================\n");
            break;
        }
    }

    /* 需要拷贝到tmp缓存，即需要拼包 */
    if (bNeedCpyTemp)
    {
        for (i = 0; i < pStream->packCount; i++)
        {
            pPack = &pStream->pack[i];
            reminSize = pStream->streamSize - pPack->offset;

            if (reminSize < pPack->length)
            {
                /* pTmpBuf 大小 LUX_VENC_CHN_TMPBUF_LEN （512K），够用 */
                memcpy((void*)(pTmpBuf + streamLen), (void*)(pStream->virAddr + pPack->offset), reminSize);
                streamLen += reminSize;

                memcpy((void*)(pTmpBuf + streamLen), (void*)pStream->virAddr, pPack->length - reminSize);
                streamLen += pPack->length - reminSize;
            }
            else
            {
                memcpy((void*)(pTmpBuf + streamLen), (void*)(pStream->virAddr + pPack->offset), pPack->length);
                streamLen += pPack->length;
            }
        }

        pOutStream->pAddr = pTmpBuf;
        pOutStream->streamLen = streamLen;
        pOutStream->type = pPack ? pPack->sliceType : 0;
        pOutStream->timeStamp = pPack ? pPack->timestamp : getTime_getUs();
    }
    else    /* 不需要拷贝拼包，直接统计码流总长度 */
    {
        for (i = 0; i < pStream->packCount; i++)
        {
            pPack = &pStream->pack[i];
            streamLen += pStream->pack[i].length;
        }

        /* 不拼包时，赋值使用sdk接口内部指针 */
        pOutStream->pAddr = (void*)pStream->virAddr + pStream->pack[0].offset;
        pOutStream->streamLen = streamLen;
        pOutStream->type = pStream->pack[i - 1].sliceType;
        pOutStream->timeStamp = pStream->pack[i - 1].timestamp;
    }
    if (pOutStream->streamLen > 384 * 1024)
    {
        if (LUX_ENC_SLICE_I == pOutStream->type)
        {
            printf("p_frame->pts : IIIIIIIIIIIIIIIIIII \n");
        }
        else
        {
            printf("p_frame->pts : p \n");
        }
        printf("p_frame->size : %d \n", pOutStream->streamLen);
        printf("p_frame->pts : %d \n", pStream->seq);
    }
    return LUX_OK;
}

/**
 * @description: 通过码流通道模式获取到实际编码器通道参数
 * @param [in]  streamChn：通道模式枚举
 * @param [out]  pAttr：通道属性结构体
 * @return 成功：通道号，失败：-1；
 */
static INT_X LUX_Video_Encoder_GetChnAttr(LUX_STREAM_CHANNEL_EN channel, ENCODER_CHN_ATTR_ST** ppAttr)
{
    INT_X i = 0;
    INT_X j = 0;
    INT_X ret = LUX_ERR;

    for (i = 0; i < LUX_VENC_GRP_NUM; i++)
    {
        for (j = 0; j < LUX_VENC_CHN_NUM; j++)
        {
            if (g_VEncAttr->encGrpAttr[i].encChnAttr[j].bValid
                && g_VEncAttr->encGrpAttr[i].encChnAttr[j].streamChn == channel)
            {
                *ppAttr = &g_VEncAttr->encGrpAttr[i].encChnAttr[j];
                ret = LUX_OK;
                break;
            }
        }
    }

    return ret;
}

/**
 * @description: 请求通道I帧
 * @param [in] chnId：编码通道号
 * @return 成功：0，失败：错误码；
 */
INT_X LUX_Video_Encoder_RequestIDR(LUX_STREAM_CHANNEL_EN channel)
{
    if (channel < 0 || channel >= LUX_STREAM_BUTT)
    {
        IMP_LOG_ERR(TAG, "Start Encoder ERR chnId:%d!\n", channel);
        return LUX_VIDEO_INVALID_PARM;
    }

    INT_X ret = LUX_OK;
    ENCODER_CHN_ATTR_ST* pAttr = NULL;

    /* 根据通道模式匹配编码器通道 */
    ret = LUX_Video_Encoder_GetChnAttr(channel, &pAttr);
    if (LUX_OK != ret || NULL == pAttr)
    {
        IMP_LOG_ERR(TAG, "Start Encoder ERR chnId:%d channel:%d!\n", pAttr->chnId, channel);
        return LUX_ERR;
    }

    /* 通道未开启 */
    if (!pAttr->bStart)
    {
        IMP_LOG_WARN(TAG, "ERR chnId:%d channel:%d bStart:%d\n", pAttr->chnId, channel, pAttr->bStart);
        return ret;
    }

    Thread_Mutex_Lock(&pAttr->chnMux);

    ret = IMP_Encoder_RequestIDR(pAttr->chnId);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "IMP_Encoder_RequestIDR(%d) timeout\n", pAttr->chnId);
        ret = LUX_VIDEO_REQUEST_IDR_ERR;
    }

    Thread_Mutex_UnLock(&pAttr->chnMux);

    IMP_LOG_INFO(TAG, "Request chan:%d,%d IDR Frame Success:%d!\n", pAttr->chnId, channel);

    return ret;
}

/**
 * @description: 刷新通道码流数据，丢掉旧的数据，重新开始
 * @param [in] chnId：编码通道号
 * @return 成功：0，失败：错误码；
 */
INT_X LUX_Video_Encoder_FlushStream(LUX_STREAM_CHANNEL_EN channel)
{
    if (channel < 0 || channel >= LUX_STREAM_BUTT)
    {
        IMP_LOG_ERR(TAG, "Start Encoder ERR chnId:%d!\n", channel);
        return LUX_VIDEO_INVALID_PARM;
    }
    INT_X ret = LUX_OK;
    ENCODER_CHN_ATTR_ST* pAttr = NULL;

    /* 根据通道模式匹配编码器通道 */
    ret = LUX_Video_Encoder_GetChnAttr(channel, &pAttr);
    if (LUX_OK != ret || NULL == pAttr)
    {
        IMP_LOG_ERR(TAG, "Start Encoder ERR chnId:%d channel:%d!\n", pAttr->chnId, channel);
        return LUX_ERR;
    }

    /* 通道未开启 */
    if (!pAttr->bStart)
    {
        IMP_LOG_WARN(TAG, "ERR chnId:%d channel:%d bStart:%d\n", pAttr->chnId, channel, pAttr->bStart);
        return ret;
    }

    Thread_Mutex_Lock(&pAttr->chnMux);

    ret = IMP_Encoder_FlushStream(pAttr->chnId);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "IMP_Encoder_RequestIDR(%d) timeout\n", pAttr->chnId);
        ret = LUX_VIDEO_REQUEST_IDR_ERR;
    }

    Thread_Mutex_UnLock(&pAttr->chnMux);

    IMP_LOG_INFO(TAG, "Flush chan:%d,%d Stream Success:%d!\n", pAttr->chnId, channel);

    return ret;
}

/**
 * @description: 获取编码帧
 * @param [in] chnId：编码通道号
 * @param [out] pOutStream：输出拼包转换后的码流结构
 * @return 成功：0，失败：错误码；
 */
INT_X LUX_Video_Encoder_GetStream(LUX_STREAM_CHANNEL_EN channel, LUX_ENCODE_STREAM_ST* pOutStream)
{
    if ((channel < 0 || channel >= LUX_STREAM_BUTT) || NULL == pOutStream)
    {
        IMP_LOG_ERR(TAG, "Start Encoder ERR chnId:%d!\n", channel);
        return LUX_VIDEO_INVALID_PARM;
    }
    INT_X ret = LUX_OK;
    INT_X chnId = 0;
    UINT_X streamIdx = 0;
    IMPEncoderStream* pStream = NULL;
    ENCODER_CHN_ATTR_ST* pAttr = NULL;

    /* 根据通道模式匹配编码器通道 */
    ret = LUX_Video_Encoder_GetChnAttr(channel, &pAttr);
    if (LUX_OK != ret || NULL == pAttr)
    {
        IMP_LOG_ERR(TAG, "Start Encoder ERR chnId:%d channel:%d!\n", pAttr->chnId, channel);
        return LUX_ERR;
    }

    /* 通道未开启 */
    if (!pAttr->bStart)
    {
        return LUX_VIDEO_GET_STREAM_ERR;
    }

    Thread_Mutex_Lock(&pAttr->chnMux);
    do
    {

        /* 使用全局 streamIdx 变量记录取帧的位置，后续释放时需要 */
        chnId = pAttr->chnId;
        streamIdx = pAttr->streamIdx % LUX_VENC_CHN_STREAM_CNT;
        pStream = &pAttr->streamTmp[streamIdx];
        pAttr->streamIdx++;

        /* polling一帧数据 */
        ret = IMP_Encoder_PollingStream(chnId, 1000);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_Encoder_PollingStream(%d) timeout\n", chnId);
            ret = LUX_VIDEO_GET_STREAM_ERR;
            break;
        }

        /* Get H264 or H265 Stream */
        memset(pStream, 0, sizeof(IMPEncoderStream));
        ret = IMP_Encoder_GetStream(chnId, pStream, 1);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_Encoder_GetStream(%d) failed\n", chnId);
            ret = LUX_VIDEO_GET_STREAM_ERR;
            break;
        }
        

#ifdef SAVE_STREAM
        save_stream(chnId, pStream);
#endif
        /* 打包码流包，存在一帧多包和码流缓冲回头情况，需要拼包 */
        ret = LUX_Video_Encoder_PackStream(pStream, pOutStream, pAttr->pTmpBuf);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_Video_Encoder_PackStream(%d) failed\n", chnId);
            ret = LUX_VIDEO_GET_STREAM_ERR;
            break;
        }


#ifdef SAVE_STREAM


        if ((pStream->seq < 3000) && (LUX_STREAM_MAIN == channel))
        {
            char DateStr[40];
            time_t currTime;
            struct tm* currDate;
            time(&currTime);
            currTime = currTime + gettimeStampS();
            currDate = localtime(&currTime);
            memset(DateStr, 0, 40);
            strftime(DateStr, 40, "%Y-%m-%d %H:%M:%S", currDate);   /* %H:24小时制 %I:12小时制 */
            save_pack_stream(chnId, pOutStream);
            printf("[%s]save_pack_stream[%d]\n", DateStr, pStream->seq);
        }
#endif
        pOutStream->chnId = chnId;
        pOutStream->pts = pStream->seq;
        pOutStream->pPrivData = (VOID_X*)pStream;

    } while (0);
    Thread_Mutex_UnLock(&pAttr->chnMux);
    return ret;
}

/**
 * @description: 释放编码帧
 * @param [in] chnId：编码通道号
 * @param [in] pStream：输出拼包转换后的码流结构
 * @return 成功：0，失败：错误码；
 */
INT_X LUX_Video_Encoder_ReleaseStream(LUX_STREAM_CHANNEL_EN channel, LUX_ENCODE_STREAM_ST* pStream)
{
    if ((channel < 0 || channel >= LUX_STREAM_BUTT)
        || (NULL == pStream || NULL == pStream->pPrivData))
    {
        IMP_LOG_ERR(TAG, "Invalid param,channel:%d!\n", channel);
        return LUX_VIDEO_NULL_PRT;
    }
    UINT_X ret = LUX_ERR;
    ENCODER_CHN_ATTR_ST* pAttr = NULL;

    /* 根据通道模式匹配编码器通道 */
    ret = LUX_Video_Encoder_GetChnAttr(channel, &pAttr);
    if (LUX_OK != ret || NULL == pAttr)
    {
        IMP_LOG_ERR(TAG, "Start Encoder ERR chnId:%d channel:%d!\n", pAttr->chnId, channel);
        return LUX_ERR;
    }

    /* 通道未开启 */
    if (!pAttr->bStart)
    {
        return LUX_VIDEO_RLS_STREAM_ERR;
    }

    Thread_Mutex_Lock(&pAttr->chnMux);

    ret = IMP_Encoder_ReleaseStream(pStream->chnId, (IMPEncoderStream*)pStream->pPrivData);
    if (ret < 0)
    {
        IMP_LOG_ERR(TAG, "IMP_Encoder_ReleaseStream(%d) failed\n", pStream->chnId);
        ret = LUX_VIDEO_RLS_STREAM_ERR;
    }
    pStream->pPrivData = NULL;

    Thread_Mutex_UnLock(&pAttr->chnMux);

    return LUX_OK;
}

/**
 * @description: 获取编码通道参数
 * @param [in] chnId：编码通道号
 * @param [out] pVencParam：编码通道属性参数
 * @return 成功：0，失败：错误码；
 */
INT_X LUX_Video_Encoder_GetChnParam(LUX_STREAM_CHANNEL_EN channel, LUX_VENC_CHN_PARAM_ST* pVencParam)
{
    if ((channel < 0 || channel >= LUX_STREAM_BUTT) || NULL == pVencParam)
    {
        IMP_LOG_ERR(TAG, "invalid param channle:%d!\n", channel);
        return LUX_VIDEO_INVALID_PARM;
    }

    UINT_X ret = LUX_ERR;
    ENCODER_CHN_ATTR_ST* pAttr = NULL;
    IMPEncoderChnAttr impChnAttr;
    IMPEncoderEncType impEncType = IMP_ENC_TYPE_AVC;
    LUX_VIDEO_ENC_TYPE_EN encType = LUX_ENC_TYPE_H264;
    LUX_VENC_RC_MODE_EN encRcMode = LUX_VENC_RC_MODE_CBR;

    /* 根据通道模式匹配编码器通道 */
    ret = LUX_Video_Encoder_GetChnAttr(channel, &pAttr);
    if (LUX_OK != ret || NULL == pAttr)
    {
        IMP_LOG_ERR(TAG, "get chan attr ERR chnId:%d channel:%d!\n", pAttr->chnId, channel);
        return LUX_ERR;
    }

    Thread_Mutex_Lock(&pAttr->chnMux);
    do
    {
        /* 通道未开启 */
        if (!pAttr->bStart)
        {
            IMP_LOG_WARN(TAG, "ERR chnId:%d channel:%d bStart:%d\n", pAttr->chnId, channel, pAttr->bStart);
            break;
        }

        memset(&impChnAttr, 0, sizeof(impChnAttr));
        ret = IMP_Encoder_GetChnAttr(pAttr->chnId, &impChnAttr);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "get chan attr ERR chnId:%d channel:%d!\n", pAttr->chnId, channel);
            ret = LUX_VIDEO_GET_ATTR_ERR;
            break;
        }

        ret = IMP_Encoder_GetChnEncType(pAttr->chnId, &impEncType);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "get chan attr ERR chnId:%d channel:%d!\n", pAttr->chnId, channel);
            ret = LUX_VIDEO_GET_ATTR_ERR;
            break;
        }

        /* 编码类型转换 */
        if (IMP_ENC_TYPE_AVC == impEncType)
        {
            encType = LUX_ENC_TYPE_H264;
        }
        else if (IMP_ENC_TYPE_HEVC == impEncType)
        {
            encType = LUX_ENC_TYPE_H265;
        }
        else if (IMP_ENC_TYPE_JPEG == impEncType)
        {
            encType = LUX_ENC_TYPE_JPEG;
        }

        /* 码流控制类型转换 */
        if (IMP_ENC_RC_MODE_CBR == impChnAttr.rcAttr.attrRcMode.rcMode)
        {
            encRcMode = LUX_VENC_RC_MODE_CBR;
        }
        else if (IMP_ENC_RC_MODE_VBR == impChnAttr.rcAttr.attrRcMode.rcMode
                || IMP_ENC_RC_MODE_CAPPED_VBR == impChnAttr.rcAttr.attrRcMode.rcMode)
        {
            encRcMode = LUX_VENC_RC_MODE_VBR;
        }

        pVencParam->bEnable = LUX_TRUE;
        pVencParam->channel = channel;
        pVencParam->fps = impChnAttr.rcAttr.outFrmRate.frmRateDen ?
            (impChnAttr.rcAttr.outFrmRate.frmRateNum / impChnAttr.rcAttr.outFrmRate.frmRateDen) :
            VIDEO_ENC_MAIN_FRAMERATE;
        pVencParam->gop = (UINT_X)impChnAttr.gopAttr.uGopLength;
        pVencParam->width = (UINT_X)impChnAttr.encAttr.uWidth;
        pVencParam->height = (UINT_X)impChnAttr.encAttr.uHeight;
        pVencParam->bitrate = (UINT_X)BITRATE_720P_Kbs;
        pVencParam->rcMode = encRcMode;
        pVencParam->format = encType;
    } while (0);

    Thread_Mutex_UnLock(&pAttr->chnMux);

    return ret;
}

/**
 * @description: 开始编码
 * @param [in] chnId：编码通道号
 * @return 成功：0，失败：错误码；
 */
INT_X LUX_Video_Encoder_Start(LUX_STREAM_CHANNEL_EN channel)
{
    INT_X ret = LUX_OK;
    ENCODER_CHN_ATTR_ST* pAttr = NULL;

    /* 根据通道模式匹配编码器通道 */
    ret = LUX_Video_Encoder_GetChnAttr(channel, &pAttr);
    if (LUX_OK != ret || NULL == pAttr)
    {
        IMP_LOG_ERR(TAG, "Start Encoder ERR chnId:%d channel:%d!\n", pAttr->chnId, channel);
        return LUX_ERR;
    }
    /* 重复start */
    if (pAttr->bStart)
    {
        IMP_LOG_WARN(TAG, "ERR chnId:%d channel:%d bStart:%d\n", pAttr->chnId, channel, pAttr->bStart);
        return LUX_VIDEO_CHAN_START_ERR;
    }

    Thread_Mutex_Lock(&pAttr->chnMux);
    do {
        ret = IMP_Encoder_FlushStream(pAttr->chnId);
        if (ret < 0)
        {
            IMP_LOG_ERR(TAG, "IMP_Encoder_FlushStream(%d) failed\n", pAttr->chnId);
        }

        ret = IMP_Encoder_StartRecvPic(pAttr->chnId);
        if (ret < 0)
        {
            IMP_LOG_ERR(TAG, "IMP_Encoder_StartRecvPic(%d) failed\n", pAttr->chnId);
            ret = LUX_VIDEO_CHAN_START_ERR;
            break;
        }
        pAttr->bStart = LUX_TRUE;
    } while (0);
    Thread_Mutex_UnLock(&pAttr->chnMux);

    return ret;
}

/**
 * @description: 停止编码
 * @param [in] chnId：编码通道号
 * @return 成功：0，失败：错误码；
 */
INT_X LUX_Video_Encoder_Stop(LUX_STREAM_CHANNEL_EN channel)
{
    INT_X ret = LUX_OK;
    ENCODER_CHN_ATTR_ST* pAttr = NULL;

    /* 根据通道模式匹配编码器通道 */
    ret = LUX_Video_Encoder_GetChnAttr(channel, &pAttr);
    if (LUX_OK != ret || NULL == pAttr)
    {
        IMP_LOG_ERR(TAG, "Start Encoder ERR chnId:%d channel:%d!\n", pAttr->chnId, channel);
        return LUX_ERR;
    }

    /* 重复stop */
    if (!pAttr->bStart)
    {
        IMP_LOG_ERR(TAG, "Start Encoder ERR chnId:%d channel:%d bStart:%d\n", pAttr->chnId, channel, pAttr->bStart);
        return LUX_VIDEO_CHAN_STOP_ERR;
    }

    Thread_Mutex_Lock(&pAttr->chnMux);
    do
    {
        ret = IMP_Encoder_StopRecvPic(pAttr->chnId);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_Encoder_StopRecvPic(%d) failed\n", pAttr->chnId);
            ret = LUX_VIDEO_CHAN_STOP_ERR;
            break;
        }
        pAttr->bStart = LUX_FALSE;
    } while (0);
    Thread_Mutex_UnLock(&pAttr->chnMux);

    return ret;
}

/**
 * @description: 使能编码模块
 * @param [void]
 * @return 成功：0，失败：错误码；
 */
INT_X LUX_Video_Encoder_Init(INT_X Venc_Param)
{
    INT_X i = 0;
    INT_X ret = LUX_ERR;
    LUX_MODULES_BIND_ST bind;
    CHAR_X sensorName[64] = { 0 };
    LUX_COMM_FUNC_SNS_WH sensorWH;

    if (g_VEncAttr->bInit)
    {
        IMP_LOG_ERR(TAG, "LUX_Video_Encoder_Init reInit, return OK!\n");
        return LUX_OK;
    }

    /*读取配置文件的sensor名字*/
    strcpy(sensorName, g_sensor_name);
    //ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "sensor", "name", sensorName);
    //if (LUX_OK != ret) {
    //    IMP_LOG_ERR(TAG, "LUX_INIParse_GetString failed,error [0x%x]\n", ret);
    //    return LUX_ERR;
    //}
    //else {
    //    MYTRACE("LUX_INIParse_GetSensor is success\n");
    //}



    /*获取镜头分辨率大小*/
    ret = LUX_COMM_GetSensorWH(sensorName, &sensorWH);
    if (LUX_OK != ret) {
        IMP_LOG_ERR(TAG, "LUX_INIParse_GetString failed,error [0x%x]\n", ret);
        return LUX_ERR;
    }
    else {
        MYTRACE("LUX_COMM_GetSensorWH is success\n");
    }

    /*设置编码grop0，通道零0的编码分辨率大小为sensor分辨率大小*/

    if (LUX_FALSE == Venc_Param)
    {
        g_VEncAttr->encGrpAttr[0].encChnAttr[0].chnAttr.encAttr.uWidth = sensorWH.width;
        g_VEncAttr->encGrpAttr[0].encChnAttr[0].chnAttr.encAttr.uHeight = sensorWH.height;
    }

    else if (LUX_TRUE == Venc_Param)
    {
        g_VEncAttr->encGrpAttr[0].encChnAttr[0].chnAttr.encAttr.uWidth = 640;
        g_VEncAttr->encGrpAttr[0].encChnAttr[0].chnAttr.encAttr.uHeight = 360;
    }
    /* video编码器 GROUP+CHAN 初始化 */
    for (i = 0; i < LUX_VENC_GRP_NUM; i++)
    {
        /* 创建 group ，用于传输码流和抓图 */
        ret = IMP_Encoder_CreateGroup(i);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_Encoder_CreateGroup(%d) error!\n", i);
            return LUX_VIDEO_CHAN_CREATE_ERR;
        }
        else {
            MYTRACE("IMP_Encoder_CreateGroup is success,grpID:%d\n", i);
        }

        /* 绑定venc通道到osd通道 */
        memset(&bind, 0, sizeof(bind));
        bind.srcChan = i;
        bind.srcModule = LUX_MODULS_OSD;
        bind.dstChan = i;
        bind.dstModule = LUX_MODULS_VENC;
        ret = LUX_COMM_Bind_AddModule(&bind);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_COMM_Bind_AddModule(%d) venc error!\n", i);
            return LUX_VIDEO_CHAN_CREATE_ERR;
        }
        else {
            MYTRACE("LUX_COMM_Bind_AddModule is success,grpID:%d\n", i);
        }

        /* 创建 venc 通道, 将通道注册到 group */
        ret = LUX_Video_Encoder_CreateChn(i);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_Video_Encoder_CreateChn(%d) error!\n", i);
            return LUX_VIDEO_CHAN_CREATE_ERR;
        }

    }
    g_VEncAttr->bInit = LUX_TRUE;

    return LUX_OK;
}

/**
 * @description: 销毁编码模块
 * @param [void]
 * @return 成功：0，失败：错误码；
 */
INT_X LUX_Video_Encoder_Exit()
{
    INT_X ret = LUX_OK;
    INT_X i = 0;

    if (!g_VEncAttr->bInit)
    {
        IMP_LOG_ERR(TAG, "LUX_Video_Encoder_Exit, src not Init, return OK!\n");
        return LUX_OK;
    }

    for (i = 0; i < LUX_VENC_GRP_NUM; i++)
    {
        /* 销毁video编码器 CHANNEL */
        ret = LUX_Video_Encoder_DestroyChn(i);
        if (ret < 0)
        {
            IMP_LOG_ERR(TAG, "LUX_Video_Encoder_DestroyChn(%d) error: %#x\n", i, ret);
            //return LUX_VIDEO_CHAN_DESTROY_ERR;
        }

        /* 销毁 video编码器的group */
        ret = IMP_Encoder_DestroyGroup(i);
        if (ret < 0)
        {
            IMP_LOG_ERR(TAG, "IMP_Encoder_DestroyGroup grpId(%d)\n", i);
            //return LUX_VIDEO_GROUP_DESTROY_ERR;
        }
    }
    g_VEncAttr->bInit = LUX_FALSE;

    return LUX_OK;
}




/***********************************分辨率的相关函数实现(haokj)********************************************/

typedef struct LUX_VIDEO_ENC_ATTR LUX_VIDEO_ENC_ATTR_ST;
extern   LUX_FSRC_ATTR_ST* g_framSrcAttr;
extern  LUX_VIDEO_ENC_ATTR_ST* g_VEncAttr;



INT_X LUX_Video_Change_Resolution_Res_Init(fsChnNum)
{
    MYTRACEING("LUX_Video_Change_Resolution_Res_Init \n");
    INT_X ret = LUX_OK;
    INT_X i = 0;
    INT_X chnID;
    BOOL_X FlipStat_h;
    BOOL_X FlipStat_v;
    LUX_ISP_SNS_ATTR_EN senceMirFlip;

#if 1
    chnID = fsChnNum;

    MYTRACE("*********chnID = %d********\n", chnID);

    /*Step.1 FrameSource_Init*/
    if (LUX_TRUE == g_framSrcAttr->bInit)
    {
        IMP_LOG_ERR(TAG, "LUX_FSrc_Init has been initialized!\n");
        return LUX_FSRC_ALREADY_INIT;
    }

    if (g_framSrcAttr->fsChn[chnID].enable)
    {
        ret = IMP_FrameSource_CreateChn(g_framSrcAttr->fsChn[chnID].index,
                                    &g_framSrcAttr->fsChn[chnID].fsChnAttr);
        if (LUX_OK != ret) {
            IMP_LOG_ERR(TAG, "IMP_FrameSource_CreateChn(chn%d) error !\n", chnID);
            return LUX_ERR;
        }

        ret = IMP_FrameSource_SetChnAttr(g_framSrcAttr->fsChn[chnID].index,
                                    &g_framSrcAttr->fsChn[chnID].fsChnAttr);
        if (LUX_OK != ret) {
            IMP_LOG_ERR(TAG, "IMP_FrameSource_SetChnAttr(chn%d) error !\n", chnID);
            return LUX_ERR;
        }

        /* 使能framesource通道，开始处理数据 */
        ret = IMP_FrameSource_EnableChn(g_framSrcAttr->fsChn[chnID].index);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_FrameSource_EnableChn(%d) error: %d\n",
                        g_framSrcAttr->fsChn[chnID].index, ret);
            return LUX_ERR;
        }

        /*设置子码流通道帧缓存为1*/
        if (chnID == LUX_STREAM_SUB)
        {
            ret = IMP_FrameSource_SetFrameDepth(chnID, 1);
            if (LUX_OK != ret)
            {
                IMP_LOG_ERR(TAG, "IMP_FrameSource_SetFrameDepth(%d) error: %d\n", 1, ret);
                return LUX_ERR;
            }
        }

        ret = Thread_Mutex_Create(&g_framSrcAttr->fsChn[chnID].mux);
        if (LUX_OK != ret) {
            IMP_LOG_ERR(TAG, "Thread_Mutex_Create(chn%d) failed !\n", chnID);
            return LUX_ERR;
        }
    }

    FlipStat_h = g_configData->image_config.hflip;
	FlipStat_v = g_configData->image_config.vflip;


    senceMirFlip = LUX_ISP_SNS_NORMAL;

    if ((FlipStat_h ^ g_sensor_h_flip) && !(FlipStat_v ^ g_sensor_v_flip))
    {
        senceMirFlip = LUX_ISP_SNS_MIRROR;
    }
    else if (!(FlipStat_h ^ g_sensor_h_flip) && (FlipStat_v ^ g_sensor_v_flip))
    {
        senceMirFlip = LUX_ISP_SNS_FLIP;
    }
    else if ((FlipStat_h ^ g_sensor_h_flip) && (FlipStat_v ^ g_sensor_v_flip))
    {
        senceMirFlip = LUX_ISP_SNS_MIRROR_FLIP;
    }

#if 1
    if (0 == fsChnNum)
    {
        ret = LUX_ISP_SetSenceFlip(senceMirFlip);
        if (LUX_OK != ret)
        {
            MYERROR("LUX_ISP_SetSenceFlip failed, return error[0x%x]\n", ret);
            return LUX_ISP_INVALID_PARM;
        }
    }
#endif

    g_framSrcAttr->bInit = LUX_TRUE;

    /*Step.2 Encoder_Init*/
    if (g_VEncAttr->bInit)
    {
        IMP_LOG_ERR(TAG, "LUX_Video_Encoder_Init reInit, return OK!\n");
        return LUX_OK;
    }

    /* 创建 group ，用于传输码流和抓图 */
    ret = IMP_Encoder_CreateGroup(chnID);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "IMP_Encoder_CreateGroup(%d) error!\n", chnID);
        return LUX_VIDEO_CHAN_CREATE_ERR;
    }
    else {
        MYTRACE("IMP_Encoder_CreateGroup is success,grpID:%d\n", chnID);
    }

    /* 创建 venc 通道, 将通道注册到 group */
    ret = LUX_Video_Encoder_CreateChn(chnID);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_Video_Encoder_CreateChn(%d) error!\n", chnID);
        return LUX_VIDEO_CHAN_CREATE_ERR;
    }
    else {
        MYTRACE("LUX_Video_Encoder_CreateChn is success,grpID:%d\n", chnID);
    }
    g_VEncAttr->bInit = LUX_TRUE;

    /* Step.3 Bind */
    ret = LUX_COMM_Bind_Connect();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "Encoder init failed\n");
        return LUX_ERR;
    }
    else {
        MYTRACE("LUX_COMM_Bind_Connect is success\n");
    }

    /* Step.4 Encoder_Start */
    ret = LUX_Video_Encoder_Start(chnID);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "ImpStreamOn failed chan:%d\n", chnID);
        return LUX_ERR;
    }
    else {
        MYTRACE("LUX_STREAM_MAIN_Video_Encoder_Start is success\n");
    }
#endif

    return LUX_OK;
}


INT_X LUX_Video_Change_Resolution_Res_Deinit(fsChnNum)
{
    INT_X ret = LUX_OK;
    INT_X i = 0;
    INT_X chnID;

    chnID = fsChnNum;

    MYTRACE("*********chnID = %d********\n", chnID);

    /* Step.1 Encoder_Stop */
#if 1
    ret = LUX_Video_Encoder_Stop(chnID);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "ImpStreamOff failed chan:%d\n", LUX_STREAM_MAIN);
        return LUX_ERR;
    }
    else {
        MYTRACE("LUX_STREAM_Video_Encoder_Stop is success\n");
    }
#endif

    /*设置子码流通道帧缓存为0*/
    if (chnID == LUX_STREAM_SUB)
    {
        ret = IMP_FrameSource_SetFrameDepth(chnID, 0);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_FrameSource_SetFrameDepth(%d) error: %d\n", 0, ret);
            return LUX_ERR;
        }
    }

    ret = IMP_FrameSource_DisableChn(g_framSrcAttr->fsChn[chnID].index);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "IMP_FrameSource_DisableChn(%d) error: %d\n",
                    g_framSrcAttr->fsChn[chnID].index, ret);
        return LUX_ERR;
    }

     /* Step.2 Unbind */
    ret = LUX_COMM_Bind_DisConnect();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_COMM_Bind_DisConnect failed\n");
        return LUX_ERR;
    }
    else {
        MYTRACE("LUX_COMM_Bind_DisConnect is success\n");
    }

    /* Step.3 Encoder_Exit */
    if (!g_VEncAttr->bInit)
    {
        IMP_LOG_ERR(TAG, "LUX_Video_Encoder_Exit, src not Init, return OK!\n");
        return LUX_OK;
    }
    /* 销毁video编码器 CHANNEL */
    ret = LUX_Video_Encoder_DestroyChn(chnID);
    if (ret < 0)
    {
        IMP_LOG_ERR(TAG, "LUX_Video_Encoder_DestroyChn(%d) error: %#x\n", chnID, ret);
        //return LUX_VIDEO_CHAN_DESTROY_ERR;
    }

    /* 销毁 video编码器的group */
    ret = IMP_Encoder_DestroyGroup(chnID);
    if (ret < 0)
    {
        IMP_LOG_ERR(TAG, "IMP_Encoder_DestroyGroup grpId(%d)\n", chnID);
        return LUX_ERR;
    }
    g_VEncAttr->bInit = LUX_FALSE;


    /* Step.4 LUX_FSrc_Exit */
    if (LUX_FALSE == g_framSrcAttr->bInit)
    {
        IMP_LOG_ERR(TAG, "LUX_FSrc_Init has not been initialized!\n");
        return LUX_FSRC_NO_INIT;
    }

    if (g_framSrcAttr->fsChn[chnID].enable)
    {
        ret = IMP_FrameSource_DestroyChn(chnID);
        if (LUX_OK != ret) {
            IMP_LOG_ERR(TAG, "IMP_FrameSource_DestroyChn(%d) error: %d\n", chnID, ret);
            return LUX_ERR;
        }

        ret = Thread_Mutex_Destroy(&g_framSrcAttr->fsChn[chnID].mux);
        if (LUX_OK != ret) {
            IMP_LOG_ERR(TAG, "Thread_Mutex_Destroy(chn%d) failed.\n", chnID);
            return LUX_ERR;
        }

    }

    return LUX_OK;
}

#if 0
/*按照君正SDK的demo所写的例程，可以作为参考，后续如有需要可以使用--HaoKaiJun*/
INT_X LUX_Video_Change_Resolution_Res_Deinit_1(fsChnNum)
{
    INT_X ret = LUX_OK;
    INT_X i = 0;
    INT_X chnID;

    chnID = fsChnNum;

    MYTRACE("*********chnID = %d********\n", chnID);

    /*Step.1 Encoder_Stop*/
    ret = LUX_Video_Encoder_Stop(chnID);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "ImpStreamOff failed chan:%d\n", LUX_STREAM_MAIN);
        return LUX_ERR;
    }
    else {
        MYTRACE("LUX_STREAM_Video_Encoder_Stop is success\n");
    }

    /* Step.2 Framesource_streamoff */
    ret = IMP_FrameSource_DisableChn(chnID); 
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "IMP_FrameSource_DisableChn(%d) error: %d\n", fsChnNum, ret);
        return -1;
    }
    else {
        MYTRACE("IMP_FrameSource_DisableChn is success\n");
    }

   /* Step.3 UnBind*/
    ret = LUX_COMM_Bind_DisConnect();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_COMM_Bind_DisConnect failed\n");
        return LUX_ERR;
    }
    else {
        MYTRACE("LUX_COMM_Bind_DisConnect is success\n");
    } 

    /*Step.4 Encoder exit*/
    if (!g_VEncAttr->bInit)
    {
        IMP_LOG_ERR(TAG, "LUX_Video_Encoder_Exit, src not Init, return OK!\n");
        return LUX_OK;
    }

    int chnNum = 0;
    IMPEncoderChnStat chn_stat;
    chnNum = chnID;
    memset(&chn_stat, 0, sizeof(IMPEncoderChnStat));
    ret = IMP_Encoder_Query(chnNum, &chn_stat);
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "IMP_Encoder_Query(%d) error: %d\n", chnNum, ret);
        return -1;
    }
    else {
        MYTRACE("IMP_Encoder_Query is success\n");
    }

    if (chn_stat.registered) {
        ret = IMP_Encoder_UnRegisterChn(chnNum);
        if (ret < 0) {
            IMP_LOG_ERR(TAG, "IMP_Encoder_UnRegisterChn(%d) error: %d\n", chnNum, ret);
            return -1;
        }
        else {
            MYTRACE("IMP_Encoder_UnRegisterChn is success\n");
        }

        ret = IMP_Encoder_DestroyChn(chnNum);
        if (ret < 0) {
            IMP_LOG_ERR(TAG, "IMP_Encoder_DestroyChn(%d) error: %d\n", chnNum, ret);
            return -1;
        }
        else {
            MYTRACE("IMP_Encoder_DestroyChn is success\n");
        }

        ret = IMP_Encoder_DestroyGroup(chnNum);
        if (ret < 0) {
            IMP_LOG_ERR(TAG, "IMP_Encoder_DestroyGroup(%d) error: %d\n", chnNum, ret);
            return -1;
        }
        else {
            MYTRACE("IMP_Encoder_DestroyGroup is success\n");
        }

    }
    g_VEncAttr->bInit = LUX_FALSE;

    /*Step.5 FrameSource exit*/
    if (LUX_FALSE == g_framSrcAttr->bInit)
    {
        IMP_LOG_ERR(TAG, "LUX_FSrc_Init has not been initialized!\n");
        return LUX_FSRC_NO_INIT;
    }

    if (g_framSrcAttr->fsChn[chnID].enable)
    {
        ret = IMP_FrameSource_DestroyChn(chnID);  
        if (LUX_OK != ret) {
            IMP_LOG_ERR(TAG, "IMP_FrameSource_DestroyChn(%d) error: %d\n", chnID, ret);
            return LUX_ERR;
        }
        else {
            MYTRACE("IMP_FrameSource_DestroyChn is success\n");
        }

        ret = Thread_Mutex_Destroy(&g_framSrcAttr->fsChn[chnID].mux);
        if (LUX_OK != ret) {
            IMP_LOG_ERR(TAG, "Thread_Mutex_Destroy(chn%d) failed.\n", chnID);
            return LUX_ERR;
        }
        else {
            MYTRACE("Thread_Mutex_Destroy is success\n");
        }
    }
       
    return LUX_OK;
}

/*按照君正SDK的demo所写的例程，可以作为参考，后续如有需要可以使用--HaoKaiJun*/
INT_X LUX_Video_Change_Resolution_Res_Init_1(fsChnNum)
{
    INT_X ret = LUX_OK;
    INT_X i = 0;
    INT_X chnID;

    chnID = fsChnNum;

    /* Step.1 FrameSource init */
    if (LUX_TRUE == g_framSrcAttr->bInit)
    {
        IMP_LOG_ERR(TAG, "LUX_FSrc_Init has been initialized!\n");
        return LUX_FSRC_ALREADY_INIT;
    }

    if (g_framSrcAttr->fsChn[chnID].enable)
    {
        ret = IMP_FrameSource_CreateChn(g_framSrcAttr->fsChn[chnID].index,
                                    &g_framSrcAttr->fsChn[chnID].fsChnAttr);
        if (LUX_OK != ret) {
            IMP_LOG_ERR(TAG, "IMP_FrameSource_CreateChn(chn%d) error !\n", chnID);
            return LUX_ERR;
        }
        else {
            MYTRACE("IMP_FrameSource_CreateChn is success\n");
        }

        ret = IMP_FrameSource_SetChnAttr(g_framSrcAttr->fsChn[chnID].index,
                                    &g_framSrcAttr->fsChn[chnID].fsChnAttr);
        if (LUX_OK != ret) {
            IMP_LOG_ERR(TAG, "IMP_FrameSource_SetChnAttr(chn%d) error !\n", chnID);
            return LUX_ERR;
        }
        else {
            MYTRACE("IMP_FrameSource_SetChnAttr is success\n");
        }

        ret = Thread_Mutex_Create(&g_framSrcAttr->fsChn[chnID].mux);
        if (LUX_OK != ret) {
            IMP_LOG_ERR(TAG, "Thread_Mutex_Create(chn%d) failed !\n", chnID);
            return LUX_ERR;
        }
        else {
            MYTRACE("Thread_Mutex_Create is success\n");
        }

    }

    /* Step.2 Create encoder group */

    if (g_VEncAttr->bInit)
    {
        IMP_LOG_ERR(TAG, "LUX_Video_Encoder_Init reInit, return OK!\n");
        return LUX_OK;
    }

    /* 创建 group ，用于传输码流和抓图 */
    ret = IMP_Encoder_CreateGroup(chnID);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "IMP_Encoder_CreateGroup(%d) error!\n", chnID);
        return LUX_VIDEO_CHAN_CREATE_ERR;
    }
    else {
        MYTRACE("IMP_Encoder_CreateGroup is success,grpID:%d\n", chnID);
    }

    /* Step.3 Encoder init */
    int chnNum = 0;
    int s32picWidth = 0, s32picHeight = 0;
    IMPFSChnAttr* imp_chn_attr_tmp;
    IMPEncoderChnAttr channel_attr;
    IMPFSI2DAttr sti2dattr;

    imp_chn_attr_tmp = &g_framSrcAttr->fsChn[chnID].fsChnAttr;
    chnNum = chnID;

    memset(&channel_attr, 0, sizeof(IMPEncoderChnAttr));
    memset(&sti2dattr, 0, sizeof(IMPFSI2DAttr));

    ret = IMP_FrameSource_GetI2dAttr(chnNum, &sti2dattr);
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "IMP_FrameSource_GetI2dAttr error !\n");
        return -1;
    }

    if ((1 == sti2dattr.i2d_enable) &&
        ((sti2dattr.rotate_enable) && (sti2dattr.rotate_angle == 90 || sti2dattr.rotate_angle == 270))) {
        s32picWidth = (g_framSrcAttr->fsChn[chnID].fsChnAttr.picWidth);/*this depend on your sensor or channels*/
        s32picHeight = (g_framSrcAttr->fsChn[chnID].fsChnAttr.picHeight);

    }
    else {
        s32picWidth = g_framSrcAttr->fsChn[chnID].fsChnAttr.picWidth;
        s32picHeight = g_framSrcAttr->fsChn[chnID].fsChnAttr.picHeight;
    }
    float ratio = 1;
    if (((uint64_t)s32picWidth * s32picHeight) > (1280 * 720)) {
        ratio = log10f(((uint64_t)s32picWidth * s32picHeight) / (1280 * 720.0)) + 1;
    }
    else {
        ratio = 1.0 / (log10f((1280 * 720.0) / ((uint64_t)s32picWidth * s32picHeight)) + 1);
    }
    ratio = ratio > 0.1 ? ratio : 0.1;
    unsigned int uTargetBitRate = BITRATE_720P_Kbs * ratio;
    printf("rcMode:%d.\n", S_RC_METHOD);
    ret = IMP_Encoder_SetDefaultParam(&channel_attr, g_VEncAttr->encGrpAttr[chnID].encChnAttr[chnID].chnAttr.encAttr.eProfile, S_RC_METHOD,
            s32picWidth, s32picHeight,
            imp_chn_attr_tmp->outFrmRateNum, imp_chn_attr_tmp->outFrmRateDen,
            imp_chn_attr_tmp->outFrmRateNum * 2 / imp_chn_attr_tmp->outFrmRateDen, 2,
            (S_RC_METHOD == IMP_ENC_RC_MODE_FIXQP) ? 35 : -1,
            uTargetBitRate);
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "IMP_Encoder_SetDefaultParam(%d) error !\n", chnNum);
        return -1;
    }


    /* 创建 venc 通道, 将通道注册到 group */
    ret = LUX_Video_Encoder_CreateChn(chnID);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_Video_Encoder_CreateChn(%d) error!\n", chnID);
        return LUX_VIDEO_CHAN_CREATE_ERR;
    }
    else {
        MYTRACE("LUX_Video_Encoder_CreateChn is success,grpID:%d\n", chnID);
    }
    g_VEncAttr->bInit = LUX_TRUE;


    /* Step.4 Bind */
    ret = LUX_COMM_Bind_Connect();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "Encoder init failed\n");
        return LUX_ERR;
    }
    else {
        MYTRACE("LUX_COMM_Bind_Connect is success\n");
    }

    /* Step.5 Stream On */
    ret = IMP_FrameSource_EnableChn(g_framSrcAttr->fsChn[chnID].index);
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "IMP_FrameSource_EnableChn error: %d\n", ret);
        return -1;
    }

    ret = LUX_Video_Encoder_Start(chnID);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "ImpStreamOn failed chan:%d\n", chnID);
        return LUX_ERR;
    }
    else {
        MYTRACE("LUX_STREAM_MAIN_Video_Encoder_Start is success\n");
    }

    return 0;
}
#endif
INT_X LUX_Video_Change_Resolution(int fsChnNum, int width_value, int height_value)
{
    INT_X ret = LUX_OK;

    //ret = LUX_Video_Change_Resolution_Res_Deinit_1(fsChnNum);
    
    ret = LUX_Video_Change_Resolution_Res_Deinit(fsChnNum);
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "sample_res_deinit failed\n");
        return -1;
    }
    else {
        MYTRACE("LUX_Video_Change_Resolution_Res_Deinit is success\n");
    }

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

    /*********视频源的值初始化***********/
    g_framSrcAttr->bInit = LUX_FALSE;

    /* 通道0（主码流）视频源参数 */
    g_framSrcAttr->fsChn[0].index = CH0_INDEX;
    g_framSrcAttr->fsChn[0].enable = CHN0_EN;
    g_framSrcAttr->fsChn[0].fsChnAttr.pixFmt = PIX_FMT_NV12;
    g_framSrcAttr->fsChn[0].fsChnAttr.picWidth = width_value;
    g_framSrcAttr->fsChn[0].fsChnAttr.picHeight = height_value;
    g_framSrcAttr->fsChn[0].fsChnAttr.outFrmRateNum = SENSOR_FRAME_RATE_NUM;
    g_framSrcAttr->fsChn[0].fsChnAttr.outFrmRateDen = SENSOR_FRAME_RATE_DEN;
    g_framSrcAttr->fsChn[0].fsChnAttr.nrVBs = 3;
    g_framSrcAttr->fsChn[0].fsChnAttr.type = FS_PHY_CHANNEL;
    /*初始化图片的相关属性*/
    g_framSrcAttr->fsChn[0].fsChnAttr.scaler.enable = LUX_TRUE;
    g_framSrcAttr->fsChn[0].fsChnAttr.scaler.outwidth = width_value;
    g_framSrcAttr->fsChn[0].fsChnAttr.scaler.outheight = height_value;
    g_framSrcAttr->fsChn[0].fsChnAttr.crop.enable = 1;
    g_framSrcAttr->fsChn[0].fsChnAttr.crop.top = 0;
    g_framSrcAttr->fsChn[0].fsChnAttr.crop.left = 0;
    g_framSrcAttr->fsChn[0].fsChnAttr.crop.width = width_value;
    g_framSrcAttr->fsChn[0].fsChnAttr.crop.height = height_value;

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

    //ret = LUX_Video_Change_Resolution_Res_Init_1(fsChnNum);
    
    ret = LUX_Video_Change_Resolution_Res_Init(fsChnNum);
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "sample_res_init failed\n");
        return -1;
    }
    else {
        MYTRACE("LUX_Video_Change_Resolution_Res_Init is success\n");
    }

    return LUX_OK;
}


//获取系统的分辨率
INT_X LUX_Video_Get_Resolution(int fsChnNum, int* picWidth, int* picHeight)
{
    INT_X ret = LUX_OK;
    IMPFSChnAttr fsChnAttr;

    memset(&fsChnAttr, 0, sizeof(IMPFSChnAttr));

    ret = IMP_FrameSource_GetChnAttr(fsChnNum, &fsChnAttr);
    if (ret < LUX_OK) {
        IMP_LOG_ERR(TAG, "Call IMP_FrameSource_GetChnAttr fial.\n");
        return ret;
    }

    *picWidth = fsChnAttr.picWidth;
    *picHeight = fsChnAttr.picHeight;

    return ret;
}

/*获取指定通道分辨率*/
INT_X LUX_Video_Fnuc_Get_Resoulution(int fsChnNum, int* picWidth, int* picHeigth)
{
    INT_X ret = LUX_OK;

    ret = LUX_Video_Get_Resolution(fsChnNum, picWidth, picHeigth);
    if (ret < LUX_OK)
    {
        IMP_LOG_ERR(TAG, "Call xcam_video_get_resolution fail.\n");
        return ret;
    }

    return ret;
}
/**********************************帧率的相关函数实现(haokj)********************************************/



INT_X LUX_Video_Set_Isp_Fps(uint32_t impvi_num,uint32_t *fps_num, uint32_t *fps_den)
{
    INT_X ret = LUX_OK;

    if ((fps_num <= 0) || (fps_den <= 0)) {
        return LUX_ERR;
    }

    // if ((fps_num == stream_attr.fps_num) && (fps_den == stream_attr.fps_den)) {
    //     IMP_LOG_INFO(TAG, "The current configuration is the same as this configuration.\n");
    //     return LUX_OK;
    // }
#ifdef PLATFORM_T40
    ret = IMP_ISP_Tuning_SetSensorFPS(impvi_num, fps_num, fps_den);
    if (ret < LUX_OK) {
        IMP_LOG_ERR(TAG, "xcam_video_set_isp_fps Call IMP_ISP_SetSensor fail.\n");
        return ret;
    }
#else
    ret = IMP_ISP_Tuning_SetSensorFPS(*fps_num,*fps_den);
    if (ret < LUX_OK) {
        IMP_LOG_ERR(TAG, "xcam_video_set_isp_fps Call IMP_ISP_SetSensor fail.\n");
        return ret;
    }
#endif
    /*
    ret = xcam_conf_set_video_isp_fps(fps_num,fps_den);
    if (ret < LUX_OK) {
        LOG_INF(LOG_TAG,"Failed to save frame rate to json profile.\n");
    }
    */
    //_xcam_video_syn_isp_fps(fps_num,fps_den);

    return ret;
}

//获取系统帧率
int LUX_Video_Get_Isp_Fps(uint32_t* fps_num, uint32_t* fps_den)
{
    int ret = LUX_OK;

    if ((fps_num == NULL) || (fps_den == NULL)) {
        return LUX_ERR;
    }
#ifdef PLATFORM_T40
    ret = IMP_ISP_Tuning_GetSensorFPS(0, fps_num, fps_den);
    if (ret < LUX_OK) {
        IMP_LOG_ERR(TAG, "xcam_video_get_isp_fps call IMP_ISP_Tuning_GetSensorFPS Fail.\n");
        return ret;
    }
#else
    ret = IMP_ISP_Tuning_GetSensorFPS(fps_num, fps_den);
    if (ret < XCAM_SUCCESS) {
        IMP_LOG_ERR(TAG, "xcam_video_get_isp_fps call IMP_ISP_Tuning_GetSensorFPS Fail.\n");
        return ret;
    }
#endif

    return ret;
}


/*获取帧率*/
INT_X LUX_Video_Func_Get_Fps(uint32_t* fps_num, uint32_t* fps_den)
{
    INT_X ret = LUX_OK;

    ret = LUX_Video_Get_Isp_Fps(fps_num, fps_den);
    if (ret < LUX_OK)
    {
        IMP_LOG_ERR(TAG, "Call xcam_video_get_fps fail.\n");
        return ret;
    }

    return ret;
}

/*设置帧率*/
INT_X LUX_Video_Func_Set_Fps(uint32_t impvi_num,uint32_t *fps_num, uint32_t *fps_den)
{
    INT_X ret = LUX_OK;

    ret = LUX_Video_Set_Isp_Fps(impvi_num,fps_num, fps_den);
    if (ret < LUX_OK)
    {
        IMP_LOG_ERR(TAG, "Call xcam_video_set_fps fail.\n");
        return ret;
    }

    return ret;
}

/**********************************码率的相关函数实现(haokj)********************************************/
//同步修改用户态分编码模式信息
static void LUX_Video_Syn_EncAttrRcMode(int encChnNum, IMPEncoderAttrRcMode* encAttrRcMode)
{
    assert(((encChnNum == 0) || (encChnNum == 1)));
    assert(encAttrRcMode != NULL);

    memcpy(&g_VEncAttr->encGrpAttr[1].encChnAttr[1].chnAttr.rcAttr.attrRcMode, encAttrRcMode, sizeof(IMPEncoderAttrRcMode));

    return;
}


//设置系统码率
int LUX_Video_Set_Bitrate(int encChnNum, int bps_num)
{

    int ret = LUX_OK;
    IMPEncoderAttrRcMode stRcModeCfg;

    memset(&stRcModeCfg, 0, sizeof(IMPEncoderAttrRcMode));

    ret = IMP_Encoder_GetChnAttrRcMode(encChnNum, &stRcModeCfg);
    if (ret < LUX_OK) {
        IMP_LOG_ERR(TAG, "Call IMP_Encode_GetChnAttrRcMode fail.\n");
        return ret;
    }

    // #if ((defined PLATFORM_T40) || (defined PLATFORM_T31))
    if (stRcModeCfg.rcMode == IMP_ENC_RC_MODE_FIXQP) {
        IMP_LOG_INFO(TAG, "info(%s,%d):The encoder mode catn't set bitrate.\n", __func__, __LINE__);
        return ret;
    }
    else if (stRcModeCfg.rcMode == IMP_ENC_RC_MODE_CBR) {
        if (stRcModeCfg.attrCbr.uTargetBitRate == bps_num) {
            IMP_LOG_INFO(TAG, "The current configuration is the same as this one.\n");
            return LUX_OK;
        }

        stRcModeCfg.attrCbr.uTargetBitRate = bps_num;
    }
    else if (stRcModeCfg.rcMode == IMP_ENC_RC_MODE_VBR) {
        //注意需要判断一下最大帧率和目标帧率之间的相互影响，以及帧率的取值范围，不能说这个说多大我都去设置
        if (stRcModeCfg.attrVbr.uTargetBitRate == bps_num) {
            IMP_LOG_INFO(TAG, "The current configuration is the same as this one.\n");
            return LUX_OK;
        }

        stRcModeCfg.attrVbr.uTargetBitRate = bps_num;
    }
    else if (stRcModeCfg.rcMode == IMP_ENC_RC_MODE_CAPPED_VBR) {
        if (stRcModeCfg.attrCappedVbr.uTargetBitRate == bps_num) {
            IMP_LOG_INFO(TAG, "The current configuration is the same as this one.\n");
            return LUX_OK;
        }

        stRcModeCfg.attrCappedVbr.uTargetBitRate = bps_num;
    }
    else if (stRcModeCfg.rcMode == IMP_ENC_RC_MODE_CAPPED_QUALITY) {
        if (stRcModeCfg.attrCappedVbr.uTargetBitRate == bps_num) {
            IMP_LOG_INFO(TAG, "The current configuration is the same as this one.\n");
            return LUX_OK;
        }
        MYTRACEING("***rcmode: [IMP_ENC_RC_MODE_CAPPED_QUALITY] bitrate:[%d] \n", bps_num);
        stRcModeCfg.attrCappedVbr.uTargetBitRate = bps_num;
        stRcModeCfg.attrCappedVbr.uMaxBitRate = bps_num * 4 / 3;
    }
    else {
        IMP_LOG_ERR(TAG, "Encode don't support this type.\n");
        return ret;
    }
#if 0
    // #endif
    ret = IMP_Encoder_SetChnAttrRcMode(encChnNum, &stRcModeCfg);
    if (ret < LUX_OK) {
        IMP_LOG_ERR(TAG, "Call IMP_Encode_GetChnAttrRcMode fail\n");
        return ret;
    }
#endif

    /*保存配置到用户空间*/
    LUX_Video_Syn_EncAttrRcMode(encChnNum, &stRcModeCfg);

    /*
    ret = xcam_conf_set_video_bps(encChnNum, bps_num);
    if (ret < XCAM_SUCCESS) {
        LOG_INF(LOG_TAG,"err(%s,%d):failed to save bitrate to json profile.\n",__func__,__LINE__);
    }
    */
    return ret;
}


/*设置指定通道码率*/
INT_X LUX_Video_Func_Set_Bitrate(int encChnNum, int bps_num)
{
    INT_X ret = LUX_OK;
#if 1
    ret = LUX_Video_Set_Bitrate(encChnNum, bps_num);
    if (ret < LUX_OK)
    {
        IMP_LOG_ERR(TAG, "Call xcam_video_set_bitrate fail.\n");
        return ret;
    }
#endif
    //IMP_Encoder_SetChnBitRate(encChnNum, bps_num * 1000, 640);
    return ret;
}


//获取系统码率
INT_X LUX_Video_Get_Bitrate(int encChnNum, int* bps_num)
{
    int ret = LUX_OK;
    IMPEncoderAttrRcMode stRcModeCfg;

    memset(&stRcModeCfg, 0, sizeof(IMPEncoderAttrRcMode));

    ret = IMP_Encoder_GetChnAttrRcMode(encChnNum, &stRcModeCfg);
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "err(%s,%d):Call IMP_Encode_GetChnAttrRcMode fail\n", __func__, __LINE__);
        return ret;
    }

#if ((defined PLATFORM_T40) || (defined PLATFORM_T31))
    if (stRcModeCfg.rcMode == IMP_ENC_RC_MODE_FIXQP) {
        IMP_LOG_INFO(TAG, "info(%s,%d):The encoder mode catn't set bitrate.\n", __func__, __LINE__);
    }
    else if (stRcModeCfg.rcMode == IMP_ENC_RC_MODE_CBR) {
        *bps_num = stRcModeCfg.attrCbr.uTargetBitRate;
    }
    else if (stRcModeCfg.rcMode == IMP_ENC_RC_MODE_VBR) {
        *bps_num = stRcModeCfg.attrVbr.uTargetBitRate;
    }
    else if (stRcModeCfg.rcMode == IMP_ENC_RC_MODE_CAPPED_VBR) {
        *bps_num = stRcModeCfg.attrCappedVbr.uTargetBitRate;
    }
    else if (stRcModeCfg.rcMode == IMP_ENC_RC_MODE_CAPPED_QUALITY) {
        *bps_num = stRcModeCfg.attrCappedVbr.uTargetBitRate;
    }
    else {
        IMP_LOG_ERR(TAG, "Encode don't support this type.\n");
        ret = LUX_ERR;
    }

#endif
    return ret;
}

/*获取指定通道码率*/
INT_X LUX_Video_Func_Get_Bitrate(int encChnNum, int* bps_num)
{
    INT_X ret = LUX_OK;

    ret = LUX_Video_Get_Bitrate(encChnNum, bps_num);
    if (ret < LUX_OK)
    {
        IMP_LOG_ERR(TAG, "Call xcam_video_get_bitrate.\n");
        return ret;
    }

    return ret;
}

