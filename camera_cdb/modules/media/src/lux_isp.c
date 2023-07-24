/*
 * lux_isp.c
 *
 */

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <imp_log.h>
#include <imp_common.h>
#include <imp_isp.h>
#include <imp_system.h>

#include <comm_error_code.h>
#include <lux_isp.h>
#include <lux_sys.h>
#include <lux_iniparse.h>
#include <lux_hal_misc.h>
#include <lux_nv2jpeg.h>
#include <lux_global.h>
#include <jwhead.h>
#include "lux_config.h"

#define 	TAG			"lux_isp"
#define		LUX_ISP_IRCUT_TOOL_PATH		"/tmp/setir"

static LUX_ISP_INIT_ATTR_ST g_ispAttr =
{
	.dayNightMdoe = LUX_DAYNIGHT_MODE_DAY,
	.ispTurningAttr =
	{
		.turningBrightness =  128,
		.turningContrast   =  128,
		.turningSaturation =  128,
		.turningSharpness  =  128,
	},
	.poolSize = 512*1024,
	.senceMirFlip = LUX_ISP_SNS_NORMAL,

};

static LUX_ISP_SENSOR_INFO_ST g_stSensorInfo[LUX_ISP_SENSOR_NUMBNER] =
{
	{
		.name = "gc2063",
		.cbusType = TX_SENSOR_CONTROL_INTERFACE_I2C,
		.i2cAddr = 0x37,
		.i2cAdapterID = 1,
		.snsWidth = 1920,
		.snsHeight = 1080,
		.resetGPIO = GPIO_PC(27),
		.powerDownGPIO = -1,
		.powerGPIO = -1,
		.sensorID = 0,
		.videoInterface = IMPISP_SENSOR_VI_MIPI_CSI0,
		.mclk = IMPISP_SENSOR_MCLK1,
		.defaultBoot = 0,
	},
	{
		.name = "gc2053",
		.cbusType = TX_SENSOR_CONTROL_INTERFACE_I2C,
		.i2cAddr = 0x37,
		.i2cAdapterID = 1,
		.snsWidth = 1920,
		.snsHeight = 1080,
		.resetGPIO = GPIO_PC(17),
		.powerDownGPIO = -1,
		.powerGPIO = -1,
		.sensorID = 0,
		.videoInterface = IMPISP_SENSOR_VI_MIPI_CSI0,
		.mclk = IMPISP_SENSOR_MCLK1,
		.defaultBoot = 0,
	},
	{
		.name = "imx307",
		.cbusType = TX_SENSOR_CONTROL_INTERFACE_I2C,
		.i2cAddr = 0x1a,
		.i2cAdapterID = 1,
		.snsWidth = 1920,
		.snsHeight = 1080,
		.resetGPIO = GPIO_PC(28),
		.powerDownGPIO = -1,
		.powerGPIO = -1,
		.sensorID = 0,
		.videoInterface = IMPISP_SENSOR_VI_MIPI_CSI0,
		.mclk = IMPISP_SENSOR_MCLK1,
		.defaultBoot = 0,
	},
	{
		.name = "sc2335",
		.cbusType = TX_SENSOR_CONTROL_INTERFACE_I2C,
		.i2cAddr = 0x30,
		.i2cAdapterID = 1,
		.snsWidth = 1920,
		.snsHeight = 1080,
		.resetGPIO = GPIO_PC(28),
		.powerDownGPIO = -1,
		.powerGPIO = -1,
		.sensorID = 0,
		.videoInterface = IMPISP_SENSOR_VI_MIPI_CSI0,
		.mclk = IMPISP_SENSOR_MCLK1,
		.defaultBoot = 0,
	},
	{
		.name = "sc4335",
		.cbusType = TX_SENSOR_CONTROL_INTERFACE_I2C,
		.i2cAddr = 0x30,
		.i2cAdapterID = 1,
		.snsWidth = 2560,
		.snsHeight = 1440,
		.resetGPIO = GPIO_PC(28),
		.powerDownGPIO = -1,
		.powerGPIO = -1,
		.sensorID = 0,
		.videoInterface = IMPISP_SENSOR_VI_MIPI_CSI0,
		.mclk = IMPISP_SENSOR_MCLK1,
		.defaultBoot = 0,
	},
	{
		.name = "sc4238",
		.cbusType = TX_SENSOR_CONTROL_INTERFACE_I2C,
		.i2cAddr = 0x30,
		.i2cAdapterID = 1,
		.snsWidth = 2560,
		.snsHeight = 1440,
		.resetGPIO = GPIO_PC(28),
		.powerDownGPIO = -1,
		.powerGPIO = -1,
		.sensorID = 0,
		.videoInterface = IMPISP_SENSOR_VI_MIPI_CSI0,
		.mclk = IMPISP_SENSOR_MCLK1,
		.defaultBoot = 0,
	},
	{
		.name = "gc4653",
		.cbusType = TX_SENSOR_CONTROL_INTERFACE_I2C,
		.i2cAddr = 0x29,
		.i2cAdapterID = 1,
		.snsWidth = 2560,
		.snsHeight = 1440,
		.resetGPIO = GPIO_PC(27),
		.powerDownGPIO = -1,
		.powerGPIO = -1,
		.sensorID = 0,
		.videoInterface = IMPISP_SENSOR_VI_MIPI_CSI0,
		.mclk = IMPISP_SENSOR_MCLK1,
		.defaultBoot = 0,
	},
	{
		.name = "imx327",
		.cbusType = TX_SENSOR_CONTROL_INTERFACE_I2C,
		.i2cAddr = 0x1a,
		.i2cAdapterID = 1,
		.snsWidth = 1920,
		.snsHeight = 1080,
		.resetGPIO = GPIO_PC(28),
		.powerDownGPIO = -1,
		.powerGPIO = -1,
		.sensorID = 0,
		.videoInterface = IMPISP_SENSOR_VI_MIPI_CSI0,
		.mclk = IMPISP_SENSOR_MCLK1,
		.defaultBoot = 0,
	},
};

