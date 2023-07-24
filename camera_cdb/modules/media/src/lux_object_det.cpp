#include "lux_object_det.hpp"
#define TAG "lux_object_det"

static UCHAR_X obj_left_cnt = 0;
static UCHAR_X obj_loss_cnt = 0;
static UCHAR_X objLeftMissCnt = 0;
static LUX_DAYNIGHT_MODE_EN last_daynight = LUX_DAYNIGHT_MODE_BUTT;
LUX_ALG_OBJDET_ATTR_ST g_obj_det_attr = 
{
    .bInit = false,
    .IsRunning = false,
    .alarmTime = 0,
    .timeInterval = LUX_IVS_TIMEINTERVAL_1MIN,
    .leftAreaCnt = 0,
    .lossObjCnt = 0,
    .in_param = {0},
};

VOID_X LUX_ALG_ObjDet_Start()
{
    g_obj_det_attr.bInit = true;
    return;
}

VOID_X LUX_ALG_ObjDet_Stop()
{
    g_obj_det_attr.bInit = false;
    return;
}

bool LUX_ObjEvent_IsAlarmByInterval(UINT_X lastTime)
{
    UINT_X timeInterval = 0;
    UINT_X curTime = 0;
    bool bAlarm = false;
#if 0
    if (1 == recordReboot)  //重新上电读取配置文件值
    {
        CHAR_X *p_getalarmInterval = IPC_APP_get_alarm_interval_X();
        g_obj_det_attr.timeInterval = (LUX_IVS_TIMEINTERVAL_EN)(*p_getalarmInterval - '0');
        recordReboot = 0;
    }
#endif
    switch (g_obj_det_attr.timeInterval)
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
 * @description: 设置物品遗留丢失侦测算法灵敏度
 * @param [in] sns   灵敏度
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_ALG_ObjDet_SetSense(LUX_IVS_SENSITIVE_EN sns)
{
    INT_X ret = LUX_ERR;
    UINT_X Sense = 0;
    objdet_param_input_t Param;

    memset(&Param, 0, sizeof(objdet_param_input_t));

    do {
        switch (sns)
        {
        case LUX_IVS_SENSITIVE_LOW_EN:
            Sense = 30;
            break;
        case LUX_IVS_SENSITIVE_MID_EN:
            Sense = 10;
            break;
        case LUX_IVS_SENSITIVE_HIG_EN:
            Sense = 5;
            break;
        default:
            IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_SetSense failed, not suppport param,\
                            it will be set default param [%d]  \n", LUX_IVS_SENSITIVE_LOW_EN);
            Sense = 5;
            break;
        }

        ret = LUX_ALG_ObjDet_GetParam(&Param);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_GetParam failed\n");
            ret = LUX_IVS_GET_PARAM_ERR;
            break;
        }

        Param.maxdet_time = Sense;

        ret = LUX_ALG_ObjDet_SetParam(&Param);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_ALG_TamperDet_SetParam failed\n");
            ret = LUX_IVS_SET_PARAM_ERR;
        }
    } while (0);
    return ret;
}

