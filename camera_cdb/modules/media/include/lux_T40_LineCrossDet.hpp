#ifndef __LUX_T40_LINECROSSDET_HPP__
#define __LUX_T40_LINECROSSDET_HPP__

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

typedef struct {
    int x;
    int y;
}spot;


typedef struct
{
    unsigned char* image;
    int w;
    int h;
}input_info_t;

// YUV  object detect
int lux_LineCrossDet_init(input_info_t* input_now, vector<spot>& detresu);
int lux_LineCrossDet_start(vector<spot>& detresu, spot p1, spot p2, vector<pair<int, spot>>& track_master_preva, vector<int>& direction1);
int lux_LineCrossDet_exit();


#endif /* __LUX_T40_LINECROSSDET_HPP__ */