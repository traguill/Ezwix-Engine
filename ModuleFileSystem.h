#ifndef __ModuleFileSystem_H__
#define __ModuleFileSystem_H__

#include "Module.h"

struct SDL_RWops;

int close_sdl_rwops(SDL_RWops *rw);

class ModuleFileSystem : public Module
{
public:

	ModuleFileSystem(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleFileSystem();

	bool Init();


	// Called before quitting
	bool CleanUp();

	// Utility functions
	bool AddPath(const char* path_or_zip, const char* mount_point = nullptr);
	bool Exists(const char* file) const;
	bool IsDirectory(const char* file) const;
	const char* GetSaveDirectory() const
	{
		return "save/";
	}

	// Open for Read/Write
	unsigned int Load(const char* file, char** buffer) const;
	SDL_RWops* Load(const char* file) const;

	unsigned int Save(const char* file, const void* buffer, unsigned int size) const;
	bool SaveUnique(const char* file, const void* buffer, unsigned int size, const char* path, const char* extension,std::string& output_name);

	bool GetEnumerateFiles(const char* dir, std::vector<std::string>& buffer);
	void GetFilesAndDirectories(const char* dir, std::vector<string>& folders, std::vector<string>& files);

private:

	void SearchResourceFolders();

};

#endif // __ModuleFileSystem_H__