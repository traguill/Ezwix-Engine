
#ifndef __ASSETS_H__
#define __ASSETS_H__

#include "Window.h"
#include <vector>

struct Directory
{
	std::string name;
	std::string path; //Path from Assets to the current directory
	std::vector<Directory> directories;
	std::vector<string> files;
};

class Assets : public Window
{
public:
	Assets();
	~Assets();

	void Draw();

private:

	void Init();
	void FillDirectoriesRecursive(Directory& root_dir);

	bool IsMeshExtension(std::string file_name);

private:
	Directory root; //Assets directory

	Directory* current_dir = &root;
	std::string file_selected;
	//Icons
	uint folder_id;
	uint file_id;

};

#endif
