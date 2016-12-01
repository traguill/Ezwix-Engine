#include "Application.h"
#include "ModuleResourceManager.h"
#include "MaterialImporter.h"
#include "MeshImporter.h"
#include "Random.h"
#include "Data.h"
#include "ResourceFileMesh.h"
#include "ResourceFileTexture.h"
#include "ModuleGOManager.h"
#include "GameObject.h"
#include "Glew\include\glew.h"
#include <gl\GL.h>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "Devil/include/il.h"
#include "Devil/include/ilut.h"
#pragma comment ( lib, "Devil/libx86/DevIL.lib" )
#pragma comment ( lib, "Devil/libx86/ILU.lib" )
#pragma comment ( lib, "Devil/libx86/ILUT.lib" )

ModuleResourceManager::ModuleResourceManager(const char* name, bool start_enabled) : Module(name, start_enabled)
{
}

ModuleResourceManager::~ModuleResourceManager()
{
	for (list<ResourceFile*>::iterator rc_file = resource_files.begin(); rc_file != resource_files.end(); ++rc_file)
		delete *rc_file;
	resource_files.clear();
}

bool ModuleResourceManager::Init(Data & config)
{
	aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	//Initialize Devil
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	return true;
}

bool ModuleResourceManager::CleanUp()
{
	ilShutDown();
	aiDetachAllLogStreams();
	return true;
}

void ModuleResourceManager::FileDropped(const char * file_path)
{
	//Files extensions accepted
	//Images: PNG TGA
	//Meshes: FBX / OBJ
	//Audio: PENDING

	if (App->file_system->IsDirectoryOutside(file_path))
	{
		vector<tmp_mesh_file> mesh_files;
		ImportFolder(file_path, mesh_files);

		for (vector<tmp_mesh_file>::iterator tmp = mesh_files.begin(); tmp != mesh_files.end(); tmp++)
		{
			ImportFile(tmp->mesh_path.data(), tmp->assets_folder, tmp->library_folder);
		}
		mesh_files.clear();
	}
	App->editor->RefreshAssets();
}

void ModuleResourceManager::LoadFile(const string & library_path, const FileTypes & type)
{
	switch (type)
	{
	case MESH:
		LoadPrefabFile(library_path);
		break;
	case PREFAB:
		LoadPrefabFile(library_path);
		break;
	}
}

ResourceFile * ModuleResourceManager::LoadResource(const string & path, ResourceFileType type)
{
	ResourceFile* rc_file = nullptr;

	string name = path.substr(path.find_last_of("/\\") + 1);
	name = name.substr(0, name.find_last_of('.'));
	unsigned int uuid = std::stoul(name);
	rc_file = FindResourceByUUID(uuid);

	if (rc_file == nullptr)
	{
		switch (type)
		{
		case RES_MESH:
			rc_file = new ResourceFileMesh(type, path, uuid);
			break;
		case RES_TEXTURE:
			rc_file = new ResourceFileTexture(type, path, uuid);
			break;
		}
	}

	return rc_file;
}

void ModuleResourceManager::UnloadResource(const string & path)
{
	ResourceFile* rc_file = nullptr;

	string name = path.substr(path.find_last_of("/\\") + 1);
	name = name.substr(0, name.find_last_of('.'));
	unsigned int uuid = std::stoul(name);
	rc_file = FindResourceByUUID(uuid);

	if (rc_file != nullptr)
	{
		rc_file->Unload();
	}
}

ResourceFile * ModuleResourceManager::FindResourceByUUID(unsigned int uuid)
{
	for (list<ResourceFile*>::iterator rc_file = resource_files.begin(); rc_file != resource_files.end(); ++rc_file)
		if ((*rc_file)->GetUUID() == uuid)
			return *rc_file;

	return nullptr;
}

