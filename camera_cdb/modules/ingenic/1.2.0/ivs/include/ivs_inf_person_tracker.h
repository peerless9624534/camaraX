#ifndef __IVS_INF_PERSON_TRACKER_H__
#define __IVS_INF_PERSON_TRACKER_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#include "ivs_common.h"

/*
 * 移动跟踪算法的输入结构体
 */
typedef struct {
  IVSFrameInfo frameInfo;   /**< 帧信息 */
  int stable_time_out;  /**< 稳定期最大间隔 */
  int move_sense;      /**< 检测灵敏度 0-4 ascend*/
  int obj_min_width;   /**< 最小检测目标大小宽度*/
  int obj_min_height;  /**< 最小检测目标大小高度*/

  int move_thresh_denoise; /**< 降噪处理阈值*/
  int add_smooth; /**< whether open the denoise process*/
  float conf_thresh; /**<跟踪约束，跟踪置信度变化幅度阈值，大于该阈值跳出检测状态，[0,1]*/
  float conf_lower; /**<跟踪约束，跟踪置信度最小阈值，小于该阈值跳出检测状态，[0,1]*/
  int frozen_in_boundary; /**<在左右边界处物体连续移动帧数小于该阈值，保持静止状态*/
  float boundary_ratio; /**<左/右边界占图像宽度比例，[0,1]*/
  int is_feedback_motor_status; /**< 电机回馈信息*/
  int is_motor_stop;            /**< 电机停止状态*/
  bool enable_move;           /**< 使能移动跟踪,在人形检测失效时起作用; 当 mode = 3 时 使能移动检测,输出移动人形结果*/
    int mode;    /**< 设定跟踪模式 0 : 优先跟踪移动人形目标; 1 : 优先跟踪主要人形目标 2:  3: 检测模式,检测移动和人形目标, 4:优先跟踪人形目标,其次跟踪移动目标*/
    float area_ratio; /**< 面积对比系数 0.0 ~ 1 , 仅在mode=2生效*/
    float exp_x;    /**<*设定有效跟踪区域系数 仅在X轴生效,有效区域计算方法: -frame_width / 2 * exp_x  ~ frame_width / 2 * exp_x */
    int detdist; /**< 检测距离 0~4  0:6米 max(img_w, img_h) >= 320 ;  1:8米  max(img_w, img_h) >= 416 \
                    2:10米 max(img_w, img_h) >= 512   3:11米 max(img_w, img_h) >= 640 \
                    4:13米  max(img_w, img_h) >= 800 default:2 */
    int sense; /**< 检测灵敏度 0~5 0:最不灵敏 5:最灵敏 default:4 */
    bool ptime;
    int check_freq;

}person_tracker_param_input_t;

/*
 * 移动跟踪算法的输出结构体
 */
typedef struct {
  int count;		/**< 检测到的矩形框目标个数 */
  IVSRect rect[20];	/**< 检测到的目标矩形框 */

  int move_count;		/**< 当 mode = 3 时,检测到的移动目标个数 */
  IVSRect move_rect[20];	/**< 当 mode = 3 时, 检测到的移动目标矩形框 */

  int person_count;		/**< 当 mode = 3 时, 检测到的人形目标个数 */
  IVSRect person_rect[20];	/**< 当 mode = 3 时, 检测到的人形矩形框 */
    int person_status;     /** -1 : 表示初始化中  ; 0 : 表示画面中没有检测到人形目标; 1 : 表示画面中有人形目标*/
    int target_type;      /** 目标类型 只在跟踪模式下生效 0: 移动目标 ; 1: 人形目标 -1: 初始状态 */
  int dx;              /**< 检测到的目标dx */
  int dy;        /**< 检测到的目标dy */
  int step;   /**< 检测到的目标step */
  int capture_lost; /**< 何种状态下跟踪失效 */
  int tracking;  /**< 是否处于跟踪状态 */
  int64_t timeStamp; /**<时间戳*/
} person_tracker_param_output_t;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* __IVS_INF_TRACKER_H__ */

