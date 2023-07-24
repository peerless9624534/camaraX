
#define TAG "rtsp_main"

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
#include "lux_nv2jpeg.h"
#include "lux_ivs.h"
#include <imp_log.h>

#include <idip.h>                    // the main library header
#include <idip/idip_cloud_config.h>  // cloud config helper API
#include <idip/idip_custom_events.h> // extended events API
#include <idip/idip_log.h>           // logging functions
#include <jwhead.h>
#include "fdk-aac/aacenc_lib.h"
#include "lux_global.h"
#include "lux_record.h"
#include "lux_config.h"
#include "lux_person_det.h"
#include "lux_face_det.h"
#include "lux_perm_det.h"
#include "rtsp_demo.h"

//============================================================
OSI_SEM g_liveAudioSem;
OSI_SEM g_liveVideoMainSem;
OSI_SEM g_liveVideoSubSem;
OSI_SEM g_liveEvent;
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
    unsigned char    *p_buf;
    UINT_X    size;
    UINT64_X  pts;
    UINT64_X  timestamp;
}MEDIA_FRAME_S;


extern INT_X LUX_ALG_TamperDet_Init();
extern INT_X LUX_ALG_ObjDet_Init();
extern INT_X LUX_ALG_LineCrossDet_Init();
extern INT_X LUX_ALG_LoiterDet_Init();

