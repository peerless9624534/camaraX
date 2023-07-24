/*
 * lux_audio.h
 *
 * led
 */

#ifndef __LUX_HAL_LED_H__
#define __LUX_HAL_LED_H__

#include "comm_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

typedef enum
{
    LUX_HAL_LED_GREEN,
    LUX_HAL_LED_RED,
    LUX_HAL_LED_NUM,
} LUX_HAL_LED_EN;

typedef enum
{
    LUX_HAL_LED_OFF,        /*常灭*/
    LUX_HAL_LED_ON,         /*常亮*/
    LUX_HAL_LED_BLINK,       /*闪烁*/
} LUX_HAL_LED_STATUS_EN;

typedef enum {
    LED_STATUS_NONE,/*0*/
    LED_STATUS_NETWORK_UP,/*1*/
    LED_STATUS_RECORDING,/*2*/
    LED_STATUS_NETWORK_DOWN,/*3*/
    LED_STATUS_LAST_NONE,/*4*/
}LUX_HAL_LED_STATUS;

LUX_HAL_LED_STATUS lux_hal_led_status_num;

typedef struct {
    int number;
    char* led_color;/*灯光颜色*/
    int  LED_FlashTime;/*灯光闪烁时间*/
    int priority;/*灯光优先级*/
}LUX_LED_BLINK_TYPE_ST;

extern char* led_color;
extern int LED_FlashTime;

extern LUX_HAL_LED_STATUS lux_hal_led_status;



#if 1
/**
 * @description: 设置led灯的状态
 * @param [in] colour  灯的颜色
 * @param [in] status  led灯状态
 * @return 0 成功， 非零失败
 */
int LUX_HAL_LedSetStatus(LUX_HAL_LED_EN colour, LUX_HAL_LED_STATUS_EN status);
#endif

/**
 * @description: led初始化
 * @return 0 成功， 非零失败
 */
int LUX_HAL_LedInit(void);


/**
 * @description: LED灯切换开关
 * @param [in] enable 开启或关闭
 * @return 0 成功， 非零失败，返回错误码
 */
int LUX_SetLed_OnOff(int *times, int* FlashTime, const char* color);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __LUX_HAL_LED_H__ */
