#ifndef __OBJECT_DETECT_HPP__
#define __OBJECT_DETECT_HPP__
// #ifndef __T40_UMDet_opencv_v230228_HPP__
// #define __T40_UMDet_opencv_v230228_HPP__
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <unistd.h>

using namespace std;
using namespace cv;

typedef struct
{
    float x1;
    float y1;
    float x2;
    float y2;
} UM_ObjectBox;

typedef struct
{
    unsigned char *image;
    int w;
    int h;
} um_input_info_t;

struct top_contour
{
    int sid;
    unsigned int start_time;
    unsigned int last_time;
};

typedef struct
{
    um_input_info_t input_org;   // 初始化获取的帧信息
    int ts_s;            // 帧的时间戳，单位S
    unsigned char gray_scale;           // 灰度图分割阈值0-255
    unsigned int maxdet_time;          // 告警的计时阈值，单位S
    unsigned int min_area;             // 检测的最小面积
    unsigned int max_area;             // 检测的最大面积
    unsigned int cons_count;           // 触发开始计时的阈值
} objdet_param_input_t;

typedef vector<UM_ObjectBox> objdet_param_output_t;

// YUV  object detect
int lux_UMDet_init(um_input_info_t *input_org);
int lux_UMDet_start(um_input_info_t *input_now, objdet_param_input_t *input_param, objdet_param_output_t &out_result);
int lux_UMDet_exit();

#endif /* __OBJECT_DETECT_HPP__ */