#include "lux_face_det.h"

#define TAG "lux_face_det"

/*打印结果过滤次数*/
static UCHAR_X face_cnt = 0;
/* 未侦测到人脸次数 */
static UCHAR_X faceMissCnt = 0;

LUX_IVS_FACEDET_ATTR_ST g_facedet_attr =
{
    .bInit = LUX_FALSE,
    .alarmTime = 0,
    .timeInterval = LUX_IVS_TIMEINTERVAL_1MIN,
    .bindAttr =
    {
        .grpId = 0,
        .chnNum = LUX_IVS_FACE_DET_EN,
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
        .max_face_box = LUX_FACTRECT_NUM_MAX,                 /**< 人脸检测处理过程中保留的框数量 */
        .sense = 1,                        /**< 检测灵敏度 0~3 0:最不灵敏 3:最灵敏 default:1 */
        .detdist = 3,                       /**<  检测距离 0~3 default:3 */
        .model_path = "/system/lib/model/face_det.bin",
        .skip_num = 0,                     /**< 跳帧数目 */
        .rot90 = LUX_FALSE,                    /**< 图像是否顺时针旋转90度 */
        .switch_liveness = LUX_TRUE,           /**< 人脸活体检测模块开关, 当true时会做活体检测, 当false时不做活体检测返回-1 */
        .switch_face_pose = LUX_TRUE,          /**< 人脸角度检测模块开关, 当true时会做人脸角度检测, 当false时不做人脸角度检测返回-1 */
        .switch_face_blur = LUX_TRUE,          /**< 人脸模糊检测模块开关, 当true时会做人脸模糊检测, 当false时不做人脸模糊检测返回-1 */
        .switch_landmark = LUX_TRUE,	        /* < 人脸关键检测开关 */
        .switch_track = LUX_FALSE,
        .ptime = LUX_FALSE,
        .nmem_size = 0,//(8.5*1024*1024),//8.5*1024*1024;
        .share_mem_mode = 4,
    },
};

extern LUX_FSRC_ATTR_ST* g_framSrcAttr;
extern LUX_CONFIG_DATA_ST* g_configData;
extern VOID_X LUX_OSD_FaceRect_Show(facedet_param_output_t* ivsFaceDetRet);

