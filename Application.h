#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <list>
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "Editor.h"
#include "ModuleMeshes.h"
#include "ModuleFileSystem.h"
#include "ModuleGOManager.h"

#include "AutoProfile.h"

using namespace std; 

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void OpenURL(const char* url);
	void SetMaxFPS(int max_fps);
	int GetFPS();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModuleMeshes* meshes;
	ModuleFileSystem* file_system;
	ModuleGOManager* go_manager;
	Editor* editor;

private:

	Timer	ms_timer;
	float	dt;
	list<Module*> list_modules;
	int fps = 60;
	int capped_ms = -1;
	int frame_count = 0;
	int last_sec_frame_count = 0;
	Timer last_sec_frame_time;

};

extern Application* App;

#endif 