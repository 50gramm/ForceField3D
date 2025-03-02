#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#ifdef ANDROID
#	include <android/log.h>
#endif
#include "Timing.hpp"
#include "Error.hpp"



void assertion_failed(
	const char* file, 
	const char* function, 
	int line, 
	const char* expression,
	const char *format,
	...
) 
{
	char message[1024] = "";
	va_list ap;
	static int count = 0;

	if(format != 0)
	{
		va_start(ap, format);
		vsprintf(message, format, ap);
		va_end(ap);
	}

	if(20 < count++)
		return;
	
	LOG_E(
		"Assertion failed!\n"
		"  File: %s:%d\n"
		"  Function: %s\n"
		"  Expression: \"%s\"\n",
		file, line, function, expression
	);
	if(message[0])
		LOG_E("  Message: \"%s\"\n", message);
}


void log(int level, const char *format, ...)
{
	static int min_log_level = 0;
	
	if(level < min_log_level)
		return;
	
	char message[4096] = "";
	va_list ap;
	va_start(ap, format);
	vsnprintf(message, sizeof(message), format, ap);	
	va_end(ap);
	
	size_t len = strlen(message);
	if(0 < len && message[len-1] == '\n')
		message[len-1] = 0;
	
#	ifdef ANDROID
	static int android_log_level[] = {ANDROID_LOG_DEBUG, ANDROID_LOG_INFO, ANDROID_LOG_ERROR};
	__android_log_print(android_log_level[level], MODULE_NAME, "[%06lld] %s", Timing::getMilliSec()%1000000, message);
#	else
	static FILE *output[] = {stdout, stdout, stderr};
	static const char* tags[] = {" (D)", "", " (E)"};
	fprintf(output[level], "[%06lld]%s %s\n", Timing::getMilliSec()%1000000, tags[level], message);
#	endif
}
