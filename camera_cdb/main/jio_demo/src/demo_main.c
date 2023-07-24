#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <sys/mman.h>
#include <linux/limits.h>
#include <poll.h>
#include <getopt.h>
//#define NDEBUG //可在包含的#include前加#define NDBUG来禁用assert调用
#include <assert.h>
#include <libwebsockets.h>

// #include <libavformat/avformat.h>
// #include <libavfilter/avfilter.h>
#include <idip.h>                    // the main library header
#include <idip/idip_cloud_config.h>  // cloud config helper API
#include <idip/idip_custom_events.h> // extended events API
#include <idip/idip_log.h>           // logging functions
#include <imp_log.h>
#include "avpkt_stream_source.h"
#include "archive.h"
#include "playback.h"
#include "idip/idip_wrappers.h"
#include <idip_stream_ar.h>

#include "comm_def.h"
#include "comm_func.h" 
#include "lux_base.h"
#include "lux_video.h"
#include "lux_qrcode.h"
#include "lux_event.h"
#include "lux_wifi.h"
#include "lux_hal_motor.h"
#include "lux_audio.h"
#include "lux_time.h"
#include <lux_ipc_control.h>
#include "lux_daynight.h"
#include "lux_ivs.h"
#include "lux_pwm.h"
#include "lux_hal_led.h"
#include "comm_error_code.h"
#include "lux_config.h"
#include <jwhead.h>
#include "fdk-aac/aacenc_lib.h"
#include "lux_global.h"
#include "lux_record.h"
#include "lux_hal_led.h"
#include "AppVersion.h"
#include "lux_person_det.h"


#if 1//cxj test
#define KEY "idip_jio_luxshare_camera-ak99g8j5ee"
#define URL "https://idip-v1.cloud.smartmonitoring.jio.com/idip_uplink"
#define HOST_PATH "/system/jio/idip.sock"
#define OTA_TMP_PATH "/tmp/ota.zip"
#define PERSIET_PATH "/system/jio/idip-persistent"
#define SERIAL_PATH "/system/jio/serial_num"
//#define VERSION "a.b.c.d_xxxx_xx_xx_zzzzzzzz"
#define CERT_PATH "/system/jio/cacert.pem"
#define ALIVE_TIME 20000ul
#define SERIAL_LEN 10
#define APP_VERSION "0.1.0"
// #define QR_PARSE_STR ""

//#define FILE_SOURCE_ENABLE

//#define WIFI_ENABLE
//#define QR_PARSE_ENABLE
#endif

#ifdef FILE_SOURCE_ENABLE
#define VIDEO_SOURCE_FILE "/tmp/meeting.mp4"
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE( x ) (sizeof(x)/sizeof((x)[0]))
#endif

#ifndef ARRAY_GET_LAST
#define ARRAY_GET_LAST( x ) (x)[(sizeof(x)/sizeof((x)[0])) - 1]
#endif

// DVR上的最大摄像机数量。如果要模拟摄影机，请设置为1。
#define DVR_CAMERAS_MAX 1

// 每个目标的最大配置文件数
#define DVR_PROFILES_MAX 3
#define PROFILE_HIGH 0
#define PROFILE_MID  1
#define PROFILE_LOW  2


//#ifdef QR_PARSE_ENABLE
//extern char g_parseStr[QR_PARSE_STRLEN];
//extern char g_ssidStr[WIFI_SSID_LEN + 1];
//extern char g_passwdStr[WIFI_PASSWD_LEN + 1];
//#endif

//DWORD_X bPlayingCryMusic;

#define MOVIE_DURATION_MS (6000)
OSI_SEM g_liveVideoSem;
OSI_SEM g_liveVideoSubSem;
OSI_SEM g_liveAudioSem;
//extern LUX_CONFIG_DATA_ST g_configData_st;
static LUX_CONFIG_DATA_ST jio_cfg;
const char* const target = "camera0";
#if DVR_CAMERAS_MAX > 1
const char* const target1 = "camera1";
#endif


#ifdef WIFI_ENABLE
extern BOOL_X wifiFlag;
#endif


#define TAG_IVS_DETECTION
#define TAG "demo_main"

// 每个目标功能的初始化/设置常量
//------------------------dp------------------------
enum {
    DVR_TARGET_HAS_LIVE_STREAM = (1 << 0),
    DVR_TARGET_HAS_SNAPHOT = (1 << 1),
    DVR_TARGET_HAS_ARCHIVE = (1 << 2),
    DVR_TARGET_HAS_P2T = (1 << 3),
    DVR_TARGET_HAS_CONFIG = (1 << 4),
    DVR_TARGET_HAS_PTZ = (1 << 5),
};

idip_server_t* m_idip_server;
// Options for demo
typedef struct demo_options_t {
    // 模拟实时视频流的文件路径
    char* live_video_file;
    char* live_mid_file;
    char* live_low_file;

    //包含“存档”记录的文件的路径。
    //它是纯文本文件，其中每行描述一个“归档记录”：
    //“start_ms duration_ms/full/path/to/file.mp4”
    char* archive_list_path;

    //预览图像的路径
    char* snapshot_path;

    //永久存储文件的路径
    char* persistent_storage;
#if defined(USE_ALSA_BACKEND)
    // playback implementation
    //回放实现
    playback_t* playback;
#endif /* USE_ALSA_BACKEND */

    //实时视频循环标志
    int enable_live_loop;

    //假事件启用标志
    int enable_events;

    //假事件时间表：
    //
    // |       |M.S.A.M.S|           |A.M.S.A.M|           | ...
    // |--(1)--|---(2)---|----(3)----|---(2)---|----(3)----| ...
    //
    // [M]otion, [S]ound, [A]larm, [.] = 1 sec
    //
    int first_event_delay_sec; // (1) 第一个事件延时时间
    int continuous_events_duration_sec; // (2) 连续事件持续时间
    int no_events_duraction_sec; // (3) 无事件持续时间


    //温度“快照”图像的路径
    char* ev_thermal_image_path;

    //不要打开端口进行监听。该选项对于无代理集成很有用。
    int disable_listen;

    //Web套接字地址和（可选）端口。
    //idip_server将端口80用于http连接，443用于https连接。
    char* ws_url;

    //Web套接字开发密钥（请参阅手册）
    char* ws_key;

    //一个数组，用于保存指向demo流的指针
    avpkt_stream_source_t* live_source_streams[DVR_CAMERAS_MAX][DVR_PROFILES_MAX];

    //上面数组的实际长度
    size_t                 live_source_streams_size;

    //从命令行覆盖mac地址。格式XX:XX:XX:XX:XX:%XX:XX。
    char* mac_override;

    //从命令行解析的mac地址
    uint8_t mac[6];

    //解析的二维码数据文件。用于测试目的。
    char* qr_file;
} demo_options_t;

static demo_options_t demo_options = {
#ifdef FILE_SOURCE_ENABLE
    .live_video_file = VIDEO_SOURCE_FILE,
#else
    .live_video_file = NULL,
#endif
    .live_low_file = NULL,
    .live_mid_file = NULL,
    .archive_list_path = NULL,
    .snapshot_path = NULL,
    .persistent_storage = PERSIET_PATH,
#if defined(USE_ALSA_BACKEND)
    .playback = NULL,
#endif /* USE_ALSA_BACKEND */
    .enable_live_loop = 1,
    .enable_events = 1,
    .first_event_delay_sec = 15,
    .continuous_events_duration_sec = 1,
    .no_events_duraction_sec = 60,
    .ev_thermal_image_path = "",
    .live_source_streams = {{0}},
    .live_source_streams_size = 0,
    .mac_override = NULL,
    .mac = {0},
    .qr_file = QR_PARSE_FILE,
};

idip_parsed_token_t token = { 0 };

static void* ota_upgrade_thread(void* args);
void ivs_event_cb(CHAR_X* snap_buffer, UINT_X snap_size, LUX_EVENT_NOTIFI_CONTENT_EN type);
//----- 具体实现的声明 ----------------------------------------------------------------

static void* live_streamer_thread(void* arg_);
static void* thread_live_audio(void* arg);

static idip_status_code_t
get_interfaces_handler(idip_stream_system_get_interfaces_t* stream, void* udata);

static idip_status_code_t
snapshot_handler_impl(idip_stream_snapshot_t* stream,
                      const idip_stream_snapshot_args_t* args,
                      void* udata);

static void
av_source_event_handler_impl(idip_av_source_event_t event,
                             const idip_av_source_t* source,
                             const void* event_data,
                             void* udata);

static idip_status_code_t
archive_handler_impl(idip_stream_archive_t* stream,
                     const char* target,
                     const idip_archive_play_params_t* params,
                     void* udata);

static idip_status_code_t
archive_list_handler_impl(idip_stream_archive_list_t* stream,
                          const char* target,
                          const archive_interval_t* params,
                          void* udata);

static idip_status_code_t
archive_days_handler_impl(idip_stream_archive_days_t* stream,
                          const char* target,
                          const archive_interval_t* params,
                          void* udata);

#if defined(USE_ALSA_BACKEND)
static idip_status_code_t
playback_handler_impl(const char* target,
                      const audio_stream_info_t* info,
                      const void* data,
                      size_t size,
                      void* udata);
#endif /* USE_ALSA_BACKEND */

static idip_status_code_t
config_get_handler_impl(idip_stream_config_t* stream,
                        const char* target,
                        void* udata);

static idip_status_code_t
config_update_handler_impl(idip_stream_config_t* stream,
                           const char* target,
                           const idip_cfg_param_t** params, size_t params_count,
                           void* udata);

static idip_status_code_t
ptz_handler_impl(const char* target, const idip_ptz_command_t* cmd, void* udata);

static idip_status_code_t
persistent_data_get_impl(idip_stream_persistent_data_t* stream,
                         void* udata);

static idip_status_code_t
persistent_data_set_impl(const void* data, size_t size, void* udata);
#if 1//cxj test
static idip_status_code_t
wireless_scan_impl(idip_stream_wireless_scan_t* stream, void* udata);

static idip_status_code_t
wireless_setup_impl(const idip_ap_connect_parameters_t* settings, void* udata);
#endif
static idip_status_code_t
system_notify_impl(const idip_system_notify_query_args_t* args, void* udata);

static idip_status_code_t
system_update_impl(const idip_system_update_query_args_t* args, void* udata);

static idip_status_code_t
system_passwd(const idip_user_credentials_t* cred, void* udata);

static idip_status_code_t
system_get_users(idip_stream_system_get_users_t* stream, void* udata);

static idip_status_code_t
system_token_handler(idip_stream_system_token_t* stream, void* user_data);

// helper functions
static int setup_notifiers(void);
static void initialize_config(void);

static idip_status_code_t read_sn_file(char* outSn, int len);
static idip_status_code_t check_account();
static idip_status_code_t check_sock();
static idip_status_code_t setup_connection();

static idip_status_code_t read_token_file(const char* path, idip_parsed_token_t* token);


void ivs_event_cb(CHAR_X* snap_buffer, UINT_X snap_size, LUX_EVENT_NOTIFI_CONTENT_EN type)
{
    printf("!!!!!!!!!detect move!!!!!!!!!!\n");
    idip_server_send_vca_crowd_detection_event(m_idip_server, "camera0", time_ms(), 1);
}

static idip_status_code_t read_token_file(const char* path, idip_parsed_token_t* token)
{
    const size_t kMaxTokenFileSize = 512;
    char json[kMaxTokenFileSize];
    // reading a file with json, parse the json array and put the result to the
    // reply stream.
    printf("read_token_file\n");
    memset(json, 0, sizeof(json));
    FILE* in = fopen(QR_PARSE_FILE, "r");
    if (!in) {
        idip_log_warning("Can't open QR data file %s: %s", QR_PARSE_FILE, strerror(errno));
        return IDIP_GENERIC_ERROR;
    }

    size_t json_len = fread(json, 1, kMaxTokenFileSize, in);
    printf("read len:%d \n", json_len);
    fclose(in);

    int rc = idip_parse_cloud_qr_code_token_json(json, json_len, token);
    printf("read_token_file ssid:%s pw:%s\n", token->ssid, token->passwd);
    return rc == 0 ? IDIP_OK : IDIP_GENERIC_ERROR;
}

//检查WIFI连接
static idip_status_code_t setup_connection(idip_parsed_token_t* token)
{
    char ssid[128] = { 0 };
    if (lux_wf_get_ssid(ssid) == 0)
    {
        if (strncmp(ssid, token->ssid, strlen(ssid)) == 0)
        {
            //已经连接指定wifi
            if (lux_wf_check_connect() == 0)
            {
                printf("wifi already connected!\n");
                return IDIP_OK;
            }
        }
    }
    printf("cur wifi ssid: %s", ssid);
    //查找QR文件
    if (0 == lux_wf_connect(token->ssid, token->passwd))
    {
        printf("setup connection OK\n");
        return IDIP_OK;
    }
    printf("setup connection error\n");
    return IDIP_GENERIC_ERROR;
}

//检查是否有账号信息
static idip_status_code_t check_account()
{
    FILE* fp = fopen(PERSIET_PATH, "r");
    if (!fp)
    {
        printf("Can't open persistent_storage file %s: %s", PERSIET_PATH, strerror(errno));
        fp = fopen(PERSIET_PATH, "w");
        fclose(fp);
        usleep(20 * 1000);
        chmod(PERSIET_PATH, S_IRUSR |
                            S_IWUSR |
                            S_IRGRP |
                            S_IWGRP |
                            S_IROTH |
                            S_IWOTH);
        return IDIP_GENERIC_ERROR;
    }
    char line[16] = { 0 };
    size_t rlen = fread(line, 1, 16, fp);
    fclose(fp);
    if (rlen > 0 && strstr(line, "email"))
    {
        printf("check_account file ok\n ");
        return IDIP_OK;
    }
    return IDIP_GENERIC_ERROR;
}

//检查是否有idip.sock文件
static idip_status_code_t check_sock()
{
    if (0 != access(HOST_PATH, F_OK))
    {
        return IDIP_GENERIC_ERROR;
    }
    chmod(HOST_PATH, S_IRUSR |
                        S_IWUSR |
                        S_IXUSR |
                        S_IRGRP |
                        S_IWGRP |
                        S_IXGRP |
                        S_IROTH |
                        S_IWOTH |
                        S_IXOTH);
    return IDIP_OK;
}

static idip_status_code_t read_sn_file(char* outSn, int len)
{
    FILE* in = fopen(SERIAL_PATH, "r");
    if (!in) {
        idip_log_warning("Can't open serial data file %s: %s", SERIAL_PATH, strerror(errno));
        return IDIP_GENERIC_ERROR;
    }
    size_t rlen = fread(outSn, 1, len, in);
    fclose(in);
    printf("get sn: %s\n", outSn);
    return rlen == len ? IDIP_OK : IDIP_GENERIC_ERROR;
}
#if 0
static void usage(const char* program_name);
static void version(void);
#endif
static void app_stop_notify(void);

//----- static configuration ---------------------------------------------------
// Global server's configuration.
// static char g_serial_str[50] = { 0 };
static idip_server_conf_t server_config = {
#if 0 /* TCP connection */
    /* set host_or_path to "*" or "0.0.0.0" if you need to use INADDR_ANY */
  .host_or_path = "*",
  .port = 55500,
#else /* Unix Domain socket connection. Port always 0 */
    .host_or_path = NULL,
    .port = 0,
#endif
    .enable_local_listener = 0,
    .keep_alive_ms = IDIP_KEEP_ALIVE_AUTO,
    .disable_ipv6 = 0, // Set to 1 in order to disable IPv6
    .cloud_ssl_ca_filepath = CERT_PATH,
    .vendor = "Luxshare",
    .model = "Demo 18K XXHD",
    .serial_number = NULL,
    .firmware_version = APP_DETAIL_VERSION"("BUILD_DATE")",
    .cloud_enpoint_url = URL,
    .dev_key = KEY,

};


//----- globals ----------------------------------------------------------------
static int pipefds[2] = { -1, -1 };
static int timerfd = -1;
static pthread_mutex_t cfg_mutex = PTHREAD_MUTEX_INITIALIZER;

static inline void syscall_print_error(const char* fn_name)
{
    // printf("%s(): %s\n", fn_name, strerror(errno));
    printf("%s - %s() - [%d] : %s\n", __FILE__, fn_name, __LINE__, strerror(errno));//cxj test
}

static void app_stop_notify(void)
{
    char b = 0;
    (void)write(pipefds[1], &b, sizeof(b));
}
//------------------------------------------------------------------------------
static int
parse_mac_address(const char* macstr, uint8_t* mac)
{
    assert(macstr);
    assert(mac);
    if (!macstr || !mac)
        return -1;

    unsigned int mac_bytes[6];
    const int kSuccessScannedItems = 6;
    int rc = sscanf(macstr, "%x:%x:%x:%x:%x:%x",
                    &mac_bytes[0], &mac_bytes[1], &mac_bytes[2],
                    &mac_bytes[3], &mac_bytes[4], &mac_bytes[5]);
    if (rc != kSuccessScannedItems) {
        printf("Invalid mac address format!\n");
        return -1;
    }
    mac[0] = mac_bytes[0] & 0xFF;
    mac[1] = mac_bytes[1] & 0xFF;
    mac[2] = mac_bytes[2] & 0xFF;
    mac[3] = mac_bytes[3] & 0xFF;
    mac[4] = mac_bytes[4] & 0xFF;
    mac[5] = mac_bytes[5] & 0xFF;
    return 0;
}
//------------------------------------------------------------------------------
static int
parse_wlan0_ip_address(const char* wlan0IpAddrStr, unsigned int* wlan0IpAddr)
{
    if (!wlan0IpAddrStr || !wlan0IpAddr)
        return -1;

    const int kSuccessScannedItems = 4;
    int rc = sscanf(wlan0IpAddrStr, "%u.%u.%u.%u",
                    &wlan0IpAddr[0], &wlan0IpAddr[1], &wlan0IpAddr[2], &wlan0IpAddr[3]);
    //printf("\n************ get ip success: %u.%u.%u.%u ******************\n", \
    //                wlan0IpAddr[0], wlan0IpAddr[1], wlan0IpAddr[2], wlan0IpAddr[3]);
    if (rc != kSuccessScannedItems) {
        printf("Invalid wlan0 ip address format!\n");
        return -1;
    }
    return 0;
}
//------------------------------------------------------------------------------
int parse_wlan0_mask(const char* maskStr, unsigned int* maskLen)
{
    unsigned int mask[4];
    unsigned int len = 0;
    unsigned int i = 0;
    memset(mask, 0, 4*sizeof(int));
    sscanf(maskStr, "%u.%u.%u.%u", &mask[0], &mask[1], &mask[2], &mask[3]);

    for (i = 0;i < 4;i++)
    {
        //printf("%u ", mask[i]);
        switch (mask[i])
        {
        case 255: len += 8;break;
        case 254: len += 7;break;
        case 252: len += 6;break;
        case 248: len += 5;break;
        case 240: len += 4;break;
        case 224: len += 3;break;
        case 192: len += 2;break;
        case 128: len++;break;
        }
    }
    //printf("\n");
    *maskLen = len;
    return 0;
}


