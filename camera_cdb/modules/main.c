/*
 * main.c
 *
 * 主函数，对接涂鸦
 * 
 */

#include <stdio.h>


int main(int argc, char *argv[])
{
  int ret = -1;

  /*
  init();
  LUXSHARE_product_common();
  LUXSHARE_product_led();
  LUXSHARE_IMP();
  LUXSHARE_SYSUTILS();
  LUXSHARE_tuya();
  LUXSHARE_sample();
  */
  printf("%s:%d for test!\n", __func__, __LINE__);
  //ret = LUSHARE_test_sample_Encode_video(argc, &argv[0]);
  printf("%s:%d for test!  ret:%d\n", __func__, __LINE__, ret);

  return 0;

}
