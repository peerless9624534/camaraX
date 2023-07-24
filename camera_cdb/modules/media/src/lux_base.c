/*
 * lux_base.c
 *
 * 基础功能、通用功能函数
 * 
 */
#include "lux_base.h"

#include <stdio.h>
#include <time.h>
#include <string.h>

#include <sys/time.h>
#include <sys/prctl.h>
#include "dirent.h"
#include "sys/stat.h"

#include "lux_event.h"
#include "lux_fsrc.h"
#include "lux_nv2jpeg.h"
#include "lux_global.h"

LUX_BASE_IVSQUE_ST g_ivs_que;
OSI_MUTEX g_ivs_que_mux;


//整数转字符串
void app_itoa(int value, char cBuf[8])
{
    char table[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    int i = 0, cnt = 0, count1 = 0;
    char str[10] = {0};

    for (i = 0; i < 8; i++)
    {
        str[i] = table[(((value >> (32 - 4*(i+1)))) & 0xf)];
        if (('0' != str[i]) || count1)
            count1++;
    }

    if (0 == count1)
    {
        strcpy(cBuf, "0");
        return;
    }

    for (i = (8-count1); i < 8; i++)
    {
        sprintf(&cBuf[cnt], "%c", str[i]);
        cnt++;
        if (cnt == count1)
            break;
    }
    return;
}
/**
 * @description: 获取系统精确时间，精确到ns
 * @param [in] wait_ms：获取超时时间
 * @param [out] tp：系统时间
 * @return void
 */
static inline void getTimespec(int wait_ms, struct timespec *tp)
{
    time_t sec, t;
    long long nsec;

    sec = 0;
    if (wait_ms == SEM_NO_WAIT)
    {
        nsec = 0;
    }
    else if (wait_ms < 0)
    {
        nsec = (1000*10000) * 1000000LL; /* 特别大的值，直接修改为等10000s*/
    }
    else
    {
        nsec = wait_ms * 1000000LL;
    }

    if (clock_gettime(CLOCK_REALTIME, tp) == -1)
    {
        tp->tv_sec = time(NULL) + 1;
        tp->tv_nsec = 0;
    }
    else
    {
        t = time(NULL) + 1;
        if ((int)(tp->tv_sec - t) > 30)
        {
            tp->tv_sec = t;
            tp->tv_nsec = 0;
        }
    }

    nsec += tp->tv_nsec;
    if (nsec >= 1000000000)
    {
        sec = nsec / 1000000000;
        nsec = nsec % 1000000000;
    }

    tp->tv_sec += sec;
    tp->tv_nsec = nsec;

    return;
}

/**
 * @description: 创建一个线程,并设置分离属性
 * @param [in] func：需要创建的线程函数
 * @param [in] arg：线程函数输入参数，可以为NULL
 * @return 0：成功，-1：失败；
 */
int Task_CreateThread(void *func, void *arg)
{
    pthread_t threadId;

    if (NULL == func)
    {
        printf("%s:%d null prt!\n", __func__, __LINE__);
        return BASE_COMM_ERR;
    }

    if (0 != pthread_create(&threadId, NULL, func, arg))
    {
        printf("%s:%d pthread_create error!\n", __func__, __LINE__);
        return BASE_COMM_ERR;

    }
    /* 设置线程分离属性 */
    pthread_detach(threadId);

    return 0;
}

/**
 * @description: 创建互斥锁
 * @param [out] pMutex：互斥锁handle
 * @return 0：成功，-1：失败；
 */
int Thread_Mutex_Create(OSI_MUTEX *pMutex)
{
    if (NULL == pMutex)
    {
        printf("%s:%d null ptr!\n", __func__, __LINE__);
        return BASE_COMM_ERR;
    }
    /* 静态初始化 */
    //*pMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_init(pMutex, NULL);

    return 0;
}

/**
 * @description: 销毁互斥锁
 * @param [in] pMutex：handle
 * @return 0：成功，-1：失败；
 */
int Thread_Mutex_Destroy(OSI_MUTEX *pMutex)
{
    if (NULL == pMutex)
    {
        printf("%s:%d null ptr!\n", __func__, __LINE__);
        return BASE_COMM_ERR;
    }

    pthread_mutex_destroy(pMutex);

    return 0;
}

/**
 * @description: 互斥锁加锁
 * @param [in] pMutex：handle
 * @return 0：成功，-1：失败；
 */
int Thread_Mutex_Lock(OSI_MUTEX *pMutex)
{
    if (NULL == pMutex)
    {
        printf("%s:%d null ptr!\n", __func__, __LINE__);
        return BASE_COMM_ERR;
    }

    pthread_mutex_lock(pMutex);
    
    return 0;
}

/**
 * @description: 互斥锁解锁
 * @param [in] pMutex：handle
 * @return 0：成功，-1：失败；
 */
int Thread_Mutex_UnLock(OSI_MUTEX *pMutex)
{
    if (NULL == pMutex)
    {
        printf("%s:%d null ptr!\n", __func__, __LINE__);
        return BASE_COMM_ERR;
    }

    pthread_mutex_unlock(pMutex);
    
    return 0;
}

/**
 * @description: 创建信号量,仅限单进程可用，多进程不可用
 * @param [in] value：创建资源个数，为0可当作互斥锁使用，大于0当作资源锁使用
 * @param [out] pSem：信号量handle
 * @return 0：成功，-1：失败
 */
int Semaphore_Create(unsigned int value, OSI_SEM *pSem)
{
    if (NULL == pSem)
    {
        printf("%s:%d null ptr!\n", __func__, __LINE__);
        return BASE_COMM_ERR;
    }

    /* 0代表只在进程内使用 */
    if (sem_init(pSem, 0, value) != 0)
    {
        printf("%s:%d init error!\n", __func__, __LINE__);
        return BASE_COMM_ERR;
    }

    return 0;
}

/**
 * @description: 销毁信号量
 * @param [in] pSem：信号量handle
 * @return 0：成功，-1：失败
 */
int Semaphore_Destroy(OSI_SEM *pSem)
{
    if (NULL == pSem)
    {
        printf("%s:%d null ptr!\n", __func__, __LINE__);
        return BASE_COMM_ERR;
    }

    if (0 != sem_destroy(pSem))
    {
        printf("%s:%d error!\n", __func__, __LINE__);
        return BASE_COMM_ERR;
    }

    return 0;
}

/**
 * @description: 释放信号量
 * @param [in] pSem：信号量handle
 * @return 0：成功，-1：失败
 */
int Semaphore_Post(OSI_SEM *pSem)
{
    if (NULL == pSem)
    {
        printf("%s:%d null ptr!\n", __func__, __LINE__);
        return BASE_COMM_ERR;
    }

    if (0 != sem_post(pSem))
    {
        printf("%s:%d post error!\n", __func__, __LINE__);
        return BASE_COMM_ERR;
    }

    return 0;
}

/**
 * @description: 获取信号量
 * @param [in] pSem：信号量handle
 * @param [in] waitMs：等待时间，-1：死等，0：立即返回，大于0：超时时间
 * @return 0：成功，-1：失败
 */
int Semaphore_Pend(OSI_SEM *pSem, int waitMs)
{
    struct timespec timeout;

    if (pSem == NULL)
    {
        printf("%s:%d null ptr!\n", __func__, __LINE__);
        return BASE_COMM_ERR;
    }

    /* 等待方式 */
    if (SEM_FOREVER == waitMs)
    {
        if (sem_wait(pSem) != 0)
        {
            printf("%s:%d sem_wait error!\n", __func__, __LINE__);
            return BASE_COMM_ERR;
        }
    }
    else if (SEM_NO_WAIT == waitMs)
    {
        if (sem_trywait(pSem) != 0)
        {
            printf("%s:%d sem_trywait error!\n", __func__, __LINE__);
            return BASE_COMM_ERR;
        }
    }
    else
    {
        getTimespec(waitMs, &timeout);
        if (sem_timedwait(pSem, &timeout) != 0)
        {
            printf("%s:%d sem_timedwait error!\n", __func__, __LINE__);
            return SEM_WAIT_TIMEOUT;
        }
    }

    return 0;
}


int baseTimeZone = 0;
/**
 * @description: 保存时区时间差
 * @param void
 * @return [out] 时间：单位:S
 */
void setTimeStampdDiff(int time_zone)
{
    //if(( 0 == baseTimeZone ) && ( 0 != time_zone ))
    //{
        baseTimeZone = time_zone;
    //}
}
/**
 * @description: 获取时区时间差
 * @param void
 * @return [out] 时间：单位:S
 */
int getTimeStampdDiff(void)
{
    return baseTimeZone;
}

/**
 * @description: 获取当前时间戳
 * @param void
 * @return [out] 时间：单位:S
 */
unsigned int gettimeStampS(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec;
}


/**
 * @description: 获取系统启动时间
 * @param void
 * @return [out] 时间：单位:S
 */
unsigned int getTime_getS(void)
{
    /*
       获取特定时钟的时间，目前定义了6种时钟，分别是:
       CLOCK_REALTIME 系统当前时间，从1970年1月1日算起
       CLOCK_MONOTONIC 系统的启动时间，不能被设置
       CLOCK_PROCESS_CPUTIME_ID 进程运行时间
       CLOCK_THREAD_CPUTIME_ID 线程运行时间
       CLOCK_REALTIME_HR CLOCK_REALTIME的高精度版本
       CLOCK_MONOTONIC_HR CLOCK_MONOTONIC的高精度版本
    */

    struct timespec ts;
    clock_gettime(CLOCK_BOOTTIME, &ts);

    return ((ts.tv_sec) + ts.tv_nsec / 1000000000);
}

/**
 * @description: 获取系统启动时间
 * @param void
 * @return [out] 时间：单位:ms
 */
unsigned int getTime_getMs(void)
{
    struct timespec ts;
    
    clock_gettime(CLOCK_BOOTTIME, &ts);

    return ((ts.tv_sec * 1000) + ts.tv_nsec / 1000000);
}

/**
 * @description: 获取系统启动时间
 * @param void
 * @return [out] 时间：单位:us
 */
unsigned long int getTime_getUs(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_BOOTTIME, &ts);

    return ((ts.tv_sec * 1000000) + ts.tv_nsec / 1000);
}