/**
 * @description: 设置人脸侦测算法灵敏度
 * @param [in] sns   灵敏度
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_FD_SetSense(LUX_IVS_SENSITIVE_EN sns)
{
    INT_X ret = LUX_ERR;
    INT_X FDSense = 0;
    facedet_param_input_t FDParam;

    memset(&FDParam, 0, sizeof(facedet_param_input_t));

    Thread_Mutex_Lock(&g_facedet_attr.paramMux);
    do {
        switch (sns)
        {
        case LUX_IVS_SENSITIVE_LOW_EN:
            FDSense = 1;
            break;
        case LUX_IVS_SENSITIVE_MID_EN:
            FDSense = 2;
            break;
        case LUX_IVS_SENSITIVE_HIG_EN:
            FDSense = 3;
            break;
        default:
            IMP_LOG_ERR(TAG, "LUX_IVS_FD_SetSense failed, chn(%d), not suppport param,\
                            it will be set default param [%d]  \n", g_facedet_attr.bindAttr.chnNum, LUX_IVS_SENSITIVE_LOW_EN);
            FDSense = 1;
            break;
        }

        ret = IMP_IVS_GetParam(g_facedet_attr.bindAttr.chnNum, (PVOID_X)&FDParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_GetParam failed, chn(%d) . \n", g_facedet_attr.bindAttr.chnNum);
            ret = LUX_IVS_GET_PARAM_ERR;
            break;
        }

        FDParam.sense = FDSense;

        ret = IMP_IVS_SetParam(g_facedet_attr.bindAttr.chnNum, (PVOID_X)&FDParam);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_SetParam chn(%d) failed. \n", g_facedet_attr.bindAttr.chnNum);
            ret = LUX_IVS_SET_PARAM_ERR;
            break;
        }
    } while (0);
    Thread_Mutex_UnLock(&g_facedet_attr.paramMux);
    return ret;
}

/**
 * @description: 开启人脸侦测算法通道
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_FD_StartChn()
{
    INT_X ret = LUX_OK;

    if (LUX_TRUE == g_facedet_attr.bInit)
    {
        IMP_LOG_ERR(TAG, "Warnning, LUX_IVS_FD_StartChn already Started\n");
        return LUX_OK;
    }

    Thread_Mutex_Lock(&g_facedet_attr.mux);
    do {
        ret = IMP_IVS_StartRecvPic(g_facedet_attr.bindAttr.chnNum);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_StartRecvPic failed\n");
            break;
        }
        printf("IMP_IVS_StartRecvPic g_facedet_attr.bindAttr.chnNum success\n");//test
        g_facedet_attr.bInit = LUX_TRUE;
    } while (0);
    Thread_Mutex_UnLock(&g_facedet_attr.mux);

    return ret;
}

/**
 * @description: 关闭人脸侦测算法通道
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_FD_StopChn()
{
    INT_X ret = LUX_OK;

    if (LUX_FALSE == g_facedet_attr.bInit)
    {
        IMP_LOG_ERR(TAG, "Warnning, LUX_IVS_FD_StopChn already Stopped\n");
        return LUX_OK;
    }

    Thread_Mutex_Lock(&g_facedet_attr.mux);
    do {
        ret = IMP_IVS_StopRecvPic(g_facedet_attr.bindAttr.chnNum);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_StopRecvPic failed\n");
            break;
        }

        g_facedet_attr.bInit = LUX_FALSE;
    } while (0);
    Thread_Mutex_UnLock(&g_facedet_attr.mux);

    return ret;
}

/**
 * @description: 开启IVS人脸侦测
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_FD_StartAlgo()
{
    INT_X ret = LUX_ERR;
    IMP_LOG_INFO(TAG, "LUX_IVS_FD_StartAlgo\n");
    ret = LUX_IVS_FD_StartChn();
    if (ret != LUX_OK)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_FD_StartChn failed\n");
        return LUX_IVS_START_RECIVE_PIC_ERR;
    }
    g_facedet_attr.bStart = LUX_TRUE;
    return LUX_OK;
}

/**
 * @description: 关闭IVS人脸侦测
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_FD_StopAlgo()
{
    INT_X ret = LUX_ERR;
    IMP_LOG_INFO(TAG, "LUX_IVS_FD_StopAlgo\n");
    ret = LUX_IVS_FD_StopChn();
    if (ret != LUX_OK)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_FD_StopChn failed\n");
        return LUX_IVS_STOP_RECIVE_PIC_ERR;
    }
    g_facedet_attr.bStart = LUX_FALSE;
    return LUX_OK;
}

/**
 * @description: 人脸侦测事件报警间隔
 * @param  [in] ts 时间间隔,单位s
 * @return 0 成功，非零失败，返回错误码
 */
VOID_X LUX_IVS_FD_SetTimeInterval(LUX_IVS_TIMEINTERVAL_EN ts)
{
    Thread_Mutex_Lock(&g_facedet_attr.paramMux);
    if (ts < LUX_IVS_TIMEINTERVAL_1MIN || ts >= LUX_IVS_TIME_INTERVAL_BUTTON)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_FD_SetTimeInterval failed, not support param [%d]\n", ts);
        g_facedet_attr.timeInterval = LUX_IVS_TIMEINTERVAL_1MIN;
    }

    switch (ts)
    {
    case LUX_IVS_TIMEINTERVAL_1MIN ... LUX_IVS_TIMEINTERVAL_5MIN:
        g_facedet_attr.timeInterval = ts;
        break;
    case LUX_IVS_TIME_INTERVAL_BUTTON:
    default:
        IMP_LOG_ERR(TAG, "LUX_IVS_FD_SetTimeInterval failed, not support param [%d]\n", ts);
        g_facedet_attr.timeInterval = LUX_IVS_TIMEINTERVAL_1MIN;
        break;
    }
    Thread_Mutex_UnLock(&g_facedet_attr.paramMux);

    return;
}

/**
 * @description: 设置人脸侦测的属性
 * @param  [in] param 属性
 */
VOID_X LUX_IVS_FD_SetAttr(facedet_param_input_t* pParam)
{
    Thread_Mutex_Lock(&g_facedet_attr.paramMux);
    memcpy(pParam, &g_facedet_attr.in_param, sizeof(facedet_param_input_t));
    Thread_Mutex_UnLock(&g_facedet_attr.paramMux);

    return;
}

