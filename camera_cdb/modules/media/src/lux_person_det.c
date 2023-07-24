#include "lux_person_det.h"

#define TAG "lux_person_det"

/*打印结果过滤次数*/
static UCHAR_X person_cnt = 0;
/* 未侦测到人形移动次数 */
static UCHAR_X personMissCnt = 0;
/*人形侦测周界*/
static IVSPoint g_persondet_perm[MAX_PERSON_DET_PERM_CNT][PERSON_DET_PERM_POINTCNT];

LUX_IVS_PERSONDET_ATTR_ST g_persondet_attr =
{
    .bInit = LUX_FALSE,
    .alarmZone.bOpen = LUX_TRUE,
    .alarmTime = 0,
    .timeInterval = LUX_IVS_TIMEINTERVAL_1MIN,
    .bindAttr =
    {
        .grpId = 0,
        .chnNum = LUX_IVS_PERSON_DET_EN,
        .bindSrcChan = 1,
        .bindSrcModule = LUX_MODULS_FSRC,
        .bindDstChan = 0,                   /*绑定的channal相当于君正的group*/
        .bindDstModule = LUX_MODULS_IVS,
    },
    .in_param =
    {
        /* T40 7.2.0-1.2.0 */
        .frameInfo.width = SENSOR_WIDTH_SECOND,
        .frameInfo.height = SENSOR_HEIGHT_SECOND,
        .skip_num = 0,      //skip num
        .max_person_box = LUX_HUMANRECT_NUM_MAX,  //<=50
        .sense = 4,
        .detdist = 3,
        .switch_track = LUX_FALSE,
        .enable_move = LUX_FALSE,
        .open_move_filter = LUX_TRUE,
        .model_path = "/system/lib/model/person_det_11M.bin",
        .model_type = 3,      //4个不同大小的人形模型 0:2M  1:3.9M  2:6.3M  3:11M (T40N)
        .enable_perm = LUX_FALSE,
        .permcnt = LUX_IVS_REGION_NUM,
        .perms[0].pcnt = PERSON_DET_PERM_POINTCNT,
        .mod = 0,
        .switch_stop_det = LUX_FALSE,
        .fast_update_params = LUX_FALSE,
        .ptime = LUX_FALSE,
#if 0
        .nmem_size = 22*1024*1024,      //2M:9M  3.9M:12.5M  6.3M:13M  11M:22M
        .share_memory_mode = 0,
#else
        .nmem_size = 0,//17*1024*1024,      //2M:5.5M  3.9M:7.5M  6.3M:11M  11M:17M
        .share_memory_mode = 1,
#endif
    },
};

extern LUX_FSRC_ATTR_ST* g_framSrcAttr;
extern LUX_CONFIG_DATA_ST* g_configData;
extern VOID_X LUX_OSD_PersonRect_Show(persondet_param_output_t* ivsPersonDetRet);

/**
 * @description: 人形侦测报警区域开关
 * @param [in] bOpen IVS处理结果
 * @return 0 成功，非零失败
 */
INT_X LUX_IVS_PD_AlarmZone_Open(BOOL_X bOpen)
{
    INT_X ret = LUX_OK;
    persondet_param_input_t PDParam;

    Thread_Mutex_Lock(&g_persondet_attr.paramMux);
    do {
        g_persondet_attr.alarmZone.bOpen = bOpen;

        /*人形侦测区域报警开关*/
        g_persondet_attr.in_param.enable_perm = bOpen;

        ret = IMP_IVS_GetParam(g_persondet_attr.bindAttr.chnNum, (PVOID_X)&PDParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_GetParam failed, chn(%d) . \n", g_persondet_attr.bindAttr.chnNum);
            ret = LUX_IVS_GET_PARAM_ERR;
            break;
        }

        PDParam.enable_perm = g_persondet_attr.in_param.enable_perm;

        ret = IMP_IVS_SetParam(g_persondet_attr.bindAttr.chnNum, (PVOID_X)&PDParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_SetParam failed, chn(%d) . \n", g_persondet_attr.bindAttr.chnNum);
            ret = LUX_IVS_GET_PARAM_ERR;
            break;
        }
    } while (0);
    Thread_Mutex_UnLock(&g_persondet_attr.paramMux);

    return ret;
}

/**
 * @description: 人形侦测获取报警区域开关
 * @return 0 成功，非零失败
 */
