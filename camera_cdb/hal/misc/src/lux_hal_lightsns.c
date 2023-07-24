/*
 * lux_hal_lightsns.c
 *
 * 基于Ingenic平台封装的adc
 * 
 */

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>

/* define ioctl command. they are fixed. don't modify! */
#define ADC_ENABLE 		0
#define ADC_DISABLE 	1
#define ADC_SET_VREF 	2
#define ADC_PATH_LEN    32


#define ADC_PATH    "/dev/ingenic_adc_aux_" /* adc channal 0-3 */
#define STD_VAL_VOLTAGE 1800 /* The unit is mv. T10/T20 VREF=3300; T30/T21/T31 VREF=1800 */
#define TAG         "lux_hal_lightsns"

int g_fd = -1;
int g_buesd = 0;


/**
 * @description: 获取LightSns的值 
 * @return 返回LightSns的值 ,错误返回-1
 */
int LUX_Hal_LightSns_GetValue()
{	   
    int value;
    if(0 == g_buesd)
    {   
        printf("%s %s:[%d] Error, lightSns uninitialized!\n", TAG, __FUNCTION__, __LINE__);
		return -1;
    }

    /* get value */
	read(g_fd, &value, sizeof(int));

    return value;
}


/**
 * @description: LightSns 初始化
 * @return 0：成功  非0失败
 */
int LUX_Hal_LightSns_Init()
{
    int ret = 0;
    int vol_n;
    char path[ADC_PATH_LEN] = {0};
    int channal = 0;

    if(1 == g_buesd)
    {
        printf("%s %s:[%d] Warning LightSns Already initialized!\n",TAG, __FUNCTION__, __LINE__);
		return -1;
    }

    sprintf(path, "%s%d", ADC_PATH, channal);

    g_fd = open(path, O_RDONLY);
	if(g_fd < 0)
    {
        printf("%s %s:[%d] ADC_lightSns_init:open error !\n", TAG, __FUNCTION__, __LINE__);
		return -1;
	}

    /* set adc voltage */
	vol_n = STD_VAL_VOLTAGE;
	ret = ioctl(g_fd, ADC_SET_VREF, &vol_n);
	if(ret)
    {
        printf("%s %s:[%d] Failed to set adc voltage!\n", TAG, __FUNCTION__, __LINE__);
		return -1;
	}

    /* enable adc */
	ret = ioctl(g_fd, ADC_ENABLE);
	if(ret)
    {
        printf("%s %s:[%d] Failed to enable adc failed!\n", TAG, __FUNCTION__, __LINE__);
		return -1;
	}

    g_buesd = 1;

  return 0;  
}

/**
 * @description: LightSns 去初始化
 * @return 0：成功  非0失败
 */
int LUX_Hal_LightSns_DeInit()
{
    int ret = 0;

    if(0 == g_buesd)
    {
        printf("%s %s:[%d] Error, LightSns uninitialized!\n", TAG, __FUNCTION__, __LINE__);
		return -1;
    }

    /* disenable adc */
	ret = ioctl(g_fd, ADC_DISABLE);
	if(ret)
    {
		printf("%s %s:[%d] Failed to disenable adc failed!\n", TAG, __FUNCTION__, __LINE__);
		return -1;
	}
    
    close(g_fd);

    g_buesd = 0;

    return 0;

}
