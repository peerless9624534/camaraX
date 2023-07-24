/*
 * lux_sys.h
 *
 * system功能函数，包括系统绑定，系统时间等
 * 
 */

#ifndef __LUX_SYS_H__
#define __LUX_SYS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */


#include <comm_def.h>
#include <comm_func.h>
#include <imp_system.h>
/**
 * IMP系统版本号定义.
 */
typedef struct {
	char aVersion[64];	/**< IMP系统版本号 */
} LUX_SYS_IMPVersion;


/**
 * @description: IMP系统初始化.
 * @return 0 成功. 非0 失败.
 * @remarks 此API调用后会初始化基础的数据结构，但不会初始化硬件单元.
 * @attention 在IMP的任何操作之前必须先调用此接口进行初始化.
 */
INT_X LUX_SYS_Init(void);

/**
 * @description: IMP系统去初始化.
 * @return 0 成功. 非0 失败.
 * @remarks 此函数调用后会释放IMP所有的内存以及句柄，并关闭硬件单元.
 * @attention 在在调用此API后，若要再次使用IMP则需重新进行IMP系统初始化.
 */
INT_X LUX_SYS_Exit(void);

/**
 * @description: 获得IMP系统的时间戳，单位为微秒。
 * @return 时间(usec)
 * @remarks 系统初始化后时间戳自动被初始化。系统去初始化后时间戳失效.
 */
DLONG_X LUX_SYS_GetTimeStamp(void);

/**
 * @description: 设置IMP系统的时间戳，单位为微秒。
 * @param[in] basets 基础时间。
 * @return 0 成功. 非0 失败.
 */
INT_X LUX_SYS_RebaseTimeStamp(DLONG_X basets);

/**
 * @description: 读取32位寄存器的值。
 * @param[in] regAddr 寄存器的物理地址。
 * @return 寄存器的值（32位）
 */
UINT_X LUX_SYS_ReadReg32(UINT_X regAddr);

unsigned long LUX_SYS_GetFileSize(const char *filename);

/**
 * @description: 向32位寄存器中写值。
 * @param[in] regAddr 寄存器的物理地址。
 * @param[in] value 要写入的值。
 * @attention 在不明确寄存器的含义之前请谨慎调用此API，否则可能会导致系统错误。
 */
void LUX_SYS_WriteReg32(UINT_X regAddr, UINT_X value);

/**
 * @description: 获取IMP系统版本号
 * @param[out] pstVersion IMP系统版本号结构体指针.
 * @return 0 成功. 非0 失败.
 */
INT_X LUX_SYS_GetVersion(LUX_SYS_IMPVersion *pstVersion);

/**
 * @description: 获取CPU型号信息
 * @return CPU型号字符串
 * @remarks 返回值是CPU型号类型的字符串,例如对于T10来说,有"T10"及"T10-Lite".
 */
const PCHAR_X LUX_SYS_GetCPUInfo();

/**
 * @description: 绑定源Cell和目的Cell.
 * @param[in] srcCell 源Cell指针.
 * @param[in] dstCell 目的Cell指针.
 * @return 0 成功,非0 失败.
 * @remarks 根据Device、Group和Output的概念，每个Device可能有多个Group，每个Group可能有多个Output，
 * Group作为Device的输入接口，而Output作为Device的输出接口.因此绑定实际上是将输出Device的某
 * 个Output连接到输入Device的某个Group上.
 * @remarks 绑定关系成功后，源Cell(Output)产生的数据会自动传送到目的Cell(Group).
 */
INT_X LUX_SYS_Bind(LUX_MODULES_BIND_ST *pModules);

/**
 * @description: 解除源Cell和目的Cell的绑定.
 * @param[in] srcCell 源Cell指针.
 * @param[in] dstCell 目的Cell指针.
 * @return 0 成功,非0 失败.
 */
INT_X LUX_SYS_UnBind(LUX_MODULES_BIND_ST *pModules);

#if 0
/**
 * @description: 获取绑定在目的Cell的源Cell信息.
 * @param[in] dstCell 目的Cell指针.
 * @param[out] srcCell 源Cell指针.
 * @return 0 成功,非0 失败.
 */
INT_X LUX_SYS_GetBindbyDest(IMPCell *dstCell, IMPCell *srcCell);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __LUX_SYS_H__ */
