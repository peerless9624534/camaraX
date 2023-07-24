#include "lux_perm_det.h"

#define TAG "lux_perm_det"

/*打印结果过滤次数*/
static UCHAR_X cross_cnt = 0;
/* 未侦测到周界移动次数 */
static UCHAR_X permMissCnt = 0;
/* 周界侦测区域 */
static IVSPoint g_permdet_perm[MAX_PERM_NUM][MAX_PERM_POINTS_NUM];

LUX_IVS_PERMDET_ATTR_ST g_permdet_attr =
{
    .bInit = LUX_FALSE,
    .alarmTime = 0,
    .timeInterval = LUX_IVS_TIMEINTERVAL_1MIN,
    .bindAttr =
    {
        .grpId = 0,
        .chnNum = LUX_IVS_PERM_DET_EN,
        .bindSrcChan = 1,
        .bindSrcModule = LUX_MODULS_FSRC,
        .bindDstChan = 0,                   /*绑定的channal相当于君正的group*/
        .bindDstModule = LUX_MODULS_IVS,
    },
    .in_param =
    {
        /* T40 7.2.0-1.2.0 */
        .sense = 2,
        .frameInfo.width = SENSOR_WIDTH_SECOND,
        .frameInfo.height = SENSOR_HEIGHT_SECOND,
        .isSkipFrame = LUX_FALSE,
        .permcnt = PERMDET_RECT_NUM,
        .perms[0].fun = LUX_PERM_FUN_MOV_DIRE_EN,
        .perms[0].pcnt = 4,
        .light = LUX_TRUE,
        .level = 0,
        .timeon = 110,
        .timeoff = 310,
    },
};

extern LUX_FSRC_ATTR_ST* g_framSrcAttr;
extern LUX_CONFIG_DATA_ST* g_configData;

extern VOID_X LUX_OSD_PermOBJ_Rect_Show(perm_param_output_t* ivsPermDetRet);

