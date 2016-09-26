#ifndef __PROFILER_H__
#define __PROFILER_H__

#include <map>
#include <vector>

using namespace std;

class Profiler 
{
public:
	Profiler();
	~Profiler();

	void StoreSample(const char* name, double time);

private:
	map<const char*, vector<double>> samples;
};

extern Profiler g_Profiler;

#endif