//------------------------------------------------------------------------------
// extern BOOL_X lux_wf_check_connect();
extern int lux_wf_get_mac_ip_mask(char* macStr, char* ipStr, char* maskStr);
extern int lux_wf_scan_all(const char* real_ssid, idip_ap_security_type_t* auth_type);
pthread_t qrcode_thId; // ID of capture thread
// 这些句柄是libidip必需的。它返回接口信息。
static idip_status_code_t
get_interfaces_handler(idip_stream_system_get_interfaces_t* stream, void* udata)
{
    //const demo_options_t* opts = udata;
    //在演示中，我们只返回一个简单的假数据。---这里我们直接使用，只有wlan0传入真实数据
    //真正的实现应该返回实际的接口数据。
    //注意！列表中的第一个接口是“激活”的：它被设备用于连接云。
    //注意！设置attach_iface标志为需要用于连接云过程的接口。只有一个
    //intherface可以使用该标志。libidip将使用由标志标记的第一个接口的mac地址。
    // 注意！不要在生产中使用这些mac地址！
    // uint8_t mac[6] = { 0x00, 0x0c, 0x29, 0xd1, 0x4c, 0xe6 }; // "01:02:03:04:05:06",
    // uint8_t mac[6] = { 0x40, 0x24, 0xB2, 0xA1, 0x05, 0xF4 };
    // if (opts->mac_override) {
    //     memcpy(&mac, opts->mac, sizeof(mac));
    // }
    uint8_t mac[6];
    int rc = IDIP_OK;
    char macStr[18];
    char ipStr[16];
    char maskStr[16];
    idip_ipv4_addr_t wlan0_addr;// = { {10, 110, 4, 176}, 24 };  // 192.168.0.100/24
    memset(macStr, 0, sizeof(macStr));
    memset(ipStr, 0, sizeof(ipStr));
    memset(maskStr, 0, sizeof(maskStr));
    //while (1)
    {
        //if (wifiFlag)
        {
            rc = lux_wf_get_mac_ip_mask(macStr, ipStr, maskStr);
            if (0 != rc)
            {
                syscall_print_error(__func__);
                return IDIP_INVALID_ARGUMENTS;
            }
            memset(mac, 0, sizeof(mac));
            rc = parse_mac_address(macStr, mac);
            if (0 != rc)
            {
                syscall_print_error(__func__);
                return IDIP_INVALID_ARGUMENTS;
            }
            memset(&wlan0_addr, 0, sizeof(wlan0_addr));
            unsigned int ipAddr[4];
            memset(&ipAddr, 0, sizeof(ipAddr));
            rc = parse_wlan0_ip_address(ipStr, ipAddr);
            if (0 != rc)
            {
                syscall_print_error(__func__);
                return IDIP_INVALID_ARGUMENTS;
            }
            int i;
            for (i = 0; i < 4; i++)
            {
                wlan0_addr.addr[i] = ipAddr[i];
            }
            rc = parse_wlan0_mask(maskStr, (unsigned int*)&wlan0_addr.mask_length);
            if (0 != rc)
            {
                syscall_print_error(__func__);
                return IDIP_INVALID_ARGUMENTS;
            }
            //break;
        }
        /*else
        {
            printf("\n****************wifi connecting. wait ***********\n");
            sleep(1);
        }*/
    }
#if 0//cxj test
    //例如，所有地址都在这里
    const idip_ipv4_addr_t eth0_addr = { {192, 168, 34, 56}, 24 };    // 192.168.0.20/24
    const idip_ipv4_addr_t eth0_alt_addr = { {172, 17, 1, 50}, 16 }; // 172.17.1.50/16
    // const idip_ipv4_addr_t wlan0_addr = { {10, 110, 4, 176}, 24 };  // 192.168.0.100/24
    const idip_ipv6_addr_t eth0_addr6 = { // 2001:db8:85a3:8d3:1319:8a2e:370:7348/24
        {0xfe80, 0x43f6, 0x4a7d, 0xeefc, 0x49d1}, 64
    };

    const idip_iface_t fake_ifaces[2] = {
        (idip_iface_t) {
            .type = IDIP_IFACE_WIRED,
            .ifname = "eth0",
            .attach_iface = 1,
            .mac_addr = { mac[0], mac[1], mac[2], mac[3], mac[4], mac[5] },
            .ipv4_addr = { eth0_addr, eth0_alt_addr },
            .ipv4_addr_count = 2,
            .ipv6_addr = { eth0_addr6 },
            .ipv6_addr_count = 1,
        },
        (idip_iface_t) {
            .type = IDIP_IFACE_WIRELESS,
            .ifname = WLAN_DEV,
            .mac_addr = { mac[0], mac[1], mac[2], mac[3], mac[4], mac[5] },
            .ipv4_addr = { wlan0_addr },
            .ipv4_addr_count = 1,
            .ipv6_addr = {{}},
            .ipv6_addr_count = 0,
            .ssid = NULL,
            .auth_type = AP_SEC_TYPE_WPA2,
        },
    };
    memset(fake_ifaces[1].ssid, 0, sizeof(fake_ifaces[1].ssid));
    strcpy(fake_ifaces[1].ssid, g_ssidStr);
    printf("\n**************************** wlan0 ssid: %s ****************************\n", fake_ifaces[1].ssid);

#ifdef QR_PARSE_ENABLE
    QR_Thread_Exit();
    pthread_join(qrcode_thId, NULL);
#endif
    return idip_stream_system_get_interfaces_put_iface_data(stream, fake_ifaces, 2,
                                                            IDIP_INFINITE_TIMEOUT);
#endif
    idip_iface_t real_ifaces = {
        .type = IDIP_IFACE_WIRELESS,
        .ifname = WLAN_DEV,
        .attach_iface = 1,
        .mac_addr = {{ mac[0], mac[1], mac[2], mac[3], mac[4], mac[5] }},
        .ipv4_addr = { wlan0_addr },
        .ipv4_addr_count = 1,
        .ipv6_addr = {{}},
        .ipv6_addr_count = 0,
        .ssid = NULL,
        .auth_type = AP_SEC_TYPE_WPA2,
    };
    real_ifaces.ssid = token.ssid;

    //rc = lux_wf_scan_all(real_ifaces.ssid, &(real_ifaces.auth_type));
    //if (0 != rc)
    //    return IDIP_GENERIC_ERROR;
    //printf("\n**************************** wlan0 ssid: %s  auth_type: %d****************************\n", real_ifaces.ssid, real_ifaces.auth_type);

    //int i;
    //for (i = 0; i < 6; i++)
    //{
    //    printf("mac[%d]=%X\t", i, mac[i]);
    //}
    //printf("\n");

#ifdef QR_PARSE_ENABLE
    //QR_Thread_Exit();
    //pthread_join(qrcode_thId, NULL);
#endif
    return idip_stream_system_get_interfaces_put_iface_data(stream, &real_ifaces, 1,
                                                            IDIP_INFINITE_TIMEOUT);
}

//------------------------------------------fts define------------------------------------------
///按目标功能集设置。
///为了简单起见，我们将相同的@a user_data传递给所有句柄。
///如果需要，您的解决方案可以将不同的数据指针传递给句柄。
static int
setup_features_per_target(const char* target,
                          int feature_set_mask,
                          idip_server_t* server,
                          void* user_data)
{
    assert(target);
    assert(server);

    int rc = IDIP_OK;
    //2.安装程序明确支持的功能。
    //------------------------------------------fts callback------------------------------------------

    // if ((feature_set_mask & DVR_TARGET_HAS_ARCHIVE) &&
    //     (NULL != demo_options.archive_list_path) &&
    //     (0 == access(demo_options.archive_list_path, R_OK)))
    if ((feature_set_mask & DVR_TARGET_HAS_ARCHIVE) &&
        (NULL != target))
    {
        archive_play_speed_t supported_archive_speed_factors[] = {
            ARCHIVE_PLAY_SPEED_1X
        };//支持的存档回放速度

        // rc = idip_server_setup_archive_feature(server, target,
        //         archive_handler_impl, user_data,
        //         archive_list_handler_impl, user_data,
        //         archive_days_handler_impl, user_data,
        //         (idip_archive_capabilities_t) {
        //     .speed_factors = supported_archive_speed_factors,
        //         .speed_factors_count = ARRAY_SIZE(supported_archive_speed_factors)
        // });
        rc = idip_server_setup_archive_feature(server, target,
                archive_handler_impl, &demo_options.persistent_storage,
                archive_list_handler_impl, NULL,
                archive_days_handler_impl, NULL,
                (idip_archive_capabilities_t) {
            .speed_factors = supported_archive_speed_factors,
                .speed_factors_count = ARRAY_SIZE(supported_archive_speed_factors)
        });
        if (0 != rc) {
            return rc;
        }
    }
    printf("\n********************* DVR_TARGET_HAS_ARCHIVE is success *********************\n");


    if ((feature_set_mask & DVR_TARGET_HAS_SNAPHOT) &&
        demo_options.snapshot_path)
    {
        rc = idip_server_setup_snapshot_feature(server, target,
                                                snapshot_handler_impl,
                                                user_data);
        if (0 != rc) {
            return rc;
        }
    }

    printf("\n********************* DVR_TARGET_HAS_SNAPHOT is success *********************\n");

#if defined(USE_ALSA_BACKEND)
    if (feature_set_mask & DVR_TARGET_HAS_P2T) {
        audio_codec_t supported_p2t_acodecs[] = {
            AUDIO_CODEC_PCM16, AUDIO_CODEC_PCMA, AUDIO_CODEC_PCMU
        };

        rc = idip_server_setup_p2t_feature(server, target,
                                           playback_handler_impl, user_data,
                                           (idip_p2t_playback_capabilities_t) {
            .acodecs = supported_p2t_acodecs,
                .acodecs_size = ARRAY_SIZE(supported_p2t_acodecs)
        });
        if (0 != rc) {
            return rc;
        }
    }
    printf("\n********************* DVR_TARGET_HAS_P2T is success *********************\n");
#endif /* USE_ALSA_BACKEND */

    if (feature_set_mask & DVR_TARGET_HAS_CONFIG)
    {
        rc = idip_server_setup_config_feature(server, target,
            config_get_handler_impl, user_data,
            config_update_handler_impl, user_data);
        if (0 != rc) {
            return rc;
        }
    }
    printf("\n********************* DVR_TARGET_HAS_CONFIG is success *********************\n");


    if (feature_set_mask & DVR_TARGET_HAS_PTZ) {
        rc = idip_server_setup_ptz_feature(server, target,
            ptz_handler_impl, NULL,
            (idip_ptz_capabilities_t) {
            .commands = (ptz_command_type_t[]){
                IDIP_PTZ_HOME, IDIP_PTZ_ZOOM_IN, IDIP_PTZ_ZOOM_OUT,
                IDIP_PTZ_LEFT, IDIP_PTZ_RIGHT,   IDIP_PTZ_UP,
                IDIP_PTZ_DOWN },
                .commands_count = 7
        });
        if (0 != rc) {
            return rc;
        }
    }
    printf("\n********************* DVR_TARGET_HAS_PTZ is success *********************\n");
    return 0;
}

// /这些功能对于摄像机/DVR是常见的，并且不绑定到目标。帮助设置摄像机范围/DVR范围处理程序的包装器。
static inline int
setup_features_nontarget(idip_server_t* server)
{
    int rc = 0;
    rc = idip_server_setup_wireless_feature(server,
                                            wireless_scan_impl, NULL,
                                            wireless_setup_impl, NULL);
    if (0 != rc)
        return rc;
    // rc = lux_wf_scan_all();
    // if (0 != rc)
    //     return rc;

    // 设置系统处理程序以获取接口信息。必需的。
    // rc = idip_server_setup_get_interfaces_feature(server, get_interfaces_handler, &demo_options);
    rc = idip_server_setup_get_interfaces_feature(server, get_interfaces_handler, NULL);
    if (0 != rc)
        return rc;

    //设置系统处理程序以获取接口信息。必需的。
    rc = idip_server_setup_persistent_storage_feature(server,
                                                      persistent_data_get_impl,
                                                      &demo_options,
                                                      persistent_data_set_impl,
                                                      &demo_options,
                                                      1024ul * 1024ul);
    if (0 != rc)
        return rc;



    rc = idip_server_setup_system_notify_handler(server, system_notify_impl, NULL);
    if (0 != rc)
        return rc;

    rc = idip_server_setup_update_handler(server, system_update_impl, NULL);
    if (0 != rc)
        return rc;

    rc = idip_server_setup_account_management_handlers(server,
                                                       system_passwd, NULL,
                                                       system_get_users, NULL);
    if (0 != rc)
        return rc;

#if 0
    //将路径传递给--qr-code选项时启用qr-code仿真
    if (demo_options.qr_file) {
        rc = idip_server_setup_token_handler(server, system_token_handler, &demo_options);
        if (0 != rc)
            return rc;
}
#endif

    return 0;
}

//创建和配置idip服务器实例的通用方法。
static idip_server_t*
create_and_start_idip_server(const idip_server_conf_t* config, void* user_data)
{
    int rc = 0;
    //1.创建服务器实例。
    idip_server_t* server = idip_server_new(config);
    if (!server) {
        return NULL;
    }
    printf("idip_server_new is success \n");

    //检查sock文件是否存在，并修改权限
    rc = check_sock();
    if (rc != IDIP_OK)
    {
        MYERRORING("no idip.sock file! \n");
        return NULL;
    }

    idip_server_set_app_stop_notify(server, app_stop_notify);
    printf("idip_server_set_app_stop_notify is success\n");


    // 2. 安装程序明确支持的功能。
#if 1
    // rc = setup_features_per_target(target,
    rc = setup_features_per_target(target,
                                   DVR_TARGET_HAS_LIVE_STREAM |
                                   DVR_TARGET_HAS_SNAPHOT |
                                   DVR_TARGET_HAS_ARCHIVE |
                                   DVR_TARGET_HAS_P2T |
                                   DVR_TARGET_HAS_CONFIG,
                                   server, user_data);
    if (0 != rc)
        goto fail;
    printf("setup_features_per_target is success\n");
#if DVR_CAMERAS_MAX > 1
    rc = setup_features_per_target(demo_target1,
                                   DVR_TARGET_HAS_LIVE_STREAM |
                                   DVR_TARGET_HAS_SNAPHOT |
                                   DVR_TARGET_HAS_ARCHIVE |
                                   DVR_TARGET_HAS_PTZ,
                                   server, user_data);
    if (0 != rc)
        goto fail;
    printf("\n********************* setup_features_per_target1 is success *********************\n");

#endif
#endif
    rc = setup_features_nontarget(server);
    if (0 != rc)
        goto fail;
    printf("setup_features_nontarget is success \n");

    //3.运行服务器
    if (IDIP_OK != idip_server_run(server)) {
        goto fail;
    }
    printf("idip_server_run is success \n");
    return server;



fail:
    // Notify server instance about stop.
    // Function blocks main thread until server instance will not done.
    idip_server_stop(server);
    return NULL;

}

///初始化演示的所有流源。当任何源无法正确初始化时，其他源将被销毁。
///如果成功，@return 0，否则-1

static int
initialize_demo_sources(avpkt_stream_source_t* demo_sources[][DVR_PROFILES_MAX],
                        size_t demo_sources_size,
                        demo_options_t* demo_options)
{
#if 0
    demo_options->live_source_streams_size = 0;
    const char* live[DVR_PROFILES_MAX] = {
        demo_options->live_video_file,
        demo_options->live_mid_file,
        demo_options->live_low_file,
    };
    const bool enabled = demo_options->enable_live_loop;
    size_t i = 0, j = 0;;
    for (i = 0; i < demo_sources_size; ++i) {
        for (j = 0; j < DVR_PROFILES_MAX; ++j) {
            if (!live[j]) // skip sources without file //跳过没有文件的源
                continue;
            avpkt_stream_source_t* new_src = IDIP_NEW(*new_src);
            if (!new_src)
                goto init_failed;
            int err = avpkt_stream_source_init(new_src, live[j], enabled, true);
            if (err) {
                free(new_src);
                goto init_failed;
            }
            demo_sources[i][j] = new_src;
            demo_options->live_source_streams[i][j] = demo_sources[i][j];
        }
        demo_options->live_source_streams_size = i + 1;
    }

    return 0;


init_failed:
    for (i = 0; i < demo_sources_size; ++i) {
        for (j = 0; j < DVR_PROFILES_MAX; ++j) {
            avpkt_stream_source_t* src = demo_sources[i][j];
            if (src) {
                avpkt_stream_source_destroy(src);
                free(src);
            }
            demo_sources[i][j] = NULL;
        }
    }
#endif

    return -1;
}

//------------------------------------------------------------------------------
#ifdef FILE_SOURCE_ENABLE
static int
create_register_and_bind_av_source(idip_server_t* server,
                              avpkt_stream_source_t* stream_source,
                              idip_av_source_params_t* av_source_params,
                              idip_av_source_t** p_av_source)
{
    *p_av_source = NULL;
    idip_av_source_t* new_av_source = NULL;

    int err = idip_server_av_source_register(server, av_source_params, &new_av_source);
    if (err)
        return -1;
    err = avpkt_stream_source_set(stream_source, server, new_av_source);
    if (err) {
        idip_server_av_source_unregister(server, &new_av_source);
        return -1;
    }

    *p_av_source = new_av_source;
    return 0;
}
#endif

