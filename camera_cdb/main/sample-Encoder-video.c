/*
 * sample-Encoder-video.c
 *
 * Copyright (C) 2014 Ingenic Semiconductor Co.,Ltd
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <imp_log.h>
#include <imp_common.h>
#include <imp_system.h>
#include <imp_framesource.h>
#include <imp_encoder.h>
#include <imp_log.h>
#include <comm_def.h>
#include <comm_error_code.h>
#include <comm_func.h>
#include <lux_isp.h>
#include <lux_fsrc.h>
#include <lux_video.h>
#include <lux_base.h>
#include "lux_audio.h"
#include <lux_iniparse.h>
#include <lux_hal_led.h>
#include <lux_hal_lightsns.h>
#include <lux_hal_misc.h>
#include <lux_daynight.h>
#include <lux_ivs.h>
#include <lux_osd.h>
#include <lux_pwm.h>
 //#define CONFIG_JIO_DEMO
#include <ivs_common.h>
#include <ivs_inf_move.h>
#include "ivs_interface.h"

#include <lux_nv2jpeg.h>
#include <lux_ipc_control.h>
#include <lux_global.h>
#include <lux_users.h>
#include <lux_config.h>
#include <jwhead.h>
#include "lux_reset_key.h"

#define TAG 	"Sample-Encoder-video"
//#define TEST_DUMP_NV12
extern int rtsp_demo_main();
extern int jio_demo_main();
extern int jio_skel_main();
//extern int tuya_demo_main(char *inputToken);

#ifdef TEST_DUMP_NV12
static void*
Therad_TestFunc(void* args)
{
    INT_X	ret = LUX_ERR;

    sleep(2);
    ret = LUX_FSrc_SaveDumpNV12(1, LUX_TRUE);
    if (LUX_OK != ret) {
        IMP_LOG_ERR(TAG, "save_dumpNV12Pic failed. error number [x0%x]\n", ret);
        return (void*)LUX_ERR;
    }

    return (void*)LUX_OK;
}

/**
 * @description: 创建一个线程,并设置分离属性
 * @return 0：成功，-1：失败；返回错误码;
 */
static INT_X SAMPLE_EcodeVideo_TestFunc(void)
{
    INT_X ret = LUX_ERR;
    ret = Task_CreateThread(Therad_TestFunc, NULL);
    if (LUX_OK != ret) {
        IMP_LOG_ERR(TAG, "Task_CreateThread failed.\n");
        return LUX_ERR;
    }

    return LUX_OK;
}
#endif
#if 1  /*包装过的sample*/
INT_X LUSHARE_test_sample_Encode_video(INT_X argc, PCHAR_X argv)
{
    INT_X ret = 0;

    /*设置打印属性*/
    IMP_Log_Set_Option(IMP_LOG_OP_DEFAULT);
    printf("IMP_Log_Set_Option OK! \n");

    /*全局初始化*/
    LUX_Global_Init();
    printf("LUX_Global_Init OK! \n");

    /*配置初始化*/
    LUX_Config_Init();

    /*初始化用户管理*/
    LUX_UserMngr_Init();
    printf("LUX_UserMngr_Init OK! \n");

#ifdef CONFIG_JIO_DEMO
    /*录像初始化*/
    LUX_Record_Init();
    printf("LUX_Record_Init OK! \n");
#endif

    // load_jio_config();
    // g_JioConfig = &cfg;

    printf("LUX_BASE_TimeInit ... \n");
    /*初始化计时器*/
    ret = LUX_BASE_TimeInit();
    if (LUX_OK != ret) {
        IMP_LOG_ERR(TAG, "LUX_BASE_TimeInit, return error[0x%x]\n", ret);
        return LUX_ERR;
    }
    printf("LUX_BASE_TimeInit OK! \n");

    printf("LUX_ISP_Init ... \n");
    /* Step.1 System init */
    ret = LUX_ISP_Init();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_ISP_Init failed, return error[0x%x]\n", ret);
        return LUX_ERR;
    }
    printf("LUX_ISP_Init OK! \n");
    /* Step.2 FrameSource init */
    ret = LUX_FSrc_Init(0);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "FrameSource init failed\n");
        return LUX_ERR;
    }
    printf("LUX_FSrc_Init OK! \n");

    ret = LUX_IVS_Init();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "Encoder init failed\n");
        return LUX_ERR;
    }
    printf("LUX_IVS_Init OK! \n");

    /* Step.3 Encoder init */
    ret = LUX_Video_Encoder_Init(0);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "Encoder init failed\n");
        return LUX_ERR;
    }
    printf("LUX_Video_Encoder_Init OK! \n");

    /* Step.4 OSD init */
    ret = LUX_OSD_Init();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "OSD init failed\n");
        return LUX_ERR;
    }
    printf("LUX_OSD_Init OK! \n");


    /* AUDIO IN Init */
    ret = LUX_AUDIO_Init();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_AI_Init failed\n");
        return LUX_ERR;
    }
    printf("LUX_AUDIO_Init OK! \n");

    /* Step.4 Bind */
    ret = LUX_COMM_Bind_Connect();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "Encoder init failed\n");
        return LUX_ERR;
    }

    /* Step.6 OSD time stamp update */
    ret = LUX_OSD_Time_Stamp_Update();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_OSD_Time_Stamp_Update failed\n");
        return LUX_ERR;
    }

    printf("LUX_Video_Encoder_Start main \n");
    ret = LUX_Video_Encoder_Start(LUX_STREAM_MAIN);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "ImpStreamOn failed chan:%d\n", LUX_STREAM_MAIN);
    }

    printf("LUX_Video_Encoder_Start sub \n");
    ret = LUX_Video_Encoder_Start(LUX_STREAM_SUB);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "ImpStreamOn failed chan:%d\n", LUX_STREAM_SUB);
    }

    // ret = LUX_Video_Encoder_Start(LUX_STREAM_JPEG);
    // if (LUX_OK != ret)
    // {
    //     IMP_LOG_ERR(TAG, "ImpStreamOn failed chan:%d\n", LUX_STREAM_JPEG);
    // }

