#include "lux_tamper_det.hpp"
#define TAG "lux_tamper_det"

static UCHAR_X tamper_cnt = 0;
static UINT_X cover_time = 0;

LUX_TAMPER_DET_ATTR_ST g_tamperdet_attr = 
{
    .bInit = false,
    .IsRunning = false,
    .alarmTime = 0,
    .first_cover_time = 0,
    .timeInterval = LUX_IVS_TIMEINTERVAL_1MIN,
    .in_param = {0},
};

VOID_X LUX_ALG_TamperDet_Start()
{
    g_tamperdet_attr.bInit = true;
}

VOID_X LUX_ALG_TamperDet_Stop()
{
    g_tamperdet_attr.bInit = false;
}

bool LUX_TamperEvent_IsAlarmByTime(UINT_X lastTime)
{
    UINT_X timeInterval = 0;
    UINT_X curTime = 0;
    bool bAlarm = false;
#if 0
    if (1 == recordReboot)  //重新上电读取配置文件值
    {
        CHAR_X *p_getalarmInterval = IPC_APP_get_alarm_interval_X();
        g_tamperdet_attr.timeIntervel = (LUX_IVS_TIMEINTERVAL_EN)(*p_getalarmInterval - '0');
        recordReboot = 0;
    }
#endif

    curTime = getTime_getS();
    if (curTime - lastTime > g_tamperdet_attr.in_param.sense)
    {
        bAlarm = true;
    }
    //printf("INFO: %s:%d timeInterval:%d curTime:%d lastTime:%d-%d bAlarm:%d\n",
    //       __func__, __LINE__, timeInterval, curTime, lastTime, *pLastTime, bAlarm);

    return bAlarm;
}

bool LUX_TamperEvent_IsAlarmByInterval(UINT_X lastTime)
{
    UINT_X timeInterval = 0;
    UINT_X curTime = 0;
    bool bAlarm = false;
#if 0
    if (1 == recordReboot)  //重新上电读取配置文件值
    {
        CHAR_X *p_getalarmInterval = IPC_APP_get_alarm_interval_X();
        g_tamperdet_attr.timeIntervel = (LUX_IVS_TIMEINTERVAL_EN)(*p_getalarmInterval - '0');
        recordReboot = 0;
    }
#endif
    switch (g_tamperdet_attr.timeInterval)
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
 * @description: 篡改侦测事件报警间隔
 * @param  [in] ts 时间间隔,单位s
 * @return 0 成功，非零失败，返回错误码
 */
VOID_X LUX_ALG_TamperDet_SetTimeInterval(LUX_IVS_TIMEINTERVAL_EN ts)
{
    Thread_Mutex_Lock(&g_tamperdet_attr.paramMux);
    if(ts < LUX_IVS_TIMEINTERVAL_1MIN || ts >= LUX_IVS_TIME_INTERVAL_BUTTON)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_TamperDet_SetTimeInterval failed, not support param [%d]\n", ts);
        g_tamperdet_attr.timeInterval = LUX_IVS_TIMEINTERVAL_1MIN;
    }

    switch (ts)
    {
    case LUX_IVS_TIMEINTERVAL_1MIN ... LUX_IVS_TIMEINTERVAL_5MIN:
        g_tamperdet_attr.timeInterval = ts;
        break;
    case LUX_IVS_TIME_INTERVAL_BUTTON:
    default:
        IMP_LOG_ERR(TAG, "LUX_ALG_TamperDet_SetTimeInterval failed, not support param [%d]\n", ts);
        g_tamperdet_attr.timeInterval = LUX_IVS_TIMEINTERVAL_1MIN;
        break;
    }
    Thread_Mutex_UnLock(&g_tamperdet_attr.paramMux);
}

