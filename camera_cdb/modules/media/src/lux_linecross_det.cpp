#include "lux_linecross_det.hpp"
#define TAG "lux_linecross_det"

static UCHAR_X line_cnt = 0;
LUX_ALG_LINEDET_ATTR_ST g_linedet_attr =
{
    .bInit = false,
    .IsRunning = false,
    .alarmTime = 0,
    .timeInterval = LUX_IVS_TIMEINTERVAL_1MIN,
    .in_param = {0},
};

VOID_X LUX_ALG_LineCrossDet_Start()
{
    g_linedet_attr.bInit = true;
    return;
}

VOID_X LUX_ALG_LineCrossDet_Stop()
{
    g_linedet_attr.bInit = false;
    return;
}

bool LUX_LineEvent_IsAlarmByInterval(UINT_X lastTime)
{
    UINT_X timeInterval = 0;
    UINT_X curTime = 0;
    bool bAlarm = false;
#if 0
    if (1 == recordReboot)  //重新上电读取配置文件值
    {
        CHAR_X *p_getalarmInterval = IPC_APP_get_alarm_interval_X();
        g_linedet_attr.timeIntervel = (LUX_IVS_TIMEINTERVAL_EN)(*p_getalarmInterval - '0');
        recordReboot = 0;
    }
#endif
    switch (g_linedet_attr.timeInterval)
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
        bAlarm = true;
    }
    //printf("INFO: %s:%d timeInterval:%d curTime:%d lastTime:%d-%d bAlarm:%d\n",
    //       __func__, __LINE__, timeInterval, curTime, lastTime, *pLastTime, bAlarm);

    return bAlarm;
}

/**
 * @description: 越线侦测事件报警间隔
 * @param  [in] ts 时间间隔,单位s
 * @return 0 成功，非零失败，返回错误码
 */
VOID_X LUX_ALG_LineCrossDet_SetTimeInterval(LUX_IVS_TIMEINTERVAL_EN ts)
{
    Thread_Mutex_Lock(&g_linedet_attr.paramMux);
    if(ts < LUX_IVS_TIMEINTERVAL_1MIN || ts >= LUX_IVS_TIME_INTERVAL_BUTTON)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_LineCrossDet_SetTimeInterval failed, not support param [%d]\n", ts);
        g_linedet_attr.timeInterval = LUX_IVS_TIMEINTERVAL_1MIN;
    }

    switch (ts)
    {
    case LUX_IVS_TIMEINTERVAL_1MIN ... LUX_IVS_TIMEINTERVAL_5MIN:
        g_linedet_attr.timeInterval = ts;
        break;
    case LUX_IVS_TIME_INTERVAL_BUTTON:
    default:
        IMP_LOG_ERR(TAG, "LUX_ALG_LineCrossDet_SetTimeInterval failed, not support param [%d]\n", ts);
        g_linedet_attr.timeInterval = LUX_IVS_TIMEINTERVAL_1MIN;
        break;
    }
    Thread_Mutex_UnLock(&g_linedet_attr.paramMux);
    return;
}

VOID_X LUX_ALG_LineCrossDet_SetAttr(linedet_param_input_t* pInParam)
{
    if (pInParam == NULL)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_LineCrossDet_SetAttr failed, empty point.\n");
    }
    Thread_Mutex_Lock(&g_linedet_attr.paramMux);
    g_linedet_attr.in_param = *pInParam;
    Thread_Mutex_UnLock(&g_linedet_attr.paramMux);
    return;
}

/**
 * @description: 越线侦测报警事件函数
 * @return 0 成功，非零失败
 */
INT_X LUX_ALG_LineCrossDet_Alarm()
{
    Thread_Mutex_Lock(&g_linedet_attr.paramMux);
    /*报警时间间隔*/
    if (LUX_LineEvent_IsAlarmByInterval(g_linedet_attr.alarmTime))
    {
        g_linedet_attr.alarmTime = getTime_getS();
        /*上报抓图 */   //TODO

        /* 云存储 */    //TODO

        /*事件录像*/    //TODO
        printf("*** LUX_ALG_LineCrossDet_Alarm ***\n\n\n");
    }
    Thread_Mutex_UnLock(&g_linedet_attr.paramMux);
    return LUX_OK;
}

