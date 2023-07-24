/**********************************************************
 * Author        : kaiwang
 * Email         : kai.wang@ingenic.com
 * Last modified : 2020-09-15 21:25
 * Filename      : spi.c
 * Description   :
 * *******************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <lux_hal_gpio.h>

#define GPIO_VD_FZ GPIO_PB(26)
#define GPIO_RSTB GPIO_PB(30)

#define CFG_BACKWARD (1<<8)
#define VD_FREQ 100
#define MOTOR_PPS 400

#define BIT_EN 10
#define BIT_BRAK 9
#define BIT_DIR 8

char *device = "/dev/spidev0.0";
static uint32_t mode = SPI_MODE_3 | SPI_CS_HIGH | SPI_LSB_FIRST; //空闲高电平，第二个上升沿
static uint8_t bits = 8;
static uint32_t speed = 10 * 1000;
static uint16_t delay = 0;
static int steps_zoom = 48;
static int steps_focus = 48;
//static uint8_t direction = 0;
//static uint16_t step_per_pulse;
static unsigned char rxbuf[8] = {0};
static unsigned char txbuf[8] = {0};
int spi_dev_fd = 0;
int gpio_vdfz_fd = 0;
int gpio_rst_fd = 0;

int spi_read_reg(int addr, unsigned char* rbuf);
void toggle_vdfz(int cycle);
int spi_write_reg(int addr, int value);
int spi_write_bytes(int fd, unsigned char *pdata_buff, unsigned int len);

void ms419xx_stop()
{
    unsigned int value = 0x0000;
    //value = value | direction;
    spi_write_reg(0x24, value); // AB control reg
    spi_write_reg(0x29, value); // CD control reg
    toggle_vdfz(1);
}

void ms419xx_zoom_in(int step)
{
    unsigned int value = (0x3400) + steps_zoom; //vd=100hz, 800pps
    spi_write_reg(0x29, value); // AB control reg
    memset(rxbuf, 0, 4);
    spi_read_reg(0x29, rxbuf);
    printf("read 0x24 :%x%x\n", rxbuf[0], rxbuf[1]);
    usleep(1000);
    toggle_vdfz(step);
    usleep(1000);
    ms419xx_stop();
}

void ms419xx_zoom_out(int step)
{
    unsigned int value = (0x3400| CFG_BACKWARD) + steps_zoom; //vd=100hz, 800pps
    spi_write_reg(0x29, value); // AB control reg
    memset(rxbuf, 0, 4);
    spi_read_reg(0x29, rxbuf);
    printf("read 0x24 :%x%x\n", rxbuf[0], rxbuf[1]);
    usleep(1000);
    toggle_vdfz(step);
    usleep(1000);
    ms419xx_stop();
}

void ms419xx_focus_near(int step)
{
    unsigned int value = (0x3400| CFG_BACKWARD) + steps_focus; //vd=100hz, 800pps
    spi_write_reg(0x24, value); // AB control reg
    memset(rxbuf, 0, 4);
    spi_read_reg(0x24, rxbuf);
    printf("read 0x24 :%x%x\n", rxbuf[0], rxbuf[1]);
    usleep(1000);
    toggle_vdfz(step);
    usleep(1000);
    ms419xx_stop();
}

void ms419xx_focus_far(int step)
{
    unsigned int value = (0x3400) + steps_focus;
    spi_write_reg(0x24, value); // AB control reg
    memset(rxbuf, 0, 4);
    spi_read_reg(0x24, rxbuf);
    printf("read 0x24 :%x%x\n", rxbuf[0], rxbuf[1]);
    usleep(1000);
    
    toggle_vdfz(step);
    usleep(1000);
    ms419xx_stop();
}

void ms419_reset(void)
{
    lux_gpio_write(gpio_rst_fd, 0);
    usleep(1000);
    lux_gpio_write(gpio_rst_fd, 1);
    usleep(2000);
}

void toggle_vdfz(int cycle)
{
    while (cycle --)
    {
        lux_gpio_write(gpio_vdfz_fd, 1);
        usleep(100);
        lux_gpio_write(gpio_vdfz_fd, 0);
        usleep(10*1000);
    }
}

int spi_write_reg(int addr, int value)
{
    char wbuf[8] = {0};
    //memset(txbuf, 0 , sizeof(txbuf));
    wbuf[0] = addr & 0xff;

    wbuf[1] = value & 0xff;
    wbuf[2] = (value >> 8) & 0xff;
    spi_write_bytes(spi_dev_fd, wbuf, 3);
    return 0;
}

int ms419xx_init(void)
{
    //spi_write_reg(0x0B, 0x0080);
    gpio_vdfz_fd = lux_gpio_open(GPIO_VD_FZ);
    gpio_rst_fd = lux_gpio_open(GPIO_RSTB);
    ms419_reset();
    //20h 
    //D14-D13 //20h D12-D10 PWMRES[1:0]设置由 PWMMODE[4:0]决定的频率的分频数。
    //D12-D10 PWMMODE[4:0]通过设置系统时钟 OSCIN 的分频数来设置微型步进输出 PWM 的频率 
    //D7-D0 DT1[7:0]设置数据写入系统的延时时间
    //PWM 112.5Khz
    spi_write_reg(0x20, 0x1e03);
    //22h 27h 
    //D7-D0 DT2A[7:0]和 DT2B[7:0]设置α 电机和β 电机开始转动前的等待延迟时间。
    //D13-D10  α 电机和β 电机电流的相位差分别由 PHMODAB[5:0]和 PHMODCD[5:0]设置
    spi_write_reg(0x22, 0x0001);
    spi_write_reg(0x27, 0x0001); //
    //23h 28h  PWM 波的最大占空比， 决定了驱动器 A 到 D 输出电流峰值的位置
    //最大占空比 = PPWx/ (PWMMODE × 8) =x/(0x1e*8)
    spi_write_reg(0x23, 0xb0b0); // AB PWM duty
    spi_write_reg(0x28, 0xb0b0); // CD PWM duty
    //24h 29h 
    //D7-D0 PSUMAB[7:0]和 PSUMCD[7:0]分别设置α 电机和β 电机的总步数
    //D8 0 正1 反
    //D9 0正常, 1刹车
    //D10 输出控制
    //D12-D13 分频数
    //spi_write_reg(0x24, 0X1C00); // AB control reg
    //spi_write_reg(0x29, 0X1C00); // CD control reg
    //设置VD=100HZ，800pps, 2-2,INTCTxx[15:0] × 768 = OSCIN 频率 / 转动频率
    //INTCTxx[15:0] = 176
    //PSUMxx[7:0] = 1/（100Hz） × 27MHz/ (176 × 24) = 64
    //                   VD     OSC      PSUMxx
    spi_write_reg(0x25, 176); // INTCTAB,
    spi_write_reg(0x2a, 176); // INTCTCD, 176
    toggle_vdfz(1);
    return 0;
}
/*
 * 初始化SPI
 */