/******************************************************************
 *                                                                *
 *                         记录录像事件开始时间戳                     *
 *                                                                *
 ******************************************************************/
unsigned int EventTime = 0;

/**
 * @description: 获取EventTime的值，时间戳
 * @return unsigned int
 */
unsigned int GetEventTime()
{
    //printf("%s:%d GetTime:[%d] \n", __func__, __LINE__, EventTime);
    return EventTime;
}

/**
 * @description: 设置当前时间为EventTime的值，时间戳
 * @return void
 */
void SetEventTime()
{
    EventTime = getTime_getMs();
    //printf("%s:%d SetTime:[%d] \n", __func__, __LINE__, EventTime);
}

/******************************************************************
 *                                                                *
 *                         IVS队列基本操作函数                      *
 *                                                                *
 ******************************************************************/

/**
 * @description: 消息队列初始化
 * @return 0 成功，非零失败
 */
int InitQueue()
{
    Thread_Mutex_Create(&g_ivs_que_mux);

    return 0;
}

/**
 * @description: 消息队列长度
 * @param [in] pQue  队列
 * @return 0 成功，非零失败
 */
int QueueLength(LUX_BASE_IVSQUE_ST *pQue)
{
    if(NULL == pQue)
    {
        printf("%s:%d Point NULL!\n", __func__, __LINE__);
        return BASE_COMM_ERR;
    }

    return (pQue->rear - pQue->front + LUX_BASE_QUE_MAXSIZE) % LUX_BASE_QUE_MAXSIZE;
}