/**
 * @description: 设置周界侦测算法灵敏度
 * @param [in] sns   灵敏度
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PermDet_SetSense(LUX_IVS_SENSITIVE_EN sns)
{
    INT_X ret = LUX_ERR;
    INT_X PermDetSense = 0;
    perm_param_input_t PermDetParam;

    memset(&PermDetParam, 0, sizeof(perm_param_input_t));

    Thread_Mutex_Lock(&g_permdet_attr.paramMux);
    do {
        switch (sns)
        {
        case LUX_IVS_SENSITIVE_LOW_EN:
            PermDetSense = 1;
            break;
        case LUX_IVS_SENSITIVE_MID_EN:
            PermDetSense = 2;
            break;
        case LUX_IVS_SENSITIVE_HIG_EN:
            PermDetSense = 4;
            break;
        default:
            IMP_LOG_ERR(TAG, "LUX_IVS_PermDet_SetSense failed, chn(%d), not suppport param,\
                            it will be set default param [%d]  \n", g_permdet_attr.bindAttr.chnNum, LUX_IVS_SENSITIVE_LOW_EN);
            PermDetSense = 2;
            break;
        }

        ret = IMP_IVS_GetParam(g_permdet_attr.bindAttr.chnNum, (PVOID_X)&PermDetParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_GetParam failed, chn(%d) . \n", g_permdet_attr.bindAttr.chnNum);
            ret = LUX_IVS_GET_PARAM_ERR;
            break;
        }

        g_permdet_attr.in_param.sense = PermDetSense;
        PermDetParam.sense = g_permdet_attr.in_param.sense;

        ret = IMP_IVS_SetParam(g_permdet_attr.bindAttr.chnNum, (PVOID_X)&PermDetParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_SetParam chn(%d) failed. \n", g_permdet_attr.bindAttr.chnNum);
            ret = LUX_IVS_SET_PARAM_ERR;
            break;
        }
    } while (0);
    Thread_Mutex_UnLock(&g_permdet_attr.paramMux);
    return ret;
}

/**
 * @description: 设置周界侦测算法功能
 * @param [in] perm_no  第几个周界框
 * @param [in] fun      功能
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PermDet_SetFun(INT_X perm_no, LUX_PERM_FUN_EN fun)
{
    if (perm_no >= MAX_PERM_NUM)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PermDet_SetFun failed, chn(%d), perm_no more than maximum perimeter number\n", g_permdet_attr.bindAttr.chnNum);
        return LUX_IVS_SET_PARAM_ERR;
    }

    INT_X ret = LUX_ERR;
    INT_X PermDetFun = 0;
    perm_param_input_t PermDetParam;

    memset(&PermDetParam, 0, sizeof(perm_param_input_t));

    Thread_Mutex_Lock(&g_permdet_attr.paramMux);
    do {
        if (perm_no >= g_permdet_attr.in_param.permcnt)
        {
            IMP_LOG_ERR(TAG, "LUX_IVS_PermDet_SetFun failed, chn(%d), perm_no exceeded the range of settable perimeter numbers\n", g_permdet_attr.bindAttr.chnNum);
            return LUX_IVS_SET_PARAM_ERR;
        }

        switch (fun)
        {
        case LUX_PERM_FUN_PERM_NEAR_EN ... LUX_PERM_FUN_MOV_DIRE_EN:
            PermDetFun = fun;
            break;
        default:
            IMP_LOG_ERR(TAG, "LUX_IVS_PermDet_SetFun failed, chn(%d), not suppport param,\
                            it will be set default param [%d]  \n", g_permdet_attr.bindAttr.chnNum, LUX_PERM_FUN_MOV_DIRE_EN);
            PermDetFun = LUX_PERM_FUN_MOV_DIRE_EN;
            break;
        }

        ret = IMP_IVS_GetParam(g_permdet_attr.bindAttr.chnNum, (PVOID_X)&PermDetParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_GetParam failed, chn(%d) . \n", g_permdet_attr.bindAttr.chnNum);
            ret = LUX_IVS_GET_PARAM_ERR;
            break;
        }

        g_permdet_attr.in_param.perms[perm_no].fun = PermDetFun;
        PermDetParam.perms[perm_no].fun = g_permdet_attr.in_param.perms[perm_no].fun;

        ret = IMP_IVS_SetParam(g_permdet_attr.bindAttr.chnNum, (PVOID_X)&PermDetParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_SetParam chn(%d) failed. \n", g_permdet_attr.bindAttr.chnNum);
            ret = LUX_IVS_SET_PARAM_ERR;
            break;
        }
    } while (0);
    Thread_Mutex_UnLock(&g_permdet_attr.paramMux);
    return ret;
}

/**
 * @description: 开启周界侦测算法通道
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PermDet_StartChn()
{
    INT_X ret = LUX_OK;

    if (LUX_TRUE == g_permdet_attr.bInit)
    {
        IMP_LOG_ERR(TAG, "Warnning, LUX_IVS_PermDet_StartChn already Started\n");
        return LUX_OK;
    }

    Thread_Mutex_Lock(&g_permdet_attr.mux);
    do {
        ret = IMP_IVS_StartRecvPic(g_permdet_attr.bindAttr.chnNum);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_StartRecvPic failed\n");
            break;
        }
        g_permdet_attr.bInit = LUX_TRUE;
    } while (0);
    Thread_Mutex_UnLock(&g_permdet_attr.mux);

    return ret;
}

/**
 * @description: 关闭周界侦测算法通道
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PermDet_StopChn()
{
    INT_X ret = LUX_OK;

    if (LUX_FALSE == g_permdet_attr.bInit)
    {
        IMP_LOG_ERR(TAG, "Warnning, LUX_IVS_PermDet_StopChn already Stopped\n");
        return LUX_OK;
    }

    Thread_Mutex_Lock(&g_permdet_attr.mux);
    do {
        ret = IMP_IVS_StopRecvPic(g_permdet_attr.bindAttr.chnNum);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_StopRecvPic failed\n");
            break;
        }

        g_permdet_attr.bInit = LUX_FALSE;
    } while (0);
    Thread_Mutex_UnLock(&g_permdet_attr.mux);

    return ret;
}

/**
 * @description: 开启IVS周界侦测
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PermDet_StartAlgo()
{
    INT_X ret = LUX_ERR;
    IMP_LOG_INFO(TAG, "LUX_IVS_PermDet_StartAlgo\n");
    ret = LUX_IVS_PermDet_StartChn();
    if (ret != LUX_OK)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PermDet_StartChn failed\n");
        return LUX_IVS_START_RECIVE_PIC_ERR;
    }
    g_permdet_attr.bStart = LUX_TRUE;
    return LUX_OK;
}

/**
 * @description: 关闭IVS周界侦测
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PermDet_StopAlgo()
{
    INT_X ret = LUX_ERR;
    IMP_LOG_INFO(TAG, "LUX_IVS_PermDet_StopAlgo\n");
    ret = LUX_IVS_PermDet_StopChn();
    if (ret != LUX_OK)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PermDet_StopChn failed\n");
        return LUX_IVS_STOP_RECIVE_PIC_ERR;
    }
    g_permdet_attr.bStart = LUX_FALSE;
    return LUX_OK;
}

/**
 * @description: 周界侦测事件报警间隔
 * @param  [in] ts 时间间隔,单位s
 * @return 0 成功，非零失败，返回错误码
 */
