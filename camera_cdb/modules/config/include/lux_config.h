/**************************************************
*	      lux 配置模块
*         humy  2023.1.30  create
**************************************************/

#ifndef   __LUX_CONFIG_H__
#define   __LUX_CONFIG_H__
#define GRID_CELLS_X 20
#define GRID_CELLS_Y 20

#include <lux_iniparse.h>

#define RECT_STR_LEN_MAX	512					//多个矩形围栏坐标信息字符串最大长度
#define LINE_STR_LEN_MAX	(RECT_STR_LEN_MAX)	//多条报警线坐标信息字符串最大长度
#define POLYGON_STR_LEN_MAX	(STR_LEN_MAX)		//多个多边形围栏坐标信息字符串最大长度
typedef enum
{
	PCMU,
}LUX_ACODEC_EN;

//创建所需参数的结构体
typedef struct
{
	int irled; //=gpio59			#红外
	int reset; //=gpio50			#复位
	int speaker; //gpio7			#语音通道
	int color_green; //    =gpio9
	int color_red; //      =gpio10
	int ircutn;
	int ircutp;
}LUX_CONFIG_GPIO_CFG_ST;//GPIO参数

typedef struct
{	
	int led_enabled;			//led灯开关和颜色
	// int timezone;    	     //时区                       
	// int power_freq;        //电机频率设置
	int platform_type;			//[0:AC,1-PTZ]
	// int online;					//临时在线参数
	int jio_enabled;    
	const char* timezone; 
	int online_mode;
	const char* power_freq;     
	int timezone_offset_s;
	bool online;
}LUX_CONFIG_SYSTEM_CFG_ST;//系统参数

typedef enum
{
    AUTO,
    DAY,
	NIGHT,
}LUX_DayNight_Mode;

typedef struct
{
	int vflip;			//水平翻转
	int hflip;			//垂直翻转
	int daynight_lowthreshold;			//日夜切换夜间曝光值
	int daynight_highthreshold;		//日夜切换白天曝光值
	//int cntmax;						
	int osd_show_time;						//时间水印开关
	//LUX_DayNight_Mode daynight;							//daynight mode, 0:auto, 1: day, 2:night
	const char* daynight;				//daynight mode, auto:auto, off: day, on:night //0：off 1：on 2:auto
    const char* resolution_0;
    const char* resolution_1;
    // current bitrate
    int bitrate_0;
    int bitrate_1;
    // current fps
    int fps_0;
    int fps_1;
	int wdr_onoff; //宽动态 0,关闭，1，打开
	int watermark_onoff;			//图片水印开关
//	bool wdr;		//宽动态 0,关闭，1，打开
}LUX_CONFIG_IMAGE_CFG_ST;//图像参数