/**
 * @description: 人脸侦测启动设置配置
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_FD_SetConf()
{
    INT_X ret = LUX_ERR;
    INT_X alarmSen = LUX_IVS_SENSITIVE_LOW_EN;
    BOOL_X sleepMode = LUX_FALSE;

    ret = LUX_IVS_FD_StartChn();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_FD_StartChn(%d) failed\n", g_facedet_attr.bindAttr.chnNum);
        return LUX_IVS_START_RECIVE_PIC_ERR;
    }

    /*隐私模式下不开启人脸侦测*/
    if (LUX_TRUE == g_configData->alg_config.sleepMode)
    {
        ret = LUX_IVS_FD_StopAlgo();
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_IVS_FD_StopAlgo failed\n");
            return LUX_IVS_STOP_RECIVE_PIC_ERR;
        }
        printf("+++++++++++++++++ Line[%d] Function[%s] face_det off +++++++++++++++++\n", __LINE__, __FUNCTION__);
    }
    else
    {
        printf("+++++++++++============= g_configData->alg_config.face_detect_enabled:%d\n", g_configData->alg_config.face_detect_enabled);
        if (LUX_TRUE == g_configData->alg_config.face_detect_enabled)
        {
            ret = LUX_IVS_FD_StartAlgo();
            if (LUX_OK != ret)
            {
                IMP_LOG_ERR(TAG, "LUX_IVS_FD_StartAlgo failed\n");
                return LUX_IVS_START_RECIVE_PIC_ERR;
            }
            printf("+++++++++++++++++ Line[%d] Function[%s] face_det on +++++++++++++++++\n", __LINE__, __FUNCTION__);
        }
        else
        {
            ret = LUX_IVS_FD_StopAlgo();
            if (LUX_OK != ret)
            {
                IMP_LOG_ERR(TAG, "LUX_IVS_FD_StopAlgo failed\n");
                return LUX_IVS_STOP_RECIVE_PIC_ERR;
            }
            printf("+++++++++++++++++ Line[%d] Function[%s] face_det off +++++++++++++++++\n", __LINE__, __FUNCTION__);
        }
    }

    alarmSen = g_configData->alg_config.face_detect_sensitivity;
    printf("face alarmSen=%d\n", alarmSen);
    ret = LUX_IVS_FD_SetSense((LUX_IVS_SENSITIVE_EN)alarmSen);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_FD_SetSense failed\n");
        return LUX_IVS_SET_PARAM_ERR;
    }
    printf("+++++++++++++++++ Line[%d] Function[%s] face_det sensitive [%d] +++++++++++++++++\n", __LINE__, __FUNCTION__, alarmSen);
    return LUX_OK;
}

/**
 * @description: 根据人脸侦测报警时间判断是否上报
 * @param  [in] lastTime 人脸侦测的报警时间
 * @return LUX_FALSE不报警，LUX_TRUE报警
 */
