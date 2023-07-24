/*
 * lux_ivs.h
 *
 * 智能模块，包括移动侦测、人脸检测、人形侦测等功能
 *
 */

#ifndef __LUX_IVS_H__
#define __LUX_IVS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#include <comm_def.h>
#include <lux_base.h>
#include <imp_ivs.h>
#include <ivs_common.h>
#include <ivs_interface.h>
#include <ivs_inf_personDet.h>
#include <ivs_inf_faceDet.h>
#include <ivs_inf_perm.h>
#include <sys/prctl.h>

/*周界侦测周界数*/
#define MAX_PERM_DET_PERM_CNT      6

#define MAX_PERM_NUM             (6)//最大周界数
#define MAX_PERM_POINTS_NUM      (16)//最大点数

#define MISS_NUM    2   //未识别的次数超过该次数则框消失
#define SLEEP_TIME (400 * 1000) //us级

/*算法类型*/
typedef enum
{
    LUX_IVS_PERSON_DET_EN,
    LUX_IVS_FACE_DET_EN,
    LUX_IVS_PERM_DET_EN,
    LUX_IVS_ALGO_EN_BUTTON,
}LUX_IVS_ALGO_EN;

/*时间间隔*/
typedef enum
{
    LUX_IVS_TIMEINTERVAL_1MIN,
    LUX_IVS_TIMEINTERVAL_3MIN,
    LUX_IVS_TIMEINTERVAL_5MIN,
    LUX_IVS_TIME_INTERVAL_BUTTON,
}LUX_IVS_TIMEINTERVAL_EN;

/*灵敏度设置*/
typedef enum
{
   LUX_IVS_SENSITIVE_LOW_EN,
   LUX_IVS_SENSITIVE_MID_EN,
   LUX_IVS_SENSITIVE_HIG_EN,
   LUX_IVS_SENSITIVE_BUTTON,
}LUX_IVS_SENSITIVE_EN;

//Detection area structure
typedef struct
{
    INT_X pointX;    //Starting point x  [0-100]
    INT_X pointY;    //Starting point Y  [0-100]
    INT_X width;     //width    [0-100]
    INT_X height;    //height    [0-100]

}LUX_IVS_ZONE_BASE_ST;

//Multiple rect
typedef struct
{
    INT_X regionCnt;//矩形区域数
    LUX_IVS_ZONE_BASE_ST rect[MAX_PERM_NUM];
}LUX_IVS_M_ZONE_BASE_ST;

//Detection polygon area structure
typedef struct
{
    INT_X pointCnt;//多边形区域点数，至少3点且不在一条直线上
    IVSPoint zonePoints[MAX_PERM_POINTS_NUM];
}LUX_IVS_ZONE_POLYGON_ST;

//Multiple polygons
typedef struct
{
    INT_X regionCnt;//多边形区域数
    LUX_IVS_ZONE_POLYGON_ST polygon[MAX_PERM_NUM];
}LUX_IVS_M_POLYGON_ST;

/*移动侦测感兴趣坐标*/
typedef struct
{
    IVSPoint    ul;     /*左上角*/
    IVSPoint    br;     /*右下角*/

}LUX_IVS_ROI_ST;

/*报警感兴趣区域*/
typedef struct
{
    BOOL_X  bOpen;
    LUX_IVS_ROI_ST movDet;      /*移动侦测报警区域信息*/

}LUX_IVS_ALARM_ZONE_ST;

/*事件报警事件间隔*/
typedef struct
{
    INT_X   timeInterval;        /*时间间隔*/
    UINT_X  lastTime;
    UINT_X  curTime;

}LUX_IVS_TIME_INTERVAL_ST;


/*IVS模块的绑定信息*/
typedef struct 
{
    INT_X             grpId;              /* 组ID号 */
    INT_X             chnNum;             /*算法通道*/
    INT_X             bindSrcChan;        /* 编码通道要绑定的源通道 */
    LUX_MODULS_DEV_EN bindSrcModule;      /* 编码通道要绑定的源模块 */
    INT_X             bindDstChan;        /* 编码通道要绑定的目标通道 */
    LUX_MODULS_DEV_EN bindDstModule;      /* 编码通道要绑定的目标模块 */
}LUX_IVS_BIND_ATTR_ST;

typedef struct
{
    BOOL_X      bInit;
    OSI_MUTEX   paramMux;                                 /*设置属性锁*/ 
    LUX_IVS_TIME_INTERVAL_ST    rptEventTI;              /*事件报警时间间隔*/
    LUX_IVS_BIND_ATTR_ST        bindAttr;                /*绑定信息*/
}LUX_IVS_ATTR_ST;


extern OSI_SEM g_ivsAlgSem;
extern LUX_IVS_ATTR_ST g_ivsAttr;

/**
 * @description: 解析矩形报警区域字符串,将其转化为结构体
 * @param [out] pStrAlarmZoneInfo 区域报警参数
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_alarmZone_Parse(CHAR_X *pStrAlarmZoneInfo, LUX_IVS_ZONE_BASE_ST *pAlarmZone);

/**
 * @description: cjson解析多个矩形报警区域字符串,将其转化为结构体
 * @param [out] pStrAlarmZoneInfo 区域报警参数
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_alarmZone_cJsonParse(CHAR_X *pStrAlarmZoneInfo, LUX_IVS_M_ZONE_BASE_ST *pAlarmZone);

/**
 * @description: 解析多边形报警区域字符串,将其转化为结构体
 * @param [out] pStrAlarmZoneInfo 区域报警参数
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_alarmPolygonZone_Parse(CHAR_X *pStrAlarmZoneInfo, LUX_IVS_M_POLYGON_ST *pAlarmZone);

/**
 * @description: json解析多边形报警区域字符串,将其转化为结构体
 * @param [out] pStrAlarmZoneInfo 区域报警参数
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_alarmPolygonZone_cJsonParse(CHAR_X* pStrAlarmZoneInfo, LUX_IVS_M_POLYGON_ST* pAlarmZone);

/**
 * @description: ivs算法认证初始化
 * @return 0成功, 非0错误
 * @attention 需要连接网路后初始化
 */
INT_X LUX_IVS_Iaac_Init();



/**
 * @description: ivs事件报警间隔倒计时
 * @return 倒计时未到返回FALSE, 到了返回 TURE,
 */
BOOL_X LUX_IVS_GetReportEvent_Countdown();

/**
 * @description: 判断移动侦测的结果是否在感兴趣区域内
 * @param [in] IvsResult IVS处理结果
 * @return 在区域内返回ture，否则返回false
 */
BOOL_X LUX_IVS_MovDetRegion_Process(LUX_BASE_IVSDATA_ST *MovResult);

/**
 * @description: 设置报警区域转换
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_SetAlarmRegion_Flip();

/**
 * @description: 设置多边形报警区域转换
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_SetAlarmPolygonRegion_Flip();

/**
 * @description: 获取移动侦测灵敏度
 * @return 返回灵敏度
 */
INT_X LUX_IVS_GetMovSensitive();

/**
 * @description: ivs事件报警间隔
 * @param  [in] ts 时间间隔,单位s
 * @return 0 成功，非零失败，返回错误码
 */
VOID_X LUX_IVS_SetTimeInterval(LUX_IVS_TIMEINTERVAL_EN ts);

/**
 * @description: ivs模块初始化
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_Init();

/**
 * @description: IVS模块去初始化
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_IVS_Exit();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __LUX_IVS_H__ */
