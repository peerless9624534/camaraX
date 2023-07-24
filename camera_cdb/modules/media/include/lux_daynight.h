/*
 * lux_automode.h
 *
 * 白天黑夜自动切换
 */

#ifndef __LUX_DAYNIGHT_H__
#define __LUX_DAYNIGHT_H__

#include "comm_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

typedef enum
{
    LUX_DAYNIGHT_MANUAL_MODE,
    LUX_DAYNIGHT_AUTO_MODE,

}LUX_DayNight_AutoMode_EN;

typedef enum
{
    LUX_DAYNIGHT_DAY_MODE,
    LUX_DAYNIGHT_NIGHT_MODE,
    LUX_DAYNIGHT_NONE_MODE,
}LUX_DayNight_Mode_EN;

/**
 * @description:    设置白天黑夜模式
 * @param   [in]    mode 模式
 * @return  0 成功， 非零失败
 */
INT_X LUX_DayNight_SetMode(LUX_DayNight_Mode_EN Mode);

/**
 * @description:    设置白天黑夜模切换模式
 * @param   [in]    mode 模式
 * @return  0 成功， 非零失败
 */
INT_X LUX_DayNight_SetAutoMode(LUX_DayNight_AutoMode_EN autoMode);

/**
 * @description:    白天黑夜模式初始化
 * @return  0 成功， 非零失败,返回错误码
 */
INT_X LUX_DayNight_Init();

/**
 * @description:    白天黑夜模式去初始化
 * @return  0 成功， 非零失败,返回错误码
 */
INT_X LUX_DayNight_DeInit();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__LUX_DAYNIGHT_H__*/