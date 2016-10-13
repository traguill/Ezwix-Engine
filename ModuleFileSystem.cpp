#include "Application.h"
#include "Globals.h"
#include "ModuleFileSystem.h"
#include "PhysFS/include/physfs.h"
#include "SDL/include/SDL.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

ModuleFileSystem::ModuleFileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	// need to be created before Awake so other modules can use it
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(base_path);
	SDL_free(base_path);

	// By default we include executable's own directory
	AddPath(".");
}

// Destructor
ModuleFileSystem::~ModuleFileSystem()
{
	PHYSFS_deinit();
}

// Called before render is available
bool ModuleFileSystem::Init()
{
	LOG("Loading File System");
	bool ret = true;

	// Add all paths in configuration in order
	//AddPath() manually


	if (PHYSFS_setWriteDir(".") == 0)
	{
		LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());
	}

	//Search Assets and Library folders
	SearchResourceFolders();

	return ret;
}

// Called before quitting
bool ModuleFileSystem::CleanUp()
{
	//LOG("Freeing File System subsystem");
	return true;
}

// Add a new zip file or folder
bool ModuleFileSystem::AddPath(const char* path_or_zip, const char* mount_point)
{
	bool ret = false;

	if (PHYSFS_mount(path_or_zip, mount_point, 1) == 0)
	{
		LOG("File System error while adding a path or zip(%s): %s\n", path_or_zip, PHYSFS_getLastError());
	}	
	else
		ret = true;

	return ret;
}

// Check if a file exists
bool ModuleFileSystem::Exists(const char* file) const
{
	return PHYSFS_exists(file) != 0;
}

// Check if a file is a directory
bool ModuleFileSystem::IsDirectory(const char* file) const
{
	return PHYSFS_isDirectory(file) != 0;
}

// Read a whole file and put it in a new buffer
unsigned int ModuleFileSystem::Load(const char* file, char** buffer) const
{
	unsigned int ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openRead(file);

	if (fs_file != NULL)
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(fs_file);

		if (size > 0)
		{
			*buffer = new char[(uint)size];
			PHYSFS_sint64 readed = PHYSFS_read(fs_file, *buffer, 1, (PHYSFS_sint32)size);
			if (readed != size)
			{
				LOG("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				if (buffer)
					delete[] buffer;
			}
			else
				ret = (uint)readed;
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

// Read a whole file and put it in a new buffer
SDL_RWops* ModuleFileSystem::Load(const char* file) const
{
	char* buffer;
	int size = Load(file, &buffer);

	if (size > 0)
	{
		SDL_RWops* r = SDL_RWFromConstMem(buffer, size);
		if (r != NULL)
			r->close = close_sdl_rwops;

		return r;
	}
	else
		return NULL;
}

int close_sdl_rwops(SDL_RWops *rw)
{
	if (rw->hidden.mem.base)
		delete rw->hidden.mem.base;
	SDL_FreeRW(rw);
	return 0;
}

// Save a whole buffer to disk
unsigned int ModuleFileSystem::Save(const char* file, const void* buffer, unsigned int size) const
{
	unsigned int ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openWrite(file);

	if (fs_file != NULL)
	{
		PHYSFS_sint64 written = PHYSFS_write(fs_file, (const void*)buffer, 1, size);
		if (written != size)
		{
			LOG("File System error while writing to file %s: %s\n", file, PHYSFS_getLastError());
		}
		else
			ret = (uint)written;

		if (PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

bool ModuleFileSystem::SaveUnique(const char * file, const void* buffer, unsigned int size, const char * path, const char * extension,std::string& output_name)
{ //TODO: check if the file already exists
	
	char name_to_save[50];
	sprintf_s(name_to_save, 50, "%s.%s", file, extension);

	vector<string> files_in_path;
	GetEnumerateFiles(path, files_in_path);

	int copies = 0;

	bool name_unique = false;

	while (name_unique == false)
	{
		name_unique = true;

		vector<string>::iterator name_file = files_in_path.begin();
		for (name_file; name_file != files_in_path.end(); name_file++)
		{
			
			if ((*name_file).compare(name_to_save) == 0)
			{
				//add +1 to the file number
				++copies;
				sprintf_s(name_to_save, 50, "%s%d.%s", file, copies, extension);
				name_unique = false;
				break;
			}
		}	
	}

	char name[300];
	sprintf_s(name, 300, "%s%s", path, name_to_save);

	if (Save(name, buffer, size) > 0)
	{
		output_name = name;
		return true;
	}
	else
		return false;
}

bool ModuleFileSystem::GetEnumerateFiles(const char * dir, std::vector<std::string>& buffer)
{
	char** ef =PHYSFS_enumerateFiles(dir);

	for (char** i = ef; *i != NULL; i++)
	{
		buffer.push_back(*i);
	}

	PHYSFS_freeList(ef);

	return (ef != NULL) ? true : false;
}

void ModuleFileSystem::GetFilesAndDirectories(const char * dir, std::vector<string>& folders, std::vector<string>& files)
{
	char** ef = PHYSFS_enumerateFiles(dir);

	string directory(dir);
	for (char**i = ef; *i != NULL; i++)
	{
		if (PHYSFS_isDirectory((directory+(*i)).c_str()))
		{
			folders.push_back(*i);
		}
		else
		{
			files.push_back(*i);
		}
	}

	PHYSFS_freeList(ef);
}

void ModuleFileSystem::SearchResourceFolders()
{
	const char* folders[] = { ASSETS_FOLDER, LIBRARY_FOLDER, LIBRARY_TEXTURES_FOLDER, LIBRARY_MESHES_FOLDER }; //4 folders

	for (int i = 0; i < 4; i++)
		if (PHYSFS_exists(folders[i]) == 0)
			PHYSFS_mkdir(folders[i]);
	
}