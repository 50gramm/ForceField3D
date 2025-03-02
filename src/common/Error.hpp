#pragma once

#ifdef _DEVELOPMENT

#	if defined(_MSC_VER)
#		define DBG_BREAK			__debugbreak();
#	elif defined(__i386__) || defined(__x86_64__) 
#		define DBG_BREAK		 	__asm__ volatile("int $0x03");
#	else
#		define DBG_BREAK
#	endif

	void assertion_failed(
		const char* file,
		const char* function,
		int line,
		const char* expression,
		const char *format = 0,
		...
	);
#	define ASSERT(condition, ...)		{ if(!(condition)) { assertion_failed(__FILE__, __FUNCTION__, __LINE__, #condition, ##__VA_ARGS__); DBG_BREAK }}
#else
#	define DBG_BREAK
#	define ASSERT(condition, ...)
#	define dprint(...)
#endif

void log(int type, const char *format, ...);

#ifdef _DEVELOPMENT
#	define LOG_D(...) log(0, __VA_ARGS__)
#	define LOG_I(...) log(1, __VA_ARGS__)
#	define LOG_E(...) log(2, __VA_ARGS__)
#else
#	define LOG_D(...) 
#	define LOG_I(...) 
#	define LOG_E(...) 
#endif
