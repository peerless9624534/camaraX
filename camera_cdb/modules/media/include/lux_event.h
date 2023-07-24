/*
 * lux_event.h
 *
 * 事件处理中心，报警，事件下发、上传
 */

#ifndef __LUX_EVENT_H__
#define __LUX_EVENT_H__

#include "comm_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */


/**
 * 系统时间结构体.
 */
typedef struct
{
    int sec;  /**< 秒数，范围：0~59 */
    int min;  /**< 分钟数，范围：0~59 */
    int hour; /**< 小时数，范围：0~23 */
    int mday; /**< 一个月中的第几天，范围：1~31 */
    int mon;  /**< 月份，范围：1~12 */
    int year; /**< 年份，范围：>1900 */
} LUX_EVENT_SYS_TIME;

typedef enum{
    LUX_EVENT_NOTIFI_CONTENT_MP4 = 0,   /* mp4 for video, NOT supported now */
    LUX_EVENT_NOTIFI_CONTENT_JPEG,      /* jpeg for snapshot */
    LUX_EVENT_NOTIFI_CONTENT_PNG,       /* png for snapshot */
    LUX_EVENT_NOTIFI_CONTENT_MAX,
} LUX_EVENT_NOTIFI_CONTENT_EN;

/*录像事件状态*/
typedef enum
{
    LUX_RECOR_STOP,
    LUX_RECOR_START,
    //below are internal status, should NOT be used for developer
    LUX_RECOR_ONGOING,
    LUX_RECOR_READY_TO_STOP,
    LUX_RECOR_RESTART,
    LUX_RECOR_UNWRITEABLE,
    LUX_RECOR_INVALID
}LUX_RECOR_STATUS_E;


/* 云存储录像状态 */
typedef enum
{
    LUX_EVENT_NONE,
    LUX_EVENT_ONGOING,
    LUX_EVENT_READY,
    LUX_EVENT_INVALID
} LUX_EVENT_STATUS_E;

typedef enum
{
    LUX_NOTIFICATION_NAME_MOTION = 0,      /* 移动侦测 */
    LUX_NOTIFICATION_NAME_DOORBELL = 1,    /* 门铃按下 */
    LUX_NOTIFICATION_NAME_DEV_LINK = 2,        /* IOT设备联动触发 */
    LUX_NOTIFICATION_NAME_PASSBY = 3,          /* 正常经过 */
    LUX_NOTIFICATION_NAME_LINGER = 4,          /* 异常逗留 */
    LUX_NOTIFICATION_NAME_MESSAGE = 5,         /* 留言信息 */
    LUX_NOTIFICATION_NAME_CALL_ACCEPT = 6,     /* 门铃接听 */
    LUX_NOTIFICATION_NAME_CALL_NOT_ACCEPT = 7, /* 门铃未接听 */
    LUX_NOTIFICATION_NAME_CALL_REFUSE = 8,     /* 门铃拒绝接听 */
    LUX_NOTIFICATION_NAME_HUMAN = 9,           /* 人形检测 */
    LUX_NOTIFICATION_NAME_PCD = 10,            /* 宠物检测 */
    LUX_NOTIFICATION_NAME_CAR = 11,            /* 车辆检测 */
    LUX_NOTIFICATION_NAME_BABY_CRY = 12,       /* 婴儿哭声 */
    LUX_NOTIFICATION_NAME_ABNORMAL_SOUND = 13, /* 声音异响 */
    LUX_NOTIFICATION_NAME_FACE = 14,           /* 人脸检测 */
    LUX_NOTIFICATION_NAME_ANTIBREAK = 15,      /* 强拆告警 */
    LUX_NOTIFICATION_NAME_RECORD_ONLY = 16,    /* 占位，无意义 */
    LUX_NOTIFICATION_NAME_IO_ALARM = 17,       /* 本地IO设备触发 */
    LUX_NOTIFICATION_NAME_LOW_BATTERY = 18,    /* 低电量*/
    LUX_NOTIFICATION_NAME_NOUSE = 19,          /* 忽略*/
    LUX_NOTIFICATION_NAME_COVER = 20,          /* 封面 */
    LUX_NOTIFICATION_NAME_FULL_BATTERY = 21,   /* 电量满 */
    LUX_NOTIFICATION_NAME_USER_IO = 22,        /* 本地IO设备触发-2 */
    LUX_NOTIFY_TYPE_MAX
} LUX_NOTIFICATION_NAME_E;

