#ifndef _LOITER_DETECT_HPP_
#define _LOITER_DETECT_HPP_

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

typedef struct
{
    float x1;
    float y1;
    float x2;
    float y2;
    float score;
    int classid;
} personBox;

struct LoiterTrack
{
    int track_id;
    vector<pair<int, int>> boxes;
    unsigned int lost_time;
    unsigned int cur_time;
    unsigned int first_time;
    unsigned int last_time;
    unsigned int movement;
    vector<pair<float, float>> rect_p1;
    vector<pair<float, float>> rect_p2;
};

struct CurBox
{
    pair<int, int> center_xy;
    pair<float, float> point_p1;
    pair<float, float> point_p2;
};

struct rectBox
{
    pair<float, float> p1;
    pair<float, float> p2;
};

typedef struct
{
    unsigned int remain_time;        // 停留时间的阈值，单位s
    unsigned int lost_time;          // 最大丢失的阈值，单位s
    unsigned int single_move;        // 单次移动阈值，单位像素点
    unsigned int max_move;           // 累积最大移动阈值，单位像素点
} loiterdet_param_input_t;

typedef vector<personBox> loiterdet_person_input_t;

typedef vector<rectBox> loiterdet_param_output_t;

int loiter_det_init();

int loiter_det_start(loiterdet_param_input_t* input_param, loiterdet_person_input_t& personbox, loiterdet_param_output_t& loiterbox);

int loiter_det_exit();

#endif
