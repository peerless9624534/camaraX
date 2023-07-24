
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <endian.h>
#include "lux_base.h"
#include "lux_time.h"
#include <jwhead.h>

char* lux_get_ip_by_domain(const char* const domain)
{
    int i = 0;
    char* ipaddr = NULL;
    struct hostent* _hostent = NULL;
    _hostent = gethostbyname(domain);
    
    if (NULL == _hostent)
    {
        MYWARN("_hostent:[%p] is null\n", _hostent);
        return NULL;
    }
    while (_hostent->h_addr_list[i] != NULL)
    {
        ipaddr = inet_ntoa(*((struct in_addr*)_hostent->h_addr_list[i]));
        printf("ip addr%d: %s\n", i + 1, ipaddr);
        if (ipaddr != NULL)
        {
            break;
        }
        i++;
    }
    return ipaddr;
}

in_addr_t lux_inet_host(const char* host)
{
    in_addr_t saddr;
    struct hostent* hostent;

    if ((saddr = inet_addr(host)) == INADDR_NONE) {
        if ((hostent = gethostbyname(host)) == NULL)
            return INADDR_NONE;

        memmove(&saddr, hostent->h_addr_list, hostent->h_length);
    }

    return saddr;
}


int lux_get_ntp_packet(void* buf, size_t* size)  //构建并发送NTP请求报文
{
    struct ntphdr* ntp;
    struct timeval tv;

    if (!size || *size < NTP_HLEN)
        return -1;

    memset(buf, 0, *size);

    ntp = (struct ntphdr*)buf;
    ntp->ntp_li = NTP_LI;
    ntp->ntp_vn = NTP_VN;
    ntp->ntp_mode = NTP_MODE;
    ntp->ntp_stratum = NTP_STRATUM;
    ntp->ntp_poll = NTP_POLL;
    ntp->ntp_precision = NTP_PRECISION;

    gettimeofday(&tv, NULL);  //把目前的时间用tv 结构体返回
    ntp->ntp_transts.intpart = htonl(tv.tv_sec + JAN_1970);
    ntp->ntp_transts.fracpart = htonl(USEC2FRAC(tv.tv_usec));

    *size = NTP_HLEN;

    return 0;
}

double lux_get_rrt(const struct ntphdr* ntp, const struct timeval* recvtv)  //往返时延
{
    double t1, t2, t3, t4;

    t1 = NTP_LFIXED2DOUBLE(&ntp->ntp_orits);
    t2 = NTP_LFIXED2DOUBLE(&ntp->ntp_recvts);
    t3 = NTP_LFIXED2DOUBLE(&ntp->ntp_transts);
    t4 = recvtv->tv_sec + recvtv->tv_usec / 1000000.0;

    return (t4 - t1) - (t3 - t2);
}

double lux_get_offset(const struct ntphdr* ntp, const struct timeval* recvtv)  //偏移量
{
    double t1, t2, t3, t4;

    t1 = NTP_LFIXED2DOUBLE(&ntp->ntp_orits);
    t2 = NTP_LFIXED2DOUBLE(&ntp->ntp_recvts);
    t3 = NTP_LFIXED2DOUBLE(&ntp->ntp_transts);
    t4 = recvtv->tv_sec + recvtv->tv_usec / 1000000.0;

    return ((t2 - t1) + (t3 - t4)) / 2;
}

int64_t lux_get_timezone_offset(const char* timezone_info)
{
#if 0
    char symbol='0';
	char ch[2] = ":";
	char tmp[8];
	int cnt=0;
	int64_t ms = 0;
	memset(tmp, 0, sizeof(tmp));
	strcpy(tmp, timezone_info);
	char* ptr = strtok(tmp, ch); // 实现字符串的分割
	while(ptr != NULL) 
	{		
		printf("%s\n", ptr);
		if ((cnt == 0) && ((symbol = *ptr) == '0'))
		{
			return 0;
		}
		else if ((cnt == 0) && ((symbol = *ptr) == '+'))
		{
			ms += atoi(ptr+1) * 3600;
		}
		else if ((cnt == 1) && (symbol == '+'))
		{
			ms += atoi(ptr) * 60;
		}
		else if ((cnt == 0) && ((symbol = *ptr) == '-'))
		{
			ms -= atoi(ptr+1) * 3600;
		}
		else if ((cnt == 1) && (symbol == '-'))
		{
			ms -= atoi(ptr) * 60;
		}
		cnt++;
		ptr = strtok(NULL, ch); 
	}
	return ms;
#else
    char tmp[8];
    char* ptr = tmp;
    memset(tmp, 0, sizeof(tmp));
    strcpy(tmp, timezone_info);
    int iPrefix = tmp[0] == '-' ? -1 : 1;

    return iPrefix * (3600*atoi(ptr+1) + 60*atoi(ptr+4));

#endif
}

