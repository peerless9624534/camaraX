
#include <imp_log.h>

#include <lux_daynight.h>
#include <comm_error_code.h>
#include <lux_hal_misc.h>
#include <lux_hal_lightsns.h>
#include <lux_base.h>
#include <lux_isp.h>
#include <lux_iniparse.h>
#include <imp_isp.h>
#include "lux_config.h"
#include "lux_global.h"
#include <jwhead.h>

#define TAG         "lux_daynight"
#define TIMEINTERVAL    200 * 1000

typedef struct 
{
    BOOL_X binit;
    OSI_MUTEX mux;
    BOOL_X irled;
    INT_X lowThreshold;
    INT_X highThreshold;
    LUX_DayNight_Mode_EN    dayNightMod;
    LUX_DayNight_AutoMode_EN    autoMod;

}LUX_DayNight_INFO_ST;

static LUX_DayNight_INFO_ST g_DayNightInfo =
{
    .binit = LUX_FALSE,
    .irled = LUX_FALSE,
    .lowThreshold = 300,
    .highThreshold = 500,
    .dayNightMod = LUX_DAYNIGHT_NONE_MODE,
    .autoMod = LUX_DAYNIGHT_AUTO_MODE,
};



/**
 * @description:    设置白天黑夜模切换模式
 * @param   [in]    mode 模式
 * @return  0 成功， 非零失败
 */
INT_X LUX_DayNight_SetAutoMode(LUX_DayNight_AutoMode_EN autoMode)
{
    if(0 == g_DayNightInfo.binit)
    {
        IMP_LOG_ERR(TAG, "LUX_DayNight_SetAutoMode failed, DayNight uninitalized\n");
    }

    Thread_Mutex_Lock(&g_DayNightInfo.mux);
    g_DayNightInfo.autoMod = autoMode;
    Thread_Mutex_UnLock(&g_DayNightInfo.mux);

    return LUX_OK;
}

/**
 * @description:    设置白天黑夜模式
 * @param   [in]    mode 模式
 * @return  0 成功， 非零失败
 */
INT_X LUX_DayNight_SetMode(LUX_DayNight_Mode_EN Mode)
{
    if(0 == g_DayNightInfo.binit)
    {
        IMP_LOG_ERR(TAG, "LUX_DayNight_SetMode failed, DayNight uninitalized\n");
    }

    if(Mode > LUX_DAYNIGHT_NIGHT_MODE || Mode < LUX_DAYNIGHT_DAY_MODE)
    {
        IMP_LOG_ERR(TAG, "LUX_DayNight_SetMode failed, input Invalid parameter\n");
        return LUX_ERR;
    }

    if(g_DayNightInfo.dayNightMod == Mode)
    {
        return LUX_OK;
    }

    int ret =LUX_ERR;

    Thread_Mutex_Lock(&g_DayNightInfo.mux);

    printf("Set DayNight Mode [%d]\n", Mode);
    g_DayNightInfo.dayNightMod = Mode;

    if(LUX_DAYNIGHT_DAY_MODE == g_DayNightInfo.dayNightMod)
    {
        ret = LUX_Hal_Misc_IRLightOnOff(LUX_FALSE);
        if (LUX_OK != ret)
        {
		    IMP_LOG_ERR(TAG, "LUX_Hal_Misc_IRLightOnOff failed. \n");
	    }

        ret = LUX_ISP_SetDayDightMode(LUX_DAYNIGHT_MODE_DAY);
        if (LUX_OK != ret)
        {
		    IMP_LOG_ERR(TAG, "LUX_ISP_SetDayDightMode failed. \n");
	    }
    }
    else if(LUX_DAYNIGHT_NIGHT_MODE == g_DayNightInfo.dayNightMod)
    {
        ret = LUX_ISP_SetDayDightMode(LUX_DAYNIGHT_MODE_NIGHT);
        if (LUX_OK != ret)
        {
		    IMP_LOG_ERR(TAG, "LUX_ISP_SetDayDightMode failed. \n");
	    }
        ret = LUX_Hal_Misc_IRLightOnOff(LUX_TRUE);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_Hal_Misc_IRLightOnOff failed. \n");
        }
    }
    Thread_Mutex_UnLock(&g_DayNightInfo.mux);

    return LUX_OK;
}