/**
 * @description: 设置物品遗留丢失侦测算法灰度图分割阈值
 * @param [in] sns   灵敏度
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_ALG_ObjDet_SetGrayScale(INT_X scale_value)
{
    if (scale_value < 0 || scale_value > 255)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_SetGrayScale failed, not suppport param,\
                            it will be set default param [%d]  \n", 50);
        scale_value = 50;
    }
    INT_X ret = LUX_ERR;
    objdet_param_input_t Param;

    memset(&Param, 0, sizeof(objdet_param_input_t));

    do {
        ret = LUX_ALG_ObjDet_GetParam(&Param);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_GetParam failed\n");
            ret = LUX_IVS_GET_PARAM_ERR;
            break;
        }

        Param.gray_scale = scale_value;

        ret = LUX_ALG_ObjDet_SetParam(&Param);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_ALG_TamperDet_SetParam failed\n");
            ret = LUX_IVS_SET_PARAM_ERR;
        }
    } while (0);
    return ret;
}

/**
 * @description: 设置物品遗留丢失侦测算法可识别的最大面积
 * @param [in] max_area   最大面积值
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_ALG_ObjDet_SetMaxArea(INT_X max_area)
{
    if (max_area < 0 || max_area > (g_obj_det_attr.in_param.input_org.w * g_obj_det_attr.in_param.input_org.h))
    {
        max_area = g_obj_det_attr.in_param.input_org.w * g_obj_det_attr.in_param.input_org.h / 24;
        IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_SetMaxArea failed, not suppport param,\
                            it will be set default param [%d]  \n", max_area);
    }
    
    INT_X ret = LUX_ERR;
    objdet_param_input_t Param;

    memset(&Param, 0, sizeof(objdet_param_input_t));

    do {
        ret = LUX_ALG_ObjDet_GetParam(&Param);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_GetParam failed\n");
            ret = LUX_IVS_GET_PARAM_ERR;
            break;
        }

        Param.max_area = max_area;

        ret = LUX_ALG_ObjDet_SetParam(&Param);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_SetParam failed\n");
            ret = LUX_IVS_SET_PARAM_ERR;
        }
    } while (0);
    return ret;
}

/**
 * @description: 设置物品遗留丢失侦测算法可识别的最小面积
 * @param [in] sns   灵敏度
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_ALG_ObjDet_SetMinArea(INT_X min_area)
{
    if (min_area < 0)
    {
        min_area = g_obj_det_attr.in_param.input_org.w * g_obj_det_attr.in_param.input_org.h / 6000;
        IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_SetMaxArea failed, not suppport param,\
                            it will be set default param [%d]  \n", min_area);
    }
    if (min_area > g_obj_det_attr.in_param.max_area)
    {
        min_area = g_obj_det_attr.in_param.input_org.w * g_obj_det_attr.in_param.input_org.h / 6000;
        IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_SetMaxArea failed, min_area more than max_area,\
                            it will be set default param [%d]  \n", min_area);
    }

    INT_X ret = LUX_ERR;
    objdet_param_input_t Param;

    memset(&Param, 0, sizeof(objdet_param_input_t));

    do {
        ret = LUX_ALG_ObjDet_GetParam(&Param);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_GetParam failed\n");
            ret = LUX_IVS_GET_PARAM_ERR;
            break;
        }

        Param.min_area = min_area;

        ret = LUX_ALG_ObjDet_SetParam(&Param);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_SetParam failed\n");
            ret = LUX_IVS_SET_PARAM_ERR;
        }
    } while (0);
    return ret;
}

/**
 * @description: 物品遗留丢失侦测事件报警间隔
 * @param  [in] ts 时间间隔,单位s
 * @return 0 成功，非零失败，返回错误码
 */
VOID_X LUX_ALG_ObjDet_SetTimeInterval(LUX_IVS_TIMEINTERVAL_EN ts)
{
    Thread_Mutex_Lock(&g_obj_det_attr.paramMux);
    if(ts < LUX_IVS_TIMEINTERVAL_1MIN || ts >= LUX_IVS_TIME_INTERVAL_BUTTON)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_SetTimeInterval failed, not support param [%d]\n", ts);
        g_obj_det_attr.timeInterval = LUX_IVS_TIMEINTERVAL_1MIN;
    }

    switch (ts)
    {
    case LUX_IVS_TIMEINTERVAL_1MIN ... LUX_IVS_TIMEINTERVAL_5MIN:
        g_obj_det_attr.timeInterval = ts;
        break;
    case LUX_IVS_TIME_INTERVAL_BUTTON:
    default:
        IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_SetTimeInterval failed, not support param [%d]\n", ts);
        g_obj_det_attr.timeInterval = LUX_IVS_TIMEINTERVAL_1MIN;
        break;
    }
    Thread_Mutex_UnLock(&g_obj_det_attr.paramMux);
    return;
}

INT_X LUX_ALG_ObjDet_GetParam(objdet_param_input_t* pInParam)
{
    if (pInParam == NULL)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_GetParam failed, empty point.\n");
        return LUX_PARM_NULL_PTR;
    }
    Thread_Mutex_Lock(&g_obj_det_attr.paramMux);
    memcpy(pInParam, &g_obj_det_attr.in_param, sizeof(objdet_param_input_t));
    Thread_Mutex_UnLock(&g_obj_det_attr.paramMux);
        return LUX_OK;
}

INT_X LUX_ALG_ObjDet_SetParam(objdet_param_input_t* pInParam)
{
    if (pInParam == NULL)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_SetAttr failed, empty point.\n");
        return LUX_PARM_NULL_PTR;
    }
    Thread_Mutex_Lock(&g_obj_det_attr.paramMux);
    memcpy(&g_obj_det_attr.in_param, pInParam, sizeof(objdet_param_input_t));
    Thread_Mutex_UnLock(&g_obj_det_attr.paramMux);
    return LUX_OK;
}

/**
 * @description: 物品遗留丢失侦测报警事件函数
 * @return 0 成功，非零失败
 */