/**
 * @description: cjson解析报警线区域字符串,将其转化为结构体
 * @param [out] pStrAlarmLineInfo 区域报警参数
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_ALG_LineCross_cJsonParse(CHAR_X* pStrAlarmLineInfo, P_LUX_M_SLASH_ST pAlarmLine)
{
    if (NULL == pStrAlarmLineInfo || NULL == pAlarmLine)
    {
        IMP_LOG_ERR(TAG, " LUX_ALG_LineCross_cJsonParse failed, empty point\n");
        return   LUX_PARM_NULL_PTR;
    }
    //{"num":2, "line0":{"x0":10,"y0":0,"x1":10,"y1":100},"line1":{"x0":90,"y0":0,"x1":90,"y1":100}}
    INT_X i, j;
    cJSON* pJson = NULL;
    cJSON* tmp = NULL;
    CHAR_X regionStr[32] = { 0 };
    CHAR_X pointStr[16] = { 0 };
    cJSON* cJSONRegion = NULL;
    cJSON* cJSONPoint = NULL;

    pJson = cJSON_Parse((CHAR_X*)pStrAlarmLineInfo);
    if (NULL == pJson)
    {
        printf("%s %d step error\n", __FUNCTION__, __LINE__);
        return LUX_GET_INI_VALUE_ERR;
    }

    /*线数获取*/
    tmp = cJSON_GetObjectItem(pJson, "num");
    if (NULL == tmp)
    {
        printf("%s %d step error\n", __FUNCTION__, __LINE__);
        cJSON_Delete(pJson);
        return LUX_GET_INI_VALUE_ERR;
    }
    memset(pAlarmLine, 0, sizeof(LUX_M_SLASH_ST));
    pAlarmLine->slashCnt = tmp->valueint;
    IMP_LOG_INFO(TAG, "lines num[%d]\n", pAlarmLine->slashCnt);
    if (pAlarmLine->slashCnt > LUX_LINECROSS_LINE_MAX)
    {
        printf("##### error lines num too big[%d] #####\n", pAlarmLine->slashCnt);
        cJSON_Delete(pJson);
        return LUX_GET_INI_VALUE_ERR;
    }

    for (i = 0; i < pAlarmLine->slashCnt; i++)
    {
        /*报警线数获取*/
        memset(regionStr, 0, sizeof(regionStr));
        snprintf(regionStr, sizeof(regionStr), "line%d", i);
        cJSONRegion = cJSON_GetObjectItem(pJson, regionStr);
        if (NULL == cJSONRegion)
        {
            printf("##### [%s][%d]error #####\n", __FUNCTION__, __LINE__);
            cJSON_Delete(pJson);
            return LUX_GET_INI_VALUE_ERR;
        }
        /*线 p0点、p1点 x，y坐标解析*/
        memset(pointStr, 0, sizeof(pointStr));
        snprintf(pointStr, sizeof(pointStr), "x0", j);
        pAlarmLine->slash[i].p0.x = cJSON_GetObjectItem(cJSONRegion, pointStr)->valueint;
        memset(pointStr, 0, sizeof(pointStr));
        snprintf(pointStr, sizeof(pointStr), "y0", j);
        pAlarmLine->slash[i].p0.y = cJSON_GetObjectItem(cJSONRegion, pointStr)->valueint;
        memset(pointStr, 0, sizeof(pointStr));
        snprintf(pointStr, sizeof(pointStr), "x1", j);
        pAlarmLine->slash[i].p1.x = cJSON_GetObjectItem(cJSONRegion, pointStr)->valueint;
        memset(pointStr, 0, sizeof(pointStr));
        snprintf(pointStr, sizeof(pointStr), "y1", j);
        pAlarmLine->slash[i].p1.y = cJSON_GetObjectItem(cJSONRegion, pointStr)->valueint;
        IMP_LOG_INFO(TAG, "line[%d] point[0]:(%d,%d) point[1]:(%d,%d)\n", i, pAlarmLine->slash[i].p0.x, pAlarmLine->slash[i].p0.y, pAlarmLine->slash[i].p1.x, pAlarmLine->slash[i].p1.y);
        cJSONRegion = NULL;
    }
    cJSON_Delete(pJson);

    return LUX_OK;
}

