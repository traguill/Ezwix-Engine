#include "Application.h"
#include "Editor.h"
#include "Imgui\imgui.h"
#include "Random.h"
#include "FPSGraph.h"

Editor::Editor(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

Editor::~Editor()
{}

bool Editor::Start()
{
	bool ret = true;

	LOG("Start Editor");

	//Set Camera Position
	App->camera->Move(vec(10.0f, 6.0f, 30.0f));
	App->camera->LookAt(vec(0, 0, 0));

	//Create Windows
	windows.push_back(fps_graph_win = new FPSGraph());

	return ret;
}

bool Editor::CleanUp()
{
	LOG("Clean Up Editor");

	return true;
}

update_status Editor::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;
	

	ret = EditorWindows(); //Update the windows of the editor

	
	//Draw Grid
	Plane_P grid(0,1,0,0);
	grid.axis = true;
	grid.Render();
	
	return ret;	
}

update_status Editor::EditorWindows()
{
	update_status ret = UPDATE_CONTINUE;

	//Main MenuBar --------------------------------------------------------------------------------------------
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			WindowsMenu();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			HelpMenu();
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Quit", NULL))
		{
			ret = UPDATE_STOP;
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::ShowTestWindow();

	//Windows ----------------------------------------------------------------------------------------------------
	vector<Window*>::iterator win = windows.begin();
	while (win != windows.end())
	{
		(*win)->Draw();
		++win;
	}
	
	return ret;
}


//Menus -----------------------------------------------------------------------------------------------
void Editor::HelpMenu()
{
	if (ImGui::MenuItem("Documentation"))
		App->OpenURL("https://github.com/traguill/Ezwix-Engine/wiki");

	if (ImGui::MenuItem("Download latest build"))
		App->OpenURL("https://github.com/traguill/Ezwix-Engine/releases");

	if (ImGui::MenuItem("Report a bug"))
		App->OpenURL("https://github.com/traguill/Ezwix-Engine/issues");

	if (ImGui::MenuItem("About"))
	{
		ImGui::OpenPopup("##1 About");	
		if (ImGui::BeginPopup("##1 About"))
		{
			ImGui::Text("Ezwix Engine");
			ImGui::Text("A 3D engine for learning purposes.");
			ImGui::Text("Autor: Guillem Travila");
			ImGui::Text("Libraries: SDL 2.0, ImGui, MathGeoLib, Bullet, OpenGl 3.2");
			ImGui::Text("License"); //TODO

			ImGui::EndPopup();
		}
	}	
}

void Editor::WindowsMenu()
{
	if (ImGui::BeginMenu("Configuration"))
	{
		if (ImGui::MenuItem("FPS Graph"))
		{
			fps_graph_win->SetActive(true);
			
		}
			
		ImGui::EndMenu();
	}
}