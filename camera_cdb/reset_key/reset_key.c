#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

/*===============================宏定义===============================*/
#define     INI_FILES_MAX   16
#define     INI_NAME_LEN    64
#define 	LUX_KEY_RESET_TIME_INTERVAL 5   //复位按键持续时间间隔 单位：秒
#define 	PR_DEBUG(fmt, ...)  printf("filename: %s function: %s line :[%d] Dbg:"fmt"\r\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

/*================================函数定义================================*/
/**
 * @description: 获取系统启动时间
 * @param void
 * @return [out] 时间：单位:S
 */
static unsigned int getTime_getS(void)
{
    /*
       获取特定时钟的时间，目前定义了6种时钟，分别是:
       CLOCK_REALTIME 系统当前时间，从1970年1月1日算起
       CLOCK_MONOTONIC 系统的启动时间，不能被设置
       CLOCK_PROCESS_CPUTIME_ID 进程运行时间
       CLOCK_THREAD_CPUTIME_ID 线程运行时间
       CLOCK_REALTIME_HR CLOCK_REALTIME的高精度版本
       CLOCK_MONOTONIC_HR CLOCK_MONOTONIC的高精度版本
    */

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ((ts.tv_sec) + ts.tv_nsec / 1000000000);
}

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

    sprintf(tmpPath, "/sys/class/gpio/gpio50/value");

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

/* 执行系统指令 */
static int LUX_BASE_System_CMD(char* pCmd)
{
    int ret = 0;
    char cCmd[256] = { 0 };
    char buffer[512] = { 0 };
    //strncpy(cCmd,256,pCmd);
    snprintf(cCmd, 256, "%s", pCmd);
    printf("cmd: %s\n", pCmd);
    FILE* pp = popen(cCmd, "r");
    if (NULL == pp)
    {
        ret = -1;
        printf("LUX_BASE_System_CMD Err(%d)\n", ret);
        return ret;
    }

    while (fgets(buffer, sizeof(buffer), pp) != NULL)
    {
        printf("%s\n", buffer);
    }

    pclose(pp);

    return ret;
}

static void IPC_APP_set_factory(void)
{
    PR_DEBUG("curr IPC_APP_set_factory\r\n");

    LUX_BASE_System_CMD("/system/init/backup/factory_reset.sh");
    sleep(1);

    LUX_BASE_System_CMD("reboot");

    return;
}



/*===============================主函数：复位键恢复出厂设置===============================*/
int main(void)
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
                IPC_APP_set_factory();
        }
        else
        {
            last = getTime_getS();
            cnt = 0;
        }
        sleep(1);
    }
    return 0;
}