//------------------------------------------------------------------------------
#if 0
static void parse_argv(int argc, char** argv)
{
    int c = 0;
#if 0//cxj test
    if (argc < 2) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
#endif
    while (1) {
        int option_index = 0;
        static struct option long_options[] = {
           {"live",                 required_argument, 0, 'l' },                            // [0]
           {"live-mid",             required_argument, 0, 0 },                              // [1]
           {"live-low",             required_argument, 0, 0 },                              // [2]
           {"archive",              required_argument, 0, 'a' },                            // [3]
           {"snapshot",             required_argument, 0, 's' },                            // [4]
           {"persistent-storage",   required_argument, 0, 'p' },                            // [5]
           {"disable-live-loop",    no_argument,       &demo_options.enable_live_loop,  0 },// [6]
           {"disable-events",       no_argument,       &demo_options.enable_events,     0 },// [7]
           {"thermal-image",        required_argument, 0, 0 },                              // [8]
           {"port",                 required_argument, 0, 'P' },                            // [9]
           {"iface",                required_argument, 0, 'I' },                            // [10]
           {"ws-url",               required_argument, 0, 0 },                              // [11]
           {"dev-key",              required_argument, 0, 0 },                              // [12]
           {"sn",                   required_argument, 0, 0 },                              // [13]
           {"mac",                  required_argument, 0, 0 },                              // [14]
           {"qr-code",              required_argument, 0, 0 },                              // [15]
           {"events-schedule",      required_argument, 0, 0 },                              // [16]
           {"disable-listen",       no_argument,       &demo_options.disable_listen, 0 },   // [17]
           {"version",              no_argument,       0, 'v' },                            // [18]
           {"help",                 no_argument,       0, 'h' },                            // [19]
           {0,                      0,                 0,  0  },
        };

        c = getopt_long(argc, argv, "a:l:s:p:P:A:vh",
                        long_options, &option_index);
        if (c == -1)
            break;
        switch (c) {
        case 0: {
            if (1 == option_index) { // --live-mid
                demo_options.live_mid_file = optarg;//optarg在#include <getopt.h>中已包含extern char* optarg;
            }
            else if (2 == option_index) { // --live-low
                demo_options.live_low_file = optarg;
            }
            else if (8 == option_index) { // --thermal-image
                demo_options.ev_thermal_image_path = optarg;
            }
            else if (11 == option_index) { // --ws-url
                server_config.cloud_enpoint_url = optarg;
            }
            else if (12 == option_index) { // --dev-key
                server_config.dev_key = optarg;
            }
            else if (13 == option_index) { // --sn
                server_config.serial_number = optarg;
            }
            else if (14 == option_index) { // --mac
                if (0 != parse_mac_address(optarg, demo_options.mac)) {
                    usage(argv[0]);
                    exit(EXIT_FAILURE);
                }
                demo_options.mac_override = optarg;
            }
            else if (15 == option_index) { // --qr-code
                demo_options.qr_file = optarg;
            }
            else if (16 == option_index) { // --events-schedule
                int c = sscanf(optarg,
                               "%d/%d/%d",
                               &demo_options.first_event_delay_sec,
                               &demo_options.continuous_events_duration_sec,
                               &demo_options.no_events_duraction_sec);
                if (c != 3 || demo_options.first_event_delay_sec < 0 ||
                        demo_options.continuous_events_duration_sec < 1 ||
                        demo_options.no_events_duraction_sec < 0) {
                    fprintf(stderr, "Error: invalid event schedule: %s\n\n", optarg);
                    usage(argv[0]);
                    exit(EXIT_FAILURE);
                }
            }
            break; // long option
        }
        case 'l': demo_options.live_video_file = optarg; break;
        case 'a': demo_options.archive_list_path = optarg;  break;
        case 's': demo_options.snapshot_path = optarg; break;
        case 'p': demo_options.persistent_storage = optarg; break;
        case 'P': {
            char* pend = NULL;
            unsigned long port = strtoul(optarg, &pend, 10);
            if ((errno != ERANGE) && (port <= USHRT_MAX)) {
                server_config.port = (int)port;
            }
            else {
                fprintf(stderr, "Error: invalid port number: %s\n\n", optarg);
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            break;
        }
        case 'I': server_config.host_or_path = optarg; break;
        case 'v':
            version();
            exit(EXIT_SUCCESS);
            break;
        case 'h':
            usage(argv[0]);
            exit(EXIT_SUCCESS);
            break;
        case '?':
        default:
            usage(argv[0]);
            exit(EXIT_FAILURE);
            break;
        }
    }
    if (optind < argc) {
        demo_options.live_video_file = argv[optind];
    }

    if (!demo_options.live_video_file) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    if (!demo_options.persistent_storage) {
        printf("[WARN] Persistent storage file path wasn't specified.\n");
        usage(argv[0]);
    }
}
#endif
//----- main -------------------------------------------------------------------
//----- mn ---------------------------------------------------------------------
extern void lux_qrcode_parse();
extern void* thread_qrcode(void* arg);

static void* ota_upgrade_thread(void* args)
{
    char cmdBuf[256] = { 0 };
    snprintf(cmdBuf, sizeof(cmdBuf), "/system/bin/busybox wget --no-check-certificate -O /tmp/ota.zip %s", (char*)args);
    printf("OTA upgrading...\n");
    LUX_BASE_System_CMD(cmdBuf);
    LUX_BASE_System_CMD("unzip -o /tmp/ota.zip -d /tmp;chmod 775 -R /tmp/ota;sh /tmp/ota/ota.sh");
    printf("ota upgrade successfully! Now reboot.\n");
    usleep(500 * 1000);
    LUX_Event_Reboot();
    return (void*)0;
}


//----- implementations --------------------------------------------------------
//----- ipl define -------------------------------------------------------------

typedef enum
{
    E_CHANNEL_VIDEO_MAIN = 0,
    E_CHANNEL_VIDEO_SUB,
    E_CHANNEL_VIDEO_3RD,
    E_CHANNEL_VIDEO_4TH,
    E_CHANNEL_VIDEO_MAX = 8,    //max video streams
    E_CHANNEL_AUDIO,
    E_CHANNEL_AUDIO_2RD,
    E_CHANNEL_AUDIO_3RD,
    E_CHANNEL_AUDIO_4TH,
    E_CHANNEL_MAX = 16
}CHANNEL_E;

typedef enum
{
    E_VIDEO_PB_FRAME = 0,
    E_VIDEO_I_FRAME,
    E_VIDEO_TS_FRAME,
    E_AUDIO_FRAME,
    E_CMD_FRAME,
    E_VIDEO_KEY_FRAME,
    E_MEDIA_FRAME_TYPE_MAX
}MEDIA_FRAME_TYPE_E;

typedef struct
{
    MEDIA_FRAME_TYPE_E type;
    unsigned char* p_buf;
    UINT_X    size;
    UINT64_X  pts;
    UINT64_X  timestamp;
}MEDIA_FRAME_S;


typedef struct PacketInfo {
    bool isKeyFrame;
    int64_t dts;
    int64_t pts;
    int64_t delay;
    size_t size;
    const uint8_t* data;
} PacketInfo;



#if 0
static idip_status_code_t
live_handler_impl(const idip_av_source_t* source,
                     const char* target,
                     void* udata)
{
    return IDIP_OK;
}
#endif
//------------------------------------------------------------------------------
#ifdef FILE_SOURCE_ENABLE
static idip_status_code_t
av_source_force_video_key_request_impl(avpkt_stream_source_t* self)
{
    assert(self);
    if (!self) {
        return IDIP_INVALID_ARGUMENTS;
    }

    idip_status_code_t rc = IDIP_OK;
    int err = avpkt_stream_source_force_video_key_frame(self);
    if (err) {
        rc = (err == EINVAL ? IDIP_INVALID_ARGUMENTS : IDIP_GENERIC_ERROR);
    }
    return rc;
}
#endif
//------------------------------------------------------------------------------
static inline const char* null_to_empty_str(const char* s)
{
    return (s ? s : "");
}
//------------------------------------------------------------------------------

static void
av_source_event_handler_impl(idip_av_source_event_t event,
                             const idip_av_source_t* source,
                             const void* event_data,
                             void* udata)
{
    IDIP_UNUSED(event_data);

    char* target = idip_av_source_get_target(source);
    char* profile = idip_av_source_get_profile(source);

    switch (event) {
    case IDIP_AV_EVENT_CLIENT_ENTER:
        // Someone is requesting live video.
        printf("IDIP_AV_EVENT_CLIENT_ENTER: target=%s, profile=%s",
                        null_to_empty_str(target),
                        null_to_empty_str(profile));
        break;
    case IDIP_AV_EVENT_FIRST_CLIENT_ENTER:
        // Hint: On the event we can enable encoder if it was suspended.
        printf("IDIP_AV_EVENT_FIRST_CLIENT_ENTER: target=%s, profile=%s",
                        null_to_empty_str(target),
                        null_to_empty_str(profile));
        break;
    case IDIP_AV_EVENT_CLIENT_LEAVE:
        // Someone leaves live stream reading. But there are some readers left.
        // We can skip the event on most cases.
        printf("IDIP_AV_EVENT_CLIENT_LEAVE: target=%s, profile=%s",
                        null_to_empty_str(target),
                        null_to_empty_str(profile));
        break;
    case IDIP_AV_EVENT_LAST_CLIENT_LEAVE:
        // Nobody's reading live video.
        // Hint: we can suspend live strem encoder for power consumption reasons.
        printf("IDIP_AV_EVENT_LAST_CLIENT_LEAVE: target=%s, profile=%s",
                        null_to_empty_str(target),
                        null_to_empty_str(profile));
        break;
    case IDIP_AV_EVENT_KEYFRAME_REQUEST:
        // A key frame requested from encoder.
        //av_source_force_video_key_request_impl((avpkt_stream_source_t*)udata);
        break;

    default:
        assert(false);
        break;
    }
    IDIP_FREE(target);
    IDIP_FREE(profile);
}

//------------------------------------------------------------------------------
static idip_status_code_t
archive_play(idip_stream_archive_t* stream, const char* path, int64_t start, int64_t duration)
{
#if 0  
    video_stream_info_t v_info;
    audio_stream_info_t a_info;
    archive_record_t    d_info;

    printf("Playing archive %s %" PRId64 "..%" PRId64 "...",
                    path, start, start + duration);

    avpkt_source_t* source = avpkt_source_new_from_file(path, false);
    if (!source) {
        return IDIP_GENERIC_ERROR;
    }

    avpkt_source_video_info(source, &v_info);
    avpkt_source_audio_info(source, &a_info);
    d_info.start_ms = (uint64_t)start;
    d_info.duration_ms = (uint64_t)(duration);

    // NB! User MUST check return values of these functions.
    if (IDIP_OK != idip_stream_ar_put_meta_info(stream, &v_info, &a_info, &d_info, IDIP_INFINITE_TIMEOUT)) {
        goto end;
    }

    // to correct timestamps ans delays
    avpkt_source_start_play(source, start);

    // Playing streams up to IDIP server accepts data
    avpkt_t pkt;
    for (;;) {
        idip_status_code_t rc;

        // Get video and/or audio packet.
        // We use here simple wrapper to get data.
        if (0 != avpkt_source_get_next_packet(source, &pkt)) {
            break;
        }

        // All idip_stream_xxx_put_yyy() functions are synchronous. They do not
        // copy data to avoid performance troubles
        if (pkt.video != NULL) {
            rc = idip_stream_ar_put_video_frame(stream,
                                                pkt.is_key_frame, pkt.ts_ms,
                                                pkt.video, pkt.video_size,
                                                IDIP_INFINITE_TIMEOUT);
            // User MUST checks return value to determine exit request
            if (IDIP_OK != rc) {
                break;
            }
        }

        // Put audio data samples if you got it
        if (pkt.audio != NULL) {
            rc = idip_stream_ar_put_audio(stream, pkt.ts_ms,
                                          pkt.audio, pkt.audio_size,
                                          IDIP_INFINITE_TIMEOUT);
            if (IDIP_OK != rc) {
                break;
            }

        }
    }

end:
    avpkt_source_delete(source);
#endif
    return IDIP_OK;
}

typedef enum play_file_result
{
    PLAY_BACK_ERROR_IS_OK = 0,
    PLAY_BACK_ERROR_FILE_EOF = 1,
    PLAY_BACK_ERROR_FILE_FAILED = 2,
    PLAY_BACK_ERROR_IDIP_FAILED = 3,
}play_file_result_t;

static play_file_result_t play_record_file(IN idip_stream_archive_t* stream, IN const char* fileName, IN uint64_t startTime_ms, IN archive_play_mode_t play_mode, IN int playBeginKeyFrame, OUT uint64_t *lastTimeStamp_ms)
{
    int ret = PLAY_BACK_ERROR_IS_OK;
    frame_head_t frameHead;
    video_frame_head_t videoHead;
    int breakRead = 0;
    idip_status_code_t rc;
    int readLen = 0;

    FILE* fp1 = fopen(fileName, "rb");
    if (fp1)
    {
        archive_record_t arcRecord;
        video_stream_info_t v_info;
        audio_stream_info_t a_info;

        v_info.codec = VIDEO_CODEC_H264;
        v_info.width = 1920;
        v_info.height = 1080;

        a_info.channels = 1;
        a_info.codec_type = AUDIO_CODEC_AAC;
        a_info.sample_format = AUDIO_SAMPLE_FORMAT_FLOATP;
        a_info.sample_rate = 16000;

        arcRecord.start_ms = startTime_ms;
        arcRecord.duration_ms = 5 * 60 * 1000L;

        //find last frame timestamp
        uint64_t curLastTimestamp = 0L;
        do
        {
            readLen = fread(&frameHead, 1, sizeof(frame_head_t), fp1);

            if (readLen != sizeof(frame_head_t))
            {
                break;
            }

            if (frameHead.frame_head_len == 0 && frameHead.frame_data_len == 0)
            {
                break;
            }

            if (frameHead.frame_type == FRAME_TYPE_VIDEO)
            {
                curLastTimestamp = frameHead.timestamp_ms;
            }
            
            fseek(fp1, frameHead.frame_head_len+frameHead.frame_data_len, SEEK_CUR);            

        } while (1);

        

        if (curLastTimestamp > startTime_ms)
        {
            arcRecord.duration_ms = curLastTimestamp - startTime_ms;

            if (arcRecord.duration_ms > 5 * 24 * 3600 * 1000L)              
            {
                MYERRORING("record duration is error:[%llu] \n", arcRecord.duration_ms);          
            }
        }

        //arcRecord.duration_ms = 30 * 60 * 1000L;

        rc = idip_stream_ar_put_meta_info(stream, &v_info, NULL, &arcRecord, IDIP_INFINITE_TIMEOUT);

        if (IDIP_OK != rc) {
            // return from the handler immediately because the exit
            // condition is met.
            if (fp1)
            {
                fclose(fp1);
                fp1 = NULL;
            }

            return PLAY_BACK_ERROR_IDIP_FAILED;
        }

        usleep(2*1000);

        fseek(fp1, 0, SEEK_SET);

        int hasKeyFrame = 0;

        do
        {
            readLen = fread(&frameHead, 1, sizeof(frame_head_t), fp1);

            if (frameHead.frame_head_len == 0 && frameHead.frame_data_len == 0)
            {
                break;
            }

            if (readLen != sizeof(frame_head_t) && !feof(fp1))
            {
                MYERROR("read record file[%s] error:[%d][%s] \n", fileName, errno, strerror(errno));
                ret = PLAY_BACK_ERROR_FILE_FAILED;
                break;
            }
            else if (feof(fp1))
            {
                MYWARN("read record file[%s] is eof!", fileName);
                ret = PLAY_BACK_ERROR_FILE_EOF;
                break;
            }
            else if (frameHead.timestamp_ms < startTime_ms)
            {
                usleep(5);
                continue;
            }

            if (frameHead.frame_type == FRAME_TYPE_VIDEO)
            {
                *lastTimeStamp_ms = frameHead.timestamp_ms;
            }

            readLen = fread(&videoHead, 1, sizeof(video_frame_head_t), fp1);
            if (readLen != sizeof(video_frame_head_t) && !feof(fp1))
            {
                MYERROR("read record file[%s] error:[%d][%s] \n", fileName, errno, strerror(errno));
                ret = PLAY_BACK_ERROR_FILE_FAILED;
                break;
            }
            else if (feof(fp1))
            {
                MYWARN("read record file[%s] is eof!", fileName);
                ret = PLAY_BACK_ERROR_FILE_EOF;
                break;
            }

            if (!hasKeyFrame && videoHead.frame_type == VIDEO_I_FRAME)
            {
                hasKeyFrame = 1;
            }

            if (playBeginKeyFrame && !hasKeyFrame)
            {
                fseek(fp1, videoHead.frame_len, SEEK_CUR);
                usleep(1);
                continue;
            }            

            //if (play_mode == ARCHIVE_PLAY_MODE_ONESHOT && videoHead.frame_type != VIDEO_I_FRAME)
            //{
            //    fseek(fp1, videoHead.frame_len, SEEK_CUR);
            //    usleep(1);
            //    continue;
            //}

            int cacheFrames = 100;
            int frameCount = 0;
            pthread_mutex_lock(g_lux_record->playback_mutex);
            do
            {
                if (fread(g_lux_record->playback_framebuf, 1, videoHead.frame_len, fp1) == videoHead.frame_len)
                {
                    if (g_lux_record->playback_framebuf[0] == frame_head_magic[0] && g_lux_record->playback_framebuf[1] == frame_head_magic[1])
                    {
                        g_lux_record->playback_framebuf[0] = 0x00;
                        g_lux_record->playback_framebuf[1] = 0x00;
                    }
                    rc = idip_stream_ar_put_video_frame(stream, videoHead.frame_type == VIDEO_I_FRAME ? 1 : 0, frameHead.timestamp_ms, g_lux_record->playback_framebuf, videoHead.frame_len, IDIP_INFINITE_TIMEOUT);
                    //MYTRACE("idip_stream_ar_put_video_frame, ret:[%d] data len:[%d] frame type:[%d] start timestamp:[%llu] cur timestamp:[%llu] duration time:[%llu]\n", rc, videoHead.frame_len, videoHead.frame_type, startTime_ms, frameHead.timestamp_ms, arcRecord.duration_ms);
                    if (rc != IDIP_OK /* && rc != IDIP_GENERIC_ERROR*/)
                    {
                        MYERROR("idip_stream_ar_put_video_frame error, playmode:%d, datalen:%d, ret:%d, frame type:[%d], data:[%02X %02X %02X %02X %02X]\n", 
                            play_mode, 
                            videoHead.frame_len, 
                            rc,
                            videoHead.frame_type,
                            g_lux_record->playback_framebuf[0], 
                            g_lux_record->playback_framebuf[1], 
                            g_lux_record->playback_framebuf[2], 
                            g_lux_record->playback_framebuf[3],
                            g_lux_record->playback_framebuf[4]);
                        ret = PLAY_BACK_ERROR_IDIP_FAILED;
                        breakRead = 1;
                        break;
                    }
                    else
                    {
                        switch (play_mode)
                        {
                        case ARCHIVE_PLAY_MODE_ONESHOT:
                        {
                            MYTRACE("idip_stream_ar_put_video_frame key frame finished, len:[%d] \n", videoHead.frame_len);
                            ret = PLAY_BACK_ERROR_IS_OK;
                            breakRead = 1;
                        }
                        break;

                        case ARCHIVE_PLAY_MODE_SINGLE:
                        case ARCHIVE_PLAY_MODE_CONTINOUS:
                        {
                            breakRead = 0;
                            if (frameCount < cacheFrames)
                            {
                                usleep(1 * 1000);
                            }
                            else
                            {
                                usleep(40 * 1000);
                            }
                            frameCount++;
                        }
                        break;

                        case _ARCHIVE_PLAY_MODE_MAX:
                        {
                            breakRead = 0;
                            usleep(5);
                        }
                        break;

                        default:
                        {
                            //???
                        }
                        break;
                        }
                    }
                }
                else
                {
                    if (feof(fp1))
                    {
                        MYTRACE("read record file[%s] is eof!", fileName);
                        ret = PLAY_BACK_ERROR_FILE_EOF;
                    }
                    else
                    {
                        MYERROR("read video frame from file[%s] error:[%d][%s] \n", fileName, errno, strerror(errno));
                        ret = PLAY_BACK_ERROR_FILE_FAILED;
                    }

                    breakRead = 1;

                    break;
                }
            } while (0);
            pthread_mutex_unlock(g_lux_record->playback_mutex);

        } while (!breakRead);

        fclose(fp1);
    }
    else
    {
        MYERROR("open record file[%s] error:[%d][%s]\n", fileName, errno, strerror(errno));
        ret = PLAY_BACK_ERROR_FILE_FAILED;
    }

    return ret;
}

//------------------------------------------------------------------------------
static idip_status_code_t
archive_handler_impl(idip_stream_archive_t* stream,
                     const char* target,
                     const idip_archive_play_params_t* params,
                     void* udata)
{
    IDIP_UNUSED(target);
    demo_options_t* options = udata;

    time_t mytime = params->start_ms;
    struct tm* tm_start = gmtime(&mytime);

    MYWARN("enter archive_handler_impl(), playmode=%d, target=%s, start=[%llu][%s])\n", 
        params->play_mode, target, params->start_ms, format_time(params->start_ms/1000));

    printf("play mode:[%d] \n", stream->query_args.play_params.play_mode);
    
    char recordFileName[128];
    play_file_result_t playResult;
    uint64_t lastTimeStamp_ms;

    archive_record_t arcRecord;
    video_stream_info_t v_info;
    audio_stream_info_t a_info;
    int rc;

    v_info.codec = VIDEO_CODEC_H264;
    v_info.width = 1920;
    v_info.height = 1080;

    a_info.channels = 1;
    a_info.codec_type = AUDIO_CODEC_AAC;
    a_info.sample_format = AUDIO_SAMPLE_FORMAT_FLOATP;
    a_info.sample_rate = 16000;

#if 0
    rc = idip_stream_ar_put_video_info(stream, &v_info, IDIP_INFINITE_TIMEOUT);
    if (IDIP_OK != rc) {
        // return from the handler immediately because the exit
        // condition is met.
        MYERROR("idip_stream_ar_put_video_info error:[%d] \n", rc);
        return IDIP_OK;
    }
#endif

#if 0
    rc = idip_stream_ar_put_audio_info(stream, &a_info, IDIP_INFINITE_TIMEOUT);
    if (IDIP_OK != rc)
    {
        MYERROR("idip_stream_ar_put_audio_info error:[%d] \n", rc);
        return IDIP_OK;
    }
#endif

    if (LUX_Record_Find_By_Time(params->start_ms, recordFileName) != 0)
    {
        MYWARN("not found record file by timestamp:[%llu][%s]\n", params->start_ms, format_time(params->start_ms/1000));
        return IDIP_INVALID_ARGUMENTS;
    }

    switch (params->play_mode)
    {
    case ARCHIVE_PLAY_MODE_SINGLE:
    case ARCHIVE_PLAY_MODE_ONESHOT:
    {
        playResult = play_record_file(stream, recordFileName, params->start_ms, params->play_mode, 1, &lastTimeStamp_ms);
    }
    break;

    case _ARCHIVE_PLAY_MODE_MAX:
    case ARCHIVE_PLAY_MODE_CONTINOUS:
    {
        do
        {
            playResult = play_record_file(stream, recordFileName, params->start_ms, params->play_mode, 1, &lastTimeStamp_ms);

            if (playResult == PLAY_BACK_ERROR_FILE_EOF
                || playResult == PLAY_BACK_ERROR_FILE_FAILED
                || playResult == PLAY_BACK_ERROR_IS_OK)
            {
                //查找下一个录像文件
                if (LUX_Record_Find_By_Time(lastTimeStamp_ms + 5, recordFileName) != 0)
                {
                    MYTRACE("not found record file at timestamp:[%llu]\n", lastTimeStamp_ms + 5);
                    break;
                }

                continue;
            }
            else if (playResult == PLAY_BACK_ERROR_IDIP_FAILED)
            {
                break;
            }
            else
            {
                break;
            }
        } while (1);
    }
    break;

    default:
    {
    }
    break;
    }

    return IDIP_OK;
}

//------------------------------------------------------------------------------
static idip_status_code_t
archive_list_handler_impl(idip_stream_archive_list_t* stream,
                          const char* target,
                          const archive_interval_t* params,
                          void* udata)
{
    int i;
    char filePath[64];
    time_t start1 = params->start_ms / 1000;
    time_t end1 = params->end_ms / 1000;
    struct tm tmStart = *gmtime(&start1);
    struct tm tmEnd = *gmtime(&end1);
    record_info_t recordInfo;
    archive_record_t item;

    char strtime1[32], strtime2[32];
    strcpy(strtime1, format_time(start1));
    strcpy(strtime2, format_time(end1));

    MYTRACE("enter archive_list_handler_impl(),begin:%s end:%s \n", strtime1, strtime2);

    //计算天数
    uint64_t days = (params->end_ms - params->start_ms) / 1000 / 3600 / 24;

    //查找符合时间范围的录像
    for (i = 0; i < days + 3; i++)
    {
        start1 = params->start_ms / 1000 + i * 24 * 3600;
        tmStart = *gmtime(&start1);

        sprintf(filePath, "%s/%04d/%02d/%02d/record.info", g_lux_record->basePath_DCIM, tmStart.tm_year + 1900, tmStart.tm_mon + 1, tmStart.tm_mday);

        FILE* fp1 = fopen(filePath, "rb");
        if (fp1)
        {
            fseek(fp1, sizeof(int), SEEK_SET);
            do
            {
                if (fread(&recordInfo, 1, sizeof(record_info_t), fp1) == sizeof(record_info_t))
                {
                    if (recordInfo.start_time_ms > params->end_ms)
                    {
                        break;
                    }
                    else if (recordInfo.end_time_ms < params->start_ms)
                    {
                        continue;
                    }
                    else
                    {
                        uint64_t metaBeginTime = max(recordInfo.start_time_ms, params->start_ms);
                        uint64_t metaEndTime = min(recordInfo.end_time_ms, params->end_ms);

                        item.start_ms = metaBeginTime;
                        item.duration_ms = metaEndTime - metaBeginTime;
                        if (item.duration_ms > 1 * 3600 * 1000)
                        {
                            item.duration_ms = 5 * 60 * 1000;;
                        }

                        int rc = idip_stream_ar_list_put_items(stream, &item, 1, IDIP_INFINITE_TIMEOUT);

                        if (rc != IDIP_OK)
                        {
                            MYERROR("idip_stream_ar_list_put_items error:%d \n", rc);
                            break;
                        }
                    }
                }
                else
                {
                    break;
                }
            } while (1);

            fclose(fp1);
        }
        else
        {
            MYWARN("open record info file[%s] error:[%d][%s] \n", filePath, errno, strerror(errno));
        }
    }

    return IDIP_OK;
}
//------------------------------------------------------------------------------
static idip_status_code_t archive_days_handler_impl(
        idip_stream_archive_days_t* stream,
        const char* target,
        const archive_interval_t* params,
        void* udata)
{
    IDIP_UNUSED(target);
    demo_options_t* options = udata;

    int i;
    char filePath[64];
    time_t start1 = params->start_ms / 1000;
    time_t end1 = params->end_ms / 1000;
    struct tm tmStart = *gmtime(&start1);
    struct tm tmEnd = *gmtime(&end1);
    record_info_t recordInfo;
    archive_record_t item;
    FILE* fp1;

    MYTRACE("enter archive_days_handler_impl(), begin:%04d-%02d-%02d %02d:%02d:%02d end:%04d-%02d-%02d %02d:%02d:%02d \n",
        tmStart.tm_year + 1900, tmStart.tm_mon + 1, tmStart.tm_mday, tmStart.tm_hour, tmStart.tm_min, tmStart.tm_sec,
        tmEnd.tm_year + 1900, tmEnd.tm_mon + 1, tmEnd.tm_mday, tmEnd.tm_hour, tmEnd.tm_min, tmEnd.tm_sec);

    //计算天数
    uint64_t days = (params->end_ms - params->start_ms) / 1000 / 3600 / 24 + 1;

    for (i = 0; i < days; i++)
    {
        start1 = params->start_ms / 1000 + i * 24 * 3600;
        tmStart = *gmtime(&start1);
        sprintf(filePath, "%s/%04d/%02d/%02d/record.info", g_lux_record->basePath_DCIM, tmStart.tm_year + 1900, tmStart.tm_mon + 1, tmStart.tm_mday);

        fp1 = fopen(filePath, "rb");
        if (fp1)
        {
            if (fread(&recordInfo, 1, sizeof(record_info_t), fp1) == sizeof(record_info_t))
            {
                item.start_ms = recordInfo.start_time_ms;
                item.duration_ms = recordInfo.end_time_ms - recordInfo.start_time_ms;

                int rc = idip_stream_ar_list_put_items(stream, &item, 1, IDIP_INFINITE_TIMEOUT);
                if (rc != IDIP_OK)
                {
                    MYERROR("idip_stream_ar_list_put_items error:%d \n", rc);
                    break;
                }
            }
            else
            {
                //???
            }

            fclose(fp1);
        }
        else
        {
            //???
        }
    }

    return IDIP_OK;
}
//------------------------------------------------------------------------------
// example model of device configuration

// timezone offsets
// https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
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
static const char* const resolutions_0[] = { "1920x1080","1280x720" };
static const char* const resolutions_1[] = {"800x600","720x576", "640x480", "320x240"};
// supported bitrates
static const int64_t bitrates_0[] = {4096,3072, 2048,1536,1024, 512,256};
static const int64_t bitrates_1[] = { 512,256,128, 64 };
// supported fps values
static const int64_t fps_list_0[] = { 30, 25, 20, 15, 10, 5, 1 };
static const int64_t fps_list_1[] = { 30, 25, 20, 15, 10, 5, 1 };
// supported video codecs
static const char* const codecs[] = { "H.264" };
// supported audio codecs  支持音频编解码器
static const char* const acodecs[] = { "AAC" };

void set_light_onoff(BOOL_X light_on_off);
void set_night_vision_mode(const char* value);
// hook forwards
static idip_cfg_hook_status_t
cfg_resolution_0_hook(const char* target, const char* param_name,
                      const char* value, idip_cfg_meta_t* meta,
                      void* hook_user_data);
static idip_cfg_hook_status_t
cfg_bool_update_hook(const char* target,
                        const char* param_name,
                        bool value,
                        idip_cfg_meta_t* meta,
                        void* hook_user_data);

static idip_status_code_t
cfg_read_hook(const char* target,
                        const idip_cfg_meta_t* meta,
                        struct idip_stream_config* out_stream,
                        void* hook_user_data);

static idip_cfg_hook_status_t
cfg_str_update_hook(const char* target, const char* param_name,
                      const char* value, idip_cfg_meta_t* meta,
                      void* hook_user_data);

static idip_cfg_hook_status_t
cfg_int64_update_hook(const char* target, const char* param_name,
                       const int64_t value, idip_cfg_meta_t* meta,
                        void* hook_user_data);


// Range for a motion detector grid

//static instance of configuration model
// static LUX_CONFIG_JIO_CFG_ST cfg = 
// {
//     .online_mode = 1,
//     .timezone = "+08:00",
//     .timezone_offset_s = 28800,
//     // "microphone/enabled"
//     .microphone_enabled = 1,
//     // "led/enabled"
//     .led_enabled = 0,
//     // "night_vision/mode"
//     .night_vision_mode = "off",
//     // "general/power_freq"
//     .power_freq = "50 HZ",

//     // "general/flip_mode"
//     .flip_mode = 0,

//     // Detectors
//     // "sound_detection/enabled"
//     .sound_detection = 0,
//     // "sound_detection/sensitivity"
//     .sound_detection_sensitivity = 64,
//     // "pir_detection/enabled"
//     .pir_detection = 0,
//     // "motion_detector/enabled"
//     .motion_detector = 0,
//     // "motion_detector/N/sensitivity"
//     .motion_detector_0_sensitivity = 0,
//     // "motion_detector/N/threshold"
//     .motion_detector_0_threshold = 0,

//     .motion_detector_0_zone = {0},

//     // current resolution
//     .resolution_0 = "1920x1080",
//     .resolution_1 = "720x576",
//     // current bitrate
//     .bitrate_0 = 1536,
//     .bitrate_1 = 256,
//     // current fps
//     .fps_0 = 25,
//     .fps_1 = 25,
//     // osd/show_time
//     .osd_show_time = 1,
//     // video codecs
//     .codec_0 = "H264",
//     .codec_1 = "H264",
//     // audio codecs
//     .acodec_0 = "AAC",
//     .acodec_1 = "AAC",
//     .online = 0,

//     .face_detection = 0,
//     .privacy_mask = 0
// };

static idip_cfg_meta_t cfg_meta[] = {
    //[0]
    IDIP_CFG_META_INIT_GENERAL_ONLINE_MODE(&jio_cfg.system_config.online_mode, NULL, NULL, NULL),
    //[1] 
    IDIP_CFG_META_INIT_GENERAL_TIMEZONE(&jio_cfg.system_config.timezone, tz_offsets, ARRAY_SIZE(tz_offsets), cfg_str_update_hook, NULL, NULL),
    //[2]
    IDIP_CFG_META_INIT_GENERAL_MICROPHONE_ENABLED(&jio_cfg.media_config.audio_config.microphone_enabled, cfg_bool_update_hook, NULL, NULL),
    //[3]
    IDIP_CFG_META_INIT_LED_ENABLED(&jio_cfg.system_config.led_enabled, cfg_bool_update_hook, NULL, NULL),
    //[4]
    IDIP_CFG_META_INIT_NIGHT_VISION_MODE(&jio_cfg.image_config.daynight, cfg_str_update_hook, NULL, NULL),
    //[5]
    IDIP_CFG_META_INIT_GENERAL_POWER_FREQ(&jio_cfg.system_config.power_freq, cfg_str_update_hook, NULL, NULL),
    //[6]
    IDIP_CFG_META_INIT_SOUND_DETECTION_ENABLED(&jio_cfg.alg_config.sound_detect_enabled, cfg_bool_update_hook, NULL, NULL),
    //[7]
    IDIP_CFG_META_INIT_SOUND_DETECTION_SENSITIVITY(&jio_cfg.alg_config.sound_detect_sensitivity, cfg_int64_update_hook, NULL, NULL),
    //[8]
    IDIP_CFG_META_INIT_MOTION_DETECTION_ENABLED(&jio_cfg.alg_config.motion_detect_enabled, cfg_bool_update_hook, NULL, NULL),
    //[9]
    IDIP_CFG_META_INIT_MOTION_DETECTION_SENSITIVITY(0, &jio_cfg.alg_config.motion_detect_0_sensitivity, NULL, NULL, NULL),
    //[10]
    IDIP_CFG_META_INIT_MOTION_DETECTION_THRESHOLD(0, &jio_cfg.alg_config.motion_detect_1_threshold, NULL, NULL, NULL),
    //[11]
    IDIP_CFG_META_INIT_MOTION_DETECTION_ZONES(0, &jio_cfg.alg_config.motion_detect_0_zone, motion_detector_grid_range, NULL, NULL, NULL),

    //[12]
    IDIP_CFG_META_INIT_STREAMS_N_RESOLUTION(0, &jio_cfg.image_config.resolution_0, resolutions_0, ARRAY_SIZE(resolutions_0), cfg_resolution_0_hook, NULL, NULL),
    //[13]
    IDIP_CFG_META_INIT_STREAMS_N_BITRATE(0, &jio_cfg.image_config.bitrate_0, bitrates_0, ARRAY_SIZE(bitrates_0), cfg_int64_update_hook, NULL, NULL),
    //[14]
    IDIP_CFG_META_INIT_STREAMS_N_FPS(0, &jio_cfg.image_config.fps_0, fps_list_0, ARRAY_SIZE(fps_list_0), cfg_int64_update_hook, NULL, NULL),
    //[15]
    IDIP_CFG_META_INIT_STREAMS_N_VCODEC(0, &jio_cfg.media_config.video_config.codec_0, codecs, ARRAY_SIZE(codecs), cfg_str_update_hook, NULL, NULL),
    //[16]
    IDIP_CFG_META_INIT_STREAMS_N_ACODEC(0, &jio_cfg.media_config.audio_config.acodec_0, acodecs, ARRAY_SIZE(acodecs), cfg_str_update_hook, NULL, NULL),
    //[17]
    IDIP_CFG_META_INIT_STREAMS_N_RESOLUTION(1, &jio_cfg.image_config.resolution_1, resolutions_1, ARRAY_SIZE(resolutions_1), cfg_resolution_0_hook, NULL, NULL),
    //[18]
    IDIP_CFG_META_INIT_STREAMS_N_BITRATE(1, &jio_cfg.image_config.bitrate_1, bitrates_1, ARRAY_SIZE(bitrates_1), cfg_int64_update_hook, NULL, NULL),
    //[19]
    IDIP_CFG_META_INIT_STREAMS_N_FPS(1, &jio_cfg.image_config.fps_1, fps_list_1, ARRAY_SIZE(fps_list_1), cfg_int64_update_hook, NULL, NULL),
    //[20]
    IDIP_CFG_META_INIT_STREAMS_N_VCODEC(1, &jio_cfg.media_config.video_config.codec_1, codecs, ARRAY_SIZE(codecs), cfg_str_update_hook, NULL, NULL),
    //[21]
    IDIP_CFG_META_INIT_STREAMS_N_ACODEC(1, &jio_cfg.media_config.audio_config.acodec_1, acodecs, ARRAY_SIZE(acodecs), cfg_str_update_hook, NULL, NULL),

    //[22]
    IDIP_CFG_META_INIT_OSD_DATETIME_ENABLED(&jio_cfg.image_config.osd_show_time, cfg_bool_update_hook, NULL, NULL),

    //[23]

    //[24]face_cover_test
    IDIP_CFG_META_INIT_FACE_DETECTION_ENABLED(&jio_cfg.alg_config.face_detect_enabled, NULL, NULL, NULL),
    
    IDIP_CFG_META_INIT_PRIVACY_MASK_N_ENABLED(&jio_cfg.alg_config.sleepMode, cfg_bool_update_hook, NULL, NULL, NULL),

};

#if 0
//------------------------------------------------------------------------------
static void initialize_config(void)
{
    // general
    jio_cfg.system_config.online_mode = 1;
    jio_cfg.system_config.timezone = tz_offsets[0];
    jio_cfg.media_config.audio_config.microphone_enabled = 1;
    jio_cfg.system_config.led_enabled = 1;
    jio_cfg.image_config.daynight = nightvision_modes[2];
    jio_cfg.system_config.power_freq = idip_cfg_range_power_freq[0];
    jio_cfg.g_JioConfig.flip_mode = 0;

    // Detectors
    jio_cfg.alg_config.sound_detect_enabled = 1;
    jio_cfg.alg_config.sound_detect_sensitivity = 40;
    jio_cfg.g_JioConfig.pir_detection = 0;
    jio_cfg.alg_config.motion_detect_enabled = 1;
    jio_cfg.alg_config.motion_detect_0_sensitivity = 80;
    jio_cfg.alg_config.motion_detect_1_threshold = 80;
    memset(jio_cfg.g_JioConfig.motion_detector_0_zone, '0', sizeof(jio_cfg.g_JioConfig.motion_detector_0_zone));
    jio_cfg.g_JioConfig.motion_detector_0_zone[sizeof(jio_cfg.g_JioConfig.motion_detector_0_zone) - 1] = '\0';

    // Video and audio
    jio_cfg.image_config.resolution_0 = resolutions_0[0];
    jio_cfg.image_config.resolution_1 = resolutions_1[0];
    jio_cfg.image_config.bitrate_0 = bitrates_0[0];
    jio_cfg.image_config.bitrate_1 = bitrates_1[0];
    jio_cfg.image_config.fps_0 = fps_list_0[0];
    jio_cfg.image_config.fps_1 = fps_list_1[0];
    jio_cfg.media_config.video_config.codec_0 = codecs[0];
    jio_cfg.media_config.video_config.codec_1 = codecs[0];
    jio_cfg.media_config.audio_config.acodec_0 = acodecs[0];
    jio_cfg.media_config.audio_config.acodec_1 = acodecs[0];

    jio_cfg.image_config.osd_show_time = 0;
    jio_cfg.system_config.online = 0;

    jio_cfg.alg_config.face_detect_enabled = 1;
    jio_cfg.alg_config.sleepMode = 1;
    // if (jio_cfg.system_config.led_enabled)
    // {
    //     LUX_PWM_Start();
    // }
    // else
    // {
    //     LUX_PWM_Stop();
    // }

    if (jio_cfg.media_config.audio_config.microphone_enabled)
    {
        int ret = LUX_AI_Start();
        if (0 != ret)
        {
            IMP_LOG_ERR(TAG, "ImpStreamOn failed\n");
            return -1;
        }
    }
    // if(jio_cfg.system_config.led_enabled)
    // {
    //     LUX_HAL_LedSetStatus(LUX_HAL_LED_GREEN, LUX_HAL_LED_ON);
    // }
    // else 
    // {
    //     LUX_HAL_LedSetStatus(LUX_HAL_LED_GREEN, LUX_HAL_LED_OFF);
    // }
    set_light_onoff(jio_cfg.system_config.led_enabled);
    set_night_vision_mode(jio_cfg.image_config.daynight);
    set_cry_detect_onoff(jio_cfg.alg_config.sound_detect_enabled);
}
#endif

// void load_jio_config()
// {
//     INT_X ret = 0;
//     char tempStr[32] = { 0 };
//     size_t i;
//     // general
//     //jio_cfg.system_config.online_mode = 1;


//------------------------------------------------------------------------------
void init_jio_config(void)
{
 int ret = 0;
#if 0
 int ret = 0;
	  char tempStr[32] = { 0 };
	  LUX_INIParse_GetInt(LUX_CONFIG_FILE,"system_config", "online", &jio_cfg.system_config.online_mode);
	  LUX_INIParse_GetString(LUX_CONFIG_FILE,"system_config", "timezone", tempStr);
	  jio_cfg.system_config.timezone = find_option(tempStr, tz_offsets, (sizeof(tz_offsets) / sizeof(tz_offsets[0])));
	  LUX_INIParse_GetInt(LUX_CONFIG_FILE,"media_config_audio", "microphone_enabled", &jio_cfg.media_config.audio_config.microphone_enabled);
	  LUX_INIParse_GetInt(LUX_CONFIG_FILE,"system_config", "led_enabled", &jio_cfg.system_config.led_enabled); 
	  LUX_INIParse_GetString(LUX_CONFIG_FILE,"image_config", "daynight",tempStr );
	  jio_cfg.image_config.daynight = find_option(tempStr, nightvision_modes, (sizeof(nightvision_modes) / sizeof(nightvision_modes[0])));
	  LUX_INIParse_GetString(LUX_CONFIG_FILE,"system_config", "power_freq",tempStr);
	  jio_cfg.system_config.power_freq = find_option(tempStr, idip_cfg_range_power_freq, (sizeof(idip_cfg_range_power_freq) / sizeof(idip_cfg_range_power_freq[0])));
	  LUX_INIParse_GetInt(LUX_CONFIG_FILE,"image_config", "vflip", &jio_cfg.g_JioConfig.flip_mode);   
	  LUX_INIParse_GetInt(LUX_CONFIG_FILE,"algorithm_config", "sound_detection_enabled", &jio_cfg.alg_config.sound_detect_enabled);   
	  LUX_INIParse_GetInt(LUX_CONFIG_FILE,"algorithm_config", "sound_detection_sensitivity", &jio_cfg.alg_config.sound_detect_sensitivity); 
	  LUX_INIParse_GetInt(LUX_CONFIG_FILE,"algorithm_config", "pir_detection_enabled", &jio_cfg.g_JioConfig.pir_detection);  
	  LUX_INIParse_GetInt(LUX_CONFIG_FILE,"algorithm_config", "motion_detector_enabled", &jio_cfg.alg_config.motion_detect_enabled);
	  LUX_INIParse_GetInt(LUX_CONFIG_FILE,"algorithm_config", "motion_detector_0_sensitivity", &jio_cfg.alg_config.motion_detect_0_sensitivity);   
	  LUX_INIParse_GetInt(LUX_CONFIG_FILE,"algorithm_config", "motion_detector_1_threshold", &jio_cfg.alg_config.motion_detect_1_threshold);
	  memset(jio_cfg.g_JioConfig.motion_detector_0_zone, '0', sizeof(jio_cfg.g_JioConfig.motion_detector_0_zone));
	    jio_cfg.g_JioConfig.motion_detector_0_zone[sizeof(jio_cfg.g_JioConfig.motion_detector_0_zone) - 1] = '\0';
	  LUX_INIParse_GetInt(LUX_CONFIG_FILE,"image_config", "osd_show_time", &jio_cfg.image_config.osd_show_time);    
	  LUX_INIParse_GetString(LUX_CONFIG_FILE,"image_config", "resolution_0", tempStr); 
	  jio_cfg.image_config.resolution_0 = find_option(tempStr, resolutions_0, (sizeof(resolutions_0) / sizeof(resolutions_0[0])));   
	  LUX_INIParse_GetString(LUX_CONFIG_FILE,"image_config", "resolution_1", tempStr);
	  jio_cfg.image_config.resolution_1 = find_option(tempStr, resolutions_1, (sizeof(resolutions_1) / sizeof(resolutions_1[0])));   
	  LUX_INIParse_GetInt(LUX_CONFIG_FILE,"image_config", "bitrate_0", &jio_cfg.image_config.bitrate_0);   	
	  LUX_INIParse_GetInt(LUX_CONFIG_FILE,"image_config", "bitrate_1", &jio_cfg.image_config.bitrate_1);   
	  LUX_INIParse_GetInt(LUX_CONFIG_FILE,"image_config", "fps_0", &jio_cfg.image_config.fps_0);
	  LUX_INIParse_GetInt(LUX_CONFIG_FILE,"image_config", "fps_0", &jio_cfg.image_config.fps_0);
	  LUX_INIParse_GetInt(LUX_CONFIG_FILE,"image_config", "fps_1", &jio_cfg.image_config.fps_1);
	  LUX_INIParse_GetInt(LUX_CONFIG_FILE,"image_config", "fps_1", &jio_cfg.image_config.fps_1);
	  LUX_INIParse_GetString(LUX_CONFIG_FILE,"media_config_video", "codec_0",tempStr);  
	  jio_cfg.media_config.video_config.codec_0 = find_option(tempStr, codecs, (sizeof(codecs) / sizeof(codecs[0])));     
	  LUX_INIParse_GetString(LUX_CONFIG_FILE,"media_config_video", "codec_1", tempStr);
	  jio_cfg.media_config.video_config.codec_1 = find_option(tempStr, codecs, (sizeof(codecs) / sizeof(codecs[0])));
	  LUX_INIParse_GetString(LUX_CONFIG_FILE,"media_config_audio", "acodec_0",tempStr);
	  jio_cfg.media_config.audio_config.acodec_0 = find_option(tempStr, acodecs, (sizeof(acodecs) / sizeof(acodecs[0])));
	  LUX_INIParse_GetString(LUX_CONFIG_FILE,"media_config_audio", "acodec_1", tempStr);
	  jio_cfg.media_config.audio_config.acodec_1 = find_option(tempStr, acodecs, (sizeof(acodecs) / sizeof(acodecs[0])));
	  LUX_INIParse_GetInt(LUX_CONFIG_FILE,"algorithm_config", "face_detect_enabled", &jio_cfg.alg_config.face_detect_enabled);
	  LUX_INIParse_GetInt(LUX_CONFIG_FILE,"algorithm_config", "sleepMode", &jio_cfg.alg_config.sleepMode);
        
    if (jio_cfg.system_config.led_enabled)
    {
        LUX_PWM_Start();
    }
    else
    {
        LUX_PWM_Start();
    }
    if (jio_cfg.media_config.audio_config.microphone_enabled)
    {
        int ret = LUX_AI_Start();
        if (0 != ret)
        {
            MYERROR( "ImpStreamOn failed\n");
            return -1;
        }
    }
#endif
#if 0
    INT_X ret = 0;
    char tempStr[32] = { 0 };
    // general
    //jio_cfg.system_config.online_mode = 1;

    ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "system_config", "online", &jio_cfg.system_config.online_mode);
    ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "system_config", "timezone", tempStr);
   jio_cfg.system_config.timezone = find_option(tempStr, tz_offsets, (sizeof(tz_offsets) / sizeof(tz_offsets[0])));
       if (!jio_cfg.system_config.timezone) {
        jio_cfg.system_config.timezone = tz_offsets[0];
    }
    ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "media_config_audio", "microphone_enabled", &jio_cfg.media_config.audio_config.microphone_enabled);
    ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "system_config", "led_enabled", &jio_cfg.system_config.led_enabled);
    ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "image_config", "daynight", tempStr);
    jio_cfg.image_config.daynight = find_option(tempStr, nightvision_modes, (sizeof(nightvision_modes) / sizeof(nightvision_modes[0])));
           if (!jio_cfg.image_config.daynight) {
        jio_cfg.image_config.daynight = nightvision_modes[0];
    }
    ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "system_config", "power_freq", tempStr);
    jio_cfg.system_config.power_freq = find_option(tempStr, idip_cfg_range_power_freq, (sizeof(idip_cfg_range_power_freq) / sizeof(idip_cfg_range_power_freq[0])));
           if (!jio_cfg.system_config.power_freq) {
        jio_cfg.system_config.power_freq = idip_cfg_range_power_freq[0];
    }
    ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "image_config", "vflip", &jio_cfg.g_JioConfig.flip_mode);
    ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "algorithm_config", "sound_detection_enabled", &jio_cfg.alg_config.sound_detect_enabled);
    ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "algorithm_config", "sound_detection_sensitivity", &jio_cfg.alg_config.sound_detect_sensitivity);
    ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "algorithm_config", "pir_detection_enabled", &jio_cfg.g_JioConfig.pir_detection);
    ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "algorithm_config", "motion_detector_enabled", &jio_cfg.alg_config.motion_detect_enabled);
    ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "algorithm_config", "motion_detector_0_sensitivity", &jio_cfg.alg_config.motion_detect_0_sensitivity);
    ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "algorithm_config", "motion_detector_1_threshold", &jio_cfg.alg_config.motion_detect_1_threshold);
    memset(jio_cfg.g_JioConfig.motion_detector_0_zone, '0', sizeof(jio_cfg.g_JioConfig.motion_detector_0_zone));
    jio_cfg.g_JioConfig.motion_detector_0_zone[sizeof(jio_cfg.g_JioConfig.motion_detector_0_zone) - 1] = '\0';
    ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "image_config", "osd_show_time", &jio_cfg.image_config.osd_show_time);
    ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "image_config", "resolution_0", tempStr);
    jio_cfg.image_config.resolution_0 = find_option(tempStr, resolutions_0, (sizeof(resolutions_0) / sizeof(resolutions_0[0])));   
    if (!jio_cfg.image_config.resolution_0) {
        jio_cfg.image_config.resolution_0 = resolutions_0[0];
    }
    ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "image_config", "resolution_1", tempStr);
    jio_cfg.image_config.resolution_1 = find_option(tempStr, resolutions_1, (sizeof(resolutions_1) / sizeof(resolutions_1[0])));   
       if (!jio_cfg.image_config.resolution_1) {
        jio_cfg.image_config.resolution_1 = resolutions_1[0];
    }
    ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "image_config", "bitrate_0", &jio_cfg.image_config.bitrate_0);
    ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "image_config", "bitrate_1", &jio_cfg.image_config.bitrate_1);
    ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "image_config", "fps_0", &jio_cfg.image_config.fps_0);
    ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "image_config", "fps_1", &jio_cfg.image_config.fps_1);
    ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "media_config_video", "codec_0", tempStr);
	 jio_cfg.media_config.video_config.codec_0 = find_option(tempStr, codecs, (sizeof(codecs) / sizeof(codecs[0])));
            if (!jio_cfg.media_config.video_config.codec_0) {
        jio_cfg.media_config.video_config.codec_0 = codecs[0];
    } 
    ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "media_config_video", "codec_1", tempStr);
	  jio_cfg.media_config.video_config.codec_1 = find_option(tempStr, codecs, (sizeof(codecs) / sizeof(codecs[0])));
                  if (!jio_cfg.media_config.video_config.codec_1) {
        jio_cfg.media_config.video_config.codec_1 = codecs[0];
    }
    ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "media_config_audio", "acodec_0", tempStr);
	  jio_cfg.media_config.audio_config.acodec_0 = find_option(tempStr, acodecs, (sizeof(acodecs) / sizeof(acodecs[0])));
                        if (!jio_cfg.media_config.audio_config.acodec_0) {
        jio_cfg.media_config.audio_config.acodec_0 = acodecs[0];
    }
    ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "media_config_audio", "acodec_1", tempStr);
	  jio_cfg.media_config.audio_config.acodec_1 = find_option(tempStr, acodecs, (sizeof(acodecs) / sizeof(acodecs[0])));
                              if (!jio_cfg.media_config.audio_config.acodec_1) {
        jio_cfg.media_config.audio_config.acodec_1 = acodecs[0];
    }
    ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "algorithm_config", "face_detection_enabled", &jio_cfg.alg_config.face_detect_enabled);
    ret = LUX_INIParse_GetInt(LUX_CONFIG_FILE, "algorithm_config", "privacy_mask", &jio_cfg.alg_config.sleepMode);