VOID_X LUX_IVS_PermDet_SetTimeInterval(LUX_IVS_TIMEINTERVAL_EN ts)
{
    Thread_Mutex_Lock(&g_permdet_attr.paramMux);
    if (ts < LUX_IVS_TIMEINTERVAL_1MIN || ts >= LUX_IVS_TIME_INTERVAL_BUTTON)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PermDet_SetTimeInterval failed, not support param [%d]\n", ts);
        g_permdet_attr.timeInterval = LUX_IVS_TIMEINTERVAL_1MIN;
    }

    switch (ts)
    {
    case LUX_IVS_TIMEINTERVAL_1MIN ... LUX_IVS_TIMEINTERVAL_5MIN:
        g_permdet_attr.timeInterval = ts;
        break;
    case LUX_IVS_TIME_INTERVAL_BUTTON:
    default:
        IMP_LOG_ERR(TAG, "LUX_IVS_PermDet_SetTimeInterval failed, not support param [%d]\n", ts);
        g_permdet_attr.timeInterval = LUX_IVS_TIMEINTERVAL_1MIN;
        break;
    }
    Thread_Mutex_UnLock(&g_permdet_attr.paramMux);

    return;
}

/**
 * @description: 设置周界侦测报警区域
 * @param  [in] zoneInfo  坐标信息
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PermDet_SetRegion(LUX_IVS_M_ZONE_BASE_ST* zoneInfo)
{
    if (NULL == zoneInfo)
    {
        IMP_LOG_ERR(TAG, "LUX_Ivs_SetPermDet_Region failed, empty point.\n");
        return LUX_PARM_NULL_PTR;
    }

    if (zoneInfo->regionCnt > MAX_PERM_NUM)
    {
        zoneInfo->regionCnt = MAX_PERM_NUM;
        IMP_LOG_ERR(TAG, "[ERROR] alarmArea Number more than %d !!!\n", MAX_PERM_NUM);
    }

    INT_X i, j, ret = LUX_OK;
    perm_param_input_t PermDetParam;
    DOUBLE_X widthRate = g_permdet_attr.in_param.frameInfo.width / 100.0;
    DOUBLE_X heightRate = g_permdet_attr.in_param.frameInfo.height / 100.0;

    Thread_Mutex_Lock(&g_permdet_attr.paramMux);
    do {
        memset(&PermDetParam, 0, sizeof(perm_param_input_t));
        ret = IMP_IVS_GetParam(g_permdet_attr.bindAttr.chnNum, (PVOID_X)&PermDetParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_GetParam failed, chn(%d) . \n", g_permdet_attr.bindAttr.chnNum);
            ret = LUX_IVS_GET_PARAM_ERR;
            break;
        }
        printf("IMP_IVS_GetParam PermDet Success\n");

        g_permdet_attr.in_param.permcnt = zoneInfo->regionCnt;
        PermDetParam.permcnt = g_permdet_attr.in_param.permcnt;
        for (i = 0; i < PermDetParam.permcnt; i++)
        {
            /*左上角为(0,0)p0点, 顺时针排列顺序, 依次为p0,p1,p2,p3*/
            g_permdet_attr.in_param.perms[i].p[0].x = alignment_down(((INT_X)(zoneInfo->rect[i].pointX * widthRate)), 1);
            g_permdet_attr.in_param.perms[i].p[0].y = alignment_down(((INT_X)(zoneInfo->rect[i].pointY * heightRate)), 1);
            g_permdet_attr.in_param.perms[i].p[1].x = alignment_down(((INT_X)((zoneInfo->rect[i].pointX + zoneInfo->rect[i].width) * widthRate) - 1), 1);
            g_permdet_attr.in_param.perms[i].p[1].y = alignment_down(((INT_X)(zoneInfo->rect[i].pointY * heightRate)), 1);
            g_permdet_attr.in_param.perms[i].p[2].x = alignment_down(((INT_X)((zoneInfo->rect[i].pointX + zoneInfo->rect[i].width) * widthRate) - 1), 1);
            g_permdet_attr.in_param.perms[i].p[2].y = alignment_down(((INT_X)((zoneInfo->rect[i].pointY + zoneInfo->rect[i].height) * heightRate) - 1), 1);
            g_permdet_attr.in_param.perms[i].p[3].x = alignment_down(((INT_X)(zoneInfo->rect[i].pointX * widthRate)), 1);
            g_permdet_attr.in_param.perms[i].p[3].y = alignment_down(((INT_X)((zoneInfo->rect[i].pointY + zoneInfo->rect[i].height) * heightRate) - 1), 1);
            for (j = 0; j < 4; j++)
            {
                PermDetParam.perms[i].p[j].x = g_permdet_attr.in_param.perms[i].p[j].x;
                PermDetParam.perms[i].p[j].y = g_permdet_attr.in_param.perms[i].p[j].y;
            }
            IMP_LOG_INFO(TAG, "LUX_Ivs_SetPermDet_Region region[%d]: { P0(%d,%d),P1(%d,%d),P2(%d,%d),P3(%d,%d) }. \n", i,
                    PermDetParam.perms[i].p[0].x, PermDetParam.perms[i].p[0].y, PermDetParam.perms[i].p[1].x, PermDetParam.perms[i].p[1].y,
                    PermDetParam.perms[i].p[2].x, PermDetParam.perms[i].p[2].y, PermDetParam.perms[i].p[3].x, PermDetParam.perms[i].p[3].y);
        }

        ret = IMP_IVS_SetParam(g_permdet_attr.bindAttr.chnNum, (PVOID_X)&PermDetParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_SetParam failed, chn(%d) . \n", g_permdet_attr.bindAttr.chnNum);
            ret = LUX_IVS_SET_PARAM_ERR;
            break;
        }
        printf("IMP_IVS_SetParam PermDet Success\n");
    } while (0);
    Thread_Mutex_UnLock(&g_permdet_attr.paramMux);

    return ret;
}

