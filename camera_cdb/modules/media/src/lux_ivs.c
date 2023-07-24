/*
 * lux_ivs.c
 *
 * 基于Ingenic平台封装的智能模块，包括移动侦测、人脸检测、人形侦测等
 * 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <imp_log.h>
#include <imp_common.h>
#include <imp_system.h>
#include <imp_framesource.h>
#include <imp_encoder.h>


#include <lux_ivs.h>
#include <comm_func.h>
#include <comm_error_code.h>
#include <lux_iniparse.h>
#include <lux_event.h>
#include "lux_osd.h"
#include "cJSON.h"

#define TAG "lux_ivs"
#include "lux_config.h"
#include "lux_global.h"

extern LUX_BASE_IVSQUE_ST g_ivs_que;
extern OSI_MUTEX g_ivs_que_mux;

OSI_SEM g_ivsAlgSem;

LUX_IVS_ATTR_ST g_ivsAttr =
{
    .bInit = LUX_FALSE,
    .rptEventTI = 
    {
        .timeInterval = 1 * 60,
    },
    .bindAttr =
    {
        .grpId = 0,
        .bindSrcChan = 1,
        .bindSrcModule = LUX_MODULS_FSRC,
        .bindDstChan = 0,                   /*绑定的channal相当于君正的group*/
        .bindDstModule = LUX_MODULS_IVS,
    },
};

//TODO:编译的时候请开启宏定义
#ifdef CONFIG_PTZ_IPC

/**
 * @description: 设置人形追踪的属性 
 * @param  [in] param 属性
 */
VOID_X LUX_IVS_SetPTAttr(person_tracker_param_input_t *pPtParam)
{
    Thread_Mutex_Lock(&g_ivsAttr.paramMux);
    memcpy(pPtParam, &g_ivsAttr.personTrackAttr.param, sizeof(persondet_param_input_t));
    Thread_Mutex_UnLock(&g_ivsAttr.paramMux);

    return;
}

/**
 * @description: 设置人形追踪结果buf
 * @param  [out] pOutPTResultBuf 人形追踪输出结果 mm
 */
static INT_X LUX_IVS_GetResultBuf(LUX_IVS_PTRESULT_QUE_ST **pOutPTResultBuf, person_tracker_param_output_t *pPTResult)
{
    if(NULL == pPTResult)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_GetResultBuf failed, empty pointer\n");
        return LUX_PARM_NULL_PTR;
    }

    INT_X i = 0;
    LUX_IVS_PTRESULT_QUE_ST *PTResultBuf = g_ivsAttr.personTrackAttr.ivsPTResultQue;

    for(i = 0; i < LUX_PTRESULT_QUE_LEN; i++)
    {
       if(LUX_TRUE != PTResultBuf[i].bUsed)
       {
           memcpy(&PTResultBuf[i], pPTResult, sizeof(person_tracker_param_output_t));
            *pOutPTResultBuf = &PTResultBuf[i];
            PTResultBuf[i].bUsed = LUX_TRUE;
           
           return LUX_OK;
       }
    }

    IMP_LOG_ERR(TAG, "LUX_IVS_GetResultBuf failed, The buffer is full\n");
    return LUX_ERR;

}

/**
 * @description: 人形追踪的结果释放函数
 * @param  [in] pParam 传入的参数
 * @return 0 成功，非零失败，返回错误码
 */
static VOID_X LUX_IVS_PTRelease(void *pParam)
{
    LUX_IVS_PTRESULT_QUE_ST *pEle = (LUX_IVS_PTRESULT_QUE_ST*)pParam;
    pEle->bUsed = LUX_FALSE;

    return;
}

/**
 * @description: 发送人形追踪的消息
 * @param  [in] idx  人形追踪的结果数组下标
 * @param  [in] pPTResult 人形追踪的结果
 * @return 0 成功，非零失败，返回错误码
 */