#endif
    //初始化系统
    if (jio_cfg.system_config.led_enabled)
    {
        LUX_PWM_Start();
    }
    else
    {
        LUX_PWM_Start();
    }

    if (jio_cfg.media_config.audio_config.microphone_enabled)
    {
        //int ret = LUX_AI_Start();
        if (0 != ret)
        {
            IMP_LOG_ERR(TAG, "ImpStreamOn failed\n");
            return -1;
        }
    }
    if (jio_cfg.system_config.led_enabled)
    {
        //LUX_HAL_LedSetStatus(LUX_HAL_LED_GREEN, LUX_HAL_LED_ON);  //humy
    }
    else
    {
        //LUX_HAL_LedSetStatus(LUX_HAL_LED_GREEN, LUX_HAL_LED_OFF); //humy
    }
    // set_night_vision_mode(jio_cfg.image_config.daynight);
    int width_value;
    int height_value;
#if 1
    /*通道0的分辨率设置为默认值*/
    int fsChnNum =0;
    uint32_t fps_den = 1;
    uint32_t fps_num = 0;
    uint32_t impvi_num = 0;
    MYTRACE("channel_0 resolution_0:[%s] bitrate:[%d] fps:[%d] \n", jio_cfg.image_config.resolution_0, jio_cfg.image_config.bitrate_0, jio_cfg.image_config.fps_0);
    sscanf(jio_cfg.image_config.resolution_0, "%dx%d", &width_value, &height_value);

    ret = LUX_Video_Change_Resolution(fsChnNum, width_value, height_value);
    if (0 != ret)
    {
        MYERROR("video change resolution failed,error [0x%x]\n", ret);
    }

    LUX_Video_Func_Set_Bitrate(fsChnNum, jio_cfg.image_config.bitrate_0);
    if (0 != ret)
    {
        MYERROR("video change bitrate failed failed,error [0x%x]\n", ret);
    }
    fps_num = jio_cfg.image_config.fps_0;
    impvi_num = 0;
    LUX_Video_Func_Set_Fps(0, &fps_num, &fps_den);
    if (0 != ret)
    {
        MYERROR("video change Fps failed failed,error [0x%x]\n", ret);
    }

    /*通道1的分辨率设置设置为默认值*/
    fsChnNum = 1;
    MYTRACE("channel_1 resolution_0:[%s] bitrate:[%d] fps:[%d] \n", jio_cfg.image_config.resolution_1, jio_cfg.image_config.bitrate_1, jio_cfg.image_config.fps_1);
    sscanf(jio_cfg.image_config.resolution_1, "%dx%d", &width_value, &height_value);

    ret = LUX_Video_Change_Resolution(fsChnNum, width_value, height_value);
    if (0 != ret)
    {
        MYERROR("video change resolution failed,error [0x%x]\n", ret);
    }

    LUX_Video_Func_Set_Bitrate(fsChnNum, jio_cfg.image_config.bitrate_1);
    if (0 != ret)
    {
        MYERROR("video change bitrate failed failed,error [0x%x]\n", ret);
    }
    fps_num = jio_cfg.image_config.fps_1;
    impvi_num = 1;
    LUX_Video_Func_Set_Fps(0, &fps_num, &fps_den);
    if (0 != ret)
    {
        MYERROR("video change Fps failed failed,error [0x%x]\n", ret);
    }