static	OSI_MUTEX	g_isp_mux;

// extern int IMP_OSD_SetPoolSize(int size);

/**
 * @description: 根据sensor的名字找到对应的sensor配置参数
 * @param [in] sensorName  sensor的名字
 * @param [out] pOutSensorInfor  sensor的配置参数
 * @return 0 成功， 非零失败，返回错误码
 * @attention	未找到的异常情况,默认返回gc2053
 */
INT_X LUX_ISP_GetSensorInfo(PCHAR_X sensorName, LUX_ISP_SENSOR_INFO_ST *pOutSensorInfor)
{	
	if(NULL == sensorName || NULL == pOutSensorInfor)
	{
		IMP_LOG_ERR(TAG, "LUX_ISP_GetSensorInfo failed\n");
		return LUX_PARM_NULL_PTR;
	}

	INT_X i;
	for(i = 0; i < LUX_ISP_SENSOR_NUMBNER; i++)
	{
		if(g_stSensorInfo[i].name && !strcmp(g_stSensorInfo[i].name, sensorName))
		{
			memcpy(pOutSensorInfor, &g_stSensorInfo[i], sizeof(g_stSensorInfo[i]));
			break;
		}
	}

	/*未找到的异常情况,默认返回gc2053*/
    if(LUX_ISP_SENSOR_NUMBNER == i)
    {
        IMP_LOG_ERR(TAG, "Not found %s, Set [gc2063] as the default parameter!\n", sensorName);
		memcpy(pOutSensorInfor, &g_stSensorInfo[0], sizeof(g_stSensorInfo[0]));
    }

    return LUX_OK;
}

