#ifndef __DEBUGDRAW_H__
#define __DEBUGDRAW_H__

#include "MathGeoLib\include\MathGeoLib.h"
#include <list>


struct DebugPrimitive
{
	math::float3 color;
	float line_width;
	bool depth_enabled;
	float life = 0; //Life = 0 means the Primitive only lasts one frame
	math::float4x4 global_matrix = math::float4x4::identity;
	unsigned int vertices_id;
	unsigned int indices_id;
	unsigned int num_indices;
};

class DebugDraw
{
public:
	DebugDraw();
	~DebugDraw();

	void Init();

	void Draw();

	void AddCube(math::float3 center, math::float3 size, math::float3 color, float line_width = 1.0f, float duration = 0.0f, bool depth_enabled = true);

private:
	std::list<DebugPrimitive*> draw_list;

	//Base primitives
	unsigned int id_vertices_cube;
	unsigned int id_indices_cube;
	unsigned int num_indices_cube;
	
};


#endif // !__DEBUGDRAW_H__