typedef struct
{
	//char codec_0[32];			//H.264
	//char codec_1[32];			//H.264	
	int chn0_encAttr_eProfile;  //AVC_BASELINE_H264=66, AVC_MAIN_H264=77, AVC_HIGH_H264=100 ,HEVC_MAIN_H265=16777217    	
	int chn0_encAttr_uLevel;
	int chn0_encAttr_uTier;
	int chn0_encAttr_ePicFormat;
	int chn0_encAttr_eEncOptions;
	int chn0_encAttr_eEncTools;
	int chn0_rcAttr_rcMode;
	int chn0_rcAttr_uTargetBitRate;
	int chn0_rcAttr_uMaxBitRate;
	int chn0_rcAttr_iInitialQP;
	int chn0_rcAttr_iMinQP;
	int chn0_rcAttr_iMaxQP;
	int chn0_rcAttr_iIPDelta;
	int chn0_rcAttr_iPBDelta;
	int chn0_rcAttr_eRcOptions;
	int chn0_rcAttr_uMaxPictureSize;
	int chn0_rcAttr_uMaxPSNR;
	int chn0_rcAttr_frmRateDen;
	int chn0_gopAttr_uGopCtrlMode;
	int chn0_gopAttr_uGopLength;
	int chn0_gopAttr_uNumB;
	int chn0_gopAttr_uMaxSameSenceCnt;
	int chn0_gopAttr_bEnableLT;
	int chn0_gopAttr_uFreqLT;
	int chn0_gopAttr_bLTRC;
	//sub chan resolution 640 * 360
	//AVC_BASELINE_H264=66, AVC_MAIN_H264=77, AVC_HIGH_H264=100 ,HEVC_MAIN_H265=16777217
	int chn1_encAttr_eProfile;
	int chn1_encAttr_uLevel;
	int chn1_encAttr_uTier;
	int chn1_encAttr_ePicFormat;
	int chn1_encAttr_eEncOptions;
	int chn1_encAttr_eEncTools;
	int chn1_rcAttr_rcMode;
	int chn1_rcAttr_uTargetBitRate;
	int chn1_rcAttr_uMaxBitRate;
	int chn1_rcAttr_iInitialQP;
	int chn1_rcAttr_iMinQP;
	int chn1_rcAttr_iMaxQP;
	int chn1_rcAttr_iIPDelta;
	int chn1_rcAttr_iPBDelta;
	int chn1_rcAttr_eRcOptions;
	int chn1_rcAttr_uMaxPictureSize;
	int chn1_rcAttr_uMaxPSNR;
	int chn1_rcAttr_frmRateDen;
	int chn1_gopAttr_uGopCtrlMode;
	int chn1_gopAttr_uGopLength;
	int chn1_gopAttr_uNumB;
	int chn1_gopAttr_uMaxSameSenceCnt;
	int chn1_gopAttr_bEnableLT;
	int chn1_gopAttr_uFreqLT;
	int chn1_gopAttr_bLTRC;
	//third chan resolution 640 * 360 jpeg, Used in the screenshots.
	int chn2_encAttr_eProfile;
	int chn2_encAttr_uLevel;
	int chn2_encAttr_uTier;
	int chn2_encAttr_ePicFormat;
	int chn2_encAttr_eEncOptions;
	int chn2_encAttr_eEncTools;
	int chn2_rcAttr_rcMode;
	int chn2_rcAttr_iInitialQP;
	int chn2_rcAttr_frmRateDen;
	int chn2_gopAttr_uGopCtrlMode;
	int chn2_gopAttr_uGopLength;
	int chn2_gopAttr_uNumB;
	int chn2_gopAttr_uMaxSameSenceCnt;
	int chn2_gopAttr_bEnableLT;
	int chn2_gopAttr_uFreqLT;
	int chn2_gopAttr_bLTRC;
	const char* codec_0;
    const char* codec_1;

}LUX_CONFIG_VIDEO_CFG_ST;//视频参数

typedef struct
{
	int microphone_enabled;					//麦克分开关
    const char* acodec_0;
    const char* acodec_1;
	int audio_aec_en;             		//TRUE or FALSE, detail at /etc/webrtc_profile.in
	int audio_in_type;                		//[0:PCM,1:G711A,2:G711U,3:G726,4:AAC], only pcm, G711
	int audio_in_sample_rate;     	//ai sample rate 8000 or 16000 (Hz)
	int ai_volume;                    		//[-30 ~ 120], -30 means mute(0), 120 means 30d
	int ai_gain;                    		//[0 ~ 31], ai_gain < 0 ? 0, ai_gain > 31 ? 10
	int ai_ns_en;                  		//TRUE or FALSE   mutex with AEC
	int ai_ns_level;                  		//[0 ~ 3] -> [LOW ~ HIGH]
	int ai_hpf_en;                		//TRUE or FALSE   mutex with AEC
	int ai_agc_en;                 		//TRUE or FALSE   mutex with AEC
	int ai_agc_level;                		//[0, 31] means -xx:dB
	int ai_agc_gain;                 		//[0, 90] -> [LOW ~ HIGH]
	int audio_out_type;             		//[0:PCM,1:G711A,2:G711U,3:G726:AAC], support G711U
	int audio_out_sample_rate;     	//ao sample rate support 16kHz/8kHz, need modifi
	int ao_volume;                  		//[-30 ~ 120], -30 means mute(0), 120 means 30dB
	int ao_gain;                    		//[0 ~ 31]->[-39dB ~ 6dB], step 1.5dB.
	int ao_hpf_en;                 		//TRUE or FALSE    mutex with AEC
	int ao_agc_en;                		//TRUE or FALSE    mutex with AEC
	int ao_agc_level;                 		//[0, 31] means -xx:dB
	int ao_agc_gain;                  		//[0, 90] -> [LOW ~ HIGH]

}LUX_CONFIG_AUDIO_CFG_ST;//音频参数