void ModuleResourceManager::SaveScene(const char * file_name, string base_library_path)
{
	string name_to_save = file_name;

	Data root_node;
	root_node.AppendArray("GameObjects");

	App->go_manager->root->Save(root_node);
	
	char* buf;
	size_t size = root_node.Serialize(&buf);

	//Add extension if doesn't have it yet
	if (name_to_save.find(".ezx", name_to_save.length() - 4) == string::npos)
		name_to_save += ".ezx";

	App->file_system->Save(name_to_save.data(), buf, size);
	

	string meta_file = name_to_save.substr(0, name_to_save.length() - 4) + ".meta";
	if (App->file_system->Exists(meta_file.data()))
	{
		//Refresh
		LOG("The scene already exists");
	}
	else
	{
		unsigned int uuid = App->rnd->RandomInt();
		string library_dir = base_library_path + "/" + std::to_string(uuid) + "/";
		App->file_system->GenerateDirectory(library_dir.data());
		GenerateMetaFile(name_to_save.data(), FileTypes::SCENE, uuid, library_dir.data());
		string library_filename = library_dir + std::to_string(uuid) + ".ezx";
		App->file_system->Save(library_filename.data(), buf, size); //Duplicate the file in library
	}

	delete[] buf;
	App->editor->RefreshAssets();
}

bool ModuleResourceManager::LoadScene(const char * file_name)
{
	bool ret = false;
	//TODO: Now the current scene is destroyed. Ask the user if wants to save the changes.

	char* buffer = nullptr;
	uint size = App->file_system->Load(file_name, &buffer);
	if (size == 0)
	{
		LOG("Error while loading Scene: %s", file_name);
		if (buffer)
			delete[] buffer;
		return false;
	}

	Data scene(buffer);
	Data root_objects;
	root_objects = scene.GetArray("GameObjects", 0);

	if (root_objects.IsNull() == false)
	{
		//Remove the current scene
		App->go_manager->ClearScene();

		for (int i = 0; i < scene.GetArraySize("GameObjects"); i++)
		{
			if (i == 0)
				App->go_manager->root = App->go_manager->LoadGameObject(scene.GetArray("GameObjects", i));
			else
				App->go_manager->LoadGameObject(scene.GetArray("GameObjects", i));
		}
		App->go_manager->SetCurrentScenePath(file_name);
		ret = true;
	}
	else
	{
		LOG("The scene %s is not a valid scene file", file_name);
	}

	delete[] buffer;

	return ret;
}

string ModuleResourceManager::FindFile(const string & assets_file_path)
{
	string ret;

	string meta = assets_file_path.substr(0, assets_file_path.length() - 4);
	meta += ".meta";

	char* buffer = nullptr;
	int size = App->file_system->Load(meta.data(), &buffer);
	if (size > 0)
	{
		Data data(buffer);
		ret = data.GetString("library_path");
	}
	else
	{
		LOG("Could not find file %s", assets_file_path.data());
	}
	delete[] buffer;

	return ret;
}

