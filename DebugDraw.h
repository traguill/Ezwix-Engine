#ifndef __DEBUGDRAW_H__
#define __DEBUGDRAW_H__

#include "MathGeoLib\include\MathGeoLib.h"
#include <list>
#include "Module.h"

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

class DebugDraw : public Module
{
public:
	DebugDraw(Application* app, bool start_enabled = true);
	~DebugDraw();

	bool CleanUp();

	bool Start();

	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);

	void AddCross(const float3& point, math::float3 color, float size, float line_width = 1.0f, float duration = 0.0f, bool depth_enabled = true);
	void AddLine(const math::float3& from_position, const math::float3& to_position, math::float3 color, float line_width = 1.0f, float duration = 0.0f, bool depth_enabled = true);
	void AddAABB(const math::AABB& aabb, math::float3 color, float line_width = 1.0f, float duration = 0.0f, bool depth_enabled = true);
	void AddAABB(const math::float3& min_point,const math::float3& max_point, math::float3 color, float line_width = 1.0f, float duration = 0.0f, bool depth_enabled = true);

private:
	void Draw();
	//Removes the primitive from the list and returns the next element
	std::list<DebugPrimitive*>::iterator RemovePrimitive(std::list<DebugPrimitive*>::iterator& it);

	void FillCommonPrimitiveValues(DebugPrimitive* primitive, float3 color, float line_width, float duration, bool depth_enabled, unsigned int vertices_id, unsigned int indices_id, unsigned int num_indices);

	void CreateBaseLine();
	void CreateBaseCube();
	void CreateBaseCross();

public:
	//Some colors to paint the primitives
	math::float3 red = float3(1, 0, 0);
	math::float3 blue = float3(0, 0, 1);
	math::float3 green = float3(0, 1, 0);
	math::float3 white = float3(0, 0, 0);
	math::float3 black = float3(1, 1, 1);
	math::float3 yellow = float3(1, 1, 0);
	math::float3 orange = float3(1, 0.5f, 0);
	math::float3 pink = float3(1, 0, 0.9f);

private:
	std::list<DebugPrimitive*> draw_list;

	//Base primitives ids

	//Cube
	unsigned int id_vertices_cube;
	unsigned int id_indices_cube;
	unsigned int num_indices_cube;
	
	//Line
	unsigned int id_vertices_line;
	unsigned int id_indices_line;
	unsigned int num_indices_line;

	//Cross
	unsigned int id_vertices_cross;
	unsigned int id_indices_cross;
	unsigned int num_indices_cross;
};

extern DebugDraw* g_Debug;

#endif // !__DEBUGDRAW_H__
