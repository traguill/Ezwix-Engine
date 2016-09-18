#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include <list>

struct GameObject
{
	Primitive render;
	PhysBody3D* body;
};

class Editor : public Module
{
public:
	Editor(Application* app, bool start_enabled = true);
	~Editor();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

private:

	void Render(); //Render all the stuff

	//Editor Windows
	update_status EditorWindows();

	//Testing
	void CreateRndPoints(float2 min_max, int quantity);
	void CreateBoundingCube();
	void ClearObjects();
	void RotateCube();
	void CreateBoundingSphere();

private:
	//Testing
	float2 range;
	int quantity;
	float3 rot_deg;
	float4 rot_quat;

	list<Sphere_P> points;
	Cube_P cube;
	bool render_cube = false;

	Sphere_P sphere;
	bool render_sphere = false;

	//Test2
	Sphere_P sphereT2A;
	Sphere_P sphereT2B;
	bool render_ST2A = false;
	bool render_ST2B = false;

	float3 ST2A_pos;
	float3 ST2B_pos;

	float ST2A_radius = 0;
	float ST2B_radius = 0;

	//Test3
	Plane_P plane3;
	bool render_plane3 = false;
	vec normal_plane;
	Sphere_P point3;
	bool render_point3 = false;
	vec point3_pos = point3_pos.zero;

	string point_plane = "";

};