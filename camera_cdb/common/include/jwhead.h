#ifndef __JW_HEAD_H__
#define __JW_HEAD_H__

#define JW_IN
#define JW_OUT
#define JW_OUT_ARRAY
#define JW_INOUT

#ifndef _APP_TITLE_
#define _APP_TITLE_ ""
#endif

/*
MS VC++ 16.0 _MSC_VER = 1928 (Visual Studio 2019)

MS VC++ 15.0 _MSC_VER = 1910 (Visual Studio 2017)

MS VC++ 14.0 _MSC_VER = 1900 (Visual Studio 2015)

MS VC++ 12.0 _MSC_VER = 1800 (VisualStudio 2013)

MS VC++ 11.0 _MSC_VER = 1700 (VisualStudio 2012)

MS VC++ 10.0 _MSC_VER = 1600(VisualStudio 2010)

MS VC++ 9.0 _MSC_VER = 1500(VisualStudio 2008)

MS VC++ 8.0 _MSC_VER = 1400(VisualStudio 2005)

MS VC++ 7.1 _MSC_VER = 1310(VisualStudio 2003)

MS VC++ 7.0 _MSC_VER = 1300(VisualStudio .NET)

MS VC++ 6.0 _MSC_VER = 1200(VisualStudio 98)

MS VC++ 5.0 _MSC_VER = 1100(VisualStudio 97)
*/

/*
格式如下：
printf("\033[字背景颜色;字体颜色m 字符串 \033[0m" );

例子：
printf("\033[1m\033[45;33m HELLO_WORLD \033[0m\n");

颜色代码:

字背景颜色范围: 40--49             字颜色: 30—39

 40: 黑                           30: 黑

 41: 红                           31: 红

 42: 绿                           32: 绿

 43: 黄                           33: 黄

 44: 蓝                           34: 蓝

 45: 紫                           35: 紫

 46: 深绿                         36: 深绿

 47: 白色                         37: 白色

ANSI控制码:
   \033[0m   关闭所有属性

   \033[1m   设置高亮度

   \033[4m   下划线

   \033[5m   闪烁

   \033[7m   反显

  \033[8m   消隐

  \033[30m   --   \033[37m   设置前景色

  \033[40m   --   \033[47m   设置背景色

  \033[nA   光标上移n行

  \03[nB   光标下移n行

  \033[nC   光标右移n行

  \033[nD   光标左移n行
*/

//double整型化: % 四舍五入 向上取整（ceil()） 向下取整（floor）


#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <stdio.h>


#ifndef WIN32
#define _atoi64(val)     strtoll(val, NULL, 10)
#endif

#if defined(_WIN32) || defined(WIN32)
#define DIR_SEPARATOR "\\"
#define DIR_SEPARATOR_CH '\\'
#else
#define DIR_SEPARATOR "/"
#define DIR_SEPARATOR_CH '/'
#endif

#define DIR_SEPARATOR_UNIX "/"
#define DIR_SEPARATOR_CH_UNIX '/'

#ifdef _WIN32

#define CHECK_HANDLE_INVALID(a)	(NULL == a)

#define SAFE_CLOSESOCK(a)		{ if(INVALID_SOCKET != a) { ::closesocket(a); a = INVALID_SOCKET; } }
#define SAFE_CLOSEHANDLE(a)		{ if(NULL != a) { ::CloseHandle(a); a = NULL; } }
#define SAFE_CLOSEFILE(a)		{ if(NULL != a) { ::fclose(a); a = NULL; } }

#include <conio.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <WinBase.h>
#include <io.h>
#include <direct.h>
#include <winsock2.h>
#include <MSWSock.h>
#include <objbase.h>
#include <tchar.h>
#include <process.h>
#include <fcntl.h>
#include <sys\Stat.h>
#include <WS2tcpip.h>
#include <malloc.h>
#include <Shlwapi.h>
#include <shellapi.h>
#include <DbgHelp.h>

#if _MSC_VER >= 1800
#include <mutex>
#endif