/**
 * @description: 消息入队
 * @param [in] pQue  队列
 * @param [in] pInData 出队数据
 * @return 0 成功，非零失败
 */
int PushQueue(LUX_BASE_IVSQUE_ST *pQue, LUX_BASE_IVSDATA_ST *pInData)
{
    if(NULL == pQue || NULL == pInData)
    {
        printf("%s:%d Point NULL!\n", __func__, __LINE__);
        return BASE_COMM_ERR;
    }

    /*判满*/
    if((pQue->rear + 1) % LUX_BASE_QUE_MAXSIZE == pQue->front)
    {
        return 0;
    }

    Thread_Mutex_Lock(&g_ivs_que_mux);
    memcpy(&pQue->IVSDetData[pQue->rear], pInData, sizeof(LUX_BASE_IVSDATA_ST));
    pQue->rear = (pQue->rear + 1) % LUX_BASE_QUE_MAXSIZE;
    Thread_Mutex_UnLock(&g_ivs_que_mux);

    return 0;
}

/**
 * @description: 消息出队
 * @param [in] pQue  队列
 * @param [out] pOutData 出队数据
 * @return 0 成功，非零失败
 */
int PopQueue(LUX_BASE_IVSQUE_ST *pQue, LUX_BASE_IVSDATA_ST *pOutData)
{
    if(NULL == pQue || NULL == pOutData)
    {
        printf("%s:%d Point NULL!\n", __func__, __LINE__);
        return BASE_COMM_ERR;
    }

    /*判空*/
    if(pQue->front == pQue->rear)
    {
        printf("%s:%d Queue is empty !\n", __func__, __LINE__);
        return 0;
    }

    Thread_Mutex_Lock(&g_ivs_que_mux);
    memcpy(pOutData, &pQue->IVSDetData[pQue->front], sizeof(LUX_BASE_IVSDATA_ST));
    memset(&pQue->IVSDetData[pQue->front], 0, sizeof(LUX_BASE_IVSDATA_ST));
    pQue->front = (pQue->front + 1) % LUX_BASE_QUE_MAXSIZE;
    Thread_Mutex_UnLock(&g_ivs_que_mux);

    return 0;
}