static INT_X LUX_IVS_SendPTMsg(LUX_IVS_PTRESULT_QUE_ST *pPTResult)
{
    if(NULL == pPTResult)
    {
       IMP_LOG_ERR(TAG, "LUX_IVS_SendPTMsg failed, empty pointer\n");
       return LUX_PARM_NULL_PTR;
    }

    INT_X ret = LUX_ERR;
    LUX_BASE_IVSDATA_ST popResult;
    popResult.type = LUX_BASE_PERSONTRACK_QUE_EN;
    popResult.IVSResult = (void *)pPTResult;
    popResult.pCallBackInfo= (void *)pPTResult;
    popResult.pCallBackFunc = LUX_IVS_PTRelease;

    ret = PushQueue(&g_ivs_que, &popResult);
    if(LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, " PopQueue failed\n");
        return LUX_ERR;
    }

    return LUX_OK;
}

#endif



/**
 * @description: ivs算法认证初始化
 * @return 0成功, 非0错误
 * @attention 需要连接网路后初始化
 */
INT_X LUX_IVS_Iaac_Init()
{
#if 0   //T40 移值暂时屏蔽算法功能  humy
    INT_X ret = LUX_ERR;

    ret = IAAC_Init(&g_ivsAttr.iaacInfo);
    if (ret)
    {
        IMP_LOG_ERR(TAG, "IAAC_Init error!\n");
        return LUX_ERR;
    }
#endif
    return LUX_OK;
}

/**
 * @description: ivs事件报警间隔倒计时
 * @return 倒计时未到返回FALSE, 到了返回 TURE,
 */
BOOL_X LUX_IVS_GetReportEvent_Countdown()
{
    INT_X ret = LUX_FALSE;

    Thread_Mutex_Lock(&g_ivsAttr.paramMux);
    g_ivsAttr.rptEventTI.curTime = getTime_getMs();

    if(g_ivsAttr.rptEventTI.curTime - g_ivsAttr.rptEventTI.lastTime > g_ivsAttr.rptEventTI.timeInterval)
    {
        g_ivsAttr.rptEventTI.lastTime = g_ivsAttr.rptEventTI.curTime;
        ret = LUX_TRUE;
    }
    Thread_Mutex_UnLock(&g_ivsAttr.paramMux);

    return ret;
}

/**
 * @description: ivs事件报警间隔
 * @param  [in] ts 时间间隔,单位s
 * @return 0 成功，非零失败，返回错误码
 */
void LUX_IVS_SetTimeInterval(LUX_IVS_TIMEINTERVAL_EN ts)
{
    if(ts < LUX_IVS_TIMEINTERVAL_1MIN || ts >= LUX_IVS_TIME_INTERVAL_BUTTON)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_SetTimeInterval failed, not support param [%d]\n", ts);
        g_ivsAttr.rptEventTI.timeInterval = 1* 60 * 1000;
    }

    Thread_Mutex_Lock(&g_ivsAttr.paramMux);
    switch(ts)
    {
        case LUX_IVS_TIMEINTERVAL_1MIN:
            g_ivsAttr.rptEventTI.timeInterval = 1 * 60 * 1000;
            break;
        case LUX_IVS_TIMEINTERVAL_3MIN:
            g_ivsAttr.rptEventTI.timeInterval = 3 * 60 * 1000;
            break;
        case LUX_IVS_TIMEINTERVAL_5MIN:
            g_ivsAttr.rptEventTI.timeInterval = 5 * 60 * 1000;
            break;
        case LUX_IVS_TIME_INTERVAL_BUTTON:
        default:
            IMP_LOG_ERR(TAG, "LUX_IVS_SetTimeInterval failed, not support param [%d]\n", ts);
            g_ivsAttr.rptEventTI.timeInterval = 1 * 60 * 1000;
            break;
    }
    Thread_Mutex_UnLock(&g_ivsAttr.paramMux);

    return;
}

/**
 * @description: 坐标转换
 * @param [in] pAlarmZone 坐标参数
 * @return 0 成功，非零失败，返回错误码
 */
