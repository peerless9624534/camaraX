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
#include "AppVersion.h"

extern int rtsp_demo_main();
extern int xcamer_rtsp_demo();
extern int jio_demo_main();
extern int jio_skel_main();

#define TAG "LuxTest-"
#define LUX_DEVICE_INFO_FILE              "/syskey/device_info.ini"
#define LUX_TEST_WRITE              1
#define LUX_TEST_READ               0

INT_X lux_fatory_test_device_info_option(int wr,char** options)
{
    printf("++++%s START++++\n", options[0]);
    int ret = 0;
    char buffer[64] = { 0 };

    ret = LUX_BASE_System_CMD("mount -t ubifs /dev/ubi1_0 /syskey/");
    if (LUX_OK != ret) {
        printf("++++%s FAIL++++\n", options[0]);
        return LUX_ERR;
    }

    ret = LUX_INIParse_Load(LUX_DEVICE_INFO_FILE);
    if (LUX_OK != ret) {
        LUX_BASE_System_CMD("umount /syskey/");
        printf("++++%s FAIL++++\n", options[0]);
        return LUX_ERR;
    }
    if (LUX_TEST_READ == wr)
    {
        ret = LUX_INIParse_GetString(LUX_DEVICE_INFO_FILE, options[2], options[3], buffer);
        if (0 != ret)
        {
            LUX_BASE_System_CMD("umount /syskey/");
            printf("++++%s FAIL++++\n", options[0]);
            return LUX_ERR;
        }
        printf("++++%s Value is [%s]++++\n", options[0],buffer);
    }
    else if ( LUX_TEST_WRITE == wr)
    {
        if (NULL == options[1])
        {
            LUX_BASE_System_CMD("umount /syskey/");
            printf("++++%s FAIL,Missing parameter++++\n", options[0]);
            return LUX_ERR;
        }
        strcpy(buffer, options[1]);
        ret = LUX_INIParse_SetKey(LUX_DEVICE_INFO_FILE, options[2], options[3], buffer);
        if (LUX_OK != ret)
        {
            LUX_BASE_System_CMD("umount /syskey/");
            printf("++++%s FAIL++++\n", options[0]);
            return LUX_ERR;
        }
    }

    ret = LUX_BASE_System_CMD("sync");
    if (LUX_OK != ret) {
        LUX_BASE_System_CMD("umount /syskey/");
        printf("++++%s FAIL++++\n", options[0]);
        return LUX_ERR;
    }

    ret = LUX_BASE_System_CMD("umount /syskey/");
    if (LUX_OK != ret) {
        printf("++++%s FAIL++++\n", options[0]);
        return LUX_ERR;
    }

    printf("++++%s PASS++++\n",options[0]);

    return LUX_OK;
}

