#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <imp_log.h>
#include <imp_common.h>
#include <imp_system.h>
#include <imp_framesource.h>
#include <imp_encoder.h>
#include <imp_log.h>
#include <comm_def.h>
#include <comm_error_code.h>
#include <comm_func.h>
#include <lux_isp.h>
#include <lux_fsrc.h>
#include <lux_video.h>
#include <lux_base.h>
#include "lux_audio.h"
#include <lux_iniparse.h>
#include <lux_hal_led.h>
#include <lux_hal_lightsns.h>
#include <lux_hal_misc.h>
#include <lux_daynight.h>
#include <lux_ivs.h>
#include <lux_osd.h>
#include <lux_pwm.h>

#include <ivs_common.h>
#include <ivs_inf_move.h>
#include <ivs_interface.h>
//#include <lux_hal_motor.h>

#include <lux_nv2jpeg.h>

#define TAG 	"Sample-Encoder-video"

/**
 * Minimal IDIP server skeleton.
 *
 * PAY ATTENTION: The skeleton is implemented with a lot of stubs you
 * will have to replace your code.
 */
#include <getopt.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <idip.h>
#include <idip/idip_av_source.h>

#include <pthread.h>
#include <inttypes.h>
#include <assert.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <libgen.h>
#include "comm_func.h" 
#include "comm_def.h"
#include <lux_hal_led.h>
#include "lux_iniparse.h"
#include "lux_base.h"
#include "lux_audio.h"
#include "lux_ivs.h"
#include <lux_event.h>
#include "lux_hal_misc.h"
//#include "tuya_ipc_stream_storage.h"
//#include "lux_smileDet.h"
//#include "lux_coverFace.h"
#include "lux_nv2jpeg.h"
//#include "tuya_ipc_p2p.h"
//#include "tuya_cloud_base_defs.h"
//#include "tuya_ipc_dp_utils.h"
//#include "tuya_ipc_cloud_storage.h"
//#include "tuya_ipc_common_demo.h"
//#include "tuya_ipc_system_control_demo.h"
//#include "tuya_ipc_media_demo.h"
//#include "tuya_ipc_motion_detect_demo.h"
//#include "tuya_ipc_doorbell_demo.h"
//#include "tuya_iot_config.h"

//#include "lux_sleepDet.h"


#include <stdint.h>
#include <stddef.h>


#define MOVIE_DURATION_MS (6000)
extern OSI_SEM g_liveVideoMainSem;
extern BOOL_X g_halTempHumiEnable;
extern BOOL_X g_pwmBlubEnable;
extern BOOL_X g_bcryDetctAlarmSong;


typedef struct PacketInfo {
    bool isKeyFrame;
    int64_t dts;
    int64_t pts;
    int64_t delay;
    size_t size;
    const uint8_t* data;
} PacketInfo;



//------------------------------------------------------------------------------
#ifndef ARRAY_SIZE
#define ARRAY_SIZE( x ) (sizeof(x)/sizeof(x[0]))
#endif

//------------------------------------------------------------------------------
// Global server configuration.
static idip_server_conf_t server_config = {
#if 0 /* TCP connection */
      /* set host_or_path to "*" or "0.0.0.0" if you need to use INADDR_ANY */
    .host_or_path = "*",
    .port = 55500,
#else /* Unix Domain socket connection. Port always 0 */
    .host_or_path = "/tmp/idip.sock",
    .port = 0,
#endif
    .keep_alive_ms = 2000000ul,

    .vendor = "Ivideon",
    .model = "Skeleton",
    .serial_number = "0000000003",

    .disable_ipv6 = 1, // Set to 1 in order to disable IPv6

    // TODO: use this only if you need to integrate as WebSocket client.
    // Set both values to NULL when you have the videoserver from Ivideon on
    // firmware.
    .dev_key = "idip_jio_luxshare_camera-ak99g8j5ee",
    .cloud_enpoint_url = "https://idip-v1.cloud.smartmonitoring.jio.com/idip_uplink",
};

typedef struct user_data {
    // path to persistent storage file
    char* persistent_storage;
    // TODO: append required fields here
} user_data_t;

static user_data_t static_user_data = {
    // TODO: use correct path in production!
    .persistent_storage = "/system/init/idip-persistent_skel"
    // TODO: Initialize user data here
};

typedef struct live_streamer_args_t {
    idip_av_source_t* av_source;
    int done;
} live_streamer_args_t;

static live_streamer_args_t live_streamer_args = {
    .av_source = NULL,
    .done = 0
};

static const char camera_target_name[] = "camera0";
static const char camera_profile_name[] = "0";

//------------------------------------------------------------------------------
//static int parse_command_line_options(int argc, char** argv);
//static void usage(const char* program_name);

