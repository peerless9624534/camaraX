#ifndef __IVS_INF_MOVE_H__
#define __IVS_INF_MOVE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#include "ivs_common.h"

#define NUM_OF_RECTS 8
#define MAX_DET_NUM 200
/*
 * 高级移动侦测算法的输入结构体
 */

typedef struct{
    IVSPoint det_rects[MAX_DET_NUM];/**< 检测出移动区域的宫格坐标 */
    int area[MAX_DET_NUM];/**< 移动区域的area */
}move_g;
    
typedef struct {
  int sense; /**< 高级移动侦测的灵敏度，范围为0-4 */

  int min_h; /**< 高级移动侦测物体的最小高度 */
  int min_w; /**< 高级移动侦测物体的最小宽度 */

  IVSRect* rois; /**< 高级移动侦测待检测的区域信息 */
  int cntRoi; /**< 高级移动侦测待检测区域的数量 */

  int isSkipFrame; /**< 高级移动侦测跳帧处理开关 */

  int det_w; /**<宫格最小单元宽度*/
  int det_h; /**<宫格最小单元高度 */

  bool light; /**< 高级移动侦测光照处理开关 */
  float level;/**0-1,屏幕检测程度  */
  int timeon;/**开灯用时  */
  int timeoff;/**开灯间隔  */

  IVSFrameInfo frameInfo; /**< 帧信息 */
}move_param_input_t;

/*
 * 高级移动侦测算法的输出结构体
 */
typedef struct {
  int ret;		/**< 是否检测出移动区域 */
  int count; /**< 检测出移动区域的数量 */
  IVSRect rects[NUM_OF_RECTS]; /**< 检测出移动区域的信息 */
  int blockSize; /**< 检测出移动区域块大小 */
  IVSPoint *pt; /**< 移动区域左上角点坐标 */

  move_g g_res; /**< 检测出移动区域的宫格信息 */
  int detcount; /**< 检测出移动区域的宫格数量 */
}move_param_output_t;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* __IVS_INF_MOVE_H__ */
