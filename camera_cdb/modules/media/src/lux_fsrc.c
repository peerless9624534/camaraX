/*
 * lux_fsrc.c
 *
 *基于Ingenic封装的framesource模块
 *
 */
#include <stdlib.h>
#include <string.h>
#include <imp_log.h>
#include <imp_common.h>
#include <imp_framesource.h>
#include <imp_encoder.h>

#include <lux_isp.h>
#include "lux_fsrc.h"
#include <comm_func.h>
#include <lux_iniparse.h>
#include <lux_global.h>
#include "lux_config.h"

#include <lux_nv2jpeg.h>
#include <jwhead.h>


#define TAG "lux_fs"

 // /*视频源模块通道属性*/
 // typedef struct
 // {
 //     UINT_X index; //0 for main channel ,1 for second channel
 //     UINT_X enable;
 //     OSI_MUTEX mux;
 //     IMPFSChnAttr fsChnAttr;
 // } LUX_FSRC_CHN_ATTR_ST;

 // /*视频源模块属性*/
 // typedef struct
 // {
 //     BOOL_X bInit;
 //     LUX_FSRC_CHN_ATTR_ST fsChn[FS_CHN_NUM];
 // } LUX_FSRC_ATTR_ST;
LUX_FSRC_ATTR_ST* g_framSrcAttr;
#if 0
LUX_FSRC_ATTR_ST g_framSrcAttr =
{
	.bInit = LUX_FALSE,
	.fsChn =
	{
		{
			/* 通道0（主码流）视频源参数 */
			.index = CH0_INDEX,
			.enable = CHN0_EN,
			.fsChnAttr =
			{
				.pixFmt = PIX_FMT_NV12,
				.picWidth = SENSOR_WIDTH_DEFAULT,
				.picHeight = SENSOR_HEIGHT_DEFAULT,
				.outFrmRateNum = SENSOR_FRAME_RATE_NUM,
				.outFrmRateDen = SENSOR_FRAME_RATE_DEN,
				.nrVBs = 3,
				.type = FS_PHY_CHANNEL,
		   },
		},
		{
			/* 通道1（子码流）视频源参数 */
			.index = CH1_INDEX,
			.enable = CHN1_EN,
			.fsChnAttr =
			{
				.pixFmt = PIX_FMT_NV12,
				.picWidth = SENSOR_WIDTH_SECOND,
				.picHeight = SENSOR_HEIGHT_SECOND,
				.outFrmRateNum = SENSOR_FRAME_RATE_NUM,
				.outFrmRateDen = SENSOR_FRAME_RATE_DEN,
				.nrVBs = 3,
				.type = FS_PHY_CHANNEL,

				/*	//不需要裁剪
				.crop.enable = 0,
				.crop.top = 0,
				.crop.left = 0,
				.crop.width = SENSOR_WIDTH_SECOND,
				.crop.height = SENSOR_HEIGHT_SECOND,
				*/

				.scaler.enable = LUX_TRUE,
				.scaler.outwidth = SENSOR_WIDTH_SECOND,
				.scaler.outheight = SENSOR_HEIGHT_SECOND,
		   },
		},
	}
};
#endif

/*********************************************************************************
* Tuya qrcode image enhance
* in_data  		input YUV420 ,from platform decode
* int_w			input width	,from platform decode
* in_h			input height ,from platform decode
* out_data		output YUV420 ,send in zbar
* out_w			output width ,send in zbar
* out_h			output height ,send in zabr

* binary_thres	image binary threshold，depends on different ISP para
				binary data range: 100-150, recommend 128.
* scale_flag	image scale, needed when Low-resolution images
				1: scale; 0: no scale
				if origin size 16*9, the output will be 32*18 when seting 1
				recommend original size 640*360, slower when scale to 1280*720.
**********************************************************************************/
// extern INT_X Tuya_Ipc_QRCode_Enhance(UCHAR_X *in_data, INT_X in_w, INT_X in_h, 
// 							  UCHAR_X **out_data, INT_X* out_w, INT_X* out_h, 
// 							  INT_X binary_thres, BOOL_X scale_flag);

/**
 * @description: 保存dump下来的图片数据
 * @param [in]	chnID	通道的编号
 * @param [in]	frameInfo	要保存的图片信息
 * @param [in]	enhanceEnable	是否开启QRcode图片增强，0关闭，1开启；
 * @return 0 成功， 非零失败，返回错误码
 */