INT_X LUX_ISP_SetSenceFlipOther(LUX_ISP_SNS_ATTR_EN mode)
{
	MYTRACEING("LUX_ISP_SetSenceFlipOther, mode:[%d] \n", mode);
	if (mode <= LUX_ISP_SNS_NONE || mode >= LUX_ISP_SNS_BUTT)
	{
		MYERROR("LUX_ISP_SetSenceFlipOther mode parameter error\n");
		return LUX_ISP_INVALID_PARM;
	}

	INT_X  ret = LUX_OK;
	IMPISPTuningOpsMode hFlip = IMPISP_TUNING_OPS_MODE_DISABLE;
	IMPISPTuningOpsMode vFlip = IMPISP_TUNING_OPS_MODE_DISABLE;

	switch (mode)
	{
	case LUX_ISP_SNS_NORMAL:
		hFlip = IMPISP_TUNING_OPS_MODE_DISABLE;
		vFlip = IMPISP_TUNING_OPS_MODE_DISABLE;
		break;
	case LUX_ISP_SNS_MIRROR:
		hFlip = IMPISP_TUNING_OPS_MODE_ENABLE;
		vFlip = IMPISP_TUNING_OPS_MODE_DISABLE;
		break;
	case LUX_ISP_SNS_FLIP:
		hFlip = IMPISP_TUNING_OPS_MODE_DISABLE;
		vFlip = IMPISP_TUNING_OPS_MODE_ENABLE;
		break;
	case LUX_ISP_SNS_MIRROR_FLIP:
		hFlip = IMPISP_TUNING_OPS_MODE_ENABLE;
		vFlip = IMPISP_TUNING_OPS_MODE_ENABLE;
		break;
	default:
		MYERROR("LUX_ISP_SetSenceFlipOther set mode error\n");
		break;
	}

	Thread_Mutex_Lock(&g_isp_mux);
	do {
#ifdef PLATFORM_T40
		IMPISPHVFLIP flip = IMPISP_FLIP_NORMAL_MODE;
		if (hFlip && !vFlip)
		{
			flip = IMPISP_FLIP_SENSOR_H_MODE;
		}
		else if (!hFlip && vFlip)
		{
			flip = IMPISP_FLIP_SENSOR_V_MODE;
		}
		else if (hFlip && vFlip)
		{
			flip = IMPISP_FLIP_SENSOR_HV_MODE;
		}

		ret = IMP_ISP_Tuning_SetHVFLIP(IMPVI_MAIN, &flip);

		if (LUX_OK != ret)
		{
			MYERROR("IMP_ISP_Tuning_SetHVFLIP failed \n");
			break;
		}

#endif

	} while (0);
	Thread_Mutex_UnLock(&g_isp_mux);
}


/**
 * @description: 设置白天黑夜模式
 * @param [IN] dayNightMode  白天黑夜模式
 * @return 0 成功， 非零失败, 返回错误码
 */
INT_X LUX_ISP_SetDayDightMode(LUX_DAYNIGHT_MODE_EN dayNightMode)
{
	INT_X ret = LUX_ERR;

    if(LUX_DAYNIGHT_MODE_DAY == dayNightMode)
    {
        /*设置ircut是否开启*/
        ret = LUX_ISP_SetIRCUT(LUX_FALSE);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_ISP_SetIRCUT set failed\n");
            return LUX_ERR;
        }

        /*设置ISP白天黑夜模式*/
		#ifdef PLATFORM_T40
        ret = IMP_ISP_Tuning_SetISPRunningMode(IMPVI_MAIN, (IMPISPRunningMode*)&dayNightMode);
		#else
		ret = IMP_ISP_Tuning_SetISPRunningMode(dayNightMode);
		#endif
		if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "failed to set running mode\n");
            return LUX_ERR;
        }
    }
    else
	{
        /*设置ISP白天黑夜模式*/
		#ifdef PLATFORM_T40
        ret = IMP_ISP_Tuning_SetISPRunningMode(IMPVI_MAIN, (IMPISPRunningMode*)&dayNightMode);
		#else
		ret = IMP_ISP_Tuning_SetISPRunningMode(dayNightMode);
		#endif
		if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "failed to set running mode\n");
            return LUX_ERR;
        }
        usleep(300*1000);

        /*设置ircut是否开启*/
        ret = LUX_ISP_SetIRCUT(LUX_TRUE);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_ISP_SetIRCUT set failed\n");
            return LUX_ERR;
        }
    }

    return LUX_OK;
}

/**
 * @description: 获取白天黑夜模式
 * @param [out] dayNightMode  白天黑夜模式
 * @return 0 成功， 非零失败, 返回错误码
 */
INT_X LUX_ISP_GetDayDightMode(LUX_DAYNIGHT_MODE_EN *dayNightMode)
{
	if(NULL == dayNightMode)
	{
		IMP_LOG_ERR(TAG, "LUX_IMP_ISP_SetTurningAttr parameter error\n");
		return LUX_PARM_NULL_PTR;
	}

	INT_X ret = LUX_OK;
	IMPISPRunningMode tmp_dayNightMode;

	Thread_Mutex_Lock(&g_isp_mux);
	do{
		/*设置ISP白天黑夜模式*/
#ifdef PLATFORM_T40
		ret = IMP_ISP_Tuning_GetISPRunningMode(IMPVI_MAIN, &tmp_dayNightMode);
#else
		ret = IMP_ISP_Tuning_GetISPRunningMode(&tmp_dayNightMode);
#endif
  		if (LUX_OK != ret){
    		IMP_LOG_ERR(TAG, "failed to get running mode\n");
			break;
    	}

		*dayNightMode = tmp_dayNightMode;

	}while(0);
	Thread_Mutex_UnLock(&g_isp_mux);

	return ret;
}

