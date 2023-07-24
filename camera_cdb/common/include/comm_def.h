/*
 * comm_def.h
 *
 * 公共宏定义，数据结构
 * 
 */

#ifndef __COMM_DEF_H__
#define __COMM_DEF_H__

#include <unistd.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

typedef enum
{
    LUX_FALSE,
    LUX_TRUE,

}BOOL_X;

typedef long long DLONG_X;
typedef DLONG_X *PDLONG_X;
typedef unsigned long long DDWORD_X;
typedef DDWORD_X *PDDWORD_X;
typedef float FLOAT_X;
typedef FLOAT_X *PFLOAT_X;
typedef signed int INT_X;
typedef int *PINT_X;
typedef void VOID_X;
typedef void *PVOID_X;
typedef char CHAR_X;
typedef char *PCHAR_X;
typedef signed char SCHAR_X;
typedef unsigned char UCHAR_X;
typedef short SHORT_X;
typedef unsigned short USHORT_X;
typedef short *PSHORT_X;
typedef long LONG_X;
typedef unsigned long ULONG_X;
typedef long *PLONG_X;
typedef unsigned char BYTE_X;
typedef BYTE_X *PBYTE_X;
typedef unsigned short WORD_X;
typedef WORD_X *PWORD_X;
typedef unsigned int DWORD_X;
typedef DWORD_X *PDWORD_X;
typedef unsigned int UINT_X;
typedef unsigned int *PUINT_X;
typedef BOOL_X *PBOOL_X;
typedef long long int INT64_X;
typedef INT64_X *PINT64_X;
typedef unsigned long long int UINT64_X;
typedef UINT64_X *PUINT64_X;
typedef short INT16_X;
typedef INT16_X *PINT16_X;
typedef unsigned short UINT16_X;
typedef UINT16_X *PUINT16_X;
typedef char INT8_X;
typedef INT8_X *PINT8_X;
typedef unsigned char UINT8_X;
typedef UINT8_X *PUINT8_X;
typedef double DOUBLE_X;
typedef double *PDOUBLE_X;

typedef ULONG_X TIME_MS;
typedef ULONG_X TIME_S;
typedef unsigned int TIME_X;

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef CONST
#define CONST const
#endif

#ifndef CHAR_T
#define CHAR_T char
#endif

#ifndef UINT_T
#define UINT_T unsigned int
#endif

#ifndef INT_T
#define INT_T int
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define SENSOR_FRAME_RATE_NUM       25
#define SENSOR_FRAME_RATE_DEN       1

#if 0
#define SENSOR_WIDTH_DEFAULT    2560
#define SENSOR_HEIGHT_DEFAULT   1440
#else
#define SENSOR_WIDTH_DEFAULT    1920
#define SENSOR_HEIGHT_DEFAULT   1080
#endif

#define CHN0_EN                 1
#define CHN1_EN                 1
#define CHN2_EN                 0
#define CHN3_EN                 0
#define CROP_EN                 0


#define SENSOR_WIDTH_SECOND     800
#define SENSOR_HEIGHT_SECOND    600

#define SENSOR_WIDTH_THIRD      640
#define SENSOR_HEIGHT_THIRD     360

#define BITRATE_720P_Kbs        1000


#define STREAM_FILE_PATH_PREFIX      "/mnt"
#define SNAP_FILE_PATH_PREFIX        "/mnt"
//#define INI_FILE_PATH                "/system/init/"
//#define INI_JIO_FILE_PATH            "/system/jio/"
#define LUX_CONFIG_FILE              "/system/config/luxconfig.ini"
#define INGENIT_ALG_FILE_NAME        "/system/etc/license.txt"
#define AUDIO_PCM_FILE_PATH          "/system/bin/audio/"
#define UNNORMAL_REBOOT_FILE_NAME    "/system/bin/unnormalReboot.txt"    //非常规重启标识文件
#define TUYA_DEBUG_LEVEL_FILE_NAME   "/system/init/Debug/IsDebug.txt"    //TUYA Debug Level 标识文件[0～5]
#define TUYA_EVENTS_TIME_DIR         "/system/init/event/"               //用于记录事件时间的文件夹
/* OSD 时间戳宽度和高度,根据每个分辨率的bitmapinfo头文件进行更改 */
#define OSD_GRP0_REGION_1440P_W      48
#define OSD_GRP0_REGION_1440P_H      98
#define OSD_GRP0_REGION_1080P_W      36
#define OSD_GRP0_REGION_1080P_H      74

#define OSD_GRP1_REGION_360P_W       12
#define OSD_GRP1_REGION_360P_H       26


#define FS_CHN_NUM      2  //MIN 1,MAX 4
#define IVS_CHN_ID      3

/* VIDEO 编解码模块 */
#define VIDEO_ENC_CHN_MAX (CHN0_EN + CHN1_EN + CHN2_EN + CHN3_EN)
#define VIDEO_ENC_MAIN_FRAMERATE 25 //主码流帧率：fps，暂不支持修改
#define VIDEO_ENC_SUB_FRAMERATE  25 //子码流帧率：fps，暂不支持修改
#define CH0_INDEX  0
#define CH1_INDEX  1
#define CH2_INDEX  2
#define CH3_INDEX  3


/*ivs 智能算法模块*/
//#ifdef CONFIG_PTZ_IPC
#define LUX_IVS_ALGO_NUM       3        /*IVS算法个数*/
#define LUX_IVS_ALGO_TYPE      3        /*IVS算法类型*/
// #else
// #define LUX_IVS_ALGO_NUM       2        /*IVS算法个数*/
// #define LUX_IVS_ALGO_TYPE      3        /*IVS算法类型*/
// #endif/*CONFIG_PTZ_IPC*/

#define LUX_IVS_REGION_NUM     1        /*报警区域个数*/
#define LUX_PERMDET_REGION_NUM     6        /*报警区域个数*/
#define LUX_COMM_EVENT_RECORD_TIME (30*1000)   /* 事件录像时长30s */

/* 复位按键持续时间间隔 */
#define LUX_KEY_RESET_TIME_INTERVAL 5   //单位：秒

/* 云存储录像时长 */
#define LUX_CLOUD_STORAGE_TIME 30  //单位：秒

/* 温湿度报警阈值 */
#define LUX_TUYA_TEMP_ALARM_THRESHOLD   41*10 //超过此阈值--41摄氏度就报警
#define LUX_TUYA_HUMI_ALARM_THRESHOLD   90*10 //超过此阈值--90TH就报警

/* 哭声报警触发呼吸灯持续时间 */
#define LUX_BABY_CRY_PWM_BLUB_TIME (60*1000) //ms
/* 哭声报警触发安抚音乐的额外次数 */
#define LUX_BABY_CRY_MUSIC_EXTRO_TIMES  2
//#define  CHN_NUM  ARRAY_SIZE(chn)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __COMM_DEF_H__ */
