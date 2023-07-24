#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <func_log.h>

#if 1
static int _log_level = LOG_LEVEL_VERBOSE;

void _log(int level, const char* tag, const char* file, const char* func, int line, const char* fmt, ...)
{
    va_list vl;
    if (level < _log_level)
	return;

    if (level >= LOG_LEVEL_ERROR) {
	fprintf(stdout, "\e[1;31m");
    }

    fprintf(stdout, "[%-6s]:%s-%d ", tag, func, line);
    va_start(vl, fmt);
    vfprintf(stdout, fmt, vl);
    va_end(vl);

    if (level >= LOG_LEVEL_ERROR) {
	fprintf(stdout, "\e[0m");
    }

}
#endif
