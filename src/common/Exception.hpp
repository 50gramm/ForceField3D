#pragma once

#include <stdexcept>
#include "StringUtils.hpp"


class Exception : public std::runtime_error
{
public:
	template<typename... Args>
	Exception(const char* format, Args... args)
		: std::runtime_error(sformat(format, args...))
	{
	}
};
