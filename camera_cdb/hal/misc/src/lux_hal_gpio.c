#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "fcntl.h"
#include "sys/stat.h"
#include "sys/types.h"
//static char cmdbuf[CMD_LEN] = {0};

/*set gpio value: only valid for inited output gpio*/

int lux_gpio_open(int index)
{
    #define CMD_LEN 64
    char buf[CMD_LEN] = {0};
    snprintf(buf, CMD_LEN, "/sys/class/gpio/gpio%d/value", index);
    //printf("*****open%s*********\n",buf);
    return open(buf, O_RDWR);
}

int lux_gpio_read(int fd)
{
    if(!fd) 
    {
        return -1; 
    }
    lseek(fd, 0, SEEK_SET);
    char buf;
    read(fd, (void*)&buf, 1);
    return buf == '0'? 0: 1;
}

int lux_gpio_write(int fd, char value)
{
    if(!fd)
    {
        return -1;
    }
    lseek(fd, 0, SEEK_SET);
    write(fd, value? "1": "0", 1);
    return 0;
}