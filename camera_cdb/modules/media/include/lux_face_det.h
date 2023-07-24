/*************************************************************************
 > File Name: lux_face_det.h
 > Description:
 > Author: cxj
 > Mail: Xiangjie.Chen@luxshare-ict.com
 > Created Time: Wed 26 Apr 2023 02:59:02 AM UTC
 ************************************************************************/

#ifndef __LUX_FACE_DET_H__
#define __LUX_FACE_DET_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /*__cplusplus*/
#include "imp_log.h"
#include "lux_base.h"
#include "lux_iniparse.h"
#include "lux_config.h"
#include "lux_ivs.h"
#include "lux_osd.h"

/*人脸侦测打印过滤值*/
#define FACEDET_FILTER_NUM          32
/*未识别人形的次数超过该次数则框消失*/
#define FACEDET_MISS_NUM  MISS_NUM
/*人脸侦测置信度*/
#define  FACE_DET_CONFIDENCE        0.36
/*人脸侦测算法延时时间*/
#define FACEDET_DELAY_TIME (SLEEP_TIME / 5)

    /*IVS人脸侦测模块的绑定信息*/
    typedef LUX_IVS_BIND_ATTR_ST LUX_IVS_FD_BIND_ATTR_ST;

    /*人脸侦测的属性信息*/
    typedef struct
    {
        BOOL_X                      IsRunning;              /*是否正在执行*/
        BOOL_X                      bInit;                  /*算法初始化*/
        BOOL_X                      bStart;
        UINT_X                      alarmTime;
        OSI_MUTEX                   mux;                    /*通道锁*/
        OSI_MUTEX                   paramMux;               /*设置属性锁*/
        LUX_IVS_TIMEINTERVAL_EN     timeInterval;           /*事件报警时间间隔*/
        LUX_IVS_FD_BIND_ATTR_ST     bindAttr;               /*绑定信息*/
        facedet_param_input_t       in_param;               /*人脸侦测的算法参数设置*/
        facedet_param_output_t*     ivsFaceDetResult;       /*人脸侦测的计算结果*/
        IMPIVSInterface*            interface;              /*算法句柄*/
    }LUX_IVS_FACEDET_ATTR_ST;

    extern LUX_IVS_FACEDET_ATTR_ST g_facedet_attr;

    /**
     * @description: 人脸侦测报警区域开关
     * @param [in] bOpen IVS处理结果
     * @return 0 成功，非零失败
     */
    INT_X LUX_IVS_FD_AlarmZone_Open(BOOL_X bOpen);

    /**
     * @description: 人脸侦测获取报警区域开关
     * @return 0 成功，非零失败
     */
    BOOL_X LUX_IVS_FD_GetAlarmZone_OnOff();

    /**
     * @description: 设置人脸侦测算法灵敏度
     * @param [in] sns   灵敏度
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_FD_SetSense(LUX_IVS_SENSITIVE_EN sns);

    /**
     * @description: 开启人脸侦测算法通道
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_FD_StartChn();

    /**
     * @description: 关闭人脸侦测算法通道
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_FD_StopChn();

    /**
     * @description: 开启IVS人脸侦测
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_FD_StartAlgo();

    /**
     * @description: 关闭IVS人脸侦测
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_FD_StopAlgo();

    /**
     * @description: 人脸侦测事件报警间隔
     * @param  [in] ts 时间间隔,单位s
     * @return 0 成功，非零失败，返回错误码
     */
    VOID_X LUX_IVS_FD_SetTimeInterval(LUX_IVS_TIMEINTERVAL_EN ts);

    /**
     * @description: 设置人脸侦测报警区域
     * @param  [in] zoneInfo  坐标信息
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_FD_SetRegion(LUX_IVS_M_ZONE_BASE_ST* zoneInfo);

    /**
     * @description: 人脸侦测启动设置配置
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_FD_SetConf();

    /**
     * @description: 人脸侦测算法获取结果
     * @param  [in] p_out 人脸侦测算法识别结果
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_FD_GetResult(facedet_param_output_t* p_out);


    /**
     * @description: 根据人脸侦测报警时间判断是否上报
     * @param  [in] lastTime 人脸侦测的报警时间
     * @return LUX_FALSE不报警，LUX_TRUE报警
     */
    BOOL_X LUX_FaceEvent_IsAlarmByInterval(UINT_X lastTime);

    /**
     * @description: 人脸侦测报警事件函数
     * @return 0 成功，非零失败
     */
    INT_X LUX_IVS_FD_Alarm();

    /**
     * @description: 人脸侦测算法处理
     */
    VOID_X LUX_IVS_FD_Process();

    /**
     * @description: 人脸侦测算法线程函数
     */
    PVOID_X person_det_process_thread();

    /**
     * @description: 人脸侦测算法的初始化函数
     * @return 0成功,非0返回失败返回错误码
     */
    INT_X LUX_IVS_FD_AlgoInit();

    /**
     * @description: 算法的去初始化函数
     * @return 0成功,非0返回失败返回错误码
     */
    INT_X LUX_IVS_FD_AlgoExit();

    /**
     * @description: 人脸侦测算法初始化
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_FD_Init();

    /**
     * @description: 人脸侦测算法去初始化
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_FD_Exit();
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*__cplusplus*/

#endif /*__LUX_FACE_DET_H__*/
