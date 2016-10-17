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

	void AddAABB(const math::AABB& aabb, math::float3 color, float line_width = 1.0f, float duration = 0.0f, bool depth_enabled = true);
	void AddAABB(const math::float3& min_point,const math::float3& max_point, math::float3 color, float line_width = 1.0f, float duration = 0.0f, bool depth_enabled = true);

private:
	void Draw();

	void CreateBaseCube();

private:
	std::list<DebugPrimitive*> draw_list;

	//Base primitives
	unsigned int id_vertices_cube;
	unsigned int id_indices_cube;
	unsigned int num_indices_cube;
	
};

extern DebugDraw* g_Debug;

#endif // !__DEBUGDRAW_H__
