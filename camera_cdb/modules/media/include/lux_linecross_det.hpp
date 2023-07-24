/*************************************************************************
	> File Name: lux_linecross_det.hpp
	> Description:
	> Author: cxj
	> Mail: Xiangjie.Chen@luxshare-ict.com
	> Created Time: Mon 10 Apr 2023 11:34:12 AM UTC
 ************************************************************************/

#ifndef __LUX_LINECROSS_DET_HPP__
#define __LUX_LINECROSS_DET_HPP__

#include "lux_T40_LineCrossDet.hpp"
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
#include "lux_osd.h"
#include "lux_video.h"
#include "cJSON.h"
#include "lux_config.h"
#include "lux_ivs.h"

	#define LINE_DET_FLITER_NUMS	3
	#define LINEDET_DELAY_TIME (SLEEP_TIME / 5)

	typedef struct 
	{
		LUX_M_SLASH_ST lineInfo;				//报警线信息
		vector<pair<int, spot>> lastTrace;	//保存上一次识别帧的信息
	}linedet_param_input_t;

	typedef vector<int> linedet_param_output_t;//第1个点为北 第2个点为南，算法识别结果方向 0--左西 1--右东

	typedef struct
	{
		bool bInit;
		bool IsRunning;
		UINT_X alarmTime;
		LUX_IVS_TIMEINTERVAL_EN timeInterval;
		linedet_param_input_t in_param;
		linedet_param_output_t objResult;
		OSI_MUTEX mux;
		OSI_MUTEX paramMux;/*设置属性锁*/
	}LUX_ALG_LINEDET_ATTR_ST;

	extern LUX_CONFIG_DATA_ST* g_configData;
	extern  LUX_FSRC_ATTR_ST* g_framSrcAttr;

	VOID_X LUX_ALG_LineCrossDet_Start();

	VOID_X LUX_ALG_LineCrossDet_Stop();

	bool LUX_LineEvent_IsAlarmByInterval(UINT_X lastTime);

	VOID_X LUX_ALG_LineCrossDet_SetTimeInterval(LUX_IVS_TIMEINTERVAL_EN ts);

	VOID_X LUX_ALG_LineCrossDet_SetAttr(linedet_param_input_t* pInParam);

	INT_X LUX_ALG_LineCrossDet_Alarm();

	INT_X LUX_ALG_LineCross_cJsonParse(CHAR_X* pStrAlarmLineInfo, P_LUX_M_SLASH_ST pAlarmLine);

	INT_X LUX_ALG_LineCross_CoorConvertSet(P_LUX_M_SLASH_ST pAlarmLine);

	INT_X LUX_ALG_LineCrossDet_SetConf();

	INT_X LUX_ALG_LineCrossDet_GetResult(linedet_param_input_t *p_in, linedet_param_output_t *p_out);

	VOID_X LUX_ALG_LineCrossDet_Process();

	VOID_X linecross_det_process_thread();

	INT_X LUX_ALG_LineCrossDet_Init();

	INT_X LUX_ALG_LineCrossDet_Exit();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*__cplusplus*/

#endif /*__LUX_LINECROSS_DET_HPP__*/