INT_X LUX_ALG_ObjDet_Alarm()
{
    Thread_Mutex_Lock(&g_obj_det_attr.paramMux);
    /*报警时间间隔*/
    if (LUX_ObjEvent_IsAlarmByInterval(g_obj_det_attr.alarmTime))
    {
        g_obj_det_attr.alarmTime = getTime_getS();
        /*上报抓图 */   //TODO

        /* 云存储 */    //TODO

        /*事件录像*/    //TODO
        printf("*** LUX_ALG_ObjDet_Alarm ***\n\n\n");
    }
    Thread_Mutex_UnLock(&g_obj_det_attr.paramMux);
    return LUX_OK;
}

/**
 * @description: 设置物品遗留侦测报警区域
 * @param  [in] zoneInfo  坐标信息
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_ALG_ObjDet_SetRegion(LUX_IVS_M_ZONE_BASE_ST* zoneInfo)
{
    if (NULL == zoneInfo)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_SetRegion failed, empty point.\n");
        return LUX_PARM_NULL_PTR;
    }

    if (zoneInfo->regionCnt > MAX_LEFTDET_AREA_NUMS)
    {
        zoneInfo->regionCnt = MAX_LEFTDET_AREA_NUMS;
        IMP_LOG_ERR(TAG, "[ERROR] alarmArea Number more than %d !!!\n", MAX_LEFTDET_AREA_NUMS);
    }

    INT_X i, j, ret = LUX_OK;
    DOUBLE_X widthRate = g_obj_det_attr.in_param.input_org.w / 100.0;
    DOUBLE_X heightRate = g_obj_det_attr.in_param.input_org.h  / 100.0;
    LUX_UMDET_OBJ_ST area;
    memset(&area, 0, sizeof(LUX_UMDET_OBJ_ST));

    do {
        g_obj_det_attr.leftAreaCnt = zoneInfo->regionCnt;
        area.rectcnt = g_obj_det_attr.leftAreaCnt;

        for (i = 0; i < area.rectcnt; i++)
        {
            /*左上角为(0,0)p0点, 顺时针排列顺序, 依次为p0,p1,p2,p3*/
            g_obj_det_attr.left_area[i].ul.x = alignment_down(((INT_X)(zoneInfo->rect[i].pointX * widthRate)), 1);
            g_obj_det_attr.left_area[i].ul.y = alignment_down(((INT_X)(zoneInfo->rect[i].pointY * heightRate)), 1);
            g_obj_det_attr.left_area[i].br.x = alignment_down(((INT_X)((zoneInfo->rect[i].pointX + zoneInfo->rect[i].width) * widthRate) - 1), 1);
            g_obj_det_attr.left_area[i].br.y = alignment_down(((INT_X)((zoneInfo->rect[i].pointY + zoneInfo->rect[i].height) * heightRate) - 1), 1);

            IMP_LOG_INFO(TAG, "LUX_ALG_ObjDet_SetRegion region[%d]: { ur(%d,%d), br(%d,%d) }. \n", i,
                    g_obj_det_attr.left_area[i].ul.x, g_obj_det_attr.left_area[i].ul.y,
                    g_obj_det_attr.left_area[i].br.x, g_obj_det_attr.left_area[i].br.y);
            area.rects[i].ul.x = g_obj_det_attr.left_area[i].ul.x;
            area.rects[i].ul.y = g_obj_det_attr.left_area[i].ul.y;
            area.rects[i].br.x = g_obj_det_attr.left_area[i].br.x;
            area.rects[i].br.y = g_obj_det_attr.left_area[i].br.y;
        }
        LUX_OSD_UMDetArea_Rect_Show(&area, LUX_TRUE);
    } while (0);

    return ret;
}