BOOL_X LUX_FaceEvent_IsAlarmByInterval(UINT_X lastTime)
{
    UINT_X timeInterval = 0;
    UINT_X curTime = 0;
    BOOL_X bAlarm = LUX_FALSE;
#if 0
    if (1 == recordReboot)  //重新上电读取配置文件值
    {
        CHAR_X *p_getalarmInterval = IPC_APP_get_alarm_interval_X();
        g_facedet_attr.timeIntervel = (LUX_IVS_TIMEINTERVAL_EN)(*p_getalarmInterval - '0');
        recordReboot = 0;
    }
#endif
    switch (g_facedet_attr.timeInterval)
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
 * @description: 人脸侦测报警事件函数
 * @return 0 成功，非零失败
 */
INT_X LUX_IVS_FD_Alarm()
{
    Thread_Mutex_Lock(&g_facedet_attr.paramMux);
    /*报警时间间隔*/
    if (LUX_FaceEvent_IsAlarmByInterval(g_facedet_attr.alarmTime))
    {
        g_facedet_attr.alarmTime = getTime_getS();
        /*上报抓图 */   //TODO

        /* 云存储 */    //TODO

        /*事件录像*/    //TODO
        printf("*** LUX_IVS_FD_Alarm ***\n\n\n");
    }
    Thread_Mutex_UnLock(&g_facedet_attr.paramMux);
    return LUX_OK;
}

/**
 * @description: 人脸侦测算法处理
 */
VOID_X LUX_IVS_FD_Process()
{
    INT_X ret = LUX_ERR;
    INT_X i = 0;
    facedet_param_output_t show_data;

    Thread_Mutex_Lock(&g_facedet_attr.mux);
    do
    {
        ret = IMP_IVS_PollingResult(g_facedet_attr.bindAttr.chnNum, IMP_IVS_DEFAULT_TIMEOUTMS);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_PollingResult(%d, %d) failed\n", g_facedet_attr.bindAttr.chnNum, IMP_IVS_DEFAULT_TIMEOUTMS);
            break;
        }

        ret = IMP_IVS_GetResult(g_facedet_attr.bindAttr.chnNum, (PVOID_X*)&g_facedet_attr.ivsFaceDetResult);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_GetResult(%d) failed\n", g_facedet_attr.bindAttr.chnNum);
            break;
        }

        memset(&show_data, 0, sizeof(facedet_param_output_t));
        if (g_facedet_attr.ivsFaceDetResult->count > 0)
        {
            for (i = 0; i < g_facedet_attr.ivsFaceDetResult->count; i++)
            {
                if (g_facedet_attr.ivsFaceDetResult->face[i].class_id == 0)
                {
                    show_data.face[show_data.count] = g_facedet_attr.ivsFaceDetResult->face[i];
                    show_data.count++;
                }
            }
#if 0
            if (face_cnt == FACEDET_FILTER_NUM)
            {
                printf("++++++++++ Line[%d] Function[%s] face nums=%d ++++++++++\n", __LINE__, __FUNCTION__, show_data.count);
                face_cnt = 0;
            }
            face_cnt++;
#endif
            LUX_OSD_FaceRect_Miss();
            LUX_OSD_FaceRect_Show(&show_data);
            faceMissCnt = 0;
        }
        else if (faceMissCnt > FACEDET_MISS_NUM)
        {
            LUX_OSD_FaceRect_Miss();
            faceMissCnt = 0;
        }
        else
        {
            faceMissCnt++;
        }

        ret = IMP_IVS_ReleaseResult(g_facedet_attr.bindAttr.chnNum, (PVOID_X)g_facedet_attr.ivsFaceDetResult);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_IVS_ReleaseResult(%d) failed\n", g_facedet_attr.bindAttr.chnNum);
            break;
        }

        if (show_data.count > 0)
        {
            LUX_IVS_FD_Alarm();
        }
    } while (0);
    Thread_Mutex_UnLock(&g_facedet_attr.mux);
    usleep(FACEDET_DELAY_TIME);
    return;
}

/**
 * @description: 人脸侦测算法线程函数
 */
PVOID_X face_det_process_thread()
{
    CHAR_X threadName[64] = {0};
    CHAR_X ivsChnName[32] = {0};

    strcpy(ivsChnName, "FACE_DETECTION");
    g_facedet_attr.IsRunning = LUX_TRUE;

    sprintf(threadName, "IVS_%s", ivsChnName);
    prctl(PR_SET_NAME, threadName);
    while (g_facedet_attr.IsRunning)
    {
        if(LUX_TRUE != g_facedet_attr.bStart || g_ivsAttr.bInit != LUX_TRUE || g_framSrcAttr->bInit != LUX_TRUE)
        {  
            usleep(SLEEP_TIME);
            continue;
        }
        else
        {
            LUX_IVS_FD_Process();
        }
    }
    return NULL;
}

/**
 * @description: 人脸侦测算法的初始化函数
 * @return 0成功,非0返回失败返回错误码
 */
INT_X LUX_IVS_FD_AlgoInit()
{
    printf("+++++++++++++================ enter LUX_IVS_FD_AlgoInit\n");

    INT_X ret = LUX_ERR;

    /*创建通道*/
    ret = IMP_IVS_CreateChn(g_facedet_attr.bindAttr.chnNum, g_facedet_attr.interface);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "IMP_IVS_CreateChn(%d) failed\n", g_facedet_attr.bindAttr.chnNum);
        return LUX_IVS_CHAN_CREATE_ERR;
    }
    printf("IMP_IVS_CreateChn[%d] Success\n", g_facedet_attr.bindAttr.chnNum);


    /*通道和ivs组绑定*/
    ret = IMP_IVS_RegisterChn(g_facedet_attr.bindAttr.grpId, g_facedet_attr.bindAttr.chnNum);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "IMP_IVS_RegisterChn(%d, %d) failed\n", g_facedet_attr.bindAttr.grpId, g_facedet_attr.bindAttr.chnNum);
        return LUX_IVS_CHAN_RIGISTER_ERR;
    }
    printf("IMP_IVS_RegisterChn[%d] Success\n", g_facedet_attr.bindAttr.chnNum);

    /*根据配置文件是否使能通道*/
    ret = LUX_IVS_FD_SetConf();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_FD_SetConf failed, return error [0x%x]\n", ret);
        return LUX_IVS_CHAN_RIGISTER_ERR;
    }
    printf("LUX_IVS_FD_SetConf Success\n");

    return LUX_OK;
}

