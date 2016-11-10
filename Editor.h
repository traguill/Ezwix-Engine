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
class CameraWindow;

class Editor : public Module
{
public:
	Editor(const char* name, bool start_enabled = true);
	~Editor();

	bool Init(Data& config);

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

private:
	//Game Simulation Options
	void GameOptions()const;

	//Editor Windows
	update_status EditorWindows();

	//Menus
	void FileMenu();
	void HelpMenu();
	void WindowsMenu();
	void EditMenu();

	bool QuitWindow()const;

private:

	vector<Window*> windows;

	//Windows
	FPSGraph* fps_graph_win = nullptr;
	WindowOptions* winoptions_win = nullptr;
	HardwareInfo* hardware_win = nullptr;
	Assets* assets = nullptr;
	CameraWindow* camera_win = nullptr;
};

#endif