LUX_BASE_System_CMD_Ex(const char* pCmd, char* out)
{
    INT_X ret = LUX_OK;
    CHAR_X cCmd[256] = { 0 };
    CHAR_X buffer[512] = { 0 };
    //strncpy(cCmd,256,pCmd);
    snprintf(cCmd, 256, pCmd);
    //printf("cmd: %s\n", pCmd);
    FILE* pp = popen(cCmd, "r");
    if (NULL == pp)
    {
        ret = LUX_ERR;
        printf("LUX_BASE_System_CMD Err(%d)\n", ret);
        return ret;
    }

    while (fgets(buffer, sizeof(buffer), pp) != NULL)
    {
        printf("%s\n", buffer);
        if (out)
        {
            trim_all(buffer);
            strcpy(out, buffer);
        }
    }

    pclose(pp);

    return ret;
}

/* 执行系统指令 */
int LUX_BASE_System_CMD(char *pCmd)
{
    return system(pCmd);
}
#if 0
{
    INT_X ret = LUX_OK;
    CHAR_X cCmd[256] = {0};
    CHAR_X buffer[512] = {0};
    //strncpy(cCmd,256,pCmd);
    snprintf(cCmd, 256, pCmd);
    printf("cmd: %s\n",pCmd);
    FILE *pp = popen(cCmd, "r");
    if (NULL == pp)
    {
        ret = LUX_ERR;
        printf("LUX_BASE_System_CMD Err(%d)\n", ret);
        return ret;
    }

    while(fgets(buffer, sizeof(buffer), pp) != NULL)
    {
        printf("%s\n", buffer);
    }
 
    pclose(pp);

    return ret;
}
#endif

