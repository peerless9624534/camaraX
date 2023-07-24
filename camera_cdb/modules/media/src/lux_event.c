/*
 * lux_event.c
 *
 * 事件处理中心，报警，事件下发、上传
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/prctl.h>

#include <imp_log.h>

#include "su_base.h"
#include "lux_event.h"
#include "comm_error_code.h"
#include <lux_base.h>
#include <lux_ivs.h>
#include "comm_def.h"
#include <lux_iniparse.h>
#include "lux_fsrc.h"
// #include "lux_sleepDet.h"
// #include "lux_summary.h"
#include "lux_config.h"
#include "lux_global.h"
#define TAG     "lux_event"

#define LUX_EVENT_PD_LIMIT 0.64

extern LUX_BASE_IVSQUE_ST g_ivs_que;
extern OSI_MUTEX g_ivs_que_mux;

BOOL_X recordReboot = 1;  //判断是否为重新上电，1表示重新上电

LUX_EVENT_ATTR_ST   g_eventAttr = {.cloudId = 0XFFFFFFFF};  //初始化无效值


/**
 * @description: 重启设备
 * @return 成功：0，失败：-1；
 */
INT_X LUX_Event_Reboot()
{
    return SU_Base_Reboot();
}

/**
 * @description: 获取板子系统时间
 * @param: [out] pSysTime:系统时间结构
 * @return 成功：0，失败：-1；
 */
INT_X LUX_Event_GetSysTime(LUX_EVENT_SYS_TIME *pSysTime)
{
    if (NULL == pSysTime)
    {
        printf("%s:%d ERROR:null ptr!\n", __func__, __LINE__);
        return LUX_SYS_NULL_PTR;
    }
    INT_X ret = LUX_ERR;

    ret = SU_Base_GetTime((SUTime*)pSysTime);
    if (0 != ret)
    {
        printf("%s:%d SU_Base_GetTime ERROR ret:%#x\n", __func__, __LINE__, ret);
        ret = LUX_SYS_INVALID_PARM;
    }

    return ret;
}

/**
 * @description: 设置系统时间.
 * @param: [out] pSysTime:系统时间结构
 * @return 成功：0，失败：-1
 */
INT_X LUX_Event_SetSysTime(LUX_EVENT_SYS_TIME *pSysTime)
{
    if (NULL == pSysTime)
    {
        printf("%s:%d SU_Base_SetTime ERROR NULL PTR\n", __func__, __LINE__);
        return LUX_SYS_NULL_PTR;
    }
    INT_X ret = LUX_OK;
    CHAR_X tmp[32] = {0};

#if 0
    SUTime time;
    memcpy(&time, pSysTime, sizeof(SUTime));
    ret = SU_Base_SetTime(&time);
    if (0 != ret)
    {
        printf("%s:%d SU_Base_SetTime ERROR ret:%#x[%d-%d-%d %d:%d:%d]\n", 
                __func__, __LINE__, ret, time.year, time.mon, time.mday, time.hour, time.min, time.sec);
        ret = LUX_SYS_INVALID_PARM;
    }
#endif
    sprintf(tmp, "date -s \"%d-%d-%d %d:%d:%d\"", pSysTime->year, pSysTime->mon, pSysTime->mday, pSysTime->hour, pSysTime->min, pSysTime->sec);
    //system(tmp);
    LUX_BASE_System_CMD(tmp);

    printf("Time Success Time Stampd: %d hours %d minutes\n", (getTimeStampdDiff() / 3600) ,((getTimeStampdDiff() /60) % 60));
    return ret;
}


/**
 * @description: 移动侦测事件处理函数
 * @param [in] pFuncCBAttr 事件上报回调函数
 */
