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
class ResourcesWindow;

class Editor : public Module
{
public:
	Editor(const char* name, bool start_enabled = true);
	~Editor();

	bool Init(Data& config);

	bool Start();
	update_status Update();
	bool CleanUp();

	string GetAssetsCurrentDir()const;
	void RefreshAssets()const;

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
	void DebugMenu();

	bool QuitWindow();
	void SaveSceneWindow();

public:
	Assets* assets = nullptr;

private:

	vector<Window*> windows;

	//Windows
	FPSGraph* fps_graph_win = nullptr;
	WindowOptions* winoptions_win = nullptr;
	HardwareInfo* hardware_win = nullptr;
	CameraWindow* camera_win = nullptr;
	ResourcesWindow* resource_win = nullptr;

	bool save_scene_win = false;
	string scene_name_to_save;
};

#endif