static idip_status_code_t
get_interfaces_handler(idip_stream_system_get_interfaces_t* stream, void* udata);

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

static idip_status_code_t
system_notify_handler_impl(const idip_system_notify_query_args_t* args,
                           void* udata);

static idip_status_code_t
persistent_data_get_impl(idip_stream_persistent_data_t* stream,
                    void* udata);

static idip_status_code_t
persistent_data_set_impl(const void* data, size_t size, void* udata);


//void*
//live_streamer_thread(void* arg_)


void*
live_streamer_thread(void* arg_)
{
    printf("******************live_streamer_thread**********************\n" );
        INT_X ret = 0;
         LUX_STREAM_CHANNEL_EN channel = LUX_STREAM_MAIN;
        PacketInfo packets = {0};
        LUX_ENCODE_STREAM_ST luxFrame;
    
        /* 阻塞等待mqtt上线 */
      //  if (LUX_STREAM_MAIN == channel){
      //      Semaphore_Pend(&g_liveVideoMainSem, SEM_FOREVER);
       // }
      //  else if (LUX_STREAM_SUB == channel)
         //  Semaphore_Pend(&g_liveVideoSubSem, SEM_FOREVER);
    
#ifdef RTSP_ENABLE
        if (1 == channel)
            RTSP_Server_Init();
#endif

    live_streamer_args_t* args = arg_;
    //int nPackets = (int)ARRAY_SIZE(packets);

    int64_t start_ts;
    int64_t offset =0;
    
    while (1)
        {
            
            ret = LUX_Video_Encoder_GetStream(channel, &luxFrame);
            
            if (0 != ret)
            {
                usleep(100*1000);
                continue;
            }
            
            if (luxFrame.type == LUX_ENC_SLICE_P)
            {
                packets.isKeyFrame = 0;//不为i帧就为0
            }
            else
            {
                packets.isKeyFrame = 1;
            }
    
            packets.size = luxFrame.streamLen;
            packets.data = luxFrame.pAddr;
            packets.pts = luxFrame.pts;
            start_ts = luxFrame.timeStamp;
   // printf("luxFrame.type=%d,luxFrame.pts=%d,luxFrame.streamLen=%d,luxFrame.pAddr=%x,%d\n", 
                //    luxFrame.type,luxFrame.pts,luxFrame.streamLen,luxFrame.pAddr,luxFrame.pAddr);
            if (LUX_STREAM_MAIN == channel)
            {

                //printf("luxFrame.type=%d,luxFrame.pts=%d,luxFrame.streamLen=%d,luxFrame.pAddr=%x\n", 
                //    luxFrame.type,luxFrame.pts,luxFrame.streamLen,luxFrame.pAddr);
               int rc = idip_av_source_push_video(args->av_source,
                                              start_ts+offset+packets.pts,
                                               packets.data,
                                               packets.size,
                                               packets.isKeyFrame);
               offset += MOVIE_DURATION_MS;
            if( IDIP_OK != rc   ) {
                printf("idip_av_source_push_video() returns %d\n", rc);
            }

/*

            int64_t delay = packets.delay;
            if( (packets.dts - prev_dts < delay) && (packets.dts > prev_dts) ) {
                delay = packets.dts - prev_dts;
            }
            usleep(delay);

            prev_dts = packets.dts;
 */  

            
        #ifdef RTSP_ENABLE
                RTSP_STREAM_S rtspStream;
                memcpy(&rtspStream, &h264_frame, sizeof(rtspStream));
                RTSP_Send2Client_Sync(&rtspStream);
        #endif
    
            }
#if 0            
            else if (LUX_STREAM_SUB == channel)
            {
                /* Send SD video data to the SDK */
                TUYA_APP_Put_Frame(E_CHANNEL_VIDEO_SUB, &h264_frame);
            }
#endif    
            LUX_Video_Encoder_ReleaseStream(channel, &luxFrame);
    
            usleep(15 * 1000);
        }

}  



//==============================================================================
int jio_skel_main()

//int main(int argc, char *argv[])

