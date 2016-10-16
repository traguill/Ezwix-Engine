#ifndef __MODULESCENEINTRO_H__
#define __MODULESCENEINTRO_H__

#include "Module.h"
#include "Globals.h"
#include <list>

#include "MathGeoLib\include\MathGeoLib.h"
#include "MathGeoLib\include\MathBuildConfig.h"

class Mesh;
class GameObject;
class ComponentTransform;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

};
#endif // !__MODULESCENEINTRO_H__
