/*
 * lux_isp.h
 *
 * isp模块，包括翻转、图像调优等功能配置
 *
 */

#ifndef __LUX_ISP_H__
#define __LUX_ISP_H__

#include <comm_def.h>
#include <lux_base.h>
#include <imp_isp.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#define 	LUX_ISP_SENSOR_NUMBNER		16

/*镜像翻转属性参数*/
typedef enum
{
	LUX_ISP_SNS_NONE,
    LUX_ISP_SNS_NORMAL,
    LUX_ISP_SNS_MIRROR,
    LUX_ISP_SNS_FLIP,
    LUX_ISP_SNS_MIRROR_FLIP,
    LUX_ISP_SNS_BUTT,

}LUX_ISP_SNS_ATTR_EN;

/*图像调优参数配置, 参数不全后期有待添加*/
typedef struct
{
    UCHAR_X   turningContrast;
    UCHAR_X   turningSharpness;
    UCHAR_X   turningSaturation;
    UCHAR_X   turningBrightness;

}LUX_ISP_TURNING_ATTR_ST;

/*ISP白天黑夜参数配置*/
typedef enum
{
    LUX_DAYNIGHT_MODE_DAY = 0,      /*白天模式*/
    LUX_DAYNIGHT_MODE_NIGHT = 1,    /*黑夜模式*/
    LUX_DAYNIGHT_MODE_CUSTOM = 2,     /**< 定制模式 */
    LUX_DAYNIGHT_MODE_BUTT = 3,

}LUX_DAYNIGHT_MODE_EN;

/*ISP初始化的参数配置*/
typedef struct
{   
    UINT_X                      poolSize;           /*OSD功能缓存池大小*/
    LUX_ISP_TURNING_ATTR_ST     ispTurningAttr;     /*图像校准参数*/
    LUX_DAYNIGHT_MODE_EN        dayNightMdoe;       /*白天黑夜模式*/
    LUX_ISP_SNS_ATTR_EN         senceMirFlip;       /*画面翻转模式*/

} LUX_ISP_INIT_ATTR_ST;

/*镜头传感器信息*/
typedef struct 
{
    PCHAR_X                     name;
    IMPSensorControlBusType     cbusType;         
    UINT_X                      i2cAddr;          //sensor i2c address
    UINT_X                      i2cAdapterID;     //sensor controller number used (0/1/2/3)
    UINT_X                      snsWidth;         //sensor width
    UINT_X                      snsHeight;        //sensor height
    INT_X                       resetGPIO;        //sensor reset gpio
    INT_X                       powerDownGPIO;    //sensor pwdn gpio
    INT_X                       powerGPIO;        //sensor power gpio
    INT_X                       sensorID;         //sensor index
    INT_X                       videoInterface;   //sensor interface type (dvp/csi0/csi1)
    INT_X                       mclk;             //sensor clk source (mclk0/mclk1/mclk2)
    INT_X                       defaultBoot;      //sensor default mode(0/1/2/3/4)
}LUX_ISP_SENSOR_INFO_ST;

/**
 * @description: 设置白天黑夜模式
 * @param [IN] dayNightMode  白天黑夜模式
 * @return 0 成功， 非零失败
 */
INT_X LUX_ISP_SetDayDightMode(LUX_DAYNIGHT_MODE_EN dayNightMode);

/**
 * @description: 获取白天黑夜模式
 * @param [out] dayNightMode  白天黑夜模式
 * @return 0 成功， 非零失败, 返回错误码
 */
INT_X LUX_ISP_GetDayDightMode(LUX_DAYNIGHT_MODE_EN *dayNightMode);

/**
 * @description: 图像调优
 * @param [in] pIspTurningAttr  图像调优相关参数
 * @return 0 成功，非零错误
 */
INT_X LUX_IMP_ISP_SetTurningAttr(LUX_ISP_TURNING_ATTR_ST *pIspTurningAttr);

/**
 * @description: 获取图像调优参数
 * @param [out] pIspTurningAttr  图像调优相关参数
 * @return 0 成功，非零错误
 */
INT_X LUX_IMP_ISP_GetTurningAttr(LUX_ISP_TURNING_ATTR_ST *pIspTurningAttr);

/**
 * @description: 图像翻转功能设置
 * @param [in] mode 水平、垂直翻转属性
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_ISP_SetSenceFlip(LUX_ISP_SNS_ATTR_EN mode);

/**
 * @description: 获取图像翻参数
 * @param [out] mode 获取图像翻转属性
 * @return 0 成功，非零错误, 返回错误码
 */
INT_X LUX_ISP_GetSenceFlip(LUX_ISP_SNS_ATTR_EN *mode);

/**
 * @description: ISP模块初始化
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_ISP_Init();

/**
 * @description: ISP模块去初始化
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_ISP_Exit();

/**
 * @description: 根据sensor的名字找到对应的sensor配置参数
 * @param [in] sensorName  sensor的名字
 * @param [out] pOutSensorInfor  sensor的配置参数
 * @return 0 成功， 非零失败，返回错误码
 * @attention	未找到的异常情况,默认返回gc2053
 */
INT_X LUX_ISP_GetSensorInfo(PCHAR_X sensorName, LUX_ISP_SENSOR_INFO_ST *pOutSensorInfor);

INT_X LUX_ISP_SetSenceFlipOther(LUX_ISP_SNS_ATTR_EN mode);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __LUX_ISP_H__ */
