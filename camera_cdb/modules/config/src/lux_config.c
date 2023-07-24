/**************************************************
*	      lux_config.c luxshare配置模块
*         humy  2023.1.30 create
**************************************************/

#include "lux_config.h"
#include "lux_iniparse.h"
#include "lux_global.h"
#include <jwhead.h>
#include <idip_cloud_config.h>
#define TAG "lux_config.c"

pthread_mutex_t config_mMutex;
const char* find_option(const char* tempStr, const char* const* array, int size);
static LUX_INIPARSE_ATTR_ST* pAttr = NULL;
static LUX_CONFIG_DATA_ST g_configData_st =
{
		.system_config =
		{
			.led_enabled = 1,
			.platform_type =0,
			.jio_enabled = 1,
			.timezone = "+08:00",
			.power_freq = "50 HZ",
			.online_mode = 1,
			.timezone_offset_s = 28800,
			.online = 0,
		},
		.gpio_config =
		{
			.color_green = 9,
			.color_red = 10,
			.irled = 116,
			.reset = 50,
			.speaker = 95,
			.ircutn = 122,
			.ircutp = 123,
		},
		.image_config =
		{
			.vflip = 0,
			.hflip = 0,
			.daynight = "auto",
			.daynight_lowthreshold = 1800,
			.daynight_highthreshold = 2150,
			.wdr_onoff = 0,
			.watermark_onoff = 1,
			//.wdr = 0,
			.resolution_0 = "1920x1080",
			.resolution_1 = "720x576",
				// current bitrate
			.bitrate_0 = 2048,
			.bitrate_1 = 256,
				// current fps
			.fps_0 = 25,
			.fps_1 = 25,
			.osd_show_time = 1,
		},
		.record_config =
		{
			.record_onoff = 1,
			.record_mode = 1,
			.Reserved_space_KB = 40
		},
		.alg_config =
		{
			.pir_detect_enabled = 0,
			.pir_detect_m_enabled = 0,
			.pir_detect_sensitivity= 2,
			.perm_area = {"{\"num\":1,\"region0\":{\"x\":0,\"xlen\":100,\"y\":0,\"ylen\":100}}"},
			.perm_area2 = {"{\"num\":1, \"region0\":{\"pnum\":6,\"x0\":5, \"y0\":0,\"x1\":10,\"y1\":0,\"x2\":15,\"y2\":5,\"x3\":10,\"y3\":10, \"x4\":5, \"y4\":10,\"x5\":0, \"y5\":5}}"},
			.face_detect_enabled = 0,
			.face_detect_sensitivity = 2,
			.human_detect_enabled = 1,
			.human_detect_sensitivity = 2,
			.alarm_zone_size = {"{\"num\":1,\"region0\":{\"x\":0,\"xlen\":100,\"y\":0,\"ylen\":100}}"},
			.motion_detect_enabled = 0,
			.motion_detect_0_sensitivity = 80,
			.motion_detect_1_threshold = 80,
			.motion_detect_0_zone = {0},
			.sound_detect_enabled = 1,
			.sound_detect_sensitivity = 40,
			.smile_shot_enabled = 0,
			.cry_detect_enabled = 0,
			.alarm_zone_onoff = 0,
			.alarm_interval = 0,
			.choose_alarm_sound = 0,
			.sleepMode = 0,
			.tamper_det_enabled=0,
			.tamper_cover_th=60,
			.tamper_det_sensitivity=2,
			.crossline_det_enabled=0,
			.line_area={"{\"num\":1,\"line0\":{\"x0\":50,\"y0\":0,\"x1\":50,\"y1\":100}}"},
			.loiter_det_enabled=0,
			.object_det_enabled=0,
			.object_det_sensitivity=2,
			.object_det_gray_scale=50,
			.object_det_max_area=20000,
			.object_det_min_area=80,
			.leftdet_area={"{\"num\":1,\"region0\":{\"x\":0,\"xlen\":100,\"y\":0,\"ylen\":100}}"},
			.lossdet_obj={"{\"num\":1,\"region0\":{\"x\":0,\"xlen\":1,\"y\":0,\"ylen\":1}}"},
		},
		.network_config =
		{
			.webport = 8080,
			.rtsp_port = 554,
			.onvif_port = 80,
			.ssh_port = 22,
		},

		.media_config =
		{
			.audio_config =
			{
				.audio_in_type = 0,
				.audio_aec_en = 0,
				.audio_in_sample_rate = 16000,
				.ai_volume = 67,
				.ai_gain = 31,
				.ai_hpf_en = 1,
				.ai_ns_level = 0,
				.ai_ns_en = 1,
				.ai_agc_en = 1,
				.ai_agc_level = 3,
				.ai_agc_gain = 5,
				.audio_out_type = 0,
				.audio_out_sample_rate = 16000,
				.ao_volume = 48,
				.ao_gain = 28,
				.ao_hpf_en = 1,
				.ao_agc_en = 0,
				.ao_agc_level = 0,
				.ao_agc_gain = 6,
				.acodec_0 = "AAC",
				.acodec_1 = "AAC",
			},
				.video_config =
			{
				.chn0_encAttr_eProfile = 100,
				.chn0_encAttr_uLevel = 51,
				.chn0_encAttr_uTier = 0,
				.chn0_encAttr_ePicFormat = 392,
				.chn0_encAttr_eEncOptions = 294952,
				.chn0_encAttr_eEncTools = 156,
				.chn0_rcAttr_rcMode = 8,
				.chn0_rcAttr_uTargetBitRate = 2048,
				.chn0_rcAttr_uMaxBitRate = 2730,
				.chn0_rcAttr_iInitialQP = -1,
				.chn0_rcAttr_iMinQP = 22,
				.chn0_rcAttr_iMaxQP = 48,
				.chn0_rcAttr_iIPDelta = -1,
				.chn0_rcAttr_iPBDelta = -1,
				.chn0_rcAttr_eRcOptions = 1,
				.chn0_rcAttr_uMaxPictureSize = 1792,
				.chn0_rcAttr_uMaxPSNR = 42,
				.chn0_rcAttr_frmRateDen = 1,
				.chn0_gopAttr_uGopCtrlMode = 2,
				.chn0_gopAttr_uGopLength = 25,
				.chn0_gopAttr_uNumB = 0,
				.chn0_gopAttr_uMaxSameSenceCnt = 2,
				.chn0_gopAttr_bEnableLT = 0,
				.chn0_gopAttr_uFreqLT = 0,
				.chn0_gopAttr_bLTRC = 0,

				.chn1_encAttr_eProfile = 100,
				.chn1_encAttr_uLevel = 51,
				.chn1_encAttr_uTier = 0,
				.chn1_encAttr_ePicFormat = 392,
				.chn1_encAttr_eEncOptions = 294952,
				.chn1_encAttr_eEncTools = 156,
				.chn1_rcAttr_rcMode = 8,
				.chn1_rcAttr_uTargetBitRate = 512,
				.chn1_rcAttr_uMaxBitRate = 640,
				.chn1_rcAttr_iInitialQP = -1,
				.chn1_rcAttr_iMinQP = 25,
				.chn1_rcAttr_iMaxQP = 38,
				.chn1_rcAttr_iIPDelta = -1,
				.chn1_rcAttr_iPBDelta = -1,
				.chn1_rcAttr_eRcOptions = 17,
				.chn1_rcAttr_uMaxPictureSize = 832,
				.chn1_rcAttr_uMaxPSNR = 42,
				.chn1_rcAttr_frmRateDen = 1,
				.chn1_gopAttr_uGopCtrlMode = 2,
				.chn1_gopAttr_uGopLength = 25,
				.chn1_gopAttr_uNumB = 0,
				.chn1_gopAttr_uMaxSameSenceCnt = 2,
				.chn1_gopAttr_bEnableLT = 0,
				.chn1_gopAttr_uFreqLT = 0,
				.chn1_gopAttr_bLTRC = 0,

				.chn2_encAttr_eProfile = 16777217,
				.chn2_encAttr_uLevel = 0,
				.chn2_encAttr_uTier = 0,
				.chn2_encAttr_ePicFormat = 392,
				.chn2_encAttr_eEncOptions = 294952,
				.chn2_encAttr_eEncTools = 156,
				.chn2_rcAttr_rcMode = 0,
				.chn2_rcAttr_iInitialQP = 38,
				.chn2_rcAttr_frmRateDen = 1,
				.chn2_gopAttr_uGopCtrlMode = 2,
				.chn2_gopAttr_uGopLength = 5,
				.chn2_gopAttr_uNumB = 0,
				.chn2_gopAttr_uMaxSameSenceCnt = 2,
				.chn2_gopAttr_bEnableLT = 0,
				.chn2_gopAttr_uFreqLT = 0,
				.chn2_gopAttr_bLTRC = 0,
				.codec_0 = "H.264",
				.codec_1 = "H.264",
			}
		},
};
static const char* const tz_offsets[] = {
    "00:00",
    "-12:00", "-11:00", "-10:00", "-09:30", "-09:00",
    "-08:00", "-07:00", "-06:00", "-05:00", "-04:00",
    "-03:00", "-02:00", "-01:00", "+01:00", "+02:00",
    "+03:00", "+03:30", "+04:00", "+04:30", "+05:00",
    "+05:30", "+05:45", "+06:00", "+06:30", "+07:00",
    "+08:00", "+08:45", "+09:00", "+09:30", "+10:00",
    "+10:30", "+11:00", "+12:00", "+12:45", "+13:00",
    "+14:00"
};

static const char* const nightvision_modes[] = {
    "on", "off", "auto"
};