/**
 * @description: 图像调优
 * @param [in] pIspTurningAttr  图像调优相关参数
 * @return 0 成功，非零错误
 */
INT_X LUX_IMP_ISP_SetTurningAttr(LUX_ISP_TURNING_ATTR_ST *pIspTurningAttr)
{
	if(NULL == pIspTurningAttr)
	{
		IMP_LOG_ERR(TAG, "LUX_IMP_ISP_SetTurningAttr parameter error\n");
		return LUX_PARM_NULL_PTR;
	}

    INT_X ret = LUX_OK;

	Thread_Mutex_Lock(&g_isp_mux);
	do{
#ifdef PLATFORM_T40
    	ret = IMP_ISP_Tuning_SetContrast(IMPVI_MAIN, &pIspTurningAttr->turningContrast);
#else
		ret = IMP_ISP_Tuning_SetContrast(pIspTurningAttr->turningContrast);
#endif
   		if(LUX_OK != ret){
			IMP_LOG_ERR(TAG, "IMP_ISP_Tuning_SetContrast fialed\n");
			break;
		}
		g_ispAttr.ispTurningAttr.turningContrast = pIspTurningAttr->turningContrast;

#ifdef PLATFORM_T40
   		ret = IMP_ISP_Tuning_SetSharpness(IMPVI_MAIN, &pIspTurningAttr->turningSharpness);
#else
		ret = IMP_ISP_Tuning_SetSharpness(pIspTurningAttr->turningSharpness);
#endif
		if(LUX_OK != ret){
			IMP_LOG_ERR(TAG, "IMP_ISP_Tuning_SetSharpness fialed\n");
			break;
		}
		g_ispAttr.ispTurningAttr.turningSharpness = pIspTurningAttr->turningSharpness;

#ifdef PLATFORM_T40
    	ret = IMP_ISP_Tuning_SetSaturation(IMPVI_MAIN, &pIspTurningAttr->turningSaturation);
#else
		ret = IMP_ISP_Tuning_SetSaturation(pIspTurningAttr->turningSaturation);
#endif
		if(LUX_OK != ret){
			IMP_LOG_ERR(TAG, "IMP_ISP_Tuning_SetSaturation fialed\n");
			break;
		}
		g_ispAttr.ispTurningAttr.turningSaturation = pIspTurningAttr->turningSaturation;

#ifdef PLATFORM_T40
    	ret = IMP_ISP_Tuning_SetBrightness(IMPVI_MAIN, &pIspTurningAttr->turningBrightness);
#else
		ret = IMP_ISP_Tuning_SetBrightness(pIspTurningAttr->turningBrightness);
#endif
		if(LUX_OK != ret){
			IMP_LOG_ERR(TAG, "IMP_ISP_Tuning_SetBrightness fialed\n");
			break;
		}
		g_ispAttr.ispTurningAttr.turningSharpness = pIspTurningAttr->turningBrightness;

		/*
		 * 设置参数函数不全,有待后期添加
		 */
	}while(0);
	Thread_Mutex_UnLock(&g_isp_mux);

	return ret;
}

/**
 * @description: 获取图像调优参数
 * @param [out] pIspTurningAttr  图像调优相关参数
 * @return 0 成功，非零错误
 */
INT_X LUX_IMP_ISP_GetTurningAttr(LUX_ISP_TURNING_ATTR_ST *pIspTurningAttr)
{
	if(NULL == pIspTurningAttr)
	{
		IMP_LOG_ERR(TAG, "LUX_IMP_ISP_GetTurningAttr parameter error\n");
		return LUX_PARM_NULL_PTR;
	}

    INT_X ret = LUX_OK;

	Thread_Mutex_Lock(&g_isp_mux);
	do{
#ifdef PLATFORM_T40
    	ret = IMP_ISP_Tuning_GetContrast(IMPVI_MAIN, &pIspTurningAttr->turningContrast);
#else
		ret = IMP_ISP_Tuning_GetContrast(&pIspTurningAttr->turningContrast);
#endif
    	if(LUX_OK != ret){
			IMP_LOG_ERR(TAG, "IMP_ISP_Tuning_SetContrast fialed\n");
			break;
		}

#ifdef PLATFORM_T40
    	ret = IMP_ISP_Tuning_GetSharpness(IMPVI_MAIN, &pIspTurningAttr->turningSharpness);
#else
		ret = IMP_ISP_Tuning_GetSharpness(&pIspTurningAttr->turningSharpness);
#endif
		if(LUX_OK != ret){
			IMP_LOG_ERR(TAG, "IMP_ISP_Tuning_SetSharpness fialed\n");
			break;
		}

#ifdef PLATFORM_T40
    	ret = IMP_ISP_Tuning_GetSaturation(IMPVI_MAIN, &pIspTurningAttr->turningSaturation);
#else
		ret = IMP_ISP_Tuning_GetSaturation(&pIspTurningAttr->turningSaturation);
#endif
		if(LUX_OK != ret){
			IMP_LOG_ERR(TAG, "IMP_ISP_Tuning_SetSaturation fialed\n");
			break;
		}

#ifdef PLATFORM_T40
    	ret = IMP_ISP_Tuning_GetBrightness(IMPVI_MAIN, &pIspTurningAttr->turningBrightness);
#else
		ret = IMP_ISP_Tuning_GetBrightness(&pIspTurningAttr->turningBrightness);
#endif
		if(LUX_OK != ret){
			IMP_LOG_ERR(TAG, "IMP_ISP_Tuning_SetBrightness fialed\n");
			break;
		}

	    /*
	 	 * 设置参数函数不全,有待后期添加
	 	 */

	}while(0);
	Thread_Mutex_UnLock(&g_isp_mux);

	return ret;
}