BOOL_X LUX_IVS_PD_GetAlarmZone_OnOff()
{
    BOOL_X OnOff = LUX_FALSE;

    Thread_Mutex_Lock(&g_persondet_attr.paramMux);
    OnOff = g_persondet_attr.alarmZone.bOpen;
    Thread_Mutex_UnLock(&g_persondet_attr.paramMux);

    return OnOff;
}

/**
 * @description: 设置人形侦测算法灵敏度
 * @param [in] sns   灵敏度
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PD_SetSense(LUX_IVS_SENSITIVE_EN sns)
{
    INT_X ret = LUX_ERR;
    INT_X PDSense = 0;
    persondet_param_input_t PDParam;

    memset(&PDParam, 0, sizeof(persondet_param_input_t));

    Thread_Mutex_Lock(&g_persondet_attr.paramMux);
    do {
        switch (sns)
        {
        case LUX_IVS_SENSITIVE_LOW_EN:
            PDSense = 2;
            break;
        case LUX_IVS_SENSITIVE_MID_EN:
            PDSense = 4;
            break;
        case LUX_IVS_SENSITIVE_HIG_EN:
            PDSense = 6;
            break;
        default:
            IMP_LOG_ERR(TAG, "LUX_IVS_PD_SetSense failed, chn(%d), not suppport param,\
                            it will be set default param [%d]  \n", g_persondet_attr.bindAttr.chnNum, LUX_IVS_SENSITIVE_LOW_EN);
            PDSense = 2;
            break;
        }

        ret = IMP_IVS_GetParam(g_persondet_attr.bindAttr.chnNum, (PVOID_X)&PDParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_GetParam failed, chn(%d) . \n", g_persondet_attr.bindAttr.chnNum);
            ret = LUX_IVS_GET_PARAM_ERR;
            break;
        }

        PDParam.sense = PDSense;

        ret = IMP_IVS_SetParam(g_persondet_attr.bindAttr.chnNum, (PVOID_X)&PDParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_SetParam chn(%d) failed. \n", g_persondet_attr.bindAttr.chnNum);
            ret = LUX_IVS_SET_PARAM_ERR;
            break;
        }
    } while (0);
    Thread_Mutex_UnLock(&g_persondet_attr.paramMux);
    return ret;
}

/**
 * @description: 开启人形侦测算法通道
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PD_StartChn()
{
    INT_X ret = LUX_OK;

    if (LUX_TRUE == g_persondet_attr.bInit)
    {
        IMP_LOG_ERR(TAG, "Warnning, LUX_IVS_PD_StartChn already Started\n");
        return LUX_OK;
    }

    Thread_Mutex_Lock(&g_persondet_attr.mux);
    do {
        ret = IMP_IVS_StartRecvPic(g_persondet_attr.bindAttr.chnNum);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_StartRecvPic failed\n");
            break;
        }
        printf("IMP_IVS_StartRecvPic g_persondet_attr.bindAttr.chnNum success\n");//test
        g_persondet_attr.bInit = LUX_TRUE;
    } while (0);
    Thread_Mutex_UnLock(&g_persondet_attr.mux);

    return ret;
}

/**
 * @description: 关闭人形侦测算法通道
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PD_StopChn()
{
    INT_X ret = LUX_OK;

    if (LUX_FALSE == g_persondet_attr.bInit)
    {
        IMP_LOG_ERR(TAG, "Warnning, LUX_IVS_PD_StopChn already Stopped\n");
        return LUX_OK;
    }

    Thread_Mutex_Lock(&g_persondet_attr.mux);
    do {
        ret = IMP_IVS_StopRecvPic(g_persondet_attr.bindAttr.chnNum);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_StopRecvPic failed\n");
            break;
        }

        g_persondet_attr.bInit = LUX_FALSE;
    } while (0);
    Thread_Mutex_UnLock(&g_persondet_attr.mux);

    return ret;
}

/**
 * @description: 开启IVS人形侦测
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PD_StartAlgo()
{
    INT_X ret = LUX_ERR;
    IMP_LOG_INFO(TAG, "LUX_IVS_PD_StartAlgo\n");
    ret = LUX_IVS_PD_StartChn();
    if (ret != LUX_OK)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PD_StartChn failed\n");
        return LUX_IVS_START_RECIVE_PIC_ERR;
    }
    g_persondet_attr.bStart = LUX_TRUE;
    return LUX_OK;
}

/**
 * @description: 关闭IVS人形侦测
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PD_StopAlgo()
{
    INT_X ret = LUX_ERR;
    IMP_LOG_INFO(TAG, "LUX_IVS_PD_StopAlgo\n");
    ret = LUX_IVS_PD_StopChn();
    if (ret != LUX_OK)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PD_StopChn failed\n");
        return LUX_IVS_STOP_RECIVE_PIC_ERR;
    }
    g_persondet_attr.bStart = LUX_FALSE;
    return LUX_OK;
}

/**
 * @description: 人形侦测事件报警间隔
 * @param  [in] ts 时间间隔,单位s
 * @return 0 成功，非零失败，返回错误码
 */
