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
			ImGui::OpenPopup("DirectoryOptions");
			dir_selected = (*dir);
		}
	}

	//Print files
	std::vector<AssetFile*>::iterator file = current_dir->files.begin();
	for (file; file != current_dir->files.end(); file++)
	{
		if ((*file)->type != FOLDER)
		{
			ImGui::Image((ImTextureID)file_id, ImVec2(15, 15)); //Same image for every type of file for now
			ImGui::SameLine();

			switch ((*file)->type)
			{
			case IMAGE:
				if (ImGui::Selectable((*file)->name.data()))
				{
					file_selected = (*file);
					//imgui open popup fileimageoptions TODO
				}
				break;
			case MESH:
				if (ImGui::Selectable((*file)->name.data()))
				{
					file_selected = (*file);
					ImGui::OpenPopup("FileMeshOptions");
				}
				break;
			case SCENE:
				if (ImGui::Selectable((*file)->name.data()))
				{
					file_selected = (*file);
				}
					break;
			}

				//ImGui::OpenPopup("FileSceneOptions");
			
		}
	}

	//PopUps File type options --------------------------------------------------

	DirectoryOptions();
	MeshFileOptions();
	SceneFileOptions();
	
	ImGui::End();
}

string Assets::CurrentDirectory() const
{
	return current_dir->path;
}

string Assets::CurrentLibraryDirectory() const
{
	return current_dir->library_path;
}

void Assets::Init()
{
	//start active
	active = true;

	root = new Directory();
	root->path = ASSETS_FOLDER;
	root->name = "Assets";
	root->library_path = LIBRARY_FOLDER;
	FillDirectoriesRecursive(root);

	current_dir = root;

	//folder_id = App->meshes->LoadTexture("Resources/folder.png"); TODO: Best way to load icons?
	//file_id = App->meshes->LoadTexture("Resources/file.png");
}

void Assets::FillDirectoriesRecursive(Directory* root_dir)
{
	vector<string> folders;
	vector<string> files;
	App->file_system->GetFilesAndDirectories(root_dir->path.data(), folders, files, true);

	for (vector<string>::iterator file = files.begin(); file != files.end(); ++file)
	{
		char* buffer = nullptr;
		uint size = App->file_system->Load((root_dir->path + (*file)).data(), &buffer);
		if (size == 0)
		{
			LOG("Error while loading Meta file: %s", (root_dir->path + (*file)).data());
			if (buffer)
				delete[] buffer;
			return;
		}
		Data meta(buffer);

		FileTypes type = static_cast<FileTypes>(meta.GetInt("Type"));

		if (type == FOLDER)
		{
			Directory* dir = new Directory();
			string folder_name = (*file).substr(0, (*file).length() - 5);
			dir->path = root_dir->path + folder_name + "/";
			dir->name = folder_name;
			dir->parent = root_dir;
			dir->library_path = root_dir->library_path + std::to_string(meta.GetUInt("UUID")) + "/";
			root_dir->directories.push_back(dir);

			FillDirectoriesRecursive(dir);
		}
		else
		{
			AssetFile* a_file = new AssetFile();
			a_file->type = type;
			a_file->name = (*file).substr(0, (*file).length() - 5); //Substract extension (.meta)
			a_file->file_path = root_dir->path + (*file);
			a_file->uuid = meta.GetUInt("UUID");
			a_file->content_path = root_dir->library_path + std::to_string(a_file->uuid) + "/" + std::to_string(a_file->uuid);
			a_file->time_mod = meta.GetInt("time_mod");
			a_file->original_file = meta.GetString("original_file");
			a_file->directory = root_dir;
			root_dir->files.push_back(a_file);
		}
		delete[] buffer;
	}
}

bool Assets::IsMeshExtension(const std::string& fn)const
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
	file_selected = nullptr;
	dir_selected = nullptr;
	DeleteDirectoriesRecursive(root);
}

void Assets::DeleteDirectoriesRecursive(Directory* root_dir, bool keep_root)
{
	vector<Directory*>::iterator dir = root_dir->directories.begin();

	for (dir; dir != root_dir->directories.end(); dir++)
	{
		DeleteDirectoriesRecursive((*dir));
	}

	root_dir->directories.clear();
	
	vector<AssetFile*>::iterator file = root_dir->files.begin();
	for (file; file != root_dir->files.end(); ++file)
		delete *file;
	root_dir->files.clear();

	if(!keep_root)
		delete root_dir;

}

bool Assets::IsSceneExtension(const std::string& file_name) const
{
	string extension = file_name.substr(file_name.find_last_of(".") + 1);

	const char* ext = "ezx";

	return (extension.compare(ext) == 0) ? true : false;
}

void Assets::Refresh()
{
	DeleteDirectoriesRecursive(current_dir, true);
	file_selected = nullptr;
	dir_selected = nullptr;
	FillDirectoriesRecursive(current_dir);
}

void Assets::OpenInExplorer(const std::string* file_name)const
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

void Assets::MeshFileOptions() 
{
	if (ImGui::BeginPopup("FileMeshOptions"))
	{
		if (ImGui::Selectable("Load to scene"))
		{
			App->resource_manager->LoadFile(file_selected->content_path, MESH);
		}

		if (ImGui::Selectable("Remove"))
		{
			DeleteAssetFile(file_selected);
		}

		if (ImGui::Selectable("Open"))
		{
			OpenInExplorer(&file_selected->file_path);
		}

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
}

void Assets::SceneFileOptions()
{
	if (ImGui::BeginPopup("FileSceneOptions"))
	{
		if (ImGui::Selectable("Load to scene"))
		{
			//App->go_manager->LoadScene(file_selected.data()); TODO:Load scenes in new format
		}
		ImGui::EndPopup();
	}
}

void Assets::DirectoryOptions()
{
	if (ImGui::BeginPopup("DirectoryOptions"))
	{
		if (ImGui::Selectable("Open"))
		{
			current_dir = dir_selected;
			dir_selected = nullptr;
		}
		if (ImGui::Selectable("Remove"))
		{
			DeleteAssetDirectory(dir_selected);
			dir_selected = nullptr;
		}
		ImGui::EndPopup();
	}
}

void Assets::DeleteAssetDirectory(Directory * directory)
{
	if (directory == nullptr)
		return;
	App->file_system->Delete(directory->library_path);
	App->file_system->Delete(directory->path);
	string meta_file = directory->parent->path + directory->name + ".meta";
	App->file_system->Delete(meta_file);

	Directory* parent = directory->parent;
	parent->directories.erase(std::remove(parent->directories.begin(), parent->directories.end(), directory), parent->directories.end());
	delete directory;
	directory = nullptr;
}

void Assets::DeleteAssetFile(AssetFile * file)
{
	if (file == nullptr)
		return;
	
	//TODO: If the file is being used don't delete it
	string library_folder = file->content_path.substr(0, file->content_path.find_last_of("\//"));
	
	App->file_system->Delete(library_folder.data());
	App->file_system->Delete(file->file_path.data());
	App->file_system->Delete(file->original_file.data());

	Directory* dir = file->directory;
	dir->files.erase(std::remove(dir->files.begin(), dir->files.end(), file), dir->files.end());
	delete file;
	file = nullptr;
}