#endif
 
}

//------------------------------------------------------------------------------
static idip_status_code_t
config_get_handler_impl(idip_stream_config_t* stream,
                        const char* target,
                        void* udata)
{
    printf("enter config_get_handler_impl() \n");
    sleep(1);
    IDIP_UNUSED(target);
    IDIP_UNUSED(udata);

    // Create meta information helper. We're doing it using global variables.  创建元信息助手。我们用全局变量来做。
    // Production code can use udata argument to pass additional data here. 生产代码可以使用udata参数在这里传递附加数据 
    idip_cfg_all_meta_t all_meta = {
        .meta = cfg_meta,
        .meta_count = ARRAY_SIZE(cfg_meta),
    };

    idip_mutex_lock(&cfg_mutex);
    printf("\n********************* idip_mutex_lock is success *********************\n");

    idip_status_code_t result = idip_cfg_meta_put_values_to_stream(all_meta, stream, 0);
    idip_mutex_unlock(&cfg_mutex);
    printf("\n********************* idip_mutex_unlock is success *********************\n");
    return result;
}
//------------------------------------------------------------------------------
static int sub_stream_should_be_restricted(const char* main_stream_resolution)
{
    return strcmp(main_stream_resolution, ARRAY_GET_LAST(resolutions_0)) == 0;
}
//------------------------------------------------------------------------------
// simple static cache to implement stream restrictions  实现简单的静态缓存流限制
typedef struct {
    idip_cfg_meta_t* meta_resolution_1; // sub stream resolution meta info  子流解析元信息
    idip_cfg_meta_t* meta_fps_0; // main stream FPS meta info  主流FPS元信息
    idip_cfg_meta_t* meta_fps_1; // sub stream FPS meta info     子流FPS元信息
} video_meta_cache_t;

static video_meta_cache_t v_meta_cache = {
    .meta_resolution_1 = NULL,
    .meta_fps_0 = NULL,
    .meta_fps_1 = NULL,
};

static idip_cfg_hook_status_t
cfg_bool_update_hook(const char* target,
                        const char* param_name,
                        bool value,
                        idip_cfg_meta_t* meta,
                        void* hook_user_data)
{
    printf("cfg_bool_update_hook %s meta name:%s, value %d\n", param_name, meta->name, value);

    if (NULL == param_name)
    {
        MYERRORING("param_name is nULL \n");
        return IDIP_CFG_HOOK_OK;
    }

    INT_X ret = LUX_ERR;
    // char tmp[8];
    // sprintf(tmp, "%d", value);
    if (strstr(param_name, "led/enabled"))
    {
        // ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "system_config", "led_enabled", tmp);
        // if (LUX_OK != ret)
        // {
        //     IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
        // }
        g_configData->system_config.led_enabled = value;
        MYTRACEING("led g_configData->g_JioConfig.jio_config change:%d\n", g_configData->system_config.led_enabled);
        // if(g_configData->system_config.led_enabled)
        // {
        //     LUX_HAL_LedSetStatus(LUX_HAL_LED_GREEN, LUX_HAL_LED_ON);
        // }
        // else 
        // {
        //     LUX_HAL_LedSetStatus(LUX_HAL_LED_GREEN, LUX_HAL_LED_OFF);
        // }
        //set_light_onoff(g_configData->system_config.led_enabled);
    }
    else if (strstr(param_name, "microphone/enabled"))
    {
        // ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "media_config_audio", "microphone_enabled", tmp);
        // if (LUX_OK != ret)
        // {
        //     IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
        // }
        g_configData->media_config.audio_config.microphone_enabled = value;
        usleep(50);
        if (!g_configData->media_config.audio_config.microphone_enabled)
        {
            //关闭麦克风
            LUX_AI_Stop();
        }
        else
        {
            //打开麦克风
            LUX_AI_Start();
        }
    }
    else if (strstr(param_name, "motion_detector/enabled"))
    {
        // ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "algorithm_config", "motion_detector_enabled", tmp);
        // if (LUX_OK != ret)
        // {
        //     IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
        // }
        g_configData->alg_config.motion_detect_enabled = value;
        if (g_configData->alg_config.motion_detect_enabled)
        {
            //开启启动侦测
            LUX_IVS_PD_StartAlgo();
        }
        else
        {
            //停止移动侦测
            LUX_IVS_PD_StopAlgo();
        }

    }
    else if (strstr(param_name, "sound_detection/enabled"))
    {
        // ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "algorithm_config", "sound_detection_enabled", tmp);
        // if (LUX_OK != ret)
        // {
        //     IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
        // }
        g_configData->alg_config.sound_detect_enabled = value;
        if (g_configData->alg_config.sound_detect_enabled)
        {
            if (g_configData->media_config.audio_config.microphone_enabled)
            {
                //开启声音/哭声检测
                set_cry_detect_onoff(g_configData->alg_config.sound_detect_enabled);
            }
        }
        else
        {
            //关闭声音/哭声检测
            set_cry_detect_onoff(g_configData->alg_config.sound_detect_enabled);
        }
    }
    else if (strstr(param_name, "osd/datetime/enabled"))
    {
        g_configData->image_config.osd_show_time = value;
        //jio_cfg.image_config.osd_show_time = value;
        MYTRACEING("jio set osd show time:[%d] \n", g_configData->image_config.osd_show_time);
       // ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "image_config", "osd_show_time", g_configData->image_config.osd_show_time ==1?"1":"0");
        //???
    }
    return IDIP_CFG_HOOK_OK;
}

static idip_cfg_hook_status_t
cfg_int64_update_hook(const char* target, const char* param_name,
                      const int64_t value, idip_cfg_meta_t* meta,
                       void* hook_user_data)
{
    MYTRACEING("cfg_int64_update_hook %s meta name:%s, value %lld\n", param_name, meta->name, value);

    if (NULL == param_name)
    {
        MYERRORING("param_name is nULL \n");
        return IDIP_CFG_HOOK_OK;
    }

    INT_X ret = LUX_ERR;
    char tempstr[8];
    sprintf(tempstr, "%d", value);

    ENCODER_CHN_ATTR_ST* pChnAttr = NULL;
    IMPEncoderFrmRate* pstFps = NULL;
    uint32_t fps_num;
    uint32_t fps_den;
    uint32_t bitrate_tmp;
    uint32_t fps_tmp;
    uint32_t impvi_num;

    if (strstr(param_name, "sound_detection/sensitivity"))
    {
        // ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "algorithm_config", "sound_detection_sensitivity", tempstr);
        // if (LUX_OK != ret)
        // {
        //     IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
        // }
        g_configData->alg_config.sound_detect_sensitivity = value;
        printf("led g_configData->g_JioConfig.jio_config change:%d\n", g_configData->alg_config.sound_detect_sensitivity);
        if (g_configData->alg_config.sound_detect_sensitivity <= 50)
        {
            set_cry_detect_sensi("0");//0 低灵敏度 1 高灵敏度
        }
        else
        {
            set_cry_detect_sensi("1");
        }
    }
    /*通道0帧率设置*/
    else if (strstr(param_name, "streams/0/fps"))
    {
        // ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "image_config", "fps_0", tempstr);
        // if (LUX_OK != ret)
        // {
        //     IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
        // }
        g_configData->image_config.fps_0 = value;
        printf("set streams/0/fps is ENTER\n");
        fps_num = value;
        fps_den = 1;
        impvi_num = 0;
        LUX_Video_Func_Set_Fps(impvi_num,&fps_num, &fps_den);
        MYTRACE("set streams/0/fps is success\n");
        LUX_Video_Func_Get_Fps(&fps_tmp, &fps_den);
        printf("Get streams/0/fps is %d\n", fps_tmp);
    }
    /*通道0码率设置*/
    else if (strstr(param_name, "streams/0/bitrate"))
    {
        // ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "image_config", "bitrate_0", tempstr);
        // if (LUX_OK != ret)
        // {
        //     IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
        // }
        g_configData->image_config.bitrate_0 = value;
        pChnAttr = &g_VEncAttr->encGrpAttr[0].encChnAttr[0];

        // IMP_Encoder_SetChnBitRate(pChnAttr->chnId, value * 1000, 640);
        LUX_Video_Func_Set_Bitrate(pChnAttr->chnId, value * 1000);
        MYTRACE("set streams/0/bitrate is success\n");
        LUX_Video_Func_Get_Bitrate(pChnAttr->chnId, &bitrate_tmp);
        MYTRACE("Get streams/0/bitrate is %ld\n", bitrate_tmp);
    }
    /*通道1帧率设置*/
    else if (strstr(param_name, "streams/1/fps"))
    {
        // ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "image_config", "fps_1", tempstr);
        // if (LUX_OK != ret)
        // {
        //     IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
        // }
        g_configData->image_config.fps_1 = value;
        fps_num = value;
        fps_den = 1;
        impvi_num = 1;
        LUX_Video_Func_Set_Fps(impvi_num, &fps_num, &fps_den);
        MYTRACE("set streams/1/fps is success\n");
        LUX_Video_Func_Get_Fps(&fps_tmp, &fps_den);
        MYTRACE("Get streams/1/fps is %d\n", fps_tmp);
    }
    /*通道1码率设置*/
    else if (strstr(param_name, "streams/1/bitrate"))
    {
        // ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "image_config", "bitrate_1", tempstr);
        // if (LUX_OK != ret)
        // {
        //     IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
        // }
        g_configData->image_config.bitrate_1 = value;
        pChnAttr = &g_VEncAttr->encGrpAttr[0].encChnAttr[1];
        LUX_Video_Func_Set_Bitrate(pChnAttr->chnId, value * 1000);
        // IMP_Encoder_SetChnBitRate(pChnAttr->chnId, value, 640);
        MYTRACE("set streams/1/bitrate is success\n");
        LUX_Video_Func_Get_Bitrate(pChnAttr->chnId, &bitrate_tmp);
        MYTRACE("Get streams/1/bitrate is %ld\n", bitrate_tmp);
    }

    return IDIP_CFG_HOOK_OK;
}