/* This is for demo only. Should be replace with real PCM/AAC/G711 encoder output */
void *thread_live_audio(void *arg)
{
    INT_X ret = 0;
    MEDIA_FRAME_S pcm_frame = {0};
    LUX_AUDIO_FRAME_ST audioFrame;
    LUX_STREAM_CHANNEL_EN channel = (LUX_STREAM_CHANNEL_EN)arg;

    live_streamer_args_t* args;

    if (LUX_STREAM_MAIN == channel)
    {
        args = args_main;
    }
    else if (LUX_STREAM_SUB == channel)
    {
        args = args_sub;
    }

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
        MYWARN("Unsupported WAV channels %d\n", channels);
        return NULL;
    }

    if (aacEncOpen(&handle, 0, channels) != AACENC_OK) {
        MYERRORING("Unable to open encoder\n");
        return NULL;
    }

    if (aacEncoder_SetParam(handle, AACENC_AOT, aot) != AACENC_OK) {
        MYERRORING("Unable to set the AOT\n");
        return NULL;
    }
    if (aot == 39 && eld_sbr) {
        if (aacEncoder_SetParam(handle, AACENC_SBR_MODE, 1) != AACENC_OK) {
            MYERRORING("Unable to set SBR mode for ELD\n");
            return NULL;
        }
    }
    if (aacEncoder_SetParam(handle, AACENC_SAMPLERATE, sample_rate) != AACENC_OK) {
        MYERRORING("Unable to set the AOT\n");
        return NULL;
    }
    if (aacEncoder_SetParam(handle, AACENC_CHANNELMODE, mode) != AACENC_OK) {
        MYERRORING("Unable to set the channel mode\n");
        return NULL;
    }
    if (aacEncoder_SetParam(handle, AACENC_CHANNELORDER, 1) != AACENC_OK) {
        MYERRORING("Unable to set the wav channel order\n");
        return NULL;
    }
    if (vbr) {
        if (aacEncoder_SetParam(handle, AACENC_BITRATEMODE, vbr) != AACENC_OK) {
            MYERRORING("Unable to set the VBR bitrate mode\n");
            return NULL;
        }
    }
    else {
        if (aacEncoder_SetParam(handle, AACENC_BITRATE, bitrate) != AACENC_OK) {
            MYERRORING("fdk-aac Unable to set the bitrate\n");
            return NULL;
        }
    }
    if (aacEncoder_SetParam(handle, AACENC_TRANSMUX, TT_MP4_ADTS) != AACENC_OK) {
        MYERRORING("fdk-aac Unable to set the ADTS transmux\n");
        return NULL;
    }
    if (aacEncoder_SetParam(handle, AACENC_AFTERBURNER, afterburner) != AACENC_OK) {
        MYERRORING("fdk-aac Unable to set the afterburner mode\n");
        return NULL;
    }
    if (aacEncEncode(handle, NULL, NULL, NULL, NULL) != AACENC_OK) {
        MYERRORING("fdk-aac Unable to initialize the encoder\n");
        return NULL;
    }
    if (aacEncInfo(handle, &info) != AACENC_OK) {
        MYERRORING("fdk-aac Unable to get the encoder info\n");
        return NULL;
    }

    input_size = channels * 2 * info.frameLength;
    input_buf = (uint8_t*)malloc(input_size);
    convert_buf = (int16_t*)malloc(input_size);
    temp_buf = (uint8_t*)malloc(input_size * 3);
    int buf_pos = 0;
    int buf_size = 0;

    uint8_t outbuf[20480];
    AACENC_ERROR err;

    /* 阻塞等待mqtt上线并初始化完成 */
    Semaphore_Pend(&g_liveAudioSem, SEM_FOREVER);
        /*哭声检测*/
    // LUX_AI_CryDet_Init();//cxj test
    pcm_frame.type = E_AUDIO_FRAME;
    
    while(1)
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

        if (0 == g_configData->media_config.audio_config.microphone_enabled)
        {
            continue;
        }

        memset(&audioFrame, 0, sizeof(audioFrame));
        ret = LUX_AI_GetStream(&audioFrame);
        if (0 != ret)
        {
            usleep(1*1000);
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
                    uint64_t frame_time_stamp_ms = time_ms();
#if 0
                    //音频存储--将数据推到缓冲区
                    if (LUX_STREAM_MAIN == channel && out_args.numOutBytes < 5 * 1024 * 1024)
                    {
                        int pack_len = sizeof(frame_head_t) + sizeof(audio_frame_head_t) + out_args.numOutBytes;
                        frame_head_t* one_frame_head = LUX_Record_alloc_packet(FRAME_TYPE_AUDIO, pack_len);

                        if (one_frame_head)
                        {
                            one_frame_head->frame_data_len = out_args.numOutBytes;
                            one_frame_head->frame_head_len = sizeof(audio_frame_head_t);
                            one_frame_head->frame_type = FRAME_TYPE_AUDIO;
                            one_frame_head->timestamp_ms = frame_time_stamp_ms;
                            audio_frame_head_t* one_audio_frame_head = (audio_frame_head_t*)((char*)one_frame_head + sizeof(frame_head_t));
                            one_audio_frame_head->encode = AUDIO_ENCODE_AAC;
                            one_audio_frame_head->channel_num = 1;
                            one_audio_frame_head->bits = 16;
                            one_audio_frame_head->format = 1;
                            one_audio_frame_head->sample_rate = 16000;
                            one_audio_frame_head->bit_rates = 64000;
                            one_audio_frame_head->frame_len = out_args.numOutBytes;
                            char* data = ((char*)one_frame_head) + sizeof(frame_head_t) + sizeof(audio_frame_head_t);
                            memcpy(data, outbuf, out_args.numOutBytes);

                            g_lux_record->data_len += pack_len;
                        }

                        LUX_Record_free_packet(one_frame_head);
                    }
#endif

#if 1
                    if (access("/tmp/aactest", 0) == 0)
                    {
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
                    }
#endif
                    //音频推送到客户端
                   rtsp_put_audio_fream(outbuf,out_args.numOutBytes,frame_time_stamp_ms);

                    if (g_configData->system_config.jio_enabled)
                    {
                        if (g_configData->system_config.online && g_configData->system_config.online_mode)
                        {
                            ret = idip_av_source_push_audio(args->av_source,
                                                //audioFrame.timeStamp / 1000 + audioFrame.seq,
                                                frame_time_stamp_ms,
                                                //audioFrame.pData,
                                                //audioFrame.len
                                                outbuf,
                                                out_args.numOutBytes
                            );

                            if (ret != 0)
                            {
                                MYERROR("idip_av_source_push_audio() returns %d\n", ret);
                            }
                        }
                    }
                }
            }
            else
            {
                MYERROR("fdk-aac Encoding failed, ret:%d\n", err);
            }
        }
        
        LUX_AI_ReleaseStream(&audioFrame);
    }
    
    // LUX_AI_CryDet_DeInit();
    
    pthread_exit(0);
}

