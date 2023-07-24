#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <lux_reset_key.h>
#include <comm_error_code.h>
#include <lux_base.h>
#include <lux_global.h>

/*===============================宏定义===============================*/
#define     INI_FILES_MAX   16
#define     INI_NAME_LEN    64
#define 	LUX_KEY_RESET_TIME_INTERVAL 10   //复位按键持续时间间隔 单位：秒
#define 	PR_DEBUG(fmt, ...)  printf("filename: %s function: %s line :[%d] Dbg:"fmt"\r\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

/*================================函数定义================================*/
/**
 * @description: 获取复位按键状态
 * @param [in] enable 开启或关闭
 * @return 0 成功， 非零失败，返回错误码
 */
static int LUX_ResetKey_Hal_Misc_GetStatus(void)
{
    int fd;
    char status = 0;
    char tmpPath[64] = { 0 };

    sprintf(tmpPath, "/sys/class/gpio/gpio79/value");

    fd = open(tmpPath, O_RDWR);
    if (fd < 0)
    {
        printf("%s %s:[%d] open[%s] error!\n", "lux_hal_misc", __FUNCTION__, __LINE__, tmpPath);
        return -1;
    }
    read(fd, &status, 1);
    close(fd);

    return (status == '1') ? 1 : 0;
}



static void lux_set_factory(void)
{
    PR_DEBUG("curr lux_set_factory\r\n");
#if 0
    LUX_BASE_System_CMD("/system/init/backup/factory_reset.sh");
    sleep(1);
    /*暂时屏蔽，分区做好之后打开进行处理*/
#endif
    LUX_BASE_System_CMD("reboot");

    return;
}




/**
 * @description:    复位按键处理线程
 * @return  0 成功， 非零失败
 */
static void* Therad_ResetKey(void* args)
{
    int cnt = 0;
    int keyTmp = 0;
    int last = getTime_getS();

    while (1)
    {
        keyTmp = LUX_ResetKey_Hal_Misc_GetStatus();
        if (keyTmp)
        {
            printf("%s last:%d cur:%d timeOut:%d\n", __func__, last, getTime_getS(), LUX_KEY_RESET_TIME_INTERVAL - cnt);
            if ((++cnt >= LUX_KEY_RESET_TIME_INTERVAL) && (getTime_getS() - last) > LUX_KEY_RESET_TIME_INTERVAL)
                lux_set_factory();
        }
        else
        {
            last = getTime_getS();
            cnt = 0;
        }
        sleep(1);
    }
    return (void*)LUX_OK;
}


/**
 * @description: led初始化
 * @return 0 成功， 非零失败
 */
int LUX_HAL_ResetKeyInit(void)
{
    int ret = LUX_ERR;


    ret = Task_CreateThread(Therad_ResetKey, NULL);
    if (LUX_OK != ret)
    {
        printf("Task_CreateThread failed.\n");
        return LUX_ERR;
    }

    return 0;
}

