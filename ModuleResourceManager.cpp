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
		ImportFolder(file_path);
	}
	else
	{
		ImportFile(file_path);
	}
	
}

///Given a path returns if the file is one of the valid extensions to import.
FileTypes ModuleResourceManager::GetFileExtension(const char * path) const
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

string ModuleResourceManager::CopyOutsideFileToAssetsCurrentDir(const char * path, string base_dir) const
{
	string current_dir;
	if (base_dir.size() == 0)
		current_dir = App->editor->GetAssetsCurrentDir();
	else
		current_dir = base_dir;
	current_dir += App->file_system->GetNameFromPath(path);
	App->file_system->CopyFromOutsideFile(path, current_dir.data());
	return current_dir;
}

void ModuleResourceManager::GenerateMetaFile(const char * path, FileTypes type, uint uuid, bool is_file)const
{
	Data root;
	root.AppendUInt("Type", static_cast<unsigned int>(type));
	root.AppendUInt("UUID", uuid);
	root.AppendDouble("time_mod", App->file_system->GetLastModificationTime(path));

	char* buf;
	size_t size = root.Serialize(&buf);

	string final_path = path;
	if(is_file)
		final_path = final_path.substr(0, final_path.length() - 4); //Substract extension. Note: known is: ".png" ".fbx", etc. (4 char)
	final_path += ".meta";

	App->file_system->Save(final_path.data(), buf, size);

	delete[] buf;
}

void ModuleResourceManager::ImportFolder(const char * path, string base_dir, string base_library_dir) const
{
	vector<string> files, folders;
	App->file_system->GetFilesAndDirectoriesOutside(path, folders, files);
	string directory_path = path;

	//Create Folder metadata
	uint uuid = App->rnd->RandomInt();
	string folder_assets_path;
	if(base_dir.size() == 0)
		folder_assets_path = App->editor->GetAssetsCurrentDir() + App->file_system->GetNameFromPath(path);
	else
		folder_assets_path = base_dir + App->file_system->GetNameFromPath(path);
	App->file_system->GenerateDirectory(folder_assets_path.data());
	GenerateMetaFile(folder_assets_path.data(), FOLDER, uuid, false);

	//Create Folder at Library
	string library_path;
	if (base_library_dir.size() == 0)
		library_path = LIBRARY_FOLDER;
	else
		library_path = base_library_dir;
	library_path += std::to_string(uuid);
	App->file_system->GenerateDirectory(library_path.data());

	directory_path += "/"; //Add folder ending manually
	folder_assets_path += "/";
	library_path += "/";

	//Import files
	string file_path;
	for (vector<string>::const_iterator file = files.begin(); file != files.end(); ++file)
	{
		file_path = directory_path + (*file);
		ImportFile(file_path.data(), folder_assets_path, library_path);
	}

	//Import folders
	string folder_path;
	for (vector<string>::const_iterator folder = folders.begin(); folder != folders.end(); ++folder)
	{
		folder_path = directory_path + (*folder);
		ImportFolder(folder_path.data(), folder_assets_path, library_path);
	}
}

void ModuleResourceManager::ImportFile(const char * path, string base_dir, string base_library_dir) const
{
	//1-Make a copy of the file
	//2-Import to own file
	FileTypes type = GetFileExtension(path);
	switch (type)
	{
	case IMAGE:
		ImageDropped(path, base_dir, base_library_dir);
		break;
	case MESH:
		break;
	}
}

void ModuleResourceManager::ImageDropped(const char* path, string base_dir, string base_library_dir) const
{
	string file_assets_path = CopyOutsideFileToAssetsCurrentDir(path, base_dir);
	uint uuid = App->rnd->RandomInt();
	GenerateMetaFile(file_assets_path.data(), IMAGE, uuid);

	string final_image_path;
	if (base_library_dir.size() == 0)
		final_image_path = LIBRARY_FOLDER;
	else
		final_image_path = base_library_dir;

	final_image_path += std::to_string(uuid) + "/";
	App->file_system->GenerateDirectory(final_image_path.data());
	final_image_path += std::to_string(uuid);
	final_image_path += ".dds";

	MaterialImporter::Import(std::to_string(uuid).data(), file_assets_path.data(), final_image_path);
}
