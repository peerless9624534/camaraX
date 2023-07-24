#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/prctl.h> 
#include <lux_hal_led.h>
#include <lux_iniparse.h>
#include <comm_def.h>
#include <imp_log.h>
#include <lux_global.h>
#include <lux_record.h>
#include <jwhead.h>
#include "lux_config.h"

#define TAG       "lux_hal_led"


const LUX_LED_BLINK_TYPE_ST lux_led_blink_type[] = {
    {LED_STATUS_NONE,"none",500,0},/*led未使能，灯光关闭 */
    {LED_STATUS_NETWORK_UP,"green",0,1},/*网络正常，恒定绿色*/
    {LED_STATUS_RECORDING,"green",2000,2},/*正常录像，绿灯1000ms闪烁一次*/
    {LED_STATUS_NETWORK_DOWN,"red",500,3},/*网络断开，红灯500ms闪烁一次*/
};


#define CMD_LEN  64

static int LUX_net_eth0_open()
{
    /*打开文件*/

    char buf[CMD_LEN] = { 0 };
    static FILE* fd = NULL;
    snprintf(buf, CMD_LEN, "/sys/class/net/eth0/operstate");
    //printf("*****open%s*********\n",buf);
    if (fd == NULL)
    {
        fd = fopen("/sys/class/net/eth0/operstate", "rb");
    }

    /*读文件*/
    char buf_1[CMD_LEN] = { 0 };
    unsigned char byte = 0;
    if (!fd)
    {
        return -1;
    }
    fseek(fd, 0, SEEK_SET);
    int readlen = fread(buf_1, 1, 16, fd);
    //fclose(fd);
    trim_left(buf_1);
    trim_right(buf_1);
    trim_pure(buf_1);

    if (!strcasecmp(buf_1, "up"))
    {
        return 1;
    }
    else if (!strcasecmp(buf_1, "down"))
    {
        return 0;
    }
    else
    {
        return -1;
    }

}

/**
 * @description: 设置led灯的状态
 * @param [in] colour  灯的颜色
 * @param [in] status  led灯状态
 * @return 0 成功， 非零失败
 */
int LUX_HAL_LedSetStatus(LUX_HAL_LED_EN colour, LUX_HAL_LED_STATUS_EN status)
{
    char cmd[64] = { 0 };
    char* trigger = NULL;
    char* color = colour ? "red" : "green";
    char* bright = NULL;
    trigger = status == LUX_HAL_LED_BLINK ? "timer" : "none";
    snprintf(cmd, sizeof(cmd), "echo %s > /sys/class/leds/%s/trigger", trigger, color);
    //printf("cmd:%s\n", cmd);
    system(cmd);
    if (status == LUX_HAL_LED_ON)
    {
        bright = "1";
    }
    else if (status == LUX_HAL_LED_OFF)
    {
        bright = "0";
    }
    if (bright)
    {
        memset(cmd, 0, sizeof(cmd));
        snprintf(cmd, sizeof(cmd), "echo %s > /sys/class/leds/%s/brightness", bright, color);
        //printf("cmd:%s\n", cmd);
        system(cmd);
    }
    return 0;
}

#if 0
/**
 * @description: led初始化
 * @return 0 成功， 非零失败
 */
int LUX_HAL_LedInit(void)
{
    int err = 0;
    BOOL_X led_enabled = 0;
    if (access("/sys/class/leds/red/brightness", F_OK))
    {
        IMP_LOG_WARN(TAG, "ERROR! led red init fail");
    }
    if (access("/sys/class/leds/green/brightness", F_OK))
    {
        IMP_LOG_WARN(TAG, "ERROR! led green init fail");
    }

    err = LUX_INIParse_GetBool(LUX_CONFIG_FILE, "hal", "led", &led_enabled);
    if (err)
    {
        IMP_LOG_WARN(TAG, "get led config failed");
    }
    if (led_enabled)
    {
        LUX_HAL_LedSetStatus(LUX_HAL_LED_GREEN, LUX_HAL_LED_ON);
    }
    return 0;
}
#endif