/**
 * @description:    白天黑夜处理线程
 * @return  0 成功， 非零失败
 */
static void *Therad_DayNight(void *args)
{
	INT_X	ret = LUX_ERR;

#if 0
    int i = 0;
    float gb_gain, gb_gain_last = 0.0;
    float iso_buf, iso_buf_last = 0.0;
    bool ircut_status = false;
    int night_count = 0;
    int day_count = 0;
    float gb_gain_min = 200, gb_gain_max = 200;

    IMPISPRunningMode pmode;
    IMPISPEVAttr ExpAttr;
    IMPISPWB wb;
#endif
    int i = 0;
    //printf("begin read light sns... \n");
    UINT_X  dayCnt = 0;
    UINT_X  nightCnt = 0;
    INT_X   cntMax = 10;
    UINT_X  LightsnsValue = 0;

    while(g_DayNightInfo.binit)
    {
        //printf("g_DayNightInfo.autoMod:%s \n", LUX_DAYNIGHT_AUTO_MODE == g_DayNightInfo.autoMod?"auto":"manual");
        /*自动模式*/
        if (LUX_DAYNIGHT_AUTO_MODE == g_DayNightInfo.autoMod)
        {
#if 0
            ret = IMP_ISP_Tuning_GetEVAttr(&ExpAttr);
		    if (ret == 0) {
			    //printf("u32ExposureTime: %d\n", ExpAttr.ev);
		    } else {
			    return NULL;
            }
		    iso_buf = ExpAttr.ev;
		    //printf(" iso buf = %f\n",iso_buf);
		    ret = IMP_ISP_Tuning_GetWB_Statis(&wb);
		    if (ret == 0) {
			    gb_gain = wb.bgain;
			    //printf("gb_gain: %f\n", gb_gain);
			    //printf("gb_gain_min = %f, gb_gain_max = %f\n", gb_gain_min, gb_gain_max);
		    } else {
			    return NULL;
            }

		    //平均亮度小于3lux，则切到夜视模式
		    if (iso_buf >= 41528) {
			    night_count++;
			    if (night_count>5) {
				    IMP_ISP_Tuning_GetISPRunningMode(&pmode);
				    if (pmode != IMPISP_RUNNING_MODE_NIGHT) 
				    {
                        LUX_DayNight_SetMode(LUX_DAYNIGHT_NIGHT_MODE);
					    ircut_status = true;
				    }
				    //切夜视后，取10个gb_gain的的最小值，作为切换白天的参考数值gb_gain_record ，gb_gain为bayer的G/B
				    for (i = 0; i < 10; i++) 
				    {
					    IMP_ISP_Tuning_GetWB_Statis(&wb);
                        gb_gain = wb.bgain;
                        if (i == 0) {
                            gb_gain_min = gb_gain;
                            gb_gain_max = gb_gain;
                        }
                        //记录切换时bGain
                        gb_gain_min = ((gb_gain_min>gb_gain)?gb_gain:gb_gain_min);
                        //记录收敛后bGain
                        gb_gain_max = ((gb_gain_max<gb_gain)?gb_gain:gb_gain_max);
                        usleep(300000);
                        printf("gb_gain[%d]= %f, iso_buf=%f", i, gb_gain, iso_buf);
                        printf("gb_gain_min = %f, gb_gain_max = %f\n", gb_gain_min, gb_gain_max);
				    }
			    }
		    } else {
			    night_count = 0;
            }
            
		    //夜视切白天: 满足这3个条件进入白天切换判断条件
		    if (((int)iso_buf<13000) && (ircut_status == true) && (gb_gain < gb_gain_max)) 
		    {	             
                if (day_count == 0)
                {
                    gb_gain_last = gb_gain;
                    iso_buf_last = iso_buf;
                }

                //1.慢慢变亮,ev下降,bGain不变;2.开灯,ev和bGain骤降
                if ((iso_buf<12000) || (gb_gain < gb_gain_min+10)) 
		        {
                    /*
			        if (day_count 
                        && (iso_buf == iso_buf_last) 
                        && (gb_gain == gb_gain_last))
                    {  
                    }
                    else
                    */
                        day_count++;             
                }
                else 
                {
			        day_count = 0;
                }

                //夜视遮挡及放开,ev变化大,bGain变化不大
//                if (abs(iso_buf - iso_buf_last) > 5000)
//                {
//                    if (abs(gb_gain - gb_gain_last) < 5)
//                    {
//                        day_count = 0;   
//                    }
//                }
            
                gb_gain_last = gb_gain;
                iso_buf_last = iso_buf;

                printf("gb_gain_last = %f, iso_buf_last = %f\n", gb_gain_last, iso_buf_last);
                printf("day_count = %d, gb_gain = %f, iso_buf=%f", day_count, gb_gain, iso_buf);
                printf("gb_gain_min = %f, gb_gain_max = %f\n", gb_gain_min, gb_gain_max);
			    if (day_count > 6) 
			    {
				    IMP_ISP_Tuning_GetISPRunningMode(&pmode);
				    if (pmode!=IMPISP_RUNNING_MODE_DAY) 
				    {
                        LUX_DayNight_SetMode(LUX_DAYNIGHT_DAY_MODE);
					    ircut_status = false;
				    }
			    }
		    }         
		    else 
		    {
                day_count = 0;
            }
            usleep(500*1000);

#else    
#if 1
            /*开放出来给硬件测试调试*/
           // LUX_INIParse_GetInt(LUX_CONFIG_FILE, "image_config", "cntmax", &cntMax);

            /* 旧版本使用lightsensor做自动控制 */
            if(LUX_DAYNIGHT_AUTO_MODE == g_DayNightInfo.autoMod)
            {                
                LightsnsValue = LUX_Hal_LightSns_GetValue();
                if (access("/tmp/irledtest", 0) == 0)
                {
                    IMP_LOG_INFO(TAG, "light:%d\n", LightsnsValue);
                    IMP_LOG_INFO(TAG, "highThreshold:%d lowThreshold:%d\n", g_DayNightInfo.highThreshold, g_DayNightInfo.lowThreshold);
                }
                if(LightsnsValue >= g_DayNightInfo.highThreshold)
                {
                    dayCnt++;
                }
                else if(LightsnsValue >= 0 && LightsnsValue <= g_DayNightInfo.lowThreshold)
                {
                    nightCnt++;
                }
                else
                {
                    IMP_LOG_ERR(TAG, "LUX_Hal_LightSns_GetValue failed.\n");
                }
                if (access("/tmp/irledtest", 0) == 0)
                {
                    IMP_LOG_INFO(TAG, "dayCnt:%d\n", dayCnt);
                    IMP_LOG_INFO(TAG, "nightCnt:%d\n", nightCnt);
                    IMP_LOG_INFO(TAG, "---------------\n");
                }
                if(dayCnt >= cntMax)
                {
                    dayCnt = 0;
                    ret = LUX_DayNight_SetMode(LUX_DAYNIGHT_DAY_MODE);
                    if (LUX_OK != ret) 
                    {
                        MYTRACE("LUX_DayNight_SetMode failed. error number [x0%x]\n", ret);
                    }
                }
                else if(nightCnt >= cntMax)
                {
                    nightCnt = 0;
                    ret = LUX_DayNight_SetMode(LUX_DAYNIGHT_NIGHT_MODE);
                    if (LUX_OK != ret)
                    {
                        MYTRACE("LUX_DayNight_SetMode failed. error number [x0%x]\n", ret);
                    }
                }
            }
#endif
        #endif
        }
        /*手动模式*/
        else
        {
            ret = LUX_DayNight_SetMode(g_DayNightInfo.dayNightMod);
            if (LUX_OK != ret)
            {
		        IMP_LOG_ERR(TAG, "LUX_DayNight_SetMode failed. error number [x0%x]\n", ret);
            }
        }

        usleep(TIMEINTERVAL);
    }

	return (void *)LUX_OK;
}

