/*
 * Ingenic SU SDK PWM test.
 *
 * Copyright (C) 2014 Ingenic Semiconductor Co.,Ltd
 * Author: Tiger <bohu.liang@ingenic.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "lux_hal_pwm.h"
#include "lux_base.h"

#define LUX_PWM_PERIOD  10000000

#define LUX_PWM_CHAN        3
#define LUX_PWM_OPEN_DUTY   10000000 /* unit:ms */
#define LUX_PWM_CLOSE_DUTY  0

typedef struct
{
    int chan;
    int status; /* 0:stop,1:start*/
    int bStart;
    int bInit;
    OSI_MUTEX mux;
} LUX_PWM_ATTR;

LUX_PWM_ATTR g_pwmAttr = 
{
    .chan = LUX_PWM_CHAN,
    .status = 0,
    .bStart = LUX_FALSE,
};

BOOL_X g_pwmBlubEnable = LUX_TRUE;

void TASK_PWM_BreathLed_Process()
{
    int ret  = 0;
    int duty = 0;
    int i = 0;

    while (1)
    {
        if (!g_pwmAttr.bInit || !g_pwmBlubEnable || !g_pwmAttr.bStart)
        {
            usleep(100 * 1000);
            continue;
        }

        /* 暗->亮 */
        for (i = 0; i < 100; i++)
        {
            duty = i * 100000;
            ret = SU_PWM_ModifyChnDuty(LUX_PWM_CHAN, duty);
            if (ret)
            {
                printf("Failed to modify duty(%d)\n", duty);
            }

            usleep(30000);
        }
        /* 亮->暗 */
        for (i = 100; i > 0; i--)
        {
            duty = i * 100000;
            ret = SU_PWM_ModifyChnDuty(LUX_PWM_CHAN, duty);
            if (ret)
            {
                printf("Failed to modify duty(%d)\n", duty);
            }
            usleep(25000);
        }
        usleep(50000);
    }

    SU_PWM_DisableChn(LUX_PWM_CHAN);
    SU_PWM_DestroyChn(LUX_PWM_CHAN);
    SU_PWM_Exit();

    return ;
}

int LUX_PWM_Start()
{
    g_pwmAttr.bStart = LUX_TRUE;

    return 0;
}

int LUX_PWM_Stop()
{
    g_pwmAttr.bStart = LUX_FALSE;

    return 0;
}

int LUX_PWM_Init()
{
    int ret;
    SUPWMChnAttr attr;

    ret = SU_PWM_Init();
    if(ret < 0) {
        printf("SU_PWM_CreateChn error !\n");
        return 0;
    }

    attr.period = LUX_PWM_PERIOD;
    attr.duty = 0;
    attr.polarity = 1;
    ret = SU_PWM_CreateChn(LUX_PWM_CHAN, &attr);
    if (ret < 0)
    {
        printf("SU_PWM_CreateChn error !\n");
        return 0;
    }

    ret = SU_PWM_EnableChn(LUX_PWM_CHAN);
    if (ret < 0)
    {
        printf("SU_PWM_EnableChn error !\n");

        
        return 0;
    }

    ret = Thread_Mutex_Create(&g_pwmAttr.mux);
    if (0 != ret)
    {
        printf("%s:[%d] Thread_Mutex_Create error!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    Task_CreateThread(TASK_PWM_BreathLed_Process, NULL);
    g_pwmAttr.bInit = LUX_TRUE;
    return 0;
}

int LUX_PWM_Exit()
{
    Thread_Mutex_Destroy(&g_pwmAttr.mux);
    SU_PWM_DisableChn(LUX_PWM_CHAN);
    SU_PWM_DestroyChn(LUX_PWM_CHAN);
    SU_PWM_Exit();

    g_pwmAttr.bInit = LUX_FALSE;

    return 0;
}
