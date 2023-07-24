#ifndef __IVS_INF_PERM_H__
#define __IVS_INF_PERM_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#include <stdint.h>
#include "ivs_common.h"

#define PERM_VERSION_NUM 0x00000100
uint32_t perm_get_version_info();

#define IVS_PERM_MAX_ROI  8
#define IVS_PERM_MAX_RECT 8

/*
 * 周界信息结构体
 */
typedef struct{
  IVSPoint *p;    /**< 周界各个顶点信息，不能有线交叉，最多6个点 */
  int pcnt;      /**< 周界顶点数量 */
  int fun;       /**<功能：0：周界检测；1：热区检测;2：判断移动方向 */
  uint64_t alarm_last_time;    /**< 持续报警时间 */
}single_perm_t;


typedef struct{
    int status; /**<移动框的状态 -1：从内到外    0：方向不确定   1：从外到内  */
    int perm_index; /**与具体哪个周界相交 */

}rect_status_t;



/*
 * 周界防范算法的输入结构体
 */
typedef struct{
  single_perm_t perms[IVS_PERM_MAX_ROI];  /**< 周界信息 */
  int permcnt;                            /**< 周界数量 */
  int sense;                              /**< 灵敏度 */
  bool isSkipFrame;                       /**< 跳帧 >  */

  bool light;/**< 开灯 >  */
  float level;/**0-1,屏幕检测程度  */
  int timeon;/**开灯用时  */
  int timeoff;/**开灯间隔  */

  IVSFrameInfo frameInfo;                  /**< 帧信息 */
}perm_param_input_t;

/*
 * 周界防范算法的输出结构体
 */
typedef struct{
  int ret;                                /**< 是否出现越界 */
  int is_alarmed[IVS_PERM_MAX_ROI];     /**< 那个周界出现越界 */
  IVSRect rects[IVS_PERM_MAX_RECT];     /**< 越界物体的矩形信息 */
  int rectcnt;                            /**< 越界物体的数量 */
  rect_status_t rect_status[IVS_PERM_MAX_RECT]; /**<移动框的状态 fun=2生效 */
  int state[IVS_PERM_MAX_ROI];             /**< fun=0/1生效，state=0表示在周界内部，state=1表示在周界外部，state=2表示与周界相交 */
}perm_param_output_t;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* __IVS_INF_PERM_H__ */
