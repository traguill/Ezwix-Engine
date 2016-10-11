#include "Application.h"
#include "Assets.h"
#include "ModuleFileSystem.h"

Assets::Assets()
{
	Init();
}

Assets::~Assets()
{
	files.clear();
}

void Assets::Draw()
{
	if (!active)
		return;

	ImGui::Begin("Assets", &active);

	std::vector<const char*>::const_iterator it = files.begin();

	while (it != files.end())
	{
		const char* file = *it; //For some reason imgui::text does not accept the *it directly
		ImGui::Text(file);
		++it;
	}

	ImGui::End();
}

void Assets::Init()
{
	if (App->file_system->GetEnumerateFiles("Assets", files))
	{
		LOG("Files found in Assets");
	}
}
