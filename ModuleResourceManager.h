#ifndef __MODULE_RESOURCE_MANAGER_H__
#define __MODULE_RESOURCE_MANAGER_H__

#include "Module.h"
#include <list>
#include "ResourceFile.h"

#define CHECK_MOD_TIME 3

enum FileType
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

struct Directory;

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(const char* name, bool start_enabled = true);
	~ModuleResourceManager();

	bool Init(Data& config);
	update_status Update();
	bool CleanUp();

	void FileDropped(const char* file_path);
	void LoadFile(const string& library_path, const FileType& type);

	ResourceFile* LoadResource(const string& path, ResourceFileType type);
	//Deprecated
	void UnloadResource(const string& path);
	void RemoveResourceFromList(ResourceFile* file);
	ResourceFile* FindResourceByUUID(unsigned int uuid);
	ResourceFile* FindResourceByLibraryPath(const string& library);

	void SaveScene(const char* file_name, string base_library_path);
	bool LoadScene(const char* file_name);
	void SavePrefab(GameObject* gameobject);

	//Returns the path of the file in library
	string FindFile(const string& assets_file_path);

	int GetNumberResources()const;
	int GetNumberTexures()const;
	int GetNumberMeshes()const;
	int GetTotalBytesInMemory()const;
	int GetTextureBytes()const;
	int GetMeshBytes()const;

private:

	FileType GetFileExtension(const char* path)const;
	string CopyOutsideFileToAssetsCurrentDir(const char* path, string base_dir = string())const;

	void GenerateMetaFile(const char* path, FileType type, uint uuid, string library_path, bool is_file = true)const;

	void ImportFolder(const char* path, vector<tmp_mesh_file>& list_meshes, string base_dir = string(), string base_library_dir = string())const;
	void ImportFile(const char* path, string base_dir = string(), string base_library_dir = string(), unsigned int uuid = 0)const;
	void ImageDropped(const char* path, string base_dir = string(), string base_library_dir = string(), unsigned int uuid = 0)const;
	void MeshDropped(const char* path, string base_dir = string(), string base_library_dir = string(), unsigned int uuid = 0)const;

	void LoadPrefabFile(const string& library_path);

	void CheckDirectoryModification(Directory* directory);

private:
	list<ResourceFile*> resource_files;
	float modification_timer = 0.0f;

	unsigned int num_textures = 0;
	unsigned int num_meshes = 0;
	unsigned int bytes_in_memory = 0;
	unsigned int texture_bytes = 0;
	unsigned int mesh_bytes = 0;


};
#endif // !__MODULE_RESOURCE_MANAGER_H__