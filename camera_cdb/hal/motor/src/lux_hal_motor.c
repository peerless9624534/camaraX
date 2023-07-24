#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <sys/ioctl.h>
#include <sys/time.h>
#include <signal.h>

#include <lux_hal_motor.h>
#include <lux_iniparse.h>
#include <lux_base.h>
#include <imp_log.h>
#include <imp_ivs.h>
#include <ivs_inf_person_tracker.h>
#include "lux_global.h"
#include "lux_config.h"
#define FORWARD    -1
#define BACKWARD    1
#define MOV_ONE_STEP_VERTICAL    100
#define MOV_ONE_STEP_HORIZONTAL  600
#define MOVE_ONE_STEP(a, b) ((a) * (b))
#define TIMING_WHOLD_DAY_SECONDS  (24 * 60 * 60)
#define CRUISE_TIMER_NUMBER     2

#define TAG "MOTOR"

typedef struct
{
    int     bInit;                              /*初始化状态*/
    OSI_MUTEX   mux;                            /*线程锁*/
    int     motorFd;                            /*电机驱动文件描述符*/
    LUX_HAL_MOTOR_MSG_ST motorStatus;           /*电机状态信息*/
    LUX_HAL_MOTOR_RESET_DATA_ST motorRestData;  /*电机复位信息*/
    unsigned long speed;                        /*电机初始化速度*/
    unsigned long cruiseSpeed;                  /*巡航速度*/
    LUX_HAL_MOTOR_MODE_EN mode;                 /*电机运动模式*/
    int     cruiseTimeID[CRUISE_TIMER_NUMBER];  /*时间段巡航计时器*/
    int     x;                                  /*存档用的x值*/
    int     y;                                  /*存档用的y值*/

}LUX_HAL_MOTOR_ATTR_ST;


LUX_HAL_MOTOR_ATTR_ST g_motorAttr =
{
    .motorFd = -1,
    .motorRestData =
    {
        .x_cur_step  = 0,
        .y_cur_step  = 0,
        .x_max_steps = HORIZONE_STEPS_MAX,
        .y_max_steps = VERTICAL_STEPS_MAX,
    },
    .speed = 900,
    .mode = LUX_HAL_MOTOR_MODES_STOP,
    .cruiseTimeID = {-1, -1},
    .x = 5200,
    .y = 620,

};


/**
 * @description: 获取垂直方向电机应移动距离
 * @param [in] yDistance     电机水平方向运动距离
 * @return 
 */
int LUX_HAL_MOTOR_GetVerticalDis(int yDistance)
{
    if(MOV_ONE_STEP_VERTICAL  < VERTICAL_STEPS_MAX - abs(yDistance))/*防止垂直方向运动超过限制*/
    {
        return yDistance;
    }
    else                    
    {
        if(yDistance > 0)
        {
            return VERTICAL_STEPS_MAX - abs(yDistance);
        }
        else
        {
            return -(VERTICAL_STEPS_MAX - abs(yDistance));  
        }
    }  
}

/**
 * @description: 电机移动控制
 * @param [in] direction  电机运动方向
 * @param [in] ydistance   竖直步长
 * @param [in] xdistance   水平步长
 * @return 0 成功， 非零失败
 */
