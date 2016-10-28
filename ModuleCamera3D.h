#ifndef __MODULECAMERA3D_H__
#define __MODULECAMERA3D_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"

class GameObject;
class ComponentCamera;
class ComponentTransform;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	math::float3 GetPosition() const;
	math::float4x4 GetViewMatrix() const;

private:

	GameObject* go_cam = nullptr;
	ComponentTransform* cam_transform = nullptr;
	ComponentCamera* editor_cam = nullptr;

};

#endif // !__MODULECAMERA3D_H__