int spi_init(void)
{
    int ret = 0;
    /*打开 SPI 设备*/
    spi_dev_fd = open(device, O_RDWR);
    if (spi_dev_fd < 0)
    {
        printf("can't open %s", device);
    }

    /*
     * spi mode 设置SPI 工作模式
     */
    ret = ioctl(spi_dev_fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1)
    {
        printf("can't set spi mode");
        return -1;
    }

    ret = ioctl(spi_dev_fd, SPI_IOC_RD_MODE, &mode);
    if (ret == -1)
    {
        printf("can't get spi mode");
        return -1;
    }

    /*
     * bits per word  设置一个字节的位数
     */
    ret = ioctl(spi_dev_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
    {
        printf("can't set bits per word");
        return -1;
    }

    ret = ioctl(spi_dev_fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
    {
        printf("can't get bits per word");
        return -1;
    }

    /*
     * max speed hz  设置SPI 最高工作频率
     */
    ret = ioctl(spi_dev_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
    {
        printf("can't set max speed hz");
        return -1;
    }

    ret = ioctl(spi_dev_fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
    {
        printf("can't get max speed hz");
        return -1;
    }

    printf("spi mode: 0x%x\n", mode);
    printf("bits per word: %d\n", bits);
    printf("max speed: %d Hz (%d KHz)\n", speed, speed / 1000);
    return 0;
}
/**
 * spi 单字节发送
 * */
int spi_write_byte(int fd, unsigned char data)
{
    int ret = 0;
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)&data,
        .rx_buf = 0, // SPI Can't support synchronous transfer
        .len = 1,
    };

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
    {
        printf("fd:%d can't send spi message\n", fd);
        return -1;
    }
    return 0;
}

int spi_read_reg(int addr, unsigned char* rbuf)
{
    int ret = 0;
    unsigned char addr_byte = (addr & 0xff) +0x40;
    struct spi_ioc_transfer tr[2];
    memset(tr, 0, 2 * sizeof(struct spi_ioc_transfer));
    tr[0].tx_buf = &addr_byte;
    tr[0].len = 1;
    //tr[0].bits_per_word = bits,

    tr[1].tx_buf = 0;
    tr[1].rx_buf = rbuf;
    tr[1].len = 2;
    //tr[1].bits_per_word = bits,

    ret = ioctl(spi_dev_fd, SPI_IOC_MESSAGE(2), tr);
    if (ret < 1)
    {
        printf(" can't send spi message\n");
        return -1;
    }
    return 0;
}

/**
 * spi 单字节读
 * */
int spi_read_byte(int fd, unsigned char data_cmd, unsigned char* data)
{
    int ret = 0;
    struct spi_ioc_transfer tr[2];
    memset(tr, 0, 2 * sizeof(struct spi_ioc_transfer));
    tr[0].tx_buf = &data_cmd;
    tr[0].len = 1;
    tr[0].bits_per_word = bits,

    tr[1].tx_buf = 0;
    tr[1].rx_buf = data;
    tr[1].len = 1;
    tr[1].bits_per_word = bits,

    ret = ioctl(fd, SPI_IOC_MESSAGE(2), tr);
    if (ret < 1)
    {
        printf("fd:%d can't send spi message\n", fd);
        return -1;
    }
    return 0;
}

/**
 * spi 多字节发送
 * */
int spi_write_bytes(int fd, unsigned char *pdata_buff, unsigned int len)
{
    int ret = 0;
    struct spi_ioc_transfer tr = {
        .tx_buf = pdata_buff,
        .len = len, // SPI Can't support synchronous transfer
        .rx_buf = NULL,
    };

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
    {
        printf("fd:%d can't send spi message\n", fd);
        return -1;
    }
    return 0;
}