VOID_X LUX_IVS_PD_SetTimeInterval(LUX_IVS_TIMEINTERVAL_EN ts)
{
    Thread_Mutex_Lock(&g_persondet_attr.paramMux);
    if (ts < LUX_IVS_TIMEINTERVAL_1MIN || ts >= LUX_IVS_TIME_INTERVAL_BUTTON)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PD_SetTimeInterval failed, not support param [%d]\n", ts);
        g_persondet_attr.timeInterval = LUX_IVS_TIMEINTERVAL_1MIN;
    }

    switch (ts)
    {
    case LUX_IVS_TIMEINTERVAL_1MIN ... LUX_IVS_TIMEINTERVAL_5MIN:
        g_persondet_attr.timeInterval = ts;
        break;
    case LUX_IVS_TIME_INTERVAL_BUTTON:
    default:
        IMP_LOG_ERR(TAG, "LUX_IVS_PD_SetTimeInterval failed, not support param [%d]\n", ts);
        g_persondet_attr.timeInterval = LUX_IVS_TIMEINTERVAL_1MIN;
        break;
    }
    Thread_Mutex_UnLock(&g_persondet_attr.paramMux);

    return;
}

/**
 * @description: 设置人形侦测报警区域
 * @param  [in] zoneInfo  坐标信息
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PD_SetRegion(LUX_IVS_M_ZONE_BASE_ST* zoneInfo)
{
    if (NULL == zoneInfo)
    {
        IMP_LOG_ERR(TAG, "LUX_Ivs_SetPersonDet_Region failed, empty point.\n");
        return LUX_PARM_NULL_PTR;
    }

    INT_X i, j, ret = LUX_OK;
    persondet_param_input_t PDParam;
    DOUBLE_X widthRate = g_persondet_attr.in_param.frameInfo.width / 100.0;
    DOUBLE_X heightRate = g_persondet_attr.in_param.frameInfo.height / 100.0;

    Thread_Mutex_Lock(&g_persondet_attr.paramMux);
    do {
        memset(&PDParam, 0, sizeof(persondet_param_input_t));
        ret = IMP_IVS_GetParam(g_persondet_attr.bindAttr.chnNum, (PVOID_X)&PDParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_GetParam failed, chn(%d) . \n", g_persondet_attr.bindAttr.chnNum);
            ret = LUX_IVS_GET_PARAM_ERR;
            break;
        }
        printf("IMP_IVS_GetParam PersonDet Success\n");

        g_persondet_attr.in_param.permcnt = zoneInfo->regionCnt;
        PDParam.permcnt = g_persondet_attr.in_param.permcnt;
        for (i = 0; i < PDParam.permcnt; i++)
        {
            /*左上角为(0,0)p0点, 顺时针排列顺序, 依次为p0,p1,p2,p3*/
            g_persondet_attr.in_param.perms[i].p[0].x = alignment_down(((INT_X)(zoneInfo->rect[i].pointX * widthRate)), 1);
            g_persondet_attr.in_param.perms[i].p[0].y = alignment_down(((INT_X)(zoneInfo->rect[i].pointY * heightRate)), 1);
            g_persondet_attr.in_param.perms[i].p[1].x = alignment_down(((INT_X)((zoneInfo->rect[i].pointX + zoneInfo->rect[i].width) * widthRate) - 1), 1);
            g_persondet_attr.in_param.perms[i].p[1].y = alignment_down(((INT_X)(zoneInfo->rect[i].pointY * heightRate)), 1);
            g_persondet_attr.in_param.perms[i].p[2].x = alignment_down(((INT_X)((zoneInfo->rect[i].pointX + zoneInfo->rect[i].width) * widthRate) - 1), 1);
            g_persondet_attr.in_param.perms[i].p[2].y = alignment_down(((INT_X)((zoneInfo->rect[i].pointY + zoneInfo->rect[i].height) * heightRate) - 1), 1);
            g_persondet_attr.in_param.perms[i].p[3].x = alignment_down(((INT_X)(zoneInfo->rect[i].pointX * widthRate)), 1);
            g_persondet_attr.in_param.perms[i].p[3].y = alignment_down(((INT_X)((zoneInfo->rect[i].pointY + zoneInfo->rect[i].height) * heightRate) - 1), 1);
            for (j = 0; j < 4; j++)
            {
                PDParam.perms[i].p[j].x = g_persondet_attr.in_param.perms[i].p[j].x;
                PDParam.perms[i].p[j].y = g_persondet_attr.in_param.perms[i].p[j].y;
            }
            IMP_LOG_INFO(TAG, "LUX_Ivs_SetPersonDet_Region region[%d]: { P0(%d,%d),P1(%d,%d),P2(%d,%d),P3(%d,%d) }. \n", i,
                    PDParam.perms[i].p[0].x, PDParam.perms[i].p[0].y, PDParam.perms[i].p[1].x, PDParam.perms[i].p[1].y,
                    PDParam.perms[i].p[2].x, PDParam.perms[i].p[2].y, PDParam.perms[i].p[3].x, PDParam.perms[i].p[3].y);
        }

        ret = IMP_IVS_SetParam(g_persondet_attr.bindAttr.chnNum, (PVOID_X)&PDParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_SetParam failed, chn(%d) . \n", g_persondet_attr.bindAttr.chnNum);
            ret = LUX_IVS_SET_PARAM_ERR;
            break;
        }
        printf("IMP_IVS_SetParam PersonDet Success\n");
    } while (0);
    Thread_Mutex_UnLock(&g_persondet_attr.paramMux);

    return ret;
}

