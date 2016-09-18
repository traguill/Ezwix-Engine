#include "Application.h"
#include "Editor.h"
#include "Imgui\imgui.h"
#include "Random.h"

Editor::Editor(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

Editor::~Editor()
{}

bool Editor::Start()
{
	bool ret = true;

	LOG("Start Editor");

	//Set Camera Position
	App->camera->Move(vec(10.0f, 6.0f, 30.0f));
	App->camera->LookAt(vec(0, 0, 0));

	range.x = range.y = 0;
	quantity = 0;
	rot_deg.x = rot_deg.y = rot_deg.z = 0;
	rot_quat.x = rot_quat.y = rot_quat.z = rot_quat.z = 1;

	//TEST2
	ST2A_pos = ST2A_pos.zero;
	ST2B_pos = ST2B_pos.zero;

	//Test3
	normal_plane = normal_plane.zero;

	return ret;
}

bool Editor::CleanUp()
{
	LOG("Clean Up Editor");

	return true;
}

update_status Editor::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;
	

	ret = EditorWindows(); //Update the windows of the editor

	if (remove_points)
	{
		RayCastTest();
	}

	//Render
	Render();

	//Draw Grid
	Plane_P grid(0,1,0,0);
	grid.axis = true;
	grid.Render();
	
	return ret;	
}

update_status Editor::EditorWindows()
{
	update_status ret = UPDATE_CONTINUE;

	//Main MenuBar --------------------------------------------------------------------------------------------
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Quit", NULL))
		{
			ret = UPDATE_STOP;
		}
		ImGui::EndMainMenuBar();
	}

	//Testing Window -----------------------------------------------------------------------------------------
	ImGuiWindowFlags window_flags = 0;
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiSetCond_FirstUseEver);
	bool open = false;
	if (!ImGui::Begin("Test Window", &open, window_flags))
	{
		ImGui::End();
		return ret;
	}
	if (ImGui::Button("Clear"))
	{
		ClearObjects();
	}
	if (ImGui::CollapsingHeader("Test1"))
	{
		ImGui::InputFloat2("Min/Max", range.ptr());
		ImGui::InputInt("Number: ", &quantity);
		if (ImGui::Button("Create Random Points"))
		{
			CreateRndPoints(range, quantity);
		}
		if (ImGui::Button("Create cube"))
		{
			CreateBoundingCube();
		}
		ImGui::InputFloat3("Degrees: ", rot_deg.ptr());
		ImGui::InputFloat4("Quaternion: ", rot_quat.ptr());
		if (ImGui::Button("Rotate"))
		{
			RotateCube();
		}
		if (ImGui::Button("Calculate Sphere"))
		{
			CreateBoundingSphere();
		}
	}

	if (ImGui::CollapsingHeader("Test2"))
	{
		ImGui::InputFloat3("Position A", ST2A_pos.ptr());
		ImGui::InputFloat("RadiusB", &ST2A_radius);
		if (ImGui::Button("Create Sphere A"))
		{
			sphereT2A.SetPos(ST2A_pos.x, ST2A_pos.y, ST2A_pos.z);
			sphereT2A.wire = true;
			sphereT2A.radius = ST2A_radius;

			render_ST2A = true;
		}

		ImGui::InputFloat3("Position B", ST2B_pos.ptr());
		ImGui::InputFloat("RadiusB", &ST2B_radius);
		if (ImGui::Button("Create Sphere B"))
		{
			sphereT2B.SetPos(ST2B_pos.x, ST2B_pos.y, ST2B_pos.z);
			sphereT2B.wire = true;
			sphereT2B.radius = ST2B_radius;

			render_ST2B = true;
		}
	}

	if (ImGui::CollapsingHeader("Test3"))
	{
		ImGui::InputFloat3("Normal Plane", normal_plane.ptr());
		if (ImGui::Button("Create Plane"))
		{
			plane3.normal = normal_plane.Normalized();
			Quat rot = rot.RotateFromTo(vec(0, 1, 0), normal_plane.Normalized());
			plane3.transform = rot * plane3.transform;
			plane3.color = Blue;
			plane3.axis = true;

			render_plane3 = true;
		}
		ImGui::InputFloat3("Point position", point3_pos.ptr());
		if (ImGui::Button("Create point"))
		{
			point3.SetPos(point3_pos.x, point3_pos.y, point3_pos.z);
			point3.color = Red;
			point3.radius = 0.1f;

			render_point3 = true;

			Plane p;
			p.normal = normal_plane.Normalized();
			p.d = 0.0f;

			if (p.Contains(point3_pos))
			{
				point_plane = "The plane contains this point";
			}
			else
			{
				float value = p.SignedDistance(point3_pos);
				if (value > 0)
				{
					point_plane = "The point is avobe the plane";
				}
				else
				{
					point_plane = "The point is under the plane";
				}
			}
		}
		ImGui::Text(point_plane.c_str());
	}

	if (ImGui::CollapsingHeader("Test4"))
	{
		ImGui::Checkbox("RayCast to remove points", &remove_points);
	}

	ImGui::End();

	ImGui::ShowTestWindow();
	return ret;
}