static int LUX_HAL_Motor_CtrlMov(LUX_HAL_MOTOR_DIRECTION_EN direction, int yDistance, int xDistance)
{
    int speed = MOV_ONE_STEP_SPEED;
    int ret = LUX_ERR;
    int x = 0;
    int y = 0;
    LUX_HAL_MOTOR_MSG_ST motorStatus;
    char tmp[16] = {'\0'};

    /*获取电机状态*/
    memset (&g_motorAttr.motorStatus, 0 ,sizeof(LUX_HAL_MOTOR_MSG_ST));
    memset (&motorStatus, 0 ,sizeof(LUX_HAL_MOTOR_MSG_ST));

    /*检测电机的状态*/
    //if(MOTOR_IS_RUNNING == g_motorAttr.motorStatus.status)
    //{
        //printf("%s:[%d] Motor is running, control moving error!\n", __FUNCTION__, __LINE__);
        //return -1;
    //}

    switch(direction)
    {
        case LUX_HAL_MOTOR_MOVE_VERTICAL:
            g_motorAttr.motorStatus.x = 0;
            g_motorAttr.motorStatus.y = LUX_HAL_MOTOR_GetVerticalDis( yDistance);
            break;

        case LUX_HAL_MOTOR_MOVE_HORIZONE:
            g_motorAttr.motorStatus.y = 0;
            g_motorAttr.motorStatus.x = xDistance;  
            break;

        case LUX_HAL_MOTOR_MOVE_OBLIQUE:
            g_motorAttr.motorStatus.x = xDistance;
            g_motorAttr.motorStatus.y = LUX_HAL_MOTOR_GetVerticalDis(yDistance);    
            break;

       default:
            printf("%s:[%d] LUX_HAL_Motor_CtrlMov error, invalid param!\n", __FUNCTION__, __LINE__);
            return -1;
    }
    
    ioctl(g_motorAttr.motorFd, MOTOR_SPEED, (unsigned long)&speed);
    ioctl(g_motorAttr.motorFd, MOTOR_MOVE, (unsigned long)&g_motorAttr.motorStatus);

    //检查设备状态
    ioctl (g_motorAttr.motorFd, MOTOR_GET_STATUS, &motorStatus);

    printf("motorStatus.x %d\n",motorStatus.x);
    printf("motorStatus.y %d\n",motorStatus.y);
    printf("motorStatus.starus %d\n",motorStatus.status);
    printf("motorStatus.speed %d\n",motorStatus.speed);
    //修改电机状态
    x = motorStatus.x + xDistance;
    y = motorStatus.y + yDistance;
    if(HORIZONE_STEPS_MAX < x ) x = HORIZONE_STEPS_MAX;
    if(VERTICAL_STEPS_MAX < y) y = VERTICAL_STEPS_MAX;
    if(0 > x) x = 0;
    if(0 > y) y = 0;
    
    if(x != g_motorAttr.x)
    {
        g_motorAttr.x = x;
        sprintf(tmp, "%d", g_motorAttr.x);
        ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "algorithm_config", "MOTOR_X", tmp);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
            return -1;
        }

    }
    if(y != g_motorAttr.y)
    {
        g_motorAttr.y = y;
        sprintf(tmp, "%d", g_motorAttr.y);
        ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "algorithm_config", "MOTOR_Y", tmp);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
            return -1;
        }
    }

    return 0;
}

static int LUX_HAL_Motor_OpenDev()
{
    g_motorAttr.motorFd = open("/dev/motor", 0);
    if (g_motorAttr.motorFd <= 0)
    {
        printf ("%s:[%d]open motor dev is error\n", __FUNCTION__, __LINE__);
        return -1;
    }

    return 0;
}

/**
 * @description: 电机移动方向判断
 * @param [in] direction  电机运动方向
 * @return 0 成功， 非零失败
 */
