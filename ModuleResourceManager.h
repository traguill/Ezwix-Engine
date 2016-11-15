#ifndef __MODULE_RESOURCE_MANAGER_H__
#define __MODULE_RESOURCE_MANAGER_H__

#include "Module.h"

enum FileTypes
{
	NONE,
	IMAGE,
	MESH
};

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(const char* name, bool start_enabled = true);
	~ModuleResourceManager();

	void FileDropped(const char* file_path);

private:

	FileTypes GetFileExtension(const char* path);
	string CopyOutsideFileToAssetsCurrentDir(const char* path)const;

	void GenerateMetaFile(const char* path, FileTypes type, uint uuid)const;

	void ImageDropped(const char* path)const;


};
#endif // !__MODULE_RESOURCE_MANAGER_H__
