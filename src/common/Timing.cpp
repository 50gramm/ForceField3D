#include <chrono>
#include "Timing.hpp"


TimeTick Timing::getNanoSec()
{
	return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}


TimeTick Timing::getMilliSec()
{
	return getNanoSec() / 1000000;
}


double Timing::getSec()
{
	return getNanoSec() * 1E-9;
}
