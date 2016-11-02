#include "Application.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "Glew\include\glew.h"
#include <gl\GL.h>
#include "Component.h"
#include "ModuleRenderer3D.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "Random.h"

//Tesing
#include "MaterialImporter.h"
#include "Quadtree.h"

ModuleSceneIntro::ModuleSceneIntro(const char* name, bool start_enabled) : Module(name, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	
	Quadtree<float> t;
	t.Create(math::AABB());

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	

	return UPDATE_CONTINUE;
}

