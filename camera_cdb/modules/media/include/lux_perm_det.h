/*************************************************************************
 > File Name: lux_perm_det.h
 > Description: 
 > Author: cxj
 > Mail: Xiangjie.Chen@luxshare-ict.com
 > Created Time: Thu 27 Apr 2023 05:28:48 AM UTC
 ************************************************************************/

#ifndef __LUX_PERM_DET_H__
#define __LUX_PERM_DET_H__

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

/*矩形周界默认数量*/
#define PERMDET_RECT_NUM    1
/*周界侦测打印过滤值*/
#define PERMDET_FILTER_NUM            5
/*未识别入侵目标的次数超过该次数则框消失*/
#define PERMDET_MISS_NUM  MISS_NUM
/*周界侦测算法延时时间*/
#define PERMDET_DELAY_TIME (SLEEP_TIME / 5)

    /*周界侦测功能设置*/
    typedef enum
    {
       LUX_PERM_FUN_PERM_NEAR_EN,   /*0--周界检测，周界附近就会报警*/
       LUX_PERM_FUN_HOT_ZONE_EN,    /*1--热区检测，必须越界才报警*/
       LUX_PERM_FUN_MOV_DIRE_EN,    /*2--移动方向检测，热区检测加越界物体历史框，判断移动方向*/
       LUX_PERM_FUN_BUTTON,
    }LUX_PERM_FUN_EN;

    /*IVS周界侦测模块的绑定信息*/
    typedef LUX_IVS_BIND_ATTR_ST LUX_IVS_PermDet_BIND_ATTR_ST;

    /*周界侦测的属性信息*/
    typedef struct
    {
        BOOL_X                          IsRunning;              /*是否正在执行*/
        BOOL_X                          bInit;                  /*算法初始化*/
        BOOL_X                          bStart;
        UINT_X                          alarmTime;
        OSI_MUTEX                       mux;                    /*通道锁*/
        OSI_MUTEX                       paramMux;               /*设置属性锁*/
        LUX_IVS_TIMEINTERVAL_EN         timeInterval;           /*事件报警时间间隔*/
        LUX_IVS_PermDet_BIND_ATTR_ST    bindAttr;               /*绑定信息*/
        perm_param_input_t              in_param;               /*周界侦测的算法参数设置*/
        perm_param_output_t*            ivsPermDetResult;       /*周界侦测的计算结果*/
        IMPIVSInterface*                interface;              /*算法句柄*/
    }LUX_IVS_PERMDET_ATTR_ST;

    extern LUX_IVS_PERMDET_ATTR_ST g_permdet_attr;

    /**
     * @description: 设置周界侦测算法灵敏度
     * @param [in] sns   灵敏度
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_PermDet_SetSense(LUX_IVS_SENSITIVE_EN sns);

    /**
     * @description: 设置周界侦测算法功能
     * @param [in] perm_no  第几个周界框
     * @param [in] fun      功能
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_PermDet_SetFun(INT_X perm_no, LUX_PERM_FUN_EN fun);

    /**
     * @description: 开启周界侦测算法通道
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_PermDet_StartChn();

    /**
     * @description: 关闭周界侦测算法通道
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_PermDet_StopChn();

    /**
     * @description: 开启IVS周界侦测
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_PermDet_StartAlgo();

    /**
     * @description: 关闭IVS周界侦测
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_PermDet_StopAlgo();

    /**
     * @description: 周界侦测事件报警间隔
     * @param  [in] ts 时间间隔,单位s
     * @return 0 成功，非零失败，返回错误码
     */
    VOID_X LUX_IVS_PermDet_SetTimeInterval(LUX_IVS_TIMEINTERVAL_EN ts);

    /**
     * @description: 设置周界侦测报警区域
     * @param  [in] zoneInfo  坐标信息
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_PermDet_SetRegion(LUX_IVS_M_ZONE_BASE_ST* zoneInfo);

    /**
     * @description: 设置周界侦测的属性
     * @param  [in] param 属性
     */
    VOID_X LUX_IVS_PermDet_SetAttr(perm_param_input_t* pParam);

    /**
     * @description: 周界侦测启动设置配置
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_PermDet_SetConf();

    /**
     * @description: 根据周界侦测报警时间判断是否上报
     * @param  [in] lastTime 周界侦测的报警时间
     * @return LUX_FALSE不报警，LUX_TRUE报警
     */
    BOOL_X LUX_permEvent_IsAlarmByInterval(UINT_X lastTime);

    /**
     * @description: 周界侦测报警事件函数
     * @return 0 成功，非零失败
     */
    INT_X LUX_IVS_PermDet_Alarm();

    /**
     * @description: 周界侦测算法处理
     */
    VOID_X LUX_IVS_PermDet_Process();

    /**
     * @description: 周界侦测算法线程函数
     */
    PVOID_X perm_det_process_thread();

    /**
     * @description: 周界侦测算法的初始化函数
     * @return 0成功,非0返回失败返回错误码
     */
    INT_X LUX_IVS_PermDet_AlgoInit();

    /**
     * @description: 算法的去初始化函数
     * @return 0成功,非0返回失败返回错误码
     */
    INT_X LUX_IVS_PermDet_AlgoExit();

    /**
     * @description: 周界侦测算法初始化
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_PermDet_Init();

    /**
     * @description: 周界侦测算法去初始化
     * @return 0 成功，非零失败，返回错误码
     */
    INT_X LUX_IVS_PermDet_Exit();
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*__cplusplus*/

#endif /*__LUX_PERM_DET_H__*/