void Editor::CreateRndPoints(float2 min_max, int quantity)
{
	if (min_max.x >= min_max.y || quantity <= 0)
		return;

	Random rnd;
	rnd.SetSeed(SDL_GetTicks());
	for (int i = 0; i < quantity; i++)
	{
		
		vec pos;
		pos.x = rnd.RandomInt(min_max.x, min_max.y);
		pos.y = rnd.RandomInt(min_max.x, min_max.y);
		pos.z = rnd.RandomInt(min_max.x, min_max.y);

		Sphere_P s(0.1f);
		s.color = Red;
		s.SetPos(pos.x, pos.y, pos.z);
		points.push_back(s);
	}
}

void Editor::CreateBoundingCube()
{
	if (points.size() > 0)
	{
		LOG("Create cube");
		float minX, maxX, minY, maxY, minZ, maxZ;
		list<Sphere_P>::iterator item = points.begin();

		minX = maxX = (*item).GetPos().x;
		minY = maxY = (*item).GetPos().y;
		minZ = maxZ = (*item).GetPos().z;

		while (item != points.end())
		{
			vec pos = (*item).GetPos();

			if (pos.x < minX) minX = pos.x;
			if (pos.x > maxX) maxX = pos.x;
			if (pos.y < minY) minY = pos.y;
			if (pos.y > maxY) maxY = pos.y;
			if (pos.z < minZ) minZ = pos.z;
			if (pos.z > maxZ) maxZ = pos.z;

			++item;
		}

		vec position;

		position.x = (maxX - minX) / 2.0f + minX;
		position.y = (maxY - minY) / 2.0f + minY;
		position.z = (maxZ - minZ) / 2.0f + minZ;

		cube.SetPos(position.x, position.y, position.z);

		vec size;

		size.x = maxX - minX;
		size.y = maxY - minY;
		size.z = maxZ - minZ;

		cube.size = size;

		cube.axis = true;
		cube.wire = true;
		render_cube = true;

	}
}

void Editor::Render()
{
	list<Sphere_P>::iterator point = points.begin();
	while (point != points.end())
	{
		(*point).Render();
		++point;
	}

	if (render_cube)
	{
		cube.Render();
	}

	if (render_sphere)
	{
		sphere.Render();
	}

	if (render_ST2A)
	{
		sphereT2A.Render();
	}
	if (render_ST2B)
	{
		sphereT2B.Render();
	}

	if (render_plane3)
	{
		plane3.Render();
	}

	if (render_point3)
	{
		point3.Render();
	}

	line.Render();
}

void Editor::ClearObjects()
{
	points.clear();
	cube.transform = cube.transform.identity;
	render_cube = false;
	sphere.transform = sphere.transform.identity;
	render_sphere = false;

	render_ST2A = false;
	render_ST2B = false;

	render_plane3 = false;
	render_point3 = false;
}

void Editor::RotateCube()
{
	if (!render_cube)
		return;

	Quat quaternion;

	quaternion = quaternion.FromEulerXYZ(rot_deg.x, rot_deg.y, rot_deg.z);

	rot_quat.x = quaternion.x;
	rot_quat.y = quaternion.y;
	rot_quat.z = quaternion.z;
	rot_quat.w = quaternion.w;

	cube.transform = quaternion * cube.transform;
}

void Editor::CreateBoundingSphere()
{
	if (!render_cube)
		return;

	OBB obb;
	obb.pos = cube.GetPos();
	obb.r = vec(cube.size.x / 2, cube.size.y / 2, cube.size.z / 2);
	float3x3 rotation = cube.transform.RotatePart();

	vec axisX = rotation * vec(1, 0, 0);
	axisX.Normalize();

	vec axisY = rotation * vec(0, 1, 0);
	axisY.Normalize();

	vec axisZ = rotation * vec(0, 0, 1);
	axisZ.Normalize();

	obb.axis[0] = axisX;	
	obb.axis[1] = axisY;	
	obb.axis[2] = axisZ;

	Sphere s = obb.MinimalEnclosingSphere();
	
	sphere.SetPos(s.pos.x, s.pos.y, s.pos.z);
	sphere.radius = s.r;

	sphere.wire = true;

	render_sphere = true;

}

void Editor::RayCastTest()
{
	if(App->input->GetMouseButton(1) == KEY_DOWN)
	{
		LOG("Click");
		vec ray = App->camera->Raycast();
		Ray raycast;
		raycast.pos = App->camera->Position;
		raycast.dir = ray.Normalized();

		line.origin = App->camera->Position;
		line.destination = line.origin + (30 * raycast.dir);
		
		list<Sphere_P>::iterator item = points.begin();

		float min_distance = 36956453123;
		float distance;
		list<Sphere_P>::iterator point_remove = points.end();
		while (item != points.end())
		{
			Sphere s;
			s.pos = (*item).GetPos();
			s.r = (*item).radius;

			bool hit = raycast.Intersects(s, NULL, NULL, &distance);

			if (hit)
			{
				if (distance < min_distance)
				{
					min_distance = distance;
					point_remove = item;
				}
			}

			++item;
		}

		if (point_remove != points.end())
		{
			points.erase(point_remove);
			LOG("Point removed");
		}

	}
}