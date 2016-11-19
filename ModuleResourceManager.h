#ifndef __MODULE_RESOURCE_MANAGER_H__
#define __MODULE_RESOURCE_MANAGER_H__

#include "Module.h"
#include <list>
#include "ResourceFile.h"

enum FileTypes
{
	NONE,
	IMAGE,
	MESH,
	FOLDER,
	PREFAB,
	SCENE
};

struct tmp_mesh_file
{
	string mesh_path;
	string assets_folder;
	string library_folder;
};

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(const char* name, bool start_enabled = true);
	~ModuleResourceManager();

	bool Init(Data& config);
	bool CleanUp();

	void FileDropped(const char* file_path, string base_dir = string(), string base_library_dir = string());
	void LoadFile(const string& library_path, const FileTypes& type);

	ResourceFile* LoadResource(const string& path, ResourceFileType type);
	void UnloadResource(const string& path);
	ResourceFile* FindResourceByUUID(unsigned int uuid);

	//Returns the path of the file in library
	string FindFile(const string& assets_file_path);

private:

	FileTypes GetFileExtension(const char* path)const;
	string CopyOutsideFileToAssetsCurrentDir(const char* path, string base_dir = string())const;

	void GenerateMetaFile(const char* path, FileTypes type, uint uuid, string library_path, bool is_file = true)const;

	void ImportFolder(const char* path, vector<tmp_mesh_file>& list_meshes, string base_dir = string(), string base_library_dir = string())const;
	void ImportFile(const char* path, string base_dir = string(), string base_library_dir = string())const;
	void ImageDropped(const char* path, string base_dir = string(), string base_library_dir = string())const;
	void MeshDropped(const char* path, string base_dir = string(), string base_library_dir = string())const;

	void LoadPrefabFile(const string& library_path);

private:
	list<ResourceFile*> resource_files;

};
#endif // !__MODULE_RESOURCE_MANAGER_H__