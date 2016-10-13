#ifndef __PROFILER_H__
#define __PROFILER_H__

#include <map>
#include <vector>
#include "Window.h"

using namespace std;

#define MAX_TIME_ITEMS 600

struct ProfilerSample
{
	vector<double> total_ms;
	vector<unsigned int> calls;
	unsigned int id; //Position to insert new sample
};

class Profiler : public Window
{
public:
	Profiler();
	~Profiler();

	void Update();
	void Draw();

	void StoreSample(const char* name, double time);

private:
	map<const char*, ProfilerSample> samples;

	//Window
	int current_frame = MAX_TIME_ITEMS -1;
	bool is_playing = false;
};

extern Profiler g_Profiler;

#endif
