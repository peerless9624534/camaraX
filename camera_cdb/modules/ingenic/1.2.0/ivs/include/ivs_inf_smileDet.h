#ifndef __IVS_INF_SMILEDET_H__
#define __IVS_INF_SMILEDET_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#include "ivs_common.h"

#define NUM_OF_FACES 10


/*
 * 人脸信息结构体
 */
typedef struct {
    IVSRect box; /**< 人脸区域坐标,用来做人脸识别 */
    IVSRect show_box; /**< 人脸区域真实坐标 */
    float confidence; /**< 人脸检测结果的置信度 */
    float expression_score; /**< 微笑得分 1:微笑 0:不笑 */
}smile_face_info;

/*
 * 人脸检测输入结构体
 */
typedef struct {
    unsigned int max_face_box; /**< 人脸检测处理过程中保留的框数量 */
    float scale_factor; /**< 图像缩放倍数: new_size = ori_size * scale_factor */
    int skip_num; /**< 跳帧数目 */
    unsigned int delay; /**< 延时识别时间，默认为0 */
    IVSFrameInfo frameInfo; /**< 帧信息 */
    bool rot90; /**< 图像是否顺时针旋转90度*/
}smiledet_param_input_t;

/*
 * 人脸检测输出结构体
 */
typedef struct {
    int count; /**< 识别出人脸的个数 */
    smile_face_info face[NUM_OF_FACES]; /**< 识别出的人脸信息 */
    int64_t timeStamp; /**< 时间戳 */
}smiledet_param_output_t;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* __IVS_INF_SMILEDET_H__ */