/**
 * @description: 报警线坐标转换,将百分比坐标换算为实际子码流坐标
 * @param [in] pAlarmZone 坐标参数
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_ALG_LineCross_CoorConvertSet(P_LUX_M_SLASH_ST pAlarmLine)
{
    if (nullptr == pAlarmLine)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_LineCross_CoorConvert failed! param is nullptr.\n");
        return LUX_PARM_NULL_PTR;
    }
    INT_X ret = LUX_ERR, sub_res_w = 0, sub_res_h = 0, i = 0;
    FLOAT_X conval_w = 0, conval_h = 0;
    linedet_param_input_t linedParam;

    ret = LUX_Video_Fnuc_Get_Resoulution(LUX_STREAM_SUB, &sub_res_w, &sub_res_h);
    if (ret != LUX_OK)
    {
        IMP_LOG_ERR(TAG, "Call LUX_Video_Fnuc_Get_Resoulution fail! channel[%d]\n", LUX_STREAM_SUB);
        return LUX_INI_GET_PARAM_FAILED;
    }
    conval_w = sub_res_w / 100.0;
    conval_h = sub_res_h / 100.0;
    // printf("conval_w=%lf, conval_h=%lf\n", conval_w, conval_h);
    linedParam.lineInfo.slashCnt = pAlarmLine->slashCnt;
    printf("g_linedet line nums: %d\n", linedParam.lineInfo.slashCnt);
    for (i = 0; i < linedParam.lineInfo.slashCnt; i++)
    {
        pAlarmLine->slash[i].p0.x = conval_w * pAlarmLine->slash[i].p0.x;
        pAlarmLine->slash[i].p0.y = conval_h * pAlarmLine->slash[i].p0.y;
        pAlarmLine->slash[i].p1.x = conval_w * pAlarmLine->slash[i].p1.x;
        pAlarmLine->slash[i].p1.y = conval_h * pAlarmLine->slash[i].p1.y;
        linedParam.lineInfo.slash[i] = pAlarmLine->slash[i];
        printf("g_lineInfo[%d]: p0.x:%d, p0.y:%d, p1.x:%d, p1.y:%d\n", i,
        linedParam.lineInfo.slash[i].p0.x, 
        linedParam.lineInfo.slash[i].p0.y, 
        linedParam.lineInfo.slash[i].p1.x, 
        linedParam.lineInfo.slash[i].p1.y);
    }
    LUX_ALG_LineCrossDet_SetAttr(&linedParam);
    return LUX_OK;
}

/**
 * @description: 越线侦测启动设置配置
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_ALG_LineCrossDet_SetConf()
{
    INT_X i;
    INT_X ret = LUX_ERR;
    CHAR_X strAlarmLineInfo[LINE_STR_LEN_MAX] = {0};
    LUX_M_SLASH_ST lineInfo;

    /*隐私模式下不开启越线侦测*/
    if(LUX_TRUE == g_configData->alg_config.sleepMode)
    {
        LUX_ALG_LineCrossDet_Stop();
        printf("+++++++++++++++++ Line[%d] Function[%s] linecross_det off +++++++++++++++++\n", __LINE__, __FUNCTION__);
    }
    else
    {
        /*设置是否开启越线侦测警告*/
        if (LUX_TRUE == g_configData->alg_config.crossline_det_enabled)
        {
            LUX_ALG_LineCrossDet_Start();
            printf("+++++++++++++++++ Line[%d] Function[%s] linecross_det on +++++++++++++++++\n", __LINE__, __FUNCTION__);
            strcpy(strAlarmLineInfo, g_configData->alg_config.line_area);
            printf("++++ linecrossdet strAlarmLineInfo=%s +++\n", strAlarmLineInfo);

            ret = LUX_ALG_LineCross_cJsonParse(strAlarmLineInfo, &lineInfo);
            if (LUX_OK !=  ret)
            {
                IMP_LOG_ERR(TAG, " LUX_ALG_LineCross_cJsonParse failed, return error [0x%x]\n", ret);
                return LUX_IVS_SET_PARAM_ERR;
            }
            printf("linecrossdet LUX_ALG_LineCross_cJsonParse Success\n");
            ret = LUX_ALG_LineCross_CoorConvertSet(&lineInfo);
            if (LUX_OK !=  ret)
            {
                IMP_LOG_ERR(TAG, " LUX_ALG_LineCross_CoorConvertSet failed, return error [0x%x]\n", ret);
                return LUX_IVS_SET_PARAM_ERR;
            }
            printf("linecrossdet LUX_ALG_LineCross_CoorConvertSet Success\n");
        }
        else
        {
            LUX_ALG_LineCrossDet_Stop();
            printf("+++++++++++++++++ Line[%d] Function[%s] linecross_det off +++++++++++++++++\n", __LINE__, __FUNCTION__);
        }
        LUX_OSD_LineCross_Show(&g_linedet_attr.in_param.lineInfo, (BOOL_X)g_linedet_attr.bInit);
    }
    return LUX_OK;
}

