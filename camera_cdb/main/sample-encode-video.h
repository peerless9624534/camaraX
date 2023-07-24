/*
 * sample-common.h
 *
 * Copyright (C) 2014 Ingenic Semiconductor Co.,Ltd
 */

#ifndef __SAMPLE_COMMON_H__
#define __SAMPLE_COMMON_H__

#include <imp_common.h>
#include <imp_osd.h>
#include <imp_framesource.h>
#include <imp_isp.h>
#include <imp_encoder.h>
#include <unistd.h>
#include "comm_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#define SENSOR_FRAME_RATE_NUM		25
#define SENSOR_FRAME_RATE_DEN		1

INT_X LUSHARE_test_sample_Encode_video(INT_X argc, PCHAR_X argv);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __SAMPLE_COMMON_H__ */