/**
 * @description: 设置物品丢失侦测目标
 * @param  [in] zoneInfo  坐标信息
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_ALG_ObjDet_SetObj(LUX_IVS_M_ZONE_BASE_ST* zoneInfo)
{
    if (NULL == zoneInfo)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_SetObj failed, empty point.\n");
        return LUX_PARM_NULL_PTR;
    }

    if (zoneInfo->regionCnt > MAX_LOSSDET_OBJ_NUMS)
    {
        zoneInfo->regionCnt = MAX_LOSSDET_OBJ_NUMS;
        IMP_LOG_ERR(TAG, "[ERROR] alarmobj Number more than %d !!!\n", MAX_LOSSDET_OBJ_NUMS);
    }

    INT_X i, j, ret = LUX_OK;
    DOUBLE_X widthRate = g_obj_det_attr.in_param.input_org.w / 100.0;
    DOUBLE_X heightRate = g_obj_det_attr.in_param.input_org.h  / 100.0;
    LUX_UMDET_OBJ_ST area;
    memset(&area, 0, sizeof(LUX_UMDET_OBJ_ST));

    do {
        g_obj_det_attr.lossObjCnt = zoneInfo->regionCnt;
        area.rectcnt = g_obj_det_attr.lossObjCnt;
        for (i = 0; i < area.rectcnt; i++)
        {
            /*左上角为(0,0)p0点, 顺时针排列顺序, 依次为p0,p1,p2,p3*/
            g_obj_det_attr.loss_obj[i].ul.x = alignment_down(((INT_X)(zoneInfo->rect[i].pointX * widthRate)), 1);
            g_obj_det_attr.loss_obj[i].ul.y = alignment_down(((INT_X)(zoneInfo->rect[i].pointY * heightRate)), 1);
            g_obj_det_attr.loss_obj[i].br.x = alignment_down(((INT_X)((zoneInfo->rect[i].pointX + zoneInfo->rect[i].width) * widthRate) - 1), 1);
            g_obj_det_attr.loss_obj[i].br.y = alignment_down(((INT_X)((zoneInfo->rect[i].pointY + zoneInfo->rect[i].height) * heightRate) - 1), 1);

            IMP_LOG_INFO(TAG, "LUX_ALG_ObjDet_SetObj obj[%d]: { ur(%d,%d), br(%d,%d) }. \n", i,
                    g_obj_det_attr.loss_obj[i].ul.x, g_obj_det_attr.loss_obj[i].ul.y,
                    g_obj_det_attr.loss_obj[i].br.x, g_obj_det_attr.loss_obj[i].br.y);
            area.rects[i].ul.x = g_obj_det_attr.loss_obj[i].ul.x;
            area.rects[i].ul.y = g_obj_det_attr.loss_obj[i].ul.y;
            area.rects[i].br.x = g_obj_det_attr.loss_obj[i].br.x;
            area.rects[i].br.y = g_obj_det_attr.loss_obj[i].br.y;
        }
        LUX_OSD_UMDetOBJLoss_Rect_Show(&area, LUX_TRUE);
    } while (0);

    return ret;
}