///Given a path returns if the file is one of the valid extensions to import.
FileTypes ModuleResourceManager::GetFileExtension(const char * path) const
{
	char* mesh_extensions[] = { "fbx", "FBX", "obj", "OBJ"};
	char* image_extensions[] = {"png", "PNG", "tga", "TGA"};
	char* scene_extension = "ezx";
	string name = path;
	string extension = name.substr(name.find_last_of(".") + 1);

	for (int i = 0; i < 4; i++)
		if (extension.compare(mesh_extensions[i]) == 0)
			return FileTypes::MESH;

	for (int i = 0; i < 4; i++)
		if (extension.compare(image_extensions[i]) == 0)
			return FileTypes::IMAGE;

	if (extension.compare(scene_extension) == 0)
		return FileTypes::SCENE;
	
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

void ModuleResourceManager::GenerateMetaFile(const char * path, FileTypes type, uint uuid, string library_path, bool is_file)const
{
	Data root;
	root.AppendUInt("Type", static_cast<unsigned int>(type));
	root.AppendUInt("UUID", uuid);
	root.AppendDouble("time_mod", App->file_system->GetLastModificationTime(path));
	root.AppendString("library_path", library_path.data());
	root.AppendString("original_file", path);

	char* buf;
	size_t size = root.Serialize(&buf);

	string final_path = path;
	if(is_file)
		final_path = final_path.substr(0, final_path.length() - 4); //Substract extension. Note: known is: ".png" ".fbx", etc. (4 char)
	final_path += ".meta";

	App->file_system->Save(final_path.data(), buf, size);

	delete[] buf;
}

void ModuleResourceManager::ImportFolder(const char * path, vector<tmp_mesh_file>& list_meshes, string base_dir, string base_library_dir) const
{
	vector<string> files, folders;
	App->file_system->GetFilesAndDirectoriesOutside(path, folders, files);
	string directory_path = path;

	//Create Folder metadata
	uint uuid = App->rnd->RandomInt();
	string folder_assets_path;
	if (base_dir.size() == 0)
		folder_assets_path = App->editor->GetAssetsCurrentDir() + App->file_system->GetNameFromPath(path);
	else
		folder_assets_path = base_dir + App->file_system->GetNameFromPath(path);
	App->file_system->GenerateDirectory(folder_assets_path.data());
	string library_path;
	if (base_library_dir.size() == 0)
		library_path = LIBRARY_FOLDER;
	else
		library_path = base_library_dir;

	//Create Folder at Library
	library_path += std::to_string(uuid);
	App->file_system->GenerateDirectory(library_path.data());

	GenerateMetaFile(folder_assets_path.data(), FOLDER, uuid, library_path, false);

	directory_path += "/"; //Add folder ending manually
	folder_assets_path += "/";
	library_path += "/";

	//Import files
	string file_path;
	for (vector<string>::const_iterator file = files.begin(); file != files.end(); ++file)
	{
		file_path = directory_path + (*file);
		if (GetFileExtension(file->data()) == MESH)
		{
			tmp_mesh_file tmp_file;
			tmp_file.mesh_path = file_path;
			tmp_file.assets_folder = folder_assets_path;
			tmp_file.library_folder = library_path;

			list_meshes.push_back(tmp_file);
		}
		else
		{
			ImportFile(file_path.data(), folder_assets_path, library_path);
		}
	}

	//Import folders
	string folder_path;
	for (vector<string>::const_iterator folder = folders.begin(); folder != folders.end(); ++folder)
	{
		folder_path = directory_path + (*folder);
		ImportFolder(folder_path.data(), list_meshes, folder_assets_path, library_path);
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
		MeshDropped(path, base_dir, base_library_dir);
		break;
	}
}

void ModuleResourceManager::ImageDropped(const char* path, string base_dir, string base_library_dir) const
{
	string file_assets_path = CopyOutsideFileToAssetsCurrentDir(path, base_dir);
	uint uuid = App->rnd->RandomInt();

	string final_image_path;
	if (base_library_dir.size() == 0)
		final_image_path = LIBRARY_FOLDER;
	else
		final_image_path = base_library_dir;

	final_image_path += std::to_string(uuid) + "/";
	App->file_system->GenerateDirectory(final_image_path.data());
	final_image_path += std::to_string(uuid);
	final_image_path += ".dds";

	GenerateMetaFile(file_assets_path.data(), IMAGE, uuid, final_image_path);

	MaterialImporter::Import(final_image_path.data(), file_assets_path.data());
}

void ModuleResourceManager::MeshDropped(const char * path, string base_dir, string base_library_dir) const
{
	//Create a copy and a .meta inside the Assets folder
	string file_assets_path = CopyOutsideFileToAssetsCurrentDir(path, base_dir);
	uint uuid = App->rnd->RandomInt();

	//Create the link to Library
	string final_mesh_path;
	if (base_library_dir.size() == 0)
		final_mesh_path = LIBRARY_FOLDER;
	else
		final_mesh_path = base_library_dir;

	final_mesh_path += std::to_string(uuid) + "/";
	App->file_system->GenerateDirectory(final_mesh_path.data());
	string library_dir = final_mesh_path;
	final_mesh_path += std::to_string(uuid) + ".inf";

	GenerateMetaFile(file_assets_path.data(), FileTypes::MESH, uuid, final_mesh_path);
	MeshImporter::Import(final_mesh_path.data(), file_assets_path.data(), library_dir.data());
}

void ModuleResourceManager::LoadPrefabFile(const string & library_path)
{
	string library_path_extension = library_path + ".inf";
	
	char* buffer = nullptr;
	uint size = App->file_system->Load(library_path_extension.data(), &buffer);
	if (size == 0)
	{
		LOG("Error while loading: %s", library_path_extension.data());
		if (buffer)
			delete[] buffer;
		return;
	}

	Data scene(buffer);
	Data root_objects;
	root_objects = scene.GetArray("GameObjects", 0);

	if (root_objects.IsNull() == false)
	{
		for (int i = 0; i < scene.GetArraySize("GameObjects"); i++)
		{
			App->go_manager->LoadPrefabGameObject(scene.GetArray("GameObjects", i));
		}
	}
	else
	{
		LOG("The %s is not a valid mesh/prefab file", library_path_extension.data());
	}

	delete[] buffer;
}