void *thread_live_video(void* arg)
{
    INT_X ret = 0;
    LUX_STREAM_CHANNEL_EN channel = (LUX_STREAM_CHANNEL_EN)arg;
    MEDIA_FRAME_S h264_frame = {0};
    LUX_ENCODE_STREAM_ST luxFrame;
    live_streamer_args_t* args;

    if (LUX_STREAM_MAIN == channel)
    {
        Semaphore_Pend(&g_liveVideoMainSem, SEM_FOREVER);
        args = args_main;
    }
    else if (LUX_STREAM_SUB == channel)
    {
        Semaphore_Pend(&g_liveVideoSubSem, SEM_FOREVER);
        args = args_sub;
    }


    sleep(1);

    while (1)
    {
        if (LUX_STREAM_MAIN == channel)
        {
            if (0 == push_live_stream_enable_main && 0 == Semaphore_Pend_flag_main)
            {
                printf(" %s，%s:%d,,thread_live_video Wait ENTER\n", __FUNCTION__, __FILE__, __LINE__);
                Semaphore_Pend_flag_main = 1;
                Semaphore_Pend(&g_liveVideoMainSem, SEM_FOREVER);
            }
        }
        else if (LUX_STREAM_SUB == channel)
        {
            if (0 == push_live_stream_enable_sub && 0 == Semaphore_Pend_flag_sub)
            {
                printf(" %s，%s:%d,,thread_live_video Wait ENTER\n", __FUNCTION__, __FILE__, __LINE__);
                Semaphore_Pend_flag_sub  = 1;
                Semaphore_Pend(&g_liveVideoSubSem, SEM_FOREVER);
            }
        }

        if (Semaphore_Pend_flag_main == 1 && LUX_STREAM_MAIN == channel)
        {
            Semaphore_Pend_flag_main = 0;
           
            printf(" %s，%s:%d,thread_live_video ENTER\n", __FUNCTION__, __FILE__, __LINE__);
        }
        if (Semaphore_Pend_flag_sub == 1 && LUX_STREAM_SUB == channel)
        {
            Semaphore_Pend_flag_sub = 0;
            printf(" %s，%s:%d,thread_live_video ENTER\n", __FUNCTION__, __FILE__, __LINE__);
        }

        int usleep_time = 0;
        usleep_time = 1000000L / g_configData->image_config.fps_0;
        if (LUX_STREAM_MAIN == channel)
       {
        usleep(usleep_time);
       }
       else
       {
         usleep(usleep_time);
       }
        // printf("usleep_time = %ld,g_configData->image_config.fps_0 = %lld\n",usleep_time,g_configData->image_config.fps_0);
        ret = LUX_Video_Encoder_GetStream(channel, &luxFrame);
        if (0 != ret)
        {
            usleep(5*1000);
            continue;
        }
        if (luxFrame.type == LUX_ENC_SLICE_P)
        {
            h264_frame.type = E_VIDEO_PB_FRAME;
        }
        else
        {
            h264_frame.type = E_VIDEO_I_FRAME;
        }

        h264_frame.size = luxFrame.streamLen;
        h264_frame.p_buf = luxFrame.pAddr;
        h264_frame.pts = luxFrame.pts;
        h264_frame.timestamp = luxFrame.timeStamp;

        static int writecount1 = 0;
        static FILE* fp3 = NULL;

        if (access("/tmp/substreamtest", 0) == 0)
        {
            if (!fp3)
            {
                fp3 = fopen("/tmp/stream_sub.dat", "wb");
            }

            if (LUX_STREAM_SUB == channel && fp3 && writecount1 < 1 * 1024 * 1024)
            {
                fwrite(luxFrame.pAddr, 1, luxFrame.streamLen, fp3);
                writecount1 += luxFrame.streamLen;
            }
        }

        static int writecount2 = 0;
        static FILE* fp4 = NULL;

        if (access("/tmp/mainstreamtest", 0) == 0)
        {
            if (!fp4)
            {
                fp4 = fopen("/tmp/stream_main.dat", "wb");
            }

            if (LUX_STREAM_MAIN == channel && fp4 && writecount2 < 1 * 1024 * 1024)
            {
                fwrite(luxFrame.pAddr, 1, luxFrame.streamLen, fp4);
                writecount2 += luxFrame.streamLen;
            }
        }

        //if (LUX_STREAM_MAIN == channel)
        //{
            //RTSP_STREAM_S rtspStream;
            //memcpy(&rtspStream, &h264_frame, sizeof(rtspStream));
            //if(rtspStream.pts % 50 == 0)
            //{
                //printf("set frame type: %d pts: %lld size:%d timestamp:%lld\r\n",rtspStream.type,rtspStream.pts,rtspStream.size,rtspStream.timestamp);
            //}
            //RTSP_Send2Client_Sync(&rtspStream);
        //}

        uint64_t frame_time_stamp_ms = time_ms();

        //视频推送到客户端
        rtsp_put_video_fream(channel,luxFrame.pAddr,luxFrame.streamLen,frame_time_stamp_ms);
        
        int isKeyFrame = luxFrame.type == LUX_ENC_SLICE_I ? 1 : 0;

        if (g_configData->system_config.jio_enabled)
        {
            //printf("online:%d online_mode:%d \n", g_configData->system_config.online, g_configData->system_config.online_mode);
            if (g_configData->system_config.online && g_configData->system_config.online_mode)
            {

                int rc = idip_av_source_push_video(args->av_source,
                                           //videoFrame.timeStamp / 1000 + videoFrame.pts,
                                           frame_time_stamp_ms,
                                           luxFrame.pAddr,
                                           luxFrame.streamLen,
                                           isKeyFrame);
                //offset += MOVIE_DURATION_MS;
                if (rc != IDIP_OK) {
                    MYWARNING("idip_av_source_push_video() returns %d\n", rc);
                }
                //printf("idip_av_source_push_video, ret:%d \n", rc);
                if (isKeyFrame && LUX_STREAM_MAIN == channel)
                {
                    printf("Keyframe length:%u, data:[%02d %02d %02d %02d %02d] \n",
                        luxFrame.streamLen,
                        ((char*)luxFrame.pAddr)[0], ((char*)luxFrame.pAddr)[1], ((char*)luxFrame.pAddr)[2], ((char*)luxFrame.pAddr)[3], ((char*)luxFrame.pAddr)[4]);
                }
            }
        }

        if (channel == LUX_STREAM_MAIN && luxFrame.streamLen > 10 && luxFrame.streamLen < 50 * 1024 * 1024)
        {
            int pack_len = sizeof(frame_head_t) + sizeof(video_frame_head_t) + luxFrame.streamLen;
            frame_head_t* one_frame_head = LUX_Record_alloc_packet(FRAME_TYPE_VIDEO, pack_len);

            if (one_frame_head)
            {
                //MYTRACE("save video frame len:%u \n", videoFrame.streamLen);
                one_frame_head->frame_data_len = luxFrame.streamLen;
                one_frame_head->frame_head_len = sizeof(video_frame_head_t);
                one_frame_head->frame_type = FRAME_TYPE_VIDEO;
                one_frame_head->timestamp_ms = frame_time_stamp_ms;
                video_frame_head_t* one_video_frame_head = (video_frame_head_t*)((char*)one_frame_head + sizeof(frame_head_t));
                one_video_frame_head->encode = VIDEO_ENCODE_H264;
                one_video_frame_head->frame_len = luxFrame.streamLen;
                one_video_frame_head->frame_type = isKeyFrame ? VIDEO_I_FRAME : VIDEO_P_FRAME;
                char* data = ((char*)one_frame_head) + sizeof(frame_head_t) + sizeof(video_frame_head_t);
                memcpy(data, luxFrame.pAddr, luxFrame.streamLen);

                g_lux_record->data_len += pack_len;
            }
            LUX_Record_free_packet(one_frame_head);
        }

        LUX_Video_Encoder_ReleaseStream(channel, &luxFrame);

        //usleep(15 * 1000);
    }

    pthread_exit(0);
}