static INT_X LUX_IVS_ChangeCoord(LUX_IVS_ZONE_BASE_ST *pAlarmZone)
{
    if(NULL == pAlarmZone)
    {
        IMP_LOG_ERR(TAG, " LUX_IVS_ChangeCoord failed\n");
        return LUX_PARM_NULL_PTR;
    }

    if((pAlarmZone->pointX + pAlarmZone->width) > 100 || 
       (pAlarmZone->pointY - pAlarmZone->height) > 100)
    {
        IMP_LOG_ERR(TAG, " LUX_IVS_ChangeCoord failed, set param invalid\n");
        return LUX_IVS_SET_PARAM_ERR;
    }

    pAlarmZone->pointX = 100 - pAlarmZone->pointX - pAlarmZone->width;
    pAlarmZone->pointY = 100 - pAlarmZone->pointY - pAlarmZone->height;
    pAlarmZone->width  = pAlarmZone->width;
    pAlarmZone->height = pAlarmZone->height;

    return LUX_OK;
}

/**
 * @description: 解析矩形报警区域字符串,将其转化为结构体
 * @param [out] pStrAlarmZoneInfo 区域报警参数
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_alarmZone_Parse(CHAR_X *pStrAlarmZoneInfo, LUX_IVS_ZONE_BASE_ST *pAlarmZone)
{
    if(NULL == pStrAlarmZoneInfo || NULL == pAlarmZone)
    {
        IMP_LOG_ERR(TAG, " LUX_IVS_alarmZone_Parse failed, empty point\n");
        return   LUX_PARM_NULL_PTR;    
    }

    LUX_COMM_ParseStr(pStrAlarmZoneInfo, "x", &pAlarmZone->pointX);
    LUX_COMM_ParseStr(pStrAlarmZoneInfo, "y", &pAlarmZone->pointY);
    LUX_COMM_ParseStr(pStrAlarmZoneInfo, "xlen", &pAlarmZone->width);
    LUX_COMM_ParseStr(pStrAlarmZoneInfo, "ylen", &pAlarmZone->height);   

    return LUX_OK;
}

/**
 * @description: 解析多边形报警区域字符串,将其转化为结构体
 * @param [out] pStrAlarmZoneInfo 区域报警参数
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_alarmPolygonZone_Parse(CHAR_X *pStrAlarmZoneInfo, LUX_IVS_M_POLYGON_ST *pAlarmZone)
{
    if(NULL == pStrAlarmZoneInfo || NULL == pAlarmZone)
    {
        IMP_LOG_ERR(TAG, " LUX_IVS_alarmPolygonZone_Parse failed, empty point\n");
        return   LUX_PARM_NULL_PTR;
    }

    INT_X i, j;
    char tmp[32];
    // perm_area2={"num":1, "region0":{"p0num":6,"p00x":0,"p00y":200,"p01x":100,"p01y":100,"p02x":200,"p02y":100,"p03x":300,"p03y":200, "p04x":200,"p04y":300,"p05x":100,"p05y":300}}
    LUX_COMM_ParseStr(pStrAlarmZoneInfo, "num", &pAlarmZone->regionCnt);
    printf("+++ regionCnt=%d +++\n\n", pAlarmZone->regionCnt);
    for (i = 0; i < pAlarmZone->regionCnt; i++)
    {
        memset(tmp, 0, sizeof(tmp));
        sprintf(tmp,"p%xnum", i);
        LUX_COMM_ParseStr(pStrAlarmZoneInfo, tmp, &pAlarmZone->polygon[i].pointCnt);
        printf("\n\n+++ region[%d] pointCnt=%d +++\n", i, pAlarmZone->polygon[i].pointCnt);
        for (j = 0; j < pAlarmZone->polygon[i].pointCnt; j++)
        {
            memset(tmp, 0, sizeof(tmp));
            sprintf(tmp,"p%x%xx", i, j);
            LUX_COMM_ParseStr(pStrAlarmZoneInfo, tmp, &pAlarmZone->polygon[i].zonePoints[j].x);
            memset(tmp, 0, sizeof(tmp));
            sprintf(tmp,"p%x%xy", i, j);
            LUX_COMM_ParseStr(pStrAlarmZoneInfo, tmp, &pAlarmZone->polygon[i].zonePoints[j].y);
            printf("+++ p[%d]:(x:%d, y:%d) +++\n", j, pAlarmZone->polygon[i].zonePoints[j].x, pAlarmZone->polygon[i].zonePoints[j].y);
        }
    }

    return LUX_OK;
}

/**
 * @description: cjson解析多个矩形报警区域字符串,将其转化为结构体
 * @param [out] pStrAlarmZoneInfo 区域报警参数
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_alarmZone_cJsonParse(CHAR_X *pStrAlarmZoneInfo, LUX_IVS_M_ZONE_BASE_ST *pAlarmZone)
{
    if(NULL == pStrAlarmZoneInfo || NULL == pAlarmZone)
    {
        IMP_LOG_ERR(TAG, " LUX_IVS_alarmZone_cJsonParse failed, empty point\n");
        return   LUX_PARM_NULL_PTR;
    }
    //{"num":2,"region0":{"x":20,"xlen":60,"y":20,"ylen":60},"region1":{"x":10,"xlen":80,"y":10,"ylen":80}}
    INT_X i;
    cJSON* pJson = NULL;
    cJSON* tmp = NULL;
    CHAR_X regionStr[32] = { 0 };
    CHAR_X pointStr[16] = { 0 };
    cJSON* cJSONRegion = NULL;

    pJson = cJSON_Parse((CHAR_X*)pStrAlarmZoneInfo);
    if (NULL == pJson)
    {
        printf("%s %d step error\n", __FUNCTION__, __LINE__);
        return LUX_GET_INI_VALUE_ERR;
    }

    /*区域数获取*/
    tmp = cJSON_GetObjectItem(pJson, "num");
    if (NULL == tmp)
    {
        printf("%s %d step error\n", __FUNCTION__, __LINE__);
        cJSON_Delete(pJson);
        return LUX_GET_INI_VALUE_ERR;
    }
    memset(pAlarmZone, 0, sizeof(LUX_IVS_M_ZONE_BASE_ST));
    pAlarmZone->regionCnt = tmp->valueint;
    IMP_LOG_INFO(TAG, "regions num[%d]\n", pAlarmZone->regionCnt);
    if (pAlarmZone->regionCnt > MAX_PERM_NUM)
    {
        printf("##### error regions num too big[%d] #####\n", pAlarmZone->regionCnt);
        cJSON_Delete(pJson);
        return LUX_GET_INI_VALUE_ERR;
    }

    for (i = 0; i < pAlarmZone->regionCnt; i++)
    {
        /*区域获取*/
        memset(regionStr, 0, sizeof(regionStr));
        snprintf(regionStr, sizeof(regionStr), "region%d", i);
        cJSONRegion = cJSON_GetObjectItem(pJson, regionStr);
        if (NULL == cJSONRegion)
        {
            printf("##### [%s][%d]error #####\n", __FUNCTION__, __LINE__);
            cJSON_Delete(pJson);
            return LUX_GET_INI_VALUE_ERR;
        }
        /*区域点 x，width, y, height解析*/
        memset(pointStr, 0, sizeof(pointStr));
        strcpy(pointStr, "x");
        pAlarmZone->rect[i].pointX = cJSON_GetObjectItem(cJSONRegion, pointStr)->valueint;
        memset(pointStr, 0, sizeof(pointStr));
        strcpy(pointStr, "xlen");
        pAlarmZone->rect[i].width = cJSON_GetObjectItem(cJSONRegion, pointStr)->valueint;
        memset(pointStr, 0, sizeof(pointStr));
        strcpy(pointStr, "y");
        pAlarmZone->rect[i].pointY = cJSON_GetObjectItem(cJSONRegion, pointStr)->valueint;
        memset(pointStr, 0, sizeof(pointStr));
        strcpy(pointStr, "ylen");
        pAlarmZone->rect[i].height = cJSON_GetObjectItem(cJSONRegion, pointStr)->valueint;
        cJSONRegion = NULL;
        IMP_LOG_INFO(TAG, "rect[%d] (x:%d, xlen:%d, y:%d, ylen:%d)\n", i, 
        pAlarmZone->rect[i].pointX, pAlarmZone->rect[i].width, pAlarmZone->rect[i].pointY, pAlarmZone->rect[i].height);
    }
    cJSON_Delete(pJson);
    return LUX_OK;
}

