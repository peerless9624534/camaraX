#ifndef IVESDK_VERSION_H_INCLUDED
#define IVESDK_VERSION_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

const char* ivesdk_version();

const char* ivesdk_version_full();

unsigned    ivesdk_version_major();

unsigned    ivesdk_version_minor();

unsigned    ivesdk_version_build();

const char* ivesdk_version_git();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IVESDK_VERSION_H_INCLUDED */