void * thread_rtsp_do_event()
{
    Semaphore_Pend(&g_liveEvent, SEM_FOREVER);
    int ret;
    // pthread_mutex_lock(&event_mMutex);
    while(1){
    ret = rtsp_do_event(demo);
            if (ret > 0)
                continue;
            if (ret < 0)
                break;
            usleep(20000);
    }

}

int rtsp_demo_main()
{
    UINT_X timeCnt = 0;
    INT_X chnIdx = 0;

    int ret = LUX_AI_Start();

    //rtsp_init();

    //rtsp初始化
    rtsp_server_init();

    Semaphore_Create(0, &g_liveAudioSem);
    if (LuxTestEnbale == 0)/*工厂测试使用，工厂测试时，不进行视频推流*/
    {
        Semaphore_Create(0, &g_liveVideoMainSem);
        Semaphore_Create(0, &g_liveVideoSubSem);
    	Semaphore_Create(0, &g_liveEvent);

        /*Demo uses files to simulate audio/video/jpeg inputs.
        The actual data acquisition needs to be realized by developers. */
        pthread_t main_thread;
        pthread_create(&main_thread, NULL, thread_live_video, (void*)LUX_STREAM_MAIN);
        pthread_detach(main_thread);

        pthread_t sub_thread;
        pthread_create(&sub_thread, NULL, thread_live_video, (void*)LUX_STREAM_SUB);
        pthread_detach(sub_thread);
    }
    pthread_t audio_thread;
    pthread_create(&audio_thread, NULL, thread_live_audio, (void*)LUX_STREAM_MAIN);
    pthread_detach(audio_thread);
    //rtsp事件处理
    pthread_t rtsp_event;
    pthread_create(&rtsp_event, NULL, thread_rtsp_do_event, NULL);
    pthread_detach(rtsp_event);

    

    printf("\n********************* wait for 3 seconds *********************\n");
    sleep(3);
    /* 视频主、子码流开始推流 */
    if (LuxTestEnbale == 0)/*工厂测试使用，工厂测试时，不进行视频推流*/
    {
        Semaphore_Post(&g_liveVideoMainSem);
        Semaphore_Post(&g_liveVideoSubSem);
    }
    /* 音频开始推流 */
    Semaphore_Post(&g_liveAudioSem);
    Semaphore_Post(&g_liveEvent);

    if (LuxTestEnbale == 0)/*工厂测试使用，工厂测试时，不进行算法启动*/

    {
        if (g_configData->alg_config.human_detect_enabled)
        {
            LUX_IVS_PD_Init();//人形侦测初始化
        }
        if (g_configData->alg_config.face_detect_enabled)
        {
        LUX_IVS_FD_Init();//人脸侦测初始化
        }
        if (g_configData->alg_config.pir_detect_enabled || g_configData->alg_config.pir_detect_m_enabled)
        {
        LUX_IVS_PermDet_Init();//周界侦测初始化
        }
        if (g_configData->alg_config.tamper_det_enabled)
        {
        LUX_ALG_TamperDet_Init();//篡改检测初始化
        }
        if (g_configData->alg_config.object_det_enabled)
        {
        LUX_ALG_ObjDet_Init();//物品遗留丢失检测初始化
        }
        if (g_configData->alg_config.crossline_det_enabled)
        {
        LUX_ALG_LineCrossDet_Init();//越线检测初始化
        }
        if (g_configData->alg_config.loiter_det_enabled)
        {
        LUX_ALG_LoiterDet_Init();//徘徊检测初始化
        }

    }

    UINT_X lastNum = 0;
#if 0
    while (1)
    {
        /* 60秒校时一次 */
        sleep(1);
    }
#endif

    return 0;
}

