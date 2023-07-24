/*
 * lux_fsrc.h
 *
 * Ingenic 视频源模块
 */

#ifndef __LUX_FSRC_H__
#define __LUX_FSRC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

	// #include <imp_common.h>
	// #include <imp_osd.h>
	// #include <imp_framesource.h>
	// #include <imp_isp.h>
	// #include <imp_encoder.h>

	// #include <comm_error_code.h>
	// #include <comm_def.h>
	// #include <lux_base.h>

	// #include "comm_def.h"
	// #include "comm_func.h"


#include <stdlib.h>
#include <string.h>
#include <imp_log.h>
#include <imp_common.h>
#include <imp_framesource.h>
#include <imp_encoder.h>

#include <lux_isp.h>

#include <comm_func.h>
#include <lux_iniparse.h>

#include <lux_nv2jpeg.h>
#include "lux_base.h"



	/**
	 * @description: 获取指定通道的YUV一帧的图片数据,用于HD图像
	 * @param [in]  chnID：frameSource通道号
	 * @param [out]	picInfo	图片数据指针的大小
	 * @return 成功：0，失败：错误码；
	 */
	INT_X LUX_FSrc_DumpNV12_HD(INT_X chnID, LUX_ENCODE_STREAM_ST* picInfo);

	/**
	 * @description: 获取指定通道的YUV一帧的图片数据
	 * @param [in]  chnID：frameSource通道号
	 * @param [in]  enhanceEnable, 是否开启图像增强, 0,关闭, 1开启
	 * @return 成功：0，失败：错误码；
	 */
	INT_X LUX_FSrc_SaveDumpNV12(INT_X chnID, BOOL_X enhanceEnable);

	/**
	 * @description: 获取视频源通道图像
	 * @param [in]	chnID	通道的编号
	 * @param [out]	framedata	拷贝图像的内存指针
	 * @param [out]	frame	获取到图像信息
	 * @return 0 成功， 非零失败，返回错误码
	 */
	INT_X LUX_FSrc_DumpNV12(INT_X chnID, void* framedata, LUX_DUMP_NV12_PIC_INFO_ST* frame);

	/**
	 * @description: 视频源初始化
	 * @return 0 成功， 非零失败，返回错误码
	 */
	INT_X LUX_FSrc_Init(INT_X FsrcParam);

	/**
	 * @description: 视频源去初始化
	 * @return 0 成功， 非零失败，返回错误码
	 */
	INT_X LUX_FSrc_Exit(void);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __LUX_FSRC_H__ */
