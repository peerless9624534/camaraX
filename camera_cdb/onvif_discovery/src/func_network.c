#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "func_network.h"

#include "func_log.h"
#define LOG_TAG "ONVIF"

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char tmp_buf[20];
unsigned char HOST_MAC[6] = {0};
int device_network_get_ip_gateway(char *interface, char *gateway);

int device_ip_get(char *interface, char *addr)
{
	int ret;
    int inet_sock;
    struct ifreq ifr;

    inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
    strcpy(ifr.ifr_name, "eth0");
	ret = ioctl(inet_sock, SIOCGIFADDR, &ifr);
	if (ret < 0)
		return ret;

	strcpy(addr, inet_ntoa(((struct sockaddr_in *)&(ifr.ifr_addr))->sin_addr));

	return 0;
}

#ifndef DISCOVERY_ONLY
void device_ip_set(int signal)
{
	unsigned char cmd[50];
	unsigned char *str_tmp = tmp_buf;
	int i = 0;
	int ret = 0;
	char gateway [20] = {0};

	ret = device_network_get_ip_gateway("eth0",gateway);

	memset(cmd, 0x00, sizeof(char) * 50);
	sprintf(cmd, "ifconfig eth0 %s", tmp_buf);
	system(cmd);

	if (ret == 0) {
		memset(cmd, 0, sizeof(cmd));
		sprintf(cmd,"route add default gw %s",gateway);
		system(cmd);
	}

	return;
}
#endif

int device_mac_get(char *interface, unsigned char *pMac)
{
	struct ifreq ifreq;
	int sockfd = 0;
	unsigned char mac[6] = {0};

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;

	strcpy(ifreq.ifr_name, interface);

	if(ioctl(sockfd, SIOCGIFHWADDR, &ifreq) < 0)
	{
		close(sockfd);
		return -2;
	}

	memcpy(mac, ifreq.ifr_hwaddr.sa_data, 6);
	LOG_DBG(LOG_TAG, "MAC:%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	if(pMac != NULL)
	{
		memcpy(pMac, mac, 6);
	}

	close(sockfd);

	return 0;
}

int device_mac_get_by_str(char *interface, unsigned char *pMac)
{
	struct ifreq ifreq;
	int sockfd = 0;
	unsigned char mac[6] = {0};

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;

	strcpy(ifreq.ifr_name, interface);

	if(ioctl(sockfd, SIOCGIFHWADDR, &ifreq) < 0)
	{
		close(sockfd);
		return -2;
	}

	memcpy(mac, ifreq.ifr_hwaddr.sa_data, 6);
	LOG_DBG(LOG_TAG, "MAC:%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	sprintf((char*)pMac, "%02X-%02X-%02X-%02X-%02X-%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	close(sockfd);

	return 0;
}

int device_network_set_ip_gateway(char *gateway)
{
	char cmd[128] = {0};

	/*先删除*/
	sprintf(cmd,"route del default");
	system(cmd);

	/*后添加*/
	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd,"route add default gw %s",gateway);
	system(cmd);

	return 0;
}

int device_network_get_ip_gateway(char *interface, char *gateway)
{
	FILE *fp = NULL;
	int i = 0;
	char cmd[128] = {'\0'};

	sprintf(cmd ,"route -n | grep %s | grep UG | awk '{print $2}' ",interface);
	//sprintf(cmd ,"route | grep %s | grep default | awk '{print $2}' ",interface);

	fp = popen(cmd,"r");
	if ( fp == NULL ) {
		LOG_ERR(LOG_TAG,"error(%s,%d),popen fail,get gateway fail\n",__func__,__LINE__);
		return -1;
	}

	fread(gateway,1,19,fp);
	pclose(fp);

	char * temp = gateway;
	for (i = 0; i < 19;i++ ) {
		if (*temp == '\0' )
			break;
		if (*temp == '\n')
			*temp = '\0';
		temp ++;
	}

	return 0;
}

int device_network_set_ip_mask(char *interface, char *mask)
{
	char cmd[128] = {0};
	char gateway [20] = {0};
	int ret = 0;

	/*这步必须要在设置掩码之前*/
	ret = device_network_get_ip_gateway(interface,gateway);

	sprintf(cmd,"ifconfig %s netmask %s",interface,mask);
	system(cmd);

	if (ret == 0) {
		memset(cmd, 0, sizeof(cmd));
		sprintf(cmd,"route add default gw %s",gateway);
		system(cmd);
	}

	return 0;
}

#ifdef __cplusplus
}
#endif
