// #include <stdio.h>
// #include <fcntl.h>
// #include <stdlib.h>
#ifndef __MS419XX_H__
#define __MS419XX_H__

#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif/*__cplusplus*/

// extern FILE* gpio_open(int index);

int spi_init(void);
int ms419xx_init(void);
int spi_write_reg(int addr, int value);
int spi_write_bytes(int fd, unsigned char *pdata_buff, unsigned int len);
int spi_read_reg(int addr, unsigned char* value);
void toggle_vdfz(int cycle);
void ms419_reset(void);
void ms419xx_stop();
void ms419xx_start();
void ms419xx_zoom_in(int step);
void ms419xx_zoom_out(int step);
void ms419xx_focus_near(int step);
void ms419xx_focus_far(int step);

#ifdef __cplusplus
}
#endif/*__cplusplus*/
#endif /*__LUX_HAL_MOTOR_H__*/