static idip_status_code_t
cfg_read_hook(const char* target,
                        const idip_cfg_meta_t* meta,
                        struct idip_stream_config* out_stream,
                        void* hook_user_data)
{
    printf("====readcfg name%s, \n====", meta->name);

    return IDIP_OK;
}

static idip_cfg_hook_status_t
cfg_str_update_hook(const char* target, const char* param_name,
                      const char* value, idip_cfg_meta_t* meta,
                      void* hook_user_data)
{
    MYTRACE("cfg_str_update_hook %s meta name:%s, value %s\n", param_name, meta->name, value);
    int i;

    if (NULL == param_name || NULL == value)
    {
        MYERRORING("data error, param_name or value is NULL, param_name:[%p] value:[%p] \n", param_name, value);
        return IDIP_CFG_HOOK_OK;
    }

    INT_X ret = LUX_ERR;
    if (strstr(param_name, "night_vision/mode"))
    {
        // ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "image_config", "daynight", value);
        // if (LUX_OK != ret)
        // {
        //     IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
        // }
        //TODO: 保存设置
            int i = 0;
            for (i = 0; i < sizeof(nightvision_modes) / sizeof(char*); i++)
            {
                if (0 == strcmp(nightvision_modes[i], value))
                {
                 g_configData->image_config.daynight = nightvision_modes[i];
                    break;
                }
            }
         set_night_vision_mode(value);
    }
    else if (strstr(param_name, "general/timezone"))
    {
        set_time_zone(value);
    }
    else if (strstr(param_name, "general/power_freq")) //电源频率
    {
        for (i = 0; i < sizeof(idip_cfg_range_power_freq) / sizeof(char*); i++)
        {
            if (0 == strcmp(idip_cfg_range_power_freq[i], value))
            {
                g_configData->system_config.power_freq = idip_cfg_range_power_freq[i];
                //LUX_INIParse_SetKey(LUX_CONFIG_FILE, "system_config", "power_freq", g_configData->system_config.power_freq);
                break;
            }
        }
    }
    else if (strstr(param_name, "stream/0/codec"))
    {
        for (i = 0; i < sizeof(codecs) / sizeof(char*); i++)
        {
            if (0 == strcmp(codecs[i], value))
            {
                g_configData->media_config.video_config.codec_0 = codecs[i];
                break;
            }
        }
    }
    else if (strstr(param_name, "stream/1/codec"))
    {
        for (i = 0; i < sizeof(codecs) / sizeof(char*); i++)
        {
            if (0 == strcmp(codecs[i], value))
            {
                g_configData->media_config.video_config.codec_1 = codecs[i];
                break;
            }
        }
    }
    else if (strstr(param_name, "stream/0/acodec"))
    {
        for (i = 0; i < sizeof(acodecs) / sizeof(char*); i++)
        {
            if (0 == strcmp(acodecs[i], value))
            {
                g_configData->media_config.audio_config.acodec_0 = acodecs[i];
                break;
            }
        }
    }
    else if (strstr(param_name, "stream/1/acodec"))
    {
        for (i = 0; i < sizeof(acodecs) / sizeof(char*); i++)
        {
            if (0 == strcmp(acodecs[i], value))
            {
                g_configData->media_config.audio_config.acodec_1 = acodecs[i];
                break;
            }
        }
    }

    return IDIP_CFG_HOOK_OK;
}

void set_time_zone(const char* timezone_info)
{
    MYTRACE("jio set timezone:%s offset:%d\n", timezone_info, lux_get_timezone_offset(timezone_info));

    int i;

    for (i = 0; i < sizeof(tz_offsets)/sizeof(char*); i++)
    {
        if (0 == strcmp(tz_offsets[i], timezone_info))
        {
            g_configData->system_config.timezone = tz_offsets[i];
            g_configData->system_config.timezone_offset_s = lux_get_timezone_offset(timezone_info);
            setTimeStampdDiff(g_configData->system_config.timezone_offset_s);
            //LUX_INIParse_SetKey(LUX_CONFIG_FILE, "system_config", "timezone", timezone_info);
            break;
        }
    }

    if (i == sizeof(tz_offsets))
    {
        MYERROR("timezone[%s] format is wrong \n", timezone_info);
    }

}

void set_light_onoff(BOOL_X light_on_off)
{
    INT_X ret = -1;
    CHAR_X tmpStr[8] = { 0 };

    printf("set light_on_off:%d \r\n", light_on_off);

    /* Status indicator,BOOL type,true means on,false means off */
    if (light_on_off)
    {
        LUX_HAL_LedSetStatus(LUX_HAL_LED_GREEN, LUX_HAL_LED_ON);
    }
    else
    {
        LUX_HAL_LedSetStatus(LUX_HAL_LED_GREEN, LUX_HAL_LED_OFF);
        LUX_HAL_LedSetStatus(LUX_HAL_LED_RED, LUX_HAL_LED_OFF);
        //LUX_HAL_LedDeinit();
    }

    ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "hal", "led", light_on_off? "true" : "false");
    if (0 != ret)
    {
        printf("LUX_INIParse_SetKey failed!, error num [0x%x] ", ret);
    }
}

void set_night_vision_mode(const char* value)
{
    if (strncmp(value, "on", 2) == 0)
    {
        LUX_DayNight_SetAutoMode(LUX_DAYNIGHT_MANUAL_MODE);
        LUX_DayNight_SetMode(LUX_DAYNIGHT_NIGHT_MODE);
    }
    else if (strncmp(value, "off", 3) == 0)
    {
        LUX_DayNight_SetAutoMode(LUX_DAYNIGHT_MANUAL_MODE);
        LUX_DayNight_SetMode(LUX_DAYNIGHT_DAY_MODE);
    }
    else if (strncmp(value, "auto", 4) == 0)
    {
        LUX_DayNight_SetAutoMode(LUX_DAYNIGHT_AUTO_MODE);
    }
}

extern INT_X LUX_AI_SetCryDet(BOOL_X bOpen);
extern INT_X LUX_INIParse_SetKey(const PCHAR_X pFilename, const PCHAR_X pTitle, const PCHAR_X pKey, PCHAR_X pValue);
void set_cry_detect_onoff(const int on_off)
{
    int ret = -1;

    printf("set cry_on_off:%d \r\n", on_off);

    /*在隐私模式下不执行相关动作*/
    // if(TRUE == IPC_APP_get_sleep_mode())
    // {
    //     printf("WARNING: set sd_cry_onoff:%d invalied in the sleep mode!\r\n", on_off);

    //     ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "algorithm_config", "cry_onoff", on_off ? "1" : "0");
    //     if (0 != ret)
    //     {
    //         printf("ERROR:LUX_INIParse_SetKey ret:%#x", ret);
    //     }

    //     return;
    // }

    /* Status indicator,BOOL type,true means on,false means off */
    LUX_AI_SetCryDet(on_off);

    ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "algorithm_config", "cry_onoff", on_off ? "1" : "0");
    if (0 != ret)
    {
        printf("cry_onoff failed,error [0x%x]\n", ret);
    }
    g_bcryDetctAlarmSong = on_off;

    return;
}

void set_cry_detect_sensi(const char* value)
{
    int ret;

    ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "algorithm_config", "sound_det_sensi", (value ? "1" : "0"));
    if (0 != ret)
    {
        printf("LUX_INIParse_SetKey failed!, error num [0x%x] ", ret);
    }
    int tmpValue = atoi(value);
    g_cryDetectSensitive = tmpValue ? JIO_SOUND_DET_SENSI_HIGH : JIO_SOUND_DET_SENSI_LOW;

    printf("set IPC_APP_set_cry_sensi:%d g_cryDetectSensitive:%f\r\n", tmpValue, g_cryDetectSensitive);

    return;
}
//------------------------------------------------------------------------------
// Configuration parameters dependency example.   配置参数依赖示例。
// Let the solution has some CPU limits. When user selects maximal resolution
// for the main stream we should  main stream FPS up to 25fps, restrict
// sub stream FPS to 15, and sub stream resolution to "640x480". This callback
// does these restrictions.

static idip_cfg_hook_status_t
cfg_resolution_0_hook(const char* target, const char* param_name,
                      const char* value, idip_cfg_meta_t* meta,
                      void* hook_user_data)
{
    printf("cfg_resolution_0_hook %s meta name:%s, value %s\n", param_name, meta->name, value);

    int i;

    if (NULL == param_name || NULL == value)
    {
        MYERRORING("data error, param_name or value is NULL, param_name:[%p] value:[%p] \n", param_name, value);
        return IDIP_CFG_HOOK_OK;
    }

    INT_X ret = LUX_ERR;
    if (strstr(param_name, "streams/0/resolution"))
    {
        //ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "current", "resolution_0", value);
        usleep(200*1000);
    }

    // INT_X ret = LUX_ERR;
    // if (strstr(param_name, "resolution/sensitivity"))
    // {
    //     ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "sound_detection", "sensitivity", value);
    //     if (LUX_OK != ret)
    //     {
    //         IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
    //     }
    // }


    printf("*************cfg_resolution_0_hook***************\n");
    // Complexity of idip_cfg_meta_find() is O(n). We cache here          idip cfg元find()的复杂度是O(n)。我们在这里缓存
    // some results of its look ups for performance reasons.                    由于性能原因，它的一些查找结果。
    if (!v_meta_cache.meta_fps_0) {
        idip_cfg_all_meta_t all_meta = {
            .meta = cfg_meta,
            .meta_count = ARRAY_SIZE(cfg_meta),
        };

        v_meta_cache.meta_resolution_1 = idip_cfg_meta_find(all_meta, "streams/1/resolution");
        v_meta_cache.meta_fps_0 = idip_cfg_meta_find(all_meta, "streams/0/fps");
        v_meta_cache.meta_fps_1 = idip_cfg_meta_find(all_meta, "streams/1/fps");
    }

    if (!v_meta_cache.meta_fps_0 || !v_meta_cache.meta_fps_1 ||
        !v_meta_cache.meta_resolution_1)
    {
        fprintf(stdout, "ERROR: configuration meta information was not found\n");
        return IDIP_CFG_HOOK_ERR;
    }

    // if (sub_stream_should_be_restricted(value)) { // set restrictions  设置限制
    //     MYTRACE("cfg_resolution_0_hook:ENTER-------0\n");
    //     idip_cfg_meta_limit_range(v_meta_cache.meta_fps_0, ARRAY_SIZE(fps_list_0) - 1);
    //     MYTRACE("cfg_resolution_0_hook:ENTER-------1\n");
    //     idip_cfg_meta_limit_range(v_meta_cache.meta_fps_1, 3);
    //     MYTRACE("cfg_resolution_0_hook:ENTER-------2\n");
    //     idip_cfg_meta_limit_range(v_meta_cache.meta_resolution_1, 3);
    //     MYTRACE("cfg_resolution_0_hook:ENTER-------3\n");
    //     // do apply changes in a codec   在编解码器中应用更改
    //     // ...
    // }
    // else { // cancel restrictions   取消限制
    idip_cfg_meta_limit_range(v_meta_cache.meta_fps_0, ARRAY_SIZE(fps_list_0));
    idip_cfg_meta_limit_range(v_meta_cache.meta_fps_1, ARRAY_SIZE(fps_list_1));
    idip_cfg_meta_limit_range(v_meta_cache.meta_resolution_1, ARRAY_SIZE(resolutions_1));
    // }

    // do apply changes in a codec to set up new resolution  是否在编解码器中应用更改以设置新的分辨率
    // ...
    push_live_stream_enable_main = 0;/*阻塞推流线程,等待推流线程进入阻塞*/
    push_live_stream_enable_sub = 0;/*阻塞推流线程,等待推流线程进入阻塞*/

    usleep(500*1000);

    while (0 == Semaphore_Pend_flag_main || 0 == Semaphore_Pend_flag_sub)
    {
        usleep(10 * 1000);
    }

    int width_value;
    int height_value;
    int fsChnNum;
    sscanf(value, "%dx%d", &width_value, &height_value);
    MYTRACE("width_value:[%d] height_value:[%d]\n", width_value, height_value);

    if (strstr(param_name, "streams/0/resolution"))
    {
        fsChnNum = 0;
        // ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "image_config", "resolution_0", value);
        // if (LUX_OK != ret)
        // {
        //     IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
        // }
        for (i = 0; i < sizeof(resolutions_0) / sizeof(char*); i++)
        {
            if (0 == strcmp(resolutions_0[i], value))
            {
                g_configData->image_config.resolution_0 = resolutions_0[i];
                break;
            }
        }
    }
    else if (strstr(param_name, "streams/1/resolution"))
    {
        fsChnNum = 1;
        // ret = LUX_INIParse_SetKey(LUX_CONFIG_FILE, "image_config", "resolution_1", value);
        // if (LUX_OK != ret)
        // {
        //     IMP_LOG_ERR(TAG, "ERROR:LUX_INIParse_SetKey ret:%#x", ret);
        // }
        for (i = 0; i < sizeof(resolutions_1) / sizeof(char*); i++)
        {
            if (0 == strcmp(resolutions_1[i], value))
            {
                g_configData->image_config.resolution_1 = resolutions_1[i];
                break;
            }
        }
    }

    LUX_Video_Change_Resolution(fsChnNum, width_value, height_value);

    uint32_t picWidth;
    uint32_t picHeight;
    ENCODER_CHN_ATTR_ST* pChnAttr = NULL;

    pChnAttr = &g_VEncAttr->encGrpAttr[0].encChnAttr[0];
    LUX_Video_Fnuc_Get_Resoulution(pChnAttr->chnId, &picWidth, &picHeight);
    MYTRACE("resloution Width:[%d], height:[%d]\n", picWidth, picHeight);

    if (0) {
        demo_options_t* opts = (demo_options_t*)hook_user_data;
        fprintf(stdout, "Reconfiguring the cdi-target\n");
        //avpkt_stream_source_reconfigure(opts->live_source_streams[0][PROFILE_HIGH]);
    }
    usleep(10 * 1000);
    push_live_stream_enable_main = 1;
    push_live_stream_enable_sub = 1;
    Semaphore_Pend_flag_main = 0;
    Semaphore_Pend_flag_sub = 0;
    Semaphore_Post(&g_liveVideoMainSem);
    Semaphore_Post(&g_liveVideoSubSem);
    
    return IDIP_CFG_HOOK_OK;
}

// static idip_cfg_hook_status_t
// cfg_bitrate_0_hook(const char* target, const char* param_name,
//                       const char* value, idip_cfg_meta_t* meta,
//                       void* hook_user_data)
// {
//     MYTRACE("cfg_bitrate_0_hook is enter\n");
// }

// static idip_cfg_hook_status_t
// cfg_bitrate_0_hook(const char* target,
//                        const char* param_name,
//                        int64_t value,
//                        idip_cfg_meta_t* meta,
//                        void* hook_user_data)
// {
//     MYTRACE("cfg_bitrate_0_hook is enter\n");
// }
//------------------------------------------------------------------------------
static idip_status_code_t
config_update_handler_impl(idip_stream_config_t* stream,
                           const char* target,
                           const idip_cfg_param_t** params,
                           size_t params_count,
                           void* udata)
{
    printf("enter config_update_handler_impl() \n");
    idip_cfg_all_meta_t all_meta = {
        .meta = cfg_meta,
        .meta_count = ARRAY_SIZE(cfg_meta),
    };
    idip_status_code_t result = IDIP_OK;
    (void)udata;

    idip_mutex_lock(&cfg_mutex);
    printf("\n********************* idip_mutex_lock1 is success *********************\n");
    // Call helper. It will update all variables, which were described
    // during cfg_meta initialization. The helper will call registered hooks
    // (w_hook) BEFORE update of variables.
    result = idip_cfg_meta_handle_updates(all_meta, target, params, params_count, udata);

    // Put only modified values.
    result = idip_cfg_meta_put_values_to_stream(all_meta, stream, 1);
    LUX_Config_Set_AllConfig(g_configData);
    idip_mutex_unlock(&cfg_mutex);

    printf("\n********************* idip_mutex_unlock1 is success *********************\n");
    return result;
}

#if defined(USE_ALSA_BACKEND)
//------------------------------------------------------------------------------
static idip_status_code_t
playback_handler_impl(const char* target,
                      const audio_stream_info_t* info,
                      const void* data,
                      size_t size,
                      void* udata)
{
    IDIP_UNUSED(target); // disable warning

    demo_options_t* options = udata;
    if (!options || !options->playback) {
        return IDIP_GENERIC_ERROR;
    }

    // set up decoder by info's settings if needed and
    // play audio stream data
    playback_play(options->playback, info, data, size);

    return IDIP_OK;
}
#endif /* USE_ALSA_BACKEND */
//------------------------------------------------------------------------------
// helper method for ptz_handler()
static const char* ptz_cmd_to_str(ptz_command_type_t cmd, const char* def)
{
    switch (cmd) {
    case IDIP_PTZ_HOME:     return "HOME";
    case IDIP_PTZ_ZOOM_IN:  return "ZOOM IN (Increase zoom factor)";
    case IDIP_PTZ_ZOOM_OUT: return "ZOOM OUT (Decrease zoom factor)";
    case IDIP_PTZ_LEFT:     return "LEFT (yaw ckw)";
    case IDIP_PTZ_RIGHT:    return "RIGHT (yaw ccw)";
    case IDIP_PTZ_UP:       return "UP (pitch up)";
    case IDIP_PTZ_DOWN:     return "DOWN (pitch down)";
    default: return def;
    }
}
//------------------------------------------------------------------------------
static idip_status_code_t
ptz_handler_impl(const char* target, const idip_ptz_command_t* cmd, void* udata)
{
    IDIP_UNUSED(udata);
    printf("PTZ command %s for target %s\n",
           ptz_cmd_to_str(cmd->command, "UNKNOWN"), (target ? target : ""));
    LUX_HAL_MOTOR_MOV_DIRECTION_EN dir;
    switch (cmd->command)
    {
    case IDIP_PTZ_UP:
        dir = LUX_HAL_MOTOR_UP;
        break;
    case IDIP_PTZ_LEFT:
        dir = LUX_HAL_MOTOR_LEFT;
        break;
    case IDIP_PTZ_RIGHT:
        dir = LUX_HAL_MOTOR_RIGHT;
        break;
    case IDIP_PTZ_DOWN:
        dir = LUX_HAL_MOTOR_DOWN;
        break;
    default:
        return IDIP_INVALID_ARGUMENTS;
    }
    int ret = LUX_HAL_Motor_CtrlMovOneStep(dir);
    usleep(1000 * 1000ul);
    printf("PTZ command %s for target %s finished\n",
           ptz_cmd_to_str(cmd->command, "UNKNOWN"), (target ? target : ""));
    return IDIP_OK;
}
//------------------------------------------------------------------------------
static idip_status_code_t
snapshot_handler_impl(idip_stream_snapshot_t* stream,
                      const idip_stream_snapshot_args_t* args,
                      void* udata)
{
    //自行处理args->target和args->profile。我们在这里忽略了它们。
    IDIP_UNUSED(args);

    MYWARN("=========================================================\n");
    MYWARN("enter snapshot_handler_impl \n");
    MYWARN("=========================================================\n");

    demo_options_t* options = udata;
    if (!options->snapshot_path) {
        printf("Snapshot file is not set\n");
        return IDIP_GENERIC_ERROR;
    }
    //准备要发送的数据。在这个例子中，我们从文件中读取它
    FILE* f = fopen(options->snapshot_path, "rb+");
    if (!f) {
        printf("Can't open snapshot file %s: %s. \n",
               options->snapshot_path, strerror(errno));
        return IDIP_GENERIC_ERROR;
    }
    fseek(f, 0L, SEEK_END);
    size_t data_size = ftell(f);
    fseek(f, 0L, SEEK_SET);

    void* data = mmap(NULL, data_size, PROT_READ, MAP_PRIVATE, fileno(f), 0);
    if (MAP_FAILED == data) {
        printf("Can't mmap snapshot file %s: %s. \n",
               options->snapshot_path, strerror(errno));
        fclose(f);
        return IDIP_GENERIC_ERROR;
    }

    // fake video info
    video_stream_info_t info = {
        .codec = VIDEO_CODEC_MJPEG,
        // .codec = VIDEO_CODEC_TEXT,
        .width = 0,
        .height = 0
    };



    idip_status_code_t rc;
    rc = idip_stream_snapshot_set_video_info(stream, &info, IDIP_INFINITE_TIMEOUT);
    if (rc != IDIP_OK) {
        goto done;
    }

    rc = idip_stream_snapshot_put_video_frame(stream, time_ms(), data, data_size, IDIP_INFINITE_TIMEOUT);
    if (rc != IDIP_OK) {
        goto done;
    }

done:
    munmap(data, data_size);
    fclose(f);

    return IDIP_OK;
}
//------------------------------------------------------------------------------
static void restore_factory_settings()
{
    unlink(PERSIET_PATH);
    unlink(QR_PARSE_FILE);
    LUX_BASE_System_CMD("reboot");
}

