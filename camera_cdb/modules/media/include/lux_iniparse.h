/*
 * lux_iniparse.h
 *
 * ini文件解析
 */

#ifndef __LUX_INIPARSE_H__
#define __LUX_INIPARSE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#include <comm_error_code.h>
#include <comm_def.h>
#include <lux_base.h>
#include <iniparser.h>

#define     INI_NAME_LEN    64
typedef struct 
{
    BOOL_X  bUsed;
    CHAR_X  name[INI_NAME_LEN];
    dictionary* ini;

}LUX_INIPARSE_ATTR_ST;
/**
 * @description: 加载解析ini文件
 * @param [in]	chnID	通道的编号
 * @return 0 成功， 非零失败，返回错误码
 */
INT_X LUX_INIParse_jio_Load(const CHAR_X *pFileName);
/**
 * @description: 加载解析ini文件
 * @param [in]	chnID	通道的编号
 * @return 0 成功， 非零失败，返回错误码
 */
INT_X LUX_INIParse_Load(const CHAR_X *pFileName);
/**
 * @description: 获取键的布尔值
 * @param [in]	pFileName	文件名称
 * @param [in]	pTitle   	标题
 * @param [in]	pKey	    键
 * @param [out] value       获取到的值
 * @return 成功返回0， 错误返回错误码
 */
INT_X LUX_INIParse_GetBool(const PCHAR_X pFilename, const PCHAR_X pTitle, const PCHAR_X pKey, BOOL_X *value);

/**
 * @description: 获取键的字符类型值
 * @param [in]	pFileName	文件名称
 * @param [in]	pTitle   	标题
 * @param [in]	pKey	    键
 * @param [out] pOutStr     获取到字符串
 * @return 成功返回0，错误返回错误码
 */
INT_X LUX_INIParse_GetString(const PCHAR_X pFilename, const PCHAR_X pTitle, const PCHAR_X pKey, PCHAR_X pOutStr);

/**
 * @description: 获取键的整形值
 * @param [in]	pFileName	文件名称
 * @param [in]	pTitle   	标题
 * @param [in]	pKey	    键
 * @param [out] value       获取到的值
 * @return 成功返回0， 错误返回错误码
 */
INT_X LUX_INIParse_GetInt(const PCHAR_X pFilename, const PCHAR_X pTitle, const PCHAR_X pKey, PINT_X value);

/**
 * @description: 获取键的浮点值
 * @param [in]	pFileName	文件名称
 * @param [in]	pTitle   	标题
 * @param [in]	pKey	    键
 * @param [out] value       获取到的值
 * @return 成功返回0， 错误返回错误码
 */
INT_X LUX_INIParse_GetDouble(const PCHAR_X pFilename, const PCHAR_X pTitle, const PCHAR_X pKey, PDOUBLE_X value);

/**
 * @description: 设置键的值
 * @param [in]	 pFileName	文件名称
 * @param [in]	 pTitle   	标题
 * @param [in]	 pKey	    键
 * @param [in]  pinStr      需要设置的值
 * @return 成功返回0，错误返回错误码
 */ 


 INT_X LUX_INIParse_SetKey(const PCHAR_X pFilename, const PCHAR_X pTitle, const PCHAR_X pKey, PCHAR_X pValue);

 LUX_INIPARSE_ATTR_ST* LUX_INIParse_SetKey_begin(const PCHAR_X pFilename);

 INT_X LUX_INIParse_SetKey_Input(LUX_INIPARSE_ATTR_ST* pAttr,const PCHAR_X pTitle, const PCHAR_X pKey, PCHAR_X pValue);

INT_X  LUX_INIParse_SetKey_End(const PCHAR_X pFilename,LUX_INIPARSE_ATTR_ST* pAttr);
/**
 * @description: 释放dictionary申请的资源
 * @param [in]	 pFileName	文件名称
 * @return 成功返回0，错误返回错误码
 */
INT_X LUX_INIParse_FreeDict(const PCHAR_X pFilename);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __LUX_INIPARSE_H__ */
