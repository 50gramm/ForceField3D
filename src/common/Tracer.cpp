#ifdef _TRACE

#include <iomanip>
#include "Tracer.hpp"

std::map<std::string, TraceStatistics::Statistic> TraceStatistics::statistics;

template<class T>
std::string formatCompact(T val)
{
	const char* metrics = " KMG";
	int i=0;
	while(30000 < val && metrics[i+1])
	{
		val /= 1000;
		i += 1;
	}

	std::string res = std::to_string(val) + metrics[i];
	return res;
}


TraceStatistics::Tracer::Tracer(Statistic& stat)
	: stat(stat)
	, t1(Timing::getNanoSec())
{}


TraceStatistics::Tracer::~Tracer()
{
	stat.totalTime += Timing::getNanoSec() - t1;
	stat.count += 1;
}


void TraceStatistics::print(std::ostream& out)
{
	out << "=============================== Trace Statistics ===============================\n";
	out << "|              Name              | Total time(μs) | Avg time(μs) |    Count    |\n";
	for(const auto& stat : statistics)
	{
		if(stat.second.count == 0)
			continue;
		
		out << "|" << stat.first << std::setw(22 - stat.first.size()) << " "
			<< std::setw(27) << formatCompact(stat.second.totalTime/1000) << " "
			<< std::setw(14) << formatCompact(stat.second.totalTime/1000/stat.second.count) << " "
			<< std::setw(13) << stat.second.count << "|\n";
	}
	out << "--------------------------------------------------------------------------------" << std::endl;
}


struct TraceAutoPrint
{
	~TraceAutoPrint()
	{
		TraceStatistics::print(std::cout);
	}
} traceAutoPrint;

#endif
