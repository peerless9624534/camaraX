/*
 * lux_hal_lightsns.h
 *
 * light sensor
 */

#ifndef __LUX_HAL_LIGHTSNS_H__
#define __LUX_HAL_LIGHTSNS_H__

#include "comm_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

/**
 * @description: 获取LightSns的值
 * @return 返回LightSns的值
 */
int LUX_Hal_LightSns_GetValue();

/**
 * @description: LightSns 初始化
 * @param [int] channal 通道号
 * @return 0：成功  非0失败
 */
int LUX_Hal_LightSns_Init();

/**
 * @description: LightSns 去初始化
 * @return 0：成功  非0失败
 */
int LUX_Hal_LightSns_DeInit();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __LUX_HAL_LIGHTSNS_H__ */