/**
 * @description: 算法的去初始化函数
 * @return 0成功,非0返回失败返回错误码
 */
INT_X LUX_IVS_FD_AlgoExit()
{
    INT_X ret = LUX_ERR;

    g_facedet_attr.IsRunning = LUX_FALSE;

    /* 关闭通道 */
    ret = LUX_IVS_FD_StopChn();
    if (LUX_OK !=  ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_FD_StopChn failed\n");
        return LUX_IVS_STOP_RECIVE_PIC_ERR;
    }

    /* 从ivs通道组注销通道 */
    ret = IMP_IVS_UnRegisterChn(g_facedet_attr.bindAttr.chnNum);
    if (LUX_OK !=  ret)
    {
        IMP_LOG_ERR(TAG, "IMP_IVS_UnRegisterChn(%d) failed\n", g_facedet_attr.bindAttr.chnNum);
        return LUX_IVS_CHAN_UNRIGISTER_ERR;
    }

    /* 销毁通道 */
    ret = IMP_IVS_DestroyChn(g_facedet_attr.bindAttr.chnNum);
    if (LUX_OK !=  ret)
    {
        IMP_LOG_ERR(TAG, "IMP_IVS_DestroyChn(%d) failed\n", g_facedet_attr.bindAttr.chnNum);
        return LUX_IVS_CHAN_DESTROY_ERR;
    }

    return LUX_OK;
}

/**
 * @description: 人脸侦测算法初始化
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_FD_Init()
{
    INT_X ret = LUX_ERR;
    /*检查算法版本*/
    UINT_X facedet_ver = facedet_get_version_info();
    if(facedet_ver != FACEDET_VERSION_NUM)
    {
        printf("facedet_ver: The version numbers of head file and lib do not match, head file version: %08x, lib version: %08x\n", FACEDET_VERSION_NUM, facedet_ver);
        return LUX_ERR;
    }

    Thread_Mutex_Create(&g_facedet_attr.paramMux);
    Thread_Mutex_Create(&g_facedet_attr.mux);

    facedet_param_input_t fdParam;
    /*设置人脸侦测的属性*/
    memset(&fdParam, 0, sizeof(facedet_param_input_t));
    LUX_IVS_FD_SetAttr(&fdParam);

    /*获取人脸侦测算法句柄*/
    g_facedet_attr.interface = FaceDetInterfaceInit(&fdParam);
    if (NULL == g_facedet_attr.interface)
    {
        IMP_LOG_ERR(TAG, "FaceDetInterfaceInit failed\n");
        return LUX_IVS_SET_PARAM_ERR;
    }

    /*创建人脸侦测算法通道、绑定算法组、使能通道*/
    ret = LUX_IVS_FD_AlgoInit();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_FD_AlgoInit failed, return error [0x%x]\n", ret);
        return LUX_ERR;
    }

    /*创建线程处理ivs人脸侦测结果*/
    ret = Task_CreateThread(face_det_process_thread, NULL);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, " pthread_create failed\n");
        return LUX_IVS_CREAT_THREAD_ERR;
    }
    return LUX_OK;
}

/**
 * @description: 人脸侦测算法去初始化
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_FD_Exit()
{
    INT_X ret = LUX_ERR;
    ret = LUX_IVS_FD_AlgoExit();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_FD_AlgoExit failed, return error [0x%x]\n", ret);
        return LUX_ERR;
    }

    FaceDetInterfaceExit(g_facedet_attr.interface);
    Thread_Mutex_Destroy(&g_facedet_attr.mux);
    Thread_Mutex_Destroy(&g_facedet_attr.paramMux);
    return LUX_OK;
}
