/*
 * lux_sys.c
 *
 * 基于Ingenic平台system模块封装的功能函数，包括系统绑定，系统时间等
 * 
 */

#include <string.h>
#include <comm_def.h>
#include <comm_error_code.h>
#include <lux_sys.h>
#include "imp_log.h"

#define TAG "lux_sys"

/**
 * @description: IMP系统初始化.
 * @return 0 成功. 非0 失败.
 * @remarks 此API调用后会初始化基础的数据结构，但不会初始化硬件单元.
 * @attention 在IMP的任何操作之前必须先调用此接口进行初始化.
 */
INT_X LUX_SYS_Init(void)
{
    return IMP_System_Init();
}

/**
 * @description: IMP系统去初始化.
 * @return 0 成功. 非0 失败.
 * @remarks 此函数调用后会释放IMP所有的内存以及句柄，并关闭硬件单元.
 * @attention 在在调用此API后，若要再次使用IMP则需重新进行IMP系统初始化.
 */
INT_X LUX_SYS_Exit(void)
{
    return IMP_System_Exit();
}

/**
 * @description: 获得IMP系统的时间戳，单位为微秒。
 * @return 时间(usec)
 * @remarks 系统初始化后时间戳自动被初始化。系统去初始化后时间戳失效.
 */
DLONG_X LUX_SYS_GetTimeStamp(void)
{
    return IMP_System_GetTimeStamp();
}

/**
 * @description: 设置IMP系统的时间戳，单位为微秒。
 * @param[in] basets 基础时间。
 * @return 0 成功. 非0 失败.
 */
INT_X LUX_SYS_RebaseTimeStamp(DLONG_X basets)
{
    return IMP_System_RebaseTimeStamp(basets);
}

/**
 * @description: 读取32位寄存器的值。
 * @param[in] regAddr 寄存器的物理地址。
 * @return 寄存器的值（32位）
 */
UINT_X LUX_SYS_ReadReg32(UINT_X regAddr)
{
    return IMP_System_ReadReg32(regAddr);
}

/**
 * @description: 向32位寄存器中写值。
 * @param[in] regAddr 寄存器的物理地址。
 * @param[in] value 要写入的值。
 * @attention 在不明确寄存器的含义之前请谨慎调用此API，否则可能会导致系统错误。
 */
void LUX_SYS_WriteReg32(UINT_X regAddr, UINT_X value)
{
    IMP_System_WriteReg32(regAddr, value);
}

/**
 * @description: 获取IMP系统版本号
 * @param[out] pstVersion IMP系统版本号结构体指针.
 * @return 0 成功. 非0 失败.
 */
INT_X LUX_SYS_GetVersion(LUX_SYS_IMPVersion *pstVersion)
{
    return IMP_System_GetVersion((IMPVersion *)pstVersion);
}

/**
 * @description: 获取CPU型号信息
 * @return CPU型号字符串
 * @remarks 返回值是CPU型号类型的字符串,例如对于T10来说,有"T10"及"T10-Lite".
 */
const PCHAR_X LUX_SYS_GetCPUInfo()
{
    return (const PCHAR_X)IMP_System_GetCPUInfo();
}

/**
 * @description: 绑定源模块和目的模块.
 * @param[in] pModules 绑定模块信息.
 * @return 0 成功,非0 失败.
 * @remarks 根据Device、Group和Output的概念，每个Device可能有多个Group，每个Group可能有多个Output，
 * Group作为Device的输入接口，而Output作为Device的输出接口.因此绑定实际上是将输出Device的某
 * 个Output连接到输入Device的某个Group上.
 * @remarks 绑定关系成功后，源Cell(Output)产生的数据会自动传送到目的Cell(Group).
 */