// supported resolutions
static const char* const resolutions_0[] = { /*"2560x1440",*/"1920x1080","1280x720" };
static const char* const resolutions_1[] = {"800x600","720x576", "640x480", "320x240"};
// supported bitrates
static const int64_t bitrates_0[] = { 2048,1536,1024, 512,256,128, 64 };
static const int64_t bitrates_1[] = { 512,256,128, 64 };
// supported fps values
static const int64_t fps_list_0[] = { 30, 25, 20, 15, 10, 5, 1 };
static const int64_t fps_list_1[] = { 30, 25, 20, 15, 10, 5, 1 };
// supported video codecs
static const char* const codecs[] = { "H.264", "H.265"};
// supported audio codecs  支持音频编解码器
static const char* const acodecs[] = { "AAC" };
int LUX_Config_Init()
{
	int ret;
	//解析配置文件
    ret = LUX_INIParse_Load(LUX_CONFIG_FILE);
    if (LUX_OK != ret) {
        MYERROR("LUX_INIParse_Load, return error[0x%x]\n", ret);
        return LUX_ERR;
    }
	g_configData =&g_configData_st;

	pthread_mutex_init(&config_mMutex, NULL);

	//gpio参数初始化
	LUX_Config_GetGPIOCfg(&g_configData->gpio_config);

	//音视频参数初始化
	LUX_Config_GetMediaCfg(&g_configData->media_config);

	//系统参数初始化
	LUX_Config_GetSysCfg(&g_configData->system_config);

	//算法参数初始化
	LUX_Config_GetAlgCfg(&g_configData->alg_config);

	//录像参数初始化
	LUX_Config_GetRecordCfg(&g_configData->record_config);

	//图像参数初始化
	LUX_Config_GetImageCfg(&g_configData->image_config);

	//网络配置参数初始化
	LUX_Config_GetNetworkCfg(&g_configData->network_config);

	//jio云配置初始化
	//LUX_Config_GetJiocfg(&g_configData->g_JioConfig);

	//默认参数写入
	// LUX_Config_SetSysCfg(&g_configData_st.system_config);
	// LUX_Config_SetGPIOCfg(&g_configData_st.gpio_config);
	// LUX_Config_SetImageCfg(&g_configData_st.image_config);
	// LUX_Config_SetMediaCfg(&g_configData_st.media_config);
	// LUX_Config_SetRecordCfg(&g_configData_st.record_config);
	// LUX_Config_SetAlgCfg(&g_configData_st.alg_config);
	//  //LUX_Config_SetNetworkCfg(&g_configData_st.network_config);
	// LUX_Config_SetJiocfg(&g_configData_st.g_JioConfig);

	return ret;
}
/// <summary>
/// 系统参数
/// </summary>
/// <param name="sys_cfg"></param>
/// <returns></returns>
int LUX_Config_GetSysCfg(LUX_CONFIG_SYSTEM_CFG_ST* sys_cfg)
{
	int ret = 0;
	char tempStr[32] = { 0 };
	int temp;
	//char tempbuf[32] = {0};
	LUX_INIParse_GetIntConfig("system_config", "led_enabled", &sys_cfg->led_enabled);
	LUX_INIParse_GetIntConfig("system_config", "platform_type", &sys_cfg->platform_type);
	LUX_INIParse_GetIntConfig("system_config", "jio_enabled", &sys_cfg->jio_enabled);
	LUX_INIParse_GetStrConfig("system_config", "timezone", tempStr);
	sys_cfg->timezone = find_option(tempStr, tz_offsets, (sizeof(tz_offsets) / sizeof(tz_offsets[0])));
	if (!sys_cfg->timezone) {
		sys_cfg->timezone = tz_offsets[0];
	}
	LUX_INIParse_GetIntConfig("system_config", "online_mode", &sys_cfg->online_mode);
	LUX_INIParse_GetIntConfig("system_config", "power_freq", &temp);

	sprintf(tempStr,"%d HZ",temp);

	sys_cfg->power_freq = find_option(tempStr, idip_cfg_range_power_freq, (sizeof(idip_cfg_range_power_freq) / sizeof(idip_cfg_range_power_freq[0])));
	if (!sys_cfg->power_freq) {
		sys_cfg->power_freq = idip_cfg_range_power_freq[0];
	}
	LUX_INIParse_GetIntConfig("system_config", "timezone_offset_s", &sys_cfg->timezone_offset_s);
	//LUX_INIParse_GetIntConfig("system_config", "power_freq", &sys_cfg->power_freq);
	// LUX_INIParse_GetIntConfig("system_config", "timezone", &sys_cfg->timezone);
	// LUX_INIParse_GetIntConfig("system_config", "online", &sys_cfg->online);
	//memcpy(sys_cfg, &g_configData->system_config, sizeof(LUX_CONFIG_SYSTEM_CFG_ST));
		if (sys_cfg->led_enabled)
	{
		LUX_PWM_Start();
	}
	else
	{
		LUX_PWM_Start();
	}
	return ret;
}

int LUX_Config_SetSysCfg(const LUX_CONFIG_SYSTEM_CFG_ST* sys_cfg)
{
	pthread_mutex_lock(&config_mMutex);
	int ret = 0;
	char tmpBuf[16] ={0};

	pAttr = LUX_INIParse_SetKey_begin(LUX_CONFIG_FILE);
	memcpy(&g_configData->system_config, sys_cfg, sizeof(LUX_CONFIG_SYSTEM_CFG_ST));

	sprintf(tmpBuf, "%d", sys_cfg->led_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr,"system_config", "led_enabled", tmpBuf);

	// sprintf(tmpBuf, "%d", sys_cfg->timezone);
	LUX_INIParse_SetKey_Input(pAttr, "system_config", "timezone", sys_cfg->timezone);
	
	sprintf(tmpBuf, "%d", atoi(sys_cfg->power_freq));
	LUX_INIParse_SetKey_Input(pAttr, "system_config", "power_freq", tmpBuf);

	sprintf(tmpBuf, "%d", sys_cfg->platform_type);
	ret = LUX_INIParse_SetKey_Input(pAttr, "system_config", "platform_type", tmpBuf);

	// sprintf(tmpBuf, "%d", sys_cfg->online);
	LUX_INIParse_SetKey_Input(pAttr, "system_config", "online_mode", sys_cfg->online_mode == 0 ? "0" : "1");
	
	sprintf(tmpBuf, "%d", sys_cfg->jio_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "system_config", "jio_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", sys_cfg->timezone_offset_s);
	ret = LUX_INIParse_SetKey_Input(pAttr, "system_config", "timezone_offset_s", tmpBuf);
	LUX_INIParse_SetKey_End(LUX_CONFIG_FILE,pAttr);
	pthread_mutex_unlock(&config_mMutex);

	return ret;
}

/// <summary>
/// gpio参数
/// </summary>
/// <param name="gpio_cfg"></param>
/// <returns></returns>
int LUX_Config_GetGPIOCfg(LUX_CONFIG_GPIO_CFG_ST* gpio_config)
{
	int ret = 0;
	LUX_INIParse_GetIntConfig("gpio_config", "color_green", &gpio_config->color_green);
	LUX_INIParse_GetIntConfig("gpio_config", "color_red", &gpio_config->color_red);
	LUX_INIParse_GetIntConfig("gpio_config", "irled", &gpio_config->irled);
	LUX_INIParse_GetIntConfig("gpio_config", "reset", &gpio_config->reset);
	LUX_INIParse_GetIntConfig("gpio_config", "speaker",&gpio_config->speaker);
	LUX_INIParse_GetIntConfig("gpio_config", "ircutn", &gpio_config->ircutn);
	LUX_INIParse_GetIntConfig("gpio_config", "ircutp", &gpio_config->ircutp);	
	return ret;
}

int LUX_Config_SetGPIOCfg(const LUX_CONFIG_GPIO_CFG_ST* gpio_cfg)
{
	pthread_mutex_lock(&config_mMutex);
	int ret = 0;
	char tmpBuf[16] ={0};
	pAttr = LUX_INIParse_SetKey_begin(LUX_CONFIG_FILE);
	memcpy(&g_configData->gpio_config, gpio_cfg, sizeof(LUX_CONFIG_GPIO_CFG_ST));

	sprintf(tmpBuf, "%d", gpio_cfg->irled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "gpio_config", "irled", tmpBuf);

	sprintf(tmpBuf, "%d", gpio_cfg->speaker);
	ret = LUX_INIParse_SetKey_Input(pAttr, "gpio_config", "speaker", tmpBuf);

	sprintf(tmpBuf, "%d", gpio_cfg->reset);
	ret = LUX_INIParse_SetKey_Input(pAttr, "gpio_config", "reset", tmpBuf);

	sprintf(tmpBuf, "%d", gpio_cfg->color_green);
	ret = LUX_INIParse_SetKey_Input(pAttr, "gpio_config", "color_green", tmpBuf);

	sprintf(tmpBuf, "%d", gpio_cfg->color_red);
	ret = LUX_INIParse_SetKey_Input(pAttr, "gpio_config", "color_red", tmpBuf);

	sprintf(tmpBuf, "%d", gpio_cfg->ircutn);
	ret = LUX_INIParse_SetKey_Input(pAttr, "gpio_config", "ircutn", tmpBuf);

	sprintf(tmpBuf, "%d", gpio_cfg->ircutp);
	ret = LUX_INIParse_SetKey_Input(pAttr, "gpio_config", "ircutp", tmpBuf);
	LUX_INIParse_SetKey_End(LUX_CONFIG_FILE, pAttr);
	pthread_mutex_unlock(&config_mMutex);

	return ret;
}

/// <summary>
/// 图像参数
/// </summary>
/// <param name="image_cfg"></param>
/// <returns></returns>
int LUX_Config_GetImageCfg(LUX_CONFIG_IMAGE_CFG_ST* image_cfg)
{
	int ret = 0;
	char tempStr[16] = { 0 };
	int temp;
	//获取SENSOR的初始状态
	//LUX_INIParse_GetIntConfig("image_config", "init_sensor_h_flip", &g_sensor_h_flip);
	//LUX_INIParse_GetIntConfig("image_config", "init_sensor_v_flip", &g_sensor_v_flip);
	LUX_INIParse_GetIntConfig("image_config", "hflip", &image_cfg->hflip);
	LUX_INIParse_GetIntConfig("image_config", "vflip", &image_cfg->vflip);
	LUX_INIParse_GetIntConfig("image_config", "daynight_lowthreshold", &image_cfg->daynight_lowthreshold);
	LUX_INIParse_GetIntConfig("image_config", "daynight_highthreshold", &image_cfg->daynight_highthreshold);
	LUX_INIParse_GetIntConfig("image_config", "osd_show_time", &image_cfg->osd_show_time);
 	LUX_INIParse_GetIntConfig("image_config", "daynight", &temp);
	if(0 == temp) //.daynight = 0, #0 off 1 on 2 auto
	{
		sprintf(tempStr,"%s","off");
	}
	if (1 == temp)
	{
		sprintf(tempStr,"%s","on");
	}
	else
	{
		sprintf(tempStr,"%s","auto");
	}
	image_cfg->daynight = find_option(tempStr, nightvision_modes, (sizeof(nightvision_modes) / sizeof(nightvision_modes[0])));
	if (!image_cfg->daynight) {
		image_cfg->daynight = nightvision_modes[0];
	}
	LUX_INIParse_GetIntConfig("image_config", "wdr_onoff", &image_cfg->wdr_onoff);
	LUX_INIParse_GetIntConfig("image_config", "watermark_onoff", &image_cfg->watermark_onoff);
	LUX_INIParse_GetStrConfig("image_config", "resolution_0", tempStr);
	image_cfg->resolution_0 = find_option(tempStr, resolutions_0, (sizeof(resolutions_0) / sizeof(resolutions_0[0])));
	if (!image_cfg->resolution_0) {
		image_cfg->resolution_0 = resolutions_0[0];
	}
	LUX_INIParse_GetStrConfig("image_config", "resolution_1", tempStr);
	image_cfg->resolution_1 = find_option(tempStr, resolutions_1, (sizeof(resolutions_1) / sizeof(resolutions_1[0])));
	if (!image_cfg->resolution_1) {
		image_cfg->resolution_1 = resolutions_1[0];
	}
	LUX_INIParse_GetIntConfig("image_config", "bitrate_0", &image_cfg->bitrate_0);
	LUX_INIParse_GetIntConfig("image_config", "bitrate_1", &image_cfg->bitrate_1);
	LUX_INIParse_GetIntConfig("image_config", "fps_0", &image_cfg->fps_0);
	LUX_INIParse_GetIntConfig("image_config", "fps_1", &image_cfg->fps_1);
	//memcpy(image_cfg, &g_configData->image_config, sizeof(LUX_CONFIG_IMAGE_CFG_ST));
	return ret;
}