static INT_X save_dumpNV12Pic(INT_X chnID, void* pOutFrameDate, LUX_DUMP_NV12_PIC_INFO_ST* frameInfo, BOOL_X enhanceEnable)
{
	if (NULL == pOutFrameDate || NULL == frameInfo)
	{
		IMP_LOG_ERR(TAG, "pOutFrameDate or frameInfo point  error.\n");
		return LUX_PARM_NULL_PTR;
	}

	FILE* fp = NULL;
	INT_X ret = LUX_ERR;
	char saveName[64] = { 0 };

	sprintf(saveName, "dump_nv12_pic_chn%d.YUV", chnID);
	fp = fopen(saveName, "wb");
	if (NULL == fp)
	{
		return LUX_ERR;
	}

	ret = fwrite(pOutFrameDate, 1, frameInfo->size, fp);
	if (ret != frameInfo->size)
	{
		IMP_LOG_ERR(TAG, "save_dumpNV12Pic write ret(%d) streamLen(%d)\n", ret, frameInfo->size);
		return LUX_ERR;
	}

	fclose(fp);

	if (enhanceEnable) {
		UCHAR_X* enhanceQRYUV = NULL;
		INT_X enhanceQRWidth = 0;
		INT_X enhanceQRHeight = 0;
		INT_X enhanceQRSize = 0;
		CHAR_X saveNameEnhance[64] = { 0 };

		sprintf(saveNameEnhance, "dump_enhance_pic_chn%d.YUV", chnID);
		/*涂鸦二维码增强*/
		// Tuya_Ipc_QRCode_Enhance(pOutFrameDate, frameInfo->width, frameInfo->height,
		// 						&enhanceQRYUV, &enhanceQRWidth, &enhanceQRHeight, 128, 0);
		// printf("enhance_qrcode_width = [%d] \n", enhanceQRWidth);
		// printf("enhance_qrcode_height = [%d] \n", enhanceQRWidth);
		// enhanceQRSize = enhanceQRWidth * enhanceQRWidth;

		// if(NULL == enhanceQRYUV)
		// {
		// 	IMP_LOG_ERR(TAG, "Tuya_Ipc_QRCode_Enhance enhanceQRYUV point error.\n");
		// 	return LUX_ERR;
		// }

		sprintf(saveName, "%s", "enhance");
		FILE* fp_h = NULL;
		fp_h = fopen(saveNameEnhance, "wb");
		if (NULL == fp_h)
		{
			return LUX_ERR;
		}

		ret = fwrite(enhanceQRYUV, 1, enhanceQRSize, fp_h);
		if (ret != enhanceQRSize)
		{
			IMP_LOG_ERR(TAG, "save_dumpNV12Pic write ret(%d) streamLen(%d)\n", ret, enhanceQRSize);
			return LUX_ERR;
		}
		fclose(fp_h);
	}

	return LUX_OK;
}

/**
 * @description: 获取指定通道的YUV一帧的图片数据,用于HD图像
 * @param [in]  chnID：frameSource通道号
 * @return 成功：0，失败：错误码；
 */
INT_X LUX_FSrc_DumpNV12_HD(INT_X chnID, LUX_ENCODE_STREAM_ST* picInfo)
{

	INT_X ret = LUX_ERR;
	LUX_DUMP_NV12_PIC_INFO_ST frameInfo;
	void* pOutFrameDate;
	if (NULL == picInfo)
	{
		IMP_LOG_ERR(TAG, "picInfo point  error.\n");
		return LUX_ERR;
	}

	pOutFrameDate = malloc(g_framSrcAttr->fsChn[chnID].fsChnAttr.picWidth *
						   g_framSrcAttr->fsChn[chnID].fsChnAttr.picHeight * 2);
	if (NULL == pOutFrameDate)
	{
		IMP_LOG_ERR(TAG, "pOutFrameDate malloc failed.\n");
		return LUX_ERR;
	}
	do
	{
		/*获取指定通道图片数据*/
		ret = LUX_FSrc_DumpNV12(chnID, pOutFrameDate, &frameInfo);
		if (LUX_OK != ret) {
			IMP_LOG_ERR(TAG, "LUX_FSrc_DumpNV12 failed, error number [x0%x]\n", ret);
			break;
		}

		ret = LUX_Jpeg_Encode(chnID, (uint8_t*)pOutFrameDate, frameInfo.size, picInfo);
		if (ret != LUX_OK)
		{
			IMP_LOG_ERR(TAG, "LUX_Jpeg_Encode Fail ret(%d)\n", ret);
			break;
		}
	} while (0);
	free(pOutFrameDate);
	return ret;
}

