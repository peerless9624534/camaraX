#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <pthread.h>
#include "lux_iniparse.h"
#include "lux_base.h"

#include "idip/details/idip_stream_wscan.h"
#include "idip.h"
#include "cJSON.h"
#include "lux_qrcode.h"
#include "lux_wifi.h"
#include <jwhead.h>
//-------------------------------------------------------------------
// static BOOL_X sniffer_set_done = LUX_FALSE;
// extern char g_parseStr[QR_PARSE_STRLEN];
extern char g_ssidStr[WIFI_SSID_LEN+1];
extern char g_passwdStr[WIFI_PASSWD_LEN+1];
BOOL_X wifiFlag=LUX_FALSE;

//-------------------------------------------------------------------
int lux_wf_check_connect()
{
    FILE* pp = popen("ifconfig "WLAN_DEV"", "r");
    if (pp == NULL)
    {
        printf("popen fails\r\n");
        return LUX_FALSE;
    }

    char tmp[256] = { 0 };
    memset(tmp, 0, sizeof(tmp));

    while (fgets(tmp, sizeof(tmp), pp) != NULL)
    {      
        {
            /* find the ip */
            char* pIpStart = strstr(tmp, "inet addr:");
            if (pIpStart != NULL)
            {
                pclose(pp);
                return 0;  
            }
            else
            {
                pclose(pp);
                return -1; 
            }    
        }
    }
    return -1;
}

int lux_wf_get_ssid(char* ssid)
{
    FILE* fp;
    char buf[512] = {0};
    char command[16] = "iwconfig wlan0";
    int ret = -1;
    char str[100];
    memset(str, '\0', sizeof(str));
    if((fp = popen(command, "r")) != NULL)
    {
        if(fgets(buf, 128, fp) != NULL)
        {
                char * q = strstr(buf, "ESSID:");
                if (q == NULL)
                {
                    pclose(fp);
                    return -1;
                }
                ret = 0;
                //正则表达式含义：匹配ESSID后引号内所有非引号的内容
                sscanf(q, "ESSID:\"%[^\"]\"", ssid);
        }
        pclose(fp);
    }
    //memcpy(ssid, str, strlen(str));
 
    return ret;
}

//-------------------------------------------------------------------
int lux_wf_scan_all(const char* real_ssid, idip_ap_security_type_t* auth_type)
{
    printf("\n****************** wifi scan start ********************\n");
    int rc = LUX_OK;
    idip_ap_description_t wifi_list[MAX_AP_SEARCH];
    memset(wifi_list, 0, sizeof(wifi_list));
    FILE* pp = popen("iwlist "WLAN_DEV" scan", "r");
    if (pp == NULL)
    {
        printf("popen fails\r\n");
        return LUX_ERR;
    }
    char tmp[256];
    char tmp_ssid[MAX_AP_SEARCH][WIFI_SSID_LEN + 1];
    memset(tmp, 0, sizeof(tmp));
    memset(tmp_ssid, 0 , sizeof(tmp_ssid));

    int recordIdx = -1;
    unsigned int ssidLen = 0;

    while (fgets(tmp, sizeof(tmp), pp) != NULL)
    {
         {
            /* find the ssid  */
            ssidLen = 0;
            char* pSSIDStart = strstr(tmp, "ESSID:\"");
            if (pSSIDStart != NULL)
            {
                recordIdx++;
                if(recordIdx >= MAX_AP_SEARCH)
                {
                    printf(" Reach Max Record \r\n");
                    recordIdx--;
                    break;
                }
                sscanf(pSSIDStart + strlen("ESSID:\""), "%s", tmp_ssid[recordIdx]);
                ssidLen = strlen(tmp_ssid[recordIdx]) - 1;
                tmp_ssid[recordIdx][ssidLen] = '\0';
                wifi_list[recordIdx].ssid = tmp_ssid[recordIdx];
                goto ReadNext;
            }
            else
            {
                if(recordIdx < 0)
                {/* find the first ssid to continue reading */
                    goto ReadNext;
                }
            }
        }

        {
            /* find signal */
            char* pSIGNALStart = strstr(tmp, "Signal level=");
            if (pSIGNALStart != NULL)
            { 
                int x = 0;
                int y = 0;
                sscanf(pSIGNALStart + strlen("Signal level="), "%d/%d ", &x, &y);
                wifi_list[recordIdx].rssi = x * 100 / (y + 1);
                goto ReadNext;
            }
        }

        {
            /* find the security(AP TYPE)  */
            char* pAPTypeStart = strstr(tmp, "Encryption key:");
            char apType[64];
            memset(apType, 0, 64);
            if (pAPTypeStart != NULL)
            {
                sscanf(pAPTypeStart + strlen("Encryption key:"), "%s", apType);
                if (0 == strcmp(apType, "off") && (0 != ssidLen))
                {   
                    wifi_list[recordIdx].security = AP_SEC_TYPE_OPEN;
                }
                goto ReadNext;   
            }
            else
            {
                pAPTypeStart = strstr(tmp, "IE: ");
                memset(apType, 0, 64);
                if (pAPTypeStart != NULL)
                {
                    sscanf(pAPTypeStart + strlen("IE: "), "%s", apType);
                    if (strstr(apType, "WPA"))
                    {   
                        wifi_list[recordIdx].security = AP_SEC_TYPE_WPA;
                    }
                    else if (strstr(apType, "IEEE"))
                    {
                        wifi_list[recordIdx].security = AP_SEC_TYPE_WPA2;
                    }
                    else if (strstr(apType, "WEP"))
                    {
                        wifi_list[recordIdx].security = AP_SEC_TYPE_WEP;
                    }
                    goto ReadNext; 
                }
            }
        }

    ReadNext:
        memset(tmp, 0, sizeof(tmp));
    }

    pclose(pp);
    
    int i = 0;
    idip_stream_wireless_scan_t stream;
    memset(&stream, 0, sizeof(stream));
    for (i = 0; i < recordIdx; ++i) {
        printf("\n****** wifi_list[%d] wifi_ssid: %s, wifi_type: %d, wifi_quality: %d ******\n",\
                                                    i,  wifi_list[i].ssid, wifi_list[i].security, wifi_list[i].rssi);
        if (strcmp(real_ssid, wifi_list[i].ssid) == 0)
        {
            *auth_type = wifi_list[i].security;
        }    
        rc = idip_stream_wireless_scan_put_ap_description(&stream, &wifi_list[i]);
    }
    printf("WIFI Scan AP End\r\n");
    return LUX_OK;
}

