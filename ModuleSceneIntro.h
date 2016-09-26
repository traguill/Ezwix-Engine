#pragma once
#include "Module.h"
#include "Globals.h"
#include "StackAllocator.h"
#include <list>

#include "MathGeoLib\include\MathGeoLib.h"
#include "MathGeoLib\include\MathBuildConfig.h"

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

private:
	void DrawVertexArray();

private:

	//Testing
	uint id;
	uint id_indices;
	vector<uint> indices;
	vector<vec> vertices;
};
