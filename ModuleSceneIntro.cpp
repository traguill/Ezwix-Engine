#include "Application.h"
#include "ModuleSceneIntro.h"

#include "Glew\include\glew.h"
#include <gl\GL.h>




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

	vector<vec> buff;
		buff.push_back(vec(0.f, 0.f, 0.f));
		buff.push_back(vec(1.f, 0.f, 0.f));
		buff.push_back(vec(0.f, 1.f, 0.f));
		buff.push_back(vec(1.f, 0.f, 0.f));
		buff.push_back(vec(1.f, 1.f, 0.f));
		buff.push_back(vec(0.f, 1.f, 0.f));
		buff.push_back(vec(1.f, 0.f, 0.f));
		buff.push_back(vec(1.f, 0.f, -1.f));
		buff.push_back(vec(1.f, 1.f, 0.f) );
		buff.push_back(vec(1.f, 0.f, -1.f ));
		buff.push_back(vec(1.f, 1.f, -1.f ));
		buff.push_back(vec(1.f, 1.f, 0.f) );
		buff.push_back(vec(0.f, 1.f, 0.f) );
		buff.push_back(vec(1.f, 1.f, 0.f) );
		buff.push_back(vec(0.f, 1.f, -1.f) );
		buff.push_back(vec(1.f, 1.f, 0.f) );
		buff.push_back(vec(1.f, 1.f, -1.f) );
		buff.push_back(vec(0.f, 1.f, -1.f ));
		buff.push_back(vec(0.f, 0.f, -1.f ));
		buff.push_back(vec(0.f, 0.f, 0.f) );
		buff.push_back(vec(0.f, 1.f, -1.f) );
		buff.push_back(vec(0.f, 0.f, 0.f) );
		buff.push_back(vec(0.f, 1.f, 0.f) );
		buff.push_back(vec(0.f, 1.f, -1.f) );
		buff.push_back(vec(0.f, 0.f, -1.f) );
		buff.push_back(vec(0.f, 1.f, -1.f) );
		buff.push_back(vec(1.f, 1.f, -1.f) );
		buff.push_back(vec(1.f, 0.f, -1.f) );
		buff.push_back(vec(0.f, 0.f, -1.f) );
		buff.push_back(vec(1.f, 1.f, -1.f) );
		buff.push_back(vec(1.f, 0.f, 0.f) );
		buff.push_back(vec(0.f, 0.f, 0.f) );
		buff.push_back(vec(1.f, 0.f, -1.f ));
		buff.push_back(vec(0.f, 0.f, 0.f) );
		buff.push_back(vec(0.f, 0.f, -1.f) );
		buff.push_back(vec(1.f, 0.f, -1.f));

	glGenBuffers(1, (GLuint*)&(id));
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * buff.size(), buff.data(), GL_STATIC_DRAW);




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

void ModuleSceneIntro::DrawVertexArray()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawArrays(GL_TRIANGLES, 0, 36 * 3);
	glDisableClientState(GL_VERTEX_ARRAY);
}


