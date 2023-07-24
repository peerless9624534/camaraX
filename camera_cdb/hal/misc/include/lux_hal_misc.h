/*
 * lux_hal_misc.h
 *
 * peripheral equipment gpio control
 */

#ifndef __LUX_HAL_MISC_H__
#define __LUX_HAL_MISC_H__

#include "comm_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

typedef enum
{
	LUX_PLATFORM_TYPE_AC = 0,
	LUX_PLATFORM_TYPE_PTZ,
	LUX_PLATFORM_TYPE_MAX,
}LUX_PLATFORM_TYPE;

/**
 * @description: 红外灯亮灭控制
 * @param [in] enable 开启或关闭
 * @return 0 成功， 非零失败，返回错误码
 */
int LUX_Hal_Misc_IRLightOnOff(int enable);

/**
 * @description: 红外滤光片切换开关
 * @param [in] enable 开启或关闭
 * @return 0 成功， 非零失败，返回错误码
 */
int LUX_ISP_SetIRCUT(BOOL_X enable);

 /**
 * @description: 扬声器开关控制
 * @param [in] enable 开启或关闭
 * @return 0 成功， 非零失败，返回错误码
 */
int LUX_Hal_Misc_SpeakerOnOff(int enable);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__LUX_HAL_MISC_H__*/