/**
 * @description: 物品遗留丢失侦测启动设置配置
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_ALG_ObjDet_SetConf()
{
    INT_X ret = LUX_ERR;
    LUX_IVS_M_ZONE_BASE_ST alarmZone;
    CHAR_X strAlarmZoneInfo[STR_LEN_MAX];

    /*隐私模式下不开启物品遗留丢失侦测*/
    if(LUX_TRUE == g_configData->alg_config.sleepMode)
    {
        LUX_ALG_ObjDet_Stop();
        printf("+++++++++++++++++ Line[%d] Function[%s] object_det off +++++++++++++++++\n", __LINE__, __FUNCTION__);
    }
    else
    {
        /*设置是否开启物品遗留丢失侦测警告*/
        if (LUX_TRUE == g_configData->alg_config.object_det_enabled)
        {
            LUX_ALG_ObjDet_Start();
            printf("+++++++++++++++++ Line[%d] Function[%s] object_det on +++++++++++++++++\n", __LINE__, __FUNCTION__);

            ret = LUX_ALG_ObjDet_SetSense((LUX_IVS_SENSITIVE_EN)g_configData->alg_config.object_det_sensitivity);
            if (LUX_OK !=  ret)
            {
                IMP_LOG_ERR(TAG, " LUX_ALG_ObjDet_SetSense failed", ret);
                return LUX_IVS_SET_PARAM_ERR;
            }
            printf("obj_det LUX_ALG_ObjDet_SetSense Success\n");

            ret = LUX_ALG_ObjDet_SetGrayScale(g_configData->alg_config.object_det_gray_scale);
            if (LUX_OK !=  ret)
            {
                IMP_LOG_ERR(TAG, " LUX_ALG_ObjDet_SetGrayScale failed", ret);
                return LUX_IVS_SET_PARAM_ERR;
            }
            printf("obj_det LUX_ALG_ObjDet_SetGrayScale Success\n");

            ret = LUX_ALG_ObjDet_SetMaxArea(g_configData->alg_config.object_det_max_area);
            if (LUX_OK !=  ret)
            {
                IMP_LOG_ERR(TAG, " LUX_ALG_ObjDet_SetMaxArea failed", ret);
                return LUX_IVS_SET_PARAM_ERR;
            }
            printf("obj_det LUX_ALG_ObjDet_SetMaxArea Success\n");

            ret = LUX_ALG_ObjDet_SetMinArea(g_configData->alg_config.object_det_min_area);
            if (LUX_OK !=  ret)
            {
                IMP_LOG_ERR(TAG, " LUX_ALG_ObjDet_SetMinArea failed", ret);
                return LUX_IVS_SET_PARAM_ERR;
            }
            printf("obj_det LUX_ALG_ObjDet_SetMinArea Success\n");


            memset(&alarmZone, 0, sizeof(LUX_IVS_M_ZONE_BASE_ST));
            strcpy(strAlarmZoneInfo, g_configData->alg_config.leftdet_area);
            printf("+++ left_alarmArea: %s +++\n", strAlarmZoneInfo);
            ret= LUX_IVS_alarmZone_cJsonParse(strAlarmZoneInfo, &alarmZone);
            if (LUX_OK !=  ret)
            {
                IMP_LOG_ERR(TAG, " LUX_IVS_alarmZone_cJsonParse failed, return error [0x%x]\n", ret);
                return LUX_IVS_SET_PARAM_ERR;
            }
            printf("obj_det LUX_IVS_alarmZone_cJsonParse leftdet_area Success\n");
            ret = LUX_ALG_ObjDet_SetRegion(&alarmZone);
            if (LUX_OK !=  ret)
            {
                IMP_LOG_ERR(TAG, " LUX_ALG_ObjDet_SetRegion failed\n");
                return LUX_IVS_SET_PARAM_ERR;
            }
            printf("obj_det LUX_ALG_ObjDet_SetRegion Success\n");

            memset(&alarmZone, 0, sizeof(LUX_IVS_M_ZONE_BASE_ST));
            strcpy(strAlarmZoneInfo, g_configData->alg_config.lossdet_obj);
            printf("+++ loss_objRect: %s +++\n", strAlarmZoneInfo);
            ret= LUX_IVS_alarmZone_cJsonParse(strAlarmZoneInfo, &alarmZone);
            if (LUX_OK !=  ret)
            {
                IMP_LOG_ERR(TAG, " LUX_IVS_alarmZone_cJsonParse failed, return error [0x%x]\n", ret);
                return LUX_IVS_SET_PARAM_ERR;
            }
            printf("obj_det LUX_IVS_alarmZone_cJsonParse lossdet_obj Success\n");
            ret = LUX_ALG_ObjDet_SetObj(&alarmZone);
            if (LUX_OK !=  ret)
            {
                IMP_LOG_ERR(TAG, " LUX_ALG_ObjDet_SetObj failed\n");
                return LUX_IVS_SET_PARAM_ERR;
            }
            printf("obj_det LUX_ALG_ObjDet_SetObj Success\n");

        }
        else
        {
            LUX_ALG_ObjDet_Stop();
            printf("+++++++++++++++++ Line[%d] Function[%s] object_det off +++++++++++++++++\n", __LINE__, __FUNCTION__);
        }
    }
    return LUX_OK;
}

/**
 * @description: 物品遗留丢失侦测算法更新初始化帧接口
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_ALG_ObjDet_UpdateInitFrame()
{
    INT_X ret = LUX_ERR;
    UINT_X area = 0;
    IMPFrameInfo* pFrame = nullptr;
    um_input_info_t input;

    do
    {
        Thread_Mutex_Lock(&g_framSrcAttr->fsChn[LUX_STREAM_SUB].mux);
        /*获取帧*/
        ret = IMP_FrameSource_GetFrame(LUX_STREAM_SUB, &pFrame);
        if (ret != LUX_OK) {
            IMP_LOG_ERR(TAG, "IMP_FrameSource_GetFrame failed\n");
            break;
        }
        // printf("frame info: w=%u, h=%u, size=%u\n", pFrame->width, pFrame->height, pFrame->size);

        /*转换数据格式*/
        memset(&input, 0, sizeof(um_input_info_t));
        input.image = (unsigned char*)pFrame->virAddr;
        input.w = pFrame->width;
        input.h = pFrame->height;

        /*保存初始化时的帧信息*/
        Thread_Mutex_Lock(&g_obj_det_attr.paramMux);
        g_obj_det_attr.in_param.input_org = input;
        g_obj_det_attr.in_param.ts_s = pFrame->timeStamp / 1000000;
        Thread_Mutex_UnLock(&g_obj_det_attr.paramMux);

        /*初始化*/
        ret = lux_UMDet_init(&input);
        if (ret == LUX_ERR)
        {
            IMP_LOG_ERR(TAG, "lux_UMDet_init failed\n");
            break;
        }

        /*释放帧*/
        ret = IMP_FrameSource_ReleaseFrame(LUX_STREAM_SUB, pFrame);
        if (ret != LUX_OK) {
            IMP_LOG_ERR(TAG, "IMP_FrameSource_ReleaseFrame failed\n");
            break;
        }
        Thread_Mutex_UnLock(&g_framSrcAttr->fsChn[LUX_STREAM_SUB].mux);
    } while (0);
    return ret;
}