/**
 * @description: cjson解析多边形报警区域字符串,将其转化为结构体
 * @param [out] pStrAlarmZoneInfo 区域报警参数
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_alarmPolygonZone_cJsonParse(CHAR_X* pStrAlarmZoneInfo, LUX_IVS_M_POLYGON_ST* pAlarmZone)
{
    if (NULL == pStrAlarmZoneInfo || NULL == pAlarmZone)
    {
        IMP_LOG_ERR(TAG, " LUX_IVS_alarmPolygonZone_cJsonParse failed, empty point\n");
        return   LUX_PARM_NULL_PTR;
    }
    //{"num":2, "region0":{"pnum":4,"x0":0,"y0":0,"x1":100,"y1":0,"x2":100,"y2":100,"x3":0,"y3":100},"region1":{"pnum":4,"x0":10,"y0":10,"x1":90,"y1":10,"x2":90,"y2":90,"x3":10,"y3":90}}
    INT_X i, j;
    cJSON* pJson = NULL;
    cJSON* tmp = NULL;
    CHAR_X regionStr[32] = { 0 };
    CHAR_X pointStr[16] = { 0 };
    cJSON* cJSONRegion = NULL;
    cJSON* cJSONPoint = NULL;

    pJson = cJSON_Parse((CHAR_X*)pStrAlarmZoneInfo);
    if (NULL == pJson)
    {
        printf("%s %d step error\n", __FUNCTION__, __LINE__);
        return LUX_GET_INI_VALUE_ERR;
    }

    /*区域数获取*/
    tmp = cJSON_GetObjectItem(pJson, "num");
    if (NULL == tmp)
    {
        printf("%s %d step error\n", __FUNCTION__, __LINE__);
        cJSON_Delete(pJson);
        return LUX_GET_INI_VALUE_ERR;
    }
    memset(pAlarmZone, 0, sizeof(LUX_IVS_M_POLYGON_ST));
    pAlarmZone->regionCnt = tmp->valueint;
    IMP_LOG_INFO(TAG, "regions num[%d]\n", pAlarmZone->regionCnt);
    if (pAlarmZone->regionCnt > MAX_PERM_NUM)
    {
        printf("##### error regions num too big[%d] #####\n", pAlarmZone->regionCnt);
        cJSON_Delete(pJson);
        return LUX_GET_INI_VALUE_ERR;
    }

    for (i = 0; i < pAlarmZone->regionCnt; i++)
    {
        /*区域点数获取*/
        memset(regionStr, 0, sizeof(regionStr));
        snprintf(regionStr, sizeof(regionStr), "region%d", i);
        cJSONRegion = cJSON_GetObjectItem(pJson, regionStr);
        if (NULL == cJSONRegion)
        {
            printf("##### [%s][%d]error #####\n", __FUNCTION__, __LINE__);
            cJSON_Delete(pJson);
            return LUX_GET_INI_VALUE_ERR;
        }
        cJSONPoint = cJSON_GetObjectItem(cJSONRegion, "pnum");
        if (NULL == cJSONPoint)
        {
            printf("##### [%s][%d]error #####\n", __FUNCTION__, __LINE__);
            return LUX_GET_INI_VALUE_ERR;
        }
        pAlarmZone->polygon[i].pointCnt = cJSONPoint->valueint;
        IMP_LOG_INFO(TAG,"regions[%d] pointNum[%d]\n", i, pAlarmZone->polygon[i].pointCnt);
        if (pAlarmZone->polygon[i].pointCnt > MAX_PERM_POINTS_NUM)
        {
            printf("##### error point num too big[%d] #####\n", pAlarmZone->polygon[i].pointCnt);
            return LUX_GET_INI_VALUE_ERR;
        }

        for (j = 0; j < pAlarmZone->polygon[i].pointCnt; j++)
        {
            /*区域点 x，y坐标解析*/
            memset(pointStr, 0, sizeof(pointStr));
            snprintf(pointStr, sizeof(pointStr), "x%x", j);
            pAlarmZone->polygon[i].zonePoints[j].x = cJSON_GetObjectItem(cJSONRegion, pointStr)->valueint;
            memset(pointStr, 0, sizeof(pointStr));
            snprintf(pointStr, sizeof(pointStr), "y%x", j);
            pAlarmZone->polygon[i].zonePoints[j].y = cJSON_GetObjectItem(cJSONRegion, pointStr)->valueint;
            IMP_LOG_INFO(TAG, "point[%d]:(%d,%d)\n", j, pAlarmZone->polygon[i].zonePoints[j].x, pAlarmZone->polygon[i].zonePoints[j].y);
        }
        cJSONRegion = NULL;
    }
    cJSON_Delete(pJson);

    return LUX_OK;
}
#if 0
/**
 * @description: 设置矩形报警区域转换
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_SetAlarmRegion_Flip()
{
    INT_X ret = LUX_ERR;
    CHAR_X strAlarmZoneInfo[64] = {0};
    LUX_IVS_ZONE_BASE_ST alarmZone;

    ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "algorithm_config", "alarm_zone_size", strAlarmZoneInfo);
    if (LUX_OK !=  ret)
    {
        IMP_LOG_ERR(TAG, " LUX_INIParse_GetString key[%s] failed\n", "alarm_zone_size");
        return LUX_GET_INI_VALUE_ERR;
    }

    ret = LUX_IVS_alarmZone_cJsonParse(strAlarmZoneInfo, &alarmZone);
    if (LUX_OK !=  ret)
    {
        IMP_LOG_ERR(TAG, " LUX_IVS_alarmZone_cJsonParse failed, return error [0x%x]\n", ret);
        return LUX_IVS_SET_PARAM_ERR;
    }

    ret = LUX_IVS_ChangeCoord(&alarmZone);
    if (LUX_OK !=  ret)
    {
        IMP_LOG_ERR(TAG, " LUX_IVS_ChangeCoord failed, return error [0x%x]\n", ret);
        return LUX_IVS_SET_PARAM_ERR;
    }

    ret = LUX_IVS_SetAlarmRegion(LUX_IVS_PERSON_DET_EN, &alarmZone);
    if (LUX_OK !=  ret)
    {
        IMP_LOG_ERR(TAG, " LUX_IVS_SetAlarmRegion failed\n");
        return LUX_IVS_SET_PARAM_ERR;
    }

    memset(strAlarmZoneInfo, 0, sizeof(strAlarmZoneInfo) / sizeof(&strAlarmZoneInfo[0]));
    sprintf(strAlarmZoneInfo, "{\"num\":1,\"region0\":{\"x\":%d,\"xlen\":%d,\"y\":%d,\"ylen\":%d}}", 
            alarmZone.pointX,alarmZone.width, alarmZone.pointY, alarmZone.height);
    
    strcpy(&g_configData->alg_config.alarm_zone_size,strAlarmZoneInfo);
    // ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "algorithm_config", "alarm_zone_size", strAlarmZoneInfo);
    // if (LUX_OK != ret)
    // {
    //     IMP_LOG_ERR(TAG, "LUX_INIParse_SetKey failed,  key[%s]!, error num [0x%x] ", "alarm_zone_size", ret);
    //     return LUX_INI_SET_PARAM_FAILED;
    // }

    return LUX_OK;
}

/**
 * @description: 设置多边形报警区域转换
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_SetAlarmPolygonRegion_Flip()
{
    INT_X ret = LUX_ERR;
    CHAR_X permArea[256] = {0};
    CHAR_X region[256] = {0};
    LUX_IVS_ZONE_POLYGON_ST alarmArea;
    LUX_IVS_ZONE_POLYGON_ST newAlarmArea;
    INT_X i = 0;
    ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "algorithm_config", "perm_area_2", permArea);
    if (LUX_OK !=  ret)
    {
        IMP_LOG_ERR(TAG, " LUX_INIParse_GetString key[%s] failed\n", "perm_area_2");
        return LUX_GET_INI_VALUE_ERR;
    }
    LUX_IVS_alarmPolygonZone_cJsonParse(permArea, &alarmArea);

    newAlarmArea.pointNum = alarmArea.pointNum;
    if((0 < alarmArea.pointNum) || (MAX_PERM_AREA_POINTS_NUM > alarmArea.pointNum))
    {
        INT_X j = alarmArea.pointNum;
        for (i = 0; i < alarmArea.pointNum; i++)
        {
            newAlarmArea.zonePoints[i].x = 100 - alarmArea.zonePoints[--j].x;
            newAlarmArea.zonePoints[i].y = 100 - alarmArea.zonePoints[--j].y;
        }
    }
    memset(permArea, 0, sizeof(permArea));
    for (i = 0; i < newAlarmArea.pointNum; i++)
    {

        if (0 == i)
        {
            snprintf(permArea, 256, "{\"num\":%d", newAlarmArea.pointNum);
        }

        snprintf(region, 256, ",\"point%d\":{\"x\":%d,\"y\":%d}", i, newAlarmArea.zonePoints[i].x,newAlarmArea.zonePoints[i].y);
        //printf("\n\n\n\n ------------%s\n", region);
        strcat(permArea, region);

        if (i == (newAlarmArea.pointNum - 1))
        {
            strcat(permArea, "}");
        }
    }
    ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "algorithm_config", "perm_area_2", permArea);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_INIParse_SetKey failed,  key[%s]!, error num [0x%x] ", "alarm_zone_size", ret);
        return LUX_INI_SET_PARAM_FAILED;
    }

    return LUX_OK;
}
#endif

/**
 * @description: 创建ivs通道组，并绑定视频源通道
 * @return 0 成功，非零失败，返回错误码
 */