/**
 * @description: LED灯切换开关
 * @param [in] enable 开启或关闭
 * @return 0 成功， 非零失败，返回错误码
 */
int LUX_SetLed_OnOff(int* times, int* FlashTime, const char* color)
{
    int fd_green, fd_red;
    int gpio_ircut_red, gpio_ircut_green;

    if (!strcmp(color, "red") && !*times)
    {
        //printf("2------------red LED_FlashTime \n");
        fd_green = lux_gpio_open(60);
        fd_red = lux_gpio_open(59);
        lux_gpio_write(fd_green, 0);
        lux_gpio_write(fd_red, 1);
        *times = 1;
        close(fd_green);
        close(fd_red);

    }
    else if (!strcmp(color, "green") && !*times)
    {
        //printf("1------------green LED_FlashTime \n");
        fd_green = lux_gpio_open(60);
        fd_red = lux_gpio_open(59);
        lux_gpio_write(fd_green, 1);
        lux_gpio_write(fd_red, 0);
        *times = 1;
        close(fd_green);
        close(fd_red);
    }
    else if (!strcmp(color, "none") || (*times && *FlashTime != 0))
    {
        fd_green = lux_gpio_open(60);
        fd_red = lux_gpio_open(59);
        lux_gpio_write(fd_green, 0);
        lux_gpio_write(fd_red, 0);
        *times = 0;
        close(fd_green);
        close(fd_red);
    }

    return 0;
}
#if 1
static void* Therad_LED_OnOff(void* args)
{
    INT_X	ret = LUX_ERR;

    int times = 0;/*LED灯ON/OFF切换指示开关*/
    LUX_LED_BLINK_TYPE_ST* net_status;

    while (1)
    {
        int networkValue = LUX_net_eth0_open();
        if (0 >= networkValue || 0 == g_configData->system_config.led_enabled && 0 >= networkValue)
        {
            net_status = &lux_led_blink_type[3];
            // if (times != 0)
            // {
            //     times = 0;
            // }
        }
        else if (1 == networkValue && 0 == g_configData->system_config.led_enabled)
        {
            net_status = &lux_led_blink_type[0];
        }
        else if (1 == networkValue && 1 == g_configData->system_config.led_enabled)
        {
            if (g_lux_record->is_running == 1)
            {
                net_status = &lux_led_blink_type[2];

                // if (times != 0)
                // {
                //     times = 0;
                // }
            }
            else 
            {
                net_status = &lux_led_blink_type[1];
                if (times != 0)
                {
                    times = 0;
                }
            }

            if (times != 0)
            {
                net_status = &lux_led_blink_type[0];
            }
        }

        LUX_SetLed_OnOff(&times, &net_status->LED_FlashTime, net_status->led_color);

        if (net_status->LED_FlashTime)
        {
            usleep(net_status->LED_FlashTime * 1000);
        }
        else
        {
            usleep(200 * 1000);
        }
    }

    return (void*)LUX_OK; 
}
#endif
#if 0
/**
 * @description:    灯光处理线程
 * @return  0 成功， 非零失败
 */