/**
 * @description: 获取图像翻参数
 * @param [out] mode 获取图像翻转参数
 * @return 0 成功，非零错误, 返回错误码
 */
INT_X LUX_ISP_GetSenceFlip(LUX_ISP_SNS_ATTR_EN *mode)
{
	INT_X ret = LUX_OK;
	IMPISPTuningOpsMode hFlip = IMPISP_TUNING_OPS_MODE_DISABLE;
	IMPISPTuningOpsMode vFlip = IMPISP_TUNING_OPS_MODE_DISABLE;

	Thread_Mutex_Lock(&g_isp_mux);
	do{	
#ifdef PLATFORM_T31
		/*获取画面水平翻转*/
		ret = IMP_ISP_Tuning_GetISPHflip(&hFlip);
		if(LUX_OK != ret){
			IMP_LOG_ERR(TAG, "IMP_ISP_Tuning_GetISPHflip failed\n");
			break;
		}

		/*设置画面垂直翻转*/
		ret = IMP_ISP_Tuning_GetISPVflip(&vFlip);
		if(LUX_OK != ret){
			IMP_LOG_ERR(TAG, "IMP_ISP_Tuning_GetISPVflip failed\n");
			break;
		}
#else
		IMPISPHVFLIP flip;
		ret = IMP_ISP_Tuning_GetHVFlip(IMPVI_MAIN, &flip);
		if (LUX_OK != ret)
		{
			IMP_LOG_ERR(TAG, "IMP_ISP_Tuning_GetHVFlip failed\n");
			break;
		}

		switch (flip)
		{
		case IMPISP_FLIP_NORMAL_MODE:	
		{
			hFlip = IMPISP_TUNING_OPS_MODE_DISABLE;
			vFlip = IMPISP_TUNING_OPS_MODE_DISABLE;
		}
		break;

		case IMPISP_FLIP_H_MODE:
		case IMPISP_FLIP_SENSOR_H_MODE:
		{
			hFlip = IMPISP_TUNING_OPS_MODE_ENABLE;
			vFlip = IMPISP_TUNING_OPS_MODE_DISABLE;
		}
		break;

		case IMPISP_FLIP_V_MODE:
		case IMPISP_FLIP_SENSOR_V_MODE:
		{
			hFlip = IMPISP_TUNING_OPS_MODE_DISABLE;
			vFlip = IMPISP_TUNING_OPS_MODE_ENABLE;
		}
		break;
		case IMPISP_FLIP_HV_MODE:
		case IMPISP_FLIP_SENSOR_HV_MODE:
		{
			hFlip = IMPISP_TUNING_OPS_MODE_ENABLE;
			vFlip = IMPISP_TUNING_OPS_MODE_ENABLE;
		}
		break;
		default:
		{
			hFlip = IMPISP_TUNING_OPS_MODE_DISABLE;
			vFlip = IMPISP_TUNING_OPS_MODE_DISABLE;
		}
		break;

		}
#endif

		/*水平翻转 +1，垂直翻转 +2，水平垂直翻转 +3*/
		*mode = (LUX_ISP_SNS_ATTR_EN)(hFlip + (vFlip * 2) + 1);

	}while(0);
	Thread_Mutex_UnLock(&g_isp_mux);

	return ret;
}