INT_X LUX_EVENT_IvsRegistCB(LUX_EVENT_ATTR_ST *pFuncCBAttr)
{

    if(NULL == pFuncCBAttr)
    {
        printf("%s:%d LUX_EVENT_IvsRegistCB failed, empty pointer\n", __func__, __LINE__);
        return LUX_ERR;
    }

    if(NULL == pFuncCBAttr->pEvntReportFuncCB || NULL == pFuncCBAttr->pStartEventRecordFuncCB || NULL == pFuncCBAttr->pStopEventRecordFuncCB || NULL == pFuncCBAttr->pGetRecordstatus
        || NULL == pFuncCBAttr->pAddCloudEvent || NULL == pFuncCBAttr->pDeletCloudEvent || NULL == pFuncCBAttr->pGetCloudEventStatus)
    {
        printf("%s:%d LUX_EVENT_IvsRegistCB failed, empty funcCB pointer\n", __func__, __LINE__);
        return LUX_ERR;
    }

    memcpy(&g_eventAttr, pFuncCBAttr, sizeof(LUX_EVENT_ATTR_ST));

    return LUX_OK;
}

/**
 * @description: 停止事件录像回调函数
 */
static void *LUX_Base_stopEventRecordCB(void *arg)
{
    //事件录制时间大于30秒，并且录制状态为未停止
    unsigned int NowTime = getTime_getMs();
    unsigned int EventTime = GetEventTime();
    //printf("%s:%d NowTime:[%d] \n", __func__, __LINE__, NowTime);
    //printf("%s:%d EventTime:[%d] \n", __func__, __LINE__, EventTime);
    if(NowTime - EventTime >= LUX_COMM_EVENT_RECORD_TIME) 
    {
        if(LUX_RECOR_STOP != (LUX_RECOR_STATUS_E)g_eventAttr.pGetRecordstatus())
        {
            g_eventAttr.pStopEventRecordFuncCB();
        }
    }
    return (void *)0;
}

/**
 * @description: 侦测事件录像
 * @param [in] ts 录像时长,单位ms
 * @return 0 成功，非零失败
 */
INT_X LUX_Event_Record(UINT_X ts)
{   
    INT_X ret = LUX_ERR;
    INT_X timerID = LUX_ERR;
    LUX_BASE_TIMER_PARAM RecordParam;

    /*记录当前事件时间戳*/
    SetEventTime();
    GetEventTime();
    RecordParam.bIntervalEnable = 0;
    RecordParam.pTimerProcess = LUX_Base_stopEventRecordCB;
    RecordParam.cbArgs = NULL;
    RecordParam.timeInterval  = ts;

    timerID = LUX_BASE_TimerCreate(&RecordParam);
    if(LUX_ERR == timerID)
    {
        IMP_LOG_ERR(TAG, " LUX_BASE_TimerCreate failed\n");
        return -1;
    }
    /*录制状态为停止时开始录制*/
    if (LUX_RECOR_STOP == (LUX_RECOR_STATUS_E)g_eventAttr.pGetRecordstatus())
    {   
        g_eventAttr.pStartEventRecordFuncCB();
    }

    ret = LUX_BASE_TimerStart(timerID);
    if(LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, " LUX_BASE_TimerStart failed\n");
        return -1;
    }

    return LUX_OK;
}

int LUX_Event_GetAlarmCbk(LUX_EVENT_ATTR_ST *pEventCbk)
{
    if (NULL == pEventCbk)
    {
        printf("%s:%d null prt!\n", __func__, __LINE__);
        return -1;
    }

    memcpy(pEventCbk, &g_eventAttr, sizeof(LUX_EVENT_ATTR_ST));

    return 0;
}

/**
 * @description: 人形侦测事件处理函数
 * @param [in] IvsResult IVS处理结果
 * @return 0 成功，非零失败
 */