VOID_X LUX_ALG_ObjDet_CheckInit()
{
    LUX_DAYNIGHT_MODE_EN daynight_mode;
    /*子码流是否切换分辨率或红外模式是否切换*/
    LUX_ISP_GetDayDightMode(&daynight_mode);
    if (g_obj_det_attr.in_param.input_org.w != g_framSrcAttr->fsChn[LUX_STREAM_SUB].fsChnAttr.picWidth || last_daynight != daynight_mode)
    {
        sleep(2);//待画面稳定
        // printf("+++++ last_daynight=%d, daynight_mode=%d +++++\n", last_daynight, daynight_mode);
        last_daynight = daynight_mode;
        INT_X ret = LUX_ERR;
        UINT_X area = g_framSrcAttr->fsChn[LUX_STREAM_SUB].fsChnAttr.picWidth * g_framSrcAttr->fsChn[LUX_STREAM_SUB].fsChnAttr.picHeight;
        objdet_param_input_t odParam;
        memset(&odParam, 0, sizeof(objdet_param_input_t));
        /*默认阈值*/
        odParam.gray_scale = 50;
        odParam.cons_count = 1;
        odParam.maxdet_time = 10;
        odParam.min_area = area / 6000;
        odParam.max_area = area / 24;
        LUX_ALG_ObjDet_SetParam(&odParam);
        ret = LUX_ALG_ObjDet_UpdateInitFrame();
        if (ret != LUX_OK)
        {
            IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_UpdateInitFrame failed\n");
        }
    }
}

INT_X LUX_ALG_ObjDet_GetResult(objdet_param_input_t *p_in, LUX_OBJ_TYPE_EN obj_type, objdet_param_output_t *p_out)
{
    if (nullptr == p_in || p_out == nullptr)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_GetResult failed.\n");
        return LUX_PARM_NULL_PTR;
    }
    INT_X ret = LUX_ERR;
    IMPFrameInfo* pFrame = nullptr;
    um_input_info_t input;
    Thread_Mutex_Lock(&g_framSrcAttr->fsChn[LUX_STREAM_SUB].mux);
    do
    {
        /*获取帧*/
        ret = IMP_FrameSource_GetFrame(LUX_STREAM_SUB, &pFrame);
        if (ret != LUX_OK) {
            IMP_LOG_ERR(TAG, "IMP_FrameSource_GetFrame failed\n");
            break;
        }
        // printf("objectDet frame info: w=%u, h=%u, size=%u ts=%ld\n", pFrame->width, pFrame->height, pFrame->size, pFrame->timeStamp);

        /*转换数据格式*/
        memset(&input, 0, sizeof(um_input_info_t));
        input.image = (unsigned char*)pFrame->virAddr;
        input.w = pFrame->width;
        input.h = pFrame->height;
        p_in->ts_s = pFrame->timeStamp / 1000000;

        /*获取结果*/
        g_obj_det_attr.objResult.clear();
        ret = lux_UMDet_start(&input, p_in, g_obj_det_attr.objResult);
        if (ret != LUX_OK) {
            IMP_LOG_ERR(TAG, "lux_UMDet_start failed\n");
            break;
        }

        /*释放帧*/
        ret = IMP_FrameSource_ReleaseFrame(LUX_STREAM_SUB, pFrame);
        if (ret != LUX_OK) {
            IMP_LOG_ERR(TAG, "IMP_FrameSource_ReleaseFrame failed\n");
            break;
        }
    } while (0);
    Thread_Mutex_UnLock(&g_framSrcAttr->fsChn[LUX_STREAM_SUB].mux);
    p_out->clear();
    for (int i = 0; i < g_obj_det_attr.objResult.size(); i++)
    {
        if (obj_type == LUX_OBJ_LEFT)        //判遗留
        {
            for (int j = 0; j < g_obj_det_attr.leftAreaCnt; j++)
            {
                if (g_obj_det_attr.objResult[i].x1 >= g_obj_det_attr.left_area[j].ul.x &&
                    g_obj_det_attr.objResult[i].y1 >= g_obj_det_attr.left_area[j].ul.y &&
                    g_obj_det_attr.objResult[i].x2 <= g_obj_det_attr.left_area[j].br.x &&
                    g_obj_det_attr.objResult[i].y2 <= g_obj_det_attr.left_area[j].br.y)
                {
                    p_out->push_back(g_obj_det_attr.objResult[i]);
                }
            }
        }

        else if (obj_type == LUX_OBJ_LOSS)        //判丢失
        {
            UM_ObjectBox obj;
            memset(&obj, 0, sizeof(UM_ObjectBox));
            for (int k = 0; k < g_obj_det_attr.lossObjCnt; k++)
            {
                if (g_obj_det_attr.objResult[i].x1 >= g_obj_det_attr.loss_obj[k].ul.x &&
                    g_obj_det_attr.objResult[i].y1 >= g_obj_det_attr.loss_obj[k].ul.y &&
                    g_obj_det_attr.objResult[i].x2 <= g_obj_det_attr.loss_obj[k].br.x &&
                    g_obj_det_attr.objResult[i].y2 <= g_obj_det_attr.loss_obj[k].br.y)
                {
                    obj.x1 = g_obj_det_attr.loss_obj[k].ul.x;
                    obj.y1 = g_obj_det_attr.loss_obj[k].ul.y;
                    obj.x2 = g_obj_det_attr.loss_obj[k].br.x;
                    obj.y2 = g_obj_det_attr.loss_obj[k].br.y;
                    p_out->push_back(obj);
                }
            }
        }
    }
    return ret;
}

