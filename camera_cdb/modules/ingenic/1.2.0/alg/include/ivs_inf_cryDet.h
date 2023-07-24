#ifndef __IVS_INF_CRYDET_H__
#define __IVS_INF_CRYDET_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#include "ivs_common.h"

/*
 * 车辆检测输入结构体
 */
typedef struct {
    int skip_num; /**< 跳帧数目 */
    IVSFrameInfo frameInfo; /**< 帧信息 */
    int stride;/**< 隔几秒检测一次 max=5 */
    int numPerFrm;/**< 一帧数据*/
}crydet_param_input_t;

/*
 * 车辆检测输出结构体
 */
typedef struct {
	float confidence;/**< 检测结果的置信度 */
    int64_t timeStamp; /**< 时间戳 */
	int ret;
}crydet_param_output_t;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* __IVS_INF_CRYDET_H__ */