static INT_X LUX_Event_PD_Process(LUX_BASE_IVSDATA_ST *PDResult)
{
    INT_X ret = LUX_ERR;
    INT_X peopleNum = 1;
    LUX_ENCODE_STREAM_ST picInfo[2];
    BOOL_X recordOnOff = 0;
    INT_X  recordMode = 0;
    INT_X i;
    BOOL_X findPerson = 0;

    if(LUX_TRUE != PDResult->ivsStart[LUX_IVS_PERSON_DET_EN])
    {
        return 0;
    }
    if(NULL == PDResult)
    {
        printf("%s:%d LUX_Event_PD_Process failed, empty PDResult\n", __func__, __LINE__);
        return LUX_ERR;
    }

    if(PDResult->rectcnt >= peopleNum)
    {
        for (i = 0; i < PDResult->rectcnt; i++)
        { 
            if(PDResult->person[i].confidence > LUX_EVENT_PD_LIMIT)
            {
                //printf("%s:%d confidence:%f \n", __func__, __LINE__, PDResult->person[i].confidence);
                findPerson = 1;
            }
        }
    }
    /*判断置信度 && 时间间隔*/
    if (findPerson && LUX_IVS_GetReportEvent_Countdown())
    {
        //LUX_SUM_Update_Event(LUX_SUM_EVENT_PERSON_DET ,gettimeStampS(),NULL);

        ret = LUX_BASE_CapturePic_HD(LUX_STREAM_SUB,&picInfo[LUX_STREAM_SUB]);
        if(LUX_OK != ret)
        {
            LUX_BASE_ReleasePic_HD(LUX_STREAM_SUB,&picInfo[LUX_STREAM_SUB]);
            IMP_LOG_ERR(TAG, " LUX_BASE_CapturePic failed\n");
            return LUX_ERR;
        }
        /*上报抓图*/
        if(NULL != g_eventAttr.pEvntReportFuncCB)
        {                                                                   //LUX_NOTIFICATION_NAME_HUMAN
            g_eventAttr.pEvntReportFuncCB(picInfo[LUX_STREAM_SUB].pAddr, picInfo[LUX_STREAM_SUB].streamLen, LUX_NOTIFICATION_NAME_MOTION);
        }

        /* 云存储 */
        if (NULL != g_eventAttr.pAddCloudEvent && NULL != g_eventAttr.pDeletCloudEvent && NULL != g_eventAttr.pGetCloudEventStatus)
        {

            //if (g_eventAttr.pGetCloudEventStatus(g_eventAttr.cloudId))
            g_eventAttr.cloudId = g_eventAttr.pAddCloudEvent((char *)picInfo[LUX_STREAM_SUB].pAddr, picInfo[LUX_STREAM_SUB].streamLen, LUX_EVENT_TYPE_MOTION_DETECT, LUX_CLOUD_STORAGE_TIME); //最长录300s
            printf("%s:%d cloudId:%d \n", __func__, __LINE__, g_eventAttr.cloudId);
        }
        LUX_BASE_ReleasePic_HD(LUX_STREAM_SUB,&picInfo[LUX_STREAM_SUB]);

        /*事件录像*/
        // ret = LUX_INIParse_GetBool(LUX_CONFIG_FILE, "record", "record_onoff", &recordOnOff);
        // if (LUX_OK != ret)
        // {
        //     IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
        // }

        // ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "record", "record_mode", &recordMode);
        // if (LUX_OK != ret)
        // {
        //     IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
        // }
        
        if(LUX_TRUE == g_configData->record_config.record_onoff && 0 == g_configData->record_config.record_mode)
        {   
            ret = LUX_Event_Record(LUX_COMM_EVENT_RECORD_TIME);
            if (LUX_OK != ret)
            {
                IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
            }
        }
        
    }
        
    
    
    return LUX_OK;
}

/**
 * @description: IVS智能模块事件处理函数
 * @param [in] IvsResult IVS处理结果
 * @return 0 成功，非零失败，返回错误码
 */
