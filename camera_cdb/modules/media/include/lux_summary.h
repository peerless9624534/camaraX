/*
 * lux_sleepDet.h
 *
 * 事件记录总结
 * 
 */

#ifndef LUX_EVENT_SUMMARY_H
#define LUX_EVENT_SUMMARY_H

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#include "lux_base.h"


typedef enum
{
    LUX_SUM_EVENT_NONE,
    LUX_SUM_EVENT_BABY_CRY,
    LUX_SUM_EVENT_COVER_FACE,
    LUX_SUM_EVENT_MOVE_DET,
    LUX_SUM_EVENT_PERSON_DET,
    LUX_SUM_EVENT_SMILE_DET,
    LUX_SUM_EVENT_SLEEP_START,
    LUX_SUM_EVENT_SLEEP_STOP,
    LUX_SUM_EVENT_SLEEP_KEEP,
} LUX_SUM_EVENT;

/* 记录事件时间*/
INT_X LUX_SUM_Update_Event(LUX_SUM_EVENT TYPE,UINT_X timeStamp,char *strInfo);
INT_X LUX_SUM_GetInfoStringA(CHAR_X *cmd ,CHAR_X *retStr);
INT_X LUX_SUM_DP_CMD(CHAR_X *cmd,CHAR_X *tuyaStr);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif/* LUX_EVENT_SUMMARY_H */