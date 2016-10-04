#pragma once
#include "Module.h"
#include "Globals.h"
#include "StackAllocator.h"
#include <list>

#include "MathGeoLib\include\MathGeoLib.h"
#include "MathGeoLib\include\MathBuildConfig.h"

class Mesh;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();


private:

	//Testing
	vector<Mesh> object;
public:
	uint lenna_id = 0;
};