/**
 * @description: 设置多边形周界侦测报警区域
 * @param  [in] zoneInfo  坐标信息
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PermDet_SetPolygonRegion(LUX_IVS_M_POLYGON_ST* zoneInfo)
{
    if (NULL == zoneInfo)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_SetPermDet_PolygonRegion failed, empty point.\n");
        return LUX_PARM_NULL_PTR;
    }

    INT_X ret = LUX_ERR;
    IVSPoint convPoints[MAX_PERM_POINTS_NUM];
    if (zoneInfo->regionCnt > MAX_PERM_NUM)
    {
        zoneInfo->regionCnt = MAX_PERM_NUM;
        IMP_LOG_ERR(TAG, "[ERROR] alarmArea Number more than %d !!!\n", MAX_PERM_NUM);
    }
    
    int i, j;
    for (i = 0; i < zoneInfo->regionCnt; i++)
    {
        if (3 > zoneInfo->polygon[i].pointCnt)
        {
            IMP_LOG_ERR(TAG, "[ERROR] alarmArea[%d] Point Number atleast 3 !!!\n", i);
            return LUX_ERR;
        }
        if (MAX_PERM_POINTS_NUM < zoneInfo->polygon[i].pointCnt)//多边形点数等于边数
        {
            zoneInfo->polygon[i].pointCnt = MAX_PERM_POINTS_NUM;
            IMP_LOG_ERR(TAG, "[ERROR] alarmArea[%d] Point Number more than %d !!!\n", i, MAX_PERM_POINTS_NUM);
        }
        
    }

    perm_param_input_t PermParam;
    DOUBLE_X widthRate = g_permdet_attr.in_param.frameInfo.width / 100.0;
    DOUBLE_X heightRate = g_permdet_attr.in_param.frameInfo.height / 100.0;

    Thread_Mutex_Lock(&g_permdet_attr.paramMux);
    do
    {
        memset(&PermParam, 0, sizeof(perm_param_input_t));
        ret = IMP_IVS_GetParam(g_permdet_attr.bindAttr.chnNum, (void*)&PermParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_GetParam failed, chn(%d) . \n", g_permdet_attr.bindAttr.chnNum);
            ret = LUX_IVS_GET_PARAM_ERR;
            break;
        }
        printf("IMP_IVS_GetParam PermDet Success\n");

        g_permdet_attr.in_param.permcnt = zoneInfo->regionCnt;
        PermParam.permcnt = g_permdet_attr.in_param.permcnt;
        for (i = 0; i < PermParam.permcnt; i++)
        {
            g_permdet_attr.in_param.perms[i].pcnt = zoneInfo->polygon[i].pointCnt;
            PermParam.perms[i].pcnt = g_permdet_attr.in_param.perms[i].pcnt;
            for (j = 0; j < PermParam.perms[i].pcnt; j++)
            {
                convPoints[j].x = zoneInfo->polygon[i].zonePoints[j].x * widthRate;
                convPoints[j].y = zoneInfo->polygon[i].zonePoints[j].y * heightRate;
                g_permdet_attr.in_param.perms[i].p[j].x = alignment_down(convPoints[j].x, 1);
                g_permdet_attr.in_param.perms[i].p[j].y = alignment_down(convPoints[j].y, 1);

                PermParam.perms[i].p[j].x = g_permdet_attr.in_param.perms[i].p[j].x;
                PermParam.perms[i].p[j].y = g_permdet_attr.in_param.perms[i].p[j].y;
                IMP_LOG_INFO(TAG, "perm_area[%d](%d point): POINT[%d](%d,%d)", i, PermParam.perms[i].pcnt, j,
                    PermParam.perms[i].p[j].x, PermParam.perms[i].p[j].y);
            }
        }

        ret = IMP_IVS_SetParam(g_permdet_attr.bindAttr.chnNum, (void*)&PermParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_SetParam failed, chn(%d) . \n", g_permdet_attr.bindAttr.chnNum);
            ret = LUX_IVS_SET_PARAM_ERR;
            break;
        }
        printf("IMP_IVS_SetParam PermDet Success\n");
    } while (0);
    Thread_Mutex_UnLock(&g_permdet_attr.paramMux);

    return ret;
}

/**
 * @description: 设置周界侦测的属性
 * @param  [in] param 属性
 */