#if 0
/* 检测相关文件的是否与版本匹配 */
int LUX_BASE_Check_File_Vertion()
{
    INT_X ret = LUX_ERR;
    //CHAR_X *fileName = "/system/init/app_wdt";
    CHAR_X *fileInit = "/system/init/app_init.sh";
    
    CHAR_X tmp[128] = {0};
    BOOL_X doReset = true;

    FILE *fp = fopen(fileInit,"r+");
    if(NULL == fp)
    {
        printf("fopen app_init.sh error\n");
        return LUX_ERR;
    }

    while ( NULL != fgets(tmp,sizeof(tmp),fp))
    {
        printf(tmp);
        if(NULL != strstr(tmp,"APP_INIT_VERTION_003"))//包含此字段，则说明文件与版本匹配已经
        {   
            doReset = false;
            break;
        }
    }
    pclose(fp);
    //否则修改app_init.sh文件及/backup中app_init.sh 然后重启设备
#if 0
    if(doReset)
    {
        CHAR_X *fileTmp = "/system/init/File_tmp.sh";
        //修改初始化文件
        fp = fopen(fileTmp,"w+");
        fputs("#!/bin/sh\n"                                                         ,fp);
        fputs("#APP_INIT_VERTION_003\n"                                                 ,fp);
        fputs("mdev -s\n"                                                           ,fp);
        fputs("/system/gpio/gpio_init.sh\n"                                         ,fp);
        fputs("\n"                                                                  ,fp);
        fputs("wpa_supplicant -D nl80211 -i wlan0 -c /system/init/wpa_supplicant.conf -B\n",fp);
        fputs("\n"                                                                  ,fp);
        fputs("/system/init/checkwlan0\n"                                           ,fp);
        fputs("\n"                                                                  ,fp);
        fputs("/system/bin/media_sample > /dev/null 2>&1 &\n"                       ,fp);
        fputs("/system/bin/media_sample save &\n"                                   ,fp);
        fputs("#/system/init/app_wdt &\n"                                           ,fp);

        fclose(fp);
        fp = NULL;

        LUX_BASE_System_CMD("chmod 777 /system/init/File_tmp.sh");
        LUX_BASE_System_CMD("cp /system/init/File_tmp.sh /system/init/app_init.sh");
        LUX_BASE_System_CMD("cp /system/init/File_tmp.sh /system/init/Debug/app_init.sh");

        //修改Debug文件
        fp = fopen(fileTmp,"w+");
        fputs("#!/bin/sh\n"                                                         ,fp);
        fputs("#APP_INIT_VERTION_003\n"                                             ,fp);
        fputs("mdev -s\n"                                                           ,fp);
        fputs("/system/gpio/gpio_init.sh\n"                                         ,fp);
        fputs("\n"                                                                  ,fp);
        fputs("wpa_supplicant -D nl80211 -i wlan0 -c /system/init/wpa_supplicant.conf -B\n",fp);
        fputs("\n"                                                                  ,fp);
        fputs("/system/init/checkwlan0\n"                                           ,fp);
        fputs("\n"                                                                  ,fp);
        fputs("/system/bin/media_sample&\n"                                          ,fp);
        fputs("#/system/bin/media_sample save &\n"                                   ,fp);
        fputs("#/system/init/app_wdt &\n"                                           ,fp);

        fclose(fp);
        fp = NULL;

        LUX_BASE_System_CMD("chmod 777 /system/init/File_tmp.sh");
        LUX_BASE_System_CMD("cp /system/init/File_tmp.sh /system/init/Debug/app_init_debug.sh");


        //修改factory_reset.sh
        fp = fopen(fileTmp,"w+");
        fputs("#!/bin/sh\n"                                                     ,fp);
        fputs("\n"                                                              ,fp);
        fputs("cp /system/init/backup/media_config.ini /system/init/\n"         ,fp);
        fputs("cp /system/init/backup/tuya_config.ini /system/init/\n"          ,fp);
        fputs("cp /system/init/backup/hostapd.conf /system/init/\n"             ,fp);
        fputs("cp /system/init/backup/udhcpd.conf /system/init/\n"              ,fp);
        fputs("cp /system/init/backup/wpa_supplicant.conf /system/init/\n"      ,fp);
        fputs("cp /system/init/Debug/app_init.sh /system/init/app_init.sh\n"    ,fp);
        fputs("\n"                                                              ,fp);
        fputs("rm /system/etc/first_set_wifi\n"                                 ,fp);
        fputs("rm /system/bin/unnormalReboot.txt\n"                             ,fp);
        fputs("\n"                                                              ,fp);
        fputs("rm /system/tuya/*\n"                                             ,fp);
        fputs("rm /system/init/event/*\n"                                       ,fp);
        fclose(fp);
        fp = NULL;
        
        LUX_BASE_System_CMD("chmod 777 /system/init/File_tmp.sh");
        LUX_BASE_System_CMD("cp /system/init/File_tmp.sh /system/init/backup/factory_reset.sh");

        LUX_BASE_System_CMD("rm /system/init/File_tmp.sh");

        LUX_Event_Reboot();
        return LUX_ERR;
    }
#endif
    return LUX_OK;  

}
#endif

/**
 * @description: 抓取高清图片
 * @param [in] picType 抓取照片的
 * @param [out] pPicInfo 抓取的图片数据
 * @return 0 成功，非零失败
 */
int LUX_BASE_CapturePic_HD(LUX_STREAM_CHANNEL_EN picType, LUX_ENCODE_STREAM_ST *pPicInfo)
{
    return LUX_FSrc_DumpNV12_HD(picType,pPicInfo);
}

/**
 * @description: 释放图片
 * @param [in] IvsResult IVS处理结果
 * @return 0 成功，非零失败
 */
int LUX_BASE_ReleasePic_HD(LUX_STREAM_CHANNEL_EN picType ,LUX_ENCODE_STREAM_ST *pPicInfo)
{
    return LUX_Jpeg_FreeStream(picType,pPicInfo);
}

