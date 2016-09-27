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

	/*vector<vec> buff;
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
		buff.push_back(vec(1.f, 0.f, -1.f));*/

	


	vector<vec> vertices;

	vertices.push_back(vec(0,0,0));
	vertices.push_back(vec(1,0,0));
	vertices.push_back(vec(1,1,0));
	vertices.push_back(vec(0,1,0));
	vertices.push_back(vec(1,0,-1));
	vertices.push_back(vec(1,1,-1));
	vertices.push_back(vec(0,0,-1));
	vertices.push_back(vec(0,1,-1));

	glGenBuffers(1, (GLuint*)&(id));
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	vector<uint> indices;

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);

	indices.push_back(1);
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(5);
	indices.push_back(2);
	indices.push_back(1);
	
	indices.push_back(4);
	indices.push_back(6);
	indices.push_back(7);
	indices.push_back(7);
	indices.push_back(5);
	indices.push_back(4);

	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(5);
	indices.push_back(5);
	indices.push_back(7);
	indices.push_back(3);

	indices.push_back(6);
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(3);
	indices.push_back(7);
	indices.push_back(6);

	indices.push_back(1);
	indices.push_back(0);
	indices.push_back(6);
	indices.push_back(6);
	indices.push_back(4);
	indices.push_back(1);

	glGenBuffers(1, (GLuint*) &(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), indices.data(), GL_STATIC_DRAW);

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
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

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