VOID_X LUX_IVS_PermDet_SetAttr(perm_param_input_t* pParam)
{
    INT_X i;
    Thread_Mutex_Lock(&g_permdet_attr.paramMux);
    for (i = 0; i < MAX_PERM_NUM; i++)
    {
        g_permdet_attr.in_param.perms[i].p = &g_permdet_perm[i][0];
    }
    memcpy(pParam, &g_permdet_attr.in_param, sizeof(perm_param_input_t));
    Thread_Mutex_UnLock(&g_permdet_attr.paramMux);

    return;
}

/**
 * @description: 周界侦测启动设置配置
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PermDet_SetConf()
{
    INT_X i;
    INT_X ret = LUX_ERR;
    INT_X alarmSen = LUX_IVS_SENSITIVE_LOW_EN;
    BOOL_X alarmZoneOnOff = LUX_FALSE;
    CHAR_X strAlarmZoneInfo[STR_LEN_MAX];
    LUX_IVS_M_ZONE_BASE_ST alarmZone;
    LUX_IVS_M_POLYGON_ST alarmPolygonZone;
    BOOL_X sleepMode = LUX_FALSE;

    ret = LUX_IVS_PermDet_StartChn();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PermDet_StartChn(%d) failed\n", g_permdet_attr.bindAttr.chnNum);
        return LUX_IVS_START_RECIVE_PIC_ERR;
    }

    /*隐私模式下不开启周界侦测*/
    if (LUX_TRUE == g_configData->alg_config.sleepMode)
    {
        ret = LUX_IVS_PermDet_StopAlgo();
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_IVS_PermDet_StopAlgo failed\n");
            return LUX_IVS_STOP_RECIVE_PIC_ERR;
        }
        printf("+++++++++++++++++ Line[%d] Function[%s] perm_det off +++++++++++++++++\n", __LINE__, __FUNCTION__);
    }
    else
    {
        printf("+++++++++++============= g_configData->alg_config.pir_detect_enabled:%d\n", g_configData->alg_config.pir_detect_enabled);
        printf("+++++++++++============= g_configData->alg_config.pir_detect_m_enabled:%d\n", g_configData->alg_config.pir_detect_m_enabled);
        if (LUX_TRUE == g_configData->alg_config.pir_detect_enabled || LUX_TRUE == g_configData->alg_config.pir_detect_m_enabled)
        {
            ret = LUX_IVS_PermDet_StartAlgo();
            if (LUX_OK != ret)
            {
                IMP_LOG_ERR(TAG, "LUX_IVS_PermDet_StartAlgo failed\n");
                return LUX_IVS_START_RECIVE_PIC_ERR;
            }
            printf("+++++++++++++++++ Line[%d] Function[%s] perm_det on +++++++++++++++++\n", __LINE__, __FUNCTION__);
        }
        else
        {
            ret = LUX_IVS_PermDet_StopAlgo();
            if (LUX_OK != ret)
            {
                IMP_LOG_ERR(TAG, "LUX_IVS_PermDet_StopAlgo failed\n");
                return LUX_IVS_STOP_RECIVE_PIC_ERR;
            }
            printf("+++++++++++++++++ Line[%d] Function[%s] perm_det off +++++++++++++++++\n", __LINE__, __FUNCTION__);
        }
    }

    alarmSen = g_configData->alg_config.pir_detect_sensitivity;
    printf("perm alarmSen=%d\n", alarmSen);
    ret = LUX_IVS_PermDet_SetSense((LUX_IVS_SENSITIVE_EN)alarmSen);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PermDet_SetSense failed\n");
        return LUX_IVS_SET_PARAM_ERR;
    }
    printf("+++++++++++++++++ Line[%d] Function[%s] perm_det sensitive [%d] +++++++++++++++++\n", __LINE__, __FUNCTION__, alarmSen);

    /*多边形周界*/
    if (g_configData->alg_config.pir_detect_m_enabled)//多边形周界开关
    {
        memset(strAlarmZoneInfo, 0, STR_LEN_MAX);
        strcpy(strAlarmZoneInfo, g_configData->alg_config.perm_area2);
        printf("++++ permdet polygon strAlarmZoneInfo=%s +++\n", strAlarmZoneInfo);

        memset(&alarmPolygonZone, 0, sizeof(LUX_IVS_M_POLYGON_ST));
        ret= LUX_IVS_alarmPolygonZone_cJsonParse(strAlarmZoneInfo, &alarmPolygonZone);
        if (LUX_OK !=  ret)
        {
            IMP_LOG_ERR(TAG, " LUX_IVS_alarmZone_Parse failed, return error [0x%x]\n", ret);
            return LUX_IVS_SET_PARAM_ERR;
        }
        printf("LUX_IVS_alarmPolygonZone_cJsonParse Success\n");

        ret = LUX_IVS_PermDet_SetPolygonRegion(&alarmPolygonZone);
        if (LUX_OK !=  ret)
        {
            IMP_LOG_ERR(TAG, " LUX_IVS_PermDet_SetPolygonRegion failed\n");
            return LUX_IVS_SET_PARAM_ERR;
        }
        printf("LUX_IVS_PermDet_SetPolygonRegion Success\n");

        for (i = 0; i < g_permdet_attr.in_param.permcnt; i++)
        {
            LUX_IVS_PermDet_SetFun(i, LUX_PERM_FUN_MOV_DIRE_EN);
        }

        LUX_OSD_PermPolygon_Show(g_configData->alg_config.pir_detect_m_enabled);
        return LUX_OK;
    }

    /*矩形周界*/
    if (g_configData->alg_config.pir_detect_enabled)//矩形形周界开关
    {
        memset(strAlarmZoneInfo, 0, STR_LEN_MAX);
        strcpy(strAlarmZoneInfo, g_configData->alg_config.perm_area);
        printf("++++ permdet rect strAlarmZoneInfo=%s +++\n", strAlarmZoneInfo);

        memset(&alarmZone, 0, sizeof(LUX_IVS_M_ZONE_BASE_ST));
        ret= LUX_IVS_alarmZone_cJsonParse(strAlarmZoneInfo, &alarmZone);
        if (LUX_OK !=  ret)
        {
            IMP_LOG_ERR(TAG, " LUX_IVS_alarmZone_cJsonParse failed, return error [0x%x]\n", ret);
            return LUX_IVS_SET_PARAM_ERR;
        }
        printf("permdet LUX_IVS_alarmZone_cJsonParse Success\n");

        ret = LUX_IVS_PermDet_SetRegion(&alarmZone);
        if (LUX_OK !=  ret)
        {
            IMP_LOG_ERR(TAG, " LUX_IVS_PermDet_SetRegion failed\n");
            return LUX_IVS_SET_PARAM_ERR;
        }
        printf("permdet LUX_IVS_PermDet_SetRegion Success\n");

        for (i = 0; i < g_permdet_attr.in_param.permcnt; i++)
        {
            LUX_IVS_PermDet_SetFun(i, LUX_PERM_FUN_MOV_DIRE_EN);
        }

        LUX_ISPOSD_PermRect_Draw(g_configData->alg_config.pir_detect_enabled);
    }

    return LUX_OK;
}

