
#ifndef __ASSETS_H__
#define __ASSETS_H__

#include "Window.h"
#include "ModuleResourceManager.h"
#include <vector>

struct AssetFile
{
	FileTypes type;
	std::string name; //To display
	std::string file_path;
	std::string content_path;
	int time_mod;
};

struct Directory
{
	std::string name; //To display
	std::string path; //Path from Assets to the current directory
	std::vector<Directory*> directories;
	std::vector<AssetFile*> files;
	std::string library_path;
	Directory* parent = nullptr;
};

class Assets : public Window
{
public:
	Assets();
	~Assets();

	void Draw();
	string CurrentDirectory()const;

private:

	void Init();
	void CleanUp();
	void FillDirectoriesRecursive(Directory* root_dir);
	void DeleteDirectoriesRecursive(Directory* root_dir, bool keep_root = false);

	bool IsMeshExtension(const std::string& file_name)const;
	bool IsSceneExtension(const std::string& file_name)const;

	void Refresh();
	void OpenInExplorer(const std::string* file = NULL)const;

	void MeshFileOptions()const;
	void SceneFileOptions()const;

private:
	Directory* root = nullptr; //Assets directory

	Directory* current_dir = root;
	AssetFile* file_selected = nullptr;
	//Icons
	uint folder_id;
	uint file_id;

};

#endif
