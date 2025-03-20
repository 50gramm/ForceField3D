#ifdef _TIMING

#include <iomanip>
#include "Tracer.hpp"

std::map<std::string, TraceStatistics::Statistic> TraceStatistics::statistics;


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
	out << "|              Name              | Total time(ns) | Avg time(ns) |    Count    |\n";
	for(const auto& stat : statistics)
	{
		if(stat.second.count == 0)
			continue;
		
		out << "|" << stat.first << std::setw(22 - stat.first.size()) << " "
			<< std::setw(27) << stat.second.totalTime << " "
			<< std::setw(14) << (stat.second.totalTime/stat.second.count) << " "
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