/**
 * @description: 根据周界侦测报警时间判断是否上报
 * @param  [in] lastTime 周界侦测的报警时间
 * @return LUX_FALSE不报警，LUX_TRUE报警
 */
BOOL_X LUX_PermEvent_IsAlarmByInterval(UINT_X lastTime)
{
    UINT_X timeInterval = 0;
    UINT_X curTime = 0;
    BOOL_X bAlarm = LUX_FALSE;
#if 0
    if (1 == recordReboot)  //重新上电读取配置文件值
    {
        CHAR_X *p_getalarmInterval = IPC_APP_get_alarm_interval_X();
        g_permdet_attr.timeIntervel = (LUX_IVS_TIMEINTERVAL_EN)(*p_getalarmInterval - '0');
        recordReboot = 0;
    }
#endif
    switch (g_permdet_attr.timeInterval)
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
 * @description: 周界侦测报警事件函数
 * @return 0 成功，非零失败
 */
INT_X LUX_IVS_PermDet_Alarm()
{
    Thread_Mutex_Lock(&g_permdet_attr.paramMux);
    /*报警时间间隔*/
    if (LUX_PermEvent_IsAlarmByInterval(g_permdet_attr.alarmTime))
    {
        g_permdet_attr.alarmTime = getTime_getS();
        /*上报抓图 */   //TODO

        /* 云存储 */    //TODO

        /*事件录像*/    //TODO
        printf("*** LUX_IVS_PermDet_Alarm ***\n\n\n");
    }
    Thread_Mutex_UnLock(&g_permdet_attr.paramMux);
    return LUX_OK;
}

/**
 * @description: 周界侦测算法处理
 */
VOID_X LUX_IVS_PermDet_Process()
{
    INT_X ret = LUX_ERR;
    INT_X i = 0;
    perm_param_output_t show_data;

    Thread_Mutex_Lock(&g_permdet_attr.mux);
    do
    {
        /*阻塞判断是否获得结果*/
        ret = IMP_IVS_PollingResult(g_permdet_attr.bindAttr.chnNum, IMP_IVS_DEFAULT_TIMEOUTMS);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_PollingResult(%d, %d) failed\n", g_permdet_attr.bindAttr.chnNum, IMP_IVS_DEFAULT_TIMEOUTMS);
            break;
        }

        /*获取算法结果*/
        ret = IMP_IVS_GetResult(g_permdet_attr.bindAttr.chnNum, (PVOID_X*)&g_permdet_attr.ivsPermDetResult);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_GetResult(%d) failed\n", g_permdet_attr.bindAttr.chnNum);
            break;
        }

        memset(&show_data, 0, sizeof(perm_param_output_t));
        if (g_permdet_attr.ivsPermDetResult->ret > 0)
        {
            for (i = 0; i < g_permdet_attr.ivsPermDetResult->rectcnt; i++)
            {
                if (g_permdet_attr.ivsPermDetResult->rect_status[i].status == 1)
                {
                    show_data.rects[show_data.rectcnt] = g_permdet_attr.ivsPermDetResult->rects[i];
                    show_data.rectcnt++;
                }
            }
#if 0
            if (cross_cnt == PERMDET_FILTER_NUM)
            {
                printf("++++++++++ Line[%d] Function[%s] cross perm obj nums=%d ++++++++++\n", __LINE__, __FUNCTION__, show_data.rectcnt);
                cross_cnt = 0;
            }
            cross_cnt++;
#endif
            if (show_data.rectcnt > 0)
            {
                LUX_OSD_PermOBJ_Rect_Miss();
                LUX_OSD_PermOBJ_Rect_Show(&show_data);
                permMissCnt = 0;
            }
            else if (permMissCnt > PERMDET_MISS_NUM)
            {
                LUX_OSD_PermOBJ_Rect_Miss();
            }
            else if (show_data.rectcnt == 0)
            {
                LUX_OSD_PermOBJ_Rect_Miss();
            }
            else
            {
                permMissCnt++;
            }
        }

        /*释放算法结果*/
        ret = IMP_IVS_ReleaseResult(g_permdet_attr.bindAttr.chnNum, (PVOID_X)g_permdet_attr.ivsPermDetResult);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_ReleaseResult(%d) failed\n", g_permdet_attr.bindAttr.chnNum);
            break;
        }

        if (show_data.rectcnt > 0)
        {
            LUX_IVS_PermDet_Alarm();
        }
    } while (0);
    Thread_Mutex_UnLock(&g_permdet_attr.mux);
    usleep(PERMDET_DELAY_TIME);
    return;
}