#if _MSC_VER
//#define snprintf	_snprintf
#define snprintf(dst, size, fmt, ...)	_snprintf_s(dst, size, _TRUNCATE, fmt, ##__VA_ARGS__)
#define	strtok_r	strtok_s
#define	strcasecmp	_stricmp
#define	strncasecmp	_strnicmp
#define bzero(ptr, size) memset(ptr, 0, size)
#endif

#else 
#define LOWORD(l)           ((unsigned short)(((unsigned long)(l)) & 0xffff))
#define HIWORD(l)           ((unsigned short)((((unsigned long)(l)) >> 16) & 0xffff))
#include <unistd.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <pthread.h>
#include <time.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/ether.h> 
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/reboot.h>
#include <pwd.h>
#include <errno.h>

//#include <pcap.h>
#include <net/route.h>
#include <regex.h>
#include <libgen.h>
#include <sys/statfs.h>
#include <signal.h>
#include <setjmp.h>
#include <stdarg.h>
#include <linux/hdreg.h>

#define WINAPI 
#define stricmp strcasecmp

#endif

#define CHECK_POINT_NULL(a)		(NULL == a)
#define ASSIGN_POINT(a)		(NULL != a)
#define SAFE_DELETE(a)			{ if(NULL != a) { delete a; a = NULL; } }
#define SAFE_DELETEA(a)			{ if(NULL != a) { delete[] a; a = NULL; } }
#define SAFE_MEMSET(a)          {memset(a,0,sizeof(a));}

#if defined (__linux)
#include <sys/epoll.h>
#include <sys/stat.h>
#endif

#ifndef _MY_LOG_LEVEL_
#define _MY_LOG_LEVEL_
typedef enum MY_LOG_LEVEL
{
	MY_LOG_LEVEL_TRACE = 0,
	MY_LOG_LEVEL_TRACEING = 1,
	MY_LOG_LEVEL_WARN = 2,
	MY_LOG_LEVEL_WARNING = 3,
	MY_LOG_LEVEL_ERROR = 4,
	MY_LOG_LEVEL_ERRORING = 5,
}MY_LOG_LEVEL_T;
#endif

extern void debug_printf(int level, const char* file, const char* fun, const int line, const char* fmt, ...);

#if 1
#ifndef MYTRACE
#define MYTRACE(fmt,...) debug_printf(MY_LOG_LEVEL_TRACE, __FILE__,__FUNCTION__,__LINE__,fmt,##__VA_ARGS__)
#endif

#ifndef MYTRACEING
#define MYTRACEING(fmt,...) debug_printf(MY_LOG_LEVEL_TRACEING, __FILE__,__FUNCTION__,__LINE__,fmt,##__VA_ARGS__)
#endif

#ifndef MYWARN
#define MYWARN(fmt,...)  debug_printf(MY_LOG_LEVEL_WARN, __FILE__,__FUNCTION__,__LINE__,fmt,##__VA_ARGS__)
#endif

#ifndef MYWARNING
#define MYWARNING(fmt,...)  debug_printf(MY_LOG_LEVEL_WARNING, __FILE__,__FUNCTION__,__LINE__,fmt,##__VA_ARGS__)
#endif

#ifndef MYERROR
#define MYERROR(fmt,...) debug_printf(MY_LOG_LEVEL_ERROR, __FILE__,__FUNCTION__,__LINE__,fmt,##__VA_ARGS__)
#endif

#ifndef MYERRORING
#define MYERRORING(fmt,...) debug_printf(MY_LOG_LEVEL_ERRORING, __FILE__,__FUNCTION__,__LINE__,fmt,##__VA_ARGS__)
#endif

#else
#define MYTRACE printf
#define MYERROR printf
#define MYWARN  printf
#endif