INT_X LUX_SYS_Bind(LUX_MODULES_BIND_ST *pModules)
{
    if (NULL == pModules)
    {
        IMP_LOG_ERR(TAG, "LUX_SYS_Bind invalid param!\n");
        return LUX_SYS_NULL_PTR;
    }
    IMPCell srcDev;
    IMPCell dstDev;
    INT_X ret = LUX_OK;

    if (!pModules->bUsed)
    {
        IMP_LOG_ERR(TAG, "LUX_SYS_Bind module not used!\n");
        return LUX_SYS_NULL_PTR;
    }

    /* 绑定源模块结构体转换 */
    memset(&srcDev, 0, sizeof(srcDev));
    switch (pModules->srcModule)
    {
        case LUX_MODULS_FSRC:
            srcDev.deviceID = DEV_ID_FS;
            srcDev.groupID = pModules->srcChan;
            break;
        case LUX_MODULS_IVS:
            srcDev.deviceID = DEV_ID_IVS;
            srcDev.groupID = pModules->srcChan;
            break;
        case LUX_MODULS_OSD:
            srcDev.deviceID = DEV_ID_OSD;
            srcDev.groupID = pModules->srcChan;
            break;
        default:
            ret = LUX_SYS_BIND_ERROR;
            IMP_LOG_ERR(TAG, "Bind module:%d channel%d error!\n", pModules->srcModule, pModules->srcChan);
            break;
    }

    /* 绑定目标模块结构体转换 */
    memset(&dstDev, 0, sizeof(dstDev));
    switch (pModules->dstModule)
    {
        case LUX_MODULS_IVS:
            dstDev.deviceID = DEV_ID_IVS;
            dstDev.groupID = pModules->dstChan;
            break;
        case LUX_MODULS_OSD:
            dstDev.deviceID = DEV_ID_OSD;
            dstDev.groupID = pModules->dstChan;
            break;
        case LUX_MODULS_VENC:
            dstDev.deviceID = DEV_ID_ENC;
            dstDev.groupID = pModules->dstChan;
            break;
        default:
            ret = LUX_SYS_BIND_ERROR;
            IMP_LOG_ERR(TAG, "Bind module:%d channel%d error!\n", pModules->dstModule, pModules->dstChan);
            break;
    }

    /* 源和目标模块都正确，调用IMP绑定 */
    if (LUX_OK == ret)
    {
        ret = IMP_System_Bind(&srcDev, &dstDev);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "Bind srcChn[%d] and dstChn[%d] failed %#x\n", ret, srcDev.groupID, dstDev.groupID);
        }
    }

    return ret;
}

/**
 * @description: 解除源模块和目的模块的绑定.
 * @param[in] pModules 解绑模块信息.
 * @return 0 成功,非0 失败.
 */
INT_X LUX_SYS_UnBind(LUX_MODULES_BIND_ST *pModules)
{
    if (NULL == pModules)
    {
        IMP_LOG_ERR(TAG, "LUX_SYS_UnBind invalid param!\n");
        return LUX_SYS_NULL_PTR;
    }
    IMPCell srcDev;
    IMPCell dstDev;
    INT_X ret = LUX_OK;

    /* 绑定源模块结构体转换 */
    memset(&srcDev, 0, sizeof(srcDev));
    switch (pModules->srcModule)
    {
        case LUX_MODULS_FSRC:
            srcDev.deviceID = DEV_ID_FS;
            srcDev.groupID = pModules->srcChan;
            break;
        case LUX_MODULS_IVS:
            srcDev.deviceID = DEV_ID_IVS;
            srcDev.groupID = pModules->srcChan;
            break;
        case LUX_MODULS_OSD:
            srcDev.deviceID = DEV_ID_OSD;
            srcDev.groupID = pModules->srcChan;
            break;
        default:
            ret = LUX_SYS_UNBIND_ERROR;
            IMP_LOG_ERR(TAG, "Bind module:%d channel%d error!\n", pModules->srcModule, pModules->srcChan);
            break;
    }

    /* 绑定目标模块结构体转换 */
    memset(&dstDev, 0, sizeof(dstDev));
    switch (pModules->dstModule)
    {
        case LUX_MODULS_IVS:
            dstDev.deviceID = DEV_ID_IVS;
            dstDev.groupID = pModules->dstChan;
            break;
        case LUX_MODULS_OSD:
            dstDev.deviceID = DEV_ID_OSD;
            dstDev.groupID = pModules->dstChan;
            break;
        case LUX_MODULS_VENC:
            dstDev.deviceID = DEV_ID_ENC;
            dstDev.groupID = pModules->dstChan;
            break;
        default:
            ret = LUX_SYS_UNBIND_ERROR;
            IMP_LOG_ERR(TAG, "Bind module:%d channel%d error!\n", pModules->dstModule, pModules->dstChan);
            break;
    }

    /* 源和目标模块都正确，调用IMP解绑定 */
    if (LUX_OK == ret)
    {
        ret = IMP_System_UnBind(&srcDev, &dstDev);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "Bind FrameSource channel and Encoder failed %#x\n", ret);
        }
    }

    return ret;
}
#if 0
/**
 * @description: 获取绑定在目的Cell的源Cell信息.
 * @param[in] dstCell 目的Cell指针.
 * @param[out] srcCell 源Cell指针.
 * @return 0 成功,非0 失败.
 */
INT_X LUX_SYS_GetBindbyDest(IMPCell *dstCell, IMPCell *srcCell)
{
    return IMP_System_GetBindbyDest(dstCell, srcCell);
}
#endif