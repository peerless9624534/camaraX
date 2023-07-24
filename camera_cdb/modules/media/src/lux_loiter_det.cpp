#include "lux_loiter_det.hpp"
#define TAG "lux_loiter_det"

static UINT_X loiter_fliter_cnt = 0;
LUX_ALG_LOITERDET_ATTR_ST g_loiterdet_attr =
{
    .bInit = false,
    .IsRunning = false,
    .alarmTime = 0,
};

VOID_X LUX_ALG_LoiterDet_Start()
{
    g_loiterdet_attr.bInit = true;
    return;
}

VOID_X LUX_ALG_LoiterDet_Stop()
{
    g_loiterdet_attr.bInit = false;
    return;
}

bool LUX_LoiterEvent_IsAlarmByInterval(UINT_X lastTime)
{
    UINT_X timeInterval = 0;
    UINT_X curTime = 0;
    bool bAlarm = false;
#if 0
    if (1 == recordReboot)  //重新上电读取配置文件值
    {
        CHAR_X *p_getalarmInterval = IPC_APP_get_alarm_interval_X();
        g_loiterdet_attr.timeInterval = (LUX_IVS_TIMEINTERVAL_EN)(*p_getalarmInterval - '0');
        recordReboot = 0;
    }
#endif
    switch (g_loiterdet_attr.timeInterval)
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
 * @description: 徘徊侦测事件报警间隔
 * @param  [in] ts 时间间隔,单位s
 * @return 0 成功，非零失败，返回错误码
 */
VOID_X LUX_ALG_LoiterDet_SetTimeInterval(LUX_IVS_TIMEINTERVAL_EN ts)
{
    Thread_Mutex_Lock(&g_loiterdet_attr.paramMux);
    if(ts < LUX_IVS_TIMEINTERVAL_1MIN || ts >= LUX_IVS_TIME_INTERVAL_BUTTON)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_LoiterDet_SetTimeInterval failed, not support param [%d]\n", ts);
        g_loiterdet_attr.timeInterval = LUX_IVS_TIMEINTERVAL_1MIN;
    }

    switch (ts)
    {
    case LUX_IVS_TIMEINTERVAL_1MIN ... LUX_IVS_TIMEINTERVAL_5MIN:
        g_loiterdet_attr.timeInterval = ts;
        break;
    case LUX_IVS_TIME_INTERVAL_BUTTON:
    default:
        IMP_LOG_ERR(TAG, "LUX_ALG_LoiterDet_SetTimeInterval failed, not support param [%d]\n", ts);
        g_loiterdet_attr.timeInterval = LUX_IVS_TIMEINTERVAL_1MIN;
        break;
    }
    Thread_Mutex_UnLock(&g_loiterdet_attr.paramMux);
    return;
}

INT_X LUX_ALG_LoiterDet_SetConf()
{
    INT_X i;
    INT_X ret = LUX_ERR;
    loiterdet_param_input_t inParam;

    /*隐私模式下不开启徘徊侦测*/
    if(LUX_TRUE == g_configData->alg_config.sleepMode)
    {
        LUX_ALG_LoiterDet_Stop();
        printf("+++++++++++++++++ Line[%d] Function[%s] loiter_det off +++++++++++++++++\n", __LINE__, __FUNCTION__);
    }
    else
    {
        /*设置是否开启徘徊侦测警告*/
        if (LUX_TRUE == g_configData->alg_config.loiter_det_enabled)
        {
            LUX_ALG_LoiterDet_Start();
            printf("+++++++++++++++++ Line[%d] Function[%s] loiter_det on +++++++++++++++++\n", __LINE__, __FUNCTION__);
            LUX_ALG_LoiterDet_GetParam(&inParam);
            printf("+++ default: single_move=%u +++\n", inParam.single_move);
            inParam.single_move = 120;
            LUX_ALG_LoiterDet_SetParam(&inParam);
            memset(&inParam, 0, sizeof(loiterdet_param_input_t));
            LUX_ALG_LoiterDet_GetParam(&inParam);
            printf("+++ set after: single_move=%u +++\n", inParam.single_move);
        }
        else
        {
            LUX_ALG_LoiterDet_Stop();
            printf("+++++++++++++++++ Line[%d] Function[%s] loiter_det off +++++++++++++++++\n", __LINE__, __FUNCTION__);
        }
    }
    return LUX_OK;
}