{

    // Initialize library
    if( IDIP_OK != idip_init() ) {
        // TODO: log_error(...);
        exit(EXIT_FAILURE);
    }

    // 1. Create an idip server instance.
    idip_server_t* server = idip_server_new(&server_config);
    if( !server ) {
        // TODO: log_error(...);
        idip_cleanup();
        exit(EXIT_FAILURE);
    }
    
    // 2. Set up server features. Set up implemented features only.
    int rc = idip_server_setup_archive_feature(server, camera_target_name,
        archive_handler_impl, &static_user_data,
        archive_list_handler_impl, NULL,
        archive_days_handler_impl, NULL,
        (idip_archive_capabilities_t){
            .speed_factors = (archive_play_speed_t[]){ ARCHIVE_PLAY_SPEED_1X },
            .speed_factors_count = 1
        });
    if( 0 != rc ) {
        // log_error(...);
        goto fail;
    }
    rc = idip_server_setup_system_notify_handler(server,
                                                 system_notify_handler_impl,
                                                 NULL);
    if( 0 != rc ) {
        // log_error(...);
        goto fail;
    }
    // Setup a system handlers to get and set persistent data (cloud data). REQUIRED.
    rc = idip_server_setup_persistent_storage_feature(server,
                                                      persistent_data_get_impl,
                                                      &static_user_data,
                                                      persistent_data_set_impl,
                                                      &static_user_data,
                                                      1024ul*1024ul);
    if( 0 != rc ) {
        goto fail;
    }
    // Setup a system handler to get interfaces information. REQUIRED.
    rc = idip_server_setup_get_interfaces_feature(server, get_interfaces_handler, NULL);
    if( 0 != rc ) {
        goto fail;
    }
    // ... and so on.

    // 3. Set up live streaming sources
    idip_av_source_params_t av_source_params = (idip_av_source_params_t)
    {
        .target_name = camera_target_name,
        .profile_name = camera_profile_name,
        .v_info = (video_stream_info_t)
        {
            .codec = VIDEO_CODEC_H264,
            .width = SENSOR_WIDTH_DEFAULT,
            .height = SENSOR_HEIGHT_DEFAULT
        },
        .a_info = (audio_stream_info_t)
        {
            .codec_type = AUDIO_CODEC_NONE, //AUDIO_CODEC_AAC,
            .sample_format = AUDIO_SAMPLE_FORMAT_FLOATP,
            .channels = 1,
            .sample_rate = 0//44100
        },
        .limits = IDIP_AV_LIMITS_INIT(2000, 64, 5),
        .event_handler = NULL,
        .event_handler_data = NULL,
    };

    live_streamer_args.av_source = NULL;
    rc = idip_server_av_source_register(server, &av_source_params, &live_streamer_args.av_source);
    if( 0 != rc ) {
        // log_error(...);
        goto fail;
    }

    // 4. Start the server
    if( IDIP_OK != idip_server_run(server) ) {
        // TODO: log_error(...);
        rc = -1;
        goto fail;
    }
 /*  
    WIFI_INIT_MODE_E mode = WIFI_INIT_DEBUG;
#if 1
#if defined(WIFI_GW) && (WIFI_GW==0)
    mode = WIFI_INIT_NULL;
#endif

    mode = WIFI_INIT_AP;
#endif
      IPC_APP_Init_SDK(mode, NULL);
*/
    pthread_t live_streamer_thid;
    rc = pthread_create(&live_streamer_thid, NULL, live_streamer_thread, &live_streamer_args);
    if( rc < 0 ) {
        rc = -1;
        goto fail;
    }

    // main loop
    for(;;) {
        // stub
        usleep(100000ul);
    }
    //Semaphore_Post(&g_liveVideoMainSem);
    //live_streamer_args.done = 1;
    //pthread_join(live_streamer_thid, NULL);

fail:
    if( live_streamer_args.av_source ) {
        idip_server_av_source_unregister(server, &live_streamer_args.av_source);
    }
    // Notify server instance about stop.
    // Function blocks main thread until server instance will not done.
    idip_server_stop(server);

    // Free resources
    idip_cleanup();

    return rc;
}
//==============================================================================