INT_X lux_fatory_test_init(INT_X argc, char** argv)
{
    int ret;

    char buffer[512] = { 0 };
    char buffer_1[64] = { 0 };
    strcpy(buffer, argv[2]);
    int wr = 0;
  
    if (0 == strcmp(buffer, "LuxTestVideo"))
    {
        printf("++++LuxTestSD_Write START++++\n");

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

        printf("LUX_BASE_TimeInit ... \n");
        /*初始化计时器*/
        ret = LUX_BASE_TimeInit();
        if (LUX_OK != ret) {
            printf("++++LuxTestVideo FAIL++++\n");
            return LUX_ERR;
        }
        printf("LUX_BASE_TimeInit OK! \n");

        printf("LUX_ISP_Init ... \n");
        /* Step.1 System init */
        ret = LUX_ISP_Init();
        if (LUX_OK != ret)
        {
            printf("++++LuxTestVideo FAIL++++\n");
            return LUX_ERR;
        }
        printf("LUX_ISP_Init OK! \n");

        /* Step.2 FrameSource init */
        ret = LUX_FSrc_Init(0);
        if (LUX_OK != ret)
        {
            printf("++++LuxTestVideo FAIL++++\n");
            return LUX_ERR;
        }
        printf("LUX_FSrc_Init OK! \n");

        ret = LUX_IVS_Init();
        if (LUX_OK != ret)
        {
            printf("++++LuxTestVideo FAIL++++\n");
            return LUX_ERR;
        }
        printf("LUX_IVS_Init OK! \n");

        /* Step.3 Encoder init */
        ret = LUX_Video_Encoder_Init(0);
        if (LUX_OK != ret)
        {
            printf("++++LuxTestVideo FAIL++++\n");
            return LUX_ERR;
        }
        printf("LUX_Video_Encoder_Init OK! \n");

        /* Step.4 OSD init */
        ret = LUX_OSD_Init();
        if (LUX_OK != ret)
        {
            printf("++++LuxTestVideo FAIL++++\n");
            return LUX_ERR;
        }
        printf("LUX_OSD_Init OK! \n");

        /* Step.4 Bind */
        ret = LUX_COMM_Bind_Connect();
        if (LUX_OK != ret)
        {
            printf("++++LuxTestVideo FAIL++++\n");
            return LUX_ERR;
        }

        /* Step.6 OSD time stamp update */
        ret = LUX_OSD_Time_Stamp_Update();
        if (LUX_OK != ret)
        {
            printf("++++LuxTestVideo FAIL++++\n");
            return LUX_ERR;
        }

        printf("LUX_Video_Encoder_Start main \n");
        ret = LUX_Video_Encoder_Start(LUX_STREAM_MAIN);
        if (LUX_OK != ret)
        {
            printf("++++LuxTestVideo FAIL++++\n");
            return LUX_ERR;
        }

        printf("LUX_Video_Encoder_Start sub \n");
        ret = LUX_Video_Encoder_Start(LUX_STREAM_SUB);
        if (LUX_OK != ret)
        {
            printf("++++LuxTestVideo FAIL++++\n");
            return LUX_ERR;
        }

        ret = rtsp_demo_main(NULL);
        printf("rtsp_demo_main ret:%d\n", ret);

        printf("++++LuxTestVideo PASS++++\n");

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
    }
    else if (0 == strcmp(buffer, "LuxTestAudio"))
    {
        printf("++++LuxTestAudio START++++\n");

        /*全局初始化*/
        LUX_Global_Init();
        printf("LUX_Global_Init OK! \n");

        /*配置初始化*/
        LUX_Config_Init();

        /* AUDIO IN Init */
        ret = LUX_AUDIO_Init();
        if (LUX_OK != ret)
        {
            printf("++++LuxTestAudio FAIL++++\n");
            return LUX_ERR;
        }
        printf("LUX_AUDIO_Init OK! \n");
        ret = rtsp_demo_main();
        printf("rtsp_demo_main ret:%d\n", ret); 
        printf("++++LuxTestAudio Recording++++\n");
        int time = 0;
        time = atoi(argv[3]);
        printf("time:%d\n", time);

        sleep(time);
  
        printf("++++LuxTestAudio PASS++++\n");

    }
    else if (0 == strcmp(buffer, "LuxTestSD_Write"))
    {
        printf("++++LuxTestSD_Write START++++\n");
        char buffer_write[] = "LuxTestSD_Write";
        static FILE* fp_sd_wt = NULL;

        fp_sd_wt = fopen("/mnt/sdcard2/LuxTest.dat", "wb");
        if (NULL == fp_sd_wt)
        {
            printf("++++LuxTestSD_Write FAIL++++\n");
            return LUX_ERR;
        }
        fwrite(buffer_write, 1, sizeof(buffer_write), fp_sd_wt);
        close(fp_sd_wt);

        printf("++++LuxTestSD_Write PASS++++\n");
    }
    else if (0 == strcmp(buffer, "LuxTestSD_Read"))
    {
        printf("++++LuxTestSD_Read START++++\n");

        FILE* fp_sd_rd = NULL;
        int _size = 0;

        _size = sizeof("LuxTestSD_Write");

        fp_sd_rd = fopen("/mnt/sdcard2/LuxTest.dat", "rb");
        if (NULL == fp_sd_rd)
        {
            printf("++++LuxTestSD_Read FAIL++++\n");
            return LUX_ERR;
        }

        ret = fread(buffer, _size, 1, fp_sd_rd);
        printf("++++LuxTestSD_Read is [%s]++++\n",buffer);
        if (ret == 0)
        {
            printf("++++LuxTestSD_Read FAIL++++\n");
            return LUX_ERR;
        }
        close(fp_sd_rd);

        printf("++++LuxTestSD_Read PASS++++\n");

    }
    else if (0 == strcmp(buffer, "LuxTestIRlightOn"))
    {
        printf("++++LuxTestIRlightOn START++++\n");

        fd_116 = lux_gpio_open(116); 
        if (-1 == fd_116)
        {
            printf("++++LuxTestIRlightOff FAIL++++\n");
            return LUX_ERR;
        }
        /*全局初始化*/
        LUX_Global_Init();

        ret = LUX_Hal_Misc_IRLightOnOff(LUX_TRUE);
        if (LUX_OK != ret)
        {
            printf("++++LuxTestIRlightOn FAIL++++\n");
            return LUX_ERR;
        }
        printf("++++LuxTestIRlightOn PASS++++\n");

    }
    else if (0 == strcmp(buffer, "LuxTestIRlightOff"))
    {
        printf("++++LuxTestIRlightOff START++++\n");

        fd_116 = lux_gpio_open(116); 
        if (-1 == fd_116)
        {
            printf("++++LuxTestIRlightOff FAIL++++\n");
            return LUX_ERR;
        }

        /*全局初始化*/
        LUX_Global_Init();

        ret = LUX_Hal_Misc_IRLightOnOff(LUX_FALSE);
        if (LUX_OK != ret)
        {
            printf("++++LuxTestIRlightOff FAIL++++\n");
            return LUX_ERR;
        }
        printf("++++LuxTestIRlightOff PASS++++\n");

    }
    else if (0 == strcmp(buffer, "LuxTestIRcutOn"))
    {
        printf("++++LuxTestIRcutOn START++++\n");
        /*设置ircut是否开启*/
        fd_122 = lux_gpio_open(122);
        fd_123 = lux_gpio_open(123);
        ret = LUX_ISP_SetIRCUT(LUX_TRUE);
        if (LUX_OK != ret)
        {
            printf("++++LuxTestIRcutOn FAIL++++\n");
            return LUX_ERR;
        }
        printf("++++LuxTestIRcutOn PASS++++\n");

    }
    else if (0 == strcmp(buffer, "LuxTestIRcutOff"))
    {
        printf("++++LuxTestIRcutOff START++++\n");
        /*设置ircut是否开启*/
        fd_122 = lux_gpio_open(122);
        if (-1 == fd_122)
        {
            printf("++++LuxTestIRlightOff FAIL++++\n");
            return LUX_ERR;
        }
        fd_123 = lux_gpio_open(123);
        if (-1 == fd_123)
        {
            printf("++++LuxTestIRlightOff FAIL++++\n");
            return LUX_ERR;
        }
        ret = LUX_ISP_SetIRCUT(LUX_FALSE);
        if (LUX_OK != ret)
        {
            printf("++++LuxTestIRcutOff FAIL++++\n");
            return LUX_ERR;
        }
        printf("++++LuxTestIRcutOff PASS++++\n");
    }
    else if (0 == strcmp(buffer, "LuxTestLightSensor"))
    {
        printf("++++LuxTestLightSensor START++++\n");
        /*初始化光感*/
        ret = LUX_Hal_LightSns_Init();
        if (LUX_OK != ret)
        {
            printf("++++LuxTestLightSensor FAIL++++\n");
            return LUX_ERR;
        }

        int LuxTest_LightsnsValue;
        int sum = 0,i=0;
        for (i=0;i<20;i++)
        {
            LuxTest_LightsnsValue = LUX_Hal_LightSns_GetValue();
            sum = sum + LuxTest_LightsnsValue;
        }
        LuxTest_LightsnsValue = sum/20;
        printf("++++LuxTest_LightsnsValue  is %d++++\n",LuxTest_LightsnsValue);
        printf("++++LuxTestLightSensor PASS++++\n");

    }
    else if (0 == strcmp(buffer, "LuxTestIR_Led_Red"))
    {
        printf("++++LuxTestIR_Led_Red START++++\n");
        int fd_green, fd_red;
        int gpio_ircut_red, gpio_ircut_green;

        fd_green = lux_gpio_open(60);
        if (-1 == fd_green)
        {
            printf("++++LuxTestIR_Led_Red FAIL++++\n");
            return LUX_ERR;
        }
        fd_red = lux_gpio_open(59);
        if (-1 == fd_red)
        {
            printf("++++LuxTestIR_Led_Red FAIL++++\n");
            return LUX_ERR;
        }
        lux_gpio_write(fd_green, 0);
        lux_gpio_write(fd_red, 1);
        close(fd_green);
        close(fd_red);

        printf("++++LuxTestIR_Led_Red PASS++++\n");
    }
    else if (0 == strcmp(buffer, "LuxTestIR_Led_Green"))
    {
        printf("++++LuxTestIR_Led_Green START++++\n");
        int fd_green, fd_red;
        int gpio_ircut_red, gpio_ircut_green;

        fd_green = lux_gpio_open(60);
        if (-1 == fd_green)
        {
            printf("++++LuxTestIR_Led_Green FAIL++++\n");
            return LUX_ERR;
        }
        fd_red = lux_gpio_open(59);
        if (-1 == fd_red)
        {
            printf("++++LuxTestIR_Led_Green FAIL++++\n");
            return LUX_ERR;
        }
        lux_gpio_write(fd_green, 1);
        lux_gpio_write(fd_red, 0);
        close(fd_green);
        close(fd_red);

        printf("++++LuxTestIR_Led_Green PASS++++\n");

    }
    else if (0 == strcmp(buffer, "LuxTestIR_Led_Off"))
    {
        printf("++++LuxTestIR_Led_Off START++++\n");
        int fd_green, fd_red;
        int gpio_ircut_red, gpio_ircut_green;

        fd_green = lux_gpio_open(60);
        if (-1 == fd_green)
        {
            printf("++++LuxTestIR_Led_Off FAIL++++\n");
            return LUX_ERR;
        }
        fd_red = lux_gpio_open(59);
        if (-1 == fd_red)
        {
            printf("++++LuxTestIR_Led_Off FAIL++++\n");
            return LUX_ERR;
        }
        lux_gpio_write(fd_green, 0);
        lux_gpio_write(fd_red, 0);

        close(fd_green);
        close(fd_red);

        printf("++++LuxTestIR_Led_Off PASS++++\n");
    }
    else if (0 == strcmp(buffer, "LuxTestResetKey"))
    {
        printf("++++LuxTestResetKey START++++\n");

        char tmpPath[64] = { 0 };
        int test_fd_79;
        int  Switch_Status = 0;
        uint32_t FTM_FirstTime = 0;
        uint32_t FTM_SecondTime = 0;
        uint32_t FTM_JudgeTime = 0;
        uint32_t Flag_1 = 0;
        uint32_t Flag_2 = 0;

        test_fd_79 = lux_gpio_open(79);
        if (test_fd_79 < 0)
        {
            printf("++++LuxTestResetKey FAIL++++\n");
            return -1;
        }

        FTM_FirstTime = time_ms();

        do
        {
            Switch_Status = lux_gpio_read(test_fd_79);

            if (Switch_Status == 0 && Flag_2 == 0)
            {
                Flag_1++;
                Flag_2 = 1;

                if (Flag_1 == 2)
                {
                    printf("[ResetKey]:enter1\r\n");
                    break;
                }
            }
            else if (Switch_Status == 1 && Flag_2 == 1)
            {

                Flag_2 = 0;
                printf("[ResetKey]:enter2\r\n");
            }

            FTM_SecondTime = time_ms();
            FTM_JudgeTime = FTM_SecondTime - FTM_FirstTime;

        } while (FTM_JudgeTime <= 10000);
        close(test_fd_79);
        if (Flag_1 == 2)
        {
            printf("++++LuxTestResetKey PASS++++\n");
        }
        else
        {
            printf("++++LuxTestResetKey FAIL++++\n");
        }
    }
    else if (0 == strcmp(buffer, "LuxTestProduct_SN_Write"))
    {
        char* options[] = {"LuxTestProduct_SN_Write",argv[3],"uuid","app_uuid" };
        wr = LUX_TEST_WRITE;
        ret = lux_fatory_test_device_info_option(wr, options);
        if (LUX_OK != ret)
        {
            return LUX_ERR;
        }

    }
    else if (0 == strcmp(buffer, "LuxTestProduct_SN_Read"))
    {
        char* options[] = {"LuxTestProduct_SN_Read",argv[3],"uuid","app_uuid" };
        wr = LUX_TEST_READ;
        ret = lux_fatory_test_device_info_option(wr,options);
        if (LUX_OK != ret) 
        {
            return LUX_ERR;
        }
    }
    else if (0 == strcmp(buffer, "LuxTestMac_Set"))
    {
        char* options[] = {"LuxTestMac_Set",argv[3],"hw_addr","mac_addr" };
        wr = LUX_TEST_WRITE;
        ret = lux_fatory_test_device_info_option(wr, options);
        if (LUX_OK != ret)
        {
            return LUX_ERR;
        }

    }
    else if (0 == strcmp(buffer, "LuxTestMac_Read"))
    {
        char* options[] = {"LuxTestMac_Read",argv[3],"hw_addr","mac_addr" };
        wr = LUX_TEST_READ;
        ret = lux_fatory_test_device_info_option(wr, options);
        if (LUX_OK != ret)
        {
            return LUX_ERR;
        }

    }
    else if (0 == strcmp(buffer, "LuxTestFW_Version_Read"))
    {
        printf("++++LuxTestFW_Version_Read START++++\n");
        printf("++++LuxTestMac_ReadValue  is [%s]++++\n", APP_DETAIL_VERSION);
        printf("++++LuxTestFW_Version_Read PASS++++\n");

    }
    else if (0 == strcmp(buffer, "LuxTestReset"))
    {
        printf("++++LuxTestReset START++++\n");
        printf("++++LuxTestReset PASS++++\n");
    }
    else if (0 == strcmp(buffer, "LuxTestProduceDate_Write"))
    {
        char* options[] = {"LuxTestProduct_SN_Write",argv[3],"produce_info","product_date" };
        wr = LUX_TEST_WRITE;
        ret = lux_fatory_test_device_info_option(wr, options);
        if (LUX_OK != ret)
        {
            return LUX_ERR;
        }
    }
    else if (0 == strcmp(buffer, "LuxTestProduceDate_Read"))
    {
        char* options[] = {"LuxTestProduceDate_Read",argv[3],"produce_info","product_date" };
        wr = LUX_TEST_READ;
        ret = lux_fatory_test_device_info_option(wr, options);
        if (LUX_OK != ret)
        {
            return LUX_ERR;
        }

    }
    else if (0 == strcmp(buffer, "LuxTestHW_Version_Read"))
    {
        char* options[] = {"LuxTestHW_Version_Read",argv[3],"produce_info","hw_version" };
        wr = LUX_TEST_READ;
        ret = lux_fatory_test_device_info_option(wr, options);
        if (LUX_OK != ret)
        {
            return LUX_ERR;
        }

    }
    else if (0 == strcmp(buffer, "LuxTestHW_Version_Write"))
    {
        char* options[] = {"LuxTestHW_Version_Write",argv[3],"produce_info","hw_version" };
        wr = LUX_TEST_WRITE;
        ret = lux_fatory_test_device_info_option(wr, options);
        if (LUX_OK != ret)
        {
            return LUX_ERR;
        }
    }
    else if (0 == strcmp(buffer, "LuxTest_MainPCBA_SN_Write"))
    {
        char* options[] = {"LuxTest_MainPCBA_SN_Write",argv[3],"produce_info","main_pcba_sn" };
        wr = LUX_TEST_WRITE;
        ret = lux_fatory_test_device_info_option(wr, options);
        if (LUX_OK != ret)
        {
            return LUX_ERR;
        }
    }
    else if (0 == strcmp(buffer, "LuxTest_MainPCBA_SN_Read"))
    {
        char* options[] = {"LuxTest_MainPCBA_SN_Read",argv[3],"produce_info","main_pcba_sn" };
        wr = LUX_TEST_READ;
        ret = lux_fatory_test_device_info_option(wr, options);
        if (LUX_OK != ret)
        {
            return LUX_ERR;
        }
    }
    else if (0 == strcmp(buffer, "LuxTest_SensorPCBA_SN_Write"))
    {
        char* options[] = { "LuxTest_SensorPCBA_SN_Write",argv[3],"produce_info","sensor_pcba_sn" };
        wr = LUX_TEST_WRITE;
        ret = lux_fatory_test_device_info_option(wr, options);
        if (LUX_OK != ret)
        {
            return LUX_ERR;
        }
    }
    else if (0 == strcmp(buffer, "LuxTest_SensorPCBA_SN_Read"))
    {
        char* options[] = { "LuxTest_SensorPCBA_SN_Read",argv[3],"produce_info","sensor_pcba_sn" };
        wr = LUX_TEST_READ;
        ret = lux_fatory_test_device_info_option(wr, options);
        if (LUX_OK != ret)
        {
            return LUX_ERR;
        }
    }
    else
    {
        printf("++++Incorrect command input!++++\n");
    }

    return 0;
}

