#ifndef __COMPONENT_CAMERA_H__
#define __COMPONENT_CAMERA_H__

#include "Component.h"
#include "MathGeoLib\include\MathGeoLib.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera(ComponentType type, GameObject* game_object);
	~ComponentCamera();

	void Update(float dt);

	void OnInspector();
	void OnTransformModified();

	void SetNearPlane(float value);
	void SetFarPlane(float value);
	void SetFOV(float value);

	bool IsVisible(const math::AABB& box)const;

private:
	float near_plane = 0.3f;
	float far_plane = 1000.0f;
	float fov = 60;
	float ratio_x = 5;
	float ratio_y = 4;
	float aspect_ratio = 1.25f;
	math::Frustum frustum;
	math::float3 color; 

};
#endif // !__COMPONENT_MATERIAL_H__
