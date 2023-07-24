#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>

#include "rtp.h"
#include "lux_video.h"
#include "imp_encoder.h"
#include "imp_log.h"

#define TAG "rtsp_demo"

#define H264_FILE_NAME   "test.h264"
#define SERVER_PORT      8554
#define SERVER_RTP_PORT  55532
#define SERVER_RTCP_PORT 55533
#define BUF_MAX_SIZE     (1024*1024)

/* 功能简述:RTSP服务器实现与客户端的交互，通过RTP协议向客户端发送当前文件夹下的.h264文件 */

static int createTcpSocket()
{
    int sockfd;
    int on = 1;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        return -1;
    }

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on));

    return sockfd;
}

static int createUdpSocket()
{
    int sockfd;
    int on = 1;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
    {
        return -1;
    }

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on));

    return sockfd;
}

static int bindSocketAddr(int sockfd, char* ip, int port)
{
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    if(bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr)) < 0)
    {
        return -1;
    }

    return 0;
}

static int acceptClient(int sockfd, char* ip, int* port)
{
    int clientfd;
    socklen_t len = 0;
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    len = sizeof(addr);

    clientfd = accept(sockfd, (struct sockaddr *)&addr, &len);
    if(clientfd < 0)
    {
        return -1;
    }

    strcpy(ip, inet_ntoa(addr.sin_addr));
    *port = ntohs(addr.sin_port);

    return clientfd;
}

static inline int startCode3(char* buf)
{
    if(buf[0] == 0 && buf[1] == 0 && buf[2] == 1)
    {
        return 1;
    }
    else
     {
        return 0;
     }
}