/**
 * @description: 设置人形侦测的属性
 * @param  [in] param 属性
 */
VOID_X LUX_IVS_PD_SetAttr(persondet_param_input_t* pParam)
{
    INT_X i;
    Thread_Mutex_Lock(&g_persondet_attr.paramMux);
    for (i = 0; i < MAX_PERSON_DET_PERM_CNT; i++)
    {
        g_persondet_attr.in_param.perms[i].p = &g_persondet_perm[i][0];
    }
    memcpy(pParam, &g_persondet_attr.in_param, sizeof(persondet_param_input_t));
    Thread_Mutex_UnLock(&g_persondet_attr.paramMux);

    return;
}

/**
 * @description: 人形侦测启动设置配置
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PD_SetConf()
{
    INT_X ret = LUX_ERR;
    INT_X alarmSen = LUX_IVS_SENSITIVE_LOW_EN;
    BOOL_X alarmZoneOnOff = LUX_FALSE;
    CHAR_X strAlarmZoneInfo[1024] = { 0 };
    LUX_IVS_M_ZONE_BASE_ST alarmZone;
    BOOL_X sleepMode = LUX_FALSE;

    ret = LUX_IVS_PD_StartChn();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PD_StartChn(%d) failed\n", g_persondet_attr.bindAttr.chnNum);
        return LUX_IVS_START_RECIVE_PIC_ERR;
    }

    /*隐私模式下不开启人形侦测*/
    if (LUX_TRUE == g_configData->alg_config.sleepMode)
    {
        ret = LUX_IVS_PD_StopAlgo();
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_IVS_PD_StopAlgo failed\n");
            return LUX_IVS_STOP_RECIVE_PIC_ERR;
        }
        printf("+++++++++++++++++ Line[%d] Function[%s] person_det off +++++++++++++++++\n", __LINE__, __FUNCTION__);
    }
    else
    {
        printf("+++++++++++============= g_configData->alg_config.human_detect_enabled:%d\n", g_configData->alg_config.human_detect_enabled);
        if (LUX_TRUE == g_configData->alg_config.human_detect_enabled)
        {
            ret = LUX_IVS_PD_StartAlgo();
            if (LUX_OK != ret)
            {
                IMP_LOG_ERR(TAG, "LUX_IVS_PD_StartAlgo failed\n");
                return LUX_IVS_START_RECIVE_PIC_ERR;
            }
            printf("+++++++++++++++++ Line[%d] Function[%s] person_det on +++++++++++++++++\n", __LINE__, __FUNCTION__);
        }
        else
        {
            ret = LUX_IVS_PD_StopAlgo();
            if (LUX_OK != ret)
            {
                IMP_LOG_ERR(TAG, "LUX_IVS_PD_StopAlgo failed\n");
                return LUX_IVS_STOP_RECIVE_PIC_ERR;
            }
            printf("+++++++++++++++++ Line[%d] Function[%s] person_det off +++++++++++++++++\n", __LINE__, __FUNCTION__);
        }
    }

    alarmSen = g_configData->alg_config.human_detect_sensitivity;
    printf("person alarmSen=%d\n", alarmSen);
    ret = LUX_IVS_PD_SetSense((LUX_IVS_SENSITIVE_EN)alarmSen);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PD_SetSense failed\n");
        return LUX_IVS_SET_PARAM_ERR;
    }
    printf("+++++++++++++++++ Line[%d] Function[%s] person_det sensitive [%d] +++++++++++++++++\n", __LINE__, __FUNCTION__, alarmSen);

    /*设置区域报警*/
    ret = LUX_IVS_PD_AlarmZone_Open(g_configData->alg_config.alarm_zone_onoff);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PD_AlarmZone_Open failed\n");
        return LUX_GET_INI_VALUE_ERR;
    }
    printf("LUX_IVS_PD_AlarmZone_Open Success\n");

    strcpy(strAlarmZoneInfo, g_configData->alg_config.alarm_zone_size);
    printf("++++ persondet strAlarmZoneInfo=%s +++\n", strAlarmZoneInfo);

    memset(&alarmZone, 0, sizeof(LUX_IVS_M_ZONE_BASE_ST));
    ret = LUX_IVS_alarmZone_cJsonParse(strAlarmZoneInfo, &alarmZone);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, " LUX_Ivs_alarmZone_Parse failed, return error [0x%x]\n", ret);
        return LUX_IVS_SET_PARAM_ERR;
    }
    printf("persondet LUX_IVS_alarmZone_cJsonParse Success\n");


    ret = LUX_IVS_PD_SetRegion(&alarmZone);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PD_SetRegion failed\n");
        return LUX_IVS_SET_PARAM_ERR;
    }
    printf("LUX_IVS_PD_SetRegion Success\n");

    return LUX_OK;
}