/**
 * @description: 设置篡改侦测算法灵敏度
 * @param [in] sns   灵敏度
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_ALG_TamperDet_SetSense(LUX_IVS_SENSITIVE_EN sns)
{
    INT_X ret = LUX_ERR;
    UINT_X TDSense = 0;
    tamper_param_input_t TDParam;

    memset(&TDParam, 0, sizeof(tamper_param_input_t));

    do {
        switch (sns)
        {
        case LUX_IVS_SENSITIVE_LOW_EN:
            TDSense = 30;
            break;
        case LUX_IVS_SENSITIVE_MID_EN:
            TDSense = 20;
            break;
        case LUX_IVS_SENSITIVE_HIG_EN:
            TDSense = 10;
            break;
        default:
            IMP_LOG_ERR(TAG, "LUX_ALG_TamperDet_SetSense failed, not suppport param,\
                            it will be set default param [%d]  \n", LUX_IVS_SENSITIVE_LOW_EN);
            TDSense = 20;
            break;
        }

        ret = LUX_ALG_TamperDet_GetParam(&TDParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_ALG_TamperDet_GetParam failed\n");
            ret = LUX_IVS_GET_PARAM_ERR;
            break;
        }

        TDParam.sense = TDSense;

        ret = LUX_ALG_TamperDet_SetParam(&TDParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_ALG_TamperDet_SetParam failed\n");
            ret = LUX_IVS_SET_PARAM_ERR;
        }
    } while (0);
    return ret;
}

/**
 * @description: 设置篡改侦测算法遮挡阈值
 * @param [in] sns   灵敏度
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_ALG_TamperDet_SetCoverThreshold(UINT_X cover_th)
{
    INT_X ret = LUX_ERR;
    UINT_X TDCoverTh = 0;
    tamper_param_input_t TDParam;

    memset(&TDParam, 0, sizeof(tamper_param_input_t));

    do {
        switch (cover_th)
        {
        case 0 ... 100:
            TDCoverTh = cover_th;
            break;
        default:
            IMP_LOG_ERR(TAG, "LUX_ALG_TamperDet_SetSense failed, not suppport param,\
                            it will be set default param cover_th=[%d]  \n", 60);
            TDCoverTh = 60;
            break;
        }

        ret = LUX_ALG_TamperDet_GetParam(&TDParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_ALG_TamperDet_GetParam failed\n");
            ret = LUX_IVS_GET_PARAM_ERR;
            break;
        }

        TDParam.cover_th = TDCoverTh;

        ret = LUX_ALG_TamperDet_SetParam(&TDParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_ALG_TamperDet_SetParam failed\n");
            ret = LUX_IVS_SET_PARAM_ERR;
        }
    } while (0);
    return ret;
}

/**
 * @description: 篡改侦测启动设置配置
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_ALG_TamperDet_SetConf()
{
    INT_X ret = LUX_ERR;

    /*隐私模式下不开启篡改侦测*/
    if(LUX_TRUE == g_configData->alg_config.sleepMode)
    {
        LUX_ALG_TamperDet_Stop();
        printf("+++++++++++++++++ Line[%d] Function[%s] tampering_det off +++++++++++++++++\n", __LINE__, __FUNCTION__);
    }
    else
    {
        if (LUX_TRUE == g_configData->alg_config.tamper_det_enabled)
        {
            LUX_ALG_TamperDet_Start();
            printf("+++++++++++++++++ Line[%d] Function[%s] tampering_det on +++++++++++++++++\n", __LINE__, __FUNCTION__);
            ret = LUX_ALG_TamperDet_SetCoverThreshold(g_configData->alg_config.tamper_cover_th);
            if (LUX_OK != ret)
            {
                IMP_LOG_ERR(TAG, "LUX_ALG_TamperDet_SetSense failed\n");
                return LUX_IVS_SET_PARAM_ERR;
            }
            printf("LUX_ALG_TamperDet_SetCoverThreshold success\n");
            ret = LUX_ALG_TamperDet_SetSense((LUX_IVS_SENSITIVE_EN)g_configData->alg_config.tamper_det_sensitivity);
            if (LUX_OK != ret)
            {
                IMP_LOG_ERR(TAG, "LUX_ALG_TamperDet_SetSense failed\n");
                return LUX_IVS_SET_PARAM_ERR;
            }
            printf("LUX_ALG_TamperDet_SetSense success\n");
            printf("tamper det set sense=%d\n", g_tamperdet_attr.in_param.sense);
        }
        else
        {
            LUX_ALG_TamperDet_Stop();
            printf("+++++++++++++++++ Line[%d] Function[%s] tampering_det off +++++++++++++++++\n", __LINE__, __FUNCTION__);
        }
    }
    printf("+++++++++++++++++ Line[%d] Function[%s] tampering_det cover threshold [%d] +++++++++++++++++\n", __LINE__, __FUNCTION__, g_configData->alg_config.tamper_cover_th);
    printf("+++++++++++++++++ Line[%d] Function[%s] tampering_det sensitive [%d] +++++++++++++++++\n", __LINE__, __FUNCTION__, g_configData->alg_config.tamper_det_sensitivity);
    return LUX_OK;
}

