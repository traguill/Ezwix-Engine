#include "Application.h"
#include "ModuleSceneIntro.h"

#include "Glew\include\glew.h"
#include <gl\GL.h>

#include "ModuleRenderer3D.h"

#include "Random.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	
	object = App->meshes->Load("cube_testing.fbx");
	

	lenna_id = App->meshes->LoadTexture("lenna.png");

	object.begin()->id_texture = lenna_id;

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
	glColor4f(1, 1, 1, 1);
	vector<Mesh>::iterator item = object.begin();

	while (item != object.end())
	{
		App->renderer3D->Draw((*item));
		item++;
	}

	return UPDATE_CONTINUE;
}

