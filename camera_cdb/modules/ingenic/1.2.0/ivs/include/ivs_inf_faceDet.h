#ifndef __IVS_INF_FACEDET_H__
#define __IVS_INF_FACEDET_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#include "ivs_common.h"


#define FACEDET_VERSION_NUM 0x00000200
uint32_t facedet_get_version_info();

#define NUM_OF_FACES 50

/*
 * 人脸信息角度结构体
 */
typedef struct {
    float yaw;
    float pitch;
    float roll;
}face_pose;

/*
 * 人脸信息结构体
 */
typedef struct {
    IVSRect box; /**< reserved */
    IVSRect show_box; /**< 人脸区域真实坐标 */
    int class_id; /**< 类别号 0: 人脸>*/
    int track_id; /**< 跟踪ID>*/

    face_pose face_pose_res; /**< reserved **/
    float confidence; /**< 人脸检测结果的置信度 */
    float blur_score; /**< reserved */
    int landmark[10]; /** reserved */
    int landmark10[20]; /** reserved */
    float faceLivenessScore; /**<  reserved */
}face_info;

/*
 * 人脸检测输入结构体
 */
typedef struct {
    int nmem_size; /**< 可配置的nmem size 单位:字节，配置0会默认分配所有nmem内存.*/
    int share_mem_mode; /**< 共享内存模式，目前支持0和4; 0: DEFAULT 方式. 4: SMART_REUSE_MEM方式,更省nmem内存.*/
    int ptime; /**< 1: 打印算法处理时间信息，用于debug; 0 :不打印处理时间.*/
    const char* model_path;
    unsigned int max_face_box; /**< 人脸检测处理过程中保留的框数量. */
    int sense; /**< 检测灵敏度 0~3 0:最不灵敏 3:最灵敏 default:1 */
    int detdist; /**<  检测距离 0~3 default:3 */
    int skip_num; /**< 跳帧数目 */
    IVSFrameInfo frameInfo; /**< 帧信息 */
    bool switch_track; /**< 取值true/false; true :开启跟踪，track_id 会输出跟踪ID号，false: 关闭跟踪, track_id=-1 为无效状态. */
    bool rot90; /**< reserved*/
    bool switch_liveness; /**< reserved*/
    bool switch_face_pose; /**< reserved */
    bool switch_face_blur; /**< reserved*/
    bool switch_landmark;    /** < reserved  */
}facedet_param_input_t;

/*
 * 人脸检测输出结构体
 */
typedef struct {
    int count; /**< 识别出人脸的个数 */
    face_info face[NUM_OF_FACES]; /**< 识别出的人脸信息 */
    int64_t timeStamp; /**< 时间戳 */
}facedet_param_output_t;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* __IVS_INF_FACEDET_H__ */