INT_X LUX_ALG_TamperDet_GetParam(tamper_param_input_t* pInParam)
{
    if (pInParam == NULL)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_TamperDet_GetParam failed, empty point.\n");
        return LUX_PARM_NULL_PTR;
    }
    Thread_Mutex_Lock(&g_tamperdet_attr.paramMux);
    memcpy(pInParam, &g_tamperdet_attr.in_param, sizeof(tamper_param_input_t));
    Thread_Mutex_UnLock(&g_tamperdet_attr.paramMux);
    return LUX_OK;
}

INT_X LUX_ALG_TamperDet_SetParam(tamper_param_input_t* pInParam)
{
    if (pInParam == NULL)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_TamperDet_SetParam failed, empty point.\n");
        return LUX_PARM_NULL_PTR;
    }
    Thread_Mutex_Lock(&g_tamperdet_attr.paramMux);
    memcpy(&g_tamperdet_attr.in_param, pInParam, sizeof(tamper_param_input_t));
    Thread_Mutex_UnLock(&g_tamperdet_attr.paramMux);
    return LUX_OK;
}

/**
 * @description: 篡改侦测报警事件函数
 * @return 0 成功，非零失败
 */
INT_X LUX_ALG_TamperDet_Alarm()
{
    Thread_Mutex_Lock(&g_tamperdet_attr.paramMux);
    /*报警时间间隔*/
    if (LUX_TamperEvent_IsAlarmByInterval(g_tamperdet_attr.alarmTime))
    {
        g_tamperdet_attr.alarmTime = getTime_getS();
        /*上报抓图 */   //TODO

        /* 云存储 */    //TODO

        /*事件录像*/    //TODO
        printf("*** LUX_ALG_TamperDet_Alarm ***\n\n\n");
    }
    Thread_Mutex_UnLock(&g_tamperdet_attr.paramMux);
    return LUX_OK;
}