int LUX_Config_SetImageCfg(const LUX_CONFIG_IMAGE_CFG_ST* image_cfg)
{
	pthread_mutex_lock(&config_mMutex);
	int ret = 0;
	char tempStr[16] ={0};
	pAttr = LUX_INIParse_SetKey_begin(LUX_CONFIG_FILE);
	memcpy(&g_configData->image_config, image_cfg, sizeof(LUX_CONFIG_IMAGE_CFG_ST));

	sprintf(tempStr, "%d", image_cfg->vflip);
	ret = LUX_INIParse_SetKey_Input(pAttr, "image_config", "vflip", tempStr);

	sprintf(tempStr, "%d", image_cfg->hflip);
	ret = LUX_INIParse_SetKey_Input(pAttr, "image_config", "hflip", tempStr);
	
	sprintf(tempStr, "%d", image_cfg->daynight_lowthreshold);
	ret = LUX_INIParse_SetKey_Input(pAttr, "image_config", "daynight_lowthreshold", tempStr);

	sprintf(tempStr, "%d", image_cfg->daynight_highthreshold);
	ret = LUX_INIParse_SetKey_Input(pAttr, "image_config", "daynight_highthreshold", tempStr);

	/*sprintf(tempStr, "%d", image_cfg->cntmax);
	ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "image_config", "cntmax", tempStr);*/

	 sprintf(tempStr, "%d", image_cfg->osd_show_time);
	 ret = LUX_INIParse_SetKey_Input(pAttr, "image_config", "osd_show_time", tempStr);

	 if (strcmp(image_cfg->daynight, "auto") == 0)
	 {
		 sprintf(tempStr, "%d", 2);
	 }
	 else if (strcmp(image_cfg->daynight, "off") == 0)
	 {
		 sprintf(tempStr, "%d", 0);
	 }
	 else if (strcmp(image_cfg->daynight, "on") == 0)
	 {
		 sprintf(tempStr, "%d", 1);
	 }
	 
	 ret = LUX_INIParse_SetKey_Input(pAttr, "image_config", "daynight", tempStr);

	// sprintf(tempStr, "%s", image_cfg->resolution_0);
	// ret = LUX_INIParse_SetKey_Input(pAttr, "image_config", "resolution_0", tempStr);

	// sprintf(tempStr, "%s", image_cfg->resolution_1);
	// ret = LUX_INIParse_SetKey_Input(pAttr, "image_config", "resolution_1", tempStr);

		LUX_INIParse_SetKey_Input(pAttr, "image_config", "resolution_1", image_cfg->resolution_1);
		LUX_INIParse_SetKey_Input(pAttr, "image_config", "resolution_0", image_cfg->resolution_0);
		sprintf(tempStr, "%d", image_cfg->bitrate_0);
		LUX_INIParse_SetKey_Input(pAttr, "image_config", "bitrate_0", tempStr);
		sprintf(tempStr, "%d", image_cfg->bitrate_1);
		LUX_INIParse_SetKey_Input(pAttr, "image_config", "bitrate_1", tempStr);
		sprintf(tempStr, "%d", image_cfg->fps_0);
		LUX_INIParse_SetKey_Input(pAttr, "image_config", "fps_0", tempStr);
		sprintf(tempStr, "%d", image_cfg->fps_1);
 		LUX_INIParse_SetKey_Input(pAttr, "image_config", "fps_1", tempStr);

	sprintf(tempStr, "%d", image_cfg->wdr_onoff);
	ret = LUX_INIParse_SetKey_Input(pAttr, "image_config", "wdr_onoff", tempStr);

	sprintf(tempStr, "%d", image_cfg->watermark_onoff);
	ret = LUX_INIParse_SetKey_Input(pAttr, "image_config", "watermark_onoff", tempStr);

	LUX_INIParse_SetKey_End(LUX_CONFIG_FILE, pAttr);
	pthread_mutex_unlock(&config_mMutex);

	return ret;

}

/// <summary>
/// 音视频参数
/// </summary>
/// <param name="MediaConfig"></param>
/// <returns></returns>
int  LUX_Config_GetMediaCfg(LUX_CONFIG_MEDIA_CFG_ST* MediaConfig)
{
	int ret = 0;
	char tempStr[16] ={0};
	LUX_INIParse_GetIntConfig("media_config_audio", "microphone_enabled", &MediaConfig->audio_config.microphone_enabled);
	// LUX_INIParse_GetStrConfig("media_config_audio", "acodec_0", tempStr);
	// MediaConfig->audio_config.acodec_0 = find_option(tempStr, acodecs, (sizeof(acodecs) / sizeof(acodecs[0])));
	// if (!MediaConfig->audio_config.acodec_0) {
	// 	MediaConfig->audio_config.acodec_0 = acodecs[0];
	// }
	// LUX_INIParse_GetStrConfig("media_config_audio", "acodec_1", tempStr);
	// MediaConfig->audio_config.acodec_1 = find_option(tempStr, acodecs, (sizeof(acodecs) / sizeof(acodecs[0])));
	// if (!MediaConfig->audio_config.acodec_1) {
	// 	MediaConfig->audio_config.acodec_1 = acodecs[0];
	// }
	LUX_INIParse_GetIntConfig("media_config_audio", "audio_in_type", &MediaConfig->audio_config.audio_in_type);
	if(0 == MediaConfig->audio_config.audio_in_type)
	{
		sprintf(tempStr,"PCM");
	}
	else
	{
		sprintf(tempStr,"AAC");
	}
	MediaConfig->audio_config.acodec_0 = find_option(tempStr, acodecs, (sizeof(acodecs) / sizeof(acodecs[0])));
	if (!MediaConfig->audio_config.acodec_0) {
		MediaConfig->audio_config.acodec_0 = acodecs[0];
	}
	MediaConfig->audio_config.acodec_1 = find_option(tempStr, acodecs, (sizeof(acodecs) / sizeof(acodecs[0])));
	if (!MediaConfig->audio_config.acodec_1) {
		MediaConfig->audio_config.acodec_1 = acodecs[0];
	}
	LUX_INIParse_GetIntConfig("media_config_audio", "audio_aec_en", &MediaConfig->audio_config.audio_aec_en);
	LUX_INIParse_GetIntConfig("media_config_audio", "audio_in_sample_rate", &MediaConfig->audio_config.audio_in_sample_rate);
	LUX_INIParse_GetIntConfig("media_config_audio", "ai_volume", &MediaConfig->audio_config.ai_volume);
	LUX_INIParse_GetIntConfig("media_config_audio", "ai_gain", &MediaConfig->audio_config.ai_gain);
	LUX_INIParse_GetIntConfig("media_config_audio", "ai_hpf_en", &MediaConfig->audio_config.ai_hpf_en);
	LUX_INIParse_GetIntConfig("media_config_audio", "ai_ns_level", &MediaConfig->audio_config.ai_ns_level);
	LUX_INIParse_GetIntConfig("media_config_audio", "ai_ns_en", &MediaConfig->audio_config.ai_ns_en);
	LUX_INIParse_GetIntConfig("media_config_audio", "ai_agc_en", &MediaConfig->audio_config.ai_agc_en);
	LUX_INIParse_GetIntConfig("media_config_audio", "ai_agc_level", &MediaConfig->audio_config.ai_agc_level);
	LUX_INIParse_GetIntConfig("media_config_audio", "ai_agc_gain", &MediaConfig->audio_config.ai_agc_gain);
	LUX_INIParse_GetIntConfig("media_config_audio", "audio_out_type", &MediaConfig->audio_config.audio_out_type);
	LUX_INIParse_GetIntConfig("media_config_audio", "audio_out_sample_rate", &MediaConfig->audio_config.audio_out_sample_rate);
	LUX_INIParse_GetIntConfig("media_config_audio", "ao_volume", &MediaConfig->audio_config.ao_volume);
	LUX_INIParse_GetIntConfig("media_config_audio", "ao_gain", &MediaConfig->audio_config.ao_gain);
	LUX_INIParse_GetIntConfig("media_config_audio", "ao_hpf_en", &MediaConfig->audio_config.ao_hpf_en);
	LUX_INIParse_GetIntConfig("media_config_audio", "ao_agc_en", &MediaConfig->audio_config.ao_agc_en);
	LUX_INIParse_GetIntConfig("media_config_audio", "ao_agc_level", &MediaConfig->audio_config.ao_agc_level);
	LUX_INIParse_GetIntConfig("media_config_audio", "ao_agc_gain", &MediaConfig->audio_config.ao_agc_gain);
	if (MediaConfig->audio_config.microphone_enabled)
	{
		//int ret = LUX_AI_Start();
		if (0 != ret)
		{
			MYERROR("ImpStreamOn failed\n");
			return -1;
		}
	}
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_encAttr_eEncOptions", &MediaConfig->video_config.chn0_encAttr_eEncOptions);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_encAttr_eProfile", &MediaConfig->video_config.chn0_encAttr_eProfile);
	if(16777217 == MediaConfig->video_config.chn0_encAttr_eProfile)
	{
		sprintf(tempStr,"H.265");
	}
	else
	{
		sprintf(tempStr,"H.264");
	}
	MediaConfig->video_config.codec_0 = find_option(tempStr, codecs, (sizeof(codecs) / sizeof(codecs[0])));
	if (!MediaConfig->video_config.codec_0) {
		MediaConfig->video_config.codec_0 = codecs[0];
	}

	LUX_INIParse_GetIntConfig("media_config_video", "chn0_encAttr_uLevel", &MediaConfig->video_config.chn0_encAttr_uLevel);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_encAttr_uTier", &MediaConfig->video_config.chn0_encAttr_uTier);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_encAttr_ePicFormat", &MediaConfig->video_config.chn0_encAttr_ePicFormat);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_encAttr_eEncTools", &MediaConfig->video_config.chn0_encAttr_eEncTools);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_rcAttr_rcMode", &MediaConfig->video_config.chn0_rcAttr_rcMode);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_rcAttr_uTargetBitRate", &MediaConfig->video_config.chn0_rcAttr_uTargetBitRate);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_rcAttr_uMaxBitRate", &MediaConfig->video_config.chn0_rcAttr_uMaxBitRate);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_rcAttr_iInitialQP", &MediaConfig->video_config.chn0_rcAttr_iInitialQP);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_rcAttr_iMinQP", &MediaConfig->video_config.chn0_rcAttr_iMinQP);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_rcAttr_iMaxQP", &MediaConfig->video_config.chn0_rcAttr_iMaxQP);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_rcAttr_iIPDelta", &MediaConfig->video_config.chn0_rcAttr_iIPDelta);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_rcAttr_iPBDelta", &MediaConfig->video_config.chn0_rcAttr_iPBDelta);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_rcAttr_eRcOptions", &MediaConfig->video_config.chn0_rcAttr_eRcOptions);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_rcAttr_uMaxPictureSize", &MediaConfig->video_config.chn0_rcAttr_uMaxPictureSize);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_rcAttr_uMaxPSNR", &MediaConfig->video_config.chn0_rcAttr_uMaxPSNR);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_rcAttr_frmRateDen", &MediaConfig->video_config.chn0_rcAttr_frmRateDen);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_gopAttr_uGopCtrlMode", &MediaConfig->video_config.chn0_gopAttr_uGopCtrlMode);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_gopAttr_uGopLength", &MediaConfig->video_config.chn0_gopAttr_uGopLength);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_gopAttr_uNumB", &MediaConfig->video_config.chn0_gopAttr_uNumB);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_gopAttr_uMaxSameSenceCnt", &MediaConfig->video_config.chn0_gopAttr_uMaxSameSenceCnt);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_gopAttr_bEnableLT", &MediaConfig->video_config.chn0_gopAttr_bEnableLT);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_gopAttr_uFreqLT", &MediaConfig->video_config.chn0_gopAttr_uFreqLT);
	LUX_INIParse_GetIntConfig("media_config_video", "chn0_gopAttr_bLTRC", &MediaConfig->video_config.chn0_gopAttr_bLTRC);
	

	LUX_INIParse_GetIntConfig("media_config_video", "chn1_encAttr_eEncOptions", &MediaConfig->video_config.chn1_encAttr_eEncOptions);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_encAttr_eProfile", &MediaConfig->video_config.chn1_encAttr_eProfile);
	if(16777217 == MediaConfig->video_config.chn1_encAttr_eProfile)
	{
		sprintf(tempStr,"H.265");
	}
	else 
	{
		sprintf(tempStr,"H.264");
	}
	MediaConfig->video_config.codec_1 = find_option(tempStr, codecs, (sizeof(codecs) / sizeof(codecs[0])));
	if (!MediaConfig->video_config.codec_1) {
		MediaConfig->video_config.codec_1 = codecs[0];
	}
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_encAttr_uLevel", &MediaConfig->video_config.chn1_encAttr_uLevel);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_encAttr_uTier", &MediaConfig->video_config.chn1_encAttr_uTier);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_encAttr_ePicFormat", &MediaConfig->video_config.chn1_encAttr_ePicFormat);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_encAttr_eEncTools", &MediaConfig->video_config.chn1_encAttr_eEncTools);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_rcAttr_rcMode", &MediaConfig->video_config.chn1_rcAttr_rcMode);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_rcAttr_uTargetBitRate", &MediaConfig->video_config.chn1_rcAttr_uTargetBitRate);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_rcAttr_uMaxBitRate", &MediaConfig->video_config.chn1_rcAttr_uMaxBitRate);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_rcAttr_iInitialQP", &MediaConfig->video_config.chn1_rcAttr_iInitialQP);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_rcAttr_iMinQP", &MediaConfig->video_config.chn1_rcAttr_iMinQP);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_rcAttr_iMaxQP", &MediaConfig->video_config.chn1_rcAttr_iMaxQP);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_rcAttr_iIPDelta", &MediaConfig->video_config.chn1_rcAttr_iIPDelta);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_rcAttr_iPBDelta", &MediaConfig->video_config.chn1_rcAttr_iPBDelta);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_rcAttr_eRcOptions", &MediaConfig->video_config.chn1_rcAttr_eRcOptions);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_rcAttr_uMaxPictureSize", &MediaConfig->video_config.chn1_rcAttr_uMaxPictureSize);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_rcAttr_uMaxPSNR", &MediaConfig->video_config.chn1_rcAttr_uMaxPSNR);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_rcAttr_frmRateDen", &MediaConfig->video_config.chn1_rcAttr_frmRateDen);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_gopAttr_uGopCtrlMode", &MediaConfig->video_config.chn1_gopAttr_uGopCtrlMode);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_gopAttr_uGopLength", &MediaConfig->video_config.chn1_gopAttr_uGopLength);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_gopAttr_uNumB", &MediaConfig->video_config.chn1_gopAttr_uNumB);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_gopAttr_uMaxSameSenceCnt", &MediaConfig->video_config.chn1_gopAttr_uMaxSameSenceCnt);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_gopAttr_bEnableLT", &MediaConfig->video_config.chn1_gopAttr_bEnableLT);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_gopAttr_uFreqLT", &MediaConfig->video_config.chn1_gopAttr_uFreqLT);
	LUX_INIParse_GetIntConfig("media_config_video", "chn1_gopAttr_bLTRC", &MediaConfig->video_config.chn1_gopAttr_bLTRC);


	LUX_INIParse_GetIntConfig("media_config_video", "chn2_encAttr_eProfile", &MediaConfig->video_config.chn2_encAttr_eProfile);
	LUX_INIParse_GetIntConfig("media_config_video", "chn2_encAttr_eEncOptions", &MediaConfig->video_config.chn2_encAttr_eEncOptions);
	LUX_INIParse_GetIntConfig("media_config_video", "chn2_encAttr_uLevel", &MediaConfig->video_config.chn2_encAttr_uLevel);
	LUX_INIParse_GetIntConfig("media_config_video", "chn2_encAttr_uTier", &MediaConfig->video_config.chn2_encAttr_uTier);
	LUX_INIParse_GetIntConfig("media_config_video", "chn2_encAttr_ePicFormat", &MediaConfig->video_config.chn2_encAttr_ePicFormat);
	LUX_INIParse_GetIntConfig("media_config_video", "chn2_encAttr_eEncTools", &MediaConfig->video_config.chn2_encAttr_eEncTools);
	LUX_INIParse_GetIntConfig("media_config_video", "chn2_rcAttr_rcMode", &MediaConfig->video_config.chn2_rcAttr_rcMode);
	LUX_INIParse_GetIntConfig("media_config_video", "chn2_rcAttr_iInitialQP", &MediaConfig->video_config.chn2_rcAttr_iInitialQP);
	LUX_INIParse_GetIntConfig("media_config_video", "chn2_rcAttr_frmRateDen", &MediaConfig->video_config.chn2_rcAttr_frmRateDen);
	LUX_INIParse_GetIntConfig("media_config_video", "chn2_gopAttr_uGopCtrlMode", &MediaConfig->video_config.chn2_gopAttr_uGopCtrlMode);
	LUX_INIParse_GetIntConfig("media_config_video", "chn2_gopAttr_uGopLength", &MediaConfig->video_config.chn2_gopAttr_uGopLength);
	LUX_INIParse_GetIntConfig("media_config_video", "chn2_gopAttr_uNumB", &MediaConfig->video_config.chn2_gopAttr_uNumB);
	LUX_INIParse_GetIntConfig("media_config_video", "chn2_gopAttr_uMaxSameSenceCnt", &MediaConfig->video_config.chn2_gopAttr_uMaxSameSenceCnt);
	LUX_INIParse_GetIntConfig("media_config_video", "chn2_gopAttr_bEnableLT", &MediaConfig->video_config.chn2_gopAttr_bEnableLT);
	LUX_INIParse_GetIntConfig("media_config_video", "chn2_gopAttr_uFreqLT", &MediaConfig->video_config.chn2_gopAttr_uFreqLT);
	LUX_INIParse_GetIntConfig("media_config_video", "chn2_gopAttr_bLTRC", &MediaConfig->video_config.chn2_gopAttr_bLTRC);
	// LUX_INIParse_GetStrConfig("media_config_video", "codec_0", tempStr);
	// MediaConfig->video_config.codec_0 = find_option(tempStr, codecs, (sizeof(codecs) / sizeof(codecs[0])));
	// if (!MediaConfig->video_config.codec_0) {
	// 	MediaConfig->video_config.codec_0 = codecs[0];
	// }
	// LUX_INIParse_GetStrConfig("media_config_video", "codec_1", tempStr);
	// MediaConfig->video_config.codec_1 = find_option(tempStr, codecs, (sizeof(codecs) / sizeof(codecs[0])));
	// if (!MediaConfig->video_config.codec_1) {
	// 	MediaConfig->video_config.codec_1 = codecs[0];
	// }
	return ret;
}

