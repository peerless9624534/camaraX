/*************************************************************************
	> File Name: lux_tamper_det.h
	> Description:
	> Author: cxj
	> Mail: Xiangjie.Chen@luxshare-ict.com
	> Created Time: Tue 04 Apr 2023 05:50:53 AM UTC
 ************************************************************************/

#ifndef __LUX_TAMPER_DET_HPP__
#define __LUX_TAMPER_DET_HPP__

#include "lux_T40_TamperingDet.hpp"
#include <thread>

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
#include "lux_isp.h"

	#define TAMPER_DET_FLITER_NUMS	3
	#define TAMPERDET_DELAY_TIME (SLEEP_TIME / 5)

	typedef struct
	{
		bool bInit;
		bool IsRunning;
		UINT_X alarmTime;
		UINT_X first_cover_time;
		LUX_IVS_TIMEINTERVAL_EN timeInterval;
		tamper_param_input_t in_param;
		tamper_param_output_t tamperResult;
		OSI_MUTEX mux;
		OSI_MUTEX paramMux;/*设置属性锁*/
	}LUX_TAMPER_DET_ATTR_ST;

	extern LUX_CONFIG_DATA_ST* g_configData;

	extern  LUX_FSRC_ATTR_ST* g_framSrcAttr;

	VOID_X LUX_ALG_TamperDet_Start();

	VOID_X LUX_ALG_TamperDet_Stop();

	bool LUX_TamperEvent_IsAlarmByInterval(UINT_X lastTime);

	VOID_X LUX_ALG_TamperDet_SetTimeInterval(LUX_IVS_TIMEINTERVAL_EN ts);

	INT_X LUX_ALG_TamperDet_GetParam(tamper_param_input_t* pInParam);

	INT_X LUX_ALG_TamperDet_SetParam(tamper_param_input_t* pInParam);

	INT_X LUX_ALG_TamperDet_SetSense(LUX_IVS_SENSITIVE_EN sns);

	INT_X LUX_ALG_TamperDet_SetConf();

	INT_X LUX_ALG_TamperDet_Alarm();

	INT_X LUX_ALG_TamperDet_GetResult(tamper_param_input_t *p_in, tamper_param_output_t *p_out);

	VOID_X LUX_ALG_TamperDet_Process();

	VOID_X tamper_det_process_thread();

	INT_X LUX_ALG_TamperDet_Init();

	INT_X LUX_ALG_TamperDet_Exit();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*__cplusplus*/
#endif /*__LUX_TAMPER_DET_HPP__*/