/**
 * @description: 图像翻转功能设置
 * @param [in] mode 水平、垂直翻转属性
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_ISP_SetSenceFlip(LUX_ISP_SNS_ATTR_EN mode)
{
	MYTRACEING("LUX_ISP_SetSenceFlip, mode:[%d] \n", mode);
	if(mode <= LUX_ISP_SNS_NONE || mode >= LUX_ISP_SNS_BUTT)
	{
		IMP_LOG_ERR(TAG, "LUX_ISP_SetSenceFlip mode parameter error\n");
		return LUX_ISP_INVALID_PARM;
	}

	INT_X  ret = LUX_OK;
	IMPISPTuningOpsMode hFlip = IMPISP_TUNING_OPS_MODE_DISABLE;
	IMPISPTuningOpsMode vFlip = IMPISP_TUNING_OPS_MODE_DISABLE;

	switch (mode)
	{
		case LUX_ISP_SNS_NORMAL:
			hFlip = IMPISP_TUNING_OPS_MODE_DISABLE;
			vFlip = IMPISP_TUNING_OPS_MODE_DISABLE;
			break;
		case LUX_ISP_SNS_MIRROR:
			hFlip = IMPISP_TUNING_OPS_MODE_ENABLE;
			vFlip = IMPISP_TUNING_OPS_MODE_DISABLE;
			break;
		case LUX_ISP_SNS_FLIP:
			hFlip = IMPISP_TUNING_OPS_MODE_DISABLE;
			vFlip = IMPISP_TUNING_OPS_MODE_ENABLE;
			break;
		case LUX_ISP_SNS_MIRROR_FLIP:
			hFlip = IMPISP_TUNING_OPS_MODE_ENABLE;
			vFlip = IMPISP_TUNING_OPS_MODE_ENABLE;
			break;
		default:
			IMP_LOG_ERR(TAG, "LUX_ISP_SetSenceFlip set mode error\n");
			break;
	}

	Thread_Mutex_Lock(&g_isp_mux);
	do{
		/*设置画面水平翻转*/
#ifdef PLATFORM_T31
		ret = IMP_ISP_Tuning_SetISPHflip(hFlip);
		if (LUX_OK != ret) {
			IMP_LOG_ERR(TAG, "IMP_ISP_Tuning_SetHVFLIP failed\n");
			break;
		}
#else
		IMPISPHVFLIP flip = IMPISP_FLIP_H_MODE;
		//ret = IMP_ISP_Tuning_SetHVFLIP(IMPVI_MAIN, &flip);
#endif

		/*设置画面垂直翻转*/
#ifdef PLATFORM_T31
		ret = IMP_ISP_Tuning_SetISPVflip(vFlip);
		if (LUX_OK != ret) {
			IMP_LOG_ERR(TAG, "IMP_ISP_Tuning_SetHVFLIP failed\n");
			break;
		}
#else
		flip = IMPISP_FLIP_V_MODE;
		//ret = IMP_ISP_Tuning_SetHVFLIP(IMPVI_MAIN, &flip);
#endif

#ifdef PLATFORM_T40
		flip = IMPISP_FLIP_NORMAL_MODE;
		if (hFlip && !vFlip)
		{
			flip = IMPISP_FLIP_H_MODE;
		}
		else if (!hFlip && vFlip)
		{
			flip = IMPISP_FLIP_V_MODE;
		}
		else if (hFlip && vFlip)
		{
			flip = IMPISP_FLIP_HV_MODE;
		}

		ret = IMP_ISP_Tuning_SetHVFLIP(IMPVI_MAIN, &flip);

		if (LUX_OK != ret) 
		{
			MYERROR("IMP_ISP_Tuning_SetHVFLIP failed \n");
			break;
		}

#endif
		
	}while(0);
	Thread_Mutex_UnLock(&g_isp_mux);

	return ret;
}

/*待后期更改*/
IMPSensorInfo sensor_info;


/**
 * @description: ISP模块初始化
 * @return 0 成功，非零失败，返回错误码
 */