static inline int startCode4(char* buf)
{
    if(buf[0] == 0 && buf[1] == 0 && buf[2] == 0 && buf[3] == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#if 0
static char* findNextStartCode(char* buf, int len)
{
    int i;

    if(len < 3)
    {
        return NULL;
    }

    for(i = 0; i < len-3; ++i)
    {
        if(startCode3(buf) || startCode4(buf))
        {
            return buf;
        }
        
        ++buf;
    }

    if(startCode3(buf))
    {
        return buf;
    }

    return NULL;
}

static int getFrameFromH264File(int fd, char* frame, int size)
{
    int rSize, frameSize;
    char* nextStartCode;

    if(fd < 0)
    {
        return fd;
    }

    rSize = read(fd, frame, size);
    if(!startCode3(frame) && !startCode4(frame))
    {
        return -1;
    }
    
    nextStartCode = findNextStartCode(frame+3, rSize-3);
    if(!nextStartCode)
    {
        //lseek(fd, 0, SEEK_SET);
        //frameSize = rSize;
        return -1;
    }
    else
    {
        frameSize = (nextStartCode-frame);
        lseek(fd, frameSize-rSize, SEEK_CUR);
    }

    return frameSize;
}
#endif
static int rtpSendH264Frame(int socket, char* ip, int16_t port,
                            struct RtpPacket* rtpPacket, uint8_t* frame, uint32_t frameSize)
{
    uint8_t naluType; // nalu第一个字节
    int sendBytes = 0;
    int ret;

    naluType = frame[0];

    if (frameSize <= RTP_MAX_PKT_SIZE) // nalu长度小于最大包场：单一NALU单元模式
    {
        /*
         *   0 1 2 3 4 5 6 7 8 9
         *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
         *  |F|NRI|  Type   | a single NAL unit ... |
         *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
         */
        memcpy(rtpPacket->payload, frame, frameSize);
        ret = rtpSendPacket(socket, ip, port, rtpPacket, frameSize);
        if(ret < 0)
        {
            return -1;
        }

        rtpPacket->rtpHeader.seq++;
        sendBytes += ret;
        if ((naluType & 0x1F) == 7 || (naluType & 0x1F) == 8) // 如果是SPS、PPS就不需要加时间戳
        {
            goto out;
        }
    }
    else // nalu长度小于最大包长：分片模式
    {
        /*
         *  0                   1                   2
         *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
         * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
         * | FU indicator  |   FU header   |   FU payload   ...  |
         * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
         */

        /*
         *     FU Indicator
         *    0 1 2 3 4 5 6 7
         *   +-+-+-+-+-+-+-+-+
         *   |F|NRI|  Type   |
         *   +---------------+
         */

        /*
         *      FU Header
         *    0 1 2 3 4 5 6 7
         *   +-+-+-+-+-+-+-+-+
         *   |S|E|R|  Type   |
         *   +---------------+
         */

        int pktNum = frameSize / RTP_MAX_PKT_SIZE;       // 有几个完整的包
        int remainPktSize = frameSize % RTP_MAX_PKT_SIZE; // 剩余不完整包的大小
        int i, pos = 1;

        /* 发送完整的包 */
        for (i = 0; i < pktNum; i++)
        {
            rtpPacket->payload[0] = (naluType & 0x60) | 28;
            rtpPacket->payload[1] = naluType & 0x1F;
            
            if (i == 0) //第一包数据
            {
                rtpPacket->payload[1] |= 0x80; // start
            }
            else if (remainPktSize == 0 && i == pktNum - 1) //最后一包数据
            {
                rtpPacket->payload[1] |= 0x40; // end
            }

            memcpy(rtpPacket->payload+2, frame+pos, RTP_MAX_PKT_SIZE);
            ret = rtpSendPacket(socket, ip, port, rtpPacket, RTP_MAX_PKT_SIZE+2);
            if(ret < 0)
            {
                return -1;
            }

            rtpPacket->rtpHeader.seq++;
            sendBytes += ret;
            pos += RTP_MAX_PKT_SIZE;
        }

        /* 发送剩余的数据 */
        if (remainPktSize > 0)
        {
            rtpPacket->payload[0] = (naluType & 0x60) | 28;
            rtpPacket->payload[1] = naluType & 0x1F;
            rtpPacket->payload[1] |= 0x40; //end

            memcpy(rtpPacket->payload+2, frame+pos, remainPktSize+2);
            ret = rtpSendPacket(socket, ip, port, rtpPacket, remainPktSize+2);
            if(ret < 0)
            {
                return -1;
            }

            rtpPacket->rtpHeader.seq++;
            sendBytes += ret;
        }
    }

out:

    return sendBytes;
}

static char* getLineFromBuf(char* buf, char* line)
{
    while(*buf != '\n')
    {
        *line = *buf;
        line++;
        buf++;
    }

    *line = '\n';
    ++line;
    *line = '\0';

    ++buf;
    return buf; 
}

static int handleCmd_OPTIONS(char* result, int cseq)
{
    sprintf(result, "RTSP/1.0 200 OK\r\n"
                    "CSeq: %d\r\n"
                    "Public: OPTIONS, DESCRIBE, SETUP, PLAY\r\n"
                    "\r\n",
                    cseq);
                
    return 0;
}

static int handleCmd_DESCRIBE(char* result, int cseq, char* url)
{
    char sdp[500];
    char localIp[100];

    sscanf(url, "rtsp://%[^:]:", localIp);

    sprintf(sdp, "v=0\r\n"
                 "o=- 9%ld 1 IN IP4 %s\r\n"
                 "t=0 0\r\n"
                 "a=control:*\r\n"
                 "m=video 0 RTP/AVP 96\r\n"
                 "a=rtpmap:96 H264/90000\r\n"
                 "a=control:track0\r\n",
                 time(NULL), localIp);
    
    sprintf(result, "RTSP/1.0 200 OK\r\nCSeq: %d\r\n"
                    "Content-Base: %s\r\n"
                    "Content-type: application/sdp\r\n"
                    "Content-length: %d\r\n\r\n"
                    "%s",
                    cseq,
                    url,
                    strlen(sdp),
                    sdp);
    printf("sdp:%s result:%s\n", sdp, result);
    return 0;
}

static int handleCmd_SETUP(char* result, int cseq, int clientRtpPort)
{
    sprintf(result, "RTSP/1.0 200 OK\r\n"
                    "CSeq: %d\r\n"
                    "Transport: RTP/AVP;unicast;client_port=%d-%d;server_port=%d-%d\r\n"
                    "Session: 66334873\r\n"
                    "\r\n",
                    cseq,
                    clientRtpPort,
                    clientRtpPort+1,
                    SERVER_RTP_PORT,
                    SERVER_RTCP_PORT);
    
    return 0;
}

static int handleCmd_PLAY(char* result, int cseq)
{
    sprintf(result, "RTSP/1.0 200 OK\r\n"
                    "CSeq: %d\r\n"
                    "Range: npt=0.000-\r\n"
                    "Session: 66334873; timeout=60\r\n\r\n",
                    cseq);
    
    return 0;
}

static void doClient(int clientSockfd, char* clientIP, int clientPort,
                                        int serverRtpSockfd, int serverRtcpSockfd)
{
    char method[40];
    char url[100];
    char version[40];
    int cseq;
    int clientRtpPort, clientRtcpPort;
    char *bufPtr;
    char* rBuf = malloc(BUF_MAX_SIZE);
    char* sBuf = malloc(BUF_MAX_SIZE);
    char line[400];
    int ret = 0, nr_pack = 0;

    while(1)
    {
        int recvLen;

        recvLen = recv(clientSockfd, rBuf, BUF_MAX_SIZE, 0);
        if(recvLen <= 0)
            goto out;

        rBuf[recvLen] = '\0';
        printf("---------------C->S--------------\n");
        printf("%s", rBuf);

        /* 解析方法 */
        bufPtr = getLineFromBuf(rBuf, line);
        if(sscanf(line, "%s %s %s\r\n", method, url, version) != 3)
        {
            printf("parse err:%s:%d\n", __func__, __LINE__);
            goto out;
        }

        /* 解析序列号 */
        bufPtr = getLineFromBuf(bufPtr, line);
        if(sscanf(line, "CSeq: %d\r\n", &cseq) != 1)
        {
            printf("parse err:%s:%d\n", __func__, __LINE__);
            goto out;
        }

        /* 如果是SETUP，那么就再解析client_port */
        if(!strcmp(method, "SETUP"))
        {
            while(1)
            {
                bufPtr = getLineFromBuf(bufPtr, line);
                if(!strncmp(line, "Transport:", strlen("Transport:")))
                {
                    sscanf(line, "Transport: RTP/AVP;unicast;client_port=%d-%d\r\n",
                                    &clientRtpPort, &clientRtcpPort);
                    break;
                }
            }
        }

        if(!strcmp(method, "OPTIONS"))
        {
            if(handleCmd_OPTIONS(sBuf, cseq))
            {
                printf("failed to handle options\n");
                goto out;
            }
        }
        else if(!strcmp(method, "DESCRIBE"))
        {
            if(handleCmd_DESCRIBE(sBuf, cseq, url))
            {
                printf("failed to handle describe\n");
                goto out;
            }
        }
        else if(!strcmp(method, "SETUP"))
        {
            if(handleCmd_SETUP(sBuf, cseq, clientRtpPort))
            {
                printf("failed to handle setup\n");
                goto out;
            }
        }
        else if(!strcmp(method, "PLAY"))
        {
            if(handleCmd_PLAY(sBuf, cseq))
            {
                printf("failed to handle play\n");
                goto out;
            }
        }
        else
        {
            goto out;
        }

        printf("---------------S->C--------------\n");
        printf("%s", sBuf);
        send(clientSockfd, sBuf, strlen(sBuf), 0);

        /* 开始播放，发送RTP包 */
        if(!strcmp(method, "PLAY"))
        {
            int frameSize, startCode;
            char* frame = malloc(500000);
            struct RtpPacket* rtpPacket = (struct RtpPacket*)malloc(500000);

            rtpHeaderInit(rtpPacket, 0, 0, 0, RTP_VESION, RTP_PAYLOAD_TYPE_H264, 0,
                            0, 0, 0x88923423);

            printf("start play\n");
            printf("client ip:%s\n", clientIP);
            printf("client port:%d\n", clientRtpPort);
            
            int i = 0;
            IMPEncoderStream tmpStru;
            IMPEncoderStream *stream = &tmpStru;

            while (1)
            {
                /* polling一帧数据 */
                ret = IMP_Encoder_PollingStream(0, 1000);
                if (0 != ret)
                {
                    printf("STREAM INFO: %s:%d\n", __func__, __LINE__);
                    continue;
                }

                /* Get H264 or H265 Stream */
                ret = IMP_Encoder_GetStream(0, stream, 1);
                if (0 != ret)
                {
                    printf("STREAM INFO: %s:%d\n", __func__, __LINE__);
                    continue;
                }

                nr_pack = stream->packCount;

                for (i = 0; i < nr_pack; i++)
                {
                    IMPEncoderPack *pack = &stream->pack[i];
                    if (pack->length)
                    {
                        uint32_t remSize = stream->streamSize - pack->offset;
                        if (remSize < pack->length)
                        {
                            memcpy((void*)frame, (void*)(stream->virAddr + pack->offset), remSize);
                            memcpy((void*)frame + remSize, (void*)stream->virAddr, pack->length - remSize);
                        }
                        else
                        {
                            memcpy((void *)frame, (void *)(stream->virAddr + pack->offset), pack->length);
                        }
                    }

                    if (startCode3(frame))
                        startCode = 3;
                    else
                        startCode = 4;

                    frameSize = pack->length - startCode;
                    rtpSendH264Frame(serverRtpSockfd, clientIP, clientRtpPort,
                                     rtpPacket, (uint8_t *)(frame + startCode), frameSize);
                    rtpPacket->rtpHeader.timestamp += 90000 / 25;
                }

                ret = IMP_Encoder_ReleaseStream(0, stream);
                if (0 != ret)
                {
                    printf("LUX_Video_Encoder_ReleaseStream error:%#x\n", ret);
                    continue;
                }
            }
            free(frame);
            free(rtpPacket);
            goto out;
        }

    }
out:
    printf("finish\n");
    close(clientSockfd);
    free(rBuf);
    free(sBuf);
}

int rtsp_demo_rtsp_demo_main()
{
    int serverSockfd;
    int serverRtpSockfd, serverRtcpSockfd;

    serverSockfd = createTcpSocket();  //创建socket
    if(serverSockfd < 0)
    {
        printf("failed to create tcp socket\n");
        return -1;
    }

    if(bindSocketAddr(serverSockfd, "0.0.0.0", SERVER_PORT) < 0)  
    {
        printf("failed to bind addr\n");
        return -1;
    }

    if(listen(serverSockfd, 10) < 0)
    {
        printf("failed to listen\n");
        return -1;
    }

    serverRtpSockfd = createUdpSocket();
    serverRtcpSockfd = createUdpSocket();
    if(serverRtpSockfd < 0 || serverRtcpSockfd < 0)
    {
        printf("failed to create udp socket\n");
        return -1;
    }

    if(bindSocketAddr(serverRtpSockfd, "0.0.0.0", SERVER_RTP_PORT) < 0 ||
        bindSocketAddr(serverRtcpSockfd, "0.0.0.0", SERVER_RTCP_PORT) < 0)
    {
        printf("failed to bind addr\n");
        return -1;
    }
    
    printf("rtsp://127.0.0.1:%d\n", SERVER_PORT);

    while(1)
    {
        int clientSockfd;
        char clientIp[40];
        int clientPort;

        clientSockfd = acceptClient(serverSockfd, clientIp, &clientPort);  //阻塞等待连接
        if(clientSockfd < 0)
        {
            printf("failed to accept client\n");
            return -1;
        }

        printf("accept client;client ip:%s,client port:%d\n", clientIp, clientPort);

        doClient(clientSockfd, clientIp, clientPort, serverRtpSockfd, serverRtcpSockfd);
    }

    return 0;
}