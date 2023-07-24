[  1%] Building C object CMakeFiles/media_sample.dir/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_deviceio.c.o
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_deviceio.c: In function '__tds__GetServices':
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_deviceio.c:2112:59: warning: implicit declaration of function 'soap_zalloc'; did you mean 'soap_malloc'? [-Wimplicit-function-declaration]
     struct tds__Service *Service = (struct tds__Service *)soap_zalloc(soap, sizeof(struct tds__Service));
                                                           ^~~~~~~~~~~
                                                           soap_malloc
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_deviceio.c: In function '__tds__GetNetworkInterfaces':
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_deviceio.c:2984:21: warning: assignment makes pointer from integer without a cast [-Wint-conversion]
     Manual->Address = soap_zalloc(soap, STR_LEN_IP_ADDRESS);
                     ^
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_deviceio.c:2936:9: warning: unused variable 'ret' [-Wunused-variable]
     int ret = 0;
         ^~~
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_deviceio.c: In function '__tds__GetNetworkDefaultGateway':
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_deviceio.c:3068:23: warning: initialization from incompatible pointer type [-Wincompatible-pointer-types]
  char **IPv4Address = (char*)soap_zalloc(soap, 20);
                       ^
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_deviceio.c: At top level:
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_deviceio.c:3592:1: warning: multi-line comment [-Wcomment]
 // /******************************************************************************\
 ^
[  2%] Building C object CMakeFiles/media_sample.dir/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_devicemgmt.c.o
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_devicemgmt.c:2135:1: warning: multi-line comment [-Wcomment]
 // /******************************************************************************\
 ^
[  3%] Building C object CMakeFiles/media_sample.dir/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_display.c.o
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_display.c:266:1: warning: multi-line comment [-Wcomment]
 // /******************************************************************************\
 ^
[  4%] Building C object CMakeFiles/media_sample.dir/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_event.c.o
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_event.c:527:1: warning: multi-line comment [-Wcomment]
 // /******************************************************************************\
 ^
[  5%] Building C object CMakeFiles/media_sample.dir/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_imaging.c.o
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_imaging.c: In function '__timg__GetImagingSettings':
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_imaging.c:241:77: warning: implicit declaration of function 'soap_zalloc'; did you mean 'soap_malloc'? [-Wimplicit-function-declaration]
     struct tt__ImagingSettings20* Settings = (struct tt__ImagingSettings20*)soap_zalloc(soap, sizeof(struct tt__ImagingSettings20));
                                                                             ^~~~~~~~~~~
                                                                             soap_malloc
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_imaging.c: At top level:
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_imaging.c:470:1: warning: multi-line comment [-Wcomment]
 // /******************************************************************************\
 ^