#if 0 //T40移植暂时屏蔽  humy
    /* audio out start */
    ret = LUX_AO_Start();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "ImpStreamOn failed\n");
        return LUX_ERR;
    }
#endif

#ifdef TEST_DUMP_NV12
    /*测试功能代码用的线程*/
    ret = (INT_X)SAMPLE_EcodeVideo_TestFunc();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "SAMPLE_EcodeVideo_TestFunc failed\n");
        return LUX_ERR;
}
#endif
    printf("LUX_DayNight_Init \n");
    ret = LUX_DayNight_Init();
    printf("LUX_DayNight_Init end\n");
    if (0 != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_DayNight_Init failed!, error num [0x%x] \n", ret);
    }

#if 0
    ret = LUX_PWM_Init();
    if (0 != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_PWM_Init failed!, error num [0x%x] \n", ret);
    }
#endif

    printf("LUX_HAL_LedInit \n");
    LUX_HAL_LedInit();
    printf("LUX_HAL_LedInit end\n");

#if 0
    ret = LUX_HAL_Motor_CtrlInit();
    if (0 != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_HAL_Motor_CtrlInit failed!, error num [0x%x] \n", ret);
    }
#endif

    printf("set_time_zone \n");
    jio_system_init(&g_configData->image_config);
    set_time_zone(g_configData->system_config.timezone);
    set_night_vision_mode(g_configData->image_config.daynight);
#if 1
    //begin video and audio recording ...
    LUX_Record_Start();
    printf("LUX_Record_Start OK! \n");
#endif

#if 0
    ret = LUX_IVS_FaceDet_Init();
    if (0 != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_FaceDet_Init failed!, error num [0x%x] \n", ret);
    }
#endif

    /*start onvif service*/
    onvif_start();
    //     int* p = &g_configData->media_config.video_config;
    // int i;
    // for (i = 0; i < sizeof(g_configData->media_config.video_config)/sizeof(int); i++)
    // {
    //     (*p++) = i + 1;
    //     printf(" sizeof(date) =%d,  p = %d\n",sizeof(g_configData->media_config.video_config), *p);
    // }
    LUX_Config_Set_AllConfig(g_configData);


    ret = rtsp_demo_main(NULL);
    printf("rtsp_demo_main ret:%d\n", ret);

    if (g_configData->system_config.jio_enabled)
    {
        ret = jio_demo_main(NULL);
        printf("jio_demo_main ret:%d\n", ret);
    }

    int timeCnt = 0;
    for (;;) {
        if (timeCnt >= 600)
        {
            //set_time_zone(jio_cfg.system_config.timezone);
            lux_time_ntp_sync("00:00");
            //lux_time_ntp_cmd_sync();
            timeCnt = 0;
        }
        timeCnt++;
        sleep(1);
    }



	 //LUX_Config_SetNetworkCfg(&g_configData->network_config);
	//LUX_Config_SetJiocfg(&g_configData->g_JioConfig);
#ifdef CONFIG_JIO_SKEL
    ret = jio_skel_main(NULL);
    IMP_LOG_ERR(TAG, "jio_skel_main ret:%d\n", ret);
#endif
    ret = LUX_AO_Stop();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_AO_Stop failed\n");
        return LUX_ERR;
    }

    ret = LUX_AI_Stop();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_AI_Stop failed\n");
        return LUX_ERR;
    }

    ret = LUX_Video_Encoder_Stop(LUX_STREAM_MAIN);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "ImpStreamOff failed chan:%d\n", LUX_STREAM_MAIN);
    }

    ret = LUX_Video_Encoder_Stop(LUX_STREAM_SUB);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "ImpStreamOff failed chan:%d\n", LUX_STREAM_SUB);
    }

    // ret = LUX_Video_Encoder_Stop(LUX_STREAM_JPEG);
    // if (LUX_OK != ret)
    // {
    //     IMP_LOG_ERR(TAG, "ImpStreamOff failed chan:%d\n", LUX_STREAM_JPEG);
    // }

    /* Step.b UnBind */
    ret = LUX_COMM_Bind_DisConnect();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_COMM_Bind_DisConnect failed\n");
        return LUX_ERR;
    }

    /* Step.c OSD exit */
    ret = LUX_OSD_Exit();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "OSD init failed\n");
        return LUX_ERR;
    }

    //LUX_AUDIO_Exit();

    /* Step.c Encoder exit */
    ret = LUX_Video_Encoder_Exit();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "Encoder exit failed\n");
        return LUX_ERR;
    }

    ret = LUX_IVS_Exit();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_IVS_Exit failed\n");
        return LUX_ERR;
    }

    /* Step.d FrameSource exit */
    ret = LUX_FSrc_Exit();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_FSrc_Exit failed\n");
        return LUX_ERR;
    }

    /* Step.e System exit */
    ret = LUX_ISP_Exit();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_ISP_Exit failed\n");
        return LUX_ERR;
    }

    return 0;
}
#endif  /*包装过的sample*/
