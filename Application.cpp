#include "Application.h"
#include "PerfTimer.h"
#include "LinearAllocator.h"
#include "Profiler.h"
#include "Data.h"

using namespace std;

Application::Application()
{
	window = new ModuleWindow("window");
	input = new ModuleInput("input");
	audio = new ModuleAudio("audio");
	scene_intro = new ModuleSceneIntro("scene_intro");
	renderer3D = new ModuleRenderer3D("renderer");
	camera = new ModuleCamera3D("camera");
	physics = new ModulePhysics3D("physics");
	meshes = new ModuleMeshes("meshes");
	editor = new Editor("editor");
	file_system = new ModuleFileSystem("file_system");
	go_manager = new ModuleGOManager("go_manager");

	//Globals
	g_Debug = new DebugDraw("debug_draw");

	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(file_system);
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(g_Debug);
	AddModule(meshes);
	AddModule(audio);
	AddModule(physics);
	AddModule(go_manager);
	
	// Scenes
	AddModule(scene_intro);

	//Editor
	AddModule(editor);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	list<Module*>::reverse_iterator i = list_modules.rbegin();

	while (i != list_modules.rend())
	{
		delete (*i);
		++i;
	}
}

bool Application::Init()
{
	bool ret = true;

	//Load Configuration
	char* buffer;
	if (App->file_system->Load("Configuration.json", &buffer) == 0)
		LOG("Error while loading Configuration file");
	Data config(buffer);
	delete[] buffer;

	// Call Init() in all modules
	list<Module*>::iterator i = list_modules.begin();

	while (i != list_modules.end() && ret == true)
	{
		ret = (*i)->Init(config.GetJObject((*i)->GetName()));
		++i;
	}


	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	i = list_modules.begin();

	while(i != list_modules.end() && ret == true)
	{
		ret = (*i)->Start();
		++i;
	}

	capped_ms = 1000 / fps;
	
	ms_timer.Start();
	last_sec_frame_time.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	dt = ms_timer.ReadSec();
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	//Update Profiler
	g_Profiler.Update();

	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		last_sec_frame_count = frame_count;
		frame_count = 0;
	}

	uint32_t last_frame_ms = ms_timer.Read();
	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		SDL_Delay(capped_ms - last_frame_ms);
	}
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	list<Module*>::iterator i = list_modules.begin();

	while (i != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*i)->PreUpdate(dt);
		++i;
	}

	i = list_modules.begin();

	while(i != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*i)->Update(dt);
		++i;
	}

	i = list_modules.begin();

	while (i != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*i)->PostUpdate(dt);
		i++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	list<Module*>::reverse_iterator i = list_modules.rbegin();

	while (i != list_modules.rend() && ret == true)
	{
		ret = (*i)->CleanUp();
		++i;
	}


	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::OpenURL(const char* url)
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

void Application::SetMaxFPS(int max_fps)
{
	fps = max_fps;
	if (fps == 0) fps = -1;
	capped_ms = 1000 / fps;
}

int Application::GetFPS()
{
	return last_sec_frame_count;
}