[  6%] Building C object CMakeFiles/media_sample.dir/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c.o
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c: In function '__trt__GetVideoSources':
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:1092:70: warning: implicit declaration of function 'soap_zalloc'; did you mean 'soap_malloc'? [-Wimplicit-function-declaration]
     struct tt__VideoSource *VideoSources = (struct tt__VideoSource *)soap_zalloc(soap, sizeof(struct tt__VideoSource) * 1);
                                                                      ^~~~~~~~~~~
                                                                      soap_malloc
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c: In function '__trt__GetVideoEncoderConfigurations':
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:1505:26: warning: assignment from incompatible pointer type [-Wincompatible-pointer-types]
  Configurations[0].__any = (char *)soap_zalloc(soap, 100);
                          ^
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:1540:26: warning: assignment from incompatible pointer type [-Wincompatible-pointer-types]
  Configurations[1].__any = (char *)soap_zalloc(soap, 100);
                          ^
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c: In function '__trt__GetVideoEncoderConfiguration':
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:1739:8: warning: implicit declaration of function 'LUX_Video_Func_Get_Fps' [-Wimplicit-function-declaration]
  ret = LUX_Video_Func_Get_Fps(&iFrmRateNum, &iFrmRateDen);
        ^~~~~~~~~~~~~~~~~~~~~~
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:1748:8: warning: implicit declaration of function 'LUX_Video_Func_Get_Bitrate'; did you mean 'func_video_get_bitrate'? [-Wimplicit-function-declaration]
  ret = LUX_Video_Func_Get_Bitrate(iEncChnNum, &iBitRate);
        ^~~~~~~~~~~~~~~~~~~~~~~~~~
        func_video_get_bitrate
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:1756:8: warning: implicit declaration of function 'LUX_Video_Fnuc_Get_Resoulution'; did you mean 'func_video_get_resoulution'? [-Wimplicit-function-declaration]
  ret = LUX_Video_Fnuc_Get_Resoulution(iFsChnNum, &iPicWidth, &iPicHeigth);
        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        func_video_get_resoulution
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c: In function '__trt__SetVideoEncoderConfiguration':
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:1951:11: warning: implicit declaration of function 'LUX_Video_Change_Resolution'; did you mean 'func_video_get_resoulution'? [-Wimplicit-function-declaration]
     ret = LUX_Video_Change_Resolution(frmChnNum, picWidth, picHeigth);
           ^~~~~~~~~~~~~~~~~~~~~~~~~~~
           func_video_get_resoulution
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:1965:11: warning: implicit declaration of function 'LUX_Video_Func_Set_Fps' [-Wimplicit-function-declaration]
     ret = LUX_Video_Func_Set_Fps(impvi_num,&frmRateNum, &frmRateDen);
           ^~~~~~~~~~~~~~~~~~~~~~
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:1977:11: warning: implicit declaration of function 'LUX_Video_Func_Set_Bitrate'; did you mean 'func_video_get_bitrate'? [-Wimplicit-function-declaration]
     ret = LUX_Video_Func_Set_Bitrate(encChnNum, targetBitRate);
           ^~~~~~~~~~~~~~~~~~~~~~~~~~
           func_video_get_bitrate
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:1983:5: warning: implicit declaration of function 'LUX_Video_Get_Bitrate'; did you mean 'func_video_get_bitrate'? [-Wimplicit-function-declaration]
     LUX_Video_Get_Bitrate(encChnNum, &targetBitRate);
     ^~~~~~~~~~~~~~~~~~~~~
     func_video_get_bitrate
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c: At top level:
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:2133:31: error: redefinition of '__trt__GetAudioEncoderConfigurationOptions'
     SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurationOptions(struct soap* soap, struct _trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions, struct _trt__GetAudioEncoderConfigurationOptionsResponse *trt__GetAudioEncoderConfigurationOptionsResponse){
                               ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:2109:27: note: previous definition of '__trt__GetAudioEncoderConfigurationOptions' was here
 SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurationOptions(struct soap* soap, struct _trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions, struct _trt__GetAudioEncoderConfigurationOptionsResponse *trt__GetAudioEncoderConfigurationOptionsResponse)
                           ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c: In function '__trt__CreateOSD':
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:2434:22: warning: format '%d' expects argument of type 'int', but argument 2 has type 'double' [-Wformat=]
     printf("pos_x : %d\n", pos_x);
                     ~^
                     %f
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:2435:22: warning: format '%d' expects argument of type 'int', but argument 2 has type 'double' [-Wformat=]
     printf("pos_y : %d\n", pos_y);
                     ~^
                     %f
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:2443:40: warning: passing argument 2 of 'func_osd_create_text' makes integer from pointer without a cast [-Wint-conversion]
  ret = func_osd_create_text(streamnum, token, x, y, text);
                                        ^~~~~
In file included from /home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:33:0:
/home/sj/camaraX/camera_cdb/main/../modules/onvif/src/func_osd.h:9:12: note: expected 'int' but argument is of type 'char *'
 extern int func_osd_create_text(int streamnum, int pos_x, int pos_y, char *text, char *token);
            ^~~~~~~~~~~~~~~~~~~~
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:2443:50: warning: passing argument 4 of 'func_osd_create_text' makes pointer from integer without a cast [-Wint-conversion]
  ret = func_osd_create_text(streamnum, token, x, y, text);
                                                  ^
In file included from /home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:33:0:
/home/sj/camaraX/camera_cdb/main/../modules/onvif/src/func_osd.h:9:12: note: expected 'char *' but argument is of type 'int'
 extern int func_osd_create_text(int streamnum, int pos_x, int pos_y, char *text, char *token);
            ^~~~~~~~~~~~~~~~~~~~
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c: In function '__trt__DeleteOSD':
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:2461:8: warning: unused variable 'token' [-Wunused-variable]
  char* token = trt__DeleteOSD->OSDToken;
        ^~~~~
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:2459:6: warning: unused variable 'ret' [-Wunused-variable]
  int ret = 0;
      ^~~
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c: At top level:
/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c:2468:1: warning: multi-line comment [-Wcomment]
 // /******************************************************************************\
 ^
CMakeFiles/media_sample.dir/build.make:1181: recipe for target 'CMakeFiles/media_sample.dir/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c.o' failed
make[2]: *** [CMakeFiles/media_sample.dir/home/sj/camaraX/camera_cdb/modules/onvif/src_soap_new_2/function_module/onvif_func_media.c.o] Error 1
CMakeFiles/Makefile2:84: recipe for target 'CMakeFiles/media_sample.dir/all' failed
make[1]: *** [CMakeFiles/media_sample.dir/all] Error 2
Makefile:90: recipe for target 'all' failed
make: *** [all] Error 2