static INT_X LUX_Event_IVSProcess(LUX_BASE_IVSDATA_ST *IvsResult)
{
    INT_X ret = LUX_ERR;

    if(NULL == IvsResult)
    {
        printf("%s:%d LUX_Event_IVSProcess failed, empty IvsResult\n", __func__, __LINE__);
        return LUX_ERR;
    }
    
    switch(IvsResult->type)
    {
        case LUX_BASE_PERSONDET_QUE_EN:
            ret = LUX_Event_PD_Process(IvsResult);
            if(LUX_OK != ret)
            {
                IMP_LOG_ERR(TAG, "LUX_Event_PDIVS_Process failed\n");
                return LUX_ERR;
            }
            break;

        default:
                IMP_LOG_ERR(TAG, "process IVS event error, not support the event!\n");
                return LUX_ERR;
    }

    return LUX_OK;
}

/**
 * @description: IVS智能模块抓图处理线程
 * @return 0 成功，非零失败，返回错误码
 */
static void *lux_event_ivs_process_thread(void* arg)
{
    LUX_BASE_IVSDATA_ST getIvsResult;
    INT_X ret = LUX_ERR;

    prctl(PR_SET_NAME, "IVS_EVENT_PROCESS");

    while(1)
    {
        if(0 == QueueLength(&g_ivs_que))
        {
            usleep(100 * 1000);
            continue;
        }

        ret = PopQueue(&g_ivs_que, &getIvsResult);

        /*分类处理事件*/
        ret = LUX_Event_IVSProcess(&getIvsResult);
        if(LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, " LUX_Event_IVSProcess failed\n");
        }
        usleep(10 * 1000);
    }

    return (void*)LUX_OK;
}


/**
 * @description: IVS智能模块抓图初始化
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_EVENT_IVSProcess_Init()
{
    INT_X ret = LUX_ERR;

    ret = Task_CreateThread(lux_event_ivs_process_thread, NULL);
    if(LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, " pthread_create failed\n");
		return LUX_ERR;
    }

    return LUX_OK;
}

CHAR_X s_alarm_interval_x[4] = {0};

static INT_X __tuya_app_read_STR_X(CHAR_X *key, CHAR_X *value, INT_X value_size)
{
    CHAR_X tmp[64] = {0};
    INT_X ret = 0;

    ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "algorithm_config", key, tmp);

    if (0 != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_INIParse_GetInt failed,  key[%s]!, error num [0x%x] ", key, ret);
    }

    memset(value, 0, value_size);
    if (strlen(tmp) >= value_size)
    {
        strncpy(value, tmp, value_size - 1);
    }
    else
    {
        strcpy(value, tmp);
    }

    return 0;
}

static CHAR_X *IPC_APP_get_alarm_interval_X(VOID_X)
{
    /* Motion detection alarm interval,unit is minutes,ENUM type,"0","1","2"*/
    __tuya_app_read_STR_X("alarm_interval",s_alarm_interval_x, 4);
    printf("%s:%d curr alarm_interval:%s \r\n", __func__, __LINE__, s_alarm_interval_x);

   return s_alarm_interval_x;
}
LUX_IVS_TIMEINTERVAL_EN g_alarmInterval; /* 报警时间间隔 */

BOOL_X LUX_Event_IsAlarmByInterval(UINT_X lastTime)
{
    UINT_X timeInterval = 0;
    UINT_X curTime = 0;
    BOOL_X bAlarm = 0;

    if (1 == recordReboot)  //重新上电读取配置文件值
    {
        CHAR_X *p_getalarmInterval = IPC_APP_get_alarm_interval_X();
        g_alarmInterval = (LUX_IVS_TIMEINTERVAL_EN)(*p_getalarmInterval - '0');
        recordReboot = 0;
    }

    switch (g_alarmInterval)
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
        bAlarm = 1;
    }
    //printf("INFO: %s:%d timeInterval:%d curTime:%d lastTime:%d-%d bAlarm:%d\n",
    //       __func__, __LINE__, timeInterval, curTime, lastTime, *pLastTime, bAlarm);

    return bAlarm;
}
