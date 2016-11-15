#ifndef __MODULE_RESOURCE_MANAGER_H__
#define __MODULE_RESOURCE_MANAGER_H__

#include "Module.h"

enum FileTypes
{
	NONE,
	IMAGE,
	MESH,
	FOLDER
};

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(const char* name, bool start_enabled = true);
	~ModuleResourceManager();

	bool Init(Data& config);
	bool CleanUp();

	void FileDropped(const char* file_path);

private:

	FileTypes GetFileExtension(const char* path)const;
	string CopyOutsideFileToAssetsCurrentDir(const char* path, string base_dir = string())const;

	void GenerateMetaFile(const char* path, FileTypes type, uint uuid,bool is_file = true)const;

	void ImportFolder(const char* path, string base_dir = string(), string base_library_dir = string())const;
	void ImportFile(const char* path, string base_dir = string(), string base_library_dir = string())const;
	void ImageDropped(const char* path, string base_dir = string(), string base_library_dir = string())const;
	void MeshDropped(const char* path, string base_dir = string(), string base_library_dir = string())const;

};
#endif // !__MODULE_RESOURCE_MANAGER_H__