static idip_status_code_t
persistent_data_get_impl(idip_stream_persistent_data_t* stream,
                    void* udata)
{
    const size_t DEMO_STORAGE_MAX_SIZE = 1024 * 1024; // 1M for example

    demo_options_t* options = udata;
    if (!options) {
        printf("[ERROR] Invalid arguments\n");
        return IDIP_GENERIC_ERROR;
    }
    struct stat st_stat;

    // 获取文件的状态信息
    if (stat(options->persistent_storage, &st_stat) != 0) return IDIP_GENERIC_ERROR;

    if (st_stat.st_size == 2)
    {
        restore_factory_settings();
    }
    //注意！我们在这里使用一个简单的助手来减少代码。
    //实际实现必须考虑摄像机/DVR的具体情况。
    //但它们可以引用助手实现。TODO:jffs2可以暂时用这个，squashfs必须自己实现
    return idip_persistent_data_get_default_implementation_helper(stream,
                options->persistent_storage, DEMO_STORAGE_MAX_SIZE);
}
//------------------------------------------------------------------------------
static idip_status_code_t
persistent_data_set_impl(const void* data, size_t size, void* udata)
{
    demo_options_t* options = udata;
    if (!options) {
        printf("[ERROR] Invalid arguments\n");
        return IDIP_GENERIC_ERROR;
    }

    //注意！我们在这里使用一个简单的助手来减少代码。
    //实际实现必须考虑摄像机/DVR的具体情况。
    //但它们可以引用助手实现。TODO:jffs2可以暂时用这个，squashfs必须自己实现
    return idip_persistent_data_set_default_implementation_helper(options->persistent_storage, data, size);
}
//------------------------------------------------------------------------------
//该示例显示了如何发送有关附近无线网络的信息
/*
wlp3s0    Scan completed :
          Cell 01 - Address: 94:77:2B:0A:96:10
                    Channel:1
                    Frequency:2.412 GHz (Channel 1)
。。。。。。。
。。。。。。。
*/
//------------------------------------------------------------------------------
#if 1//cxj test
static idip_status_code_t
wireless_scan_impl(idip_stream_wireless_scan_t* stream, void* udata)
{
    return IDIP_OK;
}

//------------------------------------------------------------------------------

static idip_status_code_t
wireless_setup_impl(const idip_ap_connect_parameters_t* settings, void* udata)
{
    // wifi连接
    //printf("Request to connect to WiFi network %s. Password is %s\n",
    //g_ssidStr,
    //g_passwdStr);
    return IDIP_OK;
}
#endif

//------------------------------------------------------------------------------
static idip_status_code_t
system_notify_impl(const idip_system_notify_query_args_t* args, void* udata)
{
    IDIP_UNUSED(udata);
    if (!args->target || *args->target == '\0')
    {
        printf("\e[1mSystem.Notify: %s\e[0m \n", args->name);
        if (strstr(args->name, "online"))
        {
            //jio_cfg.system_config.online = 1;
            g_configData->system_config.online = 1;
            printf("***********online!!!***********\n");
        }
        else if (strstr(args->name, "offline"))
        {
            //jio_cfg.system_config.online = 0;
            g_configData->system_config.online = 0;

            printf("***********offline!!!***********\n");
        }

    }
    else
        printf("\e[1mSystem.Notify for target \"%s\": %s\e[0m", args->target, args->name);

    return IDIP_OK;
}
//------------------------------------------------------------------------------
static idip_status_code_t
system_update_impl(const idip_system_update_query_args_t* args, void* udata)
{
    IDIP_UNUSED(udata);
    printf("OTA version: \"%s\"\n", ((args && args->version) ? args->version : "unknown"));
    printf("OTA download link: \"%s\"\n", ((args && args->url) ? args->url : "unknown"));
    int verNew[3] = { 0 };
    int verOld[3] = { 0 };
    int rc = sscanf(args->version, "%d.%d.%d", &verNew[0], &verNew[1], &verNew[2]);
    if (rc != 3)
    {
        printf("OTA error: bad version str\n");
        return IDIP_INVALID_ARGUMENTS;
    }
    rc = sscanf(APP_VERSION, "%d.%d.%d", &verOld[0], &verOld[1], &verOld[2]);
    printf("old version: %s\n", APP_VERSION);
    //printf("old: %d.%d.%d, new: %d.%d.%d\n", verOld[0], verOld[1], verOld[2], verNew[0], verNew[1], verNew[2]);
    if (verNew[0] < verOld[0] || verNew[1] < verOld[1] || verNew[2] < verOld[2])
    {
        printf("OTA: not upgrading to old version\n");
        return IDIP_GENERIC_ERROR;
    }
    if (strstr(args->url, "http") || strstr(args->url, "ftp"))
    {
        pthread_t ota_thread_id;
        rc = pthread_create(&ota_thread_id, NULL, ota_upgrade_thread, args->url);
        pthread_detach(ota_thread_id);
        return IDIP_OK;
    }
    else
    {
        printf("OTA: bad url\n");
    }
    return IDIP_GENERIC_ERROR;
}
//------------------------------------------------------------------------------
// fake users database with 1 item
static const char admin_name[] = "admin";
static char       admin_password[32] = "password";

//------------------------------------------------------------------------------
static idip_status_code_t
system_passwd(const idip_user_credentials_t* cred, void* udata)
{
    IDIP_UNUSED(udata);
    if (cred->user) {
        if (0 != strcmp(cred->user, admin_name)) {
            printf("Unknown user: %s\n", cred->user);
            return IDIP_GENERIC_ERROR;
        }
    }
    strncpy(admin_password, cred->password, sizeof(admin_password) - 1);
    printf("Password for '%s' was changed to '%s'\n", admin_name, admin_password);
    return IDIP_OK;
}
//------------------------------------------------------------------------------
static idip_status_code_t
system_get_users(idip_stream_system_get_users_t* stream, void* udata)
{
    IDIP_UNUSED(udata);

    const idip_user_credentials_t cred = {
        .user = (char*)admin_name,
        .password = admin_password
    };

    idip_stream_system_get_users_put_credentials(stream, &cred, 1,
                                                 IDIP_INFINITE_TIMEOUT);

    return IDIP_OK;
}
//------------------------------------------------------------------------------

static idip_status_code_t
system_token_handler(idip_stream_system_token_t* stream, void* user_data)
{
    const size_t kMaxTokenFileSize = 512;
    const uint64_t kQRParsingEmulateDelayMs = 2 * 1000000ul; // delay to emulate qr-code processing
    char json[kMaxTokenFileSize];
    printf("**********system token handler**************\n");
    demo_options_t* opts = user_data;
    assert(opts);
    if (!opts || !opts->qr_file) {
        idip_log_xwarning(__func__, "Invalid arguments or qr file path wasn't specified");
        return IDIP_GENERIC_ERROR;
    }

    // reading a file with json, parse the json array and put the result to the
    // reply stream.
    memset(json, 0, sizeof(json));
    FILE* in = fopen(opts->qr_file, "r");
    if (!in) {
        idip_log_warning("Can't open QR data file %s: %s", opts->qr_file, strerror(errno));
        usleep(kQRParsingEmulateDelayMs);
        return IDIP_GENERIC_ERROR;
    }

    size_t json_len = fread(json, 1, kMaxTokenFileSize, in);
    fclose(in);
    idip_parsed_token_t token = { 0 };
    int rc = idip_parse_cloud_qr_code_token_json(json, json_len, &token);
    if (0 == rc) {
        rc = idip_stream_system_token_put_token(stream, &token);
    }
    else {
        idip_log_warning("QR-code file wasn't recognized");
        rc = IDIP_GENERIC_ERROR;
    }
    idip_parsed_token_destroy(&token); // clear data if any 

    return rc;
}

static void signals_handler(int sig)
{
    printf("Received signal %s\n", strsignal(sig));

    char b;
    switch (sig) {
        //case SIGINT:
    case SIGTERM:
    case SIGQUIT:
    case SIGALRM:
        b = 0;
        write(pipefds[1], &b, sizeof(b));
        break;

    case SIGHUP:
        b = 1;
        write(pipefds[1], &b, sizeof(b));
        break;

    default:
        break;
    }
}
//------------------------------------------------------------------------------
static int setup_notifiers()
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = signals_handler;

    //if (sigaction(SIGINT, &sa, NULL) == -1) {
    //    printf("sigaction(SIGINT): %s\n", strerror(errno));
    //    return -1;
    //}

    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        printf("sigaction(SIGTERM): %s\n", strerror(errno));
        return -1;
    }
    if (sigaction(SIGQUIT, &sa, NULL) == -1) {
        printf("sigaction(SIGQUIT): %s\n", strerror(errno));
        return -1;
    }
    if (sigaction(SIGHUP, &sa, NULL) == -1) {
        printf("sigaction(SIGHUP): %s\n", strerror(errno));
        return -1;
    }

    if (0 != pipe(pipefds)) {
        syscall_print_error("pipe");
        return -1;
    }

    timerfd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (timerfd < 0) {
        syscall_print_error("timerfd_create");
        close(pipefds[0]);
        close(pipefds[1]);
        return -1;
    }
    struct itimerspec timerspec;
    memset(&timerspec, 0, sizeof(timerspec));
    timerspec.it_interval.tv_sec = 1;
    timerspec.it_value.tv_sec = demo_options.first_event_delay_sec;
    timerspec.it_value.tv_nsec = 1;
    if (0 != timerfd_settime(timerfd, 0, &timerspec, NULL)) {
        syscall_print_error("timerfd_settime");
        close(timerfd);
        close(pipefds[0]);
        close(pipefds[1]);
        return -1;
    }

    return 0;
}

static void* live_streamer_thread(void* arg_)
{
    live_streamer_args_t* args = arg_;
    // int nPackets = (int)ARRAY_SIZE(packets);
    INT_X ret = 0;
    LUX_STREAM_CHANNEL_EN channel = LUX_STREAM_MAIN;
    LUX_ENCODE_STREAM_ST videoFrame;
    //LUX_AUDIO_FRAME_ST audioFrame;
    //int64_t start_ts;
    int64_t offset = 0;
    if (args->channel == LUX_STREAM_MAIN)
    {
        Semaphore_Pend(&g_liveVideoSem, SEM_FOREVER);
    }
    else
    {
        Semaphore_Pend(&g_liveVideoSubSem, SEM_FOREVER);
    }
    while (1)
    {
        usleep(40 * 1000);
        if (g_configData->system_config.online == 0 || g_configData->system_config.online_mode == 0) continue;
        //printf("********************* LUX_Video_Encoder_GetStream *********************\n");
        ret = LUX_Video_Encoder_GetStream(args->channel, &videoFrame);
        // printf("\n********************* LUX_Video_Encoder_GetStream is success *********************\n");
        if (0 != ret)
        {
            MYWARN("*************LUX_Video_Encoder_GetStream fail continue************\n");
            usleep(20 * 1000);
            continue;
        }
        /*packets.isKeyFrame = (luxFrame.type == LUX_ENC_SLICE_I)? 1 : 0;
        packets.size = luxFrame.streamLen;
        packets.data = luxFrame.pAddr;
        packets.pts = luxFrame.pts;
        start_ts = luxFrame.timeStamp/1000;*/
        int isKeyFrame = videoFrame.type == LUX_ENC_SLICE_I ? 1 : 0;
        // if (LUX_STREAM_MAIN == channel)
        // {
        //if (packets.isKeyFrame)
        //printf("set frame type:%d pts: %lld size:%d timestamp:%lld\r\n", luxFrame.type, packets.pts, packets.size, start_ts);
        uint64_t frame_time_stamp_ms = time_ms();
        int rc = idip_av_source_push_video(args->av_source,
                                           //videoFrame.timeStamp / 1000 + videoFrame.pts,
                                           frame_time_stamp_ms,
                                           videoFrame.pAddr,
                                           videoFrame.streamLen,
                                           isKeyFrame);
        if (isKeyFrame)
        {
            printf("Keyframe length:%u, data:[%02d %02d %02d %02d %02d] \n", 
                videoFrame.streamLen, 
                ((char*)videoFrame.pAddr)[0], ((char*)videoFrame.pAddr)[1], ((char*)videoFrame.pAddr)[2], ((char*)videoFrame.pAddr)[3], ((char*)videoFrame.pAddr)[4]);
        }
        //offset += MOVIE_DURATION_MS;
        if (rc != IDIP_OK) {
            printf("idip_av_source_push_video() returns %d\n", rc);
            //goto done;
        }


#if 1 //T40移植暂时关闭录像代码  humy
        if (args->channel == LUX_STREAM_MAIN && videoFrame.streamLen > 10 && videoFrame.streamLen < 50*1024*1024)
        {
            int pack_len = sizeof(frame_head_t) + sizeof(video_frame_head_t) + videoFrame.streamLen;
            frame_head_t* one_frame_head = LUX_Record_alloc_packet(FRAME_TYPE_VIDEO, pack_len);
            
            if (one_frame_head)
            {
                //MYTRACE("save video frame len:%u \n", videoFrame.streamLen);
                one_frame_head->frame_data_len = videoFrame.streamLen;
                one_frame_head->frame_head_len = sizeof(video_frame_head_t);
                one_frame_head->frame_type = FRAME_TYPE_VIDEO;
                one_frame_head->timestamp_ms = frame_time_stamp_ms;
                video_frame_head_t* one_video_frame_head = (video_frame_head_t*)((char*)one_frame_head + sizeof(frame_head_t));
                one_video_frame_head->encode = VIDEO_ENCODE_H264;
                one_video_frame_head->frame_len = videoFrame.streamLen;
                one_video_frame_head->frame_type = isKeyFrame ? VIDEO_I_FRAME : VIDEO_P_FRAME;
                char* data = ((char*)one_frame_head) + sizeof(frame_head_t) + sizeof(video_frame_head_t);
                memcpy(data, videoFrame.pAddr, videoFrame.streamLen);

                g_lux_record->data_len += pack_len;
#if 0
                if (one_video_frame_head->frame_type == VIDEO_I_FRAME)
                {
                    printf("%02X %02X %02X %02X %02X frame len:%d\n", data[0], data[1], data[2], data[3], data[3], videoFrame.streamLen);
                }
#endif
            }
            LUX_Record_free_packet(one_frame_head);
        }
#endif

        LUX_Video_Encoder_ReleaseStream(channel, &videoFrame);
        /*memset(&audioFrame, 0, sizeof(audioFrame));
        rc = LUX_AI_GetStream(&audioFrame);
        if (0 != rc)
        {
            usleep(20 * 1000);
            continue;
        }

        rc = idip_av_source_push_audio(args->av_source,
                                        audioFrame.timeStamp/1000 + audioFrame.seq,
                                        audioFrame.pData,
                                        audioFrame.len);
        if(rc != 0)
        {
            printf("idip_av_source_push_audio() returns %d\n", rc);
        }
        LUX_AI_ReleaseStream(&audioFrame);*/

    }
    pthread_exit(0);
done:
    return NULL;
}


#if 1
extern INT_X LUX_AI_CryDet_Init();
static void* thread_live_audio(void* arg)
{
    int ret = 0;
    //MEDIA_FRAME_S pcm_frame = { 0 };
    LUX_AUDIO_FRAME_ST audioFrame;
    live_streamer_args_t* args = args_main;

    int bitrate = 64000;
    int format = 1, sample_rate = 16000, channels = 1, bits_per_sample = 16;

    int input_size;
    uint8_t* input_buf;
    int16_t* convert_buf;
    int aot = 2;
    int afterburner = 1;
    int eld_sbr = 0;
    int vbr = 0;
    HANDLE_AACENCODER handle;
    CHANNEL_MODE mode;
    AACENC_InfoStruct info = { 0 };
    uint8_t* temp_buf;

    switch (channels) {
    case 1: mode = MODE_1;       break;
    case 2: mode = MODE_2;       break;
    case 3: mode = MODE_1_2;     break;
    case 4: mode = MODE_1_2_1;   break;
    case 5: mode = MODE_1_2_2;   break;
    case 6: mode = MODE_1_2_2_1; break;
    default:
        printf("Unsupported WAV channels %d\n", channels);
        return 1;
    }

    if (aacEncOpen(&handle, 0, channels) != AACENC_OK) {
        printf("Unable to open encoder\n");
        return 1;
    }

    if (aacEncoder_SetParam(handle, AACENC_AOT, aot) != AACENC_OK) {
        fprintf(stderr, "Unable to set the AOT\n");
        return 1;
    }
    if (aot == 39 && eld_sbr) {
        if (aacEncoder_SetParam(handle, AACENC_SBR_MODE, 1) != AACENC_OK) {
            fprintf(stderr, "Unable to set SBR mode for ELD\n");
            return 1;
        }
    }
    if (aacEncoder_SetParam(handle, AACENC_SAMPLERATE, sample_rate) != AACENC_OK) {
        fprintf(stderr, "Unable to set the AOT\n");
        return 1;
    }
    if (aacEncoder_SetParam(handle, AACENC_CHANNELMODE, mode) != AACENC_OK) {
        fprintf(stderr, "Unable to set the channel mode\n");
        return 1;
    }
    if (aacEncoder_SetParam(handle, AACENC_CHANNELORDER, 1) != AACENC_OK) {
        fprintf(stderr, "Unable to set the wav channel order\n");
        return 1;
    }
    if (vbr) {
        if (aacEncoder_SetParam(handle, AACENC_BITRATEMODE, vbr) != AACENC_OK) {
            fprintf(stderr, "Unable to set the VBR bitrate mode\n");
            return 1;
        }
    }
    else {
        if (aacEncoder_SetParam(handle, AACENC_BITRATE, bitrate) != AACENC_OK) {
            fprintf(stderr, "fdk-aac Unable to set the bitrate\n");
            return 1;
        }
    }
    if (aacEncoder_SetParam(handle, AACENC_TRANSMUX, TT_MP4_ADTS) != AACENC_OK) {
        fprintf(stderr, "fdk-aac Unable to set the ADTS transmux\n");
        return 1;
    }
    if (aacEncoder_SetParam(handle, AACENC_AFTERBURNER, afterburner) != AACENC_OK) {
        fprintf(stderr, "fdk-aac Unable to set the afterburner mode\n");
        return 1;
    }
    if (aacEncEncode(handle, NULL, NULL, NULL, NULL) != AACENC_OK) {
        fprintf(stderr, "fdk-aac Unable to initialize the encoder\n");
        return 1;
    }
    if (aacEncInfo(handle, &info) != AACENC_OK) {
        fprintf(stderr, "fdk-aac Unable to get the encoder info\n");
        return 1;
    }

    input_size = channels * 2 * info.frameLength;
    input_buf = (uint8_t*)malloc(input_size);
    convert_buf = (int16_t*)malloc(input_size);
    temp_buf = (uint8_t*)malloc(input_size * 3);
    int buf_pos = 0;
    int buf_size = 0;

    uint8_t outbuf[20480];
    AACENC_ERROR err;

    Semaphore_Pend(&g_liveAudioSem, SEM_FOREVER);

#if 0  //暂不启用哭声检测 humy
    /*哭声检测*/
    LUX_AI_CryDet_Init();
    // pcm_frame.type = E_AUDIO_FRAME;
#endif

    while (1)
    {
        AACENC_BufDesc in_buf = { 0 }, out_buf = { 0 };
        AACENC_InArgs in_args = { 0 };
        AACENC_OutArgs out_args = { 0 };
        int in_identifier = IN_AUDIO_DATA;
        int in_size, in_elem_size;
        int out_identifier = OUT_BITSTREAM_DATA;
        int out_size, out_elem_size;
        int i;
        void* in_ptr, * out_ptr;

        usleep(15 * 1000);
        if (g_configData->system_config.online == 0 || g_configData->system_config.online_mode == 0 || g_configData->media_config.audio_config.microphone_enabled == 0) continue;
        memset(&audioFrame, 0, sizeof(audioFrame));
        int rc = LUX_AI_GetStream(&audioFrame);
        if (0 != rc)
        {
            usleep(20 * 1000);
            continue;
        }

        memcpy(temp_buf + buf_size, audioFrame.pData, audioFrame.len);
        buf_size += audioFrame.len;

        //printf("bufsize:%d inputsize:%d \n", buf_size, input_size);

        if (buf_size >= input_size)
        {
            memcpy(input_buf, temp_buf, input_size);
            memcpy(temp_buf, temp_buf + input_size, buf_size - input_size);
            buf_size -= input_size;

            for (i = 0; i < input_size / 2; i++) {
                const uint8_t* in = &input_buf[2 * i];
                convert_buf[i] = in[0] | (in[1] << 8);
            }
            in_ptr = convert_buf;
            in_size = input_size;
            in_elem_size = 2;

            in_args.numInSamples = input_size <= 0 ? -1 : input_size / 2;
            in_buf.numBufs = 1;
            in_buf.bufs = &in_ptr;
            in_buf.bufferIdentifiers = &in_identifier;
            in_buf.bufSizes = &in_size;
            in_buf.bufElSizes = &in_elem_size;

            out_ptr = outbuf;
            out_size = sizeof(outbuf);
            out_elem_size = 1;
            out_buf.numBufs = 1;
            out_buf.bufs = &out_ptr;
            out_buf.bufferIdentifiers = &out_identifier;
            out_buf.bufSizes = &out_size;
            out_buf.bufElSizes = &out_elem_size;

            out_args.numOutBytes = 0;
            err = -1;

            err = aacEncEncode(handle, &in_buf, &out_buf, &in_args, &out_args);
            if (err == AACENC_OK)
            {
                if (out_args.numOutBytes > 0)
                {
#if 0
                    static FILE* fp1 = NULL;
                    static int saveAudioCount = 0;
                
                    if (saveAudioCount <= 2000)
                    {
                        if (!fp1)
                        {
                            fp1 = fopen("/tmp/tmp.aac", "wb");
                        }

                        if (fp1)
                        {
                            fwrite(outbuf, 1, out_args.numOutBytes, fp1);
                            saveAudioCount++;
                        }
                    }
                    else
                    {
                        if (fp1)
                        {
                            fclose(fp1);
                            fp1 = NULL;
                        }
                    }
#endif

                    rc = idip_av_source_push_audio(args->av_source,
                                        //audioFrame.timeStamp / 1000 + audioFrame.seq,
                                        time_ms(),
                                        //audioFrame.pData,
                                        //audioFrame.len
                                        outbuf,
                                        out_args.numOutBytes
                    );

                    if (rc != 0)
                    {
                        printf("idip_av_source_push_audio() returns %d\n", rc);
                    }
                }
            }
            else
            {
                fprintf(stderr, "fdk-aac Encoding failed, ret:%d\n", err);
            }
        }

        LUX_AI_ReleaseStream(&audioFrame);
    }

    LUX_AI_CryDet_DeInit();

    aacEncClose(handle);

    free(input_buf);
    free(convert_buf);
    free(temp_buf);

    pthread_exit(0);
}
#endif

