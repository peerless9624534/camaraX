/*
 * @Author: mc 2249913917@qq.com
 * @Date: 2022-10-25 11:00:54
 * @LastEditors: mc 2249913917@qq.com
 * @LastEditTime: 2022-10-27 10:59:24
 * @FilePath: /camera_cdb/modules/media/include/lux_ipc_qrcode.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _LUX_QRCODE_H
#define _LUX_QRCODE_H
#ifdef __cplusplus
extern "C" {
#endif

    // /*宏定义*/
#define QR_CODE_CHN_WIDTH   640
#define QR_CODE_CHN_HEIGHT  360
#define QR_CODE_THRESH      128
#define QR_PARSE_STRLEN        512
#define IDIP_TOKEN_LEN  16
#define QR_PARSE_FILE  "/system/jio/qr_file"

/*函数声明*/
    int lux_parse_json_str(char* toParseStr, char* token, char* ssid, char* passwd);

    void QR_Thread_Exit();

    void* thread_qrcode(void* arg);

    void lux_qrcode_parse();

#ifdef __cplusplus
}
#endif

#endif  /*_LUX_QRCODE_H*/
