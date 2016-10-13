
#ifndef __ASSETS_H__
#define __ASSETS_H__

#include "Window.h"
#include <vector>

struct Directory
{
	std::string name;
	std::string path; //Path from Assets to the current directory
	std::vector<Directory*> directories;
	std::vector<string> files;
	Directory* parent = nullptr;
};

class Assets : public Window
{
public:
	Assets();
	~Assets();

	void Draw();

private:

	void Init();
	void CleanUp();
	void FillDirectoriesRecursive(Directory* root_dir);
	void DeleteDirectoriesRecursive(Directory* root_dir, bool keep_root = false);

	bool IsMeshExtension(std::string file_name);

	void Refresh();
	void OpenInExplorer(std::string* file = NULL)const;

private:
	Directory* root = nullptr; //Assets directory

	Directory* current_dir = root;
	std::string file_selected;
	//Icons
	uint folder_id;
	uint file_id;

};

#endif