#if 0
#ifndef MYTRACE
#define MYTRACE(fmt, ...) do {\
struct tm* pTempTm; struct timeval time; \
gettimeofday(&time, NULL);\
pTempTm = localtime(&time.tv_sec);\
printf("[%02d:%02d:%02d.%03d %s %s %d][I] ", pTempTm->tm_hour, pTempTm->tm_min, pTempTm->tm_sec, time.tv_usec / 1000, __FILE__,__FUNCTION__,__LINE__);\
printf(fmt,  ##__VA_ARGS__);\
int size1=strlen(fmt);\
if (size1>0 && fmt[size1-1]!='\n') printf("\n");\
} while(0)
#endif

#ifndef MYWARN
#define MYWARN(fmt, ...) do {\
struct tm* pTempTm; struct timeval time; \
gettimeofday(&time, NULL);\
pTempTm = localtime(&time.tv_sec);\
printf("\033[1m\033[40;33m[%02d:%02d:%02d.%03d %s %s %d][W] ", pTempTm->tm_hour, pTempTm->tm_min, pTempTm->tm_sec, time.tv_usec / 1000, __FILE__,__FUNCTION__,__LINE__);\
char szbuf[2048]={0};\
int size1 = strlen(fmt);if (size1 > 0 && fmt[size1 - 1] == '\n') memcpy(szbuf, fmt, size1 - 1); else strncpy(szbuf,fmt,size1-1);\
printf(szbuf,  ##__VA_ARGS__);\
printf("\033[0m\n");\
} while(0)
#endif

#ifndef MYWARNING
#define MYWARNING(fmt, ...) do {\
struct tm* pTempTm; struct timeval time; \
gettimeofday(&time, NULL);\
pTempTm = localtime(&time.tv_sec);\
printf("\033[5m\033[40;33m[%02d:%02d:%02d.%03d %s %s %d][W] ", pTempTm->tm_hour, pTempTm->tm_min, pTempTm->tm_sec, time.tv_usec / 1000, __FILE__,__FUNCTION__,__LINE__);\
char szbuf[2048]={0};\
int size1 = strlen(fmt);if (size1 > 0 && fmt[size1 - 1] == '\n') memcpy(szbuf, fmt, size1 - 1); else strncpy(szbuf,fmt,size1-1);\
printf(szbuf,  ##__VA_ARGS__);\
printf("\033[0m\n");\
} while(0)
#endif

#ifndef MYERROR
#define MYERROR(fmt, ...) do {\
struct tm* pTempTm; struct timeval time; \
gettimeofday(&time, NULL);\
pTempTm = localtime(&time.tv_sec);\
printf("\033[1m\033[40;31m[%02d:%02d:%02d.%03d %s %s %d][E] ", pTempTm->tm_hour, pTempTm->tm_min, pTempTm->tm_sec, time.tv_usec / 1000, __FILE__,__FUNCTION__,__LINE__);\
char szbuf[2048]={0};\
int size1 = strlen(fmt);if (size1 > 0 && fmt[size1 - 1] == '\n') memcpy(szbuf, fmt, size1 - 1); else strncpy(szbuf,fmt,size1-1);\
printf(szbuf,  ##__VA_ARGS__);\
printf("\033[0m\n");\
} while(0)
#endif

#ifndef MYERRORING
#define MYERRORING(fmt, ...) do {\
struct tm* pTempTm; struct timeval time; \
gettimeofday(&time, NULL);\
pTempTm = localtime(&time.tv_sec);\
printf("\033[5m\033[40;31m[%02d:%02d:%02d.%03d %s %s %d][E] ", pTempTm->tm_hour, pTempTm->tm_min, pTempTm->tm_sec, time.tv_usec / 1000, __FILE__,__FUNCTION__,__LINE__);\
char szbuf[2048]={0};\
int size1 = strlen(fmt);if (size1 > 0 && fmt[size1 - 1] == '\n') memcpy(szbuf, fmt, size1 - 1); else strncpy(szbuf,fmt,size1-1);\
printf(szbuf,  ##__VA_ARGS__);\
printf("\033[0m\n");\
} while(0)
#endif
#endif

#if 0
#endif

#endif