int LUX_ISP_Init()
{
	INT_X ret = LUX_ERR;
	CHAR_X sensorName[64] = {0};
	LUX_ISP_SENSOR_INFO_ST sensorInfoTmp;

	// IMP_OSD_SetPoolSize(g_ispAttr.poolSize);
	IMP_ISP_Tuning_SetOsdPoolSize(g_ispAttr.poolSize);

	/*读取配置文件的sensor名字*/
	strcpy(sensorName, g_sensor_name);
	//ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "sensor", "name", sensorName);
	//if(LUX_OK != ret){
	//	IMP_LOG_ERR(TAG, "LUX_INIParse_GetString failed\n");
	//}

	/*读取sensor配置信息*/
	LUX_ISP_GetSensorInfo(sensorName, &sensorInfoTmp);

	memset(&sensor_info, 0, sizeof(IMPSensorInfo));
	strcpy(sensor_info.name, sensorInfoTmp.name);
	strcpy(sensor_info.i2c.type, sensorInfoTmp.name);
	sensor_info.cbus_type = sensorInfoTmp.cbusType;
	sensor_info.i2c.addr = sensorInfoTmp.i2cAddr;

#ifdef PLATFORM_T40
	sensor_info.i2c.i2c_adapter_id = sensorInfoTmp.i2cAdapterID;
	sensor_info.rst_gpio = sensorInfoTmp.resetGPIO;
	sensor_info.power_gpio = sensorInfoTmp.powerGPIO;
	sensor_info.pwdn_gpio = sensorInfoTmp.powerDownGPIO;
	sensor_info.sensor_id = sensorInfoTmp.sensorID;
	sensor_info.video_interface = sensorInfoTmp.videoInterface;
	sensor_info.mclk = sensorInfoTmp.mclk;
#if 1
	if (g_configData->image_config.wdr_onoff)
	{
		sensor_info.default_boot = 1;
	}
	else
	{
		sensor_info.default_boot = sensorInfoTmp.defaultBoot;
	}
#else
	sensor_info.default_boot = sensorInfoTmp.defaultBoot;
#endif
#endif
    printf("LUX_ISP_Init start\n");

    ret = Thread_Mutex_Create(&g_isp_mux);
	if(LUX_OK != ret){
		IMP_LOG_ERR(TAG, "Thread_Mutex_Create failed\n");
		return LUX_ERR;
	}

	ret = IMP_ISP_Open();
	if(LUX_OK != ret){
		IMP_LOG_ERR(TAG, "failed to open ISP\n");
		return LUX_ERR;
	}

#ifdef PLATFORM_T40
#if 1
	if (g_configData->image_config.wdr_onoff)
	{
		//启用宽动态功能
		IMPISPTuningOpsMode opsMode = IMPISP_TUNING_OPS_MODE_ENABLE;
		MYTRACEING("wdr enable \n");
		if (0 != IMP_ISP_WDR_ENABLE(IMPVI_MAIN, &opsMode))
		{
			MYERROR("IMP_ISP_WDR_ENABLE failed! \n");
		}
	}
#endif
    ret = IMP_ISP_AddSensor(IMPVI_MAIN, &sensor_info);
#else
	ret = IMP_ISP_AddSensor(&sensor_info);
#endif
	if(LUX_OK != ret){
        IMP_LOG_ERR(TAG, "failed to AddSensor :%s\n", sensor_info.name);
        return LUX_ERR;
	}

#ifdef PLATFORM_T40
    ret = IMP_ISP_EnableSensor(IMPVI_MAIN, &sensor_info);
#else
	ret = IMP_ISP_EnableSensor();
#endif
	if(LUX_OK != ret){
		IMP_LOG_ERR(TAG, "failed to EnableSensor\n");
		return LUX_ERR;
	}

    ret = LUX_SYS_Init();
	if(LUX_OK != ret){
		IMP_LOG_ERR(TAG, "IMP_System_Init failed\n");
		return LUX_ERR;
	}

	/* enable turning, to debug graphics */
	ret = IMP_ISP_EnableTuning();
	if(LUX_OK != ret){
		IMP_LOG_ERR(TAG, "IMP_ISP_EnableTuning failed\n");
		return LUX_ERR;
	}
#ifdef PLATFORM_T40
	if (g_configData->image_config.wdr_onoff)
	{
		//IMPISPWdrOutputMode outputMode = IMPISP_WDR_OUTPUT_MODE_FUS_FRAME;
		//MYTRACEING("IMP_ISP_Tuning_SetWdrOutputMode \n");
		//if (0 != IMP_ISP_Tuning_SetWdrOutputMode(IMPVI_MAIN, &outputMode))
		//{
		//	MYERROR("IMP_ISP_Tuning_SetWdrOutputMode failed \n");
		//}
		//usleep(1000*1000);
	}
#endif

	/*图像调优参数设置*/
    ret = LUX_IMP_ISP_SetTurningAttr(&g_ispAttr.ispTurningAttr);
	if(LUX_OK != ret){
		IMP_LOG_ERR(TAG, "LUX_IMP_ISP_TurningAttr faild\n");
		return LUX_ISP_INVALID_PARM;
	}

#if 0  //T40移植暂时关闭此代码  humy
	/*白天黑夜模式设置*/
	ret = LUX_ISP_SetDayDightMode(g_ispAttr.dayNightMdoe/*LUX_DAYNIGHT_MODE_NIGHT*/);
	if (LUX_OK != ret){
        IMP_LOG_ERR(TAG, "LUX_ISP_SetDayDightMode failed\n");
        return LUX_ISP_INVALID_PARM;
    }
#endif
    // ret = LUX_Jpeg_Init(LUX_STREAM_MAIN);
    // if (LUX_OK != ret)
    // {
	// 	IMP_LOG_ERR(TAG, "LUX_Jpeg_Init init failed\n");
	// 	return LUX_ERR;
	// }

	// ret = LUX_Jpeg_Init(LUX_STREAM_SUB);
    // if (LUX_OK != ret)
    // {
	// 	IMP_LOG_ERR(TAG, "LUX_Jpeg_Init init failed\n");
	// 	return LUX_ERR;
	// }
	ret = LUX_Jpeg_Init();
    	if (LUX_OK != ret)
    	{
		IMP_LOG_ERR(TAG, "LUX_Jpeg_Init init failed\n");
		return LUX_ERR;
	}

	// ret = LUX_Jpeg_Album_Init();
    // if (LUX_OK != ret)
    // {
	// 	IMP_LOG_ERR(TAG, "LUX_Jpeg_Album_Init init failed\n");
	// 	return LUX_ERR;
	// }
	IMP_LOG_DBG(TAG, "ImpSystemInit success\n");

	return LUX_OK;
}