/**
 * @description: 抓取图片
 * @param [in] picType 抓取照片的
 * @param [out] pPicInfo 抓取的图片数据
 * @return 0 成功，非零失败
 */
int LUX_BASE_CapturePic(LUX_STREAM_CHANNEL_EN picType, LUX_ENCODE_STREAM_ST *pPicInfo)
{
    int ret = -1;
    ret = LUX_Video_Encoder_FlushStream(picType);
    if(0 != ret)
    {
        printf("%s:%d LUX_Video_Encoder_FlushStream failed!\n", __func__, __LINE__);
        return -1;
    }

    ret = LUX_Video_Encoder_GetStream(picType, pPicInfo);
    if(0 != ret)
    {
        printf("%s:%d LUX_Video_Encoder_GetStream failed!\n", __func__, __LINE__);
        return -1;
    }

    return 0;
}


/**
 * @description: 释放图片
 * @param [in] IvsResult IVS处理结果
 * @return 0 成功，非零失败
 */
int LUX_BASE_ReleasePic(LUX_STREAM_CHANNEL_EN picType, LUX_ENCODE_STREAM_ST *pPicInfo)
{
    int ret = -1;
    
    ret =  LUX_Video_Encoder_ReleaseStream(picType, pPicInfo);
    if(0 != ret)
    {
        printf("%s:%d LUX_Video_Encoder_ReleaseStream failed!\n", __func__, __LINE__);
        return -1;
    }

    return 0;
}


/******************************************************************
 *                                                                *
 *                         计时器                                  *
 *                                                                *
 ******************************************************************/



typedef struct
{
    int     bTimerUsed;
    int     bTimeStart;         /*计时器开始标志*/
    int     bTSEnd;             /*计时器开始执行时间间隔结束标志*/
    unsigned int lastTime;      /*上一次时间戳*/
    unsigned int curTime;       /*当前时间戳*/
    unsigned int startTime;     /*计时器开始执行时间戳*/

}LUX_BASE_TIMER_INFO;


typedef struct
{
    int     bInit;
    OSI_MUTEX mux;
    LUX_BASE_TIMER_INFO  timerInfo[LUX_BASE_TIEMR_NUM];
    LUX_BASE_TIMER_PARAM timerParam[LUX_BASE_TIEMR_NUM];

}LUX_BASE_TIMER_ATTR;


static LUX_BASE_TIMER_ATTR g_timerAttr;



/**
 * @description: 创建计时器
 * @param [in] pParam 计时器参数
 * @return 成功返回计时器ID, 失败返回-1
 */
int LUX_BASE_TimerCreate(LUX_BASE_TIMER_PARAM *pParam)
{
    if(NULL == pParam)
    {
        printf("%s:%d LUX_BASE_TimerCreate error, empty point!\n", __func__, __LINE__);
        return -1;
    }

    if(NULL == pParam->pTimerProcess)
    {
        printf("%s:%d LUX_BASE_TimerCreate error, invalid funcCB pointer!\n", __func__, __LINE__);
        return -1;
    }

    int timerID = 0;
    int ret = 0;

    Thread_Mutex_Lock(&g_timerAttr.mux);
    do{
        for(timerID = 0; timerID < LUX_BASE_TIEMR_NUM; timerID++)
        {
            if(!g_timerAttr.timerInfo[timerID].bTimerUsed)
            {
                break;
            }
        }

        /*判满*/
        if(LUX_BASE_TIEMR_NUM == timerID)
        {
            printf("%s:%d LUX_BASE_TimerCreat error, The timer is full!\n", __func__, __LINE__);
            ret = -1;
            break;
        }

        memcpy(&g_timerAttr.timerParam[timerID], pParam, sizeof(LUX_BASE_TIMER_PARAM));
        g_timerAttr.timerInfo[timerID].bTimerUsed = 1;
        ret = timerID;
    }while(0);
    Thread_Mutex_UnLock(&g_timerAttr.mux);
    return ret;
}

/**
 * @description: 使能计时器
 * @param [in] timerID 计时器id
 * @return 0 成功，非零失败
 */
int LUX_BASE_TimerStart(int timerID)
{
    if(!g_timerAttr.timerInfo[timerID].bTimerUsed)
    {
        printf("%s:%d LUX_BASE_TimerStart error, The timer has not been created!\n", __func__, __LINE__);
        return -1;
    }

    g_timerAttr.timerInfo[timerID].bTimeStart = 1;

    return 0;
}

