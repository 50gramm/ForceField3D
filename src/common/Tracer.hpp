#pragma once
#ifdef _TIMING

#include <map>
#include <string>
#include "Timing.hpp"

struct TraceStatistics
{
	struct Statistic
	{
		TimeTick totalTime = 0;
		long long count = 0;
	};

	class Tracer
	{
		Statistic& stat;
		TimeTick t1;
	
	public:
		Tracer(Statistic& stat);
		~Tracer();
	};

	static void print(std::ostream& out);

	static std::map<std::string, Statistic> statistics;
};


#	define TRACE_SCOPE(name) TRACE_SCOPE1(__COUNTER__, name)
#	define TRACE_SCOPE1(counter, name) TRACE_SCOPE2(counter, name)
#	define TRACE_SCOPE2(counter, name)\
	static TraceStatistics::Statistic& _traceStat##counter = TraceStatistics::statistics[name];\
	TraceStatistics::Tracer _tracer##counter(_traceStat##counter);

#	define TRACE_FUNCTION TRACE_SCOPE(__func__)
#else
#	define TRACE_SCOPE(name)
#	define TRACE_FUNCTION
#endif