typedef enum
{
    LUX_EVENT_TYPE_MOTION_DETECT = 0,   /* ipc_event_motion,"ipc_motion"*/
    LUX_EVENT_TYPE_DOOR_BELL = 1,       /* ipc_event_doorbell,"ipc_doorbell"*/
    LUX_EVENT_TYPE_DEV_LINK = 2,        /* event triggerred by other tuya cloud device*/
    LUX_EVENT_TYPE_PASSBY = 3,          /* for doorbell, detect someone passby */
    LUX_EVENT_TYPE_LINGER = 4,          /* for doorbell, detect someone linger */
    LUX_EVENT_TYPE_LEAVE_MESSAGE = 5,   /* for doorbell, a video message leaved */
    LUX_EVENT_TYPE_CALL_ACCEPT = 6,     /* for doorbell, call is accepted */
    LUX_EVENT_TYPE_CALL_NOT_ACCEPT = 7, /* for doorbell, call is not accepted */
    LUX_EVENT_TYPE_CALL_REFUSE = 8,     /* ipc_event_call_refuse*/
    LUX_EVENT_TYPE_HUMAN_DETECT = 9,    /* ipc_event_human,"ipc_human"*/
    LUX_EVENT_TYPE_CAT_DETECT = 10,     /* ipc_event_cat,"ipc_cat"*/
    LUX_EVENT_TYPE_CAR_DETECT = 11,     /* ipc_event_car,"ipc_car"*/
    LUX_EVENT_TYPE_BABY_CRY = 12,       /* ipc_event_baby_cry,"ipc_baby_cry"*/
    LUX_EVENT_TYPE_BANG = 13,           /* ipc_event_bang,"ipc_bang"*/
    LUX_EVENT_TYPE_FACE = 14,           /* ipc_event_face,"ipc_face"*/
    LUX_EVENT_TYPE_ANTIBREAK = 15,      /* ipc_event_antibreak,"ipc_antibreak"*/
    LUX_EVENT_TYPE_RECORD_ONLY = 16,    /* only to start event record, no event reported or will be reported later */
    LUX_EVENT_TYPE_IO_ALARM = 17,       /* event triggerred by local IO devices */
    LUX_EVENT_TYPE_LOW_BATTERY = 18,    /*low_battery*/
    LUX_EVENT_TYPE_NOUSE = 19,          /*ignore it.*/
    LUX_EVENT_TYPE_COVER = 20,          /* ipc cover pic*/
    LUX_EVENT_TYPE_FULL_BATTERY = 21,   /* full battery*/
    LUX_EVENT_TYPE_USER_IO = 22,        /* io alarm 2*/
    LUX_EVENT_TYPE_INVALID
} LUX_ClOUD_STORAGE_EVENT_TYPE_E;

typedef void *(pAlarmEventReportCB)(CHAR_X *snap_buffer, UINT_X snap_size, LUX_EVENT_NOTIFI_CONTENT_EN type);
typedef void *(pStartAlarmEventRecordCB)(void);
typedef void *(pStopAlarmEventRecordCB)(void);
/* 录像事件状态 */
typedef UINT_X (*pGetStatusAlarmEventRecordCB)(void);
typedef UINT_X (*pAddCloudStorageEventCB)(CHAR_X *snap_buffer, UINT_X snap_size, LUX_ClOUD_STORAGE_EVENT_TYPE_E type, UINT_X durathon);
typedef UINT_X (*pDeletCloudStorageEventCB)(UINT_X cloudId);
typedef UINT_X (*pGetCloudStorageStatusCB)(UINT_X cloudId);

typedef struct
{
    pAlarmEventReportCB   *pEvntReportFuncCB;     /*事件上报回调函数*/
    pStartAlarmEventRecordCB   *pStartEventRecordFuncCB;  /*开始事件录像回调函数*/
    pStopAlarmEventRecordCB    *pStopEventRecordFuncCB;  /*停止事件录像回调函数*/
    /* 录像事件状态 */
    pGetStatusAlarmEventRecordCB        pGetRecordstatus;
    /* 云存储 */
    UINT_X  cloudId;
    pAddCloudStorageEventCB   pAddCloudEvent;
    pDeletCloudStorageEventCB pDeletCloudEvent;
    pGetCloudStorageStatusCB  pGetCloudEventStatus;
}LUX_EVENT_ATTR_ST;

int LUX_Event_GetAlarmCbk(LUX_EVENT_ATTR_ST *pEventCbk);

 /**
 * @description: 重启设备
 * @return 成功：0，失败：-1；
 */
INT_X LUX_Event_Reboot();
 /**
 * @description: 获取板子系统时间
 * @param: [out] pSysTime:系统时间结构
 * @return 成功：0，失败：-1；
 */
INT_X LUX_Event_GetSysTime(LUX_EVENT_SYS_TIME *pSysTime);

/**
 * @description: 设置系统时间.
 * @param: [in] pSysTime:系统时间结构
 * @return 成功：0，失败：-1；
 */
INT_X LUX_Event_SetSysTime(LUX_EVENT_SYS_TIME *pSysTime);

/**
 * @description: IVS智能模块抓图初始化
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_EVENT_IVSProcess_Init();

/**
 * @description: 移动侦测事件处理函数
 * @param [in] pFuncCBAttr 事件上报回调函数
 */
INT_X LUX_EVENT_IvsRegistCB(LUX_EVENT_ATTR_ST *pFuncCBAttr);

/* 根据报警间隔判断是否上报 */
BOOL_X LUX_Event_IsAlarmByInterval(UINT_X lastTime);

/**
 * @description: 侦测事件录像
 * @param [in] ts 录像时长,单位秒
 * @return 0 成功，非零失败
 */
INT_X LUX_Event_Record(UINT_X ts);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __LUX_EVENT_H__ */