#if 0//------------------------------------------------------------------------------
static int parse_command_line_options(int argc, char** argv)
{
    int c;
    while(1) {
        int option_index = 0;
        static struct option long_options[] = {
           {"port",                 required_argument, 0, 'P' },
           {"iface",                required_argument, 0, 'I' },
           {"help",                 no_argument,       0, 'h' },
           {0,                      0,                 0,  0  },
        };

        c = getopt_long(argc, argv, "P:A:h",
                        long_options, &option_index);
        if (c == -1)
            break;
        switch(c) {
        case 0: break; // long option
        case 'P': server_config.port = atoi(optarg); break;     // NOLINT
        case 'I': server_config.host_or_path = optarg; break;
        case 'h':
            usage(argv[0]);
            exit(EXIT_SUCCESS);
        case '?':
        default:
            usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
//------------------------------------------------------------------------------
static void usage(const char* program_name)
{
    printf("Usage:\n"
           "\t%s [options]\n"
           "Options:\n"
           "\t-P or --port - TCP listen port number. Set to 0 for Unix Domain Sockets.\n"
           "\t-I or --iface - Listening interface address or path to listening unix domain socket file.\n"
           "\t\tSet to '*' or to '0.0.0.0' for listening all available TCP interfaces.\n"
           "\t-h or --help - show this help\n",
           program_name);
}
#endif
//------------------------------------------------------------------------------
static idip_status_code_t
archive_handler_impl(idip_stream_archive_t* stream,
                     const char* target,
                     const idip_archive_play_params_t* params,
                     void* udata)
{
    printf("%s( target=%s, udata=%p )\n",
           __func__, (target ? target : "(null)"), udata);
    printf("%s(): params.speed=%d, params.start_ms=%" PRIu64 ", params.play_mode=%d )\n",
           __func__, params->speed, params->start_ms, params->play_mode);
    (void)(stream);

    // TODO: Implement archive play feature here

    return IDIP_OK;
}

//------------------------------------------------------------------------------
static idip_status_code_t
archive_list_handler_impl(idip_stream_archive_list_t* stream,
                          const char* target,
                          const archive_interval_t* params,
                          void* udata)
{
    printf("%s( target=%s, udata=%p, params.start_ms=%" PRIu64 ", params.end_ms=%" PRIu64 ")\n",
           __func__, (target ? target : "(null)"), udata,
           params->start_ms, params->end_ms);
    (void)(stream);

    // TODO: implement archive list feature here

    return IDIP_OK;
}

//------------------------------------------------------------------------------
static idip_status_code_t
archive_days_handler_impl(idip_stream_archive_days_t* stream,
                          const char* target,
                          const archive_interval_t* params,
                          void* udata)
{
    printf("%s( target=%s, udata=%p, params.start_ms=%" PRIu64 ", params.end_ms=%" PRIu64 ")\n",
           __func__, (target ? target : "(null)"), udata,
           params->start_ms, params->end_ms);
    (void)(stream);

    // TODO: implement archive days feature here

    return IDIP_OK;
}

//------------------------------------------------------------------------------
static idip_status_code_t
system_notify_handler_impl(const idip_system_notify_query_args_t* args,
                           void* udata)
{
    printf("%s(args.target=%s, args.name=%s). udata=%p\n",
           __func__, (args->target ? args->target : "(null)"),
           (args->name ? args->name : "(null)"), udata);

    // TODO: implement system notify handler here

    return IDIP_OK;
}
//------------------------------------------------------------------------------
static idip_status_code_t
persistent_data_get_impl(idip_stream_persistent_data_t* stream,
                    void* udata)
{
    user_data_t* options = udata;

    // TODO: replace the helper call to your implementation!
    //
    // PAY ATTENTION! We use a simple helper here to reduce the code.
    // Real implementations MUST take into account the specific of a camera/DVR.
    // But they can refer to the helper implementation.
    return idip_persistent_data_get_default_implementation_helper(stream,
                options->persistent_storage, 1024*1024);
}
//------------------------------------------------------------------------------
static idip_status_code_t
persistent_data_set_impl(const void* data, size_t size, void* udata)
{
    user_data_t* options = udata;

    // TODO: replace the helper call to your implementation!
    //
    // PAY ATTENTION! We use a simple helper here to reduce the code.
    // Real implementations MUST take into account the specific of a camera/DVR.
    // But they can refer to the helper implementation.
    return idip_persistent_data_set_default_implementation_helper(options->persistent_storage, data, size);
}
//------------------------------------------------------------------------------
// The handler is required by libidip. It returns interfaces infaormation.
static idip_status_code_t
get_interfaces_handler(idip_stream_system_get_interfaces_t* stream, void* udata)
{
    IDIP_UNUSED(udata);

    // In the demo we just return FAKE data for a simple.
    // Real implementation should return actual interface data.
    //
    // PAY ATTENTION! The first interface in the list is the "active" one: it's
    // used by the device to connect to the cloud.
    //
    // PAY ATTENTION! Set the attach_iface flag for an interface that's should
    // be using for the attachment to the cloud procedure. Only one
    // intherface can use the flag. The libidip will use a mac addres of
    // the first interface marked by the flag.

    // "192.168.0.20/24"
    const idip_ipv4_addr_t eth0_addr = { .addr = {10,10,100,103}, .mask_length = 24 };
    const idip_iface_t fake_ifaces[1] = {
        (const idip_iface_t){
            .type = IDIP_IFACE_WIRELESS,
            .ifname = "wlan0",
            .attach_iface = 1,
            .ipv4_addr = { eth0_addr },
            .ipv4_addr_count = 1,
            // "01:02:03:04:05:06". NOTE! Don't use these mac addresses in production!40:24:B2:A1:05:7E
            .mac_addr = { 0x40, 0x24, 0xB2, 0xA1, 0x05, 0xA0 },
            .ipv6_addr = { 0 },
            .ipv6_addr_count = 0,
        }
    };

    return idip_stream_system_get_interfaces_put_iface_data(stream, (const idip_iface_t*)&fake_ifaces,
                                                            ARRAY_SIZE(fake_ifaces),
                                                            IDIP_INFINITE_TIMEOUT);
}