static INT_X LUX_IVS_CreateGroup()
{
    INT_X ret = LUX_ERR;
    LUX_MODULES_BIND_ST bind;

    /*创建ivs通道*/
    ret = IMP_IVS_CreateGroup(g_ivsAttr.bindAttr.grpId);
	if (ret != LUX_OK)
    {
		IMP_LOG_ERR(TAG, "IMP_IVS_CreateGroup(%d) failed\n", g_ivsAttr.bindAttr.grpId);
		return LUX_IVS_GROUP_CREATE_ERR;
	}

    /* 绑定venc通道到源模块通道 */
    memset(&bind, 0, sizeof(bind));
    bind.srcChan = g_ivsAttr.bindAttr.bindSrcChan;
    bind.srcModule = g_ivsAttr.bindAttr.bindSrcModule;
    bind.dstChan = g_ivsAttr.bindAttr.bindDstChan;
    bind.dstModule = g_ivsAttr.bindAttr.bindDstModule;

    ret = LUX_COMM_Bind_AddModule(&bind);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_COMM_Bind_AddModule error!\n");
        return LUX_IVS_CHAN_CREATE_ERR;
    }

    return LUX_OK;
}

/**
 * @description: ivs模块初始化
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_Init(VOID_X)
{
    INT_X ret = LUX_ERR;

    Thread_Mutex_Create(&g_ivsAttr.paramMux);

    /* 创建ivs通道组，并绑定视频源通道 */
    ret = LUX_IVS_CreateGroup();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_CreateGroup error!\n");
        return LUX_ERR;
    }
    g_ivsAttr.bInit = LUX_TRUE;
    printf("LUX_IVS_CreateGroup() success\n");
#if 0
    /*IVS事件处理函数初始化*/
    ret = LUX_EVENT_IVSProcess_Init();
	if (LUX_OK !=  ret)
    {
		IMP_LOG_ERR(TAG, " LUX_EVENT_IVSProcess_Init failed\n");
		return LUX_IVS_CAPTURE_PIC_ERR;
	}
#endif
    return LUX_OK;
}

/**
 * @description: IVS模块去初始化
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_Exit()
{
    INT_X ret = LUX_ERR;

    /*解绑group*/
    ret = IMP_IVS_DestroyGroup(g_ivsAttr.bindAttr.grpId);
    if (ret < 0) 
    {
        IMP_LOG_ERR(TAG, "IMP_IVS_DestroyGroup(%d) failed\n", g_ivsAttr.bindAttr.grpId);
        return LUX_ERR;
    }
    g_ivsAttr.bInit = LUX_FALSE;
    Thread_Mutex_Destroy(&g_ivsAttr.paramMux);
    return LUX_OK;
}
