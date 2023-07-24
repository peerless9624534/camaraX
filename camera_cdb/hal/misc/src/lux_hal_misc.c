#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <lux_iniparse.h>
#include <lux_hal_misc.h>
#include <lux_hal_gpio.h>
#include <imp_log.h>
#include <lux_global.h>
#include <lux_global.h>
#include "lux_config.h"
#define TAG     "lux_hal_misc"


/**
 * @description: 红外灯亮灭控制
 * @param [in] enable 开启或关闭
 * @return 0 成功， 非零失败，返回错误码
 */
int LUX_Hal_Misc_IRLightOnOff(int enable)
{
	int fd = -1;
    int ret = -1;
    int gpioNum = 0;
    char tmpPath[64] = {0};

    /*从初始化配置文件中读取配置的gpio口*/
    // LUX_CONFIG_GPIO_CFG_ST* gpio_parameter = (LUX_CONFIG_GPIO_CFG_ST *)malloc(sizeof(LUX_CONFIG_GPIO_CFG_ST));
    // ret = LUX_Config_GetGPIOCfg(gpio_parameter);
    // if(LUX_OK != ret)
    // {
    //     IMP_LOG_ERR(TAG, " %s %s:[%d] LUX_INIParse_GetString failed\n", TAG, __FUNCTION__, __LINE__);
    //     return -1;
    // }
    //fd = lux_gpio_open(gpioNum);
    lux_gpio_write(fd_116, enable);

    //close(fd);

	return 0;
}


/**
 * @description: 红外滤光片切换开关
 * @param [in] enable 开启或关闭
 * @return 0 成功， 非零失败，返回错误码
 */
 int LUX_ISP_SetIRCUT(BOOL_X enable)
{
	int fd_ircutn, fd_ircutp;
    // int gpio_ircut_p, gpio_ircut_n;
    // int ret;
    // LUX_CONFIG_GPIO_CFG_ST* gpio_parameter; 
    // ret = LUX_Config_GetGPIOCfg(gpio_parameter);
    // if(LUX_OK != ret)
    // {
    //     IMP_LOG_ERR(TAG, " %s %s:[%d] LUX_INIParse_GetString failed\n", TAG, __FUNCTION__, __LINE__);
    //     return -1;
    // }
    // if(LUX_INIParse_GetInt(LUX_CONFIG_FILE, "gpio_config", "irled", &gpio_ircut_n))
    // {
    //     IMP_LOG_ERR(TAG, "ircutn fail!\n");
    //     return -1;
    // }
    // if(LUX_INIParse_GetInt(LUX_CONFIG_FILE, "gpio_config", "ircutp", &gpio_ircut_p))
    // {
    //     IMP_LOG_ERR(TAG, "ircutp fail!\n");
    //     return -1;
    // }
    //IMP_LOG_ERR(TAG, "ircutn%d, ircutp%d \n",gpio_parameter->ircutn, gpio_parameter->ircutp);
    //fd_ircutn = lux_gpio_open(gpio_ircut_n);
    //fd_ircutp = lux_gpio_open(gpio_ircut_p);
    fd_ircutn = fd_122;
    fd_ircutp = fd_123;
    if(fd_ircutn < 0 || fd_ircutp < 0)
    {
        IMP_LOG_ERR(TAG, "ircut gpio not inited\n");
    }
    lux_gpio_write(fd_ircutp, enable);
    lux_gpio_write(fd_ircutn, !enable);
    usleep(200 * 1000);
    lux_gpio_write(fd_ircutn, 0);
    lux_gpio_write(fd_ircutp, 0);
    //close(fd_ircutn);
	//close(fd_ircutp);

	return 0;
}

/**
 * @description: 扬声器开关控制
 * @param [in] enable 开启或关闭
 * @return 0 成功， 非零失败，返回错误码
 */
int LUX_Hal_Misc_SpeakerOnOff(int enable)
{
    int fd;
    int ret = -1;
    // int gpioNum;
    // char tmpPath[64] = {0};

    // /*从初始化配置文件中读取配置的gpio口*/
    // ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "gpio_config", "speaker", &gpioNum);
    // if (LUX_OK != ret)
    // {
    //     IMP_LOG_ERR(TAG, "%s %s:[%d] LUX_INIParse_GetString failed\n", TAG, __FUNCTION__, __LINE__);
    //     return -1;
    // }
    fd = lux_gpio_open(g_configData->gpio_config.speaker);
    if (fd < 0)
    {
        IMP_LOG_ERR(TAG, "%s %s:[%d] open error!\n", TAG, __FUNCTION__, __LINE__);
        return -1;
    }
    lux_gpio_write(fd, enable);
    close(fd);

    return 0;
}
