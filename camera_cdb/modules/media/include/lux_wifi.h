/*
 * lux_wifi.h
 *
 * 无线wifi模块功能
 *
 */

#ifndef __LUX_WIFI_H__
#define __LUX_WIFI_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#define WLAN_DEV "eth0"

    // if ethernet is used , replace ethernet name
#define NET_DEV "wlan0"

#define MAX_AP_SEARCH 20

#define WIFI_SSID_LEN 32    // tuya sdk definition WIFI SSID MAX LEN
#define WIFI_PASSWD_LEN 64  // tuya sdk definition WIFI PASSWD MAX LEN
    
    int lux_wf_check_connect();

    // int lux_wf_scan_all(const char* real_ssid, idip_ap_security_type_t* auth_type);

    int lux_wf_connect(const char *ssidStr, const char *passwdStr);

    int lux_wf_get_mac_ip_mask(char *macStr, char *ipStr, char *maskStr);
    int lux_wf_get_ssid(char* ssid);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __LUX_WIFI_H__ */