VOID_X LUX_ALG_ObjDet_Process()
{
    INT_X ret = LUX_ERR;
    UINT_X nowtime = 0;
    LUX_UMDET_OBJ_ST obj;

    Thread_Mutex_Lock(&g_obj_det_attr.mux);
    do
    {
        /*检查初始化，若分辨率改变，重新初始化*/
        LUX_ALG_ObjDet_CheckInit();

        /*获取物品遗留侦测结果*/
        nowtime = getTime_getMs();
        ret = LUX_ALG_ObjDet_GetResult(&g_obj_det_attr.in_param, LUX_OBJ_LEFT, &g_obj_det_attr.objleftResult);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_GetLeftResult failed.\n");
            break;
        }
        memset(&obj, 0, sizeof(LUX_UMDET_OBJ_ST));
        if (!g_obj_det_attr.objleftResult.empty())
        {
            LUX_ALG_ObjDet_Alarm();
            obj.rectcnt = g_obj_det_attr.objleftResult.size();
            if (obj.rectcnt > LUX_UM_OBJLEFT_NUM_MAX)
            {
                obj.rectcnt = LUX_UM_OBJLEFT_NUM_MAX;
            }
#if 1
            if (obj_left_cnt >= OBJDET_FLITER_NUMS)
            {
                printf("++++ time=[%u]ms leftobjCnt=%d ++++\n\n\n", getTime_getMs() - nowtime, obj.rectcnt);
                obj_left_cnt = 0;
            }
            obj_left_cnt++;
#endif
            for (int i = 0; i < obj.rectcnt; i++) {
                obj.rects[i].ul.x = (INT_X)g_obj_det_attr.objleftResult[i].x1;
                obj.rects[i].ul.y = (INT_X)g_obj_det_attr.objleftResult[i].y1;
                obj.rects[i].br.x = (INT_X)g_obj_det_attr.objleftResult[i].x2;
                obj.rects[i].br.y = (INT_X)g_obj_det_attr.objleftResult[i].y2;
                // printf("=*=  obj[%d]: x1=%d, y1=%d, x2=%d, y2=%d\n", i, obj.rects[i].ul.x, obj.rects[i].ul.y, obj.rects[i].br.x, obj.rects[i].br.y);
            }
            if(obj_left_cnt >= OBJDET_UPTATE_NUMS)
            {
                LUX_OSD_UMDetOBJLeft_Rect_Miss();
                LUX_OSD_UMDetOBJLeft_Rect_Show(&obj);
                objLeftMissCnt = 0;
            }
        }
        else if (objLeftMissCnt >= OBJDET_UPTATE_NUMS)
        {
            LUX_OSD_UMDetOBJLeft_Rect_Miss();
            objLeftMissCnt = 0;
        }
        else
        {
            objLeftMissCnt++;
        }

        /*获取物品丢失侦测结果*/
        nowtime = getTime_getMs();
        ret = LUX_ALG_ObjDet_GetResult(&g_obj_det_attr.in_param, LUX_OBJ_LOSS, &g_obj_det_attr.objlossResult);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_GetLossResult failed.\n");
            break;
        }

        memset(&obj, 0, sizeof(LUX_UMDET_OBJ_ST));
        if (!g_obj_det_attr.objlossResult.empty())
        {
            LUX_ALG_ObjDet_Alarm();
            obj.rectcnt = g_obj_det_attr.objlossResult.size();
            if (obj.rectcnt > g_obj_det_attr.lossObjCnt)
            {
                obj.rectcnt = g_obj_det_attr.lossObjCnt;
            }
#if 1
            if (obj_loss_cnt >= OBJDET_FLITER_NUMS)
            {
                printf("++++ time=[%u]ms lossobjCnt=%d ++++\n\n\n", getTime_getMs() - nowtime, obj.rectcnt);
                obj_loss_cnt = 0;
            }
            obj_loss_cnt++;
#endif
            for (int j = 0; j < obj.rectcnt; j++) {
                obj.rects[j].ul.x = (INT_X)g_obj_det_attr.objlossResult[j].x1;
                obj.rects[j].ul.y = (INT_X)g_obj_det_attr.objlossResult[j].y1;
                obj.rects[j].br.x = (INT_X)g_obj_det_attr.objlossResult[j].x2;
                obj.rects[j].br.y = (INT_X)g_obj_det_attr.objlossResult[j].y2;
                // printf("=*0=  obj[%d]: x1=%d, y1=%d, x2=%d, y2=%d\n", j, obj.rects[j].ul.x, obj.rects[j].ul.y, obj.rects[j].br.x, obj.rects[j].br.y);
            }
            if(obj_loss_cnt >= OBJDET_UPTATE_NUMS)
            {
                LUX_OSD_UMDetOBJLoss_Rect_ChangeColor(&obj, OSD_YELLOW);
            }
        }
    } while (0);
    Thread_Mutex_UnLock(&g_obj_det_attr.mux);
    usleep(OBJDET_DELAY_TIME);
    return ;
}