/**
 * @description: 获取指定通道的YUV一帧的图片数据
 * @param [in]  chnID：frameSource通道号
 * @param [in]  enhanceEnable, 是否开启图像增强, 0,关闭, 1开启
 * @return 成功：0，失败：错误码；
 */
INT_X LUX_FSrc_SaveDumpNV12(INT_X chnID, BOOL_X enhanceEnable)
{

	INT_X ret = LUX_ERR;
	LUX_DUMP_NV12_PIC_INFO_ST frameInfo;
	void* pOutFrameDate;

	pOutFrameDate = malloc(g_framSrcAttr->fsChn[chnID].fsChnAttr.picWidth *
						   g_framSrcAttr->fsChn[chnID].fsChnAttr.picHeight * 2);
	if (NULL == pOutFrameDate)
	{
		IMP_LOG_ERR(TAG, "pOutFrameDate malloc failed.\n");
		return LUX_PARM_NULL_PTR;
	}

	do
	{
		/*获取指定通道图片数据*/
		ret = LUX_FSrc_DumpNV12(chnID, pOutFrameDate, &frameInfo);
		if (LUX_OK != ret) {
			IMP_LOG_ERR(TAG, "LUX_FSrc_DumpNV12 failed, error number [x0%x]\n", ret);
			break;
		}

		printf("DAMP_NV12_PIC_INFO: frameInfo.index  = [%d]\n ", frameInfo.index);
		printf("DAMP_NV12_PIC_INFO: frameInf.height = [%d]\n ", frameInfo.height);
		printf("DAMP_NV12_PIC_INFO: frameInfo.width  = [%d]\n ", frameInfo.width);
		printf("DAMP_NV12_PIC_INFO: frameInfo.size   = [%d]\n ", frameInfo.size);

		/*保存图片*/
		ret = save_dumpNV12Pic(chnID, pOutFrameDate, &frameInfo, enhanceEnable);
		if (LUX_OK != ret) {
			IMP_LOG_ERR(TAG, "save_dumpNV12Pic failed, error number [x0%x]\n", ret);
			break;
		}
	} while (0);

	free(pOutFrameDate);
	return LUX_OK;
}

/**
 * @description: 获取视频源通道图像
 * @param [in]	chnID	通道的编号
 * @param [out]	framedata	拷贝图像的内存指针
 * @param [out]	frame	获取到图像信息
 * @return 0 成功， 非零失败，返回错误码
 */
INT_X LUX_FSrc_DumpNV12(INT_X chnID, void* framedata, LUX_DUMP_NV12_PIC_INFO_ST* frame)
{
	if (NULL == framedata || NULL == frame)
	{
		IMP_LOG_ERR(TAG, "LUX_FSrc_DumpNV12 failed!\n");
		return LUX_PARM_NULL_PTR;
	}

	INT_X ret = LUX_ERR;
	IMPFrameInfo* TempFrame = (IMPFrameInfo*)frame;

	Thread_Mutex_Lock(&g_framSrcAttr->fsChn[chnID].mux);
	ret = IMP_FrameSource_SnapFrame(g_framSrcAttr->fsChn[chnID].index,
									g_framSrcAttr->fsChn[chnID].fsChnAttr.pixFmt,
									g_framSrcAttr->fsChn[chnID].fsChnAttr.picWidth,
									g_framSrcAttr->fsChn[chnID].fsChnAttr.picHeight,
									framedata, TempFrame);
	// printf("IMP_FrameSource_SnapFrame Done\n");
	Thread_Mutex_UnLock(&g_framSrcAttr->fsChn[chnID].mux);
	if (LUX_OK != ret)
	{
		IMP_LOG_ERR(TAG, "IMP_FrameSource_SnapFrame(chn%d) faild !\n", chnID);
		return LUX_ERR;
	}

	return LUX_OK;
}

/**
 * @description: 视频源初始化
 * @return 0 成功， 非零失败，返回错误码
 */
