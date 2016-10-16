#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Window.h"
#include <list>
#include <vector>

class FPSGraph;
class WindowOptions;
class HardwareInfo;
class Console;
class Assets;

class Editor : public Module
{
public:
	Editor(Application* app, bool start_enabled = true);
	~Editor();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

private:

	//Editor Windows
	update_status EditorWindows();

	//Menus
	void HelpMenu();
	void WindowsMenu();

private:
	vector<Window*> windows;

	//Windows
	FPSGraph* fps_graph_win = nullptr;
	WindowOptions* winoptions_win = nullptr;
	HardwareInfo* hardware_win = nullptr;
	Assets* assets = nullptr;
};

#endif