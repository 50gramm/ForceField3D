#include <cstdio>
#include <cstdarg>
#include "StringUtils.hpp"


std::string sformat(const char *format, ...)
{
	char message[4096] = "";
	va_list ap;
	va_start(ap, format);
	vsnprintf(message, sizeof(message), format, ap);
	return message;
}