INT_X LUX_FSrc_Init(INT_X FsrcParam)
{
	printf("=== LUX_FSrc_Init \n");
	INT_X chnID = 0;
	INT_X ret = LUX_ERR;
	BOOL_X FlipStat_h;
	BOOL_X FlipStat_v;
	LUX_ISP_SNS_ATTR_EN senceMirFlip;
	CHAR_X sensorName[64] = { 0 };
	LUX_COMM_FUNC_SNS_WH sensorWH;


	if (LUX_TRUE == g_framSrcAttr->bInit)
	{
		IMP_LOG_ERR(TAG, "LUX_FSrc_Init has been initialized!\n");
		return LUX_FSRC_ALREADY_INIT;
	}

	/*读取配置文件的sensor名字*/
	strcpy(sensorName, g_sensor_name);
	//ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "sensor", "name", sensorName);
	//if (LUX_OK != ret) {
	//	IMP_LOG_ERR(TAG, "LUX_INIParse_GetString failed,error [0x%x]\n", ret);
	//	return LUX_ERR;
	//}

	/*获取镜头分辨率大小*/
	ret = LUX_COMM_GetSensorWH(sensorName, &sensorWH);
	if (LUX_OK != ret) {
		IMP_LOG_ERR(TAG, "LUX_INIParse_GetString failed,error [0x%x]\n", ret);
		return LUX_ERR;
	}

	/*设置通道0的分辨率大小为镜头的分辨率大小*/
	if (LUX_FALSE == FsrcParam)
	{
		g_framSrcAttr->fsChn[0].fsChnAttr.picWidth = sensorWH.width;
		g_framSrcAttr->fsChn[0].fsChnAttr.picHeight = sensorWH.height;
	}

#if 0
	g_framSrcAttr->fsChn[0].fsChnAttr.picWidth = 1280;
	g_framSrcAttr->fsChn[0].fsChnAttr.picHeight = 960;
	g_framSrcAttr->fsChn[0].fsChnAttr.crop.enable = 1;
	g_framSrcAttr->fsChn[0].fsChnAttr.crop.left = (1920-1280)/2;
	g_framSrcAttr->fsChn[0].fsChnAttr.crop.top = (1080-960)/2;
	g_framSrcAttr->fsChn[0].fsChnAttr.crop.width = 1280;
	g_framSrcAttr->fsChn[0].fsChnAttr.crop.height = 960;
	g_framSrcAttr->fsChn[0].fsChnAttr.scaler.enable = 0;
	g_framSrcAttr->fsChn[0].fsChnAttr.scaler.outwidth = 1280;
	g_framSrcAttr->fsChn[0].fsChnAttr.scaler.outheight = 960;
#endif

	// else if (1 == FsrcParam)
	// {
	// 	MYTRACE("LUX_FSrc_Init --------ENTER1\n");
	// 	g_framSrcAttr->fsChn[0].fsChnAttr.picWidth = 640;
	// 	g_framSrcAttr->fsChn[0].fsChnAttr.picHeight = 360;
	// }

	for (chnID = 0; chnID < FS_CHN_NUM; chnID++) {
		if (g_framSrcAttr->fsChn[chnID].enable)
		{
			ret = IMP_FrameSource_CreateChn(g_framSrcAttr->fsChn[chnID].index,
										   &g_framSrcAttr->fsChn[chnID].fsChnAttr);
			if (LUX_OK != ret) {
				IMP_LOG_ERR(TAG, "IMP_FrameSource_CreateChn(chn%d) error !\n", chnID);
				return LUX_ERR;
			}

			ret = IMP_FrameSource_SetChnAttr(g_framSrcAttr->fsChn[chnID].index,
											&g_framSrcAttr->fsChn[chnID].fsChnAttr);
			if (LUX_OK != ret) {
				IMP_LOG_ERR(TAG, "IMP_FrameSource_SetChnAttr(chn%d) error !\n", chnID);
				return LUX_ERR;
			}

			/* 使能framesource通道，开始处理数据 */
			ret = IMP_FrameSource_EnableChn(g_framSrcAttr->fsChn[chnID].index);
			if (LUX_OK != ret)
			{
				IMP_LOG_ERR(TAG, "IMP_FrameSource_EnableChn(%d) error: %d\n",
							g_framSrcAttr->fsChn[chnID].index, ret);
				return LUX_ERR;
			}

			/*设置子码流通道帧缓存为1*/
			if (chnID == LUX_STREAM_SUB)
			{
				ret = IMP_FrameSource_SetFrameDepth(chnID, 1);
				if (LUX_OK != ret)
				{
					IMP_LOG_ERR(TAG, "IMP_FrameSource_SetFrameDepth(%d) error: %d\n", 1, ret);
					return LUX_ERR;
				}
			}

			ret = Thread_Mutex_Create(&g_framSrcAttr->fsChn[chnID].mux);
			if (LUX_OK != ret) {
				IMP_LOG_ERR(TAG, "Thread_Mutex_Create(chn%d) failed !\n", chnID);
				return LUX_ERR;
			}
		}
	}

	//设置SENSOR的初始翻转
	senceMirFlip = LUX_ISP_SNS_NORMAL;
	if (g_sensor_h_flip && !g_sensor_v_flip)
	{
		senceMirFlip = LUX_ISP_SNS_MIRROR;
	}
	else if (!g_sensor_h_flip && g_sensor_v_flip)
	{
		senceMirFlip = LUX_ISP_SNS_FLIP;
	}
	else if (g_sensor_h_flip && g_sensor_v_flip)
	{
		senceMirFlip = LUX_ISP_SNS_MIRROR_FLIP;
	}
	LUX_ISP_SetSenceFlip(senceMirFlip);

	/*图像翻转设置, 根据涂鸦配置文件设置*/
	// ret = LUX_INIParse_GetBool(LUX_CONFIG_FILE, "image_config", "vflip", &FlipStat);
	// if (0 != ret)
	// {
	// 	IMP_LOG_ERR(TAG, "LUX_INIParse_GetBool failed!, error num [0x%x] ", ret);
	// }

	FlipStat_h = g_configData->image_config.hflip;
	FlipStat_v = g_configData->image_config.vflip;

	if (g_configData->image_config.vflip)
	{
		MYERROR("LUX_INIParse_GetBool failed!, error num [0x%x] ", ret);
	}

	senceMirFlip = LUX_ISP_SNS_NORMAL;

	if ((FlipStat_h^g_sensor_h_flip) && !(FlipStat_v^g_sensor_v_flip))
	{
		senceMirFlip = LUX_ISP_SNS_MIRROR;
	}
	else if (!(FlipStat_h^g_sensor_h_flip) && (FlipStat_v^g_sensor_v_flip))
	{
		senceMirFlip = LUX_ISP_SNS_FLIP;
	}
	else if ((FlipStat_h^g_sensor_h_flip) && (FlipStat_v^g_sensor_v_flip))
	{
		senceMirFlip = LUX_ISP_SNS_MIRROR_FLIP;
	}

#if 1
	ret = LUX_ISP_SetSenceFlip(senceMirFlip);
	if (LUX_OK != ret) 
	{
		MYERROR("LUX_ISP_SetSenceFlip failed, return error[0x%x]\n", ret);
		return LUX_ISP_INVALID_PARM;
	}
#endif

	g_framSrcAttr->bInit = LUX_TRUE;

	return LUX_OK;
}