//-------------------------------------------------------------------
int lux_wf_connect(const char *ssidStr, const char *passwdStr)
{
    if ((NULL == ssidStr) || (passwdStr == NULL))
    {
        printf("\n****************** para is NULL ******************\n");
        return LUX_ERR;
    }
    
    printf("\n****************** wifi connect start ********************\n");
    // idip_status_code_t rc = IDIP_OK;
    // rc = lux_wifi_qrcode_setup();
    char tmpCmd[128] = { 0 };
    char ssidTmp[WIFI_SSID_LEN+1] = { 0 }; //32个有效字节
    char pswdTmp[WIFI_PASSWD_LEN+1] = { 0 }; //64个有效字节
    FILE* fp = NULL;
    printf("Request to connect to WiFi network %s. Password is %s\n",
    ssidStr,
    passwdStr);
    if (0 == strlen(ssidStr))
    {
        //get bind info from ethernet network
        printf("get bind info ...\n");
        sleep(1);
    }
    else
    {
        //get bind info from ap / wifi-smart / qrcode
        // 临时使用system连接网络，后续wpa功能开发完成合入
        printf("get wifi info ...\n");
        sprintf(ssidTmp, "%s", ssidStr);
        sprintf(pswdTmp, "%s", passwdStr);

        sprintf(tmpCmd, "wpa_cli -i %s add_network", WLAN_DEV);
        LUX_BASE_System_CMD(tmpCmd);

        sprintf(tmpCmd, "wpa_cli -i %s set_network 0 ssid '\"%s\"'", WLAN_DEV, ssidTmp);
        LUX_BASE_System_CMD(tmpCmd);

        if (0 == strlen(pswdTmp))
        {
            sprintf(tmpCmd, "wpa_cli -i %s set_network 0 key_mgmt NONE", WLAN_DEV);
            LUX_BASE_System_CMD(tmpCmd);
        }
        else
        {
            sprintf(tmpCmd, "wpa_cli -i %s set_network 0 psk '\"%s\"'", WLAN_DEV, pswdTmp);
            LUX_BASE_System_CMD(tmpCmd);
        }
        sprintf(tmpCmd, "wpa_cli -i %s enable_network 0", WLAN_DEV);
        LUX_BASE_System_CMD(tmpCmd);
        sleep(2);

        // sprintf(tmpCmd, "echo \"nameserver %s\" > /etc/resolv.conf", DNS_ADDR); /* 手动添加dns */
        // LUX_BASE_System_CMD(tmpCmd);
        sprintf(tmpCmd, "udhcpc -i %s", WLAN_DEV); /* -q:获取ok后退出*/
        LUX_BASE_System_CMD(tmpCmd);
        
        fp = fopen("/system/etc/first_set_wifi", "w");
        if (fp != NULL)
        {
            fprintf(fp, "first_set_wifi");
            fclose(fp);
        }
        wifiFlag = LUX_TRUE;
    }
    sleep(2);
    return LUX_OK;
}

//-------------------------------------------------------------------
int lux_wf_get_mac_ip_mask(char *macStr, char *ipStr, char *maskStr)
{
    FILE* pp = popen("ifconfig "WLAN_DEV, "r");
    if (pp == NULL)
    {
        printf("popen fails\r\n");
        return -1;
    }

    char tmp[256] = { 0 };
    memset(tmp, 0, sizeof(tmp));

    while (fgets(tmp, sizeof(tmp), pp) != NULL)
    {      
        {
            /* find the mac  */
            char* pMacStart = strstr(tmp, "HWaddr ");
            if (pMacStart != NULL)
            {
                if (macStr)
                {
                    sscanf(pMacStart + strlen("HWaddr "), "%s", macStr);
                }
                goto ReadNext;
            }
        }

        {
            /* find the ip and mask TODO: find the broad */
            char* pIpStart = strstr(tmp, "inet addr:");
            if (pIpStart != NULL)
            {
                if (ipStr)
                {
                    sscanf(pIpStart + strlen("inet addr:"), "%s", ipStr);
                }

                char* pMaskStart = strstr(tmp, "Mask:");
                if (maskStr)
                {
                    sscanf(pMaskStart + strlen("Mask:"), "%s", maskStr);
                }
                goto ReadNext;
            }
        }
    ReadNext:
        memset(tmp, 0, sizeof(tmp));
    }

    pclose(pp);
    MYTRACEING("success: mac=%s, ip=%s, mask=%s \n", macStr, ipStr, maskStr);
    return LUX_OK;
}