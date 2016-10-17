#include "DebugDraw.h"

#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib") 

DebugDraw::DebugDraw()
{
}

DebugDraw::~DebugDraw()
{
	std::list<DebugPrimitive*>::iterator d_primitive = draw_list.begin();

	while (d_primitive != draw_list.end())
	{
		delete (*d_primitive);
		d_primitive++;
	}

	draw_list.clear();
}

void DebugDraw::Init()
{
	//Create the base primitives

	//  6------- 5
	// /       / |
	// 2------1  4
	// |      | /
	// 3----- 0

	GLfloat vertices[] = 
	{
		 0.5f, -0.5f,  0.5f, //0
		 0.5f,  0.5f,  0.5f, //1
	    -0.5f,  0.5f,  0.5f, //2
		-0.5f, -0.5f,  0.5f, //3
		 0.5f, -0.5f, -0.5f, //4
		 0.5f,  0.5f, -0.5f, //5
		-0.5f,  0.5f, -0.5f, //6
		-0.5f, -0.5f, -0.5f, //7
	};

	GLuint indices[] =
	{
		3, 0,
		0, 1,
		1, 2,
		2, 3,
		7, 4, 
		4, 5,
		5, 6,
		6, 7,
		0, 4,
		1, 5, 
		3, 7,
		2, 6
	};

	glGenBuffers(1, (GLuint*)&id_vertices_cube);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 8, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&id_indices_cube);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices_cube);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 24, indices, GL_STATIC_DRAW);

}

void DebugDraw::AddCube(math::float3 center, math::float3 size, math::float3 color, float line_width, float duration, bool depth_enabled)
{
	DebugPrimitive* d_prim = new DebugPrimitive();

	d_prim->vertices_id = id_vertices_cube;
	d_prim->indices_id = id_indices_cube;
	d_prim->num_indices = 24;

	d_prim->color = color;
	d_prim->line_width = line_width;
	d_prim->life = duration;
	d_prim->depth_enabled = depth_enabled;

	d_prim->global_matrix = d_prim->global_matrix.FromTRS(center, math::Quat::identity, size);

	draw_list.push_back(d_prim);
}

void DebugDraw::Draw()
{
	std::list<DebugPrimitive*>::iterator item = draw_list.begin();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (item; item != draw_list.end(); item++)
	{
		glColor3f((*item)->color.x, (*item)->color.y, (*item)->color.z);
		glLineWidth((*item)->line_width);

		glPushMatrix();
		glMultMatrixf(*(*item)->global_matrix.Transposed().v);

		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, (*item)->vertices_id);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*item)->indices_id);
		glDrawElements(GL_LINES, (*item)->num_indices, GL_UNSIGNED_INT, NULL);


		glDisableClientState(GL_VERTEX_ARRAY);

		glPopMatrix();
	}
	glColor3f(0, 0, 0); //Reset color. Necessary?
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
