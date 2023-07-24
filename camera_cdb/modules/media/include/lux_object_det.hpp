/*************************************************************************
	> File Name: lux_object_det.hpp
	> Description:
	> Author: cxj
	> Mail: Xiangjie.Chen@luxshare-ict.com
	> Created Time: Fri 07 Apr 2023 09:45:23 AM UTC
 ************************************************************************/

#ifndef __LUX_OBJECT_DET_HPP__
#define __LUX_OBJECT_DET_HPP__

#include "object_detect.hpp"
#include <thread>

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /*__cplusplus*/
#include <string.h>
#include "imp_log.h"
#include "lux_base.h"
#include "lux_iniparse.h"
#include "lux_osd.h"
#include "lux_config.h"
#include "lux_ivs.h"
#include "lux_isp.h"

	#define OBJDET_FLITER_NUMS	7
	#define OBJDET_UPTATE_NUMS	3
	#define OBJDET_DELAY_TIME (SLEEP_TIME / 5)
	#define MAX_LEFTDET_AREA_NUMS	LUX_UM_AREA_NUM_MAX
	#define MAX_LOSSDET_OBJ_NUMS	LUX_UM_OBJLOSS_NUM_MAX

	extern LUX_CONFIG_DATA_ST* g_configData;
	extern LUX_FSRC_ATTR_ST* g_framSrcAttr;

	typedef enum
	{
		LUX_OBJ_LEFT,
		LUX_OBJ_LOSS,
	}LUX_OBJ_TYPE_EN;

	typedef struct
	{
		bool bInit;
		bool IsRunning;
		UINT_X alarmTime;
		LUX_IVS_TIMEINTERVAL_EN timeInterval;
		UINT_X leftAreaCnt;
		UINT_X lossObjCnt;
		objdet_param_input_t in_param;
		LUX_IVS_Rect left_area[MAX_LEFTDET_AREA_NUMS];
		LUX_IVS_Rect loss_obj[MAX_LOSSDET_OBJ_NUMS];
		objdet_param_output_t objResult;
		objdet_param_output_t objlossResult;
		objdet_param_output_t objleftResult;
		OSI_MUTEX paramMux;/*设置属性锁*/
		OSI_MUTEX mux;
	}LUX_ALG_OBJDET_ATTR_ST;

	VOID_X LUX_ALG_ObjDet_Start();

	VOID_X LUX_ALG_ObjDet_Stop();

	bool LUX_ObjEvent_IsAlarmByInterval(UINT_X lastTime);

	VOID_X LUX_ALG_ObjDet_SetTimeInterval(LUX_IVS_TIMEINTERVAL_EN ts);

	INT_X LUX_ALG_ObjDet_SetSense(LUX_IVS_SENSITIVE_EN sns);

	INT_X LUX_ALG_ObjDet_SetGrayScale(INT_X scale_value);

	INT_X LUX_ALG_ObjDet_SetMaxArea(INT_X max_area);

	INT_X LUX_ALG_ObjDet_SetMinArea(INT_X min_area);

	INT_X LUX_ALG_ObjDet_GetParam(objdet_param_input_t* pInParam);

	INT_X LUX_ALG_ObjDet_SetParam(objdet_param_input_t* pInParam);

	INT_X LUX_ALG_ObjDet_UpdateInitFrame();

	INT_X LUX_ALG_ObjDet_Alarm();

	VOID_X LUX_ALG_ObjDet_CheckInit();

	INT_X LUX_ALG_ObjDet_GetResult(objdet_param_input_t *p_in, LUX_OBJ_TYPE_EN obj_type, objdet_param_output_t *p_out);

	VOID_X LUX_ALG_ObjDet_Process();

	INT_X LUX_ALG_ObjDet_SetConf();

	VOID_X object_det_process_thread();

	INT_X LUX_ALG_ObjDet_Init();

	INT_X LUX_ALG_ObjDet_Exit();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*__cplusplus*/

#endif /*__LUX_OBJECT_DET_HPP__*/
