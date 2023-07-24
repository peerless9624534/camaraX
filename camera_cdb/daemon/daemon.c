#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* 执行系统指令 */
static int LUX_BASE_System_CMD(char* pCmd)
{
    int ret = 0;
    char cCmd[256] = { 0 };
    char buffer[512] = { 0 };
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

/* 监视进程运行，异常崩溃，自动重启 */
static int save_sample(void)
{
    printf("\n**************** start save media sample! ****************\n");
#if 0//cxj test
    int ping_success = 0;
    char a = 'V';

    int offline_time = 0;
    char tmpCmd[128] = { 0 };
    int index = 0;
    int index_old = 0;
#endif
    FILE* fp = NULL;
    FILE* fp_watchdog = NULL;
    int KeepAlive = 0;
    char buf[1024] = { 0 };
    while (1)
    {
        memset(buf, 0x0, sizeof(buf));
        KeepAlive = 0;

        LUX_BASE_System_CMD("/bin/ps > /tmp/sample_pid");
        fp = fopen("/tmp/sample_pid", "r");
        if (fp == NULL)
        {
            printf("open file /tmp/sample_pid fail");
            return -1;
        }
        while (fgets(buf, sizeof(buf), fp) != NULL)
        {
            if ((strstr(buf, "media_sample") != NULL)) KeepAlive++;
            if ((strstr(buf, "reset_key") != NULL)) KeepAlive++;
            if ((strstr(buf, "ota") != NULL))
            {
                KeepAlive++;
                break;
            }
        }
        fclose(fp);

        if (access("/tmp/mark_ota.txt", F_OK) == 0) KeepAlive = 3;


        /*喂狗*/
        fp_watchdog = fopen("/dev/watchdog", "w");
        if (fp_watchdog == NULL)
        {
            printf("**************** reboot system! ****************");
            return 0;
        }
        fprintf(fp_watchdog, "a");
        fclose(fp_watchdog);

#if 0//cxj test
        if (index - index_old >= 30)
        {
            index_old = index;
            fp = NULL;
            fp = fopen("/system/etc/first_set_wifi", "r");
            if (fp != NULL)
            {
                fclose(fp);
                fp = NULL;
                memset(buf, 0x0, sizeof(buf));
                ping_success = 0;

                system("/bin/ping -c 1 -w 1 www.tuya.com > /tmp/result_ping &");

                fp = fopen("/tmp/result_ping", "r");
                if (fp != NULL)
                {
                    printf("open file /tmp/result_ping fail");
                    //return -1;


                    while (fgets(buf, sizeof(buf), fp) != NULL)
                    {
                        if (strstr(buf, "1 packets received") != NULL)
                        {
                            printf("\r\n[%s]%d: buf = %s\n", __func__, __LINE__, buf);
                            ping_success = 1;
                            offline_time = 0;
                            break;
                        }
                    }

                    fclose(fp);
                }

                if (ping_success == 0)
                {
                    sprintf(tmpCmd, "killall -9 udhcpc");
                    //LUX_BASE_System_CMD(tmpCmd);
                    sprintf(tmpCmd, "/sbin/udhcpc -i wlan0 -q &");
                    //system(tmpCmd);
                    offline_time++;
                    printf("\r\n[%s]%d: offline_time = %d\n", __func__, __LINE__, offline_time);
                }
            }
        }
        index++;
#endif
        if (KeepAlive != 3)//比较值为out目录进程数-1，减去本身daemon
        {
            printf("**************** reboot system! ****************\n");
            LUX_BASE_System_CMD("/bin/echo 1 > /system/bin/unnormalReboot.txt");
            LUX_BASE_System_CMD("/usr/bin/free > /system/etc/result_free");
            LUX_BASE_System_CMD("/bin/mpstat > /system/etc/result_mpstat");
            return 0;
        }
        sleep(1);
    }
    return 0;
}

int main(void)
{
    save_sample();
    return 0;
}

