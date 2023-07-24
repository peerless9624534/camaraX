#include <stdio.h>
#include <sample-encode-video.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <comm_def.h>
#include <lux_base.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <lux_record.h>
#include <AppVersion.h>
#include "lux_factory_test.h"
#include "lux_global.h"
//#include <libavcodec/avcodec.h>
//#include <libavformat/avformat.h>
//#include <libswscale/swscale.h>
//#include <libswresample/swresample.h>

//#include <jwhead.h>

void signalHandler( int signum )
{
    char cThreadName[32] = {0};

    prctl(PR_GET_NAME, (unsigned long)cThreadName);

    switch(signum)
    {
        case SIGABRT:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGABRT(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGALRM:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGALRM(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGBUS:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGBUS(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGCHLD:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGCHLD(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGCONT:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGCONT(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGEMT:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGEMT(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGFPE:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGFPE(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGHUP:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGHUP(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGILL:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGILL(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        //case SIGINT:
        //  printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGINT(%d) thread = %ld name = %s++++++++++++++++++\n",
         //   __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
         // break;
        case SIGIO:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGIO(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        /*case SIGIOT:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGIOT(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;*/
        case SIGKILL:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGKILL(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGPIPE:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGPIPE(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        /*case SIGPOLL:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGPOLL(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;*/
        case SIGPROF:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGPROF(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGPWR:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGPWR(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGQUIT:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGQUIT(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
            exit(0);
          break;
        case SIGSEGV:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGSEGV(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGSTOP:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGSTOP(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGSYS:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGSYS(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGTERM:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGTERM(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGTRAP:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGTRAP(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGTSTP:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGTSTP(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
            exit(0);
          break;
        case SIGTTIN:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGTTIN(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGTTOU:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGTTOU(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGURG:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGURG(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGVTALRM:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGVTALRM(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGWINCH:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGWINCH(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGXCPU:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGXCPU(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        case SIGXFSZ:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = SIGXFSZ(%d) thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, signum, syscall(__NR_gettid), cThreadName);
          break;
        default:
          printf("\r\n ++++++++++++++++++ [%s]%d: signal = unkown signal thread = %ld name = %s++++++++++++++++++\n",
            __func__, __LINE__, syscall(__NR_gettid), cThreadName);
          break;
    }

    return;

}

#if 0
int pcm_aac(const char* pcmfile, const char* aacfile)
{
    AVCodec* audioCodec;
    AVFormatContext* audioFmtCtx;
    AVCodecContext* audioCtx;
    AVStream* audioStream;
    SwrContext* swrCtx;
    AVFrame* audioFrame;

    int ret = 0;

    ret = avformat_alloc_output_context2(&audioFmtCtx, NULL, NULL, aacfile);
    
    if (ret < 0) {
        printf("avformat_alloc_output_context2 fail:%d \n", ret);
        return -1;
    }

    audioCodec = avcodec_find_encoder(AV_CODEC_ID_AAC);
    if (!audioCodec)
    {
        printf("avcodec_find_encoder(AV_CODEC_ID_AAC) failed \n ");
        return -1;
    }

    audioCtx = avcodec_alloc_context3(audioCodec);
    if (!audioCtx) {
        printf("avcodec_alloc_context3 failed:%d \n");
        return -1;
    }

    audioCtx->channels = 1; //ͨ������
    audioCtx->channel_layout = AV_CH_LAYOUT_MONO; //ͨ������
    audioCtx->sample_rate = 16000; //������
    audioCtx->sample_fmt = AV_SAMPLE_FMT_FLTP; //�������ݸ�ʽ

    audioCtx->bit_rate = 64000; //����
    audioCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER; //����ͷ��

    ret = avcodec_open2(audioCtx, audioCodec, NULL);
    if (ret != 0) {
        printf("avcodec_open2 failed:%d \n", ret);
        return -1;
    }

    //������
    audioStream = avformat_new_stream(audioFmtCtx, NULL);
    audioStream->codecpar->codec_tag = 0;

    //��������������
    avcodec_parameters_from_context(audioStream->codecpar, audioCtx);

    //������ļ�(aac�ļ�)
    ret = avio_open(&audioFmtCtx->pb, aacfile, AVIO_FLAG_WRITE);
    if (ret < 0) {
        printf("avio_open failed:%d \n", ret);
        return -1;
    }

    //д��ͷ�ļ�(�ɹ��Ժ�ᴴ��һ��������ļ�)
    avformat_write_header(audioFmtCtx, NULL);

    /**
     * SwrContext��������
     * ��һ������SwrContext
     * �ڶ������������ͨ������
     * ��������������Ĳ�����ʽ
     * ���ĸ���������Ĳ�����
     * ��������������ͨ������
     * ��������������Ĳ�����ʽ
     * ���߸���������Ĳ�����
     * �ڰ˸�����ƫ����
     * �ھŸ������Ƿ���Ҫ��־
     */
    swrCtx = swr_alloc_set_opts(NULL, audioCtx->channel_layout,
                                audioCtx->sample_fmt, audioCtx->sample_rate,
                                AV_CH_LAYOUT_MONO, AV_SAMPLE_FMT_S16,
                                16000, 0, 0);
    if (!swrCtx) {
        printf("swr_alloc_set_opts failed \n");
        return -1;
    }

    //��ʼ��SwrContext
    ret = swr_init(swrCtx);
    if (ret < 0) {
        printf("SwrContext swr_init failed:%d \n", ret);
        return -1;
    }

    //��ʼ��audioFrame
    audioFrame = av_frame_alloc();
    audioFrame->format = AV_SAMPLE_FMT_FLTP;
    audioFrame->channels = 1;
    audioFrame->channel_layout = AV_CH_LAYOUT_MONO;
    audioFrame->nb_samples = 1024; //һ֡��Ƶ������С

    ret = av_frame_get_buffer(audioFrame, 0);
    if (ret < 0) {
        printf("audioFrame create failed:%d \n", ret);
        return -1;
    }

    //��ȡpcm����һ֡�����Ĵ�С  ������*ͨ����*�洢��ʽ16λ���������ȵ�
    int readSize = audioFrame->nb_samples * 2 * 2;

    //��������ռ�,��������ÿһ֡��ȡ����pcm����
    char* pcmBuffer =  malloc(readSize);

    //�������ļ�
    FILE* file = fopen(pcmfile, "rb");

    //ѭ����ȡ����
    for (;;) {
        int len = fread(pcmBuffer, 1, readSize, file);
        if (len <= 0) {
            printf("file read finished! \n");
            break;
        }

        //���ڰѶ�ȡ����ÿһ֡pcm���ݽ���һ��ǿ��ת����uint8_t
        const uint8_t* pcmData[1];
        pcmData[0] = (uint8_t*)pcmBuffer;

        //�ز���
        swr_convert(swrCtx, audioFrame->data, audioFrame->nb_samples,
                    pcmData, 1024);

        //AVPacket��ʼ��
        AVPacket pkt;
        av_init_packet(&pkt);

        //��ʼ����
        int ret;

        //��ʼ����
        ret = avcodec_send_frame(audioCtx, audioFrame);
        if (ret != 0) continue;

        //���ս�������
        ret = avcodec_receive_packet(audioCtx, &pkt);
        if (ret != 0) continue;

        //�Ա�������ݽ��в�������
        pkt.stream_index = audioStream->index;
        pkt.pts = 0;
        pkt.dts = 0;

        av_interleaved_write_frame(audioFmtCtx, &pkt);
        printf("write local file success! \n");
    }

    //�ͷ���Դ
    free(pcmBuffer);
    pcmBuffer = NULL;

    //д���ļ�����
    av_write_trailer(audioFmtCtx);

    //�رմ�����ļ�����
    avio_close(audioFmtCtx->pb);

    //�رձ�����
    avcodec_close(audioCtx);

    //������������õĲ���
    avcodec_free_context(&audioCtx);

    //������������ĵĲ���
    avformat_free_context(audioFmtCtx);

    return 0;
}
#endif

int main(int argc, char *argv[])
{
    printf("****************************************************************\n");
    printf("*                  \033[44;37;5m CameraX  System  \033[0m                          *\n");
    printf("*      App Version: %-43s*\n", APP_DETAIL_VERSION);
    printf("*       Build Date: %-43s*\n", BUILD_DATE);
    printf("*       Repository: *\n");
    printf("*      Branch Name: *\n");
    printf("*      Commit Date: *\n");
    printf("*      Commit   ID: *\n");
    printf("****************************************************************\n");

    signal(SIGPIPE, signalHandler);

#if 0
    unsigned char buf[1024];
    int i;
    for (i = 0; i < 1024; i++)
    {
        buf[i] = i%256;
    }

    FILE* fp1 = fopen("/tmp/1.data", "wb");
    fwrite(buf, 1, 1024, fp1);
    fflush(fp1);
    FILE* fp2 = fopen("/tmp/1.data", "rb");

    if (fp2)
    {
        printf("share read success \n");
        memset(buf, 0, 1024);
        int readlen = fread(buf, 1, 1024, fp2);
        printf("read len:%d \n", readlen);

        for (i = 0; i < 1024; i++)
        {
            printf("%d ", buf[i]);
        }
    }
    else
    {
        printf("share read failed \n");
    }

    exit(1);

#endif

    if (argv[1] == NULL)
    {
       
        printf("**************** start media_sample program! ****************\n");

        LUSHARE_test_sample_Encode_video(argc, argv[1]);

        printf("**************** finished media_sample program! ****************\n"); 
        return 0;
    }
    else
    {
        if (0 == strcmp(argv[1], "LuxTestEnter"))
        {
            printf("**************** start lux_factory_test program! ****************\n");
            if (0 == strcmp(argv[2], "LuxTestAudio"))
            {
                LuxTestEnbale = 1;
            }
            lux_fatory_test_init(argc, argv);
            printf("**************** finished lux_factory_test program! ****************\n");
        }
        else
        {
            printf("**************** Startup parameters are incorrect! ****************\n");
        }
    }


    return 0;

}