/**
 * @description:    白天黑夜模式初始化
 * @param [in] 
 * @return  0 成功， 非零失败,返回错误码
 */
static INT_X LUX_DayNight_SetStartMod(const char* startMod)
{
    INT_X ret = LUX_ERR;

    /*自动模式*/
    if(0 == strcasecmp(startMod, "auto"))
    {
        ret = LUX_DayNight_SetAutoMode(LUX_DAYNIGHT_AUTO_MODE);
        if(LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_DayNight_SetAutoMode failed!, error num [0x%x] ", ret);
        }
    }
    /*手动白天*/
    else if(0 == strcasecmp(startMod, "off"))
    {
        ret = LUX_DayNight_SetAutoMode(LUX_DAYNIGHT_MANUAL_MODE);
        if(LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_DayNight_SetAutoMode failed!, error num [0x%x] ", ret);
        }

        ret = LUX_DayNight_SetMode(LUX_DAYNIGHT_DAY_MODE);
        if(LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_DayNight_SetMode failed!, error num [0x%x] ", ret);
        }
    }
    /*手动夜晚*/
    else
    {
        ret = LUX_DayNight_SetAutoMode(LUX_DAYNIGHT_MANUAL_MODE);
        if(LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_DayNight_SetAutoMode failed!, error num [0x%x] ", ret);
        }

        ret = LUX_DayNight_SetMode(LUX_DAYNIGHT_NIGHT_MODE);
        if(LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_DayNight_SetMode failed!, error num [0x%x] ", ret);
        }

    }

    return LUX_OK;
}