int LUX_HAL_Motor_CtrlMovOneStep(LUX_HAL_MOTOR_MOV_DIRECTION_EN direction)
{
    if(!g_motorAttr.bInit)
    {
        printf("%s:[%d] Error, motor control hasn't been inited!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    int sleepMode = 0;
    int ret = 0;
    int oneVerticalStepSize = MOVE_ONE_STEP(FORWARD, MOV_ONE_STEP_VERTICAL);      /*垂直*/
    int oneHorizoneStepSize = MOVE_ONE_STEP(BACKWARD, MOV_ONE_STEP_HORIZONTAL);   /*水平*/

    LUX_HAL_MOTOR_DIRECTION_EN oneStepDirection = LUX_HAL_MOTOR_MOVE_HORIZONE;

    Thread_Mutex_Lock(&g_motorAttr.mux);
    do{
        /*在隐私模式下不进行电机转动*/
        LUX_INIParse_GetInt(LUX_CONFIG_FILE, "algorithm_config", "privacy_mask", &sleepMode);
        if (1 == g_configData->alg_config.sleepMode)
        {
            printf("%s:[%d] LUX_HAL_Motor_CtrlMovOneStep in sleepMode error!\n", __FUNCTION__, __LINE__);
            ret = 0;
            break;
        }

        switch (direction)
        {
            case LUX_HAL_MOTOR_UP:
                oneVerticalStepSize = MOVE_ONE_STEP(FORWARD, MOV_ONE_STEP_VERTICAL);
                oneHorizoneStepSize = 0;
                oneStepDirection = LUX_HAL_MOTOR_MOVE_VERTICAL; 
                break;

            case LUX_HAL_MOTOR_UP_RIGHT:
                oneVerticalStepSize = MOVE_ONE_STEP(FORWARD, MOV_ONE_STEP_VERTICAL);
                oneHorizoneStepSize = MOVE_ONE_STEP(BACKWARD, MOV_ONE_STEP_HORIZONTAL);
                oneStepDirection = LUX_HAL_MOTOR_MOVE_OBLIQUE;
                break;

            case LUX_HAL_MOTOR_UP_LEFT:
                oneVerticalStepSize = MOVE_ONE_STEP(FORWARD, MOV_ONE_STEP_VERTICAL); 
                oneHorizoneStepSize = MOVE_ONE_STEP(FORWARD, MOV_ONE_STEP_HORIZONTAL);
                oneStepDirection = LUX_HAL_MOTOR_MOVE_OBLIQUE;
                break;

            case LUX_HAL_MOTOR_LEFT:
                oneVerticalStepSize = 0;   
                oneHorizoneStepSize = MOVE_ONE_STEP(FORWARD, MOV_ONE_STEP_HORIZONTAL);
                oneStepDirection = LUX_HAL_MOTOR_MOVE_HORIZONE; 
                break;

            case LUX_HAL_MOTOR_RIGHT:
                oneVerticalStepSize = 0;   
                oneHorizoneStepSize = MOVE_ONE_STEP(BACKWARD, MOV_ONE_STEP_HORIZONTAL);          
                oneStepDirection = LUX_HAL_MOTOR_MOVE_HORIZONE; 
                break;

            case LUX_HAL_MOTOR_DOWN:
                oneVerticalStepSize = MOVE_ONE_STEP(BACKWARD, MOV_ONE_STEP_VERTICAL);   
                oneHorizoneStepSize = 0;       
                oneStepDirection = LUX_HAL_MOTOR_MOVE_VERTICAL; 
                break;

            case LUX_HAL_MOTOR_DOWN_RIGHT:
                oneVerticalStepSize = MOVE_ONE_STEP(BACKWARD, MOV_ONE_STEP_VERTICAL);   
                oneHorizoneStepSize = MOVE_ONE_STEP(BACKWARD, MOV_ONE_STEP_HORIZONTAL);           
                oneStepDirection = LUX_HAL_MOTOR_MOVE_OBLIQUE; 
                break;

            case LUX_HAL_MOTOR_DOWN_LEFT:
                oneVerticalStepSize = MOVE_ONE_STEP(BACKWARD, MOV_ONE_STEP_VERTICAL);   
                oneHorizoneStepSize = MOVE_ONE_STEP(FORWARD, MOV_ONE_STEP_HORIZONTAL);             
                oneStepDirection = LUX_HAL_MOTOR_MOVE_OBLIQUE;
                break;

            default:
                printf("%s:[%d] LUX_HAL_Motor_CtrlMovOneStep set error, invalid param!\n", __FUNCTION__, __LINE__);
                ret = -1;
                break;
        }

        if(ret)
        {
            break;
        }

        ret = LUX_HAL_Motor_CtrlMov(oneStepDirection, oneVerticalStepSize, oneHorizoneStepSize);
        if (0 != ret)
        {
            printf("%s:[%d] LUX_HAL_Motor_CtrlMovOneStep error!\n", __FUNCTION__, __LINE__);
            ret = -1;
            break;
        }
    }while(0);
    Thread_Mutex_UnLock(&g_motorAttr.mux);

    return ret;
}

/**
 * @description: 解析时间
 * @return 0 成功， 非零失败
 */
static int LUX_HAL_MOTOR_Parse_CruiseTime(const char *pCTime, int *pOutTValue)
{
    if(NULL == pCTime || NULL == pOutTValue)
    {
        printf("%s:[%d] error, empty point!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    int i = 0, j = 0;
    int isMin = 0;
    char hourBuf[3] = {0};
    char minBuf[3] = {0};

    while(pCTime[i] != '\0')
    {
        if(' ' == pCTime[i])
        {
            i++;
            continue;
        }

        if(':' == pCTime[i])
        {
            isMin = 1;
            j = 0;
            i++;
            continue;
        }

        if(0 == isMin)
        {
            hourBuf[j++] = pCTime[i];
        }
        else
        {
            minBuf[j++] = pCTime[i];
        }

        i++;
    }

    hourBuf[2] = '\0';
    minBuf[2] = '\0';
    *pOutTValue = (atoi(hourBuf) * 60 + atoi(minBuf)) * 60;

    return 0;
}

/**
 * @description: 停止电机转动
 * @return 0 成功， 非零失败
 */
int LUX_HAL_MOTOR_Stop()
{

    if(MOTOR_IS_STOP == g_motorAttr.motorStatus.status)
    {
        printf("%s:[%d] Motor has stopped, there's no need to stop!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    Thread_Mutex_Lock(&g_motorAttr.mux);
    ioctl(g_motorAttr.motorFd, MOTOR_STOP, 0);
    g_motorAttr.motorStatus.status = MOTOR_IS_STOP;
    g_motorAttr.mode = LUX_HAL_MOTOR_MODES_STOP;
    Thread_Mutex_UnLock(&g_motorAttr.mux);
  
    return 0;
}


/**
 * @description: PTZ巡航
 * @param [int]  speed  速度
 * @return 0 成功， 非零失败
 */
static int LUX_HAL_MOTOR_Cruise(int speed)
{
    if(0 == g_motorAttr.bInit)
    {
        printf ("%s:[%d]motor init is error\n", __FUNCTION__, __LINE__);
        return -1;
    }

    /*检测电机的状态*/
    if(MOTOR_IS_RUNNING == g_motorAttr.motorStatus.status)
    {
        printf("%s:[%d] Motor is running, control moving error!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    Thread_Mutex_Lock(&g_motorAttr.mux);

    ioctl(g_motorAttr.motorFd, MOTOR_SPEED, &speed);
    ioctl(g_motorAttr.motorFd, MOTOR_CRUISE, NULL);
    g_motorAttr.motorStatus.status = MOTOR_IS_RUNNING;
    g_motorAttr.mode = LUX_HAL_MOTOR_MODES_CRUISE;

    Thread_Mutex_UnLock(&g_motorAttr.mux);
    return 0;

}

/**
 * @description: 设置PTZ巡航模式,巡航结束处理函数
 * @param [in]  arg  信号编号
 * @return 0 成功， 非零失败
 */
static void *LUX_HALT_MOTOR_StopCruiseHanler(void *arg)
{
    int ret = -1;
    ret = LUX_HAL_MOTOR_Stop();
    if(0 != ret)
    {
        printf("%s:[%d] LUX_HAL_MOTOR_Cruise Error!\n", __FUNCTION__, __LINE__);
    }

    /*恢复原始速度*/
    ioctl(g_motorAttr.motorFd, MOTOR_SPEED, &g_motorAttr.speed);

    return (void *)0;
}

/**
 * @description: 设置PTZ巡航模式定时处理函数
 * @param [in]  arg  参数
 * @return 0 成功， 非零失败
 */
static void *LUX_HALT_MOTOR_StartCruiseHanler(void *arg)
{
    int ret = -1;

    ret = LUX_HAL_MOTOR_Cruise(g_motorAttr.cruiseSpeed);
    if(0 != ret)
    {
        printf("%s:[%d] LUX_HAL_MOTOR_Cruise Error!\n", __FUNCTION__, __LINE__);
    }

    return (void *)0;
}

/**
 * @description: 设置PTZ巡航模式
 * @param [in]  speed  速度
 * @return 0 成功， 非零失败
 */
int LUX_HAL_MOTOR_SetCruiseMode(LUX_HAL_MOTOR_CRUISE_EN cruiseMode, int speed)
{       
    if(cruiseMode  < LUX_HAL_MOTOR_CRUISE_ALLDAY ||  cruiseMode >= LUX_HAL_MOTOR_CRUISE_BOTTOM)
    {
        printf("%s:[%d] Error, invalid param!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    int ret = -1;
    int i = 0;
    char tStartBuf[64] = {0};
    char tEndBbuf[64] = {0};
    int t_start = 0;
    int t_end = 0;
    int curTime = 0;
    int tStartValue = 0;
    int timeID = -1;
    time_t rawtime = {0};
    struct tm *info = NULL;
    LUX_BASE_TIMER_PARAM TimerParam = {0};

    time(&rawtime);
    g_motorAttr.cruiseSpeed = speed;

    /*全天巡航*/
    if(LUX_HAL_MOTOR_CRUISE_ALLDAY == cruiseMode)
    {
        /*取消定时模式下的定时器*/
        for(i = 0; i < CRUISE_TIMER_NUMBER; i++)
        {
            if( -1 != g_motorAttr.cruiseTimeID[i])
            {
                ret =  LUX_BASE_TimerDestroy(g_motorAttr.cruiseTimeID[i]);
                if(0 != ret)
                {
                    printf("%s:[%d] LUX_BASE_TimerDestroy Error!\n", __FUNCTION__, __LINE__);
                    return -1;
                }
            }
            g_motorAttr.cruiseTimeID[i] = -1;
        }

        ret = LUX_HAL_MOTOR_Cruise(speed);
        if(0 != ret)
        {
         printf("%s:[%d] LUX_HAL_MOTOR_Cruise Error!\n", __FUNCTION__, __LINE__);
         return -1;
        }
    }
    /*定时巡航*/
    else if(LUX_HAL_MOTOR_CRUISE_TIMING == cruiseMode)
    {
        if(MOTOR_IS_STOP != g_motorAttr.motorStatus.status)
        {
            ret = LUX_HAL_MOTOR_Stop();
            if(0 != ret)
            {
                printf("%s:[%d] LUX_HAL_MOTOR_Cruise Error!\n", __FUNCTION__, __LINE__);
            }
        }

        /*读取定时巡航时间并换算*/
        info = localtime(&rawtime);
        curTime = ((info->tm_hour * 60 + info->tm_min) * 60) + info->tm_sec;

        //printf("hour = %d, min = %d, sec = %d\n", info->tm_hour, info->tm_min, info->tm_sec);

        LUX_INIParse_GetString(LUX_CONFIG_FILE, "motor", "patrol_time_start", tStartBuf);

        ret = LUX_HAL_MOTOR_Parse_CruiseTime(tStartBuf, &t_start);
        if(0 != ret)
        {
            printf("%s:[%d] LUX_HAL_MOTOR_Parse_CruiseTime Error!\n", __FUNCTION__, __LINE__);
            return -1;
        }

        LUX_INIParse_GetString(LUX_CONFIG_FILE, "motor", "patrol_time_end", tEndBbuf);

        ret = LUX_HAL_MOTOR_Parse_CruiseTime(tEndBbuf, &t_end);
        if(0 != ret)
        {
            printf("%s:[%d] LUX_HAL_MOTOR_Parse_CruiseTime Error!\n", __FUNCTION__, __LINE__);
            return -1;
        }

        if(t_start >= t_end)
        {
            printf("%s:[%d] Get CruiseTime Error!\n", __FUNCTION__, __LINE__);
            return -1;
        }

        /*计算开始时间间隔*/
        if(t_start < curTime && t_end > curTime)
        {
            tStartValue = 0;
        }
        else if(t_start < curTime && t_end < curTime)
        {
            tStartValue = TIMING_WHOLD_DAY_SECONDS - (curTime - t_end);
        }
        else
        {
            tStartValue =  t_start - curTime;
        }

        /*创建开始循环计时器*/
        TimerParam.bTimeStartEnable = 1;
        TimerParam.timeStartValue = tStartValue * 1000;
        TimerParam.bIntervalEnable = 1;
        TimerParam.timeInterval = TIMING_WHOLD_DAY_SECONDS * 1000;
        TimerParam.cbArgs = NULL;
        TimerParam.pTimerProcess = LUX_HALT_MOTOR_StartCruiseHanler;

        timeID = LUX_BASE_TimerCreate(&TimerParam);
        if(timeID < 0)
        {
            printf("%s:[%d] LUX_BASE_TimerCreate Error!\n", __FUNCTION__, __LINE__);
            return -1;
        }

        g_motorAttr.cruiseTimeID[0] = timeID;

        /*创建巡航停止计时器*/
        memset(&TimerParam, 0 ,sizeof(LUX_BASE_TIMER_PARAM));
        TimerParam.bTimeStartEnable = 1;
        TimerParam.timeStartValue = (tStartValue + (t_end - (tStartValue ? t_start : curTime))) * 1000;
        TimerParam.bIntervalEnable = 1;
        TimerParam.timeInterval = TIMING_WHOLD_DAY_SECONDS * 1000;
        TimerParam.cbArgs = NULL;
        TimerParam.pTimerProcess = LUX_HALT_MOTOR_StopCruiseHanler;

        /*创建定时巡航结束计时器*/
        timeID = LUX_BASE_TimerCreate(&TimerParam);
        if(timeID < 0)
        {
            printf("%s:[%d] LUX_BASE_TimerCreate Error!\n", __FUNCTION__, __LINE__);      
        }

        g_motorAttr.cruiseTimeID[1] = timeID;

        /*使能计时器*/
        for(i = 0; i < CRUISE_TIMER_NUMBER; i++)
        {
            ret = LUX_BASE_TimerStart(g_motorAttr.cruiseTimeID[i]);
            if(0 != ret)
            {
                printf("%s:[%d] LUX_BASE_TimerCreate Error!\n", __FUNCTION__, __LINE__);
            }
        }
    }
    else if(LUX_HAL_MOTOR_CRUISE_STOP == cruiseMode)
    {
        for(i = 0; i < CRUISE_TIMER_NUMBER; i++)
        {
            if( -1 != g_motorAttr.cruiseTimeID[i])
            {
                ret =  LUX_BASE_TimerDestroy(g_motorAttr.cruiseTimeID[i]);
                if(0 != ret)
                {
                    printf("%s:[%d] LUX_BASE_TimerDestroy Error!\n", __FUNCTION__, __LINE__);
                    return -1;
                }
            }
            g_motorAttr.cruiseTimeID[i] = -1;
        }

        if(MOTOR_IS_STOP != g_motorAttr.motorStatus.status)
        {
            ret = LUX_HAL_MOTOR_Stop();
            if(0 != ret)
            {
            printf("%s:[%d] LUX_HAL_MOTOR_Stop Error!\n", __FUNCTION__, __LINE__);
            return -1;
            }
        }
    }

    return 0;
}

/**
 * @description: 设置PTZ巡航模式启动配置
 * @return 0 成功， 非零失败
 */
int LUX_HAL_MOTOR_CruiseModeConfig(void)
{
    int ret = -1;
    int patrolSwitch = 0;
    int patrolTMode = 0;
    int patrolMod = 0;
    int sleepMode = 0;

    LUX_INIParse_GetInt(LUX_CONFIG_FILE, "algorithm_config", "privacy_mask", &sleepMode);
    LUX_INIParse_GetInt(LUX_CONFIG_FILE, "motor", "patrol_switch",&patrolSwitch);
    LUX_INIParse_GetInt(LUX_CONFIG_FILE, "motor", "patrol_tmode", &patrolTMode);
    LUX_INIParse_GetInt(LUX_CONFIG_FILE, "motor", "patrol_mode", &patrolMod);

    /*隐私模式下停止巡航*/
    if(1 == sleepMode)
    {
        ret = LUX_HAL_MOTOR_SetCruiseMode(LUX_HAL_MOTOR_CRUISE_STOP, 0);
        if(0 != ret)
        {
            printf("%s:[%d] LUX_HAL_MOTOR_SetCruiseMode Error!\n", __FUNCTION__, __LINE__);
            return -1;
        }
    }
    else
    {
        /*巡航开启*/
        if(1 == patrolSwitch)
        {
            /*全景巡航*/
            if(0 == patrolMod)
            {
                /*全天巡航*/
                if(0 == patrolTMode)
                {
                    ret = LUX_HAL_MOTOR_SetCruiseMode(LUX_HAL_MOTOR_CRUISE_ALLDAY, 300);
                    if(0 != ret)
                    {
                        printf("%s:[%d] LUX_HAL_MOTOR_SetCruiseMode Error!\n", __FUNCTION__, __LINE__);
                        return -1;
                    }
                }
                /*定时巡航*/
                else
                {
                    ret = LUX_HAL_MOTOR_SetCruiseMode(LUX_HAL_MOTOR_CRUISE_TIMING, 300);
                    if(0 != ret)
                    {
                        printf("%s:[%d] LUX_HAL_MOTOR_SetCruiseMode Error!\n", __FUNCTION__, __LINE__);
                        return -1;
                    }
                }
            }
            /*收藏点巡航*/
            else
            {

            }
        }
        /*巡航关闭*/
        else
        {
            ret = LUX_HAL_MOTOR_SetCruiseMode(LUX_HAL_MOTOR_CRUISE_STOP, 0);
            if(0 != ret)
            {
                printf("%s:[%d] LUX_HAL_MOTOR_SetCruiseMode Error!\n", __FUNCTION__, __LINE__);
                return -1;
            }
        }
    }



    return 0;
}
void LUX_HAL_Motor_CalibrationProcess()
{
    /*电机复位*/
    ioctl(g_motorAttr.motorFd, MOTOR_RESET, &g_motorAttr.motorRestData);
    ioctl(g_motorAttr.motorFd, MOTOR_GOBACK, 0);

    memset (&g_motorAttr.motorStatus, 0 ,sizeof(LUX_HAL_MOTOR_MSG_ST));
    while(1)
    {
        sleep(1);
        ioctl (g_motorAttr.motorFd, MOTOR_GET_STATUS, &g_motorAttr.motorStatus);

        printf("[A]g_motorAttr.motorStatus.x %d\n",g_motorAttr.motorStatus.x);
        printf("[A]g_motorAttr.motorStatus.y %d\n",g_motorAttr.motorStatus.y);

        printf("[A]g_motorAttr.motorStatus.starus %d\n",g_motorAttr.motorStatus.status);
        printf("[A]g_motorAttr.motorStatus.speed %d\n",g_motorAttr.motorStatus.speed);

        if(g_motorAttr.motorStatus.status == 0)
        {
            printf("Motor init End\n");
            break;
        }
    }
    g_motorAttr.motorStatus.x = g_motorAttr.x - g_motorAttr.motorStatus.x;
    g_motorAttr.motorStatus.y = g_motorAttr.y - g_motorAttr.motorStatus.y;
    if(0 != g_motorAttr.motorStatus.x || 0 != g_motorAttr.motorStatus.y)
    {
        printf("Motor back to memery status  x:%d  y:%d\n",g_motorAttr.x,g_motorAttr.y);
        ioctl(g_motorAttr.motorFd, MOTOR_SPEED, (unsigned long)&g_motorAttr.speed);
        ioctl(g_motorAttr.motorFd, MOTOR_MOVE, (unsigned long)&g_motorAttr.motorStatus);
    }

    return;
}

/**
 * @description: 电机移动控制初始化
 * @return 0 成功， 非零失败
 */
int LUX_HAL_Motor_CtrlInit()
{

    /*打开文件电机驱动文件描述符*/
    int ret = -1;
    //获取电机位置
    ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "algorithm_config", "MOTOR_X", &g_motorAttr.x);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "MOTOR_X failed,error [0x%x]\n", ret);
        //如果TUYA_Config中没有AO_VOLUME,，从mediaConfig中读取默认值，再写入TUYA_Config

        char tmp[16] = {'\0'};
        sprintf(tmp, "%d", g_motorAttr.x);
        ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "algorithm_config", "MOTOR_X", tmp);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
            return -1;
        }
        ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "algorithm_config", "MOTOR_X", &g_motorAttr.x);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
            return -1;
        }
    }

    ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "algorithm_config", "MOTOR_Y", &g_motorAttr.y);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "MOTOR_Y failed,error [0x%x]\n", ret);
        
        //如果TUYA_Config中没有AO_VOLUME,，从mediaConfig中读取默认值，再写入TUYA_Config

        char tmp[16] = {'\0'};
        sprintf(tmp, "%d", g_motorAttr.y);
        ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "algorithm_config", "MOTOR_Y", tmp);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
            return -1;
        }
        ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "algorithm_config", "MOTOR_Y", &g_motorAttr.y);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
            return -1;
        }
    }

    ret = LUX_HAL_Motor_OpenDev();  
    if(0 != ret)
    {
        printf("%s:[%d] LUX_HAL_Motor_OpenDev error!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = Thread_Mutex_Create(&g_motorAttr.mux);
    if(0 != ret)
    {
        printf("%s:[%d] Thread_Mutex_Create error!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    /* 异步校准电机 */
    Task_CreateThread(LUX_HAL_Motor_CalibrationProcess, NULL);

    g_motorAttr.bInit = 1;

//TODO:删除
#if 0
    LUX_HAL_MOTOR_MSG_ST tmpMsg = {0};
    LUX_HAL_MOTOR_STEPS_ST Mov300  = {0, 300};


    while(1)
    {
        ioctl (g_motorAttr.motorFd, MOTOR_GET_STATUS, &tmpMsg);  
        printf("x %d, y %d, status %d, speed %d\n", tmpMsg.x, tmpMsg.y, tmpMsg.status, tmpMsg.speed);
        if('m' == getchar())
        {
            printf("move 300 steps\n");
            ioctl (g_motorAttr.motorFd, MOTOR_MOVE, &Mov300);
        }
    }
#endif

    return 0;
}

/**
 * @description: 
 * @return 0 成功， 非零失败
 */
int LUX_HAL_Motor_CtrlDeinit()
{
    int ret = -1;
    if(g_motorAttr.motorFd < 0)
    {
        printf("%s:[%d] LUX_HAL_Motor_CtrlInit hasn't been inited\n", __FUNCTION__, __LINE__);
        return -1;
    }
    
    ret = Thread_Mutex_Destroy(&g_motorAttr.mux);
    if(0 != ret)
    { 
        printf("%s:[%d] Thread_Mutex_Destroy error!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    close(g_motorAttr.motorFd);
    g_motorAttr.bInit = 0;

    return 0;
}


/**
 * @description: 电机人形追踪功能; 根据输入计算电机速度及步数，确定电机转动状态
 * @param [in] dx, 目标中心与原点坐标水平方向上的位移
 * @param [in] dy, 目标中心与原点坐标水平方向上的位移
 * @param [in] tracking, 算法是否在跟踪状态，否则在检测状态
 * @param [in] capture_lost, 目标丢失代价，0，未丢失，>0，丢失
 * @return 0 成功， 非零失败
 */ 
int LUX_HAL_MOTOR_PersonTracker(int dx, int dy, int tracking, int capture_lost)
{

    static int is_moving =0;
    static int dy_t1 ;

    float kpy =0.25, kdy =0.0;
    //float kiy =0.0;
    //float s_t1c =0;
    //float s_tc =0;
    
    dy = -dy; /*垂直方向控制*/
    dy_t1 = dy;

    LUX_HAL_MOTOR_STEPS_ST motorSteps = {0};
    LUX_HAL_MOTOR_MOVE_ACTION_ST motorAction = {0};

    LUX_HAL_MOTOR_MSG_ST motorMessage = {0};

    motorSteps.y =(int)(kpy*(dy)+ kdy*( dy - dy_t1));

#if 0
    /*TODO: 通道号,按获取的方式来写入*/
    int ret = -1;
    person_tracker_param_input_t param = {0};
    ret = IMP_IVS_GetParam(2, &param);
	if (ret < 0)
    {
		IMP_LOG_ERR(TAG, "IMP_IVS_GetParam(%d) failed\n", 0);
		return -1;
	}
#endif

    if(abs(dy)<50)
    {
        /* 纵向距离较小时，步数为 0,80 为经验值，可自行调试。纵向电机转动的空间较小，需要纵向跟踪的场景较少，所以 y 方向上步数计算比较简单。*/
        motorSteps.y =0;
    }
    else
    {
        ;
    }

    /*输入参数由跟踪状态得来的*/
    if(tracking)
    {
        /*根据距离不同设定不同的 speed, 区间阈值和 speed 均为经验值，可调试*/
        if(100== motorAction.motor_move_speed)
        {
            if(abs(dx)>=80)
            {
                motorAction.motor_move_speed=900;
            }
        }
        else
        {
            if(abs(dx)>=80)
            {
                motorAction.motor_move_speed=900;
            }
            else if(abs(dx)>=50)
            {
                motorAction.motor_move_speed=750;
            }
            else if(abs(dx)>=30)
            {
                motorAction.motor_move_speed=525;
            }
            else
            {
                motorAction.motor_move_speed=100;
            }
        }

        /*设定电机速度*/
        ioctl(g_motorAttr.motorFd, MOTOR_SPEED,(unsigned int)(&motorAction.motor_move_speed));

        /*计算旋转步数，此处步数均设为 160，那么就由 speed 值来控制电机转动距离，可根据实际情况调整*/
        switch(motorAction.motor_move_speed)
        {
            case 900:
                motorSteps.x = 160;
                break;
            case 750:
                motorSteps.x = 160;
                break;
            case 525:
                motorSteps.x = 160;
                break;
            case 100:
                motorSteps.x = 0;
                break;
            default:
                motorSteps.x = 0;
        }

        /*设定转动方向*/
        if(dx >= 0)
        {
            ;
        }
        else
        {
            motorSteps.x =-motorSteps.x;
        }

        /*设定电机状态，停止或移动*/
        if(0== motorSteps.x &&0== motorSteps.y)
        {
            ioctl(g_motorAttr.motorFd, MOTOR_STOP);
        }
        else
        {
            ioctl(g_motorAttr.motorFd, MOTOR_MOVE,(unsigned long)&motorSteps);
        }

    //    IMP_LOG_INFO(TAG,"T Motor speed: %d, step: [%d, %d]\n",
    //    motorAction.motor_move_speed, motorSteps.x, motorSteps.y);
    }
    else
    {
        /*输入参数由检测状态得来*/
        if(capture_lost)
        {
            /*
            该分支针对两种情况导致的检测不稳定问题：
            1、相邻帧，目标区域没有任何重叠， 可能是新目标出现或者只检到物体移动的一部分；
            2、历史帧均检到目标但当前帧没检到任何目标，目标存在短暂丢失的可能，所以参考历史帧数据设定电机转动参数。这两种情况下，电机转动 
            */

            is_moving = 1;

            /* 设定速度，经验值，可调 */
            motorAction.motor_move_speed = 800;
            ioctl(g_motorAttr.motorFd, MOTOR_SPEED, (unsigned int)(&motorAction.motor_move_speed));

            /* 计算旋转步数，可根据实际情况调整经验值大小 */
            if(abs(dx)>80)
            {
                motorSteps.x = 1.2*dx;
            }
            else
            {
                motorSteps.x = 0;
            }
            if(abs(dy)>80)
            {
                motorSteps.y = 0.8 * dy;
            }
            else
            {
                motorSteps.y = 0;
            }
            //motorSteps.y = -motorSteps.y;
            ioctl(g_motorAttr.motorFd, MOTOR_MOVE, (unsigned long)&motorSteps);
            //IMP_LOG_INFO(TAG,"L Motor speed: %d, step: [%d, %d]\n",
            //motorAction.motor_move_speed, motorSteps.x, motorSteps.y);
        }
        else
        {
            /*检测状态稳定，移动侦测对抖动敏感，所以检测状态电机是 STOP 状态*/
            if(!is_moving)
            {
                motorAction.motor_move_speed = 100;
                ioctl(g_motorAttr.motorFd, MOTOR_STOP);
            }
            else
            {
                if(MOTOR_IS_STOP == motorMessage.status)
                {
                    is_moving = 0;
                }
            }
        }
    }

#if 0
    /*获取电机状态*/
    ioctl(g_motorAttr.motorFd, MOTOR_GET_STATUS, (unsigned long) &motorMessage);

    param.is_motor_stop = ((MOTOR_IS_STOP == motorMessage.status)? 1 : 0);
	param.is_feedback_motor_status = 1;

    ret = IMP_IVS_SetParam(2, &param);
	if (ret < 0){
		  IMP_LOG_ERR(TAG, "IMP_IVS_SetParam(%d) failed\n", 0);
		  return -1;
	}
#endif    

    return 0;
}
