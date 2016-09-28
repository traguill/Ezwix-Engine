#ifndef  __MODULEMESHES_H__
#define __MODULEMESHES_H__

#include "Module.h"
#include <cstdint>

struct Mesh
{
	//Vertices
	uint id_vertices = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

	//Indices
	uint id_indices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;
};

class ModuleMeshes : public Module
{
public:
	ModuleMeshes(Application* app, bool start_enabled = true);
	~ModuleMeshes();

	bool Init();
	bool CleanUp();

	vector<Mesh> Load(const char* path);
};

#endif // __MODULEMESHES_H__