/**
 * @description: 销毁计时器
 * @param [in] timerID 计时器id
 * @return 0 成功，非零失败
 */
int LUX_BASE_TimerDestroy(int timerID)
{
    if(timerID < 0)
    {
        printf("%s:%d LUX_BASE_TimerDestroy error, invalid timer ID!\n", __func__, __LINE__);
        return -1;
    }

    Thread_Mutex_Lock(&g_timerAttr.mux);
    memset(&g_timerAttr.timerInfo[timerID], 0, sizeof(LUX_BASE_TIMER_INFO));
    memset(&g_timerAttr.timerParam[timerID], 0, sizeof(LUX_BASE_TIMER_PARAM));
    Thread_Mutex_UnLock(&g_timerAttr.mux);

    return 0;
}

/**
 * @description: 计时器处理线程
 * @param [in] arg 传参
 * @return 0 成功，非零失败
 */
void *lux_base_timer_process(void *arg)
{
    int timerID = 0;
    int CBFuncFlag = 0;
    int destroyFlag = 0;

    prctl(PR_SET_NAME, "TIMER");

    LUX_BASE_TIMER_INFO  *pTimerInfo = NULL;
    LUX_BASE_TIMER_PARAM *pTimerParam = NULL;

    while(g_timerAttr.bInit)
    {
        for(timerID = 0; timerID < LUX_BASE_TIEMR_NUM; timerID++)
        {

            pTimerInfo = &g_timerAttr.timerInfo[timerID];
            pTimerParam = &g_timerAttr.timerParam[timerID];

            Thread_Mutex_Lock(&g_timerAttr.mux);

            if(pTimerInfo->bTimerUsed && pTimerInfo->bTimeStart)
            {
                pTimerInfo->curTime = getTime_getMs();

                /*使能了计时器开始执行时间间隔*/
                if(pTimerParam->bTimeStartEnable && !pTimerInfo->bTSEnd)
                {
                    if(0 == pTimerInfo->startTime)
                    {
                        pTimerInfo->startTime = pTimerInfo->curTime;
                    }

                    if(pTimerInfo->curTime - pTimerInfo->startTime >= pTimerParam->timeStartValue)
                    {
                        if(NULL !=  pTimerParam->pTimerProcess)
                        {
                            CBFuncFlag = 1;
                        }
                        pTimerInfo->bTSEnd = 1;
                        pTimerInfo->lastTime = pTimerInfo->curTime;
                    }
                }
                /*循环执行*/
                else
                {             
                    if(0 == pTimerInfo->lastTime)
                    {
                        pTimerInfo->lastTime = pTimerInfo->curTime;
                    }

                    if(pTimerInfo->curTime - pTimerInfo->lastTime >= pTimerParam->timeInterval)
                    {
                        if(NULL !=  pTimerParam->pTimerProcess)
                        {
                            CBFuncFlag = 1;
                        }

                        //pTimerInfo->lastTime = pTimerInfo->curTime;
                        pTimerInfo->lastTime = 0;
                        
                        /*自动销毁*/
                        if (!pTimerParam->bIntervalEnable)
                        {
                            destroyFlag = 1;
                        }
                    }
                }
            }
            Thread_Mutex_UnLock(&g_timerAttr.mux);

            if(1 == CBFuncFlag)
            {
                pTimerParam->pTimerProcess(pTimerParam->cbArgs);
                CBFuncFlag = 0;
            }
            if(1 == destroyFlag)
            {
                printf("%s:%d timeInterval:%d timerID:%d\n", __func__, __LINE__, pTimerParam->timeInterval, timerID);
                memset(pTimerInfo, 0, sizeof(LUX_BASE_TIMER_INFO));
                memset(pTimerParam, 0, sizeof(LUX_BASE_TIMER_PARAM));
                destroyFlag = 0;
            }
            usleep(10 * 1000);
        }
        usleep(10 * 1000);
    }

    return (void*)0;
}


/**
 * @description: 计时器初始化
 * @return 0 成功，非零失败
 */
int LUX_BASE_TimeInit()
{
    int ret = -1;

    g_timerAttr.bInit = 1;

    Thread_Mutex_Create(&g_timerAttr.mux);

    ret = Task_CreateThread(lux_base_timer_process, NULL);
    if(ret)
    {
        printf("%s:%d Task_CreateThread error!\n", __func__, __LINE__);
        return -1;
    }

    return 0;
}

