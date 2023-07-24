#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <imp_log.h>
#include <imp_common.h>
#include <imp_system.h>
#include "func_log.h"
#include "func_video.h"
#include "config.h"
#include "lux_video.h"

//#include "xcam_general.h"
//#include "xcam_video.h"
//#ifndef ONVIF_WITH_CARRIER_SERVER
//#include "xcam_ctrl.h"
//#endif

#define LOG_TAG __FILE__

/*设置帧率*/
int func_video_set_fps(uint32_t fps_num, uint32_t fps_den)
{
	int ret = XCAM_SUCCESS;
#ifdef ONVIF_WITH_CARRIER_SERVER
	return ret;
#else
	ret = xcam_video_set_isp_fps(fps_num, fps_den);
	if(ret < XCAM_SUCCESS)
	{
		LOG_ERR(LOG_TAG,"Call xcam_video_set_fps fail.\n");
		return ret;
	}

	return ret;
#endif
}

/*获取帧率*/
int func_video_get_fps(uint32_t *fps_num, uint32_t *fps_den)
{
	int ret = XCAM_SUCCESS;
#ifdef ONVIF_WITH_CARRIER_SERVER
	return ret;
#else
	ret = xcam_video_get_isp_fps(fps_num,fps_den);
	if(ret < XCAM_SUCCESS)
	{
		LOG_ERR(LOG_TAG,"Call xcam_video_get_fps fail.\n");
		return ret;
	}

	return ret;
#endif
}

/*设置指定通道码率*/
int func_video_set_bitrate(int encChnNum, int bps_num)
{
	int ret = XCAM_SUCCESS;
#ifdef ONVIF_WITH_CARRIER_SERVER
	return ret;
#else
	ret = xcam_video_set_bitrate(encChnNum, bps_num);
	if(ret < XCAM_SUCCESS)
	{
		LOG_ERR(LOG_TAG,"Call xcam_video_set_bitrate fail.\n");
		return ret;
	}

	return ret;
#endif
}

/*获取指定通道码率*/
int func_video_get_bitrate(int encChnNum, int *bps_num)
{
	int ret = XCAM_SUCCESS;
#ifdef ONVIF_WITH_CARRIER_SERVER
	return ret;
#else
	ret = xcam_video_get_bitrate(encChnNum, bps_num);
	if(ret < XCAM_SUCCESS)
	{
		LOG_ERR(LOG_TAG,"Call xcam_video_get_bitrate.\n");
		return ret;
	}

	return ret;
#endif
}

/*设置指定通道分辨率*/
int func_video_set_resolution(int fsChnNum, int picWidth, int picHeigth)
{
	int ret = XCAM_SUCCESS;
#ifdef ONVIF_WITH_CARRIER_SERVER
	return ret;
#else
	ret = xcam_video_set_resolution(fsChnNum, picWidth, picHeigth);
	if(ret < XCAM_SUCCESS)
	{
		LOG_ERR(LOG_TAG,"Call xcam_video_set_resolution fail.\n");
		return ret;
	}

	return ret;
#endif
}

/*获取指定通道分辨率*/
int func_video_get_resoulution(int fsChnNum, int *picWidth, int *picHeigth)
{
	int ret = XCAM_SUCCESS;
#ifdef ONVIF_WITH_CARRIER_SERVER
	return ret;
#else
	ret = xcam_video_get_resolution(fsChnNum, picWidth, picHeigth);
	if(ret < XCAM_SUCCESS)
	{
		LOG_ERR(LOG_TAG,"Call xcam_video_get_resolution fail.\n");
		return ret;
	}

	return ret;
#endif
}

//设置编码方式
int func_video_set_enc_mode(int encChnNum, int enc_mode)
{
	int ret = XCAM_SUCCESS;
#ifdef ONVIF_WITH_CARRIER_SERVER
	return ret;
#else
	ret = xcam_video_set_encode_mode(encChnNum, enc_mode);
	if(ret < XCAM_SUCCESS)
	{
		LOG_ERR(LOG_TAG,"Call xcam_video_set_fps fail.\n");
		return ret;
	}

	return ret;
#endif
}