typedef struct 
{
	LUX_CONFIG_VIDEO_CFG_ST video_config;
	LUX_CONFIG_AUDIO_CFG_ST audio_config;
}LUX_CONFIG_MEDIA_CFG_ST;//音视频参数

typedef struct
{
	int record_onoff;			//0:off 1:on
	int record_mode;			//0:event 1:all
	int Reserved_space_KB;		    //预留空间

}LUX_CONFIG_RECORD_CFG_ST;//录像参数

typedef struct
{
	int webport;//=8080
	int rtsp_port;//=554
	int onvif_port;//=8088
	int ssh_port;//=22

}LUX_CONFIG_NETWORK_CFG_ST;//网络参数

static const int64_t motion_detector_grid_range[2] = {
    GRID_CELLS_X, GRID_CELLS_Y
};

typedef struct
{
	int pir_detect_enabled ;			//矩形围栏开关 0:off 1:on
	int pir_detect_m_enabled ;			//多边形围栏开关 0:off 1:on
	int pir_detect_sensitivity;      //0 1 2 3
	char perm_area[RECT_STR_LEN_MAX];				//多个矩形围栏，最多支持6个矩形
	char perm_area2[POLYGON_STR_LEN_MAX];				//多个多边形围栏，最多支持6个16边形
	int face_detect_enabled;		    //#人脸检测开关
	int face_detect_sensitivity;        //0 1 2 3
	int human_detect_enabled;		    //人形检测
	int human_detect_sensitivity;       //0 1 2 3
	char alarm_zone_size[128];
	int motion_detect_enabled;		//移动侦测开关
	int motion_detect_0_sensitivity;	//#移动侦测灵敏度
	int motion_detect_1_threshold; 	//#检测移动区域范围阈值
	int sound_detect_enabled;		//#声音检测开关
	int sound_detect_sensitivity;	//#声音检测灵敏度
	int smile_shot_enabled;			//#笑脸抓拍开关
//	int cover_face_enabled;			//#遮脸开关
	int cry_detect_enabled;			//#哭声检测开关
	int alarm_zone_onoff;			//报警开关
	int alarm_interval;        		//#0:1min, 1, 3min; 2, 5min
	int choose_alarm_sound;		//#0:off,1:welcom,2:danger,3:priv
	int sleepMode;
	char motion_detect_0_zone[GRID_CELLS_X * GRID_CELLS_Y + 1];
	int tamper_det_enabled;		//篡改侦测开关
	int tamper_cover_th;		//篡改侦测遮挡阈值
	int tamper_det_sensitivity;		//篡改侦测灵敏度，针对遮挡喷涂
	int crossline_det_enabled;	//越线侦测开关
	char line_area[LINE_STR_LEN_MAX];		//越线侦测报警线区域，最多支持6条
	int loiter_det_enabled;		//徘徊侦测开关
	int object_det_enabled;		//物品遗留丢失侦测开关
	int object_det_sensitivity; //物品遗留丢失侦测灵敏度
	int object_det_gray_scale;
	int object_det_max_area;
	int object_det_min_area;
	char leftdet_area[RECT_STR_LEN_MAX];
	char lossdet_obj[RECT_STR_LEN_MAX];
//	const char* night_vision_mode;
}LUX_CONFIG_ALG_CFG_ST;//算法参数
// configuration model

// static const int64_t motion_detector_grid_range[2] = {
//     GRID_CELLS_X, GRID_CELLS_Y
// };

// typedef struct 
// {
//     // "general/online_mode"
//     int online_mode;
//     // "general/timezone"
//     const char* timezone;
//     int timezone_offset_s;
//     // "microphone/enabled"
//     int microphone_enabled;
//     // "led/enabled"
// 	int led_enabled;
//     // "night_vision/mode"
//     const char* night_vision_mode;
//     // "general/power_freq"
//     const char* power_freq;

//     // "general/flip_mode"
//     int flip_mode;