/**
 * @description: 获取徘徊侦测的属性
 * @param  [in] param 属性
 */
VOID_X LUX_ALG_LoiterDet_GetParam(loiterdet_param_input_t* pInParam)
{
    if (pInParam == nullptr)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_LoiterDet_GetParam failed.\n");
        return;
    }

    Thread_Mutex_Lock(&g_loiterdet_attr.paramMux);
    memcpy(pInParam, &g_loiterdet_attr.in_param, sizeof(loiterdet_param_input_t));
    Thread_Mutex_UnLock(&g_loiterdet_attr.paramMux);
}

/**
 * @description: 设置徘徊侦测的属性
 * @param  [in] param 属性
 */
VOID_X LUX_ALG_LoiterDet_SetParam(loiterdet_param_input_t* pInParam)
{
    if (pInParam == nullptr)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_LoiterDet_SetParam failed.\n");
        return;
    }

    Thread_Mutex_Lock(&g_loiterdet_attr.paramMux);
    memcpy(&g_loiterdet_attr.in_param, pInParam, sizeof(loiterdet_param_input_t));
    Thread_Mutex_UnLock(&g_loiterdet_attr.paramMux);
}

/**
 * @description: 篡改侦测报警事件函数
 * @param [in] TDResult tamper算法识别结果
 * @return 0 成功，非零失败
 */
INT_X LUX_ALG_LoiterDet_Alarm()
{
    Thread_Mutex_Lock(&g_loiterdet_attr.paramMux);
    /*报警时间间隔*/
    if (LUX_LoiterEvent_IsAlarmByInterval(g_loiterdet_attr.alarmTime))
    {
        g_loiterdet_attr.alarmTime = getTime_getS();
        /*上报抓图 */   //TODO

        /* 云存储 */    //TODO

        /*事件录像*/    //TODO
        printf("*** LUX_ALG_LoiterDet_Alarm ***\n\n\n");
    }
    Thread_Mutex_UnLock(&g_loiterdet_attr.paramMux);
    return LUX_OK;
}

INT_X LUX_ALG_LoiterDet_GetResult(persondet_param_output_t *p_result, loiterdet_param_input_t *p_in, loiterdet_param_output_t &p_out)
{
    if (p_result == nullptr || nullptr == p_in)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_LoiterDet_GetResult failed.\n");
        return LUX_PARM_NULL_PTR;
    }
    INT_X i, ret = LUX_ERR;
    personBox box;
    loiterdet_person_input_t boxs;
    boxs.clear();

    if (p_result->count > LUX_LOITER_NUM_MAX)
    {
        p_result->count = LUX_LOITER_NUM_MAX;
    }

    /*人形侦测算法获取的人形坐标*/
    for (int i = 0; i < p_result->count; i++)
    {
        memset(&box, 0, sizeof(personBox));
        box.x1 = p_result->person[i].show_box.ul.x;
        box.y1 = p_result->person[i].show_box.ul.y;
        box.x2 = p_result->person[i].show_box.br.x;
        box.y2 = p_result->person[i].show_box.br.y;
        boxs.push_back(box);
    }

    /*获取徘徊结果*/
    ret = loiter_det_start(p_in, boxs, p_out);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "loiter_det_start failed.\n");
    }

    return ret;
}