/**
 * @description: 周界侦测算法线程函数
 */
PVOID_X perm_det_process_thread()
{
    CHAR_X threadName[64] = {0};
    CHAR_X ivsChnName[32] = {0};

    strcpy(ivsChnName, "PERM_DETECTION");
    g_permdet_attr.IsRunning = LUX_TRUE;

    sprintf(threadName, "IVS_%s", ivsChnName);
    prctl(PR_SET_NAME, threadName);
    while (g_permdet_attr.IsRunning)
    {
        if(LUX_TRUE != g_permdet_attr.bStart || g_ivsAttr.bInit != LUX_TRUE || g_framSrcAttr->bInit != LUX_TRUE)
        {  
            usleep(SLEEP_TIME);
            continue;
        }
        else
        {
            LUX_IVS_PermDet_Process();
        }
    }
    return NULL;
}

/**
 * @description: 周界侦测算法的初始化函数
 * @return 0成功,非0返回失败返回错误码
 */
INT_X LUX_IVS_PermDet_AlgoInit()
{
    printf("+++++++++++++================ enter LUX_IVS_PermDet_AlgoInit\n");

    INT_X ret = LUX_ERR;

    /*创建通道*/
    ret = IMP_IVS_CreateChn(g_permdet_attr.bindAttr.chnNum, g_permdet_attr.interface);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "IMP_IVS_CreateChn(%d) failed\n", g_permdet_attr.bindAttr.chnNum);
        return LUX_IVS_CHAN_CREATE_ERR;
    }
    printf("IMP_IVS_CreateChn[%d] Success\n", g_permdet_attr.bindAttr.chnNum);


    /*通道和ivs组绑定*/
    ret = IMP_IVS_RegisterChn(g_permdet_attr.bindAttr.grpId, g_permdet_attr.bindAttr.chnNum);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "IMP_IVS_RegisterChn(%d, %d) failed\n", g_permdet_attr.bindAttr.grpId, g_permdet_attr.bindAttr.chnNum);
        return LUX_IVS_CHAN_RIGISTER_ERR;
    }
    printf("IMP_IVS_RegisterChn[%d] Success\n", g_permdet_attr.bindAttr.chnNum);

    /*根据配置文件是否使能通道*/
    ret = LUX_IVS_PermDet_SetConf();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PermDet_SetConf failed, return error [0x%x]\n", ret);
        return LUX_IVS_CHAN_RIGISTER_ERR;
    }
    printf("LUX_IVS_PermDet_SetConf Success\n");

    return LUX_OK;
}

