#include "Application.h"
#include "Assets.h"
#include "ModuleFileSystem.h"

Assets::Assets()
{
	Init();
}

Assets::~Assets()
{
	CleanUp();
}

void Assets::Draw()
{
	if (!active)
		return;

	ImGui::Begin("Assets", &active);

	if(ImGui::IsMouseHoveringWindow())
		if (ImGui::IsMouseClicked(1))
			ImGui::OpenPopup("AssetsOptions");
	

	if (ImGui::BeginPopup("AssetsOptions"))
	{
		if (ImGui::Selectable("Refresh"))
		{
			Refresh();
		}
		if (ImGui::Selectable("Open in Explorer"))
		{
			OpenInExplorer();
		}
		ImGui::EndPopup();
	}

	ImGui::Text(current_dir->path.data());

	//Back folder
	if (current_dir->parent != nullptr)
	{
		ImGui::Image((ImTextureID)folder_id, ImVec2(15, 15));
		ImGui::SameLine();

		if (ImGui::Selectable("../"))
			current_dir = current_dir->parent;	
	}
	

	//Print folders
	std::vector<Directory*>::iterator dir = current_dir->directories.begin();
	for (dir; dir != current_dir->directories.end(); dir++)
	{
		ImGui::Image((ImTextureID)folder_id, ImVec2(15, 15));
		ImGui::SameLine();
		
		if (ImGui::Selectable((*dir)->name.data()))
		{
			current_dir = (*dir);
			break;
		}
	}

	//Print files
	std::vector<string>::iterator file = current_dir->files.begin();
	for (file; file != current_dir->files.end(); file++)
	{
		ImGui::Image((ImTextureID)file_id, ImVec2(15, 15));
		ImGui::SameLine();

		if (ImGui::Selectable((*file).data()))
		{
			if (IsMeshExtension((*file).data()))
			{
				file_selected = current_dir->path + (*file);
				ImGui::OpenPopup("FileOptions");
			}			
		}
	}

	if (ImGui::BeginPopup("FileOptions"))
	{
		if (ImGui::Selectable("Load to scene"))
		{
			App->meshes->Load((file_selected).c_str(), current_dir->path.data());
		}

		if (ImGui::Selectable("Open"))
		{
			OpenInExplorer(&file_selected);
		}
		ImGui::EndPopup();
	}
	
	ImGui::End();
}

void Assets::Init()
{
	root = new Directory();
	root->path = ASSETS_FOLDER;
	root->name = "Assets";
	FillDirectoriesRecursive(root);

	current_dir = root;

	folder_id = App->meshes->LoadTexture("Resources/folder.png");
	file_id = App->meshes->LoadTexture("Resources/file.png");
}

void Assets::FillDirectoriesRecursive(Directory* root_dir)
{
	std::vector<string> folders;
	App->file_system->GetFilesAndDirectories(root_dir->path.data(), folders, root_dir->files);

	for (std::vector<string>::iterator it = folders.begin(); it != folders.end(); it++)
	{
		Directory* dir = new Directory();
		dir->path = root_dir->path + *it + "/";
		dir->name = (*it);
		dir->parent = root_dir;
		FillDirectoriesRecursive(dir);
		root_dir->directories.push_back(dir);
	}
}

bool Assets::IsMeshExtension(std::string fn)
{
	string extension = fn.substr(fn.find_last_of(".") + 1);

	//TODO: improve this approach
	char* mesh_ext[] = { "fbx", "FBX", "obj", "OBJ" };

	for (int i = 0; i < 4; i++)
	{
		if (extension.compare(mesh_ext[i]) == 0)
			return true;
	}

	return false;
}

void Assets::CleanUp()
{
	DeleteDirectoriesRecursive(root);
}

void Assets::DeleteDirectoriesRecursive(Directory* root_dir, bool keep_root)
{
	std::vector<Directory*>::iterator dir = root_dir->directories.begin();

	for (dir; dir != root_dir->directories.end(); dir++)
	{
		DeleteDirectoriesRecursive((*dir));
	}

	root_dir->directories.clear();
	root_dir->files.clear();

	if(!keep_root)
		delete root_dir;
}

void Assets::Refresh()
{
	DeleteDirectoriesRecursive(current_dir, true);
	FillDirectoriesRecursive(current_dir);
}

void Assets::OpenInExplorer(std::string* file_name)const
{
	char* base_path = SDL_GetBasePath();
	std::string base = base_path;
	//Remove /Debug/ and replace it for/Game/
#if _DEBUG
	base.erase(base.length() - 7, base.length());
	base.append("/Game/");
#endif
	
	if (file_name)
		base += *file_name;
	else
		base += current_dir->path;
	ShellExecute(NULL, "open", base.data(), NULL, NULL, SW_SHOWNORMAL);
	SDL_free(base_path);
}