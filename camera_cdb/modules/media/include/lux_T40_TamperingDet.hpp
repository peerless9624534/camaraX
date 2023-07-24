#ifndef __LUX_T40_TAMPERINGDET_HPP__
#define __LUX_T40_TAMPERINGDET_HPP__

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

typedef struct ObjectBox
{
    float x1;
    float y1;
    float x2;
    float y2;

    ObjectBox()
    {
        x1 = 0.0f;
        y1 = 0.0f;
        x2 = 0.0f;
        y2 = 0.0f;
    }
} ObjectBox;

typedef struct
{
    unsigned char *image;
    int w;
    int h;
} input_info_t;

typedef struct
{
    unsigned int cover_th; // 遮挡阈值，默认60，可修改，最大值100
    unsigned int sense;      // 灵敏度
} tamper_param_input_t;

typedef enum
{
    TAMPER_TYPE_NO_DECETED,   // 未检测到篡改
    TAMPER_TYPE_MOVE,         // 移动
    TAMPER_TYPE_SPRACY_COVER, // 喷涂、遮挡
    TAMPER_TYPE_BUTTON,
} TAMPER_TYPE_EN;
typedef struct
{
    TAMPER_TYPE_EN ret; // 返回值
    vector<int> box;    // 遮挡喷涂值
} tamper_param_output_t;

// YUV  object detect
int lux_TamperingDet_init();
int lux_TamperingDet_start(input_info_t *input_now, tamper_param_input_t *p_in);
int lux_TamperingDet_start_yeshi(input_info_t *input_now, tamper_param_input_t *p_in);
int lux_TamperingDet_exit();

#endif /*__LUX_T40_TAMPERINGDET_HPP__*/