/**
 * @description: 根据人形侦测报警时间判断是否上报
 * @param  [in] lastTime 人形侦测的报警时间
 * @return LUX_FALSE不报警，LUX_TRUE报警
 */
BOOL_X LUX_PersonEvent_IsAlarmByInterval(UINT_X lastTime)
{
    UINT_X timeInterval = 0;
    UINT_X curTime = 0;
    BOOL_X bAlarm = LUX_FALSE;
#if 0
    if (1 == recordReboot)  //重新上电读取配置文件值
    {
        CHAR_X *p_getalarmInterval = IPC_APP_get_alarm_interval_X();
        g_persondet_attr.timeIntervel = (LUX_IVS_TIMEINTERVAL_EN)(*p_getalarmInterval - '0');
        recordReboot = 0;
    }
#endif
    switch (g_persondet_attr.timeInterval)
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
        bAlarm = LUX_TRUE;
    }
    //printf("INFO: %s:%d timeInterval:%d curTime:%d lastTime:%d-%d bAlarm:%d\n",
    //       __func__, __LINE__, timeInterval, curTime, lastTime, *pLastTime, bAlarm);

    return bAlarm;
}

/**
 * @description: 人形侦测报警事件函数
 * @return 0 成功，非零失败
 */
INT_X LUX_IVS_PD_Alarm()
{
    Thread_Mutex_Lock(&g_persondet_attr.paramMux);
    /*报警时间间隔*/
    if (LUX_PersonEvent_IsAlarmByInterval(g_persondet_attr.alarmTime))
    {
        g_persondet_attr.alarmTime = getTime_getS();
        /*上报抓图 */   //TODO

        /* 云存储 */    //TODO

        /*事件录像*/    //TODO
        printf("*** LUX_IVS_PD_Alarm ***\n\n\n");
    }
    Thread_Mutex_UnLock(&g_persondet_attr.paramMux);
    return LUX_OK;
}

