/**
	lux_record.h author: humy 2022.12.6
	录像模块文件
**/

#ifndef __LUX_RECORD_H__
#define __LUX_RECORD_H__

#include <comm_def.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#include "lux_base.h"

#define LUX_RECORD_CACHE_PACKET_MAX_NUM 1

#define MAX_MMC_NAME_LEN        16
#define MAX_MOUNTS_INFO_LEN     1024
#define LINUX_SD_DEV_FILE       "/dev/mmcblk0"
#define LINUX_MOUNT_INFO_FILE   "/proc/mounts"
#define SD_MOUNT_PATH           "/mnt/sdcard"
#define FORMAT_CMD              "mkfs.vfat"

#ifdef PLATFORM_T31
#define RECORD_BUF_MAX_SIZE 600*1024
#define PLAYBACK_BUF_SIZE 400*1024
#else
#define RECORD_BUF_MAX_SIZE 2*1024*1024
#define PLAYBACK_BUF_SIZE 1024*1024
#endif

#define ORIGIN_RECORD_FILE_NAME "origin_file.data"

static char s_mounts_info[MAX_MOUNTS_INFO_LEN];
static char s_mmcblk_name_p1[MAX_MMC_NAME_LEN] = { 0 };
static char s_mmcblk_name_p2[MAX_MMC_NAME_LEN] = { 0 };
static int isformatted = 0;
static unsigned char frame_head_magic[2] = {0xAE, 0xEF};

typedef struct frame_head frame_head_t;

typedef enum
{
	SD_STATUS_UNKNOWN = 0,
	SD_STATUS_NORMAL,
	SD_STATUS_ABNORMAL,
	SD_STATUS_LACK_SPACE,
	SD_STATUS_FORMATING,
	SD_STATUS_NOT_EXIST,
	SD_STATUS_MAX
}E_SD_STATUS;

E_SD_STATUS LUX_ipc_sd_get_status(VOID);
void LUX_ipc_sd_format(void);
void LUX_ipc_sd_remount(void);

typedef struct record_info record_info_t;

typedef struct lux_record
{
	unsigned char is_running:1;
	unsigned char need_re_stat_space:1;
	uint64_t free_space;
	uint64_t total_space;
	char basePath_DCIM[64];
	char cur_file_path[64];
	char origin_file_path[64];
	OSI_SEM sem_create_origin_file;
	record_info_t* cur_record_info;
	unsigned int cur_record_info_file_pos;
	int fpRecord;
	int fpInfo;
	unsigned int writeByteNum;
	unsigned int recordFileMaxByteNum;
	unsigned int originFileMaxByteNum;
	unsigned char stop_record_thread;
	unsigned char stop_loop_delete_thread;
	unsigned char *frame_buf;
	int           data_len;
	pthread_mutex_t *mutex;
	pthread_mutex_t* playback_mutex;
	pthread_mutex_t* freespace_mutex;
	unsigned char *playback_framebuf;
	int stat_space_time_s;
}lux_record_t;

/**
录像文件格式：record_info
            + frame1[frame_head+audio_frame_head+audio_data]
			+ frame2[frame_head+video_frame_head+video_data]
			+ frame3+... ...
**/

/**
音频编码类型**
**/
enum {
	AUDIO_ENCODE_PCM = 100,
	AUDIO_ENCODE_G711A = 101,
	AUDIO_ENCODE_G711U = 102,
	AUDIO_ENCODE_AAC = 103,
	AUDIO_ENCODE_G726 = 104,
	AUDIO_ENCODE_G729 = 105,
	AUDIO_ENCODE_MP3 = 106
};

/**
视频编码类型**
**/
enum
{
	VIDEO_ENCODE_H264 = 0,
	VIDEO_ENCODE_265 = 1,
	VIDEO_ENCODE_MPEG4 = 2,
	VIDEO_ENCODE_MPEG1 = 3
};

enum
{
	FRAME_TYPE_UNKNOWN = 0,
	FRAME_TYPE_VIDEO = 1,
	FRAME_TYPE_AUDIO = 2,
	FRAME_TYPE_IVS = 3,
};

enum
{
	VIDEO_UNKNOWN_FRAME = 0,
	VIDEO_I_FRAME = 1,
	VIDEO_P_FRAME = 2,
};

/** 
录像文件头结构体**
**/
typedef struct record_info
{
	char file_name[16];
	uint64_t record_size;
	uint64_t start_time_ms;
	uint64_t end_time_ms;
}record_info_t;

/**
录像帧头结构体**
**/
typedef struct frame_head
{
	int  frame_type;  //0:unknown, 1:video frame,2:audio frame,3:IVS frame  ... ...
	int frame_head_len;
	int   frame_data_len;
	uint64_t timestamp_ms;
}frame_head_t;

/**
音频数据帧头**
**/
typedef struct audio_frame_head
{
	char encode; //100:pcm, 101:g711A, 102:g711U, 103:AAC, 104:g726, 105:g729, 106:mp3
	char channel_num;
	char bits;	
	char format;
	int  sample_rate;
	int  bit_rates;
	int  frame_len;
}audio_frame_head_t;

/**
视频数据帧头**
**/
typedef struct video_frame_head
{
	char encode;  //0:h264, 1 : h265, 2 : mpeg4, 3 : mpeg1
	char frame_type;    //0:unknown, 1:key frame, 2: P frame;
	unsigned short image_width;
	unsigned short image_height;
	unsigned short  resv1;
	int  resv2;
	int  frame_len;
}video_frame_head_t;

uint64_t get_sdcard_free_space();
int      set_sdcard_free_space(uint64_t val);

/**
* @description: 录像模块初始化
* @param [in] 
* @return 
*/
int LUX_Record_Init();

/**
* @description: 启动录像模块
* @param [in]
* @return
*/
int LUX_Record_Start();

/**
* @description:分配一个包缓存
* @param [in]
* @return
*/
frame_head_t* LUX_Record_alloc_packet(int type, int data_len);

/**
* @description:从队列中拉取一个包
* @param [in]
* @return
*/
frame_head_t *LUX_Record_pull_packet();

/**
* @description:将一个包推进队列
* @param [in]
* @return
*/
int LUX_Record_push_packet(frame_head_t *pack);

/**
* @description:释放包缓存
* @param [in]
* @return
*/
int LUX_Record_free_packet(frame_head_t* pack);

/**
* @description:按开始时间查找录像文件
* @param [in]
* @return
*/
int LUX_Record_Find_By_Time(IN const uint64_t startTime_ms, OUT char *file_name);

/**
* @description:停止录像模块
* @param [in]
* @return
*/
int LUX_Record_Stop();

/**
* @description:销毁录像模块
* @param [in]
* @return
*/
int LUX_Record_Final();

#endif
