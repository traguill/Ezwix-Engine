
#ifndef __ASSETS_H__
#define __ASSETS_H__

#include "Window.h"
#include <vector>

class Assets : public Window
{
public:
	Assets();
	~Assets();

	void Draw();
private:

	void Init();

private:
	std::vector<const char*> files;
};

#endif