/**
 * @description: 人形侦测算法处理
 */
VOID_X LUX_IVS_PD_Process()
{
    INT_X ret = LUX_ERR;
    INT_X i = 0;
    persondet_param_output_t show_data;

    Thread_Mutex_Lock(&g_persondet_attr.mux);
    do
    {
        ret = IMP_IVS_PollingResult(g_persondet_attr.bindAttr.chnNum, IMP_IVS_DEFAULT_TIMEOUTMS);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_PollingResult(%d, %d) failed\n", g_persondet_attr.bindAttr.chnNum, IMP_IVS_DEFAULT_TIMEOUTMS);
            break;
        }

        ret = IMP_IVS_GetResult(g_persondet_attr.bindAttr.chnNum, (PVOID_X*)&g_persondet_attr.ivsPersonDetResult);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_GetResult(%d) failed\n", g_persondet_attr.bindAttr.chnNum);
            break;
        }

        memset(&show_data, 0, sizeof(persondet_param_output_t));
        if (g_persondet_attr.ivsPersonDetResult->count > 0)
        {
            for (i = 0; i < g_persondet_attr.ivsPersonDetResult->count; i++)
            {
                if (g_persondet_attr.ivsPersonDetResult->person[i].class_id == 0)
                {
                    show_data.person[show_data.count] = g_persondet_attr.ivsPersonDetResult->person[i];
                    show_data.count++;
                }
            }
#if 0
            if (person_cnt == PERSONDET_FILTER_NUM)
            {
                printf("++++++++++ Line[%d] Function[%s] person nums=%d ++++++++++\n", __LINE__, __FUNCTION__, show_data.count);
                person_cnt = 0;
            }
            person_cnt++;
#endif
            LUX_OSD_PersonRect_Miss();
            LUX_OSD_PersonRect_Show(&show_data);
            personMissCnt = 0;
        }
        else if (personMissCnt > PERSONDET_MISS_NUM)
        {
            LUX_OSD_PersonRect_Miss();
            personMissCnt = 0;
        }
        else
        {
            personMissCnt++;
        }

        ret = IMP_IVS_ReleaseResult(g_persondet_attr.bindAttr.chnNum, (PVOID_X)g_persondet_attr.ivsPersonDetResult);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_ReleaseResult(%d) failed\n", g_persondet_attr.bindAttr.chnNum);
            break;
        }

        if (show_data.count > 0)
        {
            LUX_IVS_PD_Alarm();
        }
    } while (0);
    Thread_Mutex_UnLock(&g_persondet_attr.mux);
    usleep(PERSONDET_DELAY_TIME);
    return;
}

/**
 * @description: 人形侦测算法线程函数
 */
PVOID_X person_det_process_thread()
{
    CHAR_X threadName[64] = {0};
    CHAR_X ivsChnName[32] = {0};

    strcpy(ivsChnName, "PERSON_DETECTION");
    g_persondet_attr.IsRunning = LUX_TRUE;

    sprintf(threadName, "IVS_%s", ivsChnName);
    prctl(PR_SET_NAME, threadName);
    while (g_persondet_attr.IsRunning)
    {
        if(LUX_TRUE != g_persondet_attr.bStart || g_ivsAttr.bInit != LUX_TRUE || g_framSrcAttr->bInit != LUX_TRUE)
        {  
            usleep(SLEEP_TIME);
            continue;
        }
        else
        {
            LUX_IVS_PD_Process();
        }
    }
    return NULL;
}

/**
 * @description: 人形侦测算法的初始化函数
 * @return 0成功,非0返回失败返回错误码
 */
INT_X LUX_IVS_PD_AlgoInit()
{
    printf("+++++++++++++================ enter LUX_IVS_PD_AlgoInit\n");

    INT_X ret = LUX_ERR;

    /*创建通道*/
    ret = IMP_IVS_CreateChn(g_persondet_attr.bindAttr.chnNum, g_persondet_attr.interface);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "IMP_IVS_CreateChn(%d) failed\n", g_persondet_attr.bindAttr.chnNum);
        return LUX_IVS_CHAN_CREATE_ERR;
    }
    printf("IMP_IVS_CreateChn[%d] Success\n", g_persondet_attr.bindAttr.chnNum);


    /*通道和ivs组绑定*/
    ret = IMP_IVS_RegisterChn(g_persondet_attr.bindAttr.grpId, g_persondet_attr.bindAttr.chnNum);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "IMP_IVS_RegisterChn(%d, %d) failed\n", g_persondet_attr.bindAttr.grpId, g_persondet_attr.bindAttr.chnNum);
        return LUX_IVS_CHAN_RIGISTER_ERR;
    }
    printf("IMP_IVS_RegisterChn[%d] Success\n", g_persondet_attr.bindAttr.chnNum);

    /*根据配置文件是否使能通道*/
    ret = LUX_IVS_PD_SetConf();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PD_SetConf failed, return error [0x%x]\n", ret);
        return LUX_IVS_CHAN_RIGISTER_ERR;
    }
    printf("LUX_IVS_PD_SetConf Success\n");

    return LUX_OK;
}

