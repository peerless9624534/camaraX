/*
 * lux_video.h
 *
 * 视频模块，包括编解码码流获取转发，主要使用编码encoder
 *
 */

#ifndef __LUX_VIDEO_H__
#define __LUX_VIDEO_H__

#include "comm_def.h"
#include "comm_func.h"
#include "lux_base.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

    //保存码流开关
    //#define SAVE_STREAM


    /**
     * @description: 请求通道I帧
     * @param [in] chnId：编码通道号
     * @return 成功：0，失败：错误码；
     */
    INT_X LUX_Video_Encoder_RequestIDR(LUX_STREAM_CHANNEL_EN channel);

    /**
     * @description: 刷新通道码流数据，丢掉旧的数据，重新开始
     * @param [in] chnId：编码通道号
     * @return 成功：0，失败：错误码；
     */
    INT_X LUX_Video_Encoder_FlushStream(LUX_STREAM_CHANNEL_EN channel);

    /**
     * @description: 获取编码帧
     * @param [in] chnId：编码通道号
     * @param [out] pOutStream：输出拼包转换后的码流结构
     * @return 成功：0，失败：错误码；
     */
    INT_X LUX_Video_Encoder_GetStream(LUX_STREAM_CHANNEL_EN channel, LUX_ENCODE_STREAM_ST* pOutStream);

    /**
     * @description: 释放编码帧
     * @param [in] chnId：编码通道号
     * @param [in] pStream：输出拼包转换后的码流结构
     * @return 成功：0，失败：错误码；
     */
    INT_X LUX_Video_Encoder_ReleaseStream(LUX_STREAM_CHANNEL_EN channel, LUX_ENCODE_STREAM_ST* pStream);

    /**
     * @description: 获取编码通道参数
     * @param [in] chnId：编码通道号
     * @param [out] pVencParam：编码通道属性参数
     * @return 成功：0，失败：错误码；
     */
    INT_X LUX_Video_Encoder_GetChnParam(LUX_STREAM_CHANNEL_EN channel, LUX_VENC_CHN_PARAM_ST* pVencParam);

    /**
     * @description: 开始编码
     * @param [in] chnId：编码通道号
     * @return 成功：0，失败：错误码；
     */
    INT_X LUX_Video_Encoder_Start(LUX_STREAM_CHANNEL_EN channel);

    /**
     * @description: 停止编码
     * @param [in] chnId：编码通道号
     * @return 成功：0，失败：错误码；
     */
    INT_X LUX_Video_Encoder_Stop(LUX_STREAM_CHANNEL_EN channel);

    /**
     * @description: 使能编码模块
     * @param [void]
     * @return 成功：0，失败：错误码；
     */
    INT_X LUX_Video_Encoder_Init(INT_X Venc_Param);

    /**
     * @description: 销毁编码模块
     * @param [void]
     * @return 成功：0，失败：错误码；
     */
    INT_X LUX_Video_Encoder_Exit();

    /**
     * @description: 分辨率修改初始化
     * @param [void]
     * @return 成功：0，失败：错误码；
     */
    INT_X LUX_Video_Change_Resolution_Res_Init(int fsChnNum);

    /**
     * @description: 分辨率修改去初始化
     * @param [void]
     * @return 成功：0，失败：错误码；
     */
    INT_X LUX_Video_Change_Resolution_Res_Deinit(int fsChnNum);

    /**
     * @description: 修改分辨率
     * @param [in] value:修改的目标分辨率
     * @return 成功：0，失败：错误码；
     */
    INT_X LUX_Video_Change_Resolution(int fsChnNum, int width_value, int height_value);

    /**
     * @description: 获取系统的分辨率
     * @param [in] fsChnNum:目标通道
     * @param [out] picWidth:获取到的通道宽
     * @param [out] picHeigth:获取到的通道高
     * @return 成功：0，失败：错误码；
     */
    INT_X LUX_Video_Fnuc_Get_Resoulution(int fsChnNum, int* picWidth, int* picHeigth);

    /**
     * @description: 设置系统的帧率
     * @param [in] fps_num:帧率分母
     * @param [in] fps_den:帧率分子
     * @return 成功：0，失败：错误码；
     */
    INT_X LUX_Video_Func_Set_Fps(uint32_t impvi_num, uint32_t *fps_num, uint32_t *fps_den);

    /**
     * @description: 获取系统的帧率
     * @param [out] fps_num:获取到的分母
     * @param [out] fps_den:获取到的分子
     * @return 成功：0，失败：错误码；
     */
    INT_X LUX_Video_Func_Get_Fps(uint32_t* fps_num, uint32_t* fps_den);

    /**
     * @description: 设置系统的码率
     * @param [in] encChnNum:目标通道
     * @param [in] bps_num:目标码率
     * @return 成功：0，失败：错误码；
     */
    INT_X LUX_Video_Func_Set_Bitrate(int encChnNum, int bps_num);

    /**
     * @description: 设置系统的码率
     * @param [in] encChnNum:目标通道
     * @param [out] bps_num:获取到的码率
     * @return 成功：0，失败：错误码；
     */
    INT_X LUX_Video_Func_Get_Bitrate(int encChnNum, int* bps_num);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __LUX_VIDEO_H__ */