/**
 * @description: 视频源去初始化
 * @return 0 成功， 非零失败，返回错误码
 */
INT_X LUX_FSrc_Exit(void)
{
	INT_X chnID = 0;
	INT_X ret = LUX_ERR;

	if (LUX_FALSE == g_framSrcAttr->bInit)
	{
		IMP_LOG_ERR(TAG, "LUX_FSrc_Init has not been initialized!\n");
		return LUX_FSRC_NO_INIT;
	}

	for (chnID = 0; chnID < FS_CHN_NUM; chnID++)
	{
		if (g_framSrcAttr->fsChn[chnID].enable)
		{
			/*设置子码流通道帧缓存为0*/
			if (chnID == LUX_STREAM_SUB)
			{
				ret = IMP_FrameSource_SetFrameDepth(chnID, 0);
				if (LUX_OK != ret)
				{
					IMP_LOG_ERR(TAG, "IMP_FrameSource_SetFrameDepth(%d) error: %d\n", 0, ret);
					return LUX_ERR;
				}
			}

			ret = IMP_FrameSource_DisableChn(g_framSrcAttr->fsChn[chnID].index);
			if (LUX_OK != ret)
			{
				IMP_LOG_ERR(TAG, "IMP_FrameSource_DisableChn(%d) error: %d\n",
							g_framSrcAttr->fsChn[chnID].index, ret);
				return LUX_ERR;
			}

			/*Destroy channel */
			ret = IMP_FrameSource_DestroyChn(chnID);
			if (LUX_OK != ret) {
				IMP_LOG_ERR(TAG, "IMP_FrameSource_DestroyChn(%d) error: %d\n", chnID, ret);
				return LUX_ERR;
			}

			ret = Thread_Mutex_Destroy(&g_framSrcAttr->fsChn[chnID].mux);
			if (LUX_OK != ret) {
				IMP_LOG_ERR(TAG, "Thread_Mutex_Destroy(chn%d) failed.\n", chnID);
				return LUX_ERR;
			}
		}
	}

	return LUX_OK;
}