static void* Therad_LED_OnOff(void* args)
{
    INT_X	ret = LUX_ERR;

    int times = 0;/*LED灯ON/OFF切换指示开关*/
    char* current_led_color = NULL;
    int current_led_flashtime = 0;
    int current_led_priority = 0;
    int last_led_priority = 0;
    int current_led_status_num = LED_STATUS_NONE;
    int last_led_status_num = LED_STATUS_LAST_NONE;
    lux_hal_led_status_num = LED_STATUS_NONE;

    current_led_color = lux_led_blink_type[last_led_status_num].led_color;
    current_led_flashtime = lux_led_blink_type[last_led_status_num].LED_FlashTime;
    current_led_priority = lux_led_blink_type[last_led_status_num].priority;

    while (1)
    {
        if (0 == LUX_net_eth0_open())
        {
            lux_hal_led_status_num = LED_STATUS_NETWORK_DOWN;
        }
        else if (1 == LUX_net_eth0_open())
        {
            if (last_led_status_num == LED_STATUS_NETWORK_DOWN)
            {
                lux_hal_led_status_num = LED_STATUS_NONE;
            }
            else
            {

                if (g_lux_record->is_running == 1)
                {
                    lux_hal_led_status_num = LED_STATUS_RECORDING;
                }
                else  if (lux_hal_led_status_num != LED_STATUS_RECORDING)
                {
                    lux_hal_led_status_num = LED_STATUS_NETWORK_UP;
                }

            }
        }

        if (g_configData->system_config.led_enabled != 1)
        {
            if (0 == LUX_net_eth0_open()){
                lux_hal_led_status_num = LED_STATUS_NETWORK_DOWN;
            }
            else {
                lux_hal_led_status_num = LED_STATUS_NONE;
            }
        }

        printf("lux_hal_led_status_num = %d\n", lux_hal_led_status_num);
        current_led_status_num = lux_hal_led_status_num;
        printf("current_led_status_num = %d\n", current_led_status_num);
        printf("current_led_status_priority = %d\n", lux_led_blink_type[current_led_status_num].priority);

        if (last_led_status_num != LED_STATUS_LAST_NONE)
        {
            if ((lux_led_blink_type[current_led_status_num].priority > lux_led_blink_type[last_led_status_num].priority && \
                last_led_status_num != current_led_status_num) || current_led_status_num == LED_STATUS_NONE)
            {
                current_led_status_num = lux_led_blink_type[current_led_status_num].number;
                current_led_color = lux_led_blink_type[current_led_status_num].led_color;
                current_led_flashtime = lux_led_blink_type[current_led_status_num].LED_FlashTime;
                current_led_priority = lux_led_blink_type[current_led_status_num].priority;
                last_led_status_num = current_led_status_num;
                printf("1------------LED_FlashTime = %d\n", current_led_flashtime);
            }

            if (times != 0)
            {
                times = 0;
            }
        }
        else
        {
            current_led_status_num = lux_led_blink_type[current_led_status_num].number;
            current_led_color = lux_led_blink_type[current_led_status_num].led_color;
            current_led_flashtime = lux_led_blink_type[current_led_status_num].LED_FlashTime;
            current_led_priority = lux_led_blink_type[current_led_status_num].priority;
            last_led_status_num = current_led_status_num;
            printf("2----------LED_FlashTime = %d\n", current_led_flashtime);
            if (times != 0)
            {
                times = 0;
            }
        }
        printf("2----------times = %d\n", times);
        printf("1------------current_led_color = %s\n", current_led_color);
        LUX_SetLed_OnOff(&times, &current_led_flashtime, current_led_color);

        printf("3------------LED_FlashTime = %d\n", current_led_flashtime);
        if (current_led_flashtime)
        {
            usleep(current_led_flashtime * 1000);
        }
        else
        {
            usleep(200 * 1000);
        }
        //usleep(2000 * 1000);
    }
    return (void*)LUX_OK;
}
#endif

/**
 * @description: led初始化
 * @return 0 成功， 非零失败
 */
int LUX_HAL_LedInit(void)
{
    INT_X ret = LUX_ERR;

    //CHAR_X led_enable_status[128] = { 0 };

    LED_FlashTime = 500;

    // /*根据配置选择Led是否开启*/
    // ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "system_config", "led_enabled", led_enable_status);
    // if (0 != ret)
    // {
    //     MYERRORING("LUX_INIParse_GetBool failed!, error num [0x%x] ", ret);
    // }
    // trim_all(led_enable_status);

    // if (!strcasecmp(led_enable_status, "0"))
    // {
    //     MYTRACE("current led_enable_status is OFF\n");
    //     g_configData->system_config.led_enabled = 0;
    // }
    // else if (!strcasecmp(led_enable_status, "1"))
    // {
    //     MYTRACE("current led_enable_status is ON\n");
    //     g_configData->system_config.led_enabled = 1;
    // }
    // else
    // {
    //     MYERROR("get led_enable status is Failed!\n");
    // }

    ret = Task_CreateThread(Therad_LED_OnOff, NULL);
    if (LUX_OK != ret)
    {
        printf("Task_CreateThread failed.\n");
        return LUX_ERR;
    }

    return 0;
}

