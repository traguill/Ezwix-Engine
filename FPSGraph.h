#ifndef __FPSGRAPH_H__
#define __FPSGRAPH_H__

#include "Window.h"
#include "Application.h"
#include <vector>

using namespace std;

class FPSGraph : public Window
{
public:
	FPSGraph(Application* app);
	~FPSGraph();

	void Draw();
private:
	Application* App;
	vector<float> frames;
	int max_fps = 60;
};

#endif