VOID_X object_det_process_thread()
{
    CHAR_X threadName[64] = {0};
    CHAR_X ivsChnName[32] = {0};

    strcpy(ivsChnName, "OBJ_DETECTION");
    g_obj_det_attr.IsRunning = true;

    sprintf(threadName, "ALG_%s", ivsChnName);
    prctl(PR_SET_NAME, threadName);
    while (g_obj_det_attr.IsRunning)
    {
        if (!g_obj_det_attr.bInit || !g_framSrcAttr->bInit)
        {
            usleep(SLEEP_TIME);
            continue;
        }
        else
        {
            LUX_ALG_ObjDet_Process();
        }
    }
    return;
}

INT_X LUX_ALG_ObjDet_Init()
{
    INT_X ret = LUX_ERR;
    UINT_X area = g_framSrcAttr->fsChn[LUX_STREAM_SUB].fsChnAttr.picWidth * g_framSrcAttr->fsChn[LUX_STREAM_SUB].fsChnAttr.picHeight;
    IMPFrameInfo* pFrame = nullptr;
    um_input_info_t input;
    objdet_param_input_t odParam;

    Thread_Mutex_Create(&g_obj_det_attr.paramMux);
    Thread_Mutex_Create(&g_obj_det_attr.mux);

    LUX_ISP_GetDayDightMode(&last_daynight);
    // printf("+++++ last_daynight=%d +++++\n", last_daynight);
    /*默认阈值*/
    odParam.gray_scale = 50;
    odParam.cons_count = 1;
    odParam.maxdet_time = 5;
    odParam.min_area = area / 6000;
    odParam.max_area = area / 24;
    LUX_ALG_ObjDet_SetParam(&odParam);
    ret = LUX_ALG_ObjDet_UpdateInitFrame();
    if (ret != LUX_OK)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_ObjDet_UpdateInitFrame failed\n");
        return ret;
    }
    ret = LUX_ALG_ObjDet_SetConf();
    if (ret != LUX_OK)
    {
        IMP_LOG_ERR(TAG, "LUX_ALG_ObjectDetConf failed\n");
        return ret;
    }

    thread t_object(object_det_process_thread);
    t_object.detach();
    return ret;
}

INT_X LUX_ALG_ObjDet_Exit()
{
    g_obj_det_attr.IsRunning = false;
    LUX_ALG_ObjDet_Stop();
    INT_X ret = lux_UMDet_exit();
    if (ret == LUX_ERR)
    {
        IMP_LOG_ERR(TAG, "lux_UMDet_exit failed\n");
    }

    Thread_Mutex_Destroy(&g_obj_det_attr.mux);
    Thread_Mutex_Destroy(&g_obj_det_attr.paramMux);
    return ret;
}