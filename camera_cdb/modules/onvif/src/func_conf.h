#ifndef __FUNC_CONF_H__

#define _STR(s) #s
#define STR(s) _STR(s)

#if 1
#define TDS_DEVICE_INFO_MANUFACTURER "Luxshare"
#define TDS_DEVICE_INFO_MODE "CameraX"

#define DATE 20221228

#define ONVIF_VER_MAJOR 2
#define ONVIF_VER_MINOR 0

#if (!defined DATE)
#error should define DATE in makefile
#endif

//configs for onvif alone
#if 1

#if (!defined ONVIF_VER_MAJOR) || (!defined ONVIF_VER_MINOR)
#error should define ONVIF_VER in makefile
#endif
//#define TDS_DEVICE_INFO_FW_VERSION "XCAM-T40-2.0-20221228"
#define TT_MEDIA_0_NAME "main"
#define TT_MEDIA_1_NAME "second"

//configs for xcam
#else
#if (!defined XCAM_VER_MAJOR) || (!defined XCAM_VER_MINOR)
#error should define XCAM_VER in makefile
#endif
// #define TDS_DEVICE_INFO_FW_VERSION "XCAM-T31-"STR(XCAM_VER_MAJOR)"."STR(XCAM_VER_MINOR)"-"STR(DATE)
// #define TT_MEDIA_0_NAME "stream0"
// #define TT_MEDIA_1_NAME "stream1"

#endif




#elif defined T21
#define TDS_DEVICE_INFO_MANUFACTURER "Ingenic"
#define TDS_DEVICE_INFO_MODE "xcam"

#if (!defined DATE)
#error should define DATE in makefile
#endif

//configs for onvif alone
#ifdef ONVIF_WITH_CARRIER_SERVER
#if (!defined ONVIF_VER_MAJOR) || (!defined ONVIF_VER_MINOR)
#error should define ONVIF_VER in makefile
#endif
#define TDS_DEVICE_INFO_FW_VERSION "XCAM-T21-"STR(ONVFI_VER_MAJOR)"."STR(ONVFI_VER_MINOR)"-"STR(DATE)
#define TT_MEDIA_0_NAME "main"
#define TT_MEDIA_1_NAME "second"

//configs for xcam
#else
#if (!defined XCAM_VER_MAJOR) || (!defined XCAM_VER_MINOR)
#error should define XCAM_VER in makefile
#endif
// #define TDS_DEVICE_INFO_FW_VERSION "XCAM-T21-"STR(XCAM_VER_MAJOR)"."STR(XCAM_VER_MINOR)"-"STR(DATE)
// #define TT_MEDIA_0_NAME "stream0"
// #define TT_MEDIA_1_NAME "stream1"

#endif


#elif defined T40
#define TDS_DEVICE_INFO_MANUFACTURER "Ingenic"
#define TDS_DEVICE_INFO_MODE "xcam"

#if (!defined DATE)
#error should define DATE in makefile
#endif

//configs for onvif alone
#ifdef ONVIF_WITH_CARRIER_SERVER
#if (!defined ONVIF_VER_MAJOR) || (!defined ONVIF_VER_MINOR)
#error should define ONVIF_VER in makefile
#endif
#define TDS_DEVICE_INFO_FW_VERSION "XCAM-T40-"STR(ONVFI_VER_MAJOR)"."STR(ONVFI_VER_MINOR)"-"STR(DATE)
#define TT_MEDIA_0_NAME "main"
#define TT_MEDIA_1_NAME "second"

//configs for xcam
#else
#if (!defined XCAM_VER_MAJOR) || (!defined XCAM_VER_MINOR)
#error should define XCAM_VER in makefile
#endif
// #define TDS_DEVICE_INFO_FW_VERSION "XCAM-T40-"STR(XCAM_VER_MAJOR)"."STR(XCAM_VER_MINOR)"-"STR(DATE)
// #define TT_MEDIA_0_NAME "stream0"
// #define TT_MEDIA_1_NAME "stream1"

#endif


#elif defined T30

#endif

#endif