int lux_time_ntp_sync(const char* timezone_info)
{
    char dateBuf[64] = { 0 };
    char cmd[128] = { 0 };
    struct tm* local;
    char buf[BUFSIZE];
    size_t nbytes;
    int sockfd, maxfd1;
    struct sockaddr_in servaddr;
    fd_set readfds;
    struct timeval timeout, recvtv, tv;
    double offset;
    int64_t zone_offset;
    uid_t uid = 0;

    char* ntp_server_ip = lux_get_ip_by_domain(NTP_SERVER_DOMAIN);
    if (NULL == ntp_server_ip)
    {
        MYERROR("ntp_server_ip is NULL \n");
        return -1;
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(NTP_PORT);
    servaddr.sin_addr.s_addr = lux_inet_host(ntp_server_ip);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket error");
        return -1;
    }

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr)) != 0) {
        perror("connect error");
        return -1;
    }
    nbytes = BUFSIZE;
    if (lux_get_ntp_packet(buf, &nbytes) != 0) {
        fprintf(stderr, "construct ntp request error \n");
        return -1;
    }
    send(sockfd, buf, nbytes, 0);

    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);
    maxfd1 = sockfd + 1;

    timeout.tv_sec = TIMEOUT;
    timeout.tv_usec = 0;

    if (select(maxfd1, &readfds, NULL, NULL, &timeout) > 0)
    {
        if (FD_ISSET(sockfd, &readfds))
        {
            if ((nbytes = recv(sockfd, buf, BUFSIZE, 0)) < 0)
            {
                close(sockfd);
                MYERROR("udp recv error \n");
                return -1;
            }

            //计算C/S时间偏移量
            gettimeofday(&recvtv, NULL);
            offset = lux_get_offset((struct ntphdr*)buf, &recvtv);
            zone_offset = lux_get_timezone_offset(timezone_info);
            //更新系统时间
            gettimeofday(&tv, NULL);
            tv.tv_sec += (int)offset + zone_offset;
            tv.tv_usec += offset - (int)offset;

            // uid = getuid();//普通用户的uid
            // printf("************************** common user uid = %u **************************\n",uid);
            // if (setuid(0)) //设置为普通用户和超级用户一样的权限
            // {
            //     printf("Set root user failed!\n");
            //     return -1;
            // }
            // else
            // {
            //     printf("Set root user success! uid = %u\n", getuid());
            //     if (settimeofday(&tv, (struct timezone*)8) < 0)
            //     {
            //         printf("Set system date time failed!\n");
            //         return -1;
            //     }
            // }
            
            
            // if (setuid(uid)) //恢复uid，恢复到只有普通用户的权限
            // {
            //     printf("Set common user failed!\n");
            //     return -1;
            // }
            // printf("Set common user success! uid = %u\n", getuid());
            local = localtime((time_t*)&tv.tv_sec);
            strftime(dateBuf, 64, "%Y-%m-%d %H:%M:%S", local);
            sprintf(cmd, "date -s \"%s\"", dateBuf);
            printf("NTP synchronization time: [%s] \n", ctime((time_t*)&tv.tv_sec));
            LUX_BASE_System_CMD(cmd);
        }
    }

    close(sockfd);
    return 0;
}

int lux_time_ntp_cmd_sync()
{
    int ret = 0;
    char tmpCmd[32] = { 0 };
    char* ntp_server_ip = lux_get_ip_by_domain(NTP_SERVER_DOMAIN);

    if (NULL == ntp_server_ip)
    {
        MYERROR("ntp_server_ip is NULL \n");
        return -1;
    }

    // setTimeStampdDiff(time_zone);
    // date -R;查看设备时区
    sprintf(tmpCmd, "ntpd -p %s -qNn", ntp_server_ip);
    ret = LUX_BASE_System_CMD(tmpCmd);
    if (ret != 0)
    {
        MYERROR("ntpd Set Server Time Failed\n");
        return ret;
    }
    MYTRACE("ntpd Set Server Time Success\n");
    return 0;
}
