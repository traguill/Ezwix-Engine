#include "Application.h"
#include "ModuleResourceManager.h"
#include "MaterialImporter.h"
#include "Random.h"
#include "Data.h"

ModuleResourceManager::ModuleResourceManager(const char* name, bool start_enabled) : Module(name, start_enabled)
{
}

ModuleResourceManager::~ModuleResourceManager()
{
}

void ModuleResourceManager::FileDropped(const char * file_path)
{
	//File
	//Make a copy

	//Folder
	//Create folder
	//Look inside
	//Copy all files and folders(look inside..etc)

	//Files extensions accepted
	//Images: PNG
	//Meshes: FBX
	//Audio: PENDING

	if (App->file_system->IsDirectoryOutside(file_path))
	{
		LOG("Folder dropped");
	}
	else
	{
		//1-Make a copy of the file
		//2-Import to own file
		FileTypes type = GetFileExtension(file_path);
		string file_assets_path;
		switch (type)
		{
		case IMAGE:
			ImageDropped(file_path);
			break;
		case MESH:
			break;
		}
	}
	
}

///Given a path returns if the file is one of the valid extensions to import.
FileTypes ModuleResourceManager::GetFileExtension(const char * path)
{
	char* mesh_extensions[] = { "fbx", "FBX", "obj", "OBJ"};
	char* image_extensions[] = {"png", "PNG" };
	string name = path;
	string extension = name.substr(name.find_last_of(".") + 1);

	for (int i = 0; i < 4; i++)
		if (extension.compare(mesh_extensions[i]) == 0)
			return MESH;

	for (int i = 0; i < 2; i++)
		if (extension.compare(image_extensions[i]) == 0)
			return IMAGE;
	
	return NONE;
}

string ModuleResourceManager::CopyOutsideFileToAssetsCurrentDir(const char * path) const
{
	string current_dir = App->editor->GetAssetsCurrentDir();
	current_dir += App->file_system->GetNameFromPath(path);
	App->file_system->CopyFromOutsideFile(path, current_dir.data());
	return current_dir;
}

void ModuleResourceManager::GenerateMetaFile(const char * path, FileTypes type, uint uuid)const
{
	Data root;
	root.AppendUInt("Type", static_cast<unsigned int>(type));
	root.AppendUInt("UUID", uuid);
	root.AppendDouble("time_mod", App->file_system->GetLastModificationTime(path));

	char* buf;
	size_t size = root.Serialize(&buf);

	string final_path = path;
	final_path = final_path.substr(0, final_path.length() - 4); //Substract extension. Note: known is: ".png" or ".PNG" (4 char)
	final_path += ".meta";

	App->file_system->Save(final_path.data(), buf, size);

	delete[] buf;
}

void ModuleResourceManager::ImageDropped(const char* path) const
{
	string final_image_path; //Does nothing for now. 
	string file_assets_path = CopyOutsideFileToAssetsCurrentDir(path);
	uint uuid = App->rnd->RandomInt();
	GenerateMetaFile(file_assets_path.data(), IMAGE, uuid);
	MaterialImporter::Import(std::to_string(uuid).data(), file_assets_path.data(), final_image_path);
}