/**
 * @description:    白天黑夜模式初始化
 * @return  0 成功， 非零失败,返回错误码
 */
INT_X LUX_DayNight_Init()
{
    if (LUX_TRUE == g_DayNightInfo.binit)
    {
        IMP_LOG_ERR(TAG, "Warning DayNight uninitalized\n");
        return LUX_ERR;
    }

    INT_X ret = LUX_ERR;
    CHAR_X startMod[128] = { 0 };
    CHAR_X temp_startMod[8] = { 0 };

    fd_116 = lux_gpio_open(116);
    fd_122 = lux_gpio_open(122);
    fd_123 = lux_gpio_open(123);

    Thread_Mutex_Create(&g_DayNightInfo.mux);

    /* 配置文件读取白天黑夜切换的阈值 */
    // LUX_INIParse_GetInt(LUX_CONFIG_FILE, "image_config", "daynight_lowthreshold", &g_DayNightInfo.lowThreshold);
    // LUX_INIParse_GetInt(LUX_CONFIG_FILE, "image_config", "daynight_highthreshold", &g_DayNightInfo.highThreshold);
    g_DayNightInfo.lowThreshold  = g_configData->image_config.daynight_lowthreshold;
    g_DayNightInfo.highThreshold = g_configData->image_config.daynight_highthreshold;

    /*初始化光感*/
    ret = LUX_Hal_LightSns_Init();
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "LUX_Hal_LightSns_Init failed\n");
        return LUX_ERR;
    }

    g_DayNightInfo.binit = LUX_TRUE;

    /*根据涂鸦配置选择daynight模式*/
    // ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "image_config", "daynight" , startMod);
    // if(0 != ret)
    // {
    //     IMP_LOG_ERR(TAG, "LUX_INIParse_GetBool failed!, error num [0x%x] ", ret);
    // }
    //trim_all(startMod);
    
    //MYTRACE("night read from jio_config.ini，value = [%s]\n", startMod);
    //if (!strcasecmp(startMod, "on"))
    //{
    //    MYTRACE("current night Mode is ON\n");
    //    strcpy(temp_startMod, "2");
    //}
    //else if (!strcasecmp(startMod, "off"))
    //{
    //    MYTRACE("current night Mode is OFF\n");
    //    strcpy(temp_startMod, "1");
    //}
    //else if (!strcasecmp(startMod, "auto"))
    //{ 
    //    MYTRACE("current night Mode is Auto\n");
    //    strcpy(temp_startMod, "0");
    //}

    ret = LUX_DayNight_SetStartMod(g_configData->image_config.daynight);
    if (LUX_OK != ret)
    {
		IMP_LOG_ERR(TAG, "LUX_DayNight_SetSartMod failed\n");
		return LUX_ERR;
	}
//#endif

    ret = Task_CreateThread(Therad_DayNight, NULL);
    if (LUX_OK != ret)
    {
        IMP_LOG_ERR(TAG, "Task_CreateThread failed.\n");
        return LUX_ERR;
    }

    return LUX_OK;
}


/**
 * @description:    白天黑夜模式去初始化
 * @return  0 成功， 非零失败,返回错误码
 */
INT_X LUX_DayNight_DeInit()
{
    g_DayNightInfo.binit = LUX_FALSE;

    Thread_Mutex_Destroy(&g_DayNightInfo.mux);
    LUX_Hal_LightSns_DeInit();

    return LUX_OK;
}

