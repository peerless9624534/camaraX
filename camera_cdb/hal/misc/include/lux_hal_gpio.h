/*
 * SU PWM header file.
 *
 * Copyright (C) 2015 Ingenic Semiconductor Co.,Ltd
 * Author: Tiger <bohu.liang@ingenic.com>
 */

#ifndef __LUX_GPIO_H__
#define __LUX_GPIO_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */
#define GPIO_PA(n) (0 * 32 + (n))
#define GPIO_PB(n) (1 * 32 + (n))
#define GPIO_PC(n) (2 * 32 + (n))
#define GPIO_PD(n) (3 * 32 + (n))

typedef enum
{
    LUX_GPIO_LOW = 0,
    LUX_GPIO_HIGH = 1,
}lux_gpio_value;

typedef enum
{
    LUX_GPIO_OUT = 0,
    LUX_GPIO_IN = 1,
}lux_gpio_direction;

int lux_gpio_read(int gpiofd);
int lux_gpio_write(int gpiofd, char value);
int lux_gpio_open(int index);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

/**
 * @}
 */

#endif /* __SU_PWM_H__ */