extern void IPC_APP_AUDIO_BeepInit();
extern void IPC_APP_Notify_LED_Sound_Status_CB(IPC_APP_NOTIFY_EVENT_E notify_event);

int jio_demo_main()
{
    MYTRACEING("=============== enter jio_demo_main() ============= \n");

    //jio_cfg = g_configData_st;
    jio_cfg = *g_configData;
   // memcpy(&jio_cfg.g_JioConfig,&g_configData->g_JioConfig,sizeof(LUX_CONFIG_JIO_CFG_ST));
    // printf("begin init_config() \n");
    //init_config();
    
    //IPC_APP_AUDIO_BeepInit();
    // printf("end init_config() \n");

#ifdef FILE_SOURCE_ENABLE
    avcodec_register_all();
    av_register_all();
    avfilter_register_all();
    printf("\n********************* avfilter_register_all is running *********************\n");

    avpkt_stream_source_t* demo_sources[DVR_CAMERAS_MAX][DVR_PROFILES_MAX] = { {0} };
    idip_av_source_t* av_sources[DVR_CAMERAS_MAX][DVR_PROFILES_MAX] = { {0} };

    assert(ARRAY_SIZE(av_sources) == ARRAY_SIZE(demo_sources));

    memset(demo_sources, 0, sizeof(demo_sources));
    memset(av_sources, 0, sizeof(av_sources));
#endif

    idip_status_code_t err;
    char need_scan = 0;
    //read_token_file(QR_PARSE_FILE, &token);
    err = check_account();
    if (err != IDIP_OK)
    {
        need_scan = 1;
    }

scan_qr:
    if (need_scan)
    {
#if 0
        printf("first setup reading Qrcode \r\n");
        LUX_AO_PlayFile("/system/bin/audio/wait_conn.pcm");
        char qr_str[QR_PARSE_STRLEN];
        while (1)
        {
            pthread_create(&qrcode_thId, NULL, thread_qrcode, qr_str);
            //等待qr识别结果
            pthread_join(qrcode_thId, NULL);
            int rc = idip_parse_cloud_qr_code_token_json(qr_str, QR_PARSE_STRLEN, &token);
            if (0 == rc) {
                break;
            }
            else
            {
                printf("qr json parse fail!\n");
            }
            usleep(1000 * 50);
        }
#endif
    }

    //同步时间 ntp服务器
    printf("NTP sync time begin... \n");
    do { sleep(1); } while (lux_time_ntp_sync("00:00") != 0);
    //lux_time_ntp_cmd_sync();
    printf("NTP sync time end \n");
    usleep(500*1000);

    idip_mutex_init(&cfg_mutex);

    // open pipefds
    if (0 != setup_notifiers())
    {
        exit(EXIT_FAILURE);
    }

    MYTRACE("setup_notifiers is success\n");

    // Initialize library
    if (IDIP_OK != idip_init())
    {
        goto idip_init_fault;
    }
    MYTRACE("idip_init is success\n");


#if defined(USE_ALSA_BACKEND)
    //demo_options.playback = playback_new("default");
    //if (!demo_options.playback) {
    //    goto playback_new_fault;
    //}
#endif /* USE_ALSA_BACKEND */

    //读取序列号
    //memset(g_serial_str, 0, sizeof(g_serial_str));
    //if (read_sn_file(g_serial_str, SERIAL_LEN) != IDIP_OK)
    //{
    //    MYTRACE("error no valid product sn");
    //    goto idip_init_fault;
    //}
    server_config.serial_number = g_serial_str;    

    server_config.model = g_deivce_model_detail;

    //创建IDIP服务器实例和服务器循环。
    //用户可以通过idip_server_stop（）调用终止服务器。
    m_idip_server = create_and_start_idip_server(&server_config,
                                                         &demo_options);

    if (!m_idip_server)
    {
        MYERROR("create_and_start_idip_server error, exit!\n");
        goto server_starting_fault;
    }
    printf("idip_server_init is success\n");
#ifdef FILE_SOURCE_ENABLE
    // "0" - high, "1" - medium, "2" - low
    const char* profile_names[DVR_PROFILES_MAX] = { "0", "1", "2" };

    //创建并初始化所有idip_av_source_t对象，
    //然后将它们绑定到相应的demo_ source。
    //这里，demo_source是音频/视频packetst的源（它从文件中获取数据包）。
    //idip_av_sourcet是一个sinc，用户将帧发送到云。
    size_t i = 0, j = 0;
    for (i = 0; i < ARRAY_SIZE(demo_sources); ++i) {
        char target_name[10];
        snprintf(target_name, sizeof(target_name), "camera%zd", i);

        for (j = 0; j < DVR_PROFILES_MAX; ++j) {
            if (!demo_sources[i][j])
                continue;

            idip_av_source_params_t av_source_params = (idip_av_source_params_t){
                .target_name = target_name,
                .profile_name = profile_names[j],
                .v_info = demo_sources[i][j]->v_info,
                .a_info = demo_sources[i][j]->a_info,
                .limits = demo_sources[i][j]->buffer_limits,
                .event_handler = av_source_event_handler_impl,
                .event_handler_data = demo_sources[i][j]
            };

            err = create_register_and_bind_av_source(server,
                                                     demo_sources[i][j],
                                                     &av_source_params,
                                                     &av_sources[i][j]);
            if (err) {
                // rolling back
                size_t k = 0, l = 0;
                for (k = 0; k <= i; ++k) {
                    for (l = 0; l < DVR_PROFILES_MAX; ++l) {
                        if (!demo_sources[k][l])
                            continue;
                        if (k == i && l >= j)
                            break;
                        avpkt_stream_source_release(demo_sources[k][l], server, av_sources[k][l]);
                        idip_server_av_source_unregister(server, &av_sources[k][l]);
                    }
                }
                goto demo_source_setting_fault;
            }
        }
}
#else
    idip_av_source_params_t av_source_params = (idip_av_source_params_t)
    {
        .target_name = target,
        .profile_name = "0",
        .v_info = (video_stream_info_t)
        {
            .codec = VIDEO_CODEC_H264,
            .width = SENSOR_WIDTH_DEFAULT,
            .height = SENSOR_HEIGHT_DEFAULT
        },
        .a_info = (audio_stream_info_t)
        {
            .codec_type = AUDIO_CODEC_AAC, //AUDIO_CODEC_AAC,
            .sample_format = AUDIO_SAMPLE_FORMAT_FLOATP,
            .channels = 1,
            .sample_rate = IDIP_AUDIO_SAMPLING_FREQ_16K
        },
        .limits = IDIP_AV_LIMITS_INIT(2000, 64, 2),
        .event_handler = av_source_event_handler_impl,
        .event_handler_data = NULL,
    };

    idip_av_source_params_t av_source_params1 = (idip_av_source_params_t)
    {
        .target_name = target,
        .profile_name = "1",
        .v_info = (video_stream_info_t)
        {
            .codec = VIDEO_CODEC_H264,
            .width = SENSOR_WIDTH_SECOND,
            .height = SENSOR_HEIGHT_SECOND
        },
        .a_info = (audio_stream_info_t)
        {
            .codec_type = AUDIO_CODEC_AAC, //AUDIO_CODEC_AAC,
            .sample_format = AUDIO_SAMPLE_FORMAT_FLOATP,
            .channels = 1,
            .sample_rate = IDIP_AUDIO_SAMPLING_FREQ_16K
        },
        .limits = IDIP_AV_LIMITS_INIT(2000, 64, 2),
        .event_handler = av_source_event_handler_impl,
        .event_handler_data = NULL,
    };
    printf("****main stream %dx%d***\n", SENSOR_WIDTH_DEFAULT, SENSOR_HEIGHT_DEFAULT);
    // live_streamer_args.av_source = NULL;

    MYTRACE("begin register main av source \n");
    err = idip_server_av_source_register(m_idip_server, &av_source_params, &args_main->av_source);
    MYTRACE("end register main av source err:[%d]\n", err);
    if (0 != err) {
        MYERROR("register main av source err:[%d] \n", err);
        if (args_main->av_source) {
            idip_server_av_source_unregister(m_idip_server, &args_main->av_source);
        }
        goto demo_source_setting_fault;
    }

    MYTRACE("begin register sub av source \n");
    err = idip_server_av_source_register(m_idip_server, &av_source_params1, &args_sub->av_source);
    MYTRACE("end register sub av source err:[%d]\n", err);
    if (0 != err) {
        MYERROR("register sub av source err:[%d] \n", err);
        if (args_sub->av_source) {
            idip_server_av_source_unregister(m_idip_server, &args_sub->av_source);
        }
        goto demo_source_setting_fault;
    }
    
#if 0
    Semaphore_Create(0, &g_liveVideoSem);
    Semaphore_Create(0, &g_liveVideoSubSem);
    Semaphore_Create(0, &g_liveAudioSem);

    pthread_t main_streamer_thid;
    err = pthread_create(&main_streamer_thid, NULL, live_streamer_thread, args_main);
    pthread_detach(main_streamer_thid);

    pthread_t sub_streamer_thid;
    err = pthread_create(&sub_streamer_thid, NULL, live_streamer_thread, args_sub);
    pthread_detach(sub_streamer_thid);

    pthread_t live_audio_thid;
    err = pthread_create(&live_audio_thid, NULL, thread_live_audio, args_main);
    pthread_detach(live_audio_thid);

    Semaphore_Post(&g_liveVideoSem);
    Semaphore_Post(&g_liveVideoSubSem);
    Semaphore_Post(&g_liveAudioSem);
#endif

    // main loop
#ifdef TAG_IVS_DETECTION
    LUX_EVENT_ATTR_ST IVSFuncCB;
    /* 移动侦测 初始化 */
    err = LUX_IVS_Iaac_Init();
    if (err)
    {
        MYERRORING("\nLUX_IVS_Iaac_Init  failed \n");
    }
    // else
    // {
    //     /*ivs算法初始化*/
    //     int i = 0;
    //     for (i = 0; i < LUX_IVS_ALGO_NUM; i++)
    //     {
    //         Semaphore_Post(&g_ivsAlgSem);
    //     }

    //     /*注册回调处理人形*/
    //     IVSFuncCB.pEvntReportFuncCB = (pAlarmEventReportCB *)ivs_event_cb;
    //     err = LUX_EVENT_IvsRegistCB(&IVSFuncCB);
    //     if(jio_cfg.alg_config.motion_detect_enabled)
    //     {
    //         LUX_IVS_StartAlgo(LUX_IVS_MOVE_DET_EN); 
    //     }
    // }
    //int timeCnt = 0;
    //for (;;) {
    //    if (timeCnt >= 600)
    //    {
    //        //set_time_zone(jio_cfg.system_config.timezone);
    //        lux_time_ntp_sync("00:00");
    //        //lux_time_ntp_cmd_sync();
    //        timeCnt = 0;
    //    }
    //    timeCnt++;
    //    sleep(1);
    //}

    return 0;

#endif
#endif
    // prepare for polling
    /*struct pollfd pfds[2];
    memset(pfds, 0, sizeof(pfds));
    pfds[0].fd = pipefds[0]; pfds[0].events = POLLIN;
    pfds[1].fd = timerfd;    pfds[1].events = POLLIN;

    // example of events notify
    int done = 0;
    while (!done) {
        char event;
        int ready = poll(pfds, 2, -1);
        if (ready <= 0) {
            if (errno == EINTR)
                continue;
            syscall_print_error("ppoll");
            break;
        }
        // pipe event
        if ((pfds[0].revents & POLLIN) == POLLIN) {
            ssize_t rc = read(pipefds[0], &event, sizeof(event));
            if (rc == 0)
                break;
            if (rc < 0) {
                if (errno == EINTR)
                    continue;
                syscall_print_error("read");
                break;
            }
            switch (event) {
            case 0: {
                done = 1;
                printf("main(): exit request received\n");
                break;
            }
            case 1: {
                // send "sound" event
                int rc = idip_server_send_sound_event(server, "camera0", now_ms());
                if (0 != rc) {
                    done = 1;
                    break;
                }
            } break;
            } // switch
        }

        // timer event
        if ((pfds[1].revents & POLLIN) == POLLIN) {
            uint64_t tmp;
            ssize_t rc = read(timerfd, &tmp, sizeof(tmp));
            if (rc < 0) {
                syscall_print_error("read");
                break;
            }

            if (demo_options.enable_events) {
                static int n = 0;
                static int n_continuous = 0;
                static int n_no_events = 0;

                if (n_continuous > 0) {
                    --n_continuous;
                }
                else if (n_no_events > 0) {
                    --n_no_events;
                    continue;
                }
                else {
                    n_continuous = demo_options.continuous_events_duration_sec - 1;
                    n_no_events = demo_options.no_events_duraction_sec;
                }

                switch (n++ % 3) {
                case 0:
                    if (0 != idip_server_send_motion_event(server, "camera0", now_ms())) {
                        done = 1;
                    }
                    if (0 == access(demo_options.ev_thermal_image_path, R_OK)) {
                        idip_ivs_vca_rect_t r = { .x = 50, .y = 50, .width = 100, .height = 100 };
                        idip_ivs_vca_shot_t shot = {
                            .image_url_or_path = demo_options.ev_thermal_image_path,
                            .temp_in_celsius = 37.5,
                            .temp_treshold = NULL,
                            .temp_threshold_size = 0,
                            .rects = &r,
                            .rects_size = 1
                        };
                        idip_server_send_vca_body_temp_high(server, "camera0", now_ms(), 38.0, 37.5, &shot);
                    }
                    break;
                case 1:
                    // send "sound" event
                    if (0 != idip_server_send_sound_event(server, "camera0", now_ms())) {
                        done = 1;
                    }
                    break;
                case 2:
                    // send "alarm" event
                    if (0 != idip_server_send_alarm_event(server, "camera0", now_ms())) {
                        done = 1;
                    }
                    break;
                }
            }
        }
    } // while*/
#ifdef FILE_SOURCE_ENABLE
    for (i = 0; i < ARRAY_SIZE(demo_sources); ++i) {
        for (j = 0; j < DVR_PROFILES_MAX; ++j) {
            if (!demo_sources[i][j])
                continue;
            avpkt_stream_source_release(demo_sources[i][j], server, av_sources[i][j]);
            idip_server_av_source_unregister(server, &av_sources[i][j]);
        }
    }
#else
    if (args_main->av_source) {
        idip_server_av_source_unregister(m_idip_server, &args_main->av_source);
    }
    if (args_sub->av_source) {
        idip_server_av_source_unregister(m_idip_server, &args_sub->av_source);
    }
#endif 
    close(pipefds[0]);
    close(pipefds[1]);
    close(timerfd);


    // Notify server instance about stop.
    // Function blocks main thread until server instance will not done.
    idip_server_stop(m_idip_server);

#ifdef FILE_SOURCE_ENABLE
    for (i = 0; i < ARRAY_SIZE(demo_sources); ++i) {
        for (j = 0; j < DVR_PROFILES_MAX; ++j) {
            if (!demo_sources[i][j])
                continue;
            avpkt_stream_source_destroy(demo_sources[i][j]);
            free(demo_sources[i][j]);
            demo_sources[i][j] = NULL;
        }
    }
#endif
#if defined(USE_ALSA_BACKEND)
    IDIP_DELETE(demo_options.playback, playback_delete);
#endif /* USE_ALSA_BACKEND */
    idip_cleanup();

    return 0;


demo_source_setting_fault:
    idip_server_stop(m_idip_server);
server_starting_fault:
#ifdef FILE_SOURCE_ENABLE
    for (i = 0; i < ARRAY_SIZE(demo_sources); ++i) {
        for (j = 0; j < DVR_PROFILES_MAX; ++j) {
            if (!demo_sources[i][j])
                continue;
            avpkt_stream_source_destroy(demo_sources[i][j]);
            free(demo_sources[i][j]);
            demo_sources[i][j] = NULL;
        }
    }
#endif
demo_sources_init_fault:
#if defined(USE_ALSA_BACKEND)
    IDIP_DELETE(demo_options.playback, playback_delete);
playback_new_fault:
#endif /* USE_ALSA_BACKEND */
    idip_cleanup();

idip_init_fault:
    close(pipefds[0]);
    close(pipefds[1]);

    exit(EXIT_FAILURE);
    }

//------------------------------------------------------------------------------
#if 0
static void usage(const char* program_name)
{
    printf("Usage:\n"
           "\t%s [options] <live file path>\n"
           "Options:\n"
           "\t-l or --live <path/to/file.mp4> - path to file that emulates live video. It must contains H264-encoded video stream.\n"
           "\t-a or --archive <path/to/archive/list.txt> - path to file that describes archive.\n"
           "\t-s or --snapshot <path/to/file.jpeg> - path to file that contains snapshot (preview) image.\n"
           "\t-p or --persistent-storage <path/to/file> - path to file that emulates persistent storage.\n"
           "\t--disable-live-loop - disable looping of live video. Enabled by default.\n"
           "\t--disable-events - disable periodically fake event generation. By default is enabled.\n"
           "\t--events-schedule START_DELAY/CONTINUOUS/NO_EVENTS - 15/1/60 (seconds) by default.\n"
           "\t-P or --port - TCP listen port number. Set to 0 for Unix Domain Sockets.\n"
           "\t-I or --iface - Listening interface address or path to listening unix domain socket file.\n"
           "\t\tSet to '*' or to '0.0.0.0' for listening all available TCP interfaces.\n"
           "\t--ws-url - URL for uplink connection.\n"
           "\t--ws-key - uplink connection device key.\n"
           "\t--sn - set serial number.\n"
           "\t--mac - override active mac address. Format is XX:XX:XX:XX:XX:XX\n"
           "\t--qr-code - set path to a parsed qr-code data file. When no option is passed the QR-code feature is disabled.\n"
           "\t-v or --version - show SDK version\n"
           "\t-h or --help - show this help\n",
           program_name);
    version();
}
//------------------------------------------------------------------------------
static void version(void)
{
    printf("Ivideon Embedded SDK version: %s\n", ivesdk_version_full());
    printf("libwebsockets version: %s\n", lws_get_library_version());
}
#endif

