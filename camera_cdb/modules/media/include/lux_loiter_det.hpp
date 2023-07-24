/*************************************************************************
	> File Name: lux_loiter_det.hpp
	> Description:
	> Author: cxj
	> Mail: Xiangjie.Chen@luxshare-ict.com
	> Created Time: Wed 12 Apr 2023 09:13:34 AM UTC
 ************************************************************************/

#ifndef __LUX_LOITER_DET_HPP__
#define __LUX_LOITER_DET_HPP__

#include "loiter_detect.hpp"
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
#include "lux_video.h"
#include "lux_ivs.h"
#include "ivs_inf_personDet.h"
#include "lux_config.h"
#include "lux_person_det.h"

#define LOITER_DET_FLITER_NUMS	120
#define LOITER_PERSON_CONFIDENCE	0.6
#define LOITERDET_DELAY_TIME (SLEEP_TIME / 5)

	extern LUX_CONFIG_DATA_ST* g_configData;

	// typedef persondet_param_output_t loiterdet_param_output_t;

	typedef struct
	{
		bool bInit;
		bool IsRunning;
		UINT_X alarmTime;
		LUX_IVS_TIMEINTERVAL_EN timeInterval;
		loiterdet_param_input_t in_param;
		loiterdet_param_output_t loiterResult;
		OSI_MUTEX mux;
		OSI_MUTEX paramMux;/*设置属性锁*/
	}LUX_ALG_LOITERDET_ATTR_ST;

	extern  LUX_FSRC_ATTR_ST* g_framSrcAttr;
	extern LUX_IVS_PERSONDET_ATTR_ST g_persondet_attr;

	extern VOID_X LUX_OSD_LoiterRect_Show(persondet_param_output_t* LioterDetRet);
	extern VOID_X LUX_OSD_LoiterRect_Miss();

	VOID_X LUX_ALG_LoiterDet_Start();

	VOID_X LUX_ALG_LoiterDet_Stop();

	INT_X LUX_ALG_LoiterDet_SetConf();

	bool LUX_LoiterEvent_IsAlarmByInterval(UINT_X lastTime);

	VOID_X LUX_ALG_LoiterDet_SetTimeInterval(LUX_IVS_TIMEINTERVAL_EN ts);

	VOID_X LUX_ALG_LoiterDet_GetParam(loiterdet_param_input_t* pInParam);

	VOID_X LUX_ALG_LoiterDet_SetParam(loiterdet_param_input_t* pInParam);

	INT_X LUX_ALG_LoiterDet_Alarm();

	INT_X LUX_ALG_LoiterDet_GetResult(persondet_param_output_t* p_result, loiterdet_param_input_t* p_in, loiterdet_param_output_t* p_out);

	VOID_X loiter_det_process_thread();

	INT_X LUX_ALG_LoiterDet_Init();

	INT_X LUX_ALG_LoiterDet_Exit();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*__cplusplus*/

#endif /*__LUX_LOITER_DET_HPP__*/