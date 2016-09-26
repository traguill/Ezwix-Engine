#include "Profiler.h"

Profiler::Profiler()
{}

Profiler::~Profiler()
{
	map<const char*, vector<double>>::iterator it = samples.begin();

	for (it; it != samples.end(); it++)
		(*it).second.clear();

	samples.clear();
}

void Profiler::StoreSample(const char* name, double time)
{
	map<const char*, vector<double>>::iterator it = samples.find(name);

	if (it != samples.end())
	{
		(*it).second.push_back(time);
	}
	else
	{
		samples.insert(pair<const char*, vector<double>>(name, time));
	}
}