//     // Detectors
//     // "sound_detection/enabled"
//     int sound_detection;
//     // "sound_detection/sensitivity"
//     int64_t sound_detection_sensitivity;
//     // "pir_detection/enabled"
//     int pir_detection;
//     // "motion_detector/enabled"
//     int motion_detector;
//     // "motion_detector/N/sensitivity"
//     int64_t motion_detector_0_sensitivity;
//     // "motion_detector/N/threshold"
//     int64_t motion_detector_0_threshold;

//     char motion_detector_0_zone[GRID_CELLS_X * GRID_CELLS_Y + 1];

//     // current resolution
//     // const char* resolution_0;
//     // const char* resolution_1;
//     // // current bitrate
//     // int64_t bitrate_0;
//     // int64_t bitrate_1;
//     // // current fps
//     // int64_t fps_0;
//     // int64_t fps_1;
//     // osd/show_time
//     //int osd_show_time;
//     // video codecs
//     // const char* codec_0;
//     // const char* codec_1;
//     // audio codecs
//     // const char* acodec_0;
//     // const char* acodec_1;
//     bool online;

//     int face_detection;
//     bool privacy_mask;
// }LUX_CONFIG_JIO_CFG_ST;

typedef struct LUX_CONFIG_DATA
{
	LUX_CONFIG_GPIO_CFG_ST gpio_config;
	LUX_CONFIG_SYSTEM_CFG_ST system_config;
	LUX_CONFIG_IMAGE_CFG_ST  image_config;
	LUX_CONFIG_MEDIA_CFG_ST  media_config;
	LUX_CONFIG_RECORD_CFG_ST record_config;
	LUX_CONFIG_ALG_CFG_ST alg_config;
	LUX_CONFIG_NETWORK_CFG_ST network_config;
	//LUX_CONFIG_JIO_CFG_ST g_JioConfig;
}LUX_CONFIG_DATA_ST;




int LUX_Config_Init();


int LUX_Config_GetGPIOCfg(LUX_CONFIG_GPIO_CFG_ST *gpio_cfg);

int LUX_Config_SetGPIOCfg(const LUX_CONFIG_GPIO_CFG_ST *gpio_cfg);

int LUX_Config_GetSysCfg(LUX_CONFIG_SYSTEM_CFG_ST *sys_cfg);

int LUX_Config_SetSysCfg(const LUX_CONFIG_SYSTEM_CFG_ST *sys_cfg);

int LUX_Config_GetImageCfg(LUX_CONFIG_IMAGE_CFG_ST *image_cfg);

int LUX_Config_SetImageCfg(const LUX_CONFIG_IMAGE_CFG_ST *image_cfg);

int LUX_Config_GetMediaCfg(LUX_CONFIG_MEDIA_CFG_ST *MediaConfig);

int LUX_Config_SetMediaCfg(const LUX_CONFIG_MEDIA_CFG_ST *MediaConfig);

int LUX_Config_GetRecordCfg(LUX_CONFIG_RECORD_CFG_ST *record_cfg);

int LUX_Config_SetRecordCfg(const LUX_CONFIG_RECORD_CFG_ST *record_cfg);

int LUX_Config_GetAlgCfg(LUX_CONFIG_ALG_CFG_ST *alg_cfg);

int LUX_Config_SetAlgCfg(const LUX_CONFIG_ALG_CFG_ST *alg_cfg);

int LUX_Config_GetNetworkCfg(LUX_CONFIG_NETWORK_CFG_ST *network_cfg);

int LUX_Config_SetNetworkCfg(const LUX_CONFIG_NETWORK_CFG_ST *network_cfg);

//int LUX_Config_GetJiocfg(LUX_CONFIG_JIO_CFG_ST *g_JioConfig);

//int LUX_Config_SetJiocfg(const LUX_CONFIG_JIO_CFG_ST *g_JioConfig);

int LUX_Config_Final();

int LUX_INIParse_GetIntConfig(const char* pTitle, const char* pKey, int* value);

int LUX_INIParse_GetStrConfig(const char* pTitle, const char* pKey, char* pOutStr);

int LUX_Config_Set_AllConfig(const LUX_CONFIG_DATA_ST* g_config);

int jio_system_init(LUX_CONFIG_IMAGE_CFG_ST* image_cfg);

#endif  //__LUX_CONFIG_H__