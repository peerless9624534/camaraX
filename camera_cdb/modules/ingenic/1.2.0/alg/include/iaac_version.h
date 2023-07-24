#ifndef __IAAC_VERSION_H__
#define __IAAC_VERSION_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

/**
 * [31:20]: license version, 0x200 means 2.0.0, if changed, your must change your iaac
 * [19:12]: main version, range[0-255], means so much important changes, include some important changes, must update your iaac
 * [11: 8]: middle version, range[0-15], means important changes, advice to update iaac
 * [7:0]  : sub version, range[0-255], means unimportant changes, you can decide to update iaac whether or not by your self
 */
#define IAAC_SDK_VERSION	0x20000002

/**
 * @fn unsigned int IAAC_GetSDKVersion(void)
 *
 * 获取IAAC库版本
 *
 * @retval IAAC库版本
 *
 * @remarks 用于获取IAAC库版本，包括IAAC库协议版本，代码版本，等于IAAC_SDK_VERSION
 */
unsigned int IAAC_GetSDKVersion(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