INT_X LUX_ALG_TamperDet_GetResult(tamper_param_input_t *p_in, tamper_param_output_t *p_out)
{
    if (nullptr == p_in || p_out == nullptr)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_TamperDet_GetResult failed.\n");
        return LUX_PARM_NULL_PTR;
    }
    INT_X i = 0;
    INT_X ret = LUX_ERR;
    input_info_t img_info;    //传入算法处理的图像信息
    IMPFrameInfo* pFrame = nullptr;

    Thread_Mutex_Lock(&g_framSrcAttr->fsChn[LUX_STREAM_SUB].mux);
    do
    {
        /*获取帧*/
        ret = IMP_FrameSource_GetFrame(LUX_STREAM_SUB, &pFrame);
        if (ret != LUX_OK) {
            IMP_LOG_ERR(TAG, "IMP_FrameSource_GetFrame failed\n");
            break;
        }
        // printf("tamperDet frame info: w=%u, h=%u, size=%u\n", pFrame->width, pFrame->height, pFrame->size);

        /*转换数据格式*/
        memset(&img_info, 0, sizeof(input_info_t));
        img_info.image = (unsigned char*)pFrame->virAddr;
        img_info.w = pFrame->width;
        img_info.h = pFrame->height;

        /*获取结果*/
        LUX_DAYNIGHT_MODE_EN daynight_mode;
        LUX_ISP_GetDayDightMode(&daynight_mode);
        // printf("++++++ daynight=%d +++++\n",daynight_mode);
        if (daynight_mode == LUX_DAYNIGHT_MODE_DAY)
        {
            p_out->ret = (TAMPER_TYPE_EN)lux_TamperingDet_start(&img_info, p_in);
        }
        else if (daynight_mode == LUX_DAYNIGHT_MODE_NIGHT)
        {
            p_out->ret = (TAMPER_TYPE_EN)lux_TamperingDet_start_yeshi(&img_info, p_in);
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

VOID_X LUX_ALG_TamperDet_Process()
{
    UINT_X nowtime = 0;
    INT_X ret = LUX_ERR;

    Thread_Mutex_Lock(&g_tamperdet_attr.mux);
    do
    {
        g_tamperdet_attr.tamperResult.ret = TAMPER_TYPE_NO_DECETED;
        nowtime = getTime_getMs();
        /*获取结果*/
        ret = LUX_ALG_TamperDet_GetResult(&g_tamperdet_attr.in_param, &g_tamperdet_attr.tamperResult);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_ALG_TamperDet_GetResult failed.\n");
            break;
        }

        /*过滤结果打印*/
        switch (g_tamperdet_attr.tamperResult.ret)
        {
            case TAMPER_TYPE_MOVE:
                printf("time: [%u]ms Tampering occurs\n\n\n", getTime_getMs() - nowtime);
                /*报警消息*/
                LUX_ALG_TamperDet_Alarm();
                g_tamperdet_attr.tamperResult.box.clear();
                break;
            case TAMPER_TYPE_SPRACY_COVER:
                if (g_tamperdet_attr.tamperResult.box.empty())
                {
                    g_tamperdet_attr.first_cover_time = gettimeStampS();
                }
                cover_time = gettimeStampS();
                g_tamperdet_attr.tamperResult.box.push_back(ret);
                if ((cover_time - g_tamperdet_attr.first_cover_time) > g_tamperdet_attr.in_param.sense &&
                    (g_tamperdet_attr.tamperResult.box.size() >= g_tamperdet_attr.in_param.sense * 2))
                {
                    // printf("++++ cover_time=%u, first_cover_time=%u ++++\n", cover_time, g_tamperdet_attr.first_cover_time);
                    printf("time: [%u]ms Tampering occurs\n\n\n", getTime_getMs() - nowtime);
                    /*报警消息*/
                    LUX_ALG_TamperDet_Alarm();
                    g_tamperdet_attr.tamperResult.box.clear();
                }
                break;
            default:
                g_tamperdet_attr.tamperResult.box.clear();
                break;
        }
    } while (0);
    Thread_Mutex_UnLock(&g_tamperdet_attr.mux);
    usleep(TAMPERDET_DELAY_TIME);
    return;
}

VOID_X tampering_det_process_thread()
{
    CHAR_X threadName[64] = {0};
    CHAR_X ivsChnName[32] = {0};

    strcpy(ivsChnName, "TAMPER_DETECTION");
    g_tamperdet_attr.IsRunning = true;

    sprintf(threadName, "ALG_%s", ivsChnName);
    prctl(PR_SET_NAME, threadName);
    tamper_param_input_t testParam;
    LUX_ALG_TamperDet_GetParam(&testParam);
    while (g_tamperdet_attr.IsRunning)
    {
        if (!g_tamperdet_attr.bInit || !g_framSrcAttr->bInit)
        {
            usleep(SLEEP_TIME);
            continue;
        }
        else
        {
            LUX_ALG_TamperDet_Process();
        }
    }
    return;
}

INT_X LUX_ALG_TamperDet_Init()
{
    INT_X ret = LUX_ERR;
    Thread_Mutex_Create(&g_tamperdet_attr.paramMux);
    Thread_Mutex_Create(&g_tamperdet_attr.mux);

    tamper_param_input_t tdParam;
    tdParam.cover_th = 60;
    LUX_ALG_TamperDet_SetParam(&tdParam);

    ret = lux_TamperingDet_init();
    if (ret == LUX_ERR)
    {
        IMP_LOG_ERR(TAG, "lux_TamperDet_init failed\n");
        return ret;
    }

    ret = LUX_ALG_TamperDet_SetConf();
    if (ret != LUX_OK) {
        IMP_LOG_ERR(TAG, "LUX_ALG_TamperDetConf failed\n");
    }
    g_tamperdet_attr.tamperResult.box.clear();
    thread t_tamper(tampering_det_process_thread);
    t_tamper.detach();
    return LUX_OK;
}

INT_X LUX_ALG_TamperDet_Exit()
{
    g_tamperdet_attr.IsRunning = false;
    LUX_ALG_TamperDet_Stop();
    INT_X ret = lux_TamperingDet_exit();
    if (ret == LUX_ERR)
    {
        IMP_LOG_ERR(TAG, "lux_TamperDet_exit failed\n");
    }
    Thread_Mutex_Destroy(&g_tamperdet_attr.mux);
    Thread_Mutex_Destroy(&g_tamperdet_attr.paramMux);
    return ret;
}