/**
 * @description: 算法的去初始化函数
 * @return 0成功,非0返回失败返回错误码
 */
INT_X LUX_IVS_PermDet_AlgoExit()
{
    INT_X ret = LUX_ERR;

    g_permdet_attr.IsRunning = LUX_FALSE;

    /* 关闭通道 */
    ret = LUX_IVS_PermDet_StopChn();
    if (LUX_OK !=  ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PermDet_StopChn failed\n");
        return LUX_IVS_STOP_RECIVE_PIC_ERR;
    }

    /* 从ivs通道组注销通道 */
    ret = IMP_IVS_UnRegisterChn(g_permdet_attr.bindAttr.chnNum);
    if (LUX_OK !=  ret)
    {
        IMP_LOG_ERR(TAG, "IMP_IVS_UnRegisterChn(%d) failed\n", g_permdet_attr.bindAttr.chnNum);
        return LUX_IVS_CHAN_UNRIGISTER_ERR;
    }

    /* 销毁通道 */
    ret = IMP_IVS_DestroyChn(g_permdet_attr.bindAttr.chnNum);
    if (LUX_OK !=  ret)
    {
        IMP_LOG_ERR(TAG, "IMP_IVS_DestroyChn(%d) failed\n", g_permdet_attr.bindAttr.chnNum);
        return LUX_IVS_CHAN_DESTROY_ERR;
    }

    return LUX_OK;
}

/**
 * @description: 周界侦测算法初始化
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PermDet_Init()
{
    INT_X ret = LUX_ERR;
    /*检查算法版本*/
    UINT_X permdet_ver = perm_get_version_info();
    if (permdet_ver != PERM_VERSION_NUM)
    {
        printf("permdet_ver: The version numbers of head file and lib do not match, head file version: %08x, lib version: %08x\n", PERM_VERSION_NUM, permdet_ver);
        return LUX_ERR;
    }

    Thread_Mutex_Create(&g_permdet_attr.paramMux);
    Thread_Mutex_Create(&g_permdet_attr.mux);

    perm_param_input_t permdParam;
    /*设置周界侦测的属性*/
    memset(&permdParam, 0, sizeof(perm_param_input_t));
    LUX_IVS_PermDet_SetAttr(&permdParam);

    /*获取周界侦测算法句柄*/
    g_permdet_attr.interface = PermInterfaceInit(&permdParam);
    if (NULL == g_permdet_attr.interface)
    {
        IMP_LOG_ERR(TAG, "PermDetInterfaceInit failed\n");
        return LUX_IVS_SET_PARAM_ERR;
    }

    /*创建周界侦测算法通道、绑定算法组、使能通道*/
    ret = LUX_IVS_PermDet_AlgoInit();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PermDet_AlgoInit failed, return error [0x%x]\n", ret);
        return LUX_ERR;
    }

    /*创建线程处理ivs周界侦测结果*/
    ret = Task_CreateThread(perm_det_process_thread, NULL);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, " pthread_create failed\n");
        return LUX_IVS_CREAT_THREAD_ERR;
    }
    return LUX_OK;
}

/**
 * @description: 周界侦测算法去初始化
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_PermDet_Exit()
{
    INT_X ret = LUX_ERR;
    ret = LUX_IVS_PermDet_AlgoExit();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_PermDet_AlgoExit failed, return error [0x%x]\n", ret);
        return LUX_ERR;
    }

    PermInterfaceExit(g_permdet_attr.interface);
    Thread_Mutex_Destroy(&g_permdet_attr.mux);
    Thread_Mutex_Destroy(&g_permdet_attr.paramMux);
    return LUX_OK;
}
