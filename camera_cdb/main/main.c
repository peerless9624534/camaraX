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

    audioCtx->channels = 1; //通道数量
    audioCtx->channel_layout = AV_CH_LAYOUT_MONO; //通道类型
    audioCtx->sample_rate = 16000; //采样率
    audioCtx->sample_fmt = AV_SAMPLE_FMT_FLTP; //采样数据格式

    audioCtx->bit_rate = 64000; //码率
    audioCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER; //公共头部

    ret = avcodec_open2(audioCtx, audioCodec, NULL);
    if (ret != 0) {
        printf("avcodec_open2 failed:%d \n", ret);
        return -1;
    }

    //创建流
    audioStream = avformat_new_stream(audioFmtCtx, NULL);
    audioStream->codecpar->codec_tag = 0;

    //拷贝参数到流中
    avcodec_parameters_from_context(audioStream->codecpar, audioCtx);

    //打开输出文件(aac文件)
    ret = avio_open(&audioFmtCtx->pb, aacfile, AVIO_FLAG_WRITE);
    if (ret < 0) {
        printf("avio_open failed:%d \n", ret);
        return -1;
    }

    //写入头文件(成功以后会创建一个输出的文件)
    avformat_write_header(audioFmtCtx, NULL);

    /**
     * SwrContext参数设置
     * 第一个参数SwrContext
     * 第二个参数输出的通道类型
     * 第三个参数输出的采样格式
     * 第四个参数输出的采样率
     * 第五个参数输入的通道类型
     * 第六个参数输入的采样格式
     * 第七个参数输入的采样率
     * 第八个参数偏移量
     * 第九个参数是否需要日志
     */
    swrCtx = swr_alloc_set_opts(NULL, audioCtx->channel_layout,
                                audioCtx->sample_fmt, audioCtx->sample_rate,
                                AV_CH_LAYOUT_MONO, AV_SAMPLE_FMT_S16,
                                16000, 0, 0);
    if (!swrCtx) {
        printf("swr_alloc_set_opts failed \n");
        return -1;
    }

    //初始化SwrContext
    ret = swr_init(swrCtx);
    if (ret < 0) {
        printf("SwrContext swr_init failed:%d \n", ret);
        return -1;
    }

    //初始化audioFrame
    audioFrame = av_frame_alloc();
    audioFrame->format = AV_SAMPLE_FMT_FLTP;
    audioFrame->channels = 1;
    audioFrame->channel_layout = AV_CH_LAYOUT_MONO;
    audioFrame->nb_samples = 1024; //一帧音频样本大小

    ret = av_frame_get_buffer(audioFrame, 0);
    if (ret < 0) {
        printf("audioFrame create failed:%d \n", ret);
        return -1;
    }

    //读取pcm数据一帧样本的大小  样本数*通道数*存储格式16位还是其他等等
    int readSize = audioFrame->nb_samples * 2 * 2;

    //创建缓存空间,用来接收每一帧读取到的pcm数据
    char* pcmBuffer =  malloc(readSize);

    //打开输入文件
    FILE* file = fopen(pcmfile, "rb");

    //循环读取数据
    for (;;) {
        int len = fread(pcmBuffer, 1, readSize, file);
        if (len <= 0) {
            printf("file read finished! \n");
            break;
        }

        //用于把读取到的每一帧pcm数据进行一个强制转换成uint8_t
        const uint8_t* pcmData[1];
        pcmData[0] = (uint8_t*)pcmBuffer;

        //重采样
        swr_convert(swrCtx, audioFrame->data, audioFrame->nb_samples,
                    pcmData, 1024);

        //AVPacket初始化
        AVPacket pkt;
        av_init_packet(&pkt);

        //开始编码
        int ret;

        //开始编码
        ret = avcodec_send_frame(audioCtx, audioFrame);
        if (ret != 0) continue;

        //接收解码数据
        ret = avcodec_receive_packet(audioCtx, &pkt);
        if (ret != 0) continue;

        //对编码后数据进行参数设置
        pkt.stream_index = audioStream->index;
        pkt.pts = 0;
        pkt.dts = 0;

        av_interleaved_write_frame(audioFmtCtx, &pkt);
        printf("write local file success! \n");
    }

    //释放资源
    free(pcmBuffer);
    pcmBuffer = NULL;

    //写入文件索引
    av_write_trailer(audioFmtCtx);

    //关闭打开输出文件的流
    avio_close(audioFmtCtx->pb);

    //关闭编码器
    avcodec_close(audioCtx);

    //清理编码器设置的参数
    avcodec_free_context(&audioCtx);

    //清理输出上下文的参数
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
