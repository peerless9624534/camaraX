/*************************************************************************


 ************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include "comm.h"
#include "rtsp_demo.h"
#include "lux_global.h"
#include "lux_config.h"
#include <signal.h>
#include "comm_func.h" 
static int flag_run = 0;
static void sig_proc(int signo)
{
	flag_run = 0;
}

static int get_next_video_frame (FILE *fp, uint8_t **buff, int *size)
{
	uint8_t szbuf[1024];
	int szlen = 0;
	int ret;
	if (!(*buff)) {
		*buff = (uint8_t*)malloc(2*1024*1024);
		if (!(*buff))
			return -1;
	}

	*size = 0;

	while ((ret = fread(szbuf + szlen, 1, sizeof(szbuf) - szlen, fp)) > 0) {
		int i = 3;
		szlen += ret;
		while (i < szlen - 3 && !(szbuf[i] == 0 &&  szbuf[i+1] == 0 && (szbuf[i+2] == 1 || (szbuf[i+2] == 0 && szbuf[i+3] == 1)))) i++;
		memcpy(*buff + *size, szbuf, i);
		*size += i;
		memmove(szbuf, szbuf + i, szlen - i);
		szlen -= i;
		if (szlen > 3) {
			//printf("szlen %d\n", szlen);
			fseek(fp, -szlen, SEEK_CUR);
			break;
		}
	}
	if (ret > 0)
		return *size;
	return 0;
}

static int get_next_video_frame_from_buffer (uint8_t *ptr, int szlen, uint8_t **buff, int *size) {
    uint8_t szbuf[1024];
    int ret;
    int i = 3;

    if (!(*buff)) {
        *buff = (uint8_t*)malloc(2*1024*1024);
        if (!(*buff))
            return -1;
    }
    *size = 0;
	
    while (szlen > 0) {
        // 从szbuf末尾开始添加新数据
        int copy_len = szlen > sizeof(szbuf) - i ? sizeof(szbuf) - i : szlen;
        memcpy(szbuf + i, ptr + (*size), copy_len);
        szlen -= copy_len;
		
        // 在缓冲区szbuf中查找video帧
        while (i < sizeof(szbuf) - 3 && !(szbuf[i] == 0 && szbuf[i+1] == 0 && (szbuf[i+2] == 1 || (szbuf[i+2] == 0 && szbuf[i+3] == 1))))
            i++;

        // 如果找到了video帧，将其复制到输出缓冲区*buff中
        if (i < sizeof(szbuf) - 3) {
            memcpy(*buff + *size, szbuf, i);
            *size += i;
            memmove(szbuf, szbuf + i, sizeof(szbuf) - i);
            i = 0;
        }

        // 如果遍历全部szbuf都没有找到video帧，可能需要尝试继续读取新的数据
        if (i >= sizeof(szbuf) - 3)
            i = sizeof(szbuf);

    }
    return (*size > 0) ? *size : 0;
}


static int get_next_audio_frame (FILE *fp, uint8_t **buff, int *size)
{
	int ret;
#define AUDIO_FRAME_SIZE 320
	if (!(*buff)) {
		*buff = (uint8_t*)malloc(AUDIO_FRAME_SIZE);
		if (!(*buff))
			return -1;
	}

	ret = fread(*buff, 1, AUDIO_FRAME_SIZE, fp);
	if (ret > 0) {
		*size = ret;
		return ret;
	}
	return 0;
}

#define MAX_SESSION_NUM 64
#define DEMO_CFG_FILE "demo.cfg"

struct demo_cfg
{
	int session_count;
	struct {
		char path[64];
		char video_file[64];
		char audio_file[64];
	} session_cfg[MAX_SESSION_NUM];
};

static int load_cfg(struct demo_cfg *cfg, const char *cfg_file)
{
//cfgline: path=%s video=%s audio=%s
	FILE *fp = fopen(cfg_file, "r");
	char line[256];
	int count = 0;

	if (!fp) {
		fprintf(stderr, "open %s failed\n", cfg_file);
		return -1;
	}

	memset(cfg, 0, sizeof(*cfg));
	while (fgets(line, sizeof(line) - 1, fp)) {
		const char *p;
		memset(&cfg->session_cfg[count], 0, sizeof(cfg->session_cfg[count]));

		if (line[0] == '#')
			continue;

		p = strstr(line, "path=");
		if (!p)
			continue;
		if (sscanf(p, "path=%s", cfg->session_cfg[count].path) != 1)
			continue;
		if ((p = strstr(line, "video="))) {
			if (sscanf(p, "video=%s", cfg->session_cfg[count].video_file) != 1) {
				fprintf(stderr, "parse video file failed %s\n", p);
			}
		}
		if ((p = strstr(line, "audio="))) {
			if (sscanf(p, "audio=%s", cfg->session_cfg[count].audio_file) != 1) {
				fprintf(stderr, "parse audio file failed %s\n", p);
			}
		}
		if (strlen(cfg->session_cfg[count].video_file) || strlen(cfg->session_cfg[count].audio_file)) {
			count ++;
		} else {
			fprintf(stderr, "parse line %s failed\n", line);
		}
	}
	cfg->session_count = count;
    fclose(fp);
	return count;
}


 rtsp_demo_handle demo;
 rtsp_session_handle session[2] = { NULL };
pthread_mutex_t event_mMutex;	

int rtsp_server_init()
{

	int session_count = 0;
	int ch;
	struct demo_cfg cfg;
	int rtsp_video0_enc, rtsp_video1_enc;

	demo = rtsp_new_demo(554);
	if (NULL == demo) {
		printf("rtsp_new_demo failed\n");
		return 0;
	}
	if (16777217 == g_configData->media_config.video_config.chn0_encAttr_eProfile)
	{
		rtsp_video0_enc = RTSP_CODEC_ID_VIDEO_H265;
	}
	else
	{
		rtsp_video0_enc = RTSP_CODEC_ID_VIDEO_H264;
	}
	if (16777217 == g_configData->media_config.video_config.chn1_encAttr_eProfile)
	{
		rtsp_video1_enc = RTSP_CODEC_ID_VIDEO_H265;
	}
	else
	{
		rtsp_video1_enc = RTSP_CODEC_ID_VIDEO_H264;
	}
	sprintf(cfg.session_cfg[0].path, "%s", "/main");
	sprintf(cfg.session_cfg[1].path, "%s", "/second");
	session[0] = rtsp_new_session(demo, cfg.session_cfg[0].path);
	if (NULL == session[0]) {
		printf("rtsp_new_session failed\n");
		return;
	}
	rtsp_set_video(session[0], rtsp_video0_enc, NULL, 0);
	rtsp_set_audio(session[0], RTSP_CODEC_ID_AUDIO_AAC, NULL, 0);
	session[1] = rtsp_new_session(demo, cfg.session_cfg[1].path);
	if (NULL == session[1]) {
		printf("rtsp_new_session failed\n");
		return;
	}
	rtsp_set_video(session[1], rtsp_video1_enc, NULL, 0);
	rtsp_set_audio(session[1], RTSP_CODEC_ID_AUDIO_AAC, NULL, 0);
	return 0;
}

int rtsp_put_video_fream(int channl,const uint8_t* frame, int len, uint64_t pts)
{

	int ret;

	pthread_mutex_lock(&event_mMutex);
	//printf("frame = %x,len = %d,pts = %lld,----%d ----%s\n",frame,len,pts,__LINE__,__FUNCTION__);
	rtsp_tx_video(session[channl], frame, len, pts);
	
	pthread_mutex_unlock(&event_mMutex);
	return 0;
}


int rtsp_put_audio_fream(const uint8_t* frame, int len, uint64_t pts)
{
	int ret,channl;
	//printf("frame = %x,len = %d,pts = %lld,----%d ----%s\n",frame,len,pts,__LINE__,__FUNCTION__);
	for(channl =0; channl<2; channl++){
	rtsp_tx_audio(session[channl], frame, len, pts);
	}
//    ret = rtsp_do_event(demo);
	return 0;
}

int rtsp_server_free(int session_count,rtsp_demo_handle demo)
{

	int ch;
	for (ch = 0; ch < session_count; ch++) {
		if (session[ch])
			rtsp_del_session(session[ch]);
	}

	rtsp_del_demo(demo);
	return 0;

}