/**
 * @description: 算法的去初始化函数
 * @return 0成功,非0返回失败返回错误码
 */
INT_X LUX_IVS_PD_AlgoExit()
{
    INT_X ret = LUX_ERR;

    g_persondet_attr.IsRunning = LUX_FALSE;

    /* 关闭通道 */
    ret = LUX_IVS_PD_StopChn();
    if (LUX_OK !=  ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PD_StopChn failed\n");
        return LUX_IVS_STOP_RECIVE_PIC_ERR;
    }

    /* 从ivs通道组注销通道 */
    ret = IMP_IVS_UnRegisterChn(g_persondet_attr.bindAttr.chnNum);
    if (LUX_OK !=  ret)
    {
        IMP_LOG_ERR(TAG, "IMP_IVS_UnRegisterChn(%d) failed\n", g_persondet_attr.bindAttr.chnNum);
        return LUX_IVS_CHAN_UNRIGISTER_ERR;
    }

    /* 销毁通道 */
    ret = IMP_IVS_DestroyChn(g_persondet_attr.bindAttr.chnNum);
    if (LUX_OK !=  ret)
    {
        IMP_LOG_ERR(TAG, "IMP_IVS_DestroyChn(%d) failed\n", g_persondet_attr.bindAttr.chnNum);
        return LUX_IVS_CHAN_DESTROY_ERR;
    }

    return LUX_OK;
}

/**
 * @description: 人形侦测算法初始化
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PD_Init()
{
    INT_X ret = LUX_ERR;
    /*检查算法版本*/
    UINT_X persondet_ver = persondet_get_version_info();
    if (persondet_ver != PERSONDET_VERSION_NUM)
    {
        printf("persondet_ver: The version numbers of head file and lib do not match, head file version: %08x, lib version: %08x\n", PERSONDET_VERSION_NUM, persondet_ver);
        return LUX_ERR;
    }

    Thread_Mutex_Create(&g_persondet_attr.paramMux);
    Thread_Mutex_Create(&g_persondet_attr.mux);

    persondet_param_input_t pdParam;
    /*设置人形侦测的属性*/
    memset(&pdParam, 0, sizeof(persondet_param_input_t));
    LUX_IVS_PD_SetAttr(&pdParam);

    /*获取人形侦测算法句柄*/
    g_persondet_attr.interface = PersonDetInterfaceInit(&pdParam);
    if (NULL == g_persondet_attr.interface)
    {
        IMP_LOG_ERR(TAG, "PersonDetInterfaceInit failed\n");
        return LUX_IVS_SET_PARAM_ERR;
    }

    /*创建人形侦测算法通道、绑定算法组、使能通道*/
    ret = LUX_IVS_PD_AlgoInit();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PD_AlgoInit failed, return error [0x%x]\n", ret);
        return LUX_ERR;
    }

    /*创建线程处理ivs人形侦测结果*/
    ret = Task_CreateThread(person_det_process_thread, NULL);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, " pthread_create failed\n");
        return LUX_IVS_CREAT_THREAD_ERR;
    }
    return LUX_OK;
}

/**
 * @description: 人形侦测算法去初始化
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PD_Exit()
{
    INT_X ret = LUX_ERR;
    ret = LUX_IVS_PD_AlgoExit();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PD_AlgoExit failed, return error [0x%x]\n", ret);
        return LUX_ERR;
    }

    PersonDetInterfaceExit(g_persondet_attr.interface);
    Thread_Mutex_Destroy(&g_persondet_attr.mux);
    Thread_Mutex_Destroy(&g_persondet_attr.paramMux);
    return LUX_OK;
}