INT_X LUX_ALG_LineCrossDet_GetResult(linedet_param_input_t *p_in, linedet_param_output_t *p_out)
{
    if (nullptr == p_in || p_out == nullptr)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_LineCrossDet_GetResult failed.\n");
        return LUX_PARM_NULL_PTR;
    }

    INT_X i = 0;
    INT_X ret = LUX_ERR;
    IMPFrameInfo* pFrame = nullptr;
    input_info_t input_now;
    vector<spot> target;
    spot p1[LUX_LINECROSS_LINE_MAX], p2[LUX_LINECROSS_LINE_MAX];

    Thread_Mutex_Lock(&g_framSrcAttr->fsChn[LUX_STREAM_SUB].mux);
    do
    {
        /*获取帧*/
        ret = IMP_FrameSource_GetFrame(LUX_STREAM_SUB, &pFrame);
        if (ret != LUX_OK) {
            IMP_LOG_ERR(TAG, "IMP_FrameSource_GetFrame failed\n");
            break;
        }
        // printf("linecrossDet frame info: w=%u, h=%u, size=%u\n", pFrame->width, pFrame->height, pFrame->size);

        /*转换数据格式*/
        memset(&input_now, 0, sizeof(input_info_t));
        input_now.image = (unsigned char*)pFrame->virAddr;
        input_now.w = pFrame->width;
        input_now.h = pFrame->height;
        lux_LineCrossDet_init(&input_now, target);

        for (i = 0; i < p_in->lineInfo.slashCnt; i++)
        {
            /*设置报警线*/
            p1[i].x = p_in->lineInfo.slash[i].p0.x;
            p1[i].y = p_in->lineInfo.slash[i].p0.y;
            p2[i].x = p_in->lineInfo.slash[i].p1.x;
            p2[i].y = p_in->lineInfo.slash[i].p1.y;

            /*获取结果*/
            lux_LineCrossDet_start(target, p1[i], p2[i], p_in->lastTrace, *p_out);
        }

        /*释放帧*/
        ret = IMP_FrameSource_ReleaseFrame(LUX_STREAM_SUB, pFrame);
        if (ret != LUX_OK) {
            IMP_LOG_ERR(TAG, "IMP_FrameSource_ReleaseFrame failed\n");
            break;
        }
    } while (0);
    Thread_Mutex_UnLock(&g_framSrcAttr->fsChn[LUX_STREAM_SUB].mux);
    return ret;
}

VOID_X LUX_ALG_LineCrossDet_Process()
{
    UINT_X nowtime = 0;
    INT_X ret = LUX_ERR;

    Thread_Mutex_Lock(&g_linedet_attr.mux);
    do
    {
        g_linedet_attr.objResult.clear();
        nowtime = getTime_getMs();
        ret = LUX_ALG_LineCrossDet_GetResult(&g_linedet_attr.in_param, &g_linedet_attr.objResult);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_ALG_LineCrossDet_GetResult failed.\n");
            break;
        }

        /*判断结果输出*/
        if (g_linedet_attr.objResult.size())
        {
            LUX_ALG_LineCrossDet_Alarm();
#if 0
            printf("\n\n\nCrossLineDet:\n");
            for (int i = 0; i < g_linedet_attr.objResult.size(); i++)
            {
                printf("[%d]th Line, time: [%u]ms, Crossing direction: [%d]\n", i, getTime_getMs() - nowtime, g_linedet_attr.objResult[i]);
            }
#endif
        }
    } while (0);
    Thread_Mutex_UnLock(&g_linedet_attr.mux);
    usleep(LINEDET_DELAY_TIME);
    return;
}

VOID_X linecross_det_process_thread()
{
    CHAR_X threadName[64] = {0};
    CHAR_X ivsChnName[32] = {0};

    strcpy(ivsChnName, "LINE_DETECTION");
    g_linedet_attr.IsRunning = true;

    sprintf(threadName, "ALG_%s", ivsChnName);
    prctl(PR_SET_NAME, threadName);
    while (g_linedet_attr.IsRunning)
    {
        if (!g_linedet_attr.bInit || !g_framSrcAttr->bInit)
        {
            usleep(SLEEP_TIME);
            continue;
        }
        else
        {
            LUX_ALG_LineCrossDet_Process();
        }
    }
    return;
}

INT_X LUX_ALG_LineCrossDet_Init()
{
    INT_X ret = LUX_ERR;

    Thread_Mutex_Create(&g_linedet_attr.paramMux);
    Thread_Mutex_Create(&g_linedet_attr.mux);

    ret = LUX_ALG_LineCrossDet_SetConf();
    if (ret != LUX_OK) {
        IMP_LOG_ERR(TAG, "LUX_ALG_LineCrossDetConf failed\n");
    }

    thread t_line(linecross_det_process_thread);
    t_line.detach();
    return LUX_OK;
}

INT_X LUX_ALG_LineCrossDet_Exit()
{
    g_linedet_attr.IsRunning = false;
    LUX_ALG_LineCrossDet_Stop();
    INT_X ret = lux_LineCrossDet_exit();
    if (ret == LUX_ERR)
    {
        IMP_LOG_ERR(TAG, "lux_LineCrossDet_exit failed\n");
    }

    Thread_Mutex_Destroy(&g_linedet_attr.mux);
    Thread_Mutex_Destroy(&g_linedet_attr.paramMux);
    return ret;
}