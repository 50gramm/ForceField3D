#pragma once
#include "Core.hpp"

using TimeTick = long long;


class Timing
{
public:
	static TimeTick getNanoSec();

	static TimeTick getMilliSec();

	static double getSec();
};