VOID_X LUX_ALG_LoiterDet_Process()
{
    INT_X ret = LUX_ERR;
    ULONG_X nowtime = 0;
    persondet_param_output_t show_data;

    Thread_Mutex_Lock(&g_loiterdet_attr.mux);
    do
    {
        if (g_persondet_attr.ivsPersonDetResult)
        {
            g_loiterdet_attr.loiterResult.clear();
            nowtime = getTime_getUs();
            ret = LUX_ALG_LoiterDet_GetResult(g_persondet_attr.ivsPersonDetResult, &g_loiterdet_attr.in_param, g_loiterdet_attr.loiterResult);
            if (LUX_OK != ret)
            {
                IMP_LOG_ERR(TAG, "LUX_ALG_LoiterDet_GetResult failed.\n");
                break;
            }

            if (g_loiterdet_attr.loiterResult.size() > 0)
            {
#if 0
                if (loiter_fliter_cnt == LOITER_DET_FLITER_NUMS)
                {
                    printf("time: [%u]us Detected loiter, loiter nums=[%d]\n\n\n", getTime_getUs() - nowtime, g_loiterdet_attr.loiterResult.size());
                    loiter_fliter_cnt = 0;
                }
                loiter_fliter_cnt++;
#endif
                LUX_ALG_LoiterDet_Alarm();
                memset(&show_data, 0, sizeof(persondet_param_output_t));
                show_data.count = g_loiterdet_attr.loiterResult.size();
                for (int i = 0; i < show_data.count; i++)
                {
                    show_data.person[i].show_box.ul.x = g_loiterdet_attr.loiterResult[i].p1.first;
                    show_data.person[i].show_box.ul.y = g_loiterdet_attr.loiterResult[i].p1.second;
                    show_data.person[i].show_box.br.x = g_loiterdet_attr.loiterResult[i].p2.first;
                    show_data.person[i].show_box.br.y = g_loiterdet_attr.loiterResult[i].p2.second;
                #if 0
                    printf("i=[%d], x1=[%d] y1=[%d] x2=[%d] y2=[%d]\n", i, 
                    show_data.person[i].show_box.ul.x,
                    show_data.person[i].show_box.ul.y,
                    show_data.person[i].show_box.br.x,
                    show_data.person[i].show_box.br.y);
                #endif
                }
                LUX_OSD_LoiterRect_Miss();
                LUX_OSD_LoiterRect_Show(&show_data);
            }
            else
            {
                LUX_OSD_LoiterRect_Miss();
            }
        }
    }while (0);
    Thread_Mutex_UnLock(&g_loiterdet_attr.mux);
    usleep(LOITERDET_DELAY_TIME);
    return;
}

VOID_X loiter_det_process_thread()
{
    CHAR_X threadName[64] = {0};
    CHAR_X ivsChnName[32] = {0};

    strcpy(ivsChnName, "LOITER_DETECTION");
    g_loiterdet_attr.IsRunning = true;

    sprintf(threadName, "ALG_%s", ivsChnName);
    prctl(PR_SET_NAME, threadName);
    while (g_loiterdet_attr.IsRunning)
    {
        if (!g_loiterdet_attr.bInit || !g_persondet_attr.bStart)
        {
            usleep(SLEEP_TIME);
            continue;
        }
        else
        {
            LUX_ALG_LoiterDet_Process();
        }
    }
    return;
}

INT_X LUX_ALG_LoiterDet_Init()
{
    INT_X ret = LUX_ERR;
    loiter_det_init();

    Thread_Mutex_Create(&g_loiterdet_attr.paramMux);
    Thread_Mutex_Create(&g_loiterdet_attr.mux);

    loiterdet_param_input_t ldParam;
    ldParam.remain_time = 10;
    ldParam.lost_time = 10;
    ldParam.single_move = 100;
    ldParam.max_move = g_framSrcAttr->fsChn[LUX_STREAM_SUB].fsChnAttr.picWidth + g_framSrcAttr->fsChn[LUX_STREAM_SUB].fsChnAttr.picHeight;
    LUX_ALG_LoiterDet_SetParam(&ldParam);

    ret = LUX_ALG_LoiterDet_SetConf();
    if (ret != LUX_OK) {
        IMP_LOG_ERR(TAG, "LUX_ALG_LoiterDetConf failed\n");
    }

    thread t_loiter(loiter_det_process_thread);
    t_loiter.detach();
    return LUX_OK;
}

INT_X LUX_ALG_LoiterDet_Exit()
{
    g_loiterdet_attr.IsRunning = false;
    LUX_ALG_LoiterDet_Stop();
    loiter_det_exit();

    Thread_Mutex_Destroy(&g_loiterdet_attr.mux);
    Thread_Mutex_Destroy(&g_loiterdet_attr.paramMux);
    return LUX_OK;
}