int LUX_Config_SetMediaCfg(const LUX_CONFIG_MEDIA_CFG_ST* MediaConfig)
{
	pthread_mutex_lock(&config_mMutex);

	int ret = 0;
	pAttr = LUX_INIParse_SetKey_begin(LUX_CONFIG_FILE);
	memcpy(&g_configData->media_config, MediaConfig, sizeof(LUX_CONFIG_MEDIA_CFG_ST));

	char tmpBuf[16] ={0};
	sprintf(tmpBuf, "%d", MediaConfig->audio_config.microphone_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "microphone_enabled", tmpBuf);

    LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "acodec_0", MediaConfig->audio_config.acodec_0);
	LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "acodec_1", MediaConfig->audio_config.acodec_1);
	sprintf(tmpBuf, "%d", MediaConfig->audio_config.audio_aec_en);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "audio_aec_en", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->audio_config.audio_in_type);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "audio_in_type", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->audio_config.audio_in_sample_rate);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "audio_in_sample_rate", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->audio_config.ai_volume);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ai_volume", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->audio_config.ai_gain);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ai_gain", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->audio_config.ai_ns_en);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ai_ns_en", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->audio_config.ai_ns_level);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ai_ns_level", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->audio_config.ai_hpf_en);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ai_hpf_en", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->audio_config.ai_agc_en);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ai_agc_en", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->audio_config.ai_agc_level);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ai_agc_level", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->audio_config.ai_agc_gain);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ai_agc_gain", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->audio_config.audio_out_type);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "audio_out_type", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->audio_config.audio_out_sample_rate);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "audio_out_sample_rate", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->audio_config.ao_volume);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ao_volume", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->audio_config.ao_gain);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ao_gain", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->audio_config.ao_hpf_en);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ao_hpf_en", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->audio_config.ao_agc_en);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ao_agc_en", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->audio_config.ao_agc_level);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ao_agc_level", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->audio_config.ao_agc_gain);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ao_agc_gain", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_encAttr_eProfile);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_encAttr_eProfile", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_encAttr_uLevel);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_encAttr_uLevel", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_encAttr_uTier);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_encAttr_uTier", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_encAttr_ePicFormat);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_encAttr_ePicFormat", tmpBuf);

	sprintf(tmpBuf, "%d",  MediaConfig->video_config.chn0_encAttr_eEncOptions);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_encAttr_eEncOptions",tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_encAttr_eEncTools);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_encAttr_eEncTools", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_rcAttr_rcMode);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_rcMode", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_rcAttr_uTargetBitRate);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_uTargetBitRate", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_rcAttr_uMaxBitRate);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_uMaxBitRate", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_rcAttr_iInitialQP);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_iInitialQP", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_rcAttr_iMinQP);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_iMinQP", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_rcAttr_iMaxQP);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_iMaxQP", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_rcAttr_iIPDelta);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_iIPDelta", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_rcAttr_iPBDelta);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_iPBDelta", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_rcAttr_eRcOptions);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_eRcOptions", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_rcAttr_uMaxPictureSize);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_uMaxPictureSize", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_rcAttr_uMaxPSNR);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_uMaxPSNR", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_rcAttr_frmRateDen);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_frmRateDen", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_gopAttr_uGopCtrlMode);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_gopAttr_uGopCtrlMode", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_gopAttr_uGopLength);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_gopAttr_uGopLength", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_gopAttr_uNumB);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_gopAttr_uNumB", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_gopAttr_uMaxSameSenceCnt);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_gopAttr_uMaxSameSenceCnt", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_gopAttr_bEnableLT);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_gopAttr_bEnableLT", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_gopAttr_uFreqLT);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_gopAttr_uFreqLT", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn0_gopAttr_bLTRC);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_gopAttr_bLTRC", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_encAttr_eProfile);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_encAttr_eProfile", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_encAttr_uLevel);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_encAttr_uLevel", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_encAttr_uTier);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_encAttr_uTier", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_encAttr_ePicFormat);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_encAttr_ePicFormat", tmpBuf);
	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_encAttr_eEncOptions);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_encAttr_eEncOptions", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_encAttr_eEncTools);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_encAttr_eEncTools", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_rcAttr_rcMode);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_rcMode", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_rcAttr_uTargetBitRate);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_uTargetBitRate", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_rcAttr_uMaxBitRate);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_uMaxBitRate", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_rcAttr_iInitialQP);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_iInitialQP", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_rcAttr_iMinQP);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_iMinQP", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_rcAttr_iMaxQP);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_iMaxQP", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_rcAttr_iIPDelta);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_iIPDelta", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_rcAttr_iPBDelta);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_iPBDelta", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_rcAttr_eRcOptions);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_eRcOptions", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_rcAttr_uMaxPictureSize);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_uMaxPictureSize", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_rcAttr_uMaxPSNR);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_uMaxPSNR", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_rcAttr_frmRateDen);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_frmRateDen", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_gopAttr_uGopCtrlMode);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_gopAttr_uGopCtrlMode", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_gopAttr_uGopLength);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_gopAttr_uGopLength", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_gopAttr_uNumB);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_gopAttr_uNumB", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_gopAttr_uMaxSameSenceCnt);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_gopAttr_uMaxSameSenceCnt", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_gopAttr_bEnableLT);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_gopAttr_bEnableLT", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_gopAttr_uFreqLT);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_gopAttr_uFreqLT", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn1_gopAttr_bLTRC);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_gopAttr_bLTRC", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn2_encAttr_eProfile);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_encAttr_eProfile", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn2_encAttr_uLevel);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_encAttr_uLevel", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn2_encAttr_uTier);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_encAttr_uTier", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn2_encAttr_ePicFormat);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_encAttr_ePicFormat", tmpBuf);
	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn2_encAttr_eEncOptions);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_encAttr_eEncOptions", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn2_encAttr_eEncTools);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_encAttr_eEncTools", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn2_rcAttr_rcMode);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_rcAttr_rcMode", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn2_rcAttr_iInitialQP);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_rcAttr_iInitialQP", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn2_rcAttr_frmRateDen);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_rcAttr_frmRateDen", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn2_gopAttr_uGopCtrlMode);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_gopAttr_uGopCtrlMode", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn2_gopAttr_uGopLength);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_gopAttr_uGopLength", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn2_gopAttr_uNumB);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_gopAttr_uNumB", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn2_gopAttr_uMaxSameSenceCnt);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_gopAttr_uMaxSameSenceCnt", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn2_gopAttr_bEnableLT);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_gopAttr_bEnableLT", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn2_gopAttr_uFreqLT);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_gopAttr_uFreqLT", tmpBuf);

	sprintf(tmpBuf, "%d", MediaConfig->video_config.chn2_gopAttr_bLTRC);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_gopAttr_bLTRC", tmpBuf);

	LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "codec_0", MediaConfig->video_config.codec_0);
	LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "codec_1", MediaConfig->video_config.codec_1);
	LUX_INIParse_SetKey_End(LUX_CONFIG_FILE, pAttr);

	pthread_mutex_unlock(&config_mMutex);

	return ret;
}

/// <summary>
/// 录像参数
/// </summary>
/// <param name="Record_cfg"></param>
/// <returns></returns>
int LUX_Config_GetRecordCfg(LUX_CONFIG_RECORD_CFG_ST* record_cfg)
{
	int ret = 0;
	LUX_INIParse_GetIntConfig("record_config", "record_onoff", &record_cfg->record_onoff);
	LUX_INIParse_GetIntConfig("record_config", "record_mode", &record_cfg->record_mode);
	LUX_INIParse_GetIntConfig("record_config", "Reserved_space_KB", &record_cfg->Reserved_space_KB);
	//memcpy(record_cfg, &g_configData->record_config, sizeof(LUX_CONFIG_RECORD_CFG_ST));
	return ret;
}

int LUX_Config_SetRecordCfg(const LUX_CONFIG_RECORD_CFG_ST* record_cfg)
{
	pthread_mutex_lock(&config_mMutex);

	int ret = 0;
	char tmpBuf[16] ={0};
	pAttr = LUX_INIParse_SetKey_begin(LUX_CONFIG_FILE);
	memcpy(&g_configData->record_config, record_cfg, sizeof(LUX_CONFIG_RECORD_CFG_ST));
	sprintf(tmpBuf, "%d", record_cfg->record_onoff);
	ret = LUX_INIParse_SetKey_Input(pAttr, "record_config", "record_onoff", tmpBuf);

	sprintf(tmpBuf, "%d", record_cfg->record_mode);
	ret = LUX_INIParse_SetKey_Input(pAttr, "record_config", "record_mode", tmpBuf);

	sprintf(tmpBuf, "%d", record_cfg->Reserved_space_KB);
	ret = LUX_INIParse_SetKey_Input(pAttr, "record_config", "Reserved_space_KB", tmpBuf);

	LUX_INIParse_SetKey_End(LUX_CONFIG_FILE, pAttr);
	pthread_mutex_unlock(&config_mMutex);

	return ret;
}

/// <summary>
/// 算法参数
/// </summary>
/// <param name="alg_cfg"></param>
/// <returns></returns>
int LUX_Config_GetAlgCfg(LUX_CONFIG_ALG_CFG_ST* alg_cfg)
{
	int ret = 0;
	char tempStr[16] ={0};

	LUX_INIParse_GetIntConfig("algorithm_config", "pir_detect_enabled", &alg_cfg->pir_detect_enabled);
	LUX_INIParse_GetIntConfig("algorithm_config", "pir_detect_m_enabled", &alg_cfg->pir_detect_m_enabled);
	LUX_INIParse_GetIntConfig("algorithm_config", "pir_detect_sensitivity", &alg_cfg->pir_detect_sensitivity);
	LUX_INIParse_GetStrConfig("algorithm_config", "perm_area", alg_cfg->perm_area);
	LUX_INIParse_GetStrConfig("algorithm_config", "perm_area2", alg_cfg->perm_area2);
	LUX_INIParse_GetIntConfig("algorithm_config", "face_detect_enabled", &alg_cfg->face_detect_enabled);
	LUX_INIParse_GetIntConfig("algorithm_config", "face_detect_sensitivity", &alg_cfg->face_detect_sensitivity);
	LUX_INIParse_GetIntConfig("algorithm_config", "human_detect_enabled", &alg_cfg->human_detect_enabled);
	LUX_INIParse_GetIntConfig("algorithm_config", "human_detect_sensitivity", &alg_cfg->human_detect_sensitivity);
	LUX_INIParse_GetStrConfig("algorithm_config", "alarm_zone_size", alg_cfg->alarm_zone_size);

	LUX_INIParse_GetIntConfig("algorithm_config", "motion_detector_enabled", &alg_cfg->motion_detect_enabled);
	LUX_INIParse_GetIntConfig("algorithm_config", "motion_detector_0_sensitivity", &alg_cfg->motion_detect_0_sensitivity);
	LUX_INIParse_GetIntConfig("algorithm_config", "motion_detector_1_threshold", &alg_cfg->motion_detect_1_threshold);
	
	memset(alg_cfg->motion_detect_0_zone, '0', sizeof(alg_cfg->motion_detect_0_zone));
	alg_cfg->motion_detect_0_zone[sizeof(alg_cfg->motion_detect_0_zone) - 1] = '\0';

	LUX_INIParse_GetIntConfig("algorithm_config", "sound_detection_enabled", &alg_cfg->sound_detect_enabled);
	LUX_INIParse_GetIntConfig("algorithm_config", "sound_detection_sensitivity", &alg_cfg->sound_detect_sensitivity);
	LUX_INIParse_GetIntConfig("algorithm_config", "smile_shot_enabled", &alg_cfg->smile_shot_enabled);
	//LUX_INIParse_GetIntConfig("algorithm_config", "cover_face_enabled", &alg_cfg->cover_face_enabled);
	LUX_INIParse_GetIntConfig("algorithm_config", "cry_detect_enabled", &alg_cfg->cry_detect_enabled);

	LUX_INIParse_GetIntConfig("algorithm_config", "alarm_zone_onoff", &alg_cfg->alarm_zone_onoff);
	LUX_INIParse_GetIntConfig("algorithm_config", "alarm_interval", &alg_cfg->alarm_interval);
	LUX_INIParse_GetIntConfig("algorithm_config", "choose_alarm_sound", &alg_cfg->choose_alarm_sound);
	LUX_INIParse_GetIntConfig("algorithm_config", "sleepMode", &alg_cfg->sleepMode);
	LUX_INIParse_GetIntConfig("algorithm_config", "tamper_det_enabled", &alg_cfg->tamper_det_enabled);
	LUX_INIParse_GetIntConfig("algorithm_config", "tamper_cover_th", &alg_cfg->tamper_cover_th);
	LUX_INIParse_GetIntConfig("algorithm_config", "tamper_det_sensitivity", &alg_cfg->tamper_det_sensitivity);
	LUX_INIParse_GetIntConfig("algorithm_config", "crossline_det_enabled", &alg_cfg->crossline_det_enabled);
	LUX_INIParse_GetStrConfig("algorithm_config", "line_area", alg_cfg->line_area);
	LUX_INIParse_GetIntConfig("algorithm_config", "loiter_det_enabled", &alg_cfg->loiter_det_enabled);
	LUX_INIParse_GetIntConfig("algorithm_config", "object_det_enabled", &alg_cfg->object_det_enabled);
	LUX_INIParse_GetIntConfig("algorithm_config", "object_det_sensitivity", &alg_cfg->object_det_sensitivity);
	LUX_INIParse_GetIntConfig("algorithm_config", "object_det_gray_scale", &alg_cfg->object_det_gray_scale);
	LUX_INIParse_GetIntConfig("algorithm_config", "object_det_max_area", &alg_cfg->object_det_max_area);
	LUX_INIParse_GetIntConfig("algorithm_config", "object_det_min_area", &alg_cfg->object_det_min_area);
	LUX_INIParse_GetStrConfig("algorithm_config", "leftdet_area", alg_cfg->leftdet_area);
	LUX_INIParse_GetStrConfig("algorithm_config", "lossdet_obj", alg_cfg->lossdet_obj);
	//memcpy(alg_cfg, &g_configData->alg_config, sizeof(LUX_CONFIG_ALG_CFG_ST));
	// alg_cfg->night_vision_mode = find_option(tempStr, nightvision_modes, (sizeof(nightvision_modes) / sizeof(nightvision_modes[0])));
	// if (!alg_cfg->night_vision_mode) {
	// 	alg_cfg->night_vision_mode = nightvision_modes[0];
	// }
	return ret;
}

int LUX_Config_SetAlgCfg(const LUX_CONFIG_ALG_CFG_ST* alg_cfg)
{
	pthread_mutex_lock(&config_mMutex);

	int ret = 0;
	char tmpBuf[16] ={0};
	pAttr = LUX_INIParse_SetKey_begin(LUX_CONFIG_FILE);
	memcpy(&g_configData->alg_config, alg_cfg, sizeof(LUX_CONFIG_ALG_CFG_ST));

	sprintf(tmpBuf, "%d", alg_cfg->pir_detect_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "pir_detect_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", alg_cfg->motion_detect_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "motion_detector_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", alg_cfg->motion_detect_0_sensitivity);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "motion_detector_0_sensitivity", tmpBuf);

	sprintf(tmpBuf, "%d", alg_cfg->motion_detect_1_threshold);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "motion_detector_1_threshold", tmpBuf);

	sprintf(tmpBuf, "%d", alg_cfg->face_detect_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "face_detect_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", alg_cfg->sound_detect_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "sound_detection_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", alg_cfg->sound_detect_sensitivity);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "sound_detection_sensitivity", tmpBuf);

	sprintf(tmpBuf, "%d", alg_cfg->smile_shot_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "smile_shot_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", alg_cfg->cry_detect_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "cry_detect_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", alg_cfg->human_detect_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "human_detect_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", alg_cfg->alarm_zone_onoff);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "alarm_zone_onoff", tmpBuf);

	sprintf(tmpBuf, "%d", alg_cfg->alarm_interval);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "alarm_interval", tmpBuf);

	sprintf(tmpBuf, "%d", alg_cfg->choose_alarm_sound);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "choose_alarm_sound", tmpBuf);

	//LUX_INIParse_SetKey_Input(pAttr, "image_config", "daynight", alg_cfg->night_vision_mode);
	LUX_INIParse_SetKey_End(LUX_CONFIG_FILE, pAttr);
	pthread_mutex_unlock(&config_mMutex);

	return ret;
}

/// <summary>
/// 网络参数
/// </summary>
/// <param name="Network_cfg"></param>
/// <returns></returns>
int LUX_Config_GetNetworkCfg(LUX_CONFIG_NETWORK_CFG_ST* network_cfg)
{
	int ret = 0;
	LUX_INIParse_GetIntConfig("network_config", "webport", &network_cfg->webport);
	LUX_INIParse_GetIntConfig("network_config", "rtsp_port", &network_cfg->rtsp_port);
	LUX_INIParse_GetIntConfig("network_config", "onvif_port", &network_cfg->onvif_port);
	LUX_INIParse_GetIntConfig("network_config", "ssh_port", &network_cfg->ssh_port);
	//memcpy(network_cfg, &g_configData->network_config, sizeof(LUX_CONFIG_NETWORK_CFG_ST));
	return ret;
}

int LUX_Config_SetNetworkCfg(const LUX_CONFIG_NETWORK_CFG_ST* network_cfg)
{
	pthread_mutex_lock(&config_mMutex);

	int ret = 0;
	char tmpBuf[16] ={0};
	pAttr = LUX_INIParse_SetKey_begin(LUX_CONFIG_FILE);
	memcpy(&g_configData->network_config, network_cfg, sizeof(LUX_CONFIG_NETWORK_CFG_ST));

	sprintf(tmpBuf, "%d", network_cfg->webport);
	ret = LUX_INIParse_SetKey_Input(pAttr, "network_config", "webport", tmpBuf);

	sprintf(tmpBuf, "%d", network_cfg->rtsp_port);
	ret = LUX_INIParse_SetKey_Input(pAttr, "network_config", "rtsp_port", tmpBuf);

	sprintf(tmpBuf, "%d", network_cfg->onvif_port);
	ret = LUX_INIParse_SetKey_Input(pAttr, "network_config", "onvif_port", tmpBuf);

	sprintf(tmpBuf, "%d", network_cfg->ssh_port);
	ret = LUX_INIParse_SetKey_Input(pAttr, "network_config", "ssh_port", tmpBuf);

	LUX_INIParse_SetKey_End(LUX_CONFIG_FILE, pAttr);
	pthread_mutex_unlock(&config_mMutex);

	return ret;
}

const char* find_option(const char* tempStr, const char* const* array, int size)
{
    int i;
    const char* option;
    for (i = 0; i < size; i++)
    {
        if (0 == strcmp(array[i], tempStr))
        {
            printf( "\narray[%d] = %s \n", i, array[i]);
            option = array[i];
            return option;
        }
    }
    //printf( "\nThe configuration read is out of range,tempStr = %s \n", tempStr);
    return 0;
}

// int LUX_Config_GetJiocfg(LUX_CONFIG_JIO_CFG_ST* g_JioConfig)
// {
// 	int ret = 0;
// 	char tempStr[32] = { 0 };
// 	//LUX_INIParse_GetIntConfig("system_config", "online_mode", &g_JioConfig->online_mode);
// 	// LUX_INIParse_GetStrConfig("system_config", "timezone", tempStr);
// 	// g_JioConfig->timezone = find_option(tempStr, tz_offsets, (sizeof(tz_offsets) / sizeof(tz_offsets[0])));
// 	// if (!g_JioConfig->timezone) {
// 	// 	g_JioConfig->timezone = tz_offsets[0];
// 	// }
// 	// LUX_INIParse_GetIntConfig("media_config_audio", "microphone_enabled", &g_JioConfig->microphone_enabled);
// 	LUX_INIParse_GetIntConfig("system_config", "led_enabled", &g_JioConfig->led_enabled);
// 	LUX_INIParse_GetStrConfig("image_config", "daynight", tempStr);
// 	// g_JioConfig->night_vision_mode = find_option(tempStr, nightvision_modes, (sizeof(nightvision_modes) / sizeof(nightvision_modes[0])));
// 	// if (!g_JioConfig->night_vision_mode) {
// 	// 	g_JioConfig->night_vision_mode = nightvision_modes[0];
// 	// }
// 	// LUX_INIParse_GetStrConfig("system_config", "power_freq", tempStr);
// 	// g_JioConfig->power_freq = find_option(tempStr, idip_cfg_range_power_freq, (sizeof(idip_cfg_range_power_freq) / sizeof(idip_cfg_range_power_freq[0])));
// 	// if (!g_JioConfig->power_freq) {
// 	// 	g_JioConfig->power_freq = idip_cfg_range_power_freq[0];
// 	// }
// 	LUX_INIParse_GetIntConfig("image_config", "vflip", &g_JioConfig->flip_mode);
// 	LUX_INIParse_GetIntConfig("algorithm_config", "sound_detection_enabled", &g_JioConfig->sound_detection);
// 	LUX_INIParse_GetIntConfig("algorithm_config", "sound_detection_sensitivity", &g_JioConfig->sound_detection_sensitivity);
// 	LUX_INIParse_GetIntConfig("algorithm_config", "pir_detect_enabled", &g_JioConfig->pir_detection);
// 	LUX_INIParse_GetIntConfig("algorithm_config", "motion_detector_enabled", &g_JioConfig->motion_detector);
// 	LUX_INIParse_GetIntConfig("algorithm_config", "motion_detector_0_sensitivity", &g_JioConfig->motion_detector_0_sensitivity);
// 	LUX_INIParse_GetIntConfig("algorithm_config", "motion_detector_1_threshold", &g_JioConfig->motion_detector_0_threshold);
// 	memset(g_JioConfig->motion_detector_0_zone, '0', sizeof(g_JioConfig->motion_detector_0_zone));
// 	g_JioConfig->motion_detector_0_zone[sizeof(g_JioConfig->motion_detector_0_zone) - 1] = '\0';
// 	//LUX_INIParse_GetIntConfig("image_config", "osd_show_time", &g_JioConfig->osd_show_time);
// 	// LUX_INIParse_GetStrConfig("image_config", "resolution_0", tempStr);
// 	// g_JioConfig->resolution_0 = find_option(tempStr, resolutions_0, (sizeof(resolutions_0) / sizeof(resolutions_0[0])));
// 	// if (!g_JioConfig->resolution_0) {
// 	// 	g_JioConfig->resolution_0 = resolutions_0[0];
// 	// }
// 	// LUX_INIParse_GetStrConfig("image_config", "resolution_1", tempStr);
// 	// g_JioConfig->resolution_1 = find_option(tempStr, resolutions_1, (sizeof(resolutions_1) / sizeof(resolutions_1[0])));
// 	// if (!g_JioConfig->resolution_1) {
// 	// 	g_JioConfig->resolution_1 = resolutions_1[0];
// 	// }
// 	// LUX_INIParse_GetIntConfig("image_config", "bitrate_0", &g_JioConfig->bitrate_0);
// 	// LUX_INIParse_GetIntConfig("image_config", "bitrate_1", &g_JioConfig->bitrate_1);
// 	// LUX_INIParse_GetIntConfig("image_config", "fps_0", &g_JioConfig->fps_0);
// 	// LUX_INIParse_GetIntConfig("image_config", "fps_1", &g_JioConfig->fps_1);
// 	// LUX_INIParse_GetStrConfig("media_config_video", "codec_0", tempStr);
// 	// g_JioConfig->codec_0 = find_option(tempStr, codecs, (sizeof(codecs) / sizeof(codecs[0])));
// 	// if (!g_JioConfig->codec_0) {
// 	// 	g_JioConfig->codec_0 = codecs[0];
// 	// }
// 	// LUX_INIParse_GetStrConfig("media_config_video", "codec_1", tempStr);
// 	// g_JioConfig->codec_1 = find_option(tempStr, codecs, (sizeof(codecs) / sizeof(codecs[0])));
// 	// if (!g_JioConfig->codec_1) {
// 	// 	g_JioConfig->codec_1 = codecs[0];
// 	// }
// 	// LUX_INIParse_GetStrConfig("media_config_audio", "acodec_0", tempStr);
// 	// g_JioConfig->acodec_0 = find_option(tempStr, acodecs, (sizeof(acodecs) / sizeof(acodecs[0])));
// 	// if (!g_JioConfig->acodec_0) {
// 	// 	g_JioConfig->acodec_0 = acodecs[0];
// 	// }
// 	// LUX_INIParse_GetStrConfig("media_config_audio", "acodec_1", tempStr);
// 	// g_JioConfig->acodec_1 = find_option(tempStr, acodecs, (sizeof(acodecs) / sizeof(acodecs[0])));
// 	// if (!g_JioConfig->acodec_1) {
// 	// 	g_JioConfig->acodec_1 = acodecs[0];
// 	// }
// 	//LUX_INIParse_GetIntConfig("algorithm_config", "face_detect_enabled", &g_JioConfig->face_detection);
// 	LUX_INIParse_GetIntConfig("algorithm_config", "privacy_mask", &g_JioConfig->privacy_mask);
// 	//LUX_INIParse_GetIntConfig("system_config", "online", &g_JioConfig->online);
// 	if (g_JioConfig->led_enabled)
// 	{
// 		LUX_PWM_Start();
// 	}
// 	else
// 	{
// 		LUX_PWM_Start();
// 	}
// 	if (g_JioConfig->microphone_enabled)
// 	{
// 		//int ret = LUX_AI_Start();
// 		if (0 != ret)
// 		{
// 			MYERROR("ImpStreamOn failed\n");
// 			return -1;
// 		}
// 	}

// 	return ret;
// }

// int LUX_Config_SetJiocfg(const LUX_CONFIG_JIO_CFG_ST *g_JioConfig)
// {		
// 		pthread_mutex_lock(&config_mMutex);

// 		int ret = 0;
// 		char tempStr[16] = { 0 };
// 		pAttr = LUX_INIParse_SetKey_begin(LUX_CONFIG_FILE);
// 		 memcpy(&g_configData->g_JioConfig, g_JioConfig, sizeof(LUX_CONFIG_JIO_CFG_ST));
// 		// LUX_INIParse_SetKey_Input(pAttr, "system_config", "online_mode", g_JioConfig->online_mode == 0 ? "0" : "1");
// 		LUX_INIParse_SetKey_Input(pAttr,"media_config_audio", "microphone_enabled", g_JioConfig->microphone_enabled == 0 ? "0" : "1");
// 		LUX_INIParse_SetKey_Input(pAttr, "system_config", "led_enabled", g_JioConfig->led_enabled == 0 ? "0" : "1");
// 		//LUX_INIParse_SetKey_Input(pAttr, "system_config", "timezone", g_JioConfig->timezone);
// 		LUX_INIParse_SetKey_Input(pAttr, "image_config", "daynight", g_JioConfig->night_vision_mode);
// 		// LUX_INIParse_SetKey_Input(pAttr, "system_config", "power_freq", g_JioConfig->power_freq);
// 		LUX_INIParse_SetKey_Input(pAttr, "image_config", "vflip", g_JioConfig->flip_mode==0?"0":"1");
// 		LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "sound_detection_enabled", g_JioConfig->sound_detection==0?"0":"1");
// 		sprintf(tempStr, "%d", g_JioConfig->sound_detection_sensitivity);
// 		LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "sound_detection_sensitivity", tempStr);
// 		LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "pir_detect_enabled", g_JioConfig->pir_detection==0?"0":"1");
// 		LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "motion_detector_enabled", g_JioConfig->motion_detector==0?"0":"1");
// 		sprintf(tempStr, "%d", g_JioConfig->motion_detector_0_sensitivity);
// 		LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "motion_detector_0_sensitivity", tempStr);
// 		sprintf(tempStr, "%d", g_JioConfig->motion_detector_0_threshold);
//         LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "motion_detector_1_threshold", tempStr);
// 		//LUX_INIParse_SetKey_Input(pAttr, "image_config", "osd_show_time", g_JioConfig->osd_show_time==0?"0":"1");
// 		// LUX_INIParse_SetKey_Input(pAttr, "image_config", "resolution_1", g_JioConfig->resolution_1);
// 		// LUX_INIParse_SetKey_Input(pAttr, "image_config", "resolution_0", g_JioConfig->resolution_0);
// 		// sprintf(tempStr, "%d", g_JioConfig->bitrate_0);
// 		// LUX_INIParse_SetKey_Input(pAttr, "image_config", "bitrate_0", tempStr);
// 		// sprintf(tempStr, "%d", g_JioConfig->bitrate_1);
// 		// LUX_INIParse_SetKey_Input(pAttr, "image_config", "bitrate_1", tempStr);
// 		// sprintf(tempStr, "%d", g_JioConfig->fps_0);
// 		// LUX_INIParse_SetKey_Input(pAttr, "image_config", "fps_0", tempStr);
// 		// sprintf(tempStr, "%d", g_JioConfig->fps_1);
//  		// LUX_INIParse_SetKey_Input(pAttr, "image_config", "fps_1", tempStr);
// 		// LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "codec_0", g_JioConfig->codec_0);
// 		// LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "codec_1", g_JioConfig->codec_1);
//         // LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "acodec_0", g_JioConfig->acodec_0);
// 		// LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "acodec_1", g_JioConfig->acodec_1);
// 		//LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "face_detect_enabled", g_JioConfig->face_detection==0?"0":"1");
// 		LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "privacy_mask", g_JioConfig->privacy_mask==0?"0":"1");	
// 		//LUX_INIParse_SetKey_Input(pAttr, "system_config", "online", g_JioConfig->online == 0 ? "0" : "1");
// 		 LUX_INIParse_SetKey_End(LUX_CONFIG_FILE, pAttr);
// 		pthread_mutex_unlock(&config_mMutex);

// 	return ret;
// }

int LUX_Config_Set_AllConfig(const LUX_CONFIG_DATA_ST* g_config)
{
	int ret = 0;
	char tmpBuf[16] ={0};
	int temp;

	pAttr = LUX_INIParse_SetKey_begin(LUX_CONFIG_FILE);
	memcpy(g_configData, g_config, sizeof(LUX_CONFIG_DATA_ST));

	sprintf(tmpBuf, "%d", g_config->gpio_config.irled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "gpio_config", "irled", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->gpio_config.speaker);
	ret = LUX_INIParse_SetKey_Input(pAttr, "gpio_config", "speaker", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->gpio_config.reset);
	ret = LUX_INIParse_SetKey_Input(pAttr, "gpio_config", "reset", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->gpio_config.color_green);
	ret = LUX_INIParse_SetKey_Input(pAttr, "gpio_config", "color_green", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->gpio_config.color_red);
	ret = LUX_INIParse_SetKey_Input(pAttr, "gpio_config", "color_red", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->gpio_config.ircutn);
	ret = LUX_INIParse_SetKey_Input(pAttr, "gpio_config", "ircutn", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->gpio_config.ircutp);
	ret = LUX_INIParse_SetKey_Input(pAttr, "gpio_config", "ircutp", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.microphone_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "microphone_enabled", tmpBuf);

	if(!strcmp("PCM",g_config->media_config.audio_config.acodec_0))
	{
		temp = 0;
	}
	else
	{
		temp = 4;
	}
	sprintf(tmpBuf, "%d", temp);
	LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "audio_in_type", tmpBuf);

	if(!strcmp("PCM",g_config->media_config.audio_config.acodec_0))
	{
		temp = 0;
	}
	else
	{
		temp = 4;
	}
	sprintf(tmpBuf, "%d", temp);
	LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "audio_in_type", tmpBuf);
	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.audio_aec_en);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "audio_aec_en", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.audio_in_type);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "audio_in_type", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.audio_in_sample_rate);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "audio_in_sample_rate", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.ai_volume);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ai_volume", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.ai_gain);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ai_gain", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.ai_ns_en);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ai_ns_en", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.ai_ns_level);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ai_ns_level", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.ai_hpf_en);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ai_hpf_en", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.ai_agc_en);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ai_agc_en", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.ai_agc_level);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ai_agc_level", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.ai_agc_gain);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ai_agc_gain", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.audio_out_type);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "audio_out_type", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.audio_out_sample_rate);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "audio_out_sample_rate", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.ao_volume);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ao_volume", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.ao_gain);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ao_gain", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.ao_hpf_en);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ao_hpf_en", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.ao_agc_en);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ao_agc_en", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.ao_agc_level);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ao_agc_level", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.audio_config.ao_agc_gain);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_audio", "ao_agc_gain", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_encAttr_eProfile);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_encAttr_eProfile", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_encAttr_uLevel);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_encAttr_uLevel", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_encAttr_uTier);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_encAttr_uTier", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_encAttr_ePicFormat);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_encAttr_ePicFormat", tmpBuf);

	sprintf(tmpBuf, "%d",  g_config->media_config.video_config.chn0_encAttr_eEncOptions);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_encAttr_eEncOptions",tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_encAttr_eEncTools);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_encAttr_eEncTools", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_rcAttr_rcMode);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_rcMode", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_rcAttr_uTargetBitRate);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_uTargetBitRate", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_rcAttr_uMaxBitRate);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_uMaxBitRate", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_rcAttr_iInitialQP);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_iInitialQP", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_rcAttr_iMinQP);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_iMinQP", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_rcAttr_iMaxQP);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_iMaxQP", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_rcAttr_iIPDelta);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_iIPDelta", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_rcAttr_iPBDelta);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_iPBDelta", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_rcAttr_eRcOptions);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_eRcOptions", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_rcAttr_uMaxPictureSize);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_uMaxPictureSize", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_rcAttr_uMaxPSNR);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_uMaxPSNR", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_rcAttr_frmRateDen);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_rcAttr_frmRateDen", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_gopAttr_uGopCtrlMode);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_gopAttr_uGopCtrlMode", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_gopAttr_uGopLength);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_gopAttr_uGopLength", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_gopAttr_uNumB);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_gopAttr_uNumB", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_gopAttr_uMaxSameSenceCnt);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_gopAttr_uMaxSameSenceCnt", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_gopAttr_bEnableLT);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_gopAttr_bEnableLT", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_gopAttr_uFreqLT);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_gopAttr_uFreqLT", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn0_gopAttr_bLTRC);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_gopAttr_bLTRC", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_encAttr_eProfile);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_encAttr_eProfile", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_encAttr_uLevel);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_encAttr_uLevel", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_encAttr_uTier);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_encAttr_uTier", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_encAttr_ePicFormat);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_encAttr_ePicFormat", tmpBuf);
	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_encAttr_eEncOptions);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_encAttr_eEncOptions", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_encAttr_eEncTools);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_encAttr_eEncTools", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_rcAttr_rcMode);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_rcMode", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_rcAttr_uTargetBitRate);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_uTargetBitRate", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_rcAttr_uMaxBitRate);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_uMaxBitRate", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_rcAttr_iInitialQP);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_iInitialQP", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_rcAttr_iMinQP);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_iMinQP", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_rcAttr_iMaxQP);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_iMaxQP", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_rcAttr_iIPDelta);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_iIPDelta", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_rcAttr_iPBDelta);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_iPBDelta", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_rcAttr_eRcOptions);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_eRcOptions", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_rcAttr_uMaxPictureSize);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_uMaxPictureSize", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_rcAttr_uMaxPSNR);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_uMaxPSNR", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_rcAttr_frmRateDen);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_rcAttr_frmRateDen", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_gopAttr_uGopCtrlMode);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_gopAttr_uGopCtrlMode", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_gopAttr_uGopLength);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_gopAttr_uGopLength", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_gopAttr_uNumB);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_gopAttr_uNumB", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_gopAttr_uMaxSameSenceCnt);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_gopAttr_uMaxSameSenceCnt", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_gopAttr_bEnableLT);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_gopAttr_bEnableLT", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_gopAttr_uFreqLT);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_gopAttr_uFreqLT", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn1_gopAttr_bLTRC);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_gopAttr_bLTRC", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn2_encAttr_eProfile);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_encAttr_eProfile", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn2_encAttr_uLevel);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_encAttr_uLevel", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn2_encAttr_uTier);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_encAttr_uTier", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn2_encAttr_ePicFormat);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_encAttr_ePicFormat", tmpBuf);
	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn2_encAttr_eEncOptions);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_encAttr_eEncOptions", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn2_encAttr_eEncTools);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_encAttr_eEncTools", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn2_rcAttr_rcMode);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_rcAttr_rcMode", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn2_rcAttr_iInitialQP);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_rcAttr_iInitialQP", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn2_rcAttr_frmRateDen);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_rcAttr_frmRateDen", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn2_gopAttr_uGopCtrlMode);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_gopAttr_uGopCtrlMode", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn2_gopAttr_uGopLength);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_gopAttr_uGopLength", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn2_gopAttr_uNumB);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_gopAttr_uNumB", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn2_gopAttr_uMaxSameSenceCnt);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_gopAttr_uMaxSameSenceCnt", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn2_gopAttr_bEnableLT);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_gopAttr_bEnableLT", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn2_gopAttr_uFreqLT);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_gopAttr_uFreqLT", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->media_config.video_config.chn2_gopAttr_bLTRC);
	ret = LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn2_gopAttr_bLTRC", tmpBuf);

	if(!strcmp("H.265",g_config->media_config.video_config.codec_0))
	{
		temp = 16777217;
	}
	else
	{
		temp = 100;
	}
	sprintf(tmpBuf, "%d", temp);

	LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn0_encAttr_eProfile", tmpBuf);
	
	if(!strcmp("H.265",g_config->media_config.video_config.codec_1))
	{
		temp = 16777217;
	}
	else
	{
		temp = 100;
	}
	sprintf(tmpBuf, "%d", temp);
	LUX_INIParse_SetKey_Input(pAttr, "media_config_video", "chn1_encAttr_eProfile", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->system_config.led_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr,"system_config", "led_enabled", tmpBuf);

	// sprintf(tmpBuf, "%d", g_config->system_config.timezone);
	LUX_INIParse_SetKey_Input(pAttr, "system_config", "timezone", g_config->system_config.timezone);
	
	// sprintf(tmpBuf, "%d", g_config->system_config.power_freq);
	if(!strcmp("50 HZ",g_config->system_config.power_freq))
	{
		temp = 50;
	}
	else
	{
		temp = 60;
	}
	sprintf(tmpBuf, "%d", temp);
	LUX_INIParse_SetKey_Input(pAttr, "system_config", "power_freq", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->system_config.platform_type);
	ret = LUX_INIParse_SetKey_Input(pAttr, "system_config", "platform_type", tmpBuf);

	// sprintf(tmpBuf, "%d", g_config->system_config.online);
	LUX_INIParse_SetKey_Input(pAttr, "system_config", "online_mode", g_config->system_config.online_mode == 0 ? "0" : "1");
	
	sprintf(tmpBuf, "%d", g_config->system_config.jio_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "system_config", "jio_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->system_config.timezone_offset_s);
	ret = LUX_INIParse_SetKey_Input(pAttr, "system_config", "timezone_offset_s", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.pir_detect_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "pir_detect_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.pir_detect_m_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "pir_detect_m_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.motion_detect_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "motion_detector_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.motion_detect_0_sensitivity);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "motion_detector_0_sensitivity", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.motion_detect_1_threshold);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "motion_detector_1_threshold", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.face_detect_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "face_detect_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.sound_detect_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "sound_detection_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.sound_detect_sensitivity);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "sound_detection_sensitivity", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.smile_shot_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "smile_shot_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.cry_detect_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "cry_detect_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.human_detect_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "human_detect_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.alarm_zone_onoff);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "alarm_zone_onoff", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.alarm_interval);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "alarm_interval", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.choose_alarm_sound);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "choose_alarm_sound", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.tamper_det_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "tamper_det_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.tamper_cover_th);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "tamper_cover_th", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.tamper_det_sensitivity);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "tamper_det_sensitivity", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.crossline_det_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "crossline_det_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.object_det_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "object_det_enabled", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.object_det_sensitivity);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "object_det_sensitivity", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.object_det_gray_scale);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "object_det_gray_scale", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.object_det_max_area);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "object_det_max_area", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.object_det_min_area);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "object_det_min_area", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->alg_config.loiter_det_enabled);
	ret = LUX_INIParse_SetKey_Input(pAttr, "algorithm_config", "loiter_det_enabled", tmpBuf);

	LUX_INIParse_SetKey_Input(pAttr,"algorithm_config", "perm_area", g_config->alg_config.perm_area);

	LUX_INIParse_SetKey_Input(pAttr,"algorithm_config", "perm_area2", g_config->alg_config.perm_area2);

	LUX_INIParse_SetKey_Input(pAttr,"algorithm_config", "alarm_zone_size", g_config->alg_config.alarm_zone_size);

	LUX_INIParse_SetKey_Input(pAttr,"algorithm_config", "line_area", g_config->alg_config.line_area);

	LUX_INIParse_SetKey_Input(pAttr,"algorithm_config", "leftdet_area", g_config->alg_config.leftdet_area);

	LUX_INIParse_SetKey_Input(pAttr,"algorithm_config", "lossdet_obj", g_config->alg_config.lossdet_obj);

	sprintf(tmpBuf, "%d", g_config->record_config.record_onoff);
	ret = LUX_INIParse_SetKey_Input(pAttr, "record_config", "record_onoff", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->record_config.record_mode);
	ret = LUX_INIParse_SetKey_Input(pAttr, "record_config", "record_mode", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->record_config.Reserved_space_KB);
	ret = LUX_INIParse_SetKey_Input(pAttr, "record_config", "Reserved_space_KB", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->image_config.vflip);
	ret = LUX_INIParse_SetKey_Input(pAttr, "image_config", "vflip", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->image_config.hflip);
	ret = LUX_INIParse_SetKey_Input(pAttr, "image_config", "hflip", tmpBuf);
	
	sprintf(tmpBuf, "%d", g_config->image_config.daynight_lowthreshold);
	ret = LUX_INIParse_SetKey_Input(pAttr, "image_config", "daynight_lowthreshold", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->image_config.daynight_highthreshold);
	ret = LUX_INIParse_SetKey_Input(pAttr, "image_config", "daynight_highthreshold", tmpBuf);

	/*sprintf(tmpBuf, "%d", g_config->image_config.cntmax);
	ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "image_config", "cntmax", tmpBuf);*/

	 sprintf(tmpBuf, "%d", g_config->image_config.osd_show_time);
	 ret = LUX_INIParse_SetKey_Input(pAttr, "image_config", "osd_show_time", tmpBuf);

	if(!strcmp("auto", g_config->image_config.daynight))
	{
		temp = 2;
	}
	else if(!strcmp("on", g_config->image_config.daynight))
	{
		temp = 1;
	}
	else
	{
		temp = 0;
	}

	 sprintf(tmpBuf, "%d", temp);
	 ret = LUX_INIParse_SetKey_Input(pAttr, "image_config", "daynight", tmpBuf);
	 
		LUX_INIParse_SetKey_Input(pAttr, "image_config", "resolution_1", g_config->image_config.resolution_1);
		LUX_INIParse_SetKey_Input(pAttr, "image_config", "resolution_0", g_config->image_config.resolution_0);
		sprintf(tmpBuf, "%d", g_config->image_config.bitrate_0);
		LUX_INIParse_SetKey_Input(pAttr, "image_config", "bitrate_0", tmpBuf);
		sprintf(tmpBuf, "%d", g_config->image_config.bitrate_1);
		LUX_INIParse_SetKey_Input(pAttr, "image_config", "bitrate_1", tmpBuf);
		sprintf(tmpBuf, "%d", g_config->image_config.fps_0);
		LUX_INIParse_SetKey_Input(pAttr, "image_config", "fps_0", tmpBuf);
		sprintf(tmpBuf, "%d", g_config->image_config.fps_1);
 		LUX_INIParse_SetKey_Input(pAttr, "image_config", "fps_1", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->image_config.wdr_onoff);
	ret = LUX_INIParse_SetKey_Input(pAttr, "image_config", "wdr_onoff", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->image_config.watermark_onoff);
	ret = LUX_INIParse_SetKey_Input(pAttr, "image_config", "watermark_onoff", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->network_config.webport);
	ret = LUX_INIParse_SetKey_Input(pAttr, "network_config", "webport", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->network_config.rtsp_port);
	ret = LUX_INIParse_SetKey_Input(pAttr, "network_config", "rtsp_port", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->network_config.onvif_port);
	ret = LUX_INIParse_SetKey_Input(pAttr, "network_config", "onvif_port", tmpBuf);

	sprintf(tmpBuf, "%d", g_config->network_config.ssh_port);
	ret = LUX_INIParse_SetKey_Input(pAttr, "network_config", "ssh_port", tmpBuf);

	LUX_INIParse_SetKey_End(LUX_CONFIG_FILE, pAttr);

	//pthread_mutex_unlock(&config_mMutex);


}

int LUX_INIParse_GetIntConfig(const char* pTitle, const char* pKey, int* value)
{
	INT_X ret = 0;
	char tmpBuf[32];
	ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, (PCHAR_X)pTitle, (PCHAR_X)pKey, (PINT_X)value);
	sprintf(tmpBuf, "%d", *value);
	if (LUX_INI_GET_DIC_ATTR_FAILED == ret || LUX_INI_GET_PARAM_FAILED == ret)
    {
        MYERROR("read config %s failed, now create... ... \n",pKey);
        LUX_INIParse_SetKey(LUX_CONFIG_FILE, pTitle, pKey, tmpBuf);
    }
    else if (0 != ret)
    {
        MYERROR("read config %s failed,error [0x%x]\n", *value, ret);
    }

	return ret;
}

int LUX_INIParse_GetStrConfig(const char* pTitle, const char* pKey, char* pOutStr)
{
	INT_X ret;
	ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, (PCHAR_X)pTitle, (PCHAR_X)pKey, (PCHAR_X)pOutStr);
	if (LUX_INI_GET_DIC_ATTR_FAILED == ret || LUX_INI_GET_PARAM_FAILED == ret)
    {
        MYERROR("read config %s failed, now create... ... \n",pKey);
        //LUX_INIParse_SetKey(LUX_CONFIG_FILE, pTitle, pKey, pOutStr);
    }
    else if (0 != ret)
    {
        MYERROR("read config %s failed,error [0x%x]\n", *pOutStr, ret);
    }
	return ret;
}


int jio_system_init(LUX_CONFIG_IMAGE_CFG_ST* image_cfg){

	#if 1
	int ret;
    /*通道0的分辨率设置为默认值*/
	int width_value;
    int height_value;
    int fsChnNum =0;
    uint32_t fps_den = 1;
    uint32_t fps_num = 0;
    uint32_t impvi_num = 0;
    MYTRACE("channel_0 resolution_0:[%s] bitrate:[%d] fps:[%d] \n",image_cfg->resolution_0,image_cfg->bitrate_0,image_cfg->fps_0);
    sscanf(image_cfg->resolution_0, "%dx%d", &width_value, &height_value);

    ret = LUX_Video_Change_Resolution(fsChnNum, width_value, height_value);
    if (0 != ret)
    {
        MYERROR("video change resolution failed,error [0x%x]\n", ret);
    }

    LUX_Video_Func_Set_Bitrate(fsChnNum,image_cfg->bitrate_0);
    if (0 != ret)
    {
        MYERROR("video change bitrate failed failed,error [0x%x]\n", ret);
    }
    fps_num =image_cfg->fps_0;
    impvi_num = 0;
    LUX_Video_Func_Set_Fps(0, &fps_num, &fps_den);
    if (0 != ret)
    {
        MYERROR("video change Fps failed failed,error [0x%x]\n", ret);
    }

    /*通道1的分辨率设置设置为默认值*/
    fsChnNum = 1;
    MYTRACE("channel_1 resolution_0:[%s] bitrate:[%d] fps:[%d] \n",image_cfg->resolution_1,image_cfg->bitrate_1,image_cfg->fps_1);
    sscanf(image_cfg->resolution_1, "%dx%d", &width_value, &height_value);

    ret = LUX_Video_Change_Resolution(fsChnNum, width_value, height_value);
    if (0 != ret)
    {
        MYERROR("video change resolution failed,error [0x%x]\n", ret);
    }

    LUX_Video_Func_Set_Bitrate(fsChnNum,image_cfg->bitrate_1);
    if (0 != ret)
    {
        MYERROR("video change bitrate failed failed,error [0x%x]\n", ret);
    }
    fps_num =image_cfg->fps_1;
    impvi_num = 1;
    LUX_Video_Func_Set_Fps(0, &fps_num, &fps_den);
    if (0 != ret)
    {
        MYERROR("video change Fps failed failed,error [0x%x]\n", ret);
    }
#endif
	return ret;
}