/**
 * @description: ISP模块去初始化
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_ISP_Exit()
{
	INT_X ret = LUX_ERR;

	IMP_LOG_DBG(TAG, "LUX_ISP_Exit start\n");
	ret = LUX_Jpeg_Album_DeInit();
	if (LUX_OK != ret)
    {
		IMP_LOG_ERR(TAG, "LUX_FSrc_Exit failed\n");
		return LUX_ERR;
	}
    // ret = LUX_Jpeg_Deinit(LUX_STREAM_MAIN);
    // if (LUX_OK != ret)
    // {
	// 	IMP_LOG_ERR(TAG, "LUX_FSrc_Exit failed\n");
	// 	return LUX_ERR;
	// }
	// ret = LUX_Jpeg_Deinit(LUX_STREAM_SUB);
    // if (LUX_OK != ret)
    // {
	// 	IMP_LOG_ERR(TAG, "LUX_FSrc_Exit failed\n");
	// 	return LUX_ERR;
	// }
	ret = LUX_Jpeg_Deinit();
    	if (LUX_OK != ret)
    	{
		IMP_LOG_ERR(TAG, "LUX_FSrc_Exit failed\n");
		return LUX_ERR;
	}

	ret = Thread_Mutex_Destroy(&g_isp_mux);
    if (LUX_OK != ret) {
		IMP_LOG_ERR(TAG, "Thread_Mutex_Destroy failed: %d\n");
		return LUX_ERR;
	}

	ret = LUX_SYS_Exit();
	if(LUX_OK != ret){
		IMP_LOG_ERR(TAG, "failed to LUX_SYS_IMP_SystemExit\n");
		return LUX_ERR;
	}

#ifdef PLATFORM_T40
	ret = IMP_ISP_DisableSensor(IMPVI_MAIN);
#else
	ret = IMP_ISP_DisableSensor();
#endif
	if(LUX_OK != ret){
		IMP_LOG_ERR(TAG, "failed to EnableSensor\n");
		return LUX_ERR;
	}

#ifdef PLATFORM_T40
	ret = IMP_ISP_DelSensor(IMPVI_MAIN, &sensor_info);
#else
	ret = IMP_ISP_DelSensor(&sensor_info);
#endif
	if(LUX_OK != ret){
		IMP_LOG_ERR(TAG, "failed to AddSensor\n");
		return LUX_ERR;
	}

	ret = IMP_ISP_DisableTuning();
	if(LUX_OK != ret){
		IMP_LOG_ERR(TAG, "IMP_ISP_DisableTuning failed\n");
		return LUX_ERR;
	}

	if(IMP_ISP_Close()){
		IMP_LOG_ERR(TAG, "failed to open ISP\n");
		return LUX_ERR;
	}

	IMP_LOG_DBG(TAG, " LUX_ISP_Exit success\n");

	return LUX